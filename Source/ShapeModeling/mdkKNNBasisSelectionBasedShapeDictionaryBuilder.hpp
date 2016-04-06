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

//---------------------------------------------------------------------------------------------------------------//

template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
	m_TrainingShapeData = nullptr;
	m_LandmarkOnShape.Clear();
    m_InitialDictionary = nullptr;
	m_Dictionary.Clear();
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

	if (m_Parameter.MiniBatchSize < 0 || m_Parameter.MiniBatchSize > m_TrainingShapeData->GetLength())
	{
		MDK_Error("MiniBatchSize is out of range @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}
	
	if (m_Parameter.BasisCount <= 0)
	{
		MDK_Error("BasisCount <= 0 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}
	
	if (m_Parameter.BasisCount > m_TrainingShapeData->GetLength())
	{
		MDK_Error("BasisCount > DataCount @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxNeighbourCount <= 0)
	{
		MDK_Error("MaxNeighbourCount <= 0 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

    if (m_Parameter.WeightOnProbabiliyForBasisSelection < 0 || m_Parameter.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
        return false;
    }

	if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold is out of range @ KNNBasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
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

	ShapeDictionary<ScalarType> OutputDictionary;
	ScalarType TotalBasisExperience_init = 0;
	if (m_InitialDictionary != nullptr)
	{
		OutputDictionary.Copy(*m_InitialDictionary);
		TotalBasisExperience_init = OutputDictionary.BasisExperience().Sum();	
	}

	int_max TotalDataCount = m_TrainingShapeData->GetLength();

   //--------------------------------- run Epoch and Minibatch -------------------------------------------------------//

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
			OutputDictionary = this->BuildDictionaryInMiniBatch(OutputDictionary, ShapeData_CurrentBatch);
		}
		this->AdjustBasisExperience_AfterEachEpoch(OutputDictionary.BasisExperience(), TotalBasisExperience_init, TotalDataCount);
	}

    this->UpdateDictionaryInformation_AfterALLEpoch(OutputDictionary, TotalDataCount);
	//------------------------------------
    m_Dictionary.Copy(std::move(OutputDictionary)); 
}


template<typename ScalarType>
void
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience)
{
	if (BasisExperience.IsEmpty() == true)
	{
		return;
	}

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
ShapeDictionary<ScalarType> 
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
BuildDictionaryInMiniBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData)
{
    ShapeDictionary<ScalarType> Dictionary;// the output
    Dictionary.Name() = m_Parameter.Name;
    int_max BasisCount_desired = m_Parameter.BasisCount;

    //------------------------------------------- check input ---------------------------------------------------------------//
    //ShapeData is a mini batch from all the traning shape data
	if (ShapeData.IsEmpty() == true)
    {
        MDK_Error("ShapeData is empty @ KNNBasisSelectionBasedShapeDictionaryBuilder::BuildDictionaryInMiniBatch(...)")
        return Dictionary;
    }

    //--------------------------- combine ShapeData and Dictionary_init.Basis to Compute ShapeSimilarityMatrix ---------//
	// just for reference: what is CombinedData
	// CombinedData = { Dictionary_init.Basis(), ShapeData };

    int_max DataCount = ShapeData.GetLength();
	int_max BasisCount_init = Dictionary_init.GetBasisCount(); 
    int_max TotalShapeCount = BasisCount_init + DataCount;

    auto ShapeSimilarityMatrix = this->ComputeShapeSimilarityMatrix(Dictionary_init, ShapeData);

    if (m_Parameter.Debug_Flag == true)
    {
        String FilePathAndName = m_Parameter.Debug_FilePath + "ShapeSimilarityMatrix.json";
        SaveDenseMatrixAsJsonDataFile(ShapeSimilarityMatrix, FilePathAndName);
    }

    // ------- Output Dictionary if the number of data samples and input bases <= the number of desired bases -------------------------------------//
    // note: do not re-use data sample, i.e., OutputBasisCount may not = BasisCount_desired

    if (BasisCount_desired >= TotalShapeCount)
    {
        MDK_Warning("BasisCount_desired >= TotalShapeCount @ KNNBasisSelectionBasedShapeDictionaryBuilder::BuildDictionaryInMiniBatch(...)")

		auto& Basis = Dictionary.Basis();
		auto& BasisAge = Dictionary.BasisAge();
        BasisAge.FastResize(1, TotalShapeCount);
        BasisAge.Fill(0);
        auto& BasisExperience = Dictionary.BasisExperience(); 
        BasisExperience.FastResize(1, TotalShapeCount);
        BasisExperience.Fill(1);

		auto BasisCount_init = Dictionary_init.GetBasisCount();
        if (BasisCount_init > 0)
        {
			const auto& Basis_init = Dictionary_init.Basis();
            Basis = { &Basis_init, &ShapeData };
            BasisAge(span(0, BasisCount_init - 1)) = Dictionary_init.BasisAge();
            BasisExperience(span(0, BasisCount_init - 1)) = Dictionary_init.BasisExperience();
        }
        else
        {
            Basis = ShapeData;
        }

		// all shape as basis
        DenseMatrix<int_max> ShapeIndexList_Basis = span(0, Dictionary.GetBasisCount()-1);
        auto CodeTable = this->EncodeShapeDataBySimilarity(ShapeSimilarityMatrix, ShapeIndexList_Basis, BasisCount_init);
        this->UpdateDictionaryInformation_AtEachMiniBatch(Dictionary, ShapeData, CodeTable, ShapeSimilarityMatrix, ShapeIndexList_Basis, Dictionary_init);

        return Dictionary;
    }

    //------------------------------------------ select basis from Combined Data -----------------------------------------------------------------//

    // find KNN of each shape in Combined Data
    auto KNNShapeIndexTable = this->FindKNNShapeIndexTableByShapeSimilarityMatrix(ShapeSimilarityMatrix);

	// estimate the probability mass function
    auto RepresentativeAbilityOfEachShape_init = this->ComputeInitialRepresentativeAbilityOfEachShape(Dictionary_init, TotalShapeCount);
    auto ProbabilityOfEachShape = this->EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachShape(ShapeSimilarityMatrix,
                                                                                                      KNNShapeIndexTable, 
                                                                                                      RepresentativeAbilityOfEachShape_init);
    
    DenseMatrix<int_max> ShapeIndexList_Basis = this->SelectBasis(BasisCount_desired, ShapeSimilarityMatrix, ProbabilityOfEachShape);

    int_max OutputBasisCount = ShapeIndexList_Basis.GetElementCount();

    // ------------- create Basis --------------------------------------------------//

    auto& Basis = Dictionary.Basis();     
    Basis.FastResize(OutputBasisCount);
    //for (int_max k = 0; k <= OutputBasisCount-1; ++k)
    auto TempFunction_CreateBasis = [&](int_max k)
    {
        auto ShapeIndex = ShapeIndexList_Basis[k];
        if (ShapeIndex < BasisCount_init)
        {
			Basis[k] = Dictionary_init.Basis()[ShapeIndex];
        }
        else
        {
            int_max tempIndex_k = ShapeIndex - BasisCount_init;
			Basis[k] = ShapeData[tempIndex_k];
        }
    };
    ParallelForLoop(TempFunction_CreateBasis, 0, OutputBasisCount-1, m_Parameter.MaxThreadCount);

    // -------- encode ShapeData : code is similarity, not membership ------------------//
    auto CodeTable = this->EncodeShapeDataBySimilarity(ShapeSimilarityMatrix, ShapeIndexList_Basis, BasisCount_init);

    // --------------- Update DictionaryInformation --------------------------------------------//
    this->UpdateDictionaryInformation_AtEachMiniBatch(Dictionary, ShapeData, CodeTable, ShapeSimilarityMatrix, ShapeIndexList_Basis, Dictionary_init);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ScalarType>
DenseMatrix<int_max>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::SelectBasis(const int_max BasisCount_desired,
	                                                                  const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
	                                                                  const DenseMatrix<ScalarType>& ProbabilityOfEachShape)
{
	//---------------------------------------------------------------------
	// Combined data = {Dictionary_init.Basis(), ShapeData}
	//
	// the number of combined data is greater than the number of bases
	// extract a subset from the combined data set to be the bases
	// return the selected shape index list 
	// shape index is the index in Combined data
	//----------------------------------------------------------------------

	DenseMatrix<int_max> ShapeIndexList_Basis;//output

	int_max TotalShapeCount = ProbabilityOfEachShape.GetElementCount();
	//---------------------------------------------------------------
	DenseVector<ScalarType> ShapeScoreList;
	ShapeScoreList.Resize(TotalShapeCount);
	for (int_max k = 0; k < TotalShapeCount; ++k)
	{
		ShapeScoreList[k] = (2*m_Parameter.WeightOnProbabiliyForBasisSelection-1)*ProbabilityOfEachShape[k];
	}
	auto ShapeIndexList_sort = ShapeScoreList.Sort("descend");
    //---------------------------------------------------------------

	int_max MaxIter = 1000;
	for (int_max iter = 0; iter <= MaxIter; ++iter)
	{
		ScalarType SimilarityThreshold = m_Parameter.SimilarityThreshold + iter* (1 - m_Parameter.SimilarityThreshold) / ScalarType(MaxIter);

		DenseVector<DenseVector<ScalarType>> NeighbourTable;
		NeighbourTable.Resize(TotalShapeCount);
		for (int_max k = 0; k < TotalShapeCount; ++k)
		{
			NeighbourTable[k].SetCapacity(m_Parameter.MaxNeighbourCount*10);
			for (int_max n = 0; n < TotalShapeCount; ++n)
			{
				if (n != k)
				{
					if (ShapeSimilarityMatrix(k, n) >= SimilarityThreshold)
					{
						NeighbourTable[k].Append(n);
					}
				}
			}
		}

		DenseVector<ScalarType> FlagList_basis, FlagList_other;
		FlagList_basis.Resize(TotalShapeCount);
		FlagList_basis.Fill(0);
		FlagList_other.Resize(TotalShapeCount);
		FlagList_other.Fill(1);
		//FlagList_basis[k] is 0 if k is basis
		//FlagList_other[k] is 0 if k is not a basis 

		for (int_max k = 0; k < TotalShapeCount; ++k)
		{
			auto Index = ShapeIndexList_sort[k];
			if (FlagList_other[k] > 0)
			{
				FlagList_basis[k] = 1;
				const auto& NeighbourList = NeighbourTable[k];
				for (int_max n = 0; n < NeighbourList.GetLength(); ++n)
				{
					auto Index_n = NeighbourList[n];
					if (FlagList_basis[Index_n] == 0)
					{
						FlagList_other[Index_n] = 0;
					}
				}
			}
		}

		auto BasisCount = FlagList_basis.Sum();		
		if (FlagList_basis.Sum() >= BasisCount_desired)
		{
			ShapeIndexList_Basis.SetCapacity(BasisCount);
			for (int_max k = 0; k < TotalShapeCount; ++k)
			{
				if (FlagList_basis[k] > 0)
				{
					ShapeIndexList_Basis.Append(k);
				}
			}
			break;
		}
	}
	
	return ShapeIndexList_Basis;
}


template<typename ScalarType>
DenseMatrix<int_max>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::SelectBasis_old(const int_max BasisCount_desired,
                                                                          const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                                          const DenseMatrix<ScalarType>& ProbabilityOfEachShape)
{
	//---------------------------------------------------------------------
    // Combined data = {Dictionary_init.Basis(), ShapeData}
    //
    // the number of combined data is greater than the number of bases
    // extract a subset from the combined data set to be the bases
	// return the selected shape index list 
	// shape index is the index in Combined data
	//----------------------------------------------------------------------

    //---------- get  ShapePairIndexList and ShapePairScoreList sored from the biggest to the smallest --------------//

    int_max TotalShapeCount = ProbabilityOfEachShape.GetElementCount();
    int_max ShapePairCount = TotalShapeCount*(TotalShapeCount - 1) / 2;

	DenseMatrix<int_max> ShapePairIndexList(2, ShapePairCount);
    DenseMatrix<ScalarType> ShapePairScoreList(1, ShapePairCount);

    auto ScoreWeight_S = ScalarType(0);  // weight on similarity
    auto ScoreWeight_PS = ScalarType(0); // weight on probability * similarity
    auto ScoreWeight_P = ScalarType(0);  // weight on probability

    if (m_Parameter.WeightOnProbabiliyForBasisSelection <= ScalarType(0.5))
    {
        ScoreWeight_S = 1 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_PS = 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P = 0;
    }
    else
    {
        ScoreWeight_S = 0;
        ScoreWeight_PS = 2 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P = 2 * (m_Parameter.WeightOnProbabiliyForBasisSelection - ScalarType(0.5));
    }

    int_max Counter = 0;
    for (int_max k = 0; k < TotalShapeCount - 1; ++k)
    {
        for (int_max n = k + 1; n < TotalShapeCount; ++n)
        {
            auto tempScore_P = ProbabilityOfEachShape[n] + ProbabilityOfEachShape[k]; // use max ?
            auto tempScore_S = 1 - ShapeSimilarityMatrix(n, k);
            auto tempScore_PS = tempScore_P*tempScore_S;

            ShapePairScoreList[Counter] = ScoreWeight_PS * tempScore_PS + ScoreWeight_S * tempScore_S + ScoreWeight_P * tempScore_P;
            ShapePairIndexList(0, Counter) = k;
            ShapePairIndexList(1, Counter) = n;
            Counter += 1;
        }
    }
    // sort ShapePairScoreList
	DenseMatrix<int_max> tempIndexList_sort = ShapePairScoreList.Sort("ascend");
	ShapePairScoreList = ShapePairScoreList.GetSubMatrix(ALL, tempIndexList_sort);
	// update ShapePairIndexList
	ShapePairIndexList = ShapePairIndexList.GetSubMatrix(ALL, tempIndexList_sort);

    //------------------ select basis by removing redundant shape ---------------------------------//
    // get the pair (shape_a, shape_b) with the highest score in the current basis set
    // remove shape_a or shape_b (that has a lower probability) from the current basis set
    // repeat, until the number of shapes in the basis set is m_Parameter.BasisCount 

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    DenseMatrix<int_max> ShapeFlagList(1, TotalShapeCount);
    ShapeFlagList.Fill(1);
    // ShapeFlagList(i) is 0 : shape_i is removed from the current basis set
    // ShapeFlagList(i) is 1 : shape_i is still in the current basis set

    int_max CurrentBasisCount = TotalShapeCount; // CurrentBasisCount is the number of shapes In Current Basis Set

    for (int_max k = 0; k < ShapePairCount; ++k)
    {
        if (CurrentBasisCount <= BasisCount_desired)
        {
            break;
        }

        auto Index_a = ShapePairIndexList(0, k);
        auto Index_b = ShapePairIndexList(1, k);

        if (ShapeFlagList[Index_a] > 0 && ShapeFlagList[Index_b] > 0)
        {
            auto Prob_a = ProbabilityOfEachShape[Index_a];
            auto Prob_b = ProbabilityOfEachShape[Index_b];

            if (std::abs(Prob_a - Prob_b) <= eps_value)
            {
                bool temp_bool = BoolRandomNumber(gen_bool);
                // randomly discard shape_a or shape_b
                //if (k % 2 == 0)
                if (temp_bool == true)
                {
                    ShapeFlagList[Index_a] = 0;
                }
                else
                {
                    ShapeFlagList[Index_b] = 0;
                }
            }
            else if (Prob_a > Prob_b)
            {
                ShapeFlagList[Index_b] = 0;
            }
            else// if (Prob_a < Prob_b)
            {
                ShapeFlagList[Index_a] = 0;
            }

            CurrentBasisCount -= 1;
        }
    }

    DenseMatrix<int_max> ShapeIndexList_Basis;
    ShapeIndexList_Basis.SetCapacity(CurrentBasisCount);
    for (int_max k = 0; k < TotalShapeCount; ++k)
    {
        if (ShapeFlagList[k] > 0)
        {
            ShapeIndexList_Basis.AppendCol({ k });
        }
    }

    return ShapeIndexList_Basis;
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
ComputeInitialRepresentativeAbilityOfEachShape(const ShapeDictionary<ScalarType>& Dictionary_init, int_max TotalShapeCount)
{
    //---------------------------------------------------------------------------------------------------
    // Input:    
    // CombinedData = { Dictionary_init.Basis(),  ShapeData}
    //
    // TotalShapeCount = BasisCount_init + DataCount
    //
    // if Dictionary_init is empty, then CombinedData = ShapeData 
    //
    // Output:
    // RepresentativeAbility is the ability to represent many shapes (including self, so it is >= 1)
    //----------------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> RepresentativeAbility(1, TotalShapeCount);
    RepresentativeAbility.Fill(ScalarType(1));

	auto BasisCount_init = Dictionary_init.GetBasisCount();
    if (BasisCount_init > 0)
    {
		const auto& BasisExperience_init = Dictionary_init.BasisExperience();

        for (int_max k = 0; k < BasisCount_init; k++)
        {
            RepresentativeAbility[k] = BasisExperience_init[k];
        }

        for (int_max k = BasisCount_init; k < TotalShapeCount; k++)
        {
            RepresentativeAbility[k] = ScalarType(1);
        }
    }

    return RepresentativeAbility;
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
ComputeShapeSimilarityMatrix(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData)
{
    //------------------------------------------------------------------------------------------
    // Input:
    // ShapeData is a set of shapes
    // 
    // Output:
    // ShapeSimilarityMatrix of Combined data = [Dictionary_init.Basis(), ShapeData]
    // 
    // ShapeSimilarityMatrix(i, j) = ShapeSimilarityMatrix(j, i)
    // ShapeSimilarityMatrix(i, i) = 0 for all i
    //
    // a pair of shape_i and shape_j is called shape pair
    //------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> ShapeSimilarityMatrix;

	auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold;

    //---------------------------------------------------------------------------------------------

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    std::random_device rd_uniform;
    std::mt19937 gen_uniform(rd_uniform());
    std::uniform_real_distribution<ScalarType> UniformRandomNumber(0, ScalarType(1));

    //-------------------------------------------------------------------------------------------------------------
    auto TempFunction_AndRandomNumberToSimilarity = [&](ScalarType Similarity)
    {
        // (1) if Similarity >=  SimilarityThreshold + eps_value
        //     add eps_value noise to similarity of every pair in case there are two pairs with the same similarity
        //
        // (2) if Similarity < SimilarityThreshold + eps_value
        //     add some random noise, and make sure Similarity < SimilarityThreshold + eps_value
        //     then,  data pair with Similarity < SimilarityThreshold + eps_value will be sorted/sampled randomly
        //     It is a kind of random sampling
        //     note: when Similarity is small, the Similarity metric it self is not accurate
        //
        // (3) make sure Similarity in the range [0, 1]

        if (Similarity >= SimilarityThreshold + eps_value)
        {
            bool Flag = BoolRandomNumber(gen_bool);

            if (Flag == true)
            {
                Similarity += eps_value;
            }
            else
            {
                Similarity -= eps_value;
            }
        }
        else
        {
            auto tempValue = UniformRandomNumber(gen_uniform);

            Similarity *= tempValue;

        }

        if (Similarity < ScalarType(0))
        {
            Similarity = ScalarType(0);
        }
        else if (Similarity > ScalarType(1))
        {
            Similarity = ScalarType(1);
        }

        return Similarity;
    };

    //--------------------------------------------------------------------------------------------------------------

    if (Dictionary_init.BasisSimilarity().IsEmpty() == true)
    {
        int_max TotalShapeCount = ShapeData.GetElementCount();

        ShapeSimilarityMatrix.FastResize(TotalShapeCount, TotalShapeCount);
        ShapeSimilarityMatrix.Fill(ScalarType(0));  // ShapeSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalShapeCount - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            const auto& Shape_k = ShapeData[k];
            for (int_max n = k + 1; n < TotalShapeCount; ++n) // start from k+1
            {
                auto Shape_n = ShapeData[n];
                auto Similarity = this->ComputeShapeSimilarity(Shape_k, Shape_n);
                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);            
                ShapeSimilarityMatrix(k, n) = Similarity;
                ShapeSimilarityMatrix(n, k) = Similarity;
            }
        };
        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalShapeCount - 2, m_Parameter.MaxThreadCount);
    }
    else
    {
		const auto& Basis_init = Dictionary_init.Basis();
		const auto& BasisSimilarity_init = Dictionary_init.BasisSimilarity();

		int_max BasisCount_init = Dictionary_init.GetBasisCount();
		int_max DataCount = ShapeData.GetElementCount();
		auto TotalShapeCount = BasisCount_init + DataCount;

        ShapeSimilarityMatrix.FastResize(TotalShapeCount, TotalShapeCount);
        ShapeSimilarityMatrix.Fill(ScalarType(0));  // ShapeSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalShapeCount - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
			DenseMatrix<ScalarType> Shape_k;            
            if (k < BasisCount_init)
            {
				Shape_k = Basis_init[k];
            }
            else if (k >= BasisCount_init)
            {
                int_max temp_k = k - BasisCount_init;
				Shape_k = ShapeData[temp_k];
            }

            for (int_max n = k + 1; n < TotalShapeCount; ++n) // start from k+1
            {
				DenseMatrix<ScalarType> Shape_n;
                if (n < BasisCount_init)
                {
					Shape_n = Basis_init[n];
                }
                else if (n >= BasisCount_init)
                {
                    int_max temp_n = n - BasisCount_init;
					Shape_n = ShapeData[temp_n];
                }

                auto Similarity = ScalarType(0);
				if (k < BasisCount_init && n < BasisCount_init)
				{
					Similarity = BasisSimilarity_init(n, k);
				}
				else
				{
					Similarity = this->ComputeShapeSimilarity(Shape_k, Shape_n);
				}
                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);
                ShapeSimilarityMatrix(k, n) = Similarity;
                ShapeSimilarityMatrix(n, k) = Similarity;
            }
        };
        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalShapeCount - 2, m_Parameter.MaxThreadCount);
    }

    return ShapeSimilarityMatrix;
}


template<typename ScalarType>
ObjectArray<DenseMatrix<int_max>>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::FindKNNShapeIndexTableByShapeSimilarityMatrix(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix)
{
    //-------------------------------------------------------------------------------------------------------------
    // Input:
    // ShapeSimilarityMatrix is from this->ComputeShapeSimilarityMatrix(...)
    // 
    // Output:
    // KNNShapeIndexTable[k] is the KNN index list of the shape with index k in ShapeSimilarityMatrix
    // Index Range in KNNShapeIndexTable[k] is from 0 to BasisCount_init + DataCount -1
    //
    // ShapeSimilarityMatrix(i, i) = 0 for all i is guaranteed in this->ComputeShapeSimilarityMatrix(...)
    // Then the shape-index of the shape-k is not included in KNNShapeIndexTable[k], i.e., self is not a KNN-neighbor of self
    //--------------------------------------------------------------------------------------------------------------

    int_max TotalShapeCount = ShapeSimilarityMatrix.GetColCount();

    ObjectArray<DenseMatrix<int_max>> KNNShapeIndexTable;
    KNNShapeIndexTable.Resize(TotalShapeCount);

    //for (int_max k = 0; k <= TotalShapeCount-1; ++k)
    auto TempFunction_FindKNN = [&](int_max k)
    {
		auto SimilarityList = ShapeSimilarityMatrix.RefCol(k);
		KNNShapeIndexTable[k] = FindKNNBySimilarityList(SimilarityList, m_Parameter.MaxNeighbourCount, m_Parameter.SimilarityThreshold);
    };
    ParallelForLoop(TempFunction_FindKNN, 0, TotalShapeCount - 1, m_Parameter.MaxThreadCount);

    return KNNShapeIndexTable;
}


template<typename ScalarType>
DenseMatrix<ScalarType>    
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachShape(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                              const ObjectArray<DenseMatrix<int_max>>& KNNShapeIndexTable,
                                                              const DenseMatrix<ScalarType>& InitialRepresentativeAbilityOfEachShape)
{
    //-----------------------------------------------------------------------------------------------------------------
    // Input:
    // ShapeSimilarityMatrix is from this->ComputeShapeSimilarityMatrix(...)
    // KNNShapeIndexTable is from this->FindKNNShapeIndexTableByShapeSimilarityMatrix(...)
    // InitialRepresentativeAbilityOfEachShape is from this->ComputeInitialRepresentativeAbilityOfEachShape(...)
    // 
    // Output:
    // Probability is the KNN-Smoothed And Normalized Representative Ability Of Each Shape
    // 
    // assume each shape can be a basis
    // then estimate the Representative Ability of each shape by counting the normalized number of KNN-neighbors
    //
    // this is no self-counting because KNN does not include self in KNNShapeIndexTable 
    //
    // note: 
    // If a shape is from initial dictionary, and one of its KNN-neighbors is also from initial dictionary
    // Then, the Representative Ability of this vector is significantly increased
    // (i.e., it has more neighbors "2*K", compared to a shape from ShapeData )
    // Therefore: The estimation accuracy may be decreased
    // But: the previous basis will have a higher chance to be re-selected, and this is good   
    // 
    //-----------------------------------------------------------------------------------------------------------------

    int_max TotalShapeCount = KNNShapeIndexTable.GetLength();

    DenseMatrix<ScalarType> Probability = InitialRepresentativeAbilityOfEachShape;

    DenseMatrix<ScalarType> MembershipList;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < TotalShapeCount; ++k)
    {
        const auto& KNN_IndexList = KNNShapeIndexTable[k];
        int_max tempNeighbourCount = KNN_IndexList.GetElementCount();
		if (tempNeighbourCount > 0)
		{
			MembershipList.FastResize(1, tempNeighbourCount);
			for (int_max m = 0; m < tempNeighbourCount; ++m)
			{
				MembershipList[m] = ShapeSimilarityMatrix(KNN_IndexList[m], k) + eps_value;
			}
			MembershipList /= MembershipList.Sum();

			for (int_max m = 0; m < tempNeighbourCount; ++m)
			{
				auto ShapeIndex_m = KNN_IndexList[m];			
				Probability[ShapeIndex_m] += MembershipList[m];
			}
		}
    }

	auto ProbSum = Probability.Sum();
	if (ProbSum <= eps_value)
	{
		Probability.Fill(ScalarType(1) / ScalarType(TotalShapeCount));
	}
	else
	{
		Probability /= ProbSum;
	}

    if (m_Parameter.Debug_Flag == true)
    {
        String FilePathAndName = m_Parameter.Debug_FilePath + "ShapeProbabilityList_with_init.json";
        SaveDenseMatrixAsJsonDataFile(Probability, FilePathAndName);
    }

    return Probability;
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>>
KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
EncodeShapeDataBySimilarity(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix, const DenseMatrix<int_max>& ShapeIndexList_Basis, int_max BasisCount_init)
{
	int_max BasisCount = ShapeIndexList_Basis.GetElementCount();
	int_max TotalShapeCount = ShapeSimilarityMatrix.GetColCount();
	int_max DataCount = TotalShapeCount - BasisCount_init;

	ObjectArray<SparseVector<ScalarType>> CodeTable;
	CodeTable.FastResize(DataCount);

	//for (int_max k = 0; k <= DataCount-1; ++k)
	auto TempFunction_EncodeDataVector = [&](int_max k)
	{
		DenseMatrix<ScalarType> SimilarityList(1, BasisCount);
		SimilarityList.Fill(ScalarType(0));

		int_max ShapeIndex_Data = BasisCount_init + k;

		for (int_max n = 0; n < BasisCount; ++n)
		{
			int_max ShapeIndex_Basis = ShapeIndexList_Basis[n];
			SimilarityList[n] = ShapeSimilarityMatrix(ShapeIndex_Basis, ShapeIndex_Data);
		}

		auto KNNBasisIndexList = FindKNNBySimilarityList(SimilarityList, m_Parameter.MaxNeighbourCount, m_Parameter.SimilarityThreshold);
		auto tempNeighbourCount = KNNBasisIndexList.GetElementCount();
		if (tempNeighbourCount > 0)
		{
			DenseMatrix<ScalarType> KNNBasisSimilarityList;
			KNNBasisSimilarityList.FastResize(1, tempNeighbourCount);
			for (int_max m = 0; m < tempNeighbourCount; ++m)
			{
				KNNBasisSimilarityList[m] = SimilarityList[KNNBasisIndexList[m]];
			}
			CodeTable[k].Initialize(KNNBasisIndexList, KNNBasisSimilarityList, BasisCount);
		}
	};
	ParallelForLoop(TempFunction_EncodeDataVector, 0, DataCount - 1, m_Parameter.MaxThreadCount);

	return CodeTable;
}


template<typename ScalarType>
ScalarType KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	return KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::ComputeShapeSimilarity(ShapeA, ShapeB, m_LandmarkOnShape, m_Parameter.TransformName, true);
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_AtEachMiniBatch(ShapeDictionary<ScalarType>& Dictionary,
                                            const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
                                            const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                            const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                            const DenseMatrix<int_max>& ShapeIndexList_Basis,
                                            const ShapeDictionary<ScalarType>& Dictionary_init)
{
	// Combined Data = {Dictionary_init.Basis(), ShapeData}
	// ShapeSimilarityMatrix is from Combined Data

    // Basis has been updated
    auto& Basis = Dictionary.Basis();
	int_max BasisCount = Dictionary.GetBasisCount();

	//To be initialized or updated in this function
    auto& BasisID = Dictionary.BasisID();
	auto& BasisAge = Dictionary.BasisAge();
	auto& BasisExperience = Dictionary.BasisExperience();
	auto& BasisSimilarity = Dictionary.BasisSimilarity();
	auto& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisCount_init = Dictionary_init.GetBasisCount();

    //---------------------- copy SeedForNewBasisIDGeneration ---------------------------//   
    if (BasisCount_init > 0)
    {
        Dictionary.SetCurrentSeedForNewBasisIDGeneration(Dictionary_init.GetCurrentSeedForNewBasisIDGeneration());
    }

    //--------------------- initialize BasisID --------------------------------------------//

    BasisID.FastResize(1, BasisCount);
    BasisID.Fill(0); // BasisID is always > 0, fill 0 to mark new basis
    if (BasisCount_init > 0)
    {
        const auto& BasisID_init = Dictionary_init.BasisID();
        for (int_max k = 0; k < BasisCount; ++k)
        {
            auto tempIndex = ShapeIndexList_Basis[k];
            if (tempIndex < BasisCount_init)
            {   // this basis is from Dictionary_init
                BasisID[k] = BasisID_init[tempIndex];
            }
        }
    }

    //------------------- initialize BasisAge ----------------------------------------//

    BasisAge.FastResize(1, BasisCount);
    BasisAge.Fill(0); // fill 0 for new basis
    if (BasisCount_init > 0)
    {
        const DenseMatrix<ScalarType>& BasisAge_init = Dictionary_init.BasisAge();
        for (int_max k = 0; k < BasisCount; ++k)
        {
            auto tempIndex = ShapeIndexList_Basis[k];
            if (tempIndex < BasisCount_init)
            {   // this basis is from Dictionary_init
                BasisAge[k] = BasisAge_init[tempIndex];
            }
        }
    }

    //--------------------- initialize BasisExperience ---------------------------------------//
    // BasisiNumber in updated Basis may be different than that in Dictionary_init

    BasisExperience.FastResize(1, BasisCount);
    BasisExperience.Fill(ScalarType(1)); // fill 1 for new basis
    if (BasisCount_init > 0)
    {
        const auto& BasisExperience_init = Dictionary_init.BasisExperience();
        for (int_max k = 0; k < BasisCount; ++k)
        {
            auto tempIndex = ShapeIndexList_Basis[k];
            if (tempIndex < BasisCount_init) 
            {   // this basis is from Dictionary_init
                BasisExperience[k] = BasisExperience_init[tempIndex];
            }
        }
    }

    // ----------- update BasisSimilarity ------------------------------------//

    if (m_Parameter.Flag_Update_BasisSimilarity == true)
    {
		BasisSimilarity.FastResize(BasisCount, BasisCount);
        for (int_max k = 0; k < BasisCount; ++k)
        {
            auto ShapeIndex_k = ShapeIndexList_Basis[k];
            for (int_max n = 0; n < BasisCount; ++n)
            {
                auto ShapeIndex_n = ShapeIndexList_Basis[n];
                auto Similarity = ShapeSimilarityMatrix(ShapeIndex_n, ShapeIndex_k);
				BasisSimilarity(n, k) = Similarity;
				BasisSimilarity(k, n) = Similarity;
            }
        }        
    }

	Dictionary.SetSimilarityThreshold(m_Parameter.SimilarityThreshold);

    //--------------------- update BasisExperience -----------------------------//
	BasisRedundancy.FastResize(1, BasisCount);
	BasisRedundancy.Fill(1);
    this->UpdateBasisExperience_AtEachMiniBatch(BasisExperience, CodeTable);
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisExperience_AtEachMiniBatch(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{
	//--------------------------------------------------------------------------------------
	// Input:
	// BasisExperience is initialized
	//
	// CodeTable is from this->EncodeShapeDataBySimilarity(...), code of each shape data
	//
	// Output:
	// BasisExperience
	//
	// BasisExperience[n] is estimated by counting the weighted-number/membership of KNN of the basis n
	//--------------------------------------------------------------------------------------

	int_max BasisCount = BasisExperience.GetElementCount();
	int_max DataCount = CodeTable.GetLength();

	auto eps_value = std::numeric_limits<ScalarType>::epsilon();

	DenseMatrix<ScalarType> Membership;

	for (int_max k = 0; k < DataCount; ++k)
	{
		const auto& KNN_IndexList = CodeTable[k].IndexList();
		const auto& KNN_Similarity = CodeTable[k].ElementList();
		auto tempNeighbourCount = KNN_IndexList.GetLength();
		if (tempNeighbourCount > 0)
		{
			Membership = KNN_Similarity + eps_value;
			Membership /= Membership.Sum();
			for (int_max m = 0; m < tempNeighbourCount; ++m)
			{
				BasisExperience[KNN_IndexList[m]] += Membership[m];
			}
		}
	}

	// the total Experience is
	// BasisExperience.Sum() <= BasisExperience.Sum() + DataCount
	//
	// the new "Experience" of the dictionary gained from data is DataCount
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, ScalarType TotalBasisExperience_init, int_max TotalDataCount)
{
	// adjust the experience of each new basis
	int_max BasisCount = BasisExperience.GetElementCount();
	ScalarType TotalExperience = BasisExperience.Sum();
	if (TotalExperience > TotalBasisExperience_init + TotalDataCount)
	{
		ScalarType factor = (TotalBasisExperience_init + ScalarType(TotalDataCount)) / TotalExperience;
		BasisExperience *= factor;
		for (int_max k = 0; k < BasisCount; ++k)
		{
			if (BasisExperience[k] < 1)
			{
				BasisExperience[k] = 1;
			}
		}
	}
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount)
{
    DenseMatrix<int_max>& BasisID = Dictionary.BasisID();
    DenseMatrix<ScalarType>& BasisAge = Dictionary.BasisAge();
    DenseMatrix<ScalarType>& BasisSimilarity = Dictionary.BasisSimilarity();
    DenseMatrix<ScalarType>& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisCount = BasisID.GetElementCount();

    //--------------------- update BasisID for new basis -------------------------//

    if (m_Parameter.Flag_Update_BasisID == true)
    {
        for (int_max k = 0; k < BasisCount; ++k)
        {
            if (BasisID[k] == 0)
            {   // this basis is from data
                BasisID[k] = Dictionary.GenerateNewBasisID();
            }
        }
    }

    //------------------- update BasisAge ------------------------------------------//

    if (m_Parameter.Flag_Update_BasisAge == true)
    {
        BasisAge += TotalDataCount;
    }

    //---------- Update BasisRedundancy --------------------------------------------//

    if (m_Parameter.Flag_Update_BasisRedundancy == true)
    {
        this->UpdateBasisRedundancy_AfterALLEpoch(BasisRedundancy, BasisSimilarity);
    }
}


template<typename ScalarType>
void KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateBasisRedundancy_AfterALLEpoch(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity)
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

}//namespace mdk
