#ifndef __mdkType_hpp
#define __mdkType_hpp

#include "mdkType.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ObjectType>
inline
mdkScalarTypeEnum FindScalarType(ObjectType Scalar)
{
    std::string TypeName(typeid(Scalar).name());

    if (TypeName == "double")
    {
        return mdkScalarTypeEnum::DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return mdkScalarTypeEnum::FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return mdkScalarTypeEnum::INT8;
    }
    else if (TypeName == "short")
    {
        return mdkScalarTypeEnum::INT16;
    }
    else if (TypeName == "int")
    {
        return mdkScalarTypeEnum::INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return mdkScalarTypeEnum::INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return mdkScalarTypeEnum::UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return mdkScalarTypeEnum::UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return mdkScalarTypeEnum::UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return mdkScalarTypeEnum::UINT64;
    }

    return mdkScalarTypeEnum::UNKNOWN;
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