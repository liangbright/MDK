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

    m_MinNumberOfDataPerThread = 1;
    m_MaxNumberOfThreads = 1;
    m_NumberOfCreatedThreads = 1;
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
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetNumberOfThreadsTobeCreated()
{
    return Compute_NecessaryNumberOfThreads_For_ParallelBlock(TotalDataVectorNumber, m_MaxNumberOfThreads, m_MinNumberOfDataPerThread);
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetMinNumberOfDataPerThread(int_max Number)
{
    m_MinNumberOfDataPerThread = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetMinNumberOfDataPerThread()
{
    return m_MinNumberOfDataPerThread;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetTotalNumberOfInputFeatureDataVectors()
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

    auto CodeLength = m_Dictionary->BasisMatrix().GetColNumber();

    auto tempSize = m_Code->GetSize();

    if (tempSize.RowNumber != CodeLength || tempSize.ColNumber != m_FeatureData->GetColNumber())
    {
        auto IsOK = m_Code->FastResize(CodeLength, m_FeatureData->GetColNumber());

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

    if (m_MinNumberOfDataPerThread <= 0)
    {
        MDK_Warning("input m_MinNumberOfDataPerThread is invalid, set to 1 @ FeatureDictionaryBasedDenseEncoder::CheckInput()")

        m_MinNumberOfDataPerThread = 1;
    }

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::Preprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::Postprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedDenseEncoder<ElementType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    if (this->Preprocess() == false)
    {
        MDK_Error("Preprocess() return false @ FeatureDictionaryBasedDenseEncoder::Update()")
            return false;
    }

    int_max TotalDataVectorNumber = this->GetTotalNumberOfInputFeatureDataVectors();

    // multi-thread -----------------------------------------------------------------

    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->GenerateCode_in_a_Thread(Index_start, Index_end, ThreadIndex); },
                  0, TotalDataVectorNumber - 1, m_MaxNumberOfThreads, m_MinNumberOfDataPerThread);
    //------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        MDK_Error("Postprocess() return false @ FeatureDictionaryBasedDenseEncoder::Update()")
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex)
{
    DenseMatrix<ElementType> CodeVector(m_Code->GetRowNumber(), 1);

    DenseMatrix<ElementType> DataVector(m_FeatureData->GetRowNumber(), 1);

    for (int_max i = IndexOfDataVector_start; i <= IndexOfDataVector_end; ++i)
    {
        m_FeatureData->GetCol(i, DataVector);

        this->EncodingFunction(CodeVector, DataVector, ThreadIndex);

        m_Code->SetCol(i, CodeVector);
    }
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedDenseEncoder<ElementType>::GetOutputCode()
{
    return &m_Code_SharedCopy;
}


}// namespace mdk


#endif