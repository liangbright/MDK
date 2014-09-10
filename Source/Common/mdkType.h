#ifndef __mdkType_h
#define __mdkType_h

#include <string>
#include <typeinfo>
#include <stdint.h>

#include "mdkOSPlatformConfig.h"
#include "mdkDebugConfig.h"

namespace mdk
{
//---------------------------------------------
#if defined(OS_Windows_x64)
  #ifdef _MSC_VER
    #define noexcept throw() 
  #endif
#endif
//-------------------------------------------------------------//
// use stdint.h
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef signed long long int_max;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef unsigned long long uint_max;
//----------------------------------
enum struct ScalarTypeEnum
{
	DOUBLE,
	FLOAT,
	INT8,
	INT16,
	INT32,
	INT64,
	UINT8,
	UINT16,
	UINT32,
	UINT64,

	UNKNOWN,
};
//-------------------------------------------------------------//
enum struct ObjectConstructionTypeEnum
{
    Copy,
    Share,
};
//-------------------------------------------------------------//
template<typename ScalarType>
inline
ScalarTypeEnum GetScalarType(ScalarType Scalar);

template<typename ScalarType>
inline
std::string GetScalarTypeName(ScalarType Scalar);

template<typename ScalarType>
inline
int_max GetByteNumberOfScalar(const ScalarType& Scalar);
//----------------------------------------------------------------//
template<typename TemplateClassType>
std::string GetTemplateClassName(const TemplateClassType& TemplateClassObject);

}//end namespace mdk

#include "mdkType.hpp"

#endif
