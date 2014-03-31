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

    m_MaxNumberOfNonzeroElementsInEachCode = 1;

    m_FeatureCodeInCompactFormat_SharedCopy.Clear();

    m_FeatureCodeInCompactFormat = &m_FeatureCodeInCompactFormat_SharedCopy;

    m_FeatureCodeInDenseFormat_SharedCopy.Clear();

    m_FeatureCodeInDenseFormat = &m_FeatureCodeInDenseFormat_SharedCopy;

    m_FeatureCodeInSparseFormat_SharedCopy.Clear();

    m_FeatureCodeInSparseFormat = &m_FeatureCodeInSparseFormat_SharedCopy;

    m_Flag_Output_FeatureCodeInDenseFormat = false;

    m_Flag_Output_FeatureCodeInSparseFormat = false;

    m_Flag_FeatureCodeInDenseFormat_Is_Updated = false;

    m_Flag_FeatureCodeInSparseFormat_Is_Updated = false;

    m_MaxNumberOfThreads = 1;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetInputFeatureData(InputFeatureData)")
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    if (Dictionary == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedDenseEncoder::SetInputDictionary(Dictionary)")
        return false;
    }

    m_Dictionary = Dictionary;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCodeInCompactFormat(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::GetOutputFeatureCodeInCompactFormat(FeatureCode)")
        return false;
    }

    m_FeatureCodeInCompactFormat = FeatureCode;

    m_FeatureCodeInCompactFormat_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCodeInDenseFormat(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputFeatureCodeInDenseFormat(FeatureCode)")
        return false;
    }

    m_FeatureCodeInDenseFormat = FeatureCode;

    m_FeatureCodeInDenseFormat_SharedCopy.ForceShare(FeatureCode);

    m_Flag_Output_FeatureCodeInDenseFormat = true;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    return this->SetOutputFeatureCodeInDenseFormat(FeatureCode);
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputFeatureCodeInSparseFormat(SparseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputFeatureCodeInSparseFormat(FeatureCode)")
        return false;
    }

    m_FeatureCodeInSparseFormat = FeatureCode;

    m_FeatureCodeInSparseFormat_SharedCopy.ForceShare(FeatureCode);

    m_Flag_Output_FeatureCodeInSparseFormat = true;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetMaxNumberOfThreads(int64 Number)
{
    if (Number <= 0)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetMaxNumberOfThreads(int64 Number)")

        m_MaxNumberOfThreads = 1;

        return false;
    }

    m_MaxNumberOfThreads = Number;

    return true;
}


template<typename ElementType>
int64 FeatureDictionaryBasedSparseEncoder<ElementType>::GetMaxNumberOfThreads()
{
    return m_MaxNumberOfThreads;
}


template<typename ElementType>
int64 FeatureDictionaryBasedSparseEncoder<ElementType>::GetTotalNumberOfInputFeatureVectors()
{
    return m_FeatureData->GetColNumber();
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("Input FeatureData is empty @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Input Dictionary is empty @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    auto DictionarySize = m_Dictionary->GetSize();

    int64 FeatureCodeDimension = DictionarySize.ColNumber;

    if (m_MaxNumberOfNonzeroElementsInEachCode <= 0 || m_MaxNumberOfNonzeroElementsInEachCode > FeatureCodeDimension)
    {
        MDK_Error("Input MaxNumberOfNonZeroElementsInEachCode is invalid @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    auto tempSize = m_FeatureCodeInCompactFormat->GetSize();

    if (tempSize.RowNumber != 2 * m_MaxNumberOfNonzeroElementsInEachCode || tempSize.ColNumber != m_FeatureData->GetColNumber())
    {
        if (m_FeatureCodeInCompactFormat->IsSizeFixed() == false)
        {
            m_FeatureCodeInCompactFormat->FastResize(2 * m_MaxNumberOfNonzeroElementsInEachCode, m_FeatureData->GetColNumber());
        }
        else
        {
            MDK_Error("can not change the size of m_FeatureCodeInCompactFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
            return false;
        }
    }

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning("Input MaxNumberOfThreads is invalid, set to 1 @ mdkFeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")

        m_MaxNumberOfThreads = 1;
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

    if (m_Flag_Output_FeatureCodeInDenseFormat == true)
    {
        this->GetOutputFeatureCodeInDenseFormat(); // update FeatureCodeInDenseFormat
    }

    m_Flag_FeatureCodeInSparseFormat_Is_Updated = false;

    if (m_Flag_Output_FeatureCodeInSparseFormat == true)
    {
        this->GetOutputFeatureCodeInSparseFormat(); // update FeatureCodeInSparseFormat
    }
   
    //--------------------------------------------------------------

    return true;
}



template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInCompactFormat()
{
    return &m_FeatureCodeInCompactFormat_SharedCopy;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInDenseFormat()
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
                m_FeatureCodeInDenseFormat->FastResize(FeatureCodeDimension, m_FeatureData->GetColNumber());

                this->GetFeatureCodeInDenseFormatFromCompactFormat(*m_FeatureCodeInDenseFormat, *m_FeatureCodeInCompactFormat);

                if (m_FeatureCodeInDenseFormat != &m_FeatureCodeInDenseFormat_SharedCopy)
                {
                    m_FeatureCodeInDenseFormat_SharedCopy.ForceShare(m_FeatureCodeInDenseFormat);
                }

                m_Flag_FeatureCodeInDenseFormat_Is_Updated = true;
            }
            else
            {
                MDK_Error("can not change the size of m_FeatureCodeInDenseFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInput()")
            }
        }       
    }

    return &m_FeatureCodeInDenseFormat_SharedCopy;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCode() // InDenseFormat
{
    return this->GetOutputFeatureCodeInDenseFormat();
}


template<typename ElementType>
SparseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputFeatureCodeInSparseFormat()
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
                m_FeatureCodeInSparseFormat->FastResize(FeatureCodeDimension, m_FeatureData->GetColNumber());

                this->GetFeatureCodeInSparseFormatFromCompactFormat(*m_FeatureCodeInSparseFormat, *m_FeatureCodeInCompactFormat);

                if (m_FeatureCodeInSparseFormat != &m_FeatureCodeInSparseFormat_SharedCopy)
                {
                    m_FeatureCodeInSparseFormat_SharedCopy.ForceShare(m_FeatureCodeInSparseFormat);
                }

                m_Flag_FeatureCodeInSparseFormat_Is_Updated = true;
            }
            else
            {
                MDK_Error("can not change the size of m_FeatureCodeInSparseFormat matrix @ mdkFeatureDictionaryBasedSparseEncoder::CheckInput()")
            }
        }
    }

    return &m_FeatureCodeInSparseFormat_SharedCopy;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::
GetFeatureCodeInDenseFormatFromCompactFormat(DenseMatrix<ElementType>& FeatureCodeInDenseFormat,
                                             const DenseMatrix<ElementType>& FeatureCodeInCompactFormat)
{

}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::
GetFeatureCodeInSparseFormatFromCompactFormat(SparseMatrix<ElementType>& FeatureCodeInSparseFormat,
                                              const DenseMatrix<ElementType>& FeatureCodeInCompactFormat)
{
}

}// namespace mdk


#endif