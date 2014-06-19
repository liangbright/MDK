#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h

#include "mdkSurfaceMesh.h"
#include "mdkPolygonMeshAttribute.h"

namespace mdk
{
//------- forward declare -------//
template<typename ScalarType>
class PolygonMesh;
//--------------------------------//

template<typename ScalarType>
struct PolygonMeshData
{
    bool IsTriangleMesh;

    SurfaceMesh<PolygonMesh<ScalarType>>  PolygonSurfaceMesh;
};

template<typename ScalarType>
class PolygonMesh : public Object
{
protected:
    std::shared_ptr<PolygonMeshData<ScalarType>> m_MeshData;

public:
    typedef PolygonMesh<ScalarType> MeshType;

    typedef ScalarType ScalarType;
    typedef int_max IndexType;

    typedef bool PointAttributeType;

    typedef VertexAttribute_Of_PolygonMesh<ScalarType>         VertexAttributeType;
    typedef EdgeAttribute_Of_PolygonMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_PolygonMesh<ScalarType>           CellAttributeType;

    typedef Vertex_Of_SurfaceMesh<MeshType>          VertexType;
    typedef Edge_Of_SurfaceMesh<MeshType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshType>            CellType;

    typedef Handle_Of_Vertex_Of_SurfaceMesh         VertexHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Vertex_Of_SurfaceMesh<MeshType>     VertexIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshType>       EdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshType>       CellIteratorType;

public:
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const PolygonMesh<ScalarType>& InputMesh);

    inline bool Copy(const PolygonMesh<ScalarType>* InputMesh);

    inline void Share(PolygonMesh& InputMesh);
    inline bool Share(PolygonMesh* InputMesh);

    inline void ForceShare(const PolygonMesh& InputMesh);
    inline bool ForceShare(const PolygonMesh* InputMesh);

    inline void Take(PolygonMesh&& InputMesh);
    inline bool Take(PolygonMesh& InputMesh);
    inline bool Take(PolygonMesh* InputMesh);

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
    inline int_max GetPointNumber() const;
    inline int_max GetVertexNumber() const;
    inline int_max GetCellNumber() const;
    inline int_max GetEdgeNumber() const;

    // 3D Position -----------------------------------------------------------

    inline void SetVertexPosition(const DenseVector<VertexHandleType>& VertexHandleList, const DenseMatrix<ScalarType>& VertexPositionMatrix);

    inline DenseMatrix<ScalarType> GetVertexPosition(const DenseVector<VertexHandleType>& VertexHandleList) const;
    inline void GetVertexPosition(DenseMatrix<ScalarType>& VertexPositionMatrix, const DenseVector<VertexHandleType>& VertexHandleList) const;

    //----- Get/Set  Mesh Item {Point, Vertex, Edge, DirectedEdge, Cell} ------//

    inline VertexType& Vertex(VertexHandleType VertexHandle);
    inline const VertexType& Vertex(VertexHandleType VertexHandle) const;

    inline EdgeType& Edge(EdgeHandleType EdgeHandle);
    inline const EdgeType& Edge(EdgeHandleType EdgeHandle) const;

    inline DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle);
    inline const DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const;

    inline CellType& Cell(CellHandleType CellHandle);
    inline const CellType& Cell(CellHandleType CellHandle) const;

    //------------- Iterator --------------------------------------------------------------//

    inline VertexIteratorType GetIteratorOfVertex() const;
    inline EdgeIteratorType   GetIteratorOfEdge() const;
    inline CellIteratorType   GetIteratorOfCell() const;

    // use this function and GetIteratorOfEdge as DirectedEdge Iterator
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(EdgeHandleType EdgeHandle, int_max RelativeIndex) const;

    // get handle by ID -----------------------------------------------------------------------//

    inline VertexHandleType GetVertexHandle(int_max VertexID) const;
    inline EdgeHandleType   GetEdgeHandle(int_max EdgeID) const;
    inline DirectedEdgeHandleType GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const;
    inline CellHandleType   GetCellHandle(int_max CellID) const;

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    VertexHandleType AddVertex(ScalarType Position[3]);
    VertexHandleType AddVertex(ScalarType x, ScalarType y, ScalarType z);
    DenseVector<VertexHandleType> AddVertex(const DenseMatrix<ScalarType>& PointSet);

    EdgeHandleType AddEdge(VertexHandleType VertexHandle0, VertexHandleType VertexHandle1);
    CellHandleType AddCell(const DenseVector<EdgeHandleType>& EdgeHandleList);

    //Delete Mesh Item ----------------------------------------------------------------------//

    bool DeleteCell(CellHandleType CellHandle);
    bool DeleteEdge(EdgeHandleType EdgeHandle);

    bool DeleteVertex(VertexHandleType VertexHandle);
    bool DeleteVertex(const DenseVector<VertexHandleType>& VertexHandleList);

    //---------------------------------------------------------------------------//
    // attention: after this function is called, every index and handle will be changed
    // and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->VertexList)
    void CleanDataStructure();

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputVertexPositionTable, const DataArray<DenseVector<int_max>>& InputCellTable);
    // index order in each VertexIndexList should be consistent

    bool Construct(SurfaceMesh<PolygonMesh<ScalarType>> InputSurfaceMesh);

    bool CheckIfTriangleMesh() const;

    //--------------------- output -------------------------------------------------//

    void GetVertexPositionTableAndCellTable(DenseMatrix<ScalarType>& VertexPositionTable, DataArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//

    void UpdateNormalAtVertex();
    void UpdateNormalAtCell();
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif