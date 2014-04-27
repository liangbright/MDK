#ifndef __mdkFeatureDictionaryBasedSparseEncoder_hpp
#define __mdkFeatureDictionaryBasedSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedSparseEncoder<ElementType>::FeatureDictionaryBasedSparseEncoder()
{
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

    this->SetupDefaultPipelineOutput();

    m_Flag_Output_CodeInDenseMatrix = false;

    m_Flag_Output_CodeInSparseMatrix = false;

    m_Flag_CodeInDenseMatrix_Is_Updated = false;

    m_Flag_CodeInSparseMatrix_Is_Updated = false;

    m_MinNumberOfDataPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetupDefaultPipelineOutput()
{
    m_CodeInDenseMatrix_SharedCopy.Clear();

    m_CodeInDenseMatrix = &m_CodeInDenseMatrix_SharedCopy;

    m_CodeInSparseMatrix_SharedCopy.Clear();

    m_CodeInSparseMatrix = &m_CodeInSparseMatrix_SharedCopy;

    m_CodeInSparseColVectorSet_SharedCopy.Clear();

    m_CodeInSparseColVectorSet = &m_CodeInSparseColVectorSet_SharedCopy;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::UpdatePipelineOutput()
{
    if (m_CodeInSparseColVectorSet != &m_CodeInSparseColVectorSet_SharedCopy)
    {
        m_CodeInSparseColVectorSet_SharedCopy.ForceShare(m_CodeInSparseColVectorSet);
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
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary)
{
    m_Dictionary = Dictionary;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCode(DenseMatrix<ElementType>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCode(...)")
        return;
    }

    m_CodeInDenseMatrix = Code;

    m_CodeInDenseMatrix_SharedCopy.ForceShare(Code);

    m_Flag_Output_CodeInDenseMatrix = true;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCode(SparseMatrix<ElementType>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCode(...)")
        return;
    }

    m_CodeInSparseMatrix = Code;

    m_CodeInSparseMatrix_SharedCopy.ForceShare(Code);

    m_Flag_Output_CodeInSparseMatrix = true;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetOutputCode(DataContainer<SparseVector<ElementType>>* Code)
{
    if (Code == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetOutputCode(...)")
        return;
    }

    m_CodeInSparseColVectorSet = Code;

    m_CodeInSparseColVectorSet_SharedCopy.ForceShare(Code);
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetMaxNumberOfThreads(int_max Number)
{
    if (Number <= 0)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetMaxNumberOfThreads(int_max Number)")

        m_MaxNumberOfThreads = 1;

        return;
    }

    m_MaxNumberOfThreads = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedSparseEncoder<ElementType>::GetNumberOfThreadsTobeCreated()
{
    auto TotalDataVectorNumber = GetTotalNumberOfInputFeatureDataVectors();

    return Compute_NecessaryNumberOfThreads_For_ParallelBlock(TotalDataVectorNumber, m_MaxNumberOfThreads, m_MinNumberOfDataPerThread);
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::SetMinNumberOfDataPerThread(int_max Number)
{
    m_MinNumberOfDataPerThread = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedSparseEncoder<ElementType>::GetMinNumberOfDataPerThread()
{
    return m_MinNumberOfDataPerThread;
}


template<typename ElementType>
int_max FeatureDictionaryBasedSparseEncoder<ElementType>::GetTotalNumberOfInputFeatureDataVectors()
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
bool FeatureDictionaryBasedSparseEncoder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("Input FeatureData is empty (nullptr) @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("Input FeatureData is empty (matrix) @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Dictionary == nullptr)
    {
        MDK_Error("Input Dictionary is empty (nullptr) @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Input Dictionary is empty (matrix) @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        return false;
    }

    m_CodeInSparseColVectorSet->FastResize(m_FeatureData->GetColNumber());

    if (m_MaxNumberOfThreads <= 0)
    {
        MDK_Warning("Input MaxNumberOfThreads is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")

        m_MaxNumberOfThreads = 1;
    }

    if (m_MinNumberOfDataPerThread <= 0)
    {
        MDK_Warning("input m_MinNumberOfDataPerThread is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")

        m_MinNumberOfDataPerThread = 1;
    }

    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::Preprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::Postprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedSparseEncoder<ElementType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    if (this->Preprocess() == false)
    {
        MDK_Error("Preprocess() return false @ FeatureDictionaryBasedSparseEncoder::Update()")
        return false;
    }

    int_max TotalDataVectorNumber = this->GetTotalNumberOfInputFeatureDataVectors();

    // multi-thread -----------------------------------------------------------------

    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->GenerateCode_in_a_Thread(Index_start, Index_end, ThreadIndex); },
                  0, TotalDataVectorNumber - 1, m_MaxNumberOfThreads, m_MinNumberOfDataPerThread);
    //------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        MDK_Error("Postprocess() return false @ FeatureDictionaryBasedSparseEncoder::Update()")
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedSparseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, 
                                                                                int_max IndexOfDataVector_end, 
                                                                                int_max ThreadIndex)
{
    for (int_max i = IndexOfDataVector_start; i <= IndexOfDataVector_end; ++i)
    {
        this->EncodingFunction(i, ThreadIndex);
    }
}


template<typename ElementType>
inline
void FeatureDictionaryBasedSparseEncoder<ElementType>::EncodeSingleDataVector(DenseMatrix<ElementType>& CodeInDenseColVector,
                                                                              const DenseMatrix<ElementType>& DataColVector)
{
    SparseVector<ElementType> CodeInSparseColVector;

    this->EncodeSingleDataVector(CodeInSparseColVector, DataColVector);

    ConvertSparseVectorToDenseMatrixAsColVector(CodeInSparseColVector, CodeInDenseColVector);
}


template<typename ElementType>
inline
void FeatureDictionaryBasedSparseEncoder<ElementType>::EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
                                                                              const DenseMatrix<ElementType>& DataColVector)
{
    this->SetInputFeatureData(&DataColVector);

    this->Update();

    CodeInSparseColVector = std::move((*m_CodeInSparseColVectorSet)[0]);
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCode() // InDenseMatrix
{
    return this->GetOutputCodeInDenseMatrix();
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInDenseMatrix()
{
    if (m_Flag_CodeInDenseMatrix_Is_Updated == false)
    {
        auto CodeLength = m_Dictionary->BasisMatrix().GetColNumber();

        auto IsOK = m_CodeInDenseMatrix->FastResize(CodeLength, m_FeatureData->GetColNumber());

        if (IsOK == true)
        {
            DenseMatrix<ElementType> tempCode(CodeLength, 1);

            for (int_max j = 0; j < m_CodeInDenseMatrix->GetColNumber(); ++j)
            {
                ConvertSparseVectorToDenseMatrixAsColVector((*m_CodeInSparseColVectorSet)[j], tempCode);

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
SparseMatrix<ElementType>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInSparseMatrix()
{
    if (m_Flag_CodeInSparseMatrix_Is_Updated == false)
    {
        auto CodeLength = m_Dictionary->BasisMatrix().GetColNumber();

        m_CodeInSparseMatrix->ConstructFromSparseColVectorSetInOrder(*m_CodeInSparseColVectorSet, CodeLength, m_FeatureData->GetColNumber());

        if (m_CodeInSparseMatrix != &m_CodeInSparseMatrix_SharedCopy)
        {
            m_CodeInSparseMatrix_SharedCopy.ForceShare(m_CodeInSparseMatrix);
        }

        m_Flag_CodeInSparseMatrix_Is_Updated = true;
    }

    return &m_CodeInSparseMatrix_SharedCopy;
}


template<typename ElementType>
DataContainer<SparseVector<ElementType>>* FeatureDictionaryBasedSparseEncoder<ElementType>::GetOutputCodeInSparseColVectorSet()
{
    return &m_CodeInSparseColVectorSet_SharedCopy;
}


}// namespace mdk


#endif