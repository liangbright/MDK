#pragma once

#include "mdkTriangleMeshProcessing.h"

namespace mdk
{

template<typename Scalar_Type>
class QuadSurfaceRemesher3 : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef typename PolygonMeshStandardAttributeType<ScalarType>       MeshAttributeType;
	typedef typename PolygonMesh<MeshAttributeType>::PointHandleType    PointHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::EdgeHandleType     EdgeHandleType;
	typedef typename PolygonMesh<MeshAttributeType>::FaceHandleType     FaceHandleType;

public:
	//------------------- input ------------------------------------//
	TriangleMesh<MeshAttributeType> m_InputMesh;

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
	TriangleMesh<MeshAttributeType> m_CandidateMesh;
	// add middle point on every edge of input mesh
	// add selected quad to candidate mesh

	DenseVector<PointHandleType> m_MiddlePointHandleList_on_CandidateMesh;
	//m_MiddlePointHandleList_on_CandidateMesh[EdgeIndex]
	// EdgeIndex is the index of input mesh
	// edge and face of m_CandidateMesh will be changed after BuildCandidateMesh

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
	PolygonMesh<MeshAttributeType> m_OutputMesh_Mixed;//triangle and quad, refined from m_CandidateMesh
	PolygonMesh<MeshAttributeType> m_OutputMesh_Quad; //Quad only,  refined from m_CandidateMesh

public:
	QuadSurfaceRemesher3();
	~QuadSurfaceRemesher3();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void InitializeCandidateMesh();
	void BuildSmallTriangleMesh();
	void GenerateCandidate();
	void GenerateCandidate(PointHandleType PointHandle_input);
	void BuildConflictTable();
	void SelectCandidate();
	void BuildCandidateMesh();
	void BuildCandidateMesh_AddQuad(const DenseVector<DenseVector<PointHandleType>, 2>& QuadPair, PointHandleType CenterPoint);
	void BuildCandidateMesh_FillGap(const DenseVector<DenseVector<PointHandleType>, 4>& SmallTriangleSet_Near_QuadPair);
	void BuildMixedTriQuadMesh();
	void BuildMixedTriQuadMesh_5PointStarRegionA();
	void BuildMixedTriQuadMesh_5PointStarRegionA(PointHandleType PointHandle_center, int_max PointCount_input);
	void BuildMixedTriQuadMesh_MergeSmallTriangleAndBigTriangle();
	void BuildMixedTriQuadMesh_MergeTwoAdjacentSmallTriangle();
	void BuildMixedTriQuadMesh_MergeTwoAdjacentTriangle(EdgeHandleType EdgeHandle_shared);
	void BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special();
	void BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special(EdgeHandleType EdgeHandle_shared);
	void BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle();
	void BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle(EdgeHandleType EdgeHandle_shared);
	void BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary();
	void BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary(FaceHandleType FaceHandle);
	void BuildMixedTriQuadMesh_CollapseTriangle(FaceHandleType Triangle_abc, PointHandleType Pa, PointHandleType Pb, PointHandleType Pc);
	void BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle();
	void BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle(FaceHandleType SmallTriangle);
	void BuildMixedTriQuadMesh_SplitQuadToTwoBigTriangle();
	void BuildMixedTriQuadMesh_SplitQuadToTwoBigTriangle(FaceHandleType Quad);

	void BuildMixedTriQuadMesh_Smooth();
	void BuildQuadMesh();


	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList);
	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList);

	DenseVector<PointHandleType> ConvertIndexToPointHandle(const DenseVector<int_max>& IndexList);
	DenseVector<FaceHandleType> ConvertIndexToFaceHandle(const DenseVector<int_max>& IndexList);

	ScalarType EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3);
	ScalarType EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3);
	ScalarType EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2);

	template<typename ElementType>
	DenseVector<ElementType> Intersect(const DenseVector<ElementType>& SetA, const DenseVector<ElementType>& SetB);
	
private:
	QuadSurfaceRemesher3(const QuadSurfaceRemesher3&) = delete;
	void operator=(const QuadSurfaceRemesher3&) = delete;
};

}//namespace

#include "QuadSurfaceRemesher3.hpp"