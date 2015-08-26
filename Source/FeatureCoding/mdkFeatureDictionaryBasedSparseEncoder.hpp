#ifndef mdk_FeatureDictionaryBasedSparseEncoder_hpp
#define mdk_FeatureDictionaryBasedSparseEncoder_hpp

namespace mdk
{

template<typename ScalarType>
FeatureDictionaryBasedSparseEncoder<ScalarType>::FeatureDictionaryBasedSparseEncoder()
{
    this->Clear();
}


template<typename ScalarType>
FeatureDictionaryBasedSparseEncoder<ScalarType>::~FeatureDictionaryBasedSparseEncoder()
{
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::Clear()
{
    m_FeatureData = nullptr;
    m_Dictionary  = nullptr;
	m_SparseCode.Clear();
    m_MinCountOfDataPerThread = 1;
    m_MaxThreadCount = 1;
    m_ThreadStatus.Clear();
    m_ThreadStatus.FastResize(1, 1);
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* Dictionary)
{
    m_Dictionary = Dictionary;
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetMaxThreadCount(int_max Count)
{
    if (Count <= 0)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetMaxCountOfThread(int_max Count)")
        m_MaxThreadCount = 1;
        return;
    }
    m_MaxThreadCount = Count;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetCountOfThreadTobeCreated()
{
    auto TotalDataVectorCount = this->GetTotalCountOfInputFeatureDataVector();

    return Compute_CountOfThreadTobeCreated_For_ParallelBlock(TotalDataVectorCount, m_MaxThreadCount, m_MinCountOfDataPerThread);
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetMinCountOfDataPerThread(int_max Count)
{
    m_MinCountOfDataPerThread = Count;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetMinCountOfDataPerThread()
{
    return m_MinCountOfDataPerThread;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetTotalCountOfInputFeatureDataVector()
{
    if (m_FeatureData != nullptr)
    {
        return m_FeatureData->GetColCount();
    }
    else
    {
        return 0;
    }
}


template<typename ScalarType>
bool FeatureDictionaryBasedSparseEncoder<ScalarType>::CheckInput()
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

    if (m_MaxThreadCount <= 0)
    {
        MDK_Warning("Input MaxCountOfThread is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        m_MaxThreadCount = 1;
    }

    if (m_MinCountOfDataPerThread <= 0)
    {
        MDK_Warning("input m_MinCountOfDataPerThread is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        m_MinCountOfDataPerThread = 1;
    }

    return true;
}


template<typename ScalarType>
bool FeatureDictionaryBasedSparseEncoder<ScalarType>::Preprocess()
{
	m_SparseCode.FastResize(m_FeatureData->GetColCount());
    return true;
}


template<typename ScalarType>
bool FeatureDictionaryBasedSparseEncoder<ScalarType>::Postprocess()
{
    return true;
}


template<typename ScalarType>
bool FeatureDictionaryBasedSparseEncoder<ScalarType>::Update()
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

    int_max TotalDataVectorCount = this->GetTotalCountOfInputFeatureDataVector();

    m_ThreadStatus.Resize(1, this->GetCountOfThreadTobeCreated());
    m_ThreadStatus.Fill(0);

    // multi-thread -----------------------------------------------------------------

    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->GenerateCode_in_a_thread(Index_start, Index_end, ThreadIndex); },
                  0, TotalDataVectorCount - 1, m_MaxThreadCount, m_MinCountOfDataPerThread);
    //------------------------------------------------------------

	this->CheckThreadStatus();

    if (this->Postprocess() == false)
    {
        MDK_Error("Postprocess() return false @ FeatureDictionaryBasedSparseEncoder::Update()")
        return false;
    }

    return true;
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::CheckThreadStatus()
{
    for (int_max k = 0; k < m_ThreadStatus.GetElementCount(); ++k)
    {
        if (m_ThreadStatus[k] == 0)
        {
            MDK_Error("m_ThreadStatus: " << k << " is 0 @ FeatureDictionaryBasedSparseEncoder::CheckThreadStatus()")
        }
        else if (m_ThreadStatus[k] == 1)
        {
            MDK_Error("m_ThreadStatus: " << k << " is 1 @ FeatureDictionaryBasedSparseEncoder::CheckThreadStatus()")
        }
    }
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::GenerateCode_in_a_thread(int_max IndexOfDataVector_start, 
                                                                               int_max IndexOfDataVector_end, 
                                                                               int_max ThreadIndex)
{
    //std::cout << "FeatureDictionaryBasedSparseEncoder" << '\n';
    //std::cout << "id: " << ThreadIndex << ", s: " << IndexOfDataVector_start << ", e: " << IndexOfDataVector_end << '\n';
    //std::cout << ThreadIndex;

    m_ThreadStatus[ThreadIndex] = 1;

    for (int_max i = IndexOfDataVector_start; i <= IndexOfDataVector_end; ++i)
    {
		auto FeatureVector = m_FeatureData->RefCol(i);
		m_SparseCode[i] = this->EncodeSingleDataVector(i, FeatureVector, ThreadIndex);
    }

    m_ThreadStatus[ThreadIndex] = 2;
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>>* FeatureDictionaryBasedSparseEncoder<ScalarType>::GetOutputCode()
{
	return &m_SparseCode;
}


template<typename ScalarType>
DenseMatrix<ScalarType> FeatureDictionaryBasedSparseEncoder<ScalarType>::ConvertOutputCodeToDenseMatrix()
{
	DenseMatrix<ScalarType> OutputCode;

	auto BasisCount = m_Dictionary->BasisMatrix().GetColCount();

	auto IsOK = OutputCode.FastResize(BasisCount, m_FeatureData->GetColCount());
	if (IsOK == true)
	{
		for (int_max j = 0; j < m_FeatureData->GetColCount(); ++j)
		{
			auto tempCode = m_SparseCode[j].CreateDenseVector();

			OutputCode.SetCol(j, tempCode);
		}
	}

	return OutputCode;
}


template<typename ScalarType>
SparseMatrix<ScalarType> FeatureDictionaryBasedSparseEncoder<ScalarType>::ConvertOutputCodeToSparseMatrix()
{
	SparseMatrix<ScalarType> OutputCode;
    auto BasisCount = m_Dictionary->BasisMatrix().GetColCount();
	OutputCode.ConstructFromSparseColVectorSetInOrder(m_SparseCode, BasisCount, m_FeatureData->GetColCount());
    return OutputCode;
}

}// namespace mdk


#endif