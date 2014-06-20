#ifndef __mdkSurfaceMeshItem_hpp
#define __mdkSurfaceMeshItem_hpp


namespace mdk
{
//=========================================================== Point_Of_SurfaceMesh ===========================================================//

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>::Point_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>::Point_Of_SurfaceMesh(const Point_Of_SurfaceMesh<MeshAttributeType>& InputPoint)
{
    (*this) = InputPoint;
}

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>::Point_Of_SurfaceMesh(Point_Of_SurfaceMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>::~Point_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::operator=(const Point_Of_SurfaceMesh<MeshAttributeType>& InputPoint)
{
    if (!InputPoint.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputPoint.m_Data->Mesh);
    m_Data->Index = InputPoint.m_Data->Index; 
    m_Data->ID = InputPoint.m_Data->ID;
    m_Data->AdjacentPointIndexList = InputPoint.m_Data->AdjacentPointIndexList;
    m_Data->AdjacentEdgeIndexList = InputPoint.m_Data->AdjacentEdgeIndexList;
    m_Data->OutgoingDirectedEdgeIndexList = InputPoint.m_Data->OutgoingDirectedEdgeIndexList;
    m_Data->IncomingDirectedEdgeIndexList = InputPoint.m_Data->IncomingDirectedEdgeIndexList;
    m_Data->AdjacentCellIndexList = InputPoint.m_Data->AdjacentCellIndexList;
    m_Data->Attribute = InputPoint.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::operator=(Point_Of_SurfaceMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_SurfaceMesh<MeshAttributeType>>();
        m_Data->Index = -1;
        m_Data->ID = -1;
    }
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::Clear()
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetParentMesh(SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max>& Point_Of_SurfaceMesh<MeshAttributeType>::AdjacentPointIndexList()
{
    return m_Data->AdjacentPointIndexList;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max>& Point_Of_SurfaceMesh<MeshAttributeType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline 
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Point_Of_SurfaceMesh<MeshAttributeType>::OutgoingDirectedEdgeIndexList()
{
    return m_Data->OutgoingDirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Point_Of_SurfaceMesh<MeshAttributeType>::IncomingDirectedEdgeIndexList()
{
    return m_Data->IncomingDirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max>& Point_Of_SurfaceMesh<MeshAttributeType>::AdjacentCellIndexList()
{
    return m_Data->AdjacentCellIndexList;
}

template<typename MeshAttributeType>
inline
bool Point_Of_SurfaceMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool Point_Of_SurfaceMesh<MeshAttributeType>::IsVertex() const
{
    return (m_Data->AdjacentPointIndexList.IsEmpty() == false);
}

template<typename MeshAttributeType>
inline
bool Point_Of_SurfaceMesh<MeshAttributeType>::IsOnEdge() const
{
    return (m_Data->AdjacentEdgeIndexList.IsEmpty() == false);
}

template<typename MeshAttributeType>
inline
bool Point_Of_SurfaceMesh<MeshAttributeType>::IsOnBoundaryEdge() const
{
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        auto EdgeIndex = m_Data->AdjacentEdgeIndexList[k];
        if (m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].IsBoundary() == true)
        {
            return true;
        }
    }
    return false;
}

template<typename MeshAttributeType>
inline
SurfaceMesh<MeshAttributeType>& Point_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const SurfaceMesh<MeshAttributeType>& Point_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh Point_Of_SurfaceMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(m_Data->Index);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetID(int_max PointID)
{
    if (PointID < 0)
    {
        MDK_Error("PointID < 0 @ Point_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto PointID_old = m_Data->ID;
    if (PointID_old != PointID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(PointID);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            MDK_Error("Input PointID has already been used for another vertex @ Point_Of_SurfaceMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(PointID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex[PointID] = m_Data->Index;
        m_Data->ID = PointID;
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetPosition(const typename MeshAttributeType::ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetPosition(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> Point_Of_SurfaceMesh<MeshAttributeType>::GetPosition() const
{
    DenseVector<ScalarType, 3> Pos;
    this->GetPosition(Pos.GetPointer());
    return Pos;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetPosition(typename MeshAttributeType::ScalarType* Pos) const
{
    m_Data->Mesh.m_MeshData->PointPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::
GetPosition(typename MeshAttributeType::ScalarType& x, typename MeshAttributeType::ScalarType& y, typename MeshAttributeType::ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentPointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentPointIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentPointIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Point_Of_SurfaceMesh<MeshAttributeType>::GetOutgoingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetOutgoingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->OutgoingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->OutgoingDirectedEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Point_Of_SurfaceMesh<MeshAttributeType>::GetIncomingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetIncomingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->IncomingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->IncomingDirectedEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentCellIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentCellIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentCellIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::PointAttributeType& Point_Of_SurfaceMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::PointAttributeType& Point_Of_SurfaceMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Edge_Of_SurfaceMesh ===========================================================//

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>::Edge_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>::Edge_Of_SurfaceMesh(const Edge_Of_SurfaceMesh<MeshAttributeType>& InputEdge)
{
    (*this) = InputEdge;
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>::Edge_Of_SurfaceMesh(Edge_Of_SurfaceMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>::~Edge_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::operator=(const Edge_Of_SurfaceMesh<MeshAttributeType>& InputEdge)
{
    if (!InputEdge.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputEdge.m_Data->Mesh);
    m_Data->Index = InputEdge.m_Data->Index;
    m_Data->ID = InputEdge.m_Data->ID;
    m_Data->VertexPointIndex0 = InputEdge.m_Data->VertexPointIndex0;
    m_Data->VertexPointIndex1 = InputEdge.m_Data->VertexPointIndex1;
    m_Data->PointIndexList = InputEdge.m_Data->PointIndexList;
    m_Data->Attribute = InputEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::operator=(Edge_Of_SurfaceMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_SurfaceMesh<MeshAttributeType>>();
        m_Data->Index = -1;
        m_Data->ID = -1;
        m_Data->VertexPointIndex0 = -1;
        m_Data->VertexPointIndex1 = -1;
    }    
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::Clear()
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetParentMesh(SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetVertexPointIndexList(const int_max VertexPointIndexList[2])
{
    this->SetVertexPointIndexList(VertexPointIndexList[0], VertexPointIndexList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointIndexList(int_max& VertexPointIndex0, int_max& VertexPointIndex1) const
{
    VertexPointIndex0 = m_Data->VertexPointIndex0;
    VertexPointIndex1 = m_Data->VertexPointIndex1;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointIndexList(int_max VertexPointIndexList[2]) const
{
    this->GetVertexPointIndexList(VertexPointIndexList[0], VertexPointIndexList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetVertexPointIndexList(int_max VertexPointIndex0, int_max VertexPointIndex1)
{
    m_Data->VertexPointIndex0 = VertexPointIndex0;
    m_Data->VertexPointIndex1 = VertexPointIndex1;
}

template<typename MeshAttributeType>
inline DenseVector<int_max>& Edge_Of_SurfaceMesh<MeshAttributeType>::PointIndexList()
{
    return m_Data->PointIndexList;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex1].GetAdjacentEdgeIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }
}

// Cell share this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(2);

    if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].IsValid() == true)
    {
        OutputIndexList.Append(m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].GetCellIndex());
    }
    else if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].IsValid() == true)
    {
        OutputIndexList.Append(m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].GetCellIndex());
    }
}

// Cell share any vertex point of this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetNeighbourCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentCellIndexList();

    OutputIndexList.FastResize(0);

    if (AdjacentCellIndexList0.IsEmpty() == true && AdjacentCellIndexList1.IsEmpty() == true)
    {
        return;
    }

    OutputIndexList.ReserveCapacity(AdjacentCellIndexList0.GetLength() + AdjacentCellIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentCellIndexList0.GetLength(); ++k)
    {
        OutputIndexList.Append(AdjacentCellIndexList0[k]);
    }

    for (int_max k = 0; k < AdjacentCellIndexList1.GetLength(); ++k)
    {
        OutputIndexList.Append(AdjacentCellIndexList1[k]);
    }

    auto tempIndexList = OutputIndexList.FindUnique();
    OutputIndexList = OutputIndexList.GetSubSet(tempIndexList);
}

template<typename MeshAttributeType>
inline
bool Edge_Of_SurfaceMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool Edge_Of_SurfaceMesh<MeshAttributeType>::IsBoundary() const
{
    if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].IsValid() == false)
    {
        return true
    }
    
    if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].IsValid() == false)
    {
        return true
    }

    return false;
}

template<typename MeshAttributeType>
inline
SurfaceMesh<MeshAttributeType>& Edge_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const SurfaceMesh<MeshAttributeType>& Edge_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh Edge_Of_SurfaceMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->Index);
    return EdgeHandle;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetID(int_max EdgeID)
{
    if (EdgeID < 0)
    {
        MDK_Error("EdgeID < 0 @ Edge_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto EdgeID_old = m_Data->ID;
    if (EdgeID_old != EdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            MDK_Error("Input EdgeID has already been used for another edge @ Edge_Of_SurfaceMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeID] = m_Data->Index;
        m_Data->ID = EdgeID;
    }
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetVertexPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(2);
    this->GetVertexPointHandleList(OutputHandleList[0], OutputHandleList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointHandleList(Handle_Of_Point_Of_SurfaceMesh OutputHandleList[2]) const
{
    this->GetVertexPointHandleList(OutputHandleList[0], OutputHandleList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::
GetVertexPointHandleList(Handle_Of_Point_Of_SurfaceMesh& VertexPointHandle0, Handle_Of_Point_Of_SurfaceMesh& VertexPointHandle1) const
{
    VertexPointHandle0.SetIndex(m_Data->VertexPointIndex0);
    VertexPointHandle1.SetIndex(m_Data->VertexPointIndex1);
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_Data->PointIndexList.GetLength();
}

template<typename MeshAttributeType>
inline 
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->PointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->PointIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex1].GetAdjacentEdgeIndexList();

    OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            EdgeHandle.SetIndex(tempIndex);
            OutputHandleList.Append(EdgeHandle);
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            EdgeHandle.SetIndex(tempIndex);
            OutputHandleList.Append(EdgeHandle);
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{    
    OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(2);

    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].IsValid() == true)
    {
        CellHandle.SetIndex(m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].GetCellIndex());
        OutputHandleList.Append(CellHandle);
    }
    else if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].IsValid() == true)
    {
        CellHandle.SetIndex(m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].GetCellIndex());
        OutputHandleList.Append(CellHandle);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetNeighbourCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexPointIndex0].GetAdjacentCellIndexList();

    OutputHandleList.FastResize(0);

    if (AdjacentCellIndexList0.IsEmpty() == true && AdjacentCellIndexList1.IsEmpty() == true)
    {
        return;
    }

    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    OutputHandleList.ReserveCapacity(AdjacentCellIndexList0.GetLength() + AdjacentCellIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentCellIndexList0.GetLength(); ++k)
    {
        CellHandle.SetIndex(AdjacentCellIndexList0[k]);
        OutputHandleList.Append(CellHandle);
    }

    for (int_max k = 0; k < AdjacentCellIndexList1.GetLength(); ++k)
    {
        CellHandle.SetIndex(AdjacentCellIndexList1[k]);
        OutputHandleList.Append(CellHandle);
    }

    auto tempIndexList = OutputHandleList.FindUnique();
    OutputHandleList = OutputHandleList.GetSubSet(tempIndexList);
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::EdgeAttributeType& Edge_Of_SurfaceMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::EdgeAttributeType& Edge_Of_SurfaceMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== DirectedEdge_Of_SurfaceMesh ===========================================================//

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::DirectedEdge_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::DirectedEdge_Of_SurfaceMesh(const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& InputDirectedEdge)
{
    (*this) = InputDirectedEdge;
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::DirectedEdge_Of_SurfaceMesh(DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::~DirectedEdge_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::operator=(const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& InputDirectedEdge)
{
    if (!InputDirectedEdge.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputDirectedEdge.m_Data->Mesh);
    m_Data->Index = InputDirectedEdge.m_Data->Index;
    m_Data->ID = InputDirectedEdge.m_Data->ID;
    m_Data->CellIndex = InputDirectedEdge.m_Data->CellIndex;
    m_Data->PointIndex_start = InputDirectedEdge.m_Data->PointIndex_start;
    m_Data->PointIndex_end = InputDirectedEdge.m_Data->PointIndex_end;
    m_Data->NextDirectedEdgeIndex = InputDirectedEdge.m_Data->NextDirectedEdgeIndex;
    m_Data->PreviousDirectedEdgeIndex = InputDirectedEdge.m_Data->PreviousDirectedEdgeIndex;
    m_Data->Attribute = InputDirectedEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::operator=(DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>>();
        
        m_Data->ID = -1;
        m_Data->Index.EdgeIndex = -1;
        m_Data->Index.RelativeIndex = -1;
        m_Data->CellIndex = -1;
        m_Data->PointIndex_start = -1;
        m_Data->PointIndex_end = -1;
        m_Data->NextDirectedEdgeIndex.EdgeIndex = -1;
        m_Data->NextDirectedEdgeIndex.RelativeIndex = -1;
        m_Data->PreviousDirectedEdgeIndex.EdgeIndex = -1;
        m_Data->PreviousDirectedEdgeIndex.RelativeIndex = -1;
    }    
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::Clear()
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetParentMesh(SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->Index.EdgeIndex = EdgeIndex;
    m_Data->Index.RelativeIndex = RelativeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetCellIndex(int_max CellIndex)
{
    m_Data->CellIndex = CellIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetStartPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_start = PointIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetEndPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_end = PointIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->NextDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->NextDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->NextDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->PreviousDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->PreviousDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->PreviousDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetCellIndex() const
{
    return m_Data->CellIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetStartPointIndex() const
{
    return m_Data->PointIndex_start;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetEndPointIndex() const
{
    return m_Data->PointIndex_end;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendDirectedEdgeIndex() const
{
    DirectedEdgeIndex_Of_SurfaceMesh  DirectedEdgeIndex;
    DirectedEdgeIndex.EdgeIndex = m_Data->Index.EdgeIndex;
    DirectedEdgeIndex.RelativeIndex = 1 - m_Data->Index.RelativeIndex;
    return DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNextDirectedEdgeIndex() const
{
    return m_Data->NextDirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPreviousDirectedEdgeIndex() const
{
    return m_Data->PreviousDirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendCellIndex() const
{
    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.IsValid() == true)
    {
        return FirendDirectedEdge.GetCellIndex();
    }

    return -1;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellIndexList();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetPointIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max tempVertexPointIndexList[2];
    m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetVertexPointIndexList(tempVertexPointIndexList);

    const auto& tempPointIndexList = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].PointIndexList();
    OutputIndexList.FastResize(tempPointIndexList.GetLength());

    if (m_Data->PointIndex_start == tempVertexPointIndexList[0] && m_Data->PointIndex_end == tempVertexPointIndexList[1])
    {
        OutputIndexList = tempPointIndexList;
    }
    else if (m_Data->PointIndex_start == tempVertexPointIndexList[1] && m_Data->PointIndex_end == tempVertexPointIndexList[0])
    {        
        auto tempLength = tempPointIndexList.GetLength();
        for (int_max n = 0; n < tempLength; ++n)
        {
            OutputIndexList[n] = tempPointIndexList[tempLength - 1 - n];
        }
    }
    else
    {
        MDK_Error("Something is wrong @ DirectedEdge_Of_SurfaceMesh::GetPointIndexList(...)")
    }
}

template<typename MeshAttributeType>
inline
bool DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index.EdgeIndex < 0 || m_Data->Index.RelativeIndex < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::IsBoundary() const
{
    return (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex].IsValid() == false);
}

template<typename MeshAttributeType>
inline
SurfaceMesh<MeshAttributeType>& DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const SurfaceMesh<MeshAttributeType>& DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline 
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->Index);
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetCellHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(m_Data->CellIndex);
    return CellHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetStartPointHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(m_Data->PointIndex_start);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetEndPointHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(m_Data->PointIndex_end);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetEdgeHandle() const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->Index.EdgeIndex);
    return EdgeHandle;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::SetID(int_max DirectedEdgeID)
{
    if (DirectedEdgeID < 0)
    {
        MDK_Error("DirectedEdgeID < 0 @ DirectedEdge_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto DirectedEdgeID_old = m_Data->ID;
    if (DirectedEdgeID_old != DirectedEdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            MDK_Error("Input DirectedEdgeID has already been used for another edge @ DirectedEdge_Of_SurfaceMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID] = m_Data->Index;
        m_Data->ID = DirectedEdgeID;
    }
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendDirectedEdgeHandle() const
{
    DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex;
    DirectedEdgeIndex.EdgeIndex = m_Data->Index.EdgeIndex;
    DirectedEdgeIndex.RelativeIndex = 1 - m_Data->Index.RelativeIndex;

    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(DirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNextDirectedEdgeHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->NextDirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPreviousDirectedEdgeHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->PreviousDirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendCellHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(-1);

    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.IsValid() == true)
    {
        CellHandle.SetIndex(FirendDirectedEdge.GetCellIndex());
    }

    return CellHandle;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetNeighbourCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellHandleList();
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_Point_Of_SurfaceMesh> DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline 
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    int_max tempVertexPointIndexList[2];
    m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetVertexIndexList(tempVertexPointIndexList);

    const auto& tempPointIndexList = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].PointIndexList();
    auto tempLength = tempPointIndexList.GetLength();

    OutputIndexList.FastResize(tempLength);

    if (m_Data->PointIndex_start == tempVertexPointIndexList[0] && m_Data->PointIndex_end == tempVertexPointIndexList[1])
    {
        for (int_max n = 0; n < tempLength; ++n)
        {
            OutputHandleList[n].SetIndex(tempPointIndexList[n]);
        }
    }
    else if (m_Data->PointIndex_start == tempVertexPointIndexList[1] && m_Data->PointIndex_end == tempVertexPointIndexList[0])
    {        
        for (int_max n = 0; n < tempLength; ++n)
        {
            OutputIndexList[n].SetIndex(tempPointIndexList[tempLength - 1 - n]);
        }
    }
    else
    {
        MDK_Error("Something is wrong @ DirectedEdge_Of_SurfaceMesh::GetPointIndexList(...)")
    }
}

template<typename MeshAttributeType>
inline 
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetPointNumber();
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline 
const typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Cell_Of_SurfaceMesh ===========================================================//

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>::Cell_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>::Cell_Of_SurfaceMesh(const Cell_Of_SurfaceMesh<MeshAttributeType>& InputCell)
{
    (*this) = InputCell;
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>::Cell_Of_SurfaceMesh(Cell_Of_SurfaceMesh<MeshAttributeType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>::~Cell_Of_SurfaceMesh()
{
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::operator=(const Cell_Of_SurfaceMesh<MeshAttributeType>& InputCell)
{
    if (!InputCell.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputCell.m_Data->Mesh);
    m_Data->Index = InputCell.m_Data->Index;
    m_Data->DirectedEdgeIndexList = InputCell.m_Data->DirectedEdgeIndexList;
    m_Data->Attribute = InputCell.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::operator=(Cell_Of_SurfaceMesh<MeshAttributeType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Cell_Of_SurfaceMesh<MeshAttributeType>>();
        m_Data->Index = -1;
    }    
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::Clear()
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::SetParentMesh(SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::SetIndex(int_max CellIndex)
{
    m_Data->Index = CellIndex;
}

template<typename MeshAttributeType>
inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Cell_Of_SurfaceMesh<MeshAttributeType>::DirectedEdgeIndexList()
{
    return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointIndexList() const
{
    DenseVector<int_max> VertexIndexList;
    this->GetVertexPointIndexList(VertexIndexList);
    return VertexIndexList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointIndexList(DenseVector<int_max>& OutputIndexList) const
{
    VertexIndexList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        VertexIndexList[k] = m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempIndex.EdgeIndex][tempIndex.RelativeIndex].GetStartPointIndex();
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIndexList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputIndexList[k] = m_Data->DirectedEdgeIndexList[k].EdgeIndex;
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetPointIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(0);

    for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = m_Data->DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = m_Data->DirectedEdgeIndexList[k].RelativeIndex;
        auto tempPointIndex_start = m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].GetStartPointIndex();
        auto tempPointIndex_end = m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].GetEndPointIndex();

        int_max tempVertexPointIndexList[2];
        m_Data->Mesh.m_MeshData->EdgeList[tempEdgeIndex].GetVertexPointIndexList(tempVertexPointIndexList);

        const auto& tempPointIndexList = m_Data->Mesh.m_MeshData->EdgeList[tempEdgeIndex].PointIndexList();

        if (tempPointIndex_start == tempVertexPointIndexList[0] && tempPointIndex_end == tempVertexPointIndexList[1])
        {
            for (int_max n = 0; n < tempPointIndexList.GetLength()-1; ++n) // do not copy the last point
            {
                OutputIndexList.Append(tempPointIndexList[n]);
            }
        }
        else if (tempPointIndex_start == tempVertexPointIndexList[1] && tempPointIndex_end == tempVertexPointIndexList[0])
        {
            for (int_max n = tempPointIndexList.GetLength() - 1; n >= 1; --n) // do not copy the first point
            {
                OutputIndexList.Append(tempPointIndexList[n]);
            }
        }
        else
        {
            MDK_Error("Something is wrong @ Cell_Of_SurfaceMesh::GetPointIndexList(...)")
            return;
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max DirectedEdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(DirectedEdgeNumber);
    for (int_max k = 0; k < DirectedEdgeNumber; ++k)
    {
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.IsValid() == true)
        {
            auto tempCellIndex = FriendDirectedEdge.GetCellIndex();
            OutputIndexList.Append(tempCellIndex);
        }
    }
    OutputIndexList.ReleaseUnusedCapacity();
}

template<typename MeshAttributeType>
inline
bool Cell_Of_SurfaceMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
SurfaceMesh<MeshAttributeType>& Cell_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const SurfaceMesh<MeshAttributeType>& Cell_Of_SurfaceMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh Cell_Of_SurfaceMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(m_Data->Index);
    return CellHandle;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::SetID(int_max CellID)
{
    if (CellID < 0)
    {
        MDK_Error("CellID < 0 @ Cell_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto CellID_old = m_Data->ID;
    if (CellID_old != CellID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.find(CellID);
        if (it != m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.end())
        {
            MDK_Error("Input CellID has already been used for another cell @ Cell_Of_SurfaceMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.find(CellID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex[CellID] = m_Data->Index;
        m_Data->ID = CellID;
    }
}

template<typename MeshAttributeType>
inline 
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetVertexPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{    
    OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        OutputHandleList[k].SetIndex(m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempIndex.EdgeIndex][tempIndex.RelativeIndex].GetStartPointIndex());
    }
}

template<typename MeshAttributeType>
inline 
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetVertexPointNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputHandleList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k].EdgeIndex);
    }
}

template<typename MeshAttributeType>
inline int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline 
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    auto PointIndexListOfCell = this->GetPointIndexList();
    OutputHandleList.FastResize(PointIndexListOfCell.GetLength());
    for (int_max k = 0; k < PointIndexListOfCell.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(PointIndexListOfCell[k]);
    }
}

template<typename MeshAttributeType>
inline
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    int_max PointNumberOfCell = 0;
    for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
    {
        PointNumberOfCell += m_Data->Mesh.m_MeshData->EdgeList[m_Data->DirectedEdgeIndexList[k].EdgeIndex].GetPointNumber();
    }
    PointNumberOfCell -= m_Data->DirectedEdgeIndexList.GetLength();
    return PointNumberOfCell;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    int_max DirectedEdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(DirectedEdgeNumber);
    for (int_max k = 0; k < DirectedEdgeNumber; ++k)
    {
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.IsValid() == true)
        {
            CellHandle.SetIndex(FriendDirectedEdge.GetCellIndex());
            OutputHandleList.Append(CellHandle);
        }
    }
    OutputHandleList.ReleaseUnusedCapacity();
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::CellAttributeType& Cell_Of_SurfaceMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::CellAttributeType& Cell_Of_SurfaceMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

}// namespace mdk

#endif