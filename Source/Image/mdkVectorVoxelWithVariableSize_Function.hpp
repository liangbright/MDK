#ifndef __mdkVectorVoxelWithVariableSize_Function_hpp
#define __mdkVectorVoxelWithVariableSize_Function_hpp

//#include "mdkVectorVoxelWithVariableSize_Function.h"

namespace mdk
{

template<typename ElementType>
inline 
VectorVoxelWithVariableSize<ElementType> 
operator+(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize_Function operator+(VoxelA, VoxelB)" << '\n';

        VectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVoxel[i] = VoxelA[i] + VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator-(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize_Function operator-(VoxelA, VoxelB)" << '\n';
        VectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVoxel[i] = VoxelA[i] - VoxelB[i];
	}

	return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>
operator*(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize_Function operator*(VoxelA, VoxelB)" << '\n';
        VectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }


    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (int_max i = 0; i < Length_A; ++i)
    {
        tempVoxel[i] = VoxelA[i] * VoxelB[i];
    }

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType>
operator/(const VectorVoxelWithVariableSize<ElementType>& VoxelA, const VectorVoxelWithVariableSize<ElementType>& VoxelB)
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
        MDK_Error << "VoxelA.size() != VoxelB.size() @ VectorVoxelWithVariableSize_Function operator/(VoxelA, VoxelB)" << '\n';
        VectorVoxelWithVariableSize<ElementType> EmptyVoxel;
        return EmptyVoxel;
    }

    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length_A);

    for (int_max i = 0; i < Length_A; ++i)
    {
        tempVoxel[i] = VoxelA[i] / VoxelB[i];
    }

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
VectorVoxelWithVariableSize<ElementType> 
operator+(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Voxel[i] + Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator-(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Voxel[i] - Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator*(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Voxel[i] * Element;
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator/(const VectorVoxelWithVariableSize<ElementType>& Voxel, const ElementType& Element)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Voxel[i] / Element;
	}

    return tempVoxel;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator+(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Element + Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator-(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Element - Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator*(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Element * Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
operator/(const ElementType& Element, const VectorVoxelWithVariableSize<ElementType>& Voxel)
{
    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Voxel.GetLength());

	for (int_max i = 0; i < Voxel.GetLength(); ++i)
	{
		tempVoxel[i] = Element / Voxel[i];
	}

    return tempVoxel;
}


template<typename ElementType>
inline
VectorVoxelWithVariableSize<ElementType> 
VectorVoxelLinearCombine(const std<ElementType>& CoefList, const std::vector<const VectorVoxelWithVariableSize<ElementType>*>& VoxelList)
{
    VectorVoxelWithVariableSize<ElementType> EmptyVoxel; // empty Voxel

    if (CoefList.size() != VoxelList.size())
    {
        MDK_Error << "CoefList.size() != VoxelList.size() @ VectorVoxelWithVariableSize_Function VectorVoxelLinearCombine" << '\n';

        return EmptyVoxel;
    } 

    int_max VoxelNumber = int_max(VoxelList.size());

    auto Length = VoxelList[0]->GetLength();

    for (int_max k = 1; k < VoxelNumber; ++k)
    {
        if (VoxelList[k]->GetLength() != Length)
        {
            MDK_Error << "Size is not the same in VoxelList @ VectorVoxelWithVariableSize_Function VectorVoxelLinearCombine" << '\n';

            return EmptyVoxel;
        }
    }

    VectorVoxelWithVariableSize<ElementType> tempVoxel(ElementType(0), Length);

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

}// namespace mdk

#endif