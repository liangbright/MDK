#ifndef __mdkDenseVectorWithVariableLength_hpp
#define __mdkDenseVectorWithVariableLength_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector()
{
    this->Clear();
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const ElementType& Element)
{
    m_DataArray.resize(1);
    m_DataArray[0] = Element;
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const std::initializer_list<ElementType>& InputVector)
{
    m_DataArray = InputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(const DenseVector<ElementType>& InputVector)
{
    m_DataArray = InputVector.m_DataArray;
}


template<typename ElementType>
template<int_max InputLength>
inline
DenseVector<ElementType>::DenseVector(const DenseVector<ElementType, InputLength>& InputVector)
{
    if (InputLength > 0)
    {
        m_DataArray.resize(InputLength);

        for (int_max i = 0; i < InputLength; ++i)
        {
            m_DataArray[i] = InputVector.m_DataArray[i];
        }
    }
}


template<typename ElementType>
inline
DenseVector<ElementType>::DenseVector(DenseVector<ElementType>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
DenseVector<ElementType>::~DenseVector()
{
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const DenseVector<ElementType>& InputVector)
{
    this->Copy(InputVector);
}


template<typename ElementType>
template<int_max InputLength>
inline 
void DenseVector<ElementType>::operator=(const DenseVector<ElementType, InputLength>& InputVector)
{
    this->Copy(InputVector.GetElementPointer(), InputVector.GetLength());
}


template<typename ElementType>
inline void DenseVector<ElementType>::operator=(DenseVector<ElementType>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const std::initializer_list<const DenseVector<ElementType>*>& InputList)
{
    auto InputVectorNumber = int_max(InputList.size());

    if (InputVectorNumber <= 0)
    {
        MDK_Error("Input is empty @ DenseVector::operator=(initializer_list of DenseVector pointer)")
        return;
    }

    bool IsSelfInInputList = false;

    int_max TotalElementNumber = 0;

    for (int_max k = 0; k < InputVectorNumber; k++)
    {
        auto InputVectorPtr = InputList.begin()[k];

        TotalElementNumber += InputVectorPtr->GetElementNumber();

        if (this->GetElementPointer() == InputVectorPtr->GetElementPointer())
        {
            IsSelfInInputList = true;
        }
    }

    if (TotalElementNumber <= 0)
    {
        MDK_Error("TotalElementNumber is 0 @ DenseVector::operator=(initializer_list of DenseVector pointer)")
        return;
    }

    auto SelfLength = this->GetLength();

    if (IsSelfInInputList == false)
    {
        m_DataArray.clear();

        for (int_max k = 0; k < InputVectorNumber; k++)
        {            
            auto InputVectorPtr = InputList.begin()[k];

            auto DataPtr = InputVectorPtr->GetElementPointer();
            if (DataPtr != nullptr)
            {
                this->Append(DataPtr, InputVectorPtr->GetElementNumber());
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

            m_DataArray = std::move(tempVector.m_DataArray);
        }
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator=(const std::initializer_list<ElementType>& InputVector)
{
    m_DataArray = InputVector;
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const std::vector<ElementType>& InputVector)
{
    this->Copy(InputVector.data(), int_max(InputVector.size()));
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator=(const DenseMatrix<ElementType>& InputVector)
{
    this->Copy(InputVector.GetElementPointer(), InputVector.GetLength());
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
        m_DataArray[k] = ElementType(InputVector[k]);
    }
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Clear()
{
    m_DataArray.clear(); 
    m_DataArray.shrink_to_fit();
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Resize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::Resize(...)")
        return;
    }

try
{
    m_DataArray.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::Resize(...)")
}
   
}


template<typename ElementType>
inline
void DenseVector<ElementType>::FastResize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::FastResize(...)")
        return;
    }

try
{
    if (Length > int_max(m_DataArray.capacity()))
    {
        m_DataArray.clear();
    }

    m_DataArray.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::FastResize(...)")
}

}


template<typename ElementType>
inline 
void DenseVector<ElementType>::ReserveCapacity(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVector::ReserveCapacity(...)")
        return;
    }

try
{
    m_DataArray.reserve(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVector::ReserveCapacity(...)")
}
}


template<typename ElementType>
inline
void DenseVector<ElementType>::ReleaseUnusedCapacity()
{
    m_DataArray.shrink_to_fit();
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Squeeze()
{
    this->ReleaseUnusedCapacity();
}


template<typename ElementType>
inline
void DenseVector<ElementType>::Fill(const ElementType& Element)
{
    auto Length = this->GetLength();
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType>
inline
int_max DenseVector<ElementType>::GetLength() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
int_max DenseVector<ElementType>::GetElementNumber() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::IsEmpty() const
{
    return (m_DataArray.size() == 0);
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
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::GetElementPointer() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::GetPointer()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::GetPointer() const
{
    return m_DataArray.data();
}

template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::begin()
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
const ElementType* DenseVector<ElementType>::begin() const
{
    return m_DataArray.data();
}


template<typename ElementType>
inline
ElementType* DenseVector<ElementType>::end()
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
const ElementType* DenseVector<ElementType>::end() const
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
ElementType& DenseVector<ElementType>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index)")
    }

#endif // MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator[](Index) const")
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVector<ElementType>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::operator()(Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVector<ElementType>::at(int_max Index)
{
    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_DataArray[0];
    }
    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVector<ElementType>::at(int_max Index) const
{
    auto Length = this->GetLength();
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVector::at(Index)")
        return m_DataArray[0];
    }
    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
std::vector<ElementType>& DenseVector<ElementType>::StdVector()
{
    return m_DataArray;
}


template<typename ElementType>
const std::vector<ElementType>& DenseVector<ElementType>::StdVector() const
{
    return m_DataArray;
}


template<typename ElementType>
std::vector<ElementType> DenseVector<ElementType>::CreateStdVector() const
{
    return m_DataArray;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<ElementType> DenseVector<ElementType>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVector<ElementType>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    auto Length = this->GetLength();

    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType>
DenseMatrix<ElementType> DenseVector<ElementType>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVector<ElementType>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
{
    auto Length = this->GetLength();

    OutputVector.FastResize(Length, 1);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
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
        SubSet[Index - Index_start] = m_DataArray[Index];
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

        SubSet[i] = m_DataArray[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const std::vector<int_max>& IndexList) const
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
        MDK_Error("Self is empty @ DenseVector::GetSubSet(...)")
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

        SubSet[i] = m_DataArray[Index];
    }

    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const DenseMatrix<int_max>& IndexList) const
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

        SubSet[i] = m_DataArray[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const SimpleDataArray<int_max>& IndexList) const
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

        SubSet[i] = m_DataArray[Index];
    }
    
    return SubSet;
}


template<typename ElementType>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const DenseVector<int_max>& IndexList) const
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

        SubSet[i] = m_DataArray[Index];
    }

    return SubSet;
}


template<typename ElementType>
template<int_max Length>
inline
DenseVector<ElementType> DenseVector<ElementType>::GetSubSet(const DenseVector<int_max, Length>& IndexList) const
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

        SubSet[i] = m_DataArray[Index];
    }
    
    return SubSet;
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubSet)
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


template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet)
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


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSet)
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


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubSet)
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


template<typename ElementType>
inline 
bool DenseVector<ElementType>::SetSubSet(const SimpleDataArray<int_max>& IndexList, const SimpleDataArray<ElementType>& SubSet)
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


template<typename ElementType>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<int_max>& SubSet)
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


template<typename ElementType>
template<int_max InputLength>
inline
bool DenseVector<ElementType>::SetSubSet(const DenseVector<int_max, InputLength>& IndexList, const DenseVector<int_max, InputLength>& SubSet)
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


template<typename ElementType>
inline
bool DenseVector<ElementType>::Append(ElementType Element)
{
    auto SelfLength = this->GetElementNumber();

    this->Resize(SelfLength + 1);

    m_DataArray[SelfLength] = std::move(Element);

    return true;
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Append(const ElementType* InputData, int_max InputLength)
{
    if (InputData == nullptr || InputLength <= 0)
    {
        MDK_Error("Invalid Input: empty @ DenseVector::Append(const ElementType* InputData, int_max InputLength)")
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
bool DenseVector<ElementType>::Delete(const std::vector<int_max>& IndexList)
{
    return this->Delete(IndexList.data(), int_max(IndexList.size()));
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

    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(const SimpleDataArray<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Delete(const DenseVector<int_max>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
template<int_max InputLength>
inline
bool DenseVector<ElementType>::Delete(const DenseVector<int_max, InputLength>& IndexList)
{
    return this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
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

    auto SelfLength = this->GetElementNumber();

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
        m_DataArray.erase(m_DataArray.begin() + IndexList[0], m_DataArray.begin() + IndexList[0] + 1);
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
                m_DataArray.erase(m_DataArray.begin() + Index_i, m_DataArray.begin() + Index_i + 1);

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
    auto SelfLength = this->GetElementNumber();

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

    m_DataArray.erase(m_DataArray.begin() + Index_start, m_DataArray.begin() + Index_end + 1);

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
bool DenseVector<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.data(), int_max(InputData.size()));
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputData)
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
bool DenseVector<ElementType>::Insert(int_max Index, const SimpleDataArray<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const DenseVector<ElementType>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
template<int_max InputLength>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const DenseVector<ElementType, InputLength>& InputData)
{
    return this->Insert(Index, InputData.GetElementPointer(), InputData.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseVector<ElementType>::Insert(int_max Index, const ElementType* InputData, int_max InputLength)
{
    auto SelfLength = this->GetElementNumber();

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
        if (Index >= SelfLength || Index < 0 || InputData == nullptr || InputLength <= 0)
        {
            MDK_Error("Invalid Input @ DenseVector::Insert(Index, const ElementType* InputData, int_max InputLength)")
            return false;
        }
    }

    m_DataArray.insert(m_DataArray.begin() + Index, InputData, InputData + InputLength);

    return true;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
bool DenseVector<ElementType>::PushBack(ElementType Element)
{
    m_DataArray.push_back(std::move(Element));
}


template<typename ElementType>
inline
ElementType DenseVector<ElementType>::PopBack()
{
    m_DataArray.pop_back(std::move(Element));
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
template<typename MatchFunctionType>
inline 
int_max DenseVector<ElementType>::Match(MatchFunctionType MatchFunction) const
{
    // find with MaxOutputNumber = 1 

    int_max Index_output = -1;

    for (int_max i = 0; i < this->GetElementNumber(); ++i)
    {
        if (MatchFunction((*this)[i]) == true)
        {
            Index_output = i;
            break;
        }
    }

    return Index_output;
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
inline 
void DenseVector<ElementType>::operator+=(const DenseVector<ElementType>& InputVector)
{
    auto SelfLength = this->GetLength();

    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        (*this) += InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator+=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType>
template<int_max InputLength>
inline
void DenseVector<ElementType>::operator+=(const DenseVector<ElementType, InputLength>& InputVector)
{
    auto SelfLength = this->GetLength();

    if (InputLength == 1)
    {
        (*this) += InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator+=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator-=(const DenseVector<ElementType>& InputVector)
{
    auto SelfLength = this->GetLength();

    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        (*this) -= InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator-=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType>
template<int_max InputLength>
inline
void DenseVector<ElementType>::operator-=(const DenseVector<ElementType, InputLength>& InputVector)
{
    auto SelfLength = this->GetLength();

    if (InputLength == 1)
    {
        (*this) -= InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator-=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}



template<typename ElementType>
inline
void DenseVector<ElementType>::operator*=(const DenseVector<ElementType>& InputVector)
{
    auto SelfLength = this->GetLength();

    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
         (*this) *= InputVector[0];
         return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator*=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType>
template<int_max InputLength>
inline
void DenseVector<ElementType>::operator*=(const DenseVector<ElementType, InputLength>& InputVector)
{
    auto SelfLength = this->GetLength();

    if (InputLength == 1)
    {
        (*this) *= InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator*=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVector<ElementType>::operator/=(const DenseVector<ElementType>& InputVector)
{
    auto SelfLength = this->GetLength();

    auto InputLength = InputVector.GetLength();

    if (InputLength == 1)
    {
        (*this) /= InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator/=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}


template<typename ElementType>
template<int_max InputLength>
inline
void DenseVector<ElementType>::operator/=(const DenseVector<ElementType, InputLength>& InputVector)
{
    auto SelfLength = this->GetLength();

    if (InputLength == 1)
    {
        (*this) /= InputVector[0];
        return;
    }

    if (SelfLength != InputLength)
    {
        MDK_Error("SelfLength != InputLength @ DenseVector::operator/=(InputVector)")
    }

    for (int_max i = 0; i < SelfLength; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void DenseVector<ElementType>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType>
inline void DenseVector<ElementType>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType>
inline 
void DenseVector<ElementType>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] /= Element;
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
        Value += m_DataArray[i];
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
        auto temp = m_DataArray[i] - MeanValue;

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

    return m_DataArray[Index];
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

    auto Value = m_DataArray[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        if (m_DataArray[i] > Value)
        {
            Value = m_DataArray[i];
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

    return m_DataArray[Index];
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

    auto Value = m_DataArray[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        if (m_DataArray[i] < Value)
        {
            Value = m_DataArray[i];
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
        Value += std::abs(m_DataArray[i]);
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

    auto Value = m_DataArray[0] * m_DataArray[0];

    for (int_max i = 1; i < this->GetLength(); ++i)
    {
        Value += m_DataArray[i] * m_DataArray[i];
    }

    Value = std::sqrt(Value);

    return Value;
}


}// namespace mdk

#endif