#ifndef __mdkFeatureSparseEncoder_hpp
#define __mdkFeatureSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureSparseEncoder<ElementType>::mdkFeatureSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
mdkFeatureSparseEncoder<ElementType>::~mdkFeatureSparseEncoder()
{

}


template<typename ElementType>
void mdkFeatureSparseEncoder<ElementType>::Clear()
{
    this->mdkFeatureEncoder::Clear();

    m_FeatureCodeInCompactFormat_SharedCopy.Clear();

    m_FeatureCodeInCompactFormat = &m_FeatureCodeInCompactFormat_SharedCopy;

}



template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::SetOutputFeatureCodeInCompactFormat(mdkDenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoder::GetOutputFeatureCodeInCompactFormat(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCodeInCompactFormat = FeatureCode;

    m_FeatureCodeInCompactFormat_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::Update()
{
    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureSparseEncoder::Update()" << '\n';
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
bool mdkFeatureSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
const mdkDenseMatrix<ElementType>* mdkFeatureSparseEncoder<ElementType>::GetOutputFeatureCodeInCompactFormat()
{
    return &m_FeatureCodeInCompactFormat_SharedCopy;
}


}// namespace mdk


#endif