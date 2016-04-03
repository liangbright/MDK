#pragma once

namespace mdk
{

template<typename ScalarType>
KNNAverageBasedShapeDictionaryBuilder<ScalarType>::KNNAverageBasedShapeDictionaryBuilder()
{
    this->Clear();
}

template<typename ScalarType>
KNNAverageBasedShapeDictionaryBuilder<ScalarType>::~KNNAverageBasedShapeDictionaryBuilder()
{
}

template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
	m_TrainingShapeData = nullptr;
    m_InitialDictionary = nullptr;
	m_Dictionary.Clear();
}


template<typename ScalarType>
bool KNNAverageBasedShapeDictionaryBuilder<ScalarType>::CheckInput()
{
	if (m_TrainingShapeData == nullptr)
	{
		MDK_Error("m_TrainingShapeData is nullptr @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_TrainingShapeData->IsEmpty() == true)
	{
		MDK_Error("InputShapeData is empty @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary == nullptr)
	{
		MDK_Error("InitialDictionary is nullptr @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary->IsEmpty() == false)
	{
		MDK_Error("InitialDictionary is empty @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.NeighbourCount <= 0)
	{
		MDK_Error("NeighbourCount <= 0 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MiniBatchSize < 0 || m_Parameter.MiniBatchSize > m_TrainingShapeData->GetLength())
	{
		MDK_Error("MiniBatchSize is out of range @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.WeightOnProbabiliyForBasisSelection < 0 || m_Parameter.WeightOnProbabiliyForBasisSelection > 1)
	{
		MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold < 0 or > 1 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	return true;
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

    auto OutputDictionary = this->PreprocessInitialDictionary(*m_InitialDictionary);

    DenseMatrix<ScalarType> BasisExperience_init;
    BasisExperience_init.Copy(OutputDictionary.BasisExperience());

    int_max TotalDataCount = m_TrainingShapeData->GetColCount();

    //------------------------------------------ run Epoch and Mini-batch -------------------------------------------------------//
        
    // random number for sampling
    std::random_device rd;
    std::mt19937 gen(rd());
	DenseVector<int_max> RandomDataIndexList = span(0, TotalDataCount - 1);

    for (int_max iter = 0; iter < m_Parameter.MaxEpochCount; ++iter)
    {
		std::shuffle(RandomDataIndexList.begin(), RandomDataIndexList.end(), gen);
		int_max BatchCount = TotalDataCount / m_Parameter.MiniBatchSize;				
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
			//encode ---------------------------------------------------
			KNNSoftAssignBasedSparseShapeEncoder<ScalarType> Encoder;
			Encoder.Parameter().NeighbourCount = m_Parameter.NeighbourCount;
			Encoder.Parameter().TransformName = m_Parameter.TransformName;
			Encoder.Parameter().SimilarityThreshold = m_Parameter.SimilarityThreshold;
			Encoder.Parameter().MaxThreadCount = m_Parameter.MaxThreadCount;
			Encoder.SetInputShapeData(m_TrainingShapeData);
			Encoder.SetInputDictionary(&OutputDictionary.Basis());
			Encoder.Update();
			const auto& CodeTable = Encoder.OutputCode();
			//--------------------------------------------------------------
			this->UpdateBasisAndBasisExperience(OutputDictionary.Basis(), OutputDictionary.BasisExperience(), ShapeData_CurrentBatch, CodeTable);            
        }            
		this->AdjustBasisExperience(OutputDictionary.BasisExperience(), BasisExperience_init, TotalDataCount);       
    }

    this->UpdateDictionaryInformation_AfterALLEpoch(OutputDictionary, BasisExperience_init, TotalDataCount);

    m_Dictionary.Copy(std::move(OutputDictionary));
}


template<typename ScalarType>
ShapeDictioanry<ScalarType>
KNNAverageBasedShapeDictionaryBuilder<ScalarType>::PreprocessInitialDictionary(const ShapeDictioanry<ScalarType>& InitialDictionary)
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;
    OutputDictionary.Copy(InitialDictionary);
    auto& BasisExperience = OutputDictionary.BasisExperience();
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
    return OutputDictionary;
}




template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisAndBasisExperience(ObjectArray<DenseMatrix<ScalarType>>& Basis,
                              DenseMatrix<ScalarType>& BasisExperience,
                              const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
                              const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{  
	auto BasisCount = Basis.GetLength();
	auto DataCount = ShapeData.GetLength();
	ObjectArray<DenseVector<ScalarType>> DataIndexTable;	
	//DataIndexTable[k]: index of shape as KNN to Basis[k]
	ObjectArray<DenseVector<ScalarType>> MembershipTable;
	//MembershipTable[k]: membership of shape as KNN to Basis[k]

	DataIndexTable.Resize(BasisCount);
	MembershipTable.Resize(BasisCount);
	for (int_max k = 0; k < BasisCount; ++k)
	{
		DataIndexTable[k].SetCapacity(DataCount/10);
		MembershipTable[k].SetCapacity(DataCount / 10);
	}
	for (int_max n = 0; n < DataCount; ++n)
	{
		const auto& KNNBasisIndexList = CodeTable[n].IndexList();
		const auto& KNNMembershipList = CodeTable[n].ElementList();
		for (int_max m = 0; m < KNNBasisIndexList.GetLength(); ++m)
		{
			DataIndexTable[KNNBasisIndexList[m]].Append(n);
			MembershipTable[KNNBasisIndexList[m]].Append(KNNMembershipList[m]);
		}
	}

	//for (int_max k = 0; k <= BasisCount-1; ++k)
	auto TempFunction = [&](int_max)
	{
		DenseMatrix<ScalarType> BasisChange;
		BasisChange.Resize(Basis[0].GetSize());
		ScalarType ExperienceChange = 0;
		const auto& DataIndexList = DataIndexTable[k];		
		const auto& MembershipList = MembershipTable[k];
		for (int_max n = 0; n < DataIndexList.GetLength(); ++n)
		{
			auto DataIndex = DataIndexList[n];
			auto Membership= MembershipList[n];
			auto Shape_new = this->TransformShape(ShapeData[DataIndex], Basis[k]);
			Shape_new -= Basis[k];
			Shape_new *= Membership;
			BasisChange += Shape_new;
			ExperienceChange += Membership;
		}
		BasisExperience[k] += ExperienceChange;
		BasisChange /= BasisExperience[k];
		Basis[k] += BasisChange;
	};
	ParallelForLoop(TempFunction, 0, BasisCount - 1, m_Parameter.MaxThreadCount);
}


template<typename ScalarType>
DenseMatrix<ScalarType> KNNAverageBasedShapeDictionaryBuilder<ScalarType>::TransformShape(const <DenseMatrix<ScalarType>& Shape, const DenseMatrix<ScalarType>& Basis)
{
	if (m_Parameter.TransformName == "RigidTransform")
	{		
		if (m_Parameter.Landmark.IsEmpty() == true)
		{
			RigidTransform3D<ScalarType> Transform;
			Transform.SetSourceLandmarkPointSet(Shape);
			Transform.SetTargetLandmarkPointSet(Basis);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
		else
		{
			RigidTransform3D<ScalarType> Transform;
			auto Shape_region = Shape.GetSubMatrix(ALL, m_Parameter.Landmark);
			auto Basis_region = Basis.GetSubMatrix(ALL, m_Parameter.Landmark);
			Transform.SetSourceLandmarkPointSet(Shape_region);
			Transform.SetTargetLandmarkPointSet(Basis_region);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
	}
	else if (m_Parameter.TransformName == "SimilarityTransform")
	{
		if (m_Parameter.Landmark.IsEmpty() == true)
		{
			SimilarityTransform3D<ScalarType> Transform;
			Transform.SetSourceLandmarkPointSet(Shape);
			Transform.SetTargetLandmarkPointSet(Basis);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
		else
		{
			SimilarityTransform3D<ScalarType> Transform;
			auto Shape_region = Shape.GetSubMatrix(ALL, m_Parameter.Landmark);
			auto Basis_region = Basis.GetSubMatrix(ALL, m_Parameter.Landmark);
			Transform.SetSourceLandmarkPointSet(Shape_region);
			Transform.SetTargetLandmarkPointSet(Basis_region);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
	}
	else if (m_Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (m_Parameter.Landmark.IsEmpty() == true)
		{
			ThinPlateSplineTransform3D<ScalarType> Transform;
			Transform.SetSourceLandmarkPointSet(Shape);
			Transform.SetTargetLandmarkPointSet(Basis);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
		else
		{
			ThinPlateSplineTransform3D<ScalarType> Transform;
			auto Shape_region = Shape.GetSubMatrix(ALL, m_Parameter.Landmark);
			auto Basis_region = Basis.GetSubMatrix(ALL, m_Parameter.Landmark);
			Transform.SetSourceLandmarkPointSet(Shape_region);
			Transform.SetTargetLandmarkPointSet(Basis_region);
			Transform.EstimateParameter();
			return Transform.TransformPoint(Shape);
		}
	}
	else
	{
		MDK_Error("Wrong @ KNNAverageBasedShapeDictionaryBuilder::TransformShape(...)")
		DenseMatrix<ScalarType> EmptyShape;
		return EmptyShape;
	}
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_AfterALLEpoch(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount)
{
	//---------------------- already updated ------------------------------------
	auto& Basis = Dictionary.Basis();
	int_max BasisCount = Dictionary.GetBasisCount();

	//----------------- adjust BasisExperience if Data is re-used ------------
	this->AdjustBasisExperience(Dictionary.BasisExperience(), BasisExperience_init, TotalDataCount);

	//-------------------------- update other -----------------------------
	if (m_Parameter.Flag_Update_BasisAge == true)
	{
		Dictionary.BasisAge() += TotalDataCount;
	}

	if (Flag_Update_BasisSimilarity == true)
	{
		this->UpdateBasisSimilarity(Dictionary.BasisSimilarity(), Dictionary.Basis());
	}

	if (Flag_Update_BasisRedundancy == true)
	{
		this->UpdateBasisRedundancy(Dictionary.BasisRedundancy(), Dictionary.BasisSimilarity());
	}

	this->SetSimilarityThreshold(m_Parameter.SimilarityThreshold);
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount)
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


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisSimilarity(DenseMatrix<ScalarType>& BasisSimilarity, const ObjectArray<DenseMatrix<ScalarType>>& Basis)
{
	int_max BasisCount = Basis.GetLength();
	BasisSimilarity.FastResize(BasisCount, BasisCount);

	//for (int_max k = 0; k <= BasisCount - 2; ++k)
	auto TempFunction_ComputeSimilarity = [&](int_max k)
	{
		const auto& Basis_k = Basis[k];
		for (int_max n = k + 1; n < BasisCount; ++n) // start from k+1
		{
			const auto& Basis_n = Basis[n];
			auto Similarity = ComputeSimilarityBetweenShapeWithPointCorrespondence(Basis[k], Basis[n], m_Parameter.Landmark, m_Parameter.TransformName, true);
			BasisSimilarity(k, n) = Similarity;
			BasisSimilarity(n, k) = Similarity;
		}
	};
	ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisCount - 2, m_Parameter.MaxThreadCount);

	for (int_max n = 0; n < BasisCount; ++n)
	{
		BasisSimilarity(n, n) = 1;
	}
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity)
{
    int_max BasisCount = BasisSimilarity.GetColCount();
    BasisRedundancy.FastResize(1, BasisCount);
    if (BasisCount == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold;

    //for (int_max k = 0; k <= BasisCount-1; ++k)
    auto TempFunction_UpdateRedundancy = [&](int_max k)
    {
        BasisRedundancy[k] = 0;
        for (int_max n = 0; n < BasisCount; ++n)
        {
            if (k != n)
            {
                if (BasisSimilarity(n, k) >= SimilarityThreshold)
                {
                    BasisRedundancy[k] += BasisSimilarity(n, k);
                }
            }
        }
    };
    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisCount - 1, m_Parameter.MaxThreadCount);
}


}// namespace mdk

