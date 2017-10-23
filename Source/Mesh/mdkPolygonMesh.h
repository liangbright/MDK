#pragma once

#include <tuple>
#include <unordered_map>

#include "mdkMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class PolygonMesh : public Mesh<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;
	//------------------------------------------------------------------------//
	typedef StandardAttribute_Of_Mesh<ScalarType>          MeshAttribute;
	typedef StandardAttribute_Of_Point_Of_Mesh<ScalarType> PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_Mesh<ScalarType>  EdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_Mesh<ScalarType>  FaceAttributeType;
	typedef StandardAttribute_Of_Cell_Of_Mesh<ScalarType>  CellAttributeType;
	//------------------------------------------------------------------------//
	typedef Point_Of_Mesh<ScalarType>  PointType;
	typedef Edge_Of_Mesh<ScalarType>   EdgeType;
	typedef Face_Of_Mesh<ScalarType>   FaceType;
	typedef Cell_Of_Mesh<ScalarType>   CellType;
	//------------------------------------------------------------------------//
	typedef StdObjectVector<PointType> PointListType;
	typedef StdObjectVector<EdgeType>  EdgeListType;
	typedef StdObjectVector<FaceType>  FaceListType;
	typedef StdObjectVector<CellType>  CellListType;
	//------------------------------------------------------------------------//

protected:
	template<typename T>
	friend class Point_Of_Mesh;

	template<typename T>
	friend class Edge_Of_Mesh;

	template<typename T>
	friend class Face_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

public:
    PolygonMesh();
    PolygonMesh(const MDK_Symbol_PureEmpty&);
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
	PolygonMesh(const Mesh& InputMesh);
	PolygonMesh(Mesh&& InputMesh);
    ~PolygonMesh();

	inline void operator=(const PolygonMesh& InputMesh);
	inline void operator=(PolygonMesh&& InputMesh);
	inline void operator=(const Mesh& InputMesh);
	inline void operator=(Mesh&& InputMesh);

	PolygonMesh<ScalarType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;

	//---------------------------------------------------------------------------------------------------------------//
	// get PointPositionMatrix And FaceTable
	// set Flag_Clean to true to output clean data structure
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> GetPointPositionMatrixAndFaceTable(bool Flag_Clean = true) const;
	void GetPointPositionMatrixAndFaceTable(DenseMatrix<ScalarType>& PointPositionMatrix, ObjectArray<DenseVector<int_max>>& FaceTable, bool Flag_Clean = true) const;

	//---- construct a polygon mesh ------------------
	void Construct(const std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>>& InputData);
	void Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable);

	//-------------- mesh editing -------------------------------------//

	// swap PointA and PointB: move A to B and move B to A
	// NOT change the mesh topology
	// A, B, C, D are point index/name
	//------------------------
	//   C          C
	//   |          |    
	//   A   B  =>  B   A
	//       |          |
	//       D          D
	//------------------------	
	bool SwapPoint(int_max PointIndexA, int_max PointIndexB);

	// swap the connection: NOT move A or B, Change the mesh topology
	// A, B, C, D are point ID/name
	//------------------------
	//   C         C 
	//   |           \ 
	//   A   B  => A   B
	//       |       \
	//       D         D
    //------------------------------
	bool SwapConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB);

	// merge the connection of PointB to the connection of PointA, then PointB become an isolated point
	// if an edge between A and B exist, then it will be deleted
	// return true if success
	// can NOT merge two point of a triangle face: return false
	//--------------------------------
	//    |   |          | /
	// ---A---B =>    ---A   B
	//    |   |          | \
	    //-------------------------------
    // special case: may produce duplicated edge
    // C---B                 C   B
    // |  /   Merge(A, B)    ||
    // A                     A
    // ABC may not be a face, i.e., ABC is a hole
    // Flag_CheckTriangle= ture then check is performed in the function to prevent this case
    //------------------------------
	bool MergeConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle = false);

	//PointB will be deleted
	bool MergePoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle = false);

	//merge Duplicated Edge between PointA and PointB
	void MergeDuplicatedEdgeBetweenPoint(int_max PointIndex0, int_max PointIndex1);

	// Point may not be on the edge
	// return true if success
	bool ShrinkEdgeToPoint(int_max EdgeIndex, int_max PointIndex);

	DenseVector<int_max, 2> SplitEdgeByPoint(int_max EdgeIndex, int_max PointIndex);

	// if sucess, return the index of the newly merged edge
	// int_max MergeAdjacentEdge(int_max EdgeIndexA, int_max EdgeIndexB);
	// int_max MergeAdjacentEdge(int_max EdgeIndexA, int_max EdgeIndexB, int_max EdgeIndex_A_or_B_or_Other);

	// Point may not be on the face
	// return true if success
	// can NOT shrink if it lead to merge of two point of a triangle face/hole: return false
	bool ShrinkFaceToPoint(int_max FaceIndex, int_max PointIndex);

	// if sucess, return the index of the newly merged face
	// int_max MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB);
	// int_max MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB, int_max FaceIndex_A_or_B_or_Other);

	//PointA, PointB belong to the face
	DenseVector<int_max, 2> SplitFaceByPoint(int_max FaceIndex, int_max PointIndexA, int_max PointIndexB);

	DenseVector<int_max, 2> SplitFaceByEdge(int_max FaceIndex, int_max EdgeABIndex);

	// bool ShrinkCellToPoint(int_max CellIndex, int_max PointIndex)
	// int_max MergeAdjacentCell(int_max CellIndexA, int_max CellIndexB);
	// int_max MergeAdjacentCell(int_max CellIndexA, int_max CellIndexB, int_max CellIndex_A_or_B_or_Other);

	int_max CutAlongEdge(int_max EdgeIndex01);
	DenseVector<int_max, 2> CutAlongEdge(int_max EdgeIndex01, int_max EdgeIndex12);
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"
#include "mdkPolygonMesh_FileIO.h"
