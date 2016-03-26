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

QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<int>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<long long>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<float>& InputArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<double>& InputArray);

QJsonArray ConvertMDKJsonArrayToQTJsonArray(const JsonArray& JArray);

//------------------ from QJsonObject/QJsonValue/QJsonArray to JsonObject/JsonValue/JsonArray ------------------------//

JsonObject ConvertQTJsonObjectToMDKJsonObject(const QJsonObject& QJObject);

JsonValue ConvertQTJsonValueToMDKJsonValue(const QJsonValue& QJValue);

bool CheckIf_QTJsonArray_Is_ScalarArray(const QJsonArray& QJArray);

DenseMatrix<double> ConvertQTJsonArrayToMDKScalarArray(const QJsonArray& QJArray);

JsonArray ConvertQTJsonArrayToMDKJsonArray(const QJsonArray& QJArray);

//--------------------------------------------------------------------------------------------------------//

}//namespace mdk

