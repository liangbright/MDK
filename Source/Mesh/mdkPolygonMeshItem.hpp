#ifndef __mdkPolygonMeshItem_hpp
#define __mdkPolygonMeshItem_hpp


namespace mdk
{
//=========================================================== Vertex_Of_PolygonMesh ===========================================================//

template<typename ScalarType>
inline
Vertex_Of_PolygonMesh<ScalarType>::Vertex_Of_PolygonMesh(PolygonMesh<ScalarType>* ParentMesh)
{
    this->Clear();
    m_Mesh = ParentMesh;
}

template<typename ScalarType>
inline
Vertex_Of_PolygonMesh<ScalarType>::Vertex_Of_PolygonMesh(const Vertex_Of_PolygonMesh<ScalarType>& InputVertex)
{
    this->Copy(InputVertex);
}

template<typename ScalarType>
inline
Vertex_Of_PolygonMesh<ScalarType>::Vertex_Of_PolygonMesh(Vertex_Of_PolygonMesh<ScalarType>&& InputVertex)
{
    this->Take(std::forward<Vertex_Of_PolygonMesh<ScalarType>&>(InputVertex));
}

template<typename ScalarType>
inline
Vertex_Of_PolygonMesh<ScalarType>::~Vertex_Of_PolygonMesh() 
{
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::operator=(const Vertex_Of_PolygonMesh<ScalarType>& InputVertex) 
{
    this->Copy(InputVertex); 
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::operator=(Vertex_Of_PolygonMesh<ScalarType>&& InputVertex) 
{
    this->Take(std::forward<Vertex_Of_PolygonMesh<ScalarType>&>(InputVertex));
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::Clear()
{
    m_Mesh = nullptr;
    m_Index = -1;
    m_AdjacentVertexIndexList.Clear();
    m_AdjacentEdgeIndexList.Clear();
    m_AdjacentHalfEdgeIndexList.Clear();
    m_AdjacentPolygonIndexList.Clear();    
}

template<typename ScalarType>
void Vertex_Of_PolygonMesh<ScalarType>::Copy(const Vertex_Of_PolygonMesh<ScalarType>& InputVertex)
{
    m_Mesh = InputVertex.m_Mesh;
    m_Index = InputVertex.m_Index;
    m_AdjacentVertexIndexList = InputVertex.m_AdjacentVertexIndexList;
    m_AdjacentEdgeIndexList = InputVertex.m_AdjacentEdgeIndexList;
    m_AdjacentHalfEdgeIndexList = InputVertex.m_AdjacentHalfEdgeIndexList;
    m_AdjacentPolygonIndexList = InputVertex.m_AdjacentPolygonIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::Take(Vertex_Of_PolygonMesh<ScalarType>& InputVertex)
{
    m_Mesh = InputVertex.m_Mesh;
    m_Index = InputVertex.m_Index;
    m_AdjacentVertexIndexList = std::move(InputVertex.m_AdjacentVertexIndexList);
    m_AdjacentEdgeIndexList = std::move(InputVertex.m_AdjacentEdgeIndexList);
    m_AdjacentHalfEdgeIndexList = std::move(InputVertex.m_AdjacentHalfEdgeIndexList);
    m_AdjacentPolygonIndexList = std::move(InputVertex.m_AdjacentPolygonIndexList);

    InputVertex.Clear();
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetParentMesh(PolygonMesh<ScalarType>* InputMesh)
{
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
PolygonMesh<ScalarType>* Vertex_Of_PolygonMesh<ScalarType>::GetParentMesh()
{
    return m_Mesh;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetIndex(int_max VertexIndex)
{
    m_Index = VertexIndex;
}

template<typename ScalarType>
inline
int_max Vertex_Of_PolygonMesh<ScalarType>::GetIndex() const
{
    return m_Index;
}

template<typename ScalarType>
inline 
void Vertex_Of_PolygonMesh<ScalarType>::GetPosition(ScalarType* Pos)
{
    m_Mesh->m_MeshData->VertexPositionTable.GetCol(m_Index, Pos);
}

template<typename ScalarType>
inline 
void Vertex_Of_PolygonMesh<ScalarType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z)
{
    ScalarType Pos[3] = { 0, 0, 0 };   
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetAdjacentHalfEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_AdjacentHalfEdgeIndexList = std::move(IndexList);
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentHalfEdgeIndexList() const
{
    return m_AdjacentHalfEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_AdjacentHalfEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetAdjacentVertexIndexList(DenseVector<int_max> IndexList)
{
    m_AdjacentVertexIndexList = std::move(IndexList);
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentVertexIndexList() const
{
    return m_AdjacentVertexIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_AdjacentVertexIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetAdjacentEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_AdjacentEdgeIndexList = std::move(IndexList);
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
    return m_AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::SetAdjacentPolygonIndexList(DenseVector<int_max> IndexList)
{
    m_AdjacentPolygonIndexList = std::move(IndexList);
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList() const
{
    return m_AdjacentPolygonIndexList;
}

template<typename ScalarType>
inline
void Vertex_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_AdjacentPolygonIndexList;
}

//=========================================================== Edge_Of_PolygonMesh ===========================================================//

template<typename ScalarType>
inline
Edge_Of_PolygonMesh<ScalarType>::Edge_Of_PolygonMesh(PolygonMesh<ScalarType>* InputMesh)
{
    this->Clear();
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
Edge_Of_PolygonMesh<ScalarType>::Edge_Of_PolygonMesh(const Edge_Of_PolygonMesh<ScalarType>& InputEdge)
{
    m_HalfEdgeIndex0 = InputEdge.m_HalfEdgeIndex0;
    m_HalfEdgeIndex1 = InputEdge.m_HalfEdgeIndex1;
}

template<typename ScalarType>
inline
Edge_Of_PolygonMesh<ScalarType>::~Edge_Of_PolygonMesh()
{
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::operator=(const Edge_Of_PolygonMesh<ScalarType>& InputEdge)
{
    m_HalfEdgeIndex0 = InputEdge.m_HalfEdgeIndex0;
    m_HalfEdgeIndex1 = InputEdge.m_HalfEdgeIndex1;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::Clear()
{
    m_Mesh = nullptr;
    m_HalfEdgeIndex0 = -1;
    m_HalfEdgeIndex1 = -1;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::SetParentMesh(PolygonMesh<ScalarType>* InputMesh)
{
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
PolygonMesh<ScalarType>* Edge_Of_PolygonMesh<ScalarType>::GetParentMesh()
{
    return m_Mesh;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::SetIndex(int_max EdgeIndex)
{
    m_Index = EdgeIndex;
}

template<typename ScalarType>
inline
int_max Edge_Of_PolygonMesh<ScalarType>::GetIndex() const
{
    return m_Index;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::SetHalfEdgeIndexList(int_max HalfEdgeIndex0, int_max HalfEdgeIndex1)
{
    m_HalfEdgeIndex0 = HalfEdgeIndex0;
    m_HalfEdgeIndex1 = HalfEdgeIndex1;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<ScalarType>::GetHalfEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetHalfEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    OutputIndexList[0] = m_HalfEdgeIndex0;
    OutputIndexList[1] = m_HalfEdgeIndex1;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetHalfEdgeIndexList(int_max& HalfEdgeIndex0, int_max& HalfEdgeIndex1) const
{
    HalfEdgeIndex0 = m_HalfEdgeIndex0;
    HalfEdgeIndex1 = m_HalfEdgeIndex1;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<ScalarType>::GetVertexIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetVertexIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    OutputIndexList[0] = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetStartVertexIndex();
    OutputIndexList[1] = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetEndVertexIndex();
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetVertexIndexList(int_max& VertexIndex0, int_max& VertexIndex1) const
{
    VertexIndex0 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetStartVertexIndex();
    VertexIndex1 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetEndVertexIndex();
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    auto VertexIndex0 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetVertexIndex();
    auto VertexIndex1 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex1].GetVertexIndex();
    const DenseVector<int_max>& AdjacentEdgeIndexList0 = m_Mesh->m_MeshData->VertexList[VertexIndex0].GetAdjacentEdgeIndexList();
    const DenseVector<int_max>& AdjacentEdgeIndexList1 = m_Mesh->m_MeshData->VertexList[VertexIndex1].GetAdjacentEdgeIndexList();

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
DenseVector<int_max> Edge_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentPolygonIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const
{
    auto VertexIndex0 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex0].GetVertexIndex();
    auto VertexIndex1 = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndex1].GetVertexIndex();
    const DenseVector<int_max>& AdjacentPolygonIndexList0 = m_Mesh->m_MeshData->VertexList[VertexIndex0].GetAdjacentPolygonIndexList();
    const DenseVector<int_max>& AdjacentPolygonIndexList1 = m_Mesh->m_MeshData->VertexList[VertexIndex1].GetAdjacentPolygonIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.ReserveCapacity(AdjacentPolygonIndexList0.GetLength() + AdjacentPolygonIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentPolygonIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentPolygonIndexList0[k];
        if (tempIndex != m_Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }

    for (int_max k = 0; k < AdjacentPolygonIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentPolygonIndexList1[k];
        if (tempIndex != m_Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }
}

//=========================================================== HalfEdge_Of_PolygonMesh ===========================================================//

template<typename ScalarType>
inline
HalfEdge_Of_PolygonMesh<ScalarType>::HalfEdge_Of_PolygonMesh(PolygonMesh<ScalarType>* InputMesh)
{
    this->Clear();
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
HalfEdge_Of_PolygonMesh<ScalarType>::HalfEdge_Of_PolygonMesh(const HalfEdge_Of_PolygonMesh<ScalarType>& InputHalfEdge)
{
    (*this) = InputHalfEdge;
}

template<typename ScalarType>
inline
HalfEdge_Of_PolygonMesh<ScalarType>::~HalfEdge_Of_PolygonMesh()
{
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::operator=(const HalfEdge_Of_PolygonMesh<ScalarType>& InputHalfEdge)
{
    m_Mesh = InputHalfEdge.m_Mesh;
    m_Index = InputHalfEdge.m_Index;
    m_VertexIndex_start = InputHalfEdge.m_VertexIndex_start;
    m_VertexIndex_end = InputHalfEdge.m_VertexIndex_end;
    m_EdgeIndex = InputHalfEdge.m_EdgeIndex;
    m_PolygonIndex = InputHalfEdge.m_PolygonIndex;
    m_OppositeHalfEdgeIndex = InputHalfEdge.m_OppositeHalfEdgeIndex;
    m_NextHalfEdgeIndex = InputHalfEdge.m_NextHalfEdgeIndex;
    m_PreviousHalfEdgeIndex = InputHalfEdge.m_PreviousHalfEdgeIndex;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::Clear()
{
    m_Mesh = nullptr;
    m_Index = -1;
    m_VertexIndex_start = -1;
    m_VertexIndex_end = -1;
    m_EdgeIndex = -1;
    m_PolygonIndex = -1;
    m_OppositeHalfEdgeIndex = -1;
    m_NextHalfEdgeIndex = -1;
    m_PreviousHalfEdgeIndex = -1;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetParentMesh(PolygonMesh<ScalarType>* InputMesh)
{
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
PolygonMesh<ScalarType>* HalfEdge_Of_PolygonMesh<ScalarType>::GetParentMesh()
{
    return m_Mesh;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetIndex(int_max HalfEdgeIndex)
{
    m_Index = HalfEdgeIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetIndex() const
{
    return m_Index;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetStartVertexIndex(int_max VertexIndex)
{
    m_VertexIndex_start = VertexIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetStartVertexIndex() const
{
    return m_VertexIndex_start;
}


template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetEndVertexIndex(int_max VertexIndex)
{
    m_VertexIndex_end = VertexIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetEndVertexIndex() const
{
    return m_VertexIndex_end;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_EdgeIndex = EdgeIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetEdgeIndex() const
{
    return m_EdgeIndex;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetPolygonIndex(int_max PolygonIndex)
{
    m_PolygonIndex = PolygonIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetPolygonIndex() const
{
    return m_PolygonIndex;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetOppositeHalfEdgeIndex(int_max HalfEdgeIndex)
{
    m_OppositeHalfEdgeIndex = HalfEdgeIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetOppositeHalfEdgeIndex() const
{
    return m_OppositeHalfEdgeIndex;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetNextHalfEdgeIndex(int_max HalfEdgeIndex)
{
    m_NextHalfEdgeIndex = HalfEdgeIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetNextHalfEdgeIndex() const
{
    return m_NextHalfEdgeIndex;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::SetPreviousHalfEdgeIndex(int_max HalfEdgeIndex)
{
    m_PreviousHalfEdgeIndex = HalfEdgeIndex;
}

template<typename ScalarType>
inline
int_max HalfEdge_Of_PolygonMesh<ScalarType>::GetPreviousHalfEdgeIndex() const
{
    return m_PreviousHalfEdgeIndex;
}

template<typename ScalarType>
inline
DenseVector<int_max> HalfEdge_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentPolygonIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(2);
    OutputIndexList[0] = m_PolygonIndex;
    OutputIndexList[1] = m_Mesh->m_MeshData->HalfEdgeList[m_OppositeHalfEdgeIndex].GetPolygonIndex();
    return OutputIndexList;
}

template<typename ScalarType>
inline
void HalfEdge_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList(int_max& PolygonIndex0, int_max& PolygonIndex1) const
{
    PolygonIndex0 = m_PolygonIndex;
    PolygonIndex1 = m_Mesh->m_MeshData->HalfEdgeList[m_OppositeHalfEdgeIndex].GetPolygonIndex();
}

//=========================================================== Polygon_Of_PolygonMesh ===========================================================//

template<typename ScalarType>
inline
Polygon_Of_PolygonMesh<ScalarType>::Polygon_Of_PolygonMesh(PolygonMesh<ScalarType>* InputMesh)
{
    this->Clear();
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
Polygon_Of_PolygonMesh<ScalarType>::Polygon_Of_PolygonMesh(const Polygon_Of_PolygonMesh<ScalarType>& InputPolygon)
{
    (*this) = InputPolygon;
}

template<typename ScalarType>
inline
Polygon_Of_PolygonMesh<ScalarType>::Polygon_Of_PolygonMesh(Polygon_Of_PolygonMesh<ScalarType>&& InputPolygon)
{
    this->operator=(std::forward<Polygon_Of_PolygonMesh<ScalarType>&&>(InputPolygon));
}

template<typename ScalarType>
inline
Polygon_Of_PolygonMesh<ScalarType>::~Polygon_Of_PolygonMesh()
{
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::operator=(const Polygon_Of_PolygonMesh<ScalarType>& InputHalfEdge)
{
    m_Mesh = InputHalfEdge.m_Mesh;
    m_Index = InputHalfEdge.m_Index;
    m_HalfEdgeIndexList = InputHalfEdge.m_HalfEdgeIndexList;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::operator=(Polygon_Of_PolygonMesh<ScalarType>&& InputHalfEdge)
{
    m_Mesh = InputHalfEdge.m_Mesh;
    m_Index = InputHalfEdge.m_Index;
    m_HalfEdgeIndexList = std::move(InputHalfEdge.m_HalfEdgeIndexList);
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::Clear()
{
    m_Mesh = nullptr;
    m_Index = -1;
    m_HalfEdgeIndexList.Clear();
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::SetParentMesh(PolygonMesh<ScalarType>* InputMesh)
{
    m_Mesh = InputMesh;
}

template<typename ScalarType>
inline
PolygonMesh<ScalarType>* Polygon_Of_PolygonMesh<ScalarType>::GetParentMesh()
{
    return m_Mesh;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::SetIndex(int_max PolygonIndex)
{
    m_Index = PolygonIndex;
}

template<typename ScalarType>
inline
int_max Polygon_Of_PolygonMesh<ScalarType>::GetIndex() const
{
    return m_Index;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::SetHalfEdgeIndexList(DenseVector<int_max> IndexList)
{
    m_HalfEdgeIndexList = std::move(IndexList);
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Polygon_Of_PolygonMesh<ScalarType>::GetHalfEdgeIndexList() const
{
    return m_HalfEdgeIndexList;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::GetHalfEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList = m_HalfEdgeIndexList;
}

template<typename ScalarType>
inline int_max Polygon_Of_PolygonMesh<ScalarType>::GetHalfEdgeNumber() const
{
    return m_HalfEdgeIndexList.GetElementNumber();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Polygon_Of_PolygonMesh<ScalarType>::GetVertexIndexList() const
{
    // DenseVector<int_max> OutputIndexList;
    // this->GetVertexIndexList(OutputIndexList);
    // return OutputIndexList;

    return m_Mesh->m_MeshData->VertexIndexTable[m_Index];
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::GetVertexIndexList(DenseVector<int_max>& OutputIndexList) const
{
    // int_max VertexNumber = m_HalfEdgeIndexList.GetLength();
    // OutputIndexList.FastResize(VertexNumber);
    // for (int_max k = 0; k < VertexNumber; ++k)
    // {
    //     int_max OppositeHalfEdgeIndex = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndexList[k]].GetOppositeHalfEdgeIndex();
    //     OutputIndexList[k] = m_Mesh->m_MeshData->HalfEdgeList[OppositeHalfEdgeIndex]].GetVertexIndex;
    // 
    // }
    
    OutputIndexList = m_Mesh->m_MeshData->VertexIndexTable[m_Index];

    // OutputIndexList[k] is the index of the starting vertex of the halfedge (m_HalfEdgeIndexList[k])

}

template<typename ScalarType>
inline int_max Polygon_Of_PolygonMesh<ScalarType>::GetVertexNumber() const
{
    return m_Mesh->m_MeshData->VertexIndexTable[m_Index].GetElementNumber();
}

template<typename ScalarType>
inline
DenseVector<int_max> Polygon_Of_PolygonMesh<ScalarType>::GetEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max VertexNumber = m_HalfEdgeIndexList.GetLength();
    OutputIndexList.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        OutputIndexList[k] = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndexList[k]].GetEdgeIndex;
    }
}

template<typename ScalarType>
inline int_max Polygon_Of_PolygonMesh<ScalarType>::GetEdgeNumber() const
{
    return m_HalfEdgeIndexList.GetElementNumber();
}


template<typename ScalarType>
inline
DenseVector<int_max> Polygon_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentPolygonIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename ScalarType>
inline
void Polygon_Of_PolygonMesh<ScalarType>::GetAdjacentPolygonIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max HalfEdgeNumber = m_HalfEdgeIndexList.GetLength();
    OutputIndexList.FastResize(HalfEdgeNumber);
    for (int_max k = 0; k < HalfEdgeNumber; ++k)
    {
        int_max OppositeHalfEdgeIndex = m_Mesh->m_MeshData->HalfEdgeList[m_HalfEdgeIndexList[k]].GetOppositeHalfEdgeIndex();
        OutputIndexList[k] = m_Mesh->m_MeshData->HalfEdgeList[OppositeHalfEdgeIndex].GetPolygonIndex();
    }
}

}// namespace mdk

#endif