#ifndef __mdkFileIO_Json_h
#define __mdkFileIO_Json_h

#include <string>

#include <QString.h>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkString.h"
#include "mdkObjectArray.h"

namespace mdk
{

//#define std::string String

//=======================================================================================================================================================//
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const String& Value);
// scalar
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const char& Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const int& Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const short& Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const long& Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const long long & Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const float& Value);
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const double& Value);
// array
inline String ConvertNameValuePairToStringInJsonFile(const String& Name, const int* ValueArray, int_max ArrayLength);

inline bool SaveJsonContentToJsonFile(const DataArray<String>& JsonContent, const String& FilePathAndName, bool Flag_Append = false);

//=======================================================================================================================================================//
// write data information as *.json file
// For example: FilePathAndName is "C:/Data/something.json"

struct NameValueQStringPair
{
	QString  Name;
	QString  Value;
};

inline bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName, bool Flag_Append = false);

struct NameValueStringPair
{
	String  Name;
	String  Value;
};

inline bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueStringPair>& PairList, const String& FilePathAndName, bool Flag_Append = false);

}//namespace mdk

#include "mdkFileIO_Json.hpp"

#endif