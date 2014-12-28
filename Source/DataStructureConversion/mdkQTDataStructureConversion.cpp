#ifndef __mdkQTDataStructureConversion_cpp
#define __mdkQTDataStructureConversion_cpp

#include "mdkQTDataStructureConversion.h"

namespace mdk
{
//------------------ from JsonObject/JsonValue/JsonArray to QJsonObject/QJsonValue/QJsonArray ------------------------//

QJsonObject ConvertMDKJsonObjectToQTJsonObject(const JsonObject& JObject)
{
	QJsonObject QJObject;
	for (auto it = JObject.begin(); it != JObject.end(); ++it)
	{
		const auto& Name = it->first;
		const auto& JValue = it->second;
		QString QName(Name.StdString().c_str());
		QJsonValue QJValue = ConvertMDKJsonValueToQTJsonValue(JValue);
		QJObject[QName] = QJValue;
	}
	return QJObject;
}


QJsonValue ConvertMDKJsonValueToQTJsonValue(const JsonValue& JValue)
{
	switch (JValue.GetType())
	{
	case JsonValue::TypeEnum::Type_Empty:
	{
		QJsonValue QJValue;
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Bool:
	{
		QJsonValue QJValue(JValue.ToBool());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Int:
	{
		QJsonValue QJValue(JValue.ToInt());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_LongLong:
	{
		QJsonValue QJValue(JValue.ToLongLong());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Float:
	{
		QJsonValue QJValue(double(JValue.ToFloat()));
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Double:
	{
		QJsonValue QJValue(JValue.ToDouble());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_IntArray:
	{
		QJsonArray QJArray = ConvertMDKScalarArrayToQTJsonArray(JValue.Ref_IntArray());
		QJsonValue QJValue(QJArray);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_LongLongArray:
	{
		QJsonArray QJArray = ConvertMDKScalarArrayToQTJsonArray(JValue.Ref_LongLongArray());
		QJsonValue QJValue(QJArray);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_FloatArray:
	{
		QJsonArray QJArray = ConvertMDKScalarArrayToQTJsonArray(JValue.Ref_FloatArray());
		QJsonValue QJValue(QJArray);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_DoubleArray:
	{
		QJsonArray QJArray = ConvertMDKScalarArrayToQTJsonArray(JValue.Ref_DoubleArray());
		QJsonValue QJValue(QJArray);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_String:
	{
		QString S(JValue.Ref_String().StdString().c_str());
		QJsonValue QJValue(S);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_JsonArray:
	{
		QJsonArray QJArray = ConvertMDKJsonArrayToQTJsonArray(JValue.Ref_JsonArray());
		QJsonValue QJValue(QJArray);
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_JsonObject:
	{
		QJsonObject QJObject = ConvertMDKJsonObjectToQTJsonObject(JValue.Ref_JsonObject());
		QJsonValue QJValue(QJObject);
		return QJValue;
	}
	default:
		MDK_Error("Somthing is wrong here @ ConvertMDKJsonValueToQTJsonValue(...)")
		QJsonValue QJValue;
		return QJValue;
	}
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<int>& IntArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < IntArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(IntArray[k]));
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<long long>& LongLongArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < LongLongArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(LongLongArray[k]));
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<float>& FloatArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < FloatArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(double(FloatArray[k]))); // QJsonValue(float) is not defined
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<double>& DoubleArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < DoubleArray.GetElementNumber(); ++k)
	{
		QJsonValue QJValue(DoubleArray[k]);
		QJArray.append(QJValue);
	}
	return QJArray;
}


QJsonArray ConvertMDKJsonArrayToQTJsonArray(const JsonArray& JArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < JArray.GetElementNumber(); ++k)
	{
		QJsonValue QJValue = ConvertMDKJsonValueToQTJsonValue(JArray[k]);
		QJArray.append(QJValue);
	}
	return QJArray;
}

//------------------ from QJsonObject/QJsonValue/QJsonArray to JsonObject/JsonValue/JsonArray ------------------------//

JsonObject ConvertQTJsonObjectToMDKJsonObject(const QJsonObject& QJObject)
{
	JsonObject JObject;
	return JObject;
}


JsonValue ConvertQTJsonValueToMDKJsonValue(const QJsonValue& QJValue)
{
	JsonValue JValue;
	return JValue;
}

//--------------------------------------------------------------------------------------------------------//

}

#endif
