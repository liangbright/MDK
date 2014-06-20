#ifndef __mdkSurfaceMesh_h
#define __mdkSurfaceMesh_h

#include <unordered_map>

#include "mdkSurfaceMeshItem.h"
#include "mdkSurfaceMeshIterator.h"

namespace mdk
{

template<typename MeshAttributeType>
struct SurfaceMeshData
{
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType  ScalarType;
    //-------------------------------------------------------------------------------------------//

    // Attention: ID must >= 0
    DenseVector<int_max> PointIDList;  // Length = Length of PointList
    DenseVector<int_max> EdgeIDList;   // Length = Length of EdgeList
    DenseVector<int_max> CellIDList;   // Length = Length of CellList

    std::unordered_map<int_max, int_max> Map_PointID_to_PointIndex;
    std::unordered_map<int_max, int_max> Map_EdgeID_to_EdgeIndex;
    std::unordered_map<int_max, int_max> Map_CellID_to_CellIndex;

    DenseMatrix<ScalarType> PointPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // a point is a vertex of an Edge or a point on Edge Edge

    DataArray<Point_Of_SurfaceMesh<MeshAttributeType>> PointList;

    DenseVector<int_max> PointValidityFlagList;
    // 1: point is an element of the mesh 
    // 0: point is deleted

    DataArray<Edge_Of_SurfaceMesh<MeshAttributeType>> EdgeList;

    DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2>> DirectedEdgePairList;

    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

    DataArray<Cell_Of_SurfaceMesh<MeshAttributeType>> CellList; // also known as face, facet, element

    DenseVector<int_max>  CellValidityFlagList;
    // 1: Cell is an element of the mesh 
    // 0: Cell is deleted
};


template<typename MeshAttributeType>
class SurfaceMesh : public Object
{
public:
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType                ScalarType;
    typedef typename MeshAttributeType::PointAttributeType        PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType         EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType         CellAttributeType;
    //-------------------------------------------------------------------------------------------//
    typedef int_max IndexType;
    //--------------------------------------------------------------------------------------------//
    typedef Point_Of_SurfaceMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_SurfaceMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>      PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>       EdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>       CellIteratorType;
    //--------------------------------------------------------------------------------------------------//
private:
    std::shared_ptr<SurfaceMeshData<MeshAttributeType>> m_MeshData;

private:
    template<typename T>
    friend class Point_Of_SurfaceMesh;

    template<typename T>
    friend struct Iterator_Of_Point_Of_SurfaceMesh;

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
    inline int_max GetEdgeNumber() const;
    inline int_max GetDirectedEdgeNumber() const;
    inline int_max GetCellNumber() const;
 
    // 3D Position ---------------------------------------------------------------------------//

    inline void SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const;
    inline void GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<PointHandleType>& PointHandleList) const;

    //----- Get/Set Mesh Item {Point, Vertex, Edge, DirectedEdge, Cell} by using Handle ------//

    inline PointType& Point(PointHandleType PointHandle);
    inline const PointType& Point(PointHandleType PointHandle) const;

    inline EdgeType& Edge(EdgeHandleType EdgeHandle);
    inline const EdgeType& Edge(EdgeHandleType EdgeHandle) const;

    inline DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle);
    inline const DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const;

    inline CellType& Cell(CellHandleType CellHandle);
    inline const CellType& Cell(CellHandleType CellHandle) const;

    //--------- get HandleList ------------------------------------------------------------//

    inline DenseVector<PointHandleType> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const;

    inline DenseVector<EdgeHandleType> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const;

    inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const;

    inline DenseVector<CellHandleType> GetCellHandleList() const;
    inline void GetCellHandleList(DenseVector<CellHandleType>& OutputHandleList) const;

    //------------- Iterator --------------------------------------------------------------//

    inline PointIteratorType  GetIteratorOfPoint() const;
    inline EdgeIteratorType   GetIteratorOfEdge() const;
    inline CellIteratorType   GetIteratorOfCell() const;

    // use this function and GetIteratorOfEdge as DirectedEdge Iterator
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(EdgeHandleType EdgeHandle, int_max RelativeIndex) const;

    //----------- get handle by ID -----------------------------------------------------------//
    inline PointHandleType  GetPointHandle(int_max PointID) const;
    inline EdgeHandleType   GetEdgeHandle(int_max EdgeID) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const;
    inline CellHandleType   GetCellHandle(int_max CellID) const;

    //-------------- check handle -------------------------------------------------------//
    bool IsValidHandle(PointHandleType PointHandle) const;
    bool IsValidHandle(EdgeHandleType EdgeHandle) const;
    bool IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    bool IsValidHandle(CellHandleType CellHandle) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointHandle -> PointIndex in m_MeshData->PointList
    PointHandleType AddPoint(const ScalarType Position[3]);
    PointHandleType AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointHandleList
    DenseVector<PointHandleType> AddPoint(const DenseMatrix<ScalarType>& PointSet);

    // add an Edge and return EdgeIndex in m_MeshData->EdgeList
    // also create invalid DirectedEdge to hold place at  m_MeshData->DirectedEdgeList[EdgeIndex]
    // PointHandleList[0] and PointHandleList[1] are two vertex of the Edge
    // new vertex is added if necessary, so AddVertex is called in this function if necessary
    EdgeHandleType AddEdge(const DenseVector<PointHandleType>& PointHandleList);

    // add a cell and return CellIndex in m_MeshData->CellList
    // add DirectedEdge of the cell
    // the order of Edge in EdgeHandleList determine the order of DirectedEdge
    CellHandleType AddCell(const DenseVector<EdgeHandleType>& EdgeHandleList);

    // Delete Mesh Item ----------------------------------------------------------------------------//

    // m_MeshData->CellList(CellIndex).Clear() only clear memory
    // this function will remove/modify any mesh item related to the cell, including DirectedEdge of the Cell
    bool DeleteCell(CellHandleType CellHandle);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only clear memory
    // this function will remove mesh item related to the Edge
    // If the edge is adjacent to a cell, then the Edge can not be deleted before the cell is deleted
    bool DeleteEdge(EdgeHandleType EdgeHandle);

    // m_MeshData->PointList[PointIndex].Cear() only clear memory
    // If the Point is on an edge, then the Point can not be deleted before the Edge is deleted
    bool DeletePoint(PointHandleType PointHandle);
    bool DeletePoint(const DenseVector<PointHandleType>& PointHandleList);

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