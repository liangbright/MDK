#ifndef __mdkTriangleMesh_hpp
#define __mdkTriangleMesh_hpp


namespace mdk
{

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh()
{
    m_MeshData = std::make_shared<TriangleMeshData<ScalarType>>();
}


template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const TriangleMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(TriangleMesh<ScalarType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
TriangleMesh<ScalarType>::~TriangleMesh()
{

}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(const TriangleMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(TriangleMesh<ScalarType>&& InputMesh)
{
    this->Take(std::forward<TriangleMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
bool TriangleMesh<ScalarType>::Construct(const DenseMatrix<ScalarType>& InputVertex, 
                                         const DenseMatrix<int_max>& InputTriangle,
                                         bool Flag_UpdateAttribute)
{
    if (InputVertex.IsEmpty() == true || InputTriangle.IsEmpty() == true)
    {
        MDK_Error("InputVertex or InputTriangle is empty @ TriangleMesh::Construct(...)")
        return false;
    }

    if (InputVertex.GetRowNumber() != 3 || InputTriangle.GetRowNumber() != 3 || InputTriangle.GetElementNumber() < InputVertex.GetColNumber())
    {
        MDK_Error("InputVertex or InputTriangle is invalid @ TriangleMesh::Construct(...)")
        return false;
    }

    //----------------------------------------------------------------------------

    m_MeshData->Vertex = InputVertex;

    //-------------------------------------

    if (this->ConstructEdge(InputTriangle) == false)
    {
        return false;
    }

    //-------------------------------------

    this->ConstructTriangle(InputTriangle);

    //----------------------------------------------------------------------------

    if (this->BuildLinkAndAjacency() == false)
    {
        return false;
    }

    if (Flag_UpdateAttribute == true)
    {
        this->UpdateAttribute();
    }

    //----------------------------------------------------------------------------

    return true;
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::ConstructEdge(const DenseMatrix<int_max>& InputTriangle)
{
    auto InputTriangleNumber = InputTriangle.GetColNumber();

    DataContainer<DenseMatrix<int_max>> HalfEdge;
    // each DenseMatrix:
    // row_0 : TriangleIndex
    // row_1 : VertexIndex_0
    // row_2 : VertexIndex_1
    // VertexIndex_0 < VertexIndex_1 always

    HalfEdge.FastResize(3 * InputTriangleNumber);

    for (int_max k = 0; k < InputTriangleNumber; ++k)
    {
        int_max VertexIndex_0 = InputTriangle(0, k);
        int_max VertexIndex_1 = InputTriangle(1, k);

        if (VertexIndex_0 > VertexIndex_1)
        {
            auto temp = VertexIndex_0;
            VertexIndex_0 = VertexIndex_1;
            VertexIndex_1 = temp;
        }

        HalfEdge[k] = { k, VertexIndex_0, VertexIndex_1 };

        //----------------------------

        VertexIndex_0 = InputTriangle(1, k);
        VertexIndex_1 = InputTriangle(2, k);

        if (VertexIndex_0 > VertexIndex_1)
        {
            auto temp = VertexIndex_0;
            VertexIndex_0 = VertexIndex_1;
            VertexIndex_1 = temp;
        }

        HalfEdge[k] = { k, VertexIndex_0, VertexIndex_1 };

        //----------------------------

        VertexIndex_0 = InputTriangle(2, k);
        VertexIndex_1 = InputTriangle(0, k);

        if (VertexIndex_0 > VertexIndex_1)
        {
            auto temp = VertexIndex_0;
            VertexIndex_0 = VertexIndex_1;
            VertexIndex_1 = temp;
        }

        HalfEdge[k] = { k, VertexIndex_0, VertexIndex_1 };
    }

    // sort HalfEdge

    std::sort(HalfEdge.begin(), HalfEdge.end(),
        [](const DenseMatrix<int_max>& A, const DenseMatrix<int_max>& B)
    {
        if (A[1] == B[1]) // VertexIndex_0 of A == VertexIndex_0 of B
        {
            return A[2] < B[2];
        }
        else
        {
            return A[1] < B[1];
        }
    });

    // get unique edge from halfedge

    DenseMatrix<int_max> UniqueEdgeFlagList(1, 3 * InputTriangleNumber);
    UniqueEdgeFlagList.Fill(0);

    auto TempFunction_SameEdge = [](const DenseMatrix<int_max>& A, const DenseMatrix<int_max>& B)
    {
        if (A[1] == B[1])
        {
            if (A[2] == B[2])
            {
                return true;
            }
        }

        return false;
    };

    UniqueEdgeFlagList[0] = 1;

    for (int_max k = 1; k < 3 * InputTriangleNumber; ++k)
    {
        if (TempFunction_SameEdge(HalfEdge[k - 1], HalfEdge[k]) == false)
        {
            UniqueEdgeFlagList[k] = 1;
        }
        else
        {
            if (k < 3 * InputTriangleNumber - 1)
            {
                if (TempFunction_SameEdge(HalfEdge[k], HalfEdge[k + 1]) == true)
                {
                    MDK_Error("InputTriangle is invalid @ TriangleMesh::ConstructEdge(...)")
                    return false;
                }
            }
        }
    }

    int_max UniqueEdgeNumber = UniqueEdgeFlagList.Sum();

    m_MeshData->Edge.FastResize(4, UniqueEdgeNumber);

    int_max EdgeIndex = 0;

    for (int_max k = 0; k < 3 * InputTriangleNumber; ++k)
    {
        if (UniqueEdgeFlagList[k] == 1)
        {
            auto TriangleIndex_0 = HalfEdge[k][0];

            int_max TriangleIndex_1 = -1;

            if (k < 3 * InputTriangleNumber - 1)
            {
                if (TempFunction_SameEdge(HalfEdge[k], HalfEdge[k + 1]) == true)
                {
                    TriangleIndex_1 = HalfEdge[k + 1][0];
                }
            }

            if (TriangleIndex_0 > TriangleIndex_1)
            {
                auto temp = TriangleIndex_0;
                TriangleIndex_0 = TriangleIndex_1;
                TriangleIndex_1 = temp;
            }

            m_MeshData->Edge(0, EdgeIndex) = HalfEdge[k][1];
            m_MeshData->Edge(1, EdgeIndex) = HalfEdge[k][2];
            m_MeshData->Edge(2, EdgeIndex) = TriangleIndex_0;
            m_MeshData->Edge(3, EdgeIndex) = TriangleIndex_1;

            EdgeIndex += 1;
        }
    }
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::ConstructTriangle(const DenseMatrix<int_max>& InputTriangle)
{
    auto InputTriangleNumber = InputTriangle.GetColNumber();

    m_MeshData->Triangle.FastResize(6, InputTriangleNumber);
    m_MeshData->Triangle.Fill(-1);
    m_MeshData->Triangle(span(0, 2), ALL) = InputTriangle;

    // re-order vertex

    for (int_max k = 0; k < InputTriangleNumber; ++k)
    {
        auto ColPtr = m_MeshData->Triangle.GetElementPointerOfCol(k);

        std::sort(ColPtr, ColPtr + 3);
    }

    //--------------------------------
    // add edge info to Triangle

    for (int_max k = 0; k < m_MeshData->Edge.GetColNumber(); ++k)
    {
        auto VertexIndex_0 = m_MeshData->Edge(0, k);
        auto VertexIndex_1 = m_MeshData->Edge(1, k);

        for (int_max n = 2; n <= 3; ++n)
        {
            auto TriangleIndex = m_MeshData->Edge(n, k);

            if (VertexIndex_0 == m_MeshData->Triangle(0, TriangleIndex) && VertexIndex_1 == m_MeshData->Triangle(1, TriangleIndex))
            {
                m_MeshData->Triangle(3, TriangleIndex) = k;
            }
            else if (VertexIndex_0 == m_MeshData->Triangle(1, TriangleIndex) && VertexIndex_1 == m_MeshData->Triangle(2, TriangleIndex))
            {
                m_MeshData->Triangle(4, TriangleIndex) = k;
            }
            else if (VertexIndex_0 == m_MeshData->Triangle(0, TriangleIndex) && VertexIndex_1 == m_MeshData->Triangle(2, TriangleIndex))
            {
                m_MeshData->Triangle(5, TriangleIndex) = k;
            }
        }
    }
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::BuildLinkAndAjacency()
{

}


template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::Clear()
{
    m_MeshData->Vertex.Clear();
    m_MeshData->Triangle.Clear();
    m_MeshData->Edge.Clear();
    
    m_MeshData->Ajacency_VertexToVertex.Clear();
    m_MeshData->Ajacency_VertexToEdge.Clear();
    m_MeshData->Ajacency_VertexToTriangle.Clear();
    m_MeshData->Ajacency_TriangleToTriangle.Clear();

    m_MeshData->NormalAtTriangle.Clear();
    m_MeshData->NormalAtVertex.Clear();
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
void TriangleMesh<ScalarType>::Copy(const TriangleMesh<ScalarType_Input>& InputMesh)
{
    this->m_MeshData->Vertex.Copy(InputMesh.m_MeshData->Vertex);
    this->m_MeshData->Triangle.Copy(InputMesh.m_MeshData->Triangle);
    this->m_MeshData->Edge.Copy(InputMesh.m_MeshData->Edge);

    this->m_MeshData->VertexGlobalIndexList.Copy(InputMesh.m_MeshData->VertexGlobalIndexList);
    this->m_MeshData->TriangleGlobalIndexList.Copy(InputMesh.m_MeshData->TriangleGlobalIndexList);

    this->m_MeshData->Ajacency_VertexToVertex.Copy(InputMesh.m_MeshData->Ajacency_VertexToVertex);
    this->m_MeshData->Ajacency_VertexToEdge.Copy(InputMesh.m_MeshData->Ajacency_VertexToEdge);
    this->m_MeshData->Ajacency_VertexToTriangle.Copy(InputMesh.m_MeshData->Ajacency_VertexToTriangle);
    this->m_MeshData->Ajacency_TriangleToTriangle.Copy(InputMesh.m_MeshData->Ajacency_TriangleToTriangle);

    this->m_MeshData->NormalAtVertex.Copy(InputMesh.m_MeshData->NormalAtVertex);
    this->m_MeshData->NormalAtTriangle.Copy(InputMesh.m_MeshData->NormalAtTriangle);
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
bool TriangleMesh<ScalarType>::Copy(const TriangleMesh<ScalarType_Input>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::Share(TriangleMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool TriangleMesh<ScalarType>::Share(TriangleMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::ForceShare(const TriangleMesh<ScalarType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::ForceShare(const TriangleMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>&& InputMesh)
{
    this->Take(std::forward<TriangleMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>& InputMesh)
{
    this->m_MeshData->Vertex = std::move(InputMesh.m_MeshData->Vertex);
    this->m_MeshData->Triangle = std::move(InputMesh.m_MeshData->Triangle);
    this->m_MeshData->Edge = std::move(InputMesh.m_MeshData->Edge);

    this->m_MeshData->VertexGlobalIndexList = std::move(InputMesh.m_MeshData->VertexGlobalIndexList);
    this->m_MeshData->TriangleGlobalIndexList = std::move(InputMesh.m_MeshData->TriangleGlobalIndexList);

    this->m_MeshData->Ajacency_VertexToVertex = std::move(InputMesh.m_MeshData->Ajacency_VertexToVertex);
    this->m_MeshData->Ajacency_VertexToEdge = std::move(InputMesh.m_MeshData->Ajacency_VertexToEdge);
    this->m_MeshData->Ajacency_VertexToTriangle = std::move(InputMesh.m_MeshData->Ajacency_VertexToTriangle);
    this->m_MeshData->Ajacency_TriangleToTriangle = std::move(InputMesh.m_MeshData->Ajacency_TriangleToTriangle);

    this->m_MeshData->NormalAtVertex = std::move(InputMesh.m_MeshData->NormalAtVertex);
    this->m_MeshData->NormalAtTriangle = std::move(InputMesh.m_MeshData->NormalAtTriangle);
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline bool TriangleMesh<ScalarType>::IsEmpty() const
{
    return m_MeshData->Vertex.IsEmpty();
}


template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::GetVertexNumber() const
{
    return m_MeshData->Vertex.GetColNumber();
}


template<typename ScalarType>
inline
int_max TriangleMesh<ScalarType>::GetTriangleNumber() const
{
    return m_MeshData->Triangle.GetColNumber();
}


template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::GetEdgeNumber() const
{
    return m_MeshData->Edge.GetColNumber();
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::Vertex()
{
    return m_MeshData->Vertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>&
TriangleMesh<ScalarType>::Vertex() const
{
    return m_MeshData->Vertex;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::Triangle() const
{
    return m_MeshData->Triangle;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::Edge() const
{
    return m_MeshData->Edge;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::VertexGlobalIndexList()
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::VertexGlobalIndexList() const
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::TriangleGlobalIndexList()
{
    return m_MeshData->TriangleGlobalIndexList;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& 
TriangleMesh<ScalarType>::TriangleGlobalIndexList() const
{
    return m_MeshData->TriangleGlobalIndexList;
}


template<typename ScalarType>
inline 
const DataContainer<DenseMatrix<int_max>>& 
TriangleMesh<ScalarType>::Ajacency_VertexToVertex() const
{
    return m_MeshData->Ajacency_VertexToVertex;
}


template<typename ScalarType>
inline 
const DataContainer<DenseMatrix<int_max>>& 
TriangleMesh<ScalarType>::Ajacency_VertexToEdge() const
{
    return m_MeshData->Ajacency_VertexToEdge;
}


template<typename ScalarType>
inline 
const DataContainer<DenseMatrix<int_max>>& 
TriangleMesh<ScalarType>::Ajacency_VertexToTriangle() const
{
    return m_MeshData->Ajacency_VertexToTriangle;
}


template<typename ScalarType>
inline 
const DataContainer<DenseMatrix<int_max>>& 
TriangleMesh<ScalarType>::Ajacency_TriangleToTriangle() const
{
    return m_MeshData->Ajacency_TriangleToTriangle;
}

//----------------------------------------------------------------------------


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAttribute()
{

}

}// namespace mdk

#endif