#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry3D.h"
#include "mdkTriangleMesh.h"

namespace mdk
{
//paper:Shin Yoshizawa Alexander Belyaev Hans-Peter Seidel, "A fast and simple stretch-minimizing mesh parameterization"

template<typename Scalar_Type>
class MinimumStretchBasedTriangleMesh3DTo2DMapper : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename TriangleMeshStandardAttributeType<ScalarType>       MeshAttributeType;	

private:
	//------------------ input -------------------------------------------//
	TriangleMesh<MeshAttributeType> m_InputMesh;

	DenseVector<int_max> m_BoundaryPointIndexList;
	// boundary point index in m_InputMesh
	// Boundary Point include all the boundary point of input mesh
	// Boundary Point may also include inner point of input mesh, such as landmark

	DenseMatrix<ScalarType> m_UVTableOfBoundary;
	// m_UVTableOfBoundary[k] = [u, v] of boundary point k in m_InputMesh

	ScalarType m_MaxInteration;// maximum
	ScalarType m_DiffusionCoef;

	//-------------- internal -------------------------------------------//
	DenseVector<int_max> m_InnerPointIndexList;
	// inner point index in m_InputMesh

	//ObjectArray<DenseVector<ScalarType>> m_WeightMatrix;
	// m_WeightMatrix[i][j] is the weight from point_i to point_j
	// i is from 0 to PointCount_inner, index in m_InnerPointIndexList
	// j is from 0 to PointCount, index in m_InputMesh

	//DenseVector<PointIndexType> m_PointIndexList;
	//m_PointIndexList={m_InnerPointIndexList, m_BoundaryPointIndexList};

	DenseVector<int_max> m_Map_PointIndex_to_Inner;
	//m_Map_PointIndex_to_Inner[PointIndex] is Index in WeightMatrix

	DenseVector<int_max> m_Map_PointIndex_to_Boundary;
	//m_Map_PointIndex_to_Boundary[PointIndex] is Index in m_BoundaryPointIndexList

	//-------------- output -------------------------------------------//
	TriangleMesh<MeshAttributeType> m_OutputMesh;// [x,y,z]->[u,v,0]

public:
	MinimumStretchBasedTriangleMesh3DTo2DMapper();
	~MinimumStretchBasedTriangleMesh3DTo2DMapper();
	void Clear();
	void SetDiffusionCoefficient(ScalarType Coef);
	void SetMaxIteration(int_max MaxIteration);
	TriangleMesh<MeshAttributeType>& InputMesh() { return m_InputMesh; }
	DenseVector<int_max>& BoundaryPointIndexList() { return m_BoundaryPointIndexList; }
	DenseMatrix<ScalarType>& UVTableOfBoundary() { return m_UVTableOfBoundary; }
	TriangleMesh<MeshAttributeType>& OutputMesh() { return m_OutputMesh; }
	void Update();

private:
	bool CheckInput();
	void Preprocess();
	
	ObjectArray<SparseVector<ScalarType>> ComputeWeightMatrix_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh);
	ScalarType ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max PointH0, int_max PointH1);

	void ApplyStretchMinimizationBasedParameterization();

	DenseVector<ScalarType> ComputeLocalStretch_Point();
	DenseVector<ScalarType> ComputeLocalStretch_Face();
	DenseVector<ScalarType> ComputeLocalStretch_Point(const DenseVector<ScalarType>& StretchList_face);
	ScalarType ComputeAverageLocalStretch(const DenseVector<ScalarType>& StretchList_face);

	//return UVTable {uv of inner point, .., uv of boundary point}
	DenseMatrix<ScalarType> ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix);

private:
	MinimumStretchBasedTriangleMesh3DTo2DMapper(const MinimumStretchBasedTriangleMesh3DTo2DMapper&) = delete;
	void operator=(const MinimumStretchBasedTriangleMesh3DTo2DMapper&) = delete;
};

}//namespace

#include "mdkMinimumStretchBasedTriangleMesh3DTo2DMapper.hpp"