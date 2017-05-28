#pragma once

#include "vtkSmoothPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"

namespace mdk
{
//no double count
template<typename MeshAttributeType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_start);

template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& InputMesh);

template<typename MeshAttributeType>
int_max FindNearestPointOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh,  const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SmoothMeshByVTKSmoothPolyDataFilter(const PolygonMesh<MeshAttributeType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SmoothMeshByVTKWindowedSincPolyDataFilter(const PolygonMesh<MeshAttributeType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

//output FaceIndexList with seed FaceIndex_seed
//ClosedEdgeCurve_EdgeIndexList can be in random order
template<typename MeshAttributeType>
DenseVector<int_max> FindFaceEnclosedByEdgeCurve(const PolygonMesh<MeshAttributeType>& Surface, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed);

}//namespace mdk

#include "mdkPolygonMeshProcessing_Function_Part_0.hpp"
