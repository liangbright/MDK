#ifndef __mdkLinearAlgebra_Function_DenseVector_Part_0_h
#define __mdkLinearAlgebra_Function_DenseVector_Part_0_h

#include "mdkType.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType, int_max Length>
class DenseVector;
//-------------------------------------//

// note:
// if Length is -1, then it is DenseVector with Variable Length
// else it is DenseVector with Fixed Length
// Therefore : template<typename ElementType, int_max L> : do not use L as vector Length

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeInnerProductOfTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

template<typename ElementType, int_max A, int_max B>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const DenseVector<ElementType, A>& VectorA, const DenseVector<ElementType, B>& VectorB);

}

#include "mdkLinearAlgebra_Function_DenseVector_Part_0.hpp"

#endif