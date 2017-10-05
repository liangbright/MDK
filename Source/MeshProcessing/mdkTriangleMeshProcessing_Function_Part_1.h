#pragma once

#include <vtkSmartPointer.h>
#include <limits>
#include <vtkCellLocator.h>
#include "mdkVTKDataStructureConversion.h"
#include "mdkGeometry.h"

namespace mdk
{
template<typename ScalarType>
TriangleMesh<ScalarType> ResampleOpenCurveOfSurface(const TriangleMesh<ScalarType>& Surface_input, const DenseVector<int_max>& CurvePointIndexList_input, const DenseMatrix<ScalarType>& Curve_output);

template<typename ScalarType>
TriangleMesh<ScalarType> ResampleOpenCurveOfSurface(const TriangleMesh<ScalarType>& Surface_input, const DenseVector<int_max>& CurvePointIndexList_input, const int_max PointCountOfCurve_output);

//input: Point, Surface
//output: projected Point_proj and the index FaceIndex_proj on the mesh
template<typename ScalarType>
void ProjectPointToSurface(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& Surface, DenseVector<ScalarType, 3>& Point_proj, int_max& FaceIndex_proj);

//input: same as above 
//return: Point_proj
template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& Surface);

//input: PointSet(:,k) is a 3D point; Surface is a mesh
//output: PointSet_proj(:,k) is the projected point; FaceIndexList_proj[k] is the face index
template<typename ScalarType>
void ProjectPointToSurface(const DenseMatrix<ScalarType>& PointSet, const TriangleMesh<ScalarType>& Surface, DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj);

//input: same as above 
//return: PointSet_proj
template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const DenseMatrix<ScalarType>& PointSet, const TriangleMesh<ScalarType>& Surface);

//return: PointIndex on modified Surface
template<typename ScalarType>
int_max Project_Add_Point_to_Surface(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point);

}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_1.hpp"
