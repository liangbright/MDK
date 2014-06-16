#ifndef __mdkSimpleDataArray_hpp
#define __mdkSimpleDataArray_hpp


namespace mdk
{
 
template<typename ElementType>
inline
SimpleDataArray<ElementType>::SimpleDataArray()
{
}


template<typename ElementType>
inline
SimpleDataArray<ElementType>::SimpleDataArray(const std::initializer_list<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
SimpleDataArray<ElementType>::SimpleDataArray(const std::vector<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
SimpleDataArray<ElementType>::SimpleDataArray(const SimpleDataArray<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move constructor
template<typename ElementType>
inline
SimpleDataArray<ElementType>::SimpleDataArray(SimpleDataArray<ElementType>&& InputData) noexcept
{
    m_DataArray = std::move(InputData.m_DataArray);
}


template<typename ElementType>
inline
SimpleDataArray<ElementType>::~SimpleDataArray()
{
}


template<typename ElementType>
inline 
std::vector<ElementType>& SimpleDataArray<ElementType>::StdVector()
{
    return m_DataArray;
}


template<typename ElementType>
inline
const std::vector<ElementType>& SimpleDataArray<ElementType>::StdVector() const
{
    return m_DataArray;
}


template<typename ElementType>
inline
void SimpleDataArray<ElementType>::operator=(const SimpleDataArray<ElementType>& InputData)
{
    this->Copy(InputData);
}


// move assignment operator
template<typename ElementType>
inline
void SimpleDataArray<ElementType>::operator=(SimpleDataArray<ElementType>&& InputData)
{
    m_DataArray = std::move(InputData.m_DataArray);
}


template<typename ElementType>
inline
void SimpleDataArray<ElementType>::operator=(const std::initializer_list<ElementType>& InputData)
{
    m_DataArray = InputData;
}


template<typename ElementType>
inline
void SimpleDataArray<ElementType>::operator=(const std::vector<ElementType>& InputData)
{
    m_DataArray = InputData;
}


template<typename ElementType>
inline
void SimpleDataArray<ElementType>::operator=(const DenseMatrix<ElementType>& InputData)
{
    auto InputLength = InputData.GetElementNumber();

    m_DataArray.resize(InputLength);

    for (int_max k = 0; k < InputLength; ++k)
    {
        m_DataArray[k] = InputData[k];
    }
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Copy(const SimpleDataArray<ElementType>& InputData)
{
    if (this == &InputData)
    {
        MDK_Warning("A SimpleDataArray tries to Copy itself @ SimpleDataArray::Copy(InputData)")
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
bool SimpleDataArray<ElementType>::Copy(const SimpleDataArray<ElementType>* InputData)
{
    if (InputData == nullptr)
    {
        MDK_Error("Input is nullptr @ SimpleDataArray::Copy(SimpleDataArray* InputData)")
        return false;
    }

    return this->Copy(*InputData);
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
        MDK_Error("Input pointer is nullptr @ SimpleDataArray::Copy(ElementType*, InputLength)")
        return false;
    }

    // if this SimpleDataArray is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A SimpleDataArray tries to Copy itself @ SimpleDataArray::Copy(ElementType*, RowNumber, ColNumber)")
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
bool SimpleDataArray<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ SimpleDataArray::Fill")
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
void SimpleDataArray<ElementType>::Clear()
{
    m_DataArray.clear();         // change size
    m_DataArray.shrink_to_fit(); // release memory
}


template<typename ElementType>
inline 
bool SimpleDataArray<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ SimpleDataArray::Resize(int_max InputLength)")
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
    MDK_Error("Out of Memory @ SimpleDataArray::Resize(int_max InputLength)")

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ SimpleDataArray::FastResize(int_max InputLength)")
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
    MDK_Error("Out of Memory @ SimpleDataArray::FastResize(int_max InputLength)")

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::ReserveCapacity(int_max InputElementNumber)
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
    MDK_Error("Out of Memory @ SimpleDataArray::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void SimpleDataArray<ElementType>::Squeeze()
{
    m_DataArray.shrink_to_fit();
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::IsEmpty() const
{
    return (m_DataArray.size() <= 0);
}


template<typename ElementType>
inline
int_max SimpleDataArray<ElementType>::GetLength() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
int_max SimpleDataArray<ElementType>::GetElementNumber() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
ElementType* SimpleDataArray<ElementType>::GetElementPointer()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* SimpleDataArray<ElementType>::GetElementPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
ElementType* SimpleDataArray<ElementType>::GetPointer()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* SimpleDataArray<ElementType>::GetPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline 
ElementType* SimpleDataArray<ElementType>::begin()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline 
const ElementType* SimpleDataArray<ElementType>::begin() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
ElementType* SimpleDataArray<ElementType>::end()
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
const ElementType* SimpleDataArray<ElementType>::end() const
{
    auto EndPtr = m_DataArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_DataArray.size();
    }

    return EndPtr;
}


//----------- Get/Set SimpleDataArray(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& SimpleDataArray<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_SimpleDataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

#endif //MDK_DEBUG_SimpleDataArray_Operator_CheckBound

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& SimpleDataArray<ElementType>::operator[](int_max Index) const
{
#if defined(MDK_DEBUG_SimpleDataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

#endif //MDK_DEBUG_SimpleDataArray_Operator_CheckBound

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& SimpleDataArray<ElementType>::operator()(int_max Index)
{
#if defined(MDK_DEBUG_SimpleDataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

#endif //MDK_DEBUG_SimpleDataArray_Operator_CheckBound

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& SimpleDataArray<ElementType>::operator()(int_max Index) const
{
#if defined(MDK_DEBUG_SimpleDataArray_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

#endif //MDK_DEBUG_SimpleDataArray_Operator_CheckBound

    return m_DataArray[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& SimpleDataArray<ElementType>::at(int_max Index)
{
    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

    return m_DataArray.at(Index);
}


template<typename ElementType>
inline
const ElementType& SimpleDataArray<ElementType>::at(int_max Index) const
{
    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ SimpleDataArray::operator[](i)")
        return m_DataArray[0];
    }

    return m_DataArray.at(Index);
}


template<typename ElementType>
inline
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    SimpleDataArray<ElementType> Subset;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataArray::GetSubSet(...)")
        return Subset;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataArray::GetSubSet(...)")
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
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(const std::vector<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.GetElementPointer(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(const SimpleDataArray<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.GetElementPointer(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
SimpleDataArray<ElementType> SimpleDataArray<ElementType>::GetSubSet(const int_max* IndexList, int_max ListLength)
{
    SimpleDataArray<ElementType> SubSet;

    if (IndexList == nullptr || ListLength <= 0)
    {
        return SubSet;
    }

    auto ElementNumber = this->GetElementNumber();

    if (ListLength > ElementNumber)
    {
        MDK_Error("Invalid ListLength @ SimpleDataArray::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(ListLength);

    for (int_max k = 0; k < ListLength; ++k)
    {
        auto tempIndex = IndexList[k];

        if (tempIndex < 0 || tempIndex >= ElementNumber)
        {
            MDK_Error("Invalid index @ SimpleDataArray::GetSubSet(...)")
                SubSet.Clear();
            return SubSet;
        }

        SubSet[k] = (*this)[tempIndex];
    }

    return SubSet;
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSetData)
{
    if (IndexList.size() != SubSetData.size())
    {
        MDK_Error("IndexList.size() != SubSetData.size() @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.begin(), SubSetData.begin(), int_max(SubSetData.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSetData)
{
    if (IndexList.size() != SubSetData.size())
    {
        MDK_Error("IndexList.size() != SubSetData.size() @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.data(), SubSetData.data(), int_max(SubSetData.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubSetData)
{
    if (IndexList.GetElementNumber() != SubSetData.GetElementNumber())
    {
        MDK_Error("IndexList.size() != SubSetData.size() @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.GetElementPointer(), SubSetData.GetElementPointer(), int_max(SubSetData.GetElementNumber()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::SetSubSet(const SimpleDataArray<int_max>& IndexList, const SimpleDataArray<ElementType>& SubSetData)
{
    if (IndexList.GetElementNumber() != SubSetData.GetElementNumber())
    {
        MDK_Error("IndexList.size() != SubSetData.size() @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.GetElementPointer(), SubSetData.GetElementPointer(), int_max(SubSetData.GetElementNumber()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::SetSubSet(const int_max* IndexList, const ElementType* SubSetData, int_max DataNumber)
{
    if (IndexList == nullptr || SubSetData == nullptr || DataNumber <= 0)
    {
        MDK_Warning("Empty input @ SimpleDataArray::SetSubSet(...)")
        return true;
    }

    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error("Self is empty @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    if (DataNumber > ElementNumber)
    {
        MDK_Error("Invalid DataNumber @ SimpleDataArray::SetSubSet(...)")
        return false;
    }

    for (int_max k = 0; k < DataNumber; ++k)
    {
        auto Index = IndexList[k];

        if (Index < 0 || Index >= ElementNumber)
        {
            MDK_Error("Invalid Index @ SimpleDataArray::SetSubSet(...)")
            return false;
        }

        (*this)[Index] = SubSetData[k];
    }

    return true;
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Append(ElementType Element)
{
    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataArray<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (InputData == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ SimpleDataArray::Append(const ElementType* InputData, int_max InputLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_DataArray[i] = InputData[i - SelfLength];
    }

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataArray<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ SimpleDataArray::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Delete(const SimpleDataArray<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ SimpleDataArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ SimpleDataArray::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ SimpleDataArray::Delete(const int_max* IndexList, int_max ListLength)")
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
                MDK_Warning("duplicate Input @ SimpleDataArray::Delete(const int_max* IndexPtr, int_max Length)")
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
bool SimpleDataArray<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ SimpleDataArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ SimpleDataArray::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_DataArray.erase(m_DataArray.begin() + Index_start, m_DataArray.begin() + Index_end + 1);
    
    return true;
}


template<typename ElementType>
inline bool SimpleDataArray<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputData)
{
    return this->Insert(Index, InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputData)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ SimpleDataArray::Insert(Index, DenseMatrix)")
        return false;
    }

    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Insert(int_max Index, const SimpleDataArray<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool SimpleDataArray<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ SimpleDataArray::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ SimpleDataArray::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    m_DataArray.insert(m_DataArray.begin() + Index, InputData, InputData + InputLength);

    return true;
}


template<typename ElementType>
inline 
bool SimpleDataArray<ElementType>::PushBack(ElementType Element)
{
    m_DataArray.push_back(std::move(Element));
}


template<typename ElementType>
inline
ElementType SimpleDataArray<ElementType>::PopBack()
{
    return m_DataArray.pop_back();
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
SimpleDataArray<int_max> SimpleDataArray<ElementType>::Find(MatchFunctionType MatchFunction)
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
SimpleDataArray<int_max> SimpleDataArray<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
SimpleDataArray<int_max> SimpleDataArray<ElementType>::
Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction)
{
    SimpleDataArray<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ SimpleDataArray::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataArray::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataArray::Find(...)")
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
template<typename CompareFunctionType>
inline
SimpleDataArray<int_max> SimpleDataArray<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength()-1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
SimpleDataArray<int_max> SimpleDataArray<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    SimpleDataArray<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ SimpleDataArray::Sort(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ SimpleDataArray::Sort(...)")
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
void SimpleDataArray<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
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
void SimpleDataArray<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}

}//end namespace mdk

#endif