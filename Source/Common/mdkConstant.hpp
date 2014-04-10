#ifndef __mdkConstant_hpp
#define __mdkConstant_hpp

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ScalarType>
inline
ScalarType EPS(const ScalarType& ReferenceScalar)
{
    MDK_Error("ScalarType is Not float or double @ mdkConstant EPS(ScalarType ReferenceScalar)")

    return ScalarType(1);
}


template<typename ScalarType>
inline
ScalarType EPS()
{
    auto Scalar = ScalarType(0);

    return EPS(Scalar);
}


inline double EPS(const double&)
{
    return 2.2204e-16;
}


inline float EPS(const float&)
{
    return float(1.1921e-07);
}


}//namespace mdk

#endif
