#ifndef __mdkPCADenseEncoder_hpp
#define __mdkPCADenseEncoder_hpp

namespace mdk
{

template<typename ElementType>
PCADenseEncoder<ElementType>::PCADenseEncoder()
{
    this->Clear();
}


template<typename ElementType>
PCADenseEncoder<ElementType>::~PCADenseEncoder()
{

}


template<typename ElementType>
void PCADenseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedEncoder::Clear();

}


template<typename ElementType>
bool PCADenseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ PCADenseEncoder::SetInputFeatureData(InputFeatureData)" << '\n';
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}


template<typename ElementType>
bool PCADenseEncoder<ElementType>::SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error << "Invalid input @ PCADenseEncoder::SetInputDictionary(Dictionary)" << '\n';
        return false;
    }

    m_Dictionary = Dictionary;

    m_Dictionary_SharedCopy.ForceShare(Dictionary);
}



template<typename ElementType>
bool PCADenseEncoder<ElementType>::SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode)
{
    if (FeatureCode == nullptr)
    {
        MDK_Error << "Invalid input @ PCADenseEncoder::SetOutputFeatureCode(FeatureCode)" << '\n';
        return false;
    }

    m_FeatureCode = FeatureCode;

    m_FeatureCode_SharedCopy.ForceShare(FeatureCode);

    return true;
}


template<typename ElementType>
bool PCADenseEncoder<ElementType>::SetMaxNumberOfThreads(int_max Number)
{
    m_MaxNumberOfThreads = Number;
}


template<typename ElementType>
int_max PCADenseEncoder<ElementType>::GetMaxNumberOfThreads()
{
    return m_MaxNumberOfThreads;
}


template<typename ElementType>
int_max PCADenseEncoder<ElementType>::GetTotalNumberOfInputFeatureVectors()
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
bool PCADenseEncoder<ElementType>::CheckInputAndOutput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error << "Input FeatureData is empty (nullptr) @ PCADenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error << "Input FeatureData is empty @ PCADenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_Dictionary == nullptr)
    {
        MDK_Error << "Input Dictionary is empty (nullptr) @ PCADenseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error << "Input Dictionary is empty @ PCADenseEncoder::CheckInputAndOutput()" << '\n';
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
            MDK_Error << "can not change the size of m_FeatureCode matrix @ PCADenseEncoder::CheckInputAndOutput()" << '\n';
            return false;        
        }
    }

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning << "input MaximunNumberOfThreads is invalid, set to 1 @ PCADenseEncoder::CheckInputAndOutput()" << '\n';

        m_MaxNumberOfThreads = 1;
    }

    return true;
}


template<typename ElementType>
bool PCADenseEncoder<ElementType>::Update()
{
    auto IsOK = this->PCAEncoder::Update();

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
void PCADenseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end)
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
DenseMatrix<ElementType>* PCADenseEncoder<ElementType>::GetOutputFeatureCode()
{
    return &m_FeatureCode_SharedCopy;
}


}// namespace mdk


#endif