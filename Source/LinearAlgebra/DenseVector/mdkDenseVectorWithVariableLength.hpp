#ifndef __mdkDenseVectorWithVariableLength_hpp
#define __mdkDenseVectorWithVariableLength_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>::DenseVectorWithVariableLength()
{
    this->Clear();
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>::DenseVectorWithVariableLength(const ElementType& Element)
{
    m_DataArray.resize(1);

    m_DataArray[0] = Element;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>::DenseVectorWithVariableLength(DenseVectorWithVariableLength<ElementType>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>::~DenseVectorWithVariableLength()
{

}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::
operator=(const DenseVectorWithVariableLength<ElementType>& InputVector);
{
    auto Length = this->GetLength();

    if (Length != InputVector.GetLength())
    {
        MDK_Error("Size does not match @ DenseVectorWithVariableLength::operator=(& Voxel)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::
operator=(DenseVectorWithVariableLength<ElementType>&& InputVector);
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::Clear()
{
    m_DataArray.clear(); 
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::Resize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::Resize(...)")
        return;
    }

try
{
    m_DataArray.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVectorWithVariableLength::Resize(...)")

    m_DataArray.clear();
}
   
}


template<typename ElementType>
inline 
void DenseVectorWithVariableLength<ElementType>::ReservCapacity(int_max Length);
{
    try
    {
        m_DataArray.reserve(Length);
    }
    catch (...)
    {
        MDK_Error("Out of Memory @ DenseVectorWithVariableLength::Resize(...)")

        m_DataArray.clear();
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::Fill(const ElementType& Element)
{
    auto Length = this->GetLength();

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType>
inline
int_max DenseVectorWithVariableLength<ElementType>::GetLength() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
int_max DenseVectorWithVariableLength<ElementType>::GetElementNumber() const
{
    return int_max(m_DataArray.size());
}


template<typename ElementType>
inline
bool DenseVectorWithVariableLength<ElementType>::IsLengthFixed() const
{
    return false;
}


template<typename ElementType>
inline
bool DenseVectorWithVariableLength<ElementType>::IsSizeFixed() const
{
    return false;
}

//-----------element access------------------//

template<typename ElementType>
inline
ElementType* DenseVectorWithVariableLength<ElementType>::GetElementPointer()
{
    return m_DataArray->data();
}


template<typename ElementType>
inline
const ElementType* DenseVectorWithVariableLength<ElementType>::GetElementPointer() const
{
    return m_DataArray->data();
}



template<typename ElementType>
inline
ElementType& DenseVectorWithVariableLength<ElementType>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::operator[](Index)")
    }

#endif // MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableLength<ElementType>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::operator[](Index) const")
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVectorWithVariableLength<ElementType>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::operator()(Index)")
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableLength<ElementType>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::operator()(Index) const")
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVectorWithVariableLength<ElementType>::at(int_max Index)
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::at(Index)")
    }

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableLength<ElementType>::at(int_max Index) const
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableLength::at(Index)")
    }

    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<ElementType> DenseVectorWithVariableLength<ElementType>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithVariableLength<ElementType, Length>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    auto Length = this->GetLength();

    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType>
DenseMatrix<ElementType> DenseVectorWithVariableLength<ElementType>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVectorWithVariableLength<ElementType>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
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
void DenseVectorWithVariableLength<ElementType>::operator+=(const DenseVectorWithVariableLength<ElementType>& InputVector)
{
    auto Length_A = this->GetLength();

    auto Length_B = InputVector.GetLength();

    if (Length_B == 1)
    {
        (*this) += InputVector[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableLength::operator+=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableLength<ElementType>::operator-=(const DenseVectorWithVariableLength<ElementType>& InputVector)
{
    auto Length_A = this->GetLength();

    auto Length_B = InputVector.GetLength();

    if (Length_B == 1)
    {
        (*this) -= InputVector[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableLength::operator-=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::operator*=(const DenseVectorWithVariableLength<ElementType>& InputVector)
{
    auto Length_A = this->GetLength();

    auto Length_B = InputVector.GetLength();

    if (Length_B == 1)
    {
         (*this) *= InputVector[0];
         return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableLength::operator*=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableLength<ElementType>::operator/=(const DenseVectorWithVariableLength<ElementType>& InputVector)
{
    auto Length_A = this->GetLength();

    auto Length_B = InputVector.GetLength();

    if (Length_B == 1)
    {
        (*this) /= InputVector[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableLength::operator/=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void DenseVectorWithVariableLength<ElementType>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableLength<ElementType>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType>
inline void DenseVectorWithVariableLength<ElementType>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableLength<ElementType>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] /= Element;
    }
}

}// namespace mdk

#endif