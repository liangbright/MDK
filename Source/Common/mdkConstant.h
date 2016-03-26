#pragma once

#include <limits>

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
ElementType GetNaNElement();

template<typename ElementType>
inline
ElementType GetNaNElement(const ElementType& ReferenceElement);

}//namespace mdk

#include "mdkConstant.hpp"

