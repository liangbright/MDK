#ifndef mdk_PolygonMesh_h
#define mdk_PolygonMesh_h

#include <utility>
#include <cmath>

#include "mdkMembraneMesh.h"
#include "mdkPolygonMeshStandardAttribute.h"
#include "mdkGeometry3D.h"

namespace mdk
{

//------------------------------- Empty MeshAttributeType -------------------------------//
template<typename ScalarType>
struct PolygonMeshEmptyAttributeType
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
struct PolygonMeshStandardAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_PolygonMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_PolygonMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_PolygonMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_PolygonMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_PolygonMesh<ScalarType>           FaceAttributeType;
};
//------------------------------------------------------------------------------------------------//

template<typename MeshAttribute_Type>
class PolygonMesh : public MembraneMesh<MeshAttribute_Type>
{
public:
	typedef MeshAttribute_Type MeshAttributeType;

    typedef PolygonMesh<MeshAttributeType> MeshType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
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
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //------------ Construct from input data ------------------------------------//

	void Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable);
    // index order in each PointIndexList should be consistent

    void Construct(MembraneMesh<MeshAttributeType> InputMembraneMesh);

    // get a sub mesh by FaceHandleList or FaceIDList ----------------------------//
    PolygonMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const;
    PolygonMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<int_max>& FaceIDList) const;

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> GetPointPositionMatrixAndFaceTable() const;

	void GetPointPositionMatrixAndFaceTable(DenseMatrix<ScalarType>& PointPositionTable, ObjectArray<DenseVector<int_max>>& FaceTable) const;

    //------------- Mesh Attribute --------------------------------------------------//
	/*
    void UpdateNormalAtFace(); // all
    void UpdateNormalAtFace(FaceHandleType FaceHandle);
    void UpdateNormalAtFace(int_max FaceID);

    void UpdateAreaOfFace(); // all
    void UpdateAreaOfFace(FaceHandleType FaceHandle);
    void UpdateAreaOfFace(int_max FaceID);

    void UpdateCornerAngleOfFace(); // all
    void UpdateCornerAngleOfFace(FaceHandleType FaceHandle);
    void UpdateCornerAngleOfFace(int_max FaceID);

	void UpdateAngleWeightedNormalAtPoint(); // all
	void UpdateAngleWeightedNormalAtPoint(PointHandleType PointHandle);
	void UpdateAngleWeightedNormalAtPoint(int_max PointID);

    void UpdateGaussianCurvatureAtPoint(); // all
    void UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateGaussianCurvatureAtPoint(int_max PointID);

    void UpdateMeanCurvatureAtPoint(); // all
    void UpdateMeanCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateMeanCurvatureAtPoint(int_max PointID);
	*/
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif