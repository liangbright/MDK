#ifndef __mdkDenseVectorWithFixedLength_Operator_hpp
#define __mdkDenseVectorWithFixedLength_Operator_hpp

namespace mdk
{

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length> 
operator+(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = VectorA[i] + VectorB[i];
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator-(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = VectorA[i] - VectorB[i];
	}
	return OutputVector;
}

/*
template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>
operator*(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
    DenseVector<ElementType, Length> OutputVector;
    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }
    return OutputVector;
}
*/

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length>
operator/(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
	DenseVector<ElementType, Length> OutputVector;
    for (int_max i = 0; i < Length; ++i)
    {
		OutputVector[i] = VectorA[i] / VectorB[i];
    }
	return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length> operator+(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

    if (LengthB == 1)
    {
        return VectorA + VectorB[0];
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthB)
    {
        MDK_Error("Length != LengthB @ DenseVectorWithFixedLength_Operator +(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] + VectorB[i];
    }

    return OutputVector;
}


template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length> operator-(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

    if (LengthB == 1)
    {
        return VectorA - VectorB[0];
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthB)
    {
        MDK_Error("Length != LengthB @ DenseVectorWithFixedLength_Operator -(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] - VectorB[i];
    }

    return OutputVector;
}

/*
template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> operator*(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

    if (LengthB == 1)
    {
        return VectorA * VectorB[0];
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthB)
    {
        MDK_Error("Length != LengthB @ DenseVectorWithFixedLength_Operator *(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }

    return OutputVector;
}
*/

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> operator/(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

    if (LengthB == 1)
    {
        return VectorA / VectorB[0];
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthB)
    {
        MDK_Error("Length != LengthB @ DenseVectorWithFixedLength_Operator /(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] / VectorB[i];
    }

    return OutputVector;
}

//----------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] + VectorB;
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthA)
    {
        MDK_Error("Length != LengthA @ DenseVectorWithFixedLength_Operator +(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] + VectorB[i];
    }

    return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] - VectorB;
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthA)
    {
        MDK_Error("Length != LengthA @ DenseVectorWithFixedLength_Operator -(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] - VectorB[i];
    }

    return OutputVector;
}

/*
template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length> operator*(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] * VectorB;
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthA)
    {
        MDK_Error("Length != LengthA @ DenseVectorWithFixedLength_Operator *(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }

    return OutputVector;
}
*/

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> operator/(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] / VectorB;
    }

    DenseVector<ElementType, Length> OutputVector;

    if (Length != LengthA)
    {
        MDK_Error("Length != LengthA @ DenseVectorWithFixedLength_Operator /(VectorA, VectorB) ")
        return OutputVector;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] / VectorB[i];
    }

    return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length> 
operator+(const DenseVector<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVector<ElementType, Length> OutputVector;
    for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Vector[i] + Element;
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator-(const DenseVector<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVector<ElementType, Length> OutputVector;
    for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Vector[i] - Element;
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator*(const DenseVector<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Vector[i] * Element;
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator/(const DenseVector<ElementType, Length>& Vector, const ElementType& Element)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Vector[i] / Element;
	}
	return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator+(const ElementType& Element, const DenseVector<ElementType, Length>& Vector)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Element + Vector[i];
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator-(const ElementType& Element, const DenseVector<ElementType, Length>& Vector)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Element - Vector[i];
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator*(const ElementType& Element, const DenseVector<ElementType, Length>& Vector)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Element * Vector[i];
	}
	return OutputVector;
}


template<typename ElementType, int_max Length>
inline
DenseVector<ElementType, Length> 
operator/(const ElementType& Element, const DenseVector<ElementType, Length>& Vector)
{
	DenseVector<ElementType, Length> OutputVector;
	for (int_max i = 0; i < Length; ++i)
	{
		OutputVector[i] = Element / Vector[i];
	}
	return OutputVector;
}

}// namespace mdk

#endif