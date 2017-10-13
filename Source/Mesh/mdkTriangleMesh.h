#pragma once

#include "mdkPolygonMesh.h"
#include "mdkGeometry.h"

namespace mdk
{

template<typename Scalar_Type>
class TriangleMesh : public PolygonMesh<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;
	//------------------------------------------------------------------------//
	typedef StandardAttribute_Of_Mesh<ScalarType>          MeshAttribute;
	typedef StandardAttribute_Of_Point_Of_Mesh<ScalarType> PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_Mesh<ScalarType>  EdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_Mesh<ScalarType>  FaceAttributeType;
	typedef StandardAttribute_Of_Cell_Of_Mesh<ScalarType>  CellAttributeType;
	//------------------------------------------------------------------------//
	typedef Point_Of_Mesh<ScalarType>  PointType;
	typedef Edge_Of_Mesh<ScalarType>   EdgeType;
	typedef Face_Of_Mesh<ScalarType>   FaceType;
	typedef Cell_Of_Mesh<ScalarType>   CellType;
	//------------------------------------------------------------------------//
	typedef StdObjectVector<PointType> PointListType;
	typedef StdObjectVector<EdgeType>  EdgeListType;
	typedef StdObjectVector<FaceType>  FaceListType;
	typedef StdObjectVector<CellType>  CellListType;
	//------------------------------------------------------------------------//

public:
    TriangleMesh();
	TriangleMesh(const MDK_Symbol_PureEmpty&);
    TriangleMesh(const TriangleMesh& InputMesh);
    TriangleMesh(TriangleMesh&& InputMesh);
	TriangleMesh(const PolygonMesh& InputMesh);
	TriangleMesh(PolygonMesh&& InputMesh);
	TriangleMesh(const Mesh& InputMesh);
	TriangleMesh(Mesh&& InputMesh);
    ~TriangleMesh();

	inline void operator=(const TriangleMesh& InputMesh);
	inline void operator=(TriangleMesh&& InputMesh);
	inline void operator=(const PolygonMesh& InputMesh);
	inline void operator=(PolygonMesh&& InputMesh);
	inline void operator=(const Mesh& InputMesh);
	inline void operator=(Mesh&& InputMesh);

    //------------------------ Add Face ---------------------------------------//
    using PolygonMesh::AddFaceByEdge;
    using PolygonMesh::AddFaceByPoint;

    inline int_max AddFaceByEdge(int_max EdgeIndex0, int_max EdgeIndex1, int_max EdgeIndex2);
    
    inline int_max AddFaceByPoint(int_max PointIndex0, int_max PointIndex1, int_max PointIndex2);    

	TriangleMesh<ScalarType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;
    //------------- Function optimized For TriangleMesh --------------------------------------------------//

	void UpdateNormalAtFace(const MDK_Symbol_ALL&);
    void UpdateNormalAtFace(int_max FaceIndex);

	void UpdateAreaOfFace(const MDK_Symbol_ALL&);
    void UpdateAreaOfFace(int_max FaceIndex);

	void UpdateCornerAngleOfFace(const MDK_Symbol_ALL&);
    void UpdateCornerAngleOfFace(int_max FaceIndex);

	// if Flag_Update_RelativeInfo is true, then  UpdateNormalAtFace() and UpdateCornerAngleOfFace() are called inside this function
	// otherwise, FaceNormal must be available: call UpdateNormalAtFace() and UpdateCornerAngleOfFace()
	void UpdateAngleWeightedNormalAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo = false);
	void UpdateAngleWeightedNormalAtPoint(int_max PointIndex);

	// if Flag_Update_RelativeInfo is true, then UpdateCornerAngleOfFace and UpdateAreaOfFace are called inside this function
	// otherwise run UpdateCornerAngleOfFace and UpdateAreaOfFace first
	void UpdateGaussianCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo = false);
    void UpdateGaussianCurvatureAtPoint(int_max PointIndex);

	// if Flag_Update_RelativeInfo is true, then UpdateAreaOfFace is called inside this function
	// otherwise, run UpdateAreaOfFace first
	void UpdateMeanCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo = false);
    void UpdateMeanCurvatureAtPoint(int_max PointIndex);

	// if Flag_Update_RelativeInfo is true, then UpdateNormalAtFace is called inside this function
	// otherwise, run UpdateNormalAtFace first
	void UpdateNormalBasedCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo = false);
	void UpdateNormalBasedCurvatureAtPoint(int_max PointIndex);

	//-------------------------- mesh editing --------------------------------//

	//collapse an edge (EdgeIndex) to a point (PointIndex) of the edge
	bool CollapseEdge(int_max EdgeIndex01, int_max PointIndex0, bool Flag_HandleSpecialCase = false);

	//if Flag_ReuseEdgeIndex is false, then re-use EdgeIndex01
	//if Flag_ReuseOldFaceIndex is false, then re-use index of face adjacent to EdgeIndex01
	//return flipped EdgeIndex
	int_max FlipEdge(int_max EdgeIndex01, bool Flag_ReuseEdgeIndex = false, bool Flag_ReuseFaceIndex = false);//only support 2 face sharing an edge

	//split face sharing edge, splitting point is PointIndex (not any point of edge)
	//output new face index
	DenseVector<int_max> SplitFaceAtEdge(int_max EdgeIndex12, int_max PointIndex0);

	//add a middle point of the edge to the mesh, get PointIndex0
	//then call SplitFaceAtEdge(EdgeIndex12, PointIndex0)
	//PointIndex0 will NOT be returned
	DenseVector<int_max> SplitFaceAtEdge(int_max EdgeIndex12);
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"
#include "mdkPolygonMesh_FileIO.hpp"
