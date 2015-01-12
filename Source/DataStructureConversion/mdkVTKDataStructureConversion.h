#ifndef mdk_VTKDataStructureConversion_h
#define mdk_VTKDataStructureConversion_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
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
VTKDataTypeEnum GetVTkScalarType(const ScalarType& ReferenceScalar);

//-------------------------------------- convert mdk Curve/Matrix to vtkPolyData ----------------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve);

template<typename ScalarType>
bool ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve, vtkPolyData& VTKCurve);

template<typename ScalarType>
bool ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve, vtkPolyData* VTKCurve);

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList);

template<typename ScalarType>
bool ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList, vtkPolyData& VTKCurve);

template<typename ScalarType>
bool ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList, vtkPolyData* VTKCurve);

//-------------------------------------- convert vtkPolyData  to mdk Curve/Matrix ----------------------------------------------------------//
template<typename ScalarType>
bool ConvertVTKPolyDataToMDK3DCurve(vtkPolyData& VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData);

template<typename ScalarType>
bool ConvertVTKPolyDataToMDK3DCurve(vtkPolyData* VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData);

//-------------------------------------- convert mdk image to vtk image (copy data) -------------------------------------------------------------//
template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage);

template<typename PixelType>
bool ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, vtkImageData& VTKImage);

template<typename PixelType>
bool ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, vtkImageData* VTKImage);

//-------------------------------------- convert vtk image to mdk image (copy data) -------------------------------------------------------------//
template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData& VTKImage, DenseImage3D<PixelType>& MDKImage);

template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage, DenseImage3D<PixelType>& MDKImage);

//--------------------------------------- convert mdk PolygonMesh to vtkPolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh);

template<typename MeshAttributeType>
bool ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh, vtkPolyData& VTKMesh);

template<typename MeshAttributeType>
bool ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh, vtkPolyData* VTKMesh);

//--------------------------------------- convert vtkPolyData to mdk PolygonMesh ------------------------------------------------//
template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData& VTKMesh, PolygonMesh<MeshAttributeType>& MDKMesh);

template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKMesh, PolygonMesh<MeshAttributeType>& MDKMesh);

//--------------------------------------- convert mdk TriangleMesh to vtkPolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh);

template<typename MeshAttributeType>
bool ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh, vtkPolyData& VTKMesh);

template<typename MeshAttributeType>
bool ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh, vtkPolyData* VTKMesh);

//--------------------------------------- convert vtkPolyData to mdk TriangleMesh ------------------------------------------------//
template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData& VTKMesh, TriangleMesh<MeshAttributeType>& MDKMesh);

template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKMesh, TriangleMesh<MeshAttributeType>& MDKMesh);

}// namespace mdk

#include "mdkVTKDataStructureConversion.hpp"

#endif