#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp

namespace mdk
{

template<typename ScalarType>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::KNNBasisSelectionOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ScalarType>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::~KNNBasisSelectionOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

	m_Dictionary.Clear();
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>* KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::GetOutputDictionary()
{
    return &m_Dictionary;
}


template<typename ScalarType>
bool KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.BasisNumber > m_FeatureData->GetColNumber())
    {
        MDK_Error("BasisNumber > DataNumber @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_InitialDictionary->IsEmpty() == false)
        {
            if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
            {
                MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("MaxNumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.BasisNumber < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("BasisNumber <  NeighbourNumber @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    auto IsSimilarityTypeSupported = KNNSoftAssignSparseEncoder<ScalarType>::
                                     CheckIfSimilarityTypeSupported(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);    

    if (IsSimilarityTypeSupported == false)
    {
        MDK_Error("SimilarityType is not supported @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("ParameterOfKNNSoftAssign.SimilarityThreshold <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_InitialDictionary->VarianceOfL1Distance().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L1 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_InitialDictionary->VarianceOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L2 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_InitialDictionary->VarianceOfKLDivergence().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_KL <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.WeightOnProbabiliyForBasisSelection < 0 || m_Parameter.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    if (m_Parameter.MaxNumberOfThread <= 0)
    {
        MDK_Warning("MaxNumberOfThread <= 0, set to 1 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.MaxNumberOfThread = 1;
    }

    return true;
}


template<typename ScalarType>
bool KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}
	this->GenerateDictionary();
	return true;
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::GenerateDictionary()
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;

    //-----------------------------------------------------------------------------
    if (m_InitialDictionary != nullptr)
    {
        OutputDictionary = this->PreprocessInitialDictionary(*m_InitialDictionary);
    }

    auto TotalExperience_init = ScalarType(0);
    if (OutputDictionary.BasisExperience().IsEmpty() == false)
    {
        TotalExperience_init = OutputDictionary.BasisExperience().Sum();
    }
    //------------------------------------------------------------------------------

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {      
        // process all the data 

        for (int_max k = 0; k < m_Parameter.MaxNumberOfInteration; ++k)
        {
            OutputDictionary = this->BuildDictionaryFromDataBatch(OutputDictionary, *m_FeatureData);

            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), TotalDataNumber, TotalExperience_init);
        }
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//

        // random number for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

        DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);

        DenseMatrix<ScalarType> FeatureData_CurrentBatch;

        for (int_max k = 0; k < m_Parameter.MaxNumberOfInteration; ++k)
        {
            DataFlagList.Fill(1);
            // 1 : not used yet
            // 0 : used

            auto DataNumberInEachBatch = this->ComputeDataNumberInEachBatch(TotalDataNumber);

            int_max BatchNumber = DataNumberInEachBatch.GetElementNumber();            

            for (int_max n = 0; n < BatchNumber; ++n)
            {
                // sample a subset from m_FeatureData

                FeatureData_CurrentBatch.FastResize(m_FeatureData->GetRowNumber(), DataNumberInEachBatch[n]);

                int_max SampleCounter = 0;
                while (true)
                {
                    auto DataIndex = UniformRandomNumber(gen);
                    if (DataFlagList[DataIndex] == 1)
                    {
                        FeatureData_CurrentBatch.SetCol(SampleCounter, m_FeatureData->GetElementPointerOfCol(DataIndex));

                        DataFlagList[DataIndex] = 0;

                        SampleCounter += 1;
                        if (SampleCounter >= DataNumberInEachBatch[n])
                        {
                            break;
                        }
                    }
                }

                // update Dictionary
                OutputDictionary = this->BuildDictionaryFromDataBatch(OutputDictionary, FeatureData_CurrentBatch);
            }

            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), TotalDataNumber, TotalExperience_init);
        }
    }

    this->UpdateDictionaryInformation_Other(OutputDictionary, TotalDataNumber);
	//------------------------------------
    m_Dictionary.Take(OutputDictionary); 
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitialDictionary)
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;

    if (InitialDictionary.IsEmpty())
    {
        return OutputDictionary;
    }

    OutputDictionary.Copy(InitialDictionary);

    DenseMatrix<ScalarType>& BasisExperience = OutputDictionary.BasisExperience();

    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // must >= 1
    for (int_max k = 0; k < BasisExperience.GetElementNumber(); k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }

    return OutputDictionary;
}


template<typename ScalarType>
DenseMatrix<int_max>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
ComputeDataNumberInEachBatch(int_max TotalDataNumber)
{
    DenseMatrix<int_max> DataNumberInEachBatch(1, 1 + TotalDataNumber / m_Parameter.MaxNumberOfDataInEachBatch);

    int_max BatchNumber = 0;

    int_max NumberOfDataInNextBatch = 0;

    int_max TotalDataNumber_used = 0;

    while (true)
    {
        TotalDataNumber_used += NumberOfDataInNextBatch;

        NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

        if (NumberOfDataInNextBatch <= 0)
        {
            break;
        }

        DataNumberInEachBatch[BatchNumber] = NumberOfDataInNextBatch;
        BatchNumber += 1;
    }

    DataNumberInEachBatch = DataNumberInEachBatch(span(0, BatchNumber - 1));

    return DataNumberInEachBatch;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType> 
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
BuildDictionaryFromDataBatch(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                             const DenseMatrix<ScalarType>& FeatureData)
{
    FeatureDictionaryForSparseCoding<ScalarType> Dictionary;
    Dictionary.SetName(m_Parameter.DictionaryName);

    int_max BasisNumber_desired = m_Parameter.BasisNumber;

    //------------------------------------------- check input ------------------------------------------------------------------------//

    if (BasisNumber_desired <= 0 || FeatureData.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromDataBatch(...)")
        return Dictionary;
    }

    if (Dictionary_init.IsEmpty() == false)
    {
        if (FeatureData.GetRowNumber() != Dictionary_init.BasisMatrix().GetRowNumber())
        {
            MDK_Error("FeatureData.GetRowNumber() != Dictionary_init.GetRowNumber() @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromDataBatch(...)")
            return Dictionary;
        }
    }

    //--------------------------- combine FeatureData and BasisMatrix Of InitialDictionary and Compute VectorSimilarityMatrix -------------//

    int_max DataNumber = FeatureData.GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init.IsEmpty() == false)
    {
        BasisNumber_init = Dictionary_init.BasisMatrix().GetColNumber();
    }

    int_max TotalVectorNumber = BasisNumber_init + DataNumber;

    // just for reference: what is CombinedData
    // DenseMatrix<ScalarType> CombinedData = { &Dictionary_init.BasisMatrix(), &FeatureData };

    auto VectorSimilarityMatrix = this->ComputeVectorSimilarityMatrix(Dictionary_init, FeatureData);

    if (m_Parameter.DebugInfo.Flag_OutputDebugInfo == true)
    {
        std::string FilePathAndName = m_Parameter.DebugInfo.FilePathToSaveDebugInfo + "VectorSimilarityMatrix.json";
        SaveDenseMatrixAsJsonDataFile(VectorSimilarityMatrix, FilePathAndName);
    }

    // ------- Output Dictionary if the number of data samples and input bases <= the number of desired bases -------------------------------------//
    // note: do not re-use data sample, i.e., OutputBasisNumber may not = BasisNumber_desired

    if (BasisNumber_desired >= TotalVectorNumber)
    {
        MDK_Warning("BasisNumber_desired >= TotalVectorNumber @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromDataBatch(...)")

		auto& BasisMatrix = Dictionary.BasisMatrix();

		auto& BasisAge = Dictionary.BasisAge();
        BasisAge.FastResize(1, TotalVectorNumber);
        BasisAge.Fill(0);

        auto& BasisExperience = Dictionary.BasisExperience(); 
        BasisExperience.FastResize(1, TotalVectorNumber);
        BasisExperience.Fill(1);

        if (Dictionary_init.IsEmpty() == false)
        {
            const auto& BasisMatrix_init = Dictionary_init.BasisMatrix();

            BasisNumber_init = BasisMatrix_init.GetColNumber();

            BasisMatrix = { &BasisMatrix_init, &FeatureData };

            BasisAge(span(0, BasisNumber_init - 1)) = Dictionary_init.BasisAge();

            BasisExperience(span(0, BasisNumber_init - 1)) = Dictionary_init.BasisExperience();
        }
        else
        {
            BasisMatrix = FeatureData;
        }

        this->ApplyConstraintOnBasis(BasisMatrix);

        DenseMatrix<int_max> VectorIndexList_Basis = span(0, BasisMatrix.GetColNumber()-1);

        auto CodeTable = this->EncodeFeatureDataBySimilarity(VectorSimilarityMatrix, VectorIndexList_Basis, BasisNumber_init);

        this->UpdateDictionaryInformation(Dictionary, FeatureData, CodeTable,
                                          VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

        return Dictionary;
    }

    //------------------------------------------ select basis from Combined Data -----------------------------------------------------------------//

    // find KNN of each vector in Combined Data
    auto KNNVectorIndexTable = this->FindKNNVectorIndexTableByVectorSimilarityMatrix(VectorSimilarityMatrix);

    auto RepresentativeAbilityOfEachVector_init = this->ComputeInitialRepresentativeAbilityOfEachVector(Dictionary_init, TotalVectorNumber);

    // estimate the probability mass function

    auto ProbabilityOfEachVector = this->EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(VectorSimilarityMatrix,
                                                                                                        KNNVectorIndexTable, 
                                                                                                        RepresentativeAbilityOfEachVector_init);
    
    DenseMatrix<int_max> VectorIndexList_Basis = this->SelectBasis(BasisNumber_desired, VectorSimilarityMatrix, ProbabilityOfEachVector);

    int_max OutputBasisNumber = VectorIndexList_Basis.GetElementNumber();

    // ------------- create BasisMatrix --------------------------------------------------//

    auto& BasisMatrix = Dictionary.BasisMatrix();
     
    BasisMatrix.FastResize(FeatureData.GetRowNumber(), OutputBasisNumber);

    //for (int_max k = 0; k <= OutputBasisNumber-1; ++k)
    auto TempFunction_CreateBasis = [&](int_max k)
    {
        auto VectorIndex = VectorIndexList_Basis[k];
        if (VectorIndex < BasisNumber_init)
        {
            BasisMatrix.SetCol(k, Dictionary_init.BasisMatrix().GetElementPointerOfCol(VectorIndex));
        }
        else
        {
            int_max tempIndex_k = VectorIndex - BasisNumber_init;

            BasisMatrix.SetCol(k, FeatureData.GetElementPointerOfCol(tempIndex_k));
        }
    };

    ParallelForLoop(TempFunction_CreateBasis, 0, OutputBasisNumber-1, m_Parameter.MaxNumberOfThread);

    // Apply Constraint On Basis

    this->ApplyConstraintOnBasis(BasisMatrix);

    // -------- encode FeatureData : code element is Similarity, not membership ------------------//

    auto CodeTable = this->EncodeFeatureDataBySimilarity(VectorSimilarityMatrix, VectorIndexList_Basis, BasisNumber_init);

    // --------------- Update DictionaryInformation --------------------------------------------//

    this->UpdateDictionaryInformation(Dictionary, FeatureData, CodeTable, 
                                      VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ScalarType>
DenseMatrix<int_max>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::SelectBasis(const int_max BasisNumber_desired,
                                                                   const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                                                   const DenseMatrix<ScalarType>& ProbabilityOfEachVector)
{
    // Combined data = [Dictionary_init.BasisMatrix(), FeatureData]
    //
    // the number of combined data is greater than the number of bases
    // extract a subset from the data set to be the bases

    // get VectorPairScoreList ----------------------------------------------------

    int_max TotalVectorNumber = ProbabilityOfEachVector.GetElementNumber();

    int_max NumberOfVectorPair = TotalVectorNumber*(TotalVectorNumber - 1) / 2;

    DenseMatrix<ScalarType> VectorPairScoreList(1, NumberOfVectorPair);

    DenseMatrix<int_max> VectorPairIndexList(2, NumberOfVectorPair);

    auto ScoreWeight_S = ScalarType(0);
    auto ScoreWeight_PS = ScalarType(0);
    auto ScoreWeight_P = ScalarType(0);

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
    for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
    {
        for (int_max n = k + 1; n < TotalVectorNumber; ++n)
        {
            auto tempScore_P = ProbabilityOfEachVector[n] + ProbabilityOfEachVector[k]; // use max ?

            auto tempScore_S = 1 - VectorSimilarityMatrix(n, k);

            auto tempScore_PS = tempScore_P*tempScore_S;

            VectorPairScoreList[Counter] = ScoreWeight_PS * tempScore_PS + ScoreWeight_S * tempScore_S + ScoreWeight_P * tempScore_P;

            VectorPairIndexList(0, Counter) = k;
            VectorPairIndexList(1, Counter) = n;

            Counter += 1;
        }
    }

    DenseMatrix<int_max> tempIndexList_sort;

    Sort(VectorPairScoreList, VectorPairScoreList, tempIndexList_sort, "ascend");

    DenseMatrix<int_max> tempPairIndexList = VectorPairIndexList;

    for (int_max k = 0; k < NumberOfVectorPair; ++k)
    {
        auto tempColIndex = tempIndexList_sort[k];
        VectorPairIndexList(0, k) = tempPairIndexList(0, tempColIndex);
        VectorPairIndexList(1, k) = tempPairIndexList(1, tempColIndex);
    }

    //------------------ select basis by removing redundant vector ---------------------------------//
    // get the data pair vector_a, vector_b with the highest score in the current basis set
    // remove vector_a or vector_b (that has a lower probability) from the current basis set
    // repeat, until the number of vectors in the basis set is m_Parameter.BasisNumber 

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    DenseMatrix<int_max> VectorFlagList(1, TotalVectorNumber);
    VectorFlagList.Fill(1);
    // FlagList(i) is 0 : vector_i is removed from the current basis set
    // FlagList(i) is 1 : vector_i is still in the current basis set

    int_max CurrentBasisNumber = TotalVectorNumber; // CurrentBasisNumber is the number of vectors In Current Basis Set

    for (int_max k = 0; k < NumberOfVectorPair; ++k)
    {
        if (CurrentBasisNumber <= BasisNumber_desired)
        {
            break;
        }

        auto Index_a = VectorPairIndexList(0, k);
        auto Index_b = VectorPairIndexList(1, k);

        if (VectorFlagList[Index_a] > 0 && VectorFlagList[Index_b] > 0)
        {
            auto Prob_a = ProbabilityOfEachVector[Index_a];
            auto Prob_b = ProbabilityOfEachVector[Index_b];

            if (std::abs(Prob_a - Prob_b) <= eps_value)
            {
                bool temp_bool = BoolRandomNumber(gen_bool);
                // randomly discard vector a or vector b
                //if (k % 2 == 0)
                if (temp_bool == true)
                {
                    VectorFlagList[Index_a] = 0;
                }
                else
                {
                    VectorFlagList[Index_b] = 0;
                }
            }
            else if (Prob_a > Prob_b)
            {
                VectorFlagList[Index_b] = 0;
            }
            else// if (Prob_a < Prob_b)
            {
                VectorFlagList[Index_a] = 0;
            }

            CurrentBasisNumber -= 1;
        }
    }

    DenseMatrix<int_max> VectorIndexList_Basis;
    VectorIndexList_Basis.ReserveCapacity(CurrentBasisNumber);

    for (int_max k = 0; k < TotalVectorNumber; ++k)
    {
        if (VectorFlagList[k] > 0)
        {
            VectorIndexList_Basis.AppendCol({ k });
        }
    }

    return VectorIndexList_Basis;
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
ComputeInitialRepresentativeAbilityOfEachVector(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                                                int_max TotalVectorNumber)
{
    //---------------------------------------------------------------------------------------------------
    // Input:    
    // CombinedData = [ BasisMatrix of Dictionary_init,  FeatureData]
    //
    // TotalVectorNumber = BasisNumber_init + DataNumber
    //
    // if Dictionary_init is empty, then CombinedData = input FeatureData 
    //
    // Output:
    // RepresentativeAbility is the ability to represent many data vectors (including self, so it is >= 1)
    //----------------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> RepresentativeAbility(1, TotalVectorNumber);

    RepresentativeAbility.Fill(ScalarType(1));

    if (Dictionary_init.IsEmpty() == false)
    {
        auto BasisNumber_init = Dictionary_init.BasisMatrix().GetColNumber();

        const DenseMatrix<ScalarType>& BasisExperience_init = Dictionary_init.BasisExperience();

        for (int_max k = 0; k < BasisNumber_init; k++)
        {
            RepresentativeAbility[k] = BasisExperience_init[k];
        }

        for (int_max k = BasisNumber_init; k < TotalVectorNumber; k++)
        {
            RepresentativeAbility[k] = ScalarType(1);
        }
    }

    return RepresentativeAbility;
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init, 
                              const DenseMatrix<ScalarType>& FeatureData)
{
    //------------------------------------------------------------------------------------------
    // Input:
    // FeatureData is a set of column data vectors
    // 
    // Output:
    // VectorSimilarityMatrix of Combined data = [Dictionary_init.BasisMatrix(), FeatureData]
    // 
    // ColNumber = RowNumber = the total number of data vectors    
    // VectorSimilarityMatrix(i, j) = VectorSimilarityMatrix(j, i)
    // VectorSimilarityMatrix(i, i) = 0 for all i
    //
    // a pair of vector_i and vector_j is called data pair
    //------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> VectorSimilarityMatrix;

    int_max VectorLength = FeatureData.GetRowNumber();

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    //---------------------------------------------------------------------------------------------
    // get the Variance to compute Similarity

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    auto Variance = ScalarType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance :
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        break;

    case VectorSimilarityTypeEnum::L2Distance:
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
        break;

    default:
        Variance = 0;
    }

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

    if (Dictionary_init.SimilarityMatrix().IsEmpty() == true)
    {
        int_max TotalVectorNumber = FeatureData.GetColNumber();

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ScalarType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            auto VectorPtr_k = FeatureData.GetElementPointerOfCol(k);

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                auto VectorPtr_n = FeatureData.GetElementPointerOfCol(n);

                auto Similarity = KNNSoftAssignSparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                            SimilarityType, Variance, false);

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);
             
                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        };

        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalVectorNumber - 2, m_Parameter.MaxNumberOfThread);
    }
    else
    {
        const DenseMatrix<ScalarType>& BasisMatrix_init = Dictionary_init.BasisMatrix();

        int_max BasisNumber_init = BasisMatrix_init.GetColNumber();
        
        const DenseMatrix<ScalarType>& SimilarityMatrix_init = Dictionary_init.SimilarityMatrix();

        int_max DataNumber = FeatureData.GetColNumber();

        auto TotalVectorNumber = BasisNumber_init + DataNumber;

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ScalarType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            const ScalarType* VectorPtr_k = nullptr;
            
            if (k < BasisNumber_init)
            {
                VectorPtr_k = BasisMatrix_init.GetElementPointerOfCol(k);
            }
            else if (k >= BasisNumber_init)
            {
                int_max temp_k = k - BasisNumber_init;

                VectorPtr_k = FeatureData.GetElementPointerOfCol(temp_k);
            }

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                const ScalarType* VectorPtr_n = nullptr;

                if (n < BasisNumber_init)
                {
                    VectorPtr_n = BasisMatrix_init.GetElementPointerOfCol(n);
                }
                else if (n >= BasisNumber_init)
                {
                    int_max temp_n = n - BasisNumber_init;

                    VectorPtr_n = FeatureData.GetElementPointerOfCol(temp_n);
                }

                auto Similarity = ScalarType(0);

                if (Dictionary_init.GetProperty_SimilarityType() == SimilarityType)
                {
                    if (k < BasisNumber_init && n < BasisNumber_init)
                    {
                        Similarity = SimilarityMatrix_init(n, k);
                    }
                    else
                    {
                        Similarity = KNNSoftAssignSparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                               SimilarityType, Variance, false);
                    }
                }
                else
                {
                    Similarity = KNNSoftAssignSparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                           SimilarityType, Variance, false);
                }

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);

                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        };

        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalVectorNumber - 2, m_Parameter.MaxNumberOfThread);
    }

    return VectorSimilarityMatrix;
}


template<typename ScalarType>
ObjectArray<DenseMatrix<int_max>>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ScalarType>& VectorSimilarityMatrix)
{
    //-------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // 
    // Output:
    // KNNVectorIndexTable[k] is the KNN index list of the vector with index k in VectorSimilarityMatrix
    // Index Range in KNNVectorIndexTable[k] is from 0 to BasisNumber_init + DataNumber -1
    //
    // VectorSimilarityMatrix(i, i) = 0 for all i is guaranteed in this->ComputeVectorSimilarityMatrix(...)
    // Then the data-index of the data vector k is not included in KNNVectorIndexTable[k]
    // i.e., self is not a KNN-neighbor of self
    //--------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = VectorSimilarityMatrix.GetColNumber();

    ObjectArray<DenseMatrix<int_max>> KNNVectorIndexTable;
    KNNVectorIndexTable.FastResize(TotalDataNumber);

    //for (int_max k = 0; k <= TotalDataNumber-1; ++k)
    auto TempFunction_FindKNN = [&](int_max k)
    {
		auto SimilarityList = VectorSimilarityMatrix.RefCol(k); //ForceShare(VectorSimilarityMatrix.GetElementPointerOfCol(k), TotalDataNumber, 1);

		KNNVectorIndexTable[k] = FindKNNBySimilarityList(SimilarityList,
                                                         m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                         m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    };

    ParallelForLoop(TempFunction_FindKNN, 0, TotalDataNumber - 1, m_Parameter.MaxNumberOfThread);

    return KNNVectorIndexTable;
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ScalarType>& VectorSimilarityMatrix, 
                                                               const ObjectArray<DenseMatrix<int_max>>& KNNVectorIndexTable)
{
    //--------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // 
    // Output:
    // Probability is the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    //
    // assume each vector can be a basis
    // then estimate the PMF of each vector by counting the normalized number of KNN-neighbors
    //--------------------------------------------------------------------------------

    int_max TotalVectorNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ScalarType> Probability(1, TotalVectorNumber);

    Probability.Fill(0); // (self represent self is not counted here)
    
    DenseMatrix<ScalarType> MembershipList;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < TotalVectorNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        int_max tempNeighbourNumber = KNN_IndexList.GetElementNumber();

		if (tempNeighbourNumber > 0)
		{
			MembershipList.FastResize(1, tempNeighbourNumber);

			for (int_max m = 0; m < tempNeighbourNumber; ++m)
			{
				auto VectorIndex_m = KNN_IndexList[m];

				MembershipList[m] = VectorSimilarityMatrix(VectorIndex_m, k) + eps_value;
			}

			MembershipList /= MembershipList.Sum();

			for (int_max m = 0; m < tempNeighbourNumber; ++m)
			{
				auto VectorIndex_m = KNN_IndexList[m];

				// the exact value of Similarity is not very accurate
				//Probability[VectorIndex_m] += VectorSimilarityMatrix(VectorIndex_m, k);

				Probability[VectorIndex_m] += MembershipList[m];
			}
		}
    }

	auto ProbSum = Probability.Sum();
	if (ProbSum <= eps_value)
	{
		Probability.Fill(ScalarType(1) / ScalarType(TotalVectorNumber));
	}
	else
	{
		Probability /= ProbSum;
	}

    if (m_Parameter.DebugInfo.Flag_OutputDebugInfo == true)
    {
        std::string FilePathAndName = m_Parameter.DebugInfo.FilePathToSaveDebugInfo + "VectorProbabilityList.json";
        SaveDenseMatrixAsJsonDataFile(Probability, FilePathAndName);
    }

    return Probability;
}


template<typename ScalarType>
DenseMatrix<ScalarType>    
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                                               const ObjectArray<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                               const DenseMatrix<ScalarType>& InitialRepresentativeAbilityOfEachVector)
{
    //-----------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // InitialRepresentativeAbilityOfEachVector is from this->ComputeInitialRepresentativeAbilityOfEachVector(...)
    // 
    // Output:
    // Probability is the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    // 
    // assume each vector can be a basis
    // then estimate the Representative Ability of each vector by counting the normalized number of KNN-neighbors
    //
    // this is no self-counting 
    // because KNN does not include self in KNNVectorIndexTable 
    //
    // note: 
    // If a vector is from initial dictionary, and one of its KNN-neighbors is also from initial dictionary
    // Then, the Representative Ability of this vector is significantly increased
    // (i.e., it has more neighbors "2*K", compared to a real data vector)
    // Therefore: The estimation accuracy may be decreased
    // But: the previous basis will have a higher chance to be re-selected, and this is good   
    // 
    //-----------------------------------------------------------------------------------------------------------------

    int_max TotalVectorNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ScalarType> Probability = InitialRepresentativeAbilityOfEachVector;

    DenseMatrix<ScalarType> MembershipList;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < TotalVectorNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        int_max tempNeighbourNumber = KNN_IndexList.GetElementNumber();

		if (tempNeighbourNumber > 0)
		{
			MembershipList.FastResize(1, tempNeighbourNumber);
			for (int_max m = 0; m < tempNeighbourNumber; ++m)
			{
				MembershipList[m] = VectorSimilarityMatrix(KNN_IndexList[m], k) + eps_value;
			}
			MembershipList /= MembershipList.Sum();

			for (int_max m = 0; m < tempNeighbourNumber; ++m)
			{
				auto VectorIndex_m = KNN_IndexList[m];

				// method_0
				//Probability[VectorIndex_m] += InitialRepresentativeAbilityOfEachVector[k] * VectorSimilarityMatrix(VectorIndex_m, k);

				// method_1
				//Probability[VectorIndex_m] += InitialRepresentativeAbilityOfEachVector[k];

				// method_2
				//Probability[VectorIndex_m] += InitialRepresentativeAbilityOfEachVector[k] * MembershipList[m];

				// method_3
				Probability[VectorIndex_m] += MembershipList[m];
			}
		}
    }

	auto ProbSum = Probability.Sum();
	if (ProbSum <= eps_value)
	{
		Probability.Fill(ScalarType(1) / ScalarType(TotalVectorNumber));
	}
	else
	{
		Probability /= ProbSum;
	}

    if (m_Parameter.DebugInfo.Flag_OutputDebugInfo == true)
    {
        std::string FilePathAndName = m_Parameter.DebugInfo.FilePathToSaveDebugInfo + "VectorProbabilityList_with_init.json";
        SaveDenseMatrixAsJsonDataFile(Probability, FilePathAndName);
    }

    return Probability;
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix)
{
    if (m_Parameter.BasisPositive == true)
    {
        for (int_max k = 0; k < BasisMatrix.GetElementNumber(); ++k)
        {
            if (BasisMatrix[k] < ScalarType(0))
            {
                BasisMatrix[k] = ScalarType(0);
            }
        }
    }

    if (m_Parameter.BasisNormalizedWithL1Norm == true)
    {
        int_max BasisNumber = BasisMatrix.GetColNumber();
        int_max Length = BasisMatrix.GetRowNumber();

        auto tempValueToFill = ScalarType(1) / ScalarType(Length);

        auto eps_value = std::numeric_limits<ScalarType>::epsilon();

        DenseMatrix<ScalarType> BasisVector;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            BasisVector.Share(BasisMatrix.GetElementPointerOfCol(k), Length, 1);

            auto tempL1Norm = BasisVector.L1Norm();
            if (tempL1Norm <= eps_value)
            {
                BasisVector.Fill(tempValueToFill);
            }
            else
            {
                BasisVector /= tempL1Norm;
            }
        }
    }
    else if (m_Parameter.BasisNormalizedWithL2Norm == true)
    {
        int_max BasisNumber = BasisMatrix.GetColNumber();
        int_max Length = BasisMatrix.GetRowNumber();

        auto tempValueToFill = std::sqrt(ScalarType(1) / ScalarType(Length));

        auto eps_value = std::numeric_limits<ScalarType>::epsilon();

        DenseMatrix<ScalarType> BasisVector;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            BasisVector.Share(BasisMatrix.GetElementPointerOfCol(k), Length, 1);

            auto tempL2Norm = BasisVector.L2Norm();
            if (tempL2Norm <= eps_value)
            {
                BasisVector.Fill(tempValueToFill);
            }
            else
            {
                BasisVector /= tempL2Norm;
            }
        }
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, int_max DataNumber, ScalarType TotalExperience_init)
{
    // adjust the experience of each new basis

    int_max BasisNumber = BasisExperience.GetElementNumber();

    ScalarType TotalExperience = BasisExperience.Sum();

    if (TotalExperience > TotalExperience_init + DataNumber)
    {
        auto factor = ScalarType(TotalExperience_init + DataNumber) / TotalExperience;

        BasisExperience *= factor;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            if (BasisExperience[k] < 1)
            {
                BasisExperience[k] = 1;
            }
        }
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                            const DenseMatrix<ScalarType>& FeatureData,
                            const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                            const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                            const DenseMatrix<int_max>& VectorIndexList_Basis,
                            const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init)
{
    // BasisMatrix has been updated
    DenseMatrix<ScalarType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<int_max>& BasisID = Dictionary.BasisID();

    DenseMatrix<ScalarType>& BasisAge = Dictionary.BasisAge();

    DenseMatrix<ScalarType>& BasisExperience = Dictionary.BasisExperience();

    DenseMatrix<ScalarType>& SimilarityMatrix = Dictionary.SimilarityMatrix();

    DenseMatrix<ScalarType>& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisNumber = Dictionary.BasisMatrix().GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init.BasisMatrix().IsEmpty() == false)
    { 
        BasisNumber_init = Dictionary_init.BasisMatrix().GetColNumber();
    }

    int_max DataNumber = FeatureData.GetColNumber();

    //---------------------- copy SeedForNewBasisIDGeneration ---------------------------//
    
    if (Dictionary_init.IsEmpty() == false)
    {
        Dictionary.SetCurrentSeedForNewBasisIDGeneration(Dictionary_init.GetCurrentSeedForNewBasisIDGeneration());
    }

    //--------------------- initialize BasisID --------------------------------------------//

    BasisID.FastResize(1, BasisNumber);
    BasisID.Fill(0); // BasisID is always > 0, fill 0 to mark new basis

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<int_max>& BasisID_init = Dictionary_init.BasisID();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init)
            {   // this basis is from Dictionary_init

                BasisID[k] = BasisID_init[tempIndex];
            }
        }
    }

    //------------------- initialize BasisAge ----------------------------------------//

    BasisAge.FastResize(1, BasisNumber);

    BasisAge.Fill(0); // fill 0 for new basis

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<ScalarType>& BasisAge_init = Dictionary_init.BasisAge();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init)
            {   // this basis is from Dictionary_init

                BasisAge[k] = BasisAge_init[tempIndex];
            }
        }
    }

    //--------------------- initialize BasisExperience ---------------------------------------//
    // BasisiNumber in updated BasisMatrix may be different than that in Dictionary_init

    BasisExperience.FastResize(1, BasisNumber);

    BasisExperience.Fill(ScalarType(1)); // fill 1 for new basis

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<ScalarType>& BasisExperience_init = Dictionary_init.BasisExperience();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init) 
            {   // this basis is from Dictionary_init
                BasisExperience[k] = BasisExperience_init[tempIndex];
            }
        }
    }

    // ----------- update SimilarityMatrix ------------------------------------//

    if (m_Parameter.Flag_Update_SimilarityMatrix == true)
    {
        SimilarityMatrix.FastResize(BasisNumber, BasisNumber);
        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto VectorIndex_k = VectorIndexList_Basis[k];

            for (int_max n = 0; n < BasisNumber; ++n)
            {
                auto VectorIndex_n = VectorIndexList_Basis[n];

                auto Similarity = VectorSimilarityMatrix(VectorIndex_n, VectorIndex_k);

                SimilarityMatrix(n, k) = Similarity;
                SimilarityMatrix(k, n) = Similarity;
            }
        }

        Dictionary.SetProperty_SimilarityType(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);
    }

    //---------------- initialize BasisRedundancy -----------------------------------//
   
    BasisRedundancy.FastResize(1, BasisNumber);
    BasisRedundancy.Fill(1);

    //--------------------- update Variance Information ----------------------------//
    // use initial BasisExperience
    
    this->UpdateDictionaryInformation_Variance(Dictionary, FeatureData, CodeTable,
                                               VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);
    

    //--------------------- update BasisExperience -----------------------------//

    this->UpdateBasisExperience(BasisExperience, CodeTable);
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, int_max DataNumber)
{
    DenseMatrix<int_max>& BasisID = Dictionary.BasisID();

    DenseMatrix<ScalarType>& BasisAge = Dictionary.BasisAge();

    DenseMatrix<ScalarType>& SimilarityMatrix = Dictionary.SimilarityMatrix();

    DenseMatrix<ScalarType>& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisNumber = BasisID.GetElementNumber();

    //--------------------- update BasisID for new basis --------------------------------------------//

    if (m_Parameter.Flag_Update_BasisID == true)
    {
        for (int_max k = 0; k < BasisNumber; ++k)
        {
            if (BasisID[k] == 0)
            {
                // this basis is from data

                BasisID[k] = Dictionary.GenerateNewBasisID();
            }
        }
    }

    //------------------- update BasisAge ---------------------------------------------------//

    if (m_Parameter.Flag_Update_BasisAge == true)
    {
        BasisAge += DataNumber;
    }

    //---------- Update BasisRedundancy --------------------------------------------//

    if (m_Parameter.Flag_Update_BasisRedundancy == true)
    {
        this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);

        Dictionary.SetProperty_SimilarityThresholdForComputeBasisRedundancy(m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy);
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                     const DenseMatrix<ScalarType>& FeatureData,
                                     const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                     const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init)
{
    DenseMatrix<ScalarType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ScalarType>& BasisExperience = Dictionary.BasisExperience();

    DenseMatrix<ScalarType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ScalarType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ScalarType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ScalarType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();
    
    int_max BasisNumber = BasisMatrix.GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init.IsEmpty() == false)
    {
        BasisNumber_init = Dictionary_init.BasisMatrix().GetColNumber();
    }

    //---------- initialize Variance --------------------------------------------//

    VarianceOfL1Distance.FastResize(1, BasisNumber);
    VarianceOfL2Distance.FastResize(1, BasisNumber);
    VarianceOfKLDivergence.FastResize(1, BasisNumber);
    VarianceOfReconstruction.FastResize(1, BasisNumber);

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<ScalarType>& VarianceOfL1Distance_init = Dictionary_init.VarianceOfL1Distance();
        const DenseMatrix<ScalarType>& VarianceOfL2Distance_init = Dictionary_init.VarianceOfL2Distance();
        const DenseMatrix<ScalarType>& VarianceOfKLDivergence_init = Dictionary_init.VarianceOfKLDivergence();
        const DenseMatrix<ScalarType>& VarianceOfReconstruction_init = Dictionary_init.VarianceOfReconstruction();

        //----- initialize VarianceOfL1Distance 

        auto MeanVar_L1Distance = ScalarType(0);
        if (VarianceOfL1Distance_init.IsEmpty() == false)
        {
            MeanVar_L1Distance = VarianceOfL1Distance_init.Mean();
        }
        else
        {
            MeanVar_L1Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        }

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto VectorIndex_k = VectorIndexList_Basis[k];

            if (VectorIndex_k < BasisNumber_init)
            {
                VarianceOfL1Distance[k] = VarianceOfL1Distance_init[VectorIndex_k];
            }
            else
            {
                VarianceOfL1Distance[k] = MeanVar_L1Distance;
            }
        }

        //----- initialize VarianceOfL2Distance 

        auto MeanVar_L2Distance = ScalarType(0);
        if (VarianceOfL2Distance_init.IsEmpty() == false)
        {
            MeanVar_L2Distance = VarianceOfL2Distance_init.Mean();
        }
        else
        {
            MeanVar_L2Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
        }

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto VectorIndex_k = VectorIndexList_Basis[k];

            if (VectorIndex_k < BasisNumber_init)
            {
                VarianceOfL2Distance[k] = VarianceOfL2Distance_init[VectorIndex_k];
            }
            else
            {
                VarianceOfL2Distance[k] = MeanVar_L2Distance;
            }
        }

        //----- initialize VarianceOfKLDivergence

        if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
        {
            auto MeanVar_KLDivergence = ScalarType(0);
            if (VarianceOfKLDivergence_init.IsEmpty() == false)
            {
                MeanVar_KLDivergence = VarianceOfKLDivergence_init.Mean();
            }
            else
            {
                MeanVar_KLDivergence = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
            }

            for (int_max k = 0; k < BasisNumber; ++k)
            {
                auto VectorIndex_k = VectorIndexList_Basis[k];

                if (VectorIndex_k < BasisNumber_init)
                {
                    VarianceOfKLDivergence[k] = VarianceOfKLDivergence_init[VectorIndex_k];
                }
                else
                {
                    VarianceOfKLDivergence[k] = MeanVar_KLDivergence;
                }
            }
        }

        //----- initialize VarianceOfReconstruction

        if (m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
        {
            auto MeanVar_Reconstruction = ScalarType(0);
            if (VarianceOfReconstruction_init.IsEmpty() == false)
            {
                MeanVar_Reconstruction = VarianceOfReconstruction_init.Mean();
            }
            else
            {
                MeanVar_Reconstruction = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
            }

            for (int_max k = 0; k < BasisNumber; ++k)
            {
                auto VectorIndex_k = VectorIndexList_Basis[k];

                if (VectorIndex_k < BasisNumber_init)
                {
                    VarianceOfReconstruction[k] = VarianceOfReconstruction_init[VectorIndex_k];
                }
                else
                {
                    VarianceOfReconstruction[k] = MeanVar_Reconstruction;
                }
            }
        }
    }
    else// Dictionary_init is empty
    {
        VarianceOfL1Distance.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);
        VarianceOfL2Distance.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
        VarianceOfKLDivergence.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);
        VarianceOfReconstruction.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    //----------- update Variance ----------------------------------------------------//

    if (m_Parameter.Flag_Update_Variance == true)
    {
        this->UpdateVarianceOfL1Distance(VarianceOfL1Distance, FeatureData, CodeTable, BasisMatrix, BasisExperience);

        this->UpdateVarianceOfL2Distance(VarianceOfL2Distance, FeatureData, CodeTable, BasisMatrix, BasisExperience);

        if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
        {
            this->UpdateVarianceOfKLDivergence(VarianceOfKLDivergence, FeatureData, CodeTable, BasisMatrix, BasisExperience);
        }

        if (m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
        {
            this->UpdateVarianceOfReconstruction(VarianceOfReconstruction, FeatureData, CodeTable, BasisMatrix, BasisExperience);
        }
    }
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>> 
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
EncodeFeatureDataBySimilarity(const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                              const DenseMatrix<int_max>&    VectorIndexList_Basis,
                              int_max BasisNumber_init)
{
    int_max BasisNumber = VectorIndexList_Basis.GetElementNumber();

    int_max TotalVectorNumber = VectorSimilarityMatrix.GetColNumber();

    auto DataNumber = TotalVectorNumber - BasisNumber_init;

    ObjectArray<SparseVector<ScalarType>> CodeTable;
    CodeTable.FastResize(DataNumber);

    //for (int_max k = 0; k <= DataNumber-1; ++k)
    auto TempFunction_EncodeDataVector = [&](int_max k)
    {
        DenseMatrix<ScalarType> SimilarityList(1, BasisNumber);
        SimilarityList.Fill(ScalarType(0));

        int_max VectorIndex_Data = BasisNumber_init + k;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            int_max VectorIndex_basis = VectorIndexList_Basis[n];

            SimilarityList[n] = VectorSimilarityMatrix(VectorIndex_basis, VectorIndex_Data);
        }

        auto KNNBasisIndexList = FindKNNBySimilarityList(SimilarityList,
                                                         m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                         m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
       
        auto tempNeighbourNumber = KNNBasisIndexList.GetElementNumber();

        if (tempNeighbourNumber > 0)
        {            
            DenseMatrix<ScalarType> KNNBasisSimilarityList;
            KNNBasisSimilarityList.FastResize(1, tempNeighbourNumber);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                KNNBasisSimilarityList[m] = SimilarityList[KNNBasisIndexList[m]];
            }

            CodeTable[k].Construct(KNNBasisIndexList, KNNBasisSimilarityList, BasisNumber);
        }
    };

    ParallelForLoop(TempFunction_EncodeDataVector, 0, DataNumber - 1, m_Parameter.MaxNumberOfThread);

    return CodeTable;
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{
    //--------------------------------------------------------------------------------------
    // Input:
    // BasisExperience is initialized
    //
    // CodeTable is from this->EncodeFeatureData(...)
    //
    // Output:
    // BasisExperience
    //
    // BasisExperience[n] is estimated by counting the weighted-number/membership of KNN data vectors of the basis n
    //--------------------------------------------------------------------------------------

    int_max BasisNumber = BasisExperience.GetElementNumber();

    int_max DataNumber = CodeTable.GetLength();

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    DenseMatrix<ScalarType> Membership;

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Similarity = CodeTable[k].ElementList();

        auto tempNeighbourNumber = KNN_IndexList.GetLength();

        if (tempNeighbourNumber > 0)
        {
            Membership  = KNN_Similarity;
            Membership /= Membership.Sum() + eps_value;

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                BasisExperience[KNN_IndexList[m]] += Membership[m];
            }
        }
    }

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix)
{
    int_max BasisNumber = SimilarityMatrix.GetColNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy;

    //for (int_max k = 0; k <= BasisNumber-1; ++k)
    auto TempFunction_UpdateRedundancy = [&](int_max k)
    {
        BasisRedundancy[k] = 0;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            if (k != n)
            {
                if (SimilarityMatrix(n, k) >= SimilarityThreshold)
                {
                    BasisRedundancy[k] += SimilarityMatrix(n, k);
                }
            }
        }
    };

    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThread);
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateVarianceOfL1Distance(DenseMatrix<ScalarType>& Variance,
                           const DenseMatrix<ScalarType>& FeatureData,
                           const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                           const DenseMatrix<ScalarType>& BasisMatrix,
                           const DenseMatrix<ScalarType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ScalarType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);

    DenseMatrix<ScalarType> WeightList(1, BasisNumber);
    WeightList.Fill(1);

    DenseMatrix<ScalarType> Membership;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
		const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Similarity = CodeTable[k].ElementList();

        auto tempNeighbourNumber = KNN_IndexList.GetLength();

        if (tempNeighbourNumber > 0)
        {
            Membership  = KNN_Similarity;
            Membership /= Membership.Sum() + eps_value;

            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto L1Distance = ComputeL1DistanceBetweenTwoVector(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                //Variance_current[BasisIndex] += L1Distance*L1Distance*Membership[m];
                //WeightList[BasisIndex] += Membership[m];

                Variance_current[BasisIndex] += L1Distance*L1Distance;
                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanVar = Variance.Mean();

    if (MeanVar <= eps_value)
    {
        MDK_Warning("MeanVar <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL1Distance(...)"
                    << '\n' << "set to std::max(eps_value, ParameterOfKNNSoftAssign.Variance_L1)")

        MeanVar = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanVar)
        {
            Variance[n] = MeanVar;
        }
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateVarianceOfL2Distance(DenseMatrix<ScalarType>& Variance,
                           const DenseMatrix<ScalarType>& FeatureData,
                           const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                           const DenseMatrix<ScalarType>& BasisMatrix,
                           const DenseMatrix<ScalarType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ScalarType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<ScalarType> WeightList(1, BasisNumber);
    WeightList.Fill(1);

    DenseMatrix<ScalarType> Membership;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
		const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Similarity = CodeTable[k].ElementList();

        auto tempNeighbourNumber = KNN_IndexList.GetLength();

        if (tempNeighbourNumber > 0)
        {
            Membership  = KNN_Similarity;
            Membership /= Membership.Sum() + eps_value;

            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVector(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                //Variance_current[BasisIndex] += SquaredL2Distance*Membership[m];
                //WeightList[BasisIndex] += Membership[m];

                Variance_current[BasisIndex] += SquaredL2Distance;
                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanVar = Variance.Mean();

    if (MeanVar <= eps_value)
    {
        MDK_Warning("MeanVar <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL2Distance(...)"
                    << '\n' << "set to std::max(eps_value, ParameterOfKNNSoftAssign.Variance_L2)");

        MeanVar = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanVar)
        {
            Variance[n] = MeanVar;
        }
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ScalarType>& Variance,
                             const DenseMatrix<ScalarType>& FeatureData,
                             const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                             const DenseMatrix<ScalarType>& BasisMatrix,
                             const DenseMatrix<ScalarType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ScalarType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);

    DenseMatrix<ScalarType> WeightList(1, BasisNumber);
    WeightList.Fill(1);

    DenseMatrix<ScalarType> Membership;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
		const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Similarity = CodeTable[k].ElementList();

        auto tempNeighbourNumber = KNN_IndexList.GetLength();

        if (tempNeighbourNumber > 0)
        {
            Membership  = KNN_Similarity;
            Membership /= Membership.Sum() + eps_value;

            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                //Variance_current[BasisIndex] += KLDivergence*KLDivergence*Membership[m];
                //WeightList[BasisIndex] += Membership[m];

                Variance_current[BasisIndex] += KLDivergence*KLDivergence;
                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanVar = Variance.Mean();

    if (MeanVar <= eps_value)
    {
        MDK_Warning("MeanVar <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfKLDivergence(...)"
                     << '\n' << "set to std::max(eps_value, ParameterOfKNNSoftAssign.Variance_KL)");

        MeanVar = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanVar)
        {
            Variance[n] = MeanVar;
        }
    }
}


template<typename ScalarType>
void KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
UpdateVarianceOfReconstruction(DenseMatrix<ScalarType>& Variance,
                               const DenseMatrix<ScalarType>& FeatureData,
                               const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                               const DenseMatrix<ScalarType>& BasisMatrix,
                               const DenseMatrix<ScalarType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    auto DataReconstructionErrorL2Norm = this->ComputeDataReconstructionErrorL2Norm(FeatureData, CodeTable, BasisMatrix);

    DenseMatrix<ScalarType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<ScalarType> WeightList(1, BasisNumber);
    WeightList.Fill(1);

    DenseMatrix<ScalarType> Membership;

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
		const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Similarity = CodeTable[k].ElementList();

        auto tempNeighbourNumber = KNN_IndexList.GetLength();

        if (tempNeighbourNumber > 0)
        {
            Membership  = KNN_Similarity;
            Membership /= Membership.Sum() + eps_value;

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                //Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k] * Membership[m];
                //WeightList[BasisIndex] += Membership[m];

                Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k];
                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance[n] /= WeightList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanVar = Variance.Mean();

    if (MeanVar <= eps_value)
    {
        MDK_Warning("MeanVar <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfReconstruction(...)"
                     << '\n' << "set to std::max(eps_value, ParameterOfKNNSoftAssign.Variance_L2)")

        MeanVar = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanVar)
        {
            Variance[n] = MeanVar;
        }
    }
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNBasisSelectionOnlineDictionaryBuilder<ScalarType>::
ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                     const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                     const DenseMatrix<ScalarType>&  BasisMatrix)
{
    auto ReconstructionCodeSet = KNNReconstructionSparseEncoder<ScalarType>::
                                 ComputeReconstructionCodeFromSimilarityCode(FeatureData,
                                                                             CodeTable,
                                                                             BasisMatrix,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne,
                                                                             m_Parameter.MaxNumberOfThread);

    auto ErrorL2NormList = KNNReconstructionSparseEncoder<ScalarType>::
                           ComputeReconstructionErrorL2Norm(FeatureData, ReconstructionCodeSet, BasisMatrix, m_Parameter.MaxNumberOfThread);

    return ErrorL2NormList;
}


}//namespace mdk

#endif