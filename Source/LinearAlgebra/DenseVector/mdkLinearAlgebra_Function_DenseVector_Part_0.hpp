#ifndef __mdkLinearAlgebra_Function_DenseVector_Part_0_hpp
#define __mdkLinearAlgebra_Function_DenseVector_Part_0_hpp


namespace mdk
{
    
template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeInnerProductOfTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeInnerProductOfTwoVectors(...)")
        return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeInnerProductOfTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeL2DistanceBetweenTwoVectors(...)")
        return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeL2DistanceBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeL1DistanceBetweenTwoVectors(...)")
            return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeL1DistanceBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB)
{
    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseVector ComputeCorrelationBetweenTwoVectors(...)")
            return ElementType(0);
    }

    // this is defined in mdkLinearAlgebra_Function_DenseMatrix
    return ComputeCorrelationBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


}

#endif