#ifndef mdk_Curve_FileIO_h
#define mdk_Curve_FileIO_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkVTKDataStructureConversion.h"

namespace mdk
{

// forward declare ---------------------
template<typename ElementType>
class ObjectArray;

template<typename ScalarType>
class DenseMatrix;
//--------------------------------------

template<typename ScalarType>
bool SaveSingle3DCurveAsVTKFile(const DenseMatrix<ScalarType>& InputCurve, const String& FilePathAndName);

template<typename ScalarType>
bool SaveMultiple3DCurveAsVTKFile(const ObjectArray<DenseMatrix<ScalarType>>& InputCurveList, const String& FilePathAndName);

template<typename ScalarType>
bool LoadSingle3DCurveFromVTKFile(DenseMatrix<ScalarType>& OutputCurve, const String& FilePathAndName);

template<typename ScalarType>
bool LoadMultiple3DCurveFromVTKFile(ObjectArray<DenseMatrix<ScalarType>>& OutputCurveList, const String& FilePathAndName);

}//namespace mdk

#include "mdkCurve_FileIO.hpp"

#endif