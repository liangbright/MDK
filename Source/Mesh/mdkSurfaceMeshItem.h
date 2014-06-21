#ifndef __mdkSurfaceMeshItem_h
#define __mdkSurfaceMeshItem_h

#include "mdkObject.h"
#include "mdkDenseMatrix.h"
#include "mdkSurfaceMeshItemHandle.h"


namespace mdk
{
//----------------- forward declare ------------------//
template<typename MeshAttributeType>
class SurfaceMesh;
//---------------------------------------------------//

//======================================  Pure_Empty_SurfaceMesh_Symbol =========================================================//

struct InputStruct_For_Pure_Empty_SurfaceMesh_Symbol
{
    std::string Name = "InputStruct_For_Pure_Empty_SurfaceMesh_Symbol";
};

struct Pure_Empty_SurfaceMesh_Symbol
{
    Pure_Empty_SurfaceMesh_Symbol(const InputStruct_For_Pure_Empty_SurfaceMesh_Symbol& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_Pure_Empty_SurfaceMesh_Symbol")
        {
            MDK_Error("Symbol error @ Pure_Empty_SurfaceMesh_Symbol")
        }
    }

    ~Pure_Empty_SurfaceMesh_Symbol() {}

    // deleted:
    Pure_Empty_SurfaceMesh_Symbol() = delete;
    Pure_Empty_SurfaceMesh_Symbol(const Pure_Empty_SurfaceMesh_Symbol&) = delete;
    Pure_Empty_SurfaceMesh_Symbol(Pure_Empty_SurfaceMesh_Symbol&&) = delete;
    void operator=(const Pure_Empty_SurfaceMesh_Symbol&) = delete;
    void operator=(Pure_Empty_SurfaceMesh_Symbol&&) = delete;
};

static InputStruct_For_Pure_Empty_SurfaceMesh_Symbol This_Is_InputStruct_For_Pure_Empty_SurfaceMesh_Symbol;

static Pure_Empty_SurfaceMesh_Symbol This_Pure_Empty_SurfaceMesh_Symbol(This_Is_InputStruct_For_Pure_Empty_SurfaceMesh_Symbol);

#define MDK_PURE_EMPTY_SURFACEMESH This_Pure_Empty_SurfaceMesh_Symbol

//------------------------------------------------- forward declare --------------------------------------------------------//
template<typename T>
class Point_Of_SurfaceMesh;

template<typename T>
class Edge_Of_SurfaceMesh;

template<typename T>
class DirectedEdge_Of_SurfaceMesh;

struct DirectedEdgeIndex_Of_SurfaceMesh;

template<typename T>
class Cell_Of_SurfaceMesh;
//----------------------------------------------------------------------------------------------------------------------------//

//====================================== Point_Of_SurfaceMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Point_Of_SurfaceMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

    SurfaceMesh<MeshAttributeType> Mesh;

    int_max Index;  // PointIndex : index in Mesh.m_MeshData->PointList; it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    DenseVector<int_max> AdjacentPointIndexList;   // index in Mesh.m_MeshData->PointList

    DenseVector<int_max> AdjacentEdgeIndexList;     // index in Mesh.m_MeshData->EdgeList
    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> OutgoingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the start point
    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> IncomingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the end point 
    
    DenseVector<int_max> AdjacentCellIndexList;     // index in Mesh.m_MeshData->CellList

    //------------------------------------------------

    PointAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Point_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Point_Of_SurfaceMesh() {};
};

template<typename MeshAttributeType>
class Point_Of_SurfaceMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

private:
    std::unique_ptr<Data_Of_Point_Of_SurfaceMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

public:
    inline Point_Of_SurfaceMesh();
    inline Point_Of_SurfaceMesh(const Point_Of_SurfaceMesh<MeshAttributeType>& InputPoint);
    inline Point_Of_SurfaceMesh(Point_Of_SurfaceMesh<MeshAttributeType>&& InputPoint);
    inline ~Point_Of_SurfaceMesh();

    inline void operator=(const Point_Of_SurfaceMesh<MeshAttributeType>& InputPoint);
    inline void operator=(Point_Of_SurfaceMesh<MeshAttributeType>&& InputPoint);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<MeshAttributeType>& ParentMesh);
    
    inline void SetIndex(int_max PointIndex);
    inline int_max GetIndex() const;

    inline DenseVector<int_max>& AdjacentPointIndexList();
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& OutgoingDirectedEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& IncomingDirectedEdgeIndexList();
    inline DenseVector<int_max>& AdjacentCellIndexList();

    //-------------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    bool IsOnEdge() const;
    bool IsOnBoundaryEdge() const;

    inline SurfaceMesh<MeshAttributeType>& GetParentMesh();
    inline const SurfaceMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_Point_Of_SurfaceMesh GetHandle() const;

    inline void SetID(int_max PointID);
    inline int_max GetID() const;

    inline void SetPosition(const ScalarType* Pos);
    inline void SetPosition(ScalarType x, ScalarType y, ScalarType z);

    inline DenseVector<ScalarType, 3> GetPosition() const;
    inline void GetPosition(ScalarType* Pos) const;
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const;

    inline DenseVector<Handle_Of_Point_Of_SurfaceMesh> GetAdjacentPointHandleList() const;
    inline void GetAdjacentPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentPointIDList() const;
    inline void GetAdjacentPointIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetAdjacentPointNumber() const;

    inline DenseVector<Handle_Of_Edge_Of_SurfaceMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetAdjacentEdgeNumber() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> GetOutgoingDirectedEdgeHandleList() const;
    inline void GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetOutgoingDirectedEdgeIDList() const;
    inline void GetOutgoingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetOutgoingDirectedEdgeNumber() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> GetIncomingDirectedEdgeHandleList() const;
    inline void GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetIncomingDirectedEdgeIDList() const;
    inline void GetIncomingDirectedEdgeIDList(DenseVector<int_max>& OutputHandleList) const;

    inline int_max GetIncomingDirectedEdgeNumber() const;

    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline int_max GetAdjacentCellNumber() const;

    inline PointAttributeType& Attribute();
    inline const PointAttributeType& Attribute() const;
};

//====================================== Edge_Of_SurfaceMesh (Cell Wall) ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Edge_Of_SurfaceMesh
{
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    SurfaceMesh<MeshAttributeType> Mesh;
    
    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    int_max PointIndex0;
    int_max PointIndex1;

    //--------------------------------

    EdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Edge_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Edge_Of_SurfaceMesh() {};
};

template<typename MeshAttributeType>
class Edge_Of_SurfaceMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

private:
    std::unique_ptr<Data_Of_Edge_Of_SurfaceMesh<MeshAttributeType>> m_Data;

public:
    inline Edge_Of_SurfaceMesh();
    inline Edge_Of_SurfaceMesh(const Edge_Of_SurfaceMesh<MeshAttributeType>& InputEdge);
    inline Edge_Of_SurfaceMesh(Edge_Of_SurfaceMesh<MeshAttributeType>&& InputEdge);
    inline ~Edge_Of_SurfaceMesh();

    inline void operator=(const Edge_Of_SurfaceMesh<MeshAttributeType>& InputEdge);
    inline void operator=(Edge_Of_SurfaceMesh<MeshAttributeType>&& InputEdge);
    
private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max EdgeIndex);

    inline int_max GetIndex() const;

    inline void SetPointIndexList(const int_max PointIndexList[2]);
    inline void SetPointIndexList(int_max PointIndex0, int_max PointIndex1);

    inline void GetPointIndexList(int_max PointIndexList[2]) const;
    inline void GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const;

    inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;
    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share this edge
    inline DenseVector<int_max> GetAdjacentCellIndexList() const;
    inline void GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share any vertex point of this edge
    inline DenseVector<int_max> GetNeighbourCellIndexList() const;
    inline void GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    //--------------------------------------------------------------------------//
public:

    inline bool IsValid() const;

    bool IsBoundary() const;

    inline SurfaceMesh<MeshAttributeType>& GetParentMesh();
    inline const SurfaceMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_Edge_Of_SurfaceMesh GetHandle() const;

    inline void SetID(int_max EdgeID);
    inline int_max GetID() const;
   
    inline DenseVector<Handle_Of_Point_Of_SurfaceMesh, 2> GetPointHandleList() const;
    inline void GetPointHandleList(Handle_Of_Point_Of_SurfaceMesh OutputHandleList[2]) const;
    inline void GetPointHandleList(Handle_Of_Point_Of_SurfaceMesh& PointHandle0, Handle_Of_Point_Of_SurfaceMesh& PointHandle1) const;

    inline DenseVector<int_max, 2> GetPointIDList() const;
    inline void GetPointIDList(int_max OutputIDList[2]) const;
    inline void GetPointIDList(int_max& PointID0, int_max& PointID1) const;

    inline DenseVector<Handle_Of_Edge_Of_SurfaceMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetAdjacentEdgeNumber() const;

    // Cell share this edge
    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetAdjacentCellNumber() const;

    // Cell share any vertex point of this edge
    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetNeighbourCellHandleList() const;
    inline void GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourCellIDList() const;
    inline void GetNeighbourCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline int_max GetNeighbourCellNumber() const;

    inline EdgeAttributeType& Attribute();
    inline const EdgeAttributeType& Attribute() const;
};

//====================================== DirectedEdge_Of_SurfaceMesh (Cell Plasma Membrane) ===================================================//

template<typename MeshAttributeType>
struct Data_Of_DirectedEdge_Of_SurfaceMesh
{
    typedef typename MeshAttributeType::ScalarType                 ScalarType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType  DirectedEdgeAttributeType;

    SurfaceMesh<MeshAttributeType> Mesh;

    DirectedEdgeIndex_Of_SurfaceMesh Index; // DirectedEdgeIndex of this DirectedEdge, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    int_max CellIndex;           // index in Mesh.m_MeshData->CellList,  It is -1 if this is a boundary Edge

    int_max PointIndex_start;   // index in Mesh.m_MeshData->PointList, the start point of the DirectedEdge 
    int_max PointIndex_end;     // index in Mesh.m_MeshData->PointList, the end point of the DirectedEdge

    // FirendDirectedEdgeIndex.RelativeIndex=0 on the same edge
    // DirectedEdgeIndex_Of_SurfaceMesh FirendDirectedEdgeIndex;  // index of the Friend DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList

    DirectedEdgeIndex_Of_SurfaceMesh NextDirectedEdgeIndex;      // index of the Next DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList
    DirectedEdgeIndex_Of_SurfaceMesh PreviousDirectedEdgeIndex;  // index of the Previous DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList

    //--------------------------------------
    
    DirectedEdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_DirectedEdge_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_DirectedEdge_Of_SurfaceMesh() {};
};

template<typename MeshAttributeType>
class DirectedEdge_Of_SurfaceMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType                 ScalarType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType  DirectedEdgeAttributeType;

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

private:
    std::unique_ptr<Data_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>> m_Data;

public:
    DirectedEdge_Of_SurfaceMesh();
    DirectedEdge_Of_SurfaceMesh(const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& InputDirectedEdge);
    DirectedEdge_Of_SurfaceMesh(DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&& InputDirectedEdge);
    ~DirectedEdge_Of_SurfaceMesh();

    void operator=(const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& InputDirectedEdge);
    void operator=(DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&& InputDirectedEdge);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline void SetStartPointIndex(int_max PointIndex);
    inline void SetEndPointIndex(int_max PointIndex);
    inline void SetEdgeIndex(int_max EdgeIndex);
    inline void SetCellIndex(int_max CellIndex);
    
    inline void SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
    
    inline void SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
   
    //-----------------------------------------------------------------------------------//
    inline int_max GetIndex() const;
    inline int_max GetStartPointIndex() const;
    inline int_max GetEndPointIndex() const;
    inline int_max GetEdgeIndex() const;
    inline int_max GetCellIndex() const;

    inline DirectedEdgeIndex_Of_SurfaceMesh GetFirendDirectedEdgeIndex() const;
    inline DirectedEdgeIndex_Of_SurfaceMesh GetNextDirectedEdgeIndex() const;
    inline DirectedEdgeIndex_Of_SurfaceMesh GetPreviousDirectedEdgeIndex() const;

    inline int_max GetFirendCellIndex() const;

    inline DenseVector<int_max> GetNeighbourCellIndexList() const;
    inline void GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    //-----------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline bool IsBoundary() const;

    inline SurfaceMesh<MeshAttributeType>& GetParentMesh();
    inline const SurfaceMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetHandle() const;

    inline void SetID(int_max DirectedEdgeID);
    inline int_max GetID() const;

    inline Handle_Of_Cell_Of_SurfaceMesh GetCellHandle() const;
    inline int_max GetCellID() const;

    inline Handle_Of_Point_Of_SurfaceMesh GetStartPointHandle() const;
    inline int_max GetStartPointID() const;

    inline Handle_Of_Point_Of_SurfaceMesh GetEndPointHandle() const;
    inline int_max GetEndPointID() const;

    inline Handle_Of_Edge_Of_SurfaceMesh GetEdgeHandle() const;
    inline int_max GetEdgeID() const;

    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetFirendDirectedEdgeHandle() const;
    inline int_max GetFirendDirectedEdgeID() const;

    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetNextDirectedEdgeHandle() const;
    inline int_max GetNextDirectedEdgeID() const;

    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetPreviousDirectedEdgeHandle() const;
    inline int_max GetPreviousDirectedEdgeID() const;

    inline Handle_Of_Cell_Of_SurfaceMesh GetFirendCellHandle() const;
    inline int_max GetFirendCellID() const;

    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetNeighbourCellHandleList() const;
    inline void GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourCellIDList() const;
    inline void GetNeighbourCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline int_max GetNeighbourCellNumber() const;

    inline DirectedEdgeAttributeType& Attribute();
    inline const DirectedEdgeAttributeType& Attribute() const;
};

//====================================== Cell_Of_SurfaceMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Cell_Of_SurfaceMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::CellAttributeType    CellAttributeType;

    SurfaceMesh<MeshAttributeType> Mesh;

    int_max Index; // CellIndex: index of the Cell in Mesh.m_MeshData->CellList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> DirectedEdgeIndexList;

    //--------------------------------------

    CellAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Cell_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Cell_Of_SurfaceMesh() {};
};

template<typename MeshAttributeType>
class Cell_Of_SurfaceMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::CellAttributeType    CellAttributeType;

private:
    std::unique_ptr<Data_Of_Cell_Of_SurfaceMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

public:
    Cell_Of_SurfaceMesh();
    Cell_Of_SurfaceMesh(const Cell_Of_SurfaceMesh<MeshAttributeType>& InputCell);
    Cell_Of_SurfaceMesh(Cell_Of_SurfaceMesh<MeshAttributeType>&& InputCell);
    ~Cell_Of_SurfaceMesh();

    void operator=(const Cell_Of_SurfaceMesh<MeshAttributeType>& InputCell);
    void operator=(Cell_Of_SurfaceMesh<MeshAttributeType>&& InputCell);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max CellIndex);   
    inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& DirectedEdgeIndexList();

    //----------------------------------------------------------------------------//    
    inline int_max GetIndex() const;

    inline DenseVector<int_max> GetPointIndexList() const;
    inline void GetPointIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline DenseVector<int_max> GetEdgeIndexList() const;
    inline void GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share any Edge of this cell, not include this cell
    inline DenseVector<int_max> GetAdjacentCellIndexList() const;
    inline void GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    //--------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline SurfaceMesh<MeshAttributeType>& GetParentMesh();
    inline const SurfaceMesh<MeshAttributeType>& GetParentMesh() const;

    inline Handle_Of_Cell_Of_SurfaceMesh GetHandle() const;

    inline void SetID(int_max CellID);
    inline int_max GetID() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetDirectedEdgeIDList() const;
    inline void GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetDirectedEdgeNumber() const; // the number of Directed Edge

    inline DenseVector<Handle_Of_Point_Of_SurfaceMesh> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetPointIDList() const;
    inline void GetPointIDList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputIDList) const;

    inline int_max GetPointNumber() const; // the number of vertex point

    inline DenseVector<Handle_Of_Edge_Of_SurfaceMesh> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetEdgeIDList() const;
    inline void GetEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetEdgeNumber() const;  // the number of Edge

    // Cell share any Edge of this cell, not include this cell
    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const;

    inline int_max GetAdjacentCellNumber() const;

    inline CellAttributeType& Attribute();
    inline const CellAttributeType& Attribute() const;
};

}// namespace mdk

#include "mdkSurfaceMeshItem.hpp"

#endif