#ifndef __mdkSurfaceMeshItem_hpp
#define __mdkSurfaceMeshItem_hpp


namespace mdk
{
//=========================================================== Point_Of_SurfaceMesh ===========================================================//

template<typename MeshType>
inline
Point_Of_SurfaceMesh<MeshType>::Point_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
Point_Of_SurfaceMesh<MeshType>::Point_Of_SurfaceMesh(const Point_Of_SurfaceMesh<MeshType>& InputVertex)
{
    (*this) = InputVertex;
}

template<typename MeshType>
inline
Point_Of_SurfaceMesh<MeshType>::Point_Of_SurfaceMesh(Point_Of_SurfaceMesh<MeshType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename MeshType>
inline
Point_Of_SurfaceMesh<MeshType>::~Point_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::operator=(const Point_Of_SurfaceMesh<MeshType>& InputVertex)
{
    if (!InputVertex.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputVertex.m_Data->Mesh);
    m_Data->Index = InputVertex.m_Data->Index;
    m_Data->VertexIndex = InputVertex.m_Data->VertexIndex;
    m_Data->EdgeIndex = InputVertex.m_Data->EdgeIndex;
    m_Data->Attribute = InputVertex.m_Data->Attribute;
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::operator=(Point_Of_SurfaceMesh<MeshType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_SurfaceMesh<MeshType>>();
        m_Data->Index = -1;
        m_Data->VertexIndex = -1;
        m_Data->EdgeIndex = -1;
    }    
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::Clear()
{
    m_Data.reset();
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetParentMesh(SurfaceMesh<MeshType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex = VertexIndex;
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_Data->EdgeIndex = EdgeIndex;
}

template<typename MeshType>
inline 
void Point_Of_SurfaceMesh<MeshType>::MarkAsNonVertex()
{
    m_Data->VertexIndex = -1;
}

template<typename MeshType>
inline
int_max Point_Of_SurfaceMesh<MeshType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshType>
inline
int_max Point_Of_SurfaceMesh<MeshType>::GetVertexIndex() const
{
    return m_Data->VertexIndex;
}

template<typename MeshType>
inline
int_max Point_Of_SurfaceMesh<MeshType>::GetEdgeIndex() const
{
    return m_Data->EdgeIndex;
}


template<typename MeshType>
inline
bool Point_Of_SurfaceMesh<MeshType>::IsValid() const
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

template<typename MeshType>
inline
bool Point_Of_SurfaceMesh<MeshType>::IsVertex() const
{
    return (m_Data->VertexIndex >= 0);
}

template<typename MeshType>
inline
bool Point_Of_SurfaceMesh<MeshType>::IsOnEdge() const
{
    return (m_Data->EdgeIndex >= 0);
}

template<typename MeshType>
inline
bool Point_Of_SurfaceMesh<MeshType>::IsOnBoundaryEdge() const
{
    if (m_Data->EdgeIndex < 0)
    {
        return false;
    }
    else
    {
        return m_Data->Mesh->m_MeshData->EdgeList[EdgeIndex].IsBoundary();
    }
}

template<typename MeshType>
inline
SurfaceMesh<MeshType>& Point_Of_SurfaceMesh<MeshType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
const SurfaceMesh<MeshType>& Point_Of_SurfaceMesh<MeshType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
Handle_Of_Point_Of_SurfaceMesh Point_Of_SurfaceMesh<MeshType>::GetHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(m_Data->Index);
    return PointHandle;
}

template<typename MeshType>
inline
Handle_Of_Vertex_Of_SurfaceMesh Point_Of_SurfaceMesh<MeshType>::GetVertexHandle() const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;
    VertexHandle.SetIndex(m_Data->VertexIndex);
    return VertexHandle;
}

template<typename MeshType>
inline
Handle_Of_Edge_Of_SurfaceMesh Point_Of_SurfaceMesh<MeshType>::GetEdgeHandle() const
{
    Handle_Of_Edge_Of_SurfaceMesh Edgehandle;
    Edgehandle.SetIndex(m_Data->EdgeIndex);
    return Edgehandle;
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetID(int_max PointID)
{
    if (PointID < 0)
    {
        MDK_Error("PointID < 0 @ Point_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto PointID_old = m_Data->Mesh.m_MeshData->PointIDList[m_Data->Index];
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
        else
        {
            MDK_Error("Map_PointID_to_PointIndex is invalid @ Point_Of_SurfaceMesh::SetID(...)")
            return;
        }

        m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex[PointID] = m_Data->Index;
        m_Data->Mesh.m_MeshData->PointIDList[m_Data->Index] = PointID;
    }
}

template<typename MeshType>
inline
int_max Point_Of_SurfaceMesh<MeshType>::GetID() const
{
    return m_Data->Mesh.m_MeshData->PointIDList[m_Data->Index];
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetPosition(const typename MeshType::ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshType>
inline
void Point_Of_SurfaceMesh<MeshType>::SetPosition(typename MeshType::ScalarType x, typename MeshType::ScalarType y, typename MeshType::ScalarType z)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename MeshType>
inline 
void Point_Of_SurfaceMesh<MeshType>::GetPosition(typename MeshType::ScalarType* Pos) const
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename MeshType>
inline 
void Point_Of_SurfaceMesh<MeshType>::GetPosition(typename MeshType::ScalarType& x, typename MeshType::ScalarType& y, typename MeshType::ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };   
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename MeshType>
inline
typename MeshType::PointAttributeType& Point_Of_SurfaceMesh<MeshType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshType>
inline
const typename MeshType::PointAttributeType& Point_Of_SurfaceMesh<MeshType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Vertex_Of_SurfaceMesh ===========================================================//

template<typename MeshType>
inline
Vertex_Of_SurfaceMesh<MeshType>::Vertex_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
Vertex_Of_SurfaceMesh<MeshType>::Vertex_Of_SurfaceMesh(const Vertex_Of_SurfaceMesh<MeshType>& InputVertex)
{
    (*this) = InputVertex;
}

template<typename MeshType>
inline
Vertex_Of_SurfaceMesh<MeshType>::Vertex_Of_SurfaceMesh(Vertex_Of_SurfaceMesh<MeshType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename MeshType>
inline
Vertex_Of_SurfaceMesh<MeshType>::~Vertex_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::operator=(const Vertex_Of_SurfaceMesh<MeshType>& InputVertex)
{
    if (!InputVertex.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputVertex.m_Data->Mesh);
    m_Data->Index = InputVertex.m_Data->Index;
    m_Data->PointIndex = InputVertex.m_Data->PointIndex;
    m_Data->AdjacentVertexIndexList = InputVertex.m_Data->AdjacentVertexIndexList;
    m_Data->AdjacentEdgeIndexList = InputVertex.m_Data->AdjacentEdgeIndexList;
    m_Data->OutgoingDirectedEdgeIndexList = InputVertex.m_Data->OutgoingDirectedEdgeIndexList;
    m_Data->IncomingDirectedEdgeIndexList = InputVertex.m_Data->IncomingDirectedEdgeIndexList;
    m_Data->AdjacentCellIndexList = InputVertex.m_Data->AdjacentCellIndexList;
    m_Data->Attribute = InputVertex.m_Data->Attribute;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::operator=(Vertex_Of_SurfaceMesh<MeshType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Vertex_Of_SurfaceMesh<MeshType>>();
        m_Data->Index = -1;
        m_Data->PointIndex = -1;
    }
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::Clear()
{
    m_Data.reset();
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetParentMesh(SurfaceMesh<MeshType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetIndex(int_max VertexIndex)
{
    m_Data->Index = VertexIndex;
}

template<typename MeshType>
inline
int_max Vertex_Of_SurfaceMesh<MeshType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetPointIndex(int_max PointIndex)
{
    m_Data->PointIndex = PointIndex;
}

template<typename MeshType>
inline
int_max Vertex_Of_SurfaceMesh<MeshType>::GetPointIndex() const
{
    return m_Data->PointIndex;
}

template<typename MeshType>
inline 
DenseVector<int_max>& Vertex_Of_SurfaceMesh<MeshType>::AdjacentVertexIndexList()
{
    return m_Data->AdjacentVertexIndexList;
}

template<typename MeshType>
inline 
DenseVector<int_max>& Vertex_Of_SurfaceMesh<MeshType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshType>
inline 
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Vertex_Of_SurfaceMesh<MeshType>::OutgoingDirectedEdgeIndexList()
{
    return m_Data->OutgoingDirectedEdgeIndexList;
}

template<typename MeshType>
inline
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Vertex_Of_SurfaceMesh<MeshType>::IncomingDirectedEdgeIndexList()
{
    return m_Data->IncomingDirectedEdgeIndexList;
}

template<typename MeshType>
inline
DenseVector<int_max>& Vertex_Of_SurfaceMesh<MeshType>::AdjacentCellIndexList()
{
    return m_Data->AdjacentCellIndexList;
}

template<typename MeshType>
inline
bool Vertex_Of_SurfaceMesh<MeshType>::IsValid() const
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

template<typename MeshType>
inline
bool Vertex_Of_SurfaceMesh<MeshType>::IsBoundary() const
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

template<typename MeshType>
inline
SurfaceMesh<MeshType>& Vertex_Of_SurfaceMesh<MeshType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
const SurfaceMesh<MeshType>& Vertex_Of_SurfaceMesh<MeshType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
Handle_Of_Vertex_Of_SurfaceMesh Vertex_Of_SurfaceMesh<MeshType>::GetHandle() const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;
    VertexHandle.SetIndex(m_Data->Index);
    return VertexHandle;
}

template<typename MeshType>
inline
Handle_Of_Point_Of_SurfaceMesh Vertex_Of_SurfaceMesh<MeshType>::GetPointHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(m_Data->PointIndex);
    return PointHandle;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetID(int_max VertexID)
{
    if (VertexID < 0)
    {
        MDK_Error("VertexID < 0 @ Vertex_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto VertexID_old = m_Data->Mesh.m_MeshData->VertexIDList[m_Data->Index];
    if (VertexID_old != VertexID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex.find(VertexID);
        if (it != m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex.end())
        {
            MDK_Error("Input VertexID has already been used for another vertex @ Vertex_Of_SurfaceMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex.find(VertexID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex.erase(it);
        }
        else
        {
            MDK_Error("Map_VertexID_to_VertexIndex is invalid @ Vertex_Of_SurfaceMesh::SetID(...)")
            return;
        }

        m_Data->Mesh.m_MeshData->Map_VertexID_to_VertexIndex[VertexID] = m_Data->Index;
        m_Data->Mesh.m_MeshData->VertexIDList[m_Data->Index] = VertexID;
    }
}

template<typename MeshType>
inline
int_max Vertex_Of_SurfaceMesh<MeshType>::GetID() const
{
    return m_Data->Mesh.m_MeshData->VertexIDList[m_Data->Index];
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetPosition(const typename MeshType::ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::SetPosition(typename MeshType::ScalarType x, typename MeshType::ScalarType y, typename MeshType::ScalarType z)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->PointIndex, { x, y, z });
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetPosition(typename MeshType::ScalarType* Pos) const
{
    m_Data->Mesh.m_MeshData->PointPositionTable.GetCol(m_Data->PointIndex, Pos);
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetPosition(typename MeshType::ScalarType& x, typename MeshType::ScalarType& y, typename MeshType::ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentVertexHandleList() const
{
    DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentVertexHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentVertexHandleList(DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentVertexIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentVertexIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].Index = m_Data->AdjacentVertexIndexList[k];
    }
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].Index = m_Data->AdjacentEdgeIndexList[k];
    }
}

template<typename MeshType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Vertex_Of_SurfaceMesh<MeshType>::GetOutgoingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetOutgoingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->OutgoingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].Index = m_Data->OutgoingDirectedEdgeIndexList[k];
    }
}

template<typename MeshType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Vertex_Of_SurfaceMesh<MeshType>::GetIncomingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetIncomingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->IncomingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].Index = m_Data->IncomingDirectedEdgeIndexList[k];
    }
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Vertex_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentCellIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentCellIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].Index = m_Data->AdjacentCellIndexList[k];
    }
}

template<typename MeshType>
inline 
typename MeshType::VertexAttributeType& Vertex_Of_SurfaceMesh<MeshType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshType>
inline
const typename MeshType::VertexAttributeType& Vertex_Of_SurfaceMesh<MeshType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Edge_Of_SurfaceMesh ===========================================================//

template<typename MeshType>
inline
Edge_Of_SurfaceMesh<MeshType>::Edge_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
Edge_Of_SurfaceMesh<MeshType>::Edge_Of_SurfaceMesh(const Edge_Of_SurfaceMesh<MeshType>& InputEdge)
{
    (*this) = InputEdge;
}

template<typename MeshType>
inline
Edge_Of_SurfaceMesh<MeshType>::Edge_Of_SurfaceMesh(Edge_Of_SurfaceMesh<MeshType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshType>
inline
Edge_Of_SurfaceMesh<MeshType>::~Edge_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::operator=(const Edge_Of_SurfaceMesh<MeshType>& InputEdge)
{
    if (!InputEdge.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputEdge.m_Data->Mesh);
    m_Data->VertexIndex0 = InputEdge.m_Data->VertexIndex0;
    m_Data->VertexIndex1 = InputEdge.m_Data->VertexIndex1;
    m_Data->PointIndexList = InputEdge.m_Data->PointIndexList;
    m_Data->Attribute = InputEdge.m_Data->Attribute;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::operator=(Edge_Of_SurfaceMesh<MeshType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_SurfaceMesh<MeshType>>();
        m_Data->VertexIndex0 = -1;
        m_Data->VertexIndex1 = -1;
    }    
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::Clear()
{
    m_Data.reset();
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::SetParentMesh(SurfaceMesh<MeshType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename MeshType>
inline
int_max Edge_Of_SurfaceMesh<MeshType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::SetVertexIndexList(const int_max VertexIndexList[2])
{
    this->SetVertexIndexList(VertexIndexList[0], VertexIndexList[1]);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetVertexIndexList(int_max& VertexIndex0, int_max& VertexIndex1) const
{
    VertexIndex0 = m_Data->VertexIndex0;
    VertexIndex1 = m_Data->VertexIndex1;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetVertexIndexList(int_max VertexIndexList[2]) const
{
    this->GetVertexIndexList(VertexIndexList[0], VertexIndexList[1]);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::SetVertexIndexList(int_max VertexIndex0, int_max VertexIndex1)
{
    m_Data->VertexIndex0 = VertexIndex0;
    m_Data->VertexIndex1 = VertexIndex1;
}

template<typename MeshType>
inline DenseVector<int_max>& Edge_Of_SurfaceMesh<MeshType>::PointIndexList()
{
    return m_Data->PointIndexList;
}

template<typename MeshType>
inline
bool Edge_Of_SurfaceMesh<MeshType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false
        }
    }

    return true;
}

template<typename MeshType>
inline
bool Edge_Of_SurfaceMesh<MeshType>::IsBoundary() const
{
    if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][0].IsValid() == false)
    {
        return true
    }
    else if (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index][1].IsValid() == false)
    {
        return true
    }
    else
    {
        return false;
    }
}

template<typename MeshType>
inline
SurfaceMesh<MeshType>& Edge_Of_SurfaceMesh<MeshType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
const SurfaceMesh<MeshType>& Edge_Of_SurfaceMesh<MeshType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
Handle_Of_Edge_Of_SurfaceMesh Edge_Of_SurfaceMesh<MeshType>::GetHandle() const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->Index);
    return EdgeHandle;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::SetID(int_max EdgeID)
{
    if (EdgeID < 0)
    {
        MDK_Error("EdgeID < 0 @ Edge_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto EdgeID_old = m_Data->Mesh.m_MeshData->EdgeIDList[m_Data->Index];
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
        else
        {
            MDK_Error("Map_EdgeID_to_EdgeIndex is invalid @ Edge_Of_SurfaceMesh::SetID(...)")
            return;
        }

        m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeID] = m_Data->Index;
        m_Data->Mesh.m_MeshData->EdgeIDList[m_Data->Index] = EdgeID;
    }
}

template<typename MeshType>
inline
int_max Edge_Of_SurfaceMesh<MeshType>::GetID() const
{
    return m_Data->Mesh.m_MeshData->EdgeIDList[m_Data->Index];
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshType>::GetVertexHandleList() const
{
    DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> OutputHandleList;
    this->GetVertexHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetVertexHandleList(DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(2);
    this->GetVertexHandleList(OutputHandleList[0], OutputHandleList[1]);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetVertexHandleList(Handle_Of_Vertex_Of_SurfaceMesh OutputHandleList[2]) const
{
    this->GetVertexHandleList(OutputHandleList[0], OutputHandleList[1]);
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetVertexHandleList(Handle_Of_Vertex_Of_SurfaceMesh& VertexHandle0, Handle_Of_Vertex_Of_SurfaceMesh& VertexHandle1) const
{
    VertexHandle0.SetIndex(m_Data->VertexIndex0);
    VertexHandle1.SetIndex(m_Data->VertexIndex1);
}

template<typename MeshType>
inline 
DenseVector<Handle_Of_Point_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline 
void Edge_Of_SurfaceMesh<MeshType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->PointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->PointIndexList[k]);
    }
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
    return OutputIndexList;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex1].GetAdjacentEdgeIndexList();

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

template<typename MeshType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
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

template<typename MeshType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Edge_Of_SurfaceMesh<MeshType>::GetNeighbourCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetNeighbourCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Edge_Of_SurfaceMesh<MeshType>::GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentCellIndexList();

    OutputIndexList.FastResize(0);

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

template<typename MeshType>
inline 
typename MeshType::EdgeAttributeType& Edge_Of_SurfaceMesh<MeshType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshType>
inline
const typename MeshType::EdgeAttributeType& Edge_Of_SurfaceMesh<MeshType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== DirectedEdge_Of_SurfaceMesh ===========================================================//

template<typename MeshType>
inline
DirectedEdge_Of_SurfaceMesh<MeshType>::DirectedEdge_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
DirectedEdge_Of_SurfaceMesh<MeshType>::DirectedEdge_Of_SurfaceMesh(const DirectedEdge_Of_SurfaceMesh<MeshType>& InputDirectedEdge)
{
    (*this) = InputDirectedEdge;
}

template<typename MeshType>
inline
DirectedEdge_Of_SurfaceMesh<MeshType>::DirectedEdge_Of_SurfaceMesh(DirectedEdge_Of_SurfaceMesh<MeshType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshType>
inline
DirectedEdge_Of_SurfaceMesh<MeshType>::~DirectedEdge_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::operator=(const DirectedEdge_Of_SurfaceMesh<MeshType>& InputDirectedEdge)
{
    if (!InputDirectedEdge.m_Data)
    {
        return;
    }

    this->Create();
    m_Data->Mesh.ForceShare(InputDirectedEdge.m_Data->Mesh);
    m_Data->Index = InputDirectedEdge.m_Data->Index;
    m_Data->CellIndex = InputDirectedEdge.m_Data->CellIndex;
    m_Data->VertexIndex_start = InputDirectedEdge.m_Data->VertexIndex_start;
    m_Data->VertexIndex_end = InputDirectedEdge.m_Data->VertexIndex_end;
    m_Data->NextDirectedEdgeIndex = InputDirectedEdge.m_Data->NextDirectedEdgeIndex;
    m_Data->PreviousDirectedEdgeIndex = InputDirectedEdge.m_Data->PreviousDirectedEdgeIndex;
    m_Data->Attribute = InputDirectedEdge.m_Data->Attribute;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::operator=(DirectedEdge_Of_SurfaceMesh<MeshType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdge_Of_SurfaceMesh<MeshType>>();
        
        m_Data->Index.EdgeIndex = -1;
        m_Data->Index.RelativeIndex = -1;
        m_Data->CellIndex = -1;
        m_Data->VertexIndex_start = -1;
        m_Data->VertexIndex_end = -1;
        m_Data->NextDirectedEdgeIndex.EdgeIndex = -1;
        m_Data->NextDirectedEdgeIndex.RelativeIndex = -1;
        m_Data->PreviousDirectedEdgeIndex.EdgeIndex = -1;
        m_Data->PreviousDirectedEdgeIndex.RelativeIndex = -1;
    }    
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::Clear()
{
    m_Data.reset();
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetParentMesh(SurfaceMesh<MeshType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->Index.EdgeIndex = EdgeIndex;
    m_Data->Index.RelativeIndex = RelativeIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetCellIndex(int_max CellIndex)
{
    m_Data->CellIndex = CellIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetStartVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_start = VertexIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetEndVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_end = VertexIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->NextDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->NextDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->NextDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->PreviousDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->PreviousDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->PreviousDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetCellIndex() const
{
    return m_Data->CellIndex;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetStartVertexIndex() const
{
    return m_Data->VertexIndex_start;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetEndVertexIndex() const
{
    return m_Data->VertexIndex_end;
}

template<typename MeshType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetFirendDirectedEdgeIndex() const
{
    DirectedEdgeIndex_Of_SurfaceMesh  DirectedEdgeIndex;
    DirectedEdgeIndex.EdgeIndex = m_Data->Index.EdgeIndex;
    DirectedEdgeIndex.RelativeIndex = 1 - m_Data->Index.RelativeIndex;
    return DirectedEdgeIndex;
}

template<typename MeshType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetNextDirectedEdgeIndex() const
{
    return m_Data->NextDirectedEdgeIndex;
}

template<typename MeshType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetPreviousDirectedEdgeIndex() const
{
    return m_Data->PreviousDirectedEdgeIndex;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetFirendCellIndex() const
{
    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.IsValid() == true)
    {
        return FirendDirectedEdge.GetCellIndex();
    }

    return -1;
}

template<typename MeshType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<MeshType>::GetNeighbourCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellIndexList();
}

template<typename MeshType>
inline
bool DirectedEdge_Of_SurfaceMesh<MeshType>::IsValid() const
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

template<typename MeshType>
inline
bool DirectedEdge_Of_SurfaceMesh<MeshType>::IsBoundary() const
{
    return (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex].IsValid() == false);
}

template<typename MeshType>
inline
SurfaceMesh<MeshType>& DirectedEdge_Of_SurfaceMesh<MeshType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
const SurfaceMesh<MeshType>& DirectedEdge_Of_SurfaceMesh<MeshType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline 
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->Index);
    return DirectedEdgeHandle;
}

template<typename MeshType>
inline
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetCellHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(m_Data->CellIndex);
    return CellHandle;
}

template<typename MeshType>
inline
Handle_Of_Vertex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetStartVertexHandle() const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;
    VertexHandle.SetIndex(m_Data->VertexIndex_start);
    return VertexHandle;
}

template<typename MeshType>
inline
Handle_Of_Vertex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetEndVertexHandle() const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;
    VertexHandle.SetIndex(m_Data->VertexIndex_end);
    return VertexHandle;
}

template<typename MeshType>
inline
Handle_Of_Edge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetEdgeHandle() const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->Index.EdgeIndex);
    return EdgeHandle;
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetEdgeID() const
{
    auto EdgeIndex = m_Data->Index.EdgeIndex;
    return m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].GetID() :
}

template<typename MeshType>
inline
int_max DirectedEdge_Of_SurfaceMesh<MeshType>::GetRelativeIndex() const
{
    return m_Data->Index.RelativeIndex;
}

template<typename MeshType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetFirendDirectedEdgeHandle() const
{
    DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex;
    DirectedEdgeIndex.EdgeIndex = m_Data->Index.EdgeIndex;
    DirectedEdgeIndex.RelativeIndex = 1 - m_Data->Index.RelativeIndex;

    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(DirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetNextDirectedEdgeHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->NextDirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetPreviousDirectedEdgeHandle() const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh  DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->PreviousDirectedEdgeIndex);
    return DirectedEdgeHandle;
}

template<typename MeshType>
inline
Handle_Of_Cell_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<MeshType>::GetFirendCellHandle() const
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

template<typename MeshType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> DirectedEdge_Of_SurfaceMesh<MeshType>::GetNeighbourCellHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetNeighbourCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void DirectedEdge_Of_SurfaceMesh<MeshType>::GetNeighbourCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellHandleList();
}

template<typename MeshType>
inline 
typename MeshType::DirectedEdgeAttributeType& DirectedEdge_Of_SurfaceMesh<MeshType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshType>
inline 
const typename MeshType::DirectedEdgeAttributeType& DirectedEdge_Of_SurfaceMesh<MeshType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Cell_Of_SurfaceMesh ===========================================================//

template<typename MeshType>
inline
Cell_Of_SurfaceMesh<MeshType>::Cell_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
Cell_Of_SurfaceMesh<MeshType>::Cell_Of_SurfaceMesh(const Cell_Of_SurfaceMesh<MeshType>& InputCell)
{
    (*this) = InputCell;
}

template<typename MeshType>
inline
Cell_Of_SurfaceMesh<MeshType>::Cell_Of_SurfaceMesh(Cell_Of_SurfaceMesh<MeshType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename MeshType>
inline
Cell_Of_SurfaceMesh<MeshType>::~Cell_Of_SurfaceMesh()
{
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::operator=(const Cell_Of_SurfaceMesh<MeshType>& InputCell)
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

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::operator=(Cell_Of_SurfaceMesh<MeshType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Cell_Of_SurfaceMesh<MeshType>>();
        m_Data->Index = -1;
    }    
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::Clear()
{
    m_Data.reset();
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::SetParentMesh(SurfaceMesh<MeshType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::SetIndex(int_max CellIndex)
{
    m_Data->Index = CellIndex;
}

template<typename MeshType>
inline DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Cell_Of_SurfaceMesh<MeshType>::DirectedEdgeIndexList()
{
    return m_Data->DirectedEdgeIndexList;
}

template<typename MeshType>
inline
int_max Cell_Of_SurfaceMesh<MeshType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshType>::GetVertexIndexList() const
{
    DenseVector<int_max> VertexIndexList;
    this->GetVertexIndexList(VertexIndexList);
    return VertexIndexList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    VertexIndexList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        VertexIndexList[k] = m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempIndex.EdgeIndex][tempIndex.RelativeIndex].GetStartVertexIndex();
    }
}

template<typename MeshType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshType>::GetEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIndexList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputIndexList[k] = m_Data->DirectedEdgeIndexList[k].EdgeIndex;
    }
}

template<typename MeshType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<MeshType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
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

template<typename MeshType>
inline
bool Cell_Of_SurfaceMesh<MeshType>::IsValid() const
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

template<typename MeshType>
inline
SurfaceMesh<MeshType>& Cell_Of_SurfaceMesh<MeshType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
const SurfaceMesh<MeshType>& Cell_Of_SurfaceMesh<MeshType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshType>
inline
Handle_Of_Cell_Of_SurfaceMesh Cell_Of_SurfaceMesh<MeshType>::GetHandle() const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    CellHandle.SetIndex(m_Data->Index);
    return CellHandle;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::SetID(int_max CellID)
{
    if (CellID < 0)
    {
        MDK_Error("CellID < 0 @ Cell_Of_SurfaceMesh::SetID(...)")
        return;
    }

    // check record
    auto CellID_old = m_Data->Mesh.m_MeshData->CellIDList[m_Data->Index];
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
        else
        {
            MDK_Error("Map_CellID_to_CellIndex is invalid @ Cell_Of_SurfaceMesh::SetID(...)")
            return;
        }

        m_Data->Mesh.m_MeshData->Map_CellID_to_CellIndex[CellID] = m_Data->Index;
        m_Data->Mesh.m_MeshData->CellIDList[m_Data->Index] = CellID;
    }
}

template<typename MeshType>
inline 
int_max Cell_Of_SurfaceMesh<MeshType>::GetID() const
{
    return m_Data->Mesh.m_MeshData->CellIDList[m_Data->Index];
}

template<typename MeshType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k]);
    }
}

template<typename MeshType>
inline int_max Cell_Of_SurfaceMesh<MeshType>::GetDirectedEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshType>::GetVertexHandleList() const
{
    DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> OutputHandleList;
    this->GetVertexHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetVertexHandleList(DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& OutputHandleList) const
{    
    OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        OutputHandleList[k].SetIndex(m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempIndex.EdgeIndex][tempIndex.RelativeIndex].GetStartVertexIndex());
    }
}

template<typename MeshType>
inline 
int_max Cell_Of_SurfaceMesh<MeshType>::GetVertexNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshType>::GetEdgeHandleList() const
{
    DenseVector<int_max> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputHandleList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k].EdgeIndex);
    }
}

template<typename MeshType>
inline int_max Cell_Of_SurfaceMesh<MeshType>::GetEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> Cell_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetAdjacentCellHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshType>
inline
void Cell_Of_SurfaceMesh<MeshType>::GetAdjacentCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
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

template<typename MeshType>
inline 
typename MeshType::CellAttributeType& Cell_Of_SurfaceMesh<MeshType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshType>
inline
const typename MeshType::CellAttributeType& Cell_Of_SurfaceMesh<MeshType>::Attribute() const
{
    return m_Data->Attribute;
}

}// namespace mdk

#endif