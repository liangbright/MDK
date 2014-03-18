#ifndef __mdkFeatureSparseEncoder_hpp
#define __mdkFeatureSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureSparseEncoder<ElementType>::mdkFeatureSparseEncoder()
{

}


template<typename ElementType>
mdkFeatureSparseEncoder<ElementType>::~mdkFeatureSparseEncoder()
{

}


template<typename ElementType>
void mdkFeatureSparseEncoder<ElementType>::Reset()
{
    m_Dictionary_Storage.Reset();

    m_Dictionary = &m_Dictionary_SharedCopy;

    m_FeatureData = nullptr;

    m_FeatureCode_Storage.Reset();

    m_FeatureCode = &m_FeatureCode_SharedCopy;

}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::SetInputFeatureData(const mdkMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::SetDictionary(const mdkFeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoder::SetDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceSharedCopy(*Dictionary);
}



template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::LoadDictionary(const std::string& FilePathAndName)
{
    auto IsOK = m_Dictionary_SharedCopy.Load(FilePathAndName);

    m_Dictionary = &m_Dictionary_SharedCopy;

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::SetOutputFeatureCode(mdkMatrix<ElementType>& FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = &FeatureCode;

    m_FeatureCode_SharedCopy.ForceSharedCopy(FeatureCode);

    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::Update()
{
    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureDictionaryBuilder::Update()" << '\n';
        return false;
    }

    //--------------------------------------------------------------

    auto IsOK = this->GenerateCode();

    //--------------------------------------------------------------

    if (m_FeatureCode != &m_FeatureCode_SharedCopy)
    {
        m_FeatureCode_SharedCopy.SharedCopy(*m_FeatureCode);
    }

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
const mdkMatrix<ElementType>& mdkFeatureSparseEncoder<ElementType>::GetOutputFeatureCode()
{
    return m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif