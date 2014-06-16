#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h


#include "mdkPolygonMesh.h"

namespace mdk
{

template<typename ScalarType = double>
class TriangleMesh : private PolygonMesh<ScalarType>
{
public:
    typedef ScalarType ScalarType;

    typedef int_max IndexType;

public:
    TriangleMesh();

    TriangleMesh(const TriangleMesh& InputMesh);

    TriangleMesh(TriangleMesh&& InputMesh);

    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);

    inline void operator=(TriangleMesh&& InputMesh);

    //---------------------------------------------------------------------------
    bool Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable);
    //---------------------------------------------------------------------------
    void CleanDataStructure();
    //---------------------------------------------------------------------------

    inline void Clear();

    template<typename ScalarType_Input>
    inline void Copy(const TriangleMesh<ScalarType_Input>& InputMesh);

    template<typename ScalarType_Input>
    inline bool Copy(const TriangleMesh<ScalarType_Input>* InputMesh);

    inline void Share(TriangleMesh& InputMesh);

    inline bool Share(TriangleMesh* InputMesh);

    inline void ForceShare(const TriangleMesh& InputMesh);

    inline bool ForceShare(const TriangleMesh* InputMesh);

    inline void Take(TriangleMesh&& InputMesh);

    inline bool Take(TriangleMesh& InputMesh);

    inline bool Take(TriangleMesh* InputMesh);

    //-------------------------------------------------------------------

    inline bool IsEmpty() const;
    inline int_max GetVertexNumber() const;
    inline int_max GetTriangleNumber() const;
    inline int_max GetEdgeNumber() const;

    // GlobalID -------------------------------------------------------------

    inline DenseVector<int_max>& VertexGlobalIDList();
    inline const DenseVector<int_max>& VertexGlobalIDList() const;

    inline DenseVector<int_max>& TriangleGlobalIDList();
    inline const DenseVector<int_max>& TriangleGlobalIDList() const;

    // 3D Position -----------------------------------------------------------

    inline DenseMatrix<ScalarType>& VertexPositionTable();
    inline const DenseMatrix<ScalarType>& VertexPositionTable() const;

    // mesh item --------------------------------------------------------------

    inline DataArray<DenseVector<int_max>>& VertexIndexTable();
    inline const DataArray<DenseVector<int_max>>& VertexIndexTable() const;

    inline DataArray<Vertex_Of_PolygonMesh<ScalarType>>& VertexList();
    inline const DataArray<Vertex_Of_PolygonMesh<ScalarType>>& VertexList() const;

    inline DataArray<Edge_Of_PolygonMesh<ScalarType>>& EdgeList();
    inline const DataArray<Edge_Of_PolygonMesh<ScalarType>>& EdgeList() const;

    inline DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& HalfEdgeList();
    inline const DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& HalfEdgeList() const;

    inline DataArray<Polygon_Of_PolygonMesh<ScalarType>>& TriangleList();
    inline const DataArray<Polygon_Of_PolygonMesh<ScalarType>>& TriangleList() const;

    // Attribute -------------------------------------------------------

    inline DenseMatrix<ScalarType>& NormalAtVertex();
    inline const DenseMatrix<ScalarType>& NormalAtVertex() const;

    inline DenseMatrix<ScalarType>& NormalAtTriangle();
    inline const DenseMatrix<ScalarType>& NormalAtTriangle() const;

    //-----------------------------------------------------------------------

    void UpdateNormalAtVertex();
    void UpdateNormalAtTriangle();
    void UpdateAttribute();
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif