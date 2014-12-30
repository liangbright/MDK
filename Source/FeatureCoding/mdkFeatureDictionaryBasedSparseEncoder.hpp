#ifndef __mdkFeatureDictionaryBasedSparseEncoder_hpp
#define __mdkFeatureDictionaryBasedSparseEncoder_hpp

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
    m_MinNumberOfDataPerThread = 1;
    m_MaxNumberOfThread = 1;
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
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetMaxNumberOfThread(int_max Number)
{
    if (Number <= 0)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBasedSparseEncoder::SetMaxNumberOfThread(int_max Number)")
        m_MaxNumberOfThread = 1;
        return;
    }
    m_MaxNumberOfThread = Number;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetNumberOfThreadTobeCreated()
{
    auto TotalDataVectorNumber = this->GetTotalNumberOfInputFeatureDataVector();

    return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(TotalDataVectorNumber, m_MaxNumberOfThread, m_MinNumberOfDataPerThread);
}


template<typename ScalarType>
void FeatureDictionaryBasedSparseEncoder<ScalarType>::SetMinNumberOfDataPerThread(int_max Number)
{
    m_MinNumberOfDataPerThread = Number;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetMinNumberOfDataPerThread()
{
    return m_MinNumberOfDataPerThread;
}


template<typename ScalarType>
int_max FeatureDictionaryBasedSparseEncoder<ScalarType>::GetTotalNumberOfInputFeatureDataVector()
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

    if (m_MaxNumberOfThread <= 0)
    {
        MDK_Warning("Input MaxNumberOfThread is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        m_MaxNumberOfThread = 1;
    }

    if (m_MinNumberOfDataPerThread <= 0)
    {
        MDK_Warning("input m_MinNumberOfDataPerThread is invalid, set to 1 @ FeatureDictionaryBasedSparseEncoder::CheckInput()")
        m_MinNumberOfDataPerThread = 1;
    }

    return true;
}


template<typename ScalarType>
bool FeatureDictionaryBasedSparseEncoder<ScalarType>::Preprocess()
{
	m_SparseCode.FastResize(m_FeatureData->GetColNumber());
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

    int_max TotalDataVectorNumber = this->GetTotalNumberOfInputFeatureDataVector();

    m_ThreadStatus.Resize(1, this->GetNumberOfThreadTobeCreated());
    m_ThreadStatus.Fill(0);

    // multi-thread -----------------------------------------------------------------

    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->GenerateCode_in_a_Thread(Index_start, Index_end, ThreadIndex); },
                  0, TotalDataVectorNumber - 1, m_MaxNumberOfThread, m_MinNumberOfDataPerThread);
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
    for (int_max k = 0; k < m_ThreadStatus.GetElementNumber(); ++k)
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
void FeatureDictionaryBasedSparseEncoder<ScalarType>::GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, 
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

	auto BasisNumber = m_Dictionary->BasisMatrix().GetColNumber();

	auto IsOK = OutputCode.FastResize(BasisNumber, m_FeatureData->GetColNumber());
	if (IsOK == true)
	{
		for (int_max j = 0; j < m_FeatureData->GetColNumber(); ++j)
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
    auto BasisNumber = m_Dictionary->BasisMatrix().GetColNumber();
	OutputCode.ConstructFromSparseColVectorSetInOrder(m_SparseCode, BasisNumber, m_FeatureData->GetColNumber());
    return OutputCode;
}

}// namespace mdk


#endif