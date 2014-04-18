#ifndef __mdkDenseVectorWithFixedSize_Operator_hpp
#define __mdkDenseVectorWithFixedSize_Operator_hpp


namespace mdk
{

template<typename ElementType, int_max Length>
inline 
DenseVectorWithFixedSize<ElementType, Length> 
operator+(const DenseVectorWithFixedSize<ElementType, Length>& VectorA, const DenseVectorWithFixedSize<ElementType, Length>& VectorB)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = VectorA[i] + VectorB[i];
	}

	return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator-(const DenseVectorWithFixedSize<ElementType, Length>& VectorA, const DenseVectorWithFixedSize<ElementType, Length>& VectorB)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = VectorA[i] - VectorB[i];
	}

	return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>
operator*(const DenseVectorWithFixedSize<ElementType, Length>& VectorA, const DenseVectorWithFixedSize<ElementType, Length>& VectorB)
{
    DenseVectorWithFixedSize<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVector[i] = VectorA[i] * VectorB[i];
    }

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length>
operator/(const DenseVectorWithFixedSize<ElementType, Length>& VectorA, const DenseVectorWithFixedSize<ElementType, Length>& VectorB)
{
    DenseVectorWithFixedSize<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVector[i] = VectorA[i] / VectorB[i];
    }

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVectorWithFixedSize<ElementType, Length> 
operator+(const DenseVectorWithFixedSize<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] + Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator-(const DenseVectorWithFixedSize<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] - Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator*(const DenseVectorWithFixedSize<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] * Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator/(const DenseVectorWithFixedSize<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] / Element;
	}

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator+(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Voxel)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element + Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator-(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Vector)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element - Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator*(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Vector)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element * Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
operator/(const ElementType& Element, const DenseVectorWithFixedSize<ElementType, Length>& Vector)
{
	DenseVectorWithFixedSize<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element / Vector[i];
	}

    return tempVector;
}

}// namespace mdk

#endif