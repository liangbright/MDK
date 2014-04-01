#ifndef __mdkVectorVoxelWithFixedSize_Function_hpp
#define __mdkVectorVoxelWithFixedSize_Function_hpp

//#include "VectorVoxelWithFixedSize_Function.h"

namespace mdk
{

template<typename ElementType, int_max Length>
inline 
VectorVoxelWithFixedSize<ElementType, Length> 
operator+(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator-(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>
operator*(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVoxel[i] = VoxelA[i] * VoxelB[i];
    }

    return tempVoxel;
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length>
operator/(const VectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const VectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVoxel[i] = VoxelA[i] / VoxelB[i];
    }

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
VectorVoxelWithFixedSize<ElementType, Length> 
operator+(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Voxel[i] + Element;
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator-(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Voxel[i] - Element;
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator*(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Voxel[i] * Element;
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator/(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Voxel[i] / Element;
	}
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator+(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Element + Voxel[i];
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator-(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Element - Voxel[i];
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator*(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Element * Voxel[i];
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
operator/(const ElementType& Element, const VectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	VectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVoxel[i] = Element / Voxel[i];
	}
}


template<typename ElementType, int_max Length>
inline
VectorVoxelWithFixedSize<ElementType, Length> 
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const VectorVoxelWithFixedSize<ElementType, Length>*>& VoxelList)
{
    VectorVoxelWithFixedSize<ElementType, Length> tempVoxel; // has been Reset to zero

    int_max VoxelNumber = int_max(VoxelList.size());

    for (int_max k = 0; k < VoxelNumber; ++k)
    {
        auto Coef_k = CoefList[k];

        auto Voxel_k = VoxelList[k];

        for (int_max i = 0; i < Length; ++i)
        {            
            tempVoxel[i] + = Coef_k * Voxel_k[i];
        }
    }

    return tempVoxel;
}


}// namespace mkd

#endif