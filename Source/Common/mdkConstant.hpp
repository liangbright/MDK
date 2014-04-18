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

//----------------------------------------------------

template<typename ElementType>
inline
ElementType GetNaNElement()
{
    return GetNaNElement(ElementType(0));
}


template<typename ElementType>
inline
ElementType GetNaNElement(const ElementType& ReferenceElement)
{
    return ReferenceElement;
}


inline double GetNaNElement(const double&)
{
    return std::nan(nullptr);
}


inline float GetNaNElement(const float&)
{
    return std::nanf(nullptr);
}


inline char GetNaNElement(const char&)
{
    return char(0);
}


inline short GetNaNElement(const short&)
{
    return short(0);
}


inline int GetNaNElement(const int&)
{
    return int(0);
}


inline long GetNaNElement(const long&)
{
    return long(0);
}


inline long long GetNaNElement(const long long&)
{
    return long long(0);
}


inline unsigned char GetNaNElement(const unsigned char&)
{
    return unsigned char(0);
}


inline unsigned short GetNaNElement(const unsigned short&)
{
    return unsigned short(0);
}


inline unsigned int GetNaNElement(const unsigned int&)
{
    return unsigned int(0);
}


inline unsigned long GetNaNElement(const unsigned long&)
{
    return unsigned long(0);
}


inline unsigned long long GetNaNElement(const unsigned long long&)
{
    return unsigned long long(0);
}

}//namespace mdk

#endif
