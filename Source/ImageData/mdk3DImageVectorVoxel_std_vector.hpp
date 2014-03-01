#ifndef __mdk3DImageVectorVoxel_std_vector_hpp
#define __mdk3DImageVectorVoxel_std_vector_hpp

#include "mdk3DImageVectorVoxel_std_vector.h"

namespace mdk
{

template<typename ElementType>
inline 
std::vector<ElementType> operator+(const std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB)
{
	std::vector<ElementType> tempVoxel;

    if (VoxelA.size() != VoxelB.size())
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxel_std_vector operator+(VoxelA, VoxelB)" << '\n';
        return tempVoxel;
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType>
inline
std::vector<ElementType> operator-(const std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB)
{
	std::vector<ElementType> tempVoxel;

    if (VoxelA.size() != VoxelB.size())
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxel_std_vector operator-(VoxelA, VoxelB)" << '\n';
        return tempVoxel;
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
std::vector<ElementType> operator+(const std::vector<ElementType>& Voxel, const ElementType& Element)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Voxel[i] + Element;
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator-(const std::vector<ElementType>& Voxel, const ElementType& Element)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Voxel[i] - Element;
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator*(const std::vector<ElementType>& Voxel, const ElementType& Element)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Voxel[i] * Element;
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator/(const std::vector<ElementType>& Voxel, const ElementType& Element)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Voxel[i] / Element;
	}
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
std::vector<ElementType> operator+(const ElementType& Element, const std::vector<ElementType>& Voxel)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Element + Voxel[i];
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator-(const ElementType& Element, const std::vector<ElementType>& Voxel)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Element - Voxel[i];
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator*(const ElementType& Element, const std::vector<ElementType>& Voxel)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Element * Voxel[i];
	}
}


template<typename ElementType>
inline
std::vector<ElementType> operator/(const ElementType& Element, const std::vector<ElementType>& Voxel)
{
	std::vector<ElementType> tempvector;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempvector[i] = Element / Voxel[i];
	}
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB)
{
    if (VoxelA.size() != VoxelB.size())
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxel_std_vector operator+=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] += VoxelB[i];
    }
}


template<typename ElementType>
inline 
void operator-=(std::vector<ElementType>& VoxelA, const std::vector<ElementType>& VoxelB)
{
    if (VoxelA.size() != VoxelB.size())
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxel_std_vector operator-=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(std::vector<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] += Element;
    }
}


template<typename ElementType>
inline 
void operator-=(std::vector<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] -= Element;
    }
}


template<typename ElementType>
inline void operator*=(std::vector<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] *= Element;
    }
}


template<typename ElementType>
inline 
void operator/=(std::vector<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] /= Element;
    }
}


}// namespace mkd

#endif