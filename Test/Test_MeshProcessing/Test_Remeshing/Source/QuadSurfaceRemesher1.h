#pragma once

#include "mdkTriangleMeshProcessing.h"

namespace mdk
{

template<typename Scalar_Type>
class QuadSurfaceRemesher1 : public Object
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

	ObjectArray<DenseVector<PointHandleType>> m_QuadLineList;
	//m_QuadLineList[k]: quad-line {Pstart, ..., Pend}

	//------------------------ output -----------------------------------//
	PolygonMesh<MeshAttributeType> m_OutputMesh_Mixed;//triangle and quad 
	PolygonMesh<MeshAttributeType> m_OutputMesh_Quad; //Quad only

public:
	QuadSurfaceRemesher1();
	~QuadSurfaceRemesher1();
	void Clear();
	void Update();

//private:
	bool CheckInput();
	void ClearInternalData();
	void PreprocessInputMesh();
	void TraceQuadLine();
	void BuildMixedTriQuadMesh();
	void BuildMixedTriQuadMesh(const DenseVector<PointHandleType>& QuadLine);
	void AdjustMixedTriQuadMesh();
	void BuildQuadMesh();

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
	QuadSurfaceRemesher1(const QuadSurfaceRemesher1&) = delete;
	void operator=(const QuadSurfaceRemesher1&) = delete;
};

}//namespace

#include "QuadSurfaceRemesher1.hpp"