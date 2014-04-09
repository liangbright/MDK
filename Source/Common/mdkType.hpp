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
std::string FindScalarTypeName(ScalarType Scalar)
{
    std::string Name;

    std::string TypeName(typeid(Scalar).name());

    if (TypeName == "double")
    {
        Name = "double";
    }
    else if (TypeName == "float")
    {
        Name = "float";
    }
    else if (TypeName == "signed char")
    {
        Name = "int8";
    }
    else if (TypeName == "short")
    {
        Name = "int16";
    }
    else if (TypeName == "int")
    {
        Name = "int32";
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        Name = "int64";
    }
    else if (TypeName == "unsigned char")
    {
        Name = "uint8";
    }
    else if (TypeName == "unsigned short")
    {
        Name = "uint16";
    }
    else if (TypeName == "unsigned int")
    {
        Name = "uint32";
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        Name = "uint64";
    }
    else
    {
        Name = "unknown";
    }

    return Name;
}


template<typename ScalarType>
inline
int_max CalByteNumberOfScalar(const ScalarType& Scalar)
{
    MDK_Error("Unknown ScalarType @ CalByteNumberOfScalar(ScalarType Scalar)")
    return 0;
}
 

inline double CalByteNumberOfScalar(const double&)
{
    return sizeof(double);
}


inline float CalByteNumberOfScalar(const float&)
{
    return sizeof(float);
}


inline char CalByteNumberOfScalar(const char&)
{
    return sizeof(char);
}


inline short CalByteNumberOfScalar(const short&)
{
    return sizeof(short);
}


inline int CalByteNumberOfScalar(const int&)
{
    return sizeof(int);
}


inline long CalByteNumberOfScalar(const long&)
{
    return sizeof(long);
}


inline long long CalByteNumberOfScalar(const long long&)
{
    return sizeof(long long);
}


inline unsigned char CalByteNumberOfScalar(const unsigned char&)
{
    return sizeof(unsigned char);
}


inline unsigned short CalByteNumberOfScalar(const unsigned short&)
{
    return sizeof(unsigned short);
}


inline unsigned int CalByteNumberOfScalar(const unsigned int&)
{
    return sizeof(unsigned int);
}


inline unsigned long CalByteNumberOfScalar(const unsigned long&)
{
    return sizeof(unsigned long);
}


inline unsigned long long CalByteNumberOfScalar(const unsigned long long&)
{
    return sizeof(unsigned long long);
}


}//end namespace mdk

#endif
