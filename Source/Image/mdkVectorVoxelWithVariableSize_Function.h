#ifndef __mdkVectorVoxelWithVariableSize_Function_h
#define __mdkVectorVoxelWithVariableSize_Function_h

#include <vector>

#include "mdkType.h"

namespace mdk
{
    
// second choice: use VectorVoxelWithVariableSize<double/float/etc>  as a variable length vector voxel

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator+(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator-(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator+(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator-(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator*(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator/(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator+(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator-(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator*(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel);

template<typename ElementType>
inline VectorVoxelWithVariableSize<ElementType> operator/(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const VectorVoxelWithVariableSize<ElementType>*>& VoxelList);

}

#include "mdkVectorVoxelWithVariableSize_Function.hpp"

#endif