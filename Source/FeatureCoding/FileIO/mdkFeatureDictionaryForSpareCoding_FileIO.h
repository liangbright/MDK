#ifndef __mdkFeatureDictionaryForSpareCoding_FileIO_h
#define __mdkFeatureDictionaryForSpareCoding_FileIO_h

#include "mdkFileIO.h"

namespace mdk
{
// forward declare --------------------
template<typename ScalaryType>
class FeatureDictionaryForSparseCoding;
//-------------------------------------

template<typename ScalaryType>
bool SaveFeatureDictionaryForSparseCodingAsJsonDataFile(const FeatureDictionaryForSparseCoding<ScalaryType>& Dictionary, const std::string& JsonFilePathAndName);

template<typename ScalaryType>
bool LoadFeatureDictionaryForSparseCodingFromJsonDataFile(FeatureDictionaryForSparseCoding<ScalaryType>& Dictionary, const std::string& JsonFilePathAndName);


}//namespace mdk

#include "mdkFeatureDictionaryForSpareCoding_FileIO.hpp"

#endif