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

    m_CodeInDenseMatrix_SharedCopy.Clear();

    m_CodeInDenseMatrix = &m_CodeInDenseMatrix_SharedCopy;

    m_CodeInSparseMatrix_SharedCopy.Clear();

    m_CodeInSparseMatrix = &m_CodeInSparseMatrix_SharedCopy;

    m_CodeInSparseVectorList_SharedCopy.Clear();

    m_CodeInSparseVectorList = &m_CodeInSparseVectorList_SharedCopy;

    m_Flag_Output_CodeInDenseMatrix = false;

    m_Flag_Output_CodeInSparseMatrix = false;

    m_Flag_CodeInDenseMatrix_Is_Updated = false;

    m_Flag_CodeInSparseMatrix_Is_Updated = false;

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
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCodeInDenseMatrix(DenseMatrix<ElementType>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCodeInDenseMatrix(Code)")
        return false;
    }

    m_CodeInDenseMatrix = Code;

    m_CodeInDenseMatrix_SharedCopy.ForceShare(Code);

    m_Flag_Output_CodeInDenseMatrix = true;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCode(DenseMatrix<ElementType>* Code)
{
    return this->SetOutputCodeInDenseMatrix(Code);
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCodeInSparseMatrix(SparseMatrix<ElementType>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCodeInSparseMatrix(Code)")
        return false;
    }

    m_CodeInSparseMatrix = Code;

    m_CodeInSparseMatrix_SharedCopy.ForceShare(Code);

    m_Flag_Output_CodeInSparseMatrix = true;

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCodeInSparseVectorList(DenseMatrix<SparseMatrix<ElementType>>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCodeInSparseVectorList(Code)")
        return false;
    }

    m_CodeInSparseVectorList = Code;

    m_CodeInSparseVectorList_SharedCopy.ForceShare(Code);

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::SetMaxNumberOfThreads(int_max Number)
{
    if (Number <= 0)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetMaxNumberOfThreads(int_max Number)")

        m_MaxNumberOfThreads = 1;

        return false;
    }

    m_MaxNumberOfThreads = Number;

    return true;
}


template<typename ElementType>
int_max FeatureDictionaryBasedSparseEncoder<ElementType>::GetMaxNumberOfThreads()
{
    return m_MaxNumberOfThreads;
}


template<typename ElementType>
int_max FeatureDictionaryBasedSparseEncoder<ElementType>::GetTotalNumberOfInputFeatureVectors()
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
bool FeatureDictionaryBasedSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("Input FeatureData is empty (nullptr) @ FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("Input FeatureData is empty matrix @ FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_Dictionary == nullptr)
    {
        MDK_Error("Input Dictionary is empty (nullptr) @ FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Input Dictionary is empty @ FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning("Input MaxNumberOfThreads is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput()")

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

    if (m_CodeInSparseVectorList != &m_CodeInSparseVectorList_SharedCopy)
    {
        m_CodeInSparseVectorList_SharedCopy.ForceShare(m_CodeInSparseVectorList);
    }

    m_Flag_CodeInDenseMatrix_Is_Updated = false;

    if (m_Flag_Output_CodeInDenseMatrix == true)
    {
        this->GetOutputCodeInDenseMatrix(); // update
    }

    m_Flag_CodeInSparseMatrix_Is_Updated = false;

    if (m_Flag_Output_CodeInSparseMatrix == true)
    {
        this->GetOutputCodeInSparseMatrix(); // update
    }
   
    //--------------------------------------------------------------

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfFeatureDataVector_start, int_max IndexOfFeatureDataVector_end)
{
    DenseMatrix<ElementType> SingleFeatureDataVector(m_FeatureData->GetRowNumber(), 1);

    for (int_max i = IndexOfFeatureDataVector_start; i <= IndexOfFeatureDataVector_end; ++i)
    {
        m_FeatureData->GetCol(i, SingleFeatureDataVector);

        this->EncodingFunction(SingleFeatureDataVector, *m_Dictionary, (*m_CodeInSparseVectorList)[i]);
    }
}


template<typename ElementType>
inline
void FeatureDictionaryBasedSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                                                        const FeatureDictionary<ElementType>& InputDictionary,
                                                                        DenseMatrix<ElementType>& CodeInDenseMatrix)
{
    SparseMatrix<ElementType> CodeInSparseMatrix(m_Dictionary->m_Record.GetColNumber(), 1);

    this->EncodingFunction(SingleFeatureDataVector, InputDictionary, CodeInSparseMatrix);

    ConvertSparseMatrixToDenseMatrix(CodeInSparseMatrix, CodeInDenseMatrix);
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInDenseMatrix()
{
    if (m_Flag_CodeInDenseMatrix_Is_Updated == false)
    {
        auto CodeDimension = m_Dictionary->m_Record.GetColNumber();

        auto IsOK = m_CodeInDenseMatrix->FastResize(CodeDimension, m_FeatureData->GetColNumber());

        if (IsOK == true)
        {
            DenseMatrix<ElementType> tempCode(CodeDimension, 1);

            for (int_max j = 0; j < m_CodeInDenseMatrix->GetColNumber(); ++j)
            {
                ConvertSparseMatrixToDenseMatrix((*m_CodeInSparseVectorList)[j], tempCode);

                m_CodeInDenseMatrix->SetCol(j, tempCode);
            }
                
            if (m_CodeInDenseMatrix != &m_CodeInDenseMatrix_SharedCopy)
            {
                m_CodeInDenseMatrix_SharedCopy.ForceShare(m_CodeInDenseMatrix);
            }

            m_Flag_CodeInDenseMatrix_Is_Updated = true;
        }
        else
        {           
            MDK_Error("Size does not match and can not change @ FeatureDictionaryBasedSparseEncoder::GetOutputCodeInDenseMatrix()") 
        }
    }       

    return &m_CodeInDenseMatrix_SharedCopy;
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCode() // InDenseMatrix
{
    return this->GetOutputCodeInDenseMatrix();
}


template<typename ElementType>
SparseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInSparseMatrix()
{
    if (m_Flag_CodeInSparseMatrix_Is_Updated == false)
    {
        auto CodeDimension = m_Dictionary->m_Record.GetColNumber();

        m_CodeInSparseMatrix->ConstructFromSparseColVectorListInOrder(*m_CodeInSparseVectorList, CodeDimension, m_FeatureData->GetColNumber());

        if (m_CodeInSparseMatrix != &m_CodeInSparseMatrix_SharedCopy)
        {
            m_CodeInSparseMatrix_SharedCopy.ForceShare(m_CodeInSparseMatrix);
        }

        m_Flag_CodeInSparseMatrix_Is_Updated = true;
    }

    return &m_CodeInSparseMatrix_SharedCopy;
}


template<typename ElementType>
DenseMatrix<SparseMatrix<ElementType>>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInSparseVectorList()
{
    return &m_CodeInSparseVectorList_SharedCopy;
}


}// namespace mdk


#endif