#pragma once

#include "mdkObjectArray.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//------------------------------------------------- forward declare --------------------------------------------------------//
template<typename T>
struct PolygonMeshData;

template<typename T>
class PolygonMesh;

template<typename T>
class Point_Of_PolygonMesh;

template<typename T>
class Edge_Of_PolygonMesh;

template<typename T>
class Face_Of_PolygonMesh;
//----------------------------------------------------------------------------------------------------------------------------//


//====================================== Point_Of_PolygonMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Point_Of_PolygonMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

	PolygonMeshData<MeshAttributeType>* MeshData;

    int_max Index;  // PointIndex : index in Mesh.m_MeshData->PointList; it may change after Mesh.CleanDataStructure()

    int_max ID; // unique identifier (valid if >=0, invalid if < 0), it will not change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

    DenseVector<int_max> AdjacentEdgeIndexList;  // index in Mesh.m_MeshData->EdgeList   

    //------------------------------------------------

    PointAttributeType Attribute;
};

template<typename MeshAttributeType>
class Point_Of_PolygonMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::PointAttributeType   PointAttributeType;

private:
    std::unique_ptr<Data_Of_Point_Of_PolygonMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class PolygonMesh;

	template<typename T>
	friend class Face_Of_PolygonMesh;

	template<typename T>
	friend class Edge_Of_PolygonMesh;

public:
	inline Point_Of_PolygonMesh();
    inline Point_Of_PolygonMesh(const Point_Of_PolygonMesh<MeshAttributeType>& InputPoint);
    inline Point_Of_PolygonMesh(Point_Of_PolygonMesh<MeshAttributeType>&& InputPoint);
    inline ~Point_Of_PolygonMesh();

    inline void operator=(const Point_Of_PolygonMesh<MeshAttributeType>& InputPoint);
    inline void operator=(Point_Of_PolygonMesh<MeshAttributeType>&& InputPoint);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(PolygonMesh<MeshAttributeType>& ParentMesh);
    
    inline void SetIndex(int_max PointIndex);
    
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
	inline const DenseVector<int_max>& AdjacentEdgeIndexList() const;

	//-------------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    bool IsOnEdge() const;
    bool IsOnBoundaryEdge() const;

    inline PolygonMesh<MeshAttributeType>& GetParentMesh();
    inline const PolygonMesh<MeshAttributeType>& GetParentMesh() const;
    inline int_max GetIndex() const;

    inline void SetID(int_max PointID);
    inline void EraseID();
    inline int_max GetID() const;

	inline void SetName(String PointName);
	inline void EraseName();
	inline String GetName() const;

    inline void SetPosition(const DenseVector<ScalarType, 3>& Pos);
    inline void SetPosition(const ScalarType Pos[3]);
    inline void SetPosition(ScalarType x, ScalarType y, ScalarType z);

    inline DenseVector<ScalarType, 3> GetPosition() const;
    inline void GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const;
    inline void GetPosition(ScalarType Pos[3]) const;

	inline int_max GetAdjacentPointCount() const;
    inline DenseVector<int_max> GetAdjacentPointIndexList() const;

	inline int_max GetAdjacentEdgeCount() const;
    inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;

	inline int_max GetAdjacentFaceCount() const;
    inline DenseVector<int_max> GetAdjacentFaceIndexList() const;

    inline PointAttributeType& Attribute();
    inline const PointAttributeType& Attribute() const;
};

//====================================== Edge_Of_PolygonMesh (Face Wall) ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Edge_Of_PolygonMesh
{
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

	PolygonMeshData<MeshAttributeType>* MeshData;
    
    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList, it may change after Mesh.CleanDataStructure()

    int_max ID; // unique identifier(valid if >=0, invalid if < 0), it will not change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

    int_max PointIndex0;
    int_max PointIndex1;

	DenseVector<int_max> AdjacentFaceIndexList;// Face share this edge, maybe more than two

    //--------------------------------

    EdgeAttributeType Attribute;
};

template<typename MeshAttributeType>
class Edge_Of_PolygonMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType         ScalarType;
    typedef typename MeshAttributeType::EdgeAttributeType  EdgeAttributeType;

    template<typename T>
    friend class PolygonMesh;

    template<typename T>
    friend class Point_Of_PolygonMesh;

    template<typename T>
    friend class Face_Of_PolygonMesh;

private:
    std::unique_ptr<Data_Of_Edge_Of_PolygonMesh<MeshAttributeType>> m_Data;

public:
	inline Edge_Of_PolygonMesh();
    inline Edge_Of_PolygonMesh(const Edge_Of_PolygonMesh<MeshAttributeType>& InputEdge);
    inline Edge_Of_PolygonMesh(Edge_Of_PolygonMesh<MeshAttributeType>&& InputEdge);
    inline ~Edge_Of_PolygonMesh();

    inline void operator=(const Edge_Of_PolygonMesh<MeshAttributeType>& InputEdge);
    inline void operator=(Edge_Of_PolygonMesh<MeshAttributeType>&& InputEdge);
    
private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(PolygonMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max EdgeIndex);

    inline void SetPointIndexList(const int_max PointIndexList[2]);
    inline void SetPointIndexList(int_max PointIndex0, int_max PointIndex1);
	inline void SetPointIndexList(const DenseVector<int_max, 2>& PointIndexList);

	inline DenseVector<int_max>& AdjacentFaceIndexList();
	inline const DenseVector<int_max>& AdjacentFaceIndexList() const;
    //--------------------------------------------------------------------------//
public:

    inline bool IsValid() const;

    bool IsBoundary() const;

    inline PolygonMesh<MeshAttributeType>& GetParentMesh();
    inline const PolygonMesh<MeshAttributeType>& GetParentMesh() const;
    inline int_max GetIndex() const;

    inline void SetID(int_max EdgeID);
    inline void EraseID();
    inline int_max GetID() const;
   
	inline void SetName(String EdgeName);
	inline void EraseName();
	inline String GetName() const;

	inline void GetPointIndexList(int_max PointIndexList[2]) const;
	inline void GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const;
	inline DenseVector<int_max, 2> GetPointIndexList() const;

	inline int_max GetAdjacentEdgeCount() const;
	inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;

	// Face share this edge
	inline int_max GetAdjacentFaceCount() const;
	inline DenseVector<int_max> GetAdjacentFaceIndexList() const;
	
	// Face share any vertex point of this edge
	inline int_max GetNeighbourFaceCount() const;
	inline DenseVector<int_max> GetNeighbourFaceIndexList() const;

    inline EdgeAttributeType& Attribute();
    inline const EdgeAttributeType& Attribute() const;
};

//====================================== Face_Of_PolygonMesh ==============================================================//

template<typename MeshAttributeType>
struct Data_Of_Face_Of_PolygonMesh
{
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::FaceAttributeType    FaceAttributeType;

	PolygonMeshData<MeshAttributeType>* MeshData;

    int_max Index; // FaceIndex: index of the Face in Mesh.m_MeshData->FaceList, it may change after Mesh.CleanDataStructure()

    int_max ID; // unique identifier(valid if >=0, invalid if < 0), it will not change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

	DenseVector<int_max> PointIndexList;// this is an ordered set
	// PointIndexList[0]->PointIndexList[1]->PointIndexList[2] define the normal direction using right-hand system
	
	DenseVector<int_max> EdgeIndexList;// this is an ordered set
	// EdgeIndexList[0] is between PointIndexList[0] and PointIndexList[1]
	// PointIndexList[end] is between PointIndexList[end] and PointIndexList[0]

    //--------------------------------------

    FaceAttributeType Attribute;
};

template<typename MeshAttributeType>
class Face_Of_PolygonMesh : public Object
{
public:
    typedef typename MeshAttributeType::ScalarType           ScalarType;
    typedef typename MeshAttributeType::FaceAttributeType    FaceAttributeType;

private:
    std::unique_ptr<Data_Of_Face_Of_PolygonMesh<MeshAttributeType>> m_Data;

    template<typename T>
    friend class PolygonMesh;

	template<typename T>
	friend class Point_Of_PolygonMesh;

public:
	Face_Of_PolygonMesh();
    Face_Of_PolygonMesh(const Face_Of_PolygonMesh<MeshAttributeType>& InputFace);
    Face_Of_PolygonMesh(Face_Of_PolygonMesh<MeshAttributeType>&& InputFace);
    ~Face_Of_PolygonMesh();

    void operator=(const Face_Of_PolygonMesh<MeshAttributeType>& InputFace);
    void operator=(Face_Of_PolygonMesh<MeshAttributeType>&& InputFace);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(PolygonMesh<MeshAttributeType>& ParentMesh);
    inline void SetIndex(int_max FaceIndex);   

	inline DenseVector<int_max>& PointIndexList();
	inline const DenseVector<int_max>& PointIndexList() const;

	inline DenseVector<int_max>& EdgeIndexList();
	inline const DenseVector<int_max>& EdgeIndexList() const;

    //--------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

    inline PolygonMesh<MeshAttributeType>& GetParentMesh();
    inline const PolygonMesh<MeshAttributeType>& GetParentMesh() const;
	inline int_max GetIndex() const;

    inline void SetID(int_max FaceID);
    inline void EraseID();
    inline int_max GetID() const;

	inline void SetName(String FaceName);
	inline void EraseName();
	inline String GetName() const;

	inline int_max GetPointCount() const;
	inline DenseVector<int_max> GetPointIndexList() const;

	inline int_max GetEdgeCount() const;
	inline DenseVector<int_max> GetEdgeIndexList() const;

	// Face share any Edge of this face, not include this face
	inline int_max GetAdjacentFaceCount() const;
	inline DenseVector<int_max> GetAdjacentFaceIndexList() const;

	// Face share any point of this face, not include this face
	inline int_max GetNeighbourFaceCount() const;
	inline DenseVector<int_max> GetNeighbourFaceIndexList() const;

    inline FaceAttributeType& Attribute();
    inline const FaceAttributeType& Attribute() const;

	inline int_max GetRelativeIndexOfPoint(int_max PointIndex) const;

	inline int_max GetEdgeIndexByPoint(int_max PointIndexA, int_max PointIndexB) const;

	// true: A -> B ; false: B->A
	bool CheckPointOrder(int_max PointIndexA, int_max PointIndexB) const;

	// output list {PointIndexA, ...}, the point-order determine face normal (right hand rule) 
	inline DenseVector<int_max> GetPointIndexList_LeadBy(int_max PointIndexA) const;

	// output list {PointIndexA, PointIndexB, ...}, the point-order may determine the opposite of face normal 
	inline DenseVector<int_max> GetPointIndexList_LeadBy(int_max PointIndexA, int_max PointIndexB) const;

	// change P0->P1->P2->...->Pn to Pn->...->P2->P1->P0
	inline void ReversePointOrder();
};

}// namespace mdk

#include "mdkPolygonMeshItem.hpp"
