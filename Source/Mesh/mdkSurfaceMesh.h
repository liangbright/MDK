#ifndef __mdkSurfaceMesh_h
#define __mdkSurfaceMesh_h

#include <unordered_map>

#include "mdkSurfaceMeshItem.h"
#include "mdkSurfaceMeshItemIterator.h"
#include "mdkSurfaceMeshAttribute.h"

namespace mdk
{

//------------------------------- standard/default/empty SurfaceMeshAttribute -----------------------//
template<typename ScalarType>
struct SurfaceMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef GlobalAttribute_Of_SurfaceMesh<ScalarType>         GlobalAttribute;
    typedef PointAttribute_Of_SurfaceMesh<ScalarType>          PointAttributeType;
    typedef EdgeAttribute_Of_SurfaceMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_SurfaceMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_SurfaceMesh<ScalarType>           CellAttributeType;
};
//------------------------------------------------------------------------------------------------//

template<typename MeshAttributeType>
struct SurfaceMeshData
{
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType      ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute GlobalAttribute;
    //-------------------------------------------------------------------------------------------//

    DenseMatrix<ScalarType> PointPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // a point is a  of an Edge or a point on Edge Edge

    DataArray<Point_Of_SurfaceMesh<MeshAttributeType>> PointList;

    DenseVector<int_max> PointValidityFlagList;
    // 1: point is an element of the mesh 
    // 0: point is deleted

    DataArray<Edge_Of_SurfaceMesh<MeshAttributeType>> EdgeList;

    DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>>> DirectedEdgePairList;

    // do not use this, DirectedEdge_Of_SurfaceMesh::operator(&&) will not work when append to the list
    //DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2>> DirectedEdgePairList;

    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

    DataArray<Cell_Of_SurfaceMesh<MeshAttributeType>> CellList; // also known as face, facet, element

    DenseVector<int_max>  CellValidityFlagList;
    // 1: Cell is an element of the mesh 
    // 0: Cell is deleted

    // Attention: ID must >= 0
    std::unordered_map<int_max, int_max> Map_PointID_to_PointIndex;
    std::unordered_map<int_max, int_max> Map_EdgeID_to_EdgeIndex;
    std::unordered_map<int_max, DirectedEdgeIndex_Of_SurfaceMesh> Map_DirectedEdgeID_to_DirectedEdgeIndex;
    std::unordered_map<int_max, int_max> Map_CellID_to_CellIndex;

    //Mesh Attribute
    GlobalAttribute Attribute;
};


template<typename MeshAttributeType>
class SurfaceMesh : public Object
{
public:
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType                ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute           GlobalAttribute;
    typedef typename MeshAttributeType::PointAttributeType        PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType         EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType         CellAttributeType;
    //--------------------------------------------------------------------------------------------//
    typedef Point_Of_SurfaceMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_SurfaceMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>           PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>            CellIteratorType;
    //--------------------------------------------------------------------------------------------------//

protected:
    std::shared_ptr<SurfaceMeshData<MeshAttributeType>> m_MeshData;

protected:
    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Point_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;    

    template<typename T>
    friend struct Iterator_Of_Cell_Of_SurfaceMesh;

public:
    SurfaceMesh();
    SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&);
    SurfaceMesh(const SurfaceMesh& InputMesh);
    SurfaceMesh(SurfaceMesh&& InputMesh);
    ~SurfaceMesh();

    inline void operator=(const SurfaceMesh& InputMesh);
    inline void operator=(SurfaceMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const SurfaceMesh& InputMesh);
    inline bool Copy(const SurfaceMesh* InputMesh);

    inline void Share(SurfaceMesh& InputMesh);
    inline bool Share(SurfaceMesh* InputMesh);

    inline void ForceShare(const SurfaceMesh& InputMesh);
    inline bool ForceShare(const SurfaceMesh* InputMesh);

    inline void Take(SurfaceMesh&& InputMesh);
    inline void Take(SurfaceMesh& InputMesh);
    inline bool Take(SurfaceMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetPointNumber() const;
    inline int_max GetEdgeNumber() const;
    inline int_max GetDirectedEdgeNumber() const;
    inline int_max GetCellNumber() const;
 
    //------ Get/Set GlobalAttribute -----------------------------------//

    inline GlobalAttribute& Attribute();
    inline const GlobalAttribute& Attribute() const;

    // Get/Set 3D Position by PointHandle or PointID --------------------------------------------------------------------------//

    inline void SetPointPosition(PointHandleType PointHandle, ScalarType x, ScalarType y, ScalarType z);
    inline void SetPointPosition(PointHandleType PointHandle, const ScalarType Position[3]);

    inline void SetPointPosition(int_max PointID, ScalarType x, ScalarType y, ScalarType z);
    inline void SetPointPosition(int_max PointID, const ScalarType Position[3]);

    inline DenseVector<ScalarType, 3> GetPointPosition(PointHandleType PointHandle);
    inline void GetPointPosition(PointHandleType PointHandle, ScalarType& x, ScalarType& y, ScalarType& z);
    inline void GetPointPosition(PointHandleType PointHandle, ScalarType Position[3]);

    inline DenseVector<ScalarType, 3> GetPointPosition(int_max PointID);
    inline void GetPointPosition(int_max PointID, ScalarType& x, ScalarType& y, ScalarType& z);
    inline void GetPointPosition(int_max PointID, ScalarType Position[3]);

    inline void SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline void SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const;
    inline void GetPointPosition(const DenseVector<PointHandleType>& PointHandleList, DenseMatrix<ScalarType>& PointPositionMatrix) const;

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<int_max>& PointIDList) const;
    inline void GetPointPosition(const DenseVector<int_max>& PointIDList, DenseMatrix<ScalarType>& PointPositionMatrix) const;
    
    //----- Get/Set Mesh Item {Point, , Edge, DirectedEdge, Cell} by using Handle or ID ------//

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

    inline bool IsValidHandle(PointHandleType PointHandle) const;
    inline bool IsValidHandle(EdgeHandleType EdgeHandle) const;
    inline bool IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    inline bool IsValidHandle(CellHandleType CellHandle) const;

    //--------- get HandleList ------------------------------------------------------------//

    inline DenseVector<PointHandleType> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const;

    inline DenseVector<EdgeHandleType> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const;

    inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const;

    inline DenseVector<CellHandleType> GetCellHandleList() const;
    inline void GetCellHandleList(DenseVector<CellHandleType>& OutputHandleList) const;

    //----------- get PointHandle by Position, ID, ----------------------------------------------//

    inline PointHandleType GetPointHandleByPosition(const DenseVector<ScalarType, 3>& Position,
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType GetPointHandleByPosition(const ScalarType Position[3], 
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType GetPointHandleByPosition(ScalarType x, ScalarType y, ScalarType z,
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType  GetPointHandleByID(int_max PointID) const;

    //----------- get EdgeHandle by ID, PointHandleList, PointIDList -------------------------------------//

    inline EdgeHandleType GetEdgeHandleByID(int_max EdgeID) const;
    inline EdgeHandleType GetEdgeHandleByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline EdgeHandleType GetEdgeHandleByPoint(int_max PointID0, int_max PointID1) const;

    //----------- get DirectedEdgeHandle by ID, PointHandleList, PointIDList -------------------------------//

    inline DirectedEdgeHandleType GetDirectedEdgeHandleByID(int_max DirectedEdgeID) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandleByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandleByPoint(int_max PointID_start, int_max PointID_end) const;

    //----------- get CellHandle by ID, PointHandleList or PointIDList EdgeHandleList or EdgeIDList ----------//

    inline CellHandleType GetCellHandleByID(int_max CellID) const;
    inline CellHandleType GetCellHandleByPoint(const DenseVector<PointHandleType>& PointHandleList) const;
    inline CellHandleType GetCellHandleByPoint(const DenseVector<int_max>& PointIDList) const;
    inline CellHandleType GetCellHandleByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline CellHandleType GetCellHandleByEdge(const DenseVector<int_max>& EdgeIDList) const;

    //-------------- check ID -------------------------------------------------------//

    inline bool IsValidPointID(int_max PointID) const;
    inline bool IsValidEdgeID(int_max EdgeID) const;
    inline bool IsValidDirectedEdgeID(int_max DirectedEdgeID) const;
    inline bool IsValidCellID(int_max CellID) const;

    //--------- get IDList ------------------------------------------------------------//

    inline DenseVector<int_max> GetPointIDList() const;
    inline void GetPointIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetEdgeIDList() const;
    inline void GetEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetDirectedEdgeIDList() const;
    inline void GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetCellIDList() const;
    inline void GetCellIDList(DenseVector<int_max>& OutputIDList) const;

    //----------- get Point ID by Handle, Position -----------------------------------------------------------//

    inline int_max GetPointIDByHandle(PointHandleType PointHandle) const;
    inline int_max GetPointIDByPosition(ScalarType Position[3]) const;
    inline int_max GetPointIDByPosition(ScalarType x, ScalarType y, ScalarType z) const;

    //----------- get Edge ID by EdgeHandle, PointHandleList, PointIDList ------------------------------------------------------//

    inline int_max GetEdgeIDByHandle(EdgeHandleType EdgeHandle) const;
    inline int_max GetEdgeIDByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline int_max GetEdgeIDByPoint(int_max PointID0, int_max PointID1) const;

    //----------- get DirectedEdge ID by DirectedEdgeHandle, PointHandle, PointID -------------------------------------------//

    inline int_max GetDirectedEdgeIDByHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    inline int_max GetDirectedEdgeIDByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
    inline int_max GetDirectedEdgeIDByPoint(int_max PointID_start, int_max PointID_end) const;

    //----------- get CellID by CellHandle, EdgeHandleList, EdgeIDList, PointHandleList, PointIDList --------------------------//

    inline int_max GetCellIDByHandle(CellHandleType CellHandle) const;
    inline int_max GetCellIDByPoint(const DenseVector<PointHandleType>& PointHandleList) const;
    inline int_max GetCellIDByPoint(const DenseVector<int_max>& PointIDList) const;
    inline int_max GetCellIDByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline int_max GetCellIDByEdge(const DenseVector<int_max>& EdgeIDList) const;

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
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointHandle -> PointIndex in m_MeshData->PointList
    PointHandleType AddPoint(const DenseVector<ScalarType, 3>& Position);
    PointHandleType AddPoint(const DenseVector<ScalarType>& Position);
    PointHandleType AddPoint(const DenseMatrix<ScalarType>& Position);
    PointHandleType AddPoint(const ScalarType Position[3]);
    PointHandleType AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointHandleList
    DenseVector<PointHandleType> AddPointSet(const DenseMatrix<ScalarType>& PointSet);

    // add an Edge and return EdgeHandle -> EdgeIndex in m_MeshData->EdgeList
    // also create invalid DirectedEdge to hold place at m_MeshData->DirectedEdgeList[EdgeIndex]
    // Check is performed in the function to ensure that an edge will not be added more than once
    EdgeHandleType AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1);
    EdgeHandleType AddEdge(int_max PointID0, int_max PointID1);

    // add a cell and return CellHandle -> CellIndex in m_MeshData->CellList
    // add DirectedEdge of the cell
    // the order of Edge in EdgeHandleList determine the direction of each DirectedEdge and the direction/sign of the normal vector
    CellHandleType AddCellByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList);
    CellHandleType AddCellByEdge(const DenseVector<int_max>& EdgeIDList);

    // Add Cell with PointHandleList, Point0 <- Edge0 -> Point1 <- Edge1 -> Point2 ... Point_end <- Edge_end -> Point0
    // in this function, AddCellByEdge() is called if necessary
    CellHandleType AddCellByPoint(const DenseVector<PointHandleType>& PointHandleList);
    CellHandleType AddCellByPoint(const DenseVector<int_max>& PointIDList);

    // Delete Mesh Item ----------------------------------------------------------------------------//

    // m_MeshData->CellList[CellIndex].Clear() only release memory
    // this function will remove each DirectedEdge of the Cell, and modify any information related to the cell
    // CellHandle and CellID of the cell become invalid after the cell is deleted
    bool DeleteCell(CellHandleType CellHandle);
    bool DeleteCell(int_max CellID);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only release memory
    // this function will modify any information related to the Edge
    // EdgeHandle and EdgeID of the edge become invalid after the edge is deleted
    // Check is performed in the function to make sure an edge can not be deleted if any adjacent cell exit
    bool DeleteEdge(EdgeHandleType EdgeHandle);
    bool DeleteEdge(int_max EdgeID);

    // m_MeshData->PointList[PointIndex].Cear() only release memory
    // this function will modify any information related to the point
    // Check is performed in the function to make sure a point can not be deleted if any adjacent edge exit
    // PointHandle and PointID of the point become invalid after the point is deleted
    bool DeletePoint(PointHandleType PointHandle);
    bool DeletePoint(const DenseVector<PointHandleType>& PointHandleList);

    bool DeletePoint(int_max PointID);
    bool DeletePoint(const DenseVector<int_max>& PointIDList);

    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after this function is called, handle may become invalid, but, ID will not change
    // use this function when InvalidPointHandleNumber/ValidPointNumber( GetPointNumber() ) > 0.5
    void CleanDataStructure();
    
    int_max GetInvalidPointHandleNumber() const; // the number of invalid point handles

    //---------------------------------------------------------------------------------------------------

    // get a sub mesh by CellHandleList or CellIDList
    SurfaceMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const;
    SurfaceMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const;

    //---------------------------------------------------------------------------------------------------

};

}// namespace mdk

#include "mdkSurfaceMesh.hpp"

#endif