#ifndef __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::KNNBasisSelectionBasedInitialDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::~KNNBasisSelectionBasedInitialDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InputDictionary = nullptr;

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::ClearPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    m_InputDictionary = InputDictionary;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionBasedInitialDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInputDictionaryEmpty = true;

    if (m_InputDictionary != nullptr)
    {
        if (m_InputDictionary->IsEmpty() == false)
        {
            if (m_FeatureData->GetRowNumber() != m_InputDictionary->BasisMatrix().GetRowNumber())
            {
                MDK_Error("RowNumber Of FeatureData != RowNumber Of InputDictionary @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
                return false;
            }
        }

        IsInputDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InputDictionary->VarianceOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_InputDictionary->VarianceOfL1Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L1 <= 0 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
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

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InputDictionary->VarianceOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_InputDictionary->VarianceOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L2 <= 0 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
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

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InputDictionary->VarianceOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_InputDictionary->VarianceOfKLDivergence().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_KL <= 0 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.WeightOnProbabiliyForBasisSelection < 0 || m_Parameter.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    if (m_Parameter.MaxNumberOfThreads <= 0)
    {
        MDK_Warning("MaxNumberOfThreads <= 0, set to 1 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")

        m_Parameter.MaxNumberOfThreads = 1;
    }

    // ------------ check input to m_KNNBasisSelectionDictionaryBuilder -----------//

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();
    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);

    if (m_KNNBasisSelectionDictionaryBuilder.CheckInput() == false)
    {
        m_KNNBasisSelectionDictionaryBuilder.Clear();

        return false;
    }

    m_KNNBasisSelectionDictionaryBuilder.Clear();

    //-------------------------

    //-------------------------

    return true;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::GenerateDictionary()
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    // check InputDictionary ----------------------------------------------------------
    bool IsInputDictionaryEmpty = true;

    if (m_InputDictionary != nullptr)
    {
        if (m_InputDictionary->IsEmpty() == false)
        {            
            OutputDictionary = this->CopyDictionaryAndDiscountBasisExperience(*m_InputDictionary);

            IsInputDictionaryEmpty = false;
        }
    }

    auto TotalExperience_init = ElementType(0);
    if (IsInputDictionaryEmpty == false)
    {
        TotalExperience_init = m_InputDictionary->BasisExperience().Sum();
    }

    // build the first dictionary
    if (IsInputDictionaryEmpty == true)
    {
        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

        m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.BasisNumber;

        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(nullptr);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        OutputDictionary = this->CopyDictionaryAndDiscountBasisExperience(*m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary());
    }

    //---------------------------------------------------------------------------------

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {
        this->UpdateDictionary(OutputDictionary, *m_FeatureData);
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

        for (int_max k = 0; k < m_Parameter.MaxNumberOfInterations; ++k)
        {
            DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);
            DataFlagList.Fill(1);
            // 1 : not used yet
            // 0 : used

            DenseMatrix<ElementType> FeatureData_current;

            for (int_max n = 0; n < BatchNumber; ++n)
            {
                // sample a subset from m_FeatureData

                FeatureData_current.FastResize(m_FeatureData->GetRowNumber(), DataNumberInEachBatch[n]);

                int_max SampleCounter = 0;
                while (true)
                {
                    auto DataIndex = UniformRandomNumber(gen);
                    if (DataFlagList[DataIndex] == 1)
                    {
                        FeatureData_current.SetCol(SampleCounter, m_FeatureData->GetElementPointerOfCol(DataIndex));

                        DataFlagList[DataIndex] = 0;

                        SampleCounter += 1;
                        if (SampleCounter >= DataNumberInEachBatch[n])
                        {
                            break;
                        }
                    }
                }

                // update Dictionary
                this->UpdateDictionary(OutputDictionary, FeatureData_current);
            }

            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), TotalDataNumber, TotalExperience_init);
        }
    }

    this->UpdateDictionary_OtherInformation(OutputDictionary, m_FeatureData->GetColNumber());

    m_Dictionary->Take(OutputDictionary);
}


template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const DenseMatrix<ElementType>& FeatureData)
{
    // note: SimilarityThreshold is very hard to set
    /*
    // encode data, code element is similarity, not membership

     m_KNNSimilaritySparseEncoder.m_Parameter.NeighbourNumber = m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber;
     m_KNNSimilaritySparseEncoder.m_Parameter.SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;
     m_KNNSimilaritySparseEncoder.SetInputFeatureData(&FeatureData);
     m_KNNSimilaritySparseEncoder.SetInputDictionary(&Dictionary);
     m_KNNSimilaritySparseEncoder.Update();

     auto CodeTable = m_KNNSimilaritySparseEncoder.GetOutputCodeInSparseColVectorSet();

    
    // perform SimilarityThreshold based Classification

    DenseMatrix<int_max> DataIndexList_InClass;
    DenseMatrix<int_max> DataIndexList_OutClass;
    this->ClassifyFeatureDataBySimilarityThreshold(DataIndexList_InClass, DataIndexList_OutClass, *CodeTable);

    if (DataIndexList_OutClass.GetElementNumber() == 0)
    {
        return;
    }

    // select new basis from data classified as OutClass

    int_max BasisNumber_input = Dictionary.BasisMatrix().GetColNumber();

    DenseMatrix<ElementType> FeatureData_OutClass = FeatureData.GetSubMatrix(ALL, DataIndexList_OutClass);
    */

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.BasisNumber;

    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(nullptr);
    m_KNNBasisSelectionDictionaryBuilder.Update();

    auto NewDictionary = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

    // release memory 
   // CodeTable->Clear();
    //FeatureData_OutClass.Clear();

    Dictionary = this->CombineInputDictionaryAndNewDictionary(Dictionary, *NewDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
CopyDictionaryAndDiscountBasisExperience(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary)
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    OutputDictionary.Copy(Dictionary); // Dictionary is not empty

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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
ClassifyFeatureDataBySimilarityThreshold(DenseMatrix<int_max>& DataIndexList_InClass,
                                         DenseMatrix<int_max>& DataIndexList_OutClass,
                                         const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    int_max DataNumber = CodeTable.GetLength();

    DataIndexList_InClass.FastResize(0, 0);
    DataIndexList_InClass.ReserveCapacity(1, DataNumber);

    DataIndexList_OutClass.FastResize(0, 0);
    DataIndexList_OutClass.ReserveCapacity(1, DataNumber);

    for (int_max n = 0; n < DataNumber; ++n)
    {
        const std::vector<ElementType>& KNN_Similarity = CodeTable[n].DataArray();

        auto tempNeighbourNumber = int_max(KNN_Similarity.size());

        auto tempSimilarity = ElementType(0);

        if (tempNeighbourNumber > 0)
        {
            for (int_max k = 0; k < tempNeighbourNumber; ++k)
            {
                tempSimilarity = std::max(tempSimilarity, KNN_Similarity[k]);
            }
        }

        if (tempSimilarity >= m_Parameter.SimilarityThreshold_For_Classification)
        {
            DataIndexList_InClass.AppendCol({ n });
        }
        else
        {
            DataIndexList_OutClass.AppendCol({ n });
        }
    }

    DataIndexList_InClass.Squeeze();

    DataIndexList_OutClass.Squeeze();
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
CombineInputDictionaryAndNewDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
                                       const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary)
{
    auto VectorSimilarityMatrix = this->ComputeVectorSimilarityMatrix(InputDictionary.BasisMatrix(), NewDictionary.BasisMatrix());

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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max DataNumber)
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
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
ComputeVectorSimilarityMatrix(const DenseMatrix<ElementType>& BasisMatrix_input,
                              const DenseMatrix<ElementType>& BasisMatrix_new)
{
    int_max BasisNumber_input = BasisMatrix_input.GetColNumber();

    int_max BasisNumber_new = BasisMatrix_new.GetColNumber();

    int_max TotalVectorNumber = BasisNumber_input + BasisNumber_new;

    int_max VectorLength = BasisMatrix_input.GetRowNumber();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    //---------------------------------------------------------------------------------------------
    // get the Variance to compute Similarity

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    auto Variance = ElementType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
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

    DenseMatrix<ElementType> VectorSimilarityMatrix(TotalVectorNumber, TotalVectorNumber);

    VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

    //for(int_max k = 0; k <= TotalVectorNumber - 2; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
    {
        const ElementType* VectorPtr_k;

        if (k < BasisNumber_input)
        {
            VectorPtr_k = BasisMatrix_input.GetElementPointerOfCol(k);
        }
        else
        {
            VectorPtr_k = BasisMatrix_new.GetElementPointerOfCol(k - BasisNumber_input);
        }

        for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
        {
            const ElementType* VectorPtr_n;

            if (n < BasisNumber_input)
            {
                VectorPtr_n = BasisMatrix_input.GetElementPointerOfCol(n);
            }
            else
            {
                VectorPtr_n = BasisMatrix_new.GetElementPointerOfCol(n - BasisNumber_input);
            }

            auto Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorPtr_k, VectorPtr_n, VectorLength,
                                                                                                          SimilarityType, Variance, false);

            Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);

            VectorSimilarityMatrix(k, n) = Similarity;

            VectorSimilarityMatrix(n, k) = Similarity;
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, TotalVectorNumber - 2, m_Parameter.MaxNumberOfThreads);

    return VectorSimilarityMatrix;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
ComputeVectorProbabilityList(const DenseMatrix<ElementType>& BasisExperience_input,
                             const DenseMatrix<ElementType>& BasisExperience_new)
{
    int_max BasisNumber_input = BasisExperience_input.GetColNumber();

    int_max BasisNumber_new = BasisExperience_new.GetColNumber();

    int_max VectorNumber = BasisNumber_input + BasisNumber_new;

    DenseMatrix<ElementType> ProbabilityList(1, VectorNumber);

    auto TotalExperience = BasisExperience_input.Sum() + BasisExperience_new.Sum();

    ProbabilityList(span(0, BasisNumber_input - 1)) = BasisExperience_input;

    ProbabilityList(span(BasisNumber_input, VectorNumber - 1)) = BasisExperience_new;

    ProbabilityList /= TotalExperience;

    return ProbabilityList;
}


template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
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


/*
template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
DirectlyRetireBasisInInputDictionary(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary_Modified,
const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
int_max BasisNumber_to_keep)
{
// check input
if (InputDictionary.IsEmpty() == true)
{
MDK_Error("InputDictionary is empty @ KNNBasisSelectionBasedInitialDictionaryBuilder::DirectlyRetireBasisInInputDictionary(...)")
return;
}

int_max BasisNumber_input = InputDictionary.BasisMatrix().GetColNumber();

// check input
if (BasisNumber_to_keep >= BasisNumber_input)
{
MDK_Error("asisNumber_to_keep >= BasisNumber_input @ KNNBasisSelectionBasedInitialDictionaryBuilder::DirectlyRetireBasisInInputDictionary(...)")
return;
}

const DenseMatrix<ElementType>& BasisAge = InputDictionary.BasisAge();
const DenseMatrix<ElementType>& BasisExperience = InputDictionary.BasisExperience();

DenseMatrix<ElementType> Ratio = BasisAge / BasisExperience;

DenseMatrix<int_max> IndexList_sort;

Sort(Ratio, Ratio, IndexList_sort, "descend");

DenseMatrix<int_max> IndexList_to_keep = IndexList_sort(span(0, BasisNumber_to_keep - 1));

InputDictionary_Modified = InputDictionary.GetSubDictionary(IndexList_to_keep);
}
*/

}//namespace mdk

#endif