#pragma once

#include "mdkMinimumStretchBasedTriangleMesh3DTo2DMapper.h"
#include "mdkGeometry.h"

namespace mdk
{

template<typename ScalarType>
struct Input_of_TemplateBasedSurfaceRemesher
{
	// input mesh must be triangle mesh
	const TriangleMesh<ScalarType>* SourceMesh;

	DenseVector<DenseVector<int_max>> BoundarySegmentListOfSourceMesh;
	//BoundaryListOfInputMesh[k] is BoundarySegment: a set of boundary point index
	//BoundarySegment = {PointIndex_start, ..., PointIndex_end}
	//BoundarySegment[k] overlap with BoundarySegment[k-1] and BoundarySegment[k+1] at start/end point
	//BoundaryListOfInputMesh[k] ~ BoundarySegmentListOfOutputMesh[k] 

	//template mesh point position is [u, v, 0]
	const PolygonMesh<ScalarType>* TemplateMesh;

	DenseVector<DenseVector<int_max>> BoundarySegmentListOfTemplateMesh;
	//BoundarySegmentListOfOutputMesh[k] is BoundarySegment: a set of boundary point index
	//BoundarySegment = {PointIndex_start, ..., PointIndex_end}
	//BoundarySegment[k] overlap with BoundarySegment[k-1] and BoundarySegment[k+1] at start/end point
	//BoundarySegmentListOfOutputMesh[k] ~ BoundaryListOfInputMesh[k] 

	//Parameter to MinimumStretchBasedTriangleMesh3DTo2DMapper
	ScalarType DiffusionCoefficientOfMeshParameterization;
	int_max MaxIterationOfMeshParameterization;

	//this is a bad method
	bool Flag_EnableTPSTransformOfTemplateMesh = false;

};

template<typename ScalarType>
struct Internal_of_TemplateBasedSurfaceRemesher
{
	DenseVector<int_max> BoundaryPointIndexListOfSourceMesh;
	DenseMatrix<ScalarType> UVTalbleOfBoundaryOfSourceMesh;// each col is [u,v]

	DenseVector<int_max> BoundaryPointIndexListOfTemplateMesh;
	DenseMatrix<ScalarType> BoundaryPositionOfTemplateMesh;// each col is [u,v,0]
	DenseMatrix<ScalarType> BoundaryPositionOfDeformedTemplateMesh;  // each col is [x,y,z]
};


template<typename ScalarType>
struct Output_of_TemplateBasedSurfaceRemesher
{
	TriangleMesh<ScalarType> ParameterizedSourceMesh;//2D mesh
	// OutputMesh is Transformed TemplateMesh from 2D to 3D
	PolygonMesh<ScalarType> DeformedTemplateMesh;
};


template<typename Scalar_Type>
class TemplateBasedSurfaceRemesher : public Object
{
public:
	typedef Scalar_Type ScalarType;	
	typedef typename TriangleMesh<ScalarType>   InputMeshType;
	typedef typename PolygonMesh<ScalarType>    TemplateMeshType;
	typedef typename PolygonMesh<ScalarType>    OutputMeshType;

public:
	Input_of_TemplateBasedSurfaceRemesher<ScalarType> Input;
private:
	Internal_of_TemplateBasedSurfaceRemesher<ScalarType> Internal;
public:
	Output_of_TemplateBasedSurfaceRemesher<ScalarType> Output;

public:
	TemplateBasedSurfaceRemesher();
	~TemplateBasedSurfaceRemesher();

	void Clear();
	void Update();

private:
	bool CheckInput();
	void ClearInternalData();
	void FindBoundaryConstraint();
	bool CheckBoundaryConstraint();
	void TransformInputMeshFrom3DTo2D();
	void TransfromTemplateMeshFrom2Dto3D();
	void TransfromTemplateMeshFrom2Dto3D_Method0_TPS();
	void TransfromTemplateMeshFrom2Dto3D_Method1_Interpolation();

	DenseVector<ScalarType> ComputeCumulativeCurveLength(const DenseMatrix<ScalarType>& CurvePosition);

	// relative length is from 0 to 1
	DenseVector<ScalarType> ComputeCumulativeCurveLength_Relative(const DenseMatrix<ScalarType>& CurvePosition);

	DenseVector<int_max, 3> Find3PointOfNearestFace(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& TargetMesh);
	
private:
	TemplateBasedSurfaceRemesher(const TemplateBasedSurfaceRemesher&) = delete;
	void operator=(const TemplateBasedSurfaceRemesher&) = delete;
};

}//namespace mdk

#include "mdkTemplateBasedSurfaceRemesher.hpp"
