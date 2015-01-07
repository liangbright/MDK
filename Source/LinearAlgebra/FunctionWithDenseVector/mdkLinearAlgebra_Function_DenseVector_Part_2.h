#ifndef __mdkLinearAlgebra_Function_DenseVector_Part_2_h
#define __mdkLinearAlgebra_Function_DenseVector_Part_2_h

#include "mdkCommonType.h"
#include "mdkDenseVector_ForwardDeclare.h"

namespace mdk
{

// note:
// if Length is -1, then it is DenseVector with Variable Length
// else it is DenseVector with Fixed Length
// Therefore : template<typename ElementType, int_max Length> : do not use Length as vector Length

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeInnerProductOfTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL2DistanceBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL1DistanceBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeCorrelationBetweenTwoVector(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

}

#include "mdkLinearAlgebra_Function_DenseVector_Part_2.hpp"

#endif