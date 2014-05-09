#ifndef __mdkDenseVectorWithFixedSize_Function_h
#define __mdkDenseVectorWithFixedSize_Function_h

#include <array>

#include "mdkType.h"

namespace mdk
{
    
// forward declare----------------------------------
template<typename ElementType, int_max Length>
class DenseVectorWithFixedSize;
/------------------------------------------------------

//---------------------------------------------- LinearCombine ------------------------------------------------------------------//

template<typename ElementType, int_max Length>
inline 
DenseVectorWithFixedSize<ElementType, Length> 
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVectorWithFixedSize<ElementType, Length>*>& VectorSet);

}

#include "mdkDenseVectorWithFixedSize_Function.hpp"

#endif