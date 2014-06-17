#ifndef __mdkPolygonMeshItem_h
#define __mdkPolygonMeshItem_h

#include "mdkObject.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//----------------- forward declare ------------------//
template<typename ScalarType>
class PolygonMesh;
//---------------------------------------------------//

//======================================  Pure_Empty_PolygonMesh_Symbol =========================================================//

struct InputStruct_For_Pure_Empty_PolygonMesh_Symbol
{
    std::string Name = "InputStruct_For_Pure_Empty_PolygonMesh_Symbol";
};

struct Pure_Empty_PolygonMesh_Symbol
{
    Pure_Empty_PolygonMesh_Symbol(const InputStruct_For_Pure_Empty_PolygonMesh_Symbol& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_Pure_Empty_PolygonMesh_Symbol")
        {
            MDK_Error("Symbol error @ Pure_Empty_PolygonMesh_Symbol")
        }
    }

    ~Pure_Empty_PolygonMesh_Symbol() {}

    // deleted:
    Pure_Empty_PolygonMesh_Symbol() = delete;
    Pure_Empty_PolygonMesh_Symbol(const Pure_Empty_PolygonMesh_Symbol&) = delete;
    Pure_Empty_PolygonMesh_Symbol(Pure_Empty_PolygonMesh_Symbol&&) = delete;
    void operator=(const Pure_Empty_PolygonMesh_Symbol&) = delete;
    void operator=(Pure_Empty_PolygonMesh_Symbol&&) = delete;
};

static InputStruct_For_Pure_Empty_PolygonMesh_Symbol This_Is_InputStruct_For_Pure_Empty_PolygonMesh_Symbol;

static Pure_Empty_PolygonMesh_Symbol This_Pure_Empty_PolygonMesh_Symbol(This_Is_InputStruct_For_Pure_Empty_PolygonMesh_Symbol);

#define MDK_PURE_EMPTY_POLYGONMESH This_Pure_Empty_PolygonMesh_Symbol

//====================================== Vertex_Of_PolygonMesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Vertex_Of_PolygonMesh
{
    PolygonMesh<ScalarType> Mesh;

    int_max Index;      // VertexIndex : index of this vertex in Mesh.m_MeshData->VertexList

    DenseVector<int_max> OutgoingHalfEdgeIndexList; // index in Mesh.m_MeshData->HalfEdgeList, this vertex is the start point
    DenseVector<int_max> IncomingHalfEdgeIndexList; // index in Mesh.m_MeshData->HalfEdgeList, this vertex is the end point 
    DenseVector<int_max> AdjacentVertexIndexList;   // index in Mesh.m_MeshData->VertexList
    DenseVector<int_max> AdjacentEdgeIndexList;     // index in Mesh.m_MeshData->EdgeList
    DenseVector<int_max> AdjacentPolygonIndexList;  // index in Mesh.m_MeshData->PolygonList

//------------------------------------------------------------------------
    Data_Of_Vertex_Of_PolygonMesh() : Mesh(MDK_PURE_EMPTY_POLYGONMESH) {}
    ~Data_Of_Vertex_Of_PolygonMesh() {};
};

template<typename ScalarType>
class Vertex_Of_PolygonMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Vertex_Of_PolygonMesh<ScalarType>> m_Data;

public:
    inline Vertex_Of_PolygonMesh();
    inline Vertex_Of_PolygonMesh(const Vertex_Of_PolygonMesh<ScalarType>& InputVertex);
    inline Vertex_Of_PolygonMesh(Vertex_Of_PolygonMesh<ScalarType>&& InputVertex);
    inline ~Vertex_Of_PolygonMesh();

    inline void operator=(const Vertex_Of_PolygonMesh<ScalarType>& InputVertex);
    inline void operator=(Vertex_Of_PolygonMesh<ScalarType>&& InputVertex);

    inline void Create();
    inline void Delete();

    inline bool IsValid() const;
    inline bool IsDeleted() const;

    //--------------------------------------------------------------//
public:
    inline void SetParentMesh(PolygonMesh<ScalarType>& ParentMesh);
    inline PolygonMesh<ScalarType>& GetParentMesh();
    inline const PolygonMesh<ScalarType>& GetParentMesh() const;

    inline void SetIndex(int_max VertexIndex);
    inline int_max GetIndex() const;

    inline void GetPosition(ScalarType* Pos);
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z);

    inline void SetOutgoingHalfEdgeIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetOutgoingHalfEdgeIndexList() const;
    inline void GetOutgoingHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline void SetIncomingHalfEdgeIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetIncomingHalfEdgeIndexList() const;
    inline void GetIncomingHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline void SetAdjacentVertexIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetAdjacentVertexIndexList() const;
    inline void GetAdjacentVertexIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline void SetAdjacentEdgeIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetAdjacentEdgeIndexList() const;
    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline void SetAdjacentPolygonIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetAdjacentPolygonIndexList() const;
    inline void GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const;

    //-------------------------------------------------------------------------------------//
    bool IsBoundary() const;
};

//====================================== Edge_Of_PolygonMesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Edge_Of_PolygonMesh
{
    PolygonMesh<ScalarType> Mesh;
    
    int_max Index; // EdgeIndex: index of this Edge  in Mesh.MeshData->EdgeList

    //int_max VertexIndex0; // starting vertex of the halfedge: HalfEdgeIndex0
    //int_max VertexIndex1; // starting vertex of the halfedge: HalfEdgeIndex1

    int_max HalfEdgeIndex0; // index in Mesh.m_MeshData->HalfEdgeList
    int_max HalfEdgeIndex1; // index in Mesh.m_MeshData->HalfEdgeList
    //If this is a boundary edge, HalfEdgeIndex0 or HalfEdgeIndex1 is -1

//------------------------------------------------------------------------
    Data_Of_Edge_Of_PolygonMesh() : Mesh(MDK_PURE_EMPTY_POLYGONMESH) {}
    ~Data_Of_Edge_Of_PolygonMesh() {};
};

template<typename ScalarType>
class Edge_Of_PolygonMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Edge_Of_PolygonMesh<ScalarType>> m_Data;

public:
    inline Edge_Of_PolygonMesh();
    inline Edge_Of_PolygonMesh(const Edge_Of_PolygonMesh<ScalarType>& InputEdge);
    inline Edge_Of_PolygonMesh(Edge_Of_PolygonMesh<ScalarType>&& InputEdge);
    inline ~Edge_Of_PolygonMesh();

    inline void operator=(const Edge_Of_PolygonMesh<ScalarType>& InputEdge);
    inline void operator=(Edge_Of_PolygonMesh<ScalarType>&& InputEdge);
    
    inline void Create();
    inline void Delete();

    inline bool IsValid() const;
    inline bool IsDeleted() const;

    //--------------------------------------------------------------//
    inline void SetParentMesh(PolygonMesh<ScalarType>& ParentMesh);
    inline PolygonMesh<ScalarType>& GetParentMesh();
    inline const PolygonMesh<ScalarType>& GetParentMesh() const;

    inline void SetIndex(int_max EdgeIndex);
    inline int_max GetIndex() const;

    inline void SetHalfEdgeIndexList(int_max HalfEdgeIndex0, int_max HalfEdgeIndex1);
    inline DenseVector<int_max> GetHalfEdgeIndexList() const;
    inline void GetHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;
    inline void GetHalfEdgeIndexList(int_max OutputIndexList[2]) const;
    inline void GetHalfEdgeIndexList(int_max& HalfEdgeIndex0, int_max& HalfEdgeIndex1) const;

    inline DenseVector<int_max> GetVertexIndexList() const;
    inline void GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const;
    inline void GetVertexIndexList(int_max OutputIndexList[2]) const;
    inline void GetVertexIndexList(int_max& VertexIndex0, int_max& VertexIndex1) const;

    inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;
    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline DenseVector<int_max> GetAdjacentPolygonIndexList() const;
    inline void GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const;

    //--------------------------------------------------------------------------//
    bool IsBoundary() const;
};

//====================================== HalfEdge_Of_PolygonMesh ==============================================================//

template<typename ScalarType>
struct Data_Of_HalfEdge_Of_PolygonMesh
{
    PolygonMesh<ScalarType> Mesh;

    int_max Index;                  // HalfEdgeIndex: index of this HalfEdge in Mesh.m_MeshData->HalfEdgeList

    int_max VertexIndex_start;      // index in Mesh.m_MeshData->VertexList, the start vertex of the HalfEdge 
    int_max VertexIndex_end;        // in Mesh.m_MeshData->VertexList, the end vertex of the HalfEdge
    int_max EdgeIndex;              // index in Mesh.m_MeshData->EdgeList
    int_max PolygonIndex;           // index in Mesh.m_MeshData->PolygonList
    int_max OppositeHalfEdgeIndex;  // index of the Opposite HalfEdge in Mesh.m_MeshData->HalfEdgeList. It is -1 if this is a boundary edge
    int_max NextHalfEdgeIndex;      // index of the Next HalfEdge in Mesh.m_MeshData->HalfEdgeList
    int_max PreviousHalfEdgeIndex;  // index of the Previous HalfEdge in Mesh.m_MeshData->HalfEdgeList

//------------------------------------------------------------------------
    Data_Of_HalfEdge_Of_PolygonMesh() : Mesh(MDK_PURE_EMPTY_POLYGONMESH) {}
    ~Data_Of_HalfEdge_Of_PolygonMesh() {};
};

template<typename ScalarType>
class HalfEdge_Of_PolygonMesh : public Object
{
private:
    std::unique_ptr<Data_Of_HalfEdge_Of_PolygonMesh<ScalarType>> m_Data;

public:
    HalfEdge_Of_PolygonMesh();
    HalfEdge_Of_PolygonMesh(const HalfEdge_Of_PolygonMesh<ScalarType>& InputHalfEdge);
    HalfEdge_Of_PolygonMesh(HalfEdge_Of_PolygonMesh<ScalarType>&& InputHalfEdge);
    ~HalfEdge_Of_PolygonMesh();

    void operator=(const HalfEdge_Of_PolygonMesh<ScalarType>& InputHalfEdge);
    void operator=(HalfEdge_Of_PolygonMesh<ScalarType>&& InputHalfEdge);

    inline void Create();
    inline void Delete();

    inline bool IsValid() const;
    inline bool IsDeleted() const;

    //--------------------------------------------------------------------------------//
    inline void SetParentMesh(PolygonMesh<ScalarType>& ParentMesh);
    inline PolygonMesh<ScalarType>& GetParentMesh();
    inline const PolygonMesh<ScalarType>& GetParentMesh() const;

    inline void SetIndex(int_max HalfEdgeIndex);
    inline int_max GetIndex() const;

    inline void SetStartVertexIndex(int_max VertexIndex);
    inline int_max GetStartVertexIndex() const;

    inline void SetEndVertexIndex(int_max VertexIndex);
    inline int_max GetEndVertexIndex() const;

    inline void SetEdgeIndex(int_max EdgeIndex);
    inline int_max GetEdgeIndex() const;

    inline void SetPolygonIndex(int_max PolygonIndex);
    inline int_max GetPolygonIndex() const;

    inline void SetOppositeHalfEdgeIndex(int_max HalfEdgeIndex);
    inline int_max GetOppositeHalfEdgeIndex() const;

    inline void SetNextHalfEdgeIndex(int_max HalfEdgeIndex);
    inline int_max GetNextHalfEdgeIndex() const;

    inline void SetPreviousHalfEdgeIndex(int_max HalfEdgeIndex);
    inline int_max GetPreviousHalfEdgeIndex() const;

    //-----------------------------------------------------------------------------------//
    inline DenseVector<int_max> GetAdjacentPolygonIndexList() const;
    inline void GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const;
    inline void GetAdjacentPolygonIndexList(int_max OutputIndexList[2]) const;
    inline void GetAdjacentPolygonIndexList(int_max& PolygonIndex0, int_max& PolygonIndex1) const;

    //-------------------------------------------------------------------------------------//
    bool IsBoundary() const;
};

//====================================== Polygon_Of_PolygonMesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Polygon_Of_PolygonMesh
{
    PolygonMesh<ScalarType> Mesh;

    int_max Index; // PolygonIndex: index of the Polygon in Mesh.m_MeshData->PolygonList

    DenseVector<int_max> HalfEdgeIndexList; // index in Mesh.m_MeshData->HalfEdgeList

//------------------------------------------------------------------------
    Data_Of_Polygon_Of_PolygonMesh() : Mesh(MDK_PURE_EMPTY_POLYGONMESH) {}
    ~Data_Of_Polygon_Of_PolygonMesh() {};
};

template<typename ScalarType>
class Polygon_Of_PolygonMesh : public Object
{
private:
    std::unique_ptr<Data_Of_Polygon_Of_PolygonMesh<ScalarType>> m_Data;
 
public:
    Polygon_Of_PolygonMesh();
    Polygon_Of_PolygonMesh(const Polygon_Of_PolygonMesh<ScalarType>& InputPolygon);
    Polygon_Of_PolygonMesh(Polygon_Of_PolygonMesh<ScalarType>&& InputPolygon);
    ~Polygon_Of_PolygonMesh();

    void operator=(const Polygon_Of_PolygonMesh<ScalarType>& InputPolygon);
    void operator=(Polygon_Of_PolygonMesh<ScalarType>&& InputPolygon);

    inline void Create();
    inline void Delete();

    inline bool IsValid() const;
    inline bool IsDeleted() const;

    inline bool IsTriangle() const;
    //--------------------------------------------------------------------------------//

    inline void SetParentMesh(PolygonMesh<ScalarType>& ParentMesh);
    inline PolygonMesh<ScalarType>& GetParentMesh();
    inline const PolygonMesh<ScalarType>& GetParentMesh() const;

    inline void SetIndex(int_max PolygonIndex);
    inline int_max GetIndex() const;

    inline void SetHalfEdgeIndexList(DenseVector<int_max> IndexList);
    inline const DenseVector<int_max>& GetHalfEdgeIndexList() const;
    inline void GetHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline int_max GetHalfEdgeNumber() const; // the number of half edges

    inline const DenseVector<int_max>& GetVertexIndexList() const;
    inline void GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline int_max GetVertexNumber() const; // the number of vertex

    inline DenseVector<int_max> GetEdgeIndexList() const;
    inline void GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline int_max GetEdgeNumber() const;  // the number of edges

    inline DenseVector<int_max> GetAdjacentPolygonIndexList() const;
    inline void GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const;
};

}// namespace mdk

#include "mdkPolygonMeshItem.hpp"

#endif