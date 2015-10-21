#ifndef mdk_TriangleMesh_h
#define mdk_TriangleMesh_h

#include "mdkPolygonMesh.h"
#include "mdkTriangleMeshStandardAttribute.h"

namespace mdk
{
//------------------------------- Empty MeshAttributeType -------------------------------//
template<typename ScalarType>
struct TriangleMeshEmptyAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_MembraneMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_MembraneMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_MembraneMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_MembraneMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_MembraneMesh<ScalarType>           FaceAttributeType;
};
//---------------------------------------------------------------------------------------------------------//

//------------------------------- standard/default MeshAttributeType -------------------------------//
template<typename ScalarType>
struct TriangleMeshStandardAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_TriangleMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_TriangleMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_TriangleMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_TriangleMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_TriangleMesh<ScalarType>           FaceAttributeType;
};
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
    typedef typename MeshAttributeType::DirectedEdgeAttributeType   DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::FaceAttributeType           FaceAttributeType;

    typedef Point_Of_MembraneMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_MembraneMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_MembraneMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Face_Of_MembraneMesh<MeshAttributeType>            FaceType;

    typedef Handle_Of_Point_Of_MembraneMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_MembraneMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_MembraneMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Face_Of_MembraneMesh           FaceHandleType;

    typedef Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType>          FaceIteratorType;

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
    inline FaceHandleType AddFaceByEdge(EdgeHandleType EdgeHandle0, EdgeHandleType EdgeHandle1, EdgeHandleType EdgeHandle2);
    inline FaceHandleType AddFaceByEdge(int_max EdgeID0, int_max EdgeID1, int_max EdgeID2);

    // direction of DirectedEdge: 0->1->2
    inline FaceHandleType AddFaceByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1, PointHandleType PointHandle2);
    inline FaceHandleType AddFaceByPoint(int_max PointID0, int_max PointID1, int_max PointID2);

    //------------ Construct from input data ------------------------------------//

    using PolygonMesh::Construct;

    void Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh);

    //--- check --------------------------//
    bool CheckIfTriangleMesh() const;

    // get a sub mesh by FaceHandleList or FaceIDList ----------------------------//
    TriangleMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const;
    TriangleMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<int_max>& FaceIDList) const;

    //------------- Function optimized For TriangleMesh --------------------------------------------------//

	void UpdateNormalAtFace(const MDK_Symbol_ALL&);
    void UpdateNormalAtFace(FaceHandleType FaceHandle);
    void UpdateNormalAtFace(int_max FaceID);

	void UpdateAreaOfFace(const MDK_Symbol_ALL&);
    void UpdateAreaOfFace(FaceHandleType FaceHandle);
    void UpdateAreaOfFace(int_max FaceID);

	void UpdateCornerAngleOfFace(const MDK_Symbol_ALL&);
    void UpdateCornerAngleOfFace(FaceHandleType FaceHandle);
    void UpdateCornerAngleOfFace(int_max FaceID);

	// FaceNormal must be available: call UpdateNormalAtFace() and UpdateCornerAngleOfFace()
	void UpdateAngleWeightedNormalAtPoint(const MDK_Symbol_ALL&);
	void UpdateAngleWeightedNormalAtPoint(PointHandleType PointHandle);
	void UpdateAngleWeightedNormalAtPoint(int_max PointID);

	// run UpdateCornerAngleOfFace and UpdateAreaOfFace first
	void UpdateGaussianCurvatureAtPoint(const MDK_Symbol_ALL&);
    void UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateGaussianCurvatureAtPoint(int_max PointID);

	// run UpdateAreaOfFace() first
	void UpdateMeanCurvatureAtPoint(const MDK_Symbol_ALL&);
    void UpdateMeanCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateMeanCurvatureAtPoint(int_max PointID);

};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif