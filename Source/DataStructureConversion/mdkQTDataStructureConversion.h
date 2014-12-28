#ifndef __mdkQTDataStructureConversion_h
#define __mdkQTDataStructureConversion_h

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

QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<int>& IntArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<long long>& LongLongArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<float>& FloatArray);
QJsonArray ConvertMDKScalarArrayToQTJsonArray(const DenseMatrix<double>& DoubleArray);

QJsonArray ConvertMDKJsonArrayToQTJsonArray(const JsonArray& JArray);

//------------------ from QJsonObject/QJsonValue/QJsonArray to JsonObject/JsonValue/JsonArray ------------------------//

JsonObject ConvertQTJsonObjectToMDKJsonObject(const QJsonObject& QJObject);
JsonValue ConvertQTJsonValueToMDKJsonValue(const QJsonValue& QJValue);
//--------------------------------------------------------------------------------------------------------//

}


#endif
