#ifndef __mdkDenseVectorWithVariableSize_hpp
#define __mdkDenseVectorWithVariableSize_hpp


namespace mdk
{
 
template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>::DenseVectorWithVariableSize()
{
    this->Clear();
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>::DenseVectorWithVariableSize(const ElementType& Element)
{
    m_DataArray.resize(1);

    m_DataArray[0] = Element;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>::DenseVectorWithVariableSize(DenseVectorWithVariableSize<ElementType>&& InputVector)
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>::~DenseVectorWithVariableSize()
{

}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::
operator=(const DenseVectorWithVariableSize<ElementType>& InputVector);
{
    auto Length = this->GetLength();

    if (Length != InputVector.GetLength())
    {
        MDK_Error("Size does not match @ DenseVectorWithVariableSize::operator=(& Voxel)")
        return;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::
operator=(DenseVectorWithVariableSize<ElementType>&& InputVector);
{
    m_DataArray = std::move(InputVector.m_DataArray);
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::Clear()
{
    m_DataArray.clear(); 
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::Resize(int_max Length)
{
    if (Length < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::Resize(...)")
        return;
    }

try
{
    m_DataArray.resize(Length);
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseVectorWithVariableSize::Resize(...)")

    m_DataArray.clear();
}
   
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::Fill(const ElementType& Element)
{
    auto Length = this->GetLength();

    for (int_max i = 0; i < Length; ++i)
    {
        m_DataArray[i] = Element;
    }
}
 

template<typename ElementType>
inline
int_max DenseVectorWithVariableSize<ElementType>::GetLength()
{
    return int_max(m_DataArray.size());
}


//-----------element access------------------//

template<typename ElementType>
inline
ElementType* DenseVectorWithVariableSize<ElementType>::GetElementPointer()
{
    return m_DataArray->data();
}


template<typename ElementType>
inline
const ElementType* DenseVectorWithVariableSize<ElementType>::GetElementPointer() const
{
    return m_DataArray->data();
}



template<typename ElementType>
inline
ElementType& DenseVectorWithVariableSize<ElementType>::operator[](int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::operator[](Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_DenseVectorWithVariableSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableSize<ElementType>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::operator[](Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVectorWithVariableSize<ElementType>::operator()(int_max Index)
{
#if defined MDK_DEBUG_DenseVectorWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::operator()(Index)")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableSize<ElementType>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_DenseVectorWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::operator()(Index) const")
        return m_DataArray[0];
    }

#endif // MDK_DEBUG_3DImageDenseVectorWithVariableSize_Operator_CheckBound    

    return m_DataArray[Index];
}


template<typename ElementType>
inline
ElementType& DenseVectorWithVariableSize<ElementType>::at(int_max Index)
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}


template<typename ElementType>
inline
const ElementType& DenseVectorWithVariableSize<ElementType>::at(int_max Index) const
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error("Invalid input @ DenseVectorWithVariableSize::at(Index)")
        return m_DataArray[0];
    }

    return m_DataArray[Index];
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<ElementType> DenseVectorWithVariableSize<ElementType>::CreateDenseMatrixAsRowVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsRowVector(OutputVector);

    return OutputVector;
}


template<typename ElementType, int_max Length>
void DenseVectorWithVariableSize<ElementType, Length>::CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const
{
    auto Length = this->GetLength();

    OutputVector.FastResize(1, Length);

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = m_DataArray[i];
    }
}


template<typename ElementType>
DenseMatrix<ElementType> DenseVectorWithVariableSize<ElementType>::CreateDenseMatrixAsColVector() const
{
    DenseMatrix<ElementType> OutputVector;

    this->CreateDenseMatrixAsColVector(OutputVector);

    return OutputVector;
}


template<typename ElementType>
void DenseVectorWithVariableSize<ElementType>::CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const
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
void DenseVectorWithVariableSize<ElementType>::operator+=(const DenseVectorWithVariableSize<ElementType>& InputVector)
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
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableSize::operator+=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] += InputVector[i];
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableSize<ElementType>::operator-=(const DenseVectorWithVariableSize<ElementType>& InputVector)
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
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableSize::operator-=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] -= InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::operator*=(const DenseVectorWithVariableSize<ElementType>& InputVector)
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
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableSize::operator*=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] *= InputVector[i];
    }
}


template<typename ElementType>
inline
void DenseVectorWithVariableSize<ElementType>::operator/=(const DenseVectorWithVariableSize<ElementType>& InputVector)
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
        MDK_Error("Length_A != Length_B @ DenseVectorWithVariableSize::operator/=(InputVector)")
    }

    for (int_max i = 0; i < Length_A; ++i)
    {
        m_DataArray[i] /= InputVector[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void DenseVectorWithVariableSize<ElementType>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] += Element;
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableSize<ElementType>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] -= Element;
    }
}


template<typename ElementType>
inline void DenseVectorWithVariableSize<ElementType>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] *= Element;
    }
}


template<typename ElementType>
inline 
void DenseVectorWithVariableSize<ElementType>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < this->GetLength(); ++i)
    {
        m_DataArray[i] /= Element;
    }
}

}// namespace mdk

#endif