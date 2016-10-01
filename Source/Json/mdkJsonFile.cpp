#pragma once

#include "mdkJsonFile.h"

namespace mdk
{
JsonFile::JsonFile(const String& FilePathAndName)
{
	m_QFile = std::make_unique<QFile>();
	m_QFile->setFileName(FilePathAndName.StdString().c_str());
	if (!m_QFile->open(QIODevice::ReadWrite))
	{
		MDK_Error("Couldn't open file @ JsonFile::JsonFile(...)")
		return;
	}
	m_QFile->resize(0);//clear the file just in case it already exist
	m_QTextStream = std::make_unique<QTextStream>(m_QFile.get());
}

JsonFile::~JsonFile()
{
}

JsonFile& JsonFile::operator<<(char InputChar)
{
	*m_QTextStream << InputChar;
	return *this;
}

JsonFile& JsonFile::operator<<(const char* InputString)
{
	*m_QTextStream << InputString;
	return *this;
}

JsonFile& JsonFile::operator<<(const String& InputString)
{
	*m_QTextStream << InputString.StdString().c_str();
	return *this;
}

void JsonFile::Close()
{
	if (m_QTextStream)
	{
		m_QTextStream->flush();
		m_QFile->close();
	}
}
//==============================================================================================================================//
//-------------------------------------- public static function -----------------------------------------------------------------//
//===============================================================================================================================//
bool JsonFile::Save(const JsonObject& InputJObject, const String& FilePathAndName, bool Flag_PreserveOrder)
{
	JsonFile OutputFile(FilePathAndName);
	if (OutputFile.m_QTextStream)
	{
		auto IsOK = JsonFile::SaveJsonValue(InputJObject, OutputFile, 0, Flag_PreserveOrder);
		OutputFile.Close();
		return IsOK;
	}
	return false;
}
//==========================================================================================================================//
bool JsonFile::Load(JsonObject& OutputJObject, const String& FilePathAndName)
{
	QFile JFile_in(FilePathAndName.StdString().c_str());
	if (!JFile_in.open(QIODevice::ReadOnly))
	{
		MDK_Error("Can not open file to read @ JsonFile::Load(...) " << FilePathAndName);
		return false;
	}
	QByteArray QBArray_in = JFile_in.readAll();
	QJsonDocument JDoc_in(QJsonDocument::fromJson(QBArray_in));
	QJsonObject QJObject_in = JDoc_in.object();
	OutputJObject = ConvertQTJsonObjectToMDKJsonObject(QJObject_in);
	return true;
}
//===============================================================================================================================//
bool JsonFile::Save(const JsonArray& InputJArray, const String& FilePathAndName, bool Flag_PreserveOrder)
{
	JsonFile OutputFile(FilePathAndName);
	auto IsOK = JsonFile::SaveJsonValue(InputJArray, OutputFile, 0, Flag_PreserveOrder);
	OutputFile.Close();
	return IsOK;
}
//==========================================================================================================================//
bool JsonFile::Load(JsonArray& OutputJArray, const String& FilePathAndName)
{
	QFile JFile_in(FilePathAndName.StdString().c_str());
	if (!JFile_in.open(QIODevice::ReadOnly))
	{
		MDK_Error("Can not open file to read @ JsonFile::Load(...) " << FilePathAndName);
		return false;
	}
	QByteArray QBArray_in = JFile_in.readAll();
	QJsonDocument JDoc_in(QJsonDocument::fromJson(QBArray_in));
	QJsonArray QJArray_in = JDoc_in.array();
	OutputJArray = ConvertQTJsonArrayToMDKJsonArray(QJArray_in);
	return true;
}
//==========================================================================================================================//
//-------------------------------------- private function -----------------------------------------------------------------//
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	switch (JValue.GetType())
	{
	case JsonValue::TypeEnum::Type_Null:
		return JsonFile::SaveNameValuePair(Name, MDK_EMPTY, OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Bool:
		return JsonFile::SaveNameValuePair(Name, JValue.ToScalar<bool>(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Int:
		return JsonFile::SaveNameValuePair(Name, JValue.ToScalar<int>(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLong:
		return JsonFile::SaveNameValuePair(Name, JValue.ToScalar<long long>(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Float:
		return JsonFile::SaveNameValuePair(Name, JValue.ToScalar<float>(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Double:
		return JsonFile::SaveNameValuePair(Name, JValue.ToScalar<double>(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_IntArray:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_IntArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLongArray:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_LongLongArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_FloatArray:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_FloatArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_DoubleArray:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_DoubleArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_String:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_String(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_JsonArray:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_JsonArray(), OutputFile, Indention, Flag_PreserveOrder);
	case JsonValue::TypeEnum::Type_JsonObject:
		return JsonFile::SaveNameValuePair(Name, JValue.Ref_JsonObject(), OutputFile, Indention, Flag_PreserveOrder);
	default:
		return false;
	}
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const MDK_Symbol_Empty&, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(MDK_EMPTY, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, bool Flag, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(Flag, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, int Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, long long Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, float Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, double Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;
	return JsonFile::SaveJsonValue(IntArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;
	return JsonFile::SaveJsonValue(LongLongArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;
	return JsonFile::SaveJsonValue(FloatArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;
	return JsonFile::SaveJsonValue(DoubleArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const String& JString, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	return JsonFile::SaveJsonValue(JString, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;
	return JsonFile::SaveJsonValue(JArray, OutputFile, Indention_next, Flag_PreserveOrder);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePair(const String& Name, const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << ": ";
	int_max Indention_next = Indention + Name.GetCharCount() + 4;	
	return JsonFile::SaveJsonValue(JObject, OutputFile, Indention_next, Flag_PreserveOrder);
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	switch (JValue.GetType())
	{
	case JsonValue::TypeEnum::Type_Null:
		return SaveJsonValue(MDK_EMPTY, OutputFile);
	case JsonValue::TypeEnum::Type_Bool:
		return JsonFile::SaveJsonValue(JValue.ToScalar<bool>(), OutputFile);
	case JsonValue::TypeEnum::Type_Int:
		return JsonFile::SaveJsonValue(JValue.ToScalar<int>(), OutputFile);
	case JsonValue::TypeEnum::Type_LongLong:
		return JsonFile::SaveJsonValue(JValue.ToScalar<long long>(), OutputFile);
	case JsonValue::TypeEnum::Type_Float:
		return JsonFile::SaveJsonValue(JValue.ToScalar<float>(), OutputFile);
	case JsonValue::TypeEnum::Type_Double:
		return JsonFile::SaveJsonValue(JValue.ToScalar<double>(), OutputFile);
	case JsonValue::TypeEnum::Type_IntArray:
		return JsonFile::SaveJsonValue(JValue.Ref_IntArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLongArray:
		return JsonFile::SaveJsonValue(JValue.Ref_LongLongArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_FloatArray:
		return JsonFile::SaveJsonValue(JValue.Ref_FloatArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_DoubleArray:
		return JsonFile::SaveJsonValue(JValue.Ref_DoubleArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_String:
		return JsonFile::SaveJsonValue(JValue.Ref_String(), OutputFile);
	case JsonValue::TypeEnum::Type_JsonArray:
		return JsonFile::SaveJsonValue(JValue.Ref_JsonArray(), OutputFile, Indention, Flag_PreserveOrder);
	case JsonValue::TypeEnum::Type_JsonObject:
		return JsonFile::SaveJsonValue(JValue.Ref_JsonObject(), OutputFile, Indention, Flag_PreserveOrder);
	default:
		return false;
	}
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const MDK_Symbol_Empty&, JsonFile& OutputFile)
{
	String NullStr = "null";
	OutputFile << NullStr;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(bool Flag, JsonFile& OutputFile)
{
	if (Flag == true)
	{
		return JsonFile::SaveJsonValue(int(1), OutputFile);
	}
	else
	{
		return JsonFile::SaveJsonValue(int(0), OutputFile);
	}
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(int Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar); // .toUtf8();
	OutputFile << QStr.toStdString();
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(long long Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar); // .toUtf8();
	String str = QStr.toStdString();	
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(float Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar, 'g', 8); // .toUtf8();
	String str = QStr.toStdString();
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(double Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar, 'g', 16); // .toUtf8();
	String str = QStr.toStdString();
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < IntArray.GetElementCount(); ++k)
	{
		QString QStr = QString::number(IntArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (IntArray.GetElementCount() > 1 && k < IntArray.GetElementCount() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxCountPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxCountPerLine_ScalarArray) == 0)
		{
			OutputFile << '\n';
			for (int_max k = 0; k < Indention + 1; ++k)
			{
				OutputFile << " ";
			}
		}
	}
	OutputFile << "]";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < LongLongArray.GetElementCount(); ++k)
	{
		QString QStr = QString::number(LongLongArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (LongLongArray.GetElementCount() > 1 && k < LongLongArray.GetElementCount() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxCountPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxCountPerLine_ScalarArray) == 0)
		{
			OutputFile << '\n';
			for (int_max k = 0; k < Indention + 1; ++k)
			{
				OutputFile << " ";
			}
		}
	}
	OutputFile << "]";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < FloatArray.GetElementCount(); ++k)
	{
		QString QStr = QString::number(FloatArray[k], 'g', 8); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (FloatArray.GetElementCount() > 1 && k < FloatArray.GetElementCount() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxCountPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxCountPerLine_ScalarArray) == 0)
		{
			OutputFile << '\n';
			for (int_max k = 0; k < Indention + 1; ++k)
			{
				OutputFile << " ";
			}
		}
	}
	OutputFile << "]";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < DoubleArray.GetElementCount(); ++k)
	{
		QString QStr = QString::number(DoubleArray[k], 'g', 16); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (DoubleArray.GetElementCount() > 1 && k < DoubleArray.GetElementCount() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxCountPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxCountPerLine_ScalarArray) == 0)
		{
			OutputFile << '\n';
			for (int_max k = 0; k < Indention + 1; ++k)
			{
				OutputFile << " ";
			}
		}
	}
	OutputFile << "]";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const String& JString, JsonFile& OutputFile)
{
	OutputFile << "\"" << JString << "\"";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	bool IsOK = true;
	//----------------------------------------------
	bool Flag_SimpleArray = true; // JArray is empty, or each JArray[k] is a scalar or Null or bool
	for (int_max k = 0; k < JArray.GetLength(); ++k)
	{
		const auto& JValue = JArray[k];
		switch (JValue.GetType())
		{
		case JsonValue::TypeEnum::Type_IntArray:
		case JsonValue::TypeEnum::Type_LongLongArray:
		case JsonValue::TypeEnum::Type_FloatArray:
		case JsonValue::TypeEnum::Type_DoubleArray:
		case JsonValue::TypeEnum::Type_String:
		case JsonValue::TypeEnum::Type_JsonArray:
		case JsonValue::TypeEnum::Type_JsonObject:
			Flag_SimpleArray = false;
			break;
		}
		if (Flag_SimpleArray == false)
		{
			break;
		}
	}
	//----------------------------------------------
	if (Flag_SimpleArray == true)
	{
		OutputFile << "[";
	}
	else
	{
		OutputFile << "[" << '\n';
	}
	//----------------------------------------------
	int_max Indention_next = Indention + 1;
	for (int_max k = 0; k < JArray.GetLength(); ++k)
	{
		const auto& JValue = JArray[k];
		if (Flag_SimpleArray == true)
		{
			if (JsonFile::SaveJsonValue(JValue, OutputFile, 0, Flag_PreserveOrder) == false) // NO Indention
			{
				IsOK = false;
			}

			if (JArray.GetLength() > 1 && k < JArray.GetLength() - 1)
			{
				OutputFile << ", ";
			}
		}
		else
		{
			for (int_max n = 0; n < Indention_next; ++n)
			{
				OutputFile << " ";
			}
			if (JsonFile::SaveJsonValue(JValue, OutputFile, Indention_next, Flag_PreserveOrder) == false)
			{
				IsOK = false;
			}
			if (JArray.GetLength() > 1 && k < JArray.GetLength() - 1)
			{
				OutputFile << "," << '\n';
			}
		}
	}
	//----------------------------------------------
	if (Flag_SimpleArray == true)
	{
		OutputFile << "]";
	}
	else
	{
		OutputFile << '\n';
		for (int_max k = 0; k < Indention; ++k)
		{
			OutputFile << " ";
		}
		OutputFile << "]";
	}
	//----------------------------------------------
	return IsOK;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValue(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{	
	OutputFile << '{' << '\n';

	int_max Indention_Current = Indention + 1;

	bool IsOK = true;

	if (Flag_PreserveOrder == true)
	{
		auto ItemCount = int_max(JObject.NameList().size());

		for (int_max k = 0; k < ItemCount; ++k)
		{
			const auto& Name_k = JObject.NameList()[k];
			auto it = JObject.DataMap().find(Name_k);
			if (it != JObject.DataMap().end())
			{
				const auto& JValue_k = it->second;
				if (JsonFile::SaveNameValuePair(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder) == false)
				{
					IsOK = false;
				}
			}
			else
			{
				MDK_Warning("Someting is missing @ JsonFile::SaveJsonValue(...)")
			}
			if (ItemCount > 1 && k < ItemCount - 1)
			{
				OutputFile << ',' << '\n';
			}
		}
	}
	else
	{
		int_max k = 0;
		for (auto it = JObject.DataMap().begin(); it != JObject.DataMap().end(); ++it)
		{
			const auto& Name_k = it->first;
			const auto& JValue_k = it->second;
			if (JsonFile::SaveNameValuePair(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder) == false)
			{
				IsOK = false;
			}
			if (k < int_max(JObject.NameList().size()) - 1)
			{
				OutputFile << ',' << '\n';
			}
			k += 1;
		}
	}
	
	OutputFile << '\n';
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << '}';

	return IsOK;
}
//==========================================================================================================================//
}//namespace mdk
