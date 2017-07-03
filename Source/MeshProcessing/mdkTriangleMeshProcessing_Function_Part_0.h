#pragma once

#include <vtkLinearSubdivisionFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkDecimatePro.h>

namespace mdk
{

template<typename ScalarType>
void ConvertPolygonMeshToTriangleMesh(const PolygonMesh<ScalarType>& InputMesh, TriangleMesh<ScalarType>& OutputMesh);

// output point index list
template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const TriangleMesh<ScalarType>& TargetMesh, int_max PointIndex_start);

// output point index list
template<typename ScalarType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const TriangleMesh<ScalarType>& TargetMesh);

// output point index
template<typename ScalarType>
int_max FindNearestPointOnMesh(const TriangleMesh<ScalarType>& TargetMesh, const DenseVector<typename ScalarType::ScalarType, 3>& PointPosition);

template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMesh_Linear(const TriangleMesh<ScalarType>& InputMesh);// divide once only

template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMesh_Linear(const TriangleMesh<ScalarType>& InputMesh, int_max SubdivisionCount);

template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMeshByVTKLinearSubdivisionFilter(const TriangleMesh<ScalarType>& TargetMesh, int_max SubdivisionCount);

template<typename ScalarType>
TriangleMesh<ScalarType> SimplifyTriangleMeshByVTKDecimatePro(const TriangleMesh<ScalarType>& TargetMesh, double TargetReduction);

template<typename ScalarType>
TriangleMesh<ScalarType> SimplifyTriangleMeshByVTKQuadricDecimation(const TriangleMesh<ScalarType>& TargetMesh, double TargetReduction);

// assume GaussianCurvature has been computed at each point of TargetMesh if Flag_UpdateAttribute is false
template<typename ScalarType>
void SmoothTriangleMeshByGaussianCurvature(TriangleMesh<ScalarType>& TargetMesh, double MaxDisplacement, bool Flag_UpdateAttribute = true);

// assume NormalBasedCurvature, FaceNormal, PointNormal has been computed 
// Alpha: NewPos = Pos + Alpha*Displacement at each iteration, Alpha must >= 0
// Set Alpha = 1 usually
template<typename ScalarType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<ScalarType>& TargetMesh, int_max MaxIter = 10, double Alpha = 1, bool Flag_BoundarySmoothing = true, bool Flag_TerminateIfTotalCurvatureIncrease = true);

template<typename ScalarType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<ScalarType>& TargetMesh, int_max MaxIter, double Alpha, const DenseVector<int_max>& PointIndexList_NOSmoothing, bool Flag_TerminateIfTotalCurvatureIncrease);


}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_0.hpp"
