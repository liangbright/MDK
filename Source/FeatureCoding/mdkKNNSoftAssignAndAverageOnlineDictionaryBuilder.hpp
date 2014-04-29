#ifndef __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_hpp
#define __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::KNNSoftAssignAndAverageOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::~KNNSoftAssignAndAverageOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("NumberOfDataInEachBatch > TotalDataNumber @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("NumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "L1Distance"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "L2Distance"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "Correlation"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "KLDivergence")
    {
        MDK_Error("SimilarityType is invalid @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L1Distance")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1 = m_Dictionary->StandardDeviationOfL1Distance().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Sigma_L1 <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L2Distance")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2 = m_Dictionary->StandardDeviationOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("Sigma_L2 <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "Correlation")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "KLDivergence")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL = m_Dictionary->StandardDeviationOfKLDivergence().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Sigma_KL <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    return true;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    m_KNNSoftAssignDictionaryBuilder.SetInputFeatureData(m_FeatureData);
    m_KNNSoftAssignDictionaryBuilder.SetInitialDictionary(m_InitialDictionary);


  
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> 
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionaryFromDataBatch(int_max BasisNumber_desired,
                                                                                  const DenseMatrix<ElementType>& FeatureData,
                                                                                  const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData,
                                                                                  const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init)
{
    

}


}// namespace mdk


#endif