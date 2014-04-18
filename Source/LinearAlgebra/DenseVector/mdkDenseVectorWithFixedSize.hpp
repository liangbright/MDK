#ifndef __mdkDenseVectorWithFixedSize_hpp
#define __mdkDenseVectorWithFixedSize_hpp


namespace mdk
{
 
template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>::DenseVectorWithFixedSize()
{
    if (Length <= 0)
    {
        MDK_Error("Length <= 0 @ DenseVectorWithFixedSize::DenseVectorWithFixedSize()")
    }

    this->Clear();
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>::DenseVectorWithFixedSize(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>::DenseVectorWithFixedSize(DenseVectorWithFixedSize<ElementType, Length>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>::~DenseVectorWithFixedSize()
{

}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::
operator=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::
operator=(DenseVectorWithFixedSize<ElementType, Length>&& InputVector);
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::Clear()
{
    this->Fill(ElementType(0));
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::Fill(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType, int_max Length>
inline
int_max DenseVectorWithFixedSize<ElementType, Length>::GetLength()
{
    return Length;
}


//-----------element access------------------//

template<typename ElementType, int_max Length>
inline
ElementType* DenseVectorWithFixedSize<ElementType, Length>::GetElementPointer()
{
    return m_DataArray->data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* DenseVectorWithFixedSize<ElementType, Length>::GetElementPointer() const
{
    return m_DataArray->data();
}



template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedSize<ElementType, Length>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::operator[](Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedSize<ElementType, Length>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::operator[](Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedSize<ElementType, Length>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::operator()(Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedSize<ElementType, Length>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::operator()(Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithFixedSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& DenseVectorWithFixedSize<ElementType, Length>::at(int_max Index)
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& DenseVectorWithFixedSize<ElementType, Length>::at(int_max Index) const
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithFixedSize::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVectorWithFixedSize<ElementType, Length>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithFixedSize<ElementType, Length>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType, int_max Length>
DenseMatrix<ElementType> DenseVectorWithFixedSize<ElementType, Length>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithFixedSize<ElementType, Length>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
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
void DenseVectorWithFixedSize<ElementType, Length>::operator+=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator-=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator*=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator/=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void DenseVectorWithFixedSize<ElementType, Length>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] /= Element;
    }
}

}// namespace mdk


#endif