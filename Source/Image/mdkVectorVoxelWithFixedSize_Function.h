#ifndef __mdkVectorVoxelWithFixedSize_Function_h
#define __mdkVectorVoxelWithFixedSize_Function_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, int64 Length>
class VectorVoxelWithFixedSize;
/------------------------------------------------------


//---------------------------------------------------- Voxel {+ - * /} Voxel ---------------------------------------------// 

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator+(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator-(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator*(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator/(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

//---------------------------------------------------- Voxel {+ - * /} Element ---------------------------------------------// 

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator+(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator-(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator*(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator/(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

//----------------------------------------------------  Element {+ - * /} Voxel ---------------------------------------------// 

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator+(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator-(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator*(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, int64 Length>
inline VectorVoxelWithFixedSize<ElementType, Length> operator/(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);


//---------------------------------------------- LinearCombine ------------------------------------------------------------------//

template<typename ElementType, int64 Length>
inline 
VectorVoxelWithFixedSize<ElementType, Length> 
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const VectorVoxelWithFixedSize<ElementType, Length>*>& VoxelList);

}

#include "mdkVectorVoxelWithFixedSize_Function.hpp"

#endif