#ifndef __mdkSurfaceMesh_h
#define __mdkSurfaceMesh_h

#include <unordered_map>

#include "mdkSurfaceMeshItem.h"
#include "mdkSurfaceMeshIterator.h"

namespace mdk
{

template<typename MeshType>
struct SurfaceMeshData
{
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshType::ScalarType  ScalarType;
    //-------------------------------------------------------------------------------------------//

    // Attention: ID must >= 0
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

    DataArray<Point_Of_SurfaceMesh<MeshType>> PointList;

    DenseVector<int_max> PointValidityFlagList;
    // 1: point is an element of the mesh 
    // 0: point is deleted

    DataArray<Vertex_Of_SurfaceMesh<MeshType>> VertexList;

    DenseVector<int_max> VertexValidityFlagList;
    // 1: vertex is an element of the mesh 
    // 0: vertex is deleted

    DataArray<Edge_Of_SurfaceMesh<MeshType>> EdgeList;

    DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<MeshType>, 2>> DirectedEdgePairList;

    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

    DataArray<Cell_Of_SurfaceMesh<MeshType>> CellList; // also known as face, facet, element

    DenseVector<int_max>  CellValidityFlagList;
    // 1: Cell is an element of the mesh 
    // 0: Cell is deleted
};


template<typename MeshType>
class SurfaceMesh : public Object
{
public:
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshType::ScalarType                ScalarType;
    typedef typename MeshType::PointAttributeType        PointAttributeType;
    typedef typename MeshType::VertexAttributeType       VertexAttributeType;
    typedef typename MeshType::EdgeAttributeType         EdgeAttributeType;
    typedef typename MeshType::DirectedEdgeAttributeType DirectedEdgeAttributeType;
    typedef typename MeshType::CellAttributeType         CellAttributeType;
    //-------------------------------------------------------------------------------------------//
    typedef int_max IndexType;

private:
    std::shared_ptr<SurfaceMeshData<MeshType>> m_MeshData;

private:
    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Point_Of_SurfaceMesh;

    template<typename T>
    friend class Vertex_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Vertex_Of_SurfaceMesh;

    template<typename T>
    friend class Edge_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Edge_Of_SurfaceMesh;

    template<typename T>
    friend class DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_DirectedEdge_Of_SurfaceMesh;

    template<typename T>
    friend class Cell_Of_SurfaceMesh;    

    template<typename T>
    friend struct Iterator_Of_Cell_Of_SurfaceMesh;

public:
    SurfaceMesh();
    SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&);
    SurfaceMesh(const SurfaceMesh& InputMesh);
    SurfaceMesh(SurfaceMesh&& InputMesh);
    ~SurfaceMesh();

    inline void operator=(const SurfaceMesh& InputMesh);
    inline void operator=(SurfaceMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const SurfaceMesh& InputMesh);
    inline bool Copy(const SurfaceMesh* InputMesh);

    inline void Share(SurfaceMesh& InputMesh);
    inline bool Share(SurfaceMesh* InputMesh);

    inline void ForceShare(const SurfaceMesh& InputMesh);
    inline bool ForceShare(const SurfaceMesh* InputMesh);

    inline void Take(SurfaceMesh&& InputMesh);
    inline void Take(SurfaceMesh& InputMesh);
    inline bool Take(SurfaceMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetPointNumber() const;
    inline int_max GetVertexNumber() const;
    inline int_max GetCellNumber() const;
    inline int_max GetEdgeNumber() const;

    // 3D Position -----------------------------------------------------------

    inline void SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const;

    inline void SetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList, const DenseMatrix<ScalarType>& VertexPositionMatrix);

    inline DenseMatrix<ScalarType> GetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const;
    inline void GetVertexPosition(DenseMatrix<ScalarType>& VertexPositionMatrix, const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const;

    //----- Get/Set  Mesh Item {Point, Vertex, Edge, DirectedEdge, Cell} ------//

    inline Point_Of_SurfaceMesh<MeshType>& Point(Handle_Of_Point_Of_SurfaceMesh PointHandle);
    inline const Point_Of_SurfaceMesh<MeshType>& Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const;

    inline Vertex_Of_SurfaceMesh<MeshType>& Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle);
    inline const Vertex_Of_SurfaceMesh<MeshType>& Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const;

    inline Edge_Of_SurfaceMesh<MeshType>& Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle);
    inline const Edge_Of_SurfaceMesh<MeshType>& Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const;

    inline DirectedEdge_Of_SurfaceMesh<MeshType>& DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle);
    inline const DirectedEdge_Of_SurfaceMesh<MeshType>& DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const;

    inline Cell_Of_SurfaceMesh<MeshType>& Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle);
    inline const Cell_Of_SurfaceMesh<MeshType>& Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const;

    //------------- Iterator --------------------------------------------------------------//

    inline Iterator_Of_Point_Of_SurfaceMesh<MeshType>  GetIteratorOfPoint() const;
    inline Iterator_Of_Vertex_Of_SurfaceMesh<MeshType> GetIteratorOfVertex() const;
    inline Iterator_Of_Edge_Of_SurfaceMesh<MeshType>   GetIteratorOfEdge() const;
    inline Iterator_Of_Cell_Of_SurfaceMesh<MeshType>   GetIteratorOfCell() const;

    // use this function and GetIteratorOfEdge as DirectedEdge Iterator
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetDirectedEdgeHandle(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle, int_max RelativeIndex) const;

    // get handle by ID -----------------------------------------------------------------------//

    inline Handle_Of_Point_Of_SurfaceMesh  GetPointHandle(int_max PointID) const;
    inline Handle_Of_Vertex_Of_SurfaceMesh GetVertexHandle(int_max VertexID) const;
    inline Handle_Of_Edge_Of_SurfaceMesh   GetEdgeHandle(int_max EdgeID) const;
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const;
    inline Handle_Of_Cell_Of_SurfaceMesh   GetCellHandle(int_max CellID) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointHandle -> PointIndex in m_MeshData->PointList
    Handle_Of_Point_Of_SurfaceMesh AddPoint(const ScalarType Position[3]);
    Handle_Of_Point_Of_SurfaceMesh AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointHandleList
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> AddPoint(const DenseMatrix<ScalarType>& PointSet);

    // add a Vertex and return VertexHandle -> VertexIndex in m_MeshData->VertexList
    // called in AddEdge() if necessary
    Handle_Of_Vertex_Of_SurfaceMesh AddVertex(Handle_Of_Point_Of_SurfaceMesh PointHandle);

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

    // m_MeshData->VertexList[VertexIndex].Cear() only clear memory
    // this function will remove mesh item related to the Vertex
    // a vertex will be removed if it has no adjacent edge
    // so this function is called in DeleteEdge() if necessary
    bool DeleteVertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle);

    // m_MeshData->PointList[PointIndex].Cear() only clear memory
    // If the Point is on an edge, then the Point can not be deleted before the Edge is deleted
    bool DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle);
    bool DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList);

    /*
    //Signal to derived class After Add or Delete -----------------------------------------------------//
protected:
    virtual inline bool AddPoint_Postprocessing(int_max PointIndex);
    virtual inline bool AddVertex_Postprocessing(int_max VertexIndex);
    virtual inline bool AddEdge_Postprocessing(int_max EdgeIndex);
    virtual inline bool AddCell_Postprocessing(int_max CellIndex);
    virtual inline bool AddDirectedEdge_Postprocessing(int_max CellIndex);

    virtual inline bool DeletePoint_Postprocessing(int_max PointIndex);
    virtual inline bool DeleteVertex_Postprocessing(int_max VertexIndex);
    virtual inline bool DeleteEdge_Postprocessing(int_max EdgeIndex);
    virtual inline bool DeleteCell_Postprocessing(int_max CellIndex);
    virtual inline bool DeleteDirectedEdge_Postprocessing(int_max CellIndex);
    */

public:
    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after this function is called, every index and every handle will be changed
    //            but, ID will not change
    void CleanDataStructure();
    //---------------------------------------------------------------------------------------------------

};

}// namespace mdk

#include "mdkSurfaceMesh.hpp"

#endif