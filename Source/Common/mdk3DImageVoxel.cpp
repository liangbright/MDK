#include "mdk3DImageVoxel.h"

template<typename ElementType, uint64 Length>
inline 
std::array<ElementType, Length> operator+(const std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB)
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
std::array<ElementType, Length> operator-(const std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB)
{
	std::array<ElementType, Length> tempVoxel(Length);

	for (uint64 i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
std::array<ElementType, Length> operator+(const std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Voxel[i] + Element;
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator-(const std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Voxel[i] - Element;
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator*(const std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Voxel[i] * Element;
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator/(const std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Voxel[i] / Element;
	}
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator+(const ElementType& Element, const std::array<ElementType, Length>& Voxel)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Element + Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator-(const ElementType& Element, const std::array<ElementType, Length>& Voxel)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Element - Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator*(const ElementType& Element, const std::array<ElementType, Length>& Voxel)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Element * Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> operator/(const ElementType& Element, const std::array<ElementType, Length>& Voxel)
{
	std::array<ElementType, Length> tempArray;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempArray[i] = Element / Voxel[i];
	}
}