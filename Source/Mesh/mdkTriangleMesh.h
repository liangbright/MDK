#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h

#include "mdkPolygonMesh.h"
#include "mdkTriangleMeshStandardAttribute.h"

namespace mdk
{
//------------------------------- standard/default MeshAttributeType -------------------------------//
template<typename ScalarType>
struct TriangleMeshStandardAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_TriangleMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_TriangleMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_TriangleMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_TriangleMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Cell_Of_TriangleMesh<ScalarType>           CellAttributeType;
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
    typedef typename MeshAttributeType::CellAttributeType           CellAttributeType;

    typedef Point_Of_MembraneMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_MembraneMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_MembraneMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_MembraneMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_MembraneMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_MembraneMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_MembraneMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_MembraneMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType>          CellIteratorType;

public:
    TriangleMesh();
    TriangleMesh(const TriangleMesh& InputMesh);
    TriangleMesh(TriangleMesh&& InputMesh);
    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);
    inline void operator=(TriangleMesh&& InputMesh);

    //------------------------ Add Cell ---------------------------------------//
    using PolygonMesh::AddCellByEdge;
    using PolygonMesh::AddCellByPoint;

    // direction of DirectedEdge: 0->1->2
    inline CellHandleType AddCellByEdge(EdgeHandleType EdgeHandle0, EdgeHandleType EdgeHandle1, EdgeHandleType EdgeHandle2);
    inline CellHandleType AddCellByEdge(int_max EdgeID0, int_max EdgeID1, int_max EdgeID2);

    // direction of DirectedEdge: 0->1->2
    inline CellHandleType AddCellByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1, PointHandleType PointHandle2);
    inline CellHandleType AddCellByPoint(int_max PointID0, int_max PointID1, int_max PointID2);

    //------------ Construct from input data ------------------------------------//

    using PolygonMesh::Construct;

    void Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh);

    //--- check --------------------------//
    bool CheckIfTriangleMesh() const;

    // get a sub mesh by CellHandleList or CellIDList ----------------------------//
    TriangleMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const;
    TriangleMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const;

    //------------- Function optimized For TriangleMesh --------------------------------------------------//

    void UpdateNormalAtCell(); // all
    void UpdateNormalAtCell(CellHandleType CellHandle);
    void UpdateNormalAtCell(int_max CellID);

    void UpdateAreaOfCell(); // all
    void UpdateAreaOfCell(CellHandleType CellHandle);
    void UpdateAreaOfCell(int_max CellID);

    void UpdateCornerAngleOfCell(); // all
    void UpdateCornerAngleOfCell(CellHandleType CellHandle);
    void UpdateCornerAngleOfCell(int_max CellID);

    void UpdateNormalAtPoint(); // all
    void UpdateNormalAtPoint(PointHandleType PointHandle);
    void UpdateNormalAtPoint(int_max PointID);

    void UpdateGaussianCurvatureAtPoint(); // all
    void UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateGaussianCurvatureAtPoint(int_max PointID);

    void UpdateMeanCurvatureAtPoint(); // all
    void UpdateMeanCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateMeanCurvatureAtPoint(int_max PointID);

};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif