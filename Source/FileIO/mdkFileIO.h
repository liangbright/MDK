#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>

#include <QString.h>

#include "mdk3DImage.h"

namespace mdk
{

struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

mdk3DImage<double> ReadGrayScale3DImageFromDICOMFile(const std::string& FilePath);

void SaveGrayScale3DImageAsRawDataFile(const std::string& FilePathAndName, const mdk3DImage<double>& Image);

bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName);


}

#endif