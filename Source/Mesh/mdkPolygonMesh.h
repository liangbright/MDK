#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h


#include "mdkObject.h"
#include "mdkDataContainer.h"
#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename ScalarType = double>
struct PolygonMeshData
{
    DenseMatrix<int_max> VertexGlobalIndexList;

    DenseMatrix<int_max> PolygonGlobalIndexList;

    DenseMatrix<ScalarType> Vertex;    
    // row_0: x
    // row_1: y
    // row_2: z

    DataContainer<DenseVector<int_max>> Polygon; // also known as cell, face, facet, element
    //
    // VertexIndex_0 -> VertexIndex_1 -> ... -> VerteIndex_end determin the direction of normal

    DenseMatrix<int_max> Edge;
    // row_0: VertexIndex_0
    // row_1: VertexIndex_1
    // row_2: PolygonIndex_0  
    // row_3: PolygonIndex_1  
    //
    // Edge(:,k) = VertexIndex_0 <-> VertexIndex_1
    //
    // Edge(:,k) is shared by PolygonIndex_0 and PolygonIndex_1
    //
    // if PolygonIndex_0 is -1, then Edge(:,k) is part of boundary
    //
    // VertexIndex_0    < VertexIndex_1   always
    // PolygonIndex_0  < PolygonIndex_1 always

    DataContainer<DenseVector<int_max>> Link_VertexToEdge;
    // Link_VertexToEdge[k] is { EdgeIndex_0, EdgeIndex_1, EdgeIndex_2, ...}, the length is not a constant
    // share the same Vertex (VertexIndex is k) 

    DataContainer<DenseVector<int_max>> Link_VertexToPolygon;
    // Link_VertexToPolygon[k] is { PolygonIndex_0, PolygonIndex_1, PolygonIndex_2, ...}, the length is not a constant
    // share the same Vertex (VertexIndex is k) 

    // DataContainer<DenseMatrix<int_max>> Link_EdgeToPolygon;
    // This link is directly stored in Edge

    DataContainer<DenseVector<int_max>> Link_PolygonToEdge;
    // Link_PolygonToEdge[k] is {EdgeIndex_0, EdgeIndex_1, EdgeIndex_2, ..., EdgeIndex_end}, the length is not a constant
    // Polygon(:,k) is {VertexIndex_0, VertexIndex_1, VertexIndex_2}    
    // Edge of EdgeIndex_0 : VertexIndex_0 <-> VertexIndex_1
    // Edge of EdgeIndex_1 : VertexIndex_1 <-> VertexIndex_2
    // Edge of EdgeIndex_2 : VertexIndex_2 <-> VertexIndex_3
    // ...
    // Edge of EdgeIndex_end : VertexIndex_0 <-> VertexIndex_end

    //---------------------- Adjacency ----------------------------------

    DataContainer<DenseVector<int_max>> Adjacency_VertexToVertex;
    // Adjacency_VertexToVertex[k] is { VertexIndex_0, VertexIndex_1, VertexIndex_2, ...}
    // share the same neighbour Vertex (VertexIndex is k) 

    //DataContainer<DenseVector<int_max>> Adjacency_EdgeToEdge_0;
    // Adjacency_EdgeToEdge_0[k] is Link_VertexToEdge[VertexIndex_0] without k
    // Edge(:,k)  is {VertexIndex_0, VertexIndex_1}

    //DataContainer<DenseVector<int_max>> Adjacency_EdgeToEdge_1;
    // Adjacency_EdgeToEdge_0[k] is Link_VertexToEdge[VertexIndex_1] without k
    // Edge(:,k)  is {VertexIndex_0, VertexIndex_1}

    DataContainer<DenseVector<int_max>> Adjacency_PolygonToPolygon;
    // Adjacency_PolygonToPolygon[k] is { PolygonIndex_0, PolygonIndex_1, PolygonIndex_2, ...}
    // share the same neighbour Polygon (PolygonIndex is k) 

    //-------------------- Normal Vector ---------------------------------

    DenseMatrix<ScalarType> NormalAtVertex; // size: 3 x ?

    DenseMatrix<ScalarType> NormalAtPolygon;  // size: 3 x ?

    //----------------- attribute at Vertex ----------------------------

    //----------------- attribute at Polygon ----------------------------

    //DenseMatrix<ScalarType> AngleInPolygon;

    //DenseMatrix<ScalarType> AeraOfPolygon;
    
};


template<typename ScalarType = double>
class PolygonMesh : public Object
{
private:
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

    bool Construct(DenseMatrix<ScalarType> InputVertex, DataContainer<DenseVector<int_max>> InputPolygon, bool Flag_BuildLinkAndAdjacency = true);

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

    //----------------------------------------------------------------------------
    
    void BuildLinkAndAdjacency();

    void BuildLink();

    void BuildLink_VertexToEdge();

    void BuildLink_VertexToPolygon();

    void BuildLink_PolygonToEdge();

    void BuildAdjacency();

    void BuildAdjacency_VertexToVertex();

    void BuildAdjacency_PolygonToPolygon();

    //-----------------------------------------------------------------------

    void UpdateNormalAtVertex();

    void UpdateNormalAtPolygonCenter();

    //-------------------------------------------------------------------

    void UpdateAttribute();

    //-------------------------------------------------------------------

    inline DenseMatrix<int_max>& VertexGlobalIndexList();

    inline const DenseMatrix<int_max>& VertexGlobalIndexList() const;

    inline DenseMatrix<int_max>& PolygonGlobalIndexList();

    inline const DenseMatrix<int_max>& PolygonGlobalIndexList() const;

    inline DenseMatrix<ScalarType>& Vertex();

    inline const DenseMatrix<ScalarType>& Vertex() const;

    inline const DataContainer<DenseVector<int_max>>& Polygon() const;

    inline const DenseMatrix<int_max>& Edge() const;

    inline const DataContainer<DenseVector<int_max>>& Link_VertexToEdge() const;

    inline const DataContainer<DenseVector<int_max>>& Link_VertexToPolygon() const;

    inline const DataContainer<DenseVector<int_max>>& Link_PolygonToEdge() const;

    inline const DataContainer<DenseVector<int_max>>& Adjacency_VertexToVertex() const;

    inline const DataContainer<DenseVector<int_max>>& Adjacency_PolygonToPolygon() const;

    //----------------------------------------------------------------------------

    inline DenseMatrix<ScalarType>& NormalAtVertex();

    inline const DenseMatrix<ScalarType>& NormalAtVertex() const;

    inline DenseMatrix<ScalarType>& NormalAtPolygon();

    inline const DenseMatrix<ScalarType>& NormalAtPolygon() const;

private:
    bool ConstructEdge();

};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif