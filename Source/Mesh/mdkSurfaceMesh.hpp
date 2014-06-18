#ifndef __mdkSurfaceMesh_hpp
#define __mdkSurfaceMesh_hpp

namespace mdk
{

template<typename ScalarType>
SurfaceMesh<ScalarType>::SurfaceMesh()
{
    m_MeshData = std::make_shared<SurfaceMeshData<ScalarType>>();
}


template<typename ScalarType>
SurfaceMesh<ScalarType>::SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&)
{
}


template<typename ScalarType>
SurfaceMesh<ScalarType>::SurfaceMesh(const SurfaceMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
SurfaceMesh<ScalarType>::SurfaceMesh(SurfaceMesh<ScalarType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
SurfaceMesh<ScalarType>::~SurfaceMesh()
{
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::operator=(const SurfaceMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::operator=(SurfaceMesh<ScalarType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<SurfaceMeshData<ScalarType>>();
    }

    this->Take(std::forward<SurfaceMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->IsTriangleMesh = false;

    m_MeshData->VertexGlobalIDList.Clear();
    m_MeshData->CellGlobalIDList.Clear();

    m_MeshData->PointPositionTable.Clear();

    m_MeshData->VertexList.Clear();
    m_MeshData->VertexValidityFlagList.Clear();

    m_MeshData->EdgeList.Clear();
    m_MeshData->DirectedEdgePairList.Clear();
    m_MeshData->EdgeValidityFlagList.Clear();

    m_MeshData->CellList.Clear();
    m_MeshData->CellValidityFlagList.Clear();
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::Copy(const SurfaceMesh<ScalarType>& InputMesh)
{
    m_MeshData->VertexGlobalIDList = InputMesh.m_MeshData->VertexGlobalIDList;
    m_MeshData->CellGlobalIDList = InputMesh.m_MeshData->CellGlobalIDList;

    m_MeshData->PointPositionTable = InputMesh.m_MeshData->PointPositionTable;

    m_MeshData->PointList = InputMesh.m_MeshData->PointList;
    m_MeshData->PointValidityFlagList = InputMesh.m_MeshData->PointValidityFlagList;

    m_MeshData->VertexList = InputMesh.m_MeshData->VertexList;
    m_MeshData->VertexValidityFlagList = InputMesh.m_MeshData->VertexValidityFlagList;

    m_MeshData->CellList = InputMesh.m_MeshData->CellList;
    m_MeshData->CellValidityFlagList = InputMesh.m_MeshData->CellValidityFlagList;

    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    m_MeshData->DirectedEdgePairList = InputMesh.m_MeshData->DirectedEdgePairList;
    m_MeshData->EdgeValidityFlagList = InputMesh.m_MeshData->EdgeValidityFlagList;
}


template<typename ScalarType>
inline
bool SurfaceMesh<ScalarType>::Copy(const SurfaceMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::Share(SurfaceMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool SurfaceMesh<ScalarType>::Share(SurfaceMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::ForceShare(const SurfaceMesh<ScalarType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool SurfaceMesh<ScalarType>::ForceShare(const SurfaceMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::Take(SurfaceMesh<ScalarType>&& InputMesh)
{
    Take(std::forward<SurfaceMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::Take(SurfaceMesh<ScalarType>& InputMesh)
{
    m_MeshData->VertexGlobalIDList = std::move(InputMesh.m_MeshData->VertexGlobalIDList);
    m_MeshData->CellGlobalIDList = std::move(InputMesh.m_MeshData->CellGlobalIDList);

    m_MeshData->PointPositionTable = std::move(InputMesh.m_MeshData->PointPositionTable);

    m_MeshData->PointList = std::move(InputMesh.m_MeshData->PointList);
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        m_MeshData->PointList[k].SetParentMesh(*this);
    }

    m_MeshData->PointValidityFlagList = std::move(InputMesh.m_MeshData->PointValidityFlagList);

    m_MeshData->VertexList = std::move(InputMesh.m_MeshData->VertexList);
    for (int_max k = 0; k < m_MeshData->VertexList.GetLength(); ++k)
    {
        m_MeshData->VertexList[k].SetParentMesh(*this);
    }

    m_MeshData->VertexValidityFlagList = std::move(InputMesh.m_MeshData->VertexValidityFlagList);

    m_MeshData->EdgeList = std::move(InputMesh.m_MeshData->EdgeList);
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        m_MeshData->EdgeList[k].SetParentMesh(*this);
    }

    m_MeshData->DirectedEdgePairList = std::move(InputMesh.m_MeshData->DirectedEdgePairList);
    for (int_max k = 0; k < m_MeshData->DirectedEdgePairList.GetLength(); ++k)
    {
        m_MeshData->DirectedEdgePairList[k][0].SetParentMesh(*this);
        m_MeshData->DirectedEdgePairList[k][1].SetParentMesh(*this);
    }

    m_MeshData->EdgeValidityFlagList = std::move(InputMesh.m_MeshData->EdgeValidityFlagList);

    m_MeshData->CellList = std::move(InputMesh.m_MeshData->CellList);
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        m_MeshData->CellList[k].SetParentMesh(*this);
    }

    m_MeshData->CellValidityFlagList = std::move(InputMesh.m_MeshData->CellValidityFlagList);
}


template<typename ScalarType>
inline
bool SurfaceMesh<ScalarType>::Take(SurfaceMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Take(...)")
        return false;
    }

    this->Take(*InputMesh);

    return true;
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline 
bool SurfaceMesh<ScalarType>::IsEmpty() const
{
    return m_MeshData->PointPositionTable.IsEmpty();
}

template<typename ScalarType>
inline
int_max SurfaceMesh<ScalarType>::GetValidPointNumber() const
{
    return m_MeshData->PointValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max SurfaceMesh<ScalarType>::GetValidVertexNumber() const
{
    return m_MeshData->VertexValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max SurfaceMesh<ScalarType>::GetValidCellNumber() const
{
    return m_MeshData->CellValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max SurfaceMesh<ScalarType>::GetValidEdgeNumber() const
{
    return m_MeshData->EdgeValidityFlagList.Sum();
}

//------------------------- Vertex and Cell GlobalID ------------------------

template<typename ScalarType>
inline
DenseVector<int_max>& SurfaceMesh<ScalarType>::VertexGlobalIDList()
{
    return m_MeshData->VertexGlobalIDList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& SurfaceMesh<ScalarType>::VertexGlobalIDList() const
{
    return m_MeshData->VertexGlobalIDList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& SurfaceMesh<ScalarType>::CellGlobalIDList()
{
    return m_MeshData->CellGlobalIDList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& SurfaceMesh<ScalarType>::CellGlobalIDList() const
{
    return m_MeshData->CellGlobalIDList;
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename ScalarType>
inline
DenseMatrix<ScalarType>& SurfaceMesh<ScalarType>::PointPositionTable()
{
    return m_MeshData->PointPositionTable;
}

template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& SurfaceMesh<ScalarType>::PointPositionTable() const
{
    return m_MeshData->PointPositionTable;
}

//------------------------- Mesh Element ----------------------------------------------

template<typename ScalarType>
inline
DataArray<Vertex_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::VertexList()
{
    return m_MeshData->VertexList;
}

template<typename ScalarType>
inline
const DataArray<Vertex_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::VertexList() const
{
    return m_MeshData->VertexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& SurfaceMesh<ScalarType>::VertexValidityFlagList()
{
    return m_MeshData->VertexValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& SurfaceMesh<ScalarType>::VertexValidityFlagList() const
{
    return m_MeshData->VertexValidityFlagList;
}

template<typename ScalarType>
inline
DataArray<Edge_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::EdgeList()
{
    return m_MeshData->EdgeList;
}

template<typename ScalarType>
inline
const DataArray<Edge_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::EdgeList() const
{
    return m_MeshData->EdgeList;
}

template<typename ScalarType>
inline
DataArray<DirectedEdge_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::DirectedEdgePairList()
{
    return m_MeshData->DirectedEdgePairList;
}

template<typename ScalarType>
inline
const DataArray<DirectedEdge_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::DirectedEdgePairList() const
{
    return m_MeshData->DirectedEdgePairList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& SurfaceMesh<ScalarType>::EdgeValidityFlagList()
{
    return m_MeshData->EdgeValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& SurfaceMesh<ScalarType>::EdgeValidityFlagList() const
{
    return m_MeshData->EdgeValidityFlagList;
}

template<typename ScalarType>
inline
DataArray<Cell_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::CellList()
{
    return m_MeshData->CellList;
}

template<typename ScalarType>
inline
const DataArray<Cell_Of_SurfaceMesh<ScalarType>>& SurfaceMesh<ScalarType>::CellList() const
{
    return m_MeshData->CellList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& SurfaceMesh<ScalarType>::CellValidityFlagList()
{
    return m_MeshData->CellValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& SurfaceMesh<ScalarType>::CellValidityFlagList() const
{
    return m_MeshData->CellValidityFlagList;
}


template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Point(int_max PointIndex)
{
    return m_MeshData->PointList[PointIndex];
}

template<typename ScalarType>
inline
const Point_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Point(int_max PointIndex) const
{
    return m_MeshData->PointList[PointIndex];
}

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Vertex(int_max VertexIndex)
{
    return m_MeshData->VertexList[VertexIndex];
}

template<typename ScalarType>
inline
const Vertex_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Vertex(int_max VertexIndex) const
{
    return m_MeshData->VertexList[VertexIndex];
}

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Edge(int_max EdgeIndex)
{
    return m_MeshData->EdgeList[VertexIndex];
}

template<typename ScalarType>
inline
const Edge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Edge(int_max EdgeIndex) const
{
    return m_MeshData->EdgeList[VertexIndex];
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(int_max EdgeIndex, int_max RelativeIndex)
{
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename ScalarType>
inline
const DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(int_max EdgeIndex, int_max RelativeInde) const
{
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    return m_MeshData->DirectedEdgePairList[DirectedEdgeIndex.EdgeIndex][DirectedEdgeIndex.RelativeIndex];
}

template<typename ScalarType>
inline
const DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex) const
{
    return m_MeshData->DirectedEdgePairList[DirectedEdgeIndex.EdgeIndex][DirectedEdgeIndex.RelativeIndex];
}

template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Cell(int_max CellIndex);

template<typename ScalarType>
inline
const Cell_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Cell(int_max CellIndex) const;

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//
template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddPoint(ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddPoint(ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColNumber() - 1;

    Point_Of_SurfaceMesh<ScalarType> Point;
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
    Point.SetVertexIndex(-1); // not know yet, assume NonVertex
    Point.SetEdgeIndex(-1); // not know yet

    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

    return PointIndex;
}


template<typename ScalarType>
DenseVector<int_max> SurfaceMesh<ScalarType>::AddPoint(const DenseMatrix<ScalarType>& PointSet)
{
    DenseVector<int_max> PointIndexList;

    if (PointSet.GetColNumber() != 3)
    {
        MDK_Error("Invalid PointSet @ SurfaceMesh::AddPoint(...)")
        return PointIndexList;
    }

    PointIndexList.Resize(PointSet.GetColNumber());
    for (int_max k = 0; k < PointSet.GetColNumber(); ++k)
    {
        PointIndexList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointIndexList;
}


template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddVertex(int_max PointIndex)
{
    if (PointIndex < 0 || PointIndex >= m_MeshData->PointValidityFlagList.GetLength())
    {
        MDK_Error("Invalid PointIndex @ SurfaceMesh::AddVertex(...)")
        return -1;
    }

    if (m_MeshData->PointValidityFlagList[PointIndex] < 0)
    {
        MDK_Error("The point has not been added: PointIndex = " << PointIndex)
        return;
    }

    if (m_MeshData->PointList[PointIndex].IsVertex() == true)
    {
        MDK_Warning("The point has been marked as Vertex: PointIndex = " << PointIndex)
        return;
    }

    //----------------------------------------------------------------------------
    auto VertexIndex = m_MeshData->VertexList.GetLength();

    Vertex_Of_SurfaceMesh<ScalarType> Vertex;
    Vertex.SetParentMesh(*this);
    Vertex.SetIndex(VertexIndex);
    Vertex.SetPointIndex(PointIndex)

    m_MeshData->VertexList.Append(std::move(Vertex));
    m_MeshData->VertexValidityFlagList.Append(1);

    // update information in m_MeshData->PointList[PointIndex]
    m_MeshData->PointList[PointIndex].MarkAsVertex();
    m_MeshData->PointList[PointIndex].SetEdgeIndex(-1);

    return VertexIndex;
}


template<typename ScalarType>
DenseVector<int_max> SurfaceMesh<ScalarType>::AddVertexSet(DenseVector<int_max> PointIndexList)
{
    DenseVector<int_max> IndexList;

    if (PointIndexList.IsEmpty() == true)
    {
        MDK_Warning("PointIndexList is empty @ SurfaceMesh::AddVertexSet(...)")
        return IndexList;
    }

    IndexList.Resize(PointIndexList.GetLength());

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        IndexList[k] = this->AddVertex(PointIndexList[k]);
    }

    return IndexList;
}


template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddEdge(int_max VertexIndex0, int_max VertexIndex1, DenseVector<int_max> PointIndexList)
{
    if (m_MeshData->VertexValidityFlagList[VertexIndex0] < 0)
    {
        MDK_Error("VertexIndex0 is invalid @ SurfaceMesh::AddEdge(...)")
        return -1;
    }

    if (m_MeshData->VertexValidityFlagList[VertexIndex1] < 0)
    {
        MDK_Error("VertexIndex1 is invalid @ SurfaceMesh::AddEdge(...)")
        return -1;
    }

    if (PointIndexList.GetLength() < 2 )
    {
        MDK_Error("PointIndexList is empty @ SurfaceMesh::AddEdge(...)")
        return -1;
    }

    auto PointIndex_a = m_MeshData->VertexList[VertexIndex0].GetPointIndex();
    auto PointIndex_b = m_MeshData->VertexList[VertexIndex1].GetPointIndex();

    if (PointIndexList[0] != PointIndex_a || PointIndexList[PointIndexList.GetLength() - 1] != PointIndex_b)
    {
        MDK_Error("PointIndexList[0] and PointIndexList[end] is invalid @ SurfaceMesh::AddEdge(...)")
        return -1;
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 0)
        {
            MDK_Error("PointIndex is invalid @ SurfaceMesh::AddEdge(...), PointIndex = " << m_MeshData->PointValidityFlagList[k])
            return -1;
        }

        if (k > 1 && k < PointIndexList.GetLength() - 1)
        {
            if (m_MeshData->PointList[PointIndexList[k]].IsVertex() == true)
            {
                MDK_Error("A middle point of PointIndexList is a vertex @ SurfaceMesh::AddEdge(...), PointIndex = " << PointIndexList[k])
                return -1;
            }
        }
    }

    //----------------------------------------------------------------
    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_SurfaceMesh<ScalarType> Edge;
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.PointIndexList() = PointIndexList;

    m_MeshData->EdgeList[EdgeIndex].Append(std::move(Edge));

    // add empty DirectedEdgePair to hold place
    DenseVector<DirectedEdge_Of_SurfaceMesh<ScalarType>, 2> DirectedEdgePair;
    m_MeshData->DirectedEdgePairList.Append(std::move(DirectedEdgePair));
    m_MeshData->DirectedEdgePairList[m_MeshData->DirectedEdgePairList.GetLength()-1].Delete;

    m_MeshData->EdgeValidityFlagList.Append(1);

    // update information in m_MeshData->VertexList[VertexIndex0] and m_MeshData->VertexList[VertexIndex1]

    m_MeshData->VertexList[VertexIndex0].AdjacentVertexIndexList().Append(VertexIndex1);
    m_MeshData->VertexList[VertexIndex0].AdjacentEdgeIndexList().Append(EdgeIndex);

    m_MeshData->VertexList[VertexIndex1].AdjacentVertexIndexList().Append(VertexIndex0);
    m_MeshData->VertexList[VertexIndex1].AdjacentEdgeIndexList().Append(EdgeIndex);

    // update information in m_MeshData->PointList
    if (PointIndexList.GetLength() > 2)
    {
        for (int_max k = 1; k < PointIndexList.GetLength()-1; ++k)
        {
            m_MeshData->PointList[PointIndexList[k]].SetEdgeIndex(EdgeIndex);
        }
    }

    //------------
    return EdgeIndex;
}


template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddCell(const DenseVector<int_max>& EdgeIndexList)
{
    if (EdgeIndexList.GetLength() < 2)
    {
        MDK_Error("length of EdgeIndexList < 2 @ SurfaceMesh::AddCell(...)")
        return -1;
    }

    for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[EdgeIndexList[k]] == 0)
        {
            MDK_Error("Invalid EdgeIndex @ SurfaceMesh::AddCell(...)" << EdgeIndexList[k])
            return -1;
        }
    }

    // the input EdgeIndexList must have been ordered
    
    DenseVector<int_max> VertexIndexList_order;
    VertexIndexList_order.Resize(EdgeIndexList.GetLength());

    for (int_max k = 0; k < EdgeIndexList.GetLength()-1; ++k)
    {
        int_max tempVertexIndexList_a[2];
        m_MeshData->EdgeList[k].GetVertexIndexList(tempVertexIndexList_a);

        int_max tempVertexIndexList_b[2];
        m_MeshData->EdgeList[k + 1].GetVertexIndexList(tempVertexIndexList_b);

        if (tempVertexIndexList_a[0] == tempVertexIndexList_b[0] && tempVertexIndexList_a[1] != tempVertexIndexList_b[1])
        {           
            VertexIndexList_order[k] = tempVertexIndexList_a[1];
            VertexIndexList_order[k+1] = tempVertexIndexList_a[0];
        }
        else if (tempVertexIndexList_a[0] == tempVertexIndexList_b[1] && tempVertexIndexList_a[1] != tempVertexIndexList_b[0])
        {
            VertexIndexList_order[k] = tempVertexIndexList_a[1];
            VertexIndexList_order[k+1] = tempVertexIndexList_a[0];
        }
        else if (tempVertexIndexList_a[1] == tempVertexIndexList_b[0] && tempVertexIndexList_a[0] != tempVertexIndexList_b[1])
        {
            VertexIndexList_order[k] = tempVertexIndexList_a[0];
            VertexIndexList_order[k+1] = tempVertexIndexList_a[1];
        }
        else if (tempVertexIndexList_a[1] == tempVertexIndexList_b[1] && tempVertexIndexList_a[0] != tempVertexIndexList_b[0])
        {
            VertexIndexList_order[k] = tempVertexIndexList_a[0];
            VertexIndexList_order[k+1] = tempVertexIndexList_a[1];
        }
        else
        {
            MDK_Error("EdgeIndexList is invalid @ SurfaceMesh::AddCell(...)")
            return -1;
        }
    }

    //-------------------------------------------------------
    auto CellIndex = m_MeshData->CellList.GetLength();

    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> DirectedEdgeIndexList;
    DirectedEdgeIndexList.Resize(EdgeIndexList.GetLength());

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto& DirectedEdge0 = m_MeshData->DirectedEdgePairList[EdgeIndexList[k]][0];
        auto& DirectedEdge1 = m_MeshData->DirectedEdgePairList[EdgeIndexList[k]][1];

        if (DirectedEdge0.IsValid() == false)
        {
            DirectedEdgeIndexList[k].RelativeIndex = 0;
        }
        else if (DirectedEdge1.IsValid() == false)
        {
            DirectedEdgeIndexList[k].RelativeIndex = 1;
        }
        else
        {
            MDK_Error("DirectedEdgePairList is wrong @ SurfaceMesh::AddCell(...)")
            return -1;
        }

        DirectedEdgeIndexList[k].EdgeIndex = EdgeIndexList[k];
    }

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];
        tempDirectedEdge.Create();
        tempDirectedEdge.SetParentMesh(*this);
        tempDirectedEdge.SetIndex(DirectedEdgeIndexList[k]);
        tempDirectedEdge.SetStartVertexIndex(VertexIndexList_order[k]);

        if (k<DirectedEdgeIndexList.GetLength()-1)
        { 
            tempDirectedEdge.SetEndVertexIndex(VertexIndexList_order[k+1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetEndVertexIndex(VertexIndexList_order[0]); 
        }

        tempDirectedEdge.SetCellIndex(CellIndex);

        if (k > 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[k-1]);
        }
        else // if (k == 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[DirectedEdgeIndexList.GetLength()-1]);
        }

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[k+1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[0]);
        }
    }

    // create cell --------------------------------------------------------------

    Cell_Of_SurfaceMesh<ScalarType> Cell;
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.DirectedEdgeIndexList() = std::move(DirectedEdgeIndexList);

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);

    // update information in m_MeshData->VertexList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempVertexIndex_start = DirectedEdgeIndexList[k].GetStartVertexIndex();
        auto tempVertexIndex_end = DirectedEdgeIndexList[k].GetEndVertexIndex();
        m_MeshData->VertexList[tempVertexIndex_start].OutgoingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
        m_MeshData->VertexList[tempVertexIndex_end].IncomingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
    }

    for (int_max k = 0; k < VertexIndexList_order.GetLength(); ++k)
    {
        m_MeshData->VertexList[VertexIndexList_order[k]].AdjacentCellIndexList().Append(CellIndex);
    }

    //-----------------------------
    return CellIndex;
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeleteCell(int_max CellIndex)
{

}


template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeleteDirectedEdge(int_max DirectedEdgeIndex)
{

}


template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeleteEdge(int_max EdgeIndex)
{

}


template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeletePoint(int_max PointIndex)
{

}


template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeletePoint(const DenseVector<int_max>& PointIndexList)
{

}

//----------------- remove deleted item from object list ----------------------------------------------------------//
// attention: after this function is called, every index will be changed
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
template<typename ScalarType>
void SurfaceMesh<ScalarType>::CleanDataStructure()
{

}
//---------------------------------------------------------------------------

}// namespace mdk

#endif