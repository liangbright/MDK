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

inline String ExtractFilePath(const String& FilePathAndName);
inline String ExtractFileName(const String& FilePathAndName);
inline String ExtractFileType(const String& FileNameWithType);
inline String RemoveFileType(const String& FilePathAndName);

template<typename ScalarType>
bool SaveScalarArrayAsDataFile(const ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName);

template<typename ScalarType>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName, const String& ScalarTypeInDataFile);

template<typename ScalarType, typename ScalarType_DataFile>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName);


}//namespace mdk

#include "mdkFileIO_Common.hpp"

#endif
