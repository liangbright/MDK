#pragma once

#include "mdkTriangleMeshProcessing"

namespace mdk
{

template<typename Scalar_Type>
class SurfaceRemesher1 : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename PolygonMeshStandardAttributeType<ScalarType>       MeshAttributeType;
	typedef typename PolygonMesh<MeshAttributeType>::PointHandleType    PointHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::EdgeHandleType     EdgeHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::FaceHandleType     FaceHandleType;

public:
	TriangleMesh<MeshAttributeType> m_InputMesh;
	DenseVector<EdgeHandleType> m_FeatureEdgeOfInputMesh;// can not break(inset point to) Feature Edge

	//------------------- internal ------------------------------------//
	DenseVector<PointHandleType> m_MiddlePointOfEdgeOfInputMesh;

	ObjectArray<DenseVector<PointHandleType, 4>> m_QuadCandidateList;
	//m_QuadCandidateList[k]: PointHandle List of Quad Candidate-k
	//m_QuadCandidateList[k][n] is PointHandle on m_InputMesh

	DenseVector<ScalarType> m_QuadCandidateScoreList;
	//m_QuadCandidateScoreList[k]: score of Quad candidate-k

	ObjectArray<DenseVector<int_max>> m_QuadCandiateIndexList_on_each_triangle;
	// m_QuadCandiateIndexList_on_each_triangle[k]: Quad index list on the triangle-k of input mesh
	// constraint: sum{indicator(Quad1), indicator(Quad2),...| quad share the same input triangle} <=1

	//DenseVector<int_max>  m_Constriant_Quad_must_be_selected;
	// constraint[k]: indicator(Quad) = 1

	DenseVector<int> m_QuadCandidateFlagList;
	// m_QuadCandidateFlagList[QuadIndex]: 0, or 1
	// 1: selected, 0: rejected

	//------------------------ output -----------------------------------//
	PolygonMesh<MeshAttributeType> m_OutputMesh;

public:
	SurfaceRemesher1();
	~SurfaceRemesher1();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void FindMiddlePointOfEdgeOfInputMesh();
	void GenerateQuadCandidate();
	void EvaluateQuadCandidate();
	void SelectQuadCandidate();
	void BuildOutputMesh();

	ScalarType EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3);

private:
	SurfaceRemesher1(const SurfaceRemesher1&) = delete;
	void operator=(const SurfaceRemesher1&) = delete;
};

}//namespace