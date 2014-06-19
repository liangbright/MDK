#ifndef __mdkSurfaceMesh_hpp
#define __mdkSurfaceMesh_hpp

namespace mdk
{

template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh()
{
    m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&)
{
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::~SurfaceMesh()
{
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::operator=(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::operator=(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
    }

    this->Take(std::forward<SurfaceMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->IsTriangleMesh = false;

    m_MeshData->PointIDList.Clear();
    m_MeshData->VertexIDList.Clear();
    m_MeshData->EdgeIDList.Clear();
    m_MeshData->CellIDList.Clear();

    Map_PointID_to_PointIndex.clear();
    Map_PointID_to_PointIndex.shrink_to_fit();
    Map_VertexID_to_VertexIndex.clear();
    Map_VertexID_to_VertexIndex.shrink_to_fit();
    Map_EdgeID_to_EdgeIndex.clear();
    Map_EdgeID_to_EdgeIndex.shrink_to_fit();
    Map_CellID_to_CellIndex.clear();
    Map_CellID_to_CellIndex.shrink_to_fit();

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


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Copy(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
    }

    if (!InputMesh.m_MeshData)
    {
        return;
    }

    m_MeshData->PointIDList = InputMesh.m_MeshData->PointIDList;
    m_MeshData->VertexIDList = InputMesh.m_MeshData->VertexIDList;
    m_MeshData->EdgeIDList = InputMesh.m_MeshData->EdgeIDList;
    m_MeshData->CellIDList = InputMesh.m_MeshData->CellIDList;

    m_MeshData->Map_PointID_to_PointIndex = InputMesh.m_MeshData->Map_PointID_to_PointIndex;
    m_MeshData->Map_VertexID_to_VertexIndex = InputMesh.m_MeshData->Map_VertexID_to_VertexIndex;
    m_MeshData->Map_EdgeID_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex;
    m_MeshData->Map_CellID_to_CellIndex = InputMesh.m_MeshData->Map_CellID_to_CellIndex;

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


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Copy(const SurfaceMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Share(SurfaceMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Share(SurfaceMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::ForceShare(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::ForceShare(const SurfaceMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    Take(std::forward<SurfaceMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData->PointIDList = std::move(InputMesh.m_MeshData->PointIDList);
    m_MeshData->VertexIDList = std::move(InputMesh.m_MeshData->VertexIDList);
    m_MeshData->EdgeIDList = std::move(InputMesh.m_MeshData->EdgeIDList);
    m_MeshData->CellIDList = std::move(InputMesh.m_MeshData->CellIDList);

    m_MeshData->Map_PointID_to_PointIndex = std::move(InputMesh.m_MeshData->Map_PointID_to_PointIndex);
    m_MeshData->Map_VertexID_to_VertexIndex = std::move(InputMesh.m_MeshData->Map_VertexID_to_VertexIndex);
    m_MeshData->Map_EdgeID_to_EdgeIndex = std::move(InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex);
    m_MeshData->Map_CellID_to_CellIndex = std::move(InputMesh.m_MeshData->Map_CellID_to_CellIndex);

    m_MeshData->PointPositionTable = std::move(InputMesh.m_MeshData->PointPositionTable);

    m_MeshData->PointValidityFlagList = std::move(InputMesh.m_MeshData->PointValidityFlagList);
    m_MeshData->PointList = std::move(InputMesh.m_MeshData->PointList);
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            m_MeshData->PointList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->VertexValidityFlagList = std::move(InputMesh.m_MeshData->VertexValidityFlagList);
    m_MeshData->VertexList = std::move(InputMesh.m_MeshData->VertexList);
    for (int_max k = 0; k < m_MeshData->VertexList.GetLength(); ++k)
    {
        if (m_MeshData->VertexValidityFlagList[k] == 1)
        {
            m_MeshData->VertexList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->EdgeValidityFlagList = std::move(InputMesh.m_MeshData->EdgeValidityFlagList);
    m_MeshData->EdgeList = std::move(InputMesh.m_MeshData->EdgeList);
    m_MeshData->DirectedEdgePairList = std::move(InputMesh.m_MeshData->DirectedEdgePairList);
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            m_MeshData->EdgeList[k].SetParentMesh(*this);
            if (m_MeshData->DirectedEdgePairList[k][0].IsValid() == true)
            {
                m_MeshData->DirectedEdgePairList[k][0].SetParentMesh(*this);
            }
            if (m_MeshData->DirectedEdgePairList[k][1].IsValid() == true)
            {
                m_MeshData->DirectedEdgePairList[k][1].SetParentMesh(*this);
            }
        }
    }    

    m_MeshData->CellValidityFlagList = std::move(InputMesh.m_MeshData->CellValidityFlagList);
    m_MeshData->CellList = std::move(InputMesh.m_MeshData->CellList);
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            m_MeshData->CellList[k].SetParentMesh(*this);
        }
    }    
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>* InputMesh)
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

template<typename MeshAttributeType>
inline 
bool SurfaceMesh<MeshAttributeType>::IsEmpty() const
{
    return m_MeshData->PointPositionTable.IsEmpty();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_MeshData->PointValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetVertexNumber() const
{
    return m_MeshData->VertexValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetCellNumber() const
{
    return m_MeshData->CellValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return m_MeshData->EdgeValidityFlagList.Sum();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
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

template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType> 
SurfaceMesh<MeshAttributeType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    DenseMatrix<MeshAttributeType::ScalarType> PointPositionMatrix;

    this->GetPointPosition(PointPositionMatrix, PointHandleList);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
GetPointPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix, const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
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


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
SetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList, const DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix)
{
    if (VertexHandleList.IsEmpty() == true && VertexPositionMatrix.IsEmpty() == true)
    {
        return;
    }
    else if (VertexHandleList.IsEmpty() == true && VertexPositionMatrix.IsEmpty() == false)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetVertexPosition(...)")
        return;
    }
    else if (VertexHandleList.IsEmpty() == false && VertexPositionMatrix.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetVertexPosition(...)")
        return;
    }

    for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
    {
        auto VertexIndex = VertexHandleList[k].Index;
        auto PointIndex = m_MeshData->VertexList[VertexIndex].GetPointIndex();

        if (PointIndex < 0 || VertexIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::SetVertexPosition(...)")
            return;
        }

        m_MeshData->PointPositionTable.SetCol(PointIndex, VertexPositionMatrix.GetPointerOfCol(k));
    }
}

template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType>
SurfaceMesh<MeshAttributeType>::GetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
    DenseMatrix<ScalarType> VertexPositionMatrix;

    this->GetVertexPosition(VertexPositionMatrix, VertexHandleList);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
GetVertexPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix, const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
    if (VertexHandleList.IsEmpty() == true)
    {
        VertexPositionMatrix.FastResize(0, 0);
        return;
    }

    VertexPositionMatrix.FastResize(3, VertexHandleList.GetLength());
    for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
    {
        auto VertexIndex = VertexHandleList[k].Index;
        auto PointIndex = m_MeshData->VertexList[VertexIndex].GetPointIndex();
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::GetVertexPosition(...)")
            VertexPositionMatrix.FastResize(0, 0);
            return;
        }
        VertexPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex))
    }
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Point_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Vertex_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    return m_MeshData->VertexList[VertexHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Vertex_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const
{
    return m_MeshData->VertexList[VertexHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Edge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle)
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex;
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex;
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}


template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Cell_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}


template<typename MeshAttributeType>
inline 
Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
    Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> it(const_cast<SurfaceMesh<MeshAttributeType>&>(*this));
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Vertex_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfVertex() const
{
    Iterator_Of_Vertex_Of_SurfaceMesh<MeshAttributeType> it(const_cast<SurfaceMesh<MeshAttributeType>&>(*this));
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> it(const_cast<SurfaceMesh<MeshAttributeType>&>(*this));
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> it(const_cast<SurfaceMesh<MeshAttributeType>&>(*this));
    return it;
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetPointHandle(int_max PointID) const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;

    int_max PointIndex = -1;

    auto it = m_MeshData->Map_PointID_to_PointIndex.find(PointID);
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
        PointIndex = it->second;
    }
    else
    {
        MDK_Error("Invalid PointID @ SurfaceMesh::GetPointHandle(...)")
        PointHandle.SetIndex(-1);
        return PointHandle;
    }
        
    PointHandle.SetIndex(PointIndex);
    return PointHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Vertex_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetVertexHandle(int_max VertexID) const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;

    int_max VertexIndex = -1;
    auto it = m_MeshData->Map_VertexID_to_VertexIndex.find(VertexID);
    if (it != m_MeshData->Map_VertexID_to_VertexIndex.end())
    {
        VertexIndex = it->second;
    }
    else
    {
        MDK_Error("Invalid VertexIndex @ SurfaceMesh::GetVertexHandle(...)")
        VertexHandle.SetIndex(-1);
        return VertexHandle;
    }

    VertexHandle.SetIndex(VertexIndex);
    return VertexHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh  SurfaceMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    int_max EdgeIndex = -1;
    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        EdgeIndex = it->second;
    }
    else
    {
        MDK_Error("Invalid EdgeIndex @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }

    EdgeHandle.SetIndex(EdgeIndex);
    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;

    if (RelativeIndex != 0 && RelativeIndex != 1)
    {
        MDK_Error("Invalid RelativeIndex @ SurfaceMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.SetIndex(-1, -1);
        return DirectedEdgeHandle;
    }

    auto EdgeHandle = this->GetEdgeHandle(EdgeID);

    DirectedEdgeHandle.SetIndex(EdgeHandle.GetIndex(), RelativeIndex);
    return DirectedEdgeHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    Handle_Of_Vertex_Of_SurfaceMesh CellHandle;

    int_max CellIndex = -1;
    auto it = m_MeshData->Map_CellID_to_CellIndex.find(CellID);
    if (it != m_MeshData->Map_CellID_to_CellIndex.end())
    {
        CellIndex = it->second;
    }
    else
    {
        MDK_Error("Invalid CellIndex @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.SetIndex(-1);
        return CellHandle;
    }

    CellHandle.SetIndex(CellIndex);
    return CellHandle;
}

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//
template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddPoint(const typename MeshAttributeType::ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh
SurfaceMesh<MeshAttributeType>::AddPoint(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColNumber() - 1;

    Point_Of_SurfaceMesh<MeshAttributeType> Point;
    Point.Create();
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
    Point.SetVertexIndex(-1); // not know yet, assume NonVertex
    Point.SetEdgeIndex(-1); // not know yet

    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

    m_MeshData->PointIDList.Append(PointIndex);
    m_MeshData->Map_PointID_to_PointIndex[PointIndex] = PointIndex;

    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(PointIndex);

    return PointHandle;
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::AddPoint(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;

    if (PointSet.GetRowNumber() != 3)
    {
        MDK_Error("PointSet.GetRowNumber() != 3 @ SurfaceMesh::AddPoint(...)")
        return PointHandleList;
    }

    PointHandleList.Resize(PointSet.GetColNumber());
    for (int_max k = 0; k < PointSet.GetColNumber(); ++k)
    {
        PointHandleList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointHandleList;
}


template<typename MeshAttributeType>
Handle_Of_Vertex_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddVertex(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;

    auto PointIndex = PointHandle.GetIndex();

    if (PointIndex < 0 || PointIndex >= m_MeshData->PointValidityFlagList.GetLength())
    {
        MDK_Error("Invalid PointIndex @ SurfaceMesh::AddVertex(...)")
        VertexHandle.SetIndex(-1);
        return VertexHandle;
    }

    if (m_MeshData->PointValidityFlagList[PointIndex] == 0)
    {
        MDK_Error("The point has been deleted: PointIndex = " << PointIndex)
        VertexHandle.SetIndex(-1);
        return VertexHandle;
    }

    if (m_MeshData->PointList[PointIndex].IsVertex() == true)
    {
        MDK_Warning("The point has been marked as Vertex: PointIndex = " << PointIndex)
        VertexHandle.SetIndex(m_MeshData->PointList[PointIndex].GetVertexIndex());
        return VertexHandle;
    }

    //----------------------------------------------------------------------------
    auto VertexIndex = m_MeshData->VertexList.GetLength();

    Vertex_Of_SurfaceMesh<MeshAttributeType> Vertex;
    Vertex.Create();
    Vertex.SetParentMesh(*this);
    Vertex.SetIndex(VertexIndex);
    Vertex.SetPointIndex(PointIndex);

    m_MeshData->VertexList.Append(std::move(Vertex));
    m_MeshData->VertexValidityFlagList.Append(1);

    m_MeshData->VertexIDList.Append(VertexIndex);
    m_MeshData->Map_VertexID_to_VertexIndex[VertexIndex] = VertexIndex;

    // update information in m_MeshData->PointList[PointIndex]
    m_MeshData->PointList[PointIndex].SetVertexIndex(VertexIndex);

    //--------------------------
    VertexHandle.SetIndex(VertexIndex);
    return VertexHandle;
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddEdge(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (PointHandleList.GetLength() < 2)
    {
        MDK_Error("PointHandleList length < 2 @ SurfaceMesh::AddEdge(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[PointHandleList[k].GetIndex()] == 0)
        {
            MDK_Error("PointIndex is invalid: " << PointHandleList[k].GetIndex() << " @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }

    //----------------------------------

    if (PointHandleList.GetLength() > 2)
    {
        DenseVector<int_max> tempEdgeIndexList;
        tempEdgeIndexList.Resize(PointHandleList.GetLength());
        tempEdgeIndexList.Fill(-1);

        for (int_max k = 1; k < PointHandleList.GetLength() - 1; ++k)
        {
            if (m_MeshData->PointList[PointHandleList[k].GetIndex()].IsVertex() == true)
            {
                MDK_Error("A middle point of PointIndexList is a vertex, PointIndex = " << PointHandleList[k].GetIndex() << " @ SurfaceMesh::AddEdge(...)")
                EdgeHandle.SetIndex(-1);
                return EdgeHandle;
            }

            tempEdgeIndexList[k] = m_MeshData->PointList[PointHandleList[k].GetIndex()].GetEdgeIndex();
        }

        int_max tempValue = 0;
        for (int_max k = 2; k < PointHandleList.GetLength() - 1; ++k)
        {
            tempValue += std::abs(tempEdgeIndexList[k] - tempEdgeIndexList[k - 1]);
        }

        if (tempValue != 0)
        {
            MDK_Error("A middle point of PointIndexList is already on an edge @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
        else
        {
            if (tempEdgeIndexList[1] >= 0)
            {
                MDK_Warning("The Edge has alreadly been added @ SurfaceMesh::AddEdge(...)")
                EdgeHandle.SetIndex(tempEdgeIndexList[1]);
                return EdgeHandle;
            }
        }
    }

    //----------------------------------

    auto VertexIndex0 = m_MeshData->PointList[PointHandleList[0].GetIndex()].GetVertexIndex();
    auto VertexIndex1 = m_MeshData->PointList[PointHandleList[PointHandleList.GetLength() - 1].GetIndex()].GetVertexIndex();

    if (VertexIndex0 >= 0)
    {
        auto PointIndex_a = m_MeshData->VertexList[VertexIndex0].GetPointIndex();
        if (PointHandleList[0].GetIndex() != PointIndex_a)
        {
            MDK_Error("PointHandleList[0] is invalid @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }
    else
    {
        if (m_MeshData->PointList[PointHandleList[0].GetIndex()].IsOnEdge() == true)
        {
            MDK_Error("The point : PointHandleList[0] is not a veterx, but is already on an edge @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }
    
    if (VertexIndex1 >= 0)
    {
        auto PointIndex_b = m_MeshData->VertexList[VertexIndex1].GetPointIndex();
        if (PointHandleList[PointHandleList.GetLength() - 1].GetIndex() != PointIndex_b)
        {
            MDK_Error("PointHandleList[end] is invalid @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }
    else
    {
        if (m_MeshData->PointList[PointHandleList[PointHandleList.GetLength() - 1].GetIndex()].IsOnEdge() == true)
        {
            MDK_Error("The point : PointHandleList[end] is not a veterx, but is already on an edge @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }
    //------------------------------------------------------------------------------------------------------------------//
    // add new vertex if necessary    
    bool VertexIndex0_is_new = false;
    bool VertexIndex1_is_new = false;

    if (VertexIndex0 < 0)
    {
        auto VertexHandle0 = this->AddVertex(PointHandleList[0]);
        VertexIndex0 = VertexHandle0.GetIndex();
        VertexIndex0_is_new = true;
    }

    if (VertexIndex1 < 0)
    {
        auto VertexHandle1 = this->AddVertex(PointHandleList[PointHandleList.GetLength() - 1]);
        VertexIndex1 = VertexHandle1.GetIndex();
        VertexIndex1_is_new = true;
    }
    //----------------------------------------------------------------
    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_SurfaceMesh<MeshAttributeType> Edge;
    Edge.Create();
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.SetVertexIndexList(VertexIndex0, VertexIndex1);
    Edge.PointIndexList().Resize(PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        Edge.PointIndexList()[k] = PointHandleList[k].GetIndex();
    }

    m_MeshData->EdgeList.Append(std::move(Edge));

    // add empty DirectedEdgePair to hold place
    DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2> DirectedEdgePair;
    m_MeshData->DirectedEdgePairList.Append(std::move(DirectedEdgePair));
    m_MeshData->DirectedEdgePairList[EdgeIndex][0].Clear();
    m_MeshData->DirectedEdgePairList[EdgeIndex][1].Clear();

    m_MeshData->EdgeValidityFlagList.Append(1);

    m_MeshData->EdgeIDList.Append(EdgeIndex);
    m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeIndex] = EdgeIndex;

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
    // information about the first point and the last point have already been set when AddVertex()
    if (PointHandleList.GetLength() > 2)
    {
        for (int_max k = 1; k < PointHandleList.GetLength() - 1; ++k)
        {
            m_MeshData->PointList[PointHandleList[k].GetIndex()].SetEdgeIndex(EdgeIndex);
        }
    }

    //------------
    EdgeHandle.SetIndex(EdgeIndex);
    return EdgeHandle;
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (EdgeHandleList.GetLength() < 2)
    {
        MDK_Error("length of EdgeHandleList < 2 @ SurfaceMesh::AddCell(...)")
        CellHandle.SetIndex(-1);
        return CellHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[EdgeHandleList[k].GetIndex()] == 0)
        {
            MDK_Error("Invalid EdgeIndex: " << EdgeHandleList[k].GetIndex() << " @ SurfaceMesh::AddCell(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    // the input EdgeIndexList must have been ordered
    
    DenseVector<int_max> VertexIndexList;
    VertexIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto tempEdgeIndex = EdgeHandleList[k].GetIndex();

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
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    // get DirectedEdgeIndexList -------------------------------------------------------
    auto CellIndex = m_MeshData->CellList.GetLength();

    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> DirectedEdgeIndexList;
    DirectedEdgeIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto& DirectedEdge0 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].GetIndex()][0];
        auto& DirectedEdge1 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].GetIndex()][1];

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
            CellHandle.SetIndex(-1);
            return CellHandle;
        }

        DirectedEdgeIndexList[k].EdgeIndex = EdgeHandleList[k].GetIndex();
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

    Cell_Of_SurfaceMesh<MeshAttributeType> Cell;
    Cell.Create();
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);

    m_MeshData->CellIDList.Append(CellIndex);
    m_MeshData->Map_CellID_to_CellIndex[CellIndex] = CellIndex;

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
    CellHandle.SetIndex(CellIndex);
    return CellHandle;
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
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
    const auto& DirectedEdgeIndexList = m_MeshData->CellList[CellIndex].DirectedEdgeIndexList();
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

    m_MeshData->Map_CellID_to_CellIndex.erase(m_MeshData->CellIDList[CellIndex]);
    m_MeshData->CellIDList[CellIndex] = -1;
    //---------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    auto EdgeIndex = EdgeHandle.GetIndex();

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
            Handle_Of_Vertex_Of_SurfaceMesh tempVertexHanlde;
            tempVertexHanlde.SetIndex(VertexIndexList[k]);
            this->DeleteVertex(tempVertexHanlde);
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

    m_MeshData->Map_EdgeID_to_EdgeIndex.erase(m_MeshData->EdgeIDList[EdgeIndex]);
    m_MeshData->EdgeIDList[EdgeIndex] = -1;
    //--------------------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteVertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    auto VertexIndex = VertexHandle.GetIndex();

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
    
    m_MeshData->Map_VertexID_to_VertexIndex.erase(m_MeshData->VertexIDList[VertexIndex]);
    m_MeshData->VertexIDList[VertexIndex] = -1;
    //-----------------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    auto PointIndex = PointHandle.GetIndex();

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

    m_MeshData->Map_PointID_to_PointIndex.erase(m_MeshData->PointIDList[PointIndex]);
    m_MeshData->PointIDList[VertexIndex] = -1;
    //---------------------------------------------------------------------
    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
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
template<typename MeshAttributeType>
void SurfaceMesh<MeshAttributeType>::CleanDataStructure()
{

}
//---------------------------------------------------------------------------

}// namespace mdk

#endif