#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h

#include <utility>

#include "mdkSurfaceMesh.h"
#include "mdkPolygonMeshAttribute.h"

namespace mdk
{
//------- forward declare -------//
template<typename MeshAttributeType>
class PolygonMesh;
//--------------------------------//

//------------------------------- standard/default PolygonMeshAttribute -------------------------------//
template<typename ScalarType>
struct PolygonMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef GlobalAttribute_Of_PolygonMesh<ScalarType>         GlobalAttribute;
    typedef PointAttribute_Of_PolygonMesh<ScalarType>          PointAttributeType;
    typedef EdgeAttribute_Of_PolygonMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_PolygonMesh<ScalarType>           CellAttributeType;
};
//------------------------------------------------------------------------------------------------//

template<typename MeshAttributeType>
class PolygonMesh : public SurfaceMesh<MeshAttributeType>
{
public:
    typedef PolygonMesh<MeshAttributeType> MeshType;

    typedef int_max IndexType;

    typedef MeshAttributeType MeshAttributeType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
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
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputPointPositionTable, const DataArray<DenseVector<int_max>>& InputCellTable);
    // index order in each PointIndexList should be consistent

    void Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh);

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> GetPointPositionMatrixAndCellTable() const;

    void GetPointPositionMatrixAndCellTable(DenseMatrix<ScalarType>& PointPositionTable, DataArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//
    void UpdatePointAttribute(PolygonMeshPointAttributeTypeEnum PointAttributeName);
    void UpdateNormalAtPoint(PointHandleType PointHandle);
    void UpdateNormalAtCell(CellHandleType CellHandle);
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif