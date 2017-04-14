#pragma once

#include "vtkSmoothPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"

#include "mdkDenseMatrix.h"
#include "mdkPolygonMesh.h"

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

// Distance is the edge count bewtween the output point and the input point, Distance >=0
// Output.ElementList[k] is the graph_distance between Point(PointIndex) and Point(PointIndex_input)
// Output.IndexList[k] is PointIndex in TargetMesh
template<typename MeshAttributeType>
SparseVector<int_max> FindNeighbourPointOfPointOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_input, int_max MaxGraphDistance);

//template<typename MeshAttributeType>
//SparseVector<int_max> FindNeighbourFaceOfPointOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_input, int_max MaxGraphDistance);

//template<typename MeshAttributeType>
//SparseVector<int_max> FindNeighbourFaceOfFaceOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, int_max FaceIndex_input, int_max MaxGraphDistance);

//template<typename MeshAttributeType>
//SparseVector<int_max> FindNeighbourPointOfFaceOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, int_max FaceIndex_input, int_max MaxGraphDistance);

//output FaceIndexList with seed FaceIndex_seed
template<typename MeshAttributeType>
DenseVector<int_max> FindFaceEnclosedByEdgeCurve(const PolygonMesh<MeshAttributeType>& Surface, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed);

}//namespace mdk

#include "mdkPolygonMeshProcessing_Function_Part_0.hpp"
