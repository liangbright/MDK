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
        if (m_FeatureData->GetRowNumber() != m_InputDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InputDictionary @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
            return false;
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

    if (m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
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


    //---------------------------------------------------------------------------------

    if (m_Parameter.MaxNumberOfNewBases < 0)
    {
        MDK_Error("MaxNumberOfNewBases < 0 @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
            return false;
    }

    if (m_Parameter.MaxNumberOfNewBases <= 0 && IsInputDictionaryEmpty == true)
    {
        MDK_Error("MaxNumberOfNewBases <= 0 and InputDictionary is empty @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
            return false;
    }

    //-------------------------

    return true;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::GenerateDictionary()
{
    // check InputDictionary
    bool IsInputDictionaryEmpty = true;

    if (m_InputDictionary != nullptr)
    {
        if (m_InputDictionary->IsEmpty() == false)
        {
            IsInputDictionaryEmpty = false;
        }
    }

    if (IsInputDictionaryEmpty == true)
    {
        m_KNNBasisSelectionDictionaryBuilder.Clear();
        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();
        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        m_Dictionary->Take(*m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary());
    }
    else
    {
        // encode data, code element is similarity, not membership
        m_KNNSimilaritySparseEncoder.Clear();
        m_KNNSimilaritySparseEncoder.m_Parameter.NeighbourNumber = m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber;
        m_KNNSimilaritySparseEncoder.m_Parameter.SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;
        m_KNNSimilaritySparseEncoder.SetInputFeatureData(m_FeatureData);
        m_KNNSimilaritySparseEncoder.SetInputDictionary(m_InputDictionary);
        m_KNNSimilaritySparseEncoder.Update();
        auto CodeTable = m_KNNSimilaritySparseEncoder.GetOutputCodeInSparseColVectorSet();

        // perform SimilarityThreshold based Classification

        DenseMatrix<int_max> IndexList_InClass;
        DenseMatrix<int_max> IndexList_OutClass;
        this->ClassifyFeatureDataBySimilarityThreshold(IndexList_InClass, IndexList_OutClass, *CodeTable);

        if (IndexList_OutClass.GetElementNumber() == 0)
        {
            MDK_Warning("All Feature Data is classified as InClass, no need to generate new basis")
            m_Dictionary->Copy(m_InputDictionary);
            return;
        }

        // select new basis from data classified as OutClass

        DenseMatrix<ElementType> FeatureData_OutClass = m_FeatureData->GetSubMatrix(ALL, IndexList_OutClass);

        m_KNNBasisSelectionDictionaryBuilder.Clear();

        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

        m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.MaxNumberOfNewBases;

        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData_OutClass);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        auto NewDicitonaryFromOutClass = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

        int_max BasisNumber_new = NewDicitonaryFromOutClass->BasisMatrix().GetColNumber();

        int_max BasisNumber_input = m_InputDictionary->BasisMatrix().GetColNumber();

        int_max BasisNumber_to_keep = m_Parameter.BasisNumber - BasisNumber_new;

        // check if we need to retire old basis of input dictionary

        FeatureDictionaryForSparseCoding<ElementType> InputDictionary_Modified;

        if (BasisNumber_to_keep >= BasisNumber_input) // keep all the bases of input dictionary
        {
            InputDictionary_Modified.Copy(m_InputDictionary);
        }
        else// if (BasisNumber_to_keep < BasisNumber_input)
        {            
            if (m_Parameter.DirectlyRetireOldAndInexperiencedBasis == true)
            {               
                this->DirectlyRetireBasisInInputDictionary(InputDictionary_Modified, *m_InputDictionary, BasisNumber_to_keep);
            }
            else
            {
                DenseMatrix<ElementType> FeatureData_InClass = m_FeatureData->GetSubMatrix(ALL, IndexList_InClass);

                // select basis from data classified as InClass

                m_KNNBasisSelectionDictionaryBuilder.Clear();

                m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

                m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = BasisNumber_to_keep;

                m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData_InClass);
                m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
                m_KNNBasisSelectionDictionaryBuilder.Update();

                InputDictionary_Modified.Take(*m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary());
            }
        }

        // In InputDictionary_Modified, 
        // check if SimilarityType and SimilarityThresholdToComputeBasisRedundancy are the same
        // if not, SimilarityMatrix and BasisRedundancy need to be updated   

        this->UpdateDictionaryInformationInModifiedInputDictionary(InputDictionary_Modified);

        this->CombineModifiedInputDictionaryAndNewDictionary(*m_Dictionary, InputDictionary_Modified, *NewDicitonaryFromOutClass);
    }

}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
ClassifyFeatureDataBySimilarityThreshold(DenseMatrix<int_max>& IndexList_InClass,
                                         DenseMatrix<int_max>& IndexList_OutClass,
                                         const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    int_max DataNumber = CodeTable.GetLength();

    IndexList_InClass.FastResize(0, 0);
    IndexList_InClass.ReserveCapacity(1, DataNumber);

    IndexList_OutClass.FastResize(0, 0);
    IndexList_OutClass.ReserveCapacity(1, DataNumber);

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
            IndexList_InClass.AppendCol({ n });
        }
        else
        {
            IndexList_OutClass.AppendCol({ n });
        }
    }

    IndexList_InClass.Squeeze();

    IndexList_OutClass.Squeeze();
}


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


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
UpdateDictionaryInformationInModifiedInputDictionary(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary_Modified)
{
    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    if (InputDictionary_Modified.GetProperty_SimilarityType() != SimilarityType)
    {
        // update SimilarityMatrix

        DenseMatrix<ElementType> BasisMatrix = InputDictionary_Modified.BasisMatrix();

        DenseMatrix<ElementType> SimilarityMatrix = InputDictionary_Modified.SimilarityMatrix();

        DenseMatrix<ElementType> VarianceList;

        switch (SimilarityType)
        {
        case VectorSimilarityTypeEnum::L1Distance:
            VarianceList.Share(InputDictionary_Modified.VarianceOfL1Distance());
            break;

        case VectorSimilarityTypeEnum::L2Distance:
            VarianceList.Share(InputDictionary_Modified.VarianceOfL2Distance());
            break;

        case VectorSimilarityTypeEnum::KLDivergence:
            VarianceList.Share(InputDictionary_Modified.VarianceOfKLDivergence());
            break;

        default:
            MDK_Error("unknown SimilarityType @ KNNBasisSelectionBasedInitialDictionaryBuilder::UpdateDictionaryInformationInModifiedInputDictionary(...)")
            return;
        }

        this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix, VarianceList);
    }

    // update BasisRedundancy ---------------------------------------------------------------------------

    if (InputDictionary_Modified.GetProperty_SimilarityType() != SimilarityType
        || InputDictionary_Modified.GetProperty_SimilarityThresholdForComputingBasisRedundancy() != m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy)
    {
        DenseMatrix<ElementType> BasisRedundancy = InputDictionary_Modified.BasisRedundancy();

        DenseMatrix<ElementType> SimilarityMatrix = InputDictionary_Modified.SimilarityMatrix();

        this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);
    }

    InputDictionary_Modified.SetProperty_SimilarityType(SimilarityType);

    InputDictionary_Modified.SetProperty_SimilarityThresholdForComputingBasisRedundancy(m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy);
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix,
                       const DenseMatrix<ElementType>& BasisMatrix,
                       const DenseMatrix<ElementType>& VarianceList)
{
    int_max BasisNumber = BasisMatrix.GetColNumber();
    int_max VectorLength = BasisMatrix.GetRowNumber();

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    SimilarityMatrix.FastResize(BasisNumber, BasisNumber);

    //for (int_max k = 0; k <= BasisNumber - 2; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
    {
        auto BasisVectorPtr_k = BasisMatrix.GetElementPointerOfCol(k);

        for (int_max n = k + 1; n < BasisNumber; ++n) // start from k+1
        {
            auto BasisVectorPtr_n = BasisMatrix.GetElementPointerOfCol(n);

            auto Variance = VarianceList[k] + VarianceList[n] / ElementType(2);

            auto Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(BasisVectorPtr_k, BasisVectorPtr_n, VectorLength,
                                                                                                          SimilarityType, Variance, false);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber - 2, m_Parameter.MaxNumberOfThreads);
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

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy;

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
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
CombineModifiedInputDictionaryAndNewDictionary(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary,
                                               const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
                                               const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary)
{
    // check input
    if (InputDictionary.IsEmpty() || NewDictionary.IsEmpty())
    {
        MDK_Error("InputDictionary is empty or NewDictionary is empty" << '\n'
                  << "@ KNNBasisSelectionBasedInitialDictionaryBuilder::CombineInputDictionaryAndNewDictionary(...)")
        return;
    }

    CombinedDictionary.Copy(InputDictionary);

    CombinedDictionary.CombineDictionary(NewDictionary);

    // update SimilarityMatrix in combined dictionary

    auto BasisNumber_input = InputDictionary.BasisMatrix().GetColNumber();

    this->UpdateSimilarityMatrixInCombinedDictionary(CombinedDictionary, BasisNumber_input);
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
UpdateSimilarityMatrixInCombinedDictionary(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max BasisNumber_input)
{
    DenseMatrix<ElementType> BasisMatrix_combined = CombinedDictionary.BasisMatrix();

    DenseMatrix<ElementType> SimilarityMatrix_combined = CombinedDictionary.SimilarityMatrix();

    int_max VectorLength = BasisMatrix_combined.GetRowNumber();

    int_max BasisNumber_combined = BasisMatrix_combined.GetColNumber();

    int_max BasisNumber_new = BasisNumber_combined - BasisNumber_input;

    //---------------------------------------------------------------------------------------------
    // get the Variance to compute Similarity

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    auto Variance = ElementType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;

    case VectorSimilarityTypeEnum::L2Distance:
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;

    case VectorSimilarityTypeEnum::KLDivergence:
        Variance = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;

    default:
        Variance = 0;
    }
    //----------------------------------------------------------------------------------------

    //for (int_max k = 0; k <= BasisNumber_input-1; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
    {
        auto VectorPtr_input = BasisMatrix_combined.GetElementPointerOfCol(k);

        for (int_max n = 0; n < BasisNumber_new; ++n)
        {
            auto tempIndex = BasisNumber_input + k;

            auto VectorPtr_new = BasisMatrix_combined.GetElementPointerOfCol(tempIndex);

            auto Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorPtr_input, VectorPtr_new, VectorLength,
                                                                                                          SimilarityType, Variance, false);

            SimilarityMatrix_combined(tempIndex, k);
            SimilarityMatrix_combined(k, tempIndex);
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber_input - 1, m_Parameter.MaxNumberOfThreads);
}


}//namespace mdk

#endif