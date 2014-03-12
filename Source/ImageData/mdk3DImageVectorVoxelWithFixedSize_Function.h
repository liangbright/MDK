#ifndef __mdk3DImageVectorVoxelWithFixedSize_Function_h
#define __mdk3DImageVectorVoxelWithFixedSize_Function_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, uint64 Length>
class mdk3DImageVectorVoxelWithFixedSize;
/------------------------------------------------------


//---------------------------------------------------- Voxel {+ - * /} Voxel ---------------------------------------------// 

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator+(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator-(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator*(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator/(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB);

//---------------------------------------------------- Voxel {+ - * /} Element ---------------------------------------------// 

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator+(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator-(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator*(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator/(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element);

//----------------------------------------------------  Element {+ - * /} Voxel ---------------------------------------------// 

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator+(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator-(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator*(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> operator/(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);


//---------------------------------------------- LinearCombine ------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>*>& VoxelPtrList);

}

#include "mdk3DImageVectorVoxelWithFixedSize_Function.hpp"

#endif