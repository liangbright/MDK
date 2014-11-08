#ifndef __mdkFileIO_DenseMatrix_h
#define __mdkFileIO_DenseMatrix_h

#include <tuple>
#include <utility>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkFileIO_Json.h"

namespace mdk
{
//------------- forward declare-------------------//
template<typename ScalarType>
class DenseMatrix;
//-----------------------------------------------------

// save/load DenseMatrix from Json data file
// for example: JsonFilePathAndName = "C:/Data/SomeMatrix.json"
// header is saved in SomeMatrix.json
// data is saved in SomeMatrix.json.data

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Header(const DenseMatrix<ScalarType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ScalarType>& InputMatrix, const std::string& DataFilePathAndName);

template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const std::string& JsonFilePathAndName);

// HeaderInfo: <MatrixSize, InputScalarTypeName>
inline std::pair<MatrixSize, std::string> LoadDenseMatrixFromJsonDataFile_Header(const std::string& JsonFilePathAndName);

// attention: OutputMatrix.FastResize(...) must be called before LoadDenseMatrixFromJsonDataFile_Data
template<typename OutputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const std::string& DataFilePathAndName, const std::string& InputScalarTypeName);

template<typename OutputScalarType, typename InputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const std::string& DataFilePathAndName);

}//namespace mdk

#include "mdkFileIO_DenseMatrix.hpp"

#endif
