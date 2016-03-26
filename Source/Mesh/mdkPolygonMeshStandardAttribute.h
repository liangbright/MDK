#pragma once

#include "mdkDenseMatrix.h"
#include "mdkStringHash.h"

namespace mdk
{

//============================================== GlobalAttribute_Of_PolygonMesh ===========================================//

template<typename ScalarType>
struct GlobalStandardAttribute_Of_PolygonMesh
{
	int_max ID; // ID of the mesh
	String Name;// name of the mesh

	std::unordered_map<String, Handle_Of_Point_Of_PolygonMesh, StringHash<String>> Map_PointName_to_PointHandle;
	std::unordered_map<String, Handle_Of_Face_Of_PolygonMesh, StringHash<String>>  Map_FaceName_to_FaceHandle;

	StdObjectVector<DenseVector<Handle_Of_Point_Of_PolygonMesh>>  PointSetList;// PointSetList[k] is PointHandleList Of a PointSet indexed by k
	std::unordered_map<String, int_max, StringHash<String>> Map_PointSetName_to_PointSetIndex;

	StdObjectVector<DenseVector<Handle_Of_Face_Of_PolygonMesh>>  FaceSetList; // FaceSetList[k] is FaceHandleList Of a FaceSet indexed by k
	std::unordered_map<String, int_max, StringHash<String>> Map_FaceSetName_to_FaceSetIndex;

//--------------------------------------------------------------
	GlobalStandardAttribute_Of_PolygonMesh() { this->Clear(); }
	GlobalStandardAttribute_Of_PolygonMesh(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	GlobalStandardAttribute_Of_PolygonMesh(GlobalStandardAttribute_Of_PolygonMesh&& InputAttribute) { (*this) = std::move(InputAttribute); }
	~GlobalStandardAttribute_Of_PolygonMesh() {}

	void operator=(const GlobalStandardAttribute_Of_PolygonMesh& InputAttribute)
    {
		ID = InputAttribute.ID;
		Name = InputAttribute.Name;
		Map_PointName_to_PointHandle = InputAttribute.Map_PointName_to_PointHandle;
		Map_FaceName_to_FaceHandle = InputAttribute.Map_FaceName_to_FaceHandle;
		PointSetList = InputAttribute.PointSetList;
		Map_PointSetName_to_PointSetIndex = InputAttribute.Map_PointSetName_to_PointSetIndex;
		FaceSetList = InputAttribute.FaceSetList;
		Map_FaceSetName_to_FaceSetIndex = InputAttribute.Map_FaceSetName_to_FaceSetIndex;
    }

	void operator=(GlobalStandardAttribute_Of_PolygonMesh&& InputAttribute)
	{
		ID = InputAttribute.ID;
		Name = InputAttribute.Name;
		Map_PointName_to_PointHandle = std::move(InputAttribute.Map_PointName_to_PointHandle);
		Map_FaceName_to_FaceHandle = std::move(InputAttribute.Map_FaceName_to_FaceHandle);
		PointSetList = std::move(InputAttribute.PointSetList);
		Map_PointSetName_to_PointSetIndex = std::move(InputAttribute.Map_PointSetName_to_PointSetIndex);
		FaceSetList = std::move(InputAttribute.FaceSetList);
		Map_FaceSetName_to_FaceSetIndex = std::move(InputAttribute.Map_FaceSetName_to_FaceSetIndex);
	}

    void Clear()
    {
		ID = -1;
		Name = "";
		Map_PointName_to_PointHandle.clear();
		Map_FaceName_to_FaceHandle.clear();
		PointSetList.Clear();
		Map_PointSetName_to_PointSetIndex.clear();
		FaceSetList.Clear();
		Map_FaceSetName_to_FaceSetIndex.clear();
    }

	Handle_Of_Point_Of_PolygonMesh GetPointHandle(const String& PointName) const
	{
		auto it = Map_PointName_to_PointHandle.find(PointName);
		if (it != Map_PointName_to_PointHandle.end())
		{
			return it->second;
		}
		else
		{
			Handle_Of_Point_Of_PolygonMesh EmptyHandle;
			MDK_Error("Unknown PointName: " << PointName << " @ PolygonMesh::Attribute().GetPointHandle()")
			return EmptyHandle;
		}
	}

	DenseVector<Handle_Of_Point_Of_PolygonMesh> GetPointSet(const String& PointSetName) const
	{
		auto it = Map_PointSetName_to_PointSetIndex.find(PointSetName);
		if (it != Map_PointSetName_to_PointSetIndex.end())
		{
			return PointSetList[it->second];
		}
		else
		{
			DenseVector<Handle_Of_Point_Of_PolygonMesh> EmptySet;
			MDK_Error("Unknown PointSetName: " << PointSetName << " @ PolygonMesh::Attribute().GetPointSet()")
			return EmptySet;
		}
	}

	Handle_Of_Face_Of_PolygonMesh GetFaceHandle(const String& FaceName) const
	{
		auto it = Map_FaceName_to_FaceHandle.find(FaceName);
		if (it != Map_FaceName_to_FaceHandle.end())
		{
			return it->second;
		}
		else
		{
			Handle_Of_Face_Of_PolygonMesh EmptyHandle;
			MDK_Error("Unknown FaceName: " << FaceName << " @ PolygonMesh::Attribute().GetFaceHandle()")
			return EmptyHandle;
		}
	}

	DenseVector<Handle_Of_Face_Of_PolygonMesh> GetFaceSet(const String& FaceSetName) const
	{
		auto it = Map_FaceSetName_to_FaceSetIndex.find(FaceSetName);
		if (it != Map_FaceSetName_to_FaceSetIndex.end())
		{
			return FaceSetList[it->second];
		}
		else
		{
			DenseVector<Handle_Of_Face_Of_PolygonMesh> EmptySet;
			MDK_Error("Unknown FaceSetName: " << FaceSetName << " @ PolygonMesh::Attribute().GetFaceSet()")
			return EmptySet;
		}
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
struct StandardAttribute_Of_Point_Of_PolygonMesh
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

//============================================== StandardAttribute_Of_DirectedEdge_Of_PolygonMesh ===========================================//
enum class PolygonMeshDirectedEdgeStandardAttributeEnum
{
	Direction,
};

template<typename ScalarType>
struct StandardAttribute_Of_DirectedEdge_Of_PolygonMesh
{
	DenseVector<ScalarType, 3> Direction; // [X, Y, Z]
//-------------------------------------------------------
	StandardAttribute_Of_DirectedEdge_Of_PolygonMesh() { this->Clear(); }
	StandardAttribute_Of_DirectedEdge_Of_PolygonMesh(const StandardAttribute_Of_DirectedEdge_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
	~StandardAttribute_Of_DirectedEdge_Of_PolygonMesh() {}

	void operator=(const StandardAttribute_Of_DirectedEdge_Of_PolygonMesh& InputAttribute)
    {
		Direction = InputAttribute.Direction;
    }

    void Clear()
    {
		Direction.Fill(0);
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
