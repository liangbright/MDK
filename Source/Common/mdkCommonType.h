﻿#pragma once

#include <stdint.h>

#include "mdkOSPlatformConfig.h"

namespace mdk
{

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

}//end namespace mdk

