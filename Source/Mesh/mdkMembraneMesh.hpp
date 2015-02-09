#ifndef mdk_MembraneMesh_hpp
#define mdk_MembraneMesh_hpp

namespace mdk
{

template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::MembraneMesh()
{
    m_MeshData = std::make_shared<MembraneMeshData<MeshAttributeType>>();
}


template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::MembraneMesh(const MDK_Symbol_PureEmpty&)
{
}


template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::MembraneMesh(const MembraneMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::MembraneMesh(MembraneMesh<MeshAttributeType>&& InputMesh)
{
	m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::~MembraneMesh()
{
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::operator=(const MembraneMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::operator=(MembraneMesh<MeshAttributeType>&& InputMesh)
{
	this->Copy(std::move(InputMesh));
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->PointPositionTable.Clear();
    m_MeshData->PointList.Clear();
    m_MeshData->PointValidityFlagList.Clear();

    m_MeshData->EdgeList.Clear();
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
bool MembraneMesh<MeshAttributeType>::Copy(const MembraneMesh<MeshAttributeType>* InputMesh)
{
	if (InputMesh == nullptr)
	{
		MDK_Error("Input is nullptr @ MembraneMesh::Copy(...)")
			return false;
	}

	this->Copy(*InputMesh);

	return true;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Copy(const MembraneMesh<MeshAttributeType>& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<MembraneMeshData<MeshAttributeType>>();
    }

    if (!InputMesh.m_MeshData)
    {
        return;
    }

    m_MeshData->PointPositionTable = InputMesh.m_MeshData->PointPositionTable;
    m_MeshData->PointValidityFlagList = InputMesh.m_MeshData->PointValidityFlagList;
    m_MeshData->PointList = InputMesh.m_MeshData->PointList;
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            m_MeshData->PointList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->EdgeValidityFlagList = InputMesh.m_MeshData->EdgeValidityFlagList;
    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            m_MeshData->EdgeList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->CellValidityFlagList = InputMesh.m_MeshData->CellValidityFlagList;
    m_MeshData->CellList = InputMesh.m_MeshData->CellList;
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            m_MeshData->CellList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->Map_PointID_to_PointIndex = InputMesh.m_MeshData->Map_PointID_to_PointIndex;
    m_MeshData->Map_EdgeID_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex;
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex;
    m_MeshData->Map_CellID_to_CellIndex = InputMesh.m_MeshData->Map_CellID_to_CellIndex;

    m_MeshData->Attribute = InputMesh.m_MeshData->Attribute;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Copy(MembraneMesh<MeshAttributeType>&& InputMesh)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<MembraneMeshData<MeshAttributeType>>();
	}

	if (!InputMesh.m_MeshData)
	{
		return;
	}

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
	for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
	{
		if (m_MeshData->EdgeValidityFlagList[k] == 1)
		{
			m_MeshData->EdgeList[k].SetParentMesh(*this);
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
bool MembraneMesh<MeshAttributeType>::Share(MembraneMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ MembraneMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Share(MembraneMesh& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool MembraneMesh<MeshAttributeType>::ForceShare(const MembraneMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ MembraneMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::ForceShare(const MembraneMesh<MeshAttributeType>& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline 
bool MembraneMesh<MeshAttributeType>::IsEmpty() const
{
    return m_MeshData->PointPositionTable.IsEmpty();
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetPointNumber() const
{
    return m_MeshData->PointValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetEdgeNumber() const
{
    return m_MeshData->EdgeValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetDirectedEdgeNumber() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
	{
		if (m_MeshData->EdgeValidityFlagList[k] == 1)
		{
			for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
			{
				const auto& DirectedEdge_n = m_MeshData->EdgeList[k].DirectedEdgeList()[n];
				if (DirectedEdge_n.IsValid() == true)
				{
					Counter += 1;
				}
			}
		}
	}

	return Counter;
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetCellNumber() const
{
    return m_MeshData->CellValidityFlagList.Sum();
}

//------ Get/Set GlobalAttribute -----------------------------------//

template<typename MeshAttributeType>
inline
typename MeshAttributeType::GlobalAttribute& MembraneMesh<MeshAttributeType>::Attribute()
{
    return m_MeshData->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::GlobalAttribute& MembraneMesh<MeshAttributeType>::Attribute() const
{
    return m_MeshData->Attribute;
}

//------------- Get All the position (valid point) --------------------------------//

template<typename MeshAttributeType>
inline 
DenseMatrix<typename MeshAttributeType::ScalarType> 
MembraneMesh<MeshAttributeType>::GetPointPositionMatrix() const
{
	DenseMatrix<ScalarType> PointPositionMatrix;
	this->GetPointPositionMatrix(PointPositionMatrix);
	return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetPointPositionMatrix(DenseMatrix<ScalarType>& PositionMatrix) const
{
	auto MaxNumber = m_MeshData->PointPositionTable.GetColNumber();
	PositionMatrix.FastResize(3, this->GetPointNumber());
	int_max Counter = 0;
	for (int_max k = 0; k < MaxNumber; ++k)
	{
		if (m_MeshData->PointValidityFlagList[k] == 1)
		{
			PositionMatrix.SetCol(Counter, m_MeshData->PointPositionTable.GetPointerOfCol(k));
			Counter += 1;
		}
	}
}

//---- Get/Set 3D Position by PointHandle or PointID --------------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
SetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, 
                typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), { x, y, z });
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::SetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, const typename MeshAttributeType::ScalarType Position[3])
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), Position);
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::
SetPointPosition(int_max PointID, typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z)
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->SetPointPosition(PointHandle, x, y, z);
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::SetPointPosition(int_max PointID, const typename MeshAttributeType::ScalarType Position[3])
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->SetPointPosition(PointHandle, Position);
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> 
MembraneMesh<MeshAttributeType>::GetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle) const
{
    DenseVector<ScalarType, 3> Position;
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position.GetPointer());
    return Position;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::
GetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, 
                 typename MeshAttributeType::ScalarType& x, typename MeshAttributeType::ScalarType& y, typename MeshAttributeType::ScalarType& z) const
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, typename MeshAttributeType::ScalarType Position[3]) const
{
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3>
MembraneMesh<MeshAttributeType>::GetPointPosition(int_max PointID) const
{
    auto PointHandle = this->GetPointHandleByID(PointID);
    return this->GetPointPosition(PointHandle);
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointPosition(int_max PointID, ScalarType& x, ScalarType& y, ScalarType& z) const
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->GetPointPosition(PointHandle, x, y, z);
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointPosition(int_max PointID, ScalarType Position[3]) const
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->GetPointPosition(PointHandle, Position);
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList, 
                 const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == true)
    {
        return;
    }
    else if (PointHandleList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == false)
    {
        MDK_Error("Invalid input @ MembraneMesh::SetPointPosition(...)")
        return;
    }
    else if (PointHandleList.IsEmpty() == false && PointPositionMatrix.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ MembraneMesh::SetPointPosition(...)")
        return;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        auto PointIndex = PointHandleList[k].GetIndex();
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColNumber())
        {
            MDK_Error("Invalid PointIndex @ MembraneMesh::SetPointPosition(...)")
            return;
        }

        m_MeshData->PointPositionTable.SetCol(PointIndex, PointPositionMatrix.GetPointerOfCol(k));
    }
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix)
{
    if (PointIDList.IsEmpty() == true)
    {
        MDK_Error("Empty PointIDList @ MembraneMesh::SetPointPosition(...)")
        return;
    }

    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);

        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("Invalid PointIDList @ MembraneMesh::SetPointPosition(...)")
            return;
        }
    }

    this->SetPointPosition(PointHandleList, PointPositionMatrix);
}


template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType> 
MembraneMesh<MeshAttributeType>::GetPointPosition(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList) const
{
    DenseMatrix<MeshAttributeType::ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointHandleList, PointPositionMatrix);
    return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
GetPointPosition(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList,
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
            MDK_Error("Invalid PointIndex @ MembraneMesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex));
    }
}


template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType>
MembraneMesh<MeshAttributeType>::GetPointPosition(const DenseVector<int_max>& PointIDList) const
{
    DenseMatrix<ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointIDList, PointPositionMatrix);
    return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
GetPointPosition(const DenseVector<int_max>& PointIDList, DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionMatrix) const
{
    if (PointIDList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);

        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            MDK_Error("Invalid PointIDList @ MembraneMesh::GetPointPosition(...)")
            return;
        }
    }

    this->GetPointPosition(PointPositionMatrix, PointHandleList);
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_MembraneMesh PointHandle)
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(Handle_Of_Point_Of_MembraneMesh PointHandle) const
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(int_max PointID)
{
	auto PointHandle = this->GetPointHandleByID(PointID);
	return this->Point(PointHandle);
}

template<typename MeshAttributeType>
inline
const Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(int_max PointID) const
{
	auto PointHandle = this->GetPointHandleByID(PointID);
	return this->Point(PointHandle);
}

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle)
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle) const
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(int_max EdgeID)
{
	auto EdgeHandle = this->GetEdgeHandleByID(EdgeID);
	return this->Edge(EdgeHandle);
}

template<typename MeshAttributeType>
inline
const Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(int_max EdgeID) const
{
	auto EdgeHandle = this->GetEdgeHandleByID(EdgeID);
	return this->Edge(EdgeHandle);
}


template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle)
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
	return m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& 
MembraneMesh<MeshAttributeType>::DirectedEdge(Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[RelativeIndex];
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::DirectedEdge(int_max DirectedEdgeID)
{
	auto DirectedEdgeHandle = this->GetDirectedEdgeHandleByID(DirectedEdgeID);
	return this->DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::DirectedEdge(int_max DirectedEdgeID) const
{
	auto DirectedEdgeHandle = this->GetDirectedEdgeHandleByID(DirectedEdgeID);
	return this->DirectedEdge(DirectedEdgeHandle);
}

template<typename MeshAttributeType>
inline
Cell_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_MembraneMesh CellHandle)
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Cell_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Cell(Handle_Of_Cell_Of_MembraneMesh CellHandle) const
{
    return m_MeshData->CellList[CellHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Cell_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Cell(int_max CellID)
{
	auto CellHandle = this->GetCellHandleByID(CellID);
	return this->Cell(CellHandle)
}

template<typename MeshAttributeType>
inline
const Cell_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Cell(int_max CellID) const
{
	auto CellHandle = this->GetCellHandleByID(CellID);
	return this->Cell(CellHandle)
}

//-------------- check handle -------------------------------------------------------//

template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Point_Of_MembraneMesh PointHandle) const
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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Edge_Of_MembraneMesh EdgeHandle) const
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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle) const
{
    auto EdgeIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();

    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength() || RelativeIndex < 0)
    {
        return false;
    }
    else
    {
		if (RelativeIndex >= m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList().GetLength())
		{
			return false
		}
		else
		{
			return m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[RelativeIndex].IsValid();
		}
    }
}

template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Cell_Of_MembraneMesh CellHandle) const
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
DenseVector<Handle_Of_Point_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(m_MeshData->PointList.GetLength());
    Handle_Of_Point_Of_MembraneMesh PointHandle;
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
DenseVector<Handle_Of_Edge_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(m_MeshData->EdgeList.GetLength());
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;
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
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(2*m_MeshData->EdgeList.GetLength());    
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
			for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
			{
				const auto& DirectedEdge_n = m_MeshData->EdgeList[k].DirectedEdgeList()[n];
				if (DirectedEdge_n.IsValid() == true)
				{
					Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle;
					DirectedEdgeHandle.SetIndex(k, n);
					OutputHandleList.Append(DirectedEdgeHandle);
				}
			}
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Cell_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetCellHandleList() const
{
    DenseVector<Handle_Of_Cell_Of_MembraneMesh> OutputHandleList;
    this->GetCellHandleList(OutputHandleList);
    return OutputHandleList;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetCellHandleList(DenseVector<Handle_Of_Cell_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.ReserveCapacity(m_MeshData->CellList.GetLength());
    Handle_Of_Cell_Of_MembraneMesh CellHandle;
    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            CellHandle.SetIndex(k);
            OutputHandleList.Append(CellHandle);
        }
    }
}

//----------- get PointHandle by Position, ID, ----------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(const DenseVector<typename MeshAttributeType::ScalarType, 3>& Position, 
                         typename MeshAttributeType::ScalarType DistanceThreshold) const
{
    return this->GetPointHandleByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(const typename MeshAttributeType::ScalarType Position[3],
                         typename MeshAttributeType::ScalarType DistanceThreshold) const
{
    return this->GetPointHandleByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(typename MeshAttributeType::ScalarType x, 
                         typename MeshAttributeType::ScalarType y, 
                         typename MeshAttributeType::ScalarType z,
                         typename MeshAttributeType::ScalarType DistanceThreshold) const
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;
    PointHandle.SetToInvalid();

    if (this->IsEmpty() == true)
    {        
        return PointHandle;
    }

    if (DistanceThreshold < 0)
    {
        MDK_Error("DistanceThreshold < 0 @ MembraneMesh::GetPointHandleByPosition(...)")
        return PointHandle;
    }

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    ScalarType Threshold_sq = DistanceThreshold*DistanceThreshold;
    ScalarType LowerLimit_sq = std::min(eps_value, Threshold_sq);

    ScalarType Distance_sq_min = 0;
    int_max PointIndex_min = -1;

    for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColNumber(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
            ScalarType Pos[3];
            m_MeshData->PointPositionTable.GetCol(k, Pos);

            auto Distance_sq = (Pos[0] - x)*(Pos[0] - x) + (Pos[1] - y)*(Pos[1] - y) + (Pos[2] - z)*(Pos[2] - z);

            if (Distance_sq <= LowerLimit_sq)
            {
                PointHandle.SetIndex(k);
                return PointHandle;
            }

            if (PointIndex_min == -1) // set initial value
            {
                Distance_sq_min = Distance_sq;
                PointIndex_min = k;
            }

            if (Distance_sq <= Distance_sq_min)
            {
                Distance_sq_min = Distance_sq;
                PointIndex_min = k;
            }                                
        }
    }

    if (Distance_sq_min <= Threshold_sq)
    {
        PointHandle.SetIndex(PointIndex_min);
    }

    //MDK_Warning("Invalid Position @ MembraneMesh::GetPointHandleByPosition(...)")
    return PointHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetPointHandleByID(int_max PointID) const
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;

    if (PointID < 0)
    {
        //MDK_Warning("Invalid PointID (< 0) @ MembraneMesh::GetPointHandleByID(...)")
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
        //MDK_Warning("Invalid PointID @ MembraneMesh::GetPointHandleByID(...)")
        PointHandle.SetToInvalid();
    }
        
    return PointHandle;
}

//----------- get EdgeHandle by ID, PointHandleList, PointIDList -------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh  MembraneMesh<MeshAttributeType>::GetEdgeHandleByID(int_max EdgeID) const
{
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    if (EdgeID < 0)
    {
        //MDK_Warning("Invalid EdgeID (< 0) @ MembraneMesh::GetEdgeHandleByID(...)")
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
        //MDK_Warning("Invalid EdgeID @ MembraneMesh::GetEdgeHandleByID(...)")
        EdgeHandle.SetToInvalid();
    }

    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandle0, Handle_Of_Point_Of_MembraneMesh PointHandle1) const
{
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    if (this->IsValidHandle(PointHandle0) == false || this->IsValidHandle(PointHandle1) == false)
    {
        //MDK_Warning("PointHandle0 or PointHandle1 is invalid @ MembraneMesh::GetEdgeHandleByPoint(...)")
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

    //MDK_Warning("No Edge between PointHandle0 and PointHandle1 @ MembraneMesh::GetEdgeHandleByPoint(...)")
    EdgeHandle.SetToInvalid();
    return EdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetEdgeHandleByPoint(int_max PointID0, int_max PointID1) const
{
	auto PointHandle0 = this->GetPointHandleByID(PointID0);
	auto PointHandle1 = this->GetPointHandleByID(PointID1);
    return this->GetEdgeHandleByPoint(PointHandle0, PointHandle1);
}

//---------- - get DirectedEdgeHandle by ID, PointHandleList, PointIDList ------------------------------------ -//

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleByID(int_max DirectedEdgeID) const
{
    Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle;

    if (DirectedEdgeID < 0)
    {
        //MDK_Warning("Invalid DirectedEdgeID (< 0) @ MembraneMesh::GetDirectedEdgeHandle(...)")
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
        //MDK_Warning("Invalid DirectedEdgeID @ MembraneMesh::GetDirectedEdgeHandle(...)")
        DirectedEdgeHandle.SetToInvalid();
    }

    return DirectedEdgeHandle;
}


template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetDirectedEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandle_start, Handle_Of_Point_Of_MembraneMesh PointHandle_end) const
{
    Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle;

    if (this->IsValidHandle(PointHandle_start) == false || this->IsValidHandle(PointHandle_end) == false)
    {
        DirectedEdgeHandle.SetToInvalid();
        return DirectedEdgeHandle;
    }
    
    auto PointIndex_start = PointHandle_start.GetIndex();
    auto PointIndex_end = PointHandle_end.GetIndex();

    const auto& OutgoingDirectedEdgeIndexList_0 = m_MeshData->PointList[PointIndex_start].OutgoingDirectedEdgeIndexList();
    for (int_max k = 0; k < OutgoingDirectedEdgeIndexList_0.GetLength(); ++k)
    {
        auto tempEdgeIndex = OutgoingDirectedEdgeIndexList_0[k].EdgeIndex;
        auto tempRelativeIndex = OutgoingDirectedEdgeIndexList_0[k].RelativeIndex;
        auto tempPointIndex_end = m_MeshData->EdgeList[tempEdgeIndex].DirectedEdgeList()[tempRelativeIndex].GetEndPointIndex();
        if (tempPointIndex_end == PointIndex_end)
        {
            DirectedEdgeHandle.SetIndex(tempEdgeIndex, tempRelativeIndex);
            return DirectedEdgeHandle;
        }
    }
  
    DirectedEdgeHandle.SetToInvalid();
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleByPoint(int_max PointID_start, int_max PointID_end) const
{
	auto PointHandle_start = this->GetPointHandleByID(PointID_start);
	auto PointHandle_end = this->GetPointHandleByID(PointID_end);
    return this->GetDirectedEdgeHandleByPoint(PointHandle_start, PointHandle_end);
}

//----------- get CellHandle by ID, PointHandleList, PointIDList, EdgeHandleList, or EdgeIDList ----------//

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetCellHandleByID(int_max CellID) const
{
    Handle_Of_Cell_Of_MembraneMesh CellHandle;

    if (CellID < 0)
    {
        //MDK_Warning("Invalid CellID (< 0) @ MembraneMesh::GetCellHandle(...)")
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
        //MDK_Warning("Invalid CellID @ MembraneMesh::GetCellHandle(...)")
        CellHandle.SetToInvalid();
    }

    return CellHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetCellHandleByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList) const
{
    Handle_Of_Cell_Of_MembraneMesh CellHandle;

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            //MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetCellHandleByEdge(...)")
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

        int_max Counter = 0;

        for (int_max n = 0; n < AdjacentCellIndexList_a.GetLength(); ++n)
        {            
            for (int_max m = 0; m < AdjacentCellIndexList_b.GetLength(); ++m)
            {
                if (AdjacentCellIndexList_a[n] == AdjacentCellIndexList_b[m])
                {
                    CellIndexList[k] = AdjacentCellIndexList_a[n];
                    Counter += 1;
                }
            }
        }

        if (Counter > 1)
        {
            MDK_Warning("EdgeHandleList is invalid : more than one copy of a handle @ MembraneMesh::GetCellHandle(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    auto tempList = CellIndexList.FindUnique();
    if (tempList.GetLength() != 1)
    {
        //MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetCellHandleByEdge(...)")
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
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetCellHandleByEdge(const DenseVector<int_max>& EdgeIDList) const
{
    if (EdgeIDList.IsEmpty() == true)
    {
        Handle_Of_Cell_Of_MembraneMesh CellHandle;
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    DenseVector<Handle_Of_Edge_Of_MembraneMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->GetCellHandleByEdge(EdgeHandleList);
}


template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_MembraneMesh 
MembraneMesh<MeshAttributeType>::GetCellHandleByPoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList) const
{
    Handle_Of_Cell_Of_MembraneMesh CellHandle;

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            //MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetCellHandleByPoint(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    DenseVector<int_max> CellIndexList;
    CellIndexList.Resize(PointHandleList.GetLength());
    CellIndexList.Fill(-1);

    for (int_max k = 0; k < PointHandleList.GetLength() - 1; ++k)
    {
        const auto& AdjacentCellIndexList_a = m_MeshData->PointList[PointHandleList[k].GetIndex()].AdjacentCellIndexList();
        const auto& AdjacentCellIndexList_b = m_MeshData->PointList[PointHandleList[k + 1].GetIndex()].AdjacentCellIndexList();

        int_max Counter = 0;

        for (int_max n = 0; n < AdjacentCellIndexList_a.GetLength(); ++n)
        {
            for (int_max m = 0; m < AdjacentCellIndexList_b.GetLength(); ++m)
            {
                if (AdjacentCellIndexList_a[n] == AdjacentCellIndexList_b[m])
                {
                    CellIndexList[k] = AdjacentCellIndexList_a[n];
                    Counter += 1;
                }
            }
        }

        if (Counter > 1)
        {
            MDK_Warning("PointHandleList is invalid : more than one copy of a handle @ MembraneMesh::GetCellHandleByPoint(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    auto tempList = CellIndexList.FindUnique();
    if (tempList.GetLength() != 1)
    {
        //MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetCellHandleByPoint(...)")
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
Handle_Of_Cell_Of_MembraneMesh
MembraneMesh<MeshAttributeType>::GetCellHandleByPoint(const DenseVector<int_max>& PointIDList) const
{
    if (PointIDList.IsEmpty() == true)
    {
        Handle_Of_Cell_Of_MembraneMesh CellHandle;
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);
    }

    return this->GetCellHandleByPoint(PointHandleList);
}


//-------------- check ID -------------------------------------------------------//

template<typename MeshAttributeType>
inline
bool MembraneMesh<MeshAttributeType>::IsValidPointID(int_max PointID) const
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
bool MembraneMesh<MeshAttributeType>::IsValidEdgeID(int_max EdgeID) const
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
bool MembraneMesh<MeshAttributeType>::IsValidDirectedEdgeID(int_max DirectedEdgeID) const
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
bool MembraneMesh<MeshAttributeType>::IsValidCellID(int_max CellID) const
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
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetPointIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_PointID_to_PointIndex.size()));
    for (auto it = m_MeshData->Map_PointID_to_PointIndex.begin(); it != m_MeshData->Map_PointID_to_PointIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_EdgeID_to_EdgeIndex.size()));
    for (auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.begin(); it != m_MeshData->Map_EdgeID_to_EdgeIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.size()));
    for (auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetCellIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetCellIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetCellIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
    OutputIDList.ReserveCapacity(int_max(m_MeshData->Map_CellID_to_CellIndex.size()));
    for (auto it = m_MeshData->Map_CellID_to_CellIndex.begin(); it != m_MeshData->Map_CellID_to_CellIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

//----------- get Point ID by Handle, Position -----------------------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetPointIDByHandle(Handle_Of_Point_Of_MembraneMesh PointHandle) const
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
int_max MembraneMesh<MeshAttributeType>::GetPointIDByPosition(typename MeshAttributeType::ScalarType Position[3]) const
{
    auto PointHandle = this->GetPointHandleByPosition(Position);
    return this->GetPointID(PointHandle);
}

template<typename MeshAttributeType>
inline int_max MembraneMesh<MeshAttributeType>::
GetPointIDByPosition(typename MeshAttributeType::ScalarType x, typename MeshAttributeType::ScalarType y, typename MeshAttributeType::ScalarType z) const
{
    auto PointHandle = this->GetPointHandleByPosition(x, y, z);
    return this->GetPointID(PointHandle);
}

//----------- get Edge ID by EdgeHandle, PointHandleList, PointIDList -----------------------------------------------------------//

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetEdgeIDByHandle(Handle_Of_Edge_Of_MembraneMesh EdgeHandle) const
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
int_max MembraneMesh<MeshAttributeType>::GetEdgeIDByPoint(Handle_Of_Point_Of_MembraneMesh PointHandle0, Handle_Of_Point_Of_MembraneMesh PointHandle1) const
{
    auto EdgeHandle = this->GetEdgeHandle(PointHandle0, PointHandle1);
    return this->GetEdgeID(EdgeHandle);
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetEdgeIDByPoint(int_max PointID0, int_max PointID1) const
{
	auto PointHandle0 = this->GetPointHandleByID(PointID0);
	auto PointHandle1 = this->GetPointHandleByID(PointID1);
    return this->GetEdgeID(PointHandle0, PointHandle1);
}

//----------- get DirectedEdge ID by DirectedEdgeHandle, PointHandle, PointID -------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDByHandle(Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle) const
{
    if (this->IsValidHandle(DirectedEdgeHandle) == true)
    {
        auto Index = DirectedEdgeHandle.GetIndex();
		return m_MeshData->EdgeList[Index.EdgeIndex].DirectedEdgeList()[Index.RelativeIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::
GetDirectedEdgeIDByPoint(Handle_Of_Point_Of_MembraneMesh PointHandle_start, Handle_Of_Point_Of_MembraneMesh PointHandle_end) const
{
    auto DirectedEdgeHandle = this->GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
    return this->GetDirectedEdgeIDByHandle(DirectedEdgeHandle);
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDByPoint(int_max PointID_start, int_max PointID_end) const
{
	auto PointHandle_start = this->GetPointHandleByID(PointID_start);
	auto PointHandle_end = this->GetPointHandleByID(PointID_end);
    return this->GetDirectedEdgeIDByPoint(PointHandle_start, PointHandle_end)
}

//----------- get CellID by CellHandle, EdgeHandleList, EdgeIDList, PointHandleList, PointIDList --------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetCellIDByHandle(Handle_Of_Cell_Of_MembraneMesh CellHandle) const
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


template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetCellIDByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList) const
{
    auto CellHandle = this->GetCellHandleByEdge(EdgeHandleList);
    return this->GetCellIDByHandle(CellHandle);
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetCellIDByEdge(const DenseVector<int_max>& EdgeIDList) const
{
    DenseVector<EdgeHandleType> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->GetCellIDByEdge(EdgeHandleList);
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
    Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
    Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfDirectedEdge()
{
    Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfDirectedEdge() const
{
    Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfCell()
{
    Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfCell() const
{
    Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

//------------ ReserveCapacity, ReleaseUnusedCapacity -------------------------------------//
template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::ReserveCapacity(int_max PointNumber, int_max EdgeNumber, int_max CellNumber)
{
	if (m_MeshData->PointPositionTable->ReserveCapacity(3 * PointNumber) == false)
	{
		return false;
	}
	if (m_MeshData->PointList->ReserveCapacity(PointNumber) == false)
	{
		return false;
	}
	if (m_MeshData->PointValidityFlagList->ReserveCapacity(PointNumber) == false)
	{
		return false;
	}
	if (m_MeshData->EdgeList->ReserveCapacity(EdgeNumber) == false)
	{
		return false;
	}
	if (m_MeshData->EdgeValidityFlagList->ReserveCapacity(EdgeNumber) == false)
	{
		return false;
	}
	if (m_MeshData->CellList->ReserveCapacity(CellNumber) == false)
	{
		return false;
	}
	if (m_MeshData->CellValidityFlagList->ReserveCapacity(CellNumber) == false)
	{
		return false;
	}
	return true;
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::ReleaseUnusedCapacity()
{
	m_MeshData->PointPositionTable->ReleaseUnusedCapacity();
	m_MeshData->PointList->ReleaseUnusedCapacity();
	m_MeshData->PointValidityFlagList->ReleaseUnusedCapacity();
	m_MeshData->EdgeList->ReleaseUnusedCapacity();
	m_MeshData->EdgeValidityFlagList->ReleaseUnusedCapacity();
	m_MeshData->CellList->ReleaseUnusedCapacity();
	m_MeshData->CellValidityFlagList->ReleaseUnusedCapacity();
}

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//

template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddPoint(const DenseVector<ScalarType, 3>& Position)
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddPoint(const DenseVector<ScalarType>& Position)
{
    if (PointSet.GetLength() != 3)
    {
        MDK_Error("Position is a vector but length != 3 @ MembraneMesh::AddPoint(...)")
        Handle_Of_Point_Of_MembraneMesh PointHandle;
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddPoint(const DenseMatrix<ScalarType>& Position)
{
    if (Position.IsVector() == true)
    {
        if (Position.GetElementNumber() != 3)
        {
            MDK_Error("Position is a vector but length != 3 @ MembraneMesh::AddPoint(...)")
            Handle_Of_Point_Of_MembraneMesh PointHandle;
            PointHandle.SetToInvalid();
            return PointHandle;
        }
    }
    else
    {
        MDK_Error("Position is a not a vector @ MembraneMesh::AddPoint(...)")
        Handle_Of_Point_Of_MembraneMesh PointHandle;
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddPoint(const ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh
MembraneMesh<MeshAttributeType>::AddPoint(ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColNumber() - 1;

    Point_Of_MembraneMesh<MeshAttributeType> Point;
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
   
    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

    Handle_Of_Point_Of_MembraneMesh PointHandle;
    PointHandle.SetIndex(PointIndex);

    return PointHandle;
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::AddPointSet(const DenseMatrix<ScalarType>& PointSet)
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;

    if (PointSet.IsVector() == true)
    {
        if (PointSet.GetElementNumber() != 3)
        {
            MDK_Error("PointSet is a vector but length != 3 @ MembraneMesh::AddPointSet(...)")
            return PointHandleList;
        }

        PointHandleList.Resize(1);
        PointHandleList[0] = this->AddPoint(PointSet.GetPointer());
        return PointHandleList;
    }

    if (PointSet.GetRowNumber() != 3)
    {
        MDK_Error("PointSet is a matrix but RowNumber != 3 @ MembraneMesh::AddPointSet(...)")
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
DenseVector<Handle_Of_Point_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::AddPointSet(DenseMatrix<ScalarType>&& PointSet)
{
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;

	if (PointSet.IsVector() == true)
	{
		if (PointSet.GetElementNumber() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ MembraneMesh::AddPointSet(...)")
			return PointHandleList;
		}

		PointHandleList.Resize(1);
		PointHandleList[0] = this->AddPoint(PointSet.GetPointer());
		return PointHandleList;
	}

	if (PointSet.GetRowNumber() != 3)
	{
		MDK_Error("PointSet is a matrix but RowNumber != 3 @ MembraneMesh::AddPointSet(...)")
		return PointHandleList;
	}

	if (this->IsEmpty() == true)
	{
		m_MeshData->PointPositionTable = std::move(PointSet);
		PointHandleList.Resize(m_MeshData->PointPositionTable.GetColNumber());
		for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColNumber(); ++k)
		{
			Point_Of_MembraneMesh<MeshAttributeType> Point;
			Point.SetParentMesh(*this);
			Point.SetIndex(k);

			m_MeshData->PointList.Append(std::move(Point));
			m_MeshData->PointValidityFlagList.Append(1);

			PointHandleList[k].SetIndex(k);
		}
	}
	else
	{
		PointHandleList.Resize(PointSet.GetColNumber());
		for (int_max k = 0; k < PointSet.GetColNumber(); ++k)
		{
			PointHandleList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
		}
	}
	return PointHandleList;
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
AddEdge(Handle_Of_Point_Of_MembraneMesh PointHandle0, Handle_Of_Point_Of_MembraneMesh PointHandle1)
{
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    if (this->IsValidHandle(PointHandle0) == false || this->IsValidHandle(PointHandle1) == false)
    {
        MDK_Error("PointHandle0 or PointHandle1 is invalid @ MembraneMesh::AddEdge(...)")
        EdgeHandle.SetToInvalid();
        return EdgeHandle;
    }

    auto EdgeHandle_temp = this->GetEdgeHandleByPoint(PointHandle0, PointHandle1);
    if (this->IsValidHandle(EdgeHandle_temp) == true)
    {
        //MDK_Warning("The edge has been added already @ MembraneMesh::AddEdge(...)")
        EdgeHandle = EdgeHandle_temp;
        return EdgeHandle;
    }

    //----------------------------------------------------------------
    auto PointIndex0 = PointHandle0.GetIndex();
    auto PointIndex1 = PointHandle1.GetIndex();

    auto EdgeIndex = m_MeshData->EdgeList.GetLength();

    Edge_Of_MembraneMesh<MeshAttributeType> Edge;
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.SetPointIndexList(PointIndex0, PointIndex1);

    m_MeshData->EdgeList.Append(std::move(Edge));
    m_MeshData->EdgeValidityFlagList.Append(1);

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
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddEdge(int_max PointID0, int_max PointID1)
{
	auto PointHandle0 = this->GetPointHandleByID(PointID0);
	auto PointHandle1 = this->GetPointHandleByID(PointID1);
    return this->AddEdge(PointHandle0, PointHandle1);
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddCellByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList)
{
    // Input: the input EdgeIndexList must have been ordered
    // the order determine the direction/sign of the normal vector
    // it also determine the direction of each DirectedEdge
    //
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

    Handle_Of_Cell_Of_MembraneMesh CellHandle;

    if (EdgeHandleList.GetLength() < 2)
    {
        MDK_Error("length of EdgeHandleList < 2 @ MembraneMesh::AddCellByEdge(...)")
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            MDK_Error("Invalid EdgeHandleList @ MembraneMesh::AddCellByEdge(...)")
            CellHandle.SetToInvalid();
            return CellHandle;
        }
    }

    auto CellHandle_temp = this->GetCellHandleByEdge(EdgeHandleList);
    if (this->IsValidHandle(CellHandle_temp) == true)
    {
        //MDK_Warning("The cell has been added already @ MembraneMesh::AddCellByEdge(...)")
        CellHandle = CellHandle_temp;
        return CellHandle;
    }

    DenseVector<int_max> EdgeIndexList;
    EdgeIndexList.Resize(EdgeHandleList.GetLength());
    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        EdgeIndexList[k] = EdgeHandleList[k].GetIndex();
    }

    // check EdgeIndexList
    auto tempList_unique = EdgeIndexList.FindUnique();
    if (tempList_unique.GetLength() != EdgeHandleList.GetLength())
    {
        MDK_Error("EdgeHandleList is invalid : more than one copy of a handle @ MembraneMesh::AddCellByEdge(...)")
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    // get PointIndexList from EdgeIndexList ---------------------------------------------------------------------------//
       
    DenseVector<int_max> PointIndexList;
    PointIndexList.Resize(EdgeIndexList.GetLength());

    for (int_max k = 0; k < EdgeIndexList.GetLength() - 1; ++k)
    {
        int_max tempPointIndexList_k[2];
        m_MeshData->EdgeList[EdgeIndexList[k]].GetPointIndexList(tempPointIndexList_k);

        int_max tempPointIndexList_next[2];
        m_MeshData->EdgeList[EdgeIndexList[k + 1]].GetPointIndexList(tempPointIndexList_next);

        if (tempPointIndexList_k[0] == tempPointIndexList_next[0])
        {           
            PointIndexList[k] = tempPointIndexList_k[1];
            PointIndexList[k + 1] = tempPointIndexList_k[0];
        }
        else if (tempPointIndexList_k[0] == tempPointIndexList_next[1])
        {
            PointIndexList[k] = tempPointIndexList_k[1];
            PointIndexList[k + 1] = tempPointIndexList_k[0];
        }
        else if (tempPointIndexList_k[1] == tempPointIndexList_next[0])
        {
            PointIndexList[k] = tempPointIndexList_k[0];
            PointIndexList[k + 1] = tempPointIndexList_k[1];
        }
        else if (tempPointIndexList_k[1] == tempPointIndexList_next[1])
        {
            PointIndexList[k] = tempPointIndexList_k[0];
            PointIndexList[k + 1] = tempPointIndexList_k[1];
        }
        else
        {
            MDK_Error("EdgeIndexList is not ordered or invalid @ MembraneMesh::AddCellByEdge(...)")
            CellHandle.SetIndex(-1);
            return CellHandle;
        }
    }

    // create DirectedEdge  -------------------------------------------------------
    auto CellIndex = m_MeshData->CellList.GetLength();

    DenseVector<DirectedEdgeIndex_Of_MembraneMesh> DirectedEdgeIndexList;
    DirectedEdgeIndexList.Resize(EdgeIndexList.GetLength());

	// create DirectedEdge in m_MeshData->EdgeList
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
		DirectedEdge_Of_MembraneMesh<MeshAttributeType> tempDirectedEdge;		
		m_MeshData->EdgeList[EdgeIndexList[k]].DirectedEdgeList().Append(std::move(tempDirectedEdge));

		DirectedEdgeIndexList[k].EdgeIndex = EdgeIndexList[k];
		DirectedEdgeIndexList[k].RelativeIndex = m_MeshData->EdgeList[EdgeIndexList[k]].DirectedEdgeList().GetLength()-1;
    }

    // set info to each DirectedEdge
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = DirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = DirectedEdgeIndexList[k].RelativeIndex;
        // attention: auto& will get reference, auto will copy
		auto& tempDirectedEdge = m_MeshData->EdgeList[tempEdgeIndex].DirectedEdgeList()[tempRelativeIndex];
		tempDirectedEdge.SetIndex(tempEdgeIndex, tempRelativeIndex);
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

    Cell_Of_MembraneMesh<MeshAttributeType> Cell;
    Cell.SetParentMesh(*this);
    Cell.SetIndex(CellIndex);
    Cell.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->CellList.Append(std::move(Cell));
    m_MeshData->CellValidityFlagList.Append(1);

    // update information in m_MeshData->PointList ------------------------------------------------------------------

    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
    {
        const auto& tempDirectedEdge = m_MeshData->EdgeList[DirectedEdgeIndexList[k].EdgeIndex].DirectedEdgeList()[DirectedEdgeIndexList[k].RelativeIndex];
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
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddCellByEdge(const DenseVector<int_max>& EdgeIDList)
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->AddCellByEdge(EdgeIDList);    
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddCellByPoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList)
{
    if (PointHandleList.GetLength() < 3)
    {        
        MDK_Error("PointHandleList length < 3 @ MembraneMesh::AddCellByPoint(...)")
        Handle_Of_Cell_Of_MembraneMesh CellHandle;
        CellHandle.SetToInvalid();
        return CellHandle;
    }

    DenseVector<Handle_Of_Edge_Of_MembraneMesh> EdgeHandleList;
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
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddCellByPoint(const DenseVector<int_max>& PointIDList)
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);
    }

    return this->AddCellByPoint(PointHandleList);
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeleteCell(Handle_Of_Cell_Of_MembraneMesh CellHandle)
{
    // this function will modify each DirectedEdge of the Cell, and modify any information related to the cell
    // CellHandle and CellID of the cell become invalid after the cell is deleted

    // check input 
    if (this->IsValidHandle(CellHandle) == false)
    {
        //MDK_Warning("Invalid CellHandle @ MembraneMesh::DeleteCell(...)")
        return true;
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
		const auto& tempDirectedEdge = m_MeshData->EdgeList[tempDirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[tempDirectedEdgeIndex.RelativeIndex];
        auto tempPointIndex_start = tempDirectedEdge.GetStartPointIndex();
        auto tempPointIndex_end = tempDirectedEdge.GetEndPointIndex();

        auto tempIndex_a = m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().ExactMatch(tempDirectedEdgeIndex);        
        m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Delete(tempIndex_a);

        auto tempIndex_b = m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().ExactMatch(tempDirectedEdgeIndex);        
        m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Delete(tempIndex_b);
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentCellIndexList().ExactMatch(CellIndex);
        m_MeshData->PointList[PointIndexList[k]].AdjacentCellIndexList().Delete(tempIndex);
    }

    // delete DirectedEdge
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); k++)
    {
		this->InternalFuction_DeleteDirectedEdge(DirectedEdgeIndexList[k]);
    }

    // delete Cell
    this->InternalFuction_DeleteCell(CellIndex);

    return true;
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeleteCell(int_max CellID)
{
    auto CellHandle = this->GetCellHandle(CellID);
    return this->DeleteCell(CellHandle);
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle)
{    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        //MDK_Warning("Invalid EdgeHandle @ MembraneMesh::DeleteEdge(...)")
        return true;
    }

    // check AdjacentCell: the edge can not be deleted if any adjacent cell exit

    auto EdgeIndex = EdgeHandle.GetIndex();

    auto AdjacentCellNumber = m_MeshData->EdgeList[EdgeIndex].GetAdjacentCellNumber();
    if (AdjacentCellNumber > 0)
    {
        MDK_Error("AdjacentCellIndexList is not empty, so this edge can not be deleted @ MembraneMesh::DeleteEdge(...)")
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
    m_MeshData->PointList[PointIndex0].AdjacentPointIndexList().Delete(tempIndex_a);

    auto tempIndex_b = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList().Find([&](int_max Index){return Index == PointIndex0; });
    m_MeshData->PointList[PointIndex1].AdjacentPointIndexList().Delete(tempIndex_b);

    // Delete Edge
    this->InternalFuction_DeleteEdge(EdgeIndex);
 
    return true;
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeleteEdge(int_max EdgeID)
{
    auto EdgeHandle = this->GetEdgeHandle(EdgeID);
    return this->DeleteEdge(EdgeHandle);
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_MembraneMesh PointHandle)
{
    if (this->IsValidHandle(PointHandle) == false)
    {
        //MDK_Warning("Invalid PointHandle @ MembraneMesh::DeletePoint(...)")
        return true;
    }

    auto PointIndex = PointHandle.GetIndex();

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ MembraneMesh::DeletePoint(...)")
        return false;
    }

    this->InternalFuction_DeletePoint(PointIndex);

    return true;
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList)
{
    if (PointHandleList.IsEmpty() == true)
    {
        //MDK_Warning("PointHandleList is empty @ MembraneMesh::DeletePoint(...)")
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
bool MembraneMesh<MeshAttributeType>::DeletePoint(int_max PointID)
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    return this->DeletePoint(PointHandle);
}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::DeletePoint(const DenseVector<int_max>& PointIDList)
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

//----------------- remove deleted item from Point/Edge/DirectedEdgeList/Cell list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index and handle may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when InvalidPointHandleNumber/ValidPointNumber( GetPointNumber() ) > 0.5

template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetInvalidPointHandleNumber() const
{
    auto ValidPointNumber = this->GetPointNumber();
    auto PointNumberOfList = m_MeshData->PointList.GetLength();
    return  ValidPointNumber - PointNumberOfList;
}

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::CleanDataStructure()
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
            if (PointIndex_k_new != k)// need to modify information related to the point
            {
                // modify AdjacentPoint information in m_MeshData->PointList
                const auto& AdjacentPointIndexList_k = m_MeshData->PointList[k].AdjacentPointIndexList();
                for (int_max n = 0; n < AdjacentPointIndexList_k.GetLength(); ++k)
                {
                    // auto&: get reference and modify
                    auto& AdjacentPointIndexList_n = m_MeshData->PointList[AdjacentPointIndexList_k[n]].AdjacentPointIndexList();
                    auto tempIndex_n = AdjacentPointIndexList_n.Find([&](int_max Index) { return Index == k; });
                    
                    if (tempIndex_n.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about AdjacentPointIndexList_n @ MembraneMesh::CleanDataStructure()")
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
                        MDK_Error("Something is wrong about AdjacentEdge_n @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
                }

                // modify PointIndex information in m_MeshData->EdgeList.DirectedEdgeList()
                const auto& OutgoingDirectedEdgeIndexList_k = m_MeshData->PointList[k].OutgoingDirectedEdgeIndexList();
                for (int_max n = 0; n < OutgoingDirectedEdgeIndexList_k.GetLength(); ++k)
                {
                    auto EdgeIndex_n = OutgoingDirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = OutgoingDirectedEdgeIndexList_k[n].RelativeIndex;
                    auto PointIndex_start_n = m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].GetStartPointIndex();
                    if (PointIndex_start_n != k)
                    {
                        MDK_Error("Something is wrong about OutgoingDirectedEdgeIndexList_k @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
					m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].SetStartPointIndex(PointIndex_k_new);
                }

                const auto& IncomingDirectedEdgeIndexList_k = m_MeshData->PointList[k].IncomingDirectedEdgeIndexList();
                for (int_max n = 0; n < IncomingDirectedEdgeIndexList_k.GetLength(); ++k)
                {
                    auto EdgeIndex_n = IncomingDirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = IncomingDirectedEdgeIndexList_k[n].RelativeIndex;
					auto PointIndex_end_n = m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].GetEndPointIndex();
                    if (PointIndex_end_n != k)
                    {
                        MDK_Error("Something is wrong about IncomingDirectedEdgeIndexList_k @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
					m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].SetEndPointIndex(PointIndex_k_new);
                }
            }
        }
    }// for each PoinIndex k
    
    if (ValidPointIndexList.GetLength() != m_MeshData->PointList.GetLength())
    {
        // remove deleted item from PointList
		PointListType PointList_new;
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
				m_MeshData->Map_PointID_to_PointIndex[PointID_k] = k;
            }
        }
    }
    //ValidPointIndexList.Clear();

    //--------------------- done clean PolintList and update Map_PointID_to_PointIndex ------------------------------------------------------------//

    //---- clean EdgeList and update Map_EdgeID_to_EdgeIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex -----------------//

    DenseVector<int_max> ValidEdgeIndexList;
    ValidEdgeIndexList.ReserveCapacity(m_MeshData->EdgeList.GetLength());

    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k) // k is EdgeIndex
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
            ValidEdgeIndexList.Append(k);

            auto EdgeIndex_k_new = ValidEdgeIndexList.GetLength() - 1;
            if (EdgeIndex_k_new != k)// need to modify information related to the edge
            {
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
                        MDK_Error("Something is wrong about PointIndexList_k @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
                    AdjacentEdgeIndexList_n[tempIndex[0]] = EdgeIndex_k_new;
                }

                // modify DirectedEdgeIndex.EdgeIndex in m_MeshData->CellList
				for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
                {
					if (m_MeshData->EdgeList[k].DirectedEdgeList()[n].IsValid() == true)
					{
						auto CellIndex_n = m_MeshData->EdgeList[k].DirectedEdgeList()[n].GetCellIndex();
						if (CellIndex_n >= 0)
						{
							// auto& : get reference and modify
							auto& DirectedEdgeIndexList_n = m_MeshData->CellList[CellIndex_n].DirectedEdgeIndexList();
							auto tempIndex = DirectedEdgeIndexList_n.Find([&](DirectedEdgeIndex_Of_MembraneMesh Index) {return Index.EdgeIndex == k; });
							if (tempIndex.GetLength() != 1)
							{
								MDK_Error("Something is wrong about DirectedEdgeIndexList @ MembraneMesh::CleanDataStructure()")
								return;
							}
							DirectedEdgeIndexList_n[tempIndex[0]].EdgeIndex = EdgeIndex_k_new;
						}
					}
                }
            }
        }
    }// for each EdgeIndex k

    if (ValidEdgeIndexList.GetLength() != m_MeshData->EdgeList.GetLength())
    {   // remove deleted item from EdgeList

		EdgeListType EdgeList_new;
        EdgeList_new.Resize(ValidEdgeIndexList.GetLength());

        for (int_max k = 0; k < ValidEdgeIndexList.GetLength(); ++k)
        {
            EdgeList_new[k] = std::move(m_MeshData->EdgeList[ValidEdgeIndexList[k]]);
            EdgeList_new[k].SetIndex(k);
        }

        m_MeshData->EdgeList = std::move(EdgeList_new);

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

			for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
			{
				// auto& : get reference and modify
				auto& tempDirectedEdge = m_MeshData->EdgeList[k].DirectedEdgeList()[n];
				if (tempDirectedEdge.IsValid() == true)
				{					
					auto tempDirectedEdgeID = tempDirectedEdge.GetID();
					if (tempDirectedEdgeID >= 0) // ID is invalid if < 0 
					{
						m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[tempDirectedEdgeID] = tempDirectedEdge.GetIndex();
					}
				}
			}
        }
    }
    //ValidEdgeIndexList.Clear();

    //---- done clean EdgeList and update Map_EdgeID_to_EdgeIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex --------//

    //------------------ clean CellList and Update Map_CellID_to_CellIndex ---------------------------------------------------------------------//

    DenseVector<int_max> ValidCellIndexList;
    ValidCellIndexList.ReserveCapacity(m_MeshData->CellList.GetLength());

    for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k) // k is CellIndex
    {
        if (m_MeshData->CellValidityFlagList[k] == 1)
        {
            ValidCellIndexList.Append(k);

            auto CellIndex_k_new = ValidCellIndexList.GetLength() - 1;
            if (CellIndex_k_new != k)// need to modify information related to the cell
            {
                // modify AdjacentCell information in m_MeshData->PointList
                auto PointIndexList_k = m_MeshData->CellList[k].GetPointIndexList();
                for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
                {
                    // auto& : get reference and modify
                    auto& tempAdjacentCellIndexList = m_MeshData->PointList[PointIndexList_k[n]].AdjacentCellIndexList();
                    auto tempIndex = tempAdjacentCellIndexList.Find([&](int_max Index) {return Index == k; });
                    if (tempIndex.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about AdjacentCellIndexList @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
                    tempAdjacentCellIndexList[tempIndex[0]] = CellIndex_k_new;
                }

                // modify CellIndex Information in m_MeshData->EdgeList.DirectedEdgeList() 
                const auto& DirectedEdgeIndexList_k = m_MeshData->CellList[k].DirectedEdgeIndexList();
                for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
                {
                    auto EdgeIndex_n = DirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = DirectedEdgeIndexList_k[n].RelativeIndex;
					m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].SetCellIndex(CellIndex_k_new);
                }
            }
        }
    }// for each CellIndex k

    // remove deleted item from CellList
    if (ValidCellIndexList.GetLength() != m_MeshData->CellList.GetLength())
    {
		CellListType CellList_new;
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
				m_MeshData->Map_CellID_to_CellIndex[CellID_k] = k;
            }
        }
    }
    //ValidCellIndexList.Clear();

    //--------------- done clean CellList and Update Map_CellID_to_CellIndex ----------------------------------------------------//

	//--------------- clean DirectedEdgeList and update Map_DirectedEdgeID_to_DirectedEdgeIndex ---------------------------------//

	for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k) // k is EdgeIndex_new
	{
		auto& DirectedEdgeList_old = m_MeshData->EdgeList[k].DirectedEdgeList();

		DenseVector<int_max> ValidDirectedEdgeRelativeIndexList;
		ValidDirectedEdgeRelativeIndexList.ReserveCapacity(DirectedEdgeList_old.GetLength());

		for (int_max n = 0; n < DirectedEdgeList_old.GetLength(); ++n)// n is RelativeIndex_old
		{
			DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex_old;
			DirectedEdgeIndex_old.EdgeIndex = k;
			DirectedEdgeIndex_old.RelativeIndex = n;

			if (DirectedEdgeList_old[n].IsValid() == true)
			{
				ValidDirectedEdgeRelativeIndexList.Append(n);
				auto RelativeIndex_new = ValidDirectedEdgeRelativeIndexList.GetLength() - 1;
				if (RelativeIndex_new != k)// need to modify information related to DirectedEdgeList_old[n]
				{
					DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex_new;
					DirectedEdgeIndex_new.EdgeIndex = k;
					DirectedEdgeIndex_new.RelativeIndex = RelativeIndex_new;

					// modify information in m_MeshData->PointList --------------------
					{
						auto StartPointIndex = DirectedEdgeList_old[n].GetStartPointIndex();
						// auto& take reference
						auto& OutgoingDirectedEdgeIndexList_n = m_MeshData->PointList[StartPointIndex].OutgoingDirectedEdgeIndexList();
						for (int_max m = 0; m < OutgoingDirectedEdgeIndexList_n.GetLength(); ++m)
						{
							auto tempIndex_m = OutgoingDirectedEdgeIndexList_n.ExactMatch("first", DirectedEdgeIndex_old);
							OutgoingDirectedEdgeIndexList_n[tempIndex_m] = DirectedEdgeIndex_new;
						}

						auto EndPointIndex = DirectedEdgeList_old[n].GetEndPointIndex();
						// auto& take reference
						auto& IncomingDirectedEdgeIndexList_n = m_MeshData->PointList[EndPointIndex].IncomingDirectedEdgeIndexList();
						for (int_max m = 0; m < IncomingDirectedEdgeIndexList_n.GetLength(); ++m)
						{
							auto tempIndex_m = IncomingDirectedEdgeIndexList_n.ExactMatch("first", DirectedEdgeIndex_old);
							IncomingDirectedEdgeIndexList_n[tempIndex_m] = DirectedEdgeIndex_new;
						}
					}
					// modify information in m_MeshData->EdgeList.DirectedEdgeList
					{
						auto NextDirectedEdgeIndex = DirectedEdgeList_old[n].GetNextDirectedEdgeIndex();
						auto tempEdgeIndex_a = NextDirectedEdgeIndex.EdgeIndex;
						auto tempRelativeIndex_a = NextDirectedEdgeIndex.RelativeIndex;
						m_MeshData->EdgeList[tempEdgeIndex_a].DirectedEdgeList()[tempRelativeIndex_a].SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_new);

						auto PreviousDirectedEdgeIndex = DirectedEdgeList_old[n].GetPreviousDirectedEdgeIndex();
						auto tempEdgeIndex_b = PreviousDirectedEdgeIndex.EdgeIndex;
						auto tempRelativeIndex_b = PreviousDirectedEdgeIndex.RelativeIndex;
						m_MeshData->EdgeList[tempEdgeIndex_b].DirectedEdgeList()[tempRelativeIndex_b].SetNextDirectedEdgeIndex(DirectedEdgeIndex_new);
					}
					// modify information in m_MeshData->CellList
					{
						auto CellIndex_n = DirectedEdgeList_old[n].GetCellIndex();
						// auto& take reference
						auto& DirectedEdgeIndexList = m_MeshData->CellList[CellIndex_n].DirectedEdgeIndexList();
						auto tempIndex_m = DirectedEdgeIndexList.ExactMatch("first", DirectedEdgeIndex_old);
						DirectedEdgeIndexList[tempIndex_m] = DirectedEdgeIndex_new;
					}
				}
			}
		}
		
		if (ValidDirectedEdgeRelativeIndexList.GetLength() != DirectedEdgeList_old.GetLength())
		{
			// remove deleted item
			DirectedEdgeListType DirectedEdgeList_new;
			DirectedEdgeList_new.Resize(ValidDirectedEdgeRelativeIndexList.GetLength());
			for (int_max n = 0; n < ValidCellIndexList.GetLength(); ++n)
			{
				DirectedEdgeList_new[n] = std::move(DirectedEdgeList_old[ValidDirectedEdgeRelativeIndexList[n]]);
				DirectedEdgeList_new[n].SetIndex(k, n);// EdgeIndex is k, RelativeIndex is n
			}
			m_MeshData->EdgeList[k].DirectedEdgeList() = std::move(DirectedEdgeList_new);

			// update m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex
			for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
			{
				// const auto& : get reference
				const auto& tempDirectedEdge = m_MeshData->EdgeList[k].DirectedEdgeList()[n];
				auto tempDirectedEdgeID = tempDirectedEdge.GetID();
				if (tempDirectedEdgeID >= 0) // ID is invalid if < 0 
				{
					m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[tempDirectedEdgeID] = tempDirectedEdge.GetIndex();
				}
			}
		}
	}
	//---------------- done clean DirectedEdgeList and update Map_DirectedEdgeID_to_DirectedEdgeIndex ----------------------------//
}

//-------------------- get a sub mesh by CellHandleList or CellIDList -----------------------------------------//

template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType> 
MembraneMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<Handle_Of_Cell_Of_MembraneMesh>& CellHandleList) const
{
    MembraneMesh<MeshAttributeType> OutputMesh;
    
    if (CellHandleList.IsEmpty() == true)
    {
        return OutputMesh;
    }

    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ MembraneMesh::GetSubMeshByCell(...)")
        return OutputMesh;
    }

    for (int_max k = 0; k < CellHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(CellHandleList[k]) == false)
        {
            MDK_Error("CellHandleList is invalid @ MembraneMesh::GetSubMeshByCell(...)")
            return OutputMesh;
        }
    }
    //-----------------------------------------------------------

    // point index on this mesh-> point index on output mesh
    std::unordered_map<int_max, int_max> Map_PointIndex_OuputIndex;        
    // edge index on this mesh-> edge index on output mesh
    std::unordered_map<int_max, int_max> Map_EdgeIndex_OuputIndex;

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
            auto PointID_n = this->Point(PointHandle_n).GetID();
            const auto& PointAttribute_n = this->Point(PointHandle_n).Attribute();
            ScalarType Position_n[3];
            this->Point(PointHandle_n).GetPosition(Position_n);

            // check if the point has already been added to OutputMesh
            auto it = Map_PointIndex_OuputIndex.find(PointHandle_n.GetIndex());
            if (it == Map_PointIndex_OuputIndex.end())
            {        
                // add Point Position
                PointHandleList_OutputMesh[n] = OutputMesh.AddPoint(Position_n);
                // copy PointID if it is valid
                if (PointID_n >= 0)
                {
                    OutputMesh.Point(PointHandleList_OutputMesh[n]).SetID(PointID_n);
                }
                // copy Attribute
                OutputMesh.Point(PointHandleList_OutputMesh[n]).Attribute() = PointAttribute_n;

                // add point to map
                Map_PointIndex_OuputIndex[PointHandle_n.GetIndex()] = PointHandleList_OutputMesh[n].GetIndex();
            }
            else
            {
                PointHandleList_OutputMesh[n].SetIndex(it->second);
            }
        }

        // add cell
        auto CellHandle_OutputMesh = OutputMesh.AddCellByPoint(PointHandleList_OutputMesh);
        // copy cell ID if it is valid
        if (CellID_k >= 0)
        {
            OutputMesh.Cell(CellHandle_OutputMesh).SetID(CellID_k);
        }
        // copy Attribute
        OutputMesh.Cell(CellHandle_OutputMesh).Attribute() = CellAttribute_k;

        // copy Edge ID and Attribute
        auto EdgeHandleList_OutputMesh = OutputMesh.Cell(CellHandle_OutputMesh).GetEdgeHandleList();
        for (int_max n = 0; n < EdgeHandleList_k.GetLength(); ++n)
        {
            // check if the edge has already been added to OutputMesh
            auto it = Map_EdgeIndex_OuputIndex.find(EdgeHandleList_k[n].GetIndex());
            if (it == Map_EdgeIndex_OuputIndex.end())
            {
                auto EdgeID_n = this->Edge(EdgeHandleList_k[n]).GetID();
                const auto& EdgeAttribute_n = this->Edge(EdgeHandleList_k[n]).Attribute();

                if (EdgeID_n >= 0)
                {
                    OutputMesh.Edge(EdgeHandleList_OutputMesh[n]).SetID(EdgeID_n);
                }
                OutputMesh.Edge(EdgeHandleList_OutputMesh[n]).Attribute() = EdgeAttribute_n;
            }
            else
            {
                // add edge to map
                Map_EdgeIndex_OuputIndex[EdgeHandleList_k[n].GetIndex()] = EdgeHandleList_OutputMesh[n].GetIndex();
            }
        }

        // copy DirectedEdge ID and Attribute
        auto DirectedEdgeHandleList_OutputMesh = OutputMesh.Cell(CellHandle_OutputMesh).GetDirectedEdgeHandleList();
        for (int_max n = 0; n < DirectedEdgeHandleList_k.GetLength(); ++n)
        {
            auto DirectedEdgeID_n = this->DirectedEdge(DirectedEdgeHandleList_k[n]).GetID();
            const auto& DirectedEdgeAttribute_n = this->DirectedEdge(DirectedEdgeHandleList_k[n]).Attribute();

            if (DirectedEdgeID_n >= 0)
            {
                OutputMesh.DirectedEdge(DirectedEdgeHandleList_OutputMesh[n]).SetID(DirectedEdgeID_n);
            }
            OutputMesh.DirectedEdge(DirectedEdgeHandleList_OutputMesh[n]).Attribute() = DirectedEdgeAttribute_n;
        }
    }

    return OutputMesh;
}


template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType> 
MembraneMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const
{
    DenseVector<Handle_Of_Cell_Of_MembraneMesh> CellHandleList;
    for (int_max k = 0; k < CellIDList.GetLength(); ++k)
    {
        CellHandleList[k] = this->GetCellHandle(CellIDList[k]);
    }
    return this->GetSubMeshByCell(CellHandleList);
}


// Change Topology ----------------------------------------------------------------------------------------

template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::DilatePointToCell(Handle_Of_Point_Of_MembraneMesh PointHandle)
{
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::DilatePointToCell(int_max PointID)
{
    auto PointHandle = this->GetPointHandleByID(PointID);
    return this->DilatePointToCell(PointHandle);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkEdgeToPoint(Handle_Of_Edge_Of_MembraneMesh EdgeHandle)
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;
    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        MDK_Error("Invalid EdgeHandle @ MembraneMesh::ShrinkEdgeToPoint(...)")
        PointHandle.SetToInvalid();
        return PointHandle;
    }

    auto EdgeIndex = EdgeHandle.GetIndex();

    DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex0, DirectedEdgeIndex1;
    DirectedEdgeIndex0.EdgeIndex = EdgeIndex;
    DirectedEdgeIndex0.RelativeIndex = 0;
    DirectedEdgeIndex1.EdgeIndex = EdgeIndex;
    DirectedEdgeIndex1.RelativeIndex = 1;

    // get AdjacentCellIndexList
    auto AdjacentCellIndexList = m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetAdjacentCellIndexList();
    int_max CellIndex0 = -1;
    int_max CellIndex1 = -1;    
    if (AdjacentCellIndexList.GetLength() == 2)
    {
        CellIndex0 = AdjacentCellIndexList[0];
        CellIndex1 = AdjacentCellIndexList[1];
    }
    else if (AdjacentCellIndexList.GetLength() == 1)
    {
        CellIndex0 = AdjacentCellIndexList[0];
    }

    for (int_max k = 0; k < AdjacentCellIndexList.GetLength(); ++k)
    {
        if (m_MeshData->CellList[AdjacentCellIndexList[k]].GetEdgeNumber() <= 3)
        {
            MDK_Error("EdgeNumber <= 3 in AdjacentCell @ MembraneMesh::ShrinkEdgeToPoint(...)")
            PointHandle.SetToInvalid();
            return PointHandle;
        }
    }

    // get the position of the middle point
    int_max PointIndex0, PointIndex1;
    m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetPointIndexList(PointIndex0, PointIndex1);

    DenseVector<ScalarType, 3> MiddlePosition;
	MiddlePosition = m_MeshData->PointList[PointIndex0].GetPosition() + m_MeshData->PointList[PointIndex1].GetPosition();
	MiddlePosition /= ScalarType(2);

    // add the middle point
	auto MiddlePointHandle = this->AddPoint(MiddlePosition);
	auto MiddlePointIndex = MiddlePointHandle.GetIndex();

    // update adjacency information

    //----------
    const auto& AdjacentPointIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentPointIndexList();
    const auto& AdjacentEdgeIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
    const auto& OutgoingDirectedEdgeIndexList0 = m_MeshData->PointList[PointIndex0].OutgoingDirectedEdgeIndexList();
    const auto& IncomingDirectedEdgeIndexList0 = m_MeshData->PointList[PointIndex0].IncomingDirectedEdgeIndexList();
    const auto& AdjacentCellIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentCellIndexList();
    //----------
    const auto& AdjacentPointIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList();
    const auto& AdjacentEdgeIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList();
    const auto& OutgoingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].OutgoingDirectedEdgeIndexList();
    const auto& IncomingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].IncomingDirectedEdgeIndexList();
    const auto& AdjacentCellIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentCellIndexList();
    //--------------------------------------------------------------------------------------------------------
	auto& AdjacentPointIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentPointIndexList();
	auto& AdjacentEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentEdgeIndexList();
	auto& OutgoingDirectedEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].OutgoingDirectedEdgeIndexList();
	auto& IncomingDirectedEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].IncomingDirectedEdgeIndexList();
	auto& AdjacentCellIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentCellIndexList();

    AdjacentPointIndexList = {&AdjacentPointIndexList0, &AdjacentPointIndexList1};    
    {
        auto tempList = AdjacentPointIndexList.Find([&](int_max Index){return Index == PointIndex0 || Index == PointIndex1; });
        AdjacentPointIndexList.Delete(tempList);
    }

    AdjacentEdgeIndexList = { &AdjacentEdgeIndexList0, &AdjacentEdgeIndexList1 };
    {
        auto tempList = AdjacentEdgeIndexList.Find([&](int_max Index){return Index == EdgeIndex; });
        AdjacentEdgeIndexList.Delete(tempList);
    }   

    OutgoingDirectedEdgeIndexList = { &OutgoingDirectedEdgeIndexList0, &OutgoingDirectedEdgeIndexList1 };
    {
        auto tempList = OutgoingDirectedEdgeIndexList.Find([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                            {return Index == DirectedEdgeIndex0 || Index == DirectedEdgeIndex1; });
        OutgoingDirectedEdgeIndexList.Delete(tempList);
    }

    IncomingDirectedEdgeIndexList = { &IncomingDirectedEdgeIndexList0, &IncomingDirectedEdgeIndexList1 };
    {
        auto tempList = IncomingDirectedEdgeIndexList.Find([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                            {return Index == DirectedEdgeIndex0 || Index == DirectedEdgeIndex1; });
        IncomingDirectedEdgeIndexList.Delete(tempList);
    }

    AdjacentCellIndexList = { &AdjacentCellIndexList0, &AdjacentCellIndexList1 };
    {
        auto tempList = AdjacentCellIndexList.FindUnique();
		AdjacentCellIndexList = AdjacentCellIndexList.GetSubSet(tempList);
    }

    //------------------------------------------------------------
    for (int_max k = 0; k < AdjacentPointIndexList.GetLength(); ++k)
    {
        auto& AdjacentPointIndexList_k = m_MeshData->PointList[AdjacentPointIndexList[k]].AdjacentPointIndexList();

        auto tempList = AdjacentPointIndexList_k.Find([&](int_max Index) {return Index == PointIndex0 || Index == PointIndex1; });
        AdjacentPointIndexList_k.Delete(tempList);

		AdjacentPointIndexList_k.Append(MiddlePointIndex);
    }   
    //-------------------------------------------------------------
    for (int_max k = 0; k < AdjacentEdgeIndexList.GetLength(); ++k)
    {
        int_max tempPointIndex0, tempPointIndex1;
        m_MeshData->EdgeList[AdjacentEdgeIndexList[k]].GetPointIndexList(tempPointIndex0, tempPointIndex1);
        if (tempPointIndex0 == PointIndex0 || tempPointIndex0 == PointIndex1)
        {
			m_MeshData->EdgeList[AdjacentEdgeIndexList[k]].SetPointIndexList(MiddlePointIndex, tempPointIndex1);
        }
        else if (tempPointIndex1 == PointIndex0 || tempPointIndex1 == PointIndex1)
        {
			m_MeshData->EdgeList[AdjacentEdgeIndexList[k]].SetPointIndexList(tempPointIndex0, MiddlePointIndex);
        }
    }
    //---------------------------------------------------------------------
    for (int_max k = 0; k < OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = OutgoingDirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = OutgoingDirectedEdgeIndexList[k].RelativeIndex;
		auto& tempDirectedEdge = m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[tempRelativeIndex];

		tempDirectedEdge.SetStartPointIndex(MiddlePointIndex);
    }
    //----------------------------------------------------------------------
    for (int_max k = 0; k < IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto tempEdgeIndex = IncomingDirectedEdgeIndexList[k].EdgeIndex;
        auto tempRelativeIndex = IncomingDirectedEdgeIndexList[k].RelativeIndex;
		auto& tempDirectedEdge = m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[tempRelativeIndex];

		tempDirectedEdge.SetEndPointIndex(MiddlePointIndex);
    }
    //---------------------------------------------------------------
    for (int_max k = 0; k < AdjacentCellIndexList.GetLength(); ++k)
    {
        auto& DirectedEdgeIndexList_k = m_MeshData->CellList[AdjacentCellIndexList[k]].DirectedEdgeIndexList();
        auto tempList = DirectedEdgeIndexList_k.Find([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                     {return Index == DirectedEdgeIndex0|| Index == DirectedEdgeIndex1;});
        DirectedEdgeIndexList_k.Delete(tempList);

        for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
        {
            auto tempEdgeIndex = DirectedEdgeIndexList_k[n].EdgeIndex;
            auto tempRelativeIndex = DirectedEdgeIndexList_k[n].RelativeIndex;
			auto& tempDirectedEdge = m_MeshData->EdgeList[tempEdgeIndex].DirectedEdgeList()[tempRelativeIndex];

            auto tempDirectedEdgeIndex_next = tempDirectedEdge.GetNextDirectedEdgeIndex();
            auto tempEdgeIndex_next = tempDirectedEdgeIndex_next.EdgeIndex;
            auto tempRelativeIndex_next = tempDirectedEdgeIndex_next.RelativeIndex;
			const auto& tempDirectedEdge_next = m_MeshData->EdgeList[tempEdgeIndex_next].DirectedEdgeList()[tempRelativeIndex];

            auto tempDirectedEdgeIndex_prev = tempDirectedEdge.GetPreviousDirectedEdgeIndex();
            auto tempEdgeIndex_prev = tempDirectedEdgeIndex_prev.EdgeIndex;
            auto tempRelativeIndex_prev = tempDirectedEdgeIndex_prev.RelativeIndex;
			const auto& tempDirectedEdge_prev = m_MeshData->EdgeList[tempEdgeIndex_prev].DirectedEdgeList()[tempRelativeIndex_prev];

            if (tempEdgeIndex_next == EdgeIndex)
            {
                tempDirectedEdge.SetNextDirectedEdgeIndex(tempDirectedEdge_next.GetNextDirectedEdgeIndex());
            }

            if (tempEdgeIndex_prev == EdgeIndex)
            {
                tempDirectedEdge.SetPreviousDirectedEdgeIndex(tempDirectedEdge_prev.GetPreviousDirectedEdgeIndex());
            }
        }
    }

    //delete Edge
    this->InternalFuction_DeleteEdge(EdgeIndex);

    // Delete Point0 and Point1
    this->InternalFuction_DeletePoint(PointIndex0);
    this->InternalFuction_DeletePoint(PointIndex1);

    //---------------------------------------------------------------

    return PointHandle;
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkEdgeToPoint(int_max EdgeID)
{
    auto EdgeHandle = this->GetEdgeHandleByID(EdgeID);
    return this->ShrinkEdgeToPoint(EdgeHandle);
}

/*
template<typename MeshAttributeType>
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentEdge(EdgeHandleType EdgeHandleA, EdgeHandleType EdgeHandleB)
{
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle_new;
    
    if (this->IsValidHandle(EdgeHandleA) == false || this->IsValidHandle(EdgeHandleB) == false)
    {
        EdgeHandle_new.SetToInvalid();
        return EdgeHandle_new;
    }

    auto EdgeIndexA = EdgeHandleA.GetIndex();
    auto EdgeIndexB = EdgeHandleB.GetIndex();

    auto AdjacentCellIndexListA = m_MeshData->EdgeList[EdgeIndexA].GetAdjacentCellIndexList();
    auto AdjacentCellIndexListB = m_MeshData->EdgeList[EdgeIndexB].GetAdjacentCellIndexList();

    if (AdjacentCellIndexListA.GetLength() != AdjacentCellIndexListB.GetLength())
    {
		MDK_Error("AdjacentCellNumber is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
        EdgeHandle_new.SetToInvalid();
        return EdgeHandle_new;
    }

    // get AdjacentCellIndexList
    int_max CellIndex0 = -1;
    int_max CellIndex1 = -1;

    if (AdjacentCellIndexListA.GetLength() == 1)
    {
        if (AdjacentCellIndexListA[0] != AdjacentCellIndexListB[0])
        {
			MDK_Error("AdjacentCellIndex is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
            EdgeHandle_new.SetToInvalid();
            return EdgeHandle_new;
        }

        CellIndex0 = AdjacentCellIndexListA[0];
    }
    else if (AdjacentCellIndexListA.GetLength() == 2)
    {
        bool tempFlag = false;

        if ((AdjacentCellIndexListA[0] == AdjacentCellIndexListB[0] && AdjacentCellIndexListA[1] == AdjacentCellIndexListB[1])
            || (AdjacentCellIndexListA[0] == AdjacentCellIndexListB[1] && AdjacentCellIndexListA[1] == AdjacentCellIndexListB[0]))
        {
            tempFlag = true;
        }

        if (tempFlag == false)
        {
			MDK_Error("AdjacentCellIndex is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
            EdgeHandle.SetToInvalid();
            return EdgeHandle;
        }

        CellIndex0 = AdjacentCellIndexListA[0];
        CellIndex1 = AdjacentCellIndexListA[1];
    }

	// check if AdjacentCell is triangle
	if (m_MeshData->CellList[CellIndex0].GetEdgeNumber() <= 3)
	{
		MDK_Error("EdgeNumber <= 3 in AdjacentCell: CellIndex0 @ MembraneMesh::MergeTwoAdjacentEdge(...)")
		EdgeHandle.SetToInvalid();
		return EdgeHandle;
	}
	if (CellIndex1 >= 0)
	{
		if (m_MeshData->CellList[CellIndex1].GetEdgeNumber() <= 3)
		{
			MDK_Error("EdgeNumber <= 3 in AdjacentCell: CellIndex1 @ MembraneMesh::MergeTwoAdjacentEdge(...)")
			EdgeHandle.SetToInvalid();
			return EdgeHandle;
		}
	}

    // get Point0 and Point1 of the Edge
    int_max PointIndex0 = -1;
    int_max PointIndex1 = -1;
    int_max PointIndex_delete = -1;

    int_max PointIndexListA[2];
    m_MeshData->EdgeList[EdgeIndexA].GetPointIndexList(PointIndexListA);
    int_max PointIndexListB[2];
    m_MeshData->EdgeList[EdgeIndexB].GetPointIndexList(PointIndexListB);

    if (PointIndexListA[0] == PointIndexListB[0])
    {
        PointIndex0 = PointIndexListA[1];
        PointIndex1 = PointIndexListB[1];
        PointIndex_delete = PointIndexListA[0];
    }
    else if (PointIndexListA[1] == PointIndexListB[1])
    {
        PointIndex0 = PointIndexListA[0];
        PointIndex1 = PointIndexListB[0];
        PointIndex_delete = PointIndexListA[1];
    }
    else if (PointIndexListA[0] == PointIndexListB[1])
    {
        PointIndex0 = PointIndexListA[1];
        PointIndex1 = PointIndexListB[0];
        PointIndex_delete = PointIndexListA[0];
    }
    else if (PointIndexListA[1] == PointIndexListB[0])
    {
        PointIndex0 = PointIndexListA[0];
        PointIndex1 = PointIndexListB[1];
        PointIndex_delete = PointIndexListA[1];
    }

    Handle_Of_Point_Of_MembraneMesh PointHandle0, PointHandle1;
    PointHandle0.SetIndex(PointIndex0);
    PointHandle1.SetIndex(PointIndex1);

    // add Edge
    EdgeHandle_new = this->AddEdge(PointHandle0, PointHandle1);
    auto EdgeIndex_new = EdgeHandle_new.GetIndex();

    DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex0_new, DirectedEdgeIndex1_new;
    DirectedEdgeIndex0_new.EdgeIndex = EdgeIndex;
    DirectedEdgeIndex0_new.RelativeIndex = 0;
    DirectedEdgeIndex1_new.EdgeIndex = EdgeIndex;
    DirectedEdgeIndex1_new.RelativeIndex = 1;
   
    // update adjacency information
	for (int_max k = 0; k < m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgeList().GetLength(); ++k)
	{
		auto& DirectedEdge_k = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgeList()[k];
		if (DirectedEdge_k.IsValid() == true)
		{
			DirectedEdge_k.SetCellIndex(CellIndex0);
		}
		DirectedEdge0_new.SetCellIndex(CellIndex0);
	}
    auto& DirectedEdge0_new = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgePair()[0];
    DirectedEdge0_new.SetCellIndex(CellIndex0);

    auto& DirectedEdge1_new = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgePair()[1];
    DirectedEdge1_new.SetCellIndex(CellIndex1);

    //----------
    auto& AdjacentPointIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentPointIndexList();
    {
        auto tempIndex = AdjacentPointIndexList0.Match([&](int_max Index) {return Index == PointIndex_delete; });
        AdjacentPointIndexList0.Delete(tempIndex);
    }

    auto& AdjacentEdgeIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
    {
        auto tempIndex = AdjacentEdgeIndexList0.Match([&](int_max Index) {return Index == EdgeIndexA || Index == EdgeIndexB; });
        AdjacentEdgeIndexList0.Delete(tempIndex);
        AdjacentEdgeIndexList0.Append(EdgeIndex_new);
    }

    auto& OutgoingDirectedEdgeIndexList0 = m_MeshData->PointList[PointIndex0].OutgoingDirectedEdgeIndexList();
    {
        auto tempIndex = OutgoingDirectedEdgeIndexList0.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                              {return Index.EdgeIndex == EdgeIndexA || Index.EdgeIndex == EdgeIndexB; });
        auto tempDirectedEdgeIndex = OutgoingDirectedEdgeIndexList0[tempIndex];
        OutgoingDirectedEdgeIndexList0.Delete(tempIndex);

		const auto& tempDirectedEdge = m_MeshData->EdgeList[tempDirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[tempDirectedEdgeIndex.RelativeIndex];
        if (tempDirectedEdge.GetCellIndex() >= 0)
        {
            if (tempDirectedEdge.GetCellIndex() == CellIndex0)
            {
                OutgoingDirectedEdgeIndexList0.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetCellIndex() == CellIndex1)
            {
                OutgoingDirectedEdgeIndexList0.Append(DirectedEdgeIndex1_new);
            }
        }
    }

    auto& IncomingDirectedEdgeIndexList0 = m_MeshData->PointList[PointIndex0].IncomingDirectedEdgeIndexList();
    {
        auto tempIndex = IncomingDirectedEdgeIndexList0.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                              {return Index.EdgeIndex == EdgeIndexA || Index.EdgeIndex == EdgeIndexB; });
        auto tempDirectedEdgeIndex = IncomingDirectedEdgeIndexList0[tempIndex];
        IncomingDirectedEdgeIndexList0.Delete(tempIndex);

		const auto& tempDirectedEdge = m_MeshData->EdgeList[tempDirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[tempDirectedEdgeIndex.RelativeIndex];
        if (tempDirectedEdge.GetCellIndex() >= 0)
        {
            if (tempDirectedEdge.GetCellIndex() == CellIndex0)
            {
                IncomingDirectedEdgeIndexList0.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetCellIndex() == CellIndex1)
            {
                IncomingDirectedEdgeIndexList0.Append(DirectedEdgeIndex1_new);
            }
        }
    }
    
    //-------------------------------------------------------------------------------------------------------

    auto& AdjacentPointIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList();
    {
        auto tempIndex = AdjacentPointIndexList1.Match([&](int_max Index) {return Index == PointIndex_delete; });
        AdjacentPointIndexList1.Delete(tempIndex);
    }

    auto& AdjacentEdgeIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList();
    {
        auto tempIndex = AdjacentEdgeIndexList1.Match([&](int_max Index) {return Index == EdgeIndexA || Index == EdgeIndexB; });
        AdjacentEdgeIndexList1.Delete(tempIndex);
        AdjacentEdgeIndexList1.Append(EdgeIndex_new);
    }

    auto& OutgoingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].OutgoingDirectedEdgeIndexList();
    {
        auto tempIndex = OutgoingDirectedEdgeIndexList1.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                              {return Index.EdgeIndex == EdgeIndexA || Index.EdgeIndex == EdgeIndexB; });
        auto tempDirectedEdgeIndex = OutgoingDirectedEdgeIndexList1[tempIndex];
        OutgoingDirectedEdgeIndexList1.Delete(tempIndex);

		const auto& tempDirectedEdge = m_MeshData->EdgeList[tempDirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[tempDirectedEdgeIndex.RelativeIndex];
        if (tempDirectedEdge.GetCellIndex() >= 0)
        {
            if (tempDirectedEdge.GetCellIndex() == CellIndex0)
            {
                OutgoingDirectedEdgeIndexList1.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetCellIndex() == CellIndex1)
            {
                OutgoingDirectedEdgeIndexList1.Append(DirectedEdgeIndex1_new);
            }
        }
    }

    auto& IncomingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].IncomingDirectedEdgeIndexList();
    {
        auto tempIndex = IncomingDirectedEdgeIndexList1.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index)
                                                              {return Index.EdgeIndex == EdgeIndexA || Index.EdgeIndex == EdgeIndexB; });
        auto tempDirectedEdgeIndex = IncomingDirectedEdgeIndexList1[tempIndex];
        IncomingDirectedEdgeIndexList1.Delete(tempIndex);

		const auto& tempDirectedEdge = m_MeshData->EdgeList[tempDirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[tempDirectedEdgeIndex.RelativeIndex];
        if (tempDirectedEdge.GetCellIndex() >= 0)
        {
            if (tempDirectedEdge.GetCellIndex() == CellIndex0)
            {
                IncomingDirectedEdgeIndexList1.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetCellIndex() == CellIndex1)
            {
                IncomingDirectedEdgeIndexList1.Append(DirectedEdgeIndex1_new);
            }
        }
    }
    //--------------------------------------------------------------

    for (int_max k = 0; k < AdjacentCellIndexListA.GetLength(); ++k)
    {
        auto AdjacentCellIndex_k = AdjacentCellIndexListA[k];

        auto& DirectedEdgeIndexList_k = m_MeshData->CellList[AdjacentCellIndexListA[k]].DirectedEdgeIndexList();
        
        auto tempIndex_replace = DirectedEdgeIndexList_k.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index) {return Index.EdgeIndex == EdgeIndexA; });        
        if (AdjacentCellIndex_k == CellIndex0)
        {
            DirectedEdgeIndexList_k[tempIndex_replace] = DirectedEdgeIndex0_new;
        }
        else if(AdjacentCellIndex_k == CellIndex1)
        {
            DirectedEdgeIndexList_k[tempIndex_replace] = DirectedEdgeIndex1_new;
        }

        auto tempIndex_delete = DirectedEdgeIndexList_k.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index) {return Index.EdgeIndex == EdgeIndexB; });
        DirectedEdgeIndexList_k.Delete(tempIndex_delete);

        for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
        {
            auto tempEdgeIndex = DirectedEdgeIndexList_k[n].EdgeIndex;
            auto tempRelativeIndex = DirectedEdgeIndexList_k[n].RelativeIndex;
			auto& tempDirectedEdge = m_MeshData->EdgeList[tempEdgeIndex].DirectedEdgeList()[tempRelativeIndex];

            if (tempEdgeIndex == EdgeIndex_new)
            {
                DirectedEdgeIndex_Of_MembraneMesh tempIndex_next;
                if (n < DirectedEdgeIndexList_k.GetLength() - 1)
                {
                    tempIndex_next = DirectedEdgeIndexList_k[n + 1];
                }
                else
                {
                    tempIndex_next = DirectedEdgeIndexList_k[0];
                }
                tempDirectedEdge.SetNextDirectedEdgeIndex(tempIndex_next);

                DirectedEdgeIndex_Of_MembraneMesh tempIndex_prev;
                if (n > 0)
                {
                    tempIndex_prev = DirectedEdgeIndexList_k[n - 1];
                }
                else
                {
                    tempIndex_prev = DirectedEdgeIndexList_k[DirectedEdgeIndexList_k.GetLength()-1];
                }
                tempDirectedEdge.SetPreviousDirectedEdgeIndex(tempIndex_prev);
            }
            else
            {
                auto tempDirectedEdgeIndex_next = tempDirectedEdge.GetNextDirectedEdgeIndex();
                auto tempEdgeIndex_next = tempDirectedEdgeIndex_next.EdgeIndex;
                auto tempRelativeIndex_next = tempDirectedEdgeIndex_next.RelativeIndex;
				const auto& tempDirectedEdge_next = m_MeshData->EdgeList[tempEdgeIndex_next].DirectedEdgeList()[tempRelativeIndex];

                auto tempDirectedEdgeIndex_prev = tempDirectedEdge.GetPreviousDirectedEdgeIndex();
                auto tempEdgeIndex_prev = tempDirectedEdgeIndex_prev.EdgeIndex;
                auto tempRelativeIndex_prev = tempDirectedEdgeIndex_prev.RelativeIndex;
				const auto& tempDirectedEdge_prev = m_MeshData->EdgeList[tempEdgeIndex_prev].DirectedEdgeList()[tempRelativeIndex_prev];

                if (tempEdgeIndex_next == EdgeIndexA || tempEdgeIndex_next == EdgeIndexB)
                {
                    if (AdjacentCellIndex_k == CellIndex0)
                    {
                        tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndex0_new);
                    }
                    else if (AdjacentCellIndex_k == CellIndex1)
                    {
                        tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndex1_new);
                    }
                }

                if (tempEdgeIndex_prev == EdgeIndexA || tempEdgeIndex_prev == EdgeIndexB)
                {
                    if (AdjacentCellIndex_k == CellIndex0)
                    {
                        tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex0_new);
                    }
                    else if (AdjacentCellIndex_k == CellIndex1)
                    {
                        tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex1_new);
                    }
                }
            }
        }
    }
    //---------------------------------------------------------------------------------------------

    this->InternalFuction_DeleteEdge(EdgeIndexA);
    this->InternalFuction_DeleteEdge(EdgeIndexB);

    return EdgeHandle_new;
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentEdge(int_max EdgeIDA, int_max EdgeIDB)
{
    auto EdgeHandleA = this->GetEdgeHandleByID(EdgeIDA);
    auto EdgeHandleB = this->GetEdgeHandleByID(EdgeIDB);
    return this->MergeTwoAdjacentEdge(EdgeHandleA, EdgeHandleB);
}
*/

template<typename MeshAttributeType>
std::pair<Handle_Of_Edge_Of_MembraneMesh, Handle_Of_Edge_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::SplitEdge(PointHandleType PointHandle)
{
	std::pair<EdgeHandleType, EdgeHandleType> EdgeHandlePair;

    if (this->IsValidHandle(PointHandle) == false)
    {
		MDK_Error("Invalid PointHandle @ MembraneMesh::SplitCellByEdge(...)")
		EdgeHandlePair.first.SetToInvalid();
		EdgeHandlePair.second.SetToInvalid();        
		return EdgeHandlePair;
    }

	EdgeHandleType EdgeHandleA, EdgeHandleB;
}


template<typename MeshAttributeType>
std::pair<Handle_Of_Edge_Of_MembraneMesh, Handle_Of_Edge_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::SplitEdge(int_max PointID)
{
    auto PointHandle = this->GetPointHandleByID(PointID);
    return this->SplitEdge(PointHandle);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkCellToPoint(Handle_Of_Cell_Of_MembraneMesh CellHandle)
{

}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkCellToPoint(int_max CellID)
{
	auto CellHandle = this->GetCellHandleByID(CellID);
	return this->ShrinkCellToPoint(CellHandle);
}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentCell(Handle_Of_Cell_Of_MembraneMesh CellHandleA, 
                                                                                     Handle_Of_Cell_Of_MembraneMesh CellHandleB)
{

}


template<typename MeshAttributeType>
Handle_Of_Cell_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentCell(int_max CellIDA, int_max CellIDB)
{

}


template<typename MeshAttributeType>
std::pair<Handle_Of_Cell_Of_MembraneMesh, Handle_Of_Cell_Of_MembraneMesh> 
MembraneMesh<MeshAttributeType>::SplitCellByEdge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle)
{
	std::pair<CellHandleType, CellHandleType> CellHandlePair;

    if (this->IsValidHandle(EdgeHandle) == false)
    {
		MDK_Error("Invalid EdgeHandle @ MembraneMesh::SplitCellByEdge(...)")
		CellHandlePair.first.SetToInvalid();
		CellHandlePair.second.SetToInvalid();
        return CellHandlePair;
    }

}


template<typename MeshAttributeType>
std::pair<Handle_Of_Cell_Of_MembraneMesh, Handle_Of_Cell_Of_MembraneMesh> 
MembraneMesh<MeshAttributeType>::SplitCellByEdge(int_max EdgeID)
{
    auto EdgeHandle = this->GetEdgeHandleByID(EdgeID);
    return this->SplitCellByEdge(EdgeHandle);
}

//----------------------------------- private InternalFunction ---------------------------------------------------------//

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeletePoint(int_max PointIndex)
{
    auto it = m_MeshData->Map_PointID_to_PointIndex.find(m_MeshData->PointList[PointIndex].GetID());
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
        m_MeshData->Map_PointID_to_PointIndex.erase(it);
    }

    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

	m_MeshData->PointList[PointIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteEdge(int_max EdgeIndex)
{
    // Delete DirectedEdge: erase ID and release memory
	for (int_max k = 0; k < m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList().GetLength(); ++k)
	{
		// take reference 
		auto& DirectedEdge_k = m_MeshData->EdgeList[EdgeIndex].DirectedEdgeList()[k];
		if (DirectedEdge_k.IsValid() == true)
		{
			this->InternalFuction_DeleteDirectedEdge(DirectedEdge_k.GetIndex());
		}
	}

    // Delete EdgeID record if the map has it 
    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(m_MeshData->EdgeList[EdgeIndex].GetID());
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
    }

    // Delete Edge: only release memory, not remove from EdgeList
	m_MeshData->EdgeList[EdgeIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteDirectedEdge(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex)
{
	// auto& take reference
	auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex.RelativeIndex];
	if (DirectedEdge_k.IsValid() == true)
	{
		auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdge_k.GetID());
		if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
		{
			m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
		}
		DirectedEdge_k.Clear(MDK_PURE_EMPTY);
	}
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteCell(int_max CellIndex)
{
    // delete CellID record if the map has it 
    auto it = m_MeshData->Map_CellID_to_CellIndex.find(m_MeshData->CellList[CellIndex].GetID());
    if (it != m_MeshData->Map_CellID_to_CellIndex.end())
    {
        m_MeshData->Map_CellID_to_CellIndex.erase(it);
    }

    // Delete Cell : only clear memory, not remove from CellList
	m_MeshData->CellList[CellIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->CellValidityFlagList[CellIndex] = 0;
}

}// namespace mdk

#endif