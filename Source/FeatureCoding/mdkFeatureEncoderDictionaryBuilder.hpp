#ifndef __mdkFeatureEncoderDictionaryBuilder_hpp
#define __mdkFeatureEncoderDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
FeatureEncoderDictionaryBuilder<ElementType>::FeatureEncoderDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
FeatureEncoderDictionaryBuilder<ElementType>::~FeatureEncoderDictionaryBuilder()
{
}


template<typename ElementType>
void FeatureEncoderDictionaryBuilder<ElementType>::Clear()
{
    m_GenericEncoder = nullptr;

    m_FeatureData = nullptr;

    m_Dictionary_SharedCopy.Clear();

    m_Dictionary = &m_Dictionary_SharedCopy;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureEncoderDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoderDictionaryBuilder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureEncoderDictionaryBuilder<ElementType>::SetGenericEncoder(const FeatureEncoder<ElementType>* FeatureEncoder)
{
    if (FeatureEncoder == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoderDictionaryBuilder::SetGenericEncoder(FeatureEncoder)" << '\n';
        return false;
    }

    m_GenericEncoder = FeatureEncoder;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureEncoderDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionary<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoderDictionaryBuilder::SetOutputDictionary(OutputDictionary)" << '\n';
        return false;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);

    return true;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool FeatureEncoderDictionaryBuilder<ElementType>::Update()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error << "m_FeatureData is empty @ FeatureEncoderDictionaryBuilder::Update()" << '\n';
        return false;
    }

    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error << "InputFeatureData is empty @ FeatureEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    auto BookSize = m_Dictionary->GetSize();

    if (BookSize.RowNumber > 0 && BookSize.RowNumber != DataSize.RowNumber)
    {
        MDK_Error << "Feature dimension does not match @ FeatureEncoderDictionaryBuilder::Run()" << '\n';
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
bool FeatureEncoderDictionaryBuilder<ElementType>::GenerateDictionary()
{
    return true;
}


template<typename ElementType>
FeatureDictionary<ElementType>* FeatureEncoderDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


}// namespace mdk


#endif