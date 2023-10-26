#pragma once

#include <memory>

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QJsonDocument>
#include <QTextStream>

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"
#include "mdkQTDataStructureConversion.h"

namespace mdk
{

#define MDK_JsonFile_MaxCountPerLine_ScalarArray 20

class JsonFile : public Object
{
private:
	std::unique_ptr<QFile> m_QFile;
	std::unique_ptr<QTextStream> m_QTextStream;

private:
	JsonFile(const String& FilePathAndName);
	~JsonFile();

	JsonFile& operator<<(char InputChar);
	JsonFile& operator<<(const char* InputString);
	JsonFile& operator<<(const std::string& InputString);
	JsonFile& operator<<(const String& InputString);

	void Close();

private:
	JsonFile() = delete;

public:
	static bool Save(const JsonObject& InputJObject, const String& FilePathAndName, bool Flag_PreserveOrder = true);
	static bool Load(JsonObject& OutputJObject, const String& FilePathAndName);

	static bool Save(const JsonArray& InputJArray, const String& FilePathAndName, bool Flag_PreserveOrder = true);
	static bool Load(JsonArray& OutputJArray, const String& FilePathAndName);

private:

	static bool SaveNameValuePair(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveNameValuePair(const String& Name, const MDK_Symbol_Empty&, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, bool Flag, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, int Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, long long Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, float Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, double Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const DenseVector<int>& IntArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const DenseVector<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const DenseVector<float>& FloatArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const DenseVector<double>& DoubleArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const String& JString, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const DenseVector<String>& JStringArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePair(const String& Name, const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveNameValuePair(const String& Name, const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);

    //for SaveJsonValue, Indention is only needed if a JsonValue need multi-line

	static bool SaveJsonValue(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveJsonValue(const MDK_Symbol_Empty&, JsonFile& OutputFile);
	static bool SaveJsonValue(bool Flag, JsonFile& OutputFile);
	static bool SaveJsonValue(int Scalar, JsonFile& OutputFile);
	static bool SaveJsonValue(long long Scalar, JsonFile& OutputFile);
	static bool SaveJsonValue(float Scalar, JsonFile& OutputFile);
	static bool SaveJsonValue(double Scalar, JsonFile& OutputFile);
	static bool SaveJsonValue(const DenseVector<int>& IntArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValue(const DenseVector<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValue(const DenseVector<float>& FloatArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValue(const DenseVector<double>& DoubleArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValue(const String& JString, JsonFile& OutputFile);
	static bool SaveJsonValue(const DenseVector<String>& JStringArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValue(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveJsonValue(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);

};

}//namespace mdk
