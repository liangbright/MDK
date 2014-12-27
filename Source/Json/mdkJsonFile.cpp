#ifndef __mdkJsonFile_cpp
#define __mdkJsonFile_cpp

#include "mdkJsonFile.h"

namespace mdk
{
JsonFile::JsonFile(const String& FilePathAndName)
{
	QFile OutputFile;
	OutputFile.setFileName(FilePathAndName.c_str());
	if (!OutputFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file to save result @ JsonFile::JsonFile(...)")
		return;
	}
	m_QTextStream = std::make_unique<QTextStream>(&OutputFile);
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

//-------------------------------------- static function -----------------------------------------------------------------//

bool JsonFile::Save(const JsonObject& InputObject, const String& FilePathAndName, bool Flag_PreserveOrder)
{
	JsonFile OutputFile(FilePathAndName);
	return JsonFile::SaveJsonValueToJsonFile(InputObject, OutputFile, 0, Flag_PreserveOrder);
}

bool JsonFile::Load(JsonObject& InputObject, const String& FilePathAndName)
{
	return true;
}

bool JsonFile::SaveNameValuePairToJsonFile(const String& Name, const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}

	OutputFile << Name << " : ";

	int_max Indention_next = Indention + Name.GetLength() + 2;

	switch (JValue.GetType())
	{
	case JsonValue::Type::Type_Int:
		if (JsonFile::SaveJsonValueToJsonFile(JValue.ToInt(), OutputFile, Indention_next) == false)
		{
			return false;
		}
		break;
	case JsonValue::Type::Type_String:
		if (JsonFile::SaveJsonValueToJsonFile(JValue.Ref_String(), OutputFile, Indention_next) == false)
		{
			return false;
		}
		break;
	case JsonValue::Type::Type_JsonArray:
		if (JsonFile::SaveJsonValueToJsonFile(JValue.Ref_JsonArray(), OutputFile, Indention_next, Flag_PreserveOrder) == false)
		{
			return false;
		}
		break;
	case JsonValue::Type::Type_JsonObject:
		if (JsonFile::SaveJsonValueToJsonFile(JValue.Ref_JsonObject(), OutputFile, Indention_next, Flag_PreserveOrder) == false)
		{
			return false;
		}
		break;
	}
}

bool JsonFile::SaveJsonValueToJsonFile(int Value, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	QString QStr = QString::number(Value).toUtf8();
	OutputFile << QStr.toStdString() << '\n';
	return true;
}

bool JsonFile::SaveJsonValueToJsonFile(long long Value, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	QString QStr = QString::number(Value).toUtf8();
	String str = QStr.toStdString();	
	OutputFile << str << '\n';
	return true;
}

bool JsonFile::SaveJsonValueToJsonFile(const String& Value, JsonFile& OutputFile, int_max Indention)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << Value << '\n';
	return true;
}


bool JsonFile::SaveJsonValueToJsonFile(const JsonArray& JArray, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "[";

	int_max Indention_next = Indention + 1;
	for (int_max k = 0; k < JArray.GetLength(); ++k)
	{
		const auto& JValue = JArray[k];
		JsonFile::SaveJsonValueToJsonFile(JValue, OutputFile, Indention_next, Flag_PreserveOrder);
		if (k < JArray.GetLength() - 1)
		{
			OutputFile << ',' << '\n';
		}
	}

	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "]";
}

bool JsonFile::SaveJsonValueToJsonFile(const JsonObject& JObject, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{
	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << "{";
	
	int_max Indention_Current = Indention + 2;

	if (Flag_PreserveOrder == true)
	{
		for (int_max k = 0; k < int_max(JObject.NameList().size()); ++k)
		{
			const auto& Name_k = JObject.NameList()[k];
			auto it = JObject.DataMap().find(Name_k);
			if (it != JObject.DataMap().end())
			{
				const auto& JValue_k = it->second;
				JsonFile::SaveNameValuePairToJsonFile(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder);
			}
			if (k < int_max(JObject.NameList().size()) - 1)
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
			JsonFile::SaveNameValuePairToJsonFile(Name_k, JValue_k, OutputFile, Indention_Current, Flag_PreserveOrder);
			
			if (k < int_max(JObject.NameList().size()) - 1)
			{
				OutputFile << ',' << '\n';
			}
			k += 1;
		}
	}

	for (int_max k = 0; k < Indention; ++k)
	{
		OutputFile << " ";
	}
	OutputFile << '}';

	return true;
}

bool JsonFile::SaveJsonValueToJsonFile(const JsonValue& JValue, JsonFile& OutputFile, int_max Indention, bool Flag_PreserveOrder)
{

}

}//namespace mdk

#endif