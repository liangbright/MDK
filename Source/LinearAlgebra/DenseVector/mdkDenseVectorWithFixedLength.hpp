#pragma once

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

/*
template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const ElementType& Element)
{
	if (Length <= 0)
	{
		MDK_Error("Length <= 0 @ DenseVector::DenseVector(Element)")
	}
	else if (Length > 1)
	{
		//MDK_Warning("Length > 1 @ DenseVector::DenseVector(Element) : fixed_length")
	}

	this->Fill(Element);
}
*/

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const std::initializer_list<ElementType>& InputVector)
{
    if (int_max(InputVector.size()) != Length)
    {
        MDK_Error("InputVector.size() != Length @ DenseVector::DenseVector(std::initializer_list)")
        return;
    }

    for (int_max k = 0; k < Length; ++k)
    {
        m_StdArray[k] = InputVector.begin()[k];
    }
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const std::vector<ElementType>& InputVector)
{
	if (int_max(InputVector.size()) != Length)
	{
		MDK_Error("InputVector.size() != Length @ DenseVector::DenseVector(std::vector)")
		return;
	}

	for (int_max k = 0; k < Length; ++k)
	{
		m_StdArray[k] = InputVector[k];
	}
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const StdObjectVector<ElementType>& InputVector)
{
	(*this) = InputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const ObjectArray<ElementType>& InputVector)
{
	(*this) = InputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const DenseMatrix<ElementType>& InputVector)
{
	(*this) = InputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(const DenseVector<ElementType, Length>& InputVector)
{
    m_StdArray = InputVector.m_StdArray;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::DenseVector(DenseVector<ElementType, Length>&& InputVector)
{
    m_StdArray = std::move(InputVector.m_StdArray);
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>::~DenseVector()
{
}


template<typename ElementType, int_max Length>
inline 
bool DenseVector<ElementType, Length>::operator==(const DenseVector<ElementType, Length>& InputVector) const
{
	return !(*this != InputVector);
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::operator!=(const DenseVector<ElementType, Length>& InputVector) const
{
	for (int_max k = 0; k < Length; ++k)
	{
		if (m_StdArray[k] != InputVector[k])
		{
			return true;
		}
	}
	return false;
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
    m_StdArray = std::move(InputVector.m_StdArray);
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

	for (int_max k = 0; k < Length; ++k)
	{
		m_StdArray[k] = InputVector.begin()[k];
	}
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

	for (int_max k = 0; k < Length; ++k)
	{
		m_StdArray[k] = InputVector[k];
	}
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const StdObjectVector<ElementType>& InputVector)
{
	if (InputVector.GetLength() != Length)
	{
		MDK_Error("InputVector.GetLength() != Length @ DenseVector::operator=(...)")
		return;
	}

	for (int_max k = 0; k < Length; ++k)
	{
		m_StdArray[k] = InputVector[k];
	}
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator=(const ObjectArray<ElementType>& InputVector)
{
	if (InputVector.GetLength() != Length)
	{
		MDK_Error("InputVector.GetLength() != Length @ DenseVector::operator=(...)")
		return;
	}

	for (int_max k = 0; k < Length; ++k)
	{
		m_StdArray[k] = InputVector[k];
	}
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
    if (InputVector.GetElementCount() != Length)
    {
        MDK_Error("InputVector.GetElementCount() != Length @ DenseVector::operator=(...)")
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
        m_StdArray[k] = ElementType(InputVector[k]);
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
        m_StdArray[i] = Element;
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
int_max DenseVector<ElementType, Length>::GetElementCount() const
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
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::GetElementPointer() const
{
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::GetPointer()
{
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::GetPointer() const
{
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::begin()
{
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::begin() const
{
    return m_StdArray.data();
}


template<typename ElementType, int_max Length>
inline
ElementType* DenseVector<ElementType, Length>::end()
{
    auto EndPtr = m_StdArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_StdArray.size();
    }

    return EndPtr;
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVector<ElementType, Length>::end() const
{
    auto EndPtr = m_StdArray.data();

    if (EndPtr != nullptr)
    {
        EndPtr += m_StdArray.size();
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
        return m_StdArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index) const")
        return m_StdArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVector<ElementType, Length>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index)")
        return m_StdArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index) const")
        return m_StdArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVector<ElementType, Length>::at(int_max Index)
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_StdArray[0];
    }

    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVector<ElementType, Length>::at(int_max Index) const
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_StdArray[0];
    }

    return m_StdArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
std::array<ElementType, Length>& DenseVector<ElementType, Length>::StdArray()
{
	return m_StdArray;
}


template<typename ElementType, int_max Length>
inline
const std::array<ElementType, Length>& DenseVector<ElementType, Length>::StdArray() const
{
	return m_StdArray;
}


template<typename ElementType, int_max Length>
std::vector<ElementType> DenseVector<ElementType, Length>::ConvertToStdVector() const
{
	std::vector<ElementType> StdVector;
	StdVector.resize(m_StdArray.size());
	for (int_max k = 0; k < int_max(m_StdArray.size()); ++k)
	{
		StdVector[k] = m_StdArray[k]
	}
	return StdVector;
}


template<typename ElementType, int_max Length>
DenseVector<ElementType> DenseVector<ElementType, Length>::ConvertToDenseVectorWithVariableLength() const
{
	DenseVector<ElementType> OutputVector;
	StdVector.Resize(int_max(m_StdArray.size()));
	for (int_max k = 0; k < int_max(m_StdArray.size()); ++k)
	{
		OutputVector[k] = m_StdArray[k]
	}
	return OutputVector;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVector<ElementType, Length>::ConvertToDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;
    this->ConvertToDenseMatrixAsRowVector(OutputVector);
    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVector<ElementType, Length>::ConvertToDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(1, Length);
    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_StdArray[i];
    }
}


template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVector<ElementType, Length>::ConvertToDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;
    this->ConvertToDenseMatrixAsColVector(OutputVector);
    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVector<ElementType, Length>::ConvertToDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(Length, 1);
    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_StdArray[i];
    }
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(int_max Index_start, int_max Index_end) const
{
    DenseVector<ElementType> SubSet;
    
    auto Length = this->GetLength();

    if (Index_start < 0 || Index_start >= Length || Index_end < 0 || Index_end >= Length)
    {
        MDK_Error("Invalid input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (Length == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

	if (Index_start <= Index_end)
	{
		SubSet.FastResize(Index_end - Index_start + 1);
		for (int_max Index = Index_start; Index <= Index_end; ++Index)
		{
			SubSet[Index - Index_start] = m_StdArray[Index];
		}
	}
	else// Index_start > Index_end
	{
		SubSet.FastResize(Index_start - Index_end + 1);
		for (int_max Index = Index_start; Index >= Index_end; --Index)
		{
			SubSet[Index_start - Index] = m_StdArray[Index];
		}
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
        SubSet[i] = m_StdArray[Index];
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
        SubSet[i] = m_StdArray[Index];
    }

    return SubSet;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType> DenseVector<ElementType, Length>::GetSubSet(const DenseMatrix<int_max>& IndexList) const
{
    DenseVector<ElementType> SubSet;
    
    auto SelfLength = this->GetLength();
    auto InputLength = IndexList.GetElementCount();
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
        SubSet[i] = m_StdArray[Index];
    }
    
    return SubSet;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubSet)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubSet.size()))
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(Index_start, Index_end, SubSet.begin());
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(int_max Index_start, int_max Index_end, const DenseMatrix<ElementType>& SubSet)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubSet.GetElementCount()))
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(Index_start, Index_end, SubSet.GetPointer());
}


template<typename ElementType, int_max Length>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType, Length>::SetSubSet(int_max Index_start, int_max Index_end, const DenseVector<ElementType, LengthParameter>& SubSet)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubSet.GetElementCount()))
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(Index_start, Index_end, SubSet.GetPointer());
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(int_max Index_start, int_max Index_end, const ElementType* SubSet)
{
	auto SelfLength = this->GetLength();
	if (Index_start < 0 || Index_start >= SelfLength || Index_end < 0 || Index_end >= SelfLength || SubSet == nullptr)
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}

	if (SelfLength == 0)
	{
		MDK_Error("Self is empty @ DenseVector::SetSubSet(...)")
		return false;
	}

	if (Index_start <= Index_end)
	{
		for (int_max Index = Index_start, Index <= Index_end; ++Index)
		{
			m_StdArray[Index] = SubSet[Index - Index_start];
		}
	}
	else//Index_end < Index_start
	{
		for (int_max Index = Index_end, Index <= Index_start; ++Index)
		{
			m_StdArray[Index] = SubSet[Index - Index_end];
		}
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
	return this->SetSubSet(IndexList.begin(), SubSet.begin(), int_max(SubSet.size()));
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubSet)
{
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(IndexList.GetPointer(), SubSet.GetPointer(), SubSet.GetElementCount());
}


template<typename ElementType, int_max Length>
template<int_max LengthParameterA, int_max LengthParameterB>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseVector<int_max, LengthParameterA>& IndexList, const DenseVector<ElementType, LengthParameterB>& SubSet)
{// do not use LengthParameter
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(IndexList.GetPointer(), SubSet.GetPointer(), SubSet.GetLength());
}


template<typename ElementType, int_max Length>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const DenseVector<int_max, LengthParameter>& IndexList, const DenseMatrix<ElementType>& SubSet)
{// do not use LengthParameter
	if (IndexList.GetElementCount() != SubSet.GetElementCount())
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(IndexList.GetPointer(), SubSet.GetPointer(), SubSet.GetElementCount());
}


template<typename ElementType, int_max Length>
inline
bool DenseVector<ElementType, Length>::SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max SubSetLength)
{
	if (IndexList == nullptr || SubSet == nullptr || SubSetLength <= 0)
	{
		return true;
	}

	auto SelfLength = this->GetLength();
	for (int_max i = 0; i < SubSetLength; ++i)
	{
		auto Index = IndexList[i];
		if (Index < 0 || Index >= SelfLength)
		{
			MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")
			return false;
		}
		m_StdArray[Index] = SubSet[i];
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
DenseVector<int_max> DenseVector<ElementType, Length>::Find(int_max MaxOutputCount, MatchFunctionType MatchFunction) const
{
    return this->Find(MaxOutputCount, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType, int_max Length>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::
Find(int_max MaxOutputCount, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const
{
    DenseVector<int_max> IndexList;

    if (MaxOutputCount == 0)
    {
        return IndexList;
    }

    auto ElementCount = this->GetElementCount();

    if (MaxOutputCount < 0 || MaxOutputCount > ElementCount)
    {
        MDK_Error("MaxOutputCount is invalid @ DenseVector::Find(...)")
        return IndexList;
    }

    if (Index_start < 0 || Index_start >= ElementCount || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DenseVector::Find(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementCount)
    {
        MDK_Error("Index_end is invalid @ DenseVector::Find(...)")
        return IndexList;
    }

    if (ElementCount == 0)
    {
        return IndexList;
    }

    IndexList.ReserveCapacity(MaxOutputCount);

	if (Index_start < Index_end)
	{
		for (int_max i = Index_start; i <= Index_end; ++i)
		{
			if (MatchFunction((*this)[i]) == true)
			{
				IndexList.Append(i);
				if (IndexList.GetElementCount() == MaxOutputCount)
				{
					break;
				}
			}
		}
	}
	else //if (Index_start > Index_end)
	{
		for (int_max i = Index_start; i >= Index_end; --i)
		{
			if (MatchFunction((*this)[i]) == true)
			{
				IndexList.Append(i);
				if (IndexList.GetElementCount() == MaxOutputCount)
				{
					break;
				}
			}
		}
	}

    return IndexList;
}


template<typename ElementType, int_max Length>
template<typename MatchFunctionType>
inline
int_max DenseVector<ElementType, Length>::Find(const std::string& first_or_last, MatchFunctionType MatchFunction) const
{
	int_max Index_output = -1;

	if (first_or_last == "first")
	{
		for (int_max i = 0; i < this->GetElementCount(); ++i)
		{
			if (MatchFunction((*this)[i]) == true)
			{
				Index_output = i;
				break;
			}
		}
	}
	else if (first_or_last == "last")
	{
		for (int_max i = this->GetElementCount() - 1; i >= 0; --i)
		{
			if (MatchFunction((*this)[i]) == true)
			{
				Index_output = i;
				break;
			}
		}
	}
	else
	{
		MDK_Error("Wrong option @  DenseVector::Find(...)")
	}

	return Index_output;
}


template<typename ElementType, int_max Length>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::ExactMatch(const ElementType& InputElement) const
{
	return this->Find([&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType, int_max Length>
inline
int_max DenseVector<ElementType, Length>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
{
	return this->Find(first_or_last, [&](const ElementType& Element){return Element == InputElement; });
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

    auto ElementCount = this->GetElementCount();

    if (Index_start < 0 || Index_start >= ElementCount || Index_start > Index_end)
    {
        MDK_Error("Index_start is invalid @ DenseVector::Sort(...)")
        return IndexList;
    }

    if (Index_end < 0 || Index_end >= ElementCount)
    {
        MDK_Error("Index_end is invalid @ DenseVector::Sort(...)")
        return IndexList;
    }

    if (ElementCount == 0)
    {
        return IndexList;
    }

    if (Index_start == Index_end)
    {
        IndexList.Append(Index_start);
        return IndexList;
    }

    IndexList.Resize(ElementCount);

    for (int_max i = Index_start; i <= Index_end; ++i)
    {
        IndexList[i] = i;
    }

    std::sort(IndexList.begin(), IndexList.end(), [&](int_max a, int_max b) { return CompareFunction((*this)[a], (*this)[b]); });

    return IndexList;
}


template<typename ElementType, int_max Length>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Sort(const char* ascend_or_descend) const
{
	std::string Order_str(ascend_or_descend);
    return this->Sort(Order_str);
}


template<typename ElementType, int_max Length>
inline
DenseVector<int_max> DenseVector<ElementType, Length>::Sort(const std::string& ascend_or_descend) const
{
    // Order: ascend or descend

	if (ascend_or_descend == "ascend")
    {
        return this->Sort(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA < ElementB; });
    }
	else if (ascend_or_descend == "descend")
    {
        return this->Sort(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA > ElementB; });
    }
    else
    {
        MDK_Error("invalid input string @ DenseVector::Sort(Order)")
        DenseVector<int_max> EmptyIndexList;
        return EmptyIndexList;
    }
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


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::SortInPlace(const char* ascend_or_descend)
{
	std::string Order_str(ascend_or_descend);
    this->SortInPlace(Order_str);
}


template<typename ElementType, int_max Length>
inline 
void DenseVector<ElementType, Length>::SortInPlace(const std::string& ascend_or_descend)
{
    // Order: ascend or descend

	if (ascend_or_descend == "ascend")
    {
        this->SortInPlace(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA < ElementB; });
    }
	else if (ascend_or_descend == "descend")
    {
        this->SortInPlace(0, this->GetLength() - 1, [](const ElementType& ElementA, const ElementType& ElementB) { return ElementA > ElementB; });
    }
    else
    {
        MDK_Error("invalid input string @ DenseVector::SortInPlace(Order)")
    }
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
        m_StdArray[i] += InputVector[i];
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
            m_StdArray[i] += InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator+=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] += InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator-=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] -= InputVector[i];
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
            m_StdArray[i] -= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator-=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] -= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator*=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] *= InputVector[i];
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
            m_StdArray[i] *= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator*=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] *= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator/=(const DenseVector<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] /= InputVector[i];
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
            m_StdArray[i] /= InputVector[0];
        }
    }

    if (Length != InputLength)
    {
        MDK_Error("Invalid input @ DenseVector::operator/=(InputVector)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] += Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] -= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] *= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVector<ElementType, Length>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_StdArray[i] /= Element;
    }
}

//--------------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Sum() const
{
    auto Value = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Value += m_StdArray[i];
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
        auto temp = m_StdArray[i] - MeanValue;

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

    auto Index = this->IndexOfMax();
    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
int_max DenseVector<ElementType, Length>::IndexOfMax() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::IndexOfMax()")
        return 0;
    }

    auto Index = int_max(0);
    auto Value = m_StdArray[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        if (m_StdArray[i] > Value)
        {
            Value = m_StdArray[i];
            Index = i;
        }
    }

    return Index;
}


template<typename ElementType, int_max Length>
inline
ElementType DenseVector<ElementType, Length>::Min() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::Min()")
        return 0;
    }

    auto Index = this->IndexOfMin();
    return m_StdArray[Index];
}


template<typename ElementType, int_max Length>
inline
int_max DenseVector<ElementType, Length>::IndexOfMin() const
{
    if (Length <= 0)
    {
        MDK_Error("Self is empty @ DenseVector::IndexOfMin()")
        return 0;
    }

    auto Index = int_max(0);
    auto Value = m_StdArray[0];

    for (int_max i = 1; i < Length; ++i)
    {
        if (m_StdArray[i] < Value)
        {
            Value = m_StdArray[i];
            Index = i;
        }
    }

    return Index;
}


template<typename ElementType, int_max Length>
int_max DenseVector<ElementType, Length>::L0Norm(ElementType Zero) const
{
	if (this->IsEmpty() == true)
	{
		MDK_Warning("Self is empty @ DenseVector::L0Norm()")
		return 0;
	}

	int_max Count = 0;
	for (int_max i = 0; i < this->GetLength(); ++i)
	{
		if (m_StdVector[i] > Zero)
		{
			Count += 1;
		}
	}
	return Count;
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
        Value += std::abs(m_StdArray[i]);
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

    auto Value = m_StdArray[0] * m_StdArray[0];

    for (int_max i = 1; i < Length; ++i)
    {
        Value += m_StdArray[i] * m_StdArray[i];
    }

    Value = std::sqrt(Value);

    return Value;
}


}// namespace mdk

