#ifndef mdk_LinearAlgebra_Function_DenseVector_Part_2_hpp
#define mdk_LinearAlgebra_Function_DenseVector_Part_2_hpp


namespace mdk
{
    
template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeInnerProductOfTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeInnerProductOfTwoVector(...)")
        return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeInnerProductOfTwoVector(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL2DistanceBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeL2DistanceBetweenTwoVector(...)")
        return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeL2DistanceBetweenTwoVector(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL1DistanceBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeL1DistanceBetweenTwoVector(...)")
            return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeL1DistanceBetweenTwoVector(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeCorrelationBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeCorrelationBetweenTwoVector(...)")
        return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeCorrelationBetweenTwoVector(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


}

#endif