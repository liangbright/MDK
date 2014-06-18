#ifndef __mdkSurfaceMesh_h
#define __mdkSurfaceMesh_h

#include <unordered_map>

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
    DenseVector<int_max> PointIDList;  // Length = Length of PointList
    DenseVector<int_max> VertexIDList; // Length = Length of VertexList
    DenseVector<int_max> EdgeIDList;   // Length = Length of EdgeList
    DenseVector<int_max> CellIDList;   // Length = Length of CellList

    std::unordered_map<int_max, int_max> Map_PointID_to_PointIndex;
    std::unordered_map<int_max, int_max> Map_VertexID_to_VertexIndex;
    std::unordered_map<int_max, int_max> Map_EdgeID_to_EdgeIndex;
    std::unordered_map<int_max, int_max> Map_CellID_to_CellIndex;

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

    DataArray<Cell_Of_SurfaceMesh<ScalarType>> CellList; // also known as face, facet, element

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
    inline int_max GetPointNumber() const;
    inline int_max GetVertexNumber() const;
    inline int_max GetCellNumber() const;
    inline int_max GetEdgeNumber() const;

    // 3D Position -----------------------------------------------------------

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const;

    inline SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, const DenseMatrix<ScalarType> PointPositionMatrix) const;

    //----- Get/Set  Mesh Item {Point, Vertex, Edge, DirectedEdge, Cell} ------//

    inline Point_Of_SurfaceMesh<ScalarType>& Point(Handle_Of_Point_Of_SurfaceMesh PointHandle);
    inline const Point_Of_SurfaceMesh<ScalarType>& Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const;

    inline Vertex_Of_SurfaceMesh<ScalarType>& Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle);
    inline const Vertex_Of_SurfaceMesh<ScalarType>& Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const;

    inline Edge_Of_SurfaceMesh<ScalarType>& Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle);
    inline const Edge_Of_SurfaceMesh<ScalarType>& Edge(andle_Of_Edge_Of_SurfaceMesh EdgeHandle) const;

    inline DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle);
    inline const DirectedEdge_Of_SurfaceMesh<ScalarType>& DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const;

    inline Cell_Of_SurfaceMesh<ScalarType>& Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle);
    inline const Cell_Of_SurfaceMesh<ScalarType>& Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const;

    // get handle -----------------------------------------------------------------------------//

    inline Handle_Of_Point_Of_SurfaceMesh  GetPointHandle(int_max PointID) const;
    inline Handle_Of_Vertex_Of_SurfaceMesh GetVertexHandle(int_max VertexID) const;
    inline Handle_Of_Edge_Of_SurfaceMesh   GetEdgeHandle(int_max EdgeID) const;
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const;
    inline Handle_Of_Cell_Of_SurfaceMesh   GetCellHandle(int_max CellID) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointHandle -> PointIndex in m_MeshData->PointList
    Handle_Of_Point_Of_SurfaceMesh AddPoint(ScalarType Position[3]);
    Handle_Of_Point_Of_SurfaceMesh AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointHandleList
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> AddPoint(const DenseMatrix<ScalarType>& PointSet);

private:
    // add a Vertex and return VertexHandle -> VertexIndex in m_MeshData->VertexList
    // called in AddEdge() if necessary, so set to private
    int_max AddVertex(int_max PointIndex);

public:
    // add an Edge and return EdgeIndex in m_MeshData->EdgeList
    // also create invalid DirectedEdge to hold place at  m_MeshData->DirectedEdgeList[EdgeIndex]
    // PointHandleList[0] and PointHandleList[1] are two vertex of the Edge
    // new vertex is added if necessary, so AddVertex is called in this function if necessary
    Handle_Of_Edge_Of_SurfaceMesh AddEdge(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList);

    // add a cell and return CellIndex in m_MeshData->CellList
    // add DirectedEdge of the cell
    // the order of Edge in EdgeHandleList determine the order of DirectedEdge
    Handle_Of_Cell_Of_SurfaceMesh AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList);

    // Delete Mesh Item ----------------------------------------------------------------------------//

    // m_MeshData->CellList(CellIndex).Clear() only clear memory
    // this function will remove/modify any mesh item related to the cell, including DirectedEdge of the Cell
    bool DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only clear memory
    // this function will remove mesh item related to the Edge
    // If the edge is adjacent to a cell, then the Edge can not be deleted before the cell is deleted
    bool DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle);

private:
    // m_MeshData->VertexList[VertexIndex].Cear() only clear memory
    // this function will remove mesh item related to the Vertex
    // a vertex will be removed if it has no adjacent edge
    // so this function is called in DeleteEdge() if necessary, and set to private
    bool DeleteVertex(int_max VertexIndex);

public:
    // m_MeshData->PointList[PointIndex].Cear() only clear memory
    // If the Point is on an edge, then the Point can not be deleted before the Edge is deleted
    bool DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle);
    bool DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList);

    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after this function is called, every index and every handle will be changed
    //            but, ID will not change
    void CleanDataStructure();
    //---------------------------------------------------------------------------------------------------

};

}// namespace mdk

#include "mdkSurfaceMesh.hpp"

#endif