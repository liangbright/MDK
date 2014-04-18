#ifndef __mdkDenseVectorWithVariableSize_Function_h
#define __mdkDenseVectorWithVariableSize_Function_h

#include <vector>

#include "mdkType.h"

namespace mdk
{
    
template<typename ElementType>
inline
DenseVectorWithVariableSize<ElementType>
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVectorWithVariableSize<ElementType>*>& VectorSet);

}

#include "mdkDenseVectorWithVariableSize_Function.hpp"

#endif