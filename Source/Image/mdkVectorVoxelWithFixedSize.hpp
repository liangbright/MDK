#ifndef __mdkVectorVoxelWithFixedSize_hpp
#define __mdkVectorVoxelWithFixedSize_hpp

//#include "mdkVectorVoxelWithFixedSize.h"

namespace mdk
{
 
template<typename ElementType, uint64 Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize()
{
    this->Clear();
}


template<typename ElementType, uint64 Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}


template<typename ElementType, uint64 Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel)
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, uint64 Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::~VectorVoxelWithFixedSize()
{

}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::
operator=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Voxel[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::
operator=(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel);
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::Clear()
{
    this->Fill(ElementType(0));

    m_ZeroElement = m_ZeroElement - m_ZeroElement;
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::Fill(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}
 

template<typename ElementType, uint64 Length>
inline
uint64 VectorVoxelWithFixedSize<ElementType, Length>::GetLength()
{
    return Length;
}


//-----------element access------------------//

template<typename ElementType, uint64 Length>
inline
ElementType* VectorVoxelWithFixedSize<ElementType, Length>::GetElementPointer()
{
    return m_ElementData->data();
}


template<typename ElementType, uint64 Length>
inline
const ElementType* VectorVoxelWithFixedSize<ElementType, Length>::GetElementPointer() const
{
    return m_ElementData->data();
}



template<typename ElementType, uint64 Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator[](uint64 Index)
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::operator[](Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator[](uint64 Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::operator[](Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator()(uint64 Index)
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::operator()(Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator()(uint64 Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::operator()(Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::at(uint64 Index)
{
    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


template<typename ElementType, uint64 Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::at(uint64 Index) const
{
    if (Index >= Length)
    {
        mdkError << "Index >= Length @ VectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] += Voxel[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator-=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] -= Voxel[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator*=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] *= Voxel[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator/=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] /= Voxel[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] += Element;
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] -= Element;
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] *= Element;
    }
}


template<typename ElementType, uint64 Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        m_ElementData[i] /= Element;
    }
}

}// namespace mdk


#endif