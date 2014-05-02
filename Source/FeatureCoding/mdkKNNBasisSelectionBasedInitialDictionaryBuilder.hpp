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

    m_InitialDictionary = nullptr;

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

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNBasisSelectionBasedInitialDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    // ------------ check input to m_KNNBasisSelectionDictionaryBuilder -----------//

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.ParameterOfKNNBasisSelection;
    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InitialDictionary);

    if (m_KNNBasisSelectionDictionaryBuilder.CheckInput() == false)
    {
        m_KNNBasisSelectionDictionaryBuilder.Clear();

        return false;
    }

    return true;
}


template<typename ElementType>
void KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType>::GenerateDictionary()
{
    if (m_InitialDictionary == nullptr)
    {
        m_KNNBasisSelectionDictionaryBuilder.m_Parameter = m_Parameter.ParameterOfKNNBasisSelection;
        m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(m_FeatureData);
        m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(m_InitialDictionary);
        m_KNNBasisSelectionDictionaryBuilder.Update();

        m_Dictionary->Take(*m_KNNBasisSelectionDictionaryBuilder->GetOutputDictionary());
    }
    else
    {

    }

}


}//namespace mdk

#endif