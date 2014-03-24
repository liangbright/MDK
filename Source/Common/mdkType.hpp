#ifndef __mdkType_hpp
#define __mdkType_hpp

//#include "mdkType.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ObjectType>
inline
ScalarTypeEnum FindScalarType(ObjectType Scalar)
{
    std::string TypeName(typeid(Scalar).name());

    if (TypeName == "double")
    {
        return ScalarTypeEnum::DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return ScalarTypeEnum::FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return ScalarTypeEnum::INT8;
    }
    else if (TypeName == "short")
    {
        return ScalarTypeEnum::INT16;
    }
    else if (TypeName == "int")
    {
        return ScalarTypeEnum::INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return ScalarTypeEnum::INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return ScalarTypeEnum::UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return ScalarTypeEnum::UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return ScalarTypeEnum::UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return ScalarTypeEnum::UINT64;
    }

    return ScalarTypeEnum::UNKNOWN;
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
