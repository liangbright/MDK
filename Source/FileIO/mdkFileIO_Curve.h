#ifndef __mdkFileIO_Curve_h
#define __mdkFileIO_Curve_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO_DenseMatrix.h"

namespace mdk
{

// forward declare ---------------------
template<typename ElementType>
class ObjectArray;

template<typename ScalarType>
class DenseMatrix;
//--------------------------------------

template<typename ScalarType>
bool SaveSingle3DCurveAsVTKFile(const DenseMatrix<ScalarType>& InputCurve, const std::string& FilePathAndName);

template<typename ScalarType>
bool SaveMultiple3DCurveAsVTKFile(const ObjectArray<DenseMatrix<ScalarType>>& InputCurveList, const std::string& FilePathAndName);

template<typename ScalarType>
DenseMatrix<ScalarType> LoadSingle3DCurveFromVTKFile(const std::string& FilePathAndName);

template<typename ScalarType>
ObjectArray<DenseMatrix<ScalarType>> LoadMultiple3DCurveFromVTKFile(const std::string& FilePathAndName);

}//namespace mdk

#include "mdkFileIO_Curve.hpp"

#endif