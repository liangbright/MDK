#ifndef __mdk3DImageVectorVoxel_std_vector_h
#define __mdk3DImageVectorVoxel_std_vector_h

#include <vector>

#include "mdkType.h"

namespace mdk
{
    
// second choice: use std::vector<double/float/etc>  as a variable length vector voxel

template<typename ElementType>
inline std::vector<ElementType> operator+(const std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB);

template<typename ElementType>
inline std::vector<ElementType> operator-(const std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline std::vector<ElementType> operator+(const std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline std::vector<ElementType> operator-(const std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline std::vector<ElementType> operator*(const std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline std::vector<ElementType> operator/(const std::vector<ElementType>& Voxel, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline std::vector<ElementType> operator+(const ElementType& Element, const std::vector<ElementType>& Voxel);

template<typename ElementType>
inline std::vector<ElementType> operator-(const ElementType& Element, const std::vector<ElementType>& Voxel);

template<typename ElementType>
inline std::vector<ElementType> operator*(const ElementType& Element, const std::vector<ElementType>& Voxel);

template<typename ElementType>
inline std::vector<ElementType> operator/(const ElementType& Element, const std::vector<ElementType>& Voxel);


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline void operator+=(std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB);

template<typename ElementType>
inline void operator-=(std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline void operator+=(std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline void operator-=(std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline void operator*=(std::vector<ElementType>& Voxel, const ElementType& Element);

template<typename ElementType>
inline void operator/=(std::vector<ElementType>& Voxel, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

}

#include "mdk3DImageVectorVoxel_std_vector.hpp"

#endif