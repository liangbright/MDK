#ifndef mdk_Constant_hpp
#define mdk_Constant_hpp


namespace mdk
{

//----------------------------------------------------

template<typename ElementType>
inline
ElementType GetNaNElement()
{
    return GetNaNElement(ElementType(0));
}


template<typename ElementType>
inline
ElementType GetNaNElement(const ElementType& ReferenceElement)
{
    return ReferenceElement;
}


inline double GetNaNElement(const double&)
{
    return std::nan(nullptr);
}


inline float GetNaNElement(const float&)
{
    return std::nanf(nullptr);
}


inline char GetNaNElement(const char&)
{
    return char(0);
}


inline short GetNaNElement(const short&)
{
    return short(0);
}


inline int GetNaNElement(const int&)
{
    return int(0);
}


inline long GetNaNElement(const long&)
{
    return long(0);
}


inline long long GetNaNElement(const long long&)
{
    return (long long)(0);
}


inline unsigned char GetNaNElement(const unsigned char&)
{
    return (unsigned char)(0);
}


inline unsigned short GetNaNElement(const unsigned short&)
{
    return (unsigned short)(0);
}


inline unsigned int GetNaNElement(const unsigned int&)
{
    return (unsigned int)(0);
}


inline unsigned long GetNaNElement(const unsigned long&)
{
    return (unsigned long)(0);
}


inline unsigned long long GetNaNElement(const unsigned long long&)
{
    return (unsigned long long)(0);
}

}//namespace mdk

#endif
