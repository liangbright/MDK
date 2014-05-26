#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h


#include "mdkObject.h"
#include "mdkDenseMatrix.h"
#include "mdkDataContainer.h"


namespace mdk
{
template<typename ScalarType = double>
struct TriangleMeshData
{
    DenseMatrix<int_max> VertexGlobalIndexList;

    DenseMatrix<int_max> TriangleGlobalIndexList;

    DenseMatrix<ScalarType> Vertex;    
    // row_0: x
    // row_1: y
    // row_2: z

    DenseMatrix<int_max> Triangle; // also known as cell, face, facet, element
    // row_0: VertexIndex_0
    // row_1: VertexIndex_1
    // row_2: VertexIndex_2
    //
    // VertexIndex_0 < VertexIndex_1 < VerteIndex_2 always
    
    DenseMatrix<int_max> Edge;
    // row_0: VertexIndex_0
    // row_1: VertexIndex_1
    // row_2: TriangleIndex_0  
    // row_3: TriangleIndex_1  
    //
    // Edge = VertexIndex_0 <-> VertexIndex_1
    //
    // Edge is shared by TriangleIndex_0 and TriangleIndex_1
    //
    // if TriangleIndex_0 is -1, then edge is part of boundary
    //
    // VertexIndex_0    < VertexIndex_1   always
    // TriangleIndex_0  < TriangleIndex_1 always

    DataContainer<DenseMatrix<int_max>> Link_VertexToEdge;
    // Link_VertexToEdge[k] is { EdgeIndex_0, EdgeIndex_1, EdgeIndex_2, ...}
    // share the same Vertex (VertexIndex is k) 

    DataContainer<DenseMatrix<int_max>> Link_VertexToTriangle;
    // Link_VertexToTriangle[k] is { TriangleIndex_0, TriangleIndex_1, TriangleIndex_2, ...}
    // share the same Vertex (VertexIndex is k) 

    DataContainer<DenseMatrix<int_max>> Link_TriangleToEdge;
    // Link_TriangleToEdge[k] is { EdgeIndex_0, EdgeIndex_1, EdgeIndex_2} in the same triangle (TriangleIndex is k)
    //
    // Triangle(:,k) : VertexIndex_0, VertexIndex_1, VertexIndex_2
    // Edge of EdgeIndex_0 : VertexIndex_0 <-> VertexIndex_1
    // Edge of EdgeIndex_1 : VertexIndex_1 <-> VertexIndex_2
    // Edge of EdgeIndex_2 : VertexIndex_0 <-> VertexIndex_2

    DataContainer<DenseMatrix<int_max>> Ajacency_VertexToVertex;
    // Ajacency_VertexToVertex[k] is { VertexIndex_0, VertexIndex_1, VertexIndex_2, ...}
    // share the same neighbour Vertex (VertexIndex is k) 

    //DataContainer<DenseMatrix<int_max>> Ajacency_EdgeToEdge; can be directly derived from Edge and Link_VertexToEdge
    // Ajacency_EdgeToEdge[k] is { Link_VertexToEdge[Edge(0, k)], Link_VertexToEdge[Edge(1, k)] } 
     
    DataContainer<DenseMatrix<int_max>> Ajacency_TriangleToTriangle;
    // Ajacency_TriangleToTriangle[k] is { TriangleIndex_0, TriangleIndex_1, TriangleIndex_2, ...}
    // share the same neighbour Triangle (TriangleIndex is k) 

    //----------------- attribute ----------------------------

    DenseMatrix<ScalarType> NormalAtVertex;

    DenseMatrix<ScalarType> NormalAtTriangle;
};


template<typename ScalarType = double>
class TriangleMesh : public Object
{
private:
    std::shared_ptr<TriangleMeshData<ScalarType>> m_MeshData;

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

    bool Construct(DenseMatrix<ScalarType> InputVertex, DenseMatrix<int_max> InputTriangle, bool Flag_BuildLinkAndAjacency = true);

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

    //----------------------------------------------------------------------------
    
    void BuildLinkAndAjacency();

    void BuildLink();

    void BuildLink_VertexToEdge();

    void BuildLink_VertexToTriangle();

    void BuildLink_TriangleToEdge();

    void BuildAjacency();

    void BuildAjacency_VertexToVertex();

    void BuildAjacency_TriangleToTriangle();

    //-----------------------------------------------------------------------

    void UpdateAttribute();

    void UpdateAttribute_NormalAtVertex();

    void UpdateAttribute_NormalAtTriangle();

    //-------------------------------------------------------------------

    inline DenseMatrix<int_max>& VertexGlobalIndexList();

    inline const DenseMatrix<int_max>& VertexGlobalIndexList() const;

    inline DenseMatrix<int_max>& TriangleGlobalIndexList();

    inline const DenseMatrix<int_max>& TriangleGlobalIndexList() const;

    inline DenseMatrix<ScalarType>& Vertex();

    inline const DenseMatrix<ScalarType>& Vertex() const;

    inline const DenseMatrix<int_max>& Triangle() const;

    inline const DenseMatrix<int_max>& Edge() const;

    inline const DataContainer<DenseMatrix<int_max>>& Link_VertexToEdge() const;

    inline const DataContainer<DenseMatrix<int_max>>& Link_VertexToTriangle() const;

    inline const DataContainer<DenseMatrix<int_max>>& Link_TriangleToEdge() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_VertexToVertex() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_TriangleToTriangle() const;

    //----------------------------------------------------------------------------

    inline DenseMatrix<ScalarType>& NormalAtVertex();

    inline const DenseMatrix<ScalarType>& NormalAtVertex() const;

    inline DenseMatrix<ScalarType>& NormalAtTriangle();

    inline const DenseMatrix<ScalarType>& NormalAtTriangle() const;

private:
    bool ConstructEdge();

};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif