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
	//do not need to set boundary point as feature because boundary point will not be modified
	DenseVector<int_max> FeaturePointIndexList;
	DenseVector<int_max> FeatureEdgeIndexList;
	ScalarType TargetEdgeLength;
	int_max MaxIter;

	//true then run OutputMesh.CleanDataStructure();
	bool Flag_CleanDataStructureOfOutputMesh;
};

template<typename ScalarType>
struct Internal_of_IsotropicTriangleSurfaceRemesher
{
	vtkSmartPointer<vtkPolyData> SourceMesh_vtk;
	vtkSmartPointer<vtkCellLocator> CellLocator_vtk;

	DenseVector<int_max> PointFlagList;
	//PointFlagList[k]: 0, nothing special for Point-k of SourceMesh
	//PointFlagList[k]: 1, boundary point or input feature point of SourceMesh

	DenseVector<int_max> EdgeFlagList;
	//EdgeFlagList[k]: 0, nothing special for Edge-k of SourceMesh
	//EdgeFlagList[k]: 1, boundary edge or input feature edge of SourceMesh

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
	void Clear();
	//TODO: void SplitLongBoundaryEdge();
	void Update();
private:
	bool CheckInput();
	void Remesh();
	void EqualizeValence();
	void SplitLongEdge();	
	void CollapseShortEdge();	
	void TangentialRelaxation_ProjectToSurface();
	DenseVector<ScalarType, 3> Project_A_Point_to_SourceMesh(const DenseVector<ScalarType, 3>& Point);
	void Refine();

private:
	IsotropicTriangleSurfaceRemesher(const IsotropicTriangleSurfaceRemesher&) = delete;
	void operator=(const IsotropicTriangleSurfaceRemesher&) = delete;
};

}//namespace mdk

#include "mdkIsotropicTriangleSurfaceRemesher.hpp"
