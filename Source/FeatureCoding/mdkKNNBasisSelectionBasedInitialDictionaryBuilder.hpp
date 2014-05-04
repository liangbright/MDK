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

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
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

    if (m_Parameter.MaxNumberOfNewBases <= 0 && IsInputDictionaryEmpty == true)
    {
        MDK_Error("MaxNumberOfNewBases <= 0 and InputDictionary is empty @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
        return false;
    }

    //----------------------------------------

    return true;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::GenerateDictionary()
{
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
        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();
        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        m_Dictionary->Take(*m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary());
    }
    else
    {
        // encode data, code element is similarity

        m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);
        m_KNNSoftAssignSparseEncoder.SetInputDictionary(m_InputDictionary);
        m_KNNSoftAssignSparseEncoder.Update();
        auto CodeTable = m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet();

        // perform SimilarityThreshold based Classification

        DenseMatrix<int_max> IndexList_InClass;
        DenseMatrix<int_max> IndexList_OutClass;
        this->ClassifyFeatureDataBySimilarityThreshold(IndexList_InClass, IndexList_OutClass, *CodeTable);

        if (IndexList_OutClass.GetElementNumber() == 0)
        {
            MDK_Warning("All Feature Data is classified as InClass, no new basis is generated")
            m_Dictionary->Copy(m_InputDictionary);
            return;
        }
      
        DenseMatrix<ElementType> FeatureData_OutClass = m_FeatureData->GetSubMatrix(ALL, IndexList_OutClass);

        // select new basis from data that is OutClass

        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

        m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.MaxNumberOfNewBases;

        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData_OutClass);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        auto NewDicitonaryFromOutClass = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

        int_max BasisNumber_new = NewDicitonaryFromOutClass->BasisMatrix().GetColNumber();

        int_max BasisNumber_input = m_InputDictionary->BasisMatrix().GetColNumber();

        int_max BasisNumber_to_keep = m_Parameter.BasisNumber - BasisNumber_new;

        // check if we need to retire old basis in input dictionary

        if (BasisNumber_to_keep >= BasisNumber_input) // keep all the bases in input dictionary
        {
            this->CombineInputDictionaryAndNewDictionary(*m_Dictionary, *m_InputDictionary, *NewDicitonaryFromOutClass);
            
            return;            
        }
        else// if (BasisNumber_to_keep < BasisNumber_input)
        {
            FeatureDictionaryForSparseCoding<ElementType> InputDictionary_Modified;

            if (m_Parameter.DirectlyRetireOldAndInexperiencedBasis == true)
            {               
                this->DirectlyRetireBasisInInputDictionary(InputDictionary_Modified, *m_InputDictionary, BasisNumber_to_keep);
            }
            else
            {
                DenseMatrix<ElementType> FeatureData_InClass = m_FeatureData->GetSubMatrix(ALL, IndexList_InClass);

                // select basis from data that is InClass

                m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder();

                m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = BasisNumber_to_keep;

                m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData_InClass);
                m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InputDictionary);
                m_KNNBasisSelectionDictionaryBuilder.Update();

                InputDictionary_Modified.Take(*m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary());
            }

            this->CombineInputDictionaryAndNewDictionary(*m_Dictionary, InputDictionary_Modified, *NewDicitonaryFromOutClass);

            return;
        }
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
CombineInputDictionaryAndNewDictionary(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary,
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

    //for (int_max k = 0; k <= BasisNumber_input-1; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
    {
        auto VectorPtr_input = BasisMatrix_combined.GetElementPointerOfCol(k);

        for (int_max n = 0; n < BasisNumber_new; ++k)
        {
            auto tempIndex = BasisNumber_input + k;

            auto VectorPtr_new = BasisMatrix_combined.GetElementPointerOfCol(tempIndex);

            auto Similarity = this->ComputeSimilarityBetweenTwoVectors(VectorPtr_input, VectorPtr_new, VectorLength);

            SimilarityMatrix_combined(tempIndex, k);
            SimilarityMatrix_combined(k, tempIndex);
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber_input - 1, m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
inline
ElementType KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length)
{
    ElementType Similarity = ElementType(0);

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L1Distance*L1Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L2Distance*L2Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, false);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, false);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = (KLDivergence*KLDivergence) / m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ KNNBasisSelectionBasedInitialDictionaryBuilder::ComputeSimilarityBetweenTwoVectors(...)")
    }

    return Similarity;
}

}//namespace mdk

#endif