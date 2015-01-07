#ifndef __mdkJsonObject_h
#define __mdkJsonObject_h

#include <unordered_map>
#include <vector>

#include "mdkObject.h"
#include "mdkCommonType.h"
#include "mdkStringHash.h"

namespace mdk
{
//--------- forward declare ------------//
class JsonFile;
class JsonValue;
//---------------------------------------//

class JsonObject : public Object
{
public:
	typedef std::unordered_map<String, JsonValue, StringHash<String>> DataMapType;

	typedef DataMapType::local_iterator local_iterator;
	typedef DataMapType::const_local_iterator const_local_iterator;

	typedef DataMapType::iterator iterator;
	typedef DataMapType::const_iterator const_iterator;

	typedef std::vector<String> NameListType;

public:
	DataMapType m_DataMap;   // <Name, Value>
	NameListType m_NameList; // preserve insertion order

public:
	JsonObject();
	JsonObject(const JsonObject& InputObject);
	JsonObject(JsonObject&& InputObject);
	~JsonObject();

	void operator=(const JsonObject& InputObject);
	void operator=(JsonObject&& InputObject);

	void Clear();

	bool IsEmpty() const { return m_DataMap.size() == 0; }

	const NameListType& NameList() const { return m_NameList; };
	const DataMapType& DataMap() const { return m_DataMap; };

	int_max GetElementNumber() const { return int_max(m_DataMap.size()); };

	JsonValue& operator[](const String& Name);

	local_iterator begin() { return m_DataMap.begin(); }
	const_local_iterator begin() const { return m_DataMap.begin(); };

	local_iterator end() { return m_DataMap.end(); };
	const_local_iterator end() const { return m_DataMap.end(); };

	iterator find(const String& Name) { return m_DataMap.find(Name); }
	const_iterator find(const String& Name) const  { return m_DataMap.find(Name); }

private:
};

}//namespace mdk

#endif