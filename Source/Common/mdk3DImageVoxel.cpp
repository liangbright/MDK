#include "mdk3DImageVoxel.h"

template<typename ElementType, uint64 Length>
inline 
std::array<ElementType, Length> operator+(std::array<ElementType, Length>& VoxelA, std::array<ElementType, Length>& VoxelB)
{
	std::array<ElementType, Length> tempVoxel(Length);

	for (uint64 i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator-(std::array<ElementType, Length>& VoxelA, std::array<ElementType, Length>& VoxelB)
{
	std::array<ElementType, Length> tempVoxel(Length);

	for (uint64 i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}