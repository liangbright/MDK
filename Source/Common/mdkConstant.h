#ifndef __mdkConstant_h
#define __mdkConstant_h

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ScalarType>
inline ScalarType EPS(const ScalarType& ReferenceScalar);

template<typename ScalarType>
inline ScalarType EPS();


}//namespace mdk

#include "mdkConstant.hpp"

#endif
