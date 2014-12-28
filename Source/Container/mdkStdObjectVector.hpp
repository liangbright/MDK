#ifndef __mdkStdObjectVector_hpp
#define __mdkStdObjectVector_hpp


namespace mdk
{
 
template<typename ElementType>
inline
StdObjectVector<ElementType>::StdObjectVector()
{
}


template<typename ElementType>
inline
StdObjectVector<ElementType>::StdObjectVector(const std::initializer_list<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
StdObjectVector<ElementType>::StdObjectVector(const std::vector<ElementType>& InputList)
{
    (*this) = InputList;
}


template<typename ElementType>
inline
StdObjectVector<ElementType>::StdObjectVector(const StdObjectVector<ElementType>& InputArray)
{
    this->Copy(InputArray);
}


// move constructor
template<typename ElementType>
inline
StdObjectVector<ElementType>::StdObjectVector(StdObjectVector<ElementType>&& InputArray) noexcept
{
    m_StdVector = std::move(InputArray.m_StdVector);
}


template<typename ElementType>
inline
StdObjectVector<ElementType>::~StdObjectVector()
{
}


template<typename ElementType>
inline 
std::vector<ElementType>& StdObjectVector<ElementType>::StdVector()
{
    return m_StdVector;
}


template<typename ElementType>
inline
const std::vector<ElementType>& StdObjectVector<ElementType>::StdVector() const
{
    return m_StdVector;
}


template<typename ElementType>
inline
void StdObjectVector<ElementType>::operator=(const StdObjectVector<ElementType>& InputArray)
{
    this->Copy(InputArray);
}


// move assignment operator
template<typename ElementType>
inline
void StdObjectVector<ElementType>::operator=(StdObjectVector<ElementType>&& InputArray)
{
    m_StdVector = std::move(InputArray.m_StdVector);
}


template<typename ElementType>
inline
void StdObjectVector<ElementType>::operator=(const std::initializer_list<ElementType>& InputArray)
{
    m_StdVector = InputArray;
}


template<typename ElementType>
inline
void StdObjectVector<ElementType>::operator=(const std::vector<ElementType>& InputArray)
{
    m_StdVector = InputArray;
}


template<typename ElementType>
inline
void StdObjectVector<ElementType>::operator=(const ObjectArray<ElementType>& InputArray)
{
	this->copy(InputArray);
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Copy(const StdObjectVector<ElementType>& InputArray)
{
    if (this == &InputArray)
    {
        MDK_Warning("A StdObjectVector try to Copy itself @ StdObjectVector::Copy(InputArray)")
        return true;
    }

    // copy data
    return this->Copy(InputArray.GetElementPointer(), InputArray.GetLength());
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Copy(const StdObjectVector<ElementType>* InputArray)
{
    if (InputArray == nullptr)
    {
        MDK_Error("InputArray is nullptr @ StdObjectVector::Copy(StdObjectVector* InputArray)")
        return false;
    }

    return this->Copy(*InputArray);
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Copy(const ElementType* InputElementPointer, int_max InputLength)
{
    if (InputElementPointer == nullptr || InputLength <= 0)
    {
		if (this->IsEmpty() == false)
		{
			this->Clear();
		}
        return true;
    }

    // if this StdObjectVector is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetElementPointer())
        {
           // MDK_Warning("A StdObjectVector tries to Copy itself @ StdObjectVector::Copy(ElementType*, RowNumber, ColNumber)")
            return true;
        }
    }

    //------------------------------------------------------------------

    m_StdVector.resize(InputLength);

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
bool StdObjectVector<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ StdObjectVector::Fill")
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
void StdObjectVector<ElementType>::Clear()
{
    m_StdVector.clear();         // change size
    m_StdVector.shrink_to_fit(); // release memory
}


template<typename ElementType>
inline 
bool StdObjectVector<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ StdObjectVector::Resize(int_max InputLength)")
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

    m_StdVector.resize(InputLength);
}
catch (...)
{
    MDK_Error("Out of Memory @ StdObjectVector::Resize(int_max InputLength)")

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ StdObjectVector::FastResize(int_max InputLength)")
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
        if (InputLength > int_max(m_StdVector.capacity()))
        {
            m_StdVector.clear();
        }

        m_StdVector.resize(InputLength);
    }

}
catch (...)
{
    MDK_Error("Out of Memory @ StdObjectVector::FastResize(int_max InputLength)")

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool StdObjectVector<ElementType>::ReserveCapacity(int_max InputElementNumber)
{
try
{
    auto SelfLength = this->GetElementNumber();

    if (InputElementNumber > SelfLength)
    {
        m_StdVector.reserve(InputElementNumber);
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ StdObjectVector::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void StdObjectVector<ElementType>::ReleaseUnusedCapacity()
{
    m_StdVector.shrink_to_fit();
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::IsEmpty() const
{
    return (m_StdVector.size() <= 0);
}


template<typename ElementType>
inline
int_max StdObjectVector<ElementType>::GetLength() const
{
    return int_max(m_StdVector.size());
}


template<typename ElementType>
inline
int_max StdObjectVector<ElementType>::GetElementNumber() const
{
    return int_max(m_StdVector.size());
}


template<typename ElementType>
inline
ElementType* StdObjectVector<ElementType>::GetElementPointer()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
const ElementType* StdObjectVector<ElementType>::GetElementPointer() const
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
ElementType* StdObjectVector<ElementType>::GetPointer()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
const ElementType* StdObjectVector<ElementType>::GetPointer() const
{
    return m_StdVector.data();
}


template<typename ElementType>
inline 
ElementType* StdObjectVector<ElementType>::begin()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline 
const ElementType* StdObjectVector<ElementType>::begin() const
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
ElementType* StdObjectVector<ElementType>::end()
{
    auto EndPtr = m_StdVector.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_StdVector.size();
    }

    return EndPtr;
}


template<typename ElementType>
inline
const ElementType* StdObjectVector<ElementType>::end() const
{
    auto EndPtr = m_StdVector.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_StdVector.size();
    }

    return EndPtr;
}


//----------- Get/Set StdObjectVector(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& StdObjectVector<ElementType>::operator[](int_max Index)
{
#if defined(MDK_DEBUG_StdObjectVector_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

#endif //MDK_DEBUG_StdObjectVector_Operator_CheckBound

    return m_StdVector[Index];
}


template<typename ElementType>
inline
const ElementType& StdObjectVector<ElementType>::operator[](int_max Index) const
{
#if defined(MDK_DEBUG_StdObjectVector_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

#endif //MDK_DEBUG_StdObjectVector_Operator_CheckBound

    return m_StdVector[Index];
}


template<typename ElementType>
inline
ElementType& StdObjectVector<ElementType>::operator()(int_max Index)
{
#if defined(MDK_DEBUG_StdObjectVector_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

#endif //MDK_DEBUG_StdObjectVector_Operator_CheckBound

    return m_StdVector[Index];
}


template<typename ElementType>
inline
const ElementType& StdObjectVector<ElementType>::operator()(int_max Index) const
{
#if defined(MDK_DEBUG_StdObjectVector_Operator_CheckBound)

    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

#endif //MDK_DEBUG_StdObjectVector_Operator_CheckBound

    return m_StdVector[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& StdObjectVector<ElementType>::at(int_max Index)
{
    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

    return m_StdVector.at(Index);
}


template<typename ElementType>
inline
const ElementType& StdObjectVector<ElementType>::at(int_max Index) const
{
    if (Index >= this->GetElementNumber() || Index < 0)
    {
        MDK_Error("Invalid Input @ StdObjectVector::operator[](i)")
        return m_StdVector[0];
    }

    return m_StdVector.at(Index);
}


template<typename ElementType>
inline
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(int_max Index_start, int_max Index_end)
{
    StdObjectVector<ElementType> Subset;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ StdObjectVector::GetSubSet(...)")
        return Subset;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ StdObjectVector::GetSubSet(...)")
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
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(const std::vector<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.GetElementPointer(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(const StdObjectVector<int_max>& IndexList)
{
    return this->GetSubSet(IndexList.GetElementPointer(), int_max(IndexList.GetElementNumber()));
}


template<typename ElementType>
inline
StdObjectVector<ElementType> StdObjectVector<ElementType>::GetSubSet(const int_max* IndexList, int_max ListLength)
{
    StdObjectVector<ElementType> SubSet;

    if (IndexList == nullptr || ListLength <= 0)
    {
        return SubSet;
    }

    auto ElementNumber = this->GetElementNumber();

    if (ListLength > ElementNumber)
    {
        MDK_Error("Invalid ListLength @ StdObjectVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(ListLength);

    for (int_max k = 0; k < ListLength; ++k)
    {
        auto tempIndex = IndexList[k];

        if (tempIndex < 0 || tempIndex >= ElementNumber)
        {
            MDK_Error("Invalid index @ StdObjectVector::GetSubSet(...)")
                SubSet.Clear();
            return SubSet;
        }

        SubSet[k] = (*this)[tempIndex];
    }

    return SubSet;
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet)
{
    if (IndexList.size() != SubSet.size())
    {
        MDK_Error("IndexList.size() != SubSet.size() @ StdObjectVector::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.begin(), SubSet.begin(), int_max(SubSet.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSet)
{
    if (IndexList.size() != SubSet.size())
    {
        MDK_Error("IndexList.size() != SubSet.size() @ StdObjectVector::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.data(), SubSet.data(), int_max(SubSet.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::SetSubSet(const StdObjectVector<int_max>& IndexList, const StdObjectVector<ElementType>& SubSet)
{
    if (IndexList.GetElementNumber() != SubSet.GetElementNumber())
    {
        MDK_Error("IndexList.size() != SubSet.size() @ StdObjectVector::SetSubSet(...)")
        return false;
    }

    return this->SetSubSet(IndexList.GetElementPointer(), SubSet.GetElementPointer(), int_max(SubSet.GetElementNumber()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max DataNumber)
{
	if (IndexList == nullptr || SubSet == nullptr || DataNumber <= 0)
    {
        MDK_Warning("Empty input @ StdObjectVector::SetSubSet(...)")
        return true;
    }

    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error("Self is empty @ StdObjectVector::SetSubSet(...)")
        return false;
    }

    if (DataNumber > ElementNumber)
    {
        MDK_Error("Invalid DataNumber @ StdObjectVector::SetSubSet(...)")
        return false;
    }

    for (int_max k = 0; k < DataNumber; ++k)
    {
        auto Index = IndexList[k];

        if (Index < 0 || Index >= ElementNumber)
        {
            MDK_Error("Invalid Index @ StdObjectVector::SetSubSet(...)")
            return false;
        }

		(*this)[Index] = SubSet[k];
    }

    return true;
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Append(ElementType Element)
{
    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    (*this)[SelfLength] = std::move(Element);

    return true;
}


template<typename ElementType>
inline 
bool StdObjectVector<ElementType>::Append(const ElementType* InputArray, int_max InputLength)
{
    if (InputArray == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ StdObjectVector::Append(const ElementType* InputArray, int_max InputLength)")
        return false;
    }

    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_StdVector[i] = InputArray[i - SelfLength];
    }

    return true;
}


template<typename ElementType>
inline 
bool StdObjectVector<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ StdObjectVector::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Delete(const StdObjectVector<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ StdObjectVector::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Error("Empty Input @ StdObjectVector::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ StdObjectVector::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

    if (ListLength == 1)
    {
        m_StdVector.erase(m_StdVector.begin() + IndexList[0], m_StdVector.begin() + IndexList[0] + 1);
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
                MDK_Warning("duplicate Input @ StdObjectVector::Delete(const int_max* IndexPtr, int_max ListLength)")
            }
            else
            {
                m_StdVector.erase(m_StdVector.begin() + Index_i, m_StdVector.begin() + Index_i + 1);

                Index_prev = Index_i;
            }
        }
    }

    return true;
}


template<typename ElementType>
inline 
bool StdObjectVector<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ StdObjectVector::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ StdObjectVector::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_StdVector.erase(m_StdVector.begin() + Index_start, m_StdVector.begin() + Index_end + 1);
    
    return true;
}


template<typename ElementType>
inline bool StdObjectVector<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.begin(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.data(), int_max(InputArray.size()));
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputArray)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ StdObjectVector::Insert(Index, DenseMatrix)")
        return false;
    }

    return this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementNumber());
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Insert(int_max Index, const StdObjectVector<ElementType>& InputArray)
{
    return this->Insert(Index, InputArray.GetElementPointer(), InputArray.GetElementNumber());
}


template<typename ElementType>
inline
bool StdObjectVector<ElementType>::Insert(int_max Index, const ElementType* InputArray, int_max InputLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ StdObjectVector::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputArray == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ StdObjectVector::Insert(Index, const ElementType* InputArray, int_max InputLength)")
            return false;
        }
    }

    m_StdVector.insert(m_StdVector.begin() + Index, InputArray, InputArray + InputLength);

    return true;
}


template<typename ElementType>
inline 
bool StdObjectVector<ElementType>::PushBack(ElementType Element)
{
    m_StdVector.push_back(std::move(Element));
}


template<typename ElementType>
inline
ElementType StdObjectVector<ElementType>::PopBack()
{
    return m_StdVector.pop_back();
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
StdObjectVector<int_max> StdObjectVector<ElementType>::Find(MatchFunctionType MatchFunction)
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
StdObjectVector<int_max> StdObjectVector<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
StdObjectVector<int_max> StdObjectVector<ElementType>::
Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction)
{
    StdObjectVector<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ StdObjectVector::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ StdObjectVector::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ StdObjectVector::Find(...)")
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
StdObjectVector<int_max> StdObjectVector<ElementType>::ExactMatch(const ElementType& InputElement) const
{
	return this->Find([&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
inline
int_max StdObjectVector<ElementType>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
{
	return this->Find(first_or_last, [&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
StdObjectVector<int_max> StdObjectVector<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength()-1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
StdObjectVector<int_max> StdObjectVector<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    StdObjectVector<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ StdObjectVector::Sort(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ StdObjectVector::Sort(...)")
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
void StdObjectVector<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
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
void StdObjectVector<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}

}//end namespace mdk

#endif