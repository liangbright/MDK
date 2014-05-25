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
    DenseMatrix<ScalarType> Vertex;    
    // row_0: x
    // row_1: y
    // row_2: z

    DenseMatrix<ScalarType> Triangle; // also known as cell, face, facet, element
    // row_0: VertexIndex_0
    // row_1: VertexIndex_1
    // row_2: VertexIndex_2
    // row_3: EdgeIndex_0
    // row_4: EdgeIndex_1
    // row_5: EdgeIndex_2
    //
    // VertexIndex_0 < VertexIndex_1 < VerteIndex_2 always
    //
    // Edge of EdgeIndex_0 : VertexIndex_0 <-> VertexIndex_1
    // Edge of EdgeIndex_1 : VertexIndex_1 <-> VertexIndex_2
    // Edge of EdgeIndex_2 : VertexIndex_0 <-> VertexIndex_2

    DenseMatrix<ScalarType> Edge;
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

    DenseMatrix<int_max> VertexGlobalIndexList;

    DenseMatrix<int_max> TriangleGlobalIndexList;

    DataContainer<DenseMatrix<int_max>> Link_VertexToEdge;    // Vertex of Edge

    DataContainer<DenseMatrix<int_max>> Link_VertexToTriangle; // Vertex of Triangle

    DataContainer<DenseMatrix<int_max>> Ajacency_VertexToVertex;

    DataContainer<DenseMatrix<int_max>> Ajacency_TriangleToTriangle;

    DataContainer<DenseMatrix<int_max>> Ajacency_TriangleToEdge; // not include any Edge of Triangle

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
    TriangleMesh();

    TriangleMesh(const TriangleMesh& InputMesh);

    TriangleMesh(TriangleMesh&& InputMesh);

    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);

    inline void operator=(TriangleMesh&& InputMesh);

    //---------------------------------------------------------------------------

    bool Construct(const DenseMatrix<ScalarType>& InputVertex,
                   const DenseMatrix<int_max>& InputTriangle,
                   bool Flag_UpdateAttribute = true);

    // InputVertex is a 3-row matrix
    // InputTriangle is a 3-row matrix

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

    //-------------------------------------------------------------------

    inline DenseMatrix<ScalarType>& Vertex();

    inline const DenseMatrix<ScalarType>& Vertex() const;

    inline const DenseMatrix<ScalarType>& Triangle() const;

    inline const DenseMatrix<ScalarType>& Edge() const;

    inline DenseMatrix<ScalarType>& VertexGlobalIndexList();

    inline const DenseMatrix<ScalarType>& VertexGlobalIndexList() const;

    inline DenseMatrix<ScalarType>& TriangleGlobalIndexList();

    inline const DenseMatrix<ScalarType>& TriangleGlobalIndexList() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_VertexToVertex() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_VertexToEdge() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_VertexToTriangle() const;

    inline const DataContainer<DenseMatrix<int_max>>& Ajacency_TriangleToTriangle() const;

    //----------------------------------------------------------------------------

    void UpdateAttribute();

private:
    bool ConstructEdge(const DenseMatrix<int_max>& InputTriangle);

    void ConstructTriangle(const DenseMatrix<int_max>& InputTriangle);

    void BuildLinkAndAjacency();
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif