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
    this->mdkFeatureEncoder::Reset();

    m_FeatureSparseCode_SharedCopy.Reset();

    m_FeatureSparseCode = &m_FeatureSparseCode_SharedCopy;

}



template<typename ElementType>
bool mdkFeatureSparseEncoder<ElementType>::SetOutputFeatureSparseCode(mdkMatrix<ElementType>* FeatureSparseCode)
{
    if (FeatureSparseCode == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureSparseEncoder::SetOutputFeatureSparseCode(FeatureSparseCode)" << '\n';
        return false;
    }

    m_FeatureSparseCode = FeatureSparseCode;

    m_FeatureSparseCode_SharedCopy.ForceSharedCopy(*FeatureSparseCode);

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
        m_FeatureCode_SharedCopy.SharedCopy(*m_FeatureCode);
    }

    if (m_FeatureSparseCode != &m_FeatureSparseCode_SharedCopy)
    {
        m_FeatureSparseCode_SharedCopy.SharedCopy(*m_FeatureSparseCode);
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
const mdkMatrix<ElementType>* mdkFeatureSparseEncoder<ElementType>::GetOutputFeatureSparseCode()
{
    return &m_FeatureSparseCode_SharedCopy;
}


}// namespace mdk


#endif