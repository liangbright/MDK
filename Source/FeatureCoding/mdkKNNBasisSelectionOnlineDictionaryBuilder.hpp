#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::KNNBasisSelectionOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::~KNNBasisSelectionOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::CheckInput()
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

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("MaxNumberOfDataInEachBatch > TotalDataNumber @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("MaxNumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L1Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L2Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::Correlation
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("SimilarityThreshold <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_Dictionary->VarianceOfL1Distance().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Variance_L1 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::L2Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_Dictionary->VarianceOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("Variance_L2 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::Correlation)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_Dictionary->VarianceOfKLDivergence().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Variance_KL <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.WeightOnProbabiliyForBasisSelection < 0 || m_Parameter.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThresholdToComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThresholdToComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    if (m_Parameter.MaxNumberOfThreads <= 0)
    {
        MDK_Warning("MaxNumberOfThreads <= 0, set to 1 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.MaxNumberOfThreads = 1;
    }

    return true;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    // compute the total BasisExperience of m_InitialDictionary

    auto TotalExperience_init = ElementType(0);
    if (m_InitialDictionary != nullptr)
    {
        TotalExperience_init = m_InitialDictionary->BasisExperience().Sum();
    }

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    if (m_InitialDictionary != nullptr)
    {
        OutputDictionary.ForceShare(m_InitialDictionary);
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {      
        // process all the data 

        OutputDictionary = this->BuildDictionaryFromData(m_Parameter.BasisNumber, *m_FeatureData, OutputDictionary);
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//
        // do not re-use data

        // random number for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

        DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);
        DataFlagList.Fill(1);
        // 1 : not used yet
        // 0 : used

        DenseMatrix<ElementType> FeatureData_current;

        int_max NumberOfDataInNextBatch = m_Parameter.MaxNumberOfDataInEachBatch;

        int_max TotalDataNumber_used = 0;

        while (true)
        {
            // sample a subset from m_FeatureData

            FeatureData_current.FastResize(m_FeatureData->GetRowNumber(), NumberOfDataInNextBatch);

            int_max SampleCounter = 0;
            while (true)
            {
                auto DataIndex = UniformRandomNumber(gen);
                if (DataFlagList[DataIndex] == 1)
                {
                    FeatureData_current.SetCol(SampleCounter, m_FeatureData->GetElementPointerOfCol(DataIndex));

                    DataFlagList[DataIndex] = 0;

                    SampleCounter += 1;
                    if (SampleCounter >= NumberOfDataInNextBatch)
                    {
                        break;
                    }
                }               
            }

            // update Dictionary
            OutputDictionary = this->BuildDictionaryFromData(m_Parameter.BasisNumber, FeatureData_current, OutputDictionary);
            
            // update NumberOfDataInNextBatch

            TotalDataNumber_used += SampleCounter;

            NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

            if (NumberOfDataInNextBatch <= 0)
            {
                break;
            }
        }
    }

    (*m_Dictionary) = std::move(OutputDictionary);

    //----------------------- just for reference: why not to re-use data in training -------------//
    //
    // BasisAge can not be adjusted : 
    // BasisAge[k] -= TotalDataNumberForTrainning - TotalDataNumber
    // then BasisAge[k] may be < 0
    //
    // BasisExperience can not be adjusted :
    // BasisExperience[k] *ExperienceDiscountFactor may be < 1
    //---------------------------------------------------------------------------------------------------------//   
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
BuildDictionaryFromData(const int_max BasisNumber_desired,
                        const DenseMatrix<ElementType>& FeatureData,
                        const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;
    Dictionary.SetName(m_Parameter.DictionaryName);

    //------------------------------------------- check input ------------------------------------------------------------------------//

    if (BasisNumber_desired <= 0 || FeatureData.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")
        return Dictionary;
    }

    if (Dictionary_init.IsEmpty() == false)
    {
        if (FeatureData.GetRowNumber() != Dictionary_init.BasisMatrix().GetRowNumber())
        {
            MDK_Error("FeatureData.GetRowNumber() != Dictionary_init.GetRowNumber() @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")
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
    // DenseMatrix<ElementType> CombinedData = { &Dictionary_init.BasisMatrix(), &FeatureData };

    auto VectorSimilarityMatrix = this->ComputeVectorSimilarityMatrix(Dictionary_init, FeatureData);

    if (m_Parameter.DebugInfo.Flag_OutputDebugInfo == true)
    {
        CharString FilePathAndName = m_Parameter.DebugInfo.FilePathToSaveDebugInfo + m_Parameter.DebugInfo.JsonDataFileName_Of_VectorSimilarityMatrix;
        SaveDenseMatrixAsJsonDataFile(VectorSimilarityMatrix, FilePathAndName);
    }

    // ------- Output Dictionary if the number of data samples is smaller than the number of desired bases -------------------------------------//

    if (BasisNumber_desired >= TotalVectorNumber)
    {
        MDK_Warning("BasisNumber_desired >= TotalVectorNumber @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")

        DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

        DenseMatrix<ElementType>& BasisAge = Dictionary.BasisAge();
        BasisAge.FastResize(1, TotalVectorNumber);

        DenseMatrix<ElementType>& BasisExperience = Dictionary.BasisExperience(); 
        BasisExperience.FastResize(1, TotalVectorNumber);

        if (Dictionary_init.IsEmpty() == false)
        {
            const DenseMatrix<ElementType>& BasisMatrix_init = Dictionary_init.BasisMatrix();

            BasisNumber_init = BasisMatrix_init.GetColNumber();

            BasisMatrix = { &BasisMatrix_init, &FeatureData };

            BasisAge.Fill(ElementType(DataNumber));
            BasisAge(span(0, BasisNumber_init - 1)) = Dictionary_init.BasisAge();

            BasisExperience.Fill(ElementType(1));
            BasisExperience(span(0, BasisNumber_init - 1)) = Dictionary_init.BasisExperience();
        }
        else
        {
            BasisMatrix = FeatureData;            
            BasisAge.Fill(ElementType(DataNumber));
            BasisExperience.Fill(ElementType(1));
        }

        DenseMatrix<int_max> VectorIndexList_Basis = span(0, BasisMatrix.GetColNumber()-1);

        auto KNNBasisIndexTableOfData = this->FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(VectorSimilarityMatrix, 
                                                                                                   VectorIndexList_Basis, BasisNumber_init);

        this->UpdateDictionaryInformation(Dictionary, FeatureData, KNNBasisIndexTableOfData,
                                          VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

        return Dictionary;
    }

    //------------------------------------------ select basis from Combined Data -----------------------------------------------------------------//

    // find KNN of each vector in Combined Data
    auto KNNVectorIndexTable = FindKNNVectorIndexTableByVectorSimilarityMatrix(VectorSimilarityMatrix);

    auto RepresentativeAbilityOfEachVector = ComputeRepresentativeAbilityOfEachVectorInCombinedData(Dictionary_init, TotalVectorNumber);

    // estimate the probability mass function based on RepresentativeAbilityOfEachVector

    auto ProbabilityOfEachVector = this->EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(VectorSimilarityMatrix,
                                                                                                        KNNVectorIndexTable, 
                                                                                                        RepresentativeAbilityOfEachVector);
    
    DenseMatrix<int_max> VectorIndexList_Basis = this->SelectBasisFromCombinedDataBySimilarityAndProbability(BasisNumber_desired,
                                                                                                             VectorSimilarityMatrix, 
                                                                                                             ProbabilityOfEachVector);

    int_max OutputBasisNumber = VectorIndexList_Basis.GetElementNumber();

    // ------------- create BasisMatrix --------------------------------------------------//

    DenseMatrix<ElementType> BasisMatrix(FeatureData.GetRowNumber(), OutputBasisNumber);

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

    ParallelForLoop(TempFunction_CreateBasis, 0, OutputBasisNumber-1, m_Parameter.MaxNumberOfThreads);

    // ------------- Apply Constraint On Basis --------------------------------------------------//

    this->ApplyConstraintOnBasis(BasisMatrix);
    
    // ----- Write BasisMatrix into Dictionary -----------------------------------------//

    Dictionary.BasisMatrix().Take(BasisMatrix);

    // -------- get KNNBasisVectorIndexTable (i.e., encode FeatureData) -----------------------//

    auto KNNBasisIndexTableOfData = FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(VectorSimilarityMatrix, VectorIndexList_Basis, BasisNumber_init);

    // --------------- Update DictionaryInformation ------------------------------//

    this->UpdateDictionaryInformation(Dictionary, FeatureData, KNNBasisIndexTableOfData, 
                                      VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
SelectBasisFromCombinedDataBySimilarityAndProbability(const int_max BasisNumber_desired,
                                                      const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                      const DenseMatrix<ElementType>& ProbabilityOfEachVector)
{
    // Combined data = [Dictionary_init.BasisMatrix(), FeatureData]
    //
    // the number of combined data is greater than the number of bases
    // extract a subset from the data set to be the bases

    // get VectorPairScoreList ----------------------------------------------------

    int_max TotalVectorNumber = ProbabilityOfEachVector.GetElementNumber();

    int_max NumberOfVectorPair = TotalVectorNumber*(TotalVectorNumber - 1) / 2;

    DenseMatrix<ElementType> VectorPairScoreList(1, NumberOfVectorPair);

    DenseMatrix<int_max> VectorPairIndexList(2, NumberOfVectorPair);

    auto ScoreWeight_S = ElementType(0);
    auto ScoreWeight_PS = ElementType(0);
    auto ScoreWeight_P = ElementType(0);

    if (m_Parameter.WeightOnProbabiliyForBasisSelection <= ElementType(0.5))
    {
        ScoreWeight_S = 1 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_PS = 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P = 0;
    }
    else
    {
        ScoreWeight_S = 0;
        ScoreWeight_PS = 2 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P = 2 * (m_Parameter.WeightOnProbabiliyForBasisSelection - ElementType(0.5));
    }

    int_max Counter = 0;
    for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
    {
        for (int_max n = k + 1; n < TotalVectorNumber; ++n)
        {
            auto tempScore_P = ProbabilityOfEachVector[n] + ProbabilityOfEachVector[k];

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

    DenseMatrix<int_max> VectorFlagList(1, TotalVectorNumber);
    VectorFlagList.Fill(1);
    // FlagList(i) is 0 : vector_i is removed from in the current basis set
    // FlagList(i) is 1 : vector_i is still in the current basis set

    int_max OutputBasisNumber = TotalVectorNumber; // OutputBasisNumber is the number of vectors In Current Basis Set

    for (int_max k = 0; k < NumberOfVectorPair; ++k)
    {
        if (OutputBasisNumber <= BasisNumber_desired)
        {
            break;
        }

        auto Index_a = VectorPairIndexList(0, k);
        auto Index_b = VectorPairIndexList(1, k);

        if (VectorFlagList[Index_a] > 0 && VectorFlagList[Index_b] > 0)
        {
            auto Prob_a = ProbabilityOfEachVector[Index_a];
            auto Prob_b = ProbabilityOfEachVector[Index_b];

            if (Prob_a == Prob_b)
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

            OutputBasisNumber -= 1;
        }
    }

    DenseMatrix<int_max> VectorIndexList_Basis;
    VectorIndexList_Basis.ReserveCapacity(OutputBasisNumber);

    for (int_max k = 0; k < TotalVectorNumber; ++k)
    {
        if (VectorFlagList[k] > 0)
        {
            VectorIndexList_Basis.AppendCol({ k });
        }
    }

    return VectorIndexList_Basis;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeRepresentativeAbilityOfEachVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                       int_max CombinedFeatureDataVectorNumber)
{
    //---------------------------------------------------------------------------------------
    // Input:    
    // CombinedData = [ BasisMatrix of Dictionary_init,  FeatureData]
    //
    // CombinedFeatureDataVectorNumber = InitialBasisNumber + FeatureDataNumber
    //
    // if Dictionary_init is empty, then CombinedData = input FeatureData 
    //
    // Output:
    // RepresentativeAbility is the ability to represent many data vectors
    //
    // 
    // If a data vector k in CombinedData is from Dictionary_init, then it can represent many data vectors in history
    // Therefore, RepresentativeAbility[k] = m_Parameter.weigth_past * the number of data in history that it represents 
    //     
    // If a data vector n in CombinedData is from FeatureData, then it only represents itself
    // Therefore RepresentativeAbility[n] =  (1 - m_Parameter.weigth_past) * 1 (only itself)
    //----------------------------------------------------------------------------------------

    DenseMatrix<ElementType> RepresentativeAbility(1, CombinedFeatureDataVectorNumber);

    RepresentativeAbility.Fill(ElementType(1));

    if (Dictionary_init.IsEmpty() == false)
    {
        auto InitialBasisNumber = Dictionary_init.BasisMatrix().GetColNumber();

        const DenseMatrix<ElementType>& BasisExperience_init = Dictionary_init.BasisExperience();

        for (int_max k = 0; k < InitialBasisNumber; k++)
        {
            RepresentativeAbility[k] = BasisExperience_init[k];
        }

        for (int_max k = InitialBasisNumber; k < CombinedFeatureDataVectorNumber; k++)
        {
            RepresentativeAbility[k] = ElementType(1);
        }
    }

    return RepresentativeAbility;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init, 
                              const DenseMatrix<ElementType>& FeatureData)
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

    DenseMatrix<ElementType> VectorSimilarityMatrix;

    int_max VectorLength = FeatureData.GetRowNumber();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    std::random_device rd_uniform;
    std::mt19937 gen_uniform(rd_uniform());
    std::uniform_real_distribution<ElementType> UniformRandomNumber(0, ElementType(1));

    //-------------------------------------------------------------------------------------------------------------
    auto TempFunction_AndRandomNumberToSimilarity = [&](ElementType Similarity)
    {
        // if Similarity <= SimilarityThreshold and > eps_value
        // add some random noise with abs < SimilarityThreshold
        // then,  data pair with Similarity <= eps_value will be sorted/sampled randomly
        // It is a kind of random sampling
        //
        // if Similarity <= eps_value
        // then set Similarity to zero or eps_value randomly

        if (Similarity <= SimilarityThreshold && Similarity > eps_value)
        {
            auto tempValue = UniformRandomNumber(gen_uniform);

            Similarity *= tempValue;

        }

        if (Similarity <= eps_value)
        {
            bool Flag = BoolRandomNumber(gen_bool);

            if (Flag == true)
            {
                Similarity = ElementType(0);
            }
            else
            {
                Similarity = eps_value;
            }
        }

        return Similarity;
    };

    //--------------------------------------------------------------------------------------------------------------

    if (Dictionary_init.IsEmpty() == true)
    {
        int_max TotalVectorNumber = FeatureData.GetColNumber();

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            auto VectorPtr_k = FeatureData.GetElementPointerOfCol(k);

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                auto VectorPtr_n = FeatureData.GetElementPointerOfCol(n);

                auto Similarity = this->ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength);

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);
             
                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        };

        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalVectorNumber - 2, m_Parameter.MaxNumberOfThreads);
    }
    else
    {
        const DenseMatrix<ElementType>& BasisMatrix_init = Dictionary_init.BasisMatrix();

        int_max BasisNumber_init = BasisMatrix_init.GetColNumber();
        
        const DenseMatrix<ElementType>& SimilarityMatrix_init = Dictionary_init.SimilarityMatrix();

        int_max DataNumber = FeatureData.GetColNumber();

        auto TotalVectorNumber = BasisNumber_init + DataNumber;

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            const ElementType* VectorPtr_k = nullptr;
            const ElementType* VectorPtr_n = nullptr;

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
                if (n < BasisNumber_init)
                {
                    VectorPtr_n = BasisMatrix_init.GetElementPointerOfCol(n);
                }
                else if (n >= BasisNumber_init)
                {
                    int_max temp_n = n - BasisNumber_init;

                    VectorPtr_n = FeatureData.GetElementPointerOfCol(temp_n);
                }

                auto Similarity = ElementType(0);

                if (Dictionary_init.GetProperty_SimilarityType() == m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
                {
                    if (k < BasisNumber_init && n < BasisNumber_init)
                    {
                        Similarity = SimilarityMatrix_init(n, k);
                    }
                    else
                    {
                        Similarity = this->ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength);
                    }
                }
                else
                {
                    Similarity = this->ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength);
                }

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);

                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        };

        ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalVectorNumber - 2, m_Parameter.MaxNumberOfThreads);
    }

    return VectorSimilarityMatrix;
}


template<typename ElementType>
inline
ElementType KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length)
{
    ElementType Similarity = ElementType(0);

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
    {
    case SimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L1Distance*L1Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case SimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L2Distance*L2Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case SimilarityTypeEnum::Correlation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            Similarity = std::abs(Correlation);
        }
        else
        {
            Similarity = (Correlation + ElementType(1)) / ElementType(2);
        }
    }
        break;

    case SimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, false);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, false);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = (KLDivergence*KLDivergence) / m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ KNNBasisSelectionOnlineDictionaryBuilder::ComputeSimilarityBetweenTwoVectors(...)")
    }

    return Similarity;
}


template<typename ElementType>
DataContainer<DenseMatrix<int_max>>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix)
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

    DataContainer<DenseMatrix<int_max>> KNNVectorIndexTable;
    KNNVectorIndexTable.FastResize(TotalDataNumber);

    DenseMatrix<ElementType> SimilarityList(1, TotalDataNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        for (int_max n = 0; n < TotalDataNumber; ++n)
        {
            SimilarityList[n] = VectorSimilarityMatrix(n, k);
        }

        KNNVectorIndexTable[k] = FindKNNBySimilarityList(SimilarityList,
                                                         m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                         m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    }

    return KNNVectorIndexTable;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix, 
                                                               const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable)
{
    //--------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // 
    // Output:
    // Probability is the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    //
    // assume each data vector can be a basis
    // then estimate the PMF of each data vector by counting the number of KNN-neighbors
    //--------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(1)); // KNN does not include self, so add 1 to count self

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            auto VectorIndex_m = KNN_IndexList[m];

            Probability[VectorIndex_m] += VectorSimilarityMatrix(VectorIndex_m, k);
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
DenseMatrix<ElementType>    
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                               const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                               const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector)
{
    //-----------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // RepresentativeAbilityOfEachVector is from this->ComputeRepresentativeAbilityOfEachVectorInCombinedData(...)
    // 
    // Output:
    // Probability is  the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    // 
    // assume each data vector can be a basis
    // then estimate the Representative Ability of each data vector by counting the normalized number of KNN-neighbors
    //
    // this is no self-counting 
    // because KNN does not include self in KNNVectorIndexTable 
    //
    // note: 
    // If a data vector is from initial dictionary, and one of its KNN-neighbors is also from initial dictionary
    // Then, the Representative Ability of this data vector is significantly increased
    // (i.e., it has more neighbors "2*K", compared to a real data vector)
    // Therefore: The estimation accuracy may be decreased
    // But: the previous basis will have a higher chance to be re-selected, and this is good   
    // 
    //-----------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(0)); // do not need to fill 1, self is already counted in RepresentativeAbilityOfEachVector

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            auto VectorIndex_m = KNN_IndexList[m];

            Probability[VectorIndex_m] += RepresentativeAbilityOfEachVector[k] * VectorSimilarityMatrix(VectorIndex_m, k);
        }
    }

    auto SumProb = Probability.Sum();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();
    if (SumProb <= eps_value)
    {
        Probability.Fill(ElementType(1) / ElementType(TotalDataNumber));
    }
    else
    {
        Probability /= SumProb;
    }

    return Probability;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix)
{
    if (m_Parameter.BasisPositive == true)
    {
        for (int_max k = 0; k < BasisMatrix.GetElementNumber(); ++k)
        {
            if (BasisMatrix[k] < ElementType(0))
            {
                BasisMatrix[k] = ElementType(0);
            }
        }
    }

    if (m_Parameter.BasisNormalizedWithL1Norm == true)
    {
        int_max BasisNumber = BasisMatrix.GetColNumber();
        int_max Length = BasisMatrix.GetRowNumber();

        auto tempValueToFill = ElementType(1) / ElementType(Length);

        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        DenseMatrix<ElementType> BasisVector;

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

        auto tempValueToFill = std::sqrt(ElementType(1) / ElementType(Length));

        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        DenseMatrix<ElementType> BasisVector;

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


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                            const DenseMatrix<ElementType>& FeatureData,
                            const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                            const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                            const DenseMatrix<int_max>& VectorIndexList_Basis,
                            const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& BasisAge = Dictionary.BasisAge();

    DenseMatrix<ElementType>& BasisExperience = Dictionary.BasisExperience();

    DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();

    DenseMatrix<ElementType>& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisNumber = Dictionary.BasisMatrix().GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init.IsEmpty() == false)
    { 
        BasisNumber_init = Dictionary_init.BasisMatrix().GetColNumber();
    }

    int_max DataNumber = FeatureData.GetColNumber();

    //------------------- initialize BasisAge ----------------------------------------//

    BasisAge.FastResize(1, BasisNumber);

    BasisAge.Fill(0); // fill 0 for new basis

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<ElementType>& BasisAge_init = Dictionary_init.BasisAge();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init)
            {   // this basis is from Dictionary_init

                BasisAge[k] = BasisAge_init[tempIndex];
            }
        }
    }

    //------------------- update BasisAge ---------------------------------------------------//

    if (m_Parameter.Update_BasisAge == true)
    {
        BasisAge += DataNumber;
    }

    //--------------------- initialize BasisExperience ---------------------------------------//
    // BasisiNumber in updated BasisMatrix may be different than that in Dictionary_init

    BasisExperience.FastResize(1, BasisNumber);

    BasisExperience.Fill(ElementType(1)); // fill 1 for new basis

    if (Dictionary_init.IsEmpty() == false)
    {
        const DenseMatrix<ElementType>& BasisExperience_init = Dictionary_init.BasisExperience();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init) 
            {   // this basis is from Dictionary_init
                BasisExperience[k] = BasisExperience_init[tempIndex];
            }
        }
    }

    //--------------------- update Variance Information ----------------------------//
    // use initial BasisExperience
    //

    if (m_Parameter.Update_Variance == true)
    {
        this->UpdateDictionaryInformation_Variance(Dictionary, FeatureData, KNNBasisIndexTableOfData,
                                                   VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);
    }

    //--------------------- update BasisExperience -----------------------------//

    if (m_Parameter.Update_BasisExperience == true)
    {
        this->UpdateBasisExperience(BasisExperience, KNNBasisIndexTableOfData);
    }

    // ----------- update SimilarityMatrix ------------------------------------//

    if (m_Parameter.Update_SimilarityMatrix == true)
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

        MDK_DebugCode
        (
            CharString FilePathAndName = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/SimilarityMatrix.json";
           SaveDenseMatrixAsJsonDataFile(SimilarityMatrix, FilePathAndName);
        )
    }

    //---------- Update BasisRedundancy --------------------------------------------//

    if (m_Parameter.Update_BasisRedundancy == true)
    {
        this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);

        Dictionary.SetProperty_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& BasisExperience = Dictionary.BasisExperience();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();
    
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
        const DenseMatrix<ElementType>& VarianceOfL1Distance_init = Dictionary_init.VarianceOfL1Distance();
        const DenseMatrix<ElementType>& VarianceOfL2Distance_init = Dictionary_init.VarianceOfL2Distance();
        const DenseMatrix<ElementType>& VarianceOfKLDivergence_init = Dictionary_init.VarianceOfKLDivergence();
        const DenseMatrix<ElementType>& VarianceOfReconstruction_init = Dictionary_init.VarianceOfReconstruction();

        //----- initialize VarianceOfL1Distance 

        auto MeanStd_L1Distance = ElementType(0);
        if (VarianceOfL1Distance_init.IsEmpty() == false)
        {
            MeanStd_L1Distance = VarianceOfL1Distance_init.Mean();
        }
        else
        {
            MeanStd_L1Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
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
                VarianceOfL1Distance[k] = MeanStd_L1Distance;
            }
        }

        //----- initialize VarianceOfL2Distance 

        auto MeanStd_L2Distance = ElementType(0);
        if (VarianceOfL2Distance_init.IsEmpty() == false)
        {
            MeanStd_L2Distance = VarianceOfL2Distance_init.Mean();
        }
        else
        {
            MeanStd_L2Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
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
                VarianceOfL2Distance[k] = MeanStd_L2Distance;
            }
        }

        //----- initialize VarianceOfKLDivergence

        if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
        {
            auto MeanStd_KLDivergence = ElementType(0);
            if (VarianceOfKLDivergence_init.IsEmpty() == false)
            {
                MeanStd_KLDivergence = VarianceOfKLDivergence_init.Mean();
            }
            else
            {
                MeanStd_KLDivergence = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
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
                    VarianceOfKLDivergence[k] = MeanStd_KLDivergence;
                }
            }
        }

        //----- initialize VarianceOfReconstruction

        if (m_Parameter.Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
        {
            auto MeanStd_Reconstruction = ElementType(0);
            if (VarianceOfReconstruction_init.IsEmpty() == false)
            {
                MeanStd_Reconstruction = VarianceOfReconstruction_init.Mean();
            }
            else
            {
                MeanStd_Reconstruction = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
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
                    VarianceOfReconstruction[k] = MeanStd_Reconstruction;
                }
            }
        }
    }
    else// Dictionary_init is empty
    {
        VarianceOfL1Distance.Clear();
        VarianceOfL2Distance.Clear();
        VarianceOfKLDivergence.Clear();
        VarianceOfReconstruction.Clear();
    }

    //----------- update Variance ----------------------------------------------------//

    this->UpdateVarianceOfL1Distance(VarianceOfL1Distance, FeatureData, KNNBasisIndexTableOfData, BasisMatrix, BasisExperience);

    this->UpdateVarianceOfL2Distance(VarianceOfL2Distance, FeatureData, KNNBasisIndexTableOfData, BasisMatrix, BasisExperience);

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(VarianceOfKLDivergence, FeatureData, KNNBasisIndexTableOfData, BasisMatrix, BasisExperience);
    }

    if (m_Parameter.Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
    {
        this->UpdateVarianceOfReconstruction(VarianceOfReconstruction, FeatureData, KNNBasisIndexTableOfData, BasisMatrix, BasisExperience);
    }
}


template<typename ElementType>
DataContainer<DenseMatrix<int_max>>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                     const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                     int_max BasisNumber_init)
{
    //--------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorIndexList_Basis is from this->BuildDictionaryFromData(...)
    // VectorIndexList_Basis[n] is the index of the basis vector in VectorSimilarityMatrix
    //
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix();
    // VectorSimilarityMatrix(i, j) is the similarity between data vector i and data vector j
    // VectorSimilarityMatrix(i, i) = 0 for all i, guaranteed in this->ComputeVectorSimilarityMatrix(...)
    //
    // Output:
    // KNNBasisIndexTableOfData[k] is the basis-index list of KNN-bases of the data vector k
    // the length of KNNBasisIndexTableOfData[k] is from 0 to m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber
    // Index Range in KNNBasisIndexTableOfData[k] is from 0 to BasisNumber - 1
    //----------------------------------------------------------------------------------------------------------------

    int_max BasisNumber = VectorIndexList_Basis.GetElementNumber();

    int_max TotalVectorNumber = VectorSimilarityMatrix.GetColNumber();

    auto DataNumber = TotalVectorNumber - BasisNumber_init;

    DataContainer<DenseMatrix<int_max>> KNNBasisIndexTableOfData;
    KNNBasisIndexTableOfData.FastResize(DataNumber);

    DenseMatrix<ElementType> SimilarityList(1, BasisNumber);

    //for (int_max k = 0; k <= DataNumber-1; ++k)
    auto TempFunction_FindKNN = [&](int_max k)
    {
        SimilarityList.Fill(ElementType(0));

        int_max VectorIndex_Data = BasisNumber_init + k;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            int_max VectorIndex_basis = VectorIndexList_Basis[n];

            SimilarityList[n] = VectorSimilarityMatrix(VectorIndex_basis, VectorIndex_Data);
        }

        KNNBasisIndexTableOfData[k] = FindKNNBySimilarityList(SimilarityList,
                                                              m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                              m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    };

    ParallelForLoop(TempFunction_FindKNN, 0, DataNumber-1, m_Parameter.MaxNumberOfThreads);

    return KNNBasisIndexTableOfData;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateBasisExperience(DenseMatrix<ElementType>& BasisExperience,
                      const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData)
{
    //--------------------------------------------------------------------------------------
    // Input:
    // BasisExperience is initialized
    //
    // KNNBasisIndexTableOfData is from this->FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(...)
    //
    // Output:
    // BasisExperience
    //
    // BasisExperience[n] is estimated by counting the normalized number of KNN data vectors of the basis n
    // this is no double counting, because KNN does not include self in KNNBasisVectorIndexTable 
    //--------------------------------------------------------------------------------------

    int_max BasisNumber = BasisExperience.GetElementNumber();

    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // must >= 1
    for (int_max k = 0; k < BasisNumber; k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }

    int_max DataNumber = KNNBasisIndexTableOfData.GetLength();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        int_max tempNeighbourNumber = KNN_IndexList.GetElementNumber();

        for (int_max m = 0; m < tempNeighbourNumber; ++m)
        {
            BasisExperience[KNN_IndexList[m]] += ElementType(1) / ElementType(tempNeighbourNumber);
        }
    }

    // do NOT use VectorSimilarityMatrix to get SimilarityList and update BasisExperience
    // If BasisExperience is used for KNNReconstruction based dictionary builder
    // Then reconstruction error of each data vector is weighted equally, not by its similarity with some basis
    // BasisExperience is just the weighted number of data vectors in training history

    // the total Experience is
    // BasisExperience.Sum() ~ m_Parameter.ExperienceDiscountFactor * BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
{
    int_max BasisNumber = SimilarityMatrix.GetColNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThresholdToComputeBasisRedundancy;

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

    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience)
{
   int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto L1Distance = ComputeL1DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += L1Distance*L1Distance;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
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

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL1Distance(...)"
                    << '\n' << "set to std::max(eps_value, Variance_L1)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanStd)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

    for (int_max k = 0; k < DataNumber; ++k)
    {        
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += SquaredL2Distance;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
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

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL2Distance(...)"
                    << '\n' << "set to std::max(eps_value, Variance_L2)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanStd)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                             const DenseMatrix<ElementType>& FeatureData,
                             const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                             const DenseMatrix<ElementType>& BasisMatrix,
                             const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += KLDivergence*KLDivergence;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
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

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfKLDivergence(...)"
                     << '\n' << "set to std::max(eps_value, Variance_KL)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanStd)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                               const DenseMatrix<ElementType>& FeatureData,
                               const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                               const DenseMatrix<ElementType>& BasisMatrix,
                               const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

    auto DataReconstructionErrorL2Norm = this->ComputeDataReconstructionErrorL2Norm(FeatureData, KNNBasisIndexTableOfData, BasisMatrix);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {            
            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k];

                CounterList[BasisIndex] += 1;
            }
        }       
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance[n] /= CounterList[n];
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

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfReconstruction(...)"
                     << '\n' << "set to std::max(eps_value, Variance_L2)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= MeanStd)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                     const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                     const DenseMatrix<ElementType>&  BasisMatrix)
{
    int_max DataNumber = KNNBasisIndexTableOfData.GetLength();

    int_max VectorLength = BasisMatrix.GetRowNumber();

    DenseMatrix<ElementType> DataReconstructionErrorL2Norm(1, DataNumber);

    DenseMatrix<ElementType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    //for(int_max DataIndex = 0; DataIndex <= DataNumber - 1; ++DataIndex)
    auto TempFunction_Reconstruction = [&](int_max DataIndex)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[DataIndex];

        if (KNN_IndexList.GetElementNumber() > 0)
        {
            DenseMatrix<ElementType> DataVector;            
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(DataIndex), VectorLength, 1);

            DenseMatrix<ElementType> ReconstructedDataVector(1, VectorLength);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            this->ReconstructDataVectorByKNNBasisMatrix(ReconstructedDataVector, DataVector, KNNBasisMatrix, KNN_IndexList, GramianMatrix_DtD);

            auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVector.GetElementPointer(),
                                                                               ReconstructedDataVector.GetElementPointer(),
                                                                               VectorLength, false);

            DataReconstructionErrorL2Norm[DataIndex] = SquaredL2Distance;
        }
    };

    ParallelForLoop(TempFunction_Reconstruction, 0, DataNumber - 1, m_Parameter.MaxNumberOfThreads);

    return DataReconstructionErrorL2Norm;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector,
                                      const DenseMatrix<ElementType>& DataVector,    
                                      const DenseMatrix<ElementType>& KNNBasisMatrix,
                                      const DenseMatrix<int_max>&     KNNBasisIndexList,
                                      const DenseMatrix<ElementType>& GramianMatrix_DtD)
{
    auto KNNBasisNumber = KNNBasisIndexList.GetElementNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ElementType> Solution;

    typedef LinearLeastSquaresProblemSolver<ElementType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == false && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);
    }
    else if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == true && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == false)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == true && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == true)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ElementType(0));

        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }
    else //if(m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == false && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == true)
    {
        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }

    MatrixMultiply(ReconstructedDataVector, KNNBasisMatrix, Solution.X);
}


}//namespace mdk

#endif