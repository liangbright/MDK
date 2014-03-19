#ifndef __mdkTypeTranslation_hpp
#define __mdkTypeTranslation_hpp

#include "mdkTypeToOpenCVTranslation.hpp"

namespace mdk
{

OpenCVSingleChannelMatrixElementTypeEnum GetOpenCVSingleChannelMatrixElementType(mdkScalarTypeEnum mdkScalarType)
{
    switch (mdkScalarType)
    {
    case mdkScalarTypeEnum::DOUBLE64 :
        return OpenCVSingleChannelMatrixElementTypeEnum::DOUBLE64;

    case mdkScalarTypeEnum::FLOAT32 :
        return OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32;

    case mdkScalarTypeEnum::INT8:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT8;

    case mdkScalarTypeEnum::INT16:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT16;

    case mdkScalarTypeEnum::INT32:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT32;

    case mdkScalarTypeEnum::UINT8:
        return OpenCVSingleChannelMatrixElementTypeEnum::UINT8;

    case mdkScalarTypeEnum::UINT16:
        return OpenCVSingleChannelMatrixElementTypeEnum::UINT16;

    default:
        return OpenCVSingleChannelMatrixElementTypeEnum::UNKNOWN;
    }
}


template<typename ScalarType>
OpenCVSingleChannelMatrixElementTypeEnum GetOpenCVSingleChannelMatrixElementType()
{
    ScalarType Scalar;

    Scalar = 0;

    auto mdkScalarType = FindScalarType(Scalar);

    return GetOpenCVSingleChannelMatrixElementType(mdkScalarType);
}

}//end namespace mdk


#endif
