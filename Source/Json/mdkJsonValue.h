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
	// int/long long/float/double Array is represented by DenseMatrix as a row vector
	// There is NO BoolArray bacause std::vector<bool> is crap
	enum struct TypeEnum
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
	JsonValue(TypeEnum = TypeEnum::Type_Null);
	JsonValue(bool Value);
	JsonValue(int Value);
	JsonValue(long long Value);
	JsonValue(float Value);
	JsonValue(double Value);	
	JsonValue(DenseMatrix<int> InputArray);
	JsonValue(DenseVector<int> InputArray);
	JsonValue(const int* InputArray, int_max ArrayLength);
	JsonValue(DenseMatrix<long long> InputArray);
	JsonValue(DenseVector<long long> InputArray);
	JsonValue(const long long* InputArray, int_max ArrayLength);
	JsonValue(DenseMatrix<float> InputArray);
	JsonValue(DenseVector<float> InputArray);
	JsonValue(const float* InputArray, int_max ArrayLength);
	JsonValue(DenseMatrix<double> InputArray);
	JsonValue(DenseVector<double> InputArray);
	JsonValue(const double* InputArray, int_max ArrayLength);
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
	void operator=(const DenseMatrix<int>& InputArray);
	void operator=(DenseMatrix<int>&& InputArray);
	void operator=(const DenseVector<int>& InputArray);
	void operator=(DenseVector<int>&& InputArray);
	void operator=(const DenseMatrix<long long>& InputArray);
	void operator=(DenseMatrix<long long>&& InputArray);
	void operator=(const DenseVector<long long>& InputArray);
	void operator=(DenseVector<long long>&& InputArray);
	void operator=(const DenseMatrix<float>& InputArray);
	void operator=(DenseMatrix<float>&& InputArray);
	void operator=(const DenseVector<float>& InputArray);
	void operator=(DenseVector<float>&& InputArray);
	void operator=(const DenseMatrix<double>& InputArray);
	void operator=(DenseMatrix<double>&& InputArray);
	void operator=(const DenseVector<double>& InputArray);
	void operator=(DenseVector<double>&& InputArray);
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

	bool IsNull() const { return m_Type == TypeEnum::Type_Null; }
	bool IsBool() const { return m_Type == TypeEnum::Type_Bool; };
	bool IsInt() const { return m_Type == TypeEnum::Type_Int; };
	bool IsLongLong() const { return m_Type == TypeEnum::Type_LongLong; };
	bool IsFloat() const { return m_Type == TypeEnum::Type_Float; };
	bool IsDouble() const { return m_Type == TypeEnum::Type_Double; };
	bool IsIntArray() const { return m_Type == TypeEnum::Type_IntArray; };
	bool IsLongLongArray() const { return m_Type == TypeEnum::Type_LongLongArray; };
	bool IsFloatArray() const { return m_Type == TypeEnum::Type_FloatArray; };
	bool IsDoubleArray() const { return m_Type == TypeEnum::Type_DoubleArray; };
	bool IsString() const { return m_Type == TypeEnum::Type_String; };
	bool IsJsonArray() const { return m_Type == TypeEnum::Type_JsonArray; };
	bool IsJsonObject() const { return m_Type == TypeEnum::Type_JsonObject; };

	//Attention: ------------------------------------------------------------------------------------------------------------//
	//  if m_Scalar is int, then GetDouble() is DefaultValue, NOT type-converted from m_Scalar
	//  if internal array is int array, then GetDoubleArray() is EmptyArray, NOT type-converted from int array
	//  similar for other type
	//  --------------------------------------------------------------------------------------
    //  ToScalar<ScalarType>()       : m_Scalar will be converted to ScalarType
	//  ToScalarArray<ScalarType>()  : internal array will be converted to ScalarType Array
	//-----------------------------------------------------------------------------------------------------------------------//

	bool GetBool(bool DefaultValue = false) const;
	int GetInt(int DefaultValue = 0) const;
	long long GetLongLong(long long DefaultValue = 0) const;
	float GetFloat(float DefaultValue = 0) const;
	double GetDouble(double DefaultValue = 0) const;
	DenseMatrix<int> GetIntArray() const;
	DenseMatrix<int> GetIntArray(const DenseMatrix<int>& DefaultArray) const;
	DenseMatrix<long long> GetLongLongArray() const;
	DenseMatrix<long long> GetLongLongArray(const DenseMatrix<long long>& DefaultArray) const;
	DenseMatrix<float> GetFloatArray() const;
	DenseMatrix<float> GetFloatArray(const DenseMatrix<float>& DefaultArray) const;
	DenseMatrix<double> GetDoubleArray() const;
	DenseMatrix<double> GetDoubleArray(const DenseMatrix<double>& DefaultArray) const;
	String GetString() const;
	String GetString(const String& DefaultValue) const;
	JsonArray GetJsonArray() const;
	JsonArray GetJsonArray(const JsonArray& DefaultValue) const;
	JsonObject GetJsonObject() const;
	JsonObject GetJsonObject(const JsonObject& DefaultValue) const;

	// check if int/long long/float/double ?
	bool IsScalar() const
	{
		return (m_Type == TypeEnum::Type_Bool) || (m_Type == TypeEnum::Type_Int) || (m_Type == TypeEnum::Type_LongLong)
			|| (m_Type == TypeEnum::Type_Float) || (m_Type == TypeEnum::Type_Double);
	};

	// convert internal int/long long/float/double to scalar
	template<typename ScalarType = double>
	ScalarType ToScalar(ScalarType DefaultValue = 0) const;

	// check if int/long long/float/double array ?
	bool IsScalarArray() const
	{
		return (m_Type == TypeEnum::Type_IntArray) || (m_Type == TypeEnum::Type_LongLongArray)
			|| (m_Type == TypeEnum::Type_FloatArray) || (m_Type == TypeEnum::Type_DoubleArray);
	};

	// convert internal int/long long/float/double array to scalar array
	template<typename ScalarType = double>
	DenseMatrix<ScalarType> ToScalarArray() const;

	template<typename ScalarType>
	DenseMatrix<ScalarType> ToScalarArray(const DenseMatrix<ScalarType>& DefaultArray) const;

	//----------------------------- use these Ref_XXX only if you are absolutely sure about the type -----------------------------------------//
 	
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

};

}//namespace mdk

#include "mdkJsonValue.hpp"

#endif
