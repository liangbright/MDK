#ifndef __mdkJsonFile_cpp
#define __mdkJsonFile_cpp

#include "mdkJsonFile.h"

namespace mdk
{
JsonFile::JsonFile(const String& FilePathAndName)
{
	m_QFile = std::make_unique<QFile>();
	m_QFile->setFileName(FilePathAndName.StdString().c_str());
	if (!m_QFile->open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file to save result @ JsonFile::JsonFile(...)")
		return;
	}
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
//==========================================================================================================================//
//-------------------------------------- public function -----------------------------------------------------------------//
//==========================================================================================================================//
bool JsonFile::Save(const JsonObject& InputObject, const String& FilePathAndName, bool Flag_PreserveOrder)
{
	JsonFile OutputFile(FilePathAndName);
	auto IsOK = JsonFile::SaveJsonValueToJsonFile(InputObject, OutputFile, 0, Flag_PreserveOrder);
	OutputFile.Close();
	return IsOK;
}

bool JsonFile::Load(JsonObject& InputObject, const String& FilePathAndName)
{
	return true;
}
//==========================================================================================================================//
//-------------------------------------- private function -----------------------------------------------------------------//
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	switch (JValue.GetType())
	{
	case JsonValue::TypeEnum::Type_Empty:
		return JsonFile::SaveNameValuePairToJsonFile(Name, MDK_EMPTY, OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Bool:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.ToBool(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Int:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.ToInt(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLong:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.ToLongLong(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Float:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.ToFloat(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_Double:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.ToDouble(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_IntArray:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_IntArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLongArray:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_LongLongArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_FloatArray:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_FloatArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_DoubleArray:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_DoubleArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_String:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_String(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_JsonArray:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_JsonArray(), OutputFile, Indention, Flag_PreserveOrder);
	case JsonValue::TypeEnum::Type_JsonObject:
		return JsonFile::SaveNameValuePairToJsonFile(Name, JValue.Ref_JsonObject(), OutputFile, Indention, Flag_PreserveOrder);
	default:
		return false;
	}
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const MDK_Symbol_Empty&, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, bool Flag, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(Flag, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, int Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, long long Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, float Scalar, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, double Scalar, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(Scalar, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;
	return JsonFile::SaveJsonValueToJsonFile(IntArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;
	return JsonFile::SaveJsonValueToJsonFile(LongLongArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;
	return JsonFile::SaveJsonValueToJsonFile(FloatArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;
	return JsonFile::SaveJsonValueToJsonFile(DoubleArray, OutputFile, Indention_next);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const String& JString, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	return JsonFile::SaveJsonValueToJsonFile(JString, OutputFile);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;
	return JsonFile::SaveJsonValueToJsonFile(JArray, OutputFile, Indention_next, Flag_PreserveOrder);
}
//==========================================================================================================================//
bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "\"" << Name << "\"" << " : ";
	int_max Indention_next = Indention + Name.GetLength() + 5;	
	return JsonFile::SaveJsonValueToJsonFile(JObject, OutputFile, Indention_next, Flag_PreserveOrder);
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	switch (JValue.GetType())
	{
	case JsonValue::TypeEnum::Type_Empty:
		return true;
	case JsonValue::TypeEnum::Type_Bool:
		return JsonFile::SaveJsonValueToJsonFile(JValue.ToBool(), OutputFile);
	case JsonValue::TypeEnum::Type_Int:
		return JsonFile::SaveJsonValueToJsonFile(JValue.ToInt(), OutputFile);
	case JsonValue::TypeEnum::Type_LongLong:
		return JsonFile::SaveJsonValueToJsonFile(JValue.ToLongLong(), OutputFile);
	case JsonValue::TypeEnum::Type_Float:
		return JsonFile::SaveJsonValueToJsonFile(JValue.ToFloat(), OutputFile);
	case JsonValue::TypeEnum::Type_Double:
		return JsonFile::SaveJsonValueToJsonFile(JValue.ToDouble(), OutputFile);
	case JsonValue::TypeEnum::Type_IntArray:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_IntArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_LongLongArray:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_LongLongArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_FloatArray:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_FloatArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_DoubleArray:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_DoubleArray(), OutputFile, Indention);
	case JsonValue::TypeEnum::Type_String:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_String(), OutputFile);
	case JsonValue::TypeEnum::Type_JsonArray:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_JsonArray(), OutputFile, Indention, Flag_PreserveOrder);
	case JsonValue::TypeEnum::Type_JsonObject:
		return JsonFile::SaveJsonValueToJsonFile(JValue.Ref_JsonObject(), OutputFile, Indention, Flag_PreserveOrder);
	default:
		return false;
	}
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(bool Flag, JsonFile& OutputFile)
{
	if (Flag == true)
	{
		return JsonFile::SaveJsonValueToJsonFile(int(1), OutputFile);
	}
	else
	{
		return JsonFile::SaveJsonValueToJsonFile(int(0), OutputFile);
	}
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(int Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar).toUtf8();
	OutputFile << QStr.toStdString();
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(long long Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar); // .toUtf8();
	String str = QStr.toStdString();	
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(float Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar); // .toUtf8();
	String str = QStr.toStdString();
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(double Scalar, JsonFile& OutputFile)
{
	QString QStr = QString::number(Scalar); // .toUtf8();
	String str = QStr.toStdString();
	OutputFile << str;
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(const DenseMatrix<int>& IntArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < IntArray.GetElementNumber(); ++k)
	{
		QString QStr = QString::number(IntArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (IntArray.GetElementNumber() > 1 && k < IntArray.GetElementNumber() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxNumberPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxNumberPerLine_ScalarArray) == 0)
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
bool JsonFile::SaveJsonValueToJsonFile(const DenseMatrix<long long>& LongLongArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < LongLongArray.GetElementNumber(); ++k)
	{
		QString QStr = QString::number(LongLongArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (LongLongArray.GetElementNumber() > 1 && k < LongLongArray.GetElementNumber() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxNumberPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxNumberPerLine_ScalarArray) == 0)
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
bool JsonFile::SaveJsonValueToJsonFile(const DenseMatrix<float>& FloatArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < FloatArray.GetElementNumber(); ++k)
	{
		QString QStr = QString::number(FloatArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (FloatArray.GetElementNumber() > 1 && k < FloatArray.GetElementNumber() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxNumberPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxNumberPerLine_ScalarArray) == 0)
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
bool JsonFile::SaveJsonValueToJsonFile(const DenseMatrix<double>& DoubleArray, JsonFile& OutputFile, int_max Indention)
{
	OutputFile << "[";
	for (int_max k = 0; k < DoubleArray.GetElementNumber(); ++k)
	{
		QString QStr = QString::number(DoubleArray[k]); // .toUtf8();
		String str = QStr.toStdString();
		OutputFile << str;
		if (DoubleArray.GetElementNumber() > 1 && k < DoubleArray.GetElementNumber() - 1)
		{
			OutputFile << ", ";
		}

		if (k + 1 >= MDK_JsonFile_MaxNumberPerLine_ScalarArray && ((k + 1) % MDK_JsonFile_MaxNumberPerLine_ScalarArray) == 0)
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
bool JsonFile::SaveJsonValueToJsonFile(const String& JString, JsonFile& OutputFile)
{
	OutputFile << "\"" << JString << "\"";
	return true;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	OutputFile << "[";

	bool Flag_ScalarArray = true; // every JArray[k] is a scalar

	bool IsOK = true;

	int_max Indention_next = Indention + 1;
	for (int_max k = 0; k < JArray.GetLength(); ++k)
	{
		const auto& JValue = JArray[k];
		switch (JValue.GetType())
		{
		case JsonValue::TypeEnum::Type_Empty:
		{
			if (JArray.GetLength() > 1 && k < JArray.GetLength() - 1)
			{
				OutputFile << ',';
			}
			break;
		}
		case JsonValue::TypeEnum::Type_Bool:
		case JsonValue::TypeEnum::Type_Int:
		case JsonValue::TypeEnum::Type_LongLong:
		case JsonValue::TypeEnum::Type_Float:
		case JsonValue::TypeEnum::Type_Double:
		{
			if (JsonFile::SaveJsonValueToJsonFile(JValue, OutputFile, 0, Flag_PreserveOrder) == false) // NO Indention
			{
				IsOK = false;
			}

			if (JArray.GetLength() > 1 && k < JArray.GetLength() - 1)
			{
				OutputFile << ", ";
			}
			break;
		}
		case JsonValue::TypeEnum::Type_IntArray:
		case JsonValue::TypeEnum::Type_LongLongArray:
		case JsonValue::TypeEnum::Type_FloatArray:
		case JsonValue::TypeEnum::Type_DoubleArray:
		case JsonValue::TypeEnum::Type_String:
		case JsonValue::TypeEnum::Type_JsonArray:
		case JsonValue::TypeEnum::Type_JsonObject:
		{
			if (k >= 1)
			{
				OutputFile << '\n';
				for (int_max k = 0; k < Indention_next; ++k)
				{
					OutputFile << " ";
				}
			}
			if (JsonFile::SaveJsonValueToJsonFile(JValue, OutputFile, Indention_next, Flag_PreserveOrder) == false)
			{
				IsOK = false;
			}
			if (JArray.GetLength() > 1 && k < JArray.GetLength() - 1)
			{
				OutputFile << ", ";
			}
			Flag_ScalarArray = false;
			break;
		}
		default:
			break;
		}
	}

	if (Flag_ScalarArray == false)
	{
		OutputFile << '\n';
	}

	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "]";

	return IsOK;
}
//==========================================================================================================================//
bool JsonFile::SaveJsonValueToJsonFile(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{	
	OutputFile << '{' << '\n';

	int_max Indention_Current = Indention + 1;

	bool IsOK = true;

	if (Flag_PreserveOrder == true)
	{
		auto ItemNumber = int_max(JObject.NameList().size());

		for (int_max k = 0; k < ItemNumber; ++k)
		{
			const auto& Name_k = JObject.NameList()[k];
			auto it = JObject.DataMap().find(Name_k);
			if (it != JObject.DataMap().end())
			{
				const auto& JValue_k = it->second;
				if (JsonFile::SaveNameValuePairToJsonFile(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder) == false)
				{
					IsOK = false;
				}
			}
			else
			{
				MDK_Warning("Someting is missing @ JsonFile::SaveJsonValueToJsonFile(...)")
			}
			if (ItemNumber > 1 && k < ItemNumber - 1)
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
			if (JsonFile::SaveNameValuePairToJsonFile(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder) == false)
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

#endif