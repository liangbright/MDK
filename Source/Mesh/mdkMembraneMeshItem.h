#ifndef mdk_MembraneMeshItem_h
#define mdk_MembraneMeshItem_h

#include "mdkObject.h"
#include "mdkObjectArray.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseMatrix.h"
#include "mdkMembraneMeshItemHandle.h"

namespace mdk
{
//----------------- forward declare ------------------//
template<typename MeshAttributeType>
class MembraneMesh;
//---------------------------------------------------//

//------------------------------------------------- forward declare --------------------------------------------------------//
template<typename T>
class Point_Of_MembraneMesh;

template<typename T>
class Edge_Of_MembraneMesh;

template<typename T>
class DirectedEdge_Of_MembraneMesh;

struct DirectedEdgeIndex_Of_MembraneMesh;

template<typename T>
class Face_Of_MembraneMesh;
//----------------------------------------------------------------------------------------------------------------------------//

//====================================== Point_Of_MembraneMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Point_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    int_max Index;  // PointIndex : index in Mesh.m_MeshData->PointList; it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier (valid if >=0, invalid if < 0), it will not change after Mesh.ClearDataStructure()

    DenseVector<int_max> AdjacentPointIndexList;   // index in Mesh.m_MeshData->PointList

    DenseVector<int_max> AdjacentEdgeIndexList;     // index in Mesh.m_MeshData->EdgeList
    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutgoingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the start point
    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> IncomingDirectedEdgeIndexList; // index in Mesh.m_MeshData->DirectedEdgeList, this is the end point 
    
    DenseVector<int_max> AdjacentFaceIndexList;     // index in Mesh.m_MeshData->FaceList

    //------------------------------------------------

    PointAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Point_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY) {}
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

	template<typename T>
	friend class Face_Of_MembraneMesh;

	template<typename T>
	friend class Edge_Of_MembraneMesh;

public:
	inline Point_Of_MembraneMesh();
    inline Point_Of_MembraneMesh(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint);
    inline Point_Of_MembraneMesh(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint);
    inline ~Point_Of_MembraneMesh();

    inline void operator=(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint);
    inline void operator=(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    
    inline void SetIndex(int_max PointIndex);
    inline int_max GetIndex() const;

    inline DenseVector<int_max>& AdjacentPointIndexList();
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& OutgoingDirectedEdgeIndexList();
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& IncomingDirectedEdgeIndexList();
    inline DenseVector<int_max>& AdjacentFaceIndexList();

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

	inline int_max GetAdjacentPointCount() const;

    inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetAdjacentPointHandleList() const;
    inline void GetAdjacentPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentPointIDList() const;
    inline void GetAdjacentPointIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetAdjacentEdgeCount() const;

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetOutgoingDirectedEdgeCount() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetOutgoingDirectedEdgeHandleList() const;
    inline void GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetOutgoingDirectedEdgeIDList() const;
    inline void GetOutgoingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetIncomingDirectedEdgeCount() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetIncomingDirectedEdgeHandleList() const;
    inline void GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetIncomingDirectedEdgeIDList() const;
    inline void GetIncomingDirectedEdgeIDList(DenseVector<int_max>& OutputHandleList) const;

	inline int_max GetAdjacentFaceCount() const;

    inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetAdjacentFaceHandleList() const;
    inline void GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentFaceIDList() const;
    inline void GetAdjacentFaceIDList(DenseVector<int_max>& OutputHandleList) const;

    inline PointAttributeType& Attribute();
    inline const PointAttributeType& Attribute() const;
};

//====================================== Edge_Of_MembraneMesh (Face Wall) ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Edge_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;
    
    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier(valid if >=0, invalid if < 0), it will not change after Mesh.ClearDataStructure()

    int_max PointIndex0;
    int_max PointIndex1;

	StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>> DirectedEdgeList;

    //--------------------------------

    EdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Edge_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY) {}
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
    friend class Face_Of_MembraneMesh;

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
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max EdgeIndex);

    inline void SetPointIndexList(const int_max PointIndexList[2]);
    inline void SetPointIndexList(int_max PointIndex0, int_max PointIndex1);
	inline void SetPointIndexList(const DenseVector<int_max, 2>& PointIndexList);

	inline StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& DirectedEdgeList();
	inline const StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& DirectedEdgeList() const;

    inline int_max GetIndex() const;

    inline void GetPointIndexList(int_max PointIndexList[2]) const;
    inline void GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const;
	inline DenseVector<int_max, 2> GetPointIndexList() const;

    inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;
    inline void GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Face share this edge
    inline DenseVector<int_max> GetAdjacentFaceIndexList() const;
    inline void GetAdjacentFaceIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Face share any vertex point of this edge
    inline DenseVector<int_max> GetNeighbourFaceIndexList() const;
    inline void GetNeighbourFaceIndexList(DenseVector<int_max>& OutputIndexList) const;

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

	inline int_max GetDirectedEdgeCount() const;

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetDirectedEdgeHandleList() const;
	inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

	inline int_max GetAdjacentEdgeCount() const;

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetAdjacentEdgeHandleList() const;
    inline void GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentEdgeIDList() const;
    inline void GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	// Face share this edge
	inline int_max GetAdjacentFaceCount() const;

    inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetAdjacentFaceHandleList() const;
    inline void GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentFaceIDList() const;
    inline void GetAdjacentFaceIDList(DenseVector<int_max>& OutputIDList) const;
	
	// Face share any vertex point of this edge
	inline int_max GetNeighbourFaceCount() const;
    
    inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetNeighbourFaceHandleList() const;
    inline void GetNeighbourFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourFaceIDList() const;
    inline void GetNeighbourFaceIDList(DenseVector<int_max>& OutputHandleList) const;

    inline EdgeAttributeType& Attribute();
    inline const EdgeAttributeType& Attribute() const;
};

//====================================== DirectedEdge_Of_MembraneMesh (Face Boundary) ===================================================//
// DirectedEdge is owned by Face: one DirectedEdge only belong to one face
template<typename MeshAttributeType>
struct Data_Of_DirectedEdge_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType                 ScalarType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType  DirectedEdgeAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    DirectedEdgeIndex_Of_MembraneMesh Index; // DirectedEdgeIndex of this DirectedEdge, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier (valid if >=0, invalid if < 0), it will not change after Mesh.ClearDataStructure()

    int_max FaceIndex;           // index in Mesh.m_MeshData->FaceList

    int_max PointIndex_start;   // index in Mesh.m_MeshData->PointList, the start point of the DirectedEdge 
    int_max PointIndex_end;     // index in Mesh.m_MeshData->PointList, the end point of the DirectedEdge

    DirectedEdgeIndex_Of_MembraneMesh NextDirectedEdgeIndex;      // index of the Next DirectedEdge in Mesh.m_MeshData->DirectedEdgeList
    DirectedEdgeIndex_Of_MembraneMesh PreviousDirectedEdgeIndex;  // index of the Previous DirectedEdge in Mesh.m_MeshData->DirectedEdgeList

    //--------------------------------------
    
    DirectedEdgeAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_DirectedEdge_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY) {}
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
    friend class Face_Of_MembraneMesh;

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
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline void SetStartPointIndex(int_max PointIndex);
    inline void SetEndPointIndex(int_max PointIndex);
    inline void SetEdgeIndex(int_max EdgeIndex);
    inline void SetFaceIndex(int_max FaceIndex);

    inline void SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
    
    inline void SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex);
   
    //-----------------------------------------------------------------------------------//
    inline DirectedEdgeIndex_Of_MembraneMesh GetIndex() const;
    inline int_max GetStartPointIndex() const;
    inline int_max GetEndPointIndex() const;
    inline int_max GetEdgeIndex() const;
    inline int_max GetFaceIndex() const;

	inline DirectedEdgeIndex_Of_MembraneMesh GetNextDirectedEdgeIndex() const;
	inline DirectedEdgeIndex_Of_MembraneMesh GetPreviousDirectedEdgeIndex() const;

    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh> GetFirendDirectedEdgeIndexList() const;
    
	inline DenseVector<int_max> GetFirendFaceIndexList() const;

    inline DenseVector<int_max> GetNeighbourFaceIndexList() const;
    inline void GetNeighbourFaceIndexList(DenseVector<int_max>& OutputIndexList) const;

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

    inline Handle_Of_Face_Of_MembraneMesh GetFaceHandle() const;
    inline int_max GetFaceID() const;

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

	inline int_max GetFirendDirectedEdgeCount() const;
	inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetFirendDirectedEdgeHandleList() const;
	inline DenseVector<int_max> GetFirendDirectedEdgeIDList() const;

	inline int_max GetFirendFaceCount() const;
	inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetFirendFaceHandleList() const;
	inline DenseVector<int_max> GetFirendFaceIDList() const;

	inline int_max GetNeighbourFaceCount() const;

	inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetNeighbourFaceHandleList() const;
    inline void GetNeighbourFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetNeighbourFaceIDList() const;
    inline void GetNeighbourFaceIDList(DenseVector<int_max>& OutputHandleList) const;

    inline DirectedEdgeAttributeType& Attribute();
    inline const DirectedEdgeAttributeType& Attribute() const;
};

//====================================== Face_Of_MembraneMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Face_Of_MembraneMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::FaceAttributeType    FaceAttributeType;

    MembraneMesh<MeshAttributeType> Mesh;

    int_max Index; // FaceIndex: index of the Face in Mesh.m_MeshData->FaceList, it may change after Mesh.ClearDataStructure()

    int_max ID; // unique identifier(valid if >=0, invalid if < 0), it will not change after Mesh.ClearDataStructure()

    // do NOT need this
	//DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;

    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> DirectedEdgeIndexList;

    //--------------------------------------

    FaceAttributeType Attribute;

//------------------------------------------------------------------------
    Data_Of_Face_Of_MembraneMesh() : Mesh(MDK_PURE_EMPTY) {}
    ~Data_Of_Face_Of_MembraneMesh() {};
};

template<typename MeshAttributeType>
class Face_Of_MembraneMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::FaceAttributeType    FaceAttributeType;

private:
    std::unique_ptr<Data_Of_Face_Of_MembraneMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class MembraneMesh;

public:
	Face_Of_MembraneMesh();
    Face_Of_MembraneMesh(const Face_Of_MembraneMesh<MeshAttributeType>& InputFace);
    Face_Of_MembraneMesh(Face_Of_MembraneMesh<MeshAttributeType>&& InputFace);
    ~Face_Of_MembraneMesh();

    void operator=(const Face_Of_MembraneMesh<MeshAttributeType>& InputFace);
    void operator=(Face_Of_MembraneMesh<MeshAttributeType>&& InputFace);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(MembraneMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max FaceIndex);   
    inline DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& DirectedEdgeIndexList();

    //----------------------------------------------------------------------------//    
    inline int_max GetIndex() const;

    inline DenseVector<int_max> GetPointIndexList() const;
    inline void GetPointIndexList(DenseVector<int_max>& OutputIndexList) const;

    inline DenseVector<int_max> GetEdgeIndexList() const;
    inline void GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const;

    // Face share any Edge of this cell, not include this cell
    inline DenseVector<int_max> GetAdjacentFaceIndexList() const;
    inline void GetAdjacentFaceIndexList(DenseVector<int_max>& OutputIndexList) const;

    //--------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline MembraneMesh<MeshAttributeType>& GetParentMesh();
    inline const MembraneMesh<MeshAttributeType>& GetParentMesh() const;

    inline Handle_Of_Face_Of_MembraneMesh GetHandle() const;

    inline void SetID(int_max FaceID);
    inline void EraseID();
    inline int_max GetID() const;

	inline int_max GetDirectedEdgeCount() const; // the number of Directed Edge

    inline DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetDirectedEdgeIDList() const;
    inline void GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetPointCount() const; // the number of vertex point

    inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetPointIDList() const;
    inline void GetPointIDList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputIDList) const;

	inline int_max GetEdgeCount() const;  // the number of Edge

    inline DenseVector<Handle_Of_Edge_Of_MembraneMesh> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetEdgeIDList() const;
    inline void GetEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetEdgeRelativeIndexList() const;
    inline void GetEdgeRelativeIndexList(DenseVector<int_max>& RelativeIndexList) const;

	inline int_max GetAdjacentFaceCount() const;// share an edge

    inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetAdjacentFaceHandleList() const;
    inline void GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

    inline DenseVector<int_max> GetAdjacentFaceIDList() const;
    inline void GetAdjacentFaceIDList(DenseVector<int_max>& OutputIDList) const;

	inline int_max GetNeighbourFaceCount() const;// share a vertex point

	inline DenseVector<Handle_Of_Face_Of_MembraneMesh> GetNeighbourFaceHandleList() const;
	inline void GetNeighbourFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const;

	inline DenseVector<int_max> GetNeighbourFaceIDList() const;
	inline void GetNeighbourFaceIDList(DenseVector<int_max>& OutputIDList) const;

    inline FaceAttributeType& Attribute();
    inline const FaceAttributeType& Attribute() const;

	inline Handle_Of_Edge_Of_MembraneMesh GetEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const;

	//Direction: A -> B
	Handle_Of_DirectedEdge_Of_MembraneMesh GetDirectedEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const;

	// output handle list {PointHandleA, ...}
	inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetPointHandleList_LeadBy(Handle_Of_Point_Of_MembraneMesh PointHandleA) const;

	// output handle list {PointHandleA, PointHandleB, ...}
	inline DenseVector<Handle_Of_Point_Of_MembraneMesh> GetPointHandleList_LeadBy(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const;

};

}// namespace mdk

#include "mdkMembraneMeshItem.hpp"

#endif