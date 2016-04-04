#pragma once

namespace mdk
{
 
template<typename ElementType>
inline
ObjectArray<ElementType>::ObjectArray()
{
    this->Resize(0);
}


template<typename ElementType>
inline
ObjectArray<ElementType>::ObjectArray(const ObjectArray<ElementType>& InputArray)
{
	this->Copy(InputArray);
}


template<typename ElementType>
inline
ObjectArray<ElementType>::ObjectArray(ObjectArray<ElementType>&& InputArray) noexcept
{
	m_Data = std::move(InputArray.m_Data);
}


template<typename ElementType>
inline ObjectArray<ElementType>::ObjectArray(const StdObjectVector<ElementType>& InputArray)
{
	this->Copy(InputArray);
}


template<typename ElementType>
inline ObjectArray<ElementType>::ObjectArray(const std::initializer_list<ElementType>& InputList)
{
	(*this) = InputList;
}


template<typename ElementType>
inline ObjectArray<ElementType>::ObjectArray(const std::initializer_list<const ObjectArray<ElementType>*>& InputList)
{
	(*this) = InputList;
}


template<typename ElementType>
inline
ObjectArray<ElementType>::~ObjectArray()
{
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const ObjectArray<ElementType>& InputArray)
{
    this->Copy(InputArray);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(ObjectArray<ElementType>&& InputArray)
{
	this->Copy(std::move(InputArray));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const StdObjectVector<ElementType>& InputArray)
{
	this->Copy(InputArray);
}



template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const std::initializer_list<ElementType>& InputArray)
{
	this->Copy(InputArray.begin(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const std::initializer_list<const ObjectArray<ElementType>*>& InputList)
{
	auto InputArrayCount = int_max(InputList.size());

	if (InputArrayCount <= 0)
	{
		MDK_Warning("Input is empty, try to clear self @ ObjectArray::operator=(initializer_list of ObjectArray pointer)")

		if (this->IsSizeFixed() == true)
		{
			if (this->IsEmpty() == false)
			{
				MDK_Error("Can not change self size @ ObjectArray::operator=(initializer_list of ObjectArray pointer)")
			}
		}
		else
		{
			this->Clear();
		}
	    return;
	}

	bool IsSelfInInputList = false;

	std::vector<int_max> InputElementCountList;
	InputElementCountList.resize(InputArrayCount);
	int_max TotalInputElementCount = 0;
	for (int_max k = 0; k < InputArrayCount; k++)
	{
		InputElementCountList[k] = 0;
		auto InputArrayPtr = InputList.begin()[k];
		if (InputArrayPtr != nullptr)
		{
			InputElementCountList[k] = InputArrayPtr->GetElementCount();
			TotalInputElementCount += InputElementCountList[k];
			if (this->GetElementPointer() != nullptr && this->GetElementPointer() == InputArrayPtr->GetElementPointer())
			{
				IsSelfInInputList = true;
			}
		}
	}

	if (TotalInputElementCount == 0)
	{
		MDK_Warning("TotalInputElementCount is 0, try to clear self @ ObjectArray::operator=(initializer_list of ObjectArray pointer)")

		if (this->IsSizeFixed() == true)
		{
			if (this->IsEmpty() == false)
			{
				MDK_Error("Can not change self size @ DenseMatrix::ObjectArray=(initializer_list of ObjectArray pointerr)")
			}
		}
		else
		{
			this->Clear();
		}
	    return;
	}

	auto SelfElementCount = this->GetElementCount();

	if (IsSelfInInputList == false)
	{
		if (this->IsSizeFixed() == true)
		{
			if (SelfElementCount != TotalInputElementCount)
			{
				MDK_Error("Can not change self size @ ObjectArray::operator=(initializer_list of ObjectArray pointer)")
				return;
			}
		}
		else
		{
			this->FastResize(TotalInputElementCount);
		}

		int_max temp_ElementCount = 0;
		for (int_max k = 0; k < InputArrayCount; k++)
		{
			auto InputArrayPtr = InputList.begin()[k];
			if (InputArrayPtr != nullptr)
			{
				auto ElementPtr = InputArrayPtr->GetElementPointer();
				for (int_max j = temp_ElementCount; j < temp_ElementCount + InputElementCountList[k]; ++j, ++ElementPtr)
				{
					(*this)[j] = *ElementPtr;
				}
				temp_ElementCount += InputElementCountList[k];
			}
		}
	}
	else // Self is in InputList 
	{
		if (TotalInputElementCount == SelfElementCount)
		{
			MDK_Warning("Self = {&Self} @ ObjectArray::operator=(initializer_list of ObjectArray pointer)")
			return;
		}
		else
		{
			ObjectArray<ElementType> tempArray = InputList;
			this->Copy(std::move(tempArray));
		}
	}
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Copy(const ObjectArray<ElementType>& InputArray)
{
	this->Copy(InputArray.GetElementPointer(), InputArray.GetLength());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Copy(ObjectArray<ElementType>&& InputArray)
{
	if (this == &InputArray)
	{
		MDK_Warning("A ObjectArray try to Copy itself @ ObjectArray::Copy(...)")
		return;
	}

	auto InputLength = InputArray.GetElementCount();

	auto SelfLength = this->GetElementCount();

	if (this->IsSizeFixed() == true)
	{
		if (InputLength != SelfLength)
		{
			MDK_Error("Size can not change @ ObjectArray::Copy(...)")
			return;
		}
	}

	if (InputArray.IsEmpty() == true)
	{
		if (SelfLength > 0)
		{
			this->Clear();
		}
		return;
	}

	if (this->GetElementPointer() == InputArray.GetElementPointer())
	{// self copy
		return;
	}

	if (!m_Data)
	{
		this->Resize(0);
	}

	// now, InputArray is not empty and not self

	//note: m_Data.swap(InputArray.m_Data) will invalidate Share()

	m_Data->StdVector = std::move(InputArray.m_Data->StdVector);
	m_Data->ElementPointer = InputArray.m_Data->ElementPointer;
	m_Data->Length = InputArray.m_Data->Length;

	// Clear InputArray to be empty
	InputArray.Clear();
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Copy(const StdObjectVector<ElementType>& InputArray)
{
	this->Copy(InputArray.GetElementPointer(), InputArray.GetLength());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
	if (InputElementPointer == nullptr)
	{
		if (this->IsSizeFixed() == true)
		{
			if (this->GetElementCount() != 0)
			{
				MDK_Error("Size can not change @ ObjectArray::Copy(...)")
				return;
			}
		}

		this->Clear();
	}
	else
	{
		if (!m_Data)
		{
			this->Resize(0);
		}

		m_Data->Copy(InputElementPointer, InputLength);
	}
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementCount();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Fill")
        return;
    }

    auto BeginPointer = this->GetElementPointer();
    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfLength; ++Ptr)
    {
        Ptr[0] = Element;
    }
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Share(ObjectArray<ElementType>& InputArray)
{
	if (IsSizeFixed == true)
	{
		if (this->GetElementCount() != InputArray.GetElementCount())
		{
			MDK_Error("Size can not be changed @ ObjectArray::Share(...)")
			return;
		}
	}

	m_Data = InputArray.m_Data;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::ForceShare(const ObjectArray<ElementType>& InputArray)
{
	m_Data = InputArray.m_Data;
}


template<typename ElementType>
inline 
void ObjectArray<ElementType>::Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid input @ ObjectArray::Share(external data)")
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        if (this->GetLength() != InputLength)
        {
            MDK_Error("Size can not change @ ObjectArray::Share(external data)")
            return;
        }
    }

    m_Data->IsSizeFixed = IsSizeFixed;

    m_Data->StdVector.clear();
    m_Data->StdVector.shrink_to_fit();

	m_Data->Length = InputLength;
    m_Data->ElementPointer = InputElementPointer;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    this->Share(const_cast<ElementType*>(InputElementPointer), InputLength, IsSizeFixed);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Swap(ObjectArray<ElementType>& InputArray)
{
	m_Data.swap(InputArray.m_Data);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Clear()
{
	if (m_Data)
	{
		m_Data->Clear();
	}
}


template<typename ElementType>
inline void ObjectArray<ElementType>::Recreate()
{
	this->Resize(0);
}

template<typename ElementType>
inline 
void ObjectArray<ElementType>::Resize(int_max InputLength)
{
	if (!m_Data)
	{
		m_Data = std::make_shared<ObjectArrayData<ElementType>>();
	}
	m_Data->Resize(InputLength);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::FastResize(int_max InputLength)
{
	if (!m_Data)
	{
		this->Resize(0);
	}
	m_Data->FastResize(InputLength);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetCapacity(int_max InputElementNumber)
{
    if (!m_Data)
    {
		this->Resize(0);
    }

try
{
    auto SelfLength = this->GetElementCount();

    if (InputElementNumber > SelfLength)
    {
        m_Data->StdVector.reserve(InputElementNumber);
        m_Data->ElementPointer = m_Data->StdVector.data();
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ ObjectArray::SetCapacity(int_max InputElementNumber)")
}
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::ReleaseUnusedCapacity()
{
    if (!m_Data)
    {
        return;
    }
	else
	{
		m_Data->StdVector.shrink_to_fit();
		m_Data->ElementPointer = m_Data->StdVector.data();
	}
}


template<typename ElementType>
inline 
void ObjectArray<ElementType>::FixSize()
{
    if (!m_Data)
    {
        this->Resize(0);
    }

    m_Data->IsSizeFixed = true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::IsSizeFixed() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        return m_Data->IsSizeFixed;
    }
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::IsEmpty() const
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
bool ObjectArray<ElementType>::IsPureEmpty() const
{
	return (!m_Data);
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::IsShared() const
{
	if (!m_Data)
	{
		return false;
	}
	else
	{
		return (m_Data.use_count() > 1);
	}
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::IsSharedWith(const ObjectArray& InputArray) const
{
	if (this->GetElementPointer() == nullptr)
	{
		return false;
	}
	return (this->GetElementPointer() == InputArray.GetElementPointer());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::IsDataInInternalArray() const
{
	if (!m_Data)
	{
		return false;
	}
	else
	{
		return (m_Data->ElementPointer == m_Data->StdVector.data());
	}
}


template<typename ElementType>
inline
int_max ObjectArray<ElementType>::GetLength() const
{
    if (!m_Data)
    {
        return 0;
    }
    else
    {
        return m_Data->Length;
    }
}


template<typename ElementType>
inline
int_max ObjectArray<ElementType>::GetElementCount() const
{
    return this->GetLength();
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetErrorElement(ElementType Error)
{
	if (!m_Data)
	{
		this->Resize(0);
	}
	m_Data->ErrorElement = std::move(Error);
}


template<typename ElementType>
inline
ElementType ObjectArray<ElementType>::GetErrorElement()  const
{
    if (!m_Data)
    {
		return GetNaNElement(ErrorElement);
    }
	else
	{
		return m_Data->ErrorElement;
	}
}


template<typename ElementType>
inline
ElementType* ObjectArray<ElementType>::GetElementPointer()
{
    if (!m_Data)
    {
        return nullptr;
    }
    else
    {
        return m_Data->ElementPointer;
    }
}


template<typename ElementType>
inline
const ElementType* ObjectArray<ElementType>::GetElementPointer() const
{
    if (!m_Data)
    {
        return nullptr;
    }
    else
    {
        return m_Data->ElementPointer;
    }
}


template<typename ElementType>
inline
ElementType* ObjectArray<ElementType>::GetPointer()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline
const ElementType* ObjectArray<ElementType>::GetPointer() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline 
ElementType* ObjectArray<ElementType>::begin()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline 
const ElementType* ObjectArray<ElementType>::begin() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline
ElementType* ObjectArray<ElementType>::end()
{
	auto BeginPtr = this->GetElementPointer();
	if (BeginPtr != nullptr)
    {
		return BeginPtr + this->GetElementCount();
    }
	else
	{
		return nullptr;
	}
}


template<typename ElementType>
inline
const ElementType* ObjectArray<ElementType>::end() const
{
	auto BeginPtr = this->GetElementPointer();
	if (BeginPtr != nullptr)
	{
		return BeginPtr + this->GetElementCount();
	}
	else
	{
		return nullptr;
	}
}


//----------- Get/Set ObjectArray(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& ObjectArray<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_ObjectArray_Operator_CheckBound)

    if (Index >= this->GetElementCount()|| Index < 0)
    {
        MDK_Error("Invalid Input @ ObjectArray::operator[](i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_ObjectArray_Operator_CheckBound

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
const ElementType& ObjectArray<ElementType>::operator[](int_max Index) const
{
#if defined(MDK_DEBUG_ObjectArray_Operator_CheckBound)

    if (Index >= this->GetElementCount() || Index < 0)
    {
        MDK_Error("Invalid Input @ ObjectArray::operator[](i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_ObjectArray_Operator_CheckBound

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
ElementType& ObjectArray<ElementType>::operator()(int_max Index)
{
#if defined(MDK_DEBUG_ObjectArray_Operator_CheckBound)

    if (Index >= this->GetElementCount() || Index < 0)
    {
        MDK_Error("Invalid Input @ ObjectArray::operator()(i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_ObjectArray_Operator_CheckBound

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
const ElementType& ObjectArray<ElementType>::operator()(int_max Index) const
{
#if defined(MDK_DEBUG_ObjectArray_Operator_CheckBound)

    if (Index >= this->GetElementCount() || Index < 0)
    {
        MDK_Error("Invalid Input @ ObjectArray::operator()(i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_ObjectArray_Operator_CheckBound

	return (*m_Data)[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& ObjectArray<ElementType>::at(int_max Index)
{
    if (Index >= this->GetElementCount() || Index < 0)
	{
		MDK_Error("Invalid Input @ ObjectArray::at(i)")
        
        return m_Data->ErrorElement;
	}

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
const ElementType& ObjectArray<ElementType>::at(int_max Index) const
{
    if (Index >= this->GetElementCount() || Index < 0)
	{
		MDK_Error("Invalid Input @ ObjectArray::at(i) const")
        
        return m_Data->ErrorElement;
	}

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Append(ElementType Element)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ ObjectArray::Append(ElementType Element)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

    m_Data->CopyDataToStdVectorIfNecessary();

    auto SelfLength = this->GetElementCount();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);
}

/*
template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Append(const std::initializer_list<ElementType>& InputArray)
{
    return this->Append(InputArray.begin(), int_max(InputArray.size()));
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const std::vector<ElementType>& InputArray)
{
    return this->Append(InputArray.data(), int_max(InputArray.size()));
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const DenseMatrix<ElementType>& InputArray)
{
    if (InputArray.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ ObjectArray::Append(const DenseMatrix<ElementType>& InputArray)")
        return false;
    }

    return this->Append(InputArray.GetElementPointer(), InputArray.GetElementCount());
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const StdObjectVector<ElementType>& InputArray)
{
	return this->Append(InputArray.GetElementPointer(), InputArray.GetElementCount());
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const ObjectArray<ElementType>& InputArray)
{
    return this->Append(InputArray.GetElementPointer(), InputArray.GetElementCount());
}
*/

template<typename ElementType>
inline 
void ObjectArray<ElementType>::Append(const ElementType* InputArray, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ ObjectArray::Append(const ElementType* InputArray, int_max InputLength)")
        return;
    }

    if (InputArray == nullptr || InputLength <= 0)
    {
        MDK_Warning("Empty Input @ ObjectArray::Append(const ElementType* InputArray, int_max InputLength)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

    m_Data->CopyDataToStdVectorIfNecessary();

    auto SelfLength = this->GetElementCount();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_Data->StdVector[i] = InputArray[i - SelfLength];
    }
}


template<typename ElementType>
inline 
void ObjectArray<ElementType>::Delete(int_max Index)
{
    this->Delete(&Index, 1);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Delete(const StdObjectVector<int_max>& IndexList)
{
	this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Delete(const ObjectArray<int_max>& IndexList)
{
	this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
template<int_max TemplateLength>
inline
void ObjectArray<ElementType>::Delete(const DenseVector<int_max, TemplateLength>& IndexList)
{
	this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
            return;
        }
    }

    m_Data->CopyDataToStdVectorIfNecessary();

    if (ListLength == 1)
    {
        m_Data->StdVector.erase(m_Data->StdVector.begin() + IndexList[0], m_Data->StdVector.begin() + IndexList[0] + 1);

        m_Data->Length -= 1;

        m_Data->ElementPointer = m_Data->StdVector.data();
    }
    else
    {
		std::vector<int_max> IndexList_max_to_min(ListLength);

		for (int_max i = 0; i < ListLength; ++i)
        {
            IndexList_max_to_min[i] = IndexList[i];
        }

        std::sort(IndexList_max_to_min.begin(), IndexList_max_to_min.end(), [](int_max a, int_max b) { return a > b; });

        int_max Index_prev = -1;

        for (int_max i = 0; i < int_max(IndexList_max_to_min.size()); ++i)
        {
            auto Index_i = IndexList_max_to_min[i];

            if (Index_i == Index_prev)
            {
                MDK_Warning("duplicate Input @ ObjectArray::Delete(const int_max* IndexPtr, int_max ListLength)")
            }
            else
            {
                m_Data->StdVector.erase(m_Data->StdVector.begin() + Index_i, m_Data->StdVector.begin() + Index_i + 1);

                Index_prev = Index_i;

                m_Data->Length -= 1;
            }
        }

        m_Data->ElementPointer = m_Data->StdVector.data();
    }
}


template<typename ElementType>
inline 
void ObjectArray<ElementType>::Delete(int_max Index_start, int_max Index_end)
{// allowed if Index_start == Index_end
	
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    m_Data->CopyDataToStdVectorIfNecessary();

    m_Data->StdVector.erase(m_Data->StdVector.begin() + Index_start, m_Data->StdVector.begin() + Index_end + 1);
    
    m_Data->Length -= Index_end - Index_start + 1;

    m_Data->ElementPointer = m_Data->StdVector.data();
}


template<typename ElementType>
inline void ObjectArray<ElementType>::Insert(int_max Index, ElementType Element)
{
	if (this->IsSizeFixed() == true)
	{
		MDK_Error("ObjectArray Size can not change @ ObjectArray::Insert(...)")
		return;
	}

	auto SelfLength = this->GetElementCount();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ ObjectArray::Insert(...)")
			return;
		}
	}
	else
	{
		if (Index >= SelfLength || Index < 0 )
		{
			MDK_Error("Invalid Input @ ObjectArray::Insert(...)")
			return;
		}
	}

	if (!m_Data)
	{
		this->Resize(0);
	}

	m_Data->CopyDataToStdVectorIfNecessary();

	m_Data->StdVector.insert(m_Data->StdVector.begin() + Index, std::move(Element));
	m_Data->ElementPointer = m_Data->StdVector.data();
	m_Data->Length = SelfLength + 1;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputArray)
{
    this->Insert(Index, InputArray.begin(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Insert(int_max Index, const StdObjectVector<ElementType>& InputArray)
{
	this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Insert(int_max Index, const ObjectArray<ElementType>& InputArray)
{
    this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Insert(int_max Index, const ElementType* InputArray, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
        return;
    }

    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return;
        }
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

    m_Data->CopyDataToStdVectorIfNecessary();

    m_Data->StdVector.insert(m_Data->StdVector.begin() + Index, InputArray, InputArray + InputLength);
	m_Data->ElementPointer = m_Data->StdVector.data();
	m_Data->Length = SelfLength + InputLength;  
}


template<typename ElementType>
inline 
void ObjectArray<ElementType>::PushBack(ElementType Element)
{
    this->Append(std::move(Element));
}


template<typename ElementType>
inline
ElementType ObjectArray<ElementType>::PopBack()
{
    auto CurrentElementNumber = this->GetElementCount();

    if (CurrentElementNumber > 0)
    {
        auto tempIndex = CurrentElementNumber - 1;
        auto OutputElement = std::move((*this)[tempIndex]);
        this->Delete(tempIndex);
		return OutputElement;
    }
    else
    {
		MDK_Error("Self is empty @ ObjectArray::PopBack()")
		return m_Data->ErrorElement;
    }
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Push(ElementType Element)
{
	this->Append(std::move(Element));
}


template<typename ElementType>
inline
ElementType ObjectArray<ElementType>::Pop()
{
	this->PopBack();
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    ObjectArray<ElementType> Subset;

    auto ElementNumber = this->GetElementCount();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ ObjectArray::GetSubSet(...)")
        return Subset;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ ObjectArray::GetSubSet(...)")
        return Subset;
    }

    if (ElementNumber == 0)
    {
        return Subset;
    }

    Subset.FastResize(Index_end - Index_start + 1);

	for (int_max i = Index_start; i <= Index_end; ++i)
    {
		Subset[i - Index_start] = (*this)[i];
    }

    return Subset;
}


template<typename ElementType>
inline 
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const ObjectArray<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const StdObjectVector<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
template<int_max TemplateLength>
inline
StdObjectVector<ElementType> ObjectArray<ElementType>::GetSubSet(const DenseVector<int_max, TemplateLength>& IndexList)
{
	return this->GetSubSet(IndexList.GetElementPointer(), int_max(IndexList.GetElementCount()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const int_max* IndexList, int_max ListLength)
{
    ObjectArray<ElementType> SubSet;

    if (IndexList == nullptr || ListLength <= 0)
    {
        return SubSet;
    }

    auto ElementNumber = this->GetElementCount();

    if (ListLength > ElementNumber)
    {
        MDK_Error("Invalid ListLength @ ObjectArray::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(ListLength);

    for (int_max k = 0; k < ListLength; ++k)
    {
        auto tempIndex = IndexList[k];

        if (tempIndex < 0 || tempIndex >= ElementNumber)
        {
            MDK_Error("Invalid index @ ObjectArray::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[k] = (*this)[tempIndex];
    }

    return SubSet;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet)
{
	if (IndexList.size() != SubSet.size())
	{
		MDK_Error("IndexList size != SubSet size @ ObjectArray::SetSubSet(...)")
		return;
	}

	this->SetSubSet(IndexList.begin(), SubSet.begin(), int_max(SubSet.size()));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetSubSet(const ObjectArray<int_max>& IndexList, const ObjectArray<ElementType>& SubSet)
{
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("IndexList size != SubSet size @ ObjectArray::SetSubSet(...)")
		return;
	}

	this->SetSubSet(IndexList.GetElementPointer(), SubSet.GetElementPointer(), SubSet.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetSubSet(const StdObjectVector<int_max>& IndexList, const ObjectArray<ElementType>& SubSet)
{
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("IndexList size != SubSet size @ ObjectArray::SetSubSet(...)")
		return;
	}

	this->SetSubSet(IndexList.GetElementPointer(), SubSet.GetElementPointer(), SubSet.GetElementCount());
}

template<typename ElementType>
inline
void ObjectArray<ElementType>::SetSubSet(const DenseMatrix<int_max>& IndexList, const ObjectArray<ElementType>& SubSet)
{
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("IndexList size != SubSet size @ ObjectArray::SetSubSet(...)")
		return;
	}

	this->SetSubSet(IndexList.GetElementPointer(), SubSet.GetElementPointer(), SubSet.GetElementCount());
}


template<typename ElementType>
template<int_max TemplateLength>
inline
void ObjectArray<ElementType>::SetSubSet(const DenseVector<int_max, TemplateLength>& IndexList, const ObjectArray<ElementType>& SubSet)
{
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("IndexList size != SubSet size @ ObjectArray::SetSubSet(...)")
		return;
	}

	this->SetSubSet(IndexList.GetElementPointer(), SubSet.GetElementPointer(), SubSet.GetElementCount());
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max SubSetElementNumber)
{
	if (IndexList == nullptr || SubSet == nullptr || SubSetElementNumber <= 0)
	{
		MDK_Warning("Empty input @ ObjectArray::SetSubSet(...)")
		return;
	}

	auto ElementNumber = this->GetElementCount();

	if (ElementNumber == 0)
	{
		MDK_Error("Self is empty @ ObjectArray::SetSubSet(...)")
		return;
	}

	if (SubSetElementNumber > ElementNumber)
	{
		MDK_Error("Invalid SubSetElementNumber @ ObjectArray::SetSubSet(...)")
		return;
	}

	for (int_max k = 0; k < SubSetElementNumber; ++k)
	{
		auto Index = IndexList[k];

		if (Index < 0 || Index >= ElementNumber)
		{
			MDK_Error("Invalid Index @ ObjectArray::SetSubSet(...)")
			return;
		}

		(*this)[Index] = SubSet[k];
	}
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Find(MatchFunctionType MatchFunction)
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction)
{
    ObjectArray<int_max> IndexList;

    auto ElementNumber = this->GetElementCount();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ ObjectArray::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ ObjectArray::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ ObjectArray::Find(...)")
        return IndexList;
    }

    if (ElementNumber == 0)
    {
        return IndexList;
    }

    if (Index_start == Index_end)
    {
        IndexList.Append(Index_start);
        return IndexList;
    }

    IndexList.ReserveCapacity(MaxOutputNumber);

    for (int_max i = Index_start; i <= Index_end; ++i)
    {
        if (MatchFunction((*this)[i]) == true)
        {
            IndexList.Append(i);

            if (IndexList.GetElementCount() == MaxOutputNumber)
            {
                break;
            }
        }
    }

    return IndexList;
}


template<typename ElementType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::ExactMatch(const ElementType& InputElement) const
{
	return this->Find([&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
inline
int_max ObjectArray<ElementType>::ExactMatch(const String& first_or_last, const ElementType& InputElement) const
{
	return this->Find(first_or_last, [&](const ElementType& Element){return Element == InputElement; });
}

template<typename ElementType>
template<typename CompareFunctionType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength()-1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    ObjectArray<int_max> IndexList;

    auto ElementNumber = this->GetElementCount();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ ObjectArray::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ ObjectArray::Find(...)")
        return IndexList;
    }

    if (ElementNumber == 0)
    {
        return IndexList;
    }

    if (Index_start == Index_end)
    {
        IndexList.Append(Index_start);
        return IndexList;
    }

    IndexList.FastResize(ElementNumber);

    for (int_max i = Index_start; i <= Index_end; ++i)
    {
        IndexList[i] = i;
    }

    std::sort(IndexList.begin(), IndexList.end(), [&](int_max a, int_max b)
    {
        return CompareFunction((*this)[a], (*this)[b]);
    });

    return IndexList;
}


template<typename ElementType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Sort(const char* Order) const
{
	String Order_str(Order);
	return this->Sort(Order_str);
}


template<typename ElementType>
inline
ObjectArray<int_max> ObjectArray<ElementType>::Sort(const String& Order) const
{
	// Order: ascend or descend

	if (Order == "ascend")
	{
		return this->Sort(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA < ElementB; });
	}
	else if (Order == "descend")
	{
		return this->Sort(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("invalid input string @ ObjectArray::Sort(Order)")
		ObjectArray<int_max> EmptyIndexList;
		return EmptyIndexList;
	}
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
void ObjectArray<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin(), this->end(), CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
void ObjectArray<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SortInPlace(const char* Order)
{
	String Order_str(Order);
	this->SortInPlace(Order_str);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SortInPlace(const String& Order)
{
	// Order: ascend or descend

	if (Order == "ascend")
	{
		this->SortInPlace(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA < ElementB; });
	}
	else if (Order == "descend")
	{
		this->SortInPlace(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("invalid input string @ DenseVector::SortInPlace(Order)")
	}
}

}//end namespace mdk
