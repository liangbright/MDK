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
int_max PolygonMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_MeshData->PolygonSurfaceMesh.GetPointNumber();
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
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, 
                  const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    m_MeshData->PolygonSurfaceMesh.SetPointPosition(PointHandleList, PointPositionMatrix);
}

template<typename MeshAttributeType>
inline 
DenseMatrix<typename MeshAttributeType::ScalarType> 
PolygonMesh<MeshAttributeType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
   return m_MeshData->PolygonSurfaceMesh.GetPointPosition(PointHandleList);
}

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::
GetPointPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix, 
                 const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetPointPosition(PointPositionMatrix, PointHandleList);
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline 
Point_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return m_MeshData->PolygonSurfaceMesh.Point(PointHandle);
}

template<typename MeshAttributeType>
inline 
const Point_Of_SurfaceMesh<MeshAttributeType>&
PolygonMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.Point(PointHandle);
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

//-------------- check handle -------------------------------------------------------//

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.IsValidHandle(PointHandle);
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.IsValidHandle(EdgeHandle);
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidHandle(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.IsValidHandle(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return m_MeshData->PolygonSurfaceMesh.IsValidHandle(CellHandle);
}

//--------- get HandleList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::GetPointHandleList() const
{
    return m_MeshData->PolygonSurfaceMesh.GetPointHandleList();
}

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetPointHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    return m_MeshData->PolygonSurfaceMesh.GetEdgeHandleList()
}

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetEdgeHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    return m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandleList();
}

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandleList(OutputHandleList);
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::GetCellHandleList() const
{
    return m_MeshData->PolygonSurfaceMesh.GetCellHandleList();
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::GetCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    m_MeshData->PolygonSurfaceMesh.GetCellHandleList(OutputHandleList);
}

// get handle by ID -----------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetPointHandle(int_max PointID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetPointHandle(PointID);
}

template<typename MeshAttributeType>
inline 
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetEdgeHandle(EdgeID);
}

template<typename MeshAttributeType>
inline 
Handle_Of_DirectedEdge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max DirectedEdgeID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandle(DirectedEdgeID);
}

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    return m_MeshData->PolygonSurfaceMesh.GetCellHandle(CellID);
}

//----------- get PointHandle by position ----------------------------------------------//
template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetPointHandle(typename MeshAttributeType::ScalarType Position[3]) const
{
    return m_MeshData->PolygonSurfaceMesh.GetPointHandle(Position);
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
GetPointHandle(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z) const
{
    return m_MeshData->PolygonSurfaceMesh.GetPointHandle(x, y, z);
}

//----------- get EdgeHandle and DirectedEdgeHandle by PointHandleList ----------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
GetEdgeHandle(Handle_Of_Point_Of_SurfaceMesh VertexPointHandle0, Handle_Of_Point_Of_SurfaceMesh VertexPointHandle1) const
{
    return m_MeshData->PolygonSurfaceMesh.GetEdgeHandle(VertexPointHandle0, VertexPointHandle1);
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
GetEdgeHandle(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    return m_MeshData->PolygonSurfaceMesh.GetEdgeHandle(PointHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
GetDirectedEdgeHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle_start, Handle_Of_Point_Of_SurfaceMesh PointHandle_end) const
{
    return m_MeshData->PolygonSurfaceMesh.GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
}

//----------- get CellHandle by EdgeHandleList ------------------------------------------//
template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::GetCellHandle(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList) const
{
    return m_MeshData->PolygonSurfaceMesh.GetCellHandle(EdgeHandleList);
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfPoint();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfPoint();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfEdge();
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  PolygonMesh<MeshAttributeType>::GetIteratorOfCell()
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfCell();
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>  PolygonMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    return m_MeshData->PolygonSurfaceMesh.GetIteratorOfCell();
}

// Add Mesh Item -------------------------------------------------------------------------//
// add an item and return index (-1 if input is invalid)

template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::AddPoint(typename MeshAttributeType::ScalarType Position[3])
{
    return m_MeshData->PolygonSurfaceMesh.AddPoint(Position[3]);
}

template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
AddPoint(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    ScalarType Position[3] = { x, y, z };
    return this->AddPoint(Position);
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> PolygonMesh<MeshAttributeType>::AddPoint(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    return m_MeshData->PolygonSurfaceMesh.AddPoint(PointSet);
}

template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh PolygonMesh<MeshAttributeType>::
AddEdge(Handle_Of_Point_Of_SurfaceMesh PointHandle0, Handle_Of_Point_Of_SurfaceMesh PointHandle1)
{
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
bool PolygonMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    if (m_MeshData->PolygonSurfaceMesh.DeletePoint(PointHandle) == false)
    {
        return false;
    }

    auto PointHandle = m_MeshData->PolygonSurfaceMesh.Point(PointHandle).GetPointHandle();
    if (m_MeshData->PolygonSurfaceMesh.DeletePoint(PointHandle) == false)
    {
        return false;
    }

    return true;
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->DeletePoint(PointHandleList[k]) == false)
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
bool PolygonMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputPointPositionTable, 
                                               const DataArray<DenseVector<int_max>>& InputCellTable)
{
    if (InputPointPositionTable.IsEmpty() == true || InputCellTable.IsEmpty() == true)
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is empty @ PolygonMesh::Construct(...)")
        return false;
    }

    if (InputPointPositionTable.GetRowNumber() != 3 || 3 * InputCellTable.GetElementNumber() < InputPointPositionTable.GetColNumber())
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }
    //--------------------------------------------------------------------------------------------------

    auto PointHandleList = this->AddPoint(InputPointPositionTable);

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            return false;
        }
    }

    DenseVector<EdgeHandleType> EdgeHandleList;

    for (int_max k = 0; k < InputCellTable.GetLength(); ++k)
    {
        const DenseVector<int_max>& Cell_k = InputCellTable[k];
        
        EdgeHandleList.FastResize(Cell_k.GetElementNumber());

        for (int_max n = 0; n < Cell_k.GetElementNumber(); ++n)
        {
            auto tempIndex_a = Cell_k[n];

            int_max tempIndex_b = -1;
            if (n < Cell_k.GetElementNumber() - 1)
            {
                tempIndex_b = Cell_k[n + 1];
            }
            else
            {
                tempIndex_b = Cell_k[0];
            }

            EdgeHandleList[n] = this->AddEdge(PointHandleList[tempIndex_a], PointHandleList[tempIndex_b]);
        }

        for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
        {
            if (this->IsValidHandle(EdgeHandleList[k]) == false)
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
        auto PointNumber = this->Cell(CellHandle).GetPointNumber();
        if (PointNumber != 3)
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
std::pair<DenseMatrix<typename MeshAttributeType::ScalarType>, DataArray<DenseVector<int_max>>>
PolygonMesh<MeshAttributeType>::GetPointPositionMatrixAndCellTable() const
{
    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> Output;
    this->GetPointPositionMatrixAndCellTable(Output.first, Output.second);
    return Output;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPositionMatrixAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable,
                                   DataArray<DenseVector<int_max>>& CellTable) const
{
    auto PointNumber = this->GetPointNumber();
    auto CellNumber = this->GetCellNumber();

    PointPositionTable.FastResize(3, PointNumber);    
    CellTable.FastResize(CellNumber);

    std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex;

    int_max PointCounter = 0;

    auto it_v = this->GetIteratorOfPoint();
    it_v.SetToBegin();
    while (it_v.IsInRange())
    {
        auto PointHandle = it_v.GetPointHandle();
        this->Point(PointHandle).GetPosition(PointPositionTable.GetPointerOfCol(PointCounter));
        Map_PointIndex_to_OutputIndex[PointHandle.GetIndex()] = PointCounter;
        PointCounter += 1;
        ++it_v;
    }

    int_max CellCounter = 0;

    auto it = this->GetIteratorOfCell();
    it.SetToBegin();
    while (it.IsInRange())
    {
        auto CellHandle = it.GetCellHandle();
        auto PointHandleList = this->Cell(CellHandle).GetPointHandleList();
        CellTable[CellCounter].FastResize(PointHandleList.GetLength());
        for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
        {
            auto tempPointIndex = PointHandleList[k].GetIndex();   
            auto it_map = Map_PointIndex_to_OutputIndex.find(tempPointIndex);
            if (it_map != Map_PointIndex_to_OutputIndex.end())
            {
                CellTable[CellCounter][k] = it_map->second;
            }
            else
            {
                MDK_Error("tempPointIndex is invalid @ PolygonMesh::GetPointPositionTableAndCellTable(...)")
                return;
            }
        }
        CellCounter += 1;
        ++it;
    }
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtPoint(PointHandleType PointHandle)
{
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtCell(CellHandleType CellHandle)
{
}


}// namespace mdk

#endif