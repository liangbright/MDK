#ifndef __mdkType_h
#define __mdkType_h

#include <string>


#include "mdkOSPlatformConfig.h"
#include "mdkDebugConfig.h"


namespace mdk
{

#if defined(OS_Windows_x64)

    typedef signed char int8;

	typedef signed short int16;

	typedef signed int int32;

	typedef signed long long int64;

    typedef signed long long int_max;

	typedef unsigned char uint8;

	typedef unsigned short uint16;

	typedef unsigned int uint32;

	typedef unsigned long long uint64;

    typedef unsigned long long uint_max;

#endif


#if defined(OS_Windows_x64)
#ifdef _MSC_VER
    #define noexcept throw() 
#endif
#endif

enum struct ScalarTypeEnum
{
	DOUBLE64,
	FLOAT32,
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


enum struct ObjectConstructionTypeEnum
{
    Copy,
    Share,
};

//-------------------------------------------------------------//
template<typename ScalarType>
inline
ScalarTypeEnum FindScalarType(ScalarType Scalar);

template<typename ScalarType>
inline
std::string FindScalarTypeName(ScalarType Scalar);

template<typename ScalarType>
inline
int_max CalByteNumberOfScalar(const ScalarType& Scalar);


}//end namespace mdk

#include "mdkType.hpp"

#endif
