#ifndef __mdkFeatureDictionaryBasedSparseEncoder_hpp
#define __mdkFeatureDictionaryBasedSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedSparseEncoder<ElementType>::FeatureDictionaryBasedSparseEncoder()
{
    m_IsDenseEncoder = false;

    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBasedSparseEncoder<ElementType>::~FeatureDictionaryBasedSparseEncoder()
{

}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::Clear()
{
    m_FeatureData = nullptr;

    m_Dictionary  = nullptr;

    m_FeatureCodeInCompactFormat_SharedCopy.Clear();

    m_FeatureCodeInCompactFormat = &m_FeatureCodeInCompactFormat_SharedCopy;

    m_FeatureCodeInDenseFormat_SharedCopy.Clear();

    m_FeatureCode = &m_FeatureCodeInDenseFormat_SharedCopy;

    m_FeatureCodeInSparseFormat_SharedCopy.Clear();

    m_FeatureCodeInSparseFormat = &m_FeatureCodeInSparseFormat_SharedCopy;

    m_Flag_FeatureCodeInDenseFormat_Is_Updated = false;

    m_Flag_FeatureCodeInSparseFormat_Is_Updated = false;

    m_MaximunNumberOfThreads = 1;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedSparseEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedDenseEncoder::SetInputDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceShare(Dictionary);
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedSparseEncoder::GetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCodeInDenseFormat = FeatureCode;

    m_FeatureCodeInDenseFormat_SharedCopy.ForceShare(FeatureCode);

    return true;
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
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCodeInSparseFormat(SparseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputFeatureCodeInSparseFormat(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCodeInSparseFormat = FeatureCode;

    m_FeatureCodeInSparseFormat_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetMaximunNumberOfThreads(int64 Number)
{
    m_MaximunNumberOfThreads = Number;
}


template<typename ElementType>
int64 FeatureDictionaryBasedSparseEncoder<ElementType>::GetMaximunNumberOfThreads()
{
    return m_MaximunNumberOfThreads;
}


template<typename ElementType>
int64 FeatureDictionaryBasedSparseEncoder<ElementType>::GetFeatureVectorNumber()
{
    return m_FeatureData->GetColNumber();
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error << "Input FeatureData is empty @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error << "Input Dictionary is empty @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    auto DictionarySize = m_Dictionary->GetSize();

    int64 FeatureCodeDimension = DictionarySize.ColNumber;

    if (m_MaxNumberOfNonZeroElementsInEachCode <= 0 || m_MaxNumberOfNonZeroElementsInEachCode > FeatureCodeDimension)
    {
        MDK_Error << "input MaxNumberOfNonZeroElementsInEachCode is invalid @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    auto tempSize = m_FeatureCodeInCompactFormat->GetSize();

    if (tempSize.RowNumber != 2 * m_MaxNumberOfNonZeroElementsInEachCode || tempSize.ColNumber != m_FeatureData->GetColNumber())
    {
        if (m_FeatureCodeInCompactFormat->IsSizeFixed() == false)
        {
            m_FeatureCodeInCompactFormat->Clear();
            m_FeatureCodeInCompactFormat->Resize(2 * m_MaxNumberOfNonZeroElementsInEachCode, m_FeatureData->GetColNumber());
        }
        else
        {
            MDK_Error << "can not change the size of m_FeatureCodeInCompactFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()" << '\n';
            return false;
        }
    }

    if (m_MaximunNumberOfThreads <= 0)
    {
        MDK_Warning << "input MaximunNumberOfThreads is invalid, set to 1 @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()" << '\n';

        m_MaximunNumberOfThreads = 1;
    }

    return true;
}



template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::Update()
{
    if (this->CheckInputAndOutput() == false)
    {
        return false;
    }

    this->FeatureDictionaryBasedEncoder::Update();

    //--------------------------------------------------------------

    if (m_FeatureCodeInCompactFormat != &m_FeatureCodeInCompactFormat_SharedCopy)
    {
        m_FeatureCodeInCompactFormat_SharedCopy.ForceShare(m_FeatureCodeInCompactFormat);
    }

    m_Flag_FeatureCodeInDenseFormat_Is_Updated = false;

    m_Flag_FeatureCodeInSparseFormat_Is_Updated = false;

    //--------------------------------------------------------------

    return true;
}



template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInCompactFormat()
{
    return &m_FeatureCodeInCompactFormat_SharedCopy;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCode() // InDenseFormat
{
    if (m_Flag_FeatureCodeInDenseFormat_Is_Updated == false)
    {
        auto DictionarySize = m_Dictionary->GetSize();

        int64 FeatureCodeDimension = DictionarySize.ColNumber;

        auto tempSize = m_FeatureCodeInDenseFormat->GetSize();

        if (tempSize.RowNumber != FeatureCodeDimension || tempSize.ColNumber != m_FeatureData->GetColNumber())
        {
            if (m_FeatureCodeInDenseFormat->IsSizeFixed() == false)
            {
                m_FeatureCodeInDenseFormat->Clear();
                m_FeatureCodeInDenseFormat->Resize(FeatureCodeDimension, m_FeatureData->GetColNumber());

                this->GetFeatureCodeInDenseFormatFromCompactFormat(*m_FeatureCodeInDenseFormat, *m_FeatureCodeInCompactFormat);

                if (m_FeatureCodeInDenseFormat != &m_FeatureCodeInDenseFormat_SharedCopy)
                {
                    m_FeatureCodeInDenseFormat_SharedCopy.ForceShare(m_FeatureCodeInDenseFormat);
                }

                m_Flag_FeatureCodeInDenseFormat_Is_Updated = true;
            }
            else
            {
                MDK_Error << "can not change the size of m_FeatureCodeInDenseFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInput()" << '\n';
            }
        }       
    }

    return &m_FeatureCodeInDenseFormat_SharedCopy;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInSparseFormat()
{
    if (m_Flag_FeatureCodeInSparseFormat_Is_Updated == false)
    {
        auto DictionarySize = m_Dictionary->GetSize();

        int64 FeatureCodeDimension = DictionarySize.ColNumber;

        auto tempSize = m_FeatureCodeInSparseFormat->GetSize();

        if (tempSize.RowNumber != FeatureCodeDimension || tempSize.ColNumber != m_FeatureData->GetColNumber())
        {
            if (m_FeatureCodeInSparseFormat->IsSizeFixed() == false)
            {
                m_FeatureCodeInSparseFormat->Clear();
                m_FeatureCodeInSparseFormat->Resize(FeatureCodeDimension, m_FeatureData->GetColNumber());

                this->GetFeatureCodeInSparseFormatFromCompactFormat(*m_FeatureCodeInSparseFormat, *m_FeatureCodeInCompactFormat);

                if (m_FeatureCodeInSparseFormat != &m_FeatureCodeInSparseFormat_SharedCopy)
                {
                    m_FeatureCodeInSparseFormat_SharedCopy.ForceShare(m_FeatureCodeInSparseFormat);
                }

                m_Flag_FeatureCodeInSparseFormat_Is_Updated = true;
            }
            else
            {
                MDK_Error << "can not change the size of m_FeatureCodeInSparseFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInput()" << '\n';
            }
        }
    }

    return &m_FeatureCodeInSparseFormat_SharedCopy;
}


}// namespace mdk


#endif