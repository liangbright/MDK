#ifndef mdk_SparseVector_hpp
#define mdk_SparseVector_hpp

namespace mdk
{

template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector()
{
	this->Initialize(0);
}

template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const MDK_Symbol_PureEmpty&)
{
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const ElementType& Element)
{
	this->Initialize(1);
	m_Data->IndexList.Append(0);
	m_Data->ElementList.Append(Element);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const SparseVector& InputVector)
{
	this->Initialize(0);
    this->Copy(InputVector);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(SparseVector&& InputVector)
{
	(*this) = std::move(InputVector);
}


template<typename ElementType>
inline
SparseVector<ElementType>::~SparseVector()
{
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Initialize(int_max Length)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}
	m_Data->Length = Length;
	m_Data->IndexList.Clear();
	m_Data->ElementList.Clear();
	m_Data->ZeroElement = ElementType(0);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Initialize(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& ElementList, int_max Length)
{
    if (IndexList.size() != ElementList.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::initializer_list ...)")
        return false;
    }

    return this->Initialize(IndexList.begin(), ElementList.data(), int_max(ElementList.size()), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Initialize(const std::vector<int_max>& IndexList, const std::vector<ElementType>& ElementList, int_max Length)
{
    if (IndexList.size() != ElementList.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::vector ...)")
        return false;
    }

    return this->Initialize(IndexList.data(), ElementList.data(), int_max(ElementList.size()), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Initialize(const DenseVector<int_max>& IndexList, const DenseVector<ElementType>& ElementList, int_max Length)
{
	if (IndexList.GetLength() != ElementList.GetLength())
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(DenseVector ...)")
		return false;
	}

	return this->Initialize(IndexList.GetPointer(), ElementList.GetPointer(), ElementList.GetLength(), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Initialize(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& ElementList, int_max Length)
{
    if (IndexList.GetElementNumber() != ElementList.GetElementNumber())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(DenseMatrix ...)")
        return false;
    }

    return this->Initialize(IndexList.GetElementPointer(), ElementList.GetElementPointer(), ElementList.GetElementNumber(), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Initialize(const DenseVector<int_max>& IndexList, const DenseMatrix<ElementType>& ElementList, int_max Length)
{
	if (IndexList.GetElementNumber() != ElementList.GetElementNumber())
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(DenseVector, DenseMatrix, ...)")
		return false;
	}

	return this->Initialize(IndexList.GetElementPointer(), ElementList.GetElementPointer(), ElementList.GetElementNumber(), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Initialize(const DenseMatrix<int_max>& IndexList, const DenseVector<ElementType>& ElementList, int_max Length)
{
	if (IndexList.GetElementNumber() != ElementList.GetElementNumber())
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(DenseMatrix, DenseVector, ...)")
		return false;
	}

	return this->Initialize(IndexList.GetElementPointer(), ElementList.GetElementPointer(), ElementList.GetElementNumber(), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Initialize(const int_max* IndexList, const ElementType* ElementList, int_max RecordedElementNumber, int_max Length)
{
	if (IndexList == nullptr && ElementList == nullptr && RecordedElementNumber == 0 && Length == 0)
	{
		this->Initialize(0);
		return true;
	}
	else if ((IndexList == nullptr && ElementList != nullptr) 
		     || (IndexList != nullptr && ElementList == nullptr)
		     || (IndexList == nullptr && RecordedElementNumber > 0)
			 || (ElementList == nullptr && RecordedElementNumber > 0)
			 || RecordedElementNumber < 0
			 || Length < 0)
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(pointer ...)")
		return false;
	}

    //--------------------------------------------------------------
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}
	m_Data->Length = Length;
	m_Data->IndexList.FastResize(RecordedElementNumber);
	m_Data->ElementList.FastResize(RecordedElementNumber);
	m_Data->ZeroElement = ElementType(0);
    //--------------------------------------------------------------

	auto tempRelativeIndexList = m_Data->IndexList.Sort("ascend");

    for (int_max i = 0; i < RecordedElementNumber; ++i)
    {
		m_Data->IndexList[i] = IndexList[tempRelativeIndexList[i]];
		m_Data->ElementList[i] = ElementList[tempRelativeIndexList[i]];
    }

    return true;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::InitializeFromSortedData(std::vector<int_max> IndexList, std::vector<ElementType> ElementList, int_max Length)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}
	m_Data->Length = Length;
	m_Data->IndexList = std::move(IndexList);
	m_Data->ElementList = std::move(ElementList);
	m_Data->ZeroElement = ElementType(0);
}


template<typename ElementType>
inline
void SparseVector<ElementType>::InitializeFromSortedData(DenseVector<int_max> IndexList, DenseVector<ElementType> ElementList, int_max Length)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}
	m_Data->Length = Length;
	m_Data->IndexList = std::move(IndexList.StdVector());
	m_Data->ElementList = std::move(ElementList.StdVector());
	m_Data->ZeroElement = ElementType(0);
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::operator=(const SparseVector& InputVector)
{
    this->Copy(InputVector);
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::operator=(SparseVector&& InputVector)
{
	m_Data = std::move(InputVector.m_Data);
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::Clear()
{
	if (m_Data)
	{
		m_Data->Length = 0;
		m_Data->IndexList.Clear();
		m_Data->ElementList.Clear();
		m_Data->ZeroElement = ElementType(0);
	}    
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid InputLength @ SparseVector::Resize(...)")
        return;
    }

	if (InputLength == m_Data->Length)
    {
        return;
    }

	if (InputLength == 0 && m_Data->Length > 0)
    {
        this->Clear();
        return;
    }

	if (InputLength > 0 && m_Data->Length == 0)
    {
        this->Initialize(InputLength);
        return;
    }

    //------ not empty , not equal to input size ---------------------//

	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}

	if (InputLength < m_Data->Length)
    {
		auto RecordedElementNumber = int_max(m_Data->IndexList.GetLength());

        auto StartIndex_erase = RecordedElementNumber;

        for (int_max i = RecordedElementNumber - 1; i >= 0; --i)
        {
			if (m_Data->IndexList[i] >= InputLength)
            {
                StartIndex_erase = i;
            }
            else
            {
                break;
            }
        }

		m_Data->IndexList.Delete(StartIndex_erase, m_Data->IndexList.GetLength()-1);
		m_Data->ElementList.Delete(StartIndex_erase, m_Data->ElementList.GetLength()-1);
    }

    m_Data->Length = InputLength;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::FastResize(int_max InputLength)
{
    this->Initialize(InputLength);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::IsEmpty() const
{
	if (!m_Data)
	{
		return true;
	}
	else
	{
		return (m_Data->Length <= 0);
	}
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::IsPureEmpty() const
{
	return (!m_Data);
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::operator[](int_max Index) const
{
	return this->GetElement(Index);
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::operator()(int_max Index) const
{
	return this->GetElement(Index);
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::GetElement(int_max Index) const
{
	if (Index >= m_Data->Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::GetElement(int_max Index)")
		return m_Data->ZeroElement;
	}

	int_max IndexInDataArray = -1;
	for (int_max i = 0; i < m_Data->IndexList.GetLength(); ++i)
	{
		if (m_Data->IndexList[i] == Index)
		{
			IndexInDataArray = i;
			break;
		}
	}

	if (IndexInDataArray >= 0)
	{
		return m_Data->ElementList[IndexInDataArray];
	}
	else
	{
		MDK_Error("Invalid Index @ SparseVector::GetElement(Index)")
		return m_Data->ZeroElement;
	}
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::SetElement(int_max Index, const ElementType& Element)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}

	if (Index >= m_Data->Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::SetElement(...)")
		return false;
	}

	auto RecordedElementNumber = m_Data->ElementList.GetLength();

	int_max IndexInElementList = -1;
	int_max IndexInElementList_insert = RecordedElementNumber;

	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		if (m_Data->IndexList[i] == Index)
		{
			IndexInElementList = i;
			break;
		}
		else if (m_Data->IndexList[i] > Index)
		{
			IndexInElementList_insert = i;
		}
	}

	if (IndexInElementList >= 0)
	{
		m_Data->ElementList[IndexInElementList] = Element;
	}
	else
	{
		m_Data->IndexList.Insert(IndexInElementList_insert, Index);
		m_Data->ElementList.Insert(IndexInElementList_insert, Element);
	}
	
	return true;
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::Copy(const SparseVector<ElementType>& InputVector)
{
	if (InputVector.m_Data)
	{
		if (!m_Data)
		{
			m_Data = std::make_unique<SparseVectorData<ElementType>>();
		}
		m_Data->Length = InputVector.m_Data->Length;
		m_Data->IndexList = InputVector.m_Data->IndexList;
		m_Data->ElementList = InputVector.m_Data->ElementList;
		m_Data->ZeroElement = InputVector.m_Data->ZeroElement;
	}
	else
	{
		this->Clear();
	}
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Take(SparseVector<ElementType>& InputVector)
{
	if (InputVector.m_Data)
	{
		if (!m_Data)
		{
			m_Data = std::make_unique<SparseVectorData<ElementType>>();
		}
		m_Data->Length = InputVector.m_Data->Length;
		m_Data->IndexList = std::move(InputVector.m_Data->IndexList);
		m_Data->ElementList = std::move(InputVector.m_Data->ElementList);
		m_Data->ZeroElement = InputVector.m_Data->ZeroElement;

		InputVector.m_Data->Length = 0;
	}
	else
	{
		this->Clear();
	}
}


template<typename ElementType>
inline int_max SparseVector<ElementType>::GetLength() const
{
	if (m_Data)
	{
		return m_Data->Length;
	}
	else
	{
		return 0;
	}
}


template<typename ElementType>
inline
int_max SparseVector<ElementType>::GetRecordedElementCount() const
{
	if (m_Data)
	{
		return m_Data->ElementList.GetLength();
	}
	else
	{
		return 0;
	}    
}


template<typename ElementType>
inline
ElementType* SparseVector<ElementType>::GetPointerOfBeginElement()
{
	if (m_Data)
	{
		return m_Data->ElementList.GetElementPointer();
	}
	else
	{
		return nullptr;
	}    
}


template<typename ElementType>
inline
const ElementType* SparseVector<ElementType>::GetPointerOfBeginElement() const
{
	if (m_Data)
	{
		return m_Data->ElementList.GetElementPointer();
	}
	else
	{
		return nullptr;
	}
}


template<typename ElementType>
inline
DenseVector<int_max>& SparseVector<ElementType>::IndexList()
{
    return m_Data->IndexList;
}


template<typename ElementType>
inline
const DenseVector<int_max>& SparseVector<ElementType>::IndexList() const
{
    return m_Data->IndexList;
}


template<typename ElementType>
inline
DenseVector<ElementType>& SparseVector<ElementType>::ElementList()
{
    return m_Data->ElementList;
}


template<typename ElementType>
inline
const DenseVector<ElementType>& SparseVector<ElementType>::ElementList() const
{
    return m_Data->ElementList;
}

//------------------------------------------ Convert to DenseMatrix or DenseVector --------------------------------------//
template<typename ElementType>
DenseMatrix<ElementType> SparseVector<ElementType>::ConvertToDenseMatrixAsColVector() const
{
	DenseMatrix<ElementType> OutputVector;
	if (OutputVector.Resize(m_Data->Length, 1) == false)
	{
		return OutputVector;
	}
	OutputVector.Fill(0);

	auto RecordedElementNumber = m_Data->IndexList.GetLength();
	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		OutputVector[m_Data->IndexList[i]] = m_Data->ElementList[i];
	}
	return OutputVector;
}


template<typename ElementType>
DenseMatrix<ElementType> SparseVector<ElementType>::ConvertToDenseMatrixAsRowVector() const
{
	DenseMatrix<ElementType> OutputVector;
	if (OutputVector.Resize(1, m_Data->Length) == false)
	{
		return OutputVector;
	}
	OutputVector.Fill(0);

	auto RecordedElementNumber = m_Data->IndexList.GetLength();
	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		OutputVector[m_Data->IndexList[i]] = m_Data->ElementList[i];
	}
	return OutputVector;
}


template<typename ElementType>
DenseVector<ElementType> SparseVector<ElementType>::ConvertToDenseVector() const
{
	DenseVector<ElementType> OutputVector;
	if (OutputVector.Resize(m_Data->Length) == false)
	{
		return false;
	}
	OutputVector.Fill(0);

	auto RecordedElementNumber = m_Data->IndexList.GetLength();
	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		OutputVector[m_Data->IndexList[i]] = m_Data->ElementList[i];
	}
	return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
ElementType SparseVector<ElementType>::Sum() const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ SparseVector::Sum()")
		return ElementType(0);
	}

	return m_Data->ElementList.Sum();
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L1Norm() const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ SparseVector::L1Norm()")
		return ElementType(0);
	}
   
	return m_Data->ElementList.L1Norm();
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L2Norm() const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ SparseVector::L2Norm()")
		return ElementType(0);
	}

	return m_Data->ElementList.L2Norm();
}


}//end namespace mdk

#endif