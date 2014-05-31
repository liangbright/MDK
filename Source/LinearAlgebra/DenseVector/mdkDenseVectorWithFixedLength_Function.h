#ifndef __mdkDenseVectorWithFixedLength_Function_h
#define __mdkDenseVectorWithFixedLength_Function_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, int_max Length>
class DenseVector;
//------------------------------------------------------

//---------------------------------------------- LinearCombine ------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVector<ElementType, Length>
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVector<ElementType, Length>*>& VectorSet);

}

#include "mdkDenseVectorWithFixedLength_Function.hpp"

#endif