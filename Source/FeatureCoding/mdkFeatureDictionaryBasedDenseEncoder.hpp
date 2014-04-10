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
    m_FeatureData = nullptr;

    m_Dictionary = nullptr;

    this->SetupDefaultPipelineOutput();

    m_MaxNumberOfThreads = 1;

    m_MinNumberOfDataPerThread = 1;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Code_SharedCopy.Clear();
    m_Code = &m_Code_SharedCopy;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::UpdatePipelineOutput()
{
    if (m_Code != &m_Code_SharedCopy)
    {
        m_Code_SharedCopy.ForceShare(m_Code);
    }
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetInputDictionary(const FeatureDictionaryForDenseCoding<ElementType>* Dictionary)
{
    m_Dictionary = Dictionary;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetOutputCode(DenseMatrix<ElementType>* Code)
{
    m_Code = Code;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetMaxNumberOfThreads(int_max Number)
{
    m_MaxNumberOfThreads = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetMaxNumberOfThreads()
{
    return m_MaxNumberOfThreads;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetMinNumberOfDataPerThread()
{
    return m_MinNumberOfDataPerThread;
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
bool FeatureDictionaryBasedDenseEncoder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error << "Input FeatureData is empty (nullptr) @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error << "Input FeatureData is empty @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        return false;
    }

    if (m_Dictionary == nullptr)
    {
        MDK_Error << "Input Dictionary is empty (nullptr) @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error << "Input Dictionary is empty @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        return false;
    }

    if (m_Code == nullptr)
    {
        MDK_Warning << "SetOutputCode(nullptr) @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        m_Code_SharedCopy.Clear();
        m_Code = &m_Code_SharedCopy;
    }

    auto CodeDimension = m_Dictionary->BasisMatrix().GetColNumber();

    auto tempSize = m_Code->GetSize();

    if (tempSize.RowNumber != CodeDimension || tempSize.ColNumber != m_FeatureData->GetColNumber())
    {
        auto IsOK = m_Code->FastResize(CodeDimension, m_FeatureData->GetColNumber());

        if (IsOK == false)
        {
            MDK_Error << "can not change the size of m_Code matrix @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
            return false;        
        }
    }

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning << "input MaximunNumberOfThreads is invalid, set to 1 @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';

        m_MaxNumberOfThreads = 1;
    }

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end)
{
    DenseMatrix<ElementType> SingleCode(m_Code->GetRowNumber(), 1);

    DenseMatrix<ElementType> SingleFeatureDataVector(m_FeatureData->GetRowNumber(), 1);

    for (int_max i = IndexOfFeatureVector_start; i <= IndexOfFeatureVector_end; ++i)
    {
        m_FeatureData->GetCol(i, SingleFeatureDataVector);

        this->EncodingFunction(SingleFeatureDataVector, SingleCode);

        m_Code->SetCol(i, SingleCode);
    }
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedDenseEncoder<ElementType>::GetOutputCode()
{
    return &m_Code_SharedCopy;
}


}// namespace mdk


#endif