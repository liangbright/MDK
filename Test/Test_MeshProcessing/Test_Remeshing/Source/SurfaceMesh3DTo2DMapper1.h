#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry3D.h"
#include "mdkTriangleMeshProcessing.h"


namespace mdk
{

template<typename Scalar_Type>
class SurfaceMesh3DTo2DMapper1 : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename TriangleMeshStandardAttributeType<ScalarType>       MeshAttributeType;
	typedef typename TriangleMesh<MeshAttributeType>::PointHandleType    PointHandleType;
	typedef typename TriangleMesh<MeshAttributeType>::EdgeHandleType     EdgeHandleType;
	typedef typename TriangleMesh<MeshAttributeType>::FaceHandleType     FaceHandleType;

public:
	//------------------ input -------------------------------------------//
	TriangleMesh<MeshAttributeType> m_InputMesh;

	// Boundary Point include all the boundary point of input mesh
	// Boundary Point may also include inner point of input mesh, such as landmark or region-boundary of input mesh
	DenseVector<PointHandleType> m_BoundaryPointHandleList;
	DenseMatrix<ScalarType> m_UVTableOfBoundary;
	// m_UVTableOfBoundaryOfInputMesh[k] = [u, v] of boundary point k

	//-------------- internal -------------------------------------------//
	DenseVector<PointHandleType> m_InnerPointHandleList;

	//ObjectArray<DenseVector<ScalarType>> m_WeightMatrix;
	// m_WeightMatrix[i][j] is the weight from point_i to point_j
	// i is from 0 to PointCount_inner, index in m_InnerPointHandleList
	// j is from 0 to PointCount, index in m_InputMesh

	//DenseVector<PointHandleType> m_PointHandleList;
	//m_PointHandleList={m_InnerPointHandleList, m_BoundaryPointHandleList};

	DenseVector<int_max> m_Map_PointIndex_to_InnerIndex;
	//m_Map_PointIndex_to_InnerIndex[PointIndex] is Index of Weight Matrix A

	DenseVector<int_max> m_Map_PointIndex_to_BoundaryIndex;
	//m_Map_PointIndex_to_BoundaryIndex[PointIndex] is Index in m_BoundaryPointHandleList

	//-------------- output -------------------------------------------//
	TriangleMesh<MeshAttributeType> m_OutputMesh;// [x,y,z]->[u,v,0]

public:
	SurfaceMesh3DTo2DMapper1();
	~SurfaceMesh3DTo2DMapper1();
	void Clear();
	bool CheckInput();
	void Preprocess();
	void Update();

	void ApplyMeanValueBasedParameterization();
	ObjectArray<SparseVector<ScalarType>> ComputeWeightMatrix_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh);
	ScalarType ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1);

	void ApplyStretchMinimizationBasedParameterization();
	DenseVector<ScalarType> ComputeLocalStretch_StretchMinimization(const TriangleMesh<MeshAttributeType>& InputMesh, const TriangleMesh<MeshAttributeType>& OutputMesh);
	
	//return UVTable {uv of inner point, .., uv of boundary point}
	DenseMatrix<ScalarType> ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix);

	DenseVector<PointHandleType> ConvertIndexToHandle(const DenseVector<int_max>& IndexList);

private:
	SurfaceMesh3DTo2DMapper1(const SurfaceMesh3DTo2DMapper1&) = delete;
	void operator=(const SurfaceMesh3DTo2DMapper1&) = delete;
};

}//namespace

#include "SurfaceMesh3DTo2DMapper1.hpp"