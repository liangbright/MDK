#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <ostream>

namespace mdk
{

inline String operator+(const String& StringA, const String& StringB);

inline String operator+(const String& StringA, const std::basic_string<char>& StringB);

inline String operator+(const std::basic_string<char>& StringA, const String& StringB);

inline String operator+(const String& StringA, const char* StringB);

inline String operator+(const char* StringA, const String& StringB);

inline bool operator==(const String& StringA, const String& StringB);

inline bool operator==(const String& StringA, const std::basic_string<char>& StringB);

inline bool operator==(const std::basic_string<char>& StringA, const String& StringB);

inline bool operator==(const String& StringA, const char* StringB);

inline bool operator==(const char* StringA, const String& StringB);

inline bool operator!=(const String& StringA, const String& StringB);

inline bool operator!=(const String& StringA, const std::basic_string<char>& StringB);

inline bool operator!=(const std::basic_string<char>& StringA, const String& StringB);

inline bool operator!=(const String& StringA, const char* StringB);

inline bool operator!=(const char* StringA, const String& StringB);

template <typename Traits = std::char_traits<char>>
inline std::basic_ostream<char, Traits>&  operator<<(std::basic_ostream<char, Traits>& os, const String& str);

}//end namespace mdk

#include "mdkStringOperator.hpp"

