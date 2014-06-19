#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh()
{
    m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
    m_MeshData->IsTriangleMesh = false;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(PolygonMesh<MeshAttributeType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::~PolygonMesh()
{
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::operator=(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::operator=(PolygonMesh<MeshAttributeType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
    }

    this->Take(std::forward<PolygonMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->IsTriangleMesh = false;
    m_MeshData->PolygonSurfaceMesh.Clear();
}


template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::Copy(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
    }

    m_MeshData->IsTriangleMesh = InputMesh.m_MeshData->IsTriangleMesh;
    m_MeshData->PolygonSurfaceMesh = InputMesh.m_MeshData->PolygonSurfaceMesh;
}


template<typename MeshAttributeType>
inline 
bool PolygonMesh<MeshAttributeType>::Copy(const PolygonMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::Share(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool PolygonMesh<MeshAttributeType>::Share(PolygonMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::ForceShare(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline 
bool PolygonMesh<MeshAttributeType>::ForceShare(const PolygonMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Take(PolygonMesh<MeshAttributeType>&& InputMesh)
{
    Take(std::forward<PolygonMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
bool PolygonMesh<MeshAttributeType>::Take(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData->IsTriangleMesh = InputMesh.m_MeshData->IsTriangleMesh;
    m_MeshData->PolygonSurfaceMesh = std::move(InputMesh.m_MeshData->PolygonSurfaceMesh);
    return true;
}


template<typename MeshAttributeType>
inline 
bool PolygonMesh<MeshAttributeType>::Take(PolygonMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline bool PolygonMesh<MeshAttributeType>::IsEmpty() const
{
    return m_MeshData->PolygonSurfaceMesh.IsEmpty();
}

template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetVertexNumber() const
{
    return m_MeshData->PolygonSurfaceMesh.GetVertexNumber();
}

template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetCellNumber() const
{
    return m_MeshData->PolygonSurfaceMesh.GetCellNumber();
}

template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return return m_MeshData->PolygonSurfaceMesh.GetEdgeNumber();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::
SetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList, 
                  const DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix)
{
    m_MeshData->PolygonSurfaceMesh.SetVertexPosition(VertexHandleList, VertexPositionMatrix);
}

template<typename MeshAttributeType>
inline 
DenseMatrix<typename MeshAttributeType::ScalarType> 
PolygonMesh<MeshAttributeType>::GetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
   return m_MeshData->PolygonSurfaceMesh.GetVertexPosition(VertexHandleList);
}

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::
GetVertexPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix, 
                 const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetVertexPosition(VertexPositionMatrix, VertexHandleList);
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline 
Vertex_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    return m_MeshData->PolygonSurfaceMesh.Vertex(VertexHandle);
}

template<typename MeshAttributeType>
inline 
const Vertex_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.Vertex(VertexHandle);
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return m_MeshData->PolygonSurfaceMesh.Edge(EdgeHandle);
}

template<typename MeshAttributeType>
inline
const Edge_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.Edge(EdgeHandle);
}

template<typename MeshAttributeType>
inline 
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle)
{
    return m_MeshData->PolygonSurfaceMesh.DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return m_MeshData->PolygonSurfaceMesh.Cell(CellHandle);
}

template<typename MeshAttributeType>
inline 
const Cell_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.Cell(CellHandle);
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline 
Iterator_Of_Vertex_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfVertex() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfVertex();
}

template<typename MeshAttributeType>
inline 
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline 
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  PolygonMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfCell();
}

template<typename MeshAttributeType>
inline 
Handle_Of_DirectedEdge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetDirectedEdgeHandle(EdgeHandleType EdgeHandle, int_max RelativeIndex) const
{
    return m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandle(EdgeHandle, RelativeIndex);
}

// get handle by ID -----------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Vertex_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetVertexHandle(int_max VertexID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetVertexHandle(VertexID);
}

template<typename MeshAttributeType>
inline 
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetEdgeHandle(EdgeID);
}

template<typename MeshAttributeType>
inline 
Handle_Of_DirectedEdge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const
{
    return m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandle(EdgeID, RelativeIndex);
}

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetCellHandle(CellID);
}

// Add Mesh Item -------------------------------------------------------------------------//
// add an item and return index (-1 if input is invalid)

template<typename MeshAttributeType>
Handle_Of_Vertex_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::AddVertex(typename MeshAttributeType::ScalarType Position[3])
{
    auto PointHandle = m_MeshData->PolygonSurfaceMesh.AddPoint(Position[3]);
    m_MeshData->PolygonSurfaceMesh.AddVertex(PointHandle);
}

template<typename MeshAttributeType>
Handle_Of_Vertex_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
AddVertex(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    ScalarType Position[3] = { x, y, z };
    return this->AddVertex(Position);
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::AddVertex(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> VertexHandleList;
    VertexHandleList.Resize(PointSet.GetColNumber());

    auto PointHandleList = m_MeshData->PolygonSurfaceMesh.AddPoint(PointSet);
    for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
    {
        VertexHandleList[k] = m_MeshData->PolygonSurfaceMesh.AddVertex(PointHandleList[k]);
    }    

    return VertexHandleList;
}

template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
AddEdge(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle0, Handle_Of_Vertex_Of_SurfaceMesh VertexHandle1)
{
    auto PointHandle0 = m_MeshData->PolygonSurfaceMesh.Vertex(VertexHandle0).GetPointHandle();
    auto PointHandle1 = m_MeshData->PolygonSurfaceMesh.Vertex(VertexHandle1).GetPointHandle();

    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;
    PointHandleList.Resize(2);
    PointHandleList[0] = PointHandle0;
    PointHandleList[1] = PointHandle1;

    return m_MeshData->PolygonSurfaceMesh.AddEdge(PointHandleList);
}

template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    return m_MeshData->PolygonSurfaceMesh.AddCell(EdgeHandleList);
}

//Delete Mesh Item ----------------------------------------------------------------------//

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return m_MeshData->PolygonSurfaceMesh.DeleteCell(CellHandle);
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return m_MeshData->PolygonSurfaceMesh.DeleteEdge(EdgeHandle);
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::DeleteVertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    if (m_MeshData->PolygonSurfaceMesh.DeleteVertex(VertexHandle) == false)
    {
        return false;
    }

    auto PointHandle = m_MeshData->PolygonSurfaceMesh.Vertex(VertexHandle).GetPointHandle();
    if (m_MeshData->PolygonSurfaceMesh.DeletePoint(PointHandle) == false)
    {
        return false;
    }

    return true;
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::DeleteVertex(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList)
{
    for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
    {
        if (this->DeleteVertex(VertexHandleList[k]) == false)
        {
            return false;
        }
    }
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::CleanDataStructure()
{
    m_MeshData->PolygonSurfaceMesh.CleanDataStructure();
}

//------------ Construct from input data ------------------------------------//
template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputVertexPositionTable, 
                                               const DataArray<DenseVector<int_max>>& InputCellTable)
{
    if (InputVertexPositionTable.IsEmpty() == true || InputCellTable.IsEmpty() == true)
    {
        MDK_Error("InputVertexPositionTable or InputVertexIndexTable is empty @ PolygonMesh::Construct(...)")
        return false;
    }

    if (InputVertexPositionTable.GetRowNumber() != 3 || 3 * InputCellTable.GetElementNumber() < InputVertexPositionTable.GetColNumber())
    {
        MDK_Error("InputVertexPositionTable or InputVertexIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }
    //--------------------------------------------------------------------------------------------------

    auto VertexHandleList = this->AddVertex(InputVertexPositionTable);

    for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
    {
        if (VertexHandleList[k].IsValid() == false)
        {
            return false;
        }
    }

    DenseVector<EdgeHandleType> EdgeHandleList;

    for (int_max k = 0; k < InputCellTable.GetLength(); ++k)
    {
        const DenseVector<int_max>& Polygon_k = InputCellTable[k];
        
        EdgeHandleList.FastResize(Polygon_k.GetElementNumber());

        for (int_max n = 0; n < Polygon_k.GetElementNumber(); ++n)
        {
            auto tempIndex_a = Polygon_k[n];

            int_max tempIndex_b = -1;
            if (n < Polygon_k.GetElementNumber() - 1)
            {
                tempIndex_b = Polygon_k[n+1];
            }
            else
            {
                tempIndex_b = Polygon_k[0];
            }

            EdgeHandleList[n] = this->AddEdge(VertexHandleList[tempIndex_a], VertexHandleList[tempIndex_b]);
        }

        for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
        {
            if (EdgeHandleList[k].IsValid() == false)
            {
                return false;
            }
        }

        this->AddCell(EdgeHandleList);
    }

    this->CheckIfTriangleMesh();

    return true;
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh)
{
    m_MeshData->PolygonSurfaceMesh = std::move(InputSurfaceMesh);

    this->CheckIfTriangleMesh();

    return true;
}

template<typename MeshAttributeType>
inline
bool PolygonMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
    if (this->IsEmpty() == true)
    {
        m_MeshData->IsTriangleMesh = false;
        return false;
    }

    auto it = this->GetIteratorOfCell();
    it.SetToBegin();
    while (it.IsInRange())
    {
        auto CellHandle = it.GetCellHandle();
        auto VertexNumber = this->Cell(CellHandle).GetVertexNumber();
        if (VertexNumber != 3)
        {
            m_MeshData->IsTriangleMesh = false;
            return false;
        }
        ++it;
    }

    m_MeshData->IsTriangleMesh = true;

    return true;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetVertexPositionTableAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionTable, 
                                   DataArray<DenseVector<int_max>>& CellTable) const
{
    auto VertexNumber = this->GetVertexNumber();
    auto CellNumber = this->GetCellNumber();

    VertexPositionTable.FastResize(3, VertexNumber);    
    CellTable.FastResize(CellNumber);

    std::unordered_map<int_max, int_max> Map_VertexIndex_to_OutputIndex;

    int_max VertexCounter = 0;

    auto it_v = this->GetIteratorOfVertex();
    it_v.SetToBegin();
    while (it_v.IsInRange())
    {
        auto VertexHandle = it_v.GetVertexHandle();
        this->Vertex(VertexHandle).GetPosition(VertexPositionTable.GetPointerOfCol(VertexCounter));
        Map_VertexIndex_to_OutputIndex[VertexHandle.GetIndex()] = VertexCounter;
        VertexCounter += 1;
        ++it_v;
    }

    int_max CellCounter = 0;

    auto it = this->GetIteratorOfCell();
    it.SetToBegin();
    while (it.IsInRange())
    {
        auto CellHandle = it.GetCellHandle();
        auto VertexHandleList = this->Cell(CellHandle).GetVertexHandleList();
        CellTable[CellCounter].FastResize(VertexHandleList.GetLength());
        for (int_max k = 0; k < VertexHandleList.GetLength(); ++k)
        {
            auto tempVertexIndex = VertexHandleList[k].GetIndex();   
            auto it_map = Map_VertexIndex_to_OutputIndex.find(tempVertexIndex);
            if (it_map != Map_VertexIndex_to_OutputIndex.end())
            {
                CellTable[CellCounter][k] = it_map->second;
            }
            else
            {
                MDK_Error("tempVertexIndex is invalid @ PolygonMesh::GetVertexPositionTableAndCellTable(...)")
                return;
            }
        }
        CellCounter += 1;
        ++it;
    }
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtVertex()
{
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtCell()
{
}


}// namespace mdk

#endif