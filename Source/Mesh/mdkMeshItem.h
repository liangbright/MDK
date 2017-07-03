#pragma once

#include "mdkObjectArray.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseMatrix.h"
#include "mdkMeshStandardAttribute.h"

namespace mdk
{
//------------------------------------------------- forward declare --------------------------------------------------------//
template<typename T>
struct MeshData;

template<typename T>
class Mesh;

template<typename T>
class Point_Of_Mesh;

template<typename T>
class Edge_Of_Mesh;

template<typename T>
class Face_Of_Mesh;

template<typename T>
class Cell_Of_Mesh;
//----------------------------------------------------------------------------------------------------------------------------//


//====================================== Point_Of_Mesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Point_Of_Mesh
{
	/*
	//weird: can not compile in vs2015
	MeshData<ScalarType>* MeshData;
	std::weak_ptr<MeshData<ScalarType>> MeshData_wp;
	*/

	std::weak_ptr<MeshData<ScalarType>> MeshData_wp;
	//Mesh<ScalarType> ParentMesh;
	//ParentMesh.m_MeshData=m_Data->MeshData_wp.lock();
	//then use any function of ParentMesh

	MeshData<ScalarType>* MeshData;
	// do NOT use Mesh<>  => cyclic link and memory leak
	// do NOT use Mesh<>* => may become invalid, e.g. Mesh A, B; A.Share(B);

    int_max Index;  // PointIndex : index in Mesh.m_MeshData->PointList; it may change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

    DenseVector<int_max> AdjacentEdgeIndexList;  // index in MeshData->EdgeList

    //------------------------------------------------

	StandardAttribute_Of_Point_Of_Mesh<ScalarType> Attribute;
};

template<typename Scalar_Type>
class Point_Of_Mesh : public Object
{
public:
    typedef Scalar_Type ScalarType;
	typedef StandardAttribute_Of_Point_Of_Mesh<ScalarType> PointAttributeType;

private:
    std::unique_ptr<Data_Of_Point_Of_Mesh<ScalarType>> m_Data;

    template<typename T>
    friend class Mesh;

	template<typename T>
	friend class Face_Of_Mesh;

	template<typename T>
	friend class Edge_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

public:
	inline Point_Of_Mesh();
    inline Point_Of_Mesh(const Point_Of_Mesh<ScalarType>& InputPoint);
    inline Point_Of_Mesh(Point_Of_Mesh<ScalarType>&& InputPoint);
    inline ~Point_Of_Mesh();

    inline void operator=(const Point_Of_Mesh<ScalarType>& InputPoint);
    inline void operator=(Point_Of_Mesh<ScalarType>&& InputPoint);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(Mesh<ScalarType>& ParentMesh);
    
    inline void SetIndex(int_max PointIndex);
    
    inline DenseVector<int_max>& AdjacentEdgeIndexList();
	inline const DenseVector<int_max>& AdjacentEdgeIndexList() const;

	//-------------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;
    bool IsOnEdge() const;
    bool IsOnBoundaryEdge() const;//polygon mesh
	bool IsOnBoundaryFace() const;//polyhedron mesh

    inline int_max GetIndex() const;

	inline void SetName(String PointName);
	inline void EraseName();
	inline String GetName() const;

	inline void SetData(int_max Index, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(int_max Index) const;

	inline void SetData(const String& Name, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(const String& Name) const;

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

	inline int_max GetAdjacentCellCount() const;
	inline DenseVector<int_max> GetAdjacentCellIndexList() const;

    inline PointAttributeType& Attribute();
    inline const PointAttributeType& Attribute() const;

	//MaxGraphDistance= edge count
	inline DenseVector<int_max> GetNeighborPointIndexList(int_max MaxGraphDistance) const;
};

//====================================== Edge_Of_Mesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Edge_Of_Mesh
{
	std::weak_ptr<MeshData<ScalarType>> MeshData_wp;
	MeshData<ScalarType>* MeshData;	

    int_max Index; // EdgeIndex: index of this Edge in Mesh.MeshData->EdgeList, it may change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

    int_max PointIndex0;
    int_max PointIndex1;

	DenseVector<int_max> AdjacentFaceIndexList;// Face share this edge, maybe more than two

    //--------------------------------

	StandardAttribute_Of_Edge_Of_Mesh<ScalarType> Attribute;
};

template<typename Scalar_Type>
class Edge_Of_Mesh : public Object
{
public:
    typedef Scalar_Type ScalarType;
	typedef StandardAttribute_Of_Edge_Of_Mesh<ScalarType> EdgeAttributeType;

    template<typename T>
    friend class Mesh;

    template<typename T>
    friend class Point_Of_Mesh;

    template<typename T>
    friend class Face_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

private:
    std::unique_ptr<Data_Of_Edge_Of_Mesh<ScalarType>> m_Data;

public:
	inline Edge_Of_Mesh();
    inline Edge_Of_Mesh(const Edge_Of_Mesh<ScalarType>& InputEdge);
    inline Edge_Of_Mesh(Edge_Of_Mesh<ScalarType>&& InputEdge);
    inline ~Edge_Of_Mesh();

    inline void operator=(const Edge_Of_Mesh<ScalarType>& InputEdge);
    inline void operator=(Edge_Of_Mesh<ScalarType>&& InputEdge);
    
private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(Mesh<ScalarType>& ParentMesh);
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

    inline int_max GetIndex() const;

    inline void SetID(int_max EdgeID);
    inline void EraseID();
    inline int_max GetID() const;
   
	inline void SetName(String EdgeName);
	inline void EraseName();
	inline String GetName() const;

	inline void SetData(int_max Index, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(int_max Index) const;

	inline void SetData(const String& Name, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(const String& Name) const;

	inline void GetPointIndexList(int_max PointIndexList[2]) const;
	inline void GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const;
	inline DenseVector<int_max, 2> GetPointIndexList() const;

	inline int_max GetAdjacentEdgeCount() const;
	inline DenseVector<int_max> GetAdjacentEdgeIndexList() const;

	// Face share this edge
	inline int_max GetAdjacentFaceCount() const;
	inline DenseVector<int_max> GetAdjacentFaceIndexList() const;
	
	// Face share any vertex point of this edge
	inline int_max GetNeighborFaceCount() const;
	inline DenseVector<int_max> GetNeighborFaceIndexList() const;

	// Cell share this edge
	inline int_max GetAdjacentCellCount() const;
	inline DenseVector<int_max> GetAdjacentCellIndexList() const;

	// Cell share any vertex point of this edge
	inline int_max GetNeighborCellCount() const;
	inline DenseVector<int_max> GetNeighborCellIndexList() const;

    inline EdgeAttributeType& Attribute();
    inline const EdgeAttributeType& Attribute() const;
};

//====================================== Face_Of_Mesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Face_Of_Mesh
{
	std::weak_ptr<MeshData<ScalarType>> MeshData_wp;
	MeshData<ScalarType>* MeshData;	

    int_max Index; // FaceIndex: index of the Face in Mesh.m_MeshData->FaceList, it may change after Mesh.CleanDataStructure()

	String Name;// may be empty, if not empty it must be unique

	DenseVector<int_max> PointIndexList;// this is an ordered set
	// PointIndexList[0]->PointIndexList[1]->PointIndexList[2] define the normal direction using right-hand system
	
	DenseVector<int_max> EdgeIndexList;// this is an ordered set
	// EdgeIndexList[0] is between PointIndexList[0] and PointIndexList[1]
	// PointIndexList[end] is between PointIndexList[end] and PointIndexList[0]

	DenseVector<int_max> AdjacentCellIndexList;// Cell share this face: none or 1 or 2
    
	StandardAttribute_Of_Face_Of_Mesh<ScalarType> Attribute;
};


template<typename Scalar_Type>
class Face_Of_Mesh : public Object
{
public:
    typedef Scalar_Type ScalarType;
	typedef StandardAttribute_Of_Face_Of_Mesh<ScalarType> FaceAttributeType;

private:
    std::unique_ptr<Data_Of_Face_Of_Mesh<ScalarType>> m_Data;

    template<typename T>
    friend class Mesh;

	template<typename T>
	friend class Point_Of_Mesh;

	template<typename T>
	friend class Edge_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

public:
	Face_Of_Mesh();
    Face_Of_Mesh(const Face_Of_Mesh<ScalarType>& InputFace);
    Face_Of_Mesh(Face_Of_Mesh<ScalarType>&& InputFace);
    ~Face_Of_Mesh();

    void operator=(const Face_Of_Mesh<ScalarType>& InputFace);
    void operator=(Face_Of_Mesh<ScalarType>&& InputFace);

private:
    inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void SetParentMesh(Mesh<ScalarType>& ParentMesh);
    inline void SetIndex(int_max FaceIndex);   

	inline DenseVector<int_max>& PointIndexList();
	inline const DenseVector<int_max>& PointIndexList() const;

	inline DenseVector<int_max>& EdgeIndexList();
	inline const DenseVector<int_max>& EdgeIndexList() const;

	inline DenseVector<int_max>& AdjacentCellIndexList();
	inline const DenseVector<int_max>& AdjacentCellIndexList() const;

    //--------------------------------------------------------------------------------//
public:
    inline bool IsValid() const;

	inline int_max GetIndex() const;

	inline void SetName(String FaceName);
	inline void EraseName();
	inline String GetName() const;

	inline void SetData(int_max Index, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(int_max Index) const;

	inline void SetData(const String& Name, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(const String& Name) const;

	inline int_max GetPointCount() const;
	inline DenseVector<int_max> GetPointIndexList() const;

	inline int_max GetEdgeCount() const;
	inline DenseVector<int_max> GetEdgeIndexList() const;

	// Face share any Edge of this face, not include this face
	inline int_max GetAdjacentFaceCount() const;
	inline DenseVector<int_max> GetAdjacentFaceIndexList() const;

	// Face share any point of this face, not include this face
	inline int_max GetNeighborFaceCount() const;
	inline DenseVector<int_max> GetNeighborFaceIndexList() const;

	// Cell share any Edge of this face, not include this face
	inline int_max GetAdjacentCellCount() const;
	inline DenseVector<int_max> GetAdjacentCellIndexList() const;

	// Cell share any point of this face, not include this face
	inline int_max GetNeighborCellCount() const;
	inline DenseVector<int_max> GetNeighborCellIndexList() const;

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

//====================================== Cell_Of_Mesh ==============================================================//

template<typename ScalarType>
struct Data_Of_Cell_Of_Mesh
{	
	std::weak_ptr<MeshData<ScalarType>> MeshData_wp;
	MeshData<ScalarType>* MeshData;	

	int_max Index; // CellIndex: index of the Cell in Mesh.m_MeshData->FaceList, it may change after Mesh.CleanDataStructure()
	String Name;// may be empty, if not empty it must be unique

	DenseVector<int_max> PointIndexList;// this is an ordered set	
	DenseVector<int_max> FaceIndexList;// this is an ordered set

	StandardAttribute_Of_Cell_Of_Mesh<ScalarType> Attribute;
};

template<typename Scalar_Type>
class Cell_Of_Mesh : public Object
{
public:
	typedef Scalar_Type ScalarType;
	typedef StandardAttribute_Of_Cell_Of_Mesh<ScalarType> CellAttributeType;

private:
	std::unique_ptr<Data_Of_Cell_Of_Mesh<ScalarType>> m_Data;

	template<typename T>
	friend class Mesh;

	template<typename T>
	friend class Point_Of_Mesh;

	template<typename T>
	friend class Edge_Of_Mesh;

	template<typename T>
	friend class Face_Of_Mesh;

public:
	Cell_Of_Mesh();
	Cell_Of_Mesh(const Cell_Of_Mesh<ScalarType>& InputCell);
	Cell_Of_Mesh(Cell_Of_Mesh<ScalarType>&& InputCell);
	~Cell_Of_Mesh();

	void operator=(const Cell_Of_Mesh<ScalarType>& InputCell);
	void operator=(Cell_Of_Mesh<ScalarType>&& InputCell);

private:
	inline void ReCreate();
	inline void Clear(const MDK_Symbol_PureEmpty&);

	inline void SetParentMesh(Mesh<ScalarType>& ParentMesh);
	inline void SetIndex(int_max CellIndex);

	inline DenseVector<int_max>& PointIndexList();
	inline const DenseVector<int_max>& PointIndexList() const;

	inline DenseVector<int_max>& FaceIndexList();
	inline const DenseVector<int_max>& FaceIndexList() const;

	//--------------------------------------------------------------------------------//
public:
	inline bool IsValid() const;

	inline int_max GetIndex() const;

	inline void SetName(String CellName);
	inline void EraseName();
	inline String GetName() const;

	inline void SetData(int_max Index, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(int_max Index) const;

	inline void SetData(const String& Name, DenseVector<ScalarType> Data);
	inline DenseVector<ScalarType> GetData(const String& Name) const;

	inline int_max GetPointCount() const;
	inline DenseVector<int_max> GetPointIndexList() const;

	inline int_max GetEdgeCount() const;
	inline DenseVector<int_max> GetEdgeIndexList() const;

	inline int_max GetFaceCount() const;
	inline DenseVector<int_max> GetFaceIndexList() const;

	// Cell share any Edge of this Cell, not include this Cell
	inline int_max GetAdjacentCellCount() const;
	inline DenseVector<int_max> GetAdjacentCellIndexList() const;

	// Cell share any point of this Cell, not include this Cell
	inline int_max GetNeighborCellCount() const;
	inline DenseVector<int_max> GetNeighborCellIndexList() const;

	inline CellAttributeType& Attribute();
	inline const CellAttributeType& Attribute() const;
};


}// namespace mdk

#include "mdkMeshItem.hpp"
