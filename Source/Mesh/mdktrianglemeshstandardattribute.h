#ifndef mdk_TriangleMeshStandardAttribute_h
#define mdk_TriangleMeshStandardAttribute_h

#include "mdkPolygonMeshStandardAttribute.h"

namespace mdk
{
//============================================== GlobalStandardAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshGlobalStandardAttributeTypeEnum
{
    MeanFaceArea,
    MeanEdgePhysicalLength
};

template<typename ScalarType>
struct GlobalStandardAttribute_Of_TriangleMesh : public GlobalStandardAttribute_Of_PolygonMesh<ScalarType>
{

//--------------------------------------------------------------
	GlobalStandardAttribute_Of_TriangleMesh() { this->Clear(); }
	GlobalStandardAttribute_Of_TriangleMesh(const GlobalStandardAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
	~GlobalStandardAttribute_Of_TriangleMesh() {}

	void operator=(const GlobalStandardAttribute_Of_TriangleMesh& InputAttribute)
    {
		this->GlobalStandardAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
		this->GlobalStandardAttribute_Of_PolygonMesh::Clear();
        MeanFaceArea = 0;
        MeanEdgePhysicalLength = 0;
    }
};

//============================================== PointAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshPointStandardAttributeTypeEnum
{
	GaussianCurvature,
	WeightedGaussianCurvature,
	MeanCurvature,
	MeanCurvatureNormal,
	AngleWeightedNormal
};

template<typename ScalarType>
struct StandardAttribute_Of_Point_Of_TriangleMesh : StandardAttribute_Of_Point_Of_PolygonMesh<ScalarType>
{

//-----------------------------------------------
	StandardAttribute_Of_Point_Of_TriangleMesh() { this->Clear(); }
	StandardAttribute_Of_Point_Of_TriangleMesh(const StandardAttribute_Of_Point_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Point_Of_TriangleMesh() {}

	void operator=(const StandardAttribute_Of_Point_Of_TriangleMesh& InputAttribute)
    {
		this->StandardAttribute_Of_Point_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
		this->StandardAttribute_Of_Point_Of_PolygonMesh::Clear();
    }
};

//============================================== StandardAttribute_Of_Edge_Of_TriangleMesh ===========================================//
enum class TriangleMeshEdgeStandardAttributeEnum
{
    PhysicalLength,
};


template<typename ScalarType>
struct StandardAttribute_Of_Edge_Of_TriangleMesh : public StandardAttribute_Of_Edge_Of_PolygonMesh<ScalarType>
{
//----------------------------------------------------------------------
	StandardAttribute_Of_Edge_Of_TriangleMesh() { this->Clear(); }
	StandardAttribute_Of_Edge_Of_TriangleMesh(const StandardAttribute_Of_Edge_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Edge_Of_TriangleMesh() {}

	void operator=(const StandardAttribute_Of_Edge_Of_TriangleMesh& InputAttribute)
    {
		this->StandardAttribute_Of_Edge_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
		this->StandardAttribute_Of_Edge_Of_PolygonMesh::Clear();
    }
};

//============================================== StandardAttribute_Of_DirectedEdge_Of_TriangleMesh ===========================================//
enum class TriangleMeshDirectedEdgeStandardAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct StandardAttribute_Of_DirectedEdge_Of_TriangleMesh : public StandardAttribute_Of_DirectedEdge_Of_PolygonMesh<ScalarType>
{
    DenseVector<ScalarType, 3> Orientation;
//-------------------------------------------------------
	StandardAttribute_Of_DirectedEdge_Of_TriangleMesh() { this->Clear(); }
	StandardAttribute_Of_DirectedEdge_Of_TriangleMesh(const StandardAttribute_Of_DirectedEdge_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_DirectedEdge_Of_TriangleMesh() {}

	void operator=(const StandardAttribute_Of_DirectedEdge_Of_TriangleMesh& InputAttribute)
    {
		this->StandardAttribute_Of_DirectedEdge_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
		this->StandardAttribute_Of_DirectedEdge_Of_PolygonMesh::Clear();
    }

};

//============================================== StandardAttribute_Of_Face_Of_TriangleMesh ===========================================//
enum class TriangleMeshFaceStandardAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct StandardAttribute_Of_Face_Of_TriangleMesh : public StandardAttribute_Of_Face_Of_PolygonMesh<ScalarType>
{
//---------------------------------------
	StandardAttribute_Of_Face_Of_TriangleMesh() { this->Clear(); }
	StandardAttribute_Of_Face_Of_TriangleMesh(const StandardAttribute_Of_Face_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Face_Of_TriangleMesh() {}

	void operator=(const StandardAttribute_Of_Face_Of_TriangleMesh& InputAttribute)
    {
		this->StandardAttribute_Of_Face_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
		this->StandardAttribute_Of_Face_Of_PolygonMesh::Clear();
        CornerAngle.FastResize(3);
        CornerAngle.Fill(0);
    }
};

}// namespace mdk


#endif