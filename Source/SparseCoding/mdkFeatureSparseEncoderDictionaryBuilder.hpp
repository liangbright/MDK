#ifndef __mdkFeatureSparseEncoderDictionaryBuilder_hpp
#define __mdkFeatureSparseEncoderDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::mdkFeatureSparseEncoderDictionaryBuilder()
{
    this->Reset();
}


template<typename ElementType>
mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::~mdkFeatureSparseEncoderDictionaryBuilder()
{
}


template<typename ElementType>
void mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::Reset()
{
    m_GenericEncoder = nullptr;

    m_FeatureData = nullptr;

    m_DictionaryLength = 0;

    m_Dictionary_SharedCopy.Reset();

    m_Dictionary = &m_Dictionary_SharedCopy;
}


//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SetInitialDictionary(const mdkFeatureDictionary<ElementType>* InitialDictionary)
{
    if (InitialDictionary == nullptr)
    {
        mdkError << "Invalid Input @ mdkFeatureSparseEncoderDictionaryBuilder::SetInitialDictionary(InitialDictionary)" << '\n';
        return false;
    }

    m_InitialDictionary = InitialDictionary;

    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::LoadDictionary(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SaveDictionary(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SetOutputDictionary(mdkFeatureDictionary<ElementType>& OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoderDictionaryBuilder::SetOutputDictionary(OutputDictionary)" << '\n';
        return false;
    }

    m_Dictionary.Share(OutputDictionary);

    return true;
}



template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SetDictionaryLength(int64 Length)
{
    if (Length <= 0)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoderDictionaryBuilder::SetDictionaryLength(Length)" << '\n';
        return false;
    }

    m_DictionaryLength = Length;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
void mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder)
{
    m_GenericEncoder = FeatureEncoder;
}

//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::SetInputFeatureData(const mdkMatrix<ElementType>& InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoderDictionaryBuilder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData.ForceShare(InputFeatureData);

    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::LoadFeatureData(const std::string& FilePathAndName)
{
    return true;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::Run()
{
    auto DataSize = m_FeatureData.GetSize();

    auto BookSize = m_Dictionary.GetSize();

    if (m_DictionaryLength == 0)
    {
        mdkError << "m_DictionaryLength is zero @ mdkFeatureSparseEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureSparseEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    if (BookSize.RowNumber > 0 && BookSize.RowNumber != DataSize.RowNumber)
    {
        mdkError << "Feature dimension does not match @ mdkFeatureSparseEncoderDictionaryBuilder::Run()" << '\n';
        return false;
    }

    //--------------------------------------------------

    auto IsOK = this->GenerateDictionary();

    //--------------------------------------------------

    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.SharedCopy(*m_Dictionary);
    }

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::GenerateDictionary()
{
    return true;
}


template<typename ElementType>
const mdkFeatureDictionary<ElementType>& mdkFeatureSparseEncoderDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return m_Dictionary_SharedCopy;
}


}// namespace mdk


#endif