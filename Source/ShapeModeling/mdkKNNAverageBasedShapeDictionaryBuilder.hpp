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
	m_LandmarkOnShape.Clear();
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

	int_max Dimension = (*m_TrainingShapeData)[0].GetRowCount();
	if (Dimension != 2 && Dimension != 3)
	{
		MDK_Error("Shape Dimension is NOT 2 or 3 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary == nullptr)
	{
		MDK_Error("InitialDictionary is nullptr @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_InitialDictionary->GetBasisCount() == 0)
	{
		MDK_Error("InitialDictionary is empty @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxNeighbourCount <= 0)
	{
		MDK_Error("MaxNeighbourCount <= 0 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MiniBatchSize < 0 || m_Parameter.MiniBatchSize > m_TrainingShapeData->GetLength())
	{
		MDK_Error("MiniBatchSize is out of range @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold < 0 or > 1 @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.TransformName != "IdentityTransform" && m_Parameter.TransformName != "RigidTransform" 
		&& m_Parameter.TransformName != "SimilarityTransform" && m_Parameter.TransformName != "ThinPlateSplineTransform")
	{
		MDK_Error("TransformName is unknown @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (m_LandmarkOnShape.GetLength() < 9)
		{
			MDK_Error("too few Landmark for ThinPlateSplineTransform @ KNNAverageBasedShapeDictionaryBuilder::CheckInput()")
			return false;
		}
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

	ShapeDictionary<ScalarType> OutputDictionary;
	OutputDictionary.Copy(*m_InitialDictionary);

    DenseMatrix<ScalarType> BasisExperience_init;
    BasisExperience_init.Copy(OutputDictionary.BasisExperience());
	BasisExperience_init *= m_Parameter.ExperienceDiscountFactor;

    int_max TotalDataCount = m_TrainingShapeData->GetLength();

    //------------------------------------------ run Epoch and Mini-batch -------------------------------------------------------//
        
    // random number for sampling
    std::random_device rd;
    std::mt19937 gen(rd());
	DenseVector<int_max> RandomDataIndexList = span(0, TotalDataCount - 1);

    for (int_max iter = 0; iter < m_Parameter.MaxEpochCount; ++iter)
    {
		this->AdjustBasisExperience_BeforeEachEpoch(OutputDictionary.BasisExperience());

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
			Encoder.SetInputDictionary(&OutputDictionary.Basis());
			Encoder.Update();
			const auto& CodeTable = Encoder.OutputCode();
			//update basis -----------------------------------------------
			this->UpdateBasisAndBasisExperience(OutputDictionary.Basis(), OutputDictionary.BasisExperience(), ShapeData_CurrentBatch, CodeTable);            
        }            
		this->AdjustBasisExperience_AfterEachEpoch(OutputDictionary.BasisExperience(), BasisExperience_init, TotalDataCount);       
    }

    this->UpdateDictionaryInformation_AfterALLEpoch(OutputDictionary, TotalDataCount);

    m_Dictionary.Copy(std::move(OutputDictionary));
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience)
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
	auto TempFunction = [&](int_max k)
	{		
		const auto& DataIndexList = DataIndexTable[k];		
		const auto& MembershipList = MembershipTable[k];
		if (DataIndexList.GetLength() > 0)
		{
			DenseMatrix<ScalarType> BasisChange;
			BasisChange.Resize(Basis[k].GetSize());
			BasisChange.Fill(0);
			ScalarType ExperienceChange = 0;
			for (int_max n = 0; n < DataIndexList.GetLength(); ++n)
			{
				auto DataIndex = DataIndexList[n];
				auto Membership = MembershipList[n];
				auto Shape_new = this->TransformShape(ShapeData[DataIndex], Basis[k]);
				Shape_new -= Basis[k];
				Shape_new *= Membership;
				BasisChange += Shape_new;
				ExperienceChange += Membership;
			}
			BasisExperience[k] += ExperienceChange;
			BasisChange /= BasisExperience[k];
			if (m_Parameter.TransformName == "SimilarityTransform")
			{	// transform basis to the orignal direction and size, prevent shrinking
				DenseMatrix<ScalarType> Basis_new = Basis[k];
				Basis_new += BasisChange;
				Basis[k] = this->TransformShape(Basis_new, Basis[k]);
			}
			else
			{
				Basis[k] += BasisChange;
			}
		}
	};
	ParallelForLoop(TempFunction, 0, BasisCount - 1, m_Parameter.MaxThreadCount);
}


template<typename ScalarType>
DenseMatrix<ScalarType> KNNAverageBasedShapeDictionaryBuilder<ScalarType>::TransformShape(const DenseMatrix<ScalarType>& Shape, const DenseMatrix<ScalarType>& Basis)
{
	if (m_Parameter.TransformName == "IdentityTransform")
	{
		return Shape;
	}
	else if (m_Parameter.TransformName == "RigidTransform")
	{		
		if (m_LandmarkOnShape.IsEmpty() == true)
		{
			int_max Dimension = Shape.GetRowCount();
			if (Dimension == 3)
			{
				RigidTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape);
				Transform.SetTargetLandmarkPointSet(&Basis);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
			else//Dimension==2
			{
				RigidTransform2D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape);
				Transform.SetTargetLandmarkPointSet(&Basis);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
		}
		else
		{
			int_max Dimension = Shape.GetRowCount();
			if (Dimension == 3)
			{
				RigidTransform3D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
			else//Dimension==2
			{
				RigidTransform2D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
		}
	}
	else if (m_Parameter.TransformName == "SimilarityTransform")
	{
		if (m_LandmarkOnShape.IsEmpty() == true)
		{
			int_max Dimension = Shape.GetRowCount();
			if (Dimension == 3)
			{
				SimilarityTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape);
				Transform.SetTargetLandmarkPointSet(&Basis);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
			else//Dimension==2
			{
				SimilarityTransform2D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape);
				Transform.SetTargetLandmarkPointSet(&Basis);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
		}
		else
		{
			int_max Dimension = Shape.GetRowCount();
			if (Dimension == 3)
			{
				SimilarityTransform3D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
			else//Dimension==2
			{
				SimilarityTransform3D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
		}
	}
	else if (m_Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (m_LandmarkOnShape.IsEmpty() == true)
		{
			MDK_Error("wrong @ KNNAverageBasedShapeDictionaryBuilder<ScalarType>::TransformShape(...)")
			return Basis;
		}
		else
		{
			int_max Dimension = Shape.GetRowCount();
			if (Dimension == 3)
			{
				ThinPlateSplineTransform3D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
			else//Dimension==2
			{
				ThinPlateSplineTransform2D<ScalarType> Transform;
				auto Shape_region = Shape.GetSubMatrix(ALL, m_LandmarkOnShape);
				auto Basis_region = Basis.GetSubMatrix(ALL, m_LandmarkOnShape);
				Transform.SetSourceLandmarkPointSet(&Shape_region);
				Transform.SetTargetLandmarkPointSet(&Basis_region);
				Transform.EstimateParameter();
				return Transform.TransformPoint(Shape);
			}
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
UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount)
{
	//---------------------- already updated ------------------------------------
	auto& Basis = Dictionary.Basis();
	int_max BasisCount = Dictionary.GetBasisCount();

	//-------------------------- update other -----------------------------
	if (m_Parameter.Flag_Update_BasisAge == true)
	{
		Dictionary.BasisAge() += TotalDataCount;
	}

	if (m_Parameter.Flag_Update_BasisSimilarity == true)
	{
		this->UpdateBasisSimilarity(Dictionary.BasisSimilarity(), Dictionary.Basis());
	}

	if (m_Parameter.Flag_Update_BasisRedundancy == true)
	{
		this->UpdateBasisRedundancy(Dictionary.BasisRedundancy(), Dictionary.BasisSimilarity());
	}

	Dictionary.SetSimilarityThreshold(m_Parameter.SimilarityThreshold);
}


template<typename ScalarType>
void KNNAverageBasedShapeDictionaryBuilder<ScalarType>::
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
			auto Similarity = ComputeSimilarityBetweenShapeWithPointCorrespondence(Basis[k], Basis[n], m_LandmarkOnShape, m_Parameter.TransformName, true);
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

