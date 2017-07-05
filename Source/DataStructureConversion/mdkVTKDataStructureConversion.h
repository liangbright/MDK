#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkTriangleFilter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>

#include "mdkString.h"
#include "mdkTypeInference.h"

namespace mdk
{

//------------- forward declare-------------------//
template<typename T>
class ObjectArray;

template<typename T>
class DenseMatrix;

template<typename T>
class SparseMatrix;

template<typename PixelType>
class DenseImage2D;

template<typename PixelType>
class DenseImage3D;

template<typename ScalarType>
class Mesh;

template<typename ScalarType>
class PolyhedronMesh;

template<typename ScalarType>
class PolygonMesh;

//-------------------------------------------------//
typedef enum
{
	VALUE_VOID = VTK_VOID,
	VALUE_BIT = VTK_BIT,
	VALUE_CHAR = VTK_CHAR,
	VALUE_SIGNED_CHAR = VTK_SIGNED_CHAR,
	VALUE_UNSIGNED_CHAR = VTK_UNSIGNED_CHAR,
	VALUE_SHORT = VTK_SHORT,
	VALUE_UNSIGNED_SHORT = VTK_UNSIGNED_SHORT,
	VALUE_INT = VTK_INT,
	VALUE_UNSIGNED_INT = VTK_UNSIGNED_INT,
	VALUE_LONG = VTK_LONG,
	VALUE_UNSIGNED_LONG = VTK_UNSIGNED_LONG,
	VALUE_LONG_LONG = VTK_LONG_LONG,
	VALUE_UNSIGNED_LONG_LONG = VTK_UNSIGNED_LONG_LONG,
	VALUE_FLOAT = VTK_FLOAT,
	VALUE_DOUBLE = VTK_DOUBLE,
	VALUE_IDTYPE = VTK_ID_TYPE,
	VALUE_STRING = VTK_STRING,

	VALUE_UNKNOWN

} VTKDataTypeEnum;
//-------------------------------------------------//

inline VTKDataTypeEnum TranslateMDKScalarTypeToVTKScalarType(const String& MDKScalarType);

template<typename ScalarType>
VTKDataTypeEnum GetVTKScalarType(const ScalarType& ReferenceScalar);

//-------------------------------------- convert between mdk Curve and vtkPolyData ----------------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve);

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList);

template<typename ScalarType>
bool ConvertVTKPolyDataToMultipleMDK3DCurve(vtkPolyData* VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData);

template<typename ScalarType>
bool ConvertVTKPolyDataToSingleMDK3DCurve(vtkPolyData* VTKCurveData, DenseMatrix<ScalarType>& MDK3DCurveData);

//-------------------------------------- convert between mdk 3D image and vtk 3D image (copy data) -----------------------------------------------------//
template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage);

template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage, DenseImage3D<PixelType>& MDKImage);

//--------------------------------------- convert between mdk PolygonMesh and vtkPolyData -----------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<ScalarType>& MDKMesh);

template<typename ScalarType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKMesh, PolygonMesh<ScalarType>& MDKMesh);

//--------------------------------------- convert between mdk PolyhedronMesh and vtkUnstructuredGrid ------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertMDKPolyhedronMeshToVTKUnstructuredGrid(const PolyhedronMesh<ScalarType>& MDKMesh);

template<typename ScalarType>
bool ConvertVTKUnstructuredGridToMDPolyhedronMesh(vtkUnstructuredGrid* VTKMesh, PolyhedronMesh<ScalarType>& MDKMesh);

//--------------------------------------- convert between mdk Mesh and vtkUnstructuredGrid ------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertMDKMeshToVTKUnstructuredGrid(const Mesh<ScalarType>& MDKMesh);

template<typename ScalarType>
bool ConvertVTKUnstructuredGridToMDKMesh(vtkUnstructuredGrid* VTKMesh, Mesh<ScalarType>& MDKMesh);

}// namespace mdk

#include "mdkVTKDataStructureConversion.hpp"
