#pragma once

#include <vtkSmartPointer.h>
#include <limits>
#include <vtkCellLocator.h>
#include "mdkVTKDataStructureConversion.h"
#include "mdkGeometry.h"

namespace mdk
{
//Surface: data structure may not be clean (input and output)
//return: CurvePointIndexList_output
template<typename ScalarType>
DenseVector<int_max> ResampleOpenCurveOfSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<int_max>& CurvePointIndexList_input, const DenseMatrix<ScalarType>& Curve_output);
template<typename ScalarType>
DenseVector<int_max> ResampleOpenCurveOfSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<int_max>& CurvePointIndexList_input, const int_max PointCountOfCurve_output);

//input: Point, Surface
//output: FaceIndex_proj on the mesh
//return: Point_proj
template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(vtkPolyData* Surface, const DenseVector<ScalarType, 3>& Point, int_max& FaceIndex_proj);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(vtkPolyData* Surface, const DenseVector<ScalarType, 3>& Point);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, int_max& FaceIndex_proj);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point);

//input: PointSet(:,k) is a 3D point; Surface is a mesh
//output: FaceIndexList_proj[k] is the face index
//return: PointSet_proj
template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(vtkPolyData* Surface, const DenseMatrix<ScalarType>& PointSet, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(vtkPolyData* Surface, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet);

//project a point to the surface, then add the projected point to surface
//return: PointIndex on modified Surface
template<typename ScalarType>
int_max Project_Add_Point_to_Surface(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point);

template<typename ScalarType>
DenseVector<int_max> Project_Add_Point_to_Surface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet);

//add an input point to surface
template<typename ScalarType>
int_max AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point);
//add many point to surface
template<typename ScalarType>
DenseVector<int_max> AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
DenseVector<int_max> AddPolyLineOnSurface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PolyLine);

}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_1.hpp"
