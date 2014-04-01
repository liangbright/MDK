#ifndef __mdkVectorVoxelWithFixedSize_hpp
#define __mdkVectorVoxelWithFixedSize_hpp

//#include "mdkVectorVoxelWithFixedSize.h"

namespace mdk
{
 
template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize()
{
    this->Clear();
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::VectorVoxelWithFixedSize(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel)
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>::~VectorVoxelWithFixedSize()
{

}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::
operator=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Voxel[i];
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::
operator=(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel);
{
    m_ElementData = std::move(Voxel.m_ElementData);
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::Clear()
{
    m_ZeroElement = m_ZeroElement - m_ZeroElement;

    this->Fill(m_ZeroElement);
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::Fill(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] = Element;
    }
}
 

template<typename ElementType, int_max Length>
inline
int_max VectorVoxelWithFixedSize<ElementType, Length>::GetLength()
{
    return Length;
}


//-----------element access------------------//

template<typename ElementType, int_max Length>
inline
ElementType* VectorVoxelWithFixedSize<ElementType, Length>::GetElementPointer()
{
    return m_ElementData->data();
}


template<typename ElementType, int_max Length>
inline
const ElementType* VectorVoxelWithFixedSize<ElementType, Length>::GetElementPointer() const
{
    return m_ElementData->data();
}



template<typename ElementType, int_max Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator[](int_max Index)
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::operator[](Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator[](int_max Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::operator[](Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator()(int_max Index)
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::operator()(Index)" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::operator()(int_max Index) const
{
#if defined MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::operator()(Index) const" << '\n';
        return m_ZeroElement;
    }

#endif // MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound    

    return m_ElementData[Index];
}


template<typename ElementType, int_max Length>
inline
ElementType& VectorVoxelWithFixedSize<ElementType, Length>::at(int_max Index)
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


template<typename ElementType, int_max Length>
inline
const ElementType& VectorVoxelWithFixedSize<ElementType, Length>::at(int_max Index) const
{
    if (Index >= Length || Index < 0)
    {
        MDK_Error << "Invalid input @ VectorVoxelWithFixedSize::at(Index)" << '\n';
        return m_ZeroElement;
    }

    return m_ElementData[Index];
}


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] += Voxel[i];
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator-=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] -= Voxel[i];
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator*=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] *= Voxel[i];
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator/=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] /= Voxel[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator+=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] += Element;
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator-=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] -= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator*=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] *= Element;
    }
}


template<typename ElementType, int_max Length>
inline
void VectorVoxelWithFixedSize<ElementType, Length>::operator/=(const ElementType& Element)
{
    for (int_max i = 0; i < Length; ++i)
    {
        m_ElementData[i] /= Element;
    }
}

}// namespace mdk


#endif