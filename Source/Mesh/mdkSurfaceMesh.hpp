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
    m_MeshData->PointList.Clear();
    m_MeshData->PointValidityFlagList.Clear();

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
int_max SurfaceMesh<ScalarType>::GetPointNumber() const
{
    return m_MeshData->PointValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max SurfaceMesh<ScalarType>::GetVertexNumber() const
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
int_max SurfaceMesh<ScalarType>::GetEdgeNumber() const
{
    return m_MeshData->EdgeValidityFlagList.Sum();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType> SurfaceMesh<ScalarType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const;
{
    DenseMatrix<ScalarType> PointPositionMatrix;

    this->GetPointPosition(PointPositionMatrix, PointHandleList);
}


template<typename ScalarType>
inline 
void SurfaceMesh<ScalarType>::
GetPointPosition(DenseMatrix<ScalarType>& PointPositionMatrix, const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    if (PointHandleList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    PointPositionMatrix.FastResize(3, PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].Index;
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex))
    }
}


template<typename ScalarType>
inline
void SurfaceMesh<ScalarType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix) const
{
    if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == true)
    {
        return;
    }
    else if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == false)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetPointPosition(...)")
        return;
    }
    else if (PointHandleList.IsEmpty() == false && PointPositionMatrix.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetPointPosition(...)")
        return;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].Index;
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::SetPointPosition(...)")
            return;
        }

        m_MeshData->PointPositionTable.SetCol(PointIndex, PointPositionMatrix.GetPointerOfCol(k));
    }
}

//------------------------- Mesh Element ----------------------------------------------

template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return m_MeshData->PointList[PointHandle.Index];
}

template<typename ScalarType>
inline
const Point_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return m_MeshData->PointList[PointHandle.Index];
}

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    return m_MeshData->VertexList[VertexHandle.Index];
}

template<typename ScalarType>
inline
const Vertex_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const
{
    return m_MeshData->VertexList[VertexHandle.Index];
}

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return m_MeshData->EdgeList[EdgeHandle.Index];
}

template<typename ScalarType>
inline
const Edge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return m_MeshData->EdgeList[EdgeHandle.Index];
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle)
{
    auto EdgeIndex = DirectedEdgeHandle.Index.EdgeIndex;
    auto RelativeIndex = DirectedEdgeHandle.Index.RelativeIndex;
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename ScalarType>
inline
const DirectedEdge_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.Index.EdgeIndex;
    auto RelativeIndex = DirectedEdgeHandle.Index.RelativeIndex;
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}


template<typename ScalarType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<ScalarType>::GetPointHandle(int_max PointID) const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;

    int_max PointIndex = -1;
    try
    {
        PointIndex = m_MeshData->Map_PointID_to_PointIndex.at(PointID)
    }
    catch (...)
    {
        MDK_Error("Invalid PointID @ SurfaceMesh::GetPointHandle(...)")
        PointHandle.Index = -1;
        return PointHandle;
    }
        
    PointHandle.Index = PointIndex;
    return PointHandle;
}


template<typename ScalarType>
inline 
Handle_Of_Vertex_Of_SurfaceMesh SurfaceMesh<ScalarType>::GetVertexHandle(int_max VertexID) const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;

    int_max VertexIndex = -1;
    try
    {
        VertexIndex = m_MeshData->Map_VertexID_to_VertexIndex.at(PointID)
    }
    catch (...)
    {
        MDK_Error("Invalid VertexID @ SurfaceMesh::GetVertexHandle(...)")
        VertexHandle.Index = -1;
        return;
    }

    VertexHandle.Index = VertexIndex;
    return VertexHandle;
}


template<typename ScalarType>
inline
Handle_Of_Edge_Of_SurfaceMesh   SurfaceMesh<ScalarType>::GetEdgeHandle(int_max EdgeID) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    int_max EdgeIndex = -1;
    try
    {
        EdgeIndex = m_MeshData->Map_EdgeID_to_EdgeIndex.at(EdgeID)
    }
    catch (...)
    {
        MDK_Error("Invalid EdgeID @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.Index = -1;
        return;
    }
    
    EdgeHandle.Index = EdgeIndex;
    return EdgeHandle;
}


template<typename ScalarType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<ScalarType>::GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;

    if (RelativeIndex != 0 && RelativeIndex != 1)
    {
        MDK_Error("Invalid RelativeIndex @ SurfaceMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.Index.EdgeIndex = -1;
        DirectedEdgeHandle.Index.RelativeIndex = -1;
        return DirectedEdgeHandle;
    }

    int_max EdgeIndex = -1;
    try
    {
        EdgeIndex = m_MeshData->Map_EdgeID_to_EdgeIndex.at(EdgeID)
    }
    catch (...)
    {
        MDK_Error("Invalid EdgeID @ SurfaceMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.Index.EdgeIndex = -1;
        DirectedEdgeHandle.Index.RelativeIndex = -1;
        return DirectedEdgeHandle;
    }

    DirectedEdgeHandle.Index.EdgeIndex = EdgeIndex;
    DirectedEdgeHandle.Index.RelativeIndex = RelativeIndex;
    return DirectedEdgeHandle;
}


template<typename ScalarType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<ScalarType>::GetCellHandle(int_max CellID) const
{
    Handle_Of_Vertex_Of_SurfaceMesh CellHandle;

    int_max CellIndex = -1;
    try
    {
        CellIndex = m_MeshData->Map_CellID_to_CellIndex.at(PointID)
    }
    catch (...)
    {
        MDK_Error("Invalid CellID @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.Index = -1;
        return;
    }

    CellHandle.Index = CellIndex;
    return CellHandle;
}


template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return m_MeshData->CellList[CellHandle.Index];
}

template<typename ScalarType>
inline
const Cell_Of_SurfaceMesh<ScalarType>& SurfaceMesh<ScalarType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return m_MeshData->CellList[CellHandle.Index];
}

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//
template<typename ScalarType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<ScalarType>::AddPoint(ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<ScalarType>::AddPoint(ScalarType x, ScalarType y, ScalarType z)
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

    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.Index = PointIndex;

    return PointHandle;
}


template<typename ScalarType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> SurfaceMesh<ScalarType>::AddPoint(const DenseMatrix<ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;

    if (PointSet.GetColNumber() != 3)
    {
        MDK_Error("Invalid PointSet @ SurfaceMesh::AddPoint(...)")
            return PointHandleList;
    }

    PointIndexList.Resize(PointSet.GetColNumber());
    for (int_max k = 0; k < PointSet.GetColNumber(); ++k)
    {
        PointHandleList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointHandleList;
}


template<typename ScalarType>
int_max SurfaceMesh<ScalarType>::AddVertex(int_max PointIndex)
{
    if (PointIndex < 0 || PointIndex >= m_MeshData->PointValidityFlagList.GetLength())
    {
        MDK_Error("Invalid PointIndex @ SurfaceMesh::AddVertex(...)")
        return -1;
    }

    if (m_MeshData->PointValidityFlagList[PointIndex] == 0)
    {
        MDK_Error("The point has been deleted: PointIndex = " << PointIndex)
        return -1;
    }

    if (m_MeshData->PointList[PointIndex].IsVertex() == true)
    {
        MDK_Warning("The point has been marked as Vertex: PointIndex = " << PointIndex)
        return m_MeshData->PointList[PointIndex].GetVertexIndex();
    }

    //----------------------------------------------------------------------------
    auto VertexIndex = m_MeshData->VertexList.GetLength();

    Vertex_Of_SurfaceMesh<ScalarType> Vertex;
    Vertex.SetParentMesh(*this);
    Vertex.SetIndex(VertexIndex);
    Vertex.SetPointIndex(PointIndex)

    m_MeshData->VertexList.Append(std::move(Vertex));
    m_MeshData->VertexValidityFlagList.Append(1);

    m_MeshData->VertexGlobalIDList.Append(-1);

    // update information in m_MeshData->PointList[PointIndex]
    m_MeshData->PointList[PointIndex].SetVertexIndex(VertexIndex);

    //--------------------------
    return VertexIndex;
}


template<typename ScalarType>
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<ScalarType>::AddEdge(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    if (PointHandleList.GetLength() < 2)
    {
        MDK_Error("PointHandleList length < 2 @ SurfaceMesh::AddEdge(...)")
        return -1;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[PointHandleList[k].Index] == 0)
        {
            MDK_Error("PointIndex is invalid: " << PointHandleList[k].Index << " @ SurfaceMesh::AddEdge(...)")
            return -1;
        }
    }

    //----------------------------------

    if (PointIndexList.GetLength() > 2)
    {
        DenseVector<int_max> tempEdgeIndexList;
        tempEdgeIndexList.Resize(PointHandleList.GetLength());
        tempEdgeIndexList.Fill(-1);

        for (int_max k = 1; k < PointHandleList.GetLength() - 1; ++k)
        {
            if (m_MeshData->PointList[PointHandleList[k].Index].IsVertex() == true)
            {
                MDK_Error("A middle point of PointIndexList is a vertex, PointIndex = " << PointIndexList[k] << " @ SurfaceMesh::AddEdge(...)")
                return -1;
            }

            tempEdgeIndexList[k] = m_MeshData->PointList[PointHandleList[k].Index].GetEdgeIndex();
        }

        int_max tempValue = 0;
        for (int_max k = 2; k < PointHandleList.GetLength() - 1; ++k)
        {
            tempValue += std::abs(tempEdgeIndexList[k] - tempEdgeIndexList[k - 1]);
        }

        if (tempValue != 0)
        {
            MDK_Error("A middle point of PointIndexList is already on an edge @ SurfaceMesh::AddEdge(...)")
            return -1;
        }
        else
        {
            if (tempEdgeIndexList[1] >= 0)
            {
                MDK_Warning("The Edge has alreadly been added @ SurfaceMesh::AddEdge(...)")
                return tempEdgeIndexList[1];
            }
        }
    }

    //----------------------------------

    auto VertexIndex0 = m_MeshData->PointList[PointHandleList[0].Index].GetVertexIndex();
    auto VertexIndex1 = m_MeshData->PointList[PointHandleList[PointIndexList.GetLength() - 1].Index].GetVertexIndex();

    if (VertexIndex0 >= 0)
    {
        auto PointIndex_a = m_MeshData->VertexList[VertexIndex0].GetPointIndex();
        if (PointIndexList[0] != PointIndex_a)
        {
            MDK_Error("PointIndexList[0] is invalid @ SurfaceMesh::AddEdge(...)")
            return -1;
        }
    }
    
    if (VertexIndex1 >= 0)
    {
        auto PointIndex_b = m_MeshData->VertexList[VertexIndex1].GetPointIndex();
        if (PointIndexList[PointIndexList.GetLength() - 1] != PointIndex_b)
        {
            MDK_Error("PointIndexList[end] is invalid @ SurfaceMesh::AddEdge(...)")
            return -1;
        }
    }

    //-------------------------------------------------------------------------------------
    // add new vertex if necessary    
    bool VertexIndex0_is_new = false;
    bool VertexIndex1_is_new = false;

    if (VertexIndex0 < 0)
    {
        VertexIndex0 = this->AddVertex(PointHandleList[0].Index);
        VertexIndex0_is_new = true;
    }

    if (VertexIndex1 < 0)
    {
        VertexIndex1 = this->AddVertex(PointHandleList[PointIndexList.GetLength() - 1].Index);
        VertexIndex1_is_new = true;
    }
    //----------------------------------------------------------------
    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_SurfaceMesh<ScalarType> Edge;
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.PointIndexList().Resize(PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        Edge.PointIndexList()[k] = PointHandleList[k].Index;
    }

    m_MeshData->EdgeList[EdgeIndex].Append(std::move(Edge));

    // add empty DirectedEdgePair to hold place
    DenseVector<DirectedEdge_Of_SurfaceMesh<ScalarType>, 2> DirectedEdgePair;
    DirectedEdgePair[0].Clear();
    DirectedEdgePair[1].Clear();
    m_MeshData->DirectedEdgePairList.Append(std::move(DirectedEdgePair));

    m_MeshData->EdgeValidityFlagList.Append(1);

    // update information in m_MeshData->VertexList[VertexIndex0] and m_MeshData->VertexList[VertexIndex1]
    if (VertexIndex0_is_new == true)
    {
        m_MeshData->VertexList[VertexIndex0].AdjacentVertexIndexList().Append(VertexIndex1);
        m_MeshData->VertexList[VertexIndex0].AdjacentEdgeIndexList().Append(EdgeIndex);
    }

    if (VertexIndex1_is_new == true)
    {
        m_MeshData->VertexList[VertexIndex1].AdjacentVertexIndexList().Append(VertexIndex0);
        m_MeshData->VertexList[VertexIndex1].AdjacentEdgeIndexList().Append(EdgeIndex);
    }

    // update information in m_MeshData->PointList
    if (PointHandleList.GetLength() > 2)
    {
        for (int_max k = 1; k < PointHandleList.GetLength() - 1; ++k)
        {
            m_MeshData->PointList[PointHandleList[k].Index].SetEdgeIndex(EdgeIndex);
        }
    }

    //------------
    return EdgeIndex;
}


template<typename ScalarType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<ScalarType>::AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (EdgeIndexList.GetLength() < 2)
    {
        MDK_Error("length of EdgeIndexList < 2 @ SurfaceMesh::AddCell(...)")
        CellHandle.Index = -1;
        return CellHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[EdgeHandleList[k]] == 0)
        {
            MDK_Error("Invalid EdgeIndex: " << EdgeHandleList[k] << " @ SurfaceMesh::AddCell(...)")
            CellHandle.Index = -1;
            return CellHandle;
        }
    }

    // the input EdgeIndexList must have been ordered
    
    DenseVector<int_max> VertexIndexList;
    VertexIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto tempEdgeIndex = EdgeHandleList[k].Index;

        int_max tempVertexIndexList_a[2];
        m_MeshData->EdgeList[tempEdgeIndex].GetVertexIndexList(tempVertexIndexList_a);

        int_max tempVertexIndexList_b[2];
        m_MeshData->EdgeList[tempEdgeIndex + 1].GetVertexIndexList(tempVertexIndexList_b);

        if (tempVertexIndexList_a[0] == tempVertexIndexList_b[0] && tempVertexIndexList_a[1] != tempVertexIndexList_b[1])
        {           
            VertexIndexList[k] = tempVertexIndexList_a[1];
            VertexIndexList[k + 1] = tempVertexIndexList_a[0];
        }
        else if (tempVertexIndexList_a[0] == tempVertexIndexList_b[1] && tempVertexIndexList_a[1] != tempVertexIndexList_b[0])
        {
            VertexIndexList[k] = tempVertexIndexList_a[1];
            VertexIndexList[k + 1] = tempVertexIndexList_a[0];
        }
        else if (tempVertexIndexList_a[1] == tempVertexIndexList_b[0] && tempVertexIndexList_a[0] != tempVertexIndexList_b[1])
        {
            VertexIndexList[k] = tempVertexIndexList_a[0];
            VertexIndexList[k + 1] = tempVertexIndexList_a[1];
        }
        else if (tempVertexIndexList_a[1] == tempVertexIndexList_b[1] && tempVertexIndexList_a[0] != tempVertexIndexList_b[0])
        {
            VertexIndexList[k] = tempVertexIndexList_a[0];
            VertexIndexList[k + 1] = tempVertexIndexList_a[1];
        }
        else
        {
            MDK_Error("EdgeIndexList is invalid @ SurfaceMesh::AddCell(...)")
            return -1;
        }
    }

    // get DirectedEdgeIndexList -------------------------------------------------------
    auto CellIndex = m_MeshData->CellList.GetLength();

    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> DirectedEdgeIndexList;
    DirectedEdgeIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto& DirectedEdge0 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].Index][0];
        auto& DirectedEdge1 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].Index][1];

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

        DirectedEdgeIndexList[k].EdgeIndex = EdgeHandleList[k].Index;
    }

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // attention: auto& will get reference, auto will copy
        auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];

        tempDirectedEdge.Create();
        tempDirectedEdge.SetParentMesh(*this);
        tempDirectedEdge.SetIndex(DirectedEdgeIndexList[k]);
        tempDirectedEdge.SetCellIndex(CellIndex);
        tempDirectedEdge.SetStartVertexIndex(VertexIndexList[k]);

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        { 
            tempDirectedEdge.SetEndVertexIndex(VertexIndexList[k + 1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetEndVertexIndex(VertexIndexList[0]);
        }

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[k + 1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[0]);
        }

        if (k > 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[k-1]);
        }
        else // if (k == 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[DirectedEdgeIndexList.GetLength()-1]);
        }
    }

    // create cell --------------------------------------------------------------

    Cell_Of_SurfaceMesh<ScalarType> Cell;
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);

    m_MeshData->CellGlobalIDList.Append(-1);

    // update information in m_MeshData->VertexList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[DirectedEdgeIndexList[k].EdgeIndex][DirectedEdgeIndexList[k].RelativeIndex];
        auto tempVertexIndex_start = tempDirectedEdge.GetStartVertexIndex();
        auto tempVertexIndex_end = tempDirectedEdge.GetEndVertexIndex();
        m_MeshData->VertexList[tempVertexIndex_start].OutgoingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
        m_MeshData->VertexList[tempVertexIndex_end].IncomingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
    }

    for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
    {
        m_MeshData->VertexList[VertexIndexList[k]].AdjacentCellIndexList().Append(CellIndex);
    }

    //-----------------------------
    CellHandle.Index = CellIndex;
    return CellHandle;
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename ScalarType>
bool SurfaceMesh<ScalarType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    auto CellIndex = CellHandle.Index;

    if (CellIndex < 0 || CellIndex >= m_MeshData->CellList.GetLength())
    {
        MKD_Error("Invalid CellIndex @ SurfaceMesh::DeleteCell(...)")
        return false;
    }

    if (m_MeshData->CellValidityFlagList[CellIndex] == 0)
    {
        MKD_Warning("The cell has been deleted already, CellIndex = " << CellIndex  << "@ SurfaceMesh::DeleteCell(...)")
        return true;
    }

    //--------------------------------------------------------------------------------------
    // auto& will get reference
    const auto& DirectedEdgeIndexList = m_MeshData->CellList[CellIndex].GetDirectedEdgeIndexList();
    const auto& VertexIndexList = m_MeshData->CellList[CellIndex].GetVertexIndexList();

    // update information in m_MeshData->VertexList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];
        auto tempVertexIndex_start = tempDirectedEdge.GetStartVertexIndex();
        auto tempVertexIndex_end = tempDirectedEdge.GetEndVertexIndex();

        auto tempIndex_a = m_MeshData->VertexList[tempVertexIndex_start].OutgoingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index.EdgeIndex == tempEdgeIndex && Index.RelativeIndex == tempRelativeIndex);
        });
        m_MeshData->VertexList[tempVertexIndex_start].OutgoingDirectedEdgeIndexList().Delete(tempIndex_a);

        auto tempIndex_b = m_MeshData->VertexList[tempVertexIndex_end].IncomingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index.EdgeIndex == tempEdgeIndex && Index.RelativeIndex == tempRelativeIndex);
        });
        m_MeshData->VertexList[tempVertexIndex_end].IncomingDirectedEdgeIndexList().Delete(tempIndex_b);
    }

    for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_MeshData->VertexList[VertexIndexList[k]].AdjacentCellIndexList().Find([&](int_max Index) {return Index == CellIndex});
        m_MeshData->VertexList[VertexIndexList[k]].AdjacentCellIndexList().Delete(tempIndex);
    }

    // Delete DirectedEdge: only clear memory, not remove from DirectedEdgePairList
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); k++)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].Clear();
    }

    // Delete Cell : only clear memory, not remove from CellList
    m_MeshData->CellList[CellIndex].Clear();
    m_MeshData->CellValidityFlagList[CellIndex] = 0;

    m_MeshData->CellGlobalIDList[CellIndex] = -1;
    //---------------------------------------------

    return true;
}


template<typename ScalarType>
bool SurfaceMesh<ScalarType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    auto EdgeIndex = EdgeHandle.Index;

    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeList.GetLength())
    {
        MDK_Error("Invalid EdgeIndex @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }

    if (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 0)
    {
        MDK_Warning("The edge has been deleted already @ SurfaceMesh::DeleteEdge(...)")
        return true;
    }

    const auto& AdjacentCellIndexList = m_MeshData->EdgeList[EdgeIndex].GetAdjacentCellIndexList();
    if (AdjacentCellIndexList.IsEmpty() == false)
    {
        MDK_Error("AdjacentCellIndexList is not empty, so this edge can not be deleted @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }

    //---------------------------------------------------------------------------------------------
    int_max VertexIndexList[2];
    m_MeshData->EdgeList[EdgeIndex].GetVertexIndexList(VertexIndexList);

    const auto& PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();

    // update information in m_MeshData->VertexList[VertexIndex0] and m_MeshData->VertexList[VertexIndex1]
    for (int_max k = 0; k < 2; ++k)
    {
        auto tempIndex = m_MeshData->VertexList[VertexIndexList[k]].AdjacentEdgeIndexList().Find([&](int_max Index) { return (Index == EdgeIndex); });
        m_MeshData->VertexList[VertexIndexList[k]].AdjacentEdgeIndexList().Delete(tempIndex);
    }

    // Delete Vertex if necessary
    for (int_max k = 0; k < 2; ++k)
    {
        const auto& tempIndexList = m_MeshData->VertexList[VertexIndexList[k]].AdjacentEdgeIndexList();
        if (tempIndexList.IsEmpty() == true)
        {
            this->DeleteVertex(VertexIndexList[k]);
        }
    }

    // update information in m_MeshData->PointList
    if (PointIndexList.GetLength() > 2)
    {
        for (int_max k = 1; k < PointIndexList.GetLength() - 1; ++k)
        {
            m_MeshData->PointList[PointIndexList[k]].SetEdgeIndex(-1);
        }
    }

    // Delete Edge: only clear memory, no remove from EdgeList
    m_MeshData->EdgeList[EdgeIndex].Clear();
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
    //--------------------------------------------------------

    return true;
}


template<typename ScalarType>
bool SurfaceMesh<ScalarType>::DeleteVertex(int_max VertexIndex)
{
    if (VertexIndex < 0 || VertexIndex >= m_MeshData->VertexList.GetLength())
    {
        MDK_Error("Invalid VertexIndex @ SurfaceMesh::DeleteVertex(...)")
        return false;
    }

    if (m_MeshData->VertexValidityFlagList[VertexIndex] == 0)
    {
        MDK_Warning("The vertex has been deleted already @ SurfaceMesh::DeleteVertex(...)")
        return true;
    }

    const auto& AdjacentEdgeIndexList = m_MeshData->VertexList[VertexIndex].AdjacentEdgeIndexList();
    if (AdjacentEdgeIndexList.IsEmpty() == false)
    {
        MDK_Error("AdjacentEdgeIndexList is not empty, so this vertex can not be deleted @ SurfaceMesh::DeleteVertex(...)")
        return false;
    }

    //-------------------------------------------------------------------
    auto PointIndex = m_MeshData->VertexList[VertexIndex].GetPointIndex();

    // update information in m_MeshData->PointList[PointIndex]
    m_MeshData->PointList[PointIndex].SetEdgeIndex(-1);
    m_MeshData->PointList[PointIndex].MarkAsNonVertex();

    // Delete Vertex : only clear memory, not remove from VertexList
    m_MeshData->VertexList[VertexIndex].Clear();
    m_MeshData->VertexValidityFlagList[VertexIndex] = 0;

    m_MeshData->VertexGlobalIDList[VertexIndex] = -1;
    //-----------------------------------------------------

    return true;
}


template<typename ScalarType>
void SurfaceMesh<ScalarType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    auto PointIndex = PointHandle.Index;

    if (PointIndex < 0 || PointIndex >= m_MeshData->PointList.GetLength())
    {
        MDK_Error("Invalid PointIndex @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    if (m_MeshData->PointValidityFlagList[PointIndex] == 0)
    {
        MDK_Warning("The point has been deleted alreay @ SurfaceMesh::DeletePoint(...)")
        return true;
    }

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    //--------------------------------------------------------------------
    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

    m_MeshData->PointList[PointIndex].Clear();
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
    //---------------------------------------------------------------------
    return true;
}


template<typename ScalarType>
bool SurfaceMesh<ScalarType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    if (PointHandleList.IsEmpty() == true)
    {
        MDK_Warning("PointHandleList is empty @ SurfaceMesh::DeletePoint(...)")
        return true;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->DeletePoint(PointHandleList[k]) == false)
        {
            return false;
        }
    }

    return true;
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