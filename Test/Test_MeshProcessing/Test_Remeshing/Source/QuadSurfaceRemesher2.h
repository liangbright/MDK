#pragma once

#include "mdkTriangleMeshProcessing.h"

namespace mdk
{

template<typename Scalar_Type>
class QuadSurfaceRemesher2 : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename PolygonMeshStandardAttributeType<ScalarType>       MeshAttributeType;
	typedef typename PolygonMesh<MeshAttributeType>::PointHandleType    PointHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::EdgeHandleType     EdgeHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::FaceHandleType     FaceHandleType;

public:
	//------------------- input ------------------------------------//
	TriangleMesh<MeshAttributeType> m_InputMesh;//input mesh is modified by adding middle point on every edge

	DenseVector<EdgeHandleType>  m_FeatureEdgeOfInputMesh;  
	// Feature Edge can not be deleted or splitted in m_OutputMesh_Mixed
	// Feature Edge can not be deleted in m_OutputMesh_Mixed
	// Feature Edge CAN be splitted in m_OutputMesh_Quad

	DenseVector<PointHandleType> m_FeaturePointOfInputMesh; 
	// Feature Point can not be deleted or moved in m_OutputMesh_Mixed and m_OutputMesh_Quad

	// do not generate quad candidate if any item is below the threshold
	// also, quad sore is 0 if any item is below the threshold
	ScalarType m_Threshold_Quad_WarpAngleScore;//min
	ScalarType m_Threshold_Quad_EdgeAngleScore;//min
	ScalarType m_Threshold_Quad_AspectScore;//min

	//------------------- internal ------------------------------------//
	DenseVector<PointHandleType> m_MiddlePointHandleList_on_InputMesh;
	//m_MiddlePointHandleList_on_InputMesh[EdgeIndex]

	PolygonMesh<MeshAttributeType> m_SmallTriangleMesh;
	//m_SmallTriangleMesh.Face(k) is a small triangle splitted from a big triangle m_InputMesh.Face(n)
	// n = std::floor(k/6)
	// k = {6*n, 6*n+1, 6*n+2, 6*n+3, 6*n+4, 6*n+5}

	ObjectArray<DenseVector<DenseVector<PointHandleType>, 2>> m_QuadCandidatePairList;
    // m_QuadCandidatePairList[k] is {QuadCandidateA, QuadCandidatB}, k is index of the pair

	DenseVector<ScalarType> m_QuadCandidatePairScoreList;
	//m_QuadCandidatePairScoreList[k]: Score of Quad Candidate Pair with Index k

	ObjectArray<DenseVector<DenseVector<PointHandleType>, 4>> m_SmallTriangleList_Near_QuadCandidatePair;
	// m_SmallTriangleList_Near_CandidatePair[k] is 4 small triangle near the candidate pair-k

	DenseVector<PointHandleType> m_QuadCandidatePairCenterPointHandleList;

	ObjectArray<DenseVector<int_max>> m_QuadCandidatePairIndexSet_Overlap_SmallTriangle;
	//m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[k]: Quad Candidate Pair that overlap the small triangle (splitted input triangle to two)

	ObjectArray<DenseVector<int_max>> m_QuadCandidatePairConflictTable;
	//CandidatePairConflictTable[k]:  index of other candidate that conflict with candidate-k

	DenseVector<int_max> m_QuadCandidatePairIndicatorList;
	// -1: unknown
	// 1: selected
	// 0: rejected

	//------------------------ output -----------------------------------//
	PolygonMesh<MeshAttributeType> m_OutputMesh_Mixed;//triangle and quad 
	PolygonMesh<MeshAttributeType> m_OutputMesh_Quad; //Quad only

public:
	QuadSurfaceRemesher2();
	~QuadSurfaceRemesher2();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void PreprocessInputMesh();
	void BuildSmallTriangleMesh();
	void GenerateCandidate();
	void GenerateCandidate(PointHandleType PointHandle_input);
	void BuildConflictTable();
	void SelectCandidate();
	void BuildMixedTriQuadMesh();	
	void BuildMixedTriQuadMesh(const DenseVector<DenseVector<PointHandleType>, 2>& QuadPair, PointHandleType CenterPoint);
	void BuildMixedTriQuadMesh(const DenseVector<DenseVector<PointHandleType>, 4>& SmallTriangleSet_Near_QuadPair);
	void AdjustMixedTriQuadMesh();
	void BuildQuadMesh();
	void RefineQuadMesh();

	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList);
	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList);

	DenseVector<PointHandleType> ConvertIndexToPointHandle(const DenseVector<int_max>& IndexList);
	DenseVector<FaceHandleType> ConvertIndexToFaceHandle(const DenseVector<int_max>& IndexList);


	ScalarType EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3);
	ScalarType EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3);
	ScalarType EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2);

	DenseVector<PointHandleType> Intersect(const DenseVector<PointHandleType>& HandleListA, const DenseVector<PointHandleType>& HandleListB);
	DenseVector<FaceHandleType> Intersect(const DenseVector<FaceHandleType>& HandleListA, const DenseVector<FaceHandleType>& HandleListB);

private:
	QuadSurfaceRemesher2(const QuadSurfaceRemesher2&) = delete;
	void operator=(const QuadSurfaceRemesher2&) = delete;
};

}//namespace

#include "QuadSurfaceRemesher2.hpp"