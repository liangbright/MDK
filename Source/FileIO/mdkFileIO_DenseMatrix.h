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
template<typename ElementType>
class DenseMatrix;
//-----------------------------------------------------

// save/load DenseMatrix from Json data file
// for example: JsonFilePathAndName = "C:/Data/SomeMatrix.json"
// header is saved in SomeMatrix.json
// data is saved in SomeMatrix.json.data

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Header(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ElementType>& InputMatrix, const std::string& DataFilePathAndName);

template<typename ElementType>
DenseMatrix<ElementType> LoadDenseMatrixFromJsonDataFile(const std::string& JsonFilePathAndName);

// HeaderInfo: <MatrixSize, InputElementTypeName>
inline std::pair<MatrixSize, std::string> LoadDenseMatrixFromJsonDataFile_Header(const std::string& JsonFilePathAndName);

template<typename OutputElementType>
DenseMatrix<OutputElementType>
LoadDenseMatrixFromJsonDataFile_Data(const std::string& DataFilePathAndName, MatrixSize OutputMatrixSize, const std::string& InputElementTypeName);

template<typename OutputElementType, typename InputElementType>
DenseMatrix<OutputElementType> LoadDenseMatrixFromJsonDataFile_Data(const std::string& DataFilePathAndName, MatrixSize OutputMatrixSize);

}//namespace mdk

#include "mdkFileIO_DenseMatrix.hpp"

#endif
