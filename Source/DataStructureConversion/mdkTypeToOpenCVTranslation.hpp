#ifndef mdk_TypeTranslation_hpp
#define mdk_TypeTranslation_hpp

namespace mdk
{

OpenCVSingleChannelMatrixElementTypeEnum GetOpenCVSingleChannelMatrixElementType(ScalarTypeEnum mdkScalarType)
{
    switch (mdkScalarType)
    {
    case ScalarTypeEnum::DOUBLE :
        return OpenCVSingleChannelMatrixElementTypeEnum::DOUBLE;

    case ScalarTypeEnum::FLOAT :
        return OpenCVSingleChannelMatrixElementTypeEnum::FLOAT;

    case ScalarTypeEnum::INT8:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT8;

    case ScalarTypeEnum::INT16:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT16;

    case ScalarTypeEnum::INT32:
        return OpenCVSingleChannelMatrixElementTypeEnum::INT32;

    case ScalarTypeEnum::UINT8:
        return OpenCVSingleChannelMatrixElementTypeEnum::UINT8;

    case ScalarTypeEnum::UINT16:
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
