#ifndef __mdkDenseVectorWithFixedLength_Operator_h
#define __mdkDenseVectorWithFixedLength_Operator_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, int_max Length>
class DenseVectorWithFixedLength;
/------------------------------------------------------


//---------------------------------------------------- Vector {+ - * /} Vector ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator+(const DenseVectorWithFixedLength<ElementType, Length>& VoxelA, 
                                                                 const DenseVectorWithFixedLength<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator-(const DenseVectorWithFixedLength<ElementType, Length>& VoxelA, 
                                                                 const DenseVectorWithFixedLength<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator*(const DenseVectorWithFixedLength<ElementType, Length>& VoxelA, 
                                                                 const DenseVectorWithFixedLength<ElementType, Length>& VoxelB);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator/(const DenseVectorWithFixedLength<ElementType, Length>& VoxelA, 
                                                                 const DenseVectorWithFixedLength<ElementType, Length>& VoxelB);

//---------------------------------------------------- Vector {+ - * /} Element ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator+(const DenseVectorWithFixedLength<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator-(const DenseVectorWithFixedLength<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator*(const DenseVectorWithFixedLength<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator/(const DenseVectorWithFixedLength<ElementType, Length>& Voxel, const ElementType& Element);

//----------------------------------------------------  Element {+ - * /} Vector ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator+(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator-(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator*(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Voxel);

template<typename ElementType, int_max Length>
inline DenseVectorWithFixedLength<ElementType, Length> operator/(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Voxel);


}

#include "mdkDenseVectorWithFixedLength_Operator.hpp"

#endif