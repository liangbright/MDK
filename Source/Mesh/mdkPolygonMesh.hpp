#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh()
{
    m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
}


template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const PolygonMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(PolygonMesh<ScalarType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
PolygonMesh<ScalarType>::~PolygonMesh()
{

}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const PolygonMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
    }

    this->Take(std::forward<PolygonMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::Construct(DenseMatrix<ScalarType> InputVertex, 
                                        DataContainer<DenseVector<int_max>> InputPolygon,
                                        bool Flag_BuildLinkAndAdjacency)
{
    if (InputVertex.IsEmpty() == true || InputPolygon.IsEmpty() == true)
    {
        MDK_Error("InputVertex or InputPolygon is empty @ PolygonMesh::Construct(...)")
        return false;
    }

    if (InputVertex.GetRowNumber() != 3 || 3 * InputPolygon.GetElementNumber() < InputVertex.GetColNumber())
    {
        MDK_Error("InputVertex or InputPolygon is invalid @ PolygonMesh::Construct(...)")
        return false;
    }

    //----------------------------------------------------------------------------

    m_MeshData->Vertex = std::move(InputVertex);

    //-------------------------------------

    m_MeshData->Polygon = std::move(InputPolygon);

    //-------------------------------------

    if (this->ConstructEdge() == false)
    {
        return false;
    }

    //----------------------------------------------------------------------------

    if (Flag_BuildLinkAndAdjacency == true)
    {
        this->BuildLink();

        this->BuildAdjacency();
    }

    //----------------------------------------------------------------------------

    m_MeshData->VertexGlobalIndexList = span(0, this->GetVertexNumber()-1);

    m_MeshData->PolygonGlobalIndexList = span(0, this->GetPolygonNumber()-1);

    //----------------------------------------------------------------------------

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ConstructEdge()
{
    auto PolygonNumber = m_MeshData->Polygon.GetLength();

    DataContainer<DenseVector<int_max>> HalfEdge;
    // HalfEdge[k] is a half edge
    // HalfEdge[k][0] : PolygonIndex
    // HalfEdge[k][1] : VertexIndex_0
    // HalfEdge[k][2] : VertexIndex_1
    // VertexIndex_0 < VertexIndex_1 always

    int_max HalfEdgeNumber = 0;

    HalfEdge.ReserveCapacity(HalfEdgeNumber);

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        auto VertexNumber_k = m_MeshData->Polygon[k].GetElementNumber();

        const DenseVector<int_max>& Polygon_k = m_MeshData->Polygon[k];

        for (int_max n = 0; n < VertexNumber_k - 1; ++n)
        {
            //VertexIndex_0 < VertexIndex_1
            int_max VertexIndex_a = Polygon_k[n];
            int_max VertexIndex_b = Polygon_k[n + 1];
            
            if (VertexIndex_a < VertexIndex_b)
            {
                HalfEdge.Append({ k, VertexIndex_a, VertexIndex_b });
            }
            else
            {
                HalfEdge.Append({ k, VertexIndex_b, VertexIndex_a });
            }

            HalfEdgeNumber += 1;
        }

        //VertexIndex_0 < VertexIndex_1
        int_max VertexIndex_0 = Polygon_k[0];
        int_max VertexIndex_end = Polygon_k[VertexNumber_k-1];

        if (VertexIndex_0 < VertexIndex_end)
        {
            HalfEdge.Append({ k, VertexIndex_0, VertexIndex_end });
        }
        else
        {
            HalfEdge.Append({ k, VertexIndex_end, VertexIndex_0 });
        }
        HalfEdgeNumber += 1;
    }

    // sort HalfEdge

    std::sort(HalfEdge.begin(), HalfEdge.end(), [](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
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

    DenseMatrix<int_max> UniqueEdgeFlagList(1, HalfEdgeNumber);
    UniqueEdgeFlagList.Fill(0);

    auto TempFunction_SameEdge = [](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
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

    for (int_max k = 1; k < HalfEdgeNumber; ++k)
    {
        if (TempFunction_SameEdge(HalfEdge[k - 1], HalfEdge[k]) == false)
        {
            UniqueEdgeFlagList[k] = 1;
        }
        else
        {
            if (k < HalfEdgeNumber - 1)
            {
                if (TempFunction_SameEdge(HalfEdge[k], HalfEdge[k + 1]) == true)
                {
                    MDK_Error("Polygon Data is invalid @ PolygonMesh::ConstructEdge()")
                    return false;
                }
            }
        }
    }

    int_max UniqueEdgeNumber = UniqueEdgeFlagList.Sum();

    m_MeshData->Edge.FastResize(4, UniqueEdgeNumber);

    int_max EdgeIndex = 0;

    for (int_max k = 0; k < HalfEdgeNumber; ++k)
    {
        if (UniqueEdgeFlagList[k] == 1)
        {
            auto PolygonIndex_0 = HalfEdge[k][0];

            int_max PolygonIndex_1 = -1;

            if (k < HalfEdgeNumber - 1)
            {
                if (TempFunction_SameEdge(HalfEdge[k], HalfEdge[k + 1]) == true)
                {
                    PolygonIndex_1 = HalfEdge[k + 1][0];
                }
            }

            if (PolygonIndex_0 > PolygonIndex_1)
            {
                auto temp_0 = PolygonIndex_0;
                PolygonIndex_0 = PolygonIndex_1;
                PolygonIndex_1 = temp_0;
            }

            m_MeshData->Edge(0, EdgeIndex) = HalfEdge[k][1];
            m_MeshData->Edge(1, EdgeIndex) = HalfEdge[k][2];
            m_MeshData->Edge(2, EdgeIndex) = PolygonIndex_0;
            m_MeshData->Edge(3, EdgeIndex) = PolygonIndex_1;

            EdgeIndex += 1;
        }
    }

    return true;
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildLinkAndAdjacency()
{
    this->BuildLink();

    this->BuildAdjacency();
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildLink()
{
    this->BuildLink_VertexToEdge();
    
    this->BuildLink_VertexToPolygon();

    this->BuildLink_PolygonToEdge();
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildLink_VertexToEdge()
{
    int_max VertexNumber = m_MeshData->Vertex.GetColNumber();

    int_max EdgeNumber = m_MeshData->Edge.GetColNumber();

    m_MeshData->Link_VertexToEdge.FastResize(VertexNumber);

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Link_VertexToEdge[k].Resize(0);
        m_MeshData->Link_VertexToEdge[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        auto VertexIndex_0 = m_MeshData->Edge(0, k);
        auto VertexIndex_1 = m_MeshData->Edge(1, k);

        m_MeshData->Link_VertexToEdge[VertexIndex_0].Append(k);
        m_MeshData->Link_VertexToEdge[VertexIndex_1].Append(k);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Link_VertexToEdge[k].Squeeze();
    }
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildLink_VertexToPolygon()
{
    int_max VertexNumber = m_MeshData->Vertex.GetColNumber();

    int_max PolygonNumber = m_MeshData->Polygon.GetElementNumber();

    m_MeshData->Link_VertexToPolygon.FastResize(VertexNumber);

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Link_VertexToPolygon[k].Resize(0);
        m_MeshData->Link_VertexToPolygon[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        auto VertexNumber_k = m_MeshData->Polygon[k].GetLength();

        const DenseVector<int_max>& Polygon_k = m_MeshData->Polygon[k];

        for (int_max n = 0; n < VertexNumber_k; ++n)
        {
            auto VertexIndex_n = Polygon_k[n];
            m_MeshData->Link_VertexToPolygon[VertexIndex_n].Append(k);
        }
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Link_VertexToPolygon[k].Squeeze();
    }
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildLink_PolygonToEdge()
{
    auto PolygonNumber = m_MeshData->Polygon.GetLength();

    m_MeshData->Link_PolygonToEdge.FastResize(PolygonNumber);

    int_max MeanEdgeNumberPerPolygon = 0;

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        MeanEdgeNumberPerPolygon += m_MeshData->Polygon[k].GetLength();
    }

    MeanEdgeNumberPerPolygon /= PolygonNumber;

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        m_MeshData->Link_PolygonToEdge[k].Resize(0);
        m_MeshData->Link_PolygonToEdge[k].ReserveCapacity(MeanEdgeNumberPerPolygon);
    }

    for (int_max k = 0; k < m_MeshData->Edge.GetColNumber(); ++k)
    {
        auto VertexIndex_0 = m_MeshData->Edge(0, k);
        auto VertexIndex_1 = m_MeshData->Edge(1, k);

        for (int_max n = 2; n <= 3; ++n)
        {
            auto PolygonIndex_n = m_MeshData->Edge(n, k);

            if (PolygonIndex_n >= 0)
            {
                const DenseVector<int_max>& Polygon_n = m_MeshData->Polygon[PolygonIndex_n];

                auto VertexNumberInPolygon = Polygon_n.GetLength();

                for (int_max m = 0; m < VertexNumberInPolygon; ++m)
                {
                    auto VertexIndex_m = Polygon_n[m];

                    if (m == 0)
                    {
                        if (VertexIndex_0 == VertexIndex_m && VertexIndex_1 == Polygon_n[VertexNumberInPolygon - 1]
                            || VertexIndex_1 == VertexIndex_m && VertexIndex_0 == Polygon_n[VertexNumberInPolygon - 1])
                        {
                            m_MeshData->Link_PolygonToEdge[PolygonIndex_n].Append(k);
                            break; // for m
                        }
                    }
                    
                    if (m < VertexNumberInPolygon - 1)
                    {
                        if (VertexIndex_0 == VertexIndex_m && VertexIndex_1 == Polygon_n[m + 1]
                            || VertexIndex_1 == VertexIndex_m && VertexIndex_0 == Polygon_n[m + 1])
                        {
                            m_MeshData->Link_PolygonToEdge[PolygonIndex_n].Append(k);
                            break; // for m
                        }                        
                    }
                }
            }
        }
    }

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        m_MeshData->Link_PolygonToEdge[k].Squeeze();
    }
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildAdjacency()
{
    this->BuildAdjacency_VertexToVertex();

    this->BuildAdjacency_PolygonToPolygon();
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildAdjacency_VertexToVertex()
{
    int_max VertexNumber = m_MeshData->Vertex.GetColNumber();

    int_max EdgeNumber = m_MeshData->Edge.GetColNumber();

    m_MeshData->Adjacency_VertexToVertex.FastResize(VertexNumber);

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Adjacency_VertexToVertex[k].Resize(0);
        m_MeshData->Adjacency_VertexToVertex[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        auto VertexIndex_0 = m_MeshData->Edge(0, k);
        auto VertexIndex_1 = m_MeshData->Edge(1, k);

        m_MeshData->Adjacency_VertexToVertex[VertexIndex_0].Append(VertexIndex_1);
        m_MeshData->Adjacency_VertexToVertex[VertexIndex_1].Append(VertexIndex_0);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        m_MeshData->Adjacency_VertexToVertex[k].Squeeze();
    }
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::BuildAdjacency_PolygonToPolygon()
{
    int_max EdgeNumber = m_MeshData->Edge.GetColNumber();

    int_max PolygonNumber = m_MeshData->Polygon.GetElementNumber();

    m_MeshData->Adjacency_PolygonToPolygon.FastResize(PolygonNumber);

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        m_MeshData->Adjacency_PolygonToPolygon[k].Resize(0);
        m_MeshData->Adjacency_PolygonToPolygon[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        auto PolygonIndex_0 = m_MeshData->Edge(2, k);
        auto PolygonIndex_1 = m_MeshData->Edge(3, k);

        if (PolygonIndex_0 >= 0 && PolygonIndex_1 >= 0)
        {
            m_MeshData->Adjacency_PolygonToPolygon[PolygonIndex_0].Append(PolygonIndex_1);
            m_MeshData->Adjacency_PolygonToPolygon[PolygonIndex_1].Append(PolygonIndex_0);
        }
    }

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        m_MeshData->Adjacency_PolygonToPolygon[k].Squeeze();
    }
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::Clear()
{
    m_MeshData->Vertex.Clear();
    m_MeshData->Polygon.Clear();
    m_MeshData->Edge.Clear();
    
    m_MeshData->Link_VertexToEdge.Clear();
    m_MeshData->Link_VertexToPolygon.Clear();
    m_MeshData->Link_PolygonToEdge.Clear();

    m_MeshData->Adjacency_VertexToVertex.Clear();
    m_MeshData->Adjacency_PolygonToPolygon.Clear();

    m_MeshData->NormalAtPolygon.Clear();
    m_MeshData->NormalAtVertex.Clear();
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
void PolygonMesh<ScalarType>::Copy(const PolygonMesh<ScalarType_Input>& InputMesh)
{
    this->m_MeshData->Vertex.Copy(InputMesh.m_MeshData->Vertex);
    this->m_MeshData->Polygon.Copy(InputMesh.m_MeshData->Polygon);
    this->m_MeshData->Edge.Copy(InputMesh.m_MeshData->Edge);

    this->m_MeshData->VertexGlobalIndexList.Copy(InputMesh.m_MeshData->VertexGlobalIndexList);
    this->m_MeshData->PolygonGlobalIndexList.Copy(InputMesh.m_MeshData->PolygonGlobalIndexList);

    this->m_MeshData->Link_VertexToEdge.Copy(InputMesh.m_MeshData->Link_VertexToEdge);
    this->m_MeshData->Link_VertexToPolygon.Copy(InputMesh.m_MeshData->Link_VertexToPolygon);
    this->m_MeshData->Link_PolygonToEdge.Copy(InputMesh.m_MeshData->Link_PolygonToEdge);

    this->m_MeshData->Adjacency_VertexToVertex.Copy(InputMesh.m_MeshData->Adjacency_VertexToVertex);
    this->m_MeshData->Adjacency_PolygonToPolygon.Copy(InputMesh.m_MeshData->Adjacency_PolygonToPolygon);

    this->m_MeshData->NormalAtVertex.Copy(InputMesh.m_MeshData->NormalAtVertex);
    this->m_MeshData->NormalAtPolygon.Copy(InputMesh.m_MeshData->NormalAtPolygon);
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
bool PolygonMesh<ScalarType>::Copy(const PolygonMesh<ScalarType_Input>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::Share(PolygonMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool PolygonMesh<ScalarType>::Share(PolygonMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::ForceShare(const PolygonMesh<ScalarType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::ForceShare(const PolygonMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>&& InputMesh)
{
    this->Take(std::forward<PolygonMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>& InputMesh)
{
    this->m_MeshData->Vertex = std::move(InputMesh.m_MeshData->Vertex);
    this->m_MeshData->Polygon = std::move(InputMesh.m_MeshData->Polygon);
    this->m_MeshData->Edge = std::move(InputMesh.m_MeshData->Edge);

    this->m_MeshData->VertexGlobalIndexList = std::move(InputMesh.m_MeshData->VertexGlobalIndexList);
    this->m_MeshData->PolygonGlobalIndexList = std::move(InputMesh.m_MeshData->PolygonGlobalIndexList);

    this->m_MeshData->Link_VertexToEdge = std::move(InputMesh.m_MeshData->Link_VertexToEdge);
    this->m_MeshData->Link_VertexToPolygon = std::move(InputMesh.m_MeshData->Link_VertexToPolygon);
    this->m_MeshData->Link_PolygonToEdge = std::move(InputMesh.m_MeshData->Link_PolygonToEdge);

    this->m_MeshData->Adjacency_VertexToVertex = std::move(InputMesh.m_MeshData->Adjacency_VertexToVertex);
    this->m_MeshData->Adjacency_PolygonToPolygon = std::move(InputMesh.m_MeshData->Adjacency_PolygonToPolygon);

    this->m_MeshData->NormalAtVertex = std::move(InputMesh.m_MeshData->NormalAtVertex);
    this->m_MeshData->NormalAtPolygon = std::move(InputMesh.m_MeshData->NormalAtPolygon);
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline bool PolygonMesh<ScalarType>::IsEmpty() const
{
    return m_MeshData->Vertex.IsEmpty();
}


template<typename ScalarType>
inline 
int_max PolygonMesh<ScalarType>::GetVertexNumber() const
{
    return m_MeshData->Vertex.GetColNumber();
}


template<typename ScalarType>
inline
int_max PolygonMesh<ScalarType>::GetPolygonNumber() const
{
    return m_MeshData->Polygon.GetElementNumber();
}


template<typename ScalarType>
inline 
int_max PolygonMesh<ScalarType>::GetEdgeNumber() const
{
    return m_MeshData->Edge.GetColNumber();
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline
DenseMatrix<int_max>& PolygonMesh<ScalarType>::VertexGlobalIndexList()
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline
const DenseMatrix<int_max>& PolygonMesh<ScalarType>::VertexGlobalIndexList() const
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline
DenseMatrix<int_max>& PolygonMesh<ScalarType>::PolygonGlobalIndexList()
{
    return m_MeshData->PolygonGlobalIndexList;
}


template<typename ScalarType>
inline
const DenseMatrix<int_max>& PolygonMesh<ScalarType>::PolygonGlobalIndexList() const
{
    return m_MeshData->PolygonGlobalIndexList;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::Vertex()
{
    return m_MeshData->Vertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::Vertex() const
{
    return m_MeshData->Vertex;
}


template<typename ScalarType>
inline
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Polygon() const
{
    return m_MeshData->Polygon;
}


template<typename ScalarType>
inline 
const DenseMatrix<int_max>& PolygonMesh<ScalarType>::Edge() const
{
    return m_MeshData->Edge;
}


template<typename ScalarType>
inline
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Link_VertexToEdge() const
{
    return m_MeshData->Link_VertexToEdge;
}


template<typename ScalarType>
inline
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Link_VertexToPolygon() const
{
    return m_MeshData->Link_VertexToPolygon;
}


template<typename ScalarType>
inline
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Link_PolygonToEdge() const
{
    return m_MeshData->Link_PolygonToEdge;
}


template<typename ScalarType>
inline 
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Adjacency_VertexToVertex() const
{
    return m_MeshData->Adjacency_VertexToVertex;
}


template<typename ScalarType>
inline 
const DataContainer<DenseVector<int_max>>& PolygonMesh<ScalarType>::Adjacency_PolygonToPolygon() const
{
    return m_MeshData->Adjacency_PolygonToPolygon;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex()
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex() const
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon()
{
    return m_MeshData->NormalAtPolygon;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon() const
{
    return m_MeshData->NormalAtPolygon;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateNormalAtVertex()
{
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateNormalAtPolygonCenter()
{
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateAttribute()
{
}




}// namespace mdk

#endif