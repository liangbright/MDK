#ifndef __mdkJsonFile_h
#define __mdkJsonFile_h

#include <memory>

#include <QString>
#include <QFile>
#include <QTextStream>

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"

namespace mdk
{

class JsonFile : public Object
{
private:
	std::unique_ptr<QTextStream> m_QTextStream;

public:
	JsonFile(const String& FilePathAndName);
	~JsonFile();

	JsonFile& operator<<(char InputChar);
	JsonFile& operator<<(const char* InputString);
	JsonFile& operator<<(const String& InputString);

private:
	JsonFile() = delete;

public:
	static bool Save(const JsonObject& InputObject, const String& FilePathAndName, bool Flag_PreserveOrder = true);
	static bool Load(JsonObject& InputObject, const String& FilePathAndName);

private:
	static bool SaveNameValuePairToJsonFile(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);

	static bool SaveJsonValueToJsonFile(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveJsonValueToJsonFile(bool Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(int Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(long long Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(float Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(double Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const String& Value, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder = true);
	static bool SaveJsonValueToJsonFile(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder = true);

};

}//namespace mdk

#endif