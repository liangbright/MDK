#ifndef __mdkJsonValue_h
#define __mdkJsonValue_h

#include <memory>

#include "mdkObject.h"
#include "mdkObjectArray.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseMatrix.h"
#include "mdkString.h"
#include "mdkSymbol.h"
#include "mdkJsonObject.h"

namespace mdk
{
//------- forward declare ------------//
class JsonValue;
//------------------------------------//
//------ define JsonArray ------------//
typedef StdObjectVector<JsonValue> JsonArray;
//------------------------------------//

class JsonValue : public Object
{
public:
	enum struct TypeEnum
	{
		Type_Empty,
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
	
	TypeEnum m_Type;
	ScalarUnion m_Scalar;
	std::unique_ptr<Object> m_OtherData;
		
public:
	JsonValue(TypeEnum = TypeEnum::Type_Empty);
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
	void operator=(const DenseMatrix<int>& IntArray);
	void operator=(DenseMatrix<int>&& IntArray);
	void operator=(const DenseVector<int>& IntArray);
	void operator=(DenseVector<int>&& IntArray);
	void operator=(const DenseMatrix<long long>& LongLongArray);
	void operator=(DenseMatrix<long long>&& LongLongArray);
	void operator=(const DenseVector<long long>& LongLongArray);
	void operator=(DenseVector<long long>&& LongLongArray);
	void operator=(const DenseMatrix<float>& FloatArray);
	void operator=(DenseMatrix<float>&& FloatArray);
	void operator=(const DenseVector<float>& FloatArray);
	void operator=(DenseVector<float>&& FloatArray);
	void operator=(const DenseMatrix<double>& DoubleArray);
	void operator=(DenseMatrix<double>&& DoubleArray);
	void operator=(const DenseVector<double>& DoubleArray);
	void operator=(DenseVector<double>&& DoubleArray);
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

	TypeEnum GetType() const { return m_Type; };
	bool IsEmpty() const { return m_Type == TypeEnum::Type_Empty; }
	bool IsScalar() const { return (m_Type == TypeEnum::Type_Bool) || (m_Type == TypeEnum::Type_Int)|| (m_Type == TypeEnum::Type_LongLong) 
								|| (m_Type == TypeEnum::Type_Float) || (m_Type == TypeEnum::Type_Double); };
	bool IsBool() const { return m_Type == TypeEnum::Type_Bool; };
	bool IsInt() const { return m_Type == TypeEnum::Type_Int; };
	bool IsLongLong() const { return m_Type == TypeEnum::Type_LongLong; };
	bool IsFloat() const { return m_Type == TypeEnum::Type_Float; };
	bool IsDouble() const { return m_Type == TypeEnum::Type_Double; };
	bool IsString() const { return m_Type == TypeEnum::Type_String; };
	bool IsJsonArray() const { return m_Type == TypeEnum::Type_JsonArray; };
	bool IsJsonObject() const { return m_Type == TypeEnum::Type_JsonObject; };

	template<typename ScalarType = double>
	ScalarType ToScalar(ScalarType DefaultValue = 0) const;

	template<typename ScalarType = double>
	DenseMatrix<ScalarType> ToScalarArray() const;

	template<typename ScalarType>
	DenseMatrix<ScalarType> ToScalarArray(const DenseMatrix<ScalarType>& DefaultArray) const;

	bool ToBool(bool DefaultValue = false) const;
	int ToInt(int DefaultValue = 0) const;
	long long ToLongLong(long long DefaultValue = 0) const;
	float ToFloat(float DefaultValue = 0) const;
	double ToDouble(double DefaultValue = 0) const;
	DenseMatrix<int> ToIntArray() const;
	DenseMatrix<int> ToIntArray(const DenseMatrix<int>& DefaultArray) const;
	DenseMatrix<long long> ToLongLongArray() const;
	DenseMatrix<long long> ToLongLongArray(const DenseMatrix<long long>& DefaultArray) const;
	DenseMatrix<float> ToFloatArray() const;
	DenseMatrix<float> ToFloatArray(const DenseMatrix<float>& DefaultArray) const;
	DenseMatrix<double> ToDoubleArray() const;
	DenseMatrix<double> ToDoubleArray(const DenseMatrix<double>& DefaultArray) const;
	String ToString() const;
	String ToString(const String& DefaultValue) const;
	JsonArray ToJsonArray() const;
	JsonArray ToJsonArray(const JsonArray& DefaultValue) const;
	JsonObject ToJsonObject() const;
	JsonObject ToJsonObject(const JsonObject& DefaultValue) const;

	// There is NO BoolArray bacause std::vector<bool> is crap

	DenseMatrix<int>& JsonValue::Ref_IntArray() { return *(static_cast<DenseMatrix<int>*>(m_OtherData.get())); }
	const DenseMatrix<int>& JsonValue::Ref_IntArray() const { return *(static_cast<DenseMatrix<int>*>(m_OtherData.get())); }

	DenseMatrix<long long>& JsonValue::Ref_LongLongArray() { return *(static_cast<DenseMatrix<long long>*>(m_OtherData.get())); }
	const DenseMatrix<long long>& JsonValue::Ref_LongLongArray() const { return *(static_cast<DenseMatrix<long long>*>(m_OtherData.get())); }

	DenseMatrix<float>& JsonValue::Ref_FloatArray() { return *(static_cast<DenseMatrix<float>*>(m_OtherData.get())); }
	const DenseMatrix<float>& JsonValue::Ref_FloatArray() const { return *(static_cast<DenseMatrix<float>*>(m_OtherData.get())); }

	DenseMatrix<double>& JsonValue::Ref_DoubleArray() { return *(static_cast<DenseMatrix<double>*>(m_OtherData.get())); }
	const DenseMatrix<double>& JsonValue::Ref_DoubleArray() const { return *(static_cast<DenseMatrix<double>*>(m_OtherData.get())); }

	String& JsonValue::Ref_String() { return *(static_cast<String*>(m_OtherData.get())); }
	const String& JsonValue::Ref_String() const { return *(static_cast<String*>(m_OtherData.get())); }

	JsonArray& JsonValue::Ref_JsonArray() { return *(static_cast<JsonArray*>(m_OtherData.get())); }
	const JsonArray& JsonValue::Ref_JsonArray() const { return *(static_cast<JsonArray*>(m_OtherData.get())); }

	JsonObject& JsonValue::Ref_JsonObject() { return *(static_cast<JsonObject*>(m_OtherData.get())); }
	const JsonObject& JsonValue::Ref_JsonObject() const { return *(static_cast<JsonObject*>(m_OtherData.get())); }

private:

};

}//namespace mdk

//#include "mdkJsonValue.hpp"

#endif