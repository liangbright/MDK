#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>

#include <QString.h>

#include "mdkImageIn3D.h"

namespace mdk
{

struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

ImageIn3D<double> ReadGrayScale3DImageFromDICOMFile(const std::string& FilePath);

void SaveGrayScale3DImageAsRawDataFile(const std::string& FilePathAndName, const ImageIn3D<double>& Image);

bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName);


}

#endif