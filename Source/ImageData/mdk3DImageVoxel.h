#ifndef __mdk3DImageVoxel_h
#define __mdk3DImageVoxel_h

#include <array>

#include "mdkType.h"

namespace mdk
{

// use std::array<double/float/etc, uint64 Length>  as a vector voxel

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(const std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(const std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(const std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(const std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator*(const std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator/(const std::array<ElementType, Length>& Voxel, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(const ElementType& Element, const std::array<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(const ElementType& Element, const std::array<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator*(const ElementType& Element, const std::array<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator/(const ElementType& Element, const std::array<ElementType, Length>& Voxel);


// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline void operator+=(std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline void operator-=(std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline void operator+=(std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline void operator-=(std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline void operator*=(std::array<ElementType, Length>& Voxel, const ElementType& Element);

template<typename ElementType, uint64 Length>
inline void operator/=(std::array<ElementType, Length>& Voxel, const ElementType& Element);

}

#include "mdk3DImageVoxel.hpp"

#endif