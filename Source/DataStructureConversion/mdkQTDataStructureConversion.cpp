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
	case JsonValue::TypeEnum::Type_Null:
	{
		QJsonValue QJValue;
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Bool:
	{
		QJsonValue QJValue(JValue.GetBool());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Int:
	{
		QJsonValue QJValue(JValue.GetInt());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_LongLong:
	{
		QJsonValue QJValue(JValue.GetLongLong());
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Float:
	{
		QJsonValue QJValue(double(JValue.GetFloat()));
		return QJValue;
	}
	case JsonValue::TypeEnum::Type_Double:
	{
		QJsonValue QJValue(JValue.GetDouble());
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


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<int>& InputArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < InputArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(InputArray[k]));
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<long long>& InputArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < InputArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(InputArray[k]));
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<float>& InputArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < InputArray.GetElementNumber(); ++k)
	{
		QJArray.append(QJsonValue(double(InputArray[k]))); // QJsonValue(float) is not defined
	}
	return QJArray;
}


QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<double>& InputArray)
{
	QJsonArray QJArray;
	for (int_max k = 0; k < InputArray.GetElementNumber(); ++k)
	{
		QJsonValue QJValue(InputArray[k]);
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
	for (auto it = QJObject.begin(); it != QJObject.end(); ++it)
	{
		String Name = it.key().toStdString();
		JsonValue JValue = ConvertQTJsonValueToMDKJsonValue(it.value());
		JObject[Name] = JValue;
	}
	return JObject;
}


JsonValue ConvertQTJsonValueToMDKJsonValue(const QJsonValue& QJValue)
{
	JsonValue JValue;
	switch (QJValue.type())
	{
	case QJsonValue::Type::Null:
	{
		break;
	}
	case QJsonValue::Type::Bool:
	{
		JValue = QJValue.toBool();
		break;
	}
	case QJsonValue::Type::Double:
	{
		JValue = QJValue.toDouble();
		break;
	}
	case QJsonValue::Type::String:
	{
		JValue = QJValue.toString().toStdString();
		break;
	}
	case QJsonValue::Type::Array:
	{
		auto QJArray = QJValue.toArray();
		if (CheckIf_QTJsonArray_Is_ScalarArray(QJArray) == true)
		{
			JValue = ConvertQTJsonArrayToMDKScalarArray(QJArray);
		}
		else
		{
			JValue = ConvertQTJsonArrayToMDKJsonArray(QJArray);
		}		
		break;
	}
	case QJsonValue::Type::Object:
	{
		auto QJObject = QJValue.toObject();
		JValue = ConvertQTJsonObjectToMDKJsonObject(QJObject);
		break;
	}
	case QJsonValue::Type::Undefined:
	{
		break;
	}
	default:
		break;
	}
	return JValue;
}

bool CheckIf_QTJsonArray_Is_ScalarArray(const QJsonArray& QJArray)
{
	bool Flag = true;
	if (QJArray.size() == 0)
	{// if QJArray is empty, then it is treated as ScalarArray
		Flag = true;
	}
	else
	{
		for (auto it = QJArray.begin(); it != QJArray.end(); ++it)
		{
			if ((*it).type() != QJsonValue::Type::Double)// QT: only Double scalar
			{
				Flag = false;
				break;
			}
		}
	}
	return Flag;
}

DenseMatrix<double> ConvertQTJsonArrayToMDKScalarArray(const QJsonArray& QJArray)
{
	DenseMatrix<double> OutputArray;
	OutputArray.ReserveCapacity(1, int_max(QJArray.size()));
	for (auto it = QJArray.begin(); it != QJArray.end(); ++it)
	{
		if ((*it).type() != QJsonValue::Type::Double)
		{			
			MDK_Error("Type is not double @ ConvertQTJsonArrayToMDKScalarArray(...)")
			OutputArray.Clear();
			return OutputArray;
		}
		OutputArray.Append((*it).toDouble());
	}
	OutputArray.Resize(1, OutputArray.GetElementNumber());
	return OutputArray;
}

JsonArray ConvertQTJsonArrayToMDKJsonArray(const QJsonArray& QJArray)
{
	JsonArray JArray;
	JArray.ReserveCapacity(int_max(QJArray.size()));
	for (auto it = QJArray.begin(); it != QJArray.end(); ++it)
	{		
		JArray.Append(ConvertQTJsonValueToMDKJsonValue(*it));
	}
	return JArray;
}

//--------------------------------------------------------------------------------------------------------//

}

#endif
