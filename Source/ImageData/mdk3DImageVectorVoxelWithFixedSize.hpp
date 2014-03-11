#ifndef __mdk3DImageVectorVoxelWithFixedSize_hpp
#define __mdk3DImageVectorVoxelWithFixedSize_hpp

#include "mdk3DImageVectorVoxelWithFixedSize.h"

namespace mdk
{
 
template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::mdk3DImageVectorVoxelWithFixedSize()
{
    this->Reset();
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::mdk3DImageVectorVoxelWithFixedSize(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::mdk3DImageVectorVoxelWithFixedSize(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>&& Voxel)
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::~mdk3DImageVectorVoxelWithFixedSize()
{

}


template<typename ElementType, uint64 Length>
inline
void mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::
operator=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Voxel[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::
operator=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>&& Voxel);
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, uint64 Length>
inline
void mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::Reset()
{
    this->Fill(ElementType(0));

    m_ZeroElement = m_ZeroElement - m_ZeroElement;
}


template<typename ElementType, uint64 Length>
inline
void mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::Fill(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}
 

template<typename ElementType, uint64 Length>
inline
uint64 mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::GetLength()
{
    return Length;
}


//-----------element access------------------//

template<typename ElementType, uint64 Length>
inline
ElementType* mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::GetElementDataRawPointer()
{
    return m_ElementData->data();
}


template<typename ElementType, uint64 Length>
inline
const ElementType* mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::GetElementDataRawPointer() const
{
    return m_ElementData->data();
}



template<typename ElementType, uint64 Length>
inline
ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::operator[](uint64 Index)
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::operator[](Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::operator[](uint64 Index) const
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::operator[](Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::operator()(uint64 Index)
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::operator()(Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::operator()(uint64 Index) const
{
#if defined MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::operator()(Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_3DImageVectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::at(uint64 Index)
{
    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>::at(uint64 Index) const
{
    if (Index >= Length)
    {
        mdkError << "Index >= Length @ mdk3DImageVectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


}// namespace mdk

#include "mdk3DImageVectorVoxel.hpp"

#endif