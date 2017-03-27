#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry3D.h"
#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class MeanValueBasedTriangleMesh3DTo2DMapper : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename TriangleMeshStandardAttributeType<ScalarType>       MeshAttributeType;
	typedef typename TriangleMesh<MeshAttributeType>::PointHandleType    PointHandleType;
	typedef typename TriangleMesh<MeshAttributeType>::EdgeHandleType     EdgeHandleType;
	typedef typename TriangleMesh<MeshAttributeType>::FaceHandleType     FaceHandleType;

public:
	//------------------ input -------------------------------------------//
	TriangleMesh<MeshAttributeType> InputMesh;

	DenseVector<PointHandleType> BoundaryPointHandleList;
	// Boundary Point include all the boundary point of input mesh
	// Boundary Point may also include inner point of input mesh, such as landmark

	DenseMatrix<ScalarType> UVTableOfBoundary;
	//UVTableOfBoundary[k] = [u, v] of boundary point k of input mesh

private:
	//-------------- internal -------------------------------------------//
	DenseVector<PointHandleType> InnerPointHandleList;

	//ObjectArray<DenseVector<ScalarType>> WeightMatrix;
	// WeightMatrix[i][j] is the weight from point_i to point_j
	// i is from 0 to PointCount_inner, index in InnerPointHandleList
	// j is from 0 to PointCount, index in m_InputMesh

	//DenseVector<PointHandleType> PointHandleList;
	//PointHandleList={InnerPointHandleList, BoundaryPointHandleList};

	DenseVector<int_max> Map_PointIndex_to_InnerIndex;
	//m_Map_PointIndex_to_InnerIndex[PointIndex] is Index of Weight Matrix A

	DenseVector<int_max> Map_PointIndex_to_BoundaryIndex;
	//Map_PointIndex_to_BoundaryIndex[PointIndex] is Index in BoundaryPointHandleList

public:
	//-------------- output -------------------------------------------//
	TriangleMesh<MeshAttributeType> OutputMesh;// [x,y,z]->[u,v,0]

public:
	MeanValueBasedTriangleMesh3DTo2DMapper();
	~MeanValueBasedTriangleMesh3DTo2DMapper();
	void Clear();
	void Update();

private:
	bool CheckInput();
	void Preprocess();
	
	void ApplyMeanValueBasedParameterization();
	ObjectArray<SparseVector<ScalarType>> ComputeWeightMatrix_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh);
	ScalarType ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1);

	//return UVTable {uv of inner point, .., uv of boundary point}
	DenseMatrix<ScalarType> ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix);

	DenseVector<PointHandleType> ConvertIndexToHandle(const DenseVector<int_max>& IndexList);

private:
	MeanValueBasedTriangleMesh3DTo2DMapper(const MeanValueBasedTriangleMesh3DTo2DMapper&) = delete;
	void operator=(const MeanValueBasedTriangleMesh3DTo2DMapper&) = delete;
};

}//namespace

#include "mdkMeanValueBasedTriangleMesh3DTo2DMapper.hpp"
