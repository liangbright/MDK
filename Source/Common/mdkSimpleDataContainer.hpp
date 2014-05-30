#ifndef __mdkSimpleDataContainer_hpp
#define __mdkSimpleDataContainer_hpp


namespace mdk
{
 
template<typename ElementType>
inline
SimpleDataContainer<ElementType>::SimpleDataContainer()
{
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType>::SimpleDataContainer(const std::initializer_list<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType>::SimpleDataContainer(const std::vector<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType>::SimpleDataContainer(const SimpleDataContainer<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move constructor
template<typename ElementType>
inline
SimpleDataContainer<ElementType>::SimpleDataContainer(SimpleDataContainer<ElementType>&& InputData) noexcept
{
    m_DataArray = std::move(InputData.m_DataArray);
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType>::~SimpleDataContainer()
{
}


template<typename ElementType>
inline 
std::vector<ElementType>& SimpleDataContainer<ElementType>::StdVector()
{
    return m_DataArray;
}


template<typename ElementType>
inline
const std::vector<ElementType>& SimpleDataContainer<ElementType>::StdVector() const
{
    return m_DataArray;
}


template<typename ElementType>
inline
void SimpleDataContainer<ElementType>::operator=(const SimpleDataContainer<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move assignment operator
template<typename ElementType>
inline
void SimpleDataContainer<ElementType>::operator=(SimpleDataContainer<ElementType>&& InputData)
{
    m_DataArray = std::move(InputData.m_DataArray);
}


template<typename ElementType>
inline
void SimpleDataContainer<ElementType>::operator=(const std::initializer_list<ElementType>& InputData)
{
    m_DataArray = InputData;
}


template<typename ElementType>
inline
void SimpleDataContainer<ElementType>::operator=(const std::vector<ElementType>& InputData)
{
    m_DataArray = InputData;
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Copy(const SimpleDataContainer<ElementType>& InputData)
{
    if (this == &InputData)
    {
        MDK_Warning("A SimpleDataContainer tries to Copy itself @ SimpleDataContainer::Copy(InputData)")
        return true;
    }

    if (InputData.IsEmpty() == true)
    {
        this->Clear();
        return true;
    }

    // copy data
    return this->Copy(InputData.GetElementPointer(), InputData.GetLength());
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Copy(const SimpleDataContainer<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ SimpleDataContainer::Copy(SimpleDataContainer* InputData)")
        return false;
    }

    return this->Copy(*InputData);
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Input pointer is nullptr @ SimpleDataContainer::Copy(ElementType*, InputLength)")
        return false;
    }

    // if this SimpleDataContainer is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A SimpleDataContainer tries to Copy itself @ SimpleDataContainer::Copy(ElementType*, RowNumber, ColNumber)")
            return true;
        }
    }

    //------------------------------------------------------------------

    m_DataArray.resize(InputLength);

    //copy data ----------------------------------------------------------

    auto BeginPointer = this->GetElementPointer();

    auto tempPtr = InputElementPointer;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + InputLength; ++Ptr, ++tempPtr)
    {
        Ptr[0] = tempPtr[0];
    }

    return true;
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ SimpleDataContainer::Fill")
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
void SimpleDataContainer<ElementType>::Clear()
{
    m_DataArray.clear();         // change size
    m_DataArray.shrink_to_fit(); // release memory
}


template<typename ElementType>
inline 
bool SimpleDataContainer<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ SimpleDataContainer::Resize(int_max InputLength)")
        return false;
    }

try
{
    //-------------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    m_DataArray.resize(InputLength);
}
catch (...)
{
    MDK_Error("Out of Memory @ SimpleDataContainer::Resize(int_max InputLength)")

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ SimpleDataContainer::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

try
{
    if (InputLength != SelfLength)
    {
        if (InputLength > int_max(m_DataArray.capacity()))
        {
            m_DataArray.clear();
        }

        m_DataArray.resize(InputLength);
    }

}
catch (...)
{
    MDK_Error("Out of Memory @ SimpleDataContainer::FastResize(int_max InputLength)")

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::ReserveCapacity(int_max InputElementNumber)
{
try
{
    auto SelfLength = this->GetElementNumber();

    if (InputElementNumber > SelfLength)
    {
        m_DataArray.reserve(InputElementNumber);
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ SimpleDataContainer::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void SimpleDataContainer<ElementType>::Squeeze()
{
    m_DataArray.shrink_to_fit();
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::IsEmpty() const
{
    return (m_DataArray.size() <= 0);
}


template<typename ElementType>
inline
int_max SimpleDataContainer<ElementType>::GetLength() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
int_max SimpleDataContainer<ElementType>::GetElementNumber() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
ElementType* SimpleDataContainer<ElementType>::GetElementPointer()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* SimpleDataContainer<ElementType>::GetElementPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline 
ElementType* SimpleDataContainer<ElementType>::begin()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline 
const ElementType* SimpleDataContainer<ElementType>::begin() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
ElementType* SimpleDataContainer<ElementType>::end()
{
    auto EndPtr = m_DataArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_DataArray.size();
    }

    return EndPtr;
}


template<typename ElementType>
inline
const ElementType* SimpleDataContainer<ElementType>::end() const
{
    auto EndPtr = m_DataArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_DataArray.size();
    }

    return EndPtr;
}


//----------- Get/Set SimpleDataContainer(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& SimpleDataContainer<ElementType>::operator[](int_max Index)
{
    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& SimpleDataContainer<ElementType>::operator[](int_max Index) const
{
    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& SimpleDataContainer<ElementType>::operator()(int_max Index)
{
    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& SimpleDataContainer<ElementType>::operator()(int_max Index) const
{
    return m_DataArray[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& SimpleDataContainer<ElementType>::at(int_max Index)
{
    return m_DataArray.at(Index);
}


template<typename ElementType>
inline
const ElementType& SimpleDataContainer<ElementType>::at(int_max Index) const
{
    return m_DataArray.at(Index);
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Append(ElementType Element)
{
    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataContainer<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (InputData == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ SimpleDataContainer::Append(const ElementType* InputData, int_max InputLength)")
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
bool SimpleDataContainer<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ SimpleDataContainer::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Delete(const SimpleDataContainer<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ SimpleDataContainer::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ SimpleDataContainer::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ SimpleDataContainer::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

    if (ListLength == 1)
    {
        m_DataArray.erase(m_DataArray.begin() + IndexList[0], m_DataArray.begin() + IndexList[0] + 1);
    }
    else
    {
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
                MDK_Warning("duplicate Input @ SimpleDataContainer::Delete(const int_max* IndexPtr, int_max Length)")
            }
            else
            {
                m_DataArray.erase(m_DataArray.begin() + Index_i, m_DataArray.begin() + Index_i + 1);

                Index_prev = Index_i;
            }
        }
    }

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataContainer<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ SimpleDataContainer::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ SimpleDataContainer::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_DataArray.erase(m_DataArray.begin() + Index_start, m_DataArray.begin() + Index_end + 1);
    
    return true;
}


template<typename ElementType>
inline bool SimpleDataContainer<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputData)
{
    return this->Insert(Index, InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputData)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ SimpleDataContainer::Insert(Index, DenseMatrix)")
        return false;
    }

    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Insert(int_max Index, const SimpleDataContainer<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataContainer<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ SimpleDataContainer::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ SimpleDataContainer::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    m_DataArray.insert(m_DataArray.begin() + Index, InputData, InputData + InputLength);

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataContainer<ElementType>::Push(ElementType Element)
{
    m_DataArray.push_back(std::move(Element));
}


template<typename ElementType>
inline
ElementType SimpleDataContainer<ElementType>::Pop()
{
    return m_DataArray.pop_back();
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    SimpleDataContainer<ElementType> Subset;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataContainer::GetSubSet(...)")
        return Subset;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataContainer::GetSubSet(...)")
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
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList)
{
    return this->GetSubSet(initializer_list.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline 
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(const std::vector<int_max>& IndexList)
{
    return this->GetSubSet(initializer_list.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
    return this->GetSubSet(initializer_list.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(const SimpleDataContainer<int_max>& IndexList)
{
    return this->GetSubSet(initializer_list.begin(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
SimpleDataContainer<ElementType> SimpleDataContainer<ElementType>::GetSubSet(const int_max* IndexList, int_max ListLength)
{
    SimpleDataContainer<ElementType> SubSet;

    auto ElementNumber = this->GetElementNumber();

    if (IndexList == nullptr || ListLength < 0 || ListLength >= ElementNumber)
    {
        MDK_Error("Invalid input @ SimpleDataContainer::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(ListLength);

    for (int_max k = 0; k < ListLength; ++k)
    {
        auto tempIndex = IndexList[k];

        if (tempIndex < 0 || tempIndex >= ElementNumber)
        {
            MDK_Error("Invalid index @ SimpleDataContainer::GetSubSet(...)")
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
SimpleDataContainer<int_max> SimpleDataContainer<ElementType>::Find(MatchFunctionType MatchFunction)
{
    return this->Find(this->GetLength(), 0, this->GetLength(), MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
SimpleDataContainer<int_max> SimpleDataContainer<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return this->Find(MaxOutputNumber, 0, this->GetLength(), MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
SimpleDataContainer<int_max> SimpleDataContainer<ElementType>::
Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction)
{
    SimpleDataContainer<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ SimpleDataContainer::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataContainer::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataContainer::Find(...)")
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

    DenseMatrix<ElementType> ColVector;

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
template<typename CompareFunctionType>
inline
SimpleDataContainer<int_max> SimpleDataContainer<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength(), CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
SimpleDataContainer<int_max> SimpleDataContainer<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    SimpleDataContainer<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ SimpleDataContainer::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataContainer::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataContainer::Find(...)")
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

    IndexList.FastResize(1, ElementNumber);

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
void SimpleDataContainer<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
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
void SimpleDataContainer<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}

}//end namespace mdk

#endif