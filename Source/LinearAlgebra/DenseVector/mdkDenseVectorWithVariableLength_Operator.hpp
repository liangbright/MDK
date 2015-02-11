#ifndef mdk_DenseVectorWithVariableLength_Operator_hpp
#define mdk_DenseVectorWithVariableLength_Operator_hpp

namespace mdk
{

template<typename ElementType>
inline 
DenseVector<ElementType> 
operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();
    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] + VectorB;
    }
    else if (Length_B == 1)
    {
        return VectorA + VectorB[0];
    }
	else if (Length_A != Length_B)
    {
        MDK_Error("VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Operator operator+(VectorA, VectorB)");
        DenseVector<ElementType> EmptyVector;
        return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Length_A);
    for (int_max i = 0; i < Length_A; ++i)
	{
		OutputVector[i] = VectorA[i] + VectorB[i];
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();
    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] - VectorB;
    }
	else if (Length_B == 1)
    {
        return VectorA - VectorB[0];
    }
	else if (Length_A != Length_B)
    {
        MDK_Error("VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Operator operator-(VectorA, VectorB)");
        DenseVector<ElementType> EmptyVector;
        return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Length_A);
    for (int_max i = 0; i < Length_A; ++i)
	{
		OutputVector[i] = VectorA[i] - VectorB[i];
	}
	return OutputVector;
}

/*
template<typename ElementType>
inline
DenseVector<ElementType>
operator*(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();
    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] * VectorB;
    }
	else if (Length_B == 1)
    {
        return VectorA * VectorB[0];
    }
	else if (Length_A != Length_B)
    {
        MDK_Error("VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Operator operator*(VectorA, VectorB)");
        DenseVector<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVector<ElementType> OutputVector;
    OutputVector.Resize(Length_A);
    for (int_max i = 0; i < Length_A; ++i)
    {
        OutputVector[i] = VectorA[i] * VectorB[i];
    }
    return OutputVector;
}
*/

template<typename ElementType>
inline
DenseVector<ElementType>
operator/(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();
    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] / VectorB;
    }
	else if (Length_B == 1)
    {
        return VectorA / VectorB[0];
    }
	else if (Length_A != Length_B)
    {
		MDK_Error("VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Operator operator/(VectorA, VectorB)");
        DenseVector<ElementType> EmptyVector;
        return EmptyVector;
    }

	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Length_A);
    for (int_max i = 0; i < Length_A; ++i)
    {
		OutputVector[i] = VectorA[i] / VectorB[i];
    }
	return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
DenseVector<ElementType> 
operator+(const DenseVector<ElementType>& Vector, const ElementType& Element)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Vector[i] + Element;
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator-(const DenseVector<ElementType>& Vector, const ElementType& Element)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Vector[i] - Element;
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator*(const DenseVector<ElementType>& Vector, const ElementType& Element)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Vector[i] * Element;
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator/(const DenseVector<ElementType>& Vector, const ElementType& Element)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Vector[i] / Element;
	}
	return OutputVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
DenseVector<ElementType> 
operator+(const ElementType& Element, const DenseVector<ElementType>& Vector)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Element + Vector[i];
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator-(const ElementType& Element, const DenseVector<ElementType>& Vector)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Element - Vector[i];
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator*(const ElementType& Element, const DenseVector<ElementType>& Vector)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Element * Vector[i];
	}
	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
operator/(const ElementType& Element, const DenseVector<ElementType>& Vector)
{
	DenseVector<ElementType> OutputVector;
	OutputVector.Resize(Vector.GetLength());
	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		OutputVector[i] = Element / Vector[i];
	}
	return OutputVector;
}


}// namespace mdk

#endif