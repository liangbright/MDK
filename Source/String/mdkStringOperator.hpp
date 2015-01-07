#ifndef __mdkStringOperator_hpp
#define __mdkStringOperator_hpp

namespace mdk
{

inline String operator+(const String& StringA, const String& StringB)
{
    String StringC;
    StringC.StdString() = StringA.StdString() + StringB.StdString();
    return StringC;
}


inline String operator+(const String& StringA, const std::basic_string<char>& StringB)
{
    String StringC;
    StringC.StdString() = StringA.StdString() + StringB;
    return StringC;
}


inline String operator+(const std::basic_string<char>& StringA, const String& StringB)
{
    String StringC;
    StringC.StdString() = StringA + StringB.StdString();
    return StringC;
}


inline String operator+(const String& StringA, const char* StringB)
{
    String StringC;
    StringC.StdString() = StringA.StdString() + StringB;
    return StringC;
}



inline String operator+(const char* StringA, const String& StringB)
{
    String StringC;
    StringC.StdString() = StringA + StringB.StdString();
    return StringC;
}


inline bool operator==(const String& StringA, const String& StringB)
{
    return StringA.StdString() == StringB.StdString();
}


inline bool operator==(const String& StringA, const std::basic_string<char>& StringB)
{
    return StringA.StdString() == StringB;
}


inline bool operator==(const std::basic_string<char>& StringA, const String& StringB)
{
    return StringA == StringB.StdString();
}


inline bool operator==(const String& StringA, const char* StringB)
{
    return StringA.StdString() == StringB;
}


inline bool operator==(const char* StringA, const String& StringB)
{
    return StringA == StringB.StdString();
}


inline bool operator!=(const String& StringA, const String& StringB)
{
    return StringA.StdString() != StringB.StdString();
}


inline bool operator!=(const String& StringA, const std::basic_string<char>& StringB)
{
    return StringA.StdString() != StringB;
}


inline bool operator!=(const std::basic_string<char>& StringA, const String& StringB)
{
    return StringA != StringB.StdString();
}


inline bool operator!=(const String& StringA, const char* StringB)
{
    return StringA.StdString() != StringB;
}


inline bool operator!=(const char* StringA, const String& StringB)
{
    return StringA != StringB.StdString();
}


template <typename Traits>
inline
std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const String& str)
{
    return operator<<(os, str.StdString());
}


}//end namespace mdk

#endif