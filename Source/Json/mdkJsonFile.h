#ifndef __mdkJsonFile_h
#define __mdkJsonFile_h

#include <memory>

#include <QString>
#include <QFile>
#include <QTextStream>

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"
#include "mdkQTDataStructureConversion.h"

namespace mdk
{

#define MDK_JsonFile_MaxNumberPerLine_ScalarArray 20

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
	JsonFile& operator<<(const String& InputString);

	void Close();

private:
	JsonFile() = delete;

public:
	static bool Save(const JsonObject& InputObject, const String& FilePathAndName, bool Flag_PreserveOrder = true);
	static bool Load(JsonObject& InputObject, const String& FilePathAndName);

private:

	static bool SaveNameValuePairToJsonFile(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveNameValuePairToJsonFile(const String& Name, const MDK_Symbol_Empty&, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, bool Flag, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, int Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, long long Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, float Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, double Scalar, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const String& JString, JsonFile& OutputFile, int_max Indention);
	static bool SaveNameValuePairToJsonFile(const String& Name, const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveNameValuePairToJsonFile(const String& Name, const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);

    //for SaveJsonValueToJsonFile, Indention is only needed if a JsonValue need multi-line

	static bool SaveJsonValueToJsonFile(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveJsonValueToJsonFile(bool Flag, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(int Scalar, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(long long Scalar, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(float Scalar, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(double Scalar, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention);
	static bool SaveJsonValueToJsonFile(const String& JString, JsonFile& OutputFile);
	static bool SaveJsonValueToJsonFile(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);
	static bool SaveJsonValueToJsonFile(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder);

};

}//namespace mdk

#endif