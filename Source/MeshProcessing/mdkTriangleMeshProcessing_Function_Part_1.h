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

//project a point to the surface, then add the projected point to surface
//return: PointIndex on modified Surface
template<typename ScalarType>
int_max ProjectPoint_AddProjectedPoint_ToSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, ScalarType DistanceThreshold);

//project a set of points to the surface, then add the projected points to surface
//return: a set of PointIndex on modified Surface
template<typename ScalarType>
DenseVector<int_max> ProjectPoint_AddProjectedPoint_ToSurface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, ScalarType DistanceThreshold);

//add an input point to surface
template<typename ScalarType>
int_max AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, ScalarType DistanceThreshold);
//add many point to surface
template<typename ScalarType>
DenseVector<int_max> AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, ScalarType DistanceThreshold);

template<typename ScalarType>
DenseVector<int_max> AddPolyLineOnSurface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PolyLine);

}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_1.hpp"
