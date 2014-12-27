#ifndef __mdkJsonValue_cpp
#define __mdkJsonValue_cpp

#include "mdkJsonValue.h"
#include "mdkJsonArray.h"
#include "mdkJsonObject.h"

namespace mdk
{

JsonValue::JsonValue(Type TypeOfValue)
{
	switch (TypeOfValue)
	{
	case Type_Bool:
		m_Scalar.Bool = true;
		break;
	case Type_Int:
		m_Scalar.Int = 0;
		break;
	case Type_LongLong:
		m_Scalar.LongLong = 0;
		break;
	case Type_Float:
		m_Scalar.Float = 0;
		break;
	case Type_Double:
		m_Scalar.Double = 0;
		break;
	case Type_IntArray:
	{
		auto TempPtr = std::make_unique<DenseMatrix<int>>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_LongLongArray:
	{
		auto TempPtr = std::make_unique<DenseMatrix<long long>>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_FloatArray:
	{
		auto TempPtr = std::make_unique<DenseMatrix<float>>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_DoubleArray:
	{
		auto TempPtr = std::make_unique<DenseMatrix<double>>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_String:
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_JsonArray:
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	case Type_JsonObject:
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
		break;
	}
	default:
		break;
	}

	m_Type = TypeOfValue;
}

JsonValue::JsonValue(bool Value)
{
	m_Scalar.Bool = Value;
	m_Type = Type_Bool;
}

JsonValue::JsonValue(int Value)
{
	m_Scalar.Int = Value;
	m_Type = Type_Int;
}

JsonValue::JsonValue(long long Value)
{
	m_Scalar.LongLong = Value;
	m_Type = Type_LongLong;
}

JsonValue::JsonValue(float Value)
{
	m_Scalar.Float = Value;
	m_Type = Type_Float;
}

JsonValue::JsonValue(double Value)
{
	m_Scalar.Double = Value;
	m_Type = Type_Double;
}

JsonValue::JsonValue(const char* Value)
{
	auto TempPtr = std::make_unique<String>();
	m_OtherData.reset(TempPtr.release());
	*(m_OtherData->String) = Value;
	m_Type = Type_String;
}

JsonValue::JsonValue(String Value)
{
	auto TempPtr = std::make_unique<String>();
	m_OtherData.reset(TempPtr.release());
	*(m_OtherData->String) = std::move(Value);
	m_Type = Type_String;
}

JsonValue::JsonValue(JsonArray Value)
{
	auto TempPtr = std::make_unique<JsonArray>();
	m_OtherData.reset(TempPtr.release());
	*(m_OtherData->JArray) = std::move(Value);
	m_Type = Type_JsonArray;
}

JsonValue::JsonValue(JsonObject Value)
{
	auto TempPtr = std::make_unique<JsonObject>();
	m_OtherData.reset(TempPtr.release());
	*(m_OtherData->JObject) = std::move(Value);
	m_Type = Type_JsonObject;
}

JsonValue::JsonValue(const JsonValue& Value)
{
	(*this) = Value;
}

JsonValue::JsonValue(JsonValue&& Value)
{
	(*this) = std::move(Value);	
}

JsonValue::~JsonValue()
{
}

void JsonValue::operator=(bool Value)
{
	if (m_Type != Type_Bool)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Bool = Value;
	m_Type = Type_Bool;
}

void JsonValue::operator=(int Value)
{
	if (m_Type != Type_Int)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Int = Value;
	m_Type = Type_Int;
}

void JsonValue::operator=(long long Value)
{
	if (m_Type != Type_LongLong)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.LongLong = Value;
	m_Type = Type_LongLong;
}

void JsonValue::operator=(float Value)
{
	if (m_Type != Type_Float)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Float = Value;
	m_Type = Type_Float;
}

void JsonValue::operator=(double Value)
{
	if (m_Type != Type_Double)
	{
		this->m_OtherData.reset();
	}
	m_Scalar.Double = Value;
	m_Type = Type_Double;
}

void JsonValue::operator=(const char* Value)
{
	if (m_Type != Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}		
	*(m_OtherData->String) = Value;
	m_Type = Type_String;
}

void JsonValue::operator=(const String& Value)
{
	if (m_Type != Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->String) = Value;
	m_Type = Type_String;
}

void JsonValue::operator=(String&& Value)
{
	if (m_Type != Type_String)
	{
		auto TempPtr = std::make_unique<String>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->String) = std::move(Value);
	m_Type = Type_String;
}

void JsonValue::operator=(const JsonArray& Value)
{
	if (m_Type != Type_JsonArray)
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->JArray) = Value;
	m_Type = Type_JsonArray;
}

void JsonValue::operator=(JsonArray&& Value)
{
	if (m_Type != Type_JsonArray)
	{
		auto TempPtr = std::make_unique<JsonArray>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->JArray) = std::move(Value);
	m_Type = Type_JsonArray;
}

void JsonValue::operator=(const JsonObject& Value)
{
	if (m_Type != Type_JsonObject)
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->JObject) = Value;
	m_Type = Type_JsonObject;
}

void JsonValue::operator=(JsonObject&& Value)
{
	if (m_Type != Type_JsonObject)
	{
		auto TempPtr = std::make_unique<JsonObject>();
		m_OtherData.reset(TempPtr.release());
	}
	*(m_OtherData->JObject) = std::move(Value);
	m_Type = Type_JsonObject;
}

void JsonValue::operator=(const JsonValue& Value)
{
	if (m_Type != Value.m_Type)
	{
		this->m_OtherData.reset();
	}

	switch (Value.m_Type)
	{
	case Type_Bool:
		m_Scalar.Bool = Value.m_Scalar.Bool;
		break;	
	case Type_Int:
		m_Scalar.Int = Value.m_Scalar.Int;
		break;
	case Type_LongLong:
		m_Scalar.LongLong = Value.m_Scalar.LongLong;
		break;
	case Type_Float:
		m_Scalar.Float = Value.m_Scalar.Float;
		break;
	case Type_Double:
		m_Scalar.Double = Value.m_Scalar.Double;
		break;
	case Type_String:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<String>();
			m_OtherData.reset(TempPtr.release());
		}		
		*(m_OtherData->String) = *(Value.m_OtherData->String);
		break;
	}
	case Type_JsonArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonArray>();
			m_OtherData.reset(TempPtr.release());
		}		
		*(m_OtherData->JArray) = *(Value.m_OtherData->JArray);
	}
	case Type_JsonObject:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonObject>();
			m_OtherData.reset(TempPtr.release());
		}
		*(m_OtherData->JObject) = *(Value.m_OtherData->JObject);
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
	case Type_Bool:
		m_Scalar.Bool = Value.m_Scalar.Bool;
		break;
	case Type_Int:
		m_Scalar.Int = Value.m_Scalar.Int;
		break;
	case Type_LongLong:
		m_Scalar.LongLong = Value.m_Scalar.LongLong;
		break;
	case Type_Float:
		m_Scalar.Float = Value.m_Scalar.Float;
		break;
	case Type_Double:
		m_Scalar.Double = Value.m_Scalar.Double;
		break;
	case Type_String:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<String>();
			m_OtherData.reset(TempPtr.release());
		}		
		*(m_OtherData->String) = std::move(*(Value.m_OtherData->String));
		break;
	}
	case Type_JsonArray:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonArray>();
			m_OtherData.reset(TempPtr.release());
		}		
		*(m_OtherData->JArray) = std::move(*(Value.m_OtherData->JArray));
	}
	case Type_JsonObject:
	{
		if (!m_OtherData)
		{
			auto TempPtr = std::make_unique<JsonObject>();
			m_OtherData.reset(TempPtr.release());
		}		
		*(m_OtherData->JObject) = std::move(*(Value.m_OtherData->JObject));
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
	case Type_Bool:
		m_Scalar.Bool = true;
		break;
	case Type_Int:
		m_Scalar.Int = 0;
		break;
	case Type_LongLong:
		m_Scalar.LongLong = 0;
		break;
	case Type_Float:
		m_Scalar.Float = 0;
		break;
	case Type_Double:
		m_Scalar.Double = 0;
		break;
	default:
		m_OtherData.reset();
		break;
	}
	m_Type = Type_Undefined;
}

bool JsonValue::ToBool(bool DefaultValue) const
{
	if (m_Type == Type_Bool)
	{
		return m_Scalar.Bool;
	}
	return DefaultValue;
}

int JsonValue::ToInt(int DefaultValue) const
{
	if (m_Type == Type_Int)
	{
		return m_Scalar.Int;
	}
	return DefaultValue;
}

long long  JsonValue::ToLongLong(long long DefaultValue) const
{
	if (m_Type == Type_LongLong)
	{
		return m_Scalar.LongLong;
	}
	return DefaultValue;
}

float JsonValue::ToFloat(float DefaultValue) const
{
	if (m_Type == Type_Float)
	{
		return m_Scalar.Float;
	}
	return DefaultValue;
}

double JsonValue::ToDouble(double DefaultValue) const
{
	if (m_Type == Type_Double)
	{
		return m_Scalar.Double;
	}
	return DefaultValue;
}

String JsonValue::ToString() const
{
	if (m_Type == Type_String)
	{
		return *(static_cast<String*>(m_OtherData.get()));
	}
	String EmptyData;
	return EmptyData;
}

String JsonValue::ToString(const String& DefaultValue) const
{
	if (m_Type == Type_String)
	{
		return *(static_cast<String*>(m_OtherData.get()));
	}
	return DefaultValue;
}

JsonArray JsonValue::ToJsonArray() const
{
	if (m_Type == Type_JsonArray)
	{
		return *(static_cast<JsonArray*>(m_OtherData.get()));
	}
	JsonArray EmptyData;
	return EmptyData;
}

JsonArray JsonValue::ToJsonArray(const JsonArray& DefaultValue) const
{
	if (m_Type == Type_JsonArray)
	{
		return *(static_cast<JsonArray*>(m_OtherData.get()));
	}
	return DefaultValue;
}

JsonObject JsonValue::ToJsonObject() const
{
	if (m_Type == Type_JsonObject)
	{
		return *(static_cast<JsonObject*>(m_OtherData.get()));
	}
	JsonObject EmptyData;
	return EmptyData;
}

JsonObject JsonValue::ToJsonObject(const JsonObject& DefaultValue) const
{
	if (m_Type == Type_JsonObject)
	{
		return *(static_cast<JsonObject*>(m_OtherData.get()));
	}
	return DefaultValue;
}


}//namespace mdk

#endif