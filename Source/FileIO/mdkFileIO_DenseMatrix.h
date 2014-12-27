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

// save/load DenseMatrix from JsonData file
// for example: FilePathAndName = "C:/SomeMatrix" 
// attention: SomeMatrix NOT SomeMatrix.json
// Info is saved in C:/SomeMatrix.json
// Element is saved in C:/SomeMatrix.data

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName);

template<typename ScalarType>
DataArray<String> SaveDenseMatrixAsJsonDataFile_Json(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName);

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName);

template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const String& FilePathAndName);

// HeaderInfo: <MatrixSize, InputScalarTypeName>
inline std::pair<MatrixSize, String> LoadDenseMatrixFromJsonDataFile_Json(const String& FilePathAndName);

// attention: OutputMatrix.FastResize(...) must be called before LoadDenseMatrixFromJsonDataFile_Data
template<typename OutputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const String& FilePathAndName, const String& InputScalarTypeName);

template<typename OutputScalarType, typename InputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const String& FilePathAndName);

}//namespace mdk

#include "mdkFileIO_DenseMatrix.hpp"

#endif
