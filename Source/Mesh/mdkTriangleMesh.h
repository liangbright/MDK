#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h

#include "mdkPolygonMesh.h"
#include "mdkTriangleMeshAttribute.h"

namespace mdk
{
//------------------------------- standard/default TriangleMeshAttribute -------------------------------//
template<typename ScalarType>
struct TriangleMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef GlobalAttribute_Of_TriangleMesh<ScalarType>         GlobalAttribute;
    typedef PointAttribute_Of_TriangleMesh<ScalarType>          PointAttributeType;
    typedef EdgeAttribute_Of_TriangleMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_TriangleMesh<ScalarType>           CellAttributeType;
};
//---------------------------------------------------------------------------------------------------------//

template<typename MeshAttributeType>
class TriangleMesh : public PolygonMesh<MeshAttributeType>
{
public:
    typedef TriangleMesh<MeshAttributeType> MeshType;

    typedef MeshAttributeType MeshAttributeType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute             GlobalAttribute;
    typedef typename MeshAttributeType::GlobalAttribute             GlobalAttribute;
    typedef typename MeshAttributeType::PointAttributeType          PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType           EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType   DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType           CellAttributeType;

    typedef Point_Of_SurfaceMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_SurfaceMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>          CellIteratorType;

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

    void UpdateNormalAtPoint();
    void UpdateNormalAtCell();
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif