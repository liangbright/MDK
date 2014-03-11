#ifndef __mdk3DImageVectorVoxelWithVariableSize_Function_hpp
#define __mdk3DImageVectorVoxelWithVariableSize_Function_hpp

#include "mdk3DImageVectorVoxelWithVariableSize_Function.h"

namespace mdk
{

template<typename ElementType>
inline 
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator+(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_A == 1)
    {
        return VoxelA[0] + VoxelB;
    }

    if (Length_B == 1)
    {
        return VoxelA + VoxelB[0];
    }

    if (Length_A != Length_B)
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator+(VoxelA, VoxelB)" << '\n';

        mdk3DImageVectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (uint64 i = 0; i < Length_A; ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator-(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_A == 1)
    {
        return VoxelA[0] - VoxelB;
    }

    if (Length_B == 1)
    {
        return VoxelA - VoxelB[0];
    }

    if (Length_A != Length_B)
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator-(VoxelA, VoxelB)" << '\n';
        mdk3DImageVectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (uint64 i = 0; i < Length_A; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>
operator*(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_A == 1)
    {
        return VoxelA[0] * VoxelB;
    }

    if (Length_B == 1)
    {
        return VoxelA * VoxelB[0];
    }

    if (Length_A != Length_B)
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator*(VoxelA, VoxelB)" << '\n';
        mdk3DImageVectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }


    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (uint64 i = 0; i < Length_A; ++i)
    {
        tempVoxel[i] = VoxelA[i] * VoxelB[i];
    }

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType>
operator/(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_A == 1)
    {
        return VoxelA[0] / VoxelB;
    }

    if (Length_B == 1)
    {
        return VoxelA / VoxelB[0];
    }

    if (Length_A != Length_B)
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator/(VoxelA, VoxelB)" << '\n';
        mdk3DImageVectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (uint64 i = 0; i < Length_A; ++i)
    {
        tempVoxel[i] = VoxelA[i] / VoxelB[i];
    }

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator+(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] + Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator-(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] - Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator*(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] * Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator/(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Voxel[i] / Element;
	}

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator+(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element + Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator-(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element - Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator*(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element * Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
operator/(const ElementType& Element, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel)
{
    mdk3DImageVectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.size());

	for (uint64 i = 0; i < Voxel.size(); ++i)
	{
		tempVoxel[i] = Element / Voxel[i];
	}

    return tempVoxel;
}

// ------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        return VoxelA + VoxelB[0];
    }

    if (Length_A != Length_B)
    {
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator+=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] += VoxelB[i];
    }
}


template<typename ElementType>
inline 
void operator-=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        return VoxelA - VoxelB[0];
    }

    if (Length_A != Length_B)
    {
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator-=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}


template<typename ElementType>
inline
void operator*=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        return VoxelA *= VoxelB[0];
    }

    if (Length_A != Length_B)
    {
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator*=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}


template<typename ElementType>
inline
void operator/=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelA, const mdk3DImageVectorVoxelWithVariableSize<ElementType>& VoxelB)
{
    auto Length_A = VoxelA.GetLength();

    auto Length_B = VoxelB.GetLength();

    if (Length_B == 1)
    {
        return VoxelA /= VoxelB[0];
    }

    if (Length_A != Length_B)
    {
    {
        mdkError << "VoxelA.size() != VoxelB.size() @ mdk3DImageVectorVoxelWithVariableSize_Function operator/=(VoxelA, VoxelB)" << '\n';
    }

    for (uint64 i = 0; i < VoxelA.size(); ++i)
    {
        VoxelA[i] -= VoxelB[i];
    }
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] += Element;
    }
}


template<typename ElementType>
inline 
void operator-=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] -= Element;
    }
}


template<typename ElementType>
inline void operator*=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] *= Element;
    }
}


template<typename ElementType>
inline 
void operator/=(mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    for (uint64 i = 0; i < Voxel.size(); ++i)
    {
        Voxel[i] /= Element;
    }
}


template<typename ElementType>
inline
mdk3DImageVectorVoxelWithVariableSize<ElementType> 
VectorVoxelLinearCombine(const std<ElementType>& CoefList, const std::vector<mdk3DImageVectorVoxelWithVariableSize<ElementType>*>& VoxelPtrList)
{
    mdk3DImageVectorVoxelWithFixedSize<ElementType> EmptyVoxel; // empty Voxel

    uint64 VoxelNumber = VoxelPtrList.size();

    if (CoefList.size() != VoxelNumber)
    {
        mdkError << "CoefList.size() != VoxelNumber @ mdk3DImageVectorVoxelWithVariableSize_Function VectorVoxelLinearCombine" << '\n';

        return EmptyVoxel;
    } 

    auto Length = VoxelPtrList[0]->GetLength();

    for (uint64 k = 1; k < VoxelNumber; ++k)
    {
        if (VoxelPtrList[k]->GetLength() != Length)
        {
            mdkError << "Size is not the same in VoxelPtrList @ mdk3DImageVectorVoxelWithVariableSize_Function VectorVoxelLinearCombine" << '\n';

            return EmptyVoxel;
        }
    }

    mdk3DImageVectorVoxelWithFixedSize<ElementType> tempVoxel(ElementType(0), Length);

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

}// namespace mdk

#endif