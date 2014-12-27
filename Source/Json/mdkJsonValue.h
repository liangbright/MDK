#ifndef __mdkJsonValue_h
#define __mdkJsonValue_h

#include <memory>

#include "mdkObject.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkString.h"
#include "mdkSymbol.h"

namespace mdk
{
//------- forward declare ------------//
class JsonObject;
class JsonValue;
//------------------------------------//
//------ define JsonArray ------------//
typedef DataArray<JsonValue> JsonArray;
//------------------------------------//

class JsonValue : public Object
{
public:
	enum Type
	{
		Type_Null,
		Type_Bool,
		Type_Int,
		Type_LongLong,
		Type_Float,
		Type_Double,
		Type_IntArray,
		Type_LongLongArray,
		Type_FloatArray,
		Type_DoubleArray,
		Type_String,
		Type_JsonArray,
		Type_JsonObject,
		Type_Undefined
	};

private:
	
	union ScalarUnion
	{
		bool Bool;
		int Int;
		long long LongLong;		
		float Float;
		double Double;
	};
	
	Type m_Type;
	ScalarUnion m_Scalar;
	std::unique_ptr<Object> m_OtherData;
		
public:
	JsonValue(Type = Type_Null);
	JsonValue(bool Value);
	JsonValue(int Value);
	JsonValue(long long Value);
	JsonValue(float Value);
	JsonValue(double Value);	
	JsonValue(DenseMatrix<int> IntArray);
	JsonValue(DenseVector<int> IntArray);
	JsonValue(const int* IntArray, int_max ArrayLength);
	JsonValue(DenseMatrix<long long> LongLongArray);
	JsonValue(DenseVector<long long> LongLongArray);
	JsonValue(const long long* LongLongArray, int_max ArrayLength);
	JsonValue(DenseMatrix<float> FloatArray);
	JsonValue(DenseVector<float> FloatArray);
	JsonValue(const float* FloatArray, int_max ArrayLength);
	JsonValue(DenseMatrix<double> DoubleArray);
	JsonValue(DenseVector<double> DoubleArray);
	JsonValue(const double* DoubleArray, int_max ArrayLength);		
	JsonValue(const char* Value);
	JsonValue(String Value);
	JsonValue(JsonArray Value);
	JsonValue(JsonObject Value);

	JsonValue(const JsonValue& Value);
	JsonValue(JsonValue&& Value);
	~JsonValue();

	void operator=(bool Value);
	void operator=(int Value);
	void operator=(long long Value);
	void operator=(float Value);
	void operator=(double Value);
	void operator=(const DataArray<int>& Value);
	void operator=(DataArray<int>&& Value);
	void operator=(const DataArray<long long>& Value);
	void operator=(DataArray<long long>&& Value);
	void operator=(const DataArray<float>& Value);
	void operator=(DataArray<float>&& Value);
	void operator=(const DataArray<double>& Value);
	void operator=(DataArray<double>&& Value);
	void operator=(const char* Value);
	void operator=(const String& Value);
	void operator=(String&& Value);
	void operator=(const JsonArray& Value);
	void operator=(JsonArray&& Value);
	void operator=(const JsonObject& Value);
	void operator=(JsonObject&& Value);

	void operator=(const JsonValue& Value);
	void operator=(JsonValue&& Value);

	void Clear();// clear memory and change type to Undefined

	bool IsEmpty() const { return m_Type == Type_Null; }

	Type GetType() const { return m_Type; };
	bool IsNull() const { return m_Type == Type_Null; };
	bool IsBool() const { return m_Type == Type_Bool; };
	bool IsInt() const { return m_Type == Type_Int; };
	bool IsLongLong() const { return m_Type == Type_LongLong; };
	bool IsFloat() const { return m_Type == Type_Float; };
	bool IsDouble() const { return m_Type == Type_Double; };
	bool IsString() const { return m_Type == Type_String; };
	bool IsJsonArray() const { return m_Type == Type_JsonArray; };
	bool IsJsonObject() const { return m_Type == Type_JsonObject; };
	bool IsUndefined() const { return m_Type == Type_Undefined; };

	bool ToBool(bool DefaultValue = false) const;
	int ToInt(int DefaultValue = 0) const;
	long long  ToLongLong(long long DefaultValue = 0) const;
	float ToFloat(float DefaultValue = 0) const;
	double ToDouble(double DefaultValue = 0) const;
	String ToString() const;
	String ToString(const String& DefaultValue) const;
	JsonArray ToJsonArray() const;
	JsonArray ToJsonArray(const JsonArray& DefaultValue) const;
	JsonObject ToJsonObject() const;
	JsonObject ToJsonObject(const JsonObject& DefaultValue) const;

	String& Ref_String() { return *(static_cast<String*>(m_OtherData.get())); }
	const String& Ref_String() const { return *(static_cast<String*>(m_OtherData.get())); }

	JsonArray& Ref_JsonArray() { return *(static_cast<JsonArray*>(m_OtherData.get())); }
	const JsonArray& Ref_JsonArray() const { return *(static_cast<JsonArray*>(m_OtherData.get())); }

	JsonObject& Ref_JsonObject() { return *(static_cast<JsonObject*>(m_OtherData.get())); }
	const JsonObject& Ref_JsonObject() const { return *(static_cast<JsonObject*>(m_OtherData.get())); }

private:

};

}//namespace mdk

//#include "mdkJsonValue.hpp"

#endif