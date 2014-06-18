#ifndef __mdkSurfaceMeshItem_hpp
#define __mdkSurfaceMeshItem_hpp


namespace mdk
{
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
bool Vertex_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
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
void Vertex_Of_SurfaceMesh<ScalarType>::SetIndex(int_max VertexIndex)
{
    m_Data->Index = VertexIndex;
}

template<typename ScalarType>
inline
int_max Vertex_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline 
void Vertex_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType* Pos)
{
    m_Data->Mesh.m_MeshData->VertexPositionTable.GetCol(m_Index, Pos);
}

template<typename ScalarType>
inline 
void Vertex_Of_SurfaceMesh<ScalarType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z)
{
    ScalarType Pos[3] = { 0, 0, 0 };   
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename ScalarType>
inline
void Vertex_Of_SurfaceMesh<ScalarType>::SetOutgoingDirectedEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_Data->OutgoingDirectedEdgeIndexList = std::move(IndexList);
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
void Vertex_Of_SurfaceMesh<ScalarType>::SetIncomingDirectedEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_Data->IncomingDirectedEdgeIndexList = std::move(IndexList);
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
void Vertex_Of_SurfaceMesh<ScalarType>::SetAdjacentVertexIndexList(DenseVector<int_max> IndexList)
{
    m_Data->AdjacentVertexIndexList = std::move(IndexList);
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
void Vertex_Of_SurfaceMesh<ScalarType>::SetAdjacentEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_Data->AdjacentEdgeIndexList = std::move(IndexList);
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
void Vertex_Of_SurfaceMesh<ScalarType>::SetAdjacentCellIndexList(DenseVector<int_max> IndexList)
{
    m_Data->AdjacentCellIndexList = std::move(IndexList);
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

template<typename ScalarType>
inline
bool Vertex_Of_SurfaceMesh<ScalarType>::IsBoundary() const
{
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        if (m_Data->AdjacentEdgeIndexList[k].IsBoundary() == true)
        {
            return true;
        }
    }
    return false;
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
    m_Data->DirectedEdgeIndex0 = InputEdge.m_Data->DirectedEdgeIndex0;
    m_Data->DirectedEdgeIndex1 = InputEdge.m_Data->DirectedEdgeIndex1;
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
        m_Data->DirectedEdgeIndex0 = -1;
        m_Data->DirectedEdgeIndex1 = -1;
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
bool Edge_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
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
void Edge_Of_SurfaceMesh<ScalarType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename ScalarType>
inline
int_max Edge_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::SetDirectedEdgeIndexList(int_max DirectedEdgeIndex0, int_max DirectedEdgeIndex1)
{
    m_Data->DirectedEdgeIndex0 = DirectedEdgeIndex0;
    m_Data->DirectedEdgeIndex1 = DirectedEdgeIndex1;
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
void Edge_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    this->GetDirectedEdgeIndexList(OutputIndexList[0], OutputIndexList[1]);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList(int_max OutputIndexList[2]) const
{
    this->GetDirectedEdgeIndexList(OutputIndexList[0], OutputIndexList[1]);
}

template<typename ScalarType>
inline
void Edge_Of_SurfaceMesh<ScalarType>::GetDirectedEdgeIndexList(int_max& DirectedEdgeIndex0, int_max& DirectedEdgeIndex1) const
{
    DirectedEdgeIndex0 = m_Data->DirectedEdgeIndex0;
    DirectedEdgeIndex1 = m_Data->DirectedEdgeIndex1;
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
    if (m_Data->DirectedEdgeIndex0 >= 0)
    {
        VertexIndex0 = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndex0].GetStartVertexIndex();
        VertexIndex1 = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndex0].GetEndVertexIndex();
    }
    else
    {
        VertexIndex0 = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndex1].GetStartVertexIndex();
        VertexIndex1 = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndex1].GetEndVertexIndex();
    }
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
    int_max VertexIndexList[2];
    this->GetVertexIndexList(VertexIndexList);
    const DenseVector<int_max>& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->VertexList[VertexIndexList[0]].GetAdjacentEdgeIndexList();
    const DenseVector<int_max>& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->VertexList[VertexIndexList[1]].GetAdjacentEdgeIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Index)
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
    int_max VertexIndexList[2];
    this->GetVertexIndexList(VertexIndexList);
    const DenseVector<int_max>& AdjacentCellIndexList0 = m_Data->Mesh.m_MeshData->VertexList[VertexIndexList[0]].GetAdjacentCellIndexList();
    const DenseVector<int_max>& AdjacentCellIndexList1 = m_Data->Mesh.m_MeshData->VertexList[VertexIndexList[1]].GetAdjacentCellIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(AdjacentCellIndexList0.GetLength() + AdjacentCellIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentCellIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentCellIndexList0[k];
        if (tempIndex != m_Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }

    for (int_max k = 0; k < AdjacentCellIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentCellIndexList1[k];
        if (tempIndex != m_Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }
}

template<typename ScalarType>
inline
bool Edge_Of_SurfaceMesh<ScalarType>::IsBoundary() const
{
    return (m_Data->DirectedEdgeIndex0 < 0 || m_Data->DirectedEdgeIndex1 < 0);
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
    m_Data->VertexIndex_start = InputDirectedEdge.m_Data->VertexIndex_start;
    m_Data->VertexIndex_end = InputDirectedEdge.m_Data->VertexIndex_end;
    m_Data->EdgeIndex = InputDirectedEdge.m_Data->EdgeIndex;
    m_Data->CellIndex = InputDirectedEdge.m_Data->CellIndex;
    m_Data->OppositeDirectedEdgeIndex = InputDirectedEdge.m_Data->OppositeDirectedEdgeIndex;
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
        m_Data->VertexIndex_start = -1;
        m_Data->VertexIndex_end = -1;
        m_Data->EdgeIndex = -1;
        m_Data->CellIndex = -1;
        m_Data->OppositeDirectedEdgeIndex = -1;
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
bool DirectedEdge_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
bool DirectedEdge_Of_SurfaceMesh<ScalarType>::IsCleard() const
{
    return (!m_Data);
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
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
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetIndex(int_max DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetStartVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_start = VertexIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetStartVertexIndex() const
{
    return m_Data->VertexIndex_start;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetEndVertexIndex(int_max VertexIndex)
{
    m_Data->VertexIndex_end = VertexIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetEndVertexIndex() const
{
    return m_Data->VertexIndex_end;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_Data->EdgeIndex = EdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetEdgeIndex() const
{
    return m_Data->EdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetCellIndex(int_max CellIndex)
{
    m_Data->CellIndex = CellIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetCellIndex() const
{
    return m_Data->CellIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetOppositeDirectedEdgeIndex(int_max DirectedEdgeIndex)
{
    m_Data->OppositeDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetOppositeDirectedEdgeIndex() const
{
    return m_Data->OppositeDirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetNextDirectedEdgeIndex(int_max DirectedEdgeIndex)
{
    m_Data->NextDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetNextDirectedEdgeIndex() const
{
    return m_Data->NextDirectedEdgeIndex;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::SetPreviousDirectedEdgeIndex(int_max DirectedEdgeIndex)
{
    m_Data->PreviousDirectedEdgeIndex = DirectedEdgeIndex;
}

template<typename ScalarType>
inline
int_max DirectedEdge_Of_SurfaceMesh<ScalarType>::GetPreviousDirectedEdgeIndex() const
{
    return m_Data->PreviousDirectedEdgeIndex;
}

template<typename ScalarType>
inline
DenseVector<int_max> DirectedEdge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentCellIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    this->GetAdjacentCellIndexList(OutputIndexList[0], OutputIndexList[1]);
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(int_max OutputIndexList[2]) const
{
    this->GetAdjacentCellIndexList(OutputIndexList[0], OutputIndexList[1]);
}

template<typename ScalarType>
inline
void DirectedEdge_Of_SurfaceMesh<ScalarType>::GetAdjacentCellIndexList(int_max& CellIndex0, int_max& CellIndex1) const
{
    CellIndex0 = m_Data->CellIndex;
    CellIndex1 = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->OppositeDirectedEdgeIndex].GetCellIndex();
}

//-------------------------------------------------------------------------------------//
template<typename ScalarType>
inline
bool DirectedEdge_Of_SurfaceMesh<ScalarType>::IsBoundary() const
{
    return (m_Data->OppositeDirectedEdgeIndex < 0);
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
bool Cell_Of_SurfaceMesh<ScalarType>::IsValid() const
{
    return bool(m_Data);
}

template<typename ScalarType>
inline
bool Cell_Of_SurfaceMesh<ScalarType>::IsCleard() const
{
    return (!m_Data);
}

template<typename ScalarType>
inline
bool Cell_Of_SurfaceMesh<ScalarType>::IsTriangle() const
{
    return (this->GetVertexNumber() == 3);
}


template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::SetParentMesh(SurfaceMesh<ScalarType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
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
void Cell_Of_SurfaceMesh<ScalarType>::SetIndex(int_max CellIndex)
{
    m_Data->Index = CellIndex;
}

template<typename ScalarType>
inline
int_max Cell_Of_SurfaceMesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::SetDirectedEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_Data->DirectedEdgeIndexList = std::move(IndexList);
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
    return m_Data->DirectedEdgeIndexList.GetElementNumber();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Cell_Of_SurfaceMesh<ScalarType>::GetVertexIndexList() const
{
    return m_Data->Mesh.m_MeshData->VertexIndexTable[m_Data->Index];
}

template<typename ScalarType>
inline
void Cell_Of_SurfaceMesh<ScalarType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{    
    OutputIndexList = m_Data->Mesh.m_MeshData->VertexIndexTable[m_Data->Index];
    // OutputIndexList[k] is the index of the starting vertex of the DirectedEdge (m_DirectedEdgeIndexList[k])
}

template<typename ScalarType>
inline 
int_max Cell_Of_SurfaceMesh<ScalarType>::GetVertexNumber() const
{
    return m_Data->Mesh.m_MeshData->VertexIndexTable[m_Data->Index].GetElementNumber();
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
    int_max VertexNumber = m_Data->DirectedEdgeIndexList.GetLength();
    OutputIndexList.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        OutputIndexList[k] = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndexList[k]].GetEdgeIndex;
    }
}

template<typename ScalarType>
inline int_max Cell_Of_SurfaceMesh<ScalarType>::GetEdgeNumber() const
{
    return m_Data->DirectedEdgeIndexList.GetElementNumber();
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
        auto OppositeDirectedEdgeIndex = m_Data->Mesh.m_MeshData->DirectedEdgeList[m_Data->DirectedEdgeIndexList[k]].GetOppositeDirectedEdgeIndex();
        if (OppositeDirectedEdgeIndex >= 0)
        {
            auto tempIndex = m_Data->Mesh.m_MeshData->DirectedEdgeList[OppositeDirectedEdgeIndex].GetCellIndex();
            OutputIndexList.Append(tempIndex);
        }
    }
    OutputIndexList.ReleaseUnusedCapacity();
}

}// namespace mdk

#endif