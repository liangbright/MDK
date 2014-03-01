#ifndef __mdkMatrixElement_h
#define __mdkMatrixElement_h

#include "mdkOSPlatformConfig.h"

namespace mdk
{

enum struct mdkMatrixElementTypeEnum
{
    Scalar_DOUBLE64,
    Scalar_FLOAT32,
    Scalar_INT8,
    Scalar_INT16,
    Scalar_INT32,
    Scalar_INT64,
    Scalar_UINT8,
    Scalar_UINT16,
    Scalar_UINT32,
    Scalar_UINT64,

    StdVector_DOUBLE64,
    StdVector_FLOAT32,
    StdVector_INT8,
    StdVector_INT16,
    StdVector_INT32,
    StdVector_INT64,
    StdVector_UINT8,
    StdVector_UINT16,
    StdVector_UINT32,
    StdVector_UINT64,

    StdArray_DOUBLE64,
    StdArray_FLOAT32,
    StdArray_INT8,
    StdArray_INT16,
    StdArray_INT32,
    StdArray_INT64,
    StdArray_UINT8,
    StdArray_UINT16,
    StdArray_UINT32,
    StdArray_UINT64,

    UNKNOWN,
};

//-------------------------------------------------------------//
template<typename ElementType>
inline
mdkMatrixElementTypeEnum FindMatrixElementType(ElementType Element);


}//end namespace mdk

#include "mdkMatrixElement.hpp"

#endif