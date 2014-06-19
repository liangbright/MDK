﻿#ifndef __mdkDenseVectorWithFixedLength_hpp
#define __mdkDenseVectorWithFixedLength_hpp


namespace mdk
{

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector()
{
    if (Length <= 0)
    {
        MDK_Error("Length <= 0 @ DenseVector::DenseVector()")
    }  
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const std::initializer_list<ElementType>& InputVector)
{
    if (int_max(InputVector.size()) != Length)
    {
        MDK_Error("InputVector.size() != Length @ DenseVector::DenseVector(std::initializer_list)")
        return;
    }

    m_DataArray = InputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const DenseVector<ElementType, Length>& InputVector)
{
    m_DataArray = InputVector.m_DataArray;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(DenseVector<ElementType, Length>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::~DenseVector()
{
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const DenseVector<ElementType, Length>& InputVector)
{
    this->Copy(InputVector);
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(DenseVector<ElementType, Length>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const std::initializer_list<ElementType>& InputVector)
{
    if (int_max(InputVector.size()) != Length)
    {
        MDK_Error("InputVector.size() != Length @ DenseVector::operator=(...)")
        return;
    }

    m_DataArray = InputVector;
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const std::vector<ElementType>& InputVector)
{
    if (int_max(InputVector.size()) != Length)
    {
        MDK_Error("InputVector.size() != Length @ DenseVector::operator=(...)")
        return;
    }

    this->Copy(InputVector.data());
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const DenseVector<ElementType>& InputVector)
{
    if (InputVector.GetLength() != Length)
    {
        MDK_Error("InputVector.GetLength() != Length @ DenseVector::operator=(...)")
        return;
    }

    this->Copy(InputVector.GetElementPointer());
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const DenseMatrix<ElementType>& InputVector)
{
    if (InputVector.GetElementNumber() != Length)
    {
        MDK_Error("InputVector.GetElementNumber() != Length @ DenseVector::operator=(...)")
        return;
    }

    this->Copy(InputVector.GetElementPointer());
}


template<typename ElementType, int_max Length>
template<typename ElementType_input>
inline void DenseVector<ElementType, Length>::Copy(const DenseVector<ElementType_input, Length>& InputVector)
{
    this->Copy(InputVector.GetElementPointer());
}


template<typename ElementType, int_max Length>
template<typename ElementType_input>
inline void DenseVector<ElementType, Length>::Copy(const ElementType_input* InputVector)
{
    if (size_t(this->GetElementPointer()) == size_t(InputVector))
    {
        return;
    }

    for (int_max k = 0; k < Length; ++k)
    {
        m_DataArray[k] = ElementType(InputVector[k]);
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::Clear()
{
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::Fill(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType, int_max Length>
inline
int_max DenseVector<ElementType, Length>::GetLength() const
{
    return Length;
}


template<typename ElementType, int_max Length>
inline
int_max DenseVector<ElementType, Length>::GetElementNumber() const
{
    return Length;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::IsEmpty() const
{
    return (Length <= 0);
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::IsLengthFixed() const
{
    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::IsSizeFixed() const
{
    return true;
}


//-----------element access------------------//

template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::GetElementPointer()
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::GetElementPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::GetPointer()
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::GetPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::begin()
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::begin() const
{
    return m_DataArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::end()
{
    auto EndPtr = m_DataArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_DataArray.size();
    }

    return EndPtr;
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::end() const
{
    auto EndPtr = m_DataArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_DataArray.size();
    }

    return EndPtr;
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVector<ElementType, Length>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVector<ElementType, Length>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVector<ElementType, Length>::at(int_max Index)
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::at(int_max Index) const
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
std::array<ElementType, Length> DenseVector<ElementType, Length>::CreateStdArray() const
{
    return m_DataArray;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVector<ElementType, Length>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVector<ElementType, Length>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVector<ElementType, Length>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVector<ElementType, Length>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(Length, 1);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(int_max Index_start, int_max Index_end) const
{
    DenseVector<ElementType> SubSet;
    
    auto Length = this->GetLength();

    if (Index_start < 0 || Index_start >= Length || Index_end < 0 || Index_end >= Length || Index_start > Index_end)
    {
        MDK_Error("Invalid input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (Length == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(Index_end - Index_start + 1);

    for (int_max i = Index_start; i <= Index_end; ++i)
    {
        SubSet[i - Index_start] = m_DataArray[i];
    }
    
    return SubSet;
}


template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(const std::initializer_list<int_max>& IndexList) const
{
    DenseVector<ElementType> SubSet;

    auto SelfLength = this->GetLength();

    auto InputLength = int_max(IndexList.size());

    if (InputLength > SelfLength)
    {
        MDK_Error("InputLength > SelfLength @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(InputLength);

    for (int_max i = 0; i < InputLength; ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[i] = m_DataArray[IndexList.begin()[i]];
    }

    return SubSet;
}


template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(const std::vector<int_max>& IndexList) const
{
    DenseVector<ElementType> SubSet;

    auto SelfLength = this->GetLength();

    auto InputLength = int_max(IndexList.size());

    if (InputLength > SelfLength)
    {
        MDK_Error("InputLength > SelfLength @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(InputLength);

    for (int_max i = 0; i < InputLength; ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[i] = m_DataArray[IndexList[i]];
    }

    return SubSet;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(const DenseMatrix<int_max>& IndexList) const
{
    DenseVector<ElementType> SubSet;
    
    auto SelfLength = this->GetLength();

    auto InputLength = IndexList.GetElementNumber();

    if (InputLength > SelfLength)
    {
        MDK_Error("InputLength > SelfLength @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(InputLength);

    for (int_max i = 0; i < InputLength; ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[i] = m_DataArray[IndexList[i]];
    }
    
    return SubSet;
}


template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(const SimpleDataArray<int_max>& IndexList) const
{
    DenseVector<ElementType> SubSet;
    
    auto SelfLength = this->GetLength();

    auto InputLength = IndexList.GetElementNumber();

    if (InputLength > SelfLength)
    {
        MDK_Error("InputLength > SelfLength @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(InputLength);

    for (int_max i = 0; i < InputLength; ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[i] = m_DataArray[IndexList[i]];
    }
    
    return SubSet;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubSet)
{
    if (Index_end - Index_start + 1 != int_max(SubSet.size()))
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    if (Index_start < 0 || Index_start >= SelfLength || Index_end < 0 || Index_end >= SelfLength || Index_start > Index_end)
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DenseVector::SetSubSet(...)")
        return false;
    }

    for (int_max Index = Index_start, Index <= Index_end; ++Index)
    {
        m_DataArray[Index] = SubSet[Index - Index_start];
    }

    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet)
{
    if (IndexList.size() != SubSet.size())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < int_max(IndexList.size()); ++i)
    {
        auto Index = IndexList.begin()[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet.begin()[i];
    }

    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSet)
{
    if (IndexList.size() != SubSet.size())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < int_max(IndexList.size()); ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet[i];
    }

    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubSet)
{
    if (IndexList.GetElementNumber() != SubSet.GetElementNumber())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < IndexList.GetElementNumber(); ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet[i];
    }

    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const SimpleDataArray<int_max>& IndexList, const SimpleDataArray<ElementType>& SubSet)
{
    if (IndexList.GetElementNumber() != SubSet.GetElementNumber())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < IndexList.GetElementNumber(); ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet[i];
    }

    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<int_max>& SubSet)
{
    if (IndexList.GetElementNumber() != SubSet.GetElementNumber())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < IndexList.GetElementNumber(); ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet[i];
    }

    return true;
}


template<typename ElementType, int_max Length>
template<int_max InputLength>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseVector<int_max, InputLength>& IndexList, const DenseVector<int_max, InputLength>& SubSet)
{
    if (IndexList.GetElementNumber() != SubSet.GetElementNumber())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

    auto SelfLength = this->GetLength();

    for (int_max i = 0; i < IndexList.GetElementNumber(); ++i)
    {
        auto Index = IndexList[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

            return false;
        }

        m_DataArray[Index] = SubSet[i];
    }

    return true;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Find(MatchFunctionType MatchFunction) const
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType, int_max Length>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType, int_max Length>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::
Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const
{
    DenseVector<int_max> IndexList;

    if (MaxOutputNumber == 0)
    {
        return IndexList;
    }

    auto ElementNumber = this->GetElementNumber();

    if (MaxOutputNumber < 0 || MaxOutputNumber > ElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ DenseVector::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DenseVector::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ DenseVector::Find(...)")
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


template<typename ElementType, int_max Length>
template<typename CompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength() - 1, CompareFunction);
}


template<typename ElementType, int_max Length>
template<typename CompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    DenseVector<int_max> IndexList;

    auto ElementNumber = this->GetElementNumber();

    if (Index_start < 0 || Index_start >= ElementNumber || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DenseVector::Sort(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementNumber)
    {
        MDK_Error("Index_end is invalid @ DenseVector::Sort(...)")
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


template<typename ElementType, int_max Length>
template<typename CompareFunctionType>
inline
void DenseVector<ElementType, Length>::SortInPlace(CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin(), this->end(), CompareFunction);
}


template<typename ElementType, int_max Length>
template<typename CompareFunctionType>
inline
void DenseVector<ElementType, Length>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::FindUnique() const
{
    return FindUniqueElementInVector(*this);  
}


template<typename ElementType, int_max Length>
template<typename SpecialCompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const
{
    return FindUniqueElementInVector(*this, SpecialCompareFunction);
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator+=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator+=(const DenseVector<ElementType>& InputVector)
{
    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        for (int_max i = 0; i < Length; ++i)
        {
            m_DataArray[i] += InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator+=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator-=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator-=(const DenseVector<ElementType>& InputVector)
{
    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        for (int_max i = 0; i < Length; ++i)
        {
            m_DataArray[i] -= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator-=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator*=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator*=(const DenseVector<ElementType>& InputVector)
{
    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        for (int_max i = 0; i < Length; ++i)
        {
            m_DataArray[i] *= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator*=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator/=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator/=(const DenseVector<ElementType>& InputVector)
{
    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        for (int_max i = 0; i < Length; ++i)
        {
            m_DataArray[i] /= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator/=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= Element;
    }
}

//--------------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Sum() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::Sum()")
        return 0;
    }

    auto Value = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Value += m_DataArray[i];
    }

    return Value;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Mean() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::Mean()")
        return 0;
    }

    auto Value = this->Sum();

    Value /= ElementType(Length);

    return Value;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Std() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::Std()")
        return 0;
    }

    auto MeanValue = this->Mean();

    auto Value = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        auto temp = m_DataArray[i] - MeanValue;

        Value += temp*temp;
    }

    Value /= ElementType(this->GetLength());

    Value = std::sqrt(Value);

    return Value;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Max() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::Max()")
        return 0;
    }

    auto Value = m_DataArray[0];

    for (int_max i = 1; i < Length; ++i)
    {
        Value = std::max(Value, m_DataArray[i]);
    }

    return Value;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Min() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::Min()")
        return 0;
    }

    auto Value = m_DataArray[0];

    for (int_max i = 1; i < Length; ++i)
    {
        Value = std::min(Value, m_DataArray[i]);
    }

    return Value;
}


template<typename ElementType, int_max Length>
ElementType DenseVector<ElementType, Length>::L1Norm() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::L1Norm()")
        return 0;
    }

    auto Value = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Value += std::abs(m_DataArray[i]);
    }

    return Value;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::L2Norm() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::L2Norm()")
        return 0;
    }

    auto Value = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Value += m_DataArray[i] * m_DataArray[i];
    }

    Value = std::sqrt(Value);

    return Value;
}


}// namespace mdk


#endif