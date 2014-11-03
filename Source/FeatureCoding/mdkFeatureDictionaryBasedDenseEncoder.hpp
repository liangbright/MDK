#ifndef __mdkFeatureDictionaryBasedDenseEncoder_hpp
#define __mdkFeatureDictionaryBasedDenseEncoder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedDenseEncoder<ElementType>::FeatureDictionaryBasedDenseEncoder()
{
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
	m_DenseCode.Clear();
    m_MinNumberOfDataPerThread = 1;
    m_MaxNumberOfThread = 1;
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
void FeatureDictionaryBasedDenseEncoder<ElementType>::SetMaxNumberOfThread(int_max Number)
{
    m_MaxNumberOfThread = Number;
}


template<typename ElementType>
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetNumberOfThreadsTobeCreated()
{
	auto TotalDataVectorNumber = this->GetTotalNumberOfInputFeatureDataVector();
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(TotalDataVectorNumber, m_MaxNumberOfThread, m_MinNumberOfDataPerThread);
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
int_max FeatureDictionaryBasedDenseEncoder<ElementType>::GetTotalNumberOfInputFeatureDataVector()
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

	if (m_Dictionary->BasisMatrix().GetRowNumber != m_FeatureData->GetColNumber())
    {
		MDK_Error << "m_Dictionary size not math DataVector length @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
		return false;
    }

    if (m_MaxNumberOfThread <= 0)
    {
        MDK_Warning << "input MaximunNumberOfThreads is invalid, set to 1 @ FeatureDictionaryBasedDenseEncoder::CheckInput()" << '\n';
        m_MaxNumberOfThread = 1;
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
	auto CodeLength = m_Dictionary->BasisMatrix().GetColNumber();
	auto DataNumber = m_FeatureData->GetColNumber();
	if (m_DenseCode.FastResize(CodeLength, DataNumber) == false)
	{
		return false;
	}
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
                  0, TotalDataVectorNumber - 1, m_MaxNumberOfThread, m_MinNumberOfDataPerThread);
    //------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        MDK_Error("Postprocess() return false @ FeatureDictionaryBasedDenseEncoder::Update()")
        return false;
    }

    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedDenseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex)
{
    for (int_max i = IndexOfDataVector_start; i <= IndexOfDataVector_end; ++i)
    {
		auto DataVector = m_FeatureData->RefCol(i);
		auto CodeVector = this->EncodeSingleDataVector(i, DataVector, ThreadIndex);
		m_DenseCode.SetCol(i, CodeVector);
    }
}


template<typename ElementType>
DenseMatrix<ElementType>* FeatureDictionaryBasedDenseEncoder<ElementType>::GetOutputCode()
{
	return &m_DenseCode;
}


}// namespace mdk


#endif