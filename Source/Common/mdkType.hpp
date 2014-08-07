#ifndef __mdkType_hpp
#define __mdkType_hpp


namespace mdk
{

template<typename ObjectType>
inline
ScalarTypeEnum GetScalarType(ObjectType Scalar)
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
std::string GetScalarTypeName(ScalarType Scalar)
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
int_max GetByteNumberOfScalar(const ScalarType& Scalar)
{
    MDK_Error("Unknown ScalarType @ GetByteNumberOfScalar(ScalarType Scalar)")
    return 0;
}
 

inline double GetByteNumberOfScalar(const double&)
{
    return sizeof(double);
}


inline float GetByteNumberOfScalar(const float&)
{
    return sizeof(float);
}


inline char GetByteNumberOfScalar(const char&)
{
    return sizeof(char);
}


inline short GetByteNumberOfScalar(const short&)
{
    return sizeof(short);
}


inline int GetByteNumberOfScalar(const int&)
{
    return sizeof(int);
}


inline long GetByteNumberOfScalar(const long&)
{
    return sizeof(long);
}


inline long long GetByteNumberOfScalar(const long long&)
{
    return sizeof(long long);
}


inline unsigned char GetByteNumberOfScalar(const unsigned char&)
{
    return sizeof(unsigned char);
}


inline unsigned short GetByteNumberOfScalar(const unsigned short&)
{
    return sizeof(unsigned short);
}


inline unsigned int GetByteNumberOfScalar(const unsigned int&)
{
    return sizeof(unsigned int);
}


inline unsigned long GetByteNumberOfScalar(const unsigned long&)
{
    return sizeof(unsigned long);
}


inline unsigned long long GetByteNumberOfScalar(const unsigned long long&)
{
    return sizeof(unsigned long long);
}


inline unsigned long long GetByteNumberOfScalar(const bool&)
{
    return sizeof(bool);
}

inline unsigned long long GetByteNumberOfScalar(const wchar_t&)
{
    return sizeof(wchar_t);
}


// unsigned short 
//inline unsigned long long GetByteNumberOfScalar(const char16_t&)
//{
//    return sizeof(char16_t);
//}


// unsigned int
//inline unsigned long long GetByteNumberOfScalar(const char32_t&)
//{
//   return sizeof(char32_t);
//}

// usinged long long
//inline unsigned long long GetByteNumberOfScalar(const size_t&)
//{
//    return sizeof(size_t);
//}


//---------------------------------------------------------------------------

template<typename TemplateClassType>
std::string GetTemplateClassName(const TemplateClassType& TemplateClassObject)
{
    std::string ObjectName = typeid(TemplateClassObject).name();

    int_max Index_start = 0;
    int_max Index_end = 0;

    for (int_max k = 0; k < int_max(ObjectName.size()); ++k)
    {
        if (ObjectName[k] == ' ')
        {
            Index_start = k + 1;
        }
        else if (ObjectName[k] == '<')
        {
            Index_end = k - 1;
        }
    }

    auto Length = Index_end - Index_start + 1;

    std::string TemplateClassName;
    TemplateClassName.resize(Length);

    for (int_max k = 0; k < Length; ++k)
    {
        TemplateClassName[k] = ObjectName[Index_start + k];
    }

    return TemplateClassName;
}

}//end namespace mdk

#endif
