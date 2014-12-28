#ifndef __mdkVTKDataStructureConversion_h
#define __mdkVTKDataStructureConversion_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkImageData.h>

#include "mdkType.h"

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

template<typename MeshAttributeType>
class TriangleMesh;

template<typename MeshAttributeType>
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
	VALUE_INTEGER = VTK_INT,
	VALUE_UNSIGNED_INTEGER = VTK_UNSIGNED_INT,
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

inline VTKDataTypeEnum TranslateMDKScalarTypeToVTKScalarType(const std::string& MDKScalarType);

template<typename ScalarType>
VTKDataTypeEnum GetVtkScalarType(const ScalarType& ReferenceScalar);

//-------------------------------------- convert mdk Curve/Matrix to vtk PolyData ----------------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve);

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList);

//-------------------------------------- convert vtk PolyData  to mdk Curve/Matrix ----------------------------------------------------------//
template<typename ScalarType>
bool ConvertVTKPolyDataToMDK3DCurve(vtkPolyData* VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData);

//-------------------------------------- convert mdk image to vtk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage);

//-------------------------------------- convert vtk image to mdk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage, DenseImage3D<PixelType>& MDKImage);

//--------------------------------------- convert mdk PolygonMesh to vtk PolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh);

//--------------------------------------- convert vtk PolyData to mdk PolygonMesh ------------------------------------------------//
template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKMesh, PolygonMesh<MeshAttributeType>& MDKMesh);

//--------------------------------------- convert mdk TriangleMesh to vtk PolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh);

//--------------------------------------- convert vtk PolyData to mdk TriangleMesh ------------------------------------------------//
template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKMesh, TriangleMesh<MeshAttributeType>& MDKMesh);

}// namespace mdk

#include "mdkVTKDataStructureConversion.hpp"

#endif