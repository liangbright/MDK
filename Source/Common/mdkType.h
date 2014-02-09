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

typedef enum
{
	MDK_DOUBLE64,
	MDK_FLOAT32,
	MDK_INT8,
	MDK_INT16,
	MDK_INT32,
	MDK_INT64,
	MDK_UINT8,
	MDK_UINT16,
	MDK_UINT32,
	MDK_UINT64,

	MDK_UNKNOWN,
} mdkScalarTypeEnum;

typedef mdkScalarTypeEnum mdkMatrixElementTypeEnum;

}//end namespace mdk

#endif