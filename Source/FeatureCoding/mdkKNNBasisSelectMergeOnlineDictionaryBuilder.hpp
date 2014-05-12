#ifndef __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::KNNBasisSelectMergeOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::~KNNBasisSelectMergeOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::ClearPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    m_InitialDictionary = InputDictionary;
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNBasisSelectMergeOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter;
    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InitialDictionary);

    if (m_KNNBasisSelectionDictionaryBuilder.CheckInput() == false)
    {
        m_KNNBasisSelectionDictionaryBuilder.Clear();

        return false;
    }

    m_KNNBasisSelectionDictionaryBuilder.Clear();

    //-------------------------

    return true;
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    //-----------------------------------------------------------------------------
    if (m_InitialDictionary != nullptr)
    {
        OutputDictionary = this->PreprocessInitialDictionary(*m_InitialDictionary);
    }

    auto TotalExperience_init = ElementType(0);
    if (OutputDictionary.BasisExperience().IsEmpty() == false)
    {
        TotalExperience_init = OutputDictionary.BasisExperience().Sum();
    }
    //------------------------------------------------------------------------------

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {     
        for (int_max k = 0; k < m_Parameter.MaxNumberOfInterations; ++k)
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

        auto DataNumberInEachBatch = this->ComputeDataNumberInEachBatch(TotalDataNumber);

        int_max BatchNumber = DataNumberInEachBatch.GetElementNumber();

        DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);

        DenseMatrix<ElementType> FeatureData_CurrentBatch;

        for (int_max k = 0; k < m_Parameter.MaxNumberOfInterations; ++k)
        {
            DataFlagList.Fill(1);
            // 1 : not used yet
            // 0 : used

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
                this->BuildDictionaryFromDataBatch(OutputDictionary, FeatureData_CurrentBatch);
            }

            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), TotalDataNumber, TotalExperience_init);
        }
    }

    this->UpdateDictionaryInformation_Other(OutputDictionary, m_FeatureData->GetColNumber());

    m_Dictionary->Take(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InitialDictionary)
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    if (InitialDictionary.IsEmpty())
    {
        return OutputDictionary;
    }

    OutputDictionary.Copy(InitialDictionary);

    DenseMatrix<ElementType>& BasisExperience = OutputDictionary.BasisExperience();

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


template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
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


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
BuildDictionaryFromDataBatch(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init, const DenseMatrix<ElementType>& FeatureData)
{
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 1;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_BasisAge = 0;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_BasisID = 0;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_BasisRedundancy = 0;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_SimilarityMatrix = 1;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_Variance = 0;

    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(nullptr);
    m_KNNBasisSelectionDictionaryBuilder.Update();

    auto NewDictionary = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

    if (Dictionary_init.IsEmpty() == true)
    {
        return *NewDictionary;
    }
    else
    {
        return this->CombineInitalDictionaryAndNewDictionary(Dictionary_init, *NewDictionary);
    }
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
CombineInitalDictionaryAndNewDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
                                        const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary)
{
    auto VectorSimilarityMatrix = this->ComputeVectorSimilarityMatrix(InputDictionary, NewDictionary.BasisMatrix());

    auto VectorProbabilityList = this->ComputeVectorProbabilityList(InputDictionary.BasisExperience(), NewDictionary.BasisExperience());

    auto SelectedVectorIndexList = this->SelectBasis(m_Parameter.BasisNumber, VectorSimilarityMatrix, VectorProbabilityList);

    auto BasisNumber_input = InputDictionary.BasisMatrix().GetColNumber();

    auto BasisNumber_new = NewDictionary.BasisMatrix().GetColNumber();

    auto BasisNumber_selected = SelectedVectorIndexList.GetElementNumber();

    DenseMatrix<int_max> SelectedBasisIndexList_input;
    SelectedBasisIndexList_input.ReserveCapacity(BasisNumber_input);

    DenseMatrix<int_max> SelectedBasisIndexList_new;
    SelectedBasisIndexList_new.ReserveCapacity(BasisNumber_new);

    for (int_max k = 0; k < BasisNumber_selected; ++k)
    {
        auto VectorIndex = SelectedVectorIndexList[k];

        if (VectorIndex < BasisNumber_input)
        {
            SelectedBasisIndexList_input.AppendCol({ VectorIndex });
        }
        else
        {
            SelectedBasisIndexList_new.AppendCol({ VectorIndex - BasisNumber_input });
        }
    }

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    if (SelectedBasisIndexList_input.IsEmpty() == false)
    {
        OutputDictionary = InputDictionary.GetSubDictionary(SelectedBasisIndexList_input);

        if (SelectedBasisIndexList_new.IsEmpty() == false)
        {
            OutputDictionary.CombineDictionary(NewDictionary.GetSubDictionary(SelectedBasisIndexList_new));
        }
    }
    else
    {
        OutputDictionary= NewDictionary.GetSubDictionary(SelectedBasisIndexList_new);
    }

    // Update SimilarityMatrix In OutputDictionary 

    DenseMatrix<ElementType>& SimilarityMatrix = OutputDictionary.SimilarityMatrix();
    SimilarityMatrix.FastResize(BasisNumber_selected, BasisNumber_selected);

    for (int_max k = 0; k < BasisNumber_selected - 1; ++k)
    {
        auto Index_k = SelectedVectorIndexList[k];

        for (int_max n = k + 1; k < BasisNumber_selected; ++k)
        {
            auto Index_n = SelectedVectorIndexList[n];

            auto Simlarity = VectorSimilarityMatrix(Index_n, Index_k);

            SimilarityMatrix(n, k) = Simlarity;

            SimilarityMatrix(n, k) = Simlarity;
        }
    }

    return OutputDictionary;
}


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
AdjustBasisExperience(DenseMatrix<ElementType>& BasisExperience, int_max DataNumber, ElementType TotalExperience_init)
{
    // adjust the experience of each new basis

    int_max BasisNumber = BasisExperience.GetElementNumber();

    ElementType TotalExperience = BasisExperience.Sum();

    if (TotalExperience > TotalExperience_init + DataNumber)
    {
        auto factor = ElementType(TotalExperience_init + DataNumber) / TotalExperience;

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


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max DataNumber)
{
    // update BasisRedundancy if required

    if (m_Parameter.Flag_Update_BasisRedundancy)
    {
        this->UpdateBasisRedundancy(CombinedDictionary.BasisRedundancy(), CombinedDictionary.SimilarityMatrix());
    }

    // update BasisID if required

    if (m_Parameter.Flag_Update_BasisID)
    {
        DenseMatrix<int_max>& BasisID = CombinedDictionary.BasisID();

        int_max BasisNumber = BasisID.GetElementNumber();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            if (BasisID[k] == 0)
            {
                BasisID[k] = CombinedDictionary.GenerateNewBasisID();
            }
        }
    }

    // update BasisAge if required

    if (m_Parameter.Flag_Update_BasisAge)
    {
        CombinedDictionary.BasisAge() += DataNumber;
    }
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init, 
                              const DenseMatrix<ElementType>& BasisMatrix_new)
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

    int_max VectorLength = BasisMatrix_new.GetRowNumber();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    //---------------------------------------------------------------------------------------------
    // get the Variance to compute Similarity

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    auto Variance = ElementType(0);

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
    std::uniform_real_distribution<ElementType> UniformRandomNumber(0, ElementType(1));

    //-------------------------------------------------------------------------------------------------------------
    auto TempFunction_AndRandomNumberToSimilarity = [&](ElementType Similarity)
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

        if (Similarity < ElementType(0))
        {
            Similarity = ElementType(0);
        }
        else if (Similarity > ElementType(1))
        {
            Similarity = ElementType(1);
        }

        return Similarity;
    };

    //--------------------------------------------------------------------------------------------------------------

    if (Dictionary_init.SimilarityMatrix().IsEmpty() == true)
    {
        int_max TotalVectorNumber = BasisMatrix_new.GetColNumber();

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            auto VectorPtr_k = BasisMatrix_new.GetElementPointerOfCol(k);

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                auto VectorPtr_n = BasisMatrix_new.GetElementPointerOfCol(n);

                auto Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                              SimilarityType, Variance, false);

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

        int_max DataNumber = BasisMatrix_new.GetColNumber();

        auto TotalVectorNumber = BasisNumber_init + DataNumber;

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
        auto TempFunction_ComputeSimilarity = [&](int_max k)
        {
            const ElementType* VectorPtr_k = nullptr;
            
            if (k < BasisNumber_init)
            {
                VectorPtr_k = BasisMatrix_init.GetElementPointerOfCol(k);
            }
            else if (k >= BasisNumber_init)
            {
                int_max temp_k = k - BasisNumber_init;

                VectorPtr_k = BasisMatrix_new.GetElementPointerOfCol(temp_k);
            }

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                const ElementType* VectorPtr_n = nullptr;

                if (n < BasisNumber_init)
                {
                    VectorPtr_n = BasisMatrix_init.GetElementPointerOfCol(n);
                }
                else if (n >= BasisNumber_init)
                {
                    int_max temp_n = n - BasisNumber_init;

                    VectorPtr_n = BasisMatrix_new.GetElementPointerOfCol(temp_n);
                }

                auto Similarity = ElementType(0);

                if (Dictionary_init.GetProperty_SimilarityType() == SimilarityType)
                {
                    if (k < BasisNumber_init && n < BasisNumber_init)
                    {
                        Similarity = SimilarityMatrix_init(n, k);
                    }
                    else
                    {
                        Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                                 SimilarityType, Variance, false);
                    }
                }
                else
                {
                    Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                             SimilarityType, Variance, false);
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
DenseMatrix<ElementType> 
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
ComputeVectorProbabilityList(const DenseMatrix<ElementType>& BasisExperience_init,
                             const DenseMatrix<ElementType>& BasisExperience_new)
{
    int_max BasisNumber_init = BasisExperience_init.GetColNumber();

    int_max BasisNumber_new = BasisExperience_new.GetColNumber();

    int_max VectorNumber = BasisNumber_init + BasisNumber_new;

    DenseMatrix<ElementType> ProbabilityList(1, VectorNumber);

    auto TotalExperience = BasisExperience_init.Sum() + BasisExperience_new.Sum();

    ProbabilityList(span(0, BasisNumber_init - 1)) = BasisExperience_init;

    ProbabilityList(span(BasisNumber_init, VectorNumber - 1)) = BasisExperience_new;

    ProbabilityList /= TotalExperience;

    return ProbabilityList;
}


template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
SelectBasis(int_max BasisNumber_desired,
            const DenseMatrix<ElementType>& VectorSimilarityMatrix,
            const DenseMatrix<ElementType>& VectorProbabilityList)
{
    int_max TotalVectorNumber = VectorProbabilityList.GetElementNumber();

    int_max NumberOfVectorPair = TotalVectorNumber*(TotalVectorNumber - 1) / 2;

    DenseMatrix<ElementType> VectorPairScoreList(1, NumberOfVectorPair);

    DenseMatrix<int_max> VectorPairIndexList(2, NumberOfVectorPair);

    auto ScoreWeight_S = ElementType(0);
    auto ScoreWeight_PS = ElementType(0);
    auto ScoreWeight_P = ElementType(0);

    if (m_Parameter.WeightOnProbabiliyForBasisSelection <= ElementType(0.5))
    {
        ScoreWeight_S  = 1 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_PS = 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P  = 0;
    }
    else
    {
        ScoreWeight_S  = 0;
        ScoreWeight_PS = 2 - 2 * m_Parameter.WeightOnProbabiliyForBasisSelection;
        ScoreWeight_P  = 2 * (m_Parameter.WeightOnProbabiliyForBasisSelection - ElementType(0.5));
    }

    int_max Counter = 0;
    for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
    {
        for (int_max n = k + 1; n < TotalVectorNumber; ++n)
        {
            auto tempScore_P = VectorProbabilityList[n] + VectorProbabilityList[k];

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

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

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
            auto Prob_a = VectorProbabilityList[Index_a];
            auto Prob_b = VectorProbabilityList[Index_b];

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


template<typename ElementType>
void KNNBasisSelectMergeOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
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

    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThreads);
}


}//namespace mdk

#endif