﻿#ifndef __mdkDenseVectorWithVariableLength_Function_h
#define __mdkDenseVectorWithVariableLength_Function_h

#include <vector>

#include "mdkType.h"

namespace mdk
{
    
template<typename ElementType>
inline
DenseVectorWithVariableLength<ElementType>
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVectorWithVariableLength<ElementType>*>& VectorSet);

}

#include "mdkDenseVectorWithVariableLength_Function.hpp"

#endif