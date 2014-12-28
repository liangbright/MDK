#ifndef __mdkFileIO_DenseMatrix_h
#define __mdkFileIO_DenseMatrix_h

#include <tuple>
#include <utility>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkJsonFile.h"

namespace mdk
{
//------------- forward declare-------------------//
template<typename ScalarType>
class DenseMatrix;
//-----------------------------------------------------

// save/load DenseMatrix from JsonData file
// for example: FilePathAndName = "C:/SomeMatrix.json" 
// attention: SomeMatrix.json NOT SomeMatrix
// Info is saved in C:/SomeMatrix.json
// Element is saved in C:/SomeMatrix.data

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName);

template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const String& FilePathAndName);


}//namespace mdk

#include "mdkFileIO_DenseMatrix.hpp"

#endif
