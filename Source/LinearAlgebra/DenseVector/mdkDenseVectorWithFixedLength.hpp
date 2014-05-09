#ifndef __mdkDenseVectorWithFixedLength_hpp
#define __mdkDenseVectorWithFixedLength_hpp


namespace mdk
{
 
template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>::DenseVectorWithFixedLength()
{
    if (Length <= 0)
    {
        MDK_Error("Length <= 0 @ DenseVectorWithFixedLength::DenseVectorWithFixedLength()")
    }

    this->Clear();
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>::DenseVectorWithFixedLength(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>::DenseVectorWithFixedLength(DenseVectorWithFixedLength<ElementType, Length>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>::~DenseVectorWithFixedLength()
{

}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::
operator=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::
operator=(DenseVectorWithFixedLength<ElementType, Length>&& InputVector);
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::Clear()
{
    this->Fill(ElementType(0));
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::Fill(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType, int_max Length>
inline
int_max DenseVectorWithFixedLength<ElementType, Length>::GetLength() const
{
    return Length;
}


template<typename ElementType, int_max Length>
inline
bool DenseVectorWithFixedLength<ElementType, Length>::IsLengthFixed() const
{
    return true;
}


template<typename ElementType, int_max Length>
inline
bool DenseVectorWithFixedLength<ElementType, Length>::IsLengthVariable() const
{
    return false;
}


//-----------element access------------------//

template<typename ElementType, int_max Length>
inline
ElementType* DenseVectorWithFixedLength<ElementType, Length>::GetElementPointer()
{
    return m_DataArray->data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVectorWithFixedLength<ElementType, Length>::GetElementPointer() const
{
    return m_DataArray->data();
}



template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedLength<ElementType, Length>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::operator[](Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedLength<ElementType, Length>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::operator[](Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedLength<ElementType, Length>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::operator()(Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedLength<ElementType, Length>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::operator()(Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedLength<ElementType, Length>::at(int_max Index)
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedLength<ElementType, Length>::at(int_max Index) const
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedLength::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVectorWithFixedLength<ElementType, Length>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithFixedLength<ElementType, Length>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVectorWithFixedLength<ElementType, Length>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithFixedLength<ElementType, Length>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
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
void DenseVectorWithFixedLength<ElementType, Length>::operator+=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator-=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator*=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator/=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedLength<ElementType, Length>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= Element;
    }
}

}// namespace mdk


#endif