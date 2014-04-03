#ifndef __mdkFeatureDictionaryBasedDenseEncoder_hpp
#define __mdkFeatureDictionaryBasedDenseEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedDenseEncoder<ElementType>::FeatureDictionaryBasedDenseEncoder()
{
    m_IsDenseEncoder = true;

    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBasedDenseEncoder<ElementType>::~FeatureDictionaryBasedDenseEncoder()
{

}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::Clear()
{
    m_Dictionary_SharedCopy.Clear();

    m_Dictionary = &m_Dictionary_SharedCopy;

    m_FeatureData = nullptr;

    m_FeatureCode_SharedCopy.Clear();

    m_FeatureCode = &m_FeatureCode_SharedCopy;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedDenseEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary)
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
bool FeatureDictionaryBasedDenseEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ FeatureDictionaryBasedDenseEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = FeatureCode;

    m_FeatureCode_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::SetMaxNumberOfThreads(int_max Number)
{
    m_MaxNumberOfThreads = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetMaxNumberOfThreads()
{
    return m_MaxNumberOfThreads;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetTotalNumberOfInputFeatureVectors()
{
    if (m_FeatureData != nullptr)
    {
        return m_FeatureData->GetColNumber();
    }
    else
    {
        return 0;
    }
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::CheckInputAndOutput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error << "Input FeatureData is empty (nullptr) @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error << "Input FeatureData is empty @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_Dictionary == nullptr)
    {
        MDK_Error << "Input Dictionary is empty (nullptr) @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error << "Input Dictionary is empty @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    auto FeatureCodeDimension = m_Dictionary->m_Record.GetColNumber();

    auto tempSize = m_FeatureCode->GetSize();

    if (tempSize.RowNumber != FeatureCodeDimension || tempSize.ColNumber != m_FeatureData->GetColNumber())
    {
        if (m_FeatureCode->IsSizeFixed() == false)
        {
            m_FeatureCode->Clear();
            m_FeatureCode->Resize(FeatureCodeDimension, m_FeatureData->GetColNumber());
        }
        else
        {
            MDK_Error << "can not change the size of m_FeatureCode matrix @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';
            return false;        
        }
    }

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning << "input MaximunNumberOfThreads is invalid, set to 1 @ FeatureDictionaryBasedDenseEncoder::CheckInputAndOutput()" << '\n';

        m_MaxNumberOfThreads = 1;
    }

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::Update()
{
    auto IsOK = this->FeatureDictionaryBasedEncoder::Update();

    if (IsOK == false)
    {
        return false;
    }

    //--------------------------------------------------------------

    if (m_FeatureCode != &m_FeatureCode_SharedCopy)
    {
        m_FeatureCode_SharedCopy.ForceShare(m_FeatureCode);
    }

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end)
{
    DenseMatrix<ElementType> SingleFeatureCode(m_FeatureCode->GetRowNumber(), 1);

    DenseMatrix<ElementType> SingleFeatureDataVector(m_FeatureData->GetRowNumber(), 1);

    for (int_max i = IndexOfFeatureVector_start; i <= IndexOfFeatureVector_end; ++i)
    {
        m_FeatureData->GetCol(i, SingleFeatureDataVector);

        this->EncodingFunction(SingleFeatureDataVector, SingleFeatureCode);

        m_FeatureCode->SetCol(i, SingleFeatureCode);
    }
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedDenseEncoder<ElementType>::GetOutputFeatureCode()
{
    return &m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif