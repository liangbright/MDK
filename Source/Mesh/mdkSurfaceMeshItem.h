#ifndef __mdkSurfaceMeshItem_h
#define __mdkSurfaceMeshItem_h

#include "mdkObject.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//----------------- forward declare ------------------//
template<typename ScalarType>
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

//====================================== Point_Of_SurfaceMesh ==============================================================//

struct Handle_Of_Point_Of_SurfaceMesh
{
private:
    int_max Index; // PointIndex in Mesh.m_MeshData->PointList

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

public:
    Handle_Of_Point_Of_SurfaceMesh() { Index = -1; }
    Handle_Of_Point_Of_SurfaceMesh(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }
    ~Handle_Of_Point_Of_SurfaceMesh() {}

    void operator=(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }

    int_max GetIndex() const
    {
        return Index;
    }

    bool IsValid() const
    {
        return (Index >= 0);
    }
}

template<typename ScalarType>
struct Data_Of_Point_Of_SurfaceMesh
{
    SurfaceMesh<ScalarType> Mesh;

    int_max Index;       // PointIndex : index of this point in Mesh.m_MeshData->PointList

    int_max VertexIndex; 
    // -1: if this is not a vertex
    // >=0; if this is a vertex

    int_max EdgeIndex;  
    // if this point is not a vertex, point is only on the Edge (EdgeIndex)
    // if this point is a vertex, then EdgeIndex is set to -1

    //------------------------------------------------------------------------
    Data_Of_Point_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Point_Of_SurfaceMesh() {};
};

template<typename ScalarType>
class Point_Of_SurfaceMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Point_Of_SurfaceMesh<ScalarType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

public:
    inline Point_Of_SurfaceMesh();
    inline Point_Of_SurfaceMesh(const Point_Of_SurfaceMesh<ScalarType>& InputVertex);
    inline Point_Of_SurfaceMesh(Point_Of_SurfaceMesh<ScalarType>&& InputVertex);
    inline ~Point_Of_SurfaceMesh();

    inline void operator=(const Point_Of_SurfaceMesh<ScalarType>& InputVertex);
    inline void operator=(Point_Of_SurfaceMesh<ScalarType>&& InputVertex);

private:
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<ScalarType>& ParentMesh); 
    inline void SetIndex(int_max PointIndex);   
    inline void SetVertexIndex(int_max VertexIndex);
    inline void SetEdgeIndex(int_max EdgeIndex);    
    inline void MarkAsNonVertex();

    //--------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    inline bool IsVertex() const;
    inline bool IsOnEdge() const;
    inline bool IsOnBoundaryEdge() const;

    inline SurfaceMesh<ScalarType>& GetParentMesh();
    inline const SurfaceMesh<ScalarType>& GetParentMesh() const;
    inline Handle_Of_Point_Of_SurfaceMesh GetHandle() const;
    inline Handle_Of_Vertex_Of_SurfaceMesh GetVertexHandle() const;
    inline Handle_Of_Edge_Of_SurfaceMesh GetEdgeHandle() const;

    inline void SetID(int_max PointID) const;
    inline int_max GetID();

    inline void SetPosition(const ScalarType* Pos);
    inline void SetPosition(ScalarType x, ScalarType y, ScalarType z);

    inline void GetPosition(ScalarType* Pos) const;
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const;
};

//====================================== Vertex_Of_SurfaceMesh ==============================================================//

// forward declare-------------------//
struct DirectedEdgeIndex_Of_SurfaceMesh;
//-----------------------------------

struct Handle_Of_Vertex_Of_SurfaceMesh
{
private:
    int_max Index; // VertexIndex in Mesh.m_MeshData->VertexList

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

public:
    Handle_Of_Vertex_Of_SurfaceMesh() { Index = -1; }
    Handle_Of_Vertex_Of_SurfaceMesh(const Handle_Of_Vertex_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }
    ~Handle_Of_Vertex_Of_SurfaceMesh() {}

    void operator=(const Handle_Of_Vertex_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }

    int_max GetIndex() const
    {
        return Index;
    }

    bool IsValid() const
    {
        return (Index >= 0);
    }
}

template<typename ScalarType>
struct Data_Of_Vertex_Of_SurfaceMesh
{
    SurfaceMesh<ScalarType> Mesh;

    int_max Index;       // VertexIndex : index of this vertex in Mesh.m_MeshData->VertexList

    int_max PointIndex;  // PointIndex : index of this vertex in Mesh.m_MeshData->PointPositionTable

    DenseVector<int_max> AdjacentVertexIndexList;   // index in Mesh.m_MeshData->VertexList

    DenseVector<int_max> AdjacentEdgeIndexList;     // index in Mesh.m_MeshData->EdgeList
    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> OutgoingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this vertex is the start point
    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> IncomingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this vertex is the end point 
    
    DenseVector<int_max> AdjacentCellIndexList;     // index in Mesh.m_MeshData->CellList

//------------------------------------------------------------------------
    Data_Of_Vertex_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Vertex_Of_SurfaceMesh() {};
};

template<typename ScalarType>
class Vertex_Of_SurfaceMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Vertex_Of_SurfaceMesh<ScalarType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

private:
    inline Vertex_Of_SurfaceMesh();
    inline Vertex_Of_SurfaceMesh(const Vertex_Of_SurfaceMesh<ScalarType>& InputVertex);
    inline Vertex_Of_SurfaceMesh(Vertex_Of_SurfaceMesh<ScalarType>&& InputVertex);
    inline ~Vertex_Of_SurfaceMesh();

    inline void operator=(const Vertex_Of_SurfaceMesh<ScalarType>& InputVertex);
    inline void operator=(Vertex_Of_SurfaceMesh<ScalarType>&& InputVertex);

    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<ScalarType>& ParentMesh);
    inline void SetIndex(int_max VertexIndex);
    inline void SetPointIndex(int_max PointIndex);
    
    inline void SetGlobalID(int_max VertexGlobalID);

    inline DenseVector<int_max>& AdjacentVertexIndexList();
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& OutgoingDirectedEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& IncomingDirectedEdgeIndexList();
    inline DenseVector<int_max>& AdjacentCellIndexList();

    //-------------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    bool IsBoundary() const;

    inline SurfaceMesh<ScalarType>& GetParentMesh();
    inline const SurfaceMesh<ScalarType>& GetParentMesh() const;
    inline int_max GetIndex() const;
    inline int_max GetPointIndex() const;

    inline void SetPosition(const ScalarType* Pos);
    inline void SetPosition(ScalarType x, ScalarType y, ScalarType z);

    inline void GetPosition(ScalarType* Pos) const;
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const;

    inline const DenseVector<int_max>& GetAdjacentVertexIndexList() const;
    inline void GetAdjacentVertexIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline const DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& GetOutgoingDirectedEdgeIndexList() const;
    inline void GetOutgoingDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline const DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& GetIncomingDirectedEdgeIndexList() const;
    inline void GetIncomingDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline const DenseVector<int_max>& GetAdjacentCellIndexList() const;
    inline void GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const;
};

//====================================== Edge_Of_SurfaceMesh (Cell Wall) ==============================================================//

struct Handle_Of_Edge_Of_SurfaceMesh
{
private:
    int_max Index; // EdgeIndex in Mesh.m_MeshData->EdgeList

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

public:
    Handle_Of_Edge_Of_SurfaceMesh() { Index = -1; }
    Handle_Of_Edge_Of_SurfaceMesh(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }
    ~Handle_Of_Edge_Of_SurfaceMesh() {}

    void operator=(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }

    int_max GetIndex() const
    {
        return Index;
    }

    bool IsValid() const
    {
        return (Index >= 0);
    }
}

template<typename ScalarType>
struct Data_Of_Edge_Of_SurfaceMesh
{
    SurfaceMesh<ScalarType> Mesh;
    
    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList

    int_max VertexIndex0;
    int_max VertexIndex1;

    // index of point on the Edge
    // for PointIndex0 to PointIndex1
    DenseVector<int_max> PointIndexList;

//------------------------------------------------------------------------
    Data_Of_Edge_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Edge_Of_SurfaceMesh() {};
};

template<typename ScalarType>
class Edge_Of_SurfaceMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Edge_Of_SurfaceMesh<ScalarType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

private:
    inline Edge_Of_SurfaceMesh();
    inline Edge_Of_SurfaceMesh(const Edge_Of_SurfaceMesh<ScalarType>& InputEdge);
    inline Edge_Of_SurfaceMesh(Edge_Of_SurfaceMesh<ScalarType>&& InputEdge);
    inline ~Edge_Of_SurfaceMesh();

    inline void operator=(const Edge_Of_SurfaceMesh<ScalarType>& InputEdge);
    inline void operator=(Edge_Of_SurfaceMesh<ScalarType>&& InputEdge);
    
    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<ScalarType>& ParentMesh);
    inline void SetIndex(int_max EdgeIndex);

    inline void SetVertexIndexList(const int_max VertexIndexList[2]);
    inline void SetVertexIndexList(int_max VertexIndex0, int_max VertexIndex1);

    inline DenseVector<int_max>& PointIndexList();

    //--------------------------------------------------------------------------//
public:

    inline bool IsValid() const;

    bool IsBoundary() const;

    inline SurfaceMesh<ScalarType>& GetParentMesh();
    inline const SurfaceMesh<ScalarType>& GetParentMesh() const;
    inline int_max GetIndex() const;

    inline void GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const;
    inline DenseVector<int_max> GetVertexIndexList() const;
    inline void GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const;
    inline void GetVertexIndexList(int_max OutputIndexList[2]) const;
    inline void GetVertexIndexList(int_max& VertexIndex0, int_max& VertexIndex1) const;

    inline const DenseVector<int_max>& GetPointIndexList() const;

    inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;
    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share this edge
    inline DenseVector<int_max> GetAdjacentCellIndexList() const;
    inline void GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share any vertex of this edge
    inline DenseVector<int_max> GetNeighbourCellIndexList() const;
    inline void GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const;
};

//====================================== DirectedEdge_Of_SurfaceMesh (Cell Plasma Membrane) ===================================================//

struct DirectedEdgeIndex_Of_SurfaceMesh
{
    int_max EdgeIndex;
    int_max RelativeIndex; // 0 or 1

    // Get a DirectedEdge : Mesh.m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex]
};

struct Handle_Of_DirectedEdge_Of_SurfaceMesh
{
private:
    DirectedEdgeIndex_Of_SurfaceMesh Index; // DirectedEdgeIndex

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

public:
    Handle_Of_DirectedEdge_Of_SurfaceMesh() { Index = -1; }
    void Handle_Of_DirectedEdge_Of_SurfaceMesh(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }
    ~Handle_Of_DirectedEdge_Of_SurfaceMesh() {}

    void operator=(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }

    DirectedEdgeIndex_Of_SurfaceMesh GetIndex() const
    {
        return Index;
    }

    bool IsValid() const
    {
        return (Index.EdgeIndex >= 0 && Index.RelativeIndex >= 0);
    }
}

template<typename ScalarType>
struct Data_Of_DirectedEdgeIndex_Of_SurfaceMesh
{
    SurfaceMesh<ScalarType> Mesh;

    DirectedEdgeIndex_Of_SurfaceMesh Index; // DirectedEdgeIndex of this DirectedEdge

    int_max CellIndex;           // index in Mesh.m_MeshData->CellList,  It is -1 if this is a boundary Edge

    int_max VertexIndex_start;   // index in Mesh.m_MeshData->VertexList, the start vertex of the DirectedEdge 
    int_max VertexIndex_end;     // index in Mesh.m_MeshData->VertexList, the end vertex of the DirectedEdge

    // FirendDirectedEdgeIndex.RelativeIndex=0 on the same edge
    // DirectedEdgeIndex_Of_SurfaceMesh FirendDirectedEdgeIndex;  // index of the Friend DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList

    DirectedEdgeIndex_Of_SurfaceMesh NextDirectedEdgeIndex;      // index of the Next DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList
    DirectedEdgeIndex_Of_SurfaceMesh PreviousDirectedEdgeIndex;  // index of the Previous DirectedEdge in Mesh.m_MeshData->DirectedEdgePairList

//------------------------------------------------------------------------
    Data_Of_DirectedEdge_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_DirectedEdge_Of_SurfaceMesh() {};
};

template<typename ScalarType>
class DirectedEdge_Of_SurfaceMesh : public Object
{
private:
    std::unique_ptr<Data_Of_DirectedEdge_Of_SurfaceMesh<ScalarType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

private:
    DirectedEdge_Of_SurfaceMesh();
    DirectedEdge_Of_SurfaceMesh(const DirectedEdge_Of_SurfaceMesh<ScalarType>& InputDirectedEdge);
    DirectedEdge_Of_SurfaceMesh(DirectedEdge_Of_SurfaceMesh<ScalarType>&& InputDirectedEdge);
    ~DirectedEdge_Of_SurfaceMesh();

    void operator=(const DirectedEdge_Of_SurfaceMesh<ScalarType>& InputDirectedEdge);
    void operator=(DirectedEdge_Of_SurfaceMesh<ScalarType>&& InputDirectedEdge);

    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<ScalarType>& ParentMesh);
    inline void SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline void SetStartVertexIndex(int_max VertexIndex);
    inline void SetEndVertexIndex(int_max VertexIndex);
    inline void SetEdgeIndex(int_max EdgeIndex);
    inline void SetCellIndex(int_max CellIndex);
    
    inline void SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
    
    inline void SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
   
private:
    inline DirectedEdgeIndex_Of_SurfaceMesh GetIndex() const;
    inline int_max GetStartVertexIndex() const;
    inline int_max GetEndVertexIndex() const;
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

    inline SurfaceMesh<ScalarType>& GetParentMesh();
    inline const SurfaceMesh<ScalarType>& GetParentMesh() const;
    inline DirectedEdgeIndex_Of_SurfaceMesh GetHandle() const;
    inline int_max GetStartVertexHandle() const;
    inline int_max GetEndVertexHandle() const;
    inline int_max GetEdgeHandle() const;
    inline int_max GetCellHandle() const;

    inline DirectedEdgeIndex_Of_SurfaceMesh GetFirendDirectedEdgeHandle() const;
    inline DirectedEdgeIndex_Of_SurfaceMesh GetNextDirectedEdgeHandle() const;
    inline DirectedEdgeIndex_Of_SurfaceMesh GetPreviousDirectedEdgeHandle() const;

    inline int_max GetFirendCellHandle() const;

    inline DenseVector<int_max> GetNeighbourCellHandleList() const;
    inline void GetNeighbourCellHandleList(DenseVector<int_max>& OutputHandleList) const;
};

//====================================== Cell_Of_SurfaceMesh ==============================================================//

struct Handle_Of_Cell_Of_SurfaceMesh
{
private:
    int_max Index; // CellIndex:  index of the Cell in Mesh.m_MeshData->CellList

    template<typename T>
    friend class SurfaceMesh;

    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;

public:
    Handle_Of_Cell_Of_SurfaceMesh() { Index = -1; }
    Handle_Of_Cell_Of_SurfaceMesh(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }
    ~Handle_Of_Cell_Of_SurfaceMesh() {}

    void operator=(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) { Index = InputHandle.Index; }

    int_max GetIndex() const
    {
        return Index;
    }

    bool IsValid() const
    {
        return (Index >= 0);
    }
}

template<typename ScalarType>
struct Data_Of_Cell_Of_SurfaceMesh
{
    SurfaceMesh<ScalarType> Mesh;

    int_max Index; // CellIndex: index of the Cell in Mesh.m_MeshData->CellList

    DenseVector<int_max> DirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList

//------------------------------------------------------------------------
    Data_Of_Cell_Of_SurfaceMesh() : Mesh(MDK_PURE_EMPTY_SURFACEMESH) {}
    ~Data_Of_Cell_Of_SurfaceMesh() {};
};

template<typename ScalarType>
class Cell_Of_SurfaceMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Cell_Of_SurfaceMesh<ScalarType>> m_Data;

    template<typename T>
    friend class SurfaceMesh;

private:
    Cell_Of_SurfaceMesh();
    Cell_Of_SurfaceMesh(const Cell_Of_SurfaceMesh<ScalarType>& InputCell);
    Cell_Of_SurfaceMesh(Cell_Of_SurfaceMesh<ScalarType>&& InputCell);
    ~Cell_Of_SurfaceMesh();

    void operator=(const Cell_Of_SurfaceMesh<ScalarType>& InputCell);
    void operator=(Cell_Of_SurfaceMesh<ScalarType>&& InputCell);

    inline void Create();
    inline void Clear();

    inline void SetParentMesh(SurfaceMesh<ScalarType>& ParentMesh);
    inline void SetIndex(int_max CellIndex);

    inline DenseVector<int_max>& DirectedEdgeIndexList();

private:

    inline const DenseVector<int_max>& GetDirectedEdgeIndexList() const;
    inline void GetDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline DenseVector<int_max> GetVertexIndexList() const;
    inline void GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline DenseVector<int_max> GetEdgeIndexList() const;
    inline void GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Cell share any Edge of this cell, not include this cell
    inline DenseVector<int_max> GetAdjacentCellIndexList() const;
    inline void GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const;

    //--------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline SurfaceMesh<ScalarType>& GetParentMesh();
    inline const SurfaceMesh<ScalarType>& GetParentMesh() const;
    inline int_max GetIndex() const;

    inline void SetGlobalID(int_max CellGlobalID);

    inline DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const;

    inline int_max GetDirectedEdgeNumber() const; // the number of Directed Edges

    inline DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> GetVertexHandleList() const;
    inline void GetVertexHandleList(DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& OutputHandleList) const;

    inline int_max GetVertexNumber() const; // the number of vertex

    inline DenseVector<Handle_Of_Edge_Of_SurfaceMesh> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const;

    inline int_max GetEdgeNumber() const;  // the number of Edges

    // Cell share any Edge of this cell, not include this cell
    inline DenseVector<Handle_Of_Cell_Of_SurfaceMesh> GetAdjacentCellHandleList() const;
    inline void GetAdjacentCellIndexList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const;
};

}// namespace mdk

#include "mdkSurfaceMeshItem.hpp"

#endif