#ifndef __mdkDataContainer_hpp
#define __mdkDataContainer_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DataContainer<ElementType>::DataContainer()
{
    this->Resize(0);
}


template<typename ElementType>
inline
DataContainer<ElementType>::DataContainer(const std::initializer_list<ElementType>& InputList)
{
    if (InputList.size() == 0)
    {
        this->Resize(0);
    }
    else
    {
        this->Resize(0);

        (*this) = InputList;
    }
}


template<typename ElementType>
inline
DataContainer<ElementType>::DataContainer(const ElementType& Element)
{
    this->Resize(1);

    (*this)(0) = Element;
}


template<typename ElementType>
inline
DataContainer<ElementType>::DataContainer(const DataContainer<ElementType>& InputData, 
                                          ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy)
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
DataContainer<ElementType>::DataContainer(DataContainer<ElementType>&& InputData) noexcept
{
    m_Data = std::move(InputData.m_Data);

    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline
DataContainer<ElementType>::~DataContainer()
{
}


template<typename ElementType>
inline
void DataContainer<ElementType>::operator=(const DataContainer<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move assignment operator
template<typename ElementType>
inline
void DataContainer<ElementType>::operator=(DataContainer<ElementType>&& InputData)
{
    this->Take(std::forward<DataContainer<ElementType>&>(InputData));
}


template<typename ElementType>
inline
void DataContainer<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (ElementNumber != 1)
        {
            MDK_Error("Can not change size @ DataContainer::operator=(Element)")
            return;
        }

        (*this)[0] = Element;
    }
    else
    {
        if (ElementNumber == 0)
        {
            this->Resize(1);

            (*this)[0] = Element;
        }
        else if (ElementNumber == 1)
        {
            (*this)[0] = Element;
        }
        else
        {
            MDK_Warning("The size changes to 1x1 @ DataContainer::operator=(Element)")

            this->Clear();

            this->Resize(1);

            (*this)[0] = Element;
        }
    }
}


template<typename ElementType>
inline
void DataContainer<ElementType>::operator=(const std::initializer_list<ElementType>& InputData)
{
    auto InputLength = int_max(InputData.size());

    if (InputLength <= 0)
    {
        MDK_Error("Input is empty @ DataContainer::operator=(initializer_list)")
        return;
    }

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (SelfLength != InputLength)
        {
            MDK_Error("Can not change size @ DataContainer::operator=(initializer_list)")
        }

        return;
    }
    else
    {
        this->Clear();    
        this->Append(InputData);
    }
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Copy(const DataContainer<ElementType>& InputData)
{
    if (this == &InputData)
    {
        MDK_Warning("A DataContainer tries to Copy itself @ DataContainer::Copy(InputData)")
        return true;
    }

    if (InputData.IsEmpty() == true)
    {
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change size @ DataContainer::Copy(InputData)")
                return false;
            }

            return true;
        }
        else
        {            
            this->Clear();
            return true;
        }       
    }

    // copy data
    return this->Copy(InputData.GetElementPointer(), InputData.GetRowNumber(), InputData.GetColNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Copy(const DataContainer<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataContainer::Copy(DataContainer* InputData)")
        return false;
    }

    return this->Copy(*InputData);
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Input pointer is nullptr @ DataContainer::Copy(ElementType*, InputLength)")
        return false;
    }

    // if this DataContainer is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A DataContainer tries to Copy itself @ DataContainer::Copy(ElementType*, RowNumber, ColNumber)")
            return true;
        }
    }

    //------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    //------------------------------------------------------------------

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Can not change size @ DataContainer::Copy(ElementType*, InputLength)")
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
bool DataContainer<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ DataContainer::Fill")
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
bool DataContainer<ElementType>::Share(DataContainer<ElementType>& InputData)
{
    // DataContainer = DataContainer
    if (this == &InputData)
    {
        MDK_Warning("A DataContainer tries to Share itself @ DataContainer::Share(InputData)")
        return true;
    }

    auto InputLength = InputData.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("DataContainer size can not be changed @ DataContainer::Share(InputData)")
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
bool DataContainer<ElementType>::Share(DataContainer<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataContainer::Share(mdkDataContainer* InputData)")
        return false;
    }

    return this->Share(*InputData);
}


template<typename ElementType>
inline
void DataContainer<ElementType>::ForceShare(const DataContainer<ElementType>& InputData)
{
    // DataContainer = DataContainer
    if (this == &InputData)
    {
        MDK_Warning("A DataContainer tries to ForceShare itself @ DataContainer::ForceShare(InputData)")
        return;
    }

    m_Data = InputData.m_Data; // std::Shared_ptr, self assignment check is not necessary
    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::ForceShare(const DataContainer<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataContainer::ForceShare(mdkDataContainer* InputData)")
        return false;
    }

    this->ForceShare(*InputData);

    return true;
}


template<typename ElementType>
inline
void DataContainer<ElementType>::Take(DataContainer<ElementType>&& InputData)
{
    this->Take(std::forward<DataContainer<ElementType>&>(InputData));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Take(DataContainer<ElementType>& InputData)
{
    // DataContainer = DataContainer
    if (this == &InputData)
    {
        MDK_Warning("A DataContainer tries to take itself @ DataContainer::take(InputData)")
        return true;
    }

    auto InputLength = InputData.GetElementNumber();

    auto SelfLength = this->GetElementNumber();

    if (this->IsSizeFixed() == true)
    {
        if (InputLength != SelfLength)
        {
            MDK_Error("Size does not match @ DataContainer::Take(InputData)")
            return false;
        }
    }

    if (InputData.IsEmpty() == true)
    {
        if (SelfLength > 0)
        {
            //MDK_Warning("InputData is empty, and this DataContainer is set to be empty @ DataContainer::Take(InputData)")
            this->Clear();
        }

        return true;
    }

    // DataContainerA = DataContainerA
    if (this->GetElementPointer() == InputData.GetElementPointer())
    {
        //MDK_Warning("A DataContainer tries to take itself @ DataContainer::Take(InputData)")
        return true;
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
bool DataContainer<ElementType>::Take(DataContainer<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ DataContainer::Take(mdkDataContainer* InputData)")
        return false;
    }

    return this->Take(*InputData);
}


template<typename ElementType>
inline
void DataContainer<ElementType>::SwapSmartPointer(DataContainer<ElementType>& InputData)
{
    // DataContainer = DataContainer
    if (this == &InputData)
    {
        MDK_Warning("A DataContainer tries to Swap with itself @ DataContainer::SwapSmartPointer(InputData)")
        return;
    }

    m_Data.swap(InputData.m_Data); // shared_ptr self swap check is not necessary

    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline
void DataContainer<ElementType>::Clear()
{
    m_Data->IsSizeFixed = false;

    m_Data->Length = 0;

    m_Data->DataArray.clear();

    m_Data->ElementPointer = nullptr;

    m_ElementPointer = nullptr;
}


template<typename ElementType>
inline 
bool DataContainer<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ DataContainer::Resize(int_max InputLength)")
        return false;
    }

try
{
    //--------initialize the DataContainer data ----------------------------------------
    if (!m_Data)
    {
        m_Data = std::make_shared<DataContainerData<ElementType>>();
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
        MDK_Error("DataContainer Size can not be changed @ DataContainer::Resize(int_max InputLength)")
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
    MDK_Error("Out of Memory @ DataContainer::Resize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = int_max(m_Data->DataArray.size());
    m_ElementPointer = m_Data->ElementPointer;

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ DataContainer::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change size @ DataContainer::FastResize(int_max InputLength)")
        return false;
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
    MDK_Error("Out of Memory @ DataContainer::FastResize(int_max InputLength)")

    m_Data->ElementPointer = m_Data->DataArray.data();
    m_Data->Length = int_max(m_Data->DataArray.size());
    m_ElementPointer = m_Data->ElementPointer;

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool DataContainer<ElementType>::ReserveCapacity(int_max InputElementNumber)
{

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
    MDK_Error("Out of Memory @ DataContainer::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void DataContainer<ElementType>::Squeeze()
{
    m_Data->DataArray.shrink_to_fit();
    m_Data->ElementPointer = m_Data->DataArray.data();
    m_ElementPointer = m_Data->ElementPointer;
}


template<typename ElementType>
inline 
void DataContainer<ElementType>::FixSize()
{
    m_Data->IsSizeFixed = true;
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::IsSizeFixed() const
{
    return m_Data->IsSizeFixed;
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::IsEmpty() const
{
    return (m_Data->Length <= 0);
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::IsShared() const
{
    return (m_Data.use_count() > 1);
}


template<typename ElementType>
inline
int_max DataContainer<ElementType>::GetElementNumber() const
{
    return m_Data->Length;
}


template<typename ElementType>
inline
const ElementType& DataContainer<ElementType>::GetErrorElement()  const
{
    return m_Data->ErrorElement;
}


template<typename ElementType>
inline
ElementType* DataContainer<ElementType>::GetElementPointer()
{
    return m_Data->ElementPointer;
}


template<typename ElementType>
inline
const ElementType* DataContainer<ElementType>::GetElementPointer() const
{
    return m_Data->ElementPointer;
}


template<typename ElementType>
inline ElementType* DataContainer<ElementType>::begin()
{
    return m_Data->ElementPointer; // the pointer of the first element
}


template<typename ElementType>
inline const ElementType* DataContainer<ElementType>::begin() const
{
    return m_Data->ElementPointer;
}


template<typename ElementType>
inline const ElementType* DataContainer<ElementType>::end() const
{
    return m_Data->ElementPointer + m_Data->Length;
}


//----------- Get/Set DataContainer(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& DataContainer<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_DataContainer_Operator_CheckBound)

    if (Index >= this->GetElementNumber()|| Index < 0)
    {
        MDK_Error("Invalid Input @ DataContainer::operator[](i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataContainer_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataContainer<ElementType>::operator[](int_max Index) const
{
#if defined(MDK_DEBUG_DataContainer_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataContainer::operator[](i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataContainer_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
ElementType& DataContainer<ElementType>::operator()(int_max Index)
{
#if defined(MDK_DEBUG_DataContainer_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataContainer::operator()(i)")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataContainer_Operator_CheckBound

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataContainer<ElementType>::operator()(int_max Index) const
{
#if defined(MDK_DEBUG_DataContainer_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ DataContainer::operator()(i) const")

        return m_Data->ErrorElement;
    }

#endif //MDK_DEBUG_DataContainer_Operator_CheckBound

    return m_ElementPointer[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& DataContainer<ElementType>::at(int_max Index)
{
    if (Index >= this->GetElementNumber() || Index < 0)
	{
		MDK_Error("Invalid Input @ DataContainer::at(i)")
        
        return m_Data->ErrorElement;
	}

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
const ElementType& DataContainer<ElementType>::at(int_max Index) const
{
    if (Index >= this->GetElementNumber() || Index < 0)
	{
		MDK_Error("Invalid Input @ DataContainer::at(i) const")
        
        return m_Data->ErrorElement;
	}

    return m_ElementPointer[Index];
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Append(const ElementType& Element)
{
    return this->Append(&Element, 1);
}


template<typename ElementType>
inline 
bool DataContainer<ElementType>::Append(const std::initializer_list<ElementType>& InputData)
{
    return this->Append(InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Append(const std::vector<ElementType>& InputData)
{
    return this->Append(InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Append(const DenseMatrix<ElementType>& InputData)
{
    if (InputData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DataContainer::Append(const DenseMatrix<ElementType>& InputData)")
        return false;
    }

    return this->Append(InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Append(const DataContainer<ElementType>& InputData)
{
    return this->Append(InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline 
bool DataContainer<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataContainer Size can not change @ DataContainer::Append(const ElementType* InputData, int_max InputLength)")
        return false;
    }

    if (InputData == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ DataContainer::Append(const ElementType* InputData, int_max InputLength)")
        return false;
    }

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
bool DataContainer<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DataContainer::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Delete(const DataContainer<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataContainer Size can not change @ DataContainer::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DataContainer::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || Length <= 0)
    {
        MDK_Error("Empty Input @ DataContainer::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + Length; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ DataContainer::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    std::vector<int_max> IndexList_max_to_min(Length);

    for (int_max i = 0; i < Length; ++i)
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
            MDK_Warning("duplicate Input @ DataContainer::Delete(const int_max* IndexPtr, int_max Length)")
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

    return true;
}


template<typename ElementType>
inline 
bool DataContainer<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataContainer Size can not change @ DataContainer::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DataContainer::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ DataContainer::Delete(int_max Index_start, int_max Index_end)")
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
inline bool DataContainer<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputData)
{
    return this->Insert(Index, InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputData)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DataContainer::Insert(Index, DenseMatrix)")
        return false;
    }

    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Insert(int_max Index, const DataContainer<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DataContainer<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("DataContainer Size can not change @ DataContainer::Insert(Index, const ElementType* InputData, int_max InputLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DataContainer::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DataContainer::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    m_Data->CopyDataToInternalDataArrayIfNecessary();

    m_Data->DataArray.insert(m_Data->DataArray.begin() + Index, InputData, InputData + InputLength);

    m_Data->Length = InputLength;

    m_Data->ElementPointer = m_Data->DataArray.data();

    m_ElementPointer = m_Data->ElementPointer;

    return true;
}


}//end namespace mdk

#endif