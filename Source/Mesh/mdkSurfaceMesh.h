#ifndef __mdkSurfaceMesh_h
#define __mdkSurfaceMesh_h

#include "mdkSurfaceMeshItem.h"

namespace mdk
{

//------------- forward declare -----------//
template<typename ScalarType>
struct SurfaceMeshAttribute;
//----------------------------------------//

template<typename ScalarType>
struct SurfaceMeshData
{
    DenseVector<int_max> VertexGlobalIDList; // Length = Length of VertexList

    DenseVector<int_max> CellGlobalIDList; // Length = Length of CellList

    DenseMatrix<ScalarType> PointPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // a point is a vertex of an Edge or a point on Edge Edge

    DataArray<Point_Of_SurfaceMesh<ScalarType>> PointList;

    DenseVector<int_max> PointValidityFlagList;
    // 1: point is an element of the mesh 
    // 0: point is deleted

    DataArray<Vertex_Of_SurfaceMesh<ScalarType>> VertexList;

    DenseVector<int_max> VertexValidityFlagList;
    // 1: vertex is an element of the mesh 
    // 0: vertex is deleted

    DataArray<Cell_Of_SurfaceMesh<ScalarType>> CellList; // also known as cell, face, facet, element

    DenseVector<int_max>  CellValidityFlagList;
    // 1: Cell is an element of the mesh 
    // 0: Cell is deleted

    DataArray<Edge_Of_SurfaceMesh<ScalarType>> EdgeList;

    DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<ScalarType>, 2>> DirectedEdgePairList;

    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

    //---------------- Mesh Attribute -----------------------------//

    SurfaceMeshAttribute<ScalarType> MeshAttribute;
}

template<typename ScalarType>
struct SurfaceMeshAttribute
{
};


template<typename ScalarType = double>
class SurfaceMesh : public Object
{
protected:
    std::shared_ptr<SurfaceMeshData<ScalarType>> m_MeshData;

public:
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
    typedef ScalarType ScalarType;
    typedef int_max IndexType;

public:
    SurfaceMesh();
    SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&);
    SurfaceMesh(const SurfaceMesh& InputMesh);
    SurfaceMesh(SurfaceMesh&& InputMesh);
    ~SurfaceMesh();

    inline void operator=(const SurfaceMesh& InputMesh);
    inline void operator=(SurfaceMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const SurfaceMesh<ScalarType>& InputMesh);

    inline bool Copy(const SurfaceMesh<ScalarType>* InputMesh);

    inline void Share(SurfaceMesh& InputMesh);
    inline bool Share(SurfaceMesh* InputMesh);

    inline void ForceShare(const SurfaceMesh& InputMesh);
    inline bool ForceShare(const SurfaceMesh* InputMesh);

    inline void Take(SurfaceMesh&& InputMesh);
    inline bool Take(SurfaceMesh& InputMesh);
    inline bool Take(SurfaceMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetValidPointNumber() const;
    inline int_max GetValidVertexNumber() const;
    inline int_max GetValidCellNumber() const;
    inline int_max GetValidEdgeNumber() const;

    // GlobalID -------------------------------------------------------------
    inline DenseVector<int_max>& VertexGlobalIDList();
    inline const DenseVector<int_max>& VertexGlobalIDList() const;

    inline DenseVector<int_max>& CellGlobalIDList();
    inline const DenseVector<int_max>& CellGlobalIDList() const;

    // 3D Position -----------------------------------------------------------
    inline DenseMatrix<ScalarType>& PointPositionTable();
    inline const DenseMatrix<ScalarType>& PointPositionTable() const;

    // mesh item --------------------------------------------------------------

    inline DataArray<Point_Of_SurfaceMesh<ScalarType>>& PointList();
    inline const DataArray<Point_Of_SurfaceMesh<ScalarType>>& PointList() const;

    inline DenseVector<int_max>& PointValidityFlagList();
    inline const DenseVector<int_max>& PointValidityFlagList() const;

    inline DataArray<Vertex_Of_SurfaceMesh<ScalarType>>& VertexList();
    inline const DataArray<Vertex_Of_SurfaceMesh<ScalarType>>& VertexList() const;

    inline DenseVector<int_max>& VertexValidityFlagList();
    inline const DenseVector<int_max>& VertexValidityFlagList() const;

    inline DataArray<Edge_Of_SurfaceMesh<ScalarType>>& EdgeList();
    inline const DataArray<Edge_Of_SurfaceMesh<ScalarType>>& EdgeList() const;

    inline DataArray<DirectedEdge_Of_SurfaceMesh<ScalarType>>& DirectedEdgePairList();
    inline const DataArray<DirectedEdge_Of_SurfaceMesh<ScalarType>>& DirectedEdgePairList() const;

    inline DenseVector<int_max>& EdgeValidityFlagList();
    inline const DenseVector<int_max>& EdgeValidityFlagList() const;

    inline DataArray<Cell_Of_SurfaceMesh<ScalarType>>& CellList();
    inline const DataArray<Cell_Of_SurfaceMesh<ScalarType>>& CellList() const;

    inline DenseVector<int_max>& CellValidityFlagList();
    inline const DenseVector<int_max>& CellValidityFlagList() const;

    //----- Convenient function to Get/Set  Mesh Item {Point, Vertex, Edge, DirectedEdge, Cell} ------//

    inline Point_Of_SurfaceMesh<ScalarType>& Point(int_max PointIndex);
    inline const Point_Of_SurfaceMesh<ScalarType>& Point(int_max PointIndex) const;

    inline Vertex_Of_SurfaceMesh<ScalarType>& Vertex(int_max VertexIndex);
    inline const Vertex_Of_SurfaceMesh<ScalarType>& Vertex(int_max VertexIndex) const;

    inline Edge_Of_SurfaceMesh<ScalarType>& Edge(int_max EdgeIndex, int_max RelativeIndex);
    inline const Edge_Of_SurfaceMesh<ScalarType>& Edge(int_max EdgeIndex, int_max RelativeIndex) const;

    inline DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(int_max EdgeIndex, int_max RelativeIndex);
    inline const DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(int_max EdgeIndex, int_max RelativeIndex) const;

    inline DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline const DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex) const;

    inline Cell_Of_SurfaceMesh<ScalarType>& Cell(int_max CellIndex);
    inline const Cell_Of_SurfaceMesh<ScalarType>& Cell(int_max CellIndex) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointIndex in m_MeshData->PointList
    int_max AddPoint(ScalarType Position[3]);
    int_max AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a point set and return PointIndexList
    DenseVector<int_max> AddPoint(const DenseMatrix<ScalarType>& PointSet);

    // add a Vertex and return VertexIndex in m_MeshData->VertexList
    int_max AddVertex(int_max PointIndex);
    DenseVector<int_max> AddVertexSet(DenseVector<int_max> PointIndexList);

    // add an Edge and return EdgeIndex in m_MeshData->EdgeList
    // also create invalid DirectedEdge to hold place at  m_MeshData->DirectedEdgeList[EdgeIndex]
    int_max AddEdge(int_max VertexIndex0, int_max VertexIndex1, DenseVector<int_max> PointIndexList);

    // add a cell and return CellIndex in m_MeshData->CellList
    // add DirectedEdge of the cell
    // the order of Edge in EdgeIndexList determine the order of DirectedEdge
    int_max AddCell(const DenseVector<int_max>& EdgeIndexList);

    // Delete Mesh Item ----------------------------------------------------------------------------//

    // m_MeshData->CellList(CellIndex).Clear() only clear memory
    // this function will remove mesh item related to the cell
    void DeleteCell(int_max CellIndex);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only clear memory
    // this function will remove mesh item related to the Edge
    void DeleteEdge(int_max EdgeIndex);

    // the same as m_MeshData->PointPositionTable.DeleteCol(...)
    void DeletePoint(int_max PointIndex);
    void DeletePoint(const DenseVector<int_max>& PointIndexList);

    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after this function is called, every index will be changed
    // and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
    void CleanDataStructure();
    //---------------------------------------------------------------------------

};

}// namespace mdk

#include "mdkSurfaceMesh.hpp"

#endif