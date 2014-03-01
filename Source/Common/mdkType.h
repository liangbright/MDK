#ifndef __mdkType_h
#define __mdkType_h

#include "mdkOSPlatformConfig.h"

namespace mdk
{

#if defined(OS_Windows_x64)

    typedef signed char int8;

	typedef signed short int16;

	typedef signed int int32;

	typedef signed long long int64;

	typedef unsigned char uint8;

	typedef unsigned short uint16;

	typedef unsigned int uint32;

	typedef unsigned long long uint64;

#endif

enum struct mdkScalarTypeEnum
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

//-------------------------------------------------------------//
template<typename ScalarType>
inline
mdkScalarTypeEnum FindScalarType(ScalarType Scalar);

template<typename ScalarType>
inline
uint64 CalByteNumberOfScalar(ScalarType Scalar);

}//end namespace mdk

#include "mdkType.hpp"

#endif