#ifndef __mdkSparseVector_hpp
#define __mdkSparseVector_hpp

namespace mdk
{

template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector()
{
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const ElementType& Element)
{
	m_Data = std::make_unique<SparseVectorData<ElementType>>();
	m_Data->Length = 1;
	m_Data->IndexList.push_back(0);
	m_Data->ElementList.push_back(Element);
	m_Data->ZeroElement = ElementType(0);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const SparseVector& InputVector)
{
	m_Data = std::make_unique<SparseVectorData<ElementType>>();
    this->Copy(InputVector);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(SparseVector&& InputVector)
{
	m_Data = std::move(InputVector.m_Data);
}


template<typename ElementType>
inline
SparseVector<ElementType>::~SparseVector()
{
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Construct(int_max Length)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}
	m_Data->Length = Length;
	m_Data->IndexList.resize(0);
	m_Data->ElementList.resize(0);
	m_Data->ZeroElement = ElementType(0);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& ElementList, int_max Length)
{
    if (IndexList.size() != ElementList.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::initializer_list ...)")
        return false;
    }

    return this->Construct(IndexList.begin(), ElementList.data(), int_max(ElementList.size()), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const std::vector<int_max>& IndexList, const std::vector<ElementType>& ElementList, int_max Length)

{
    if (IndexList.size() != ElementList.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::vector ...)")
        return false;
    }

    return this->Construct(IndexList.data(), ElementList.data(), int_max(ElementList.size()), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Construct(const DenseVector<int_max>& IndexList, const DenseVector<ElementType>& ElementList, int_max Length)
{
	if (IndexList.GetLength() != ElementList.GetLength())
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(DenseVector ...)")
		return false;
	}

	return this->Construct(IndexList.GetPointer(), ElementList.GetPointer(), ElementList.GetLength(), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& ElementList, int_max Length)
{
    if (IndexList.GetElementNumber() != ElementList.GetElementNumber())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(mdk::DenseMatrix ...)")
        return false;
    }

    return this->Construct(IndexList.GetElementPointer(), ElementList.GetElementPointer(), ElementList.GetElementNumber(), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Construct(const int_max* IndexList, const ElementType* ElementList, int_max RecordedElementNumber, int_max Length)
{
    if (IndexList == nullptr || ElementList == nullptr || RecordedElementNumber <= 0 || Length <= 0)
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
	m_Data->IndexList.resize(RecordedElementNumber);
	m_Data->ElementList.resize(RecordedElementNumber);
	m_Data->ZeroElement = ElementType(0);
    //--------------------------------------------------------------

    std::vector<int_max> LinearIndex_In_Input_IndexList(RecordedElementNumber);

	Sort(IndexList, RecordedElementNumber, m_Data->IndexList.data(), LinearIndex_In_Input_IndexList.data(), "ascend");

    //--------------------------------------------------------------

    for (int_max i = 0; i < RecordedElementNumber; ++i)
    {
		m_Data->ElementList[i] = ElementList[LinearIndex_In_Input_IndexList[i]];
    }

    return true;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::ConstructFromSortedData(std::vector<int_max> IndexList, std::vector<ElementType> ElementList, int_max Length)
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
void SparseVector<ElementType>::ConstructFromSortedData(DenseVector<int_max> IndexList, DenseVector<ElementType> ElementList, int_max Length)
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
    this->Take(InputVector);
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::Clear()
{
	if (m_Data)
	{
		m_Data->Length = 0;
		m_Data->IndexList.clear();
		m_Data->ElementList.clear();
		m_Data->ZeroElement = ElementType(0);
	}    
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Reset()
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
        this->Construct(InputLength);
        return;
    }

    //------ not empty , not equal to input size ---------------------//

	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}

	if (InputLength < m_Data->Length)
    {
		auto RecordedElementNumber = int_max(m_Data->IndexList.size());

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

		m_Data->IndexList.erase(m_Data->IndexList.begin() + StartIndex_erase, m_Data->IndexList.end());
		m_Data->ElementList.erase(m_Data->ElementList.begin() + StartIndex_erase, m_Data->ElementList.end());
    }

    m_Data->Length = InputLength;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::FastResize(int_max InputLength)
{
    this->Construct(InputLength);
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
ElementType& SparseVector<ElementType>::operator[](int_max Index)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<SparseVectorData<ElementType>>();
	}

	if (Index >= m_Data->Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::operator[](int_max Index)")
		return m_Data->ZeroElement;
	}

	auto RecordedElementNumber = int_max(m_Data->ElementList.size());

	int_max IndexInDataArray = -1;
	int_max IndexInDataArray_insert = RecordedElementNumber;

	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		if (m_Data->IndexList[i] == Index)
		{
			IndexInDataArray = i;
			break;
		}
		else if (m_Data->IndexList[i] > Index)
		{
			IndexInDataArray_insert = i;
		}
	}

	if (IndexInDataArray >= 0)
	{
		return m_Data->ElementList[IndexInDataArray];
	}
	else
	{
		m_Data->IndexList.reserve(m_Data->IndexList.size() + 1);
		m_Data->IndexList.insert(m_Data->IndexList.begin() + IndexInDataArray_insert, Index);
		m_Data->ElementList.reserve(m_Data->ElementList.size() + 1);
		m_Data->ElementList.insert(m_Data->ElementList.begin() + IndexInDataArray_insert, m_Data->ZeroElement);
		return m_Data->ElementList[IndexInDataArray_insert];
	}
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::operator[](int_max Index) const
{
	if (Index >= m_Data->Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::operator[](int_max Index)")
		return m_Data->ZeroElement;
	}

	int_max IndexInDataArray = -1;
	for (int_max i = 0; i < int_max(m_Data->IndexList.size()); ++i)
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
ElementType& SparseVector<ElementType>::operator()(int_max Index)
{
	return this->operator[](Index);
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::GetElement(int_max Index) const
{
	return this->operator[](Index);
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::operator()(int_max Index) const
{
	return this->operator[](Index);
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
int_max SparseVector<ElementType>::GetRecordedElementNumber() const
{
	if (m_Data)
	{
		return int_max(m_Data->ElementList.size());
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
		return m_Data->ElementList.data();
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
		return m_Data->ElementList.data();
	}
	else
	{
		return nullptr;
	}
}


template<typename ElementType>
inline
std::vector<int_max>& SparseVector<ElementType>::IndexList()
{
    return m_Data->IndexList;
}


template<typename ElementType>
inline
const std::vector<int_max>& SparseVector<ElementType>::IndexList() const
{
    return m_Data->IndexList;
}


template<typename ElementType>
inline
std::vector<ElementType>& SparseVector<ElementType>::ElementList()
{
    return m_Data->ElementList;
}


template<typename ElementType>
inline
const std::vector<ElementType>& SparseVector<ElementType>::ElementList() const
{
    return m_Data->ElementList;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::Sum() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_Data->ElementList.size()); ++k)
    {
        Value += m_Data->ElementList[k];
    }

    return Value;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L1Norm() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_Data->ElementList.size()); ++k)
    {
        Value += std::abs(m_Data->ElementList[k]);
    }

    return Value;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L2Norm() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_Data->ElementList.size()); ++k)
    {
        Value += m_Data->ElementList[k] * m_Data->ElementList[k];
    }

    Value = std::sqrt(Value);

    return Value;
}


}//end namespace mdk

#endif