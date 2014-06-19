#ifndef __mdkTriangleMesh_hpp
#define __mdkTriangleMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh()
: PolygonMesh()
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::~TriangleMesh()
{
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
    }

    this->Take(std::forward<TriangleMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Clear()
{
    this->PolygonMesh::Clear();
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Copy(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->PolygonMesh::Copy(InputMesh);
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Copy(const TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Share(TriangleMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool TriangleMesh<MeshAttributeType>::Share(TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::ForceShare(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::ForceShare(const TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    this->Take(std::forward<TriangleMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>& InputMesh)
{
    return this->PolygonMesh::Take(InputMesh);
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::IsEmpty() const
{
    return this->PolygonMesh::IsEmpty();
}


template<typename MeshAttributeType>
inline 
int_max TriangleMesh<MeshAttributeType>::GetVertexNumber() const
{
    return this->PolygonMesh::GetVertexNumber();
}


template<typename MeshAttributeType>
inline
int_max TriangleMesh<MeshAttributeType>::GetCellNumber() const
{
    return this->PolygonMesh::GetCellNumber();
}


template<typename MeshAttributeType>
inline 
int_max TriangleMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return this->PolygonMesh::GetEdgeNumber();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
SetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList,
                  const DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix)
{
    return this->PolygonMesh::SetVertexPostion(VertexHandleList, VertexPositionMatrix);
}

template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType>
TriangleMesh<MeshAttributeType>::GetVertexPosition(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
    return this->PolygonMesh::GetVertexPosition(VertexHandleList);
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
GetVertexPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionMatrix,
                  const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList) const
{
    this->PolygonMesh::GetVertexPosition(VertexPositionMatrix, VertexHandleList);
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Vertex_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    return this->PolygonMesh::Vertex(VertexHandle);
}

template<typename MeshAttributeType>
inline
const Vertex_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Vertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle) const
{
    return this->PolygonMesh::Vertex(VertexHandle);
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return this->PolygonMesh::Edge(EdgeHandle);
}

template<typename MeshAttributeType>
inline
const Edge_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return this->PolygonMesh::Edge(EdgeHandle);
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle)
{
    return this->PolygonMesh::DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    return this->PolygonMesh::DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return this->PolygonMesh::Cell(CellHandle);
}

template<typename MeshAttributeType>
inline
const Cell_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return this->PolygonMesh::Cell(CellHandle);
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Vertex_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfVertex() const
{
    return this->PolygonMesh::GetIteratorOfVertex();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    return this->PolygonMesh::GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  TriangleMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    return this->PolygonMesh::GetIteratorOfCell();
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetDirectedEdgeHandle(EdgeHandleType EdgeHandle, int_max RelativeIndex) const
{
    return this->PolygonMesh::GetDirectedEdgeHandle(EdgeHandle, RelativeIndex);
}

// get handle by ID -----------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Vertex_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetVertexHandle(int_max VertexID) const
{
    return this->PolygonMesh::GetVertexHandle(VertexID);
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    return this->PolygonMesh::GetEdgeHandle(EdgeID);
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max EdgeID, int_max RelativeIndex) const
{
    return this->PolygonMesh::GetDirectedEdgeHandle(EdgeID, RelativeIndex);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    return this->PolygonMesh::GetCellHandle(CellID);
}

// Add Mesh Item -------------------------------------------------------------------------//
// add an item and return index (-1 if input is invalid)

template<typename MeshAttributeType>
Handle_Of_Vertex_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::AddVertex(typename MeshAttributeType::ScalarType Position[3])
{
    return this->PolygonMesh::AddVertex(Position);
}

template<typename MeshAttributeType>
Handle_Of_Vertex_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
AddVertex(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    return this->PolygonMesh::AddVertex(x, y, z);
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Vertex_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::AddVertex(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    return this->PolygonMesh::AddVertex(PointSet);
}

template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
AddEdge(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle0, Handle_Of_Vertex_Of_SurfaceMesh VertexHandle1)
{
    return this->PolygonMesh::AddEdge(VertexHandle0, VertexHandle1);
}

template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    return this->PolygonMesh::AddCell(EdgeHandleList);
}

//Delete Mesh Item ----------------------------------------------------------------------//

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return this->PolygonMesh::DeleteCell(CellHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return this->PolygonMesh::DeleteEdge(EdgeHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::DeleteVertex(Handle_Of_Vertex_Of_SurfaceMesh VertexHandle)
{
    return this->PolygonMesh::DeleteVertex(VertexHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::DeleteVertex(const DenseVector<Handle_Of_Vertex_Of_SurfaceMesh>& VertexHandleList)
{
    return this->PolygonMesh::DeleteVertex(VertexHandleList);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::CleanDataStructure()
{
    this->PolygonMesh::CleanDataStructure();
}

//------------ Construct from input data ------------------------------------//
template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputVertexPositionTable,
                                                const DataArray<DenseVector<int_max>>& InputCellTable)
{
    return this->PolygonMesh::Construct(InputVertexPositionTable, InputCellTable);
}


template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh)
{
    return this->PolygonMesh::Construct(std::move(InputSurfaceMesh));
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh)
{
    return this->Take(InputPolygonMesh);
}

template<typename MeshAttributeType>
inline
bool TriangleMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
    return this->PolygonMesh::CheckIfTriangleMesh();
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
GetVertexPositionTableAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& VertexPositionTable,
                                   DataArray<DenseVector<int_max>>& CellTable) const
{
    this->PolygonMesh::GetVertexPositionTableAndCellTable(VertexPositionTable, CellTable);
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtVertex()
{
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell()
{
}

}// namespace mdk

#endif