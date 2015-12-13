#ifndef mdk_TriangleMesh_h
#define mdk_TriangleMesh_h

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
    
    // direction of DirectedEdge: 0->1->2
    inline FaceHandleType AddFaceByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1, PointHandleType PointHandle2);
    
    //------------ Construct from input data ------------------------------------//

    using PolygonMesh::Construct;

    void Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh);

    //--- check --------------------------//
    bool CheckIfTriangleMesh() const;

    // get a sub mesh by FaceHandleList ----------------------------//
    TriangleMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const;

    //------------- Function optimized For TriangleMesh --------------------------------------------------//

	void UpdateNormalAtFace(const MDK_Symbol_ALL&);
    void UpdateNormalAtFace(FaceHandleType FaceHandle);

	void UpdateAreaOfFace(const MDK_Symbol_ALL&);
    void UpdateAreaOfFace(FaceHandleType FaceHandle);

	void UpdateCornerAngleOfFace(const MDK_Symbol_ALL&);
    void UpdateCornerAngleOfFace(FaceHandleType FaceHandle);

	// FaceNormal must be available: call UpdateNormalAtFace() and UpdateCornerAngleOfFace()
	void UpdateAngleWeightedNormalAtPoint(const MDK_Symbol_ALL&);
	void UpdateAngleWeightedNormalAtPoint(PointHandleType PointHandle);

	// run UpdateCornerAngleOfFace and UpdateAreaOfFace first
	void UpdateGaussianCurvatureAtPoint(const MDK_Symbol_ALL&);
    void UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle);

	// run UpdateAreaOfFace() first
	void UpdateMeanCurvatureAtPoint(const MDK_Symbol_ALL&);
    void UpdateMeanCurvatureAtPoint(PointHandleType PointHandle);

};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif