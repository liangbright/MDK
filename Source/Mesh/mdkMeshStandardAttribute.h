#pragma once

#include "mdkDenseMatrix.h"
#include "mdkStringHash.h"

namespace mdk
{

//============================================== StandardAttribute_Of_Mesh ===========================================//

template<typename ScalarType>
struct StandardAttribute_Of_Mesh
{
//--------------------------------------------------------------
	StandardAttribute_Of_Mesh() { this->Clear(); }
	StandardAttribute_Of_Mesh(const StandardAttribute_Of_Mesh& InputAttribute) { (*this) = InputAttribute; }
	StandardAttribute_Of_Mesh(StandardAttribute_Of_Mesh&& InputAttribute) { (*this) = std::move(InputAttribute); }
	~StandardAttribute_Of_Mesh() {}
	void operator=(const StandardAttribute_Of_Mesh& InputAttribute){}
	void operator=(StandardAttribute_Of_Mesh&& InputAttribute){}
	void Clear(){}
};

//============================================== StandardAttribute_Of_Point_Of_Mesh ===========================================//
template<typename ScalarType>
struct StandardAttribute_Of_Point_Of_Mesh
{
    ScalarType GaussianCurvature; // [-pi, pi]
    ScalarType WeightedGaussianCurvature; //Weighted by Area
    ScalarType MeanCurvature; // > 0
	ScalarType NormalBasedCurvature;// 0 to 1
    DenseVector<ScalarType, 3> MeanCurvatureNormal; // may have different/opposite direction compared to AngleWeightedNormal
	DenseVector<ScalarType, 3> AngleWeightedNormal; // unit normal

//-----------------------------------------------
	StandardAttribute_Of_Point_Of_Mesh() { this->Clear(); }
	StandardAttribute_Of_Point_Of_Mesh(const StandardAttribute_Of_Point_Of_Mesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Point_Of_Mesh() {}

	void operator=(const StandardAttribute_Of_Point_Of_Mesh& InputAttribute)
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

//============================================== StandardAttribute_Of_Edge_Of_Mesh ===========================================//
template<typename ScalarType>
struct StandardAttribute_Of_Edge_Of_Mesh
{
    ScalarType Length;
//----------------------------------------------------------------------
	StandardAttribute_Of_Edge_Of_Mesh() { this->Clear(); }
	StandardAttribute_Of_Edge_Of_Mesh(const StandardAttribute_Of_Edge_Of_Mesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Edge_Of_Mesh() {}

	void operator=(const StandardAttribute_Of_Edge_Of_Mesh& InputAttribute)
    {
        Length = InputAttribute.Length;
    }

    void Clear()
    {
        Length = 0;
    }
};

//============================================== StandardAttribute_Of_Face_Of_Mesh ===========================================//
template<typename ScalarType>
struct StandardAttribute_Of_Face_Of_Mesh
{
    ScalarType Area;
    DenseVector<ScalarType> CornerAngle;
    DenseVector<ScalarType, 3> Normal;    
//---------------------------------------
	StandardAttribute_Of_Face_Of_Mesh() { this->Clear(); }
	StandardAttribute_Of_Face_Of_Mesh(const StandardAttribute_Of_Face_Of_Mesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Face_Of_Mesh() {}

	void operator=(const StandardAttribute_Of_Face_Of_Mesh& InputAttribute)
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

//============================================== StandardAttribute_Of_Cell_Of_Mesh ===========================================//
template<typename ScalarType>
struct StandardAttribute_Of_Cell_Of_Mesh
{
	ScalarType Volume;
	DenseVector<ScalarType> SignOfFaceNormal; //SignOfFaceNormal[k]=1 then face normal is outward
	//---------------------------------------
	StandardAttribute_Of_Cell_Of_Mesh() { this->Clear(); }
	StandardAttribute_Of_Cell_Of_Mesh(const StandardAttribute_Of_Cell_Of_Mesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_Cell_Of_Mesh() {}

	void operator=(const StandardAttribute_Of_Cell_Of_Mesh& InputAttribute)
	{
		Volume = InputAttribute.Volume;
		SignOfFaceNormal = InputAttribute.SignOfFaceNormal;		
	}

	void Clear()
	{
		Volume = 0;
		SignOfFaceNormal.Clear();		
	}
};

}// namespace mdk
