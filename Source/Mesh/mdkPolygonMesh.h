#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h

#include <utility>
#include <cmath>

#include "mdkMembraneMesh.h"
#include "mdkPolygonMeshAttribute.h"
#include "mdkGeometry.h"

namespace mdk
{

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
class PolygonMesh : public MembraneMesh<MeshAttributeType>
{
public:
    typedef PolygonMesh<MeshAttributeType> MeshType;

    typedef MeshAttributeType MeshAttributeType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
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
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputPointPositionTable, const DataArray<DenseVector<int_max>>& InputCellTable);
    // index order in each PointIndexList should be consistent

    void Construct(MembraneMesh<MeshAttributeType> InputMembraneMesh);

    // get a sub mesh by CellHandleList or CellIDList ----------------------------//
    PolygonMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const;
    PolygonMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const;

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> GetPointPositionMatrixAndCellTable() const;

    void GetPointPositionMatrixAndCellTable(DenseMatrix<ScalarType>& PointPositionTable, DataArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//

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

#include "mdkPolygonMesh.hpp"

#endif