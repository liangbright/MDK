#ifndef mdk_LinearAlgebra_Function_DenseVector_Part_0_h
#define mdk_LinearAlgebra_Function_DenseVector_Part_0_h

namespace mdk
{

//---------------------------------------------- LinearCombine ------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length>
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVector<ElementType, Length>*>& VectorSet);

template<typename ElementType>
inline
DenseVector<ElementType>
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVector<ElementType>*>& VectorSet);

}

#include "mdkLinearAlgebra_Function_DenseVector_Part_0.hpp"

#endif