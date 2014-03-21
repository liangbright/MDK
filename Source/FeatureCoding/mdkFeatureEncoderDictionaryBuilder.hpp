#ifndef __mdkFeatureEncoderDictionaryBuilder_hpp
#define __mdkFeatureEncoderDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureEncoderDictionaryBuilder<ElementType>::mdkFeatureEncoderDictionaryBuilder()
{
    this->Reset();
}


template<typename ElementType>
mdkFeatureEncoderDictionaryBuilder<ElementType>::~mdkFeatureEncoderDictionaryBuilder()
{
}


template<typename ElementType>
void mdkFeatureEncoderDictionaryBuilder<ElementType>::Reset()
{
    m_GenericEncoder = nullptr;

    m_FeatureData = nullptr;

    m_DictionaryLength = 0;

    m_InitialDictionary = nullptr;

    m_Dictionary_SharedCopy.Reset();

    m_Dictionary = &m_Dictionary_SharedCopy;
}


//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::SetInitialDictionary(const mdkFeatureDictionary<ElementType>* InitialDictionary)
{
    if (InitialDictionary == nullptr)
    {
        mdkError << "Invalid Input @ mdkFeatureEncoderDictionaryBuilder::SetInitialDictionary(InitialDictionary)" << '\n';
        return false;
    }

    m_InitialDictionary = InitialDictionary;

    return true;
}


template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::SetOutputDictionary(mdkFeatureDictionary<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoderDictionaryBuilder::SetOutputDictionary(OutputDictionary)" << '\n';
        return false;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceSharedCopy(OutputDictionary);

    return true;
}



template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::SetDictionaryLength(int64 Length)
{
    if (Length <= 0)
    {
        mdkError << "Invalid input @ mdkFeatureEncoderDictionaryBuilder::SetDictionaryLength(Length)" << '\n';
        return false;
    }

    m_DictionaryLength = Length;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder)
{
    if (FeatureEncoder == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoderDictionaryBuilder::SetGenericEncoder(FeatureEncoder)" << '\n';
        return false;
    }

    m_GenericEncoder = FeatureEncoder;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::SetInputFeatureData(const mdkDenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoderDictionaryBuilder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::Update()
{
    if (m_FeatureData == nullptr)
    {
        mdkError << "m_FeatureData is empty @ mdkFeatureEncoderDictionaryBuilder::Update()" << '\n';
        return false;
    }

    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    if (m_DictionaryLength == 0)
    {
        mdkError << "m_DictionaryLength is zero @ mdkFeatureEncoderDictionaryBuilder::Update()" << '\n';
        return false;
    }

    auto BookSize = m_Dictionary->GetSize();

    if (BookSize.RowNumber > 0 && BookSize.RowNumber != DataSize.RowNumber)
    {
        mdkError << "Feature dimension does not match @ mdkFeatureEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    //--------------------------------------------------

    auto IsOK = this->GenerateDictionary();

    //--------------------------------------------------

    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.ForceSharedCopy(m_Dictionary);
    }

    //--------------------------------------------------

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureEncoderDictionaryBuilder<ElementType>::GenerateDictionary()
{
    return true;
}


template<typename ElementType>
mdkFeatureDictionary<ElementType>* mdkFeatureEncoderDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


}// namespace mdk


#endif