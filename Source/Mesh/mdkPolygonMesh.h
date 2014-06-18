#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h

#include "mdkPolygonMeshItem.h"

namespace mdk
{
//------------- forward declare -----------//
template<typename ScalarType>
struct PolygonMeshAttribute;
//----------------------------------------//

template<typename ScalarType>
struct PolygonMeshData
{
    bool IsTriangleMesh;
    // true: triangle mesh
    // false: not triangle mesh

    DenseVector<int_max> VertexGlobalIDList; // Length = Length of VertexList

    DenseVector<int_max> PolygonGlobalIDList; // Length = Length of PolygonList

    DenseMatrix<ScalarType> VertexPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // VertexPositionTable(ALL, k) is the 3D position of the vertex k
    // ColNumber = Length of VertexList

    DataArray<DenseVector<int_max>> VertexIndexTable;
    // VertexIndexTable[PolygonIndex] is VertexIndexList of Polygon (PolygonIndex)
    // VertexIndex_0 -> VertexIndex_1 -> ... -> VerteIndex_end determin the direction of normal
    // index order in each VertexIndexList should be consistent

    DataArray<Vertex_Of_PolygonMesh<ScalarType>> VertexList;

    DenseVector<int_max> VertexValidityFlagList;
    // 1: vertex is an element of the mesh 
    // 0: vertex is deleted

    DataArray<Polygon_Of_PolygonMesh<ScalarType>> PolygonList; // also known as cell, face, facet, element
    DenseVector<int_max> PolygonValidityFlagList;
    // 1: Polygon is an element of the mesh 
    // 0: Polygon is deleted

    DataArray<Edge_Of_PolygonMesh<ScalarType>> EdgeList;
    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

    DataArray<HalfEdge_Of_PolygonMesh<ScalarType>> HalfEdgeList;
    DenseVector<int_max> HalfEdgeValidityFlagList;
    // 1: HalfEdge is an element of the mesh 
    // 0: HalfEdge is deleted

    //---------------- Mesh Attribute -----------------------------//

    PolygonMeshAttribute<ScalarType> MeshAttribute;
};

template<typename ScalarType>
struct PolygonMeshAttribute
{
    //-------------------- Normal Vector ---------------------------------

    DenseMatrix<ScalarType> NormalAtVertex; // size: 3 x Length of VertexList

    DenseMatrix<ScalarType> NormalAtPolygon; // size: 3 x Length of PolygonList 
};

template<typename ScalarType = double>
class PolygonMesh : public Object
{
protected:
    std::shared_ptr<PolygonMeshData<ScalarType>> m_MeshData;

public:
    template<typename T>
    friend class Vertex_Of_PolygonMesh;

    template<typename T>
    friend class Edge_Of_PolygonMesh;

    template<typename T>
    friend class HalfEdge_Of_PolygonMesh;

    template<typename T>
    friend class Polygon_Of_PolygonMesh;

public:
    typedef ScalarType ScalarType;
    typedef int_max IndexType;

public:
    PolygonMesh();
    PolygonMesh(const Pure_Empty_PolygonMesh_Symbol&);
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //---------------------------------------------------------------------------

    bool Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable);
    // index order in each VertexIndexList should be consistent

    bool CheckIfTriangleMesh() const;

    //---------------------------------------------------------------------------
    // attention: after this function is called, every index will be changed
    // and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->VertexList)
    
    void CleanDataStructure();

    //---------------------------------------------------------------------------

    inline void Clear();

    template<typename ScalarType_Input>
    inline void Copy(const PolygonMesh<ScalarType_Input>& InputMesh);

    template<typename ScalarType_Input>
    inline bool Copy(const PolygonMesh<ScalarType_Input>* InputMesh);

    inline void Share(PolygonMesh& InputMesh);
    inline bool Share(PolygonMesh* InputMesh);

    inline void ForceShare(const PolygonMesh& InputMesh);
    inline bool ForceShare(const PolygonMesh* InputMesh);

    inline void Take(PolygonMesh&& InputMesh);
    inline bool Take(PolygonMesh& InputMesh);
    inline bool Take(PolygonMesh* InputMesh);

    //-------------------------------------------------------------------

    inline bool IsEmpty() const;
    inline int_max GetVertexNumber() const;
    inline int_max GetPolygonNumber() const;
    inline int_max GetEdgeNumber() const;

    // GlobalID -------------------------------------------------------------

    inline DenseVector<int_max>& VertexGlobalIDList();
    inline const DenseVector<int_max>& VertexGlobalIDList() const;

    inline DenseVector<int_max>& PolygonGlobalIDList();
    inline const DenseVector<int_max>& PolygonGlobalIDList() const;

    // 3D Position -----------------------------------------------------------

    inline DenseMatrix<ScalarType>& VertexPositionTable();
    inline const DenseMatrix<ScalarType>& VertexPositionTable() const;

    // mesh item --------------------------------------------------------------

    inline DataArray<DenseVector<int_max>>& VertexIndexTable();
    inline const DataArray<DenseVector<int_max>>& VertexIndexTable() const;

    inline DataArray<Vertex_Of_PolygonMesh<ScalarType>>& VertexList();
    inline const DataArray<Vertex_Of_PolygonMesh<ScalarType>>& VertexList() const;

    inline DenseVector<int_max>& VertexValidityFlagList();
    inline const DenseVector<int_max>& VertexValidityFlagList() const;

    inline DataArray<Edge_Of_PolygonMesh<ScalarType>>& EdgeList();
    inline const DataArray<Edge_Of_PolygonMesh<ScalarType>>& EdgeList() const;

    inline DenseVector<int_max>& EdgeValidityFlagList();
    inline const DenseVector<int_max>& EdgeValidityFlagList() const;

    inline DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& HalfEdgeList();
    inline const DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& HalfEdgeList() const;

    inline DenseVector<int_max>& HalfEdgeValidityFlagList();
    inline const DenseVector<int_max>& HalfEdgeValidityFlagList() const;

    inline DataArray<Polygon_Of_PolygonMesh<ScalarType>>& PolygonList();
    inline const DataArray<Polygon_Of_PolygonMesh<ScalarType>>& PolygonList() const;

    inline DenseVector<int_max>& PolygonValidityFlagList();
    inline const DenseVector<int_max>& PolygonValidityFlagList() const;

    // Attribute -------------------------------------------------------

    inline DenseMatrix<ScalarType>& NormalAtVertex();
    inline const DenseMatrix<ScalarType>& NormalAtVertex() const;

    inline DenseMatrix<ScalarType>& NormalAtPolygon();
    inline const DenseMatrix<ScalarType>& NormalAtPolygon() const;

    //-----------------------------------------------------------------------

    void UpdateNormalAtVertex();
    void UpdateNormalAtPolygon();
    void UpdateAttribute();

private:
    DataArray<DenseVector<int_max>> ConstructTempHalfEdgeList();
    bool ConstructPolygonList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList);
    bool ConstructEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList);
    bool ConstructHalfEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList);
    bool ConstructVertexList();
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif