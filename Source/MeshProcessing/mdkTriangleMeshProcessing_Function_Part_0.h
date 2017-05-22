#pragma once

#include <vtkLinearSubdivisionFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkDecimatePro.h>

namespace mdk
{

template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertPolygonMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh);

// output point index list
template<typename MeshAttributeType>
DenseVector<int_max> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max PointIndex_start);

// output point index list
template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh);

// output point index
template<typename MeshAttributeType>
int_max FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMesh_Linear(const TriangleMesh<MeshAttributeType>& InputMesh);// divide once only

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMesh_Linear(const TriangleMesh<MeshAttributeType>& InputMesh, int_max SubdivisionCount);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMeshByVTKLinearSubdivisionFilter(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max SubdivisionCount);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKDecimatePro(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKQuadricDecimation(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction);

// assume GaussianCurvature has been computed at each point of TargetMesh if Flag_UpdateAttribute is false
template<typename MeshAttributeType>
void SmoothTriangleMeshByGaussianCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, double MaxDisplacement, bool Flag_UpdateAttribute = true);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SmoothMeshByVTKSmoothPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, int_max MaxIter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SmoothMeshByVTKWindowedSincPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, double PassBand, int_max MaxIter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

// assume NormalBasedCurvature, FaceNormal, PointNormal has been computed 
// Alpha: NewPos = Pos + Alpha*Displacement at each iteration, Alpha must >= 0
// Set Alpha = 1 usually
template<typename MeshAttributeType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, int_max MaxIter = 10, double Alpha = 1, bool Flag_BoundarySmoothing = true, bool Flag_TerminateIfTotalCurvatureIncrease = true);

template<typename MeshAttributeType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, int_max MaxIter, double Alpha, const DenseVector<int_max>& PointIndexList_NOSmoothing, bool Flag_TerminateIfTotalCurvatureIncrease);

}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_0.hpp"
