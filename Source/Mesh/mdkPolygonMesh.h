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

    DataArray<Polygon_Of_PolygonMesh<ScalarType>> PolygonList; // also known as cell, face, facet, element
    
    DataArray<Edge_Of_PolygonMesh<ScalarType>> EdgeList;

    DataArray<HalfEdge_Of_PolygonMesh<ScalarType>> HalfEdgeList;

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
    typedef ScalarType ScalarType;
    typedef int_max IndexType;

public:
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //---------------------------------------------------------------------------

    bool Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable);
    // index order in each VertexIndexList should be consistent

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

    //-----------------------------------------------------------------------

    void UpdateNormalAtVertex();
    void UpdateNormalAtPolygon();

    //-------------------------------------------------------------------

    void UpdateAttribute();

    //-------------------------------------------------------------------

    inline const DenseVector<int_max>& GetVertexGlobalIDList() const;
    inline const DenseVector<int_max>& GetPolygonGlobalIDList() const;

    inline const DenseMatrix<ScalarType>& GetVertexPositionTable() const;
    inline const DataArray<DenseVector<int_max>>& GetVertexIndexTable() const;

    //----------------------------------------------------------------------------

    inline DenseMatrix<ScalarType>& NormalAtVertex();
    inline const DenseMatrix<ScalarType>& NormalAtVertex() const;

    inline DenseMatrix<ScalarType>& NormalAtPolygon();
    inline const DenseMatrix<ScalarType>& NormalAtPolygon() const;

private:
    void ConstructTempHalfEdgeList(DataArray<DenseVector<int_max>>& TempHalfEdgeList, DataArray<DenseVector<int_max>>& HalfEdgeIndexListOfEachPolygon);
    bool ConstructPolygonList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList, const DataArray<DenseVector<int_max>>& HalfEdgeIndexListOfEachPolygon);
    bool ConstructEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList);
    bool ConstructHalfEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList);
    bool ConstructVertexList();
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif