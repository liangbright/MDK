#ifndef __mdkFeatureEncoder_hpp
#define __mdkFeatureEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureEncoder<ElementType>::mdkFeatureEncoder()
{

}


template<typename ElementType>
mdkFeatureEncoder<ElementType>::~mdkFeatureEncoder()
{

}


template<typename ElementType>
void mdkFeatureEncoder<ElementType>::Reset()
{
    m_Dictionary_Storage.Reset();

    m_Dictionary = &m_Dictionary_SharedCopy;

    m_FeatureData = nullptr;

    m_FeatureCode_Storage.Reset();

    m_FeatureCode = &m_FeatureCode_SharedCopy;

}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetInputFeatureData(const mdkMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetDictionary(const mdkFeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceSharedCopy(Dictionary);
}



template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::LoadDictionary(const std::string& FilePathAndName)
{
    auto IsOK = m_Dictionary_SharedCopy.Load(FilePathAndName);

    m_Dictionary = &m_Dictionary_SharedCopy;

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetOutputFeatureCode(mdkMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = FeatureCode;

    m_FeatureCode_SharedCopy.ForceSharedCopy(FeatureCode);

    return true;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::Update()
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
        m_FeatureCode_SharedCopy.SharedCopy(m_FeatureCode);
    }

    return IsOK;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
mdkMatrix<ElementType>* mdkFeatureEncoder<ElementType>::GetOutputFeatureCode()
{
    return &m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif