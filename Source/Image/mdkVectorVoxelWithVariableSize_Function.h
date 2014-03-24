#ifndef __mdk3DImageVectorVoxelWithVariableSize_Function_h
#define __mdk3DImageVectorVoxelWithVariableSize_Function_h

#include <vector>

#include "mdkType.h"

namespace mdk
{
    
// second choice: use mdk3DImageVectorVoxelWithVariableSize<double/float/etc>  as a variable length vector voxel

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator+(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator-(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator+(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator-(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator*(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator/(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator+(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator-(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator*(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline mdk3DImageVectorVoxelWithVariableSize<ElementType> operator/(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdk3DImageVectorVoxelWithVariableSize<ElementType>*>& VoxelPtrList);

}

#include "mdk3DImageVectorVoxelWithVariableSize_Function.hpp"

#endif