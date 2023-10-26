#pragma once

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray.h>

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"

namespace mdk
{
//------------------ from JsonObject/JsonValue/JsonArray to QJsonObject/QJsonValue/QJsonArray ------------------------//
QJsonObject ConvertMDKJsonObjectToQTJsonObject(const JsonObject& JObject);
QJsonValue ConvertMDKJsonValueToQTJsonValue(const JsonValue& JValue);
QJsonArray ConvertMDKJsonArrayToQTJsonArray(const JsonArray& JArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseVector<int>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseVector<long long>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseVector<float>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseVector<double>& InputArray);
QJsonArray ConvertMDKStringArrayToQTJsonArray(const DenseVector<String>& InputArray);
//------------------ from QJsonObject/QJsonValue/QJsonArray to JsonObject/JsonValue/JsonArray ------------------------//
JsonObject ConvertQTJsonObjectToMDKJsonObject(const QJsonObject& QJObject);
JsonValue ConvertQTJsonValueToMDKJsonValue(const QJsonValue& QJValue);
JsonArray ConvertQTJsonArrayToMDKJsonArray(const QJsonArray& QJArray);

bool CheckIf_QTJsonArray_Is_ScalarArray(const QJsonArray& QJArray);
DenseVector<double> ConvertQTJsonArrayToMDKScalarArray(const QJsonArray& QJArray);

bool CheckIf_QTJsonArray_Is_StringArray(const QJsonArray& QJArray);
DenseVector<String> ConvertQTJsonArrayToMDKStringArray(const QJsonArray& QJArray);
//--------------------------------------------------------------------------------------------------------//

}//namespace mdk
