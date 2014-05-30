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
class Image2D;

template<typename PixelType>
class Image3D;

template<typename ScalarType>
class TriangleMesh;

template<typename ScalarType>
class PolygonMesh;

//-------------------------------------------------//

//-------------------------------------- convert mdk image to vtk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const Image3D<PixelType>& InputImage);

//-------------------------------------- convert vtk image to mdk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
Image3D<PixelType> ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage);

//--------------------------------------- convert mdk TriangleMesh to vtk PolyData -----------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<ScalarType>& InputMesh);

//--------------------------------------- convert vtk PolyData to mdk TriangleMesh ------------------------------------------------//
template<typename ScalarType>
TriangleMesh<ScalarType> ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKPolyMesh, bool Flag_BuildLinkAndAjacency = true);

//--------------------------------------- convert mdk PolygonMesh to vtk PolyData -----------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<ScalarType>& InputMesh);

//--------------------------------------- convert vtk PolyData to mdk PolygonMesh ------------------------------------------------//
template<typename ScalarType>
PolygonMesh<ScalarType> ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKPolyMesh, bool Flag_BuildLinkAndAjacency = true);


}// namespace mdk

#include "mdkVTKDataStructureConversion.hpp"

#endif