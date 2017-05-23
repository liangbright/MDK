#pragma once

#include "mdkPolygonMesh.h"

namespace mdk
{
//---------------------------------- MeshAttributeType ------------------------------------------------------//
template<typename ScalarType>
using TriangleMeshEmptyAttributeType = PolygonMeshEmptyAttributeType<ScalarType>;

template<typename ScalarType>
using TriangleMeshStandardAttributeType = PolygonMeshStandardAttributeType<ScalarType>;
//---------------------------------------------------------------------------------------------------------//

template<typename MeshAttribute_Type>
class TriangleMesh : public PolygonMesh<MeshAttribute_Type>
{
public:
	typedef MeshAttribute_Type MeshAttributeType;

    typedef TriangleMesh<MeshAttributeType> MeshType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute             GlobalAttribute;
    typedef typename MeshAttributeType::GlobalAttribute             GlobalAttribute;
    typedef typename MeshAttributeType::PointAttributeType          PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType           EdgeAttributeType;    
    typedef typename MeshAttributeType::FaceAttributeType           FaceAttributeType;

    typedef Point_Of_PolygonMesh<MeshAttributeType>                 PointType;
    typedef Edge_Of_PolygonMesh<MeshAttributeType>                  EdgeType;
    typedef Face_Of_PolygonMesh<MeshAttributeType>                  FaceType;

	typedef Iterator_Of_Point_Of_PolygonMesh<MeshAttributeType>     PointIteratorType;
	typedef Iterator_Of_Edge_Of_PolygonMesh<MeshAttributeType>      EdgeIteratorType;
	typedef Iterator_Of_Face_Of_PolygonMesh<MeshAttributeType>      FaceIteratorType;

public:
    TriangleMesh();
    TriangleMesh(const TriangleMesh& InputMesh);
    TriangleMesh(TriangleMesh&& InputMesh);
    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);
    inline void operator=(TriangleMesh&& InputMesh);

    //------------------------ Add Face ---------------------------------------//
    using PolygonMesh::AddFaceByEdge;
    using PolygonMesh::AddFaceByPoint;

    // direction of DirectedEdge: 0->1->2
    inline int_max AddFaceByEdge(int_max EdgeIndex0, int_max EdgeIndex1, int_max EdgeIndex2);
    
    // direction of DirectedEdge: 0->1->2
    inline int_max AddFaceByPoint(int_max PointIndex0, int_max PointIndex1, int_max PointIndex2);
    
    //------------ Construct from input data ------------------------------------//

    using PolygonMesh::Construct;

    void Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh);

    // get a sub mesh by FaceIndexList ----------------------------//
    TriangleMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;

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

};

}// namespace mdk

#include "mdkTriangleMesh.hpp"
#include "mdkTriangleMesh_FileIO.hpp"
