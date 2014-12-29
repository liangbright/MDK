#ifndef __mdkFileIO_Common_h
#define __mdkFileIO_Common_h

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QByteArray>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkJsonFile.h"

namespace mdk
{
// Example:
// String FilePathAndName = "C:/Research/something.data";
// String FilePath = ExtractFilePath(FilePathAndName);  // "C:/Research/"
// String FileName = ExtractFileName(FilePathAndName);  // "something.data"
// String FileTypeA = ExtractFileType(FilePathAndName); // "data"
// String FileTypeB = ExtractFileType(FileName);        // "data"
// String NameA = RemoveFileType(FilePathAndName);      // "C:/Research/something"
// String NameB = RemoveFileType(FileName);             // "something"
inline String ExtractFilePath(const String& FilePathAndName);
inline String ExtractFileName(const String& FilePathAndName);
inline String ExtractFileType(const String& FileNameWithType);
inline String RemoveFileType(const String& FilePathAndName);

// save as binary data file (.data)
// example: FilePathAndName is "C:/Research/SomeArray.data"
template<typename ScalarType>
bool SaveScalarArrayAsDataFile(const ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName);

// load from binary data file (.data)
// example: FilePathAndName is "C:/Research/SomeArray.data", ScalarTypeInDataFile is "double"
template<typename ScalarType>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName, const String& ScalarTypeInDataFile);

template<typename ScalarType, typename ScalarType_DataFile>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName);


}//namespace mdk

#include "mdkFileIO_Common.hpp"

#endif
