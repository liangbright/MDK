#pragma once

#include "vtkSmoothPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkPlane.h"
#include "vtkClipPolyData.h"
#include "vtkCleanPolyData.h"
#include "vtkDijkstraGraphGeodesicPath.h"

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
//Attention: EdgeIndex, NOT PointIndex
template<typename MeshAttributeType>
DenseVector<int_max> SegmentMeshByEdgeCurve(const PolygonMesh<MeshAttributeType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed);

template<typename MeshAttributeType>
DenseVector<DenseVector<int_max>> SegmentMeshByEdgeCurve(const PolygonMesh<MeshAttributeType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList);

//clip mesh using a plane defined by origin and normal
template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> ClipMeshByVTKClipPolyData(vtkPolyData* InputMesh_vtk, const DenseVector<typename MeshAttributeType::ScalarType, 3>& Origin, const DenseVector<typename MeshAttributeType::ScalarType, 3>& Normal);

//clip mesh using a plane defined by origin and normal
template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> ClipMeshByVTKClipPolyData(const PolygonMesh<MeshAttributeType>& InputMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& Origin, const DenseVector<typename MeshAttributeType::ScalarType, 3>& Normal);

template<typename MeshAttributeType>
DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(vtkPolyData* InputMesh_vtk, int_max PointIndex_start, int_max PointIndex_end);

template<typename MeshAttributeType>
DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_start, int_max PointIndex_end);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> MergeMeshBoundary(const PolygonMesh<MeshAttributeType>& InputMeshA, const PolygonMesh<MeshAttributeType>& InputMeshB, typename MeshAttributeType::ScalarType Threshold);

}//namespace mdk

#include "mdkPolygonMeshProcessing_Function_Part_0.hpp"
