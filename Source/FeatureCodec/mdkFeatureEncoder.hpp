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
    m_FeatureData.Reset();

    m_FeatureCode.Reset();
}



template<typename ElementType>
void mdkFeatureEncoder<ElementType>::Clear()
{
    m_FeatureData.Clear();

    m_FeatureCode.Clear();
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetInputFeatureData_Shared(const mdkMatrix<ElementType>& InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData.ForceShare(InputFeatureData);

    return true;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::LoadFeatureData(const std::string& FilePathAndName)
{
    return true;
}



template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetCodebook_Shared(const mdkFeatureCodebook<ElementType>& Codebook)
{
    m_Codebook.ForceShare(Codebook);
}



template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::LoadCodebook(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::SetOutputFeatureCode(mdkMatrix<ElementType>& FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode.Share(FeatureCode);

    return true;
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::Run()
{
    auto DataSize = m_FeatureData.GetSize();

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureCodebookBuilder::Run()" << '\n';
        return false;
    }

    return this->GenerateCode();
}


template<typename ElementType>
bool mdkFeatureEncoder<ElementType>::GenerateCode()
{
    return true;
}


template<typename ElementType>
mdkMatrix<ElementType>& mdkFeatureEncoder<ElementType>::GetOutputFeatureCode()
{
    return m_FeatureCode;
}


}// namespace mdk


#endif