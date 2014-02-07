#ifndef __mdk3DImageVoxel_h
#define __mdk3DImageVoxel_h

#include <array>

#include "mdkType.h"

namespace mdk
{

// use std::array<double, Length>  as vector voxel

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(std::array<ElementType, Length>& VoxelA, std::array<ElementType, Length>& VoxelB);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(std::array<ElementType, Length>& VoxelA, std::array<ElementType, Length>& VoxelB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(std::array<ElementType, Length>& Voxel, ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(std::array<ElementType, Length>& Voxel, ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator*(std::array<ElementType, Length>& Voxel, ElementType& Element);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator/(std::array<ElementType, Length>& Voxel, ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator+(ElementType& Element, std::array<ElementType, Length>& Voxel, );

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator-(ElementType& Element, std::array<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator*(ElementType& Element, std::array<ElementType, Length>& Voxel);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> operator/(ElementType& Element, std::array<ElementType, Length>& Voxel);

}