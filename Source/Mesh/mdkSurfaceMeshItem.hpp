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
    }

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->AdjacentPointIndexList.Clear();
    m_Data->AdjacentEdgeIndexList.Clear();
    m_Data->OutgoingDirectedEdgeIndexList.Clear();
    m_Data->IncomingDirectedEdgeIndexList.Clear();
    m_Data->AdjacentCellIndexList.Clear();
    m_Data->Attribute.Clear();
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
            MDK_Error("Input PointID has already been used for another point @ Point_Of_SurfaceMesh::SetID(...)")
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
void Point_Of_SurfaceMesh<MeshAttributeType>::EraseID()
{   
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.erase(it);
        }

        m_Data->ID = -1;
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
void Point_Of_SurfaceMesh<MeshAttributeType>::SetPosition(const DenseVector<typename MeshAttributeType::ScalarType, 3>& Pos)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos.GetPointer());
}


template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetPosition(const typename MeshAttributeType::ScalarType Pos[3])
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::SetPosition(typename MeshAttributeType::ScalarType x,
                                                          typename MeshAttributeType::ScalarType y, 
                                                          typename MeshAttributeType::ScalarType z)
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
void Point_Of_SurfaceMesh<MeshAttributeType>::GetPosition(typename MeshAttributeType::ScalarType Pos[3]) const
{
    m_Data->Mesh.m_MeshData->PointPositionTable.GetCol(m_Data->Index, Pos);
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
DenseVector<int_max> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentPointIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentPointIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentPointIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentPointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentPointIndexList.GetLength(); ++k)
    {
        OutputIDList[k] = m_Data->Mesh.m_MeshData->PointList[m_Data->AdjacentPointIndexList[k]].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentPointNumber() const
{
    return m_Data->AdjacentPointIndexList().GetLength();
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
DenseVector<int_max> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        OutputIDList[k]= m_Data->Mesh.m_MeshData->EdgeList[m_Data->AdjacentEdgeIndexList[k]].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeNumber() const
{
    return m_Data->AdjacentEdgeIndexList.GetLength();
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
DenseVector<int_max> Point_Of_SurfaceMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetOutgoingDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->OutgoingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto EdgeIndex = m_Data->OutgoingDirectedEdgeIndexList[k].EdgeIndex;
        auto RelativeIndex = m_Data->OutgoingDirectedEdgeIndexList[k].RelativeIndex;
        OutputIDList[k] = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].DirectedEdgePair()[RelativeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetOutgoingDirectedEdgeNumber() const
{
    return m_Data->OutgoingDirectedEdgeIndexList.GetLength();
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
DenseVector<int_max> Point_Of_SurfaceMesh<MeshAttributeType>::GetIncomingDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetIncomingDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetIncomingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->IncomingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto EdgeIndex = m_Data->IncomingDirectedEdgeIndexList[k].EdgeIndex;
        auto RelativeIndex = m_Data->IncomingDirectedEdgeIndexList[k].RelativeIndex;
        OutputIDList[k] = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].DirectedEdgePair()[RelativeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetIncomingDirectedEdgeNumber() const
{
    return m_Data->IncomingDirectedEdgeIndexList.GetLength();
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
DenseVector<int_max> Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentCellIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentCellIndexList.GetLength(); ++k)
    {
        OutputIDList[k] = m_Data->Mesh.m_MeshData->CellList[m_Data->AdjacentCellIndexList[k]].GetID();
    }
}


template<typename MeshAttributeType>
inline
int_max Point_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellNumber() const
{
    return m_Data->AdjacentCellIndexList.GetLength();
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
    m_Data->PointIndex0 = InputEdge.m_Data->PointIndex0;
    m_Data->PointIndex1 = InputEdge.m_Data->PointIndex1;
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
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->PointIndex0 = -1;
    m_Data->PointIndex1 = -1;

    m_Data->DirectedEdgePair[0].Create();
    m_Data->DirectedEdgePair[1].Create();

    m_Data->Attribute.Clear();
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

    m_Data->DirectedEdgePair[0].SetParentMesh(InputMesh);
    m_Data->DirectedEdgePair[1].SetParentMesh(InputMesh);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;

    m_Data->DirectedEdgePair[0].SetIndex(EdgeIndex, 0);
    m_Data->DirectedEdgePair[1].SetIndex(EdgeIndex, 1);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetPointIndexList(const int_max PointIndexList[2])
{
    this->SetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline 
DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2>& 
Edge_Of_SurfaceMesh<MeshAttributeType>::DirectedEdgePair()
{
    return m_Data->DirectedEdgePair;
}

template<typename MeshAttributeType>
inline
const DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2>& 
Edge_Of_SurfaceMesh<MeshAttributeType>::DirectedEdgePair() const
{
    return m_Data->DirectedEdgePair;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const
{
    PointIndex0 = m_Data->PointIndex0;
    PointIndex1 = m_Data->PointIndex1;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList(int_max PointIndexList[2]) const
{
    this->GetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::SetPointIndexList(int_max PointIndex0, int_max PointIndex1)
{
    m_Data->PointIndex0 = PointIndex0;
    m_Data->PointIndex1 = PointIndex1;
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
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex1].GetAdjacentEdgeIndexList();

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

    if (m_Data->DirectedEdgePair[0].GetCellIndex() >= 0)
    {
        OutputIndexList.Append(m_Data->DirectedEdgePair[0].GetCellIndex());
    }
    else if (m_Data->DirectedEdgePair[1].GetCellIndex() >= 0)
    {
        OutputIndexList.Append(m_Data->DirectedEdgePair[1].GetCellIndex());
    }
}

// Cell share any  point of this edge
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
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex0].GetAdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex0].GetAdjacentCellIndexList();

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
    if (m_Data->DirectedEdgePair[0].GetCellIndex() < 0)
    {
        return true;
    }
    
    if (m_Data->DirectedEdgePair[1].GetCellIndex() < 0)
    {
        return true;
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
void Edge_Of_SurfaceMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
        }

        m_Data->ID = -1;
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
DenseVector<Handle_Of_Point_Of_SurfaceMesh, 2> Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh, 2> OutputHandleList;
    OutputHandleList[0].SetIndex(m_Data->PointIndex0);
    OutputHandleList[1].SetIndex(m_Data->PointIndex1);
    return OutputHandleList;
}


template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointHandleList(Handle_Of_Point_Of_SurfaceMesh OutputHandleList[2]) const
{
    OutputHandleList[0].SetIndex(m_Data->PointIndex0);
    OutputHandleList[1].SetIndex(m_Data->PointIndex1);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::
GetPointHandleList(Handle_Of_Point_Of_SurfaceMesh& PointHandle0, Handle_Of_Point_Of_SurfaceMesh& PointHandle1) const
{
    PointHandle0.SetIndex(m_Data->PointIndex0);
    PointHandle1.SetIndex(m_Data->PointIndex1);
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max, 2> Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max, 2> PointIDList;
    this->GetPointIDList(PointIDList[0], PointIDList[1]);
    return PointIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointIDList(int_max OutputIDList[2]) const
{
    this->GetPointIDList(PointIDList[0], PointIDList[1]);
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetPointIDList(int_max& PointID0, int_max& PointID1) const
{
    PointID0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetID();
    PointID1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetID();
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(2);
    OutputHandleList[0].SetIndex(m_Data->Index, 0);
    OutputHandleList[1].SetIndex(m_Data->Index, 1);
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
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->List[m_Data->PointIndex1].GetAdjacentEdgeIndexList();

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
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();

    OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIDList.Append(m_Data->Mesh.m_MeshData->EdgeList[tempIndex].GetID());
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIDList.Append(m_Data->Mesh.m_MeshData->EdgeList[tempIndex].GetID());
        }
    }
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentEdgeNumber() const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();
    return AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2 ;
}


template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
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

    if (m_Data->DirectedEdgePair[0].GetCellIndex() >= 0)
    {
        OutputHandleList.Append(m_Data->DirectedEdgePair[0].GetCellIndex());
    }
    else if (m_Data->DirectedEdgePair[1].GetCellIndex() >= 0)
    {
        OutputHandleList.Append(m_Data->DirectedEdgePair[1].GetCellIndex());
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(2);

    if (m_Data->DirectedEdgePair[0].GetCellIndex() >= 0)
    {
        auto CellID = m_Data->DirectedEdgePair[0].GetCellID();
        OutputIDList.Append(CellID)
    }
    else if (m_Data->DirectedEdgePair[1].GetCellIndex() >= 0)
    {
        auto CellID = m_Data->DirectedEdgePair[1].GetCellID();
        OutputHandleList.Append(CellID);
    }
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellNumber() const
{
    int_max Counter = 0;
    if (m_Data->DirectedEdgePair[0].GetCellIndex() >= 0)
    {
        Counter += 1;
    }
    else if (m_Data->DirectedEdgePair[1].GetCellIndex() >= 0)
    {
        Counter += 1;
    }
    return Counter;
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
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentCellIndexList();

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
DenseVector<int_max> Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetNeighbourCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIDList(DenseVector<int_max>& OutputIDList) const
{
    auto AdjacentCellIDList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentCellIDList();
    auto AdjacentCellIDList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentCellIDList();

    OutputIDList.FastResize(0);

    if (AdjacentCellIDList0.IsEmpty() == true && AdjacentCellIDList1.IsEmpty() == true)
    {
        return;
    }

    OutputIDList.ReserveCapacity(AdjacentCellIndexList0.GetLength() + AdjacentCellIDList1.GetLength());

    for (int_max k = 0; k < AdjacentCellIDList0.GetLength(); ++k)
    {
        OutputIDList.Append(AdjacentCellIDList0[k]);
    }

    for (int_max k = 0; k < AdjacentCellIndexList1.GetLength(); ++k)
    {
        OutputIDList.Append(AdjacentCellIDList1[k]);
    }

    auto tempIndexList = OutputIDList.FindUnique();
    OutputIDList = OutputIDList.GetSubSet(tempIndexList);
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellNumber() const
{
    auto Counter0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentCellNumber();
    auto Counter1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentCellNumber();
    return Counter0 + Counter1 - 2;
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
    }    

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

    m_Data->Attribute.Clear();
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
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::EraseInformationRelatedToCell()
{
    m_Data->CellIndex = -1;
    m_Data->PointIndex_start = -1;
    m_Data->PointIndex_end = -1;
    m_Data->NextDirectedEdgeIndex.EdgeIndex = -1;
    m_Data->NextDirectedEdgeIndex.RelativeIndex = -1;
    m_Data->PreviousDirectedEdgeIndex.EdgeIndex = -1;
    m_Data->PreviousDirectedEdgeIndex.RelativeIndex = -1;
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
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetIndex() const
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
    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.GetCellIndex() >= 0)
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
bool DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index.EdgeIndex < 0 || m_Data->Index.RelativeIndex < 0 || m_Data->Index.RelativeIndex > 1)
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
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].IsBoundary();
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
            MDK_Error("Input DirectedEdgeID has already been used for another directed-edge @ DirectedEdge_Of_SurfaceMesh::SetID(...)")
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
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        }

        m_Data->ID = -1;
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
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetCellHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(m_Data->CellIndex);
    return CellHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetCellID() const
{
    return m_Data->Mesh.m_MeshData->CellList[m_Data->CellIndex].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetStartPointID() const
{
    return m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex_start].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetEndPointID() const
{
    return m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex_end].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetEdgeID() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendDirectedEdgeID() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNextDirectedEdgeID() const
{
    auto EdgeIndex = m_Data->NextDirectedEdgeIndex.EdgeIndex;
    auto RelativeIndex = m_Data->NextDirectedEdgeIndex.RelativeIndex;
    return m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].DirectedEdgePair()[RelativeIndex].GetID();
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
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetPreviousDirectedEdgeID() const
{
    auto EdgeIndex = m_Data->PreviousDirectedEdgeIndex.EdgeIndex;
    auto RelativeIndex = m_Data->PreviousDirectedEdgeIndex.RelativeIndex;
    return m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].DirectedEdgePair()[RelativeIndex].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendCellHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(-1);

    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.GetCellIndex() >= 0)
    {
        CellHandle.SetIndex(FirendDirectedEdge.GetCellIndex());
    }

    return CellHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetFirendCellID() const
{
    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.GetCellIndex() >= 0)
    {
        return FirendDirectedEdge.GetCellID();
    }

    return -1;
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
    m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetNeighbourCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellIDList(DenseVector<int_max>& OutputIDList) const
{
    m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellIDList(OutputIDList);
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshAttributeType>::GetNeighbourCellNumber() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellNumber();
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
    m_Data->ID = InputCell.m_Data->ID;
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
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->DirectedEdgeIndexList.Clear();
    m_Data->Attribute.Clear();
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
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList() const
{
    DenseVector<int_max> IndexList;
    this->GetPointIndexList(IndexList);
    return IndexList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutputIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        OutputIndexList[k] = m_Data->Mesh.m_MeshData->EdgeList[tempIndex.EdgeIndex].DirectedEdgePair()[tempIndex.RelativeIndex].GetStartPointIndex();
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
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.GetCellIndex() >= 0)
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
void Cell_Of_SurfaceMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex.erase(it);
        }

        m_Data->ID = -1;
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
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = m_Data->DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = m_Data->DirectedEdgeIndexList[k].RelativeIndex;
        OutputIDList[k] = m_Data->Mesh.m_MeshData->EdgeList[tempEdgeIndex].DirectedEdgePair()[tempRelativeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetDirectedEdgeNumber() const
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
    OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        OutputHandleList[k].SetIndex(m_Data->Mesh.m_MeshData->EdgeList[tempIndex.EdgeIndex].DirectedEdgePair()[tempIndex.RelativeIndex].GetStartPointIndex());
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max> OutputPointIDList;
    this->GetPointIDList(OutputPointIDList);
    return OutputPointIDList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointIDList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputPointIDList) const
{    
    OutputPointIDList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        OutputPointIDList[k] = m_Data->Mesh.m_MeshData->EdgeList[tempIndex.EdgeIndex].DirectedEdgePair()[tempIndex.RelativeIndex].GetStartPointID();
    }
}

template<typename MeshAttributeType>
inline 
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
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
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIDList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputIDList = m_Data->Mesh.m_MeshData->EdgeList[m_Data->DirectedEdgeIndexList[k].EdgeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
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
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.GetCellIndex() >= 0)
        {
            CellHandle.SetIndex(FriendDirectedEdge.GetCellIndex());
            OutputHandleList.Append(CellHandle);
        }
    }
    OutputHandleList.ReleaseUnusedCapacity();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellIDList(DenseVector<int_max>& OutputIDList) const
{
    int_max DirectedEdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(DirectedEdgeNumber);
    for (int_max k = 0; k < DirectedEdgeNumber; ++k)
    {
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.GetCellIndex >= 0)
        {
            OutputIDList.Append(FriendDirectedEdge.GetCellID());
        }
    }
    OutputHandleList.ReleaseUnusedCapacity();
}

template<typename MeshAttributeType>
inline
int_max Cell_Of_SurfaceMesh<MeshAttributeType>::GetAdjacentCellNumber() const
{
    int_max Counter = 0;
    for (int_max k = 0; k < DirectedEdgeNumber; ++k)
    {
        const auto& FriendDirectedEdge = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgePair()[1 - m_Data->Index.RelativeIndex];
        if (FriendDirectedEdge.GetCellIndex() >= 0)
        {
            Counter += 1;
        }
    }
    return Counter;
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