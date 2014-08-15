#ifndef __mdkLinearAlgebra_Function_DenseVector_Part_1_h
#define __mdkLinearAlgebra_Function_DenseVector_Part_1_h

#include <array>

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
// Therefore : template<typename ElementType, int_max L>
// do not use L as vector Length

template<typename ElementType, int_max L>
inline DenseVector<int_max, -1> FindUniqueElementInVector(const DenseVector<ElementType, L>& Vector);

//SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
template<typename ElementType, int_max L, typename SpecialCompareFunctionType>
inline DenseVector<int_max, -1> FindUniqueElementInVector(const DenseVector<ElementType, L>& Vector, SpecialCompareFunctionType SpecialCompareFunction);

}// namespace mdk

#include "mdkLinearAlgebra_Function_DenseVector_Part_1.hpp"

#endif