#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry3D.h"
#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename ScalarType>
struct Input_of_MeanValueBasedTriangleMesh3DTo2DMapper
{
	TriangleMesh<ScalarType> SourceMesh;

	DenseVector<int_max> BoundaryPointIndexList;
	// Boundary Point include all the boundary point of input mesh
	// Boundary Point may also include inner point of input mesh, such as landmark

	DenseMatrix<ScalarType> UVTableOfBoundary;
	//UVTableOfBoundary[k] = [u, v] of boundary point k of input mesh
};

struct Internal_of_MeanValueBasedTriangleMesh3DTo2DMapper
{
	DenseVector<int_max> InnerPointIndexList;

	//ObjectArray<DenseVector<ScalarType>> WeightMatrix;
	// WeightMatrix[i][j] is the weight from point_i to point_j
	// i is from 0 to PointCount_inner, index in InnerPointHandleList
	// j is from 0 to PointCount, index in SourceMesh

	//DenseVector<int_max> PointIndexList;
	//PointIndexList={InnerPointIndexList, BoundaryPointIndexList};

	DenseVector<int_max> Map_PointIndex_to_InnerIndex;
	//m_Map_PointIndex_to_InnerIndex[PointIndex] is Index of Weight Matrix A

	DenseVector<int_max> Map_PointIndex_to_BoundaryIndex;
	//Map_PointIndex_to_BoundaryIndex[PointIndex] is Index in BoundaryPointHandleList
};

template<typename ScalarType>
struct Output_of_MeanValueBasedTriangleMesh3DTo2DMapper
{
	TriangleMesh<ScalarType> ParameterizedSourceMesh;// [x,y,z]->[u,v,0]
};


template<typename Scalar_Type>
class MeanValueBasedTriangleMesh3DTo2DMapper : public Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_of_MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType> Input;
private:
	Internal_of_MeanValueBasedTriangleMesh3DTo2DMapper Internal;
public:
	Output_of_MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType> Output;	

public:
	MeanValueBasedTriangleMesh3DTo2DMapper();
	~MeanValueBasedTriangleMesh3DTo2DMapper();
	void Clear();
	void Update();

private:
	bool CheckInput();
	void Preprocess();
	
	void ApplyMeanValueBasedParameterization();
	ObjectArray<SparseVector<ScalarType>> ComputeWeightMatrix_MeanValue(const TriangleMesh<ScalarType>& TargetMesh);
	ScalarType ComputeWeight_MeanValue(const TriangleMesh<ScalarType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1);

	//return UVTable {uv of inner point, .., uv of boundary point}
	DenseMatrix<ScalarType> ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix);

private:
	MeanValueBasedTriangleMesh3DTo2DMapper(const MeanValueBasedTriangleMesh3DTo2DMapper&) = delete;
	void operator=(const MeanValueBasedTriangleMesh3DTo2DMapper&) = delete;
};

}//namespace

#include "mdkMeanValueBasedTriangleMesh3DTo2DMapper.hpp"
