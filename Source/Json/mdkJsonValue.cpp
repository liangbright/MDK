#pragma once

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"

namespace mdk
{

JsonValue::JsonValue(TypeEnum TypeOfValue)
{
	switch (TypeOfValue)
	{
	case TypeEnum::Type_Null:
		m_Type = TypeEnum::Type_Null;
		break;
	case TypeEnum::Type_Bool:
		m_Scalar.Bool = true;
		m_Type = TypeEnum::Type_Bool;
		break;
	case TypeEnum::Type_Int:
		m_Scalar.Int = 0;
		m_Type = TypeEnum::Type_Int;
		break;
	case TypeEnum::Type_LongLong:
		m_Scalar.LongLong = 0;
		m_Type = TypeEnum::Type_LongLong;
		break;
	case TypeEnum::Type_Float:
		m_Scalar.Float = 0;
		m_Type = TypeEnum::Type_Float;
		break;
	case TypeEnum::Type_Double:
		m_Scalar.Double = 0;
		m_Type = TypeEnum::Type_Double;
		break;
	case TypeEnum::Type_IntArray:
	{
		auto TempPtr = std::make_unique<DenseVector<int>>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_IntArray;
		break;
	}
	case TypeEnum::Type_LongLongArray:
	{
		auto TempPtr = std::make_unique<DenseVector<long long>>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case TypeEnum::Type_FloatArray:
	{
		auto TempPtr = std::make_unique<DenseVector<float>>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_FloatArray;
		break;
	}
	case TypeEnum::Type_DoubleArray:
	{
		auto TempPtr = std::make_unique<DenseVector<double>>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_DoubleArray;
		break;
	}
	case TypeEnum::Type_String:
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_String;
		break;
	}
	case TypeEnum::Type_StringArray:
	{
		auto TempPtr = std::make_unique<DenseVector<String>>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_StringArray;
		break;
	}
	case TypeEnum::Type_JsonArray:
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_JsonArray;
		break;
	}
	case TypeEnum::Type_JsonObject:
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
		m_Type = TypeEnum::Type_JsonObject;
		break;
	}
	default:
		MDK_Error("Somthing is wrong here @ JsonValue::JsonValue(Type TypeOfValue)")
		m_Type = TypeEnum::Type_Null;
	}
}

JsonValue::JsonValue(bool Value)
{
	m_Scalar.Bool = Value;
	m_Type = TypeEnum::Type_Bool;
}

JsonValue::JsonValue(int Value)
{
	m_Scalar.Int = Value;
	m_Type = TypeEnum::Type_Int;
}

JsonValue::JsonValue(long long Value)
{
	m_Scalar.LongLong = Value;
	m_Type = TypeEnum::Type_LongLong;
}

JsonValue::JsonValue(float Value)
{
	m_Scalar.Float = Value;
	m_Type = TypeEnum::Type_Float;
}

JsonValue::JsonValue(double Value)
{
	m_Scalar.Double = Value;
	m_Type = TypeEnum::Type_Double;
}

JsonValue::JsonValue(const int* InputArray, int_max ArrayLength)
{
	auto TempPtr = std::make_unique<DenseVector<int>>();
	(*TempPtr).Append(InputArray, ArrayLength);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_IntArray;
}

JsonValue::JsonValue(const long long* InputArray, int_max ArrayLength)
{
	auto TempPtr = std::make_unique<DenseVector<long long>>();
	(*TempPtr).Append(InputArray, ArrayLength);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_LongLongArray;
}

JsonValue::JsonValue(const float* InputArray, int_max ArrayLength)
{
	auto TempPtr = std::make_unique<DenseVector<float>>();
	(*TempPtr).Append(InputArray, ArrayLength);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_FloatArray;
}

JsonValue::JsonValue(const double* InputArray, int_max ArrayLength)
{
	auto TempPtr = std::make_unique<DenseVector<double>>();
	(*TempPtr).Append(InputArray, ArrayLength);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_DoubleArray;
}

JsonValue::JsonValue(DenseVector<int> InputArray)
{
	m_Type = TypeEnum::Type_Null;//it must be Type_Null
	(*this) = std::move(InputArray);
}

JsonValue::JsonValue(DenseVector<long long> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

JsonValue::JsonValue(DenseVector<float> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

JsonValue::JsonValue(DenseVector<double> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

JsonValue::JsonValue(const char* Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = Value;
}

JsonValue::JsonValue(String Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(Value);
}

JsonValue::JsonValue(DenseVector<String> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

JsonValue::JsonValue(JsonArray Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(Value);
}

JsonValue::JsonValue(JsonObject Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(Value);
}

JsonValue::JsonValue(const JsonValue& Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = Value;
}

JsonValue::JsonValue(JsonValue&& Value)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(Value);	
}

JsonValue::~JsonValue()
{
}

void JsonValue::operator=(bool Value)
{
	if (m_Type != TypeEnum::Type_Bool)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Bool = Value;
	m_Type = TypeEnum::Type_Bool;
}

void JsonValue::operator=(int Value)
{
	if (m_Type != TypeEnum::Type_Int)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Int = Value;
	m_Type = TypeEnum::Type_Int;
}

void JsonValue::operator=(long long Value)
{
	if (m_Type != TypeEnum::Type_LongLong)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.LongLong = Value;
	m_Type = TypeEnum::Type_LongLong;
}

void JsonValue::operator=(float Value)
{
	if (m_Type != TypeEnum::Type_Float)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Float = Value;
	m_Type = TypeEnum::Type_Float;
}

void JsonValue::operator=(double Value)
{
	if (m_Type != TypeEnum::Type_Double)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Double = Value;
	m_Type = TypeEnum::Type_Double;
}

void JsonValue::operator=(const DenseVector<int>& InputArray)
{
	if (m_Type != TypeEnum::Type_IntArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<int>>();
	(*TempPtr) = InputArray;
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_IntArray;
}

void JsonValue::operator=(DenseVector<int>&& InputArray)
{
	if (m_Type != TypeEnum::Type_IntArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<int>>();
	(*TempPtr) = std::move(InputArray);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_IntArray;
}

void JsonValue::operator=(const DenseVector<long long>& InputArray)
{
	if (m_Type != TypeEnum::Type_LongLongArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<long long>>();
	(*TempPtr) = InputArray;
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_LongLongArray;
}

void JsonValue::operator=(DenseVector<long long>&& InputArray)
{
	if (m_Type != TypeEnum::Type_LongLongArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<long long>>();
	(*TempPtr) = std::move(InputArray);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_LongLongArray;
}

void JsonValue::operator=(const DenseVector<float>& InputArray)
{
	if (m_Type != TypeEnum::Type_FloatArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<float>>();
	(*TempPtr) = InputArray;
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_FloatArray;
}

void JsonValue::operator=(DenseVector<float>&& InputArray)
{
	if (m_Type != TypeEnum::Type_FloatArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<float>>();
	(*TempPtr) = std::move(InputArray);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_FloatArray;
}

void JsonValue::operator=(const DenseVector<double>& InputArray)
{
	if (m_Type != TypeEnum::Type_DoubleArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<double>>();
	(*TempPtr) = InputArray;
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_DoubleArray;
}

void JsonValue::operator=(DenseVector<double>&& InputArray)
{
	if (m_Type != TypeEnum::Type_DoubleArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<double>>();
	(*TempPtr) = std::move(InputArray);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_DoubleArray;
}

void JsonValue::operator=(const char* Value)
{
	if (m_Type != TypeEnum::Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}		
	this->Ref_String() = Value;
	m_Type = TypeEnum::Type_String;
}

void JsonValue::operator=(const String& Value)
{
	if (m_Type != TypeEnum::Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_String() = Value;
	m_Type = TypeEnum::Type_String;
}

void JsonValue::operator=(String&& Value)
{
	if (m_Type != TypeEnum::Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_String() = std::move(Value);
	m_Type = TypeEnum::Type_String;
}

void JsonValue::operator=(const DenseVector<String>& InputArray)
{
	if (m_Type != TypeEnum::Type_StringArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<String>>();
	(*TempPtr) = InputArray;
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_StringArray;
}

void JsonValue::operator=(DenseVector<String>&& InputArray)
{
	if (m_Type != TypeEnum::Type_StringArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseVector<String>>();
	(*TempPtr) = std::move(InputArray);
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_StringArray;
}

void JsonValue::operator=(const JsonArray& Value)
{
	if (m_Type != TypeEnum::Type_JsonArray)
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_JsonArray() = Value;
	m_Type = TypeEnum::Type_JsonArray;
}

void JsonValue::operator=(JsonArray&& Value)
{
	if (m_Type != TypeEnum::Type_JsonArray)
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_JsonArray() = std::move(Value);
	m_Type = TypeEnum::Type_JsonArray;
}

void JsonValue::operator=(const JsonObject& Value)
{
	if (m_Type != TypeEnum::Type_JsonObject)
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_JsonObject() = Value;
	m_Type = TypeEnum::Type_JsonObject;
}

void JsonValue::operator=(JsonObject&& Value)
{
	if (m_Type != TypeEnum::Type_JsonObject)
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
	}
	this->Ref_JsonObject() = std::move(Value);
	m_Type = TypeEnum::Type_JsonObject;
}

void JsonValue::operator=(const JsonValue& Value)
{
	if (m_Type != Value.m_Type)
	{
		this->m_OtherData.reset();
	}

	switch (Value.m_Type)
	{
	case TypeEnum::Type_Bool:
		m_Scalar.Bool = Value.m_Scalar.Bool;
		break;	
	case TypeEnum::Type_Int:
		m_Scalar.Int = Value.m_Scalar.Int;
		break;
	case TypeEnum::Type_LongLong:
		m_Scalar.LongLong = Value.m_Scalar.LongLong;
		break;
	case TypeEnum::Type_Float:
		m_Scalar.Float = Value.m_Scalar.Float;
		break;
	case TypeEnum::Type_Double:
		m_Scalar.Double = Value.m_Scalar.Double;
		break;
	case TypeEnum::Type_IntArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<int>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_IntArray() = Value.Ref_IntArray();
		break;
	}
	case TypeEnum::Type_LongLongArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<long long>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_LongLongArray() = Value.Ref_LongLongArray();
		break;
	}
	case TypeEnum::Type_FloatArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<float>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_FloatArray() = Value.Ref_FloatArray();
		break;
	}
	case TypeEnum::Type_DoubleArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<double>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_DoubleArray() = Value.Ref_DoubleArray();
		break;
	}
	case TypeEnum::Type_String:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<String>();
			m_OtherData.reset(TempPtr.release());
		}		
		this->Ref_String() = Value.Ref_String();
		break;
	}
	case TypeEnum::Type_StringArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<String>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_StringArray() = Value.Ref_StringArray();
		break;
	}
	case TypeEnum::Type_JsonArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonArray>();
			m_OtherData.reset(TempPtr.release());
		}		
		this->Ref_JsonArray() = Value.Ref_JsonArray();
		break;
	}
	case TypeEnum::Type_JsonObject:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonObject>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_JsonObject() = Value.Ref_JsonObject();
		break;
	}
	default:
		break;
	}
	m_Type = Value.m_Type;
}

void JsonValue::operator=(JsonValue&& Value)
{
	if (m_Type != Value.m_Type)
	{
		this->m_OtherData.reset();
	}

	switch (Value.m_Type)
	{
	case TypeEnum::Type_Bool:
		m_Scalar.Bool = Value.m_Scalar.Bool;
		break;
	case TypeEnum::Type_Int:
		m_Scalar.Int = Value.m_Scalar.Int;
		break;
	case TypeEnum::Type_LongLong:
		m_Scalar.LongLong = Value.m_Scalar.LongLong;
		break;
	case TypeEnum::Type_Float:
		m_Scalar.Float = Value.m_Scalar.Float;
		break;
	case TypeEnum::Type_Double:
		m_Scalar.Double = Value.m_Scalar.Double;
		break;
	case TypeEnum::Type_IntArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<int>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_IntArray() = std::move(Value.Ref_IntArray());
		break;
	}
	case TypeEnum::Type_LongLongArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<long long>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_LongLongArray() = std::move(Value.Ref_LongLongArray());
		break;
	}
	case TypeEnum::Type_FloatArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<float>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_FloatArray() = std::move(Value.Ref_FloatArray());
		break;
	}
	case TypeEnum::Type_DoubleArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<double>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_DoubleArray() = std::move(Value.Ref_DoubleArray());
		break;
	}
	case TypeEnum::Type_String:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<String>();
			m_OtherData.reset(TempPtr.release());
		}		
		this->Ref_String() = std::move(Value.Ref_String());
		break;
	}
	case TypeEnum::Type_StringArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<DenseVector<String>>();
			m_OtherData.reset(TempPtr.release());
		}
		this->Ref_StringArray() = std::move(Value.Ref_StringArray());
		break;
	}
	case TypeEnum::Type_JsonArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonArray>();
			m_OtherData.reset(TempPtr.release());
		}		
		this->Ref_JsonArray() = std::move(Value.Ref_JsonArray());
		break;
	}
	case TypeEnum::Type_JsonObject:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonObject>();
			m_OtherData.reset(TempPtr.release());
		}		
		this->Ref_JsonObject() = std::move(Value.Ref_JsonObject());
		break;
	}
	default:
		break;
	}
	m_Type = Value.m_Type;
}


void JsonValue::Clear()
{
	switch (m_Type)
	{
	case TypeEnum::Type_Bool:
		m_Scalar.Bool = true;
		break;
	case TypeEnum::Type_Int:
		m_Scalar.Int = 0;
		break;
	case TypeEnum::Type_LongLong:
		m_Scalar.LongLong = 0;
		break;
	case TypeEnum::Type_Float:
		m_Scalar.Float = 0;
		break;
	case TypeEnum::Type_Double:
		m_Scalar.Double = 0;
		break;
	default:
		m_OtherData.reset();
	}
	m_Type = TypeEnum::Type_Null;
}

/*
bool JsonValue::GetBool(bool DefaultValue) const
{
	if (m_Type == TypeEnum::Type_Bool)
	{
		return m_Scalar.Bool;
	}
	return DefaultValue;
}

int JsonValue::GetInt(int DefaultValue) const
{
	if (m_Type == TypeEnum::Type_Int)
	{
		return m_Scalar.Int;
	}
	return DefaultValue;
}

long long  JsonValue::GetLongLong(long long DefaultValue) const
{
	if (m_Type == TypeEnum::Type_LongLong)
	{
		return m_Scalar.LongLong;
	}
	return DefaultValue;
}

float JsonValue::GetFloat(float DefaultValue) const
{
	if (m_Type == TypeEnum::Type_Float)
	{
		return m_Scalar.Float;
	}
	return DefaultValue;
}

double JsonValue::GetDouble(double DefaultValue) const
{
	if (m_Type == TypeEnum::Type_Double)
	{
		return m_Scalar.Double;
	}
	return DefaultValue;
}

DenseVector<int> JsonValue::GetIntArray() const
{
	if (m_Type == TypeEnum::Type_IntArray)
	{
		return this->Ref_IntArray();
	}
	DenseVector<int> EmptyArray;
	return EmptyArray;
}

DenseVector<int> JsonValue::GetIntArray(const DenseVector<int>& DefaultArray) const
{
	if (m_Type == TypeEnum::Type_IntArray)
	{
		return this->Ref_IntArray();
	}
	return DefaultArray;
}

DenseVector<long long> JsonValue::GetLongLongArray() const
{
	if (m_Type == TypeEnum::Type_LongLongArray)
	{
		return this->Ref_LongLongArray();
	}
	DenseVector<long long> EmptyArray;
	return EmptyArray;
}

DenseVector<long long> JsonValue::GetLongLongArray(const DenseVector<long long>& DefaultArray) const
{
	if (m_Type == TypeEnum::Type_LongLongArray)
	{
		return this->Ref_LongLongArray();
	}
	return DefaultArray;
}

DenseVector<float> JsonValue::GetFloatArray() const
{
	if (m_Type == TypeEnum::Type_FloatArray)
	{
		return this->Ref_FloatArray();
	}
	DenseVector<float> EmptyArray;
	return EmptyArray;
}

DenseVector<float> JsonValue::GetFloatArray(const DenseVector<float>& DefaultArray) const
{
	if (m_Type == TypeEnum::Type_FloatArray)
	{
		return this->Ref_FloatArray();
	}
	return DefaultArray;
}

DenseVector<double> JsonValue::GetDoubleArray() const
{
	if (m_Type == TypeEnum::Type_DoubleArray)
	{
		return this->Ref_DoubleArray();
	}
	DenseVector<double> EmptyArray;
	return EmptyArray;
}

DenseVector<double> JsonValue::GetDoubleArray(const DenseVector<double>& DefaultArray) const
{
	if (m_Type == TypeEnum::Type_DoubleArray)
	{
		return this->Ref_DoubleArray();
	}
	return DefaultArray;
}
*/

String JsonValue::GetString() const
{
	if (m_Type == TypeEnum::Type_String)
	{
		return this->Ref_String();
	}
	String EmptyData;
	return EmptyData;
}

String JsonValue::GetString(const String& DefaultValue) const
{
	if (m_Type == TypeEnum::Type_String)
	{
		return this->Ref_String();
	}
	return DefaultValue;
}

DenseVector<String> JsonValue::GetStringArray() const
{
	if (m_Type == TypeEnum::Type_StringArray)
	{
		return this->Ref_StringArray();
	}
	DenseVector<String> EmptyArray;
	return EmptyArray;
}

DenseVector<String> JsonValue::GetStringArray(const DenseVector<String>& DefaultArray) const
{
	if (m_Type == TypeEnum::Type_StringArray)
	{
		return this->Ref_StringArray();
	}
	return DefaultArray;
}

JsonArray JsonValue::GetJsonArray() const
{
	if (m_Type == TypeEnum::Type_JsonArray)
	{
		return this->Ref_JsonArray();
	}
	JsonArray EmptyData;
	return EmptyData;
}

JsonArray JsonValue::GetJsonArray(const JsonArray& DefaultValue) const
{
	if (m_Type == TypeEnum::Type_JsonArray)
	{
		return this->Ref_JsonArray();
	}
	return DefaultValue;
}

JsonObject JsonValue::GetJsonObject() const
{
	if (m_Type == TypeEnum::Type_JsonObject)
	{
		return this->Ref_JsonObject();
	}
	JsonObject EmptyData;
	return EmptyData;
}

JsonObject JsonValue::GetJsonObject(const JsonObject& DefaultValue) const
{
	if (m_Type == TypeEnum::Type_JsonObject)
	{
		return this->Ref_JsonObject();
	}
	return DefaultValue;
}

}//namespace mdk
