#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>

#include "mdk3DImage.h"

namespace mdk
{

mdk3DImage<double> ReadGrayScale3DImageFromDICOMFile(const std::string& FilePath);

void SaveGrayScale3DImageToBinaryFile(const std::string& FilePathAndName, const std::string& FileFormat, const mdk3DImage<double>& Image);

}

#endif