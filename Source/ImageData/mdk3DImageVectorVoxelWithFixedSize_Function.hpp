#ifndef __mdk3DImageVectorVoxelWithFixedSize_Function_hpp
#define __mdk3DImageVectorVoxelWithFixedSize_Function_hpp

namespace mdk
{

template<typename ElementType, uint64 Length>
inline 
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator+(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator-(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Length; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>
operator*(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (uint64 i = 0; i < Length; ++i)
    {
        tempVoxel[i] = VoxelA[i] * VoxelB[i];
    }

    return tempVoxel;
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>
operator/(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

    for (uint64 i = 0; i < Length; ++i)
    {
        tempVoxel[i] = VoxelA[i] / VoxelB[i];
    }

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator+(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] + Element;
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator-(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] - Element;
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator*(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] * Element;
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator/(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] / Element;
	}
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator+(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element + Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator-(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element - Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator*(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element * Voxel[i];
	}
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
operator/(const ElementType& Element, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel)
{
	mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel;

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element / Voxel[i];
	}
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
void operator+=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] += VoxelB[i];
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator-=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}


template<typename ElementType, uint64 Length>
inline
void operator*=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] *= VoxelB[i];
    }
}

template<typename ElementType, uint64 Length>
inline
void operator/=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelA, const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& VoxelB)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        VoxelA[i] /= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, uint64 Length>
inline 
void operator+=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] += Element;
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator-=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] -= Element;
    }
}


template<typename ElementType, uint64 Length>
inline void operator*=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] *= Element;
    }
}


template<typename ElementType, uint64 Length>
inline 
void operator/=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Length; ++i)
    {
        Voxel[i] /= Element;
    }
}


template<typename ElementType, uint64 Length>
inline
mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> 
VectorVoxelLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>*>& VoxelPtrList)
{
    mdk3DImageVectorVoxelWithFixedSize<ElementType, Length> tempVoxel; // has been Reset to zero

    uint64 VoxelNumber = VoxelPtrList.size();

    for (uint64 k = 0; k < VoxelNumber; ++k)
    {
        auto Coef_k = CoefList[k];

        auto VoxelPtr_k = VoxelPtrList[k];

        for (uint64 i = 0; i < Length; ++i)
        {            
            tempVoxel[i] + = Coef_k * VoxelPtr_k[i];
        }
    }

    return tempVoxel;
}


}// namespace mkd

#endif