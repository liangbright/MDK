#ifndef __mdkType_hpp
#define __mdkType_hpp

#include "mdkType.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ScalarType>
inline
mdkScalarTypeEnum FindScalarType(ScalarType Scalar)
{
    std::string TypeName(typeid(Scalar).name());

    if (TypeName == "double")
    {
        return mdkScalarTypeEnum::MDK_DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return mdkScalarTypeEnum::MDK_FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return mdkScalarTypeEnum::MDK_INT8;
    }
    else if (TypeName == "short")
    {
        return mdkScalarTypeEnum::MDK_INT16;
    }
    else if (TypeName == "int")
    {
        return mdkScalarTypeEnum::MDK_INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return mdkScalarTypeEnum::MDK_INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return mdkScalarTypeEnum::MDK_UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return mdkScalarTypeEnum::MDK_UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return mdkScalarTypeEnum::MDK_UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return mdkScalarTypeEnum::MDK_UINT64;
    }

    return mdkScalarTypeEnum::MDK_UNKNOWN;
}

template<typename ScalarType>
inline
uint64 CalByteNumberOfScalar(ScalarType Scalar)
{
    std::string TypeName(typeid(Scalar).name());

    if (TypeName == "double")
    {
        return 8;
    }
    else if (TypeName == "float")
    {
        return 4;
    }
    else if (TypeName == "signed char")
    {
        return 1;
    }
    else if (TypeName == "short")
    {
        return 2;
    }
    else if (TypeName == "int")
    {
        return 4;
    }
    else if (TypeName == "__int64")
    {
        return 8;
    }
    else if (TypeName == "unsigned char")
    {
        return 1;
    }
    else if (TypeName == "unsigned short")
    {
        return 2;
    }
    else if (TypeName == "unsigned int")
    {
        return 4;
    }
    else if (TypeName == "unsigned __int64")
    {
        return 8;
    }

    mdkError << "Unknown ScalarType @ CalByteNumberOfScalar" << '\n';
    return 0;
}

}//end namespace mdk

#endif