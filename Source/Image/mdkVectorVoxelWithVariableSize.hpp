#ifndef __mdkVectorVoxelWithVariableSize_hpp
#define __mdkVectorVoxelWithVariableSize_hpp

//#include "mdkVectorVoxelWithVariableSize.h"

namespace mdk
{
 
template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>::VectorVoxelWithVariableSize()
{
    this->Clear();
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>::VectorVoxelWithVariableSize(const ElementType& Element, int64 Length = 1)
{
    m_ElementData.resize(Length);

    this->Fill(Element);
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>::VectorVoxelWithVariableSize(VectorVoxelWithVariableSize<ElementType>&& Voxel)
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>::~VectorVoxelWithVariableSize()
{

}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::
operator=(const VectorVoxelWithVariableSize<ElementType>& Voxel);
{
    auto Length = this->GetLength();

    if (Length != Voxel.GetLength())
    {
        MDK_Error << "Size does not match @ VectorVoxelWithVariableSize::operator=(& Voxel)" << '\n';
        return;
    }

    for (int64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Voxel[i];
    }
}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::
operator=(VectorVoxelWithVariableSize<ElementType>&& Voxel);
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::Clear()
{
    m_ElementData.clear();

    m_ZeroElement = m_ZeroElement - m_ZeroElement;    
}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::Fill(const ElementType& Element)
{
    auto Length = this->GetLength();

    for (int64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }

    m_ZeroElement = Element - Element;
}
 

template<typename ElementType>
inline
int64 VectorVoxelWithVariableSize<ElementType>::GetLength()
{
    return int64(m_ElementData.size());
}


//-----------element access------------------//

template<typename ElementType>
inline
ElementType* VectorVoxelWithVariableSize<ElementType>::GetElementPointer()
{
    return m_ElementData->data();
}


template<typename ElementType>
inline
const ElementType* VectorVoxelWithVariableSize<ElementType>::GetElementPointer() const
{
    return m_ElementData->data();
}



template<typename ElementType>
inline
ElementType& VectorVoxelWithVariableSize<ElementType>::operator[](int64 Index)
{
#if defined MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::operator[](Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& VectorVoxelWithVariableSize<ElementType>::operator[](int64 Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::operator[](Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithVariableSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
ElementType& VectorVoxelWithVariableSize<ElementType>::operator()(int64 Index)
{
#if defined MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::operator()(Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithVariableSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& VectorVoxelWithVariableSize<ElementType>::operator()(int64 Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound

    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::operator()(Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithVariableSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
ElementType& VectorVoxelWithVariableSize<ElementType>::at(int64 Index)
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& VectorVoxelWithVariableSize<ElementType>::at(int64 Index) const
{
    auto Length = this->GetLength();

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithVariableSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void VectorVoxelWithVariableSize<ElementType>::operator+=(const VectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = this->GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        (*this) += VoxelB[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize::operator+=(VoxelB)" << '\n';
    }

    for (int64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] += VoxelB[i];
    }
}


template<typename ElementType>
inline 
void VectorVoxelWithVariableSize<ElementType>::operator-=(const VectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = this->GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        (*this) -= VoxelB[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize::operator-=(VoxelB)" << '\n';
    }

    for (int64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] -= VoxelB[i];
    }
}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::operator*=(const VectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = this->GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
         (*this) *= VoxelB[0];
         return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize::operator*=(VoxelB)" << '\n';
    }

    for (int64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] *= VoxelB[i];
    }
}


template<typename ElementType>
inline
void VectorVoxelWithVariableSize<ElementType>::operator/=(const VectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = this->GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        (*this) /= VoxelB[0];
        return;
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize::operator/=(VoxelB)" << '\n';
    }

    for (int64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] /= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void VectorVoxelWithVariableSize<ElementType>::operator+=(const ElementType& Element)
{
    for (int64 i = 0; i < this->GetLength(); ++i)
    {
        m_ElementData[i] += Element;
    }
}


template<typename ElementType>
inline 
void VectorVoxelWithVariableSize<ElementType>::operator-=(const ElementType& Element)
{
    for (int64 i = 0; i < this->GetLength(); ++i)
    {
        m_ElementData[i] -= Element;
    }
}


template<typename ElementType>
inline void VectorVoxelWithVariableSize<ElementType>::operator*=(const ElementType& Element)
{
    for (int64 i = 0; i < this->GetLength(); ++i)
    {
        m_ElementData[i] *= Element;
    }
}


template<typename ElementType>
inline 
void VectorVoxelWithVariableSize<ElementType>::operator/=(const ElementType& Element)
{
    for (int64 i = 0; i < this->GetLength(); ++i)
    {
        m_ElementData[i] /= Element;
    }
}

}// namespace mdk

#endif