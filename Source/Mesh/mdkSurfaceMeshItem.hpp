#ifndef __mdkSurfaceMeshItem_hpp
#define __mdkSurfaceMeshItem_hpp


namespace mdk
{
//=========================================================== Point_Of_SurfaceMesh ===========================================================//

template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>::Point_Of_SurfaceMesh()
{
    this->Create();
}

template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>::Point_Of_SurfaceMesh(const Point_Of_SurfaceMesh<ScalarType>& InputVertex)
{
    this->Create();
    (*this) = InputVertex;
}

template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>::Point_Of_SurfaceMesh(Point_Of_SurfaceMesh<ScalarType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename ScalarType>
inline
Point_Of_SurfaceMesh<ScalarType>::~Point_Of_SurfaceMesh()
{
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::operator=(const Point_Of_SurfaceMesh<ScalarType>& InputVertex)
{
    m_Data->Mesh.ForceShare(InputVertex.m_Data->Mesh);
    m_Data->Index = InputVertex.m_Data->Index;
    m_Data->VertexIndex = InputVertex.m_Data->VertexIndex;
    m_Data->EdgeIndex = InputVertex.m_Data->EdgeIndex;
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::operator=(Point_Of_SurfaceMesh<ScalarType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_SurfaceMesh<ScalarType>>();
        m_Data->Index = -1;
        m_Data->VertexIndex = -1;
        m_Data->EdgeIndex = -1;
    }    
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::Clear()
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex = VertexIndex;
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_Data->EdgeIndex = EdgeIndex;
}

template<typename ScalarType>
inline 
void Point_Of_SurfaceMesh<ScalarType>::MarkAsNonVertex()
{
    m_Data->VertexIndex = -1;
}

template<typename ScalarType>
inline
bool Point_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
bool Point_Of_SurfaceMesh<ScalarType>::IsVertex() const
{
    return (m_Data->VertexIndex >= 0);
}

template<typename ScalarType>
inline
bool Point_Of_SurfaceMesh<ScalarType>::IsOnEdge() const
{
    return (m_Data->EdgeIndex >= 0);
}

template<typename ScalarType>
inline
bool Point_Of_SurfaceMesh<ScalarType>::IsOnBoundaryEdge() const
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

template<typename ScalarType>
inline
SurfaceMesh<ScalarType>& Point_Of_SurfaceMesh<ScalarType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
const SurfaceMesh<ScalarType>& Point_Of_SurfaceMesh<ScalarType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
Handle_Of_Point_Of_SurfaceMesh Point_Of_SurfaceMesh<ScalarType>::GetHandle() const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.Index = m_Data->Index;
    return PointHandle;
}

template<typename ScalarType>
inline
Handle_Of_Vertex_Of_SurfaceMesh Point_Of_SurfaceMesh<ScalarType>::GetVertexHandle() const
{
    Handle_Of_Vertex_Of_SurfaceMesh VertexHandle;
    VertexHandle.Index = m_Data->VertexIndex;
    return VertexHandle;
}

template<typename ScalarType>
inline
Handle_Of_Edge_Of_SurfaceMesh Point_Of_SurfaceMesh<ScalarType>::GetEdgeIndex() const
{
    Handle_Of_Edge_Of_SurfaceMesh Edgehandle;
    Edgehandle.Index = m_Data->EdgeIndex;
    return Edgehandle;
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetID(int_max PointID) const
{
    m_Data->Mesh.m_MeshData->PointIDList[m_Data->Index] = PointID;
}

template<typename ScalarType>
inline
int_max Point_Of_SurfaceMesh<ScalarType>::GetID() const
{
    return m_Data->Mesh.m_MeshData->PointIDList[m_Data->Index];
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetPosition(const ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename ScalarType>
inline
void Point_Of_SurfaceMesh<ScalarType>::SetPosition(ScalarType x, ScalarType y, ScalarType z)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename ScalarType>
inline 
void Point_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType* Pos) const
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename ScalarType>
inline 
void Point_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };   
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

//=========================================================== Vertex_Of_SurfaceMesh ===========================================================//

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>::Vertex_Of_SurfaceMesh()
{
    this->Create();
}

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>::Vertex_Of_SurfaceMesh(const Vertex_Of_SurfaceMesh<ScalarType>& InputVertex)
{
    this->Create();
    (*this) = InputVertex;
}

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>::Vertex_Of_SurfaceMesh(Vertex_Of_SurfaceMesh<ScalarType>&& InputVertex)
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename ScalarType>
inline
Vertex_Of_SurfaceMesh<ScalarType>::~Vertex_Of_SurfaceMesh() 
{
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::operator=(const Vertex_Of_SurfaceMesh<ScalarType>& InputVertex) 
{
    m_Data->Mesh.ForceShare(InputVertex.m_Data->Mesh);
    m_Data->Index = InputVertex.m_Data->Index;
    m_Data->PointIndex = InputVertex.m_Data->PointIndex;
    m_Data->AdjacentVertexIndexList = InputVertex.m_Data->AdjacentVertexIndexList;
    m_Data->AdjacentEdgeIndexList = InputVertex.m_Data->AdjacentEdgeIndexList;
    m_Data->OutgoingDirectedEdgeIndexList = InputVertex.m_Data->OutgoingDirectedEdgeIndexList;
    m_Data->IncomingDirectedEdgeIndexList = InputVertex.m_Data->IncomingDirectedEdgeIndexList;
    m_Data->AdjacentCellIndexList = InputVertex.m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::operator=(Vertex_Of_SurfaceMesh<ScalarType>&& InputVertex) 
{
    m_Data = std::move(InputVertex.m_Data);
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Vertex_Of_SurfaceMesh<ScalarType>>();
        m_Data->Index = -1;
        m_Data->PointIndex = -1;
    }
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::Clear()
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetIndex(int_max VertexIndex)
{
    m_Data->Index = VertexIndex;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetPointIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename ScalarType>
inline 
DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::AdjacentVertexIndexList()
{
    return m_Data->AdjacentVertexIndexList;
}

template<typename ScalarType>
inline 
DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline 
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Vertex_Of_SurfaceMesh<ScalarType>::OutgoingDirectedEdgeIndexList()
{
    return m_Data->OutgoingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
DenseVector<DirectedEdgeIndex_Of_SurfaceMesh>& Vertex_Of_SurfaceMesh<ScalarType>::IncomingDirectedEdgeIndexList()
{
    return m_Data->IncomingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::AdjacentCellIndexList()
{
    return m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
bool Vertex_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline Vertex_Of_SurfaceMesh<ScalarType>::IsBoundary() const
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

template<typename ScalarType>
inline
SurfaceMesh<ScalarType>& Vertex_Of_SurfaceMesh<ScalarType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
const SurfaceMesh<ScalarType>& Vertex_Of_SurfaceMesh<ScalarType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
int_max Vertex_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetGlobalID(int_max VertexGlobalID)
{
    m_Data->Mesh.m_MeshData->VertexGlobalIDList[m_Data->Index] = VertexGlobalID;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetPosition(const ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetPosition(ScalarType x, ScalarType y, ScalarType z)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.SetCol(m_Data->Index, {x, y, z});
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType* Pos) const
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.GetCol(m_Index, Pos);
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentVertexIndexList() const
{
    return m_Data->AdjacentVertexIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->AdjacentVertexIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::GetOutgoingDirectedEdgeIndexList() const
{
    return m_Data->OutgoingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetOutgoingDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->OutgoingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::GetIncomingDirectedEdgeIndexList() const
{
    return m_Data->IncomingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetIncomingDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->IncomingDirectedEdgeIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList() const
{
    return m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->AdjacentCellIndexList;
}

//=========================================================== Edge_Of_SurfaceMesh ===========================================================//

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>::Edge_Of_SurfaceMesh()
{
    this->Create();
}

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>::Edge_Of_SurfaceMesh(const Edge_Of_SurfaceMesh<ScalarType>& InputEdge)
{
    this->Create();
    (*this) = InputEdge;
}

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>::Edge_Of_SurfaceMesh(Edge_Of_SurfaceMesh<ScalarType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename ScalarType>
inline
Edge_Of_SurfaceMesh<ScalarType>::~Edge_Of_SurfaceMesh()
{
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::operator=(const Edge_Of_SurfaceMesh<ScalarType>& InputEdge)
{
    m_Data->Mesh.ForceShare(InputEdge.m_Data->Mesh);
    m_Data->VertexIndex0 = InputEdge.m_Data->VertexIndex0;
    m_Data->VertexIndex1 = InputEdge.m_Data->VertexIndex1;
    m_Data->PointIndexList = InputEdge.m_Data->PointIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::operator=(Edge_Of_SurfaceMesh<ScalarType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_SurfaceMesh<ScalarType>>();
        m_Data->VertexIndex0 = -1;
        m_Data->VertexIndex1 = -1;
    }    
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::Clear()
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetVertexIndexList(const int_max VertexIndexList[2])
{
    this->SetVertexIndexList(VertexIndexList[0], VertexIndexList[1]);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetVertexIndexList(int_max VertexIndex0, int_max VertexIndex1)
{
    m_Data->VertexIndex0 = VertexIndex0;
    m_Data->VertexIndex1 = VertexIndex1;
}

template<typename ScalarType>
inline DenseVector<int_max>& Edge_Of_SurfaceMesh<ScalarType>::PointIndexList()
{
    return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
bool Edge_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
bool Edge_Of_SurfaceMesh<ScalarType>::IsBoundary() const
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

template<typename ScalarType>
inline
SurfaceMesh<ScalarType>& Edge_Of_SurfaceMesh<ScalarType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
const SurfaceMesh<ScalarType>& Edge_Of_SurfaceMesh<ScalarType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
int_max Edge_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetDirectedEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<ScalarType>::GetVertexIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetVertexIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    this->GetVertexIndexList(OutputIndexList.GetPointer());
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetVertexIndexList(int_max OutputIndexList[2]) const
{
    this->GetVertexIndexList(OutputIndexList[0], OutputIndexList[1]);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetVertexIndexList(int_max& VertexIndex0, int_max& VertexIndex1) const
{
    VertexIndex0 = m_Data->VertexIndex0;
    VertexIndex1 = m_Data->VertexIndex1;
}

template<typename ScalarType>
inline 
const DenseVector<int_max>& Edge_Of_SurfaceMesh<ScalarType>::GetPointIndexList() const
{
    return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex1].GetAdjacentEdgeIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

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

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
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

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_SurfaceMesh<ScalarType>::GetNeighbourCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetNeighbourCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    const auto& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentCellIndexList();
    const auto& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->VertexList[m_Data->VertexIndex0].GetAdjacentCellIndexList();

    OutputIndexList.FastResize(0);

    if (AdjacentCellIndexList0.IsEmpty() == true && AdjacentCellIndexList1.IsEmpty() == true)
    {
        return;
    }

    OutputIndexList.ReserveCapacity(AdjacentCellIndexList0.GetLength() + AdjacentCellIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentCellIndexList0.GetLength(); ++k)
    {
        OutputIndexList.Append(tempIndex);
    }

    for (int_max k = 0; k < AdjacentCellIndexList1.GetLength(); ++k)
    {
        OutputIndexList.Append(tempIndex);
    }

    auto tempIndexList = OutputIndexList.FindUnique();
    OutputIndexList = OutputIndexList.GetSubSet(tempIndexList);
}


//=========================================================== DirectedEdge_Of_SurfaceMesh ===========================================================//

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>::DirectedEdge_Of_SurfaceMesh()
{
    this->Create();
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>::DirectedEdge_Of_SurfaceMesh(const DirectedEdge_Of_SurfaceMesh<ScalarType>& InputDirectedEdge)
{
    this->Create();
    (*this) = InputDirectedEdge;
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>::DirectedEdge_Of_SurfaceMesh(DirectedEdge_Of_SurfaceMesh<ScalarType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename ScalarType>
inline
DirectedEdge_Of_SurfaceMesh<ScalarType>::~DirectedEdge_Of_SurfaceMesh()
{
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::operator=(const DirectedEdge_Of_SurfaceMesh<ScalarType>& InputDirectedEdge)
{
    m_Data->Mesh.ForceShare(InputDirectedEdge.m_Data->Mesh);
    m_Data->Index = InputDirectedEdge.m_Data->Index;
    m_Data->CellIndex = InputDirectedEdge.m_Data->CellIndex;
    m_Data->VertexIndex_start = InputDirectedEdge.m_Data->VertexIndex_start;
    m_Data->VertexIndex_end = InputDirectedEdge.m_Data->VertexIndex_end;
    m_Data->NextDirectedEdgeIndex = InputDirectedEdge.m_Data->NextDirectedEdgeIndex;
    m_Data->PreviousDirectedEdgeIndex = InputDirectedEdge.m_Data->PreviousDirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::operator=(DirectedEdge_Of_SurfaceMesh<ScalarType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdge_Of_SurfaceMesh<ScalarType>>();
        
        m_Data->Index = -1;
        m_Data->CellIndex = -1;
        m_Data->VertexIndex_start = -1;
        m_Data->VertexIndex_end = -1;
        m_Data->NextDirectedEdgeIndex = -1;
        m_Data->PreviousDirectedEdgeIndex = -1;
    }    
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::Clear()
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->Index.EdgeIndex = EdgeIndex;
    m_Data->Index.RelativeIndex = RelativeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetCellIndex(int_max CellIndex)
{
    m_Data->CellIndex = CellIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetStartVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_start = VertexIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetEndVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_end = VertexIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetNextDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->NextDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetNextDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->NextDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->NextDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Data->PreviousDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetPreviousDirectedEdgeIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Data->PreviousDirectedEdgeIndex.EdgeIndex = EdgeIndex;
    m_Data->PreviousDirectedEdgeIndex.RelativeIndex = RelativeIndex;
}

template<typename ScalarType>
inline
bool DirectedEdge_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
bool DirectedEdge_Of_SurfaceMesh<ScalarType>::IsBoundary() const
{
    return (m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex].IsValid() == false);
}

template<typename ScalarType>
inline
SurfaceMesh<ScalarType>& DirectedEdge_Of_SurfaceMesh<ScalarType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
const SurfaceMesh<ScalarType>& DirectedEdge_Of_SurfaceMesh<ScalarType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}


template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetGlobalID(int_max CellGlobalID)
{
    m_Data->Mesh.m_MeshData->CellGlobalIDList[m_Data->Index] = CellGlobalID;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetCellIndex() const
{
    return m_Data->CellIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetStartVertexIndex() const
{
    return m_Data->VertexIndex_start;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetEndVertexIndex() const
{
    return m_Data->VertexIndex_end;
}

template<typename ScalarType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<ScalarType>::GetFirendDirectedEdgeIndex() const
{
    DirectedEdgeIndex_Of_SurfaceMesh  DirectedEdgeIndex;
    DirectedEdgeIndex.EdgeIndex = m_Data->Index.EdgeIndex;
    DirectedEdgeIndex.RelativeIndex = 1 - m_Data->Index.RelativeIndex;
    return DirectedEdgeIndex;
}

template<typename ScalarType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<ScalarType>::GetNextDirectedEdgeIndex() const
{
    return m_Data->NextDirectedEdgeIndex;
}

template<typename ScalarType>
inline
DirectedEdgeIndex_Of_SurfaceMesh DirectedEdge_Of_SurfaceMesh<ScalarType>::GetPreviousDirectedEdgeIndex() const
{
    return m_Data->PreviousDirectedEdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetFirendCellIndex() const
{
    const auto& FirendDirectedEdge = m_Data->Mesh.m_MeshData->DirectedEdgePairList[m_Data->Index.EdgeIndex][1 - m_Data->Index.RelativeIndex];
    if (FirendDirectedEdge.IsValid() == true)
    {
        return FirendDirectedEdge.GetCellIndex();
    }

    return -1;
}

template<typename ScalarType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<ScalarType>::GetNeighbourCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::GetNeighbourCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].GetNeighbourCellIndexList();
}

//=========================================================== Cell_Of_SurfaceMesh ===========================================================//

template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>::Cell_Of_SurfaceMesh()
{
    this->Create();
}

template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>::Cell_Of_SurfaceMesh(const Cell_Of_SurfaceMesh<ScalarType>& InputCell)
{
    this->Create();
    (*this) = InputCell;
}

template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>::Cell_Of_SurfaceMesh(Cell_Of_SurfaceMesh<ScalarType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename ScalarType>
inline
Cell_Of_SurfaceMesh<ScalarType>::~Cell_Of_SurfaceMesh()
{
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::operator=(const Cell_Of_SurfaceMesh<ScalarType>& InputCell)
{
    m_Data->Mesh.ForceShare(InputCell.m_Data->Mesh);
    m_Data->Index = InputCell.m_Data->Index;
    m_Data->DirectedEdgeIndexList = InputCell.m_Data->DirectedEdgeIndexList;
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::operator=(Cell_Of_SurfaceMesh<ScalarType>&& InputCell)
{
    m_Data = std::move(InputCell.m_Data);
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::Create()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Cell_Of_SurfaceMesh<ScalarType>>();
        m_Data->Index = -1;
    }    
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::Clear()
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::SetIndex(int_max CellIndex)
{
    m_Data->Index = CellIndex;
}

template<typename ScalarType>
inline DenseVector<int_max>& Cell_Of_SurfaceMesh<ScalarType>::DirectedEdgeIndexList()
{
    return m_Data->DirectedEdgeIndexList;
}

template<typename ScalarType>
inline
bool Cell_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
SurfaceMesh<ScalarType>& Cell_Of_SurfaceMesh<ScalarType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
const SurfaceMesh<ScalarType>& Cell_Of_SurfaceMesh<ScalarType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename ScalarType>
inline
int_max Cell_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Cell_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList() const
{
    return m_Data->DirectedEdgeIndexList;
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_Data->DirectedEdgeIndexList;
}

template<typename ScalarType>
inline int_max Cell_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Cell_Of_SurfaceMesh<ScalarType>::GetVertexIndexList() const
{
    DenseVector<int_max> VertexIndexList;
    this->GetVertexIndexList(VertexIndexList);
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{    
    VertexIndexList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_Data->DirectedEdgeIndexList[k];
        VertexIndexList[k] = m_Data->Mesh.m_MeshData->DirectedEdgePairList[tempIndex.EdgeIndex][tempIndex.RelativeIndex].GetStartVertexIndex();
    }
}

template<typename ScalarType>
inline 
int_max Cell_Of_SurfaceMesh<ScalarType>::GetVertexNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<ScalarType>::GetEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max EdgeNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIndexList.FastResize(EdgeNumber);
    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        OutputIndexList[k] = m_Data->DirectedEdgeIndexList[k].EdgeIndex;
    }
}

template<typename ScalarType>
inline int_max Cell_Of_SurfaceMesh<ScalarType>::GetEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
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

}// namespace mdk

#endif