#ifndef __mdkDenseVectorWithVariableLength_Operator_hpp
#define __mdkDenseVectorWithVariableLength_Operator_hpp


namespace mdk
{

template<typename ElementType>
inline 
DenseVectorWithVariableLength<ElementType> 
operator+(const DenseVectorWithVariableLength<ElementType>& VectorA, const DenseVectorWithVariableLength<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();

    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] + VectorB;
    }

    if (Length_B == 1)
    {
        return VectorA + VectorB[0];
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Function operator+(VectorA, VectorB)" << '\n';

        DenseVectorWithVariableLength<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVector[i] = VectorA[i] + VectorB[i];
	}

	return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator-(const DenseVectorWithVariableLength<ElementType>& VectorA, const DenseVectorWithVariableLength<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();

    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] - VectorB;
    }

    if (Length_B == 1)
    {
        return VectorA - VectorB[0];
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Function operator-(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableLength<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVector[i] = VectorA[i] - VectorB[i];
	}

	return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>
operator*(const DenseVectorWithVariableLength<ElementType>& VectorA, const DenseVectorWithVariableLength<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();

    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] * VectorB;
    }

    if (Length_B == 1)
    {
        return VectorA * VectorB[0];
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Function operator*(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableLength<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
    {
        tempVector[i] = VectorA[i] * VectorB[i];
    }

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>
operator/(const DenseVectorWithVariableLength<ElementType>& VectorA, const DenseVectorWithVariableLength<ElementType>& VectorB)
{
    auto Length_A = VectorA.GetLength();

    auto Length_B = VectorB.GetLength();

    if (Length_A == 1)
    {
        return VectorA[0] / VectorB;
    }

    if (Length_B == 1)
    {
        return VectorA / VectorB[0];
    }

    if (Length_A != Length_B)
    {
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableLength_Function operator/(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableLength<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
    {
        tempVector[i] = VectorA[i] / VectorB[i];
    }

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline 
DenseVectorWithVariableLength<ElementType> 
operator+(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] + Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator-(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] - Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator*(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] * Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator/(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] / Element;
	}

    return tempVector;
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator+(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element + Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator-(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element - Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator*(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element * Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType> 
operator/(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector)
{
    DenseVectorWithVariableLength<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element / Vector[i];
	}

    return tempVector;
}


}// namespace mdk

#endif