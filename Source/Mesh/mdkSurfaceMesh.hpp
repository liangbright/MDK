#ifndef __mdkSurfaceMesh_hpp
#define __mdkSurfaceMesh_hpp

namespace mdk
{

template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh()
{
    m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(const Pure_Empty_SurfaceMesh_Symbol&)
{
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::SurfaceMesh(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType>::~SurfaceMesh()
{
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::operator=(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::operator=(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
    }

    this->Take(std::forward<SurfaceMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->IsTriangleMesh = false;

    Map_PointID_to_PointIndex.clear();
    Map_PointID_to_PointIndex.shrink_to_fit();

    Map_EdgeID_to_EdgeIndex.clear();
    Map_EdgeID_to_EdgeIndex.shrink_to_fit();

    Map_DirectedEdgeID_to_DirectedEdgeIndex.clear();
    Map_DirectedEdgeID_to_DirectedEdgeIndex.shrink_to_fit();

    Map_CellID_to_CellIndex.clear();
    Map_CellID_to_CellIndex.shrink_to_fit();

    m_MeshData->PointPositionTable.Clear();
    m_MeshData->PointList.Clear();
    m_MeshData->PointValidityFlagList.Clear();

    m_MeshData->EdgeList.Clear();
    m_MeshData->DirectedEdgePairList.Clear();
    m_MeshData->EdgeValidityFlagList.Clear();

    m_MeshData->CellList.Clear();
    m_MeshData->CellValidityFlagList.Clear();
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Copy(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<SurfaceMeshData<MeshAttributeType>>();
    }

    if (!InputMesh.m_MeshData)
    {
        return;
    }

    m_MeshData->Map_PointID_to_PointIndex = InputMesh.m_MeshData->Map_PointID_to_PointIndex;
    m_MeshData->Map_EdgeID_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex;
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex;
    m_MeshData->Map_CellID_to_CellIndex = InputMesh.m_MeshData->Map_CellID_to_CellIndex;

    m_MeshData->PointPositionTable = InputMesh.m_MeshData->PointPositionTable;
    m_MeshData->PointList = InputMesh.m_MeshData->PointList;
    m_MeshData->PointValidityFlagList = InputMesh.m_MeshData->PointValidityFlagList;

    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    m_MeshData->DirectedEdgePairList = InputMesh.m_MeshData->DirectedEdgePairList;
    m_MeshData->EdgeValidityFlagList = InputMesh.m_MeshData->EdgeValidityFlagList;

    m_MeshData->CellList = InputMesh.m_MeshData->CellList;
    m_MeshData->CellValidityFlagList = InputMesh.m_MeshData->CellValidityFlagList;
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Copy(const SurfaceMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Share(SurfaceMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Share(SurfaceMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::ForceShare(const SurfaceMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::ForceShare(const SurfaceMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>&& InputMesh)
{
    Take(std::forward<SurfaceMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>& InputMesh)
{

    m_MeshData->Map_PointID_to_PointIndex = std::move(InputMesh.m_MeshData->Map_PointID_to_PointIndex);
    m_MeshData->Map_EdgeID_to_EdgeIndex = std::move(InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex);
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = std::move(InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex);
    m_MeshData->Map_CellID_to_CellIndex = std::move(InputMesh.m_MeshData->Map_CellID_to_CellIndex);

    m_MeshData->PointPositionTable = std::move(InputMesh.m_MeshData->PointPositionTable);
    m_MeshData->PointValidityFlagList = std::move(InputMesh.m_MeshData->PointValidityFlagList);
    m_MeshData->PointList = std::move(InputMesh.m_MeshData->PointList);
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            m_MeshData->PointList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->EdgeValidityFlagList = std::move(InputMesh.m_MeshData->EdgeValidityFlagList);
    m_MeshData->EdgeList = std::move(InputMesh.m_MeshData->EdgeList);
    m_MeshData->DirectedEdgePairList = std::move(InputMesh.m_MeshData->DirectedEdgePairList);
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            m_MeshData->EdgeList[k].SetParentMesh(*this);
            if (m_MeshData->DirectedEdgePairList[k][0].IsValid() == true)
            {
                m_MeshData->DirectedEdgePairList[k][0].SetParentMesh(*this);
            }
            if (m_MeshData->DirectedEdgePairList[k][1].IsValid() == true)
            {
                m_MeshData->DirectedEdgePairList[k][1].SetParentMesh(*this);
            }
        }
    }    

    m_MeshData->CellValidityFlagList = std::move(InputMesh.m_MeshData->CellValidityFlagList);
    m_MeshData->CellList = std::move(InputMesh.m_MeshData->CellList);
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            m_MeshData->CellList[k].SetParentMesh(*this);
        }
    }    
}


template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::Take(SurfaceMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ SurfaceMesh::Take(...)")
        return false;
    }

    this->Take(*InputMesh);

    return true;
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline 
bool SurfaceMesh<MeshAttributeType>::IsEmpty() const
{
    return m_MeshData->PointPositionTable.IsEmpty();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_MeshData->PointValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return m_MeshData->EdgeValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetDirectedEdgeNumber() const
{
    int_max Counter = 0;
    for (int_max k = 0; k < m_MeshData->EdgeValidityFlagList[k]; ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            if (m_MeshData->DirectedEdgePairList[k][0].IsValid() == true)
            {
                Counter += 1;
            }

            if (m_MeshData->DirectedEdgePairList[k][1].IsValid() == true)
            {
                Counter += 1;
            }
        }
    }
    return Counter;
}


template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetCellNumber() const
{
    return m_MeshData->CellValidityFlagList.Sum();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == true)
    {
        return;
    }
    else if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == false)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetPointPosition(...)")
        return;
    }
    else if (PointHandleList.IsEmpty() == false && PointPositionMatrix.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ SurfaceMesh::SetPointPosition(...)")
        return;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].Index;
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::SetPointPosition(...)")
            return;
        }

        m_MeshData->PointPositionTable.SetCol(PointIndex, PointPositionMatrix.GetPointerOfCol(k));
    }
}

template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType> 
SurfaceMesh<MeshAttributeType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    DenseMatrix<MeshAttributeType::ScalarType> PointPositionMatrix;

    this->GetPointPosition(PointPositionMatrix, PointHandleList);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
GetPointPosition(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix, const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    if (PointHandleList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    PointPositionMatrix.FastResize(3, PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].Index;
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex))
    }
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Point_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Point_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Edge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Edge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle)
{
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& 
SurfaceMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex;
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename MeshAttributeType>
inline
Cell_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Cell_Of_SurfaceMesh<MeshAttributeType>& SurfaceMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}

//-------------- check handle -------------------------------------------------------//

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    auto PointIndex = PointHandle.GetIndex();
    if (PointIndex < 0 || PointIndex >= m_MeshData->PointValidityFlagList.GetLength())
    {
        return false;
    }
    else
    {
        return (m_MeshData->PointValidityFlagList[PointIndex] == 1);
    }
    
    // slow
    //if (PointIndex < 0 || PointIndex >= m_MeshData->PointList.GetLength())
    //{
    //    return false;
    //}
    //else
    //{
    //    return m_MeshData->PointList[PointIndex].IsValid();
    //}    
}

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    auto EdgeIndex = EdgeHandle.GetIndex();
    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength())
    {
        return false;
    }
    else
    {
        return (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1);
    }

    // slow
    //if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeList.GetLength())
    //{
    //    return false;
    //}
    //else
    //{
    //    return m_MeshData->EdgeList[EdgeIndex].IsValid();
    //}
}

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::IsValidHandle(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();

    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength() || RelativeIndex < 0 || RelativeIndex > 1)
    {
        return false;
    }
    else
    {
        if (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 0)
        {
            return false;
        }
        else
        {
            return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex].IsValid();
        }
    }
}

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    auto CellIndex = CellHandle.GetIndex();
    if (CellIndex < 0 || CellIndex >= m_MeshData->CellValidityFlagList.GetLength())
    {
        return false;
    }
    else
    {
        return (m_MeshData->CellValidityFlagList[CellIndex] == 1);
    }

    // slow
    //if (CellIndex < 0 || CellIndex >= m_MeshData->CellList.GetLength())
    //{
    //    return false;
    //}
    //else
    //{
    //    return m_MeshData->CellList[CellIndex].IsValid();
    //}
}

//--------- get HandleList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(this->GetPointNumber());
    int_max Counter = 0;
    for (int_max k = 0; k < m_MeshData->PointValidityFlagList[k]; ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            OutputHandleList[Counter].SetIndex(k);
            Counter += 1;
        }
    }
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_Edge_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(this->GetEdgeNumber());
    int_max Counter = 0;
    for (int_max k = 0; k < m_MeshData->EdgeValidityFlagList[k]; ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            OutputHandleList[Counter].SetIndex(k);
            Counter += 1;
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.ReserveCapacity(this->GetEdgeNumber()*2);
    for (int_max k = 0; k < m_MeshData->EdgeValidityFlagList[k]; ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            Handle_Of_DirectedEdge_Of_SurfaceMesh tempHandle;

            if (m_MeshData->DirectedEdgePairList[k][0].IsValid() == true)
            {
                tempHandle.SetIndex(k, 0);
                OutputHandleList.Append(tempHandle);
            }

            if (m_MeshData->DirectedEdgePairList[k][1].IsValid() == true)
            {
                tempHandle.SetIndex(k, 1);
                OutputHandleList.Append(tempHandle);
            }
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::GetCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_SurfaceMesh> OutputHandleList;
    this->GetCellHandleList(OutputHandleList);
    return OutputHandleList;
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::GetCellHandleList(DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(this->GetCellNumber());
    int_max Counter = 0;
    for (int_max k = 0; k < m_MeshData->CellValidityFlagList[k]; ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            OutputHandleList[Counter].SetIndex(k);
            Counter += 1;
        }
    }
}

//---------- get Handle by ID -----------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetPointHandle(int_max PointID) const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;

    auto it = m_MeshData->Map_PointID_to_PointIndex.find(PointID);
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
        PointHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Error("Invalid PointID @ SurfaceMesh::GetPointHandle(...)")
        PointHandle.SetIndex(-1);
    }
        
    return PointHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh  SurfaceMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        EdgeHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Error("Invalid EdgeIndex @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetIndex(-1);
    }

    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max DirectedEdgeID) const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;

    auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
    if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
    {
        DirectedEdgeHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Error("Invalid EdgeIndex @ SurfaceMesh::GetEdgeHandle(...)")
        DirectedEdgeHandle.SetIndex(-1, -1);
    }

    return DirectedEdgeHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    auto it = m_MeshData->Map_CellID_to_CellIndex.find(CellID);
    if (it != m_MeshData->Map_CellID_to_CellIndex.end())
    {
        CellHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Error("Invalid CellIndex @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.SetIndex(-1);
    }

    return CellHandle;
}

//----------- get PointHandle by position ----------------------------------------------//
template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetPointHandle(typename MeshAttributeType::ScalarType Position[3]) const
{
    return this->GetPointHandle(Position[0], Position[1], Position[2]);
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetPointHandle(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z) const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;

    if (this->IsEmpty() == true)
    {
        PointHandle.SetIndex(-1);
        return PointHandle;
    }

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColNumber(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            ScalarType Pos[3];
            m_MeshData->PointPositionTable.GetCol(k, Pos);

            ScalarType Distance_sq = std::abs(Pos[0] - x) + std::abs(Pos[1] - y) + std::abs(Pos[2] - z);
            if (Distance_sq <= 3 * eps_value)
            {
                PointHandle.SetIndex(k);
                return PointHandle;
            }
        }
    }

    PointHandle.SetIndex(-1);
    return PointHandle;
}


//----------- get EdgeHandle and DirectedEdgeHandle by PointHandleList ----------------------------------------------//
template<typename MeshAttributeType>
inline 
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetEdgeHandle(Handle_Of_Point_Of_SurfaceMesh VertexPointHandle0, Handle_Of_Point_Of_SurfaceMesh VertexPointHandle1) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (this->IsValidHandle(VertexPointHandle0) == false || this->IsValidHandle(VertexPointHandle1) == false)
    {
        MDK_Error("VertexPointHandle0 or VertexPointHandle1 is invalid:  @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }

    auto VertexPointIndex0 = VertexPointHandle0.GetIndex();
    auto VertexPointIndex1 = VertexPointHandle1.GetIndex();

    if (m_MeshData->PointList[VertexPointIndex0].IsVertex() == true && m_MeshData->PointList[VertexPointIndex1].IsVertex() == true)
    {
        const auto& tempAdjacentEdgeIndexList = m_MeshData->PointList[VertexPointIndex0].AdjacentEdgeIndexList();
        for (int_max k = 0; k < tempAdjacentEdgeIndexList.GetLength(); ++k)
        {
            int_max tempPointIdex_a, tempPointIdex_b;
            m_MeshData->EdgeList[tempAdjacentEdgeIndexList[k]].GetVertexPointIndexList(tempPointIdex_a, tempPointIdex_b);
            if (VertexPointIndex1 == tempPointIdex_a || VertexPointIndex1 == tempPointIdex_b)
            {
                EdgeHandle.SetIndex(tempAdjacentEdgeIndexList[k]);
                return EdgeHandle;
            }
        }
    }

    MDK_Warning("No Edge between VertexPointHandle0 and VertexPointHandle1 @ SurfaceMesh::GetEdgeHandle(...)")
    EdgeHandle.SetIndex(-1);
    return EdgeHandle;
}

template<typename MeshAttributeType>
inline 
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetEdgeHandle(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    if (PointHandleList.GetLength() < 2)
    {
        MDK_Error("PointHandleList.GetLength() < 2 @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("PointHandle is invalid @ SurfaceMesh::GetEdgeHandle(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }

    auto EdgeHandle_ab = this->GetEdgeHandle(PointHandleList[0], PointHandleList[PointHandleList.GetLength() - 1]);
    if (EdgeHandle_ab.GetIndex() < 0)
    {
        EdgeHandle = EdgeHandle_ab;
        return EdgeHandle;
    }

    if (PointHandleList.GetLength() == 2)
    {
        EdgeHandle = EdgeHandle_ab;
        return EdgeHandle;
    }

    // PointHandleList.GetLength() > 2 --------------------------------------------------------

    auto VertexPointIndex0 = PointHandleList[0].GetIndex();
    auto VertexPointIndex1 = PointHandleList[PointHandleList.GetLength() - 1].GetIndex();

    if (m_MeshData->PointList[VertexPointIndex0].IsVertex() == false)
    {
        if (m_MeshData->PointList[VertexPointIndex0].IsOnEdge() == true)
        {
            MDK_Warning("The point : PointHandleList[0] is not a veterx, but is already on an edge @ SurfaceMesh::GetEdgeHandle(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }

    if (m_MeshData->PointList[VertexPointIndex1].IsVertex() == false)
    {
        if (m_MeshData->PointList[VertexPointIndex1].IsOnEdge() == true)
        {
            MDK_Warning("The point : PointHandleList[end] is not a veterx, but is already on an edge @ SurfaceMesh::GetEdgeHandle(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }

    DenseVector<int_max> tempEdgeIndexList;
    tempEdgeIndexList.Resize(PointHandleList.GetLength());
    tempEdgeIndexList.Fill(-1);

    for (int_max k = 1; k < PointHandleList.GetLength() - 1; ++k)
    {
        if (m_MeshData->PointList[PointHandleList[k].GetIndex()].IsVertex() == true)
        {
            MDK_Warning("A middle point is a vertex, PointIndex = " << PointHandleList[k].GetIndex() << " @ SurfaceMesh::GetEdgeHandle(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }

        const auto& tempAdjacentEdgeIndexList = m_MeshData->PointList[PointHandleList[k].GetIndex()].AdjacentEdgeIndexList();
        if (tempAdjacentEdgeIndexList.GetLength() > 1)
        {
            MDK_Warning("A middle point is shared by two edge " << PointHandleList[k].GetIndex() << " @ SurfaceMesh::GetEdgeHandle(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }

        if (tempAdjacentEdgeIndexList.GetLength() == 1)
        {
            tempEdgeIndexList[k] = tempAdjacentEdgeIndexList[0];
        }
    }

    int_max tempValue = 0;
    for (int_max k = 2; k < PointHandleList.GetLength() - 1; ++k)
    {
        tempValue += std::abs(tempEdgeIndexList[k] - tempEdgeIndexList[k - 1]);
    }

    if (tempValue != 0)
    {
        MDK_Warning("At least a middle point is already on an edge @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }
    else
    {
        if (tempEdgeIndexList[1] >= 0)
        {
            if (m_MeshData->PointList[VertexPointIndex0].IsVertex() == true && m_MeshData->PointList[VertexPointIndex1].IsVertex() == true)
            {
                EdgeHandle.SetIndex(tempEdgeIndexList[1]);
                return EdgeHandle;
            }
            else
            {
                MDK_Warning("At least a middle point is already on an edge @ SurfaceMesh::GetEdgeHandle(...)")
                EdgeHandle.SetIndex(-1);
                return EdgeHandle;
            }
        }
        else
        {
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetDirectedEdgeHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle_start, Handle_Of_Point_Of_SurfaceMesh PointHandle_end) const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;

    auto EdgeHandle = this->GetEdgeHandle(PointHandle_start, PointHandle_end);
    if (EdgeHandle.GetIndex() < 0)
    {
        DirectedEdgeHandle.SetIndex(-1, -1);
        return DirectedEdgeHandle;
    }

    if (m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][0].IsValid() == true)
    {
        auto StartPointIndex = m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][0].GetStartPointIndex();
        auto EndPointIndex = m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][0].GetEndPointIndex();
        if (StartPointIndex == PointHandle_start.GetIndex() && EndPointIndex == PointHandle_end.GetIndex())
        {
            DirectedEdgeHandle.SetIndex(EdgeHandle.GetIndex(), 0);
            return DirectedEdgeHandle;
        }
    }

    if (m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][1].IsValid() == true)
    {
        auto StartPointIndex = m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][1].GetStartPointIndex();
        auto EndPointIndex = m_MeshData->DirectedEdgePairList[EdgeHandle.GetIndex()][1].GetEndPointIndex();
        if (StartPointIndex == PointHandle_start.GetIndex() && EndPointIndex == PointHandle_end.GetIndex())
        {
            DirectedEdgeHandle.SetIndex(EdgeHandle.GetIndex(), 1);
            return DirectedEdgeHandle;
        }
    }

    DirectedEdgeHandle.SetIndex(-1, -1);
    return DirectedEdgeHandle;
}

//----------- get CellHandle by EdgeHandleList ------------------------------------------//
template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList) const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            MDK_Error("EdgeHandle is invalid @ SurfaceMesh::GetCellHandle(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    DenseVector<int_max> CellIndexList;
    CellIndexList.Resize(EdgeHandleList.GetLength());
    CellIndexList.Fill(-1);

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto AdjacentCellIndexList_a = m_MeshData->EdgeList[EdgeHandleList[k].GetIndex()].GetAdjacentCellIndexList();
        auto AdjacentCellIndexList_b = m_MeshData->EdgeList[EdgeHandleList[k + 1].GetIndex()].GetAdjacentCellIndexList();
        for (int_max n = 0; n < AdjacentCellIndexList_a.GetLength(); ++n)
        {
            for (int_max m = 0; m < AdjacentCellIndexList_b.GetLength(); ++m)
            {
                if (AdjacentCellIndexList_a[n] == AdjacentCellIndexList_b[m])
                {
                    CellIndexList[k] = AdjacentCellIndexList_a[n];
                    break;
                }
            }
        }
    }

    auto tempList = CellIndexList.FindUnique();
    if (tempList.GetLength() != 1)
    {
        CellHandle.SetIndex(-1);
        return CellHandle;
    }
    else
    {
        CellHandle.SetIndex(CellIndexList[tempList[0]]);
        return CellHandle;
    }
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
    Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfDirectedEdge()
{
    Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfDirectedEdge() const
{
    Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfCell()
{
    Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> SurfaceMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType> it(*this);
    return it;
}

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//
template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddPoint(const typename MeshAttributeType::ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh
SurfaceMesh<MeshAttributeType>::
AddPoint(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColNumber() - 1;

    Point_Of_SurfaceMesh<MeshAttributeType> Point;
    Point.Create();
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
    Point.SetID(PointIndex);

    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);
    m_MeshData->Map_PointID_to_PointIndex[PointIndex] = PointIndex;

    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(PointIndex);

    return PointHandle;
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::AddPoint(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;

    if (PointSet.GetRowNumber() != 3)
    {
        MDK_Error("PointSet.GetRowNumber() != 3 @ SurfaceMesh::AddPoint(...)")
        return PointHandleList;
    }

    PointHandleList.Resize(PointSet.GetColNumber());
    for (int_max k = 0; k < PointSet.GetColNumber(); ++k)
    {
        PointHandleList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointHandleList;
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddEdge(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (PointHandleList.GetLength() < 2)
    {
        MDK_Error("length of PointHandleList < 2 @ SurfaceMesh::AddEdge(...)")
        EdgeHandle.SetIndex(-1);
        return EdgeHandle;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("PointHandle is invalid @ SurfaceMesh::AddEdge(...)")
            EdgeHandle.SetIndex(-1);
            return EdgeHandle;
        }
    }

    auto EdgeHandle_temp = this->GetEdgeHandle(PointHandleList);
    if (this->IsValidHandle(EdgeHandle_temp) == true)
    {
        MDK_Warning("The edge has been added already @ SurfaceMesh::AddEdge(...)")
        EdgeHandle = EdgeHandle_temp;
        return EdgeHandle;
    }

    //----------------------------------------------------------------
    auto VertexPointIndex0 = PointHandleList[0].GetIndex();
    auto VertexPointIndex1 = PointHandleList[PointHandleList.GetLength()-1].GetIndex();

    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_SurfaceMesh<MeshAttributeType> Edge;
    Edge.Create();
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.SetID(EdgeIndex);
    Edge.SetVertexPointIndexList(VertexPointIndex0, VertexPointIndex1);
    Edge.PointIndexList().Resize(PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        Edge.PointIndexList()[k] = PointHandleList[k].GetIndex();
    }

    m_MeshData->EdgeList.Append(std::move(Edge));

    // add empty DirectedEdgePair to hold place
    DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2> DirectedEdgePair;
    m_MeshData->DirectedEdgePairList.Append(std::move(DirectedEdgePair));
    m_MeshData->DirectedEdgePairList[EdgeIndex][0].Clear();
    m_MeshData->DirectedEdgePairList[EdgeIndex][1].Clear();

    m_MeshData->EdgeValidityFlagList.Append(1);

    m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeIndex] = EdgeIndex;

    // update AdjacentPoint information in m_MeshData->PointList
    auto tempIndex0 = m_MeshData->PointList[VertexPointIndex0].AdjacentPointIndexList().Find([&](int_max Index) {return Index == VertexPointIndex1;});
    if (tempIndex0.IsEmpty() == true)
    {
        m_MeshData->PointList[VertexPointIndex0].AdjacentPointIndexList().Append(VertexPointIndex1);
    }   

    auto tempIndex1 = m_MeshData->PointList[VertexPointIndex1].AdjacentPointIndexList().Find([&](int_max Index) {return Index == VertexPointIndex0; });
    if (tempIndex1.IsEmpty() == true)
    {
        m_MeshData->PointList[VertexPointIndex1].AdjacentPointIndexList().Append(VertexPointIndex0);
    }

    // update AdjacentEdge information in m_MeshData->PointList
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        m_MeshData->PointList[PointHandleList[k].GetIndex()].AdjacentEdgeIndexList().Append(EdgeIndex);
    }
    
    //------------
    EdgeHandle.SetIndex(EdgeIndex);
    return EdgeHandle;
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCell(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    // the input EdgeIndexList must have been ordered

    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (EdgeHandleList.GetLength() < 2)
    {
        MDK_Error("length of EdgeHandleList < 2 @ SurfaceMesh::AddCell(...)")
        CellHandle.SetIndex(-1);
        return CellHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[EdgeHandleList[k].GetIndex()] == 0)
        {
            MDK_Error("Invalid Edge: " << EdgeHandleList[k].GetIndex() << " @ SurfaceMesh::AddCell(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    auto CellHandle_temp = this->GetCellHandle(EdgeHandleList);
    if (this->IsValidHandle(CellHandle_temp) == true)
    {
        MDK_Warning("The cell has been added already @ SurfaceMesh::AddCell(...)")
        CellHandle = CellHandle_temp;
        return CellHandle;
    }

    //------------------------------------------------------------------------------------------------//
       
    DenseVector<int_max> VertexPointIndexList;
    VertexPointIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto tempEdgeIndex_a = EdgeHandleList[k].GetIndex();
        auto tempEdgeIndex_b = EdgeHandleList[k+1].GetIndex();

        int_max tempIndexList_a[2];
        m_MeshData->EdgeList[tempEdgeIndex_a].GetVertexPointIndexList(tempIndexList_a);

        int_max tempIndexList_b[2];
        m_MeshData->EdgeList[tempEdgeIndex_b].GetVertexPointIndexList(tempIndexList_b);

        if (tempIndexList_a[0] == tempIndexList_b[0] && tempIndexList_a[1] != tempIndexList_b[1])
        {           
            VertexPointIndexList[k] = tempIndexList_a[1];
            VertexPointIndexList[k + 1] = tempIndexList_a[0];
        }
        else if (tempIndexList_a[0] == tempIndexList_b[1] && tempIndexList_a[1] != tempIndexList_b[0])
        {
            VertexPointIndexList[k] = tempIndexList_a[1];
            VertexPointIndexList[k + 1] = tempIndexList_a[0];
        }
        else if (tempIndexList_a[1] == tempIndexList_b[0] && tempIndexList_a[0] != tempIndexList_b[1])
        {
            VertexPointIndexList[k] = tempIndexList_a[0];
            VertexPointIndexList[k + 1] = tempIndexList_a[1];
        }
        else if (tempIndexList_a[1] == tempIndexList_b[1] && tempIndexList_a[0] != tempIndexList_b[0])
        {
            VertexPointIndexList[k] = tempIndexList_a[0];
            VertexPointIndexList[k + 1] = tempIndexList_a[1];
        }
        else
        {
            MDK_Error("EdgeIndexList is not ordered or invalid @ SurfaceMesh::AddCell(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    // get DirectedEdgeIndexList -------------------------------------------------------
    auto CellIndex = m_MeshData->CellList.GetLength();

    DenseVector<DirectedEdgeIndex_Of_SurfaceMesh> DirectedEdgeIndexList;
    DirectedEdgeIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto& DirectedEdge0 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].GetIndex()][0];
        auto& DirectedEdge1 = m_MeshData->DirectedEdgePairList[EdgeHandleList[k].GetIndex()][1];

        if (DirectedEdge0.IsValid() == false)
        {
            DirectedEdgeIndexList[k].RelativeIndex = 0;
        }
        else if (DirectedEdge1.IsValid() == false)
        {
            DirectedEdgeIndexList[k].RelativeIndex = 1;
        }
        else
        {
            MDK_Error("A cell with a DirectedEdge has been added already @ SurfaceMesh::AddCell(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }

        DirectedEdgeIndexList[k].EdgeIndex = EdgeHandleList[k].GetIndex();
    }

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // attention: auto& will get reference, auto will copy
        auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];

        tempDirectedEdge.Create();
        tempDirectedEdge.SetParentMesh(*this);
        tempDirectedEdge.SetIndex(DirectedEdgeIndexList[k]);

        auto tempID = DirectedEdgeIndexList[k].EdgeIndex * 2 + DirectedEdgeIndexList[k].RelativeIndex;
        tempDirectedEdge.SetID(tempID);
        m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[tempID] = DirectedEdgeIndexList[k];

        tempDirectedEdge.SetCellIndex(CellIndex);
        tempDirectedEdge.SetStartPointIndex(VertexPointIndexList[k]);

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        { 
            tempDirectedEdge.SetEndPointIndex(VertexPointIndexList[k + 1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetEndPointIndex(VertexPointIndexList[0]);
        }

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[k + 1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndexList[0]);
        }

        if (k > 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[k-1]);
        }
        else // if (k == 0)
        {
            tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndexList[DirectedEdgeIndexList.GetLength()-1]);
        }
    }

    // create cell --------------------------------------------------------------

    Cell_Of_SurfaceMesh<MeshAttributeType> Cell;
    Cell.Create();
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.SetID(CellIndex);
    Cell.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);
    m_MeshData->Map_CellID_to_CellIndex[CellIndex] = CellIndex;

    // update information in m_MeshData->PointList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[DirectedEdgeIndexList[k].EdgeIndex][DirectedEdgeIndexList[k].RelativeIndex];
        auto tempPointIndex_start = tempDirectedEdge.GetStartPointIndex();
        auto tempPointIndex_end = tempDirectedEdge.GetEndPointIndex();
        m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
        m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
    }

    for (int_max k = 0; k < VertexPointIndexList.GetLength(); ++k)
    {
        m_MeshData->PointList[VertexPointIndexList[k]].AdjacentCellIndexList().Append(CellIndex);
    }

    //-----------------------------
    CellHandle.SetIndex(CellIndex);
    return CellHandle;
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    auto CellIndex = CellHandle.Index;

    if (CellIndex < 0 || CellIndex >= m_MeshData->CellList.GetLength())
    {
        MKD_Error("Invalid CellIndex @ SurfaceMesh::DeleteCell(...)")
        return false;
    }

    if (m_MeshData->CellValidityFlagList[CellIndex] == 0)
    {
        MKD_Warning("The cell has been deleted already, CellIndex = " << CellIndex  << "@ SurfaceMesh::DeleteCell(...)")
        return true;
    }

    //--------------------------------------------------------------------------------------
    // auto& will get reference
    const auto& DirectedEdgeIndexList = m_MeshData->CellList[CellIndex].DirectedEdgeIndexList();
    const auto& VertexPointIndexList = m_MeshData->CellList[CellIndex].GetVertexPointIndexList();

    // update information in m_MeshData->PointList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];
        auto tempPointIndex_start = tempDirectedEdge.GetStartPointIndex();
        auto tempPointIndex_end = tempDirectedEdge.GetEndPointIndex();

        auto tempIndex_a = m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index.EdgeIndex == tempEdgeIndex && Index.RelativeIndex == tempRelativeIndex);
        });
        m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Delete(tempIndex_a);

        auto tempIndex_b = m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index.EdgeIndex == tempEdgeIndex && Index.RelativeIndex == tempRelativeIndex);
        });
        m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Delete(tempIndex_b);
    }

    for (int_max k = 0; k < VertexPointIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_MeshData->PointList[VertexPointIndexList[k]].AdjacentCellIndexList().Find([&](int_max Index) {return Index == CellIndex});
        m_MeshData->PointList[VertexPointIndexList[k]].AdjacentCellIndexList().Delete(tempIndex);
    }

    // Delete DirectedEdge: only clear memory, not remove from DirectedEdgePairList
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); k++)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // delete DirectedEdgeID
        auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].GetID());
        m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        // clear
        m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].Clear();
    }

    // delete CellID
    auto it = m_MeshData->Map_CellID_to_CellIndex.find(m_MeshData->CellList[CellIndex].GetID());
    m_MeshData->Map_CellID_to_CellIndex.erase(it);

    // Delete Cell : only clear memory, not remove from CellList
    m_MeshData->CellList[CellIndex].Clear();
    m_MeshData->CellValidityFlagList[CellIndex] = 0;
    //---------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{
    auto EdgeIndex = EdgeHandle.GetIndex();

    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeList.GetLength())
    {
        MDK_Error("Invalid EdgeIndex @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }

    if (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 0)
    {
        MDK_Warning("The edge has been deleted already @ SurfaceMesh::DeleteEdge(...)")
        return true;
    }

    const auto& AdjacentCellIndexList = m_MeshData->EdgeList[EdgeIndex].GetAdjacentCellIndexList();
    if (AdjacentCellIndexList.IsEmpty() == false)
    {
        MDK_Error("AdjacentCellIndexList is not empty, so this edge can not be deleted @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }

    //---------------------------------------------------------------------------------------------
    const auto& PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();

    // update AdjacentEdge information in m_MeshData->PointList
    for (int_max k = 1; k < PointIndexList.GetLength() - ; ++k)
    {
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentEdgeIndexList().Find([&](int_max Index) { return (Index == EdgeIndex); });
        m_MeshData->PointList[PointIndexList[k]].AdjacentEdgeIndexList().Delete(tempIndex);
    }

    // update AdjacentPoint information in m_MeshData->PointList

    int_max VertexPointIndexList[2];
    m_MeshData->EdgeList[EdgeIndex].GetVertexPointIndexList(VertexPointIndexList);

    auto VertexPointIndex0 = VertexPointIndexList[0];
    auto VertexPointIndex1 = VertexPointIndexList[1];

    for (int_max k = 0; k < 2; ++k)
    {
        bool tempFlag_0_1_connected = false;

        const auto& tempIndexList = m_MeshData->PointList[VertexPointIndexList[k]].AdjacentEdgeIndexList();
        for (int_max n = 0; n < tempIndexList.GetLength(); ++n)
        {
            int_max tempVertexPointIndex0 = -1;
            int_max tempVertexPointIndex1 = -1;
            m_MeshData->EdgeList[VertexPointIndexList[k]].GetVertexPointIndexList(tempVertexPointIndex0, tempVertexPointIndex1);
            if (tempVertexPointIndex0 == VertexPointIndex0 && tempVertexPointIndex1 == VertexPointIndex1)
            {
                tempFlag_0_1_connected = true;
                break;
            }
            else if (tempVertexPointIndex0 == VertexPointIndex1 && tempVertexPointIndex1 == VertexPointIndex0)
            {
                tempFlag_0_1_connected = true;
                break;
            }
        }

        if (tempFlag_0_1_connected == false)
        {            
            auto tempIndex0 = m_MeshData->PointList[VertexPointIndex0].AdjacentPointIndexList().Find([&](int_max Index){return Index == VertexPointIndex1;});
            m_MeshData->PointList[VertexPointIndex0].AdjacentEdgeIndexList().Delete(tempIndex0);

            auto tempIndex1 = m_MeshData->PointList[VertexPointIndex1].AdjacentPointIndexList().Find([&](int_max Index){return Index == VertexPointIndex0;});
            m_MeshData->PointList[VertexPointIndex1].AdjacentEdgeIndexList().Delete(tempIndex1);

            break;
        }
    }

    // Delete EdgeID
    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(m_MeshData->EdgeList[EdgeIndex].GetID());
    m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);

    // Delete Edge: only clear memory, not remove from EdgeList
    m_MeshData->EdgeList[EdgeIndex].Clear();
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
    //--------------------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    auto PointIndex = PointHandle.GetIndex();

    if (PointIndex < 0 || PointIndex >= m_MeshData->PointList.GetLength())
    {
        MDK_Error("Invalid PointIndex @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    if (m_MeshData->PointValidityFlagList[PointIndex] == 0)
    {
        MDK_Warning("The point has been deleted alreay @ SurfaceMesh::DeletePoint(...)")
        return true;
    }

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    //--------------------------------------------------------------------
    auto it = m_MeshData->Map_PointID_to_PointIndex.find(m_MeshData->PointIDList[PointIndex].GetID());
    m_MeshData->Map_PointID_to_PointIndex.erase(it);

    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

    m_MeshData->PointList[PointIndex].Clear();
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
    //---------------------------------------------------------------------
    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    if (PointHandleList.IsEmpty() == true)
    {
        MDK_Warning("PointHandleList is empty @ SurfaceMesh::DeletePoint(...)")
        return true;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->DeletePoint(PointHandleList[k]) == false)
        {
            return false;
        }
    }
    return true;
}

//----------------- remove deleted item from object list ----------------------------------------------------------//
// attention: after this function is called, every index will be changed
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
template<typename MeshAttributeType>
void SurfaceMesh<MeshAttributeType>::CleanDataStructure()
{

}
//---------------------------------------------------------------------------

}// namespace mdk

#endif