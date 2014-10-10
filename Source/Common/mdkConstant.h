#ifndef __mdkConstant_h
#define __mdkConstant_h

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

#endif
