#ifndef __mdkSparseVector_hpp
#define __mdkSparseVector_hpp

namespace mdk
{

template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector()
{
    this->Clear();
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const ElementType& Element)
{
	m_Length = 1;
	m_IndexList.push_back(0);
	m_DataArray.push_back(Element);
	m_ZeroElement = ElementType(0);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(const SparseVector& InputVector)
{
    this->Copy(InputVector);
}


template<typename ElementType>
inline
SparseVector<ElementType>::SparseVector(SparseVector&& InputVector)
{
    m_Length = InputVector.m_Length;
    m_IndexList = std::move(InputVector.m_IndexList);
    m_DataArray = std::move(InputVector.m_DataArray);
	m_ZeroElement = ElementType(0);

    InputVector.m_Length = 0;
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
    m_Length = Length;
    m_IndexList.resize(0);
    m_DataArray.resize(0);
	m_ZeroElement = ElementType(0);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& DataArray, int_max Length)
{
    if (IndexList.size() != DataArray.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::initializer_list ...)")
        return false;
    }

    return this->Construct(IndexList.begin(), DataArray.data(), int_max(DataArray.size()), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const std::vector<int_max>& IndexList, const std::vector<ElementType>& DataArray, int_max Length)

{
    if (IndexList.size() != DataArray.size())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(std::vector ...)")
        return false;
    }

    return this->Construct(IndexList.data(), DataArray.data(), int_max(DataArray.size()), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Construct(const DenseVector<int_max>& IndexList, const DenseVector<ElementType>& DataArray, int_max Length)

{
	if (IndexList.GetLength() != DataArray.GetLength())
	{
		MDK_Error("Invalid Input @ SparseVector::Construct(DenseVector ...)")
		return false;
	}

	return this->Construct(IndexList.GetPointer(), DataArray.GetPointer(), DataArray.GetLength(), Length);
}


template<typename ElementType>
inline 
bool SparseVector<ElementType>::Construct(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& DataArray, int_max Length)
{
    if (IndexList.GetElementNumber() != DataArray.GetElementNumber())
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(mdk::DenseMatrix ...)")
        return false;
    }

    return this->Construct(IndexList.GetElementPointer(), DataArray.GetElementPointer(), DataArray.GetElementNumber(), Length);
}


template<typename ElementType>
inline
bool SparseVector<ElementType>::Construct(const int_max* IndexList, const ElementType* DataArray, int_max RecordedElementNumber, int_max Length)
{
    if (IndexList == nullptr || DataArray == nullptr || RecordedElementNumber <= 0 || Length <= 0)
    {
        MDK_Error("Invalid Input @ SparseVector::Construct(pointer ...)")
        return false;
    }

    //--------------------------------------------------------------
    m_Length = Length;
    m_IndexList.resize(RecordedElementNumber);
    m_DataArray.resize(RecordedElementNumber);
	m_ZeroElement = ElementType(0);
    //--------------------------------------------------------------

    std::vector<int_max> LinearIndex_In_Input_IndexList(RecordedElementNumber);

    Sort(IndexList, RecordedElementNumber, m_IndexList.data(), LinearIndex_In_Input_IndexList.data(), "ascend");

    //--------------------------------------------------------------

    for (int_max i = 0; i < RecordedElementNumber; ++i)
    {
        m_DataArray[i] = DataArray[LinearIndex_In_Input_IndexList[i]];
    }

    return true;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::ConstructFromSortedData(std::vector<int_max> IndexList, std::vector<ElementType> InputDataArray, int_max Length)
{
    m_Length = Length;
    m_IndexList = std::move(IndexList);
    m_DataArray = std::move(InputDataArray);
	m_ZeroElement = ElementType(0);
}


template<typename ElementType>
inline
void SparseVector<ElementType>::ConstructFromSortedData(DenseVector<int_max> IndexList, DenseVector<ElementType> InputDataArray, int_max Length)
{
	m_Length = Length;
	m_IndexList = std::move(IndexList.StdVector());
	m_DataArray = std::move(InputDataArray.StdVector());
	m_ZeroElement = ElementType(0);
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
    m_Length = 0;
    m_IndexList.clear();
    m_DataArray.clear();
	m_ZeroElement = ElementType(0);
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

    if (InputLength == m_Length)
    {
        return;
    }

    if (InputLength == 0 && m_Length > 0)
    {
        this->Clear();
        return;
    }

    if (InputLength > 0 && m_Length == 0)
    {
        this->Construct(InputLength);
        return;
    }

    //------ not empty , not equal to input size ---------------------//

    if (InputLength < m_Length)
    {
        auto RecordedElementNumber = int_max(m_IndexList.size());

        auto StartIndex_erase = RecordedElementNumber;

        for (int_max i = RecordedElementNumber - 1; i >= 0; --i)
        {
            if (m_IndexList[i] >= InputLength)
            {
                StartIndex_erase = i;
            }
            else
            {
                break;
            }
        }

        m_IndexList.erase(m_IndexList.begin() + StartIndex_erase, m_IndexList.end());
        m_DataArray.erase(m_DataArray.begin() + StartIndex_erase, m_DataArray.end());
    }

    m_Length = InputLength;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::FastResize(int_max InputLength)
{
    this->Construct(InputLength);
}


template<typename ElementType>
inline
ElementType& SparseVector<ElementType>::operator[](int_max Index)
{
	if (Index >= m_Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::operator[](int_max Index)")
		return m_ZeroElement;
	}

	int_max IndexInDataArray = -1;
	int_max IndexInDataArray_insert = RecordedElementNumber;

	for (int_max i = 0; i < RecordedElementNumber; ++i)
	{
		if (m_IndexList[i] == Index)
		{
			IndexInDataArray = i;
			break;
		}
		else if (m_IndexList[i] > Index)
		{
			IndexInDataArray_insert = i;
		}
	}

	if (IndexInDataArray >= 0)
	{
		return m_DataArray[IndexInDataArray];
	}
	else
	{
		m_IndexList.insert(m_IndexList.begin() + IndexInDataArray_insert, Index);
		m_DataArray.insert(m_DataArray.begin() + IndexInDataArray_insert, Element);
		return m_DataArray[IndexInDataArray_insert];
	}
}


template<typename ElementType>
inline
const ElementType& SparseVector<ElementType>::operator[](int_max Index) const
{
	int_max IndexInDataArray = -1;

	if (Index >= m_Length || Index < 0)
	{
		MDK_Error("Invalid Index @ SparseVector::operator[](int_max Index)")
		return m_ZeroElement;
	}

	for (int_max i = 0; i < int_max(m_IndexList.size()); ++i)
	{
		if (m_IndexList[i] == Index)
		{
			IndexInDataArray = i;
			break;
		}
	}

	if (IndexInDataArray >= 0)
	{
		return m_DataArray[IndexInDataArray];
	}
	else
	{
		MDK_Error("Invalid Index @ SparseVector::GetElement(Index)")
		return m_ZeroElement;
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
const ElementType& SparseVector<ElementType>::operator()(int_max Index) const
{
	return this->operator[](Index);
}


template<typename ElementType>
inline 
void SparseVector<ElementType>::Copy(const SparseVector<ElementType>& InputVector)
{
    m_Length = InputVector.m_Length;
    m_IndexList = InputVector.m_IndexList;
    m_DataArray = InputVector.m_DataArray;
    m_ZeroElement = InputVector.m_ZeroElement;
}


template<typename ElementType>
inline
void SparseVector<ElementType>::Take(SparseVector<ElementType>& InputVector)
{
    m_Length = InputVector.m_Length;
    m_IndexList = std::move(InputVector.m_IndexList);
    m_DataArray = std::move(InputVector.m_DataArray);
    m_ZeroElement = InputVector.m_ZeroElement;

    InputVector.m_Length = 0;
}


template<typename ElementType>
inline int_max SparseVector<ElementType>::GetLength() const
{
    return m_Length;
}


template<typename ElementType>
inline
int_max SparseVector<ElementType>::GetRecordedElementNumber() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
ElementType* SparseVector<ElementType>::GetPointerOfBeginElement()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* SparseVector<ElementType>::GetPointerOfBeginElement() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
std::vector<int_max>& SparseVector<ElementType>::IndexList()
{
    return m_IndexList;
}


template<typename ElementType>
inline
const std::vector<int_max>& SparseVector<ElementType>::IndexList() const
{
    return m_IndexList;
}


template<typename ElementType>
inline
std::vector<ElementType>& SparseVector<ElementType>::DataArray()
{
    return m_DataArray;
}


template<typename ElementType>
inline
const std::vector<ElementType>& SparseVector<ElementType>::DataArray() const
{
    return m_DataArray;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::Sum() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_DataArray.size()); ++k)
    {
        Value += m_DataArray[k];
    }

    return Value;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L1Norm() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_DataArray.size()); ++k)
    {
        Value += std::abs(m_DataArray[k]);
    }

    return Value;
}


template<typename ElementType>
inline
ElementType SparseVector<ElementType>::L2Norm() const
{
    auto Value = ElementType(0);

    for (int_max k = 0; k < int_max(m_DataArray.size()); ++k)
    {
        Value += m_DataArray[k] * m_DataArray[k];
    }

    Value = std::sqrt(Value);

    return Value;
}


}//end namespace mdk

#endif