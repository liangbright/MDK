#ifndef __mdkVTKDataStructureConversion_h
#define __mdkVTKDataStructureConversion_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkImageData.h>


namespace mdk
{

//------------- forward declare-------------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
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

//-------------------------------------- convert mdk image to vtk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& InputImage);

//-------------------------------------- convert vtk image to mdk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
DenseImage3D<PixelType> ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage);

//--------------------------------------- convert mdk TriangleMesh to vtk PolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& InputMesh);

//--------------------------------------- convert vtk PolyData to mdk TriangleMesh ------------------------------------------------//
template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKPolyMesh);

//--------------------------------------- convert mdk PolygonMesh to vtk PolyData -----------------------------------------------//
template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& InputMesh);

//--------------------------------------- convert vtk PolyData to mdk PolygonMesh ------------------------------------------------//
template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKPolyMesh);


}// namespace mdk

#include "mdkVTKDataStructureConversion.hpp"

#endif