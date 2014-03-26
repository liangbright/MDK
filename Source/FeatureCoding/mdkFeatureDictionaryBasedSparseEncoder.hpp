#ifndef __mdkFeatureDictionaryBasedSparseEncoder_hpp
#define __mdkFeatureDictionaryBasedSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedSparseEncoder<ElementType>::FeatureDictionaryBasedSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBasedSparseEncoder<ElementType>::~FeatureDictionaryBasedSparseEncoder()
{

}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedEncoder::Clear();

    m_FeatureCodeInCompactFormat_SharedCopy.Clear();

    m_FeatureCodeInCompactFormat = &m_FeatureCodeInCompactFormat_SharedCopy;

}



template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCodeInCompactFormat(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedSparseEncoder::GetOutputFeatureCodeInCompactFormat(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCodeInCompactFormat = FeatureCode;

    m_FeatureCodeInCompactFormat_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::Update()
{
    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error << "InputFeatureData is empty @ mdkFeatureDictionaryBasedSparseEncoder::Update()" << '\n';
        return false;
    }

    //--------------------------------------------------------------

    auto IsOK = this->GenerateCode();

    //--------------------------------------------------------------

    if (m_FeatureCode != &m_FeatureCode_SharedCopy)
    {
        m_FeatureCode_SharedCopy.ForceShare(m_FeatureCode);
    }

    // note: whether or not to convert m_FeatureCode to m_FeatureCodeInCompactFormat
    // or only use m_FeatureCodeInCompactFormat
    // is determined by each specific sparse encoder

    if (m_FeatureCodeInCompactFormat != &m_FeatureCodeInCompactFormat_SharedCopy)
    {
        m_FeatureCodeInCompactFormat_SharedCopy.ForceShare(m_FeatureCodeInCompactFormat);
    }

    //--------------------------------------------------------------

    return IsOK;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
const DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInCompactFormat()
{
    return &m_FeatureCodeInCompactFormat_SharedCopy;
}


}// namespace mdk


#endif