#pragma once

#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkCellLocator.h"

#include "mdkTriangleMesh.h"
#include "mdkPolygonMeshProcessing_Function.h"

namespace mdk
{

template<typename ScalarType>
struct Input_of_IsotropicTriangleSurfaceRemesher
{
	// input mesh must be triangle mesh
	const TriangleMesh<ScalarType>* SourceMesh;
	
	//feature point/edge will not be modified
	//do not need to set boundary as feature because boundary will not be modified if Flag_ProcessBounary is true
	DenseVector<int_max> FeaturePointIndexList;
	
	//Problem: (1) Edge in Python Mesh may have different order
	//         (2) Edge index does not exit in Paraview
	DenseVector<int_max> FeatureEdgeIndexList;
	//Solution: see Convert_FeatureCurve_to_FeatureEdge function
	//FeatureCurveList[k] is PointIndexList of a curve
	DenseVector<DenseVector<int_max>> FeatureCurveList;

	ScalarType TargetEdgeLength;
	ScalarType TargetEdgeLength_Ratio_Max;
	ScalarType TargetEdgeLength_Ratio_Min;

	int_max MaxIter;
	
	//false, then boundary (point, edge) will not be modified
	bool Flag_ProcessBounary;

	//true, then project TargetMesh onto SourceMesh
	bool Flag_ProjectToSourceMesh;
};

template<typename ScalarType>
struct Internal_of_IsotropicTriangleSurfaceRemesher
{
	vtkSmartPointer<vtkPolyData> SourceMesh_vtk;
	vtkSmartPointer<vtkPolyData> BoundaryCurve_vtk;
	vtkSmartPointer<vtkCellLocator> CellLocator_of_SourceMesh_vtk;
	vtkSmartPointer<vtkCellLocator> CellLocator_of_BoundaryCurve_vtk;

	DenseVector<int_max> PointFlagList;
	//PointFlagList[k]: 0, nothing special for Point-k of SourceMesh
	//PointFlagList[k]: 1, feature point of SourceMesh

	DenseVector<int_max> EdgeFlagList;
	//EdgeFlagList[k]: 0, nothing special for Edge-k of SourceMesh
	//EdgeFlagList[k]: 1, feature edge of SourceMesh

	ScalarType  MaxEdgeLength;
	ScalarType	MinEdgeLength;
};


template<typename ScalarType>
struct Output_of_IsotropicTriangleSurfaceRemesher
{
	TriangleMesh<ScalarType> TargetMesh;
};


template<typename Scalar_Type>
class IsotropicTriangleSurfaceRemesher : public Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_of_IsotropicTriangleSurfaceRemesher<ScalarType> Input;
private:
	Internal_of_IsotropicTriangleSurfaceRemesher<ScalarType> Internal;
public:
	Output_of_IsotropicTriangleSurfaceRemesher<ScalarType> Output;

public:
	IsotropicTriangleSurfaceRemesher();
	~IsotropicTriangleSurfaceRemesher();
	void Reset();
	void Update();
	void CleanMesh();//use this Class to clean mesh
private:
	bool CheckInput();
	bool Convert_FeatureCurve_to_FeatureEdge();
	void Initialize();
	bool IsFeatureEdge(int_max EdgeIndex);
	bool IsFeaturePoint(int_max PointIndex);
	void ProcessBoundary();
	void SplitLongBoundaryEdge();
	bool TryToCollapseSingleEdge(int_max EdgeIndex, bool Flag_ProcessBoundary);
	void CollapseShortBoundaryEdge();
	void RemoveIsolatedPoint();
	void RemoveIsolatedEdge();
	void RemoveIsolatedFace();
	void RemoveDistortedFace();
	void Remesh();
	void EqualizeValence();
	void SplitLongEdge();	
	void CollapseShortEdge();	
	void FlipEdge();
	void TangentialRelaxation_BoundaryEdge();
	void TangentialRelaxation_InternalEdge();
	DenseVector<ScalarType, 3> Project_A_Point_to_BoundaryCurve(const DenseVector<ScalarType, 3>& Point);
	DenseVector<ScalarType, 3> Project_A_Point_to_SourceMesh(const DenseVector<ScalarType, 3>& Point);
	void Refine();

private:
	IsotropicTriangleSurfaceRemesher(const IsotropicTriangleSurfaceRemesher&) = delete;
	void operator=(const IsotropicTriangleSurfaceRemesher&) = delete;
};

}//namespace mdk

#include "mdkIsotropicTriangleSurfaceRemesher.hpp"
