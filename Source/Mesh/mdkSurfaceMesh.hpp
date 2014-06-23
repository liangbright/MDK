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

    m_MeshData->PointPositionTable.Clear();
    m_MeshData->PointList.Clear();
    m_MeshData->PointValidityFlagList.Clear();

    m_MeshData->EdgeList.Clear();
    m_MeshData->DirectedEdgePairList.Clear();
    m_MeshData->EdgeValidityFlagList.Clear();

    m_MeshData->CellList.Clear();
    m_MeshData->CellValidityFlagList.Clear();

    m_MeshData->Map_PointID_to_PointIndex.clear();
    m_MeshData->Map_EdgeID_to_EdgeIndex.clear();
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.clear();
    m_MeshData->Map_CellID_to_CellIndex.clear();

    m_MeshData->Attribute.Clear();
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

    m_MeshData->PointPositionTable = InputMesh.m_MeshData->PointPositionTable;
    m_MeshData->PointList = InputMesh.m_MeshData->PointList;
    m_MeshData->PointValidityFlagList = InputMesh.m_MeshData->PointValidityFlagList;

    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    m_MeshData->DirectedEdgePairList = InputMesh.m_MeshData->DirectedEdgePairList;
    m_MeshData->EdgeValidityFlagList = InputMesh.m_MeshData->EdgeValidityFlagList;

    m_MeshData->CellList = InputMesh.m_MeshData->CellList;
    m_MeshData->CellValidityFlagList = InputMesh.m_MeshData->CellValidityFlagList;

    m_MeshData->Map_PointID_to_PointIndex = InputMesh.m_MeshData->Map_PointID_to_PointIndex;
    m_MeshData->Map_EdgeID_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex;
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex;
    m_MeshData->Map_CellID_to_CellIndex = InputMesh.m_MeshData->Map_CellID_to_CellIndex;

    m_MeshData->Attribute = InputMesh.m_MeshData->Attribute;
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

    m_MeshData->Map_PointID_to_PointIndex = std::move(InputMesh.m_MeshData->Map_PointID_to_PointIndex);
    m_MeshData->Map_EdgeID_to_EdgeIndex = std::move(InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex);
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = std::move(InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex);
    m_MeshData->Map_CellID_to_CellIndex = std::move(InputMesh.m_MeshData->Map_CellID_to_CellIndex);

    m_MeshData->Attribute = std::move(InputMesh.m_MeshData->Attribute);
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

//------ Get/Set GlobalAttribute -----------------------------------//

template<typename MeshAttributeType>
inline
typename MeshAttributeType::GlobalAttribute& SurfaceMesh<MeshAttributeType>::Attribute()
{
    return m_MeshData->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::GlobalAttribute& SurfaceMesh<MeshAttributeType>::Attribute() const
{
    return m_MeshData->Attribute;
}

//---- Get/Set 3D Position by PointHandle or PointID --------------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(Handle_Of_Point_Of_SurfaceMesh PointHandle, 
                typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), { x, y, z });
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::SetPointPosition(Handle_Of_Point_Of_SurfaceMesh PointHandle, const typename MeshAttributeType::ScalarType Position[3])
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), Position);
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(int_max PointID, typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    auto PointHandle = this->GetPointHandle(PointID);
    this->SetPointPosition(PointHandle, x, y, z);
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::SetPointPosition(int_max PointID, const typename MeshAttributeType::ScalarType Position[3])
{
    auto PointHandle = this->GetPointHandle(PointID);
    this->SetPointPosition(PointHandle, Position);
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> 
SurfaceMesh<MeshAttributeType>::GetPointPosition(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    DenseVector<ScalarType, 3> Position;
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position.GetPointer());
    return Position;
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::
GetPointPosition(Handle_Of_Point_Of_SurfaceMesh PointHandle, 
                 typename MeshAttributeType::ScalarType& x, typename MeshAttributeType::ScalarType& y, typename MeshAttributeType::ScalarType& z)
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetPointPosition(Handle_Of_Point_Of_SurfaceMesh PointHandle, typename MeshAttributeType::ScalarType Position[3])
{
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3>
SurfaceMesh<MeshAttributeType>::GetPointPosition(int_max PointID)
{
    auto PointHandle = this->GetPointHandle(PointID);
    return this->GetPointPosition(PointHandle);
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetPointPosition(int_max PointID, ScalarType& x, ScalarType& y, ScalarType& z)
{
    auto PointHandle = this->GetPointHandle(PointID);
    this->GetPointPosition(PointHandle, x, y, z);
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetPointPosition(int_max PointID, ScalarType Position[3])
{
    auto PointHandle = this->GetPointHandle(PointID);
    this->GetPointPosition(PointHandle, Position);
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList, 
                 const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
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
        auto PointIndex = PointHandleList[k].GetIndex();
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
void SurfaceMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    if (PointIDList.IsEmpty() == true)
    {
        MDK_Error("Empty PointIDList @ SurfaceMesh::SetPointPosition(...)")
        return;
    }

    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
        PointHandleList[k] = this->GetPointHandle(PointIDList[k]);

        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("Invalid PointIDList @ SurfaceMesh::SetPointPosition(...)")
            return;
        }
    }

    this->SetPointPosition(PointHandleList, PointPositionMatrix);
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
GetPointPosition(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList,
                 DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix) const
{
    if (PointHandleList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    PointPositionMatrix.FastResize(3, PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].GetIndex();
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ SurfaceMesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex))
    }
}


template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType>
SurfaceMesh<MeshAttributeType>::GetPointPosition(const DenseVector<int_max>& PointIDList) const
{
    DenseMatrix<ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointPositionMatrix, PointIDList);
}


template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::
GetPointPosition(const DenseVector<int_max>& PointIDList, DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix) const
{
    if (PointIDList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
        PointHandleList[k] = this->GetPointHandle(PointIDList[k]);

        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("Invalid PointIDList @ SurfaceMesh::GetPointPosition(...)")
            return;
        }
    }

    this->GetPointPosition(PointPositionMatrix, PointHandleList);
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
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->DirectedEdgePairList[EdgeIndex][RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_SurfaceMesh<MeshAttributeType>& 
SurfaceMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
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
    OutputHandleList.ReserveCapacity(m_MeshData->PointList.GetLength());
    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            PointHandle.SetIndex(k);
            OutputHandleList.Append(PointHandle);
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
    OutputHandleList.ReserveCapacity(m_MeshData->EdgeList.GetLength());
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            EdgeHandle.SetIndex(k);
            OutputHandleList.Append(EdgeHandle);
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
void SurfaceMesh<MeshAttributeType>::
GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_SurfaceMesh>& OutputHandleList) const
{
    OutputHandleList.ReserveCapacity(2*m_MeshData->DirectedEdgePairList.GetLength());
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;
    for (int_max k = 0; k < m_MeshData->DirectedEdgePairList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            if (m_MeshData->DirectedEdgePairList[k][0].IsValid() == true)
            {
                DirectedEdgeHandle.SetIndex(k, 0);
                OutputHandleList.Append(DirectedEdgeHandle);
            }

            if (m_MeshData->DirectedEdgePairList[k][1].IsValid() == true)
            {
                DirectedEdgeHandle.SetIndex(k, 1);
                OutputHandleList.Append(DirectedEdgeHandle);
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
    OutputHandleList.ReserveCapacity(m_MeshData->CellList.GetLength());
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            CellHandle.SetIndex(k);
            OutputHandleList.Append(CellHandle);
        }
    }
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
        PointHandle.SetToInvalid();
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

    MDK_Warning("Invalid Position @ SurfaceMesh::GetPointHandle(...)")
    PointHandle.SetToInvalid();
    return PointHandle;
}

//---------- get Handle by ID -----------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetPointHandle(int_max PointID) const
{
    Handle_Of_Point_Of_SurfaceMesh PointHandle;

    if (PointID < 0)
    {
        MDK_Warning("Invalid PointID (< 0) @ SurfaceMesh::GetPointHandle(...)")
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    auto it = m_MeshData->Map_PointID_to_PointIndex.find(PointID);
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
        PointHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Warning("Invalid PointID @ SurfaceMesh::GetPointHandle(...)")
        PointHandle.SetToInvalid();
    }
        
    return PointHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh  SurfaceMesh<MeshAttributeType>::GetEdgeHandle(int_max EdgeID) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (EdgeID < 0)
    {
        MDK_Warning("Invalid EdgeID (< 0) @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetToInvalid();
        return EdgeHandle;
    }

    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        EdgeHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Warning("Invalid EdgeID @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetToInvalid();
    }

    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max DirectedEdgeID) const
{
    Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle;

    if (DirectedEdgeID < 0)
    {
        MDK_Warning("Invalid DirectedEdgeID (< 0) @ SurfaceMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.SetToInvalid();
        return DirectedEdgeHandle;
    }

    auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
    if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
    {
        DirectedEdgeHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Warning("Invalid DirectedEdgeID @ SurfaceMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.SetToInvalid();
    }

    return DirectedEdgeHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(int_max CellID) const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (CellID < 0)
    {
        MDK_Warning("Invalid CellID (< 0) @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    auto it = m_MeshData->Map_CellID_to_CellIndex.find(CellID);
    if (it != m_MeshData->Map_CellID_to_CellIndex.end())
    {
        CellHandle.SetIndex(it->second);
    }
    else
    {
        MDK_Warning("Invalid CellID @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.SetToInvalid();
    }

    return CellHandle;
}


//----------- get EdgeHandle and DirectedEdgeHandle by PointHandleList or PointIDList ----------------------------------------------//

template<typename MeshAttributeType>
inline 
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetEdgeHandle(Handle_Of_Point_Of_SurfaceMesh PointHandle0, Handle_Of_Point_Of_SurfaceMesh PointHandle1) const
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (this->IsValidHandle(PointHandle0) == false || this->IsValidHandle(PointHandle1) == false)
    {
        MDK_Warning("PointHandle0 or PointHandle1 is invalid @ SurfaceMesh::GetEdgeHandle(...)")
        EdgeHandle.SetToInvalid();
        return EdgeHandle;
    }

    auto PointIndex0 = PointHandle0.GetIndex();
    auto PointIndex1 = PointHandle1.GetIndex();

    if (m_MeshData->PointList[PointIndex0].IsOnEdge() == true && m_MeshData->PointList[PointIndex1].IsOnEdge() == true)
    {
        const auto& tempAdjacentEdgeIndexList = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
        for (int_max k = 0; k < tempAdjacentEdgeIndexList.GetLength(); ++k)
        {
            int_max tempPointIdex_a, tempPointIdex_b;
            m_MeshData->EdgeList[tempAdjacentEdgeIndexList[k]].GetPointIndexList(tempPointIdex_a, tempPointIdex_b);
            if (PointIndex1 == tempPointIdex_a || PointIndex1 == tempPointIdex_b)
            {
                EdgeHandle.SetIndex(tempAdjacentEdgeIndexList[k]);
                return EdgeHandle;
            }
        }
    }

    MDK_Warning("No Edge between PointHandle0 and PointHandle1 @ SurfaceMesh::GetEdgeHandle(...)")
    EdgeHandle.SetToInvalid();
    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
GetEdgeHandle(int_max PointID0, int_max PointID1) const
{
    auto PointHandle0 = this->GetPointHandle(PointID0);
    auto PointHandle1 = this->GetPointHandle(PointID1);
    return this->GetEdgeHandle(PointHandle0, PointHandle1);
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
        DirectedEdgeHandle.SetToInvalid();
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

    DirectedEdgeHandle.SetToInvalid();
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetDirectedEdgeHandle(int_max PointID_start, int_max PointID_end) const
{
    auto PointHandle_start = this->GetPointHandle(PointID_start);
    auto PointHandle_end = this->GetPointHandle(PointID_end);
    return this->GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
}

//----------- get CellHandle by EdgeHandleList or EdgeIDList ------------------------------------------//

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList) const
{
    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            MDK_Warning("EdgeHandleList  is invalid @ SurfaceMesh::GetCellHandle(...)")
            CellHandle.SetToInvalid();
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
        MDK_Warning("EdgeHandleList is invalid @ SurfaceMesh::GetCellHandle(...)")
        CellHandle.SetToInvalid();
        return CellHandle;
    }
    else
    {
        CellHandle.SetIndex(CellIndexList[tempList[0]]);
        return CellHandle;
    }
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::GetCellHandle(const DenseVector<int_max>& EdgeIDList) const
{
    if (EdgeIDList.IsEmpty() == true)
    {
        Handle_Of_Cell_Of_SurfaceMesh CellHandle;
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList);

        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            Handle_Of_Cell_Of_SurfaceMesh CellHandle;
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    return this->GetCellHandle(EdgeHandleList);
}

//-------------- check ID -------------------------------------------------------//

template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::IsValidPointID(int_max PointID) const
{
    if (PointID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_PointID_to_PointIndex.find(PointID);
    return (it != m_MeshData->Map_PointID_to_PointIndex.end());
}

template<typename MeshAttributeType>
inline 
bool SurfaceMesh<MeshAttributeType>::IsValidEdgeID(int_max EdgeID) const
{
    if (EdgeID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
    return (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end());
}

template<typename MeshAttributeType>
inline 
bool SurfaceMesh<MeshAttributeType>::IsValidDirectedEdgeID(int_max DirectedEdgeID) const
{
    if (DirectedEdgeID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
    return (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end());
}

template<typename MeshAttributeType>
inline
bool SurfaceMesh<MeshAttributeType>::IsValidCellID(int_max CellID) const
{
    if (CellID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_CellID_to_CellIndex.find(CellID);
    return (it != m_MeshData->Map_CellID_to_CellIndex.end());
}

//--------- get IDList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<int_max> SurfaceMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetPointIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetPointIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_PointID_to_PointIndex.size()));
    for (auto it = m_MeshData->Map_PointID_to_PointIndex.begin(); it != m_MeshData->Map_PointID_to_PointIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> SurfaceMesh<MeshAttributeType>::GetEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void SurfaceMesh<MeshAttributeType>::GetEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_EdgeID_to_EdgeIndex.size()));
    for (auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.begin(); it != m_MeshData->Map_EdgeID_to_EdgeIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> SurfaceMesh<MeshAttributeType>::GetDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.size()));
    for (auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> SurfaceMesh<MeshAttributeType>::GetCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void SurfaceMesh<MeshAttributeType>::GetCellIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_CellID_to_CellIndex.size()));
    for (auto it = m_MeshData->Map_CellID_to_CellIndex.begin(); it != m_MeshData->Map_CellID_to_CellIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

//----------- get ID by handle -----------------------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetPointID(Handle_Of_Point_Of_SurfaceMesh PointHandle) const
{
    if (this->IsValidHandle(PointHandle) == true)
    {
        return m_MeshData->PointList[PointHandle.GetIndex()].GetID();
    }
    else
    {
        return -1;
    }
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetEdgeID(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle) const
{
    if (this->IsValidHandle(EdgeHandle) == true)
    {
        return m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetID();
    }
    else
    {
        return -1;
    }
}

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetDirectedEdgeID(Handle_Of_DirectedEdge_Of_SurfaceMesh DirectedEdgeHandle) const
{
    if (this->IsValidHandle(DirectedEdgeHandle) == true)
    {
        auto Index = DirectedEdgeHandle.GetIndex();
        return m_MeshData->DirectedEdgePairList[Index.EdgeIndex][Index.RelativeIndex].GetID();
    }
    else
    {
        return -1;
    }
}

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetCellID(Handle_Of_Cell_Of_SurfaceMesh CellHandle) const
{
    if (this->IsValidHandle(CellHandle) == true)
    {
        return m_MeshData->CellList[CellHandle.GetIndex()].GetID();
    }
    else
    {
        return -1;
    }
}

//----------- get PointID by position ----------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetPointID(typename MeshAttributeType::ScalarType Position[3]) const
{
    auto PointHandle = this->GetPointHandle(Position);
    return this->GetPointID(PointHandle);
}

template<typename MeshAttributeType>
inline int_max SurfaceMesh<MeshAttributeType>::
GetPointID(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z) const
{
    auto PointHandle = this->GetPointHandle(x, y, z);
    return this->GetPointID(PointHandle);
}

//----------- get EdgeID and DirectedEdgeID by PointHandleList or PointIDList ---------------------//

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetEdgeID(Handle_Of_Point_Of_SurfaceMesh PointHandle0, Handle_Of_Point_Of_SurfaceMesh PointHandle1) const
{
    auto EdgeHandle = this->GetEdgeHandle(PointHandle0, PointHandle1);
    return this->GetEdgeID(EdgeHandle);
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetEdgeID(int_max PointID0, int_max PointID1) const
{
    auto PointHandle0 = this->GetPointHandle(PointID0);
    auto PointHandle1 = this->GetPointHandle(PointID1);
    return this->GetEdgeID(PointHandle0, PointHandle1);
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::
GetDirectedEdgeID(Handle_Of_Point_Of_SurfaceMesh PointHandle_start, Handle_Of_Point_Of_SurfaceMesh PointHandle_end) const
{
    auto DirectedEdgeHandle = this->GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
    return this->GetDirectedEdgeID(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetDirectedEdgeID(int_max PointID_start, int_max PointID_end) const
{
    auto PointHandle_start = this->GetPointHandle(PointID_start);
    auto PointHandle_end = this->GetPointHandle(PointID_end);
    return this->GetDirectedEdgeID(PointHandle_start, PointHandle_end)
}

//----------- get CellID by EdgeHandleList or EdgeIDList ------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max SurfaceMesh<MeshAttributeType>::GetCellID(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList) const
{
    auto CellHandle = this->GetCellHandle(EdgeHandleList);
    return this->GetCellID(CellHandle);
}

template<typename MeshAttributeType>
inline
int_max SurfaceMesh<MeshAttributeType>::GetCellID(const DenseVector<int_max>& EdgeIDList) const
{
    DenseVector<EdgeHandleType> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->GetCellID(EdgeHandleList);
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
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
AddPoint(const DenseVector<typename MeshAttributeType::ScalarType, 3>& Position)
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
AddPoint(const DenseVector<typename MeshAttributeType::ScalarType>& Position)
{
    if (PointSet.GetLength() != 3)
    {
        MDK_Error("Position is a vector but length != 3 @ SurfaceMesh::AddPoint(...)")
        Handle_Of_Point_Of_SurfaceMesh PointHandle;
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
AddPoint(const DenseMatrix<typename MeshAttributeType::ScalarType>& Position)
{
    if (Position.IsVector() == true)
    {
        if (Position.GetElementNumber() != 3)
        {
            MDK_Error("Position is a vector but length != 3 @ SurfaceMesh::AddPoint(...)")
            Handle_Of_Point_Of_SurfaceMesh PointHandle;
            PointHandle.SetToInvalid();
            return PointHandle;
        }
    }
    else
    {
        MDK_Error("Position is a not a vector @ SurfaceMesh::AddPoint(...)")
        Handle_Of_Point_Of_SurfaceMesh PointHandle;
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


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
   
    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

    Handle_Of_Point_Of_SurfaceMesh PointHandle;
    PointHandle.SetIndex(PointIndex);

    return PointHandle;
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_SurfaceMesh> SurfaceMesh<MeshAttributeType>::
AddPointSet(const DenseMatrix<typename MeshAttributeType::ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;

    if (PointSet.IsVector() == true)
    {
        if (PointSet.GetElementNumber() != 3)
        {
            MDK_Error("PointSet is a vector but length != 3 @ SurfaceMesh::AddPoint(...)")
            return PointHandleList;
        }

        PointHandleList.Resize(1);
        PointHandleList[0] = this->AddPoint(PointSet.GetPointer());
        return PointHandleList;
    }

    if (PointSet.GetRowNumber() != 3)
    {
        MDK_Error("PointSet is a matrix but RowNumber != 3 @ SurfaceMesh::AddPoint(...)")
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
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::
AddEdge(Handle_Of_Point_Of_SurfaceMesh PointHandle0, Handle_Of_Point_Of_SurfaceMesh PointHandle1)
{
    Handle_Of_Edge_Of_SurfaceMesh EdgeHandle;

    if (this->IsValidHandle(PointHandle0) == false || this->IsValidHandle(PointHandle1) == false)
    {
        MDK_Error("PointHandle0 or PointHandle1 is invalid @ SurfaceMesh::AddEdge(...)")
        EdgeHandle.SetToInvalid();
        return EdgeHandle;
    }

    auto EdgeHandle_temp = this->GetEdgeHandle(PointHandle0, PointHandle1);
    if (this->IsValidHandle(EdgeHandle_temp) == true)
    {
        MDK_Warning("The edge has been added already @ SurfaceMesh::AddEdge(...)")
        EdgeHandle = EdgeHandle_temp;
        return EdgeHandle;
    }

    //----------------------------------------------------------------
    auto PointIndex0 = PointHandle0.GetIndex();
    auto PointIndex1 = PointHandle1.GetIndex();

    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_SurfaceMesh<MeshAttributeType> Edge;
    Edge.Create();
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.SetPointIndexList(PointIndex0, PointIndex1);

    m_MeshData->EdgeList.Append(std::move(Edge));
    m_MeshData->EdgeValidityFlagList.Append(1);

    // add empty DirectedEdgePair to hold place
    DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2> DirectedEdgePair;
    m_MeshData->DirectedEdgePairList.Append(std::move(DirectedEdgePair));
    m_MeshData->DirectedEdgePairList[EdgeIndex][0].Clear();
    m_MeshData->DirectedEdgePairList[EdgeIndex][1].Clear();

    // update AdjacentPoint information in m_MeshData->PointList
    m_MeshData->PointList[PointIndex0].AdjacentPointIndexList().Append(PointIndex1);
    m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().Append(EdgeIndex);

    m_MeshData->PointList[PointIndex1].AdjacentPointIndexList().Append(PointIndex0);
    m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().Append(EdgeIndex);
    
    //------------
    EdgeHandle.SetIndex(EdgeIndex);
    return EdgeHandle;
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddEdge(int_max PointID0, int_max PointID1)
{
    auto PointHandle0 = this->GetPointHandle(PointID0);
    auto PointHandle1 = this->GetPointHandle(PointID1);
    return this->AddEdge(PointHandle0, PointHandle1);
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCellByEdge(const DenseVector<Handle_Of_Edge_Of_SurfaceMesh>& EdgeHandleList)
{
    // Input: the input EdgeIndexList must have been ordered
    // the order determine the direction/sign of the normal vector
    // it also determine the direction of each DirectedEdge

    Handle_Of_Cell_Of_SurfaceMesh CellHandle;

    if (EdgeHandleList.GetLength() < 2)
    {
        MDK_Error("length of EdgeHandleList < 2 @ SurfaceMesh::AddCellByEdge(...)")
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            MDK_Error("Invalid EdgeHandleList @ SurfaceMesh::AddCellByEdge(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    auto CellHandle_temp = this->GetCellHandle(EdgeHandleList);
    if (this->IsValidHandle(CellHandle_temp) == true)
    {
        MDK_Warning("The cell has been added already @ SurfaceMesh::AddCellByEdge(...)")
        CellHandle = CellHandle_temp;
        return CellHandle;
    }

    // get PointIndexList from EdgeHandleList ---------------------------------------------------------------------------//
       
    DenseVector<int_max> PointIndexList;
    PointIndexList.Resize(EdgeHandleList.GetLength());

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto tempEdgeIndex_a = EdgeHandleList[k].GetIndex();
        auto tempEdgeIndex_b = EdgeHandleList[k+1].GetIndex();

        int_max tempIndexList_a[2];
        m_MeshData->EdgeList[tempEdgeIndex_a].GetPointIndexList(tempIndexList_a);

        int_max tempIndexList_b[2];
        m_MeshData->EdgeList[tempEdgeIndex_b].GetPointIndexList(tempIndexList_b);

        if (tempIndexList_a[0] == tempIndexList_b[0] && tempIndexList_a[1] != tempIndexList_b[1])
        {           
            PointIndexList[k] = tempIndexList_a[1];
            PointIndexList[k + 1] = tempIndexList_a[0];
        }
        else if (tempIndexList_a[0] == tempIndexList_b[1] && tempIndexList_a[1] != tempIndexList_b[0])
        {
            PointIndexList[k] = tempIndexList_a[1];
            PointIndexList[k + 1] = tempIndexList_a[0];
        }
        else if (tempIndexList_a[1] == tempIndexList_b[0] && tempIndexList_a[0] != tempIndexList_b[1])
        {
            PointIndexList[k] = tempIndexList_a[0];
            PointIndexList[k + 1] = tempIndexList_a[1];
        }
        else if (tempIndexList_a[1] == tempIndexList_b[1] && tempIndexList_a[0] != tempIndexList_b[0])
        {
            PointIndexList[k] = tempIndexList_a[0];
            PointIndexList[k + 1] = tempIndexList_a[1];
        }
        else
        {
            MDK_Error("EdgeIndexList is not ordered or invalid @ SurfaceMesh::AddCellByEdge(...)")
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
            MDK_Error("A cell with a DirectedEdge has been added already @ SurfaceMesh::AddCellByEdge(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }

        DirectedEdgeIndexList[k].EdgeIndex = EdgeHandleList[k].GetIndex();
    }

    // Create each DirectedEdge
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // attention: auto& will get reference, auto will copy
        auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex];

        tempDirectedEdge.Create();
        tempDirectedEdge.SetParentMesh(*this);
        tempDirectedEdge.SetIndex(DirectedEdgeIndexList[k]);
        tempDirectedEdge.SetCellIndex(CellIndex);
        tempDirectedEdge.SetStartPointIndex(PointIndexList[k]);

        if (k < DirectedEdgeIndexList.GetLength() - 1)
        { 
            tempDirectedEdge.SetEndPointIndex(PointIndexList[k + 1]);
        }
        else // if (k == DirectedEdgeIndexList.GetLength() - 1)
        {
            tempDirectedEdge.SetEndPointIndex(PointIndexList[0]);
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

    // create cell --------------------------------------------------------------------------------------------------

    Cell_Of_SurfaceMesh<MeshAttributeType> Cell;
    Cell.Create();
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);

    // update information in m_MeshData->PointList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[DirectedEdgeIndexList[k].EdgeIndex][DirectedEdgeIndexList[k].RelativeIndex];
        auto tempPointIndex_start = tempDirectedEdge.GetStartPointIndex();
        auto tempPointIndex_end = tempDirectedEdge.GetEndPointIndex();
        m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
        m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Append(DirectedEdgeIndexList[k]);
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        m_MeshData->PointList[PointIndexList[k]].AdjacentCellIndexList().Append(CellIndex);
    }

    //-----------------------------
    CellHandle.SetIndex(CellIndex);
    return CellHandle;
}

template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCellByEdge(const DenseVector<int_max>& EdgeIDList)
{
    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->AddCellByEdge(EdgeIDList);    
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCellByPoint(const DenseVector<Handle_Of_Point_Of_SurfaceMesh>& PointHandleList)
{
    if (PointHandleList.GetLength() < 3)
    {
        MDK_Error("PointHandleList length < 3 @ SurfaceMesh::AddCellByPoint(...)")
        Handle_Of_Cell_Of_SurfaceMesh CellHandle;
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    DenseVector<Handle_Of_Edge_Of_SurfaceMesh> EdgeHandleList;
    EdgeHandleList.Resize(PointHandleList.GetLength());
    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (k < PointHandleList.GetLength() - 1)
        {
            EdgeHandleList[k] = this->AddEdge(PointHandleList[k], PointHandleList[k + 1]);
        }
        else
        {
            EdgeHandleList[k] = this->AddEdge(PointHandleList[k], PointHandleList[0]);
        }
    }

    return this->AddCellByEdge(EdgeHandleList);
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_SurfaceMesh SurfaceMesh<MeshAttributeType>::AddCellByPoint(const DenseVector<int_max>& PointIDList)
{
    DenseVector<Handle_Of_Point_Of_SurfaceMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
        PointHandleList[k] = this->GetPointHandle(PointIDList[k]);
    }

    return this->AddCellByPoint(PointHandleList);
}
//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_SurfaceMesh CellHandle)
{
    // this function will remove each DirectedEdge of the Cell, and modify any information related to the cell
    // CellHandle and CellID of the cell become invalid after the cell is deleted

    // check input 
    if (this->IsValidHandle(CellHandle) == false)
    {
        MKD_Error("Invalid CellHandle @ SurfaceMesh::DeleteCell(...)")
        return false;
    }

    //--------------------------------------------------------------------------------------
    auto CellIndex = CellHandle.GetIndex();

    // auto& will get reference
    const auto& DirectedEdgeIndexList = m_MeshData->CellList[CellIndex].DirectedEdgeIndexList();
    const auto& PointIndexList = m_MeshData->CellList[CellIndex].GetPointIndexList();

    // update information in m_MeshData->PointList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempDirectedEdgeIndex = DirectedEdgeIndexList[k];
        const auto& tempDirectedEdge = m_MeshData->DirectedEdgePairList[tempDirectedEdgeIndex.EdgeIndex][tempDirectedEdgeIndex.RelativeIndex];
        auto tempPointIndex_start = tempDirectedEdge.GetStartPointIndex();
        auto tempPointIndex_end = tempDirectedEdge.GetEndPointIndex();

        auto tempIndex_a = m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index == tempDirectedEdgeIndex);
        });
        m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Delete(tempIndex_a);

        auto tempIndex_b = m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index)
        {
            return (Index == tempDirectedEdgeIndex);
        });
        m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Delete(tempIndex_b);
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentCellIndexList().Find([&](int_max Index) {return Index == CellIndex});
        m_MeshData->PointList[PointIndexList[k]].AdjacentCellIndexList().Delete(tempIndex);
    }

    // Delete DirectedEdge: only release memory, not remove from DirectedEdgePairList
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); k++)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // delete DirectedEdgeID record if the map has it
        auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].GetID());
        if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        }
        // release memory
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
bool SurfaceMesh<MeshAttributeType>::DeleteCell(int_max CellID)
{
    auto CellHandle = this->GetCellHandle(CellID);
    return this->DeleteCell(CellHandle);
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle)
{    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        MDK_Error("Invalid EdgeHandle @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }

    // check AdjacentCell: the edge can not be deleted if any adjacent cell exit

    auto EdgeIndex = EdgeHandle.GetIndex();

    const auto& AdjacentCellIndexList = m_MeshData->EdgeList[EdgeIndex].GetAdjacentCellIndexList();
    if (AdjacentCellIndexList.IsEmpty() == false)
    {
        MDK_Error("AdjacentCellIndexList is not empty, so this edge can not be deleted @ SurfaceMesh::DeleteEdge(...)")
        return false;
    }
    // -------------------------------------------------------------------------------

    int_max PointIndexList[2];
    m_MeshData->EdgeList[EdgeIndex].GetPointIndexList(PointIndexList);

    auto PointIndex0 = PointIndexList[0];
    auto PointIndex1 = PointIndexList[1];

    // update AdjacentEdge information in m_MeshData->PointList
    for (int_max k = 0; k < 2; ++k)
    {
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentEdgeIndexList().Find([&](int_max Index) { return (Index == EdgeIndex); });
        m_MeshData->PointList[PointIndexList[k]].AdjacentEdgeIndexList().Delete(tempIndex);
    }

    // update AdjacentPoint information in m_MeshData->PointList    

    auto tempIndex_a = m_MeshData->PointList[PointIndex0].AdjacentPointIndexList().Find([&](int_max Index){return Index == PointIndex1; });
    m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().Delete(tempIndex_a);

    auto tempIndex_b = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList().Find([&](int_max Index){return Index == PointIndex0; });
    m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().Delete(tempIndex_b);

    // Delete EdgeID record if the map has it 
    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(m_MeshData->EdgeList[EdgeIndex].GetID());
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
    }

    // Delete Edge: only release memory, not remove from EdgeList
    m_MeshData->EdgeList[EdgeIndex].Clear();
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
    //--------------------------------------------------------

    return true;
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeleteEdge(int_max EdgeID)
{
    auto EdgeHandle = this->GetEdgeHandle(EdgeID);
    return this->DeleteEdge(EdgeHandle);
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_SurfaceMesh PointHandle)
{
    if (this->IsValidHandle(PointHandle) == false)
    {
        MDK_Error("Invalid PointHandle @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    auto PointIndex = PointHandle.GetIndex();

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ SurfaceMesh::DeletePoint(...)")
        return false;
    }

    //--------------------------------------------------------------------
    auto it = m_MeshData->Map_PointID_to_PointIndex.find(m_MeshData->PointList[PointIndex].GetID());
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
        m_MeshData->Map_PointID_to_PointIndex.erase(it);
    }

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


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(int_max PointID)
{
    auto PointHandle = this->GetPointHandle(PointID);
    return this->DeletePoint(PointHandle);
}


template<typename MeshAttributeType>
bool SurfaceMesh<MeshAttributeType>::DeletePoint(const DenseVector<int_max>& PointIDList)
{
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
        if (this->DeletePoint(PointIDList[k]) == false)
        {
            return false;
        }
    }
    return true;
}

//----------------- remove deleted item from Point/Edge/DirectedEdgePair/Cell list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index and handle may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when InvalidPointHandleNumber/ValidPointNumber( GetPointNumber() ) > 0.5

template<typename MeshAttributeType>
int_max SurfaceMesh<MeshAttributeType>::GetInvalidPointHandleNumber() const
{
    return m_MeshData->PointList.GetLength() - this->GetPointNumber();
}

template<typename MeshAttributeType>
void SurfaceMesh<MeshAttributeType>::CleanDataStructure()
{
    //-------------------------- clean PolintList and update Map_PointID_to_PointIndex -----------------------------------------------------------//
    DenseVector<int_max> ValidPointIndexList;
    ValidPointIndexList.ReserveCapacity(m_MeshData->PointList.GetLength());

    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k) // k is PointIndex
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            ValidPointIndexList.Append(k);

            auto PointIndex_k_new = ValidPointIndexList.GetLength() - 1;
            if (PointIndex_k_new != k)
            {// need to modify information related to the point
                // modify AdjacentPoint information in m_MeshData->PointList
                const auto& AdjacentPointIndexList_k = m_MeshData->PointList[k].AdjacentPointIndexList();
                for (int_max n = 0; n < AdjacentPointIndexList_k.GetLength(); ++k)
                {
                    // auto&: get reference and modify
                    auto& AdjacentPointIndexList_n = m_MeshData->PointList[AdjacentPointIndexList_k[n]].AdjacentPointIndexList();
                    auto tempIndex_n = AdjacentPointIndexList_n.Find([&](int_max Index) { return Index == k; });
                    
                    if (tempIndex_n.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about AdjacentPointIndexList_n @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }

                    AdjacentPointIndexList_n[tempIndex_n[0]] = PointIndex_k_new;
                }

                // modify PointIndex information in m_MeshData->EdgeList
                const auto& AdjacentEdgeIndexList_k = m_MeshData->PointList[k].AdjacentEdgeIndexList();
                for (int_max n = 0; n < AdjacentEdgeIndexList_k.GetLength(); ++k)
                {
                    // auto&: get reference and modify
                    auto& AdjacentEdge_n = m_MeshData->EdgeList[AdjacentEdgeIndexList_k[n]];
                    int_max PointIndex0, PointIndex1;                    
                    AdjacentEdge_n.GetPointIndexList(PointIndex0, PointIndex1);
                    if (PointIndex0 == k)
                    {   // keep the order in the PointIndexList
                        AdjacentEdge_n.SetPointIndexList(PointIndex_k_new, PointIndex1);
                    }
                    else if (PointIndex1 == k)
                    {
                        AdjacentEdge_n.SetPointIndexList(PointIndex0, PointIndex_k_new);
                    }
                    else
                    {
                        MDK_Error("Something is wrong about AdjacentEdge_n @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }
                }

                // modify PointIndex information in m_MeshData->DirectedEdgePairList
                const auto& OutgoingDirectedEdgeIndexList_k = m_MeshData->PointList[k].OutgoingDirectedEdgeIndexList();
                for (int_max n = 0; n < OutgoingDirectedEdgeIndexList_k.GetLength(); ++k)
                {
                    auto EdgeIndex_n = OutgoingDirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = OutgoingDirectedEdgeIndexList_k[n].RelativeIndex;
                    auto PointIndex_start_n = m_MeshData->DirectedEdgePairList[EdgeIndex_n][RelativeIndex_n].GetStartPointIndex();
                    if (PointIndex_start_n != k)
                    {
                        MDK_Error("Something is wrong about OutgoingDirectedEdgeIndexList_k @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }
                    m_MeshData->DirectedEdgePairList[EdgeIndex_n][RelativeIndex_n].SetStartPointIndex(PointIndex_k_new);
                }

                const auto& IncomingDirectedEdgeIndexList_k = m_MeshData->PointList[k].IncomingDirectedEdgeIndexList();
                for (int_max n = 0; n < IncomingDirectedEdgeIndexList_k.GetLength(); ++k)
                {
                    auto EdgeIndex_n = IncomingDirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = IncomingDirectedEdgeIndexList_k[n].RelativeIndex;
                    auto PointIndex_end_n = m_MeshData->DirectedEdgePairList[EdgeIndex_n][RelativeIndex_n].GetEndPointIndex();
                    if (PointIndex_end_n != k)
                    {
                        MDK_Error("Something is wrong about IncomingDirectedEdgeIndexList_k @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }
                    m_MeshData->DirectedEdgePairList[EdgeIndex_n][RelativeIndex_n].SetEndPointIndex(PointIndex_k_new);
                }
            }
        }
    }// for each PoinIndex k
    
    if (ValidPointIndexList.GetLength() != m_MeshData->PointList.GetLength())
    {
        // remove deleted item from PointList
        DataArray<Point_Of_SurfaceMesh<MeshAttributeType>> PointList_new;
        PointList_new.Resize(ValidPointIndexList.GetLength());
        for (int_max k = 0; k < ValidPointIndexList.GetLength(); ++k)
        {
            PointList_new[k] = std::move(m_MeshData->PointList[ValidPointIndexList[k]]);
            PointList_new[k].SetIndex(k);
        }
        m_MeshData->PointList = std::move(PointList_new);

        m_MeshData->PointValidityFlagList.FastResize(ValidPointIndexList.GetLength());
        m_MeshData->PointValidityFlagList.Fill(1);

        // update Map_PointID_to_PointIndex
        m_MeshData->Map_PointID_to_PointIndex.clear();
        for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
        {
            auto PointID_k = m_MeshData->PointList[k].GetID();
            if (PointID_k >= 0) // ID is invalid if < 0 
            {
                m_MeshData->Map_PointID_to_PointIndex[k] = PointID_k;
            }
        }
    }
    //ValidPointIndexList.Clear();

    //--------------------- done clean PolintList and update Map_PointID_to_PointIndex ------------------------------------------------------------//

    //---- clean EdgeList and DirectedEdgePairList and update Map_EdgeID_to_EdgeIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex -----------------//

    DenseVector<int_max> ValidEdgeIndexList;
    ValidEdgeIndexList.ReserveCapacity(m_MeshData->EdgeList.GetLength());

    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k) // k is EdgeIndex
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            ValidEdgeIndexList.Append(k);

            auto EdgeIndex_k_new = ValidEdgeIndexList.GetLength() - 1;
            if (EdgeIndex_k_new != k)
            {// need to modify information related to the edge

                // modify AdjacentEdge information in m_MeshData->PointList
                int_max PointIndexList_k[2];
                m_MeshData->EdgeList[k].GetPointIndexList(PointIndexList_k);
                for (int_max n = 0; n < 2; ++n)
                {
                    // auto& : get reference and modify
                    auto& AdjacentEdgeIndexList_n = m_MeshData->PointList[PointIndexList_k[n]].AdjacentEdgeIndexList();
                    auto tempIndex = AdjacentEdgeIndexList_n.Find([&](int_max Index){return Index == k; });
                    if (tempIndex.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about PointIndexList_k @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }
                    AdjacentEdgeIndexList_n[tempIndex[0]] = EdgeIndex_k_new;
                }

                // modify DirectedEdgeIndex information in m_MeshData->CellList
                for (int_max n = 0; n < 2; ++n)
                {
                    if (m_MeshData->DirectedEdgePairList[k][n].IsValid() == true)
                    {
                        auto CellIndex_n = m_MeshData->DirectedEdgePairList[k][n].GetCellIndex();
                        // auto& : get reference and modify
                        auto& DirectedEdgeIndexList_n = m_MeshData->CellList[CellIndex_n].DirectedEdgeIndexList();
                        auto tempIndex = DirectedEdgeIndexList_n.Find([&](DirectedEdgeIndex_Of_SurfaceMesh Index) {return Index.EdgeIndex == k; });
                        if (tempIndex.GetLength() != 1)
                        {
                            MDK_Error("Something is wrong about DirectedEdgeIndexList @ SurfaceMesh::CleanDataStructure()")
                            return;
                        }
                        DirectedEdgeIndexList_n[tempIndex[0]].EdgeIndex = EdgeIndex_k_new;
                    }
                }
            }
        }
    }// for each EdgeIndex k

    if (ValidEdgeIndexList.GetLength() != m_MeshData->EdgeList.GetLength())
    {
        // remove deleted item from EdgeList and DirectedEdgePairList

        DataArray<Edge_Of_SurfaceMesh<MeshAttributeType>> EdgeList_new;
        EdgeList_new.Resize(ValidEdgeIndexList.GetLength());

        DataArray<DenseVector<DirectedEdge_Of_SurfaceMesh<MeshAttributeType>, 2>> DirectedEdgePairList_new;
        DirectedEdgePairList_new.Resize(ValidEdgeIndexList.GetLength());

        for (int_max k = 0; k < ValidEdgeIndexList.GetLength(); ++k)
        {
            EdgeList_new[k] = std::move(m_MeshData->EdgeList[ValidEdgeIndexList[k]]);
            EdgeList_new[k].SetIndex(k);

            DirectedEdgePairList_new[k] = std::move(m_MeshData->DirectedEdgePairList[ValidEdgeIndexList[k]]);
            
            if (DirectedEdgePairList_new[k][0].IsValid() == true)
            {
                DirectedEdgePairList_new[k][0].SetIndex(k, 0);
            }            
           
            if (DirectedEdgePairList_new[k][1].IsValid() == true)
            {
                DirectedEdgePairList_new[k][1].SetIndex(k, 1);
            }
        }

        m_MeshData->EdgeList = std::move(EdgeList_new);
        m_MeshData->DirectedEdgePairList = std::move(DirectedEdgePairList_new);

        m_MeshData->EdgeValidityFlagList.FastResize(ValidEdgeIndexList.GetLength());
        m_MeshData->EdgeValidityFlagList.Fill(1);

        // update Map_EdgeID_to_EdgeIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex
        m_MeshData->Map_EdgeID_to_EdgeIndex.clear();
        m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.clear();
        for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
        {
            auto EdgeID_k = m_MeshData->EdgeList[k].GetID();
            if (EdgeID_k >= 0) // ID is invalid if < 0 
            {
                m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeID_k] = k;
            }

            // auto& : get reference and modify
            auto& DirectedEdge_0 = m_MeshData->DirectedEdgePairList[k][0];
            if (DirectedEdge_0.IsValid() == true)
            {
                auto DirectedEdgeID_k_0 = DirectedEdge_0.GetID();
                if (DirectedEdgeID_k_0 >= 0) // ID is invalid if < 0 
                {
                    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID_k_0] = DirectedEdge_0.GetIndex();
                }
            }

            // auto& : get reference and modify
            auto& DirectedEdge_1 = m_MeshData->DirectedEdgePairList[k][1];
            if (DirectedEdge_1.IsValid() == true)
            {
                auto DirectedEdgeID_k_1 = DirectedEdge_1.GetID();
                if (DirectedEdgeID_k_1 >= 0) // ID is invalid if < 0 
                {
                    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID_k_1] = DirectedEdge_1.GetIndex();
                }
            }
        }
    }
    //ValidEdgeIndexList.Clear();

    //---- done clean EdgeList and DirectedEdgePairList and update Map_EdgeID_to_EdgeIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex --------//

    //------------------ clean CellList and Update Map_CellID_to_CellIndex ---------------------------------------------------------------------//

    DenseVector<int_max> ValidCellIndexList;
    ValidCellIndexList.ReserveCapacity(m_MeshData->CellList.GetLength());

    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k) // k is CellIndex
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            ValidCellIndexList.Append(k);

            auto CellIndex_k_new = ValidCellIndexList.GetLength() - 1;
            if (CellIndex_k_new != k)
            {// need to modify information related to the cell

                // modify AdjacentCell information in m_MeshData->PointList
                auto PointIndexList_k = m_MeshData->CellList[k].GetPointIndexList();
                for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
                {
                    // auto& : get reference and modify
                    auto& tempAdjacentCellIndexList = m_MeshData->PointList[PointIndexList_k[n]].AdjacentCellIndexList();
                    auto tempIndex = tempAdjacentCellIndexList.Find([&](int_max Index) {return Index == k; });
                    if (tempIndex.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about AdjacentCellIndexList @ SurfaceMesh::CleanDataStructure()")
                        return;
                    }
                    tempAdjacentCellIndexList[tempIndex[0]] = CellIndex_k_new;
                }

                // modify CellIndex Information in m_MeshData->DirectedEdgePairList 
                const auto& DirectedEdgeIndexList_k = m_MeshData->CellList[k].DirectedEdgeIndexList();
                for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
                {
                    auto tempEdgeIndex = DirectedEdgeIndexList_k[n].EdgeIndex;
                    auto tempRelativeIndex = DirectedEdgeIndexList_k[n].RelativeIndex;
                    m_MeshData->DirectedEdgePairList[tempEdgeIndex][tempRelativeIndex].SetCellIndex(CellIndex_k_new);
                }
            }
        }
    }// for each CellIndex k

    // remove deleted item from CellList
    if (ValidCellIndexList.GetLength() != m_MeshData->CellList.GetLength())
    {
        DataArray<Cell_Of_SurfaceMesh<MeshAttributeType>> CellList_new;
        CellList_new.Resize(ValidCellIndexList.GetLength());
        for (int_max k = 0; k < ValidCellIndexList.GetLength(); ++k)
        {
            CellList_new[k] = std::move(m_MeshData->CellList[ValidCellIndexList[k]]);
            CellList_new[k].SetIndex(k);
        }
        m_MeshData->CellList = std::move(CellList_new);

        m_MeshData->CellValidityFlagList.FastResize(ValidCellIndexList.GetLength());
        m_MeshData->CellValidityFlagList.Fill(1);

        // update Map_CellID_to_CellIndex
        m_MeshData->Map_CellID_to_CellIndex.clear();
        for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
        {
            auto CellID_k = m_MeshData->CellList[k].GetID();
            if (CellID_k >= 0) // ID is invalid if < 0 
            {
                m_MeshData->Map_CellID_to_CellIndex[k] = CellID_k;
            }
        }
    }
    //ValidCellIndexList.Clear();

    //--------------- done clean CellList and Update Map_CellID_to_CellIndex ----------------------------------------------------//
}

//-------------------- get a sub mesh by CellHandleList or CellIDList -----------------------------------------//

template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType> 
SurfaceMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<Handle_Of_Cell_Of_SurfaceMesh>& CellHandleList) const
{
    SurfaceMesh<MeshAttributeType> OutputMesh;
    
    if (CellHandleList.IsEmpty() == true)
    {
        return OutputMesh;
    }

    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ SurfaceMesh::GetSubMeshByCell(...)")
        return OutputMesh;
    }

    for (int_max k = 0; k < CellHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(CellHandleList[k]) == false)
        {
            MDK_Error("CellHandleList is invalid @ SurfaceMesh::GetSubMeshByCell(...)")
            return OutputMesh;
        }
    }
    //-----------------------------------------------------------

    // add cell one by one
    for (int_max k = 0; k < CellHandleList.GetLength(); ++k)
    {
        auto CellID_k = this->Cell(CellHandleList[k]).GetID();
        const auto& CellAttribute_k = this->Cell(CellHandleList[k]).Attribute();
        auto PointHandleList_k = this->Cell(CellHandleList[k]).GetPointHandleList();
        auto EdgeHandleList_k = this->Cell(CellHandleList[k]).GetEdgeHandleList();
        auto DirectedEdgeHandleList_k = this->Cell(CellHandleList[k]).GetDirectedEdgeHandleList();

        // add point
        DenseVector<PointHandleType> PointHandleList_OutputMesh;
        PointHandleList_OutputMesh.Resize(PointHandleList_k.GetLength());
        for (int_max n = 0; n < PointHandleList_k.GetLength(); ++n)
        {
            auto PointHandle_n = PointHandleList_k[n];

            // add Point Position
            ScalarType Position_n[3];
            this->Point(PointHandle_n).GetPosition(Position_n);
            PointHandleList_OutputMesh[k] = OutputMesh.AddPoint(Position_n);
            // copy PointID and Attribute
            auto PointID_n = this->Point(PointHandle_n).GetID();
            const auto& PointAttribute_n = this->Point(PointHandle_n).Attribute();
            OutputMesh.Point(PointHandleList_OutputMesh[k]).SetID(PointID_n);
            OutputMesh.Point(PointHandleList_OutputMesh[k]).Attribute() = PointAttribute_n;
        }

        // add cell
        auto CellHandle_OutputMesh = OutputMesh.AddCellByPoint(PointHandleList_OutputMesh);
        // copy cell ID and Attribute
        OutputMesh.Cell(CellHandle_OutputMesh).SetID(CellID_k);
        OutputMesh.Cell(CellHandle_OutputMesh).Attribute() = CellAttribute_k;

        // copy Edge ID and Attribute
        auto EdgeHandleList_OutputMesh = OutputMesh.Cell(CellHandle_OutputMesh).GetEdgeHandleList();
        for (int_max n = 0; n < EdgeHandleList_OutputMesh.GetLength(); ++n)
        {
            auto EdgeID_n = this->Edge(EdgeHandleList_k[n]).GetID();
            const auto& EdgeAttribute_n = this->Edge(EdgeHandleList_k[n]).Attribute();

            OutputMesh.Edge(EdgeHandleList_OutputMesh[n]).SetID(EdgeID_n);
            OutputMesh.Edge(EdgeHandleList_OutputMesh[n]).Attribute() = EdgeAttribute_n;
        }

        // copy DirectedEdge ID and Attribute
        auto DirectedEdgeHandleList_OutputMesh = OutputMesh.Cell(CellHandle_OutputMesh).GetDirectedEdgeHandleList();
        for (int_max n = 0; n < DirectedEdgeHandleList_OutputMesh.GetLength(); ++n)
        {
            auto DirectedEdgeID_n = this->DirectedEdge(DirectedEdgeHandleList_k[n]).GetID();
            const auto& DirectedEdgeAttribute_n = this->DirectedEdge(DirectedEdgeHandleList_k[n]).Attribute();

            OutputMesh.DirectedEdge(DirectedEdgeHandleList_OutputMesh[n]).SetID(DirectedEdgeID_n);
            OutputMesh.DirectedEdge(DirectedEdgeHandleList_OutputMesh[n]).Attribute() = DirectedEdgeAttribute_n;
        }
    }

    return OutputMesh;
}


template<typename MeshAttributeType>
SurfaceMesh<MeshAttributeType> 
SurfaceMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const
{
    DenseVector<CellHandleType> CellHandleList;
    for (int_max k = 0; k < CellIDList.GetLength(); ++k)
    {
        CellHandleList[k] = this->GetCellHandle(CellIDList[k]);
    }
    return this->GetSubMeshByCell(CellHandleList);
}

}// namespace mdk

#endif