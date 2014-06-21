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

template<typename ScalarType>
struct PolygonMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef PointAttributeType_Of_PolygonMesh<ScalarType>      PointAttributeType;
    typedef EdgeAttribute_Of_PolygonMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_PolygonMesh<ScalarType>           CellAttributeType;
};

template<typename MeshAttributeType>
struct PolygonMeshData
{
    bool IsTriangleMesh;

    SurfaceMesh<MeshAttributeType>  PolygonSurfaceMesh;
};

template<typename MeshAttributeType>
class PolygonMesh : public Object
{
public:
    typedef PolygonMesh<MeshAttributeType> MeshType;

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

    typedef Handle_Of_Point_Of_SurfaceMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>          CellIteratorType;

protected:
    std::shared_ptr<PolygonMeshData<MeshAttributeType>> m_MeshData;

public:
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const PolygonMesh& InputMesh);
    inline bool Copy(const PolygonMesh* InputMesh);

    inline void Share(PolygonMesh& InputMesh);
    inline bool Share(PolygonMesh* InputMesh);

    inline void ForceShare(const PolygonMesh& InputMesh);
    inline bool ForceShare(const PolygonMesh* InputMesh);

    inline void Take(PolygonMesh&& InputMesh);
    inline bool Take(PolygonMesh& InputMesh);
    inline bool Take(PolygonMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetPointNumber() const;
    inline int_max GetEdgeNumber() const;
    inline int_max GetDirectedEdgeNumber() const;
    inline int_max GetCellNumber() const;

    // Get/Set 3D Position by PointHandleList or PointIDList --------------------------------------------------------------------------//

    inline void SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline void SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<PointHandleType>& PointHandleList) const;

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<int_max>& PointIDList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<int_max>& PointIDList) const;

    //----- Get/Set  Mesh Item {Point, Point, Edge, DirectedEdge, Cell} ------//

    inline PointType& Point(PointHandleType PointHandle);
    inline const PointType& Point(PointHandleType PointHandle) const;

    inline PointType& Point(int_max PointID);
    inline const PointType& Point(int_max PointID) const;

    inline EdgeType& Edge(EdgeHandleType EdgeHandle);
    inline const EdgeType& Edge(EdgeHandleType EdgeHandle) const;

    inline EdgeType& Edge(int_max EdgeID);
    inline const EdgeType& Edge(int_max EdgeID) const;

    inline DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle);
    inline const DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const;

    inline DirectedEdgeType& DirectedEdge(int_max DirectedEdgeID);
    inline const DirectedEdgeType& DirectedEdge(int_max DirectedEdgeID) const;

    inline CellType& Cell(CellHandleType CellHandle);
    inline const CellType& Cell(CellHandleType CellHandle) const;

    inline CellType& Cell(int_max CellID);
    inline const CellType& Cell(int_max CellID) const;

    //-------------- check handle -------------------------------------------------------//

    bool IsValidHandle(PointHandleType PointHandle) const;
    bool IsValidHandle(EdgeHandleType EdgeHandle) const;
    bool IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    bool IsValidHandle(CellHandleType CellHandle) const;

    //--------- get HandleList ------------------------------------------------------------//

    inline DenseVector<PointHandleType> GetValidPointHandleList() const;
    inline void GetValidPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const;

    inline DenseVector<EdgeHandleType> GetValidEdgeHandleList() const;
    inline void GetValidEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const;

    inline DenseVector<DirectedEdgeHandleType> GetValidDirectedEdgeHandleList() const;
    inline void GetValidDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const;

    inline DenseVector<CellHandleType> GetValidCellHandleList() const;
    inline void GetValidCellHandleList(DenseVector<CellHandleType>& OutputHandleList) const;

    //----------- get PointHandle by position ----------------------------------------------//
    
    inline PointHandleType GetPointHandle(ScalarType Position[3]) const;
    inline PointHandleType GetPointHandle(ScalarType x, ScalarType y, ScalarType z) const;

    // get handle by ID -----------------------------------------------------------------------//

    inline PointHandleType GetPointHandle(int_max PointID) const;
    inline EdgeHandleType   GetEdgeHandle(int_max EdgeID) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(int_max DirectedEdgeID) const;
    inline CellHandleType   GetCellHandle(int_max CellID) const;

    //----------- get EdgeHandle and DirectedEdgeHandle by PointHandleList or PointIDList ---------------------//

    inline EdgeHandleType GetEdgeHandle(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline EdgeHandleType GetEdgeHandle(int_max PointID0, int_max PointID1) const;

    inline DirectedEdgeHandleType GetDirectedEdgeHandle(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(int_max PointID_start, int_max PointID_end) const;

    //----------- get CellHandle by EdgeHandleList or EdgeIDList ------------------------------------------//

    inline CellHandleType GetCellHandle(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline CellHandleType GetCellHandle(const DenseVector<int_max>& EdgeIDList) const;

    //-------------- check ID -------------------------------------------------------//

    inline bool IsValidPointID(int_max PointID) const;
    inline bool IsValidEdgeID(int_max EdgeID) const;
    inline bool IsValidDirectedEdgeID(int_max DirectedEdgeID) const;
    inline bool IsValidCellID(int_max CellID) const;

    //--------- get IDList ------------------------------------------------------------//

    inline DenseVector<int_max> GetValidPointIDList() const;
    inline void GetValidPointIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetValidEdgeIDList() const;
    inline void GetValidEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetValidDirectedEdgeIDList() const;
    inline void GetValidDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetValidCellIDList() const;
    inline void GetValidCellIDList(DenseVector<int_max>& OutputIDList) const;

    //----------- get ID by handle -----------------------------------------------------------//

    inline int_max GetPointID(PointHandleType PointHandle) const;
    inline int_max GetEdgeID(EdgeHandleType EdgeHandle) const;
    inline int_max GetDirectedEdgeID(DirectedEdgeHandleType DirectedEdgeHandle) const;
    inline int_max GetCellID(CellHandleType CellHandle) const;

    //----------- get PointID by position ----------------------------------------------//

    inline int_max GetPointID(ScalarType Position[3]) const;
    inline int_max GetPointID(ScalarType x, ScalarType y, ScalarType z) const;

    //----------- get EdgeID and DirectedEdgeID by PointHandleList or PointIDList ---------------------//

    inline int_max GetEdgeID(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline int_max GetEdgeID(int_max PointID0, int_max PointID1) const;

    inline int_max GetDirectedEdgeID(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
    inline int_max GetDirectedEdgeID(int_max PointID_start, int_max PointID_end) const;

    //----------- get CellID by EdgeHandleList or EdgeIDList ------------------------------------------//

    inline int_max GetCellID(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline int_max GetCellID(const DenseVector<int_max>& EdgeIDList) const;

    //------------- Iterator --------------------------------------------------------------//

    inline PointIteratorType  GetIteratorOfPoint();
    inline const PointIteratorType  GetIteratorOfPoint() const;

    inline EdgeIteratorType   GetIteratorOfEdge();
    inline const EdgeIteratorType   GetIteratorOfEdge() const;

    inline DirectedEdgeIteratorType   GetIteratorOfDirectedEdge();
    inline const DirectedEdgeIteratorType   GetIteratorOfDirectedEdge() const;

    inline CellIteratorType   GetIteratorOfCell();
    inline const CellIteratorType   GetIteratorOfCell() const;

    // Add Mesh Item -------------------------------------------------------------------------//

    PointHandleType AddPoint(ScalarType Position[3]);
    PointHandleType AddPoint(ScalarType x, ScalarType y, ScalarType z);
    DenseVector<PointHandleType> AddPoint(const DenseMatrix<ScalarType>& PointSet);
    
    EdgeHandleType AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1);
    EdgeHandleType AddEdge(int_max PointID0, int_max PointID1);

    CellHandleType AddCell(const DenseVector<EdgeHandleType>& EdgeHandleList);
    CellHandleType AddCell(const DenseVector<int_max>& EdgeIDList);

    //Delete Mesh Item ----------------------------------------------------------------------//

    bool DeleteCell(CellHandleType CellHandle);
    bool DeleteCell(int_max CellID);

    bool DeleteEdge(EdgeHandleType EdgeHandle);
    bool DeleteEdge(int_max EdgeID);

    bool DeletePoint(PointHandleType PointHandle);
    bool DeletePoint(const DenseVector<PointHandleType>& PointHandleList);

    bool DeletePoint(int_max PointID);
    bool DeletePoint(const DenseVector<int_max>& PointIDList);

    //----------------- Release Unused Memory ------------------------------------//
    // attention: after this function is called, every current index and handle will be invalid
    // and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->PointList)
    void CleanDataStructure();

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputPointPositionTable, const DataArray<DenseVector<int_max>>& InputCellTable);
    // index order in each PointIndexList should be consistent

    bool Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh);

    bool CheckIfTriangleMesh() const;

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> GetPointPositionMatrixAndCellTable() const;

    void GetPointPositionMatrixAndCellTable(DenseMatrix<ScalarType>& PointPositionTable, DataArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//

    void UpdateNormalAtPoint(PointHandleType PointHandle);
    void UpdateNormalAtCell(CellHandleType CellHandle);
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif