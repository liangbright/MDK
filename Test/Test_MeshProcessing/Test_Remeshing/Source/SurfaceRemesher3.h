#pragma once

#include "mdkTriangleMeshProcessing.h"

namespace mdk
{

template<typename Scalar_Type>
class SurfaceRemesher3 : public Object
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
	DenseVector<EdgeHandleType> m_FeatureEdgeOfInputMesh;   // Feature Edge can not be deleted or splitted
	DenseVector<PointHandleType> m_FeaturePointOfInputMesh; // Feature Point can not be deleted or moved
	// m_FeaturePointOfInputMesh contain the point of m_FeatureEdgeOfInputMesh

	// set candidate score to 0 if any item is below the threshold
	ScalarType m_Threshold_Quad_WarpAngleScore;//min
	ScalarType m_Threshold_Quad_EdgeAngleScore;//min
	ScalarType m_Threshold_Quad_AspectScore;//min

	//------------------- internal ------------------------------------//
	TriangleMesh<MeshAttributeType> m_CandidateMesh;
	//m_CandidateMesh.Face(k) is Candidate-k
	// FaceList: {{BigTriangleCandidate} {SmallTriangleCandidate} {QuadCandidate}}

	int_max m_BigTriangleCandidateCount;

	DenseVector<PointHandleType> m_MiddlePointList_on_CandidateMesh;
	//m_MiddlePointList_on_CandidateMesh[EdgeIndex]

	DenseVector<int> m_CandidateTypeList;
	// 1: one big triangle (input triangle)
	// 2: one small triangle (splitted from each input triangle)
	// 3: one quad from only two big triangle
	// 4: one quad from small and big triangle

	ObjectArray<DenseVector<int_max>> m_CandidateIndexSet_Overlap_SmallTriangle;
	// candidate contain SmallTriangle-k(splitted from input triangle)

	ObjectArray<DenseVector<int_max>> m_QuadCandidateIndexSet_At_SmallTriangle;
	// quad-candidate contain SmallTriangle-k(splitted from input triangle) at object boundary

	ObjectArray<DenseVector<int_max>> m_QuadCandidateIndexSet_At_BigTriangle;
	// quad-candidate contain BigTriangle-k: input triangle

	ObjectArray<DenseVector<int_max>> m_CandidateConflictTable_Overlap;
	//m_CandidateConflictTable_TJunction[k]: other candidate overlap candidate-k at small triangle

	ObjectArray<DenseVector<int_max>> m_CandidateConflictTable_TJunction;
    //m_CandidateConflictTable_TJunction[k]: other candidate touch candidate-k at TJunction and no overlap

	DenseVector<ScalarType> m_CandidateScoreList;
	//m_CandidateScoreList[k]: score of candidate-k

	DenseVector<int> m_CandidateIndicatorList;
	// m_CandidateIndicatorList[k]: -1 ~ not-decided, 0 ~ rejected, 1 ~ selected, 

	//------------------------ output -----------------------------------//
	PolygonMesh<MeshAttributeType> m_OutputMesh;

public:
	SurfaceRemesher3();
	~SurfaceRemesher3();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void InitilizeCandidateMesh();
	void GenerateCandidate();
	void GenerateTriangleCandidate_Type1();
	void GenerateTriangleCandidate_Type2();
	void GenerateQuadCandidate_Type3();
	void GenerateQuadCandidate_Type3(EdgeHandleType EdgeHandle_input);
	void GenerateQuadCandidate_Type4();
	void GenerateQuadCandidate_Type4(EdgeHandleType EdgeHandle_input);
	void Build_CandidateConflictTable_Overlap();
	void Build_CandidateConflictTable_TJunction();
	void Preserve_FeatureEdge();
	void EvaluateCandidate();
	void SelectCandidate();
	void BuildOutputMesh();
	void RefineOutputMesh();

	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList);
	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList);

	DenseVector<int_max> FindCandidate_Conflict_with_QuadCandidate_Type4_b(int_max CandidateIndex, PointHandleType PointH0, EdgeHandleType EdgeH0, EdgeHandleType EdgeH1, EdgeHandleType EdgeH2, EdgeHandleType EdgeH3, EdgeHandleType EdgeH4);
	DenseVector<int_max> FindCandidate_Conflict_with_QuadCandidate_Type4_c(int_max CandidateIndex, PointHandleType PointH0, EdgeHandleType EdgeH0, EdgeHandleType EdgeH1, EdgeHandleType EdgeH2, EdgeHandleType EdgeH3, EdgeHandleType EdgeH4);

	ScalarType EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3);
	ScalarType EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2);
private:
	SurfaceRemesher3(const SurfaceRemesher3&) = delete;
	void operator=(const SurfaceRemesher3&) = delete;
};

}//namespace

#include "SurfaceRemesher3.hpp"