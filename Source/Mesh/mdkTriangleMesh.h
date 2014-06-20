#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h

#include "mdkPolygonMesh.h"
#include "mdkTriangleMeshAttribute.h"

namespace mdk
{

template<typename ScalarType>
struct TriangleMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef PointAttribute_Of_TriangleMesh<ScalarType>          PointAttributeType;
    typedef EdgeAttribute_Of_TriangleMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_TriangleMesh<ScalarType>           CellAttributeType;
};


template<typename MeshAttributeType>
class TriangleMesh : protected PolygonMesh<MeshAttributeType>
{
public:
    typedef TriangleMesh<MeshAttributeType> MeshType;

    typedef MeshAttributeType MeshAttributeType;

    typedef int_max IndexType;

    typedef typename MeshAttributeType::ScalarType ScalarType;

    typedef typename MeshAttributeType::PointAttributeType          PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType           EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType   DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType           CellAttributeType;

    typedef Point_Of_SurfaceMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_SurfaceMesh         PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>     PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>       EdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>       CellIteratorType;

public:
    TriangleMesh();

    TriangleMesh(const TriangleMesh& InputMesh);

    TriangleMesh(TriangleMesh&& InputMesh);

    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);

    inline void operator=(TriangleMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const TriangleMesh& InputMesh);
    inline bool Copy(const TriangleMesh* InputMesh);

    inline void Share(TriangleMesh& InputMesh);
    inline bool Share(TriangleMesh* InputMesh);

    inline void ForceShare(const TriangleMesh& InputMesh);
    inline bool ForceShare(const TriangleMesh* InputMesh);

    inline void Take(TriangleMesh&& InputMesh);
    inline bool Take(TriangleMesh& InputMesh);
    inline bool Take(TriangleMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetPointNumber() const;
    inline int_max GetEdgeNumber() const;
    inline int_max GetDirectedEdgeNumber() const;
    inline int_max GetCellNumber() const;

    // 3D Position -----------------------------------------------------------

    inline void SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<PointHandleType>& PointHandleList) const;

    //----- Get/Set  Mesh Item {Point, Point, Edge, DirectedEdge, Cell} ------//

    inline PointType& Point(PointHandleType PointHandle);
    inline const PointType& Point(PointHandleType PointHandle) const;

    inline EdgeType& Edge(EdgeHandleType EdgeHandle);
    inline const EdgeType& Edge(EdgeHandleType EdgeHandle) const;

    inline DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle);
    inline const DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const;

    inline CellType& Cell(CellHandleType CellHandle);
    inline const CellType& Cell(CellHandleType CellHandle) const;

    //--------- get HandleList ------------------------------------------------------------//
    inline DenseVector<PointHandleType> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const;

    inline DenseVector<EdgeHandleType> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const;

    inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const;

    inline DenseVector<CellHandleType> GetCellHandleList() const;
    inline void GetCellHandleList(DenseVector<CellHandleType>& OutputHandleList) const;

    //------------- Iterator --------------------------------------------------------------//
    inline PointIteratorType  GetIteratorOfPoint() const;
    inline EdgeIteratorType   GetIteratorOfEdge() const;
    inline CellIteratorType   GetIteratorOfCell() const;

    // use this function and GetIteratorOfEdge as DirectedEdge Iterator
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(EdgeHandleType EdgeHandle, int_max RelativeIndex) const;

    // get handle by ID -----------------------------------------------------------------------//
    inline PointHandleType  GetPointHandle(int_max PointID) const;
    inline EdgeHandleType   GetEdgeHandle(int_max EdgeID) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const;
    inline CellHandleType   GetCellHandle(int_max CellID) const;

    //-------------- check handle -------------------------------------------------------//
    bool IsValidHandle(PointHandleType PointHandle) const;
    bool IsValidHandle(EdgeHandleType EdgeHandle) const;
    bool IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    bool IsValidHandle(CellHandleType CellHandle) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    PointHandleType AddPoint(ScalarType Position[3]);
    PointHandleType AddPoint(ScalarType x, ScalarType y, ScalarType z);
    DenseVector<PointHandleType> AddPoint(const DenseMatrix<ScalarType>& PointSet);

    EdgeHandleType AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1);
    CellHandleType AddCell(const DenseVector<EdgeHandleType>& EdgeHandleList);

    //Delete Mesh Item ----------------------------------------------------------------------//

    bool DeleteCell(CellHandleType CellHandle);
    bool DeleteEdge(EdgeHandleType EdgeHandle);

    bool DeletePoint(PointHandleType PointHandle);
    bool DeletePoint(const DenseVector<PointHandleType>& PointHandleList);

    //---------------------------------------------------------------------------//
    // attention: after this function is called, every index and handle will be changed
    // and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->PointList)
    void CleanDataStructure();

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputPointPositionTable, const DataArray<DenseVector<int_max>>& InputCellTable);
    // index order in each PointIndexList should be consistent

    bool Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh);

    bool Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh);

    bool CheckIfTriangleMesh() const;

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> GetPointPositionMatrixAndCellTable() const;

    void GetPointPositionMatrixAndCellTable(DenseMatrix<ScalarType>& PointPositionTable, DataArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//

    void UpdateNormalAtPoint();
    void UpdateNormalAtCell();
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif