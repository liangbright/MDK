#pragma once

#include "mdkTriangleMeshProcessing.h"

namespace mdk
{

template<typename Scalar_Type>
class SurfaceRemesher6 : public Object
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
	PolygonMesh<MeshAttributeType> m_SmallTriangleMesh;
	//m_SmallTriangleMesh.Face(k) is a small triangle splitted from a big triangle m_InputMesh.Face(n)
	// n = std::floor(k/6)
	// k = {6*n, 6*n+1, 6*n+2, 6*n+3, 6*n+4, 6*n+5}

	// Candidate is stored in m_CandidateMesh and m_CandidateRegionList
	// CandidateType:
	// 1: one big triangle (input triangle)
	// 2: one quad from two big triangle
	// 3: one quad from one big triangle and one small triangle
	// 4: one quad from one big triangle and two small triangle

	PolygonMesh<MeshAttributeType> m_CandidateMesh;
	//m_CandidateMesh.Face(k) is Candidate-k
	// FaceList: {{BigTriangleCandidate} {SmallTriangleCandidate} {QuadCandidate}}

	ObjectArray<DenseVector<PointHandleType>> m_CandidateRegionList;
	//m_CandidateRegionList[k]: the region of candidate-k, i.e. all the boundary point (belong to input mesh), more than 4 for quad

	int_max m_BigTriangleCandidateCount;

	DenseVector<PointHandleType> m_MiddlePointList_on_CandidateMesh;
	//m_MiddlePointList_on_CandidateMesh[EdgeIndex]

	ObjectArray<DenseVector<int_max>> m_CandidateIndexSet_Overlap_SmallTriangle;
	// m_CandidateIndexSet_Overlap_SmallTriangle[k]: candidate contain m_SmallTriangleMesh.Face(k)

	ObjectArray<DenseVector<int_max>> m_QuadCandidateIndexSet_At_SmallTriangle;
	// m_QuadCandidateIndexSet_At_SmallTriangle[k]: quad-candidate contain m_SmallTriangleMesh.Face(k) at quad-boundary

	ObjectArray<DenseVector<int_max>> m_QuadCandidateIndexSet_At_BigTriangle;
	// m_QuadCandidateIndexSet_At_BigTriangle[k]: quad-candidate contain m_InputMesh.Face(k)

	ObjectArray<DenseVector<int_max>> m_CandidateConflictTable_Overlap;
	//m_CandidateConflictTable_TJunction[k]: other candidate overlap candidate-k at any small triangle inside candidate-k

	ObjectArray<DenseVector<int_max>> m_CandidateConflictTable_TJunction;
    //m_CandidateConflictTable_TJunction[k]: other candidate touch candidate-k at TJunction and no overlap

	DenseVector<ScalarType> m_CandidateScoreList;
	//m_CandidateScoreList[k]: score of candidate-k

	DenseVector<int_max> m_CandidateIndicatorList;
	// m_CandidateIndicatorList[k]: -1 ~ not-decided, 0 ~ rejected, 1 ~ selected, 

	DenseVector<int_max> m_Map_FaceOfMixedMesh_to_Candidate;
	//generated in BuildMixedTriQuadMesh
	//m_Map_FaceOfMixedMesh_to_Candidate[k] is the candidate index of m_OutputMesh_Mixed.Face(k)

	//------------------------ output -----------------------------------//
	PolygonMesh<MeshAttributeType> m_OutputMesh_Mixed;//triangle and quad 
	PolygonMesh<MeshAttributeType> m_OutputMesh_Quad; //Quad only

public:
	SurfaceRemesher6();
	~SurfaceRemesher6();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void InitilizeCandidateMesh();
	void InitilizeSmallTriangleMesh();
	void GenerateCandidate();
	void GenerateTriangleCandidate_Type1();
	void GenerateQuadCandidate_Type2();
	void GenerateQuadCandidate_Type2(EdgeHandleType EdgeHandle_input);
	void GenerateQuadCandidate_Type3_Type4();
	void GenerateQuadCandidate_Type3_Type4(EdgeHandleType EdgeHandle_input);
	void Build_CandidateConflictTable_Overlap();
	void Build_CandidateConflictTable_TJunction();
	void EvaluateCandidate();
	void SelectCandidate();	
	void SelectCandidate_RegionGrowing();
	void SelectCandidate_CoverTriangle();
	void BuildMixedTriQuadMesh();
	void BuildQuadMesh();

	bool Check_If_Candidate_is_Available(const DenseVector<int_max>& CandidateIndicatorList, int_max CandidateIndex);

	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList);
	DenseVector<int_max> ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList);

	ScalarType EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3);
	ScalarType EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3);
	ScalarType EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2);

private:
	SurfaceRemesher6(const SurfaceRemesher6&) = delete;
	void operator=(const SurfaceRemesher6&) = delete;
};

}//namespace

#include "SurfaceRemesher6.hpp"