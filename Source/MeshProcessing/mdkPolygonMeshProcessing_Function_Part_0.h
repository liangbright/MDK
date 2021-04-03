#pragma once

#include "vtkSmoothPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkPlane.h"
#include "vtkClipPolyData.h"
#include "vtkCleanPolyData.h"
#include "vtkDijkstraGraphGeodesicPath.h"
#include "vtkPointLocator.h"
#include "vtkKdTreePointLocator.h"
#include "vtkCellLocator.h"
#include "vtkStaticCellLocator.h"

namespace mdk
{
//trace boundary along the direction from PointIndex_start to PointIndex_next
template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start, int_max PointIndex_next);

//no double count
template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start);

template<typename ScalarType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh);

template<typename ScalarType>
int_max FindNearestPointOnMesh(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition);

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKKdTreePointLocator(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& PointPosition);

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKKdTreePointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition);

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKKdTreePointLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKKdTreePointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKPointLocator(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& PointPosition);

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKPointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition);

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKPointLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKPointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet);

template<typename ScalarType>
void ProjectPointToFaceByVTKCellLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet,
	                                    DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
void ProjectPointToFaceByVTKCellLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet,
					 			        DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
void ProjectPointToFaceByVTKStaticCellLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet,
									   	      DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
void ProjectPointToFaceByVTKStaticCellLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet,
										      DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj);

template<typename ScalarType>
void SmoothMeshByVTKSmoothPolyDataFilter(PolygonMesh<ScalarType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

template<typename ScalarType>
void SmoothMeshByVTKWindowedSincPolyDataFilter(PolygonMesh<ScalarType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing = true, bool Flag_BoundarySmoothing = true);

//output FaceIndexList with seed FaceIndex_seed
//ClosedEdgeCurve_EdgeIndexList can be in random order
//Attention: EdgeIndex, NOT PointIndex
template<typename ScalarType>
DenseVector<int_max> SegmentMeshByEdgeCurve(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed);

template<typename ScalarType>
DenseVector<DenseVector<int_max>> SegmentMeshByEdgeCurve(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList);

//clip mesh using a plane defined by origin and normal
template<typename ScalarType>
PolygonMesh<ScalarType> ClipMeshByVTKClipPolyData(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& Origin, const DenseVector<ScalarType, 3>& Normal);

//clip mesh using a plane defined by origin and normal
template<typename ScalarType>
PolygonMesh<ScalarType> ClipMeshByVTKClipPolyData(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& Origin, const DenseVector<ScalarType, 3>& Normal);

DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(vtkPolyData* InputMesh_vtk, int_max PointIndex_start, int_max PointIndex_end);

template<typename ScalarType>
DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start, int_max PointIndex_end);

//InputMesh is from mesh.Append()
//It is useful to append many mesh together and stitch them on boundary
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPointOnBoundary(const PolygonMesh<ScalarType>& InputMesh, ScalarType DistanceThreshold, bool MergePointInfo);

//InputMesh is from mesh.Append()
//It is useful to append many mesh together and stitch them on boundary
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPointOnBoundary(const PolygonMesh<ScalarType>& InputMesh, ScalarType DistanceThreshold,
											     bool MergePointInfo, DenseVector<int_max>& PointIndexMap_Input_to_Output);

template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPoint(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& PointIndexList_merge, 
	                                   ScalarType DistanceThreshold, bool MergePointInfo, DenseVector<int_max>& PointIndexMap_Input_to_Output);

//merge InputMeshA boundary and InputMeshB boundary
//It is useful to stitch two meshes on boundary
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPointOnBoundary(const PolygonMesh<ScalarType>& InputMeshA, const PolygonMesh<ScalarType>& InputMeshB, ScalarType DistanceThreshold);

template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPoint(const PolygonMesh<ScalarType>& InputMeshA, const PolygonMesh<ScalarType>& InputMeshB, ScalarType DistanceThreshold);

//Merge InputMeshA and InputMeshB
//The shared points are in PointIndexListA of InputMeshA and PointIndexListB of InputMeshB;
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPoint(const PolygonMesh<ScalarType>& InputMeshA, const DenseVector<int_max>& PointIndexListA,
									   const PolygonMesh<ScalarType>& InputMeshB, const DenseVector<int_max>& PointIndexListB, ScalarType DistanceThreshold);

//Merge InputMeshA and InputMeshB, only merge point, face/cell will not be changed, but edge may be changed
//The shared points are in PointIndexListA of InputMeshA and PointIndexListB of InputMeshB;
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshPoint(const PolygonMesh<ScalarType>& InputMeshA, const DenseVector<int_max>& PointIndexListA,
		                               const PolygonMesh<ScalarType>& InputMeshB, const DenseVector<int_max>& PointIndexListB, ScalarType DistanceThreshold,
									   DenseVector<int_max>& PointIndexMap_InputA_to_Output, DenseVector<int_max>& PointIndexMap_InputB_to_Output);
}//namespace mdk

#include "mdkPolygonMeshProcessing_Function_Part_0.hpp"
