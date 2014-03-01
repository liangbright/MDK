#ifndef __mdk3DImageVectorVoxelWithFixedLength_hpp
#define __mdk3DImageVectorVoxelWithFixedLength_hpp

#include "mdk3DImageVectorVoxel_std_array.h"

namespace mdk
{

template<typename ElementType, uint64 Length>
inline 
std::array<ElementType, Length> operator+(const std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB)
{
	std::array<ElementType, Length> tempVoxel;

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
	std::array<ElementType, Length> tempVoxel;

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

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
void operator+=(std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] += VoxelB[i];
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator-=(std::array<ElementType, Length>& VoxelA, const std::array<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
void operator+=(std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] += Element;
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator-=(std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] -= Element;
    }
}


template<typename ElementType, uint64 Length>
inline void operator*=(std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] *= Element;
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator/=(std::array<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] /= Element;
    }
}


}// namespace mkd

#endif