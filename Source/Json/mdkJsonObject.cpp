#ifndef __mdkJsonObject_cpp
#define __mdkJsonObject_cpp

#include "mdkJsonObject.h"

namespace mdk
{

JsonObject::JsonObject()
{
}

JsonObject::JsonObject(const JsonObject& InputObject)
{
}

JsonObject::JsonObject(JsonObject&& InputObject)
{
}

JsonObject::~JsonObject()
{
}

void JsonObject::operator=(const JsonObject& InputObject)
{

}

void JsonObject::operator=(JsonObject&& InputObject)
{

}

void JsonObject::Clear()
{
	m_DataMap.clear();
	m_NameList.clear();
	m_NameList.shrink_to_fit();
}

}//namespace mdk

#endif