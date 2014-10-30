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

namespace mdk
{
// write data information as *.json file
// For example: FilePathAndName is "C:/Data/something.json"

struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

inline bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName, bool Flag_Append = false);

struct NameValueStdStringPair
{
	std::string  Name;
	std::string  Value;
};

inline bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueStdStringPair>& PairList, const std::string& FilePathAndName, bool Flag_Append = false);

}//namespace mdk

#include "mdkFileIO_Json.hpp"

#endif