#ifndef __mdkFeatureDictionaryBasedEncoder_hpp
#define __mdkFeatureDictionaryBasedEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedEncoder<ElementType>::FeatureDictionaryBasedEncoder()
{
    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBasedEncoder<ElementType>::~FeatureDictionaryBasedEncoder()
{

}


template<typename ElementType>
void FeatureDictionaryBasedEncoder<ElementType>::Clear()
{
    m_Dictionary_SharedCopy.Clear();

    m_Dictionary = &m_Dictionary_SharedCopy;

    m_FeatureData = nullptr;

    m_FeatureCode_SharedCopy.Clear();

    m_FeatureCode = &m_FeatureCode_SharedCopy;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::SetDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedEncoder::SetDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceSharedCopy(Dictionary);
}



template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::LoadDictionary(const std::string& FilePathAndName)
{
    auto IsOK = m_Dictionary_SharedCopy.Load(FilePathAndName);

    m_Dictionary = &m_Dictionary_SharedCopy;

    return IsOK;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = FeatureCode;

    m_FeatureCode_SharedCopy.ForceSharedCopy(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::Update()
{
    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error << "InputFeatureData is empty @ FeatureDictionaryBasedEncoder::Update()" << '\n';
        return false;
    }

    //--------------------------------------------------------------

    auto IsOK = this->GenerateCode();

    //--------------------------------------------------------------

    if (m_FeatureCode != &m_FeatureCode_SharedCopy)
    {
        m_FeatureCode_SharedCopy.ForceSharedCopy(m_FeatureCode);
    }

    return IsOK;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedEncoder<ElementType>::GetOutputFeatureCode()
{
    return &m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif