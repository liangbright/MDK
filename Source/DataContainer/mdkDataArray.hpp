#ifndef __mdkDataArray_hpp
#define __mdkDataArray_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DataArray<ElementType>::DataArray()
{
    this->Resize(0);
}


template<typename ElementType>
inline
DataArray<ElementType>::DataArray(const std::initializer_list<ElementType>& InputList)
{
	this->Resize(0);
	(*this) = InputList;
}


template<typename ElementType>
inline DataArray<ElementType>::DataArray(const std::vector<ElementType>& InputData)
{
	this->Resize(0);
	this->Copy(InputData);
}


template<typename ElementType>
inline DataArray<ElementType>::DataArray(const SimpleDataArray<ElementType>& InputData)
{
	this->Resize(0);
	this->Copy(InputData);
}

template<typename ElementType>
inline
DataArray<ElementType>::DataArray(const DataArray<ElementType>& InputData, ObjectConstructionTypeEnum Method)
{
    if (Method == ObjectConstructionTypeEnum::Copy)
    {
        this->Resize(0);
        this->Copy(InputData);
    }
    else
    {
        this->ForceShare(InputData);
    }
}


// move constructor
template<typename ElementType>
inline
DataArray<ElementType>::DataArray(DataArray<ElementType>&& InputData) noexcept
{
    m_Data = std::move(InputData.m_Data);

    m_ElementPointer = m_Data->ElementPointer;

    // the InputData may not be destructed
    InputData.m_ElementPointer = nullptr;
}


template<typename ElementType>
inline
DataArray<ElementType>::~DataArray()
{
}


template<typename ElementType>
inline
std::vector<ElementType>& DataArray<ElementType>::StdVector()
{
    m_Data->CopyDataToInternalDataArrayIfNecessary();
    m_ElementPointer = m_Data->ElementPointer;
    return m_Data->DataArray;
}


template<typename ElementType>
inline
const std::vector<ElementType>& DataArray<ElementType>::StdVector() const
{
    m_Data->CopyDataToInternalDataArrayIfNecessary();
    m_ElementPointer = m_Data->ElementPointer;
    return m_Data->DataArray;
}


template<typename ElementType>
inline
void DataArray<ElementType>::operator=(const DataArray<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move assignment operator
template<typename ElementType>
inline
void DataArray<ElementType>::operator=(DataArray<ElementType>&& InputData)
{
    this->Take(std::forward<DataArray<ElementType>&>(InputData));
}


template<typename ElementType>
inline
void DataArray<ElementType>::operator=(const SimpleDataArray<ElementType>& InputData)
{
	this->Copy(InputData);
}


template<typename ElementType>
inline
void DataArray<ElementType>::operator=(const std::initializer_list<ElementType>& InputData)
{
    auto InputLength = int_max(InputData.size());
	auto SelfLength = this->GetElementNumber();

	if (InputLength <= 0)
	{
		if (this->IsSizeFixed() == true)
		{
			if (this->IsEmpty() == false)
			{
				MDK_Error("Can not change matrix size @ DataArray::operator=(initializer_list)")
			}
		}
		else
		{
			MDK_Warning("Input is empty, try to clear self @ DataArray::operator=(initializer_list)")
			this->Clear();
		}
		return;
	}

    if (this->IsSizeFixed() == true)
    {
        if (SelfLength != InputLength)
        {
            MDK_Error("Can not change size @ DataArray::operator=(initializer_list)")
			return;
        }		
    }

	this->FastResize(0);
	this->Append(InputData.begin(), InputLength);
}


template<typename ElementType>
inline
void DataArray<ElementType>::operator=(const std::vector<ElementType>& InputData)
{
	this->Copy(InputData);
}


template<typename ElementType>
inline
bool Copy(const std::vector<ElementType>& InputData)
{
	return this->Copy(InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool Copy(const SimpleDataArray<ElementType>& InputData)
{
	return this->Copy(InputData.GetElementPointer(), InputData.GetLength());
}


template<typename ElementType>
inline bool Copy(const SimpleDataArray<ElementType>* InputData)
{
	if (InputData == nullptr)
	{
		MDK_Error("Input is nullptr @ DataArray::Copy(const SimpleDataArray* InputData)")
		return false;
	}
	return this->Copy(InputData->GetElementPointer(), InputData->GetLength());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Copy(const DataArray<ElementType>& InputData)
{
    return this->Copy(InputData.GetElementPointer(), InputData.GetLength());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Copy(const DataArray<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataArray::Copy(DataArray* InputData)")
        return false;
    }

    return this->Copy(*InputData);
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
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
				MDK_Error("Can not change size @ DataArray::Copy(ElementType*, InputLength)")
				return false;
			}

			this->Clear();
			return true;
		}
    }

    // if this DataArray is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A DataArray tries to Copy itself @ DataArray::Copy(ElementType*, RowNumber, ColNumber)")
            return true;
        }
    }
    //------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Can not change size @ DataArray::Copy(ElementType*, InputLength)")
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
bool DataArray<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ DataArray::Fill")
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
bool DataArray<ElementType>::Share(DataArray<ElementType>& InputData)
{
    // DataArray = DataArray
    if (this == &InputData)
    {
        MDK_Warning("A DataArray tries to Share itself @ DataArray::Share(InputData)")
        return true;
    }

    auto InputLength = InputData.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("DataArray size can not be changed @ DataArray::Share(InputData)")
            return false;
        }
    }

    //--------------------------------------------------------------------------------------------------------

    m_Data = InputData.m_Data; // std::Shared_ptr, self assignment test is not necessary
    m_ElementPointer = m_Data->ElementPointer;

    return true;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Share(DataArray<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataArray::Share(mdkDataArray* InputData)")
        return false;
    }

    return this->Share(*InputData);
}


template<typename ElementType>
inline
void DataArray<ElementType>::ForceShare(const DataArray<ElementType>& InputData)
{
    // DataArray = DataArray
    if (this == &InputData)
    {
        MDK_Warning("A DataArray tries to ForceShare itself @ DataArray::ForceShare(InputData)")
        return;
    }

    m_Data = InputData.m_Data; // std::Shared_ptr, self assignment check is not necessary
    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::ForceShare(const DataArray<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataArray::ForceShare(mdkDataArray* InputData)")
        return false;
    }

    this->ForceShare(*InputData);

    return true;
}


template<typename ElementType>
inline 
bool DataArray<ElementType>::Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid input @ DataArray::Share(...)")
        return false;
    }

    if (this->IsSizeFixed() == true)
    {
        if (this->GetLength() != InputLength)
        {
            MDK_Error("Size can not change @ DataArray::Share(...)")
            return false;
        }
    }

    m_Data->IsSizeFixed = IsSizeFixed;

    m_Data->Length = InputLength;

    m_Data->DataArray.clear();
    m_Data->DataArray.shrink_to_fit();

    m_Data->ElementPointer = InputElementPointer;

    m_ElementPointer = m_Data->ElementPointer;

    return true;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed)
{
    return this->Share(const_cast<ElementType*>(InputElementPointer), InputLength, IsSizeFixed);
}


template<typename ElementType>
inline
void DataArray<ElementType>::Take(DataArray<ElementType>&& InputData)
{
    this->Take(std::forward<DataArray<ElementType>&>(InputData));
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Take(DataArray<ElementType>& InputData)
{
    // DataArray = DataArray
    if (this == &InputData)
    {
        MDK_Warning("A DataArray tries to take itself @ DataArray::take(InputData)")
        return true;
    }

    auto InputLength = InputData.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Size does not match @ DataArray::Take(InputData)")
            return false;
        }
    }

    if (InputData.IsEmpty() == true)
    {
        if (SelfLength > 0)
        {
            //MDK_Warning("InputData is empty, and this DataArray is set to be empty @ DataArray::Take(InputData)")
            this->Clear();
        }

        return true;
    }

    // DataArrayA = DataArrayA
    if (this->GetElementPointer() == InputData.GetElementPointer())
    {
        //MDK_Warning("A DataArray tries to take itself @ DataArray::Take(InputData)")
        return true;
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

    // now, InputData is not empty, and is not self
    
    //note: m_Data.swap(InputData.m_Data) will invalidate Share()

    m_Data->DataArray = std::move(InputData.m_Data->DataArray);
    m_Data->ElementPointer = InputData.m_Data->ElementPointer;
    m_Data->Length = InputData.m_Data->Length;

    m_ElementPointer = m_Data->ElementPointer;

    // Clear InputData to be empty
    InputData.Clear();

    return true;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Take(DataArray<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataArray::Take(mdkDataArray* InputData)")
        return false;
    }

    return this->Take(*InputData);
}


template<typename ElementType>
inline
void DataArray<ElementType>::SwapSmartPointer(DataArray<ElementType>& InputData)
{
    // DataArray = DataArray
    if (this == &InputData)
    {
        MDK_Warning("A DataArray tries to Swap with itself @ DataArray::SwapSmartPointer(InputData)")
        return;
    }

    m_Data.swap(InputData.m_Data); // shared_ptr self swap check is not necessary

    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline
void DataArray<ElementType>::Clear()
{
    if (!m_Data)
    {
        return;
    }

    m_Data->IsSizeFixed = false;

    m_Data->Length = 0;

    m_Data->DataArray.clear();         // change size
    m_Data->DataArray.shrink_to_fit(); // release memory

    m_Data->ElementPointer = nullptr;

    m_ElementPointer = nullptr;
}


template<typename ElementType>
inline 
bool DataArray<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ DataArray::Resize(int_max InputLength)")
        return false;
    }

try
{
    //--------initialize the DataArray data ----------------------------------------
    if (!m_Data)
    {
        m_Data = std::make_shared<DataArrayData<ElementType>>();
        m_ElementPointer = nullptr;
    }
    //-------------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not be changed @ DataArray::Resize(int_max InputLength)")
        return false;
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    m_Data->DataArray.resize(InputLength);
    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = InputLength;
    m_ElementPointer = m_Data->ElementPointer;
}
catch (...)
{
    MDK_Error("Out of Memory @ DataArray::Resize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = int_max(m_Data->DataArray.size());
    m_ElementPointer = m_Data->ElementPointer;

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ DataArray::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change size @ DataArray::FastResize(int_max InputLength)")
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
        if (InputLength > int_max(m_Data->DataArray.capacity()))
        {
            m_Data->DataArray.clear();
        }

        m_Data->DataArray.resize(InputLength);
    }
   
    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = InputLength;
    m_ElementPointer = m_Data->ElementPointer;
}
catch (...)
{
    MDK_Error("Out of Memory @ DataArray::FastResize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = int_max(m_Data->DataArray.size());
    m_ElementPointer = m_Data->ElementPointer;

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool DataArray<ElementType>::ReserveCapacity(int_max InputElementNumber)
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
        m_Data->DataArray.reserve(InputElementNumber);
        m_Data->ElementPointer = m_Data->DataArray.data();
        m_ElementPointer = m_Data->ElementPointer;
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ DataArray::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void DataArray<ElementType>::ReleaseUnusedCapacity()
{
    if (!m_Data)
    {
        return;
    }

    m_Data->DataArray.shrink_to_fit();
    m_Data->ElementPointer = m_Data->DataArray.data();
    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline 
void DataArray<ElementType>::FixSize()
{
    if (!m_Data)
    {
        this->Resize(0);
    }

    m_Data->IsSizeFixed = true;
}


template<typename ElementType>
inline
bool DataArray<ElementType>::IsSizeFixed() const
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
bool DataArray<ElementType>::IsEmpty() const
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
bool DataArray<ElementType>::IsShared() const
{
    return (m_Data.use_count() > 1);
}


template<typename ElementType>
inline
int_max DataArray<ElementType>::GetLength() const
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
int_max DataArray<ElementType>::GetElementNumber() const
{
    return this->GetLength();
}


template<typename ElementType>
inline
const ElementType& DataArray<ElementType>::GetErrorElement()  const
{
    if (!m_Data)
    {
        MDK_Error("m_Data is empty @ DataArray::GetErrorElement()")
    }

    return m_Data->ErrorElement;
}


template<typename ElementType>
inline
ElementType* DataArray<ElementType>::GetElementPointer()
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
const ElementType* DataArray<ElementType>::GetElementPointer() const
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
ElementType* DataArray<ElementType>::GetPointer()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline
const ElementType* DataArray<ElementType>::GetPointer() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline 
ElementType* DataArray<ElementType>::begin()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline 
const ElementType* DataArray<ElementType>::begin() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline
ElementType* DataArray<ElementType>::end()
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
const ElementType* DataArray<ElementType>::end() const
{
    auto EndPtr = this->GetElementPointer();

    if (EndPtr != nullptr)
    {
        EndPtr += this->GetElementNumber();
    }

    return EndPtr;
}


//----------- Get/Set DataArray(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& DataArray<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_DataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber()|| Index < 0)
    {
        MDK_Error("Invalid Input @ DataArray::operator[](i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataArray_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataArray<ElementType>::operator[](int_max Index) const
{
#if defined(MDK_DEBUG_DataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataArray::operator[](i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataArray_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
ElementType& DataArray<ElementType>::operator()(int_max Index)
{
#if defined(MDK_DEBUG_DataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataArray::operator()(i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataArray_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataArray<ElementType>::operator()(int_max Index) const
{
#if defined(MDK_DEBUG_DataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataArray::operator()(i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataArray_Operator_CheckBound

    return m_ElementPointer[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& DataArray<ElementType>::at(int_max Index)
{
    if (Index >= this->GetElementNumber() || Index < 0)
	{
		MDK_Error("Invalid Input @ DataArray::at(i)")
        
        return m_Data->ErrorElement;
	}

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataArray<ElementType>::at(int_max Index) const
{
    if (Index >= this->GetElementNumber() || Index < 0)
	{
		MDK_Error("Invalid Input @ DataArray::at(i) const")
        
        return m_Data->ErrorElement;
	}

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Append(ElementType Element)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not change @ DataArray::Append(ElementType Element)")
        return false;
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);

    return true;
}

/*
template<typename ElementType>
inline 
bool DataArray<ElementType>::Append(const std::initializer_list<ElementType>& InputData)
{
    return this->Append(InputData.begin(), int_max(InputData.size()));
}
*/
/*
template<typename ElementType>
inline
bool DataArray<ElementType>::Append(const std::vector<ElementType>& InputData)
{
    return this->Append(InputData.data(), int_max(InputData.size()));
}
*/
/*
template<typename ElementType>
inline
bool DataArray<ElementType>::Append(const DenseMatrix<ElementType>& InputData)
{
    if (InputData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DataArray::Append(const DenseMatrix<ElementType>& InputData)")
        return false;
    }

    return this->Append(InputData.GetElementPointer(), InputData.GetElementNumber());
}
*/
/*
template<typename ElementType>
inline
bool DataArray<ElementType>::Append(const SimpleDataArray<ElementType>& InputData)
{
	return this->Append(InputData.GetElementPointer(), InputData.GetElementNumber());
}
*/
/*
template<typename ElementType>
inline
bool DataArray<ElementType>::Append(const DataArray<ElementType>& InputData)
{
    return this->Append(InputData.GetElementPointer(), InputData.GetElementNumber());
}
*/

template<typename ElementType>
inline 
bool DataArray<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not change @ DataArray::Append(const ElementType* InputData, int_max InputLength)")
        return false;
    }

    if (InputData == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ DataArray::Append(const ElementType* InputData, int_max InputLength)")
        return false;
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_Data->DataArray[i] = InputData[i - SelfLength];
    }

    return true;
}


template<typename ElementType>
inline 
bool DataArray<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DataArray::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const DataArray<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const SimpleDataArray<int_max>& IndexList)
{
	return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not change @ DataArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DataArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ DataArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ DataArray::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    if (ListLength == 1)
    {
        m_Data->DataArray.erase(m_Data->DataArray.begin() + IndexList[0], m_Data->DataArray.begin() + IndexList[0] + 1);

        m_Data->Length -= 1;

        m_Data->ElementPointer = m_Data->DataArray.data();

        m_ElementPointer = m_Data->ElementPointer;
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
                MDK_Warning("duplicate Input @ DataArray::Delete(const int_max* IndexPtr, int_max ListLength)")
            }
            else
            {
                m_Data->DataArray.erase(m_Data->DataArray.begin() + Index_i, m_Data->DataArray.begin() + Index_i + 1);

                Index_prev = Index_i;

                m_Data->Length -= 1;
            }
        }

        m_Data->ElementPointer = m_Data->DataArray.data();

        m_ElementPointer = m_Data->ElementPointer;
    }

    return true;
}


template<typename ElementType>
inline 
bool DataArray<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not change @ DataArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DataArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ DataArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    m_Data->DataArray.erase(m_Data->DataArray.begin() + Index_start, m_Data->DataArray.begin() + Index_end + 1);
    
    m_Data->Length -= Index_end - Index_start + 1;

    m_Data->ElementPointer = m_Data->DataArray.data();

    m_ElementPointer = m_Data->ElementPointer;

    return true;
}



template<typename ElementType>
inline bool DataArray<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputData)
{
    return this->Insert(Index, InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Insert(int_max Index, const SimpleDataArray<ElementType>& InputData)
{
	return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Insert(int_max Index, const DataArray<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DataArray<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataArray Size can not change @ DataArray::Insert(Index, const ElementType* InputData, int_max InputLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DataArray::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DataArray::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    if (!m_Data)
    {
        this->Resize(0);
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    m_Data->DataArray.insert(m_Data->DataArray.begin() + Index, InputData, InputData + InputLength);

    m_Data->Length = InputLength;

    m_Data->ElementPointer = m_Data->DataArray.data();

    m_ElementPointer = m_Data->ElementPointer;

    return true;
}


template<typename ElementType>
inline 
bool DataArray<ElementType>::Push(ElementType Element)
{
    return this->Append(std::move(Element));
}


template<typename ElementType>
inline
ElementType DataArray<ElementType>::Pop()
{
    auto OutputElement = ElementType(0);

    auto CurrentElementNumber = this->GetElementNumber();

    if (CurrentElementNumber > 0)
    {
        auto tempIndex = CurrentElementNumber - 1;

        OutputElement = (*this)[tempIndex];

        this->Delete(tempIndex);
    }
    else
    {
        MDK_Error("Self is empty @ DataArray::Pop()")
    }
   
    return OutputElement;
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    DataArray<ElementType> Subset;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DataArray::GetSubSet(...)")
        return Subset;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ DataArray::GetSubSet(...)")
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
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline 
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const std::vector<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const SimpleDataArray<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const DataArray<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const DenseVector<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
	return this->GetSubSet(IndexList.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
DataArray<ElementType> DataArray<ElementType>::GetSubSet(const int_max* IndexList, int_max ListLength)
{
    DataArray<ElementType> SubSet;

    if (IndexList == nullptr || ListLength <= 0)
    {
        return SubSet;
    }

    auto ElementNumber = this->GetElementNumber();

    if (ListLength > ElementNumber)
    {
        MDK_Error("Invalid ListLength @ DataArray::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(ListLength);

    for (int_max k = 0; k < ListLength; ++k)
    {
        auto tempIndex = IndexList[k];

        if (tempIndex < 0 || tempIndex >= ElementNumber)
        {
            MDK_Error("Invalid index @ DataArray::GetSubSet(...)")
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
DataArray<int_max> DataArray<ElementType>::Find(MatchFunctionType MatchFunction)
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DataArray<int_max> DataArray<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DataArray<int_max> DataArray<ElementType>::Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction)
{
    DataArray<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ DataArray::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DataArray::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ DataArray::Find(...)")
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
DataArray<int_max> DataArray<ElementType>::ExactMatch(const ElementType& InputElement) const
{
	return this->Find([&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
inline
int_max DataArray<ElementType>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
{
	return this->Find(first_or_last, [&](const ElementType& Element){return Element == InputElement; });
}

template<typename ElementType>
template<typename CompareFunctionType>
inline
DataArray<int_max> DataArray<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength()-1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
DataArray<int_max> DataArray<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    DataArray<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DataArray::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ DataArray::Find(...)")
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
void DataArray<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
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
void DataArray<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}

}//end namespace mdk

#endif