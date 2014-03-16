#ifndef __mdk3DImageVectorVoxelWithVariableSize_hpp
#define __mdk3DImageVectorVoxelWithVariableSize_hpp

#include "mdk3DImageVectorVoxelWithVariableSize.h"

namespace mdk
{
 
template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>::mdk3DImageVectorVoxelWithVariableSize()
{
    this->Reset();
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>::mdk3DImageVectorVoxelWithVariableSize(const ElementType& Element, uint64 Length = 1)
{
    m_ElementData.resize(Length);

    this->Fill(Element);
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>::mdk3DImageVectorVoxelWithVariableSize(mdk3DImageVectorVoxelWithVariableSize<ElementType>&& Voxel)
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>::~mdk3DImageVectorVoxelWithVariableSize()
{

}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::
operator=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);
{
    auto Length = m_ElementData.size();

    if (Length != Voxel.GetLength())
    {
        mdkError << "Size does not match @ mdk3DImageVectorVoxelWithVariableSize::operator=(& Voxel)" << '\n';
        return;
    }

    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Voxel[i];
    }
}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::
operator=(mdk3DImageVectorVoxelWithVariableSize<ElementType>&& Voxel);
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::Reset()
{
    m_ElementData.resize(0);

    m_ZeroElement = m_ZeroElement - m_ZeroElement;
}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::Fill(const ElementType& Element)
{
    auto Length = m_ElementData.size();

    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}
 

template<typename ElementType>
inline
uint64 mdk3DImageVectorVoxelWithVariableSize<ElementType>::GetLength()
{
    return m_ElementData.size();
}


//-----------element access------------------//

template<typename ElementType>
inline
ElementType* mdk3DImageVectorVoxelWithVariableSize<ElementType>::GetElementDataRawPointer()
{
    return m_ElementData->data();
}


template<typename ElementType>
inline
const ElementType* mdk3DImageVectorVoxelWithVariableSize<ElementType>::GetElementDataRawPointer() const
{
    return m_ElementData->data();
}



template<typename ElementType>
inline
ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator[](uint64 Index)
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::operator[](Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator[](uint64 Index) const
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::operator[](Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator()(uint64 Index)
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::operator()(Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator()(uint64 Index) const
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::operator()(Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType>
inline
ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::at(uint64 Index)
{
    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


template<typename ElementType>
inline
const ElementType& mdk3DImageVectorVoxelWithVariableSize<ElementType>::at(uint64 Index) const
{
    auto Length = m_ElementData.size();

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithVariableSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator+=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize::operator+=(VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] += VoxelB[i];
    }
}


template<typename ElementType>
inline 
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator-=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize::operator-=(VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] -= VoxelB[i];
    }
}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator*=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize::operator*=(VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] -= VoxelB[i];
    }
}


template<typename ElementType>
inline
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator/=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize::operator/=(VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < Length_A; ++i)
    {
        m_ElementData[i] -= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator+=(const ElementType& Element)
{
    for (uint64 i = 0; i < m_ElementData.size(); ++i)
    {
        m_ElementData[i] += Element;
    }
}


template<typename ElementType>
inline 
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator-=(const ElementType& Element)
{
    for (uint64 i = 0; i < m_ElementData.size(); ++i)
    {
        m_ElementData[i] -= Element;
    }
}


template<typename ElementType>
inline void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator*=(const ElementType& Element)
{
    for (uint64 i = 0; i < m_ElementData.size(); ++i)
    {
        m_ElementData[i] *= Element;
    }
}


template<typename ElementType>
inline 
void mdk3DImageVectorVoxelWithVariableSize<ElementType>::operator/=(const ElementType& Element)
{
    for (uint64 i = 0; i < m_ElementData.size(); ++i)
    {
        m_ElementData[i] /= Element;
    }
}

}// namespace mdk

#endif