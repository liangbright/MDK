#ifndef __mdkMatrixElement_hpp
#define __mdkMatrixElement_hpp

#include "mdkMatrixElement.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkMatrixElementTypeEnum FindMatrixElementType(ElementType Element)
{
    std::string TypeName(typeid(Element).name());

    if (TypeName == "double")
    {
        return mdkMatrixElementTypeEnum::Scalar_DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return mdkMatrixElementTypeEnum::Scalar_FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT8;
    }
    else if (TypeName == "short")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT16;
    }
    else if (TypeName == "int")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return mdkMatrixElementTypeEnum::Scalar_INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT64;
    }

    return mdkMatrixElementTypeEnum::UNKNOWN;
}

}//end namespace mdk

#endif