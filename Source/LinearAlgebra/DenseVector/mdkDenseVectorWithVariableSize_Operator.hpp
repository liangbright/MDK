#ifndef __mdkDenseVectorWithVariableSize_Operator_hpp
#define __mdkDenseVectorWithVariableSize_Operator_hpp


namespace mdk
{

template<typename ElementType>
inline 
DenseVectorWithVariableSize<ElementType> 
operator+(const DenseVectorWithVariableSize<ElementType>& VectorA, const DenseVectorWithVariableSize<ElementType>& VectorB)
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
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableSize_Function operator+(VectorA, VectorB)" << '\n';

        DenseVectorWithVariableSize<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVector[i] = VectorA[i] + VectorB[i];
	}

	return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator-(const DenseVectorWithVariableSize<ElementType>& VectorA, const DenseVectorWithVariableSize<ElementType>& VectorB)
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
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableSize_Function operator-(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableSize<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
	{
		tempVector[i] = VectorA[i] - VectorB[i];
	}

	return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>
operator*(const DenseVectorWithVariableSize<ElementType>& VectorA, const DenseVectorWithVariableSize<ElementType>& VectorB)
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
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableSize_Function operator*(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableSize<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Length_A);

    for (int_max i = 0; i < Length_A; ++i)
    {
        tempVector[i] = VectorA[i] * VectorB[i];
    }

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>
operator/(const DenseVectorWithVariableSize<ElementType>& VectorA, const DenseVectorWithVariableSize<ElementType>& VectorB)
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
        MDK_Error << "VectorA.size() != VectorB.size() @ DenseVectorWithVariableSize_Function operator/(VectorA, VectorB)" << '\n';
        DenseVectorWithVariableSize<ElementType> EmptyVector;
        return EmptyVector;
    }

    DenseVectorWithVariableSize<ElementType> tempVector;

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
DenseVectorWithVariableSize<ElementType> 
operator+(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] + Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator-(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] - Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator*(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Vector[i] * Element;
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator/(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

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
DenseVectorWithVariableSize<ElementType> 
operator+(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element + Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator-(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element - Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator*(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element * Vector[i];
	}

    return tempVector;
}


template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType> 
operator/(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector)
{
    DenseVectorWithVariableSize<ElementType> tempVector;

    tempVector.Resize(Vector.GetLength());

	for (int_max i = 0; i < Vector.GetLength(); ++i)
	{
		tempVector[i] = Element / Vector[i];
	}

    return tempVector;
}


}// namespace mdk

#endif