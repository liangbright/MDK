#pragma once


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

//-------------------- Vector/1-Element-Vector {+ - * /} Vector/1-Element-Vector -----------------------------//
// note: VectorA or VectorB are DenseVectorWithFixedLength with different length

template<typename ElementType, int_max LengthA, int_max LengthB>
inline
DenseVector<ElementType> 
operator+(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
	auto VectorLengthA = VectorA.GetLength();
	auto VectorLengthB = VectorB.GetLength();

	if (VectorLengthA == 1)
	{
		return VectorA[0] + VectorB;
	}
	else if (VectorLengthB == 1)
	{
		return VectorA + VectorB[0];
	}
	else if (VectorLengthA != VectorLengthB)
	{
		MDK_Error("VectorLengthA != VectorLengthB @ DenseVectorWithFixedLength_Operator operator+(VectorA, VectorB)");
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
	}
	// now: VectorLengthA == VectorLengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(VectorLengthA);
	for (int_max i = 0; i < VectorLengthA; ++i)
	{
		OutputVector[i] = VectorA[i] + VectorB[i];
	}
	return OutputVector;
}


template<typename ElementType, int_max LengthA, int_max LengthB>
inline
DenseVector<ElementType> 
operator-(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
	auto VectorLengthA = VectorA.GetLength();
	auto VectorLengthB = VectorB.GetLength();

	if (VectorLengthA == 1)
	{
		return VectorA[0] - VectorB;
	}
	else if (VectorLengthB == 1)
	{
		return VectorA - VectorB[0];
	}
	else if (VectorLengthA != VectorLengthB)
	{
		MDK_Error("VectorLengthA != VectorLengthB @ DenseVectorWithFixedLength_Operator operator-(VectorA, VectorB)");
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
	}
	// now: VectorLengthA == VectorLengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(VectorLengthA);
	for (int_max i = 0; i < VectorLengthA; ++i)
	{
		OutputVector[i] = VectorA[i] - VectorB[i];
	}
	return OutputVector;
}

/*
template<typename ElementType, int_max LengthA, int_max LengthB>
inline 
DenseVector<ElementType> 
operator*(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
	auto VectorLengthA = VectorA.GetLength();
	auto VectorLengthB = VectorB.GetLength();

	if (VectorLengthA == 1)
	{
	return VectorA[0] * VectorB;
	}
	else if (VectorLengthB == 1)
	{
	return VectorA * VectorB[0];
	}
	else if (VectorLengthA != VectorLengthB)
	{
	MDK_Error("VectorLengthA != VectorLengthB @ DenseVectorWithFixedLength_Operator operator*(VectorA, VectorB)");
	DenseVector<ElementType> EmptyVector;
	return EmptyVector;
	}
	// now: VectorLengthA == VectorLengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(VectorLengthA);
	for (int_max i = 0; i < VectorLengthA; ++i)
	{
	OutputVector[i] = VectorA[i] * VectorB[i];
	}
	return OutputVector;
}
*/

template<typename ElementType, int_max LengthA, int_max LengthB>
inline
DenseVector<ElementType>
operator/(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
	auto VectorLengthA = VectorA.GetLength();
	auto VectorLengthB = VectorB.GetLength();

	if (VectorLengthA == 1)
	{
		return VectorA[0] / VectorB;
	}
	else if (VectorLengthB == 1)
	{
		return VectorA / VectorB[0];
	}
	else if (VectorLengthA != VectorLengthB)
	{
		MDK_Error("VectorLengthA != VectorLengthB @ DenseVectorWithFixedLength_Operator operator/(VectorA, VectorB)");
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
	}
	// now: VectorLengthA == VectorLengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(VectorLengthA);
	for (int_max i = 0; i < VectorLengthA; ++i)
	{
		OutputVector[i] = VectorA[i] / VectorB[i];
	}
	return OutputVector;
}

//-----------------------------------------------  fixed {+ - * /} unfixed ----------------------------------------------------------------//
template<typename ElementType, int_max LengthA>
inline 
DenseVector<ElementType> operator+(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

	if (LengthA == 1)
	{
		return VectorA[0] + VectorB;
	}
    else if (LengthB == 1)
    {
        return VectorA + VectorB[0];
    }
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator +(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
	}
	// now: LengthA == LengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthB);
	for (int_max i = 0; i < LengthB; ++i)
    {
        OutputVector[i] = VectorA[i] + VectorB[i];
    }
    return OutputVector;
}


template<typename ElementType, int_max LengthA>
inline 
DenseVector<ElementType> operator-(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

	if (LengthA == 1)
	{
		return VectorA[0] - VectorB;
	}
    else if (LengthB == 1)
    {
        return VectorA - VectorB[0];
    }
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator -(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }
	// now: LengthA == LengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthB);
	for (int_max i = 0; i < LengthB; ++i)
    {
        OutputVector[i] = VectorA[i] - VectorB[i];
    }
    return OutputVector;
}

/*
template<typename ElementType, int_max LengthA>
inline
DenseVector<ElementType> operator*(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

	if (LengthA == 1)
	{
		return VectorA[0] * VectorB;
	}
    else if (LengthB == 1)
    {
        return VectorA * VectorB[0];
    }
    else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator *(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthA);
    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }
    return OutputVector;
}
*/

template<typename ElementType, int_max LengthA>
inline
DenseVector<ElementType> operator/(const DenseVector<ElementType, LengthA>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto LengthB = VectorB.GetLength();

	if (LengthA == 1)
	{
		return VectorA[0] / VectorB;
	}
    else if (LengthB == 1)
    {
        return VectorA / VectorB[0];
    }
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator /(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }
	// now: LengthA == LengthB
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthB);
	for (int_max i = 0; i < LengthB; ++i)
    {
        OutputVector[i] = VectorA[i] / VectorB[i];
    }
    return OutputVector;
}

//-------------------------------------------------- unfixed {+ - * /} fixed --------------------------------------------------------------//

template<typename ElementType, int_max LengthB>
inline
DenseVector<ElementType> operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] + VectorB;
    }
	else if (LengthB == 1)
	{
		return VectorA + VectorB[0];
	}
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator +(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthA);
    for (int_max i = 0; i < LengthA; ++i)
    {
        OutputVector[i] = VectorA[i] + VectorB[i];
    }
    return OutputVector;
}


template<typename ElementType, int_max LengthB>
inline
DenseVector<ElementType> operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] - VectorB;
    }
	else if (LengthB == 1)
	{
		return VectorA - VectorB[0];
	}
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator -(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthA);
    for (int_max i = 0; i < LengthA; ++i)
    {
        OutputVector[i] = VectorA[i] - VectorB[i];
    }
    return OutputVector;
}

/*
template<typename ElementType, int_max LengthB>
inline 
DenseVector<ElementType> operator*(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] * VectorB;
    }
	else if (LengthB == 1)
	{
		return VectorA * VectorB[0];
	}
    else if (Length != LengthA)
    {
        MDK_Error("Length != LengthA @ DenseVectorWithFixedLength_Operator *(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Length);
    for (int_max i = 0; i < Length; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }
    return OutputVector;
}
*/

template<typename ElementType, int_max LengthB>
inline
DenseVector<ElementType> operator/(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, LengthB>& VectorB)
{
    auto LengthA = VectorA.GetLength();

    if (LengthA == 1)
    {
        return VectorA[0] / VectorB;
    }
	else if (LengthB == 1)
	{
		return VectorA / VectorB[0];
	}
	else if (LengthA != LengthB)
    {
        MDK_Error("LengthA != LengthB @ DenseVectorWithFixedLength_Operator /(VectorA, VectorB) ")
		DenseVector<ElementType> EmptyVector;
		return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(LengthA);
    for (int_max i = 0; i < LengthA; ++i)
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
