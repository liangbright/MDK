#ifndef mdk_DenseMatrix_FileIO_h
#define mdk_DenseMatrix_FileIO_h

#include <tuple>
#include <utility>

#include "mdkCommonType.h"
#include "mdkDebugConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkFileIO.h"

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

#include "mdkDenseMatrix_FileIO.hpp"

#endif
