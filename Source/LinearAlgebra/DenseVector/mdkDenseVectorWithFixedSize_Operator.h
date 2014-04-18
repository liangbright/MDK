#ifndef __mdkDenseVectorWithFixedSize_Operator_h
#define __mdkDenseVectorWithFixedSize_Operator_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, int_max Length>
class DenseVectorWithFixedSize;
/------------------------------------------------------


//---------------------------------------------------- Vector {+ - * /} Vector ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator+(const DenseVectorWithFixedSize<ElementType, Length>& VoxelA, const DenseVectorWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator-(const DenseVectorWithFixedSize<ElementType, Length>& VoxelA, const DenseVectorWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator*(const DenseVectorWithFixedSize<ElementType, Length>& VoxelA, const DenseVectorWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator/(const DenseVectorWithFixedSize<ElementType, Length>& VoxelA, const DenseVectorWithFixedSize<ElementType, Length>& VoxelB);

//---------------------------------------------------- Vector {+ - * /} Element ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator+(const DenseVectorWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator-(const DenseVectorWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator*(const DenseVectorWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator/(const DenseVectorWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

//----------------------------------------------------  Element {+ - * /} Vector ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator+(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator-(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator*(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedSize<ElementType, Length> operator/(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Voxel);


}

#include "mdkDenseVectorWithFixedSize_Operator.hpp"

#endif