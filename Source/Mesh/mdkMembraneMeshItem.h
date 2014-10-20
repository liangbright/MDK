#ifndef __mdkMembraneMeshItem_h
#define __mdkMembraneMeshItem_h

#include "mdkObject.h"
#include "mdkSimpleObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkMembraneMeshItemHandle.h"

namespace mdk
{
//----------------- forward declare ------------------//
template<typename MeshAttributeType>
class MembraneMesh;
//---------------------------------------------------//

#define MDK_PURE_EMPTY_MembraneMesh StaticGlobalObject_MDK_Symbol_PureEmpty

//------------------------------------------------- forward declare --------------------------------------------------------//
template<typename T>
class Point_Of_MembraneMesh;

template<typename T>
class Edge_Of_MembraneMesh;

template<typename T>
class DirectedEdge_Of_MembraneMesh;

struct DirectedEdgeIndex_Of_MembraneMesh;

template<typename T>
class Cell_Of_MembraneMesh;
//----------------------------------------------------------------------------------------------------------------------------//

//====================================== Point_Of_MembraneMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Point_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    int_max Index;  // PointIndex : index in Mesh.m_MeshData->PointList; it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure(), must >= 0

    DenseVector<int_max> AdjacentPointIndexList;   // index in Mesh.m_MeshData->PointList

    DenseVector<int_max> AdjacentEdgeIndexList;     // index in Mesh.m_MeshData->EdgeList
    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutgoingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the start point
    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> IncomingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the end point 
    
    DenseVector<int_max> AdjacentCellIndexList;     // index in Mesh.m_MeshData->CellList

    //------------------------------------------------

    PointAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Point_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY_MembraneMesh) {}
    ~Data_Of_Point_Of_MembraneMesh() {};
};

template<typename MeshAttributeType>
class Point_Of_MembraneMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

private:
    std::unique_ptr<Data_Of_Point_Of_MembraneMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class MembraneMesh;

public:
    inline Point_Of_MembraneMesh();
    inline Point_Of_MembraneMesh(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint);
    inline Point_Of_MembraneMesh(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint);
    inline ~Point_Of_MembraneMesh();

    inline void operator=(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint);
    inline void operator=(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    
    inline void SetIndex(int_max PointIndex);
    inline int_max GetIndex() const;

    inline DenseVector<int_max>& AdjacentPointIndexList();
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& OutgoingDirectedEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& IncomingDirectedEdgeIndexList();
    inline DenseVector<int_max>& AdjacentCellIndexList();

    //-------------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    bool IsOnEdge() const;
    bool IsOnBoundaryEdge() const;

    inline MembraneMesh<MeshAttributeType>& GetParentMesh();
    inline const MembraneMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_Point_Of_MembraneMesh GetHandle() const;

    inline void SetID(int_max PointID);
    inline void EraseID();
    inline int_max GetID() const;

    inline void SetPosition(const DenseVector<ScalarType, 3>& Pos);
    inline void SetPosition(const ScalarType Pos[3]);
    inline void SetPosition(ScalarType x, ScalarType y, ScalarType z);

    inline DenseVector<ScalarType, 3> GetPosition() const;
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const;
    inline void GetPosition(ScalarType Pos[3]) const;

	inline int_max GetAdjacentPointNumber() const;

    inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetAdjacentPointHandleList() const;
    inline void GetAdjacentPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentPointIDList() const;
    inline void GetAdjacentPointIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetAdjacentEdgeNumber() const;

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetOutgoingDirectedEdgeNumber() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetOutgoingDirectedEdgeHandleList() const;
    inline void GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetOutgoingDirectedEdgeIDList() const;
    inline void GetOutgoingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetIncomingDirectedEdgeNumber() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetIncomingDirectedEdgeHandleList() const;
    inline void GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetIncomingDirectedEdgeIDList() const;
    inline void GetIncomingDirectedEdgeIDList(DenseVector<int_max>& OutputHandleList) const;

	inline int_max GetAdjacentCellNumber() const;

    inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline PointAttributeType& Attribute();
    inline const PointAttributeType& Attribute() const;
};

//====================================== Edge_Of_MembraneMesh (Cell Wall) ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Edge_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;
    
    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    int_max PointIndex0;
    int_max PointIndex1;

	SimpleObjectArray<DirectedEdge_Of_MembraneMesh<MeshAttributeType>> DirectedEdgeList;

    //--------------------------------

    EdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Edge_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY_MembraneMesh) {}
    ~Data_Of_Edge_Of_MembraneMesh() {};
};

template<typename MeshAttributeType>
class Edge_Of_MembraneMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    template<typename T>
    friend class MembraneMesh;

    template<typename T>
    friend class Point_Of_MembraneMesh;

    template<typename T>
    friend class DirectedEdge_Of_MembraneMesh;

    template<typename T>
    friend class Cell_Of_MembraneMesh;

private:
    std::unique_ptr<Data_Of_Edge_Of_MembraneMesh<MeshAttributeType>> m_Data;

public:
    inline Edge_Of_MembraneMesh();
    inline Edge_Of_MembraneMesh(const Edge_Of_MembraneMesh<MeshAttributeType>& InputEdge);
    inline Edge_Of_MembraneMesh(Edge_Of_MembraneMesh<MeshAttributeType>&& InputEdge);
    inline ~Edge_Of_MembraneMesh();

    inline void operator=(const Edge_Of_MembraneMesh<MeshAttributeType>& InputEdge);
    inline void operator=(Edge_Of_MembraneMesh<MeshAttributeType>&& InputEdge);
    
private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max EdgeIndex);

    inline void SetPointIndexList(const int_max PointIndexList[2]);
    inline void SetPointIndexList(int_max PointIndex0, int_max PointIndex1);

	inline SimpleObjectArray<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& DirectedEdgeList();
	inline const SimpleObjectArray<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& DirectedEdgeList() const;

    inline int_max GetIndex() const;

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

    inline MembraneMesh<MeshAttributeType>& GetParentMesh();
    inline const MembraneMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_Edge_Of_MembraneMesh GetHandle() const;

    inline void SetID(int_max EdgeID);
    inline void EraseID();
    inline int_max GetID() const;
   
    inline DenseVector<Handle_Of_Point_Of_MembraneMesh, 2> GetPointHandleList() const;
    inline void GetPointHandleList(Handle_Of_Point_Of_MembraneMesh& PointHandle0, Handle_Of_Point_Of_MembraneMesh& PointHandle1) const;

    inline DenseVector<int_max, 2> GetPointIDList() const;
    inline void GetPointIDList(int_max& PointID0, int_max& PointID1) const;

	inline int_max GetDirectedEdgeNumber() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetDirectedEdgeHandleList() const;
	inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

	inline int_max GetAdjacentEdgeNumber() const;

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	// Cell share this edge
	inline int_max GetAdjacentCellNumber() const;

    inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const;
	
	// Cell share any vertex point of this edge
	inline int_max GetNeighbourCellNumber() const;
    
    inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetNeighbourCellHandleList() const;
    inline void GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourCellIDList() const;
    inline void GetNeighbourCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline EdgeAttributeType& Attribute();
    inline const EdgeAttributeType& Attribute() const;
};

//====================================== DirectedEdge_Of_MembraneMesh (Cell Plasma Membrane) ===================================================//

template<typename MeshAttributeType>
struct Data_Of_DirectedEdge_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType                 ScalarType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType  DirectedEdgeAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    DirectedEdgeIndex_Of_MembraneMesh Index; // DirectedEdgeIndex of this DirectedEdge, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    int_max CellIndex;           // index in Mesh.m_MeshData->CellList,  It is -1 if this is a boundary Edge

    int_max PointIndex_start;   // index in Mesh.m_MeshData->PointList, the start point of the DirectedEdge 
    int_max PointIndex_end;     // index in Mesh.m_MeshData->PointList, the end point of the DirectedEdge

    DirectedEdgeIndex_Of_MembraneMesh NextDirectedEdgeIndex;      // index of the Next DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList
    DirectedEdgeIndex_Of_MembraneMesh PreviousDirectedEdgeIndex;  // index of the Previous DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList

    //--------------------------------------
    
    DirectedEdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_DirectedEdge_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY_MembraneMesh) {}
    ~Data_Of_DirectedEdge_Of_MembraneMesh() {};
};

template<typename MeshAttributeType>
class DirectedEdge_Of_MembraneMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType                 ScalarType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType  DirectedEdgeAttributeType;

    template<typename T>
    friend class MembraneMesh;

    template<typename T>
    friend class Edge_Of_MembraneMesh;

    template<typename T>
    friend class Cell_Of_MembraneMesh;

private:
    std::unique_ptr<Data_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>> m_Data;

public:
    DirectedEdge_Of_MembraneMesh();
    DirectedEdge_Of_MembraneMesh(const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& InputDirectedEdge);
    DirectedEdge_Of_MembraneMesh(DirectedEdge_Of_MembraneMesh<MeshAttributeType>&& InputDirectedEdge);
    ~DirectedEdge_Of_MembraneMesh();

    void operator=(const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& InputDirectedEdge);
    void operator=(DirectedEdge_Of_MembraneMesh<MeshAttributeType>&& InputDirectedEdge);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline void SetStartPointIndex(int_max PointIndex);
    inline void SetEndPointIndex(int_max PointIndex);
    inline void SetEdgeIndex(int_max EdgeIndex);
    inline void SetCellIndex(int_max CellIndex);

    inline void SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
    
    inline void SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
   
    //-----------------------------------------------------------------------------------//
    inline DirectedEdgeIndex_Of_MembraneMesh GetIndex() const;
    inline int_max GetStartPointIndex() const;
    inline int_max GetEndPointIndex() const;
    inline int_max GetEdgeIndex() const;
    inline int_max GetCellIndex() const;

	inline DirectedEdgeIndex_Of_MembraneMesh GetNextDirectedEdgeIndex() const;
	inline DirectedEdgeIndex_Of_MembraneMesh GetPreviousDirectedEdgeIndex() const;

    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh> GetFirendDirectedEdgeIndexList() const;
    
	inline DenseVector<int_max> GetFirendCellIndexList() const;

    inline DenseVector<int_max> GetNeighbourCellIndexList() const;
    inline void GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    //-----------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline bool IsBoundary() const;

    inline MembraneMesh<MeshAttributeType>& GetParentMesh();
    inline const MembraneMesh<MeshAttributeType>& GetParentMesh() const;
    inline Handle_Of_DirectedEdge_Of_MembraneMesh GetHandle() const;

    inline void SetID(int_max DirectedEdgeID);
    inline void EraseID();
    inline int_max GetID() const;

    inline Handle_Of_Cell_Of_MembraneMesh GetCellHandle() const;
    inline int_max GetCellID() const;

    inline Handle_Of_Point_Of_MembraneMesh GetStartPointHandle() const;
    inline int_max GetStartPointID() const;

    inline Handle_Of_Point_Of_MembraneMesh GetEndPointHandle() const;
    inline int_max GetEndPointID() const;

    inline Handle_Of_Edge_Of_MembraneMesh GetEdgeHandle() const;
    inline int_max GetEdgeID() const;

    inline Handle_Of_DirectedEdge_Of_MembraneMesh GetNextDirectedEdgeHandle() const;
    inline int_max GetNextDirectedEdgeID() const;

    inline Handle_Of_DirectedEdge_Of_MembraneMesh GetPreviousDirectedEdgeHandle() const;
    inline int_max GetPreviousDirectedEdgeID() const;

	inline int_max GetFirendDirectedEdgeNumber() const;
	inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetFirendDirectedEdgeHandleList() const;
	inline DenseVector<int_max> GetFirendDirectedEdgeIDList() const;

	inline int_max GetFirendCellNumber() const;
	inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetFirendCellHandleList() const;
	inline DenseVector<int_max> GetFirendCellIDList() const;

	inline int_max GetNeighbourCellNumber() const;

	inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetNeighbourCellHandleList() const;
    inline void GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourCellIDList() const;
    inline void GetNeighbourCellIDList(DenseVector<int_max>& OutputHandleList) const;

    inline DirectedEdgeAttributeType& Attribute();
    inline const DirectedEdgeAttributeType& Attribute() const;
};

//====================================== Cell_Of_MembraneMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Cell_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::CellAttributeType    CellAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    int_max Index; // CellIndex: index of the Cell in Mesh.m_MeshData->CellList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier, it will not change after Mesh.ClearDataStructure()

    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> DirectedEdgeIndexList;

    //--------------------------------------

    CellAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Cell_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY_MembraneMesh) {}
    ~Data_Of_Cell_Of_MembraneMesh() {};
};

template<typename MeshAttributeType>
class Cell_Of_MembraneMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::CellAttributeType    CellAttributeType;

private:
    std::unique_ptr<Data_Of_Cell_Of_MembraneMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class MembraneMesh;

public:
    Cell_Of_MembraneMesh();
    Cell_Of_MembraneMesh(const Cell_Of_MembraneMesh<MeshAttributeType>& InputCell);
    Cell_Of_MembraneMesh(Cell_Of_MembraneMesh<MeshAttributeType>&& InputCell);
    ~Cell_Of_MembraneMesh();

    void operator=(const Cell_Of_MembraneMesh<MeshAttributeType>& InputCell);
    void operator=(Cell_Of_MembraneMesh<MeshAttributeType>&& InputCell);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max CellIndex);   
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& DirectedEdgeIndexList();

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

    inline MembraneMesh<MeshAttributeType>& GetParentMesh();
    inline const MembraneMesh<MeshAttributeType>& GetParentMesh() const;

    inline Handle_Of_Cell_Of_MembraneMesh GetHandle() const;

    inline void SetID(int_max CellID);
    inline void EraseID();
    inline int_max GetID() const;

	inline int_max GetDirectedEdgeNumber() const; // the number of Directed Edge

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetDirectedEdgeIDList() const;
    inline void GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetPointNumber() const; // the number of vertex point

    inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetPointIDList() const;
    inline void GetPointIDList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputIDList) const;

    inline int_max GetRelativeIndexOfPoint(Handle_Of_Point_Of_MembraneMesh PointHandle) const;
    inline int_max GetRelativeIndexOfPoint(int_max PointID) const;

	inline int_max GetEdgeNumber() const;  // the number of Edge

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetEdgeIDList() const;
    inline void GetEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetEdgeRelativeIndexList() const;
    inline void GetEdgeRelativeIndexList(DenseVector<int_max>& RelativeIndexList) const;

	inline int_max GetAdjacentCellNumber() const;

    // Cell share any Edge of this cell, not include this cell
    inline DenseVector<Handle_Of_Cell_Of_MembraneMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentCellIDList() const;
    inline void GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const;

    inline CellAttributeType& Attribute();
    inline const CellAttributeType& Attribute() const;
};

}// namespace mdk

#include "mdkMembraneMeshItem.hpp"

#endif