#pragma once

namespace mdk
{

template<typename ScalarType>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::KNNBasisSelectionBasedShapeDictionaryBuilder()
{
    this->Clear();
}

template<typename ScalarType>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::~KNNBasisSelectionBasedShapeDictionaryBuilder()
{
}

template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
	m_TrainingShapeData = nullptr;
	m_LandmarkOnShape.Clear();
    m_InitialDictionary = nullptr;
	m_SelectedBasisIndexList.Clear();
	m_SelectedBasisExperience.Clear();
}


template<typename ScalarType>
bool KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::CheckInput()
{
	if (m_TrainingShapeData == nullptr)
	{
		MDK_Error("m_TrainingShapeData is nullptr @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_TrainingShapeData->IsEmpty() == true)
	{
		MDK_Error("InputShapeData is empty @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	int_max Dimension = (*m_TrainingShapeData)[0].GetRowCount();
	if (Dimension != 2 && Dimension != 3)
	{
		MDK_Error("Shape Dimension is NOT 2 or 3 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary == nullptr)
	{
		MDK_Error("InitialDictionary is nullptr @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary->GetBasisCount() == 0)
	{
		MDK_Error("InitialDictionary is empty @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.BasisCount > m_InitialDictionary->GetBasisCount())
	{
		MDK_Error("BasisCount > BasisCount of InitialDictionary is empty @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxNeighbourCount <= 0)
	{
		MDK_Error("MaxNeighbourCount <= 0 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MiniBatchSize < 0 || m_Parameter.MiniBatchSize > m_TrainingShapeData->GetLength())
	{
		MDK_Error("MiniBatchSize is out of range @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold < 0 or > 1 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.TransformName != "IdentityTransform" && m_Parameter.TransformName != "RigidTransform" 
		&& m_Parameter.TransformName != "SimilarityTransform" && m_Parameter.TransformName != "ThinPlateSplineTransform")
	{
		MDK_Error("TransformName is unknown @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (m_LandmarkOnShape.GetLength() < 9)
		{
			MDK_Error("too few Landmark for ThinPlateSplineTransform @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
			return false;
		}
	}

	if (m_Parameter.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	return true;
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

    DenseMatrix<ScalarType> BasisExperience_init;
	BasisExperience_init.Copy(m_InitialDictionary->BasisExperience());
	BasisExperience_init *= m_Parameter.ExperienceDiscountFactor;

	DenseMatrix<ScalarType> BasisExperience;
	BasisExperience.Copy(m_InitialDictionary->BasisExperience());

	int_max TotalDataCount = m_TrainingShapeData->GetLength();

    //------------------------------------------ run Epoch and Mini-batch -------------------------------------------------------//
        
    // random number for sampling
    std::random_device rd;
    std::mt19937 gen(rd());
	DenseVector<int_max> RandomDataIndexList = span(0, TotalDataCount - 1);

    for (int_max iter = 0; iter < m_Parameter.MaxEpochCount; ++iter)
    {
		this->AdjustBasisExperience_BeforeEachEpoch(BasisExperience);

		int_max BatchCount = TotalDataCount / m_Parameter.MiniBatchSize;
		if (BatchCount > 1)
		{
			std::shuffle(RandomDataIndexList.begin(), RandomDataIndexList.end(), gen);
		}
		for (int_max n = 0; n < BatchCount; ++n)
		{
			ObjectArray<DenseMatrix<ScalarType>> ShapeData_CurrentBatch;
			{// sample a subset from m_TrainingShapeData
				int_max Idx_start = n*m_Parameter.MiniBatchSize;
				int_max Idx_end = std::min(Idx_start + m_Parameter.MiniBatchSize - 1, TotalDataCount - 1);
				auto SubSet = RandomDataIndexList.GetSubSet(Idx_start, Idx_end);
				ShapeData_CurrentBatch.Resize(SubSet.GetLength());
				for (int_max k = 0; k < SubSet.GetLength(); ++k)
				{
					ShapeData_CurrentBatch[k].ForceShare((*m_TrainingShapeData)[SubSet[k]]);
				}
			}
			//encode data ------------------------------------------------
			KNNSoftAssignBasedSparseShapeEncoder<ScalarType> Encoder;
			Encoder.Parameter().MaxNeighbourCount = m_Parameter.MaxNeighbourCount;
			Encoder.Parameter().TransformName = m_Parameter.TransformName;
			Encoder.Parameter().SimilarityThreshold = m_Parameter.SimilarityThreshold;
			Encoder.Parameter().MaxThreadCount = m_Parameter.MaxThreadCount;
			Encoder.SetInputShapeData(m_TrainingShapeData);
			Encoder.SetLandmarkOnShape(m_LandmarkOnShape);
			Encoder.SetInputDictionary(&m_InitialDictionary->Basis());
			Encoder.Update();
			const auto& CodeTable = Encoder.OutputMembershipCode();
			//update basis -----------------------------------------------
			this->UpdateBasisExperience(BasisExperience, CodeTable);
        }
		this->AdjustBasisExperience_AfterEachEpoch(BasisExperience, BasisExperience_init, TotalDataCount);
    }

    // select basis ------------------------------------------------------------------------------------------
	m_SelectedBasisExperience = BasisExperience;
	auto IndexList_sort = m_SelectedBasisExperience.Sort("descend");
	m_SelectedBasisIndexList = IndexList_sort.GetSubSet(0, m_Parameter.BasisCount-1);
	
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience)
{
    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // Experience must >= 1
    for (int_max k = 0; k < BasisExperience.GetElementCount(); k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }    
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{  
	for (int_max n = 0; n < CodeTable.GetLength(); ++n)
	{
		const auto& KNNBasisIndexList = CodeTable[n].IndexList();
		const auto& KNNMembershipList = CodeTable[n].ElementList();
		for (int_max m = 0; m < KNNBasisIndexList.GetLength(); ++m)
		{
			auto BasisIndex = KNNBasisIndexList[m];
			BasisExperience[BasisIndex] += KNNMembershipList.Sum();
		}
	}
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount)
{
    int_max BasisCount = BasisExperience.GetElementCount();

    DenseMatrix<ScalarType> BasisExperience_Diff = MatrixSubtract(BasisExperience, BasisExperience_init);

    ScalarType TotalGain = BasisExperience_Diff.Sum();

    if (TotalGain > TotalDataCount)
    {
        BasisExperience_Diff *= ScalarType(TotalDataCount) / TotalGain;

        MatrixAdd(BasisExperience, BasisExperience_init, BasisExperience_Diff);
    }
}

}// namespace mdk

