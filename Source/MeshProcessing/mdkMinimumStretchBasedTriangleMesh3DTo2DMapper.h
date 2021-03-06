#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry.h"
#include "mdkTriangleMesh.h"

namespace mdk
{
//paper:Shin Yoshizawa Alexander Belyaev Hans-Peter Seidel, "A fast and simple stretch-minimizing mesh parameterization"

template<typename ScalarType>
struct Input_of_MinimumStretchBasedTriangleMesh3DTo2DMapper
{
	const TriangleMesh<ScalarType>* SourceMesh;

	DenseVector<int_max> BoundaryPointIndexList;
	// boundary point index in SourceMesh
	// Boundary Point include all the boundary point of SourceMesh
	// Boundary Point may also include inner point of SourceMesh, such as landmark

	DenseMatrix<ScalarType> UVTableOfBoundary;
	// UVTableOfBoundary[k] = [u, v] of boundary point k of SourceMesh

	ScalarType MaxInteration;// maximum
	ScalarType DiffusionCoefficient;
};

template<typename ScalarType>
struct Internal_of_MinimumStretchBasedTriangleMesh3DTo2DMapper
{
	DenseVector<int_max> InnerPointIndexList;
	// inner point index in SourceMesh

	//ObjectArray<DenseVector<ScalarType>> WeightMatrix;
	// WeightMatrix[i][j] is the weight from point_i to point_j
	// i is from 0 to PointCount_inner, index in InnerPointIndexList
	// j is from 0 to PointCount, index in SourceMesh

	//DenseVector<PointIndexType> PointIndexList;
	//PointIndexList={InnerPointIndexList, BoundaryPointIndexList};

	DenseVector<int_max> Map_PointIndex_to_Inner;
	//Map_PointIndex_to_Inner[PointIndex] is Index in WeightMatrix

	DenseVector<int_max> Map_PointIndex_to_Boundary;
	//Map_PointIndex_to_Boundary[PointIndex] is Index in BoundaryPointIndexList

	DenseVector<ScalarType> ArealListOfSourceMesh;
};

template<typename ScalarType>
struct Output_of_MinimumStretchBasedTriangleMesh3DTo2DMapper
{
	TriangleMesh<ScalarType> ParameterizedSourceMesh;// [x,y,z]->[u,v,0]
};


template<typename Scalar_Type>
class MinimumStretchBasedTriangleMesh3DTo2DMapper : public Object
{
public:
	typedef Scalar_Type ScalarType;

public:
	Input_of_MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType> Input;
private:
	Internal_of_MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType> Internal;
public:
	Output_of_MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType> Output;

public:
	MinimumStretchBasedTriangleMesh3DTo2DMapper();
	~MinimumStretchBasedTriangleMesh3DTo2DMapper();
	void Clear();
	void Update();

private:
	bool CheckInput();
	void Preprocess();
	
	ObjectArray<SparseVector<ScalarType>> ComputeWeightMatrix_MeanValue(const TriangleMesh<ScalarType>& TargetMesh);
	ScalarType ComputeWeight_MeanValue(const TriangleMesh<ScalarType>& TargetMesh, int_max PointH0, int_max PointH1);

	void ApplyStretchMinimizationBasedParameterization();

	DenseVector<ScalarType> ComputeLocalStretch_Face();
	DenseVector<ScalarType> ComputeLocalStretch_Point(const DenseVector<ScalarType>& StretchList_face);
	ScalarType ComputeAverageLocalStretch(const DenseVector<ScalarType>& StretchList_face);

	//return UVTable {uv of inner point, .., uv of boundary point}
	DenseMatrix<ScalarType> ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix);

	ScalarType ComputeFaceAreaOfSourceMesh(int_max FaceIndex);

private:
	MinimumStretchBasedTriangleMesh3DTo2DMapper(const MinimumStretchBasedTriangleMesh3DTo2DMapper&) = delete;
	void operator=(const MinimumStretchBasedTriangleMesh3DTo2DMapper&) = delete;
};

}//namespace

#include "mdkMinimumStretchBasedTriangleMesh3DTo2DMapper.hpp"
