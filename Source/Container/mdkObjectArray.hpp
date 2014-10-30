#ifndef __mdkObjectArray_hpp
#define __mdkObjectArray_hpp


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
ObjectArray<ElementType>::ObjectArray(const std::initializer_list<ElementType>& InputList)
{
	this->Resize(0);
	(*this) = InputList;
}


template<typename ElementType>
inline ObjectArray<ElementType>::ObjectArray(const std::vector<ElementType>& InputArray)
{
	this->Resize(0);
	this->Copy(InputArray);
}


template<typename ElementType>
inline ObjectArray<ElementType>::ObjectArray(const SimpleObjectArray<ElementType>& InputArray)
{
	this->Resize(0);
	this->Copy(InputArray);
}

template<typename ElementType>
inline
ObjectArray<ElementType>::ObjectArray(const ObjectArray<ElementType>& InputArray, ObjectConstructionTypeEnum Method)
{
    if (Method == ObjectConstructionTypeEnum::Copy)
    {
        this->Resize(0);
        this->Copy(InputArray);
    }
    else
    {
        this->ForceShare(InputArray);
    }
}


// move constructor
template<typename ElementType>
inline
ObjectArray<ElementType>::ObjectArray(ObjectArray<ElementType>&& InputArray) noexcept
{
    m_Data = std::move(InputArray.m_Data);
}


template<typename ElementType>
inline
ObjectArray<ElementType>::~ObjectArray()
{
}


template<typename ElementType>
inline
std::vector<ElementType>& ObjectArray<ElementType>::StdVector()
{
    m_Data->CopyDataToInternalObjectArrayIfNecessary();
    return m_Data->StdVector;
}


template<typename ElementType>
inline
const std::vector<ElementType>& ObjectArray<ElementType>::StdVector() const
{
    m_Data->CopyDataToInternalObjectArrayIfNecessary();
    return m_Data->StdVector;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const ObjectArray<ElementType>& InputArray)
{
    this->Copy(InputArray);
}


// move assignment operator
template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(ObjectArray<ElementType>&& InputArray)
{
    this->Take(std::forward<ObjectArray<ElementType>&>(InputArray));
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const SimpleObjectArray<ElementType>& InputArray)
{
	this->Copy(InputArray);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const std::initializer_list<ElementType>& InputArray)
{
    auto InputLength = int_max(InputArray.size());
	auto SelfLength = this->GetElementNumber();

	if (InputLength <= 0)
	{
		if (this->IsSizeFixed() == true)
		{
			if (this->IsEmpty() == false)
			{
				MDK_Error("Can not change matrix size @ ObjectArray::operator=(initializer_list)")
			}
		}
		else
		{
			MDK_Warning("Input is empty, try to clear self @ ObjectArray::operator=(initializer_list)")
			this->Clear();
		}
		return;
	}

    if (this->IsSizeFixed() == true)
    {
        if (SelfLength != InputLength)
        {
            MDK_Error("Can not change size @ ObjectArray::operator=(initializer_list)")
			return;
        }		
    }

	this->FastResize(0);
	this->Append(InputArray.begin(), InputLength);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::operator=(const std::vector<ElementType>& InputArray)
{
	this->Copy(InputArray);
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const std::vector<ElementType>& InputArray)
{
	return this->Copy(InputArray.begin(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const SimpleObjectArray<ElementType>& InputArray)
{
	return this->Copy(InputArray.GetElementPointer(), InputArray.GetLength());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const SimpleObjectArray<ElementType>* InputArray)
{
	if (InputArray == nullptr)
	{
		MDK_Error("Input is nullptr @ ObjectArray::Copy(const SimpleObjectArray* InputArray)")
		return false;
	}
	return this->Copy(InputArray->GetElementPointer(), InputArray->GetLength());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const ObjectArray<ElementType>& InputArray)
{
    return this->Copy(InputArray.GetElementPointer(), InputArray.GetLength());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const ObjectArray<ElementType>* InputArray)
{
    if (InputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ObjectArray::Copy(ObjectArray* InputArray)")
        return false;
    }

    return this->Copy(*InputArray);
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
		if (this->IsEmpty() == true)
		{
			return true;
		}        
		else
		{
			if (this->IsSizeFixed() == true)
			{
				MDK_Error("Can not change size @ ObjectArray::Copy(ElementType*, InputLength)")
				return false;
			}

			this->Clear();
			return true;
		}
    }

    // if this ObjectArray is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A ObjectArray tries to Copy itself @ ObjectArray::Copy(ElementType*, RowNumber, ColNumber)")
            return true;
        }
    }
    //------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Can not change size @ ObjectArray::Copy(ElementType*, InputLength)")
            return false;
        }
    }
    else
    {
        this->FastResize(InputLength);
        SelfLength = InputLength;
    }

    //copy data ----------------------------------------------------------

    auto BeginPointer = this->GetElementPointer();

    auto tempPtr = InputElementPointer;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfLength; ++Ptr, ++tempPtr)
    {
        Ptr[0] = tempPtr[0];
    }

    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Fill")
        return false;
    }

    auto BeginPointer = this->GetElementPointer();
    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfLength; ++Ptr)
    {
        Ptr[0] = Element;
    }
    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Share(ObjectArray<ElementType>& InputArray)
{
    // ObjectArray = ObjectArray
    if (this == &InputArray)
    {
        MDK_Warning("A ObjectArray tries to Share itself @ ObjectArray::Share(InputArray)")
        return true;
    }

    auto InputLength = InputArray.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("ObjectArray size can not be changed @ ObjectArray::Share(InputArray)")
            return false;
        }
    }

    //--------------------------------------------------------------------------------------------------------
    m_Data = InputArray.m_Data; // std::Shared_ptr, self assignment test is not necessary

    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Share(ObjectArray<ElementType>* InputArray)
{
    if (InputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ObjectArray::Share(mdkObjectArray* InputArray)")
        return false;
    }

    return this->Share(*InputArray);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::ForceShare(const ObjectArray<ElementType>& InputArray)
{
    // ObjectArray = ObjectArray
    if (this == &InputArray)
    {
        MDK_Warning("A ObjectArray tries to ForceShare itself @ ObjectArray::ForceShare(InputArray)")
        return;
    }

    m_Data = InputArray.m_Data; // std::Shared_ptr, self assignment check is not necessary
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::ForceShare(const ObjectArray<ElementType>* InputArray)
{
    if (InputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ObjectArray::ForceShare(mdkObjectArray* InputArray)")
        return false;
    }

    this->ForceShare(*InputArray);

    return true;
}


template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid input @ ObjectArray::Share(...)")
        return false;
    }

    if (this->IsSizeFixed() == true)
    {
        if (this->GetLength() != InputLength)
        {
            MDK_Error("Size can not change @ ObjectArray::Share(...)")
            return false;
        }
    }

    m_Data->IsSizeFixed = IsSizeFixed;

    m_Data->Length = InputLength;

    m_Data->StdVector.clear();
    m_Data->StdVector.shrink_to_fit();

    m_Data->ElementPointer = InputElementPointer;

    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    return this->Share(const_cast<ElementType*>(InputElementPointer), InputLength, IsSizeFixed);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Take(ObjectArray<ElementType>&& InputArray)
{
    this->Take(std::forward<ObjectArray<ElementType>&>(InputArray));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Take(ObjectArray<ElementType>& InputArray)
{
    // ObjectArray = ObjectArray
    if (this == &InputArray)
    {
        MDK_Warning("A ObjectArray tries to take itself @ ObjectArray::take(InputArray)")
        return true;
    }

    auto InputLength = InputArray.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Size does not match @ ObjectArray::Take(InputArray)")
            return false;
        }
    }

    if (InputArray.IsEmpty() == true)
    {
        if (SelfLength > 0)
        {
            //MDK_Warning("InputArray is empty, and this ObjectArray is set to be empty @ ObjectArray::Take(InputArray)")
            this->Clear();
        }

        return true;
    }

    // ObjectArrayA = ObjectArrayA
    if (this->GetElementPointer() == InputArray.GetElementPointer())
    {
        //MDK_Warning("A ObjectArray tries to take itself @ ObjectArray::Take(InputArray)")
        return true;
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

    // now, InputArray is not empty, and is not self
    
    //note: m_Data.swap(InputArray.m_Data) will invalidate Share()

	m_Data->StdVector = std::move(InputArray.m_Data->StdVector);
    m_Data->ElementPointer = InputArray.m_Data->ElementPointer;
    m_Data->Length = InputArray.m_Data->Length;

    // Clear InputArray to be empty
    InputArray.Clear();

    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Take(ObjectArray<ElementType>* InputArray)
{
    if (InputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ObjectArray::Take(mdkObjectArray* InputArray)")
        return false;
    }

    return this->Take(*InputArray);
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::SwapSmartPointer(ObjectArray<ElementType>& InputArray)
{
    // ObjectArray = ObjectArray
    if (this == &InputArray)
    {
        MDK_Warning("A ObjectArray tries to Swap with itself @ ObjectArray::SwapSmartPointer(InputArray)")
        return;
    }

    m_Data.swap(InputArray.m_Data); // shared_ptr self swap check is not necessary
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::Clear()
{
    if (!m_Data)
    {
        return;
    }

    m_Data->IsSizeFixed = false;

    m_Data->Length = 0;

    m_Data->StdVector.clear();         // change size
    m_Data->StdVector.shrink_to_fit(); // release memory

    m_Data->ElementPointer = nullptr;
}


template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ ObjectArray::Resize(int_max InputLength)")
        return false;
    }

try
{
    //--------initialize the ObjectArray data ----------------------------------------
    if (!m_Data)
    {
        m_Data = std::make_shared<ObjectArrayData<ElementType>>();
    }
    //-------------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not be changed @ ObjectArray::Resize(int_max InputLength)")
        return false;
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

    m_Data->StdVector.resize(InputLength);
    m_Data->ElementPointer = m_Data->StdVector.data();
    m_Data->Length = InputLength;
}
catch (...)
{
    MDK_Error("Out of Memory @ ObjectArray::Resize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->StdVector.data();
	m_Data->Length = int_max(m_Data->StdVector.size());
    return false;
}
    return true;
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ ObjectArray::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change size @ ObjectArray::FastResize(int_max InputLength)")
        return false;
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

try
{
    if (InputLength != SelfLength)
    {
		if (InputLength > int_max(m_Data->StdVector.capacity()))
        {
            m_Data->StdVector.clear();
        }

        m_Data->StdVector.resize(InputLength);
    }
   
    m_Data->ElementPointer = m_Data->StdVector.data();
    m_Data->Length = InputLength;
}
catch (...)
{
    MDK_Error("Out of Memory @ ObjectArray::FastResize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->StdVector.data();
    m_Data->Length = int_max(m_Data->StdVector.size());
    return false;
}
    return true;
}



template<typename ElementType>
inline
bool ObjectArray<ElementType>::ReserveCapacity(int_max InputElementNumber)
{
    if (!m_Data)
    {
        this->Resize(0);
    }

try
{
    auto SelfLength = this->GetElementNumber();

    if (InputElementNumber > SelfLength)
    {
        m_Data->StdVector.reserve(InputElementNumber);
        m_Data->ElementPointer = m_Data->StdVector.data();
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ ObjectArray::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void ObjectArray<ElementType>::ReleaseUnusedCapacity()
{
    if (!m_Data)
    {
        return;
    }

    m_Data->StdVector.shrink_to_fit();
    m_Data->ElementPointer = m_Data->StdVector.data();
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
bool ObjectArray<ElementType>::IsShared() const
{
    return (m_Data.use_count() > 1);
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
int_max ObjectArray<ElementType>::GetElementNumber() const
{
    return this->GetLength();
}


template<typename ElementType>
inline
const ElementType& ObjectArray<ElementType>::GetErrorElement()  const
{
    if (!m_Data)
    {
        MDK_Error("m_Data is empty @ ObjectArray::GetErrorElement()")
    }

    return m_Data->ErrorElement;
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
    auto EndPtr = this->GetElementPointer();

    if (EndPtr != nullptr)
    {
        EndPtr += this->GetElementNumber();
    }

    return EndPtr;
}


template<typename ElementType>
inline
const ElementType* ObjectArray<ElementType>::end() const
{
    auto EndPtr = this->GetElementPointer();

    if (EndPtr != nullptr)
    {
        EndPtr += this->GetElementNumber();
    }

    return EndPtr;
}


//----------- Get/Set ObjectArray(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& ObjectArray<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_ObjectArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber()|| Index < 0)
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

    if (Index >= this->GetElementNumber() || Index < 0)
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

    if (Index >= this->GetElementNumber() || Index < 0)
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

    if (Index >= this->GetElementNumber() || Index < 0)
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
    if (Index >= this->GetElementNumber() || Index < 0)
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
    if (Index >= this->GetElementNumber() || Index < 0)
	{
		MDK_Error("Invalid Input @ ObjectArray::at(i) const")
        
        return m_Data->ErrorElement;
	}

	return (*m_Data)[Index];
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(ElementType Element)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Append(ElementType Element)")
        return false;
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);

    return true;
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

    return this->Append(InputArray.GetElementPointer(), InputArray.GetElementNumber());
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const SimpleObjectArray<ElementType>& InputArray)
{
	return this->Append(InputArray.GetElementPointer(), InputArray.GetElementNumber());
}
*/
/*
template<typename ElementType>
inline
bool ObjectArray<ElementType>::Append(const ObjectArray<ElementType>& InputArray)
{
    return this->Append(InputArray.GetElementPointer(), InputArray.GetElementNumber());
}
*/

template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Append(const ElementType* InputArray, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Append(const ElementType* InputArray, int_max InputLength)")
        return false;
    }

    if (InputArray == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ ObjectArray::Append(const ElementType* InputArray, int_max InputLength)")
        return false;
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_Data->StdVector[i] = InputArray[i - SelfLength];
    }

    return true;
}


template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ ObjectArray::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const ObjectArray<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const SimpleObjectArray<int_max>& IndexList)
{
	return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ ObjectArray::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

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

    return true;
}


template<typename ElementType>
inline 
bool ObjectArray<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ ObjectArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

    m_Data->StdVector.erase(m_Data->StdVector.begin() + Index_start, m_Data->StdVector.begin() + Index_end + 1);
    
    m_Data->Length -= Index_end - Index_start + 1;

    m_Data->ElementPointer = m_Data->StdVector.data();

    return true;
}


template<typename ElementType>
inline bool ObjectArray<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.begin(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.data(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Insert(int_max Index, const SimpleObjectArray<ElementType>& InputArray)
{
	return this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementNumber());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Insert(int_max Index, const ObjectArray<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementNumber());
}


template<typename ElementType>
inline
bool ObjectArray<ElementType>::Insert(int_max Index, const ElementType* InputArray, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("ObjectArray Size can not change @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ ObjectArray::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return false;
        }
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

    m_Data->CopyDataToInternalObjectArrayIfNecessary();

    m_Data->StdVector.insert(m_Data->StdVector.begin() + Index, InputArray, InputArray + InputLength);

    m_Data->Length = InputLength;

    m_Data->ElementPointer = m_Data->StdVector.data();

    return true;
}


template<typename ElementType>
inline 
bool ObjectArray<ElementType>::PushBack(ElementType Element)
{
    return this->Append(std::move(Element));
}


template<typename ElementType>
inline
ElementType ObjectArray<ElementType>::PopBack()
{
    auto CurrentElementNumber = this->GetElementNumber();

    if (CurrentElementNumber > 0)
    {
        auto tempIndex = CurrentElementNumber - 1;
        auto OutputElement = (*this)[tempIndex];
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
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    ObjectArray<ElementType> Subset;

    auto ElementNumber = this->GetElementNumber();

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

    for (int_max i = Index_end; i <= Index_start; ++i)
    {
        Subset[i - Index_end] = (*this)[i];
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
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const std::vector<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const SimpleObjectArray<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const ObjectArray<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const DenseVector<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
ObjectArray<ElementType> ObjectArray<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
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

    auto ElementNumber = this->GetElementNumber();

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

    auto ElementNumber = this->GetElementNumber();

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

            if (IndexList.GetElementNumber() == MaxOutputNumber)
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
int_max ObjectArray<ElementType>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
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

    auto ElementNumber = this->GetElementNumber();

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

}//end namespace mdk

#endif