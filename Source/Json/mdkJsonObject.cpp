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
	m_DataMap = InputObject.m_DataMap;
	m_NameList = InputObject.m_NameList;
}

JsonObject::JsonObject(JsonObject&& InputObject)
{
	m_DataMap = std::move(InputObject.m_DataMap);
	m_NameList = std::move(InputObject.m_NameList);
}

JsonObject::~JsonObject()
{
}

void JsonObject::operator=(const JsonObject& InputObject)
{
	m_DataMap = InputObject.m_DataMap;
	m_NameList = InputObject.m_NameList;
}

void JsonObject::operator=(JsonObject&& InputObject)
{
	m_DataMap = std::move(InputObject.m_DataMap);
	m_NameList = std::move(InputObject.m_NameList);
}

void JsonObject::Clear()
{
	m_DataMap.clear();
	m_NameList.clear();
	m_NameList.shrink_to_fit();
}

JsonValue& JsonObject::operator[](const String& Name) 
{
	auto it = m_DataMap.find(Name);
	if (it == m_DataMap.end())
	{
		m_NameList.push_back(Name);		
	}
	return m_DataMap[Name];
}

}//namespace mdk

#endif