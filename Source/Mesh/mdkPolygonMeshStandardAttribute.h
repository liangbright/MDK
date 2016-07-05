#pragma once

#include "mdkDenseMatrix.h"
#include "mdkStringHash.h"

namespace mdk
{

//============================================== GlobalAttribute_Of_PolygonMesh ===========================================//

template<typename ScalarType>
struct GlobalStandardAttribute_Of_PolygonMesh
{
//--------------------------------------------------------------
	GlobalStandardAttribute_Of_PolygonMesh() { this->Clear(); }
	GlobalStandardAttribute_Of_PolygonMesh(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	GlobalStandardAttribute_Of_PolygonMesh(GlobalStandardAttribute_Of_PolygonMesh&& InputAttribute) { (*this) = std::move(InputAttribute); }
	~GlobalStandardAttribute_Of_PolygonMesh() {}
	void operator=(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute) {}
	void operator=(GlobalStandardAttribute_Of_PolygonMesh&& InputAttribute)	{}
    void Clear() { }
};

//============================================== StandardAttribute_Of_Point_Of_PolygonMesh ===========================================//
enum class PolygonMeshPointStandardAttributeTypeEnum
{
	GaussianCurvature,
	WeightedGaussianCurvature,
    MeanCurvature,
	NormalBasedCurvature,
    MeanCurvatureNormal,
	AngleWeightedNormal
};

template<typename ScalarType>
struct StandardAttribute_Of_Point_Of_PolygonMesh
{
    ScalarType GaussianCurvature; // [-pi, pi]
    ScalarType WeightedGaussianCurvature; //Weighted by Area
    ScalarType MeanCurvature; // > 0
	ScalarType NormalBasedCurvature;// 0 to 1
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
		NormalBasedCurvature = InputAttribute.NormalBasedCurvature;
        MeanCurvatureNormal = InputAttribute.MeanCurvatureNormal;
		AngleWeightedNormal = InputAttribute.AngleWeightedNormal;
    }

    void Clear()
    {
        GaussianCurvature = 0;
		WeightedGaussianCurvature = 0;
        MeanCurvature = 0;
		NormalBasedCurvature = 0;
        MeanCurvatureNormal.Fill(0);
		AngleWeightedNormal.Fill(0);
    }
};

//============================================== StandardAttribute_Of_Edge_Of_PolygonMesh ===========================================//
enum class PolygonMeshEdgeStandardAttributeEnum
{
    Length,
};


template<typename ScalarType>
struct StandardAttribute_Of_Edge_Of_PolygonMesh
{
    ScalarType Length;

//----------------------------------------------------------------------
	StandardAttribute_Of_Edge_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_Edge_Of_PolygonMesh(const StandardAttribute_Of_Edge_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Edge_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_Edge_Of_PolygonMesh& InputAttribute)
    {
        Length = InputAttribute.Length;
    }

    void Clear()
    {
        Length = 0;
    }
};

//============================================== StandardAttribute_Of_Face_Of_PolygonMesh ===========================================//
enum class PolygonMeshFaceStandardAttributeEnum
{
    Area,
	CornerAngle,
	Normal
};

template<typename ScalarType>
struct StandardAttribute_Of_Face_Of_PolygonMesh
{
    ScalarType Area;
    DenseVector<ScalarType> CornerAngle;
    DenseVector<ScalarType, 3> Normal;    
//---------------------------------------
	StandardAttribute_Of_Face_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_Face_Of_PolygonMesh(const StandardAttribute_Of_Face_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Face_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_Face_Of_PolygonMesh& InputAttribute)
    {
        Area = InputAttribute.Area;
        CornerAngle = InputAttribute.CornerAngle;
        Normal = InputAttribute.Normal;
    }

    void Clear()
    {
        Area = 0;
        CornerAngle.Clear();
        Normal.Fill(0);
    }
};

}// namespace mdk
