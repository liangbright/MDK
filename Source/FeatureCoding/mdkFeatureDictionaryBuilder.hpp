#ifndef __mdkFeatureDictionaryBuilder_hpp
#define __mdkFeatureDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::FeatureDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::~FeatureDictionaryBuilder()
{
}


template<typename ElementType>
void FeatureDictionaryBuilder<ElementType>::Clear()
{
    m_FeatureData = nullptr;

    m_Dictionary_SharedCopy.Clear();

    m_Dictionary = &m_Dictionary_SharedCopy;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBuilder::SetInputFeatureData(InputFeatureData)")
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionary<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBuilder::SetOutputDictionary(OutputDictionary)")
        return false;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.Share(OutputDictionary);

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ FeatureDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::Update()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is empty @ FeatureDictionaryBuilder::Update()")
        return false;
    }

    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error("InputFeatureData is empty @ FeatureDictionaryBuilder::Run()")
        return false;
    }

    auto BookSize = m_Dictionary->GetSize();

    if (BookSize.RowNumber > 0 && BookSize.RowNumber != DataSize.RowNumber)
    {
        MDK_Error("Feature dimension does not match @ FeatureDictionaryBuilder::Run()")
        return false;
    }

    //--------------------------------------------------

    auto IsOK = this->GenerateDictionary();

    //--------------------------------------------------

    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.ForceShare(m_Dictionary);
    }

    //--------------------------------------------------

    return IsOK;
}


template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::GenerateDictionary()
{
    return true;
}


template<typename ElementType>
FeatureDictionary<ElementType>* FeatureDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


}// namespace mdk


#endif