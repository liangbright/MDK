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
int_max TriangleMesh<MeshAttributeType>::GetPointNumber() const
{
    return this->PolygonMesh::GetPointNumber();
}

template<typename MeshAttributeType>
inline 
int_max TriangleMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return this->PolygonMesh::GetEdgeNumber();
}

template<typename MeshAttributeType>
inline
int_max TriangleMesh<MeshAttributeType>::GetCellNumber() const
{
    return this->PolygonMesh::GetCellNumber();
}
//------------------------- Mesh 3D Position --------------------------------------

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList,
                  const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    return this->PolygonMesh::SetPointPostion(PointHandleList, PointPositionMatrix);
}

template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType>
TriangleMesh<MeshAttributeType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    return this->PolygonMesh::GetPointPosition(PointHandleList);
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
GetPointPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix,
                  const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    this->PolygonMesh::GetPointPosition(PointPositionMatrix, PointHandleList);
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return this->PolygonMesh::Point(PointHandle);
}

template<typename MeshAttributeType>
inline
const Point_Of_SurfaceMesh<MeshAttributeType>&
TriangleMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return this->PolygonMesh::Point(PointHandle);
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

//-------------- check handle -------------------------------------------------------//

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return this->PolygonMesh::IsValidHandle(PointHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return this->PolygonMesh::IsValidHandle(EdgeHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::IsValidHandle(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    return this->PolygonMesh::IsValidHandle(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return this->PolygonMesh::IsValidHandle(CellHandle);
}

//--------- get HandleList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::GetPointHandleList() const
{
    return this->PolygonMesh::GetPointHandleList();
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    this->PolygonMesh::GetPointHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    return this->PolygonMesh::GetEdgeHandleList()
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    this->PolygonMesh::GetEdgeHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    return this->PolygonMesh::GetDirectedEdgeHandleList();
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    this->PolygonMesh::GetDirectedEdgeHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::GetCellHandleList() const
{
    return this->PolygonMesh::GetCellHandleList();
}

template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::GetCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    this->PolygonMesh::GetCellHandleList(OutputHandleList);
}

// get handle by ID -----------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetPointHandle(int_max PointID) const
{
    return this->PolygonMesh::GetPointHandle(PointID);
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    return this->PolygonMesh::GetEdgeHandle(EdgeID);
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max DirectedEdgeID) const
{
    return this->PolygonMesh::GetDirectedEdgeHandle(DirectedEdgeID);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    return this->PolygonMesh::GetCellHandle(CellID);
}

//----------- get PointHandle by position ----------------------------------------------//
template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetPointHandle(typename MeshAttributeType::ScalarType Position[3]) const
{
    return this->PolygonMesh::GetPointHandle(Position);
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
GetPointHandle(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z) const
{
    return this->PolygonMesh::GetPointHandle(x, y, z);
}

//----------- get EdgeHandle and DirectedEdgeHandle by PointHandleList ----------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
GetEdgeHandle(Handle_Of_Point_Of_SurfaceMesh VertexPointHandle0, Handle_Of_Point_Of_SurfaceMesh VertexPointHandle1) const
{
    return this->PolygonMesh::GetEdgeHandle(VertexPointHandle0, VertexPointHandle1);
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
GetEdgeHandle(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    return this->PolygonMesh::GetEdgeHandle(PointHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
GetDirectedEdgeHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle_start, Handle_Of_Point_Of_SurfaceMesh PointHandle_end) const
{
    return this->PolygonMesh::GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
}

//----------- get CellHandle by EdgeHandleList ------------------------------------------//
template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::GetCellHandle(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList) const
{
    return this->PolygonMesh::GetCellHandle(EdgeHandleList);
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    return this->PolygonMesh::GetIteratorOfPoint();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
    return this->PolygonMesh::GetIteratorOfPoint();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    return this->PolygonMesh::GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    return this->PolygonMesh::GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  TriangleMesh<MeshAttributeType>::GetIteratorOfCell()
{
    return this->PolygonMesh::GetIteratorOfCell();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  TriangleMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    return this->PolygonMesh::GetIteratorOfCell();
}

// Add Mesh Item -------------------------------------------------------------------------//
// add an item and return index (-1 if input is invalid)

template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::AddPoint(typename MeshAttributeType::ScalarType Position[3])
{
    return this->PolygonMesh::AddPoint(Position);
}

template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
AddPoint(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    return this->PolygonMesh::AddPoint(x, y, z);
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> TriangleMesh<MeshAttributeType>::AddPoint(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    return this->PolygonMesh::AddPoint(PointSet);
}

template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh TriangleMesh<MeshAttributeType>::
AddEdge(Handle_Of_Point_Of_SurfaceMesh PointHandle0, Handle_Of_Point_Of_SurfaceMesh PointHandle1)
{
    return this->PolygonMesh::AddEdge(PointHandle0, PointHandle1);
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
bool TriangleMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return this->PolygonMesh::DeletePoint(PointHandle);
}

template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    return this->PolygonMesh::DeletePoint(PointHandleList);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::CleanDataStructure()
{
    this->PolygonMesh::CleanDataStructure();
}

//------------ Construct from input data ------------------------------------//
template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputPointPositionTable,
                                                const DataArray<DenseVector<int_max>>& InputCellTable)
{
    return this->PolygonMesh::Construct(InputPointPositionTable, InputCellTable);
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
std::pair<DenseMatrix<typename MeshAttributeType::ScalarType>, DataArray<DenseVector<int_max>>>
TriangleMesh<MeshAttributeType>::GetPointPositionMatrixAndCellTable() const
{
    return this->PolygonMesh::GetPointPositionMatrixAndCellTable();
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::
GetPointPositionMatrixAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable,
                                   DataArray<DenseVector<int_max>>& CellTable) const
{
    this->PolygonMesh::GetPointPositionMatrixAndCellTable(PointPositionTable, CellTable);
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtPoint()
{
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell()
{
}

}// namespace mdk

#endif