#ifndef __mdkStringHash_h
#define __mdkStringHash_h

#include "mdkString.h"

namespace mdk
{ 
template<typename T>
class StringHash;

template<>
class StringHash<String>
{
public:
	std::size_t operator()(String const& s) const
	{
		return std::hash<std::string>()(s.StdString());
	}
};

}// namespace mdk

#endif