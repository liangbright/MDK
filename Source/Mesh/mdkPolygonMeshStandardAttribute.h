#ifndef mdk_PolygonMeshStandardAttribute_h
#define mdk_PolygonMeshStandardAttribute_h

#include "mdkDenseMatrix.h"
#include "mdkMembraneMeshStandardAttribute.h"

namespace mdk
{

//============================================== GlobalAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshGlobalStandardAttributeTypeEnum
{
    MeanFaceArea,
    MeanEdgePhysicalLength
};

template<typename ScalarType>
struct GlobalStandardAttribute_Of_PolygonMesh : GlobalStandardAttribute_Of_MembraneMesh<ScalarType>
{
    ScalarType MeanFaceArea;
    ScalarType MeanEdgePhysicalLength;

//--------------------------------------------------------------
	GlobalStandardAttribute_Of_PolygonMesh() { this->Clear(); }
	GlobalStandardAttribute_Of_PolygonMesh(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~GlobalStandardAttribute_Of_PolygonMesh() {}

	void operator=(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute)
    {
        MeanFaceArea = InputAttribute.MeanFaceArea;
        MeanEdgePhysicalLength = InputAttribute.MeanFaceArea;
    }

    void Clear()
    {
        MeanFaceArea = 0;
        MeanEdgePhysicalLength = 0;
    }
};

//============================================== StandardAttribute_Of_Point_Of_PolygonMesh ===========================================//
enum class PolygonMeshPointStandardAttributeTypeEnum
{
	GaussianCurvature,
	WeightedGaussianCurvature,
    MeanCurvature,
    MeanCurvatureNormal,
	AngleWeightedNormal
};

template<typename ScalarType>
struct StandardAttribute_Of_Point_Of_PolygonMesh : StandardAttribute_Of_Point_Of_MembraneMesh<ScalarType>
{
    ScalarType GaussianCurvature; // [-pi, pi]
    ScalarType WeightedGaussianCurvature; //Weighted by Area
    ScalarType MeanCurvature; // > 0
    DenseVector<ScalarType, 3> MeanCurvatureNormal; // may have different/opposite direction compared to AngleWeightedNormal
	DenseVector<ScalarType, 3> AngleWeightedNormal; // unit normal

//-----------------------------------------------
	StandardAttribute_Of_Point_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_Point_Of_PolygonMesh(const StandardAttribute_Of_Point_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Point_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_Point_Of_PolygonMesh& InputAttribute)
    {
        GaussianCurvature = InputAttribute.GaussianCurvature;
		WeightedGaussianCurvature = InputAttribute.WeightedGaussianCurvature;
        MeanCurvature = InputAttribute.MeanCurvature;
        MeanCurvatureNormal = InputAttribute.MeanCurvatureNormal;
		AngleWeightedNormal = InputAttribute.AngleWeightedNormal;
    }

    void Clear()
    {
        GaussianCurvature = 0;
		WeightedGaussianCurvature = 0;
        MeanCurvature = 0;
        MeanCurvatureNormal.Fill(0);
		AngleWeightedNormal.Fill(0);
    }
};

//============================================== StandardAttribute_Of_Edge_Of_PolygonMesh ===========================================//
enum class PolygonMeshEdgeStandardAttributeEnum
{
    PhysicalLength,
};


template<typename ScalarType>
struct StandardAttribute_Of_Edge_Of_PolygonMesh : StandardAttribute_Of_Edge_Of_MembraneMesh<ScalarType>
{
    ScalarType PhysicalLength;

//----------------------------------------------------------------------
	StandardAttribute_Of_Edge_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_Edge_Of_PolygonMesh(const StandardAttribute_Of_Edge_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Edge_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_Edge_Of_PolygonMesh& InputAttribute)
    {
        PhysicalLength = InputAttribute.PhysicalLength;
    }

    void Clear()
    {
        PhysicalLength = 0;
    }
};

//============================================== StandardAttribute_Of_DirectedEdge_Of_PolygonMesh ===========================================//
enum class PolygonMeshDirectedEdgeStandardAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct StandardAttribute_Of_DirectedEdge_Of_PolygonMesh : StandardAttribute_Of_DirectedEdge_Of_MembraneMesh<ScalarType>
{
    DenseVector<ScalarType, 3> Orientation;
//-------------------------------------------------------
	StandardAttribute_Of_DirectedEdge_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_DirectedEdge_Of_PolygonMesh(const StandardAttribute_Of_DirectedEdge_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_DirectedEdge_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_DirectedEdge_Of_PolygonMesh& InputAttribute)
    {
        Orientation = InputAttribute.Orientation;
    }

    void Clear()
    {
        Orientation.Fill(0);
    }

};

//============================================== StandardAttribute_Of_Face_Of_PolygonMesh ===========================================//
enum class PolygonMeshFaceStandardAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct StandardAttribute_Of_Face_Of_PolygonMesh : StandardAttribute_Of_Face_Of_MembraneMesh<ScalarType>
{
    ScalarType Area;
    DenseVector<ScalarType> CornerAngle;
    DenseVector<ScalarType, 3> Normal;
    bool Flag_ReverseNormalDirection;
//---------------------------------------
	StandardAttribute_Of_Face_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_Face_Of_PolygonMesh(const StandardAttribute_Of_Face_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Face_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_Face_Of_PolygonMesh& InputAttribute)
    {
        Area = InputAttribute.Area;
        CornerAngle = InputAttribute.CornerAngle;
        Normal = InputAttribute.Normal;
        Flag_ReverseNormalDirection = InputAttribute.Flag_ReverseNormalDirection;
    }

    void Clear()
    {
        Area = 0;
        CornerAngle.Clear();
        Normal.Fill(0);
        Flag_ReverseNormalDirection = false;
    }
};

}// namespace mdk


#endif