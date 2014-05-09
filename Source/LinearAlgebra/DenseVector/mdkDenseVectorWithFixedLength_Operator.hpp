#ifndef __mdkDenseVectorWithFixedLength_Operator_hpp
#define __mdkDenseVectorWithFixedLength_Operator_hpp


namespace mdk
{

template<typename ElementType, int_max Length>
inline 
DenseVectorWithFixedLength<ElementType, Length> 
operator+(const DenseVectorWithFixedLength<ElementType, Length>& VectorA, const DenseVectorWithFixedLength<ElementType, Length>& VectorB)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = VectorA[i] + VectorB[i];
	}

	return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator-(const DenseVectorWithFixedLength<ElementType, Length>& VectorA, const DenseVectorWithFixedLength<ElementType, Length>& VectorB)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = VectorA[i] - VectorB[i];
	}

	return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>
operator*(const DenseVectorWithFixedLength<ElementType, Length>& VectorA, const DenseVectorWithFixedLength<ElementType, Length>& VectorB)
{
    DenseVectorWithFixedLength<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVector[i] = VectorA[i] * VectorB[i];
    }

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length>
operator/(const DenseVectorWithFixedLength<ElementType, Length>& VectorA, const DenseVectorWithFixedLength<ElementType, Length>& VectorB)
{
    DenseVectorWithFixedLength<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
    {
        tempVector[i] = VectorA[i] / VectorB[i];
    }

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVectorWithFixedLength<ElementType, Length> 
operator+(const DenseVectorWithFixedLength<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] + Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator-(const DenseVectorWithFixedLength<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

    for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] - Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator*(const DenseVectorWithFixedLength<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] * Element;
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator/(const DenseVectorWithFixedLength<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Vector[i] / Element;
	}

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator+(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Voxel)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element + Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator-(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Vector)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element - Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator*(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Vector)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element * Vector[i];
	}

    return tempVector;
}


template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedLength<ElementType, Length> 
operator/(const ElementType& Element, const DenseVectorWithFixedLength<ElementType, Length>& Vector)
{
	DenseVectorWithFixedLength<ElementType, Length> tempVector;

	for (int_max i = 0; i < Length; ++i)
	{
		tempVector[i] = Element / Vector[i];
	}

    return tempVector;
}

}// namespace mdk

#endif