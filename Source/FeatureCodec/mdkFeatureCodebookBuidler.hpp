#ifndef __mdkFeatureCodebookBuilder_hpp
#define __mdkFeatureCodebookBuilder_hpp

namespace mdk
{

template<typename ElementType>
mdkFeatureCodebookBuilder<ElementType>::mdkFeatureCodebookBuilder()
{
}


template<typename ElementType>
mdkFeatureCodebookBuilder<ElementType>::mdkFeatureCodebookBuilder(mdkFeatureCodebookBuilder<ElementType>&& InputBuilder)
{
    m_Codebook = std::move(InputBuilder.m_Codebook);

    m_FeatureData = std::move(InputBuilder.m_FeatureData);
}


template<typename ElementType>
mdkFeatureCodebookBuilder<ElementType>::~mdkFeatureCodebookBuilder()
{
}


template<typename ElementType>
void mdkFeatureCodebookBuilder<ElementType>::Reset()
{
    m_Codebook.Reset();

    m_CodebookLength = 0;

    m_FeatureData.Reset();

    m_GenericEncoder = nullptr;
}


template<typename ElementType>
void mdkFeatureCodebookBuilder<ElementType>::Clear()
{
    m_Codebook.Clear();

    m_CodebookLength = 0;

    m_FeatureData.Clear();

    m_GenericEncoder = nullptr;
}


//---------------------------------------------------//

template<typename ElementType>
void mdkFeatureCodebookBuilder<ElementType>::SetInitialCodebook(const mdkFeatureCodebook<ElementType>& InitialCodebook)
{
    if (m_Codebook.IsEmpty() == false)
    {
        mdkWarning << "m_Codebook is not empty @ mdkFeatureCodebookBuilder::SetInitialCodebook(InitialCodebook)" << '\n';
    }

    m_Codebook.Copy(InitialCodebook);
}


template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::LoadCodebook(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::SaveCodebook(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::SetOutputCodebook(mdkFeatureCodebook<ElementType>& OutputCodebook)
{
    if (OutputCodebook == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureCodebookBuilder::SetOutputCodebook(OutputCodebook)" << '\n';
        return false;
    }

    m_Codebook.Share(OutputCodebook);

    return true;
}



template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::SetCodebookLength(int64 Length)
{
    if (Length <= 0)
    {
        mdkError << "Invalid input @ mdkFeatureCodebookBuilder::SetCodebookLength(Length)" << '\n';
        return false;
    }

    m_CodebookLength = Length;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
void mdkFeatureCodebookBuilder<ElementType>::SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder)
{
    m_GenericEncoder = FeatureEncoder;
}

//---------------------------------------------------//

template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::SetInputFeatureData(const mdkMatrix<ElementType>& InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        mdkError << "Invalid input @ mdkFeatureCodebookBuilder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData.ForceShare(InputFeatureData);

    return true;
}


template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::LoadFeatureData(const std::string& FilePathAndName)
{
    return true;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::Run()
{
    auto DataSize = m_FeatureData.GetSize();

    auto BookSize = m_Codebook.GetSize();

    if (m_CodebookLength == 0)
    {
        mdkError << "m_CodebookLength is zero @ mdkFeatureCodebookBuilder::Run()" << '\n';
        return false;
    }

    if (DataSize.RowNumber == 0)
    {
        mdkError << "InputFeatureData is empty @ mdkFeatureCodebookBuilder::Run()" << '\n';
        return false;
    }

    if (BookSize.RowNumber > 0 && BookSize.RowNumber != DataSize.RowNumber)
    {
        mdkError << "Feature dimension does not match @ mdkFeatureCodebookBuilder::Run()" << '\n';
        return false;
    }

    return this->GenerateCodebook();

}


template<typename ElementType>
bool mdkFeatureCodebookBuilder<ElementType>::GenerateCodebook()
{
    return true;
}


template<typename ElementType>
mdkFeatureCodebook<ElementType>& mdkFeatureCodebookBuilder<ElementType>::GetOutputCodebook()
{
    return m_Codebook;
}


}// namespace mdk


#endif