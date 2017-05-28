#pragma once

namespace mdk
{

template<typename ScalarType>
BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::BasisSelectionBasedShapeDictionaryBuilder()
{
    this->Clear();
}


template<typename ScalarType>
BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::~BasisSelectionBasedShapeDictionaryBuilder()
{
}

template<typename ScalarType>
void BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::Clear()
{
    Input.Parameter.Clear();
	Input.TrainingShapeData = nullptr;
	Input.LandmarkOnShape.Clear();
    Input.InitialDictionary = nullptr;
	Output.Dictionary.Clear();
}


template<typename ScalarType>
bool BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::CheckInput()
{
    if (Input.TrainingShapeData == nullptr)
    {
        MDK_Error("Input.TrainingShapeData is nullptr @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
        return false;
    }

    if (Input.TrainingShapeData->IsEmpty() == true)
    {
        MDK_Error("InputShapeData is empty @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
        return false;
    }

	if (Input.Parameter.BatchSize < 0 || Input.Parameter.BatchSize > Input.TrainingShapeData->GetLength())
	{
		MDK_Error("BatchSize is out of range @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}
	
	if (Input.Parameter.BasisCount <= 0)
	{
		MDK_Error("BasisCount <= 0 @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}
	
	if (Input.Parameter.BasisCount > Input.TrainingShapeData->GetLength())
	{
		MDK_Error("BasisCount > DataCount @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (Input.Parameter.TransformName != "IdentityTransform" && Input.Parameter.TransformName != "RigidTransform" 
		&& Input.Parameter.TransformName != "SimilarityTransform" && Input.Parameter.TransformName != "ThinPlateSplineTransform")
	{
		MDK_Error("TransformName is unknown @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
	}

	if (Input.Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (Input.LandmarkOnShape.GetLength() < 9)
		{
			MDK_Error("too few Landmark for ThinPlateSplineTransform @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
			return false;
		}
	}

    if (Input.Parameter.MaxThreadCount <= 0)
    {
		MDK_Error("MaxThreadCount <= 0 @ BasisSelectionBasedShapeDictionaryBuilder::CheckInput()")
		return false;
    }

    return true;
}


template<typename ScalarType>
void BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	ShapeDictionary<ScalarType> OutputDictionary;
	if (Input.InitialDictionary != nullptr)
	{
		OutputDictionary.Copy(*Input.InitialDictionary);
	}

	int_max TotalDataCount = Input.TrainingShapeData->GetLength();

   //--------------------------------- run Epoch and Minibatch -------------------------------------------------------//

   // random number for sampling
    std::random_device rd;
    std::mt19937 gen(rd());
	DenseVector<int_max> RandomDataIndexList = span(0, TotalDataCount - 1);

	for (int_max iter = 0; iter < Input.Parameter.MaxEpochCount; ++iter)
	{		
		int_max BatchCount = TotalDataCount / Input.Parameter.BatchSize;
		if (BatchCount > 1)
		{
			std::shuffle(RandomDataIndexList.begin(), RandomDataIndexList.end(), gen);
		}
		for (int_max n = 0; n < BatchCount; ++n)
		{
			ObjectArray<DenseMatrix<ScalarType>> ShapeData_CurrentBatch;
			{// sample a subset from Input.TrainingShapeData
				int_max Idx_start = n*Input.Parameter.BatchSize;
				int_max Idx_end = std::min(Idx_start + Input.Parameter.BatchSize - 1, TotalDataCount - 1);
				auto SubSet = RandomDataIndexList.GetSubSet(Idx_start, Idx_end);
				ShapeData_CurrentBatch.Resize(SubSet.GetLength());
				for (int_max k = 0; k < SubSet.GetLength(); ++k)
				{
					ShapeData_CurrentBatch[k].ForceShare((*Input.TrainingShapeData)[SubSet[k]]);
				}
			}
			OutputDictionary = this->BuildDictionaryInMiniBatch(OutputDictionary, ShapeData_CurrentBatch);
		}
	}

    this->UpdateDictionaryInformation_AfterALLEpoch(OutputDictionary, TotalDataCount);
	//------------------------------------
    Output.Dictionary.Copy(std::move(OutputDictionary)); 
}


template<typename ScalarType>
ShapeDictionary<ScalarType> 
BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
BuildDictionaryInMiniBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData)
{
    ShapeDictionary<ScalarType> Dictionary;// the output
    Dictionary.Name() = Input.Parameter.Name;
    int_max BasisCount_desired = Input.Parameter.BasisCount;

    //------------------------------------------- check input ---------------------------------------------------------------//
    //ShapeData is a mini batch from all the traning shape data
	if (ShapeData.IsEmpty() == true)
    {
        MDK_Error("ShapeData is empty @ BasisSelectionBasedShapeDictionaryBuilder::BuildDictionaryInMiniBatch(...)")
        return Dictionary;
    }

    //--------------------------- combine ShapeData and Dictionary_init.Basis to Compute ShapeSimilarityMatrix ---------//
	// just for reference: what is CombinedData
	// CombinedData = { Dictionary_init.Basis(), ShapeData };

    int_max DataCount = ShapeData.GetLength();
	int_max BasisCount_init = Dictionary_init.GetBasisCount(); 
    int_max TotalShapeCount = BasisCount_init + DataCount;

    auto ShapeSimilarityMatrix = this->ComputeShapeSimilarityMatrix(Dictionary_init, ShapeData);

    if (Input.Parameter.Debug_Flag == true)
    {
        String FilePathAndName = Input.Parameter.Debug_FilePath + "ShapeSimilarityMatrix.json";
        SaveDenseMatrixAsJsonDataFile(ShapeSimilarityMatrix, FilePathAndName);
    }

    // ------- Output Dictionary if the number of data samples and input bases <= the number of desired bases -------------------------------------//
    // note: do not re-use data sample, i.e., OutputBasisCount may not = BasisCount_desired

    if (BasisCount_desired >= TotalShapeCount)
    {
        MDK_Warning("BasisCount_desired >= TotalShapeCount @ BasisSelectionBasedShapeDictionaryBuilder::BuildDictionaryInMiniBatch(...)")

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
        this->UpdateDictionaryInformation_AtEachMiniBatch(Dictionary, ShapeData, ShapeSimilarityMatrix, ShapeIndexList_Basis, Dictionary_init);

        return Dictionary;
    }

    //------------------------------------------ select basis from Combined Data -----------------------------------------------------------------//   
    auto ShapeIndexList_Basis = this->SelectBasis(BasisCount_desired, ShapeSimilarityMatrix);
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
            int_max DataIndex_k = ShapeIndex - BasisCount_init;
			Basis[k] = ShapeData[DataIndex_k];
        }
    };
    ParallelForLoop(TempFunction_CreateBasis, 0, OutputBasisCount-1, Input.Parameter.MaxThreadCount);

    // --------------- Update DictionaryInformation --------------------------------------------//
    this->UpdateDictionaryInformation_AtEachMiniBatch(Dictionary, ShapeData, ShapeSimilarityMatrix, ShapeIndexList_Basis, Dictionary_init);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ScalarType>
DenseVector<int_max> BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix)
{
	//---------------------------------------------------------------------
    // Combined data = {Dictionary_init.Basis(), ShapeData}
    //
    // the number of combined data is greater than the number of bases
    // extract a subset from the combined data set to be the bases
	// return the selected shape index list 
	// shape index is the index in Combined data
	//----------------------------------------------------------------------

	int_max TotalShapeCount = ShapeSimilarityMatrix.GetColCount();

	int_max ShapeCountPerBasis = 1 + TotalShapeCount / BasisCount_desired;

	DenseVector<ScalarType> ProbabilityList;
	ProbabilityList.Resize(TotalShapeCount);
	for (int_max k = 0; k < TotalShapeCount; ++k)
	{		
		auto SimilarityList = ShapeSimilarityMatrix.GetCol(k);
		SimilarityList.SortInPlace("descend");
		ProbabilityList[k] = 0;
		for (int_max n = 0; n < ShapeCountPerBasis; ++n)
		{
			ProbabilityList[k] += SimilarityList[n];
		}
		ProbabilityList[k] /= ScalarType(ShapeCountPerBasis);
	}
	auto ShapeIndexList_sort = ProbabilityList.Sort("descend");

	DenseVector<int_max> ShapeIndexList_Basis;	
	ScalarType Similarity_lb = 0;
	ScalarType Similarity_ub = 1;	
	for (int_max iter = 0; iter < 100; ++iter)
	{
		auto SimilarityThreshold = (Similarity_lb + Similarity_ub) / ScalarType(2);
		ShapeIndexList_Basis = this->SelectBasis_By_SimilarityThreshold(ShapeIndexList_sort, ShapeSimilarityMatrix, SimilarityThreshold);
		auto BasisCount_now = ShapeIndexList_Basis.GetLength();
		if (BasisCount_now > BasisCount_desired)
		{
			Similarity_ub = SimilarityThreshold;
		}
		else if (BasisCount_now < BasisCount_desired)
		{
			Similarity_lb = SimilarityThreshold;
		}
		else
		{
			break;
		}
	}
	for (int_max iter = 0; iter <= 10; ++iter)
	{
		auto SimilarityThreshold= Similarity_ub-(ScalarType(iter)/10)*(Similarity_ub- Similarity_lb);
		ShapeIndexList_Basis = this->SelectBasis_By_SimilarityThreshold(ShapeIndexList_sort, ShapeSimilarityMatrix, SimilarityThreshold);
		auto BasisCount_now = ShapeIndexList_Basis.GetLength();
		if (BasisCount_now <= BasisCount_desired)
		{
			break;
		}
	}

	return ShapeIndexList_Basis;   
}


template<typename ScalarType>
DenseVector<int_max> BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::SelectBasis_By_SimilarityThreshold(const DenseVector<int_max>& ShapeIndexList_sort, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix, ScalarType SimilarityThreshold)
{
	int_max TotalShapeCount = ShapeIndexList_sort.GetLength();

	DenseVector<int_max> ShapeFlagList;
	ShapeFlagList.Resize(TotalShapeCount);
	ShapeFlagList.Fill(1);
	//0: not basis
	//1: selected as basis

	for (int_max k = 0; k < TotalShapeCount; ++k)
	{
		auto ShapeIndex_k = ShapeIndexList_sort[k];
		if (ShapeFlagList[ShapeIndex_k] > 0)
		{
			for (int_max n = 0; n < TotalShapeCount; ++n)
			{
				if (n != ShapeIndex_k)
				{
					if (ShapeSimilarityMatrix(ShapeIndex_k, n) > SimilarityThreshold)
					{
						ShapeFlagList[n] = 0;
					}
				}
			}
		}
	}

	auto ShapeIndexList_Basis = ShapeFlagList.Find([](int_max Flag) { return Flag > 0; });
	return ShapeIndexList_Basis;
}



template<typename ScalarType>
DenseMatrix<ScalarType> 
BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
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

    //---------------------------------------------------------------------------------------------

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    //-------------------------------------------------------------------------------------------------------------
    auto TempFunction_AndRandomNumberToSimilarity = [&](ScalarType Similarity)
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
        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalShapeCount - 2, Input.Parameter.MaxThreadCount);
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
        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalShapeCount - 2, Input.Parameter.MaxThreadCount);
    }

    return ShapeSimilarityMatrix;
}


template<typename ScalarType>
ScalarType BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	return ComputeSimilarityBetweenShapeWithPointCorrespondence(ShapeA, ShapeB, Input.LandmarkOnShape, Input.Parameter.TransformName, true);
}


template<typename ScalarType>
void BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_AtEachMiniBatch(ShapeDictionary<ScalarType>& Dictionary,
                                            const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
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

    if (Input.Parameter.Flag_Update_BasisSimilarity == true)
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

    //--------------------- init BasisRedundancy -----------------------------//
	BasisRedundancy.FastResize(1, BasisCount);
	BasisRedundancy.Fill(1);
}



template<typename ScalarType>
void BasisSelectionBasedShapeDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount)
{
	int_max BasisCount = Dictionary.GetBasisCount();

	//--------------------- update BasisID for new basis -------------------------//
	auto& BasisID = Dictionary.BasisID();
    if (Input.Parameter.Flag_Update_BasisID == true)
    {
        for (int_max k = 0; k < BasisCount; ++k)
        {
            if (BasisID[k] == 0)
            {   // this basis is from data
                BasisID[k] = Dictionary.GenerateNewBasisID();
            }
        }
    }
}


}//namespace mdk
