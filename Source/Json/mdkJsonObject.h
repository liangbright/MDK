#ifndef __mdkJsonObject_h
#define __mdkJsonObject_h

#include <string>
#include <unordered_map>
#include <vector>

#include "mdkObject.h"
#include "mdkType.h"
#include "mdkJsonValue.h"

namespace mdk
{

//--------- forward declare ------------//
class JsonFile;
//---------------------------------------//

class JsonObject : public Object
{
public:
	std::unordered_map<std::string, JsonValue> m_DataMap; // <Name, Value>
	std::vector<std::string> m_NameList; // insertion order

	bool m_Flag_SaveInOrder; // use insertion order

public:
	JsonObject();
	JsonObject(const JsonObject& InputObject);
	JsonObject(JsonObject&& InputObject);
	~JsonObject();

	void operator=(const JsonObject& InputObject);
	void operator=(JsonObject&& InputObject);

	void Clear();

	std::vector<std::string>& NameList() { return m_NameList; };
	const std::vector<std::string>& NameList() const { return m_NameList; };

	std::unordered_map<std::string, JsonValue>& DataMap() { return m_DataMap; };
	const std::unordered_map<std::string, JsonValue>& DataMap() const { return m_DataMap; };

private:
};

}//namespace mdk

#endif