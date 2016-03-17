#ifndef mdk_DenseVectorWithVariableLength_hpp
#define mdk_DenseVectorWithVariableLength_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector()
{
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const ElementType& Element)
{
    m_StdVector.resize(1);
    m_StdVector[0] = Element;
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const std::initializer_list<ElementType>& InputVector)
{
    m_StdVector = InputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(std::vector<ElementType> InputVector)
{
	m_StdVector = std::move(InputVector);
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(StdObjectVector<ElementType> InputVector)
{
	m_StdVector = std::move(InputVector.StdVector());
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const ObjectArray<ElementType>& InputVector)
{
	(*this) = InputVector;
}


template<typename ElementType>
inline 
DenseVector<ElementType>::DenseVector(const DenseMatrix<ElementType>& InputVector)
{
	(*this) = InputVector;
}


template<typename ElementType>
template<int_max LengthParameter>
inline
DenseVector<ElementType>::DenseVector(const DenseVector<ElementType, LengthParameter>& InputVector)
{
	auto InputLength = InputVector.GetLength();
    if (InputLength > 0)
    {
        m_StdVector.resize(InputLength);

        for (int_max i = 0; i < InputLength; ++i)
        {
            m_StdVector[i] = InputVector[i];
        }
    }
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const DenseVector<ElementType>& InputVector)
{
	auto InputLength = InputVector.GetLength();
	if (InputLength > 0)
	{
		m_StdVector.resize(InputLength);

		for (int_max i = 0; i < InputLength; ++i)
		{
			m_StdVector[i] = InputVector[i];
		}
	}
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(DenseVector<ElementType>&& InputVector)
{
    m_StdVector = std::move(InputVector.m_StdVector);
}


template<typename ElementType>
inline DenseVector<ElementType>::DenseVector(const std::initializer_list<const DenseVector<ElementType>*>& InputList)
{
	(*this) = InputList;
}


template<typename ElementType>
inline DenseVector<ElementType>::DenseVector(const std::initializer_list<DenseVector<ElementType>>& InputList)
{
	(*this) = InputList;
}


template<typename ElementType>
inline
DenseVector<ElementType>::~DenseVector()
{
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const ElementType& Element)
{
	auto SelfLength = this->GetLength();
	if (SelfLength != 1)
	{
		if (SelfLength > 1)
		{
			MDK_Warning("Vector = Element, and Vector Length > 1 @ DenseVector operator=(const ElementType& Element)")
		}
		this->Clear();
		this->Resize(1);
	}
	(*this)[0] = Element;
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const DenseVector<ElementType>& InputVector)
{
    this->Copy(InputVector);
}


template<typename ElementType>
template<int_max LengthParameter>
inline 
void DenseVector<ElementType>::operator=(const DenseVector<ElementType, LengthParameter>& InputVector)
{
    this->Copy(InputVector.GetElementPointer(), InputVector.GetLength());
}


template<typename ElementType>
inline void DenseVector<ElementType>::operator=(DenseVector<ElementType>&& InputVector)
{
    m_StdVector = std::move(InputVector.m_StdVector);
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const std::initializer_list<const DenseVector<ElementType>*>& InputList)
{
    auto InputVectorNumber = int_max(InputList.size());

    if (InputVectorNumber <= 0)
    {
		this->Clear();
        return;
    }

    bool IsSelfInInputList = false;

    int_max TotalElementNumber = 0;

    for (int_max k = 0; k < InputVectorNumber; k++)
    {
        auto InputVectorPtr = InputList.begin()[k];

        TotalElementNumber += InputVectorPtr->GetElementCount();

		if (this->GetElementPointer() != nullptr && this->GetElementPointer() == InputVectorPtr->GetElementPointer())
        {
            IsSelfInInputList = true;
        }
    }

    if (TotalElementNumber <= 0)
    {
		this->Clear();
        return;
    }

    auto SelfLength = this->GetLength();

    if (IsSelfInInputList == false)
    {
		this->Clear();
		this->SetCapacity(TotalElementNumber);

        for (int_max k = 0; k < InputVectorNumber; k++)
        {            
            auto InputVectorPtr = InputList.begin()[k];

            auto DataPtr = InputVectorPtr->GetElementPointer();
            if (DataPtr != nullptr)
            {
                this->Append(DataPtr, InputVectorPtr->GetElementCount());
            }
        }
    }
    else // Self is in InputList 
    {
        if (TotalElementNumber == SelfLength)
        {
            //MDK_Warning("Self = {&Self} @  @ DenseMatrix::operator=(initializer_list)")
            return;
        }
        else
        {
            DenseVector<ElementType> tempVector;            
            tempVector = InputList;
            m_StdVector = std::move(tempVector.m_StdVector);
        }
    }
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const std::initializer_list<DenseVector<ElementType>>& InputList)
{
	auto InputVectorNumber = int_max(InputList.size());

	if (InputVectorNumber <= 0)
	{
		this->Clear();
		return;
	}

	int_max TotalElementNumber = 0;

	for (int_max k = 0; k < InputVectorNumber; k++)
	{
		const auto& InputVector = InputList.begin()[k];
		TotalElementNumber += InputVector.GetElementCount();
	}

	if (TotalElementNumber <= 0)
	{
		this->Clear();
		return;
	}

	this->Clear();
	this->SetCapacity(TotalElementNumber);

	for (int_max k = 0; k < InputVectorNumber; k++)
	{
		const auto& InputVector = InputList.begin()[k];
		auto DataPtr = InputVector.GetElementPointer();
		if (DataPtr != nullptr)
		{
			this->Append(DataPtr, InputVector.GetElementCount());
		}
	}
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator=(const std::initializer_list<ElementType>& InputVector)
{
    m_StdVector = InputVector;
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(std::vector<ElementType> InputVector)
{
	m_StdVector = std::move(InputVector);
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(StdObjectVector<ElementType> InputVector)
{
	m_StdVector = std::move(InputVector.StdVector());
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const ObjectArray<ElementType>& InputVector)
{
	this->Copy(InputVector.GetElementPointer(), InputVector.GetElementCount());
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const DenseMatrix<ElementType>& InputVector)
{
	this->Copy(InputVector.GetElementPointer(), InputVector.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
void DenseVector<ElementType>::Copy(const DenseVector<ElementType_input>& InputVector)
{
    this->Copy(InputVector.GetElementPointer(), InputVector.GetLength());
}


template<typename ElementType>
template<typename ElementType_input>
inline
void DenseVector<ElementType>::Copy(const ElementType_input* InputVector, int_max InputLength)
{
    if (InputVector == nullptr || InputLength <= 0)
    {
        //MDK_Warning("Empty input and clear self @ DenseVector::Copy(...)")
        this->Clear();
        return;
    }

    if (size_t(this->GetElementPointer()) == size_t(InputVector))
    {
        if (InputLength != this->GetLength())
        {
            MDK_Error("Something is wrong here @ DenseVector::Copy(...)")
        }
        return;
    }

    this->FastResize(InputLength);

    for (int_max k = 0; k < InputLength; ++k)
    {
        m_StdVector[k] = ElementType(InputVector[k]);
    }
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Clear()
{
    m_StdVector.clear(); 
    m_StdVector.shrink_to_fit();
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Resize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::Resize(...)")
        return false;
    }

try
{
    m_StdVector.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::Resize(...)")
	return false;
}
   return true;
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::FastResize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::FastResize(...)")
        return false;
    }

try
{
    if (Length > int_max(m_StdVector.capacity()))
    {
        m_StdVector.clear(); // no need to copy the old data
    }

    m_StdVector.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::FastResize(...)")
	return false;
}
	return true;
}


template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetCapacity(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::SetCapacity(...)")
        return false;
    }

try
{
    m_StdVector.reserve(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::SetCapacity(...)")
	return false;
}
	return true;
}


template<typename ElementType>
inline
void DenseVector<ElementType>::ReleaseUnusedCapacity()
{
    m_StdVector.shrink_to_fit();
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Fill(const ElementType& Element)
{
	auto SelfLength = this->GetLength();
	if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DenseVector::Fill(Element)")
    }

	for (int_max i = 0; i < SelfLength; ++i)
    {
        m_StdVector[i] = Element;
    }
}
 

template<typename ElementType>
inline
int_max DenseVector<ElementType>::GetLength() const
{
    return int_max(m_StdVector.size());
}


template<typename ElementType>
inline
int_max DenseVector<ElementType>::GetElementCount() const
{
    return int_max(m_StdVector.size());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::IsEmpty() const
{
    return (m_StdVector.size() == 0);
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::IsLengthFixed() const
{
    return false;
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::IsSizeFixed() const
{
    return false;
}

//-----------element access------------------//

template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::GetElementPointer()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::GetElementPointer() const
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::GetPointer()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::GetPointer() const
{
    return m_StdVector.data();
}

template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::begin()
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::begin() const
{
    return m_StdVector.data();
}


template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::end()
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
const ElementType* DenseVector<ElementType>::end() const
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
ElementType& DenseVector<ElementType>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index)")
    }

#endif // MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound    

    return m_StdVector[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index) const")
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_StdVector[Index];
}


template<typename ElementType>
inline
ElementType& DenseVector<ElementType>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index)")
        return m_StdVector[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_StdVector[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index) const")
        return m_StdVector[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_StdVector[Index];
}


template<typename ElementType>
inline
ElementType& DenseVector<ElementType>::at(int_max Index)
{
	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
		return  m_StdVector.at(0);
    }
    return m_StdVector[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::at(int_max Index) const
{
	auto SelfLength = this->GetLength();
	if (Index >= SelfLength || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_StdVector.at(0);
    }
    return m_StdVector[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
std::vector<ElementType>& DenseVector<ElementType>::StdVector()
{
    return m_StdVector;
}


template<typename ElementType>
const std::vector<ElementType>& DenseVector<ElementType>::StdVector() const
{
    return m_StdVector;
}


template<typename ElementType>
std::vector<ElementType> DenseVector<ElementType>::CreateStdVector() const
{
    return m_StdVector;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<ElementType> DenseVector<ElementType>::ConvertToDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->ConvertToDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVector<ElementType>::ConvertToDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    auto SelfLength = this->GetLength();

	OutputVector.FastResize(1, SelfLength);

	for (int_max i = 0; i < SelfLength; ++i)
    {
        OutputVector[i] = m_StdVector[i];
    }
}


template<typename ElementType>
DenseMatrix<ElementType> DenseVector<ElementType>::ConvertToDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->ConvertToDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVector<ElementType>::ConvertToDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
{
	auto SelfLength = this->GetLength();

	OutputVector.FastResize(SelfLength, 1);

	for (int_max i = 0; i < SelfLength; ++i)
    {
        OutputVector[i] = m_StdVector[i];
    }
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(int_max Index_start, int_max Index_end) const
{
    DenseVector<ElementType> SubSet;
    
    auto SelfLength = this->GetLength();

    if (Index_start < 0 || Index_start >= SelfLength || Index_end < 0 || Index_end >= SelfLength || Index_start > Index_end)
    {
        MDK_Error("Invalid input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty input @ DenseVector::GetSubSet(...)")
        return SubSet;
    }

    SubSet.FastResize(Index_end - Index_start + 1);

    for (int_max Index = Index_start; Index <= Index_end; ++Index)
    {
        SubSet[Index - Index_start] = m_StdVector[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const std::initializer_list<int_max>& IndexList) const
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
        auto Index = IndexList.begin()[i];

        if (Index < 0 || Index >= SelfLength)
        {
            MDK_Error("Index is invalid @ DenseVector::GetSubSet(...)")
            SubSet.Clear();
            return SubSet;
        }

        SubSet[i] = m_StdVector[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList) const
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

        SubSet[i] = m_StdVector[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
template<int_max LengthParameter>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const DenseVector<int_max, LengthParameter>& IndexList) const
{
    DenseVector<ElementType> SubSet;

	auto InputLength = IndexList.GetLength();

	if (InputLength == 0)
	{
		return SubSet;
	}

    auto SelfLength = this->GetLength();
	
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

        SubSet[i] = m_StdVector[Index];
    }
    
    return SubSet;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubVector.size()))
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }

	return this->SetSubSet(Index_start, Index_end, SubVector.begin());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(int_max Index_start, int_max Index_end, const DenseMatrix<ElementType>& SubVector)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubVector.GetElementCount()))
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}

	return this->SetSubSet(Index_start, Index_end, SubVector.GetPointer());
}


template<typename ElementType>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType>::SetSubSet(int_max Index_start, int_max Index_end, const DenseVector<ElementType, LengthParameter>& SubVector)
{
	if (std::abs(Index_end - Index_start) + 1 != int_max(SubVector.GetElementCount()))
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}

	return this->SetSubSet(Index_start, Index_end, SubVector.GetPointer());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(int_max Index_start, int_max Index_end, const ElementType* SubVector)
{
	auto SelfLength = this->GetLength();

	if (Index_start < 0 || Index_start >= SelfLength || Index_end < 0 || Index_end >= SelfLength || SubVector == nullptr)
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}

	if (SelfLength == 0)
	{
		MDK_Error("Self is empty @ DenseVector::SetSubSet(...)")
		return false;
	}

	if (Index_end >= Index_start)
	{
		for (int_max Index = Index_start, Index <= Index_end; ++Index)
		{
			m_StdVector[Index] = SubVector[Index - Index_start];
		}
	}
	else
	{
		for (int_max Index = Index_end, Index >= Index_start; --Index)
		{
			m_StdVector[Index] = SubVector[Index_end - Index];
		}
	}
	return true;
}


template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector)
{
	if (IndexList.size() != SubVector.size())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }
	return this->SetSubSet(IndexList.begin(), SubVector.begin(), int_max(SubVector.size()));
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector)
{
	if (IndexList.GetElementCount() != SubVector.GetElementCount())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }
	return this->SetSubSet(IndexList.GetPointer(), SubVector.GetPointer(), SubVector.GetElementCount());
}


template<typename ElementType>
template<int_max LengthParameterA, int_max LengthParameterB>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseVector<int_max, LengthParameterA>& IndexList, const DenseVector<ElementType, LengthParameterB>& SubVector)
{// do not use LengthParameter
	if (IndexList.GetElementCount() != SubVector.GetElementCount())
    {
        MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
        return false;
    }
	return this->SetSubSet(IndexList.GetPointer(), SubVector.GetPointer(), SubVector.GetLength());
}


template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(const DenseVector<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector)
{
	if (IndexList.GetElementCount() != SubVector.GetElementCount())
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(IndexList.GetPointer(), SubVector.GetPointer(), SubVector.GetElementCount());
}


template<typename ElementType>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<ElementType, LengthParameter>& SubVector)
{
	if (IndexList.GetElementCount() != SubVector.GetElementCount())
	{
		MDK_Error("Invalid input @ DenseVector::SetSubSet(...)")
		return false;
	}
	return this->SetSubSet(IndexList.GetPointer(), SubVector.GetPointer(), SubVector.GetElementCount());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(const int_max* IndexList, const ElementType* SubVector, int_max SubVectorLength)
{
	if (IndexList == nullptr || SubVector == nullptr || SubVectorLength <= 0)
	{
		return true;
	}

	auto SelfLength = this->GetLength();

	for (int_max i = 0; i < SubVectorLength; ++i)
	{
		auto Index = IndexList[i];

		if (Index < 0 || Index >= SelfLength)
		{
			MDK_Error("Index is invalid @ DenseVector::SetSubSet(...)")

			return false;
		}

		m_StdVector[Index] = SubVector[i];
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
bool DenseVector<ElementType>::Append(ElementType Element)
{
    auto SelfLength = this->GetElementCount();

    this->Resize(SelfLength + 1);

    m_StdVector[SelfLength] = std::move(Element);

    return true;
}


template<typename ElementType>
template<int_max LengthParameter>
inline 
bool DenseVector<ElementType>::Append(const DenseVector<ElementType, LengthParameter>& InputData)
{
    return this->Append(InputData.GetElementPointer(), InputData.GetElementCount());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (InputData == nullptr || InputLength <= 0)
    {
        //MDK_Warning("Input is empty @ DenseVector::Append(const ElementType* InputData, int_max InputLength)")
        return true;
    }

    auto SelfLength = this->GetElementCount();

    this->Resize(SelfLength + InputLength);

    for (int_max i = SelfLength; i < SelfLength + InputLength; ++i)
    {
        m_StdVector[i] = InputData[i - SelfLength];
    }

    return true;
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(int_max Index)
{
    return Delete(&Index, 1);
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseVector::Delete(const DenseMatrix<int_max>& IndexList)")
        return false;
    }

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType>::Delete(const DenseVector<int_max, LengthParameter>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementCount());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    if (IndexList == nullptr || ListLength <= 0)
    {
        //MDK_Warning("Empty Input @ DenseVector::Delete(const int_max* IndexList, int_max ListLength)")
        return true;
    }

    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DenseVector::Delete(const int_max* IndexList, int_max ListLength)")
        return false;
    }

    for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ DenseVector::Delete(const int_max* IndexList, int_max ListLength)")
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
                MDK_Warning("duplicate Input @ DenseVector::Delete(const int_max* IndexPtr, int_max Length)")
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
bool DenseVector<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ DenseVector::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    if (Index_end < Index_start
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0)
    {
        MDK_Error("Invalid Input @ DenseVector::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }

    m_StdVector.erase(m_StdVector.begin() + Index_start, m_StdVector.begin() + Index_end + 1);

    return true;
}


template<typename ElementType>
inline bool DenseVector<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    return this->Insert(Index, &Element, 1);
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputData)
{
    return this->Insert(Index, InputData.begin(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputData)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseVector::Insert(Index, DenseMatrix)")
        return false;
    }

    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementCount());
}


template<typename ElementType>
template<int_max LengthParameter>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const DenseVector<ElementType, LengthParameter>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementCount());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    auto SelfLength = this->GetElementCount();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DenseVector::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }
    else
    {
        if (Index >= SelfLength+1 || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DenseVector::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    m_StdVector.insert(m_StdVector.begin() + Index, InputData, InputData + InputLength);

    return true;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
bool DenseVector<ElementType>::PushBack(ElementType Element)
{
    m_StdVector.push_back(std::move(Element));
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::PopBack()
{
    m_StdVector.pop_back(std::move(Element));
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::Find(MatchFunctionType MatchFunction) const
{
    return this->Find(this->GetLength(), 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const
{
    return this->Find(MaxOutputNumber, 0, this->GetLength()-1, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::
Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const
{
    DenseVector<int_max> IndexList;

    if (MaxOutputNumber == 0)
    {
        return IndexList;
    }

    auto ElementNumber = this->GetElementCount();

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

    IndexList.SetCapacity(MaxOutputNumber);

	if (Index_start < Index_end)
	{
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
	}
	else //if (Index_start > Index_end)
	{
		for (int_max i = Index_start; i >= Index_end; --i)
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
	}

    return IndexList;
}



template<typename ElementType>
template<typename MatchFunctionType>
inline 
int_max DenseVector<ElementType>::Find(const std::string& first_or_last, MatchFunctionType MatchFunction) const
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
		for (int_max i = this->GetElementCount()-1; i >= 0; --i)
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


template<typename ElementType>
inline 
DenseVector<int_max> DenseVector<ElementType>::ExactMatch(const ElementType& InputElement) const
{
	return this->Find([&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
inline 
int_max DenseVector<ElementType>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
{
	return this->Find(first_or_last, [&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetLength() - 1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const
{
    DenseVector<int_max> IndexList;

    auto ElementNumber = this->GetElementCount();

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

    IndexList.FastResize(ElementNumber);

    for (int_max i = Index_start; i <= Index_end; ++i)
    {
        IndexList[i] = i;
    }

    std::sort(IndexList.begin(), IndexList.end(), [&](int_max a, int_max b) { return CompareFunction((*this)[a], (*this)[b]); });

    return IndexList;
}


template<typename ElementType>
inline
DenseVector<int_max> DenseVector<ElementType>::Sort(const char* Order)
{
    std::string Order_str(Order);
    return this->Sort(Order_str);
}


template<typename ElementType>
inline
DenseVector<int_max> DenseVector<ElementType>::Sort(const std::string& Order)
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
        MDK_Error("invalid input string @ DenseVector::Sort(Order)")
        DenseVector<int_max> EmptyIndexList;
        return EmptyIndexList;
    }
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
void DenseVector<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
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
void DenseVector<ElementType>::SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction)
{
    if (this->IsEmpty() == true)
    {
        return;
    }

    std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}


template<typename ElementType>
inline
void DenseVector<ElementType>::SortInPlace(const char* Order)
{
    std::string Order_str(Order);
    this->SortInPlace(Order_str);
}


template<typename ElementType>
inline
void DenseVector<ElementType>::SortInPlace(const std::string& Order)
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

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
DenseVector<int_max> DenseVector<ElementType>::FindUnique() const
{
    return FindUniqueElementInVector(*this);
}


template<typename ElementType>
template<typename SpecialCompareFunctionType>
inline
DenseVector<int_max> DenseVector<ElementType>::FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const
{
    return FindUniqueElementInVector(*this, SpecialCompareFunction);
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
template<int_max LengthParameter>
inline
void DenseVector<ElementType>::operator+=(const DenseVector<ElementType, LengthParameter>& InputVector)
{
	auto InputLength = InputVector.GetLength();
    if (InputLength == 1)
    {
        (*this) += InputVector[0];
        return;
    }

	auto SelfLength = this->GetLength();
    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator+=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_StdVector[i] += InputVector[i];
    }
}


template<typename ElementType>
template<int_max LengthParameter>
inline
void DenseVector<ElementType>::operator-=(const DenseVector<ElementType, LengthParameter>& InputVector)
{
 	auto InputLength = InputVector.GetLength();
    if (InputLength == 1)
    {
        (*this) -= InputVector[0];
        return;
    }

	auto SelfLength = this->GetLength();
    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator-=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_StdVector[i] -= InputVector[i];
    }
}

/*
template<typename ElementType>
template<int_max LengthParameter>
inline
void DenseVector<ElementType>::operator*=(const DenseVector<ElementType, LengthParameter>& InputVector)
{
	auto InputLength = InputVector.GetLength();
    if (InputLength == 1)
    {
        (*this) *= InputVector[0];
        return;
    }

	auto SelfLength = this->GetLength();
    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator*=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_StdVector[i] *= InputVector[i];
    }
}
*/

template<typename ElementType>
template<int_max LengthParameter>
inline
void DenseVector<ElementType>::operator/=(const DenseVector<ElementType, LengthParameter>& InputVector)
{
	auto InputLength = InputVector.GetLength();
    if (InputLength == 1)
    {
        (*this) /= InputVector[0];
        return;
    }

	auto SelfLength = this->GetLength();
    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator/=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_StdVector[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void DenseVector<ElementType>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_StdVector[i] += Element;
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_StdVector[i] -= Element;
    }
}


template<typename ElementType>
inline void DenseVector<ElementType>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_StdVector[i] *= Element;
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_StdVector[i] /= Element;
    }
}

//----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
ElementType DenseVector<ElementType>::Sum() const
{
    auto Value = ElementType(0);

    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        Value += m_StdVector[i];
    }

    return Value;
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::Mean() const
{
    auto Value = this->Sum();

    Value /= ElementType(this->GetLength());

    return Value;
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::Std() const
{
    auto MeanValue = this->Mean();

    auto Value = ElementType(0);

    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        auto temp = m_StdVector[i] - MeanValue;

        Value += temp*temp;
    }

    Value /= ElementType(this->GetLength());

    Value = std::sqrt(Value);

    return Value;
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::Max() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::Max()")
        return 0;
    }

    auto Index = this->IndexOfMax();
    return m_StdVector[Index];
}


template<typename ElementType>
inline
int_max DenseVector<ElementType>::IndexOfMax() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::Max()")
        return -1;
    }

    auto Index = int_max(0);
    auto Value = m_StdVector[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        if (m_StdVector[i] > Value)
        {
            Value = m_StdVector[i];
            Index = i;
        }
    }

    return Index;
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::Min() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::Min()")
        return 0;
    }

    auto Index = this->IndexOfMin();
    return m_StdVector[Index];
}


template<typename ElementType>
inline
int_max DenseVector<ElementType>::IndexOfMin() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::Min()")
        return 0;
    }

    auto Index = int_max(0);
    auto Value = m_StdVector[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        if (m_StdVector[i] < Value)
        {
            Value = m_StdVector[i];
            Index = i;
        }
    }

    return Index;
}


template<typename ElementType>
ElementType DenseVector<ElementType>::L1Norm() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::L1Norm()")
        return 0;
    }

    auto Value = ElementType(0);

    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        Value += std::abs(m_StdVector[i]);
    }

    return Value;
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::L2Norm() const
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ DenseVector::L2Norm()")
        return 0;
    }

    auto Value = m_StdVector[0] * m_StdVector[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        Value += m_StdVector[i] * m_StdVector[i];
    }

    Value = std::sqrt(Value);

    return Value;
}


}// namespace mdk

#endif