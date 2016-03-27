#pragma once

#include <array>

#include "mdkCommonType.h"
#include "mdkDenseVector_ForwardDeclare.h"

namespace mdk
{

// note:
// if Length is -1, then it is DenseVector with Variable Length
// else it is DenseVector with Fixed Length
// Therefore : template<typename ElementType, int_max L>
// do not use L as vector Length

template<typename ElementType, int_max L>
inline DenseVector<int_max> FindUniqueElementInVector(const DenseVector<ElementType, L>& InputVector);

//SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
template<typename ElementType, int_max L, typename SpecialCompareFunctionType>
inline DenseVector<int_max> FindUniqueElementInVector(const DenseVector<ElementType, L>& InputVector, SpecialCompareFunctionType SpecialCompareFunction);

template<typename ElementType, int_max A, int_max B>
DenseVector<ElementType> Intersect(const DenseVector<ElementType, A>& SetA, const DenseVector<ElementType, B>& SetB);

//in A, NOT in B
template<typename ElementType, int_max A, int_max B>
DenseVector<ElementType> SetDiff(const DenseVector<ElementType, A>& SetA, const DenseVector<ElementType, B>& SetB);

}// namespace mdk

#include "mdkLinearAlgebra_Function_DenseVector_Part_1.hpp"
