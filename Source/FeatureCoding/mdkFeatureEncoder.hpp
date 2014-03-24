#ifndef __mdkFeatureEncoder_hpp
#define __mdkFeatureEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureEncoder<ElementType>::FeatureEncoder()
{
    this->Clear();
}


template<typename ElementType>
FeatureEncoder<ElementType>::~FeatureEncoder()
{

}


template<typename ElementType>
void FeatureEncoder<ElementType>::Clear()
{
    m_Dictionary_Storage.Clear();

    m_Dictionary = &m_Dictionary_SharedCopy;

    m_FeatureData = nullptr;

    m_FeatureCode_Storage.Clear();

    m_FeatureCode = &m_FeatureCode_SharedCopy;
}


template<typename ElementType>
bool FeatureEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool FeatureEncoder<ElementType>::SetDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoder::SetDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceSharedCopy(Dictionary);
}



template<typename ElementType>
bool FeatureEncoder<ElementType>::LoadDictionary(const std::string& FilePathAndName)
{
    auto IsOK = m_Dictionary_SharedCopy.Load(FilePathAndName);

    m_Dictionary = &m_Dictionary_SharedCopy;

    return IsOK;
}


template<typename ElementType>
bool FeatureEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = FeatureCode;

    m_FeatureCode_SharedCopy.ForceSharedCopy(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureEncoder<ElementType>::Update()
{
    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error << "InputFeatureData is empty @ FeatureEncoder::Update()" << '\n';
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
bool FeatureEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureEncoder<ElementType>::GetOutputFeatureCode()
{
    return &m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif