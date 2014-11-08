#ifndef __mdkFileIO_FeatureDictionaryForSpareCoding_h
#define __mdkFileIO_FeatureDictionaryForSpareCoding_h

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

#include "mdkFileIO_FeatureDictionaryForSpareCoding.hpp"

#endif