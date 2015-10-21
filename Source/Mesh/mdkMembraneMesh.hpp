#ifndef mdk_MembraneMesh_hpp
#define mdk_MembraneMesh_hpp

namespace mdk
{

template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType>::MembraneMesh()
{
	this->Recreate();
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

    m_MeshData->FaceList.Clear();
    m_MeshData->FaceValidityFlagList.Clear();

    m_MeshData->Map_PointID_to_PointIndex.clear();
    m_MeshData->Map_EdgeID_to_EdgeIndex.clear();
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.clear();
    m_MeshData->Map_FaceID_to_FaceIndex.clear();

    m_MeshData->Attribute.Clear();
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
	if (m_MeshData)
	{
		m_MeshData.reset();
	}
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Copy(const MembraneMesh<MeshAttributeType>& InputMesh)
{
	//prevent self copy
	if (&m_MeshData == &InputMesh.m_MeshData)
	{
		return;
	}

	if (InputMesh.IsPureEmpty() == true)
    {
		this->Clear();
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
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

    m_MeshData->FaceValidityFlagList = InputMesh.m_MeshData->FaceValidityFlagList;
    m_MeshData->FaceList = InputMesh.m_MeshData->FaceList;
    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
            m_MeshData->FaceList[k].SetParentMesh(*this);
        }
    }

    m_MeshData->Map_PointID_to_PointIndex = InputMesh.m_MeshData->Map_PointID_to_PointIndex;
    m_MeshData->Map_EdgeID_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex;
    m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex;
    m_MeshData->Map_FaceID_to_FaceIndex = InputMesh.m_MeshData->Map_FaceID_to_FaceIndex;

    m_MeshData->Attribute = InputMesh.m_MeshData->Attribute;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Copy(MembraneMesh<MeshAttributeType>&& InputMesh)
{
	//prevent self copy
	if (&m_MeshData == &InputMesh.m_MeshData)
	{
		return;
	}

	if (InputMesh.IsPureEmpty() == true)
	{
		this->Clear();
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
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

	m_MeshData->FaceValidityFlagList = std::move(InputMesh.m_MeshData->FaceValidityFlagList);
	m_MeshData->FaceList = std::move(InputMesh.m_MeshData->FaceList);
	for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
	{
		if (m_MeshData->FaceValidityFlagList[k] == 1)
		{
			m_MeshData->FaceList[k].SetParentMesh(*this);
		}
	}

	m_MeshData->Map_PointID_to_PointIndex = std::move(InputMesh.m_MeshData->Map_PointID_to_PointIndex);
	m_MeshData->Map_EdgeID_to_EdgeIndex = std::move(InputMesh.m_MeshData->Map_EdgeID_to_EdgeIndex);
	m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex = std::move(InputMesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex);
	m_MeshData->Map_FaceID_to_FaceIndex = std::move(InputMesh.m_MeshData->Map_FaceID_to_FaceIndex);

	m_MeshData->Attribute = std::move(InputMesh.m_MeshData->Attribute);
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Share(MembraneMesh& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::ForceShare(const MembraneMesh<MeshAttributeType>& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::Recreate()
{
	m_MeshData = std::make_shared<MembraneMeshData<MeshAttributeType>>();
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline 
bool MembraneMesh<MeshAttributeType>::IsEmpty() const
{
	if (!m_MeshData)
	{
		return true;
	}
	else
	{
		return m_MeshData->PointPositionTable.IsEmpty();
	}
}

template<typename MeshAttributeType>
inline
bool MembraneMesh<MeshAttributeType>::IsPureEmpty() const
{
	return (!m_MeshData);
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetPointCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->PointValidityFlagList.Sum();
	}
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetEdgeCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->EdgeValidityFlagList.Sum();
	}
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetDirectedEdgeCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
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
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetFaceCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->FaceValidityFlagList.Sum();
	}
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetMaxValueOfPointIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->PointPositionTable.GetColCount();
	}
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetMaxValueOfEdgeIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->EdgeList.GetLength();
	}
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetMaxValueOfFaceIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	else
	{
		return m_MeshData->FaceList.GetLength();
	}
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
	if (this->IsEmpty() == true)
	{
		PositionMatrix.Clear();
		return;
	}

	auto MaxCount = m_MeshData->PointPositionTable.GetColCount();
	PositionMatrix.FastResize(3, this->GetPointCount());
	int_max Counter = 0;
	for (int_max k = 0; k < MaxCount; ++k)
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
SetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), { x, y, z });
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::SetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, const ScalarType Position[3])
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), Position);
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::
SetPointPosition(int_max PointID, ScalarType x, ScalarType y, ScalarType z)
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->SetPointPosition(PointHandle, x, y, z);
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::SetPointPosition(int_max PointID, const ScalarType Position[3])
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
GetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointPosition(Handle_Of_Point_Of_MembraneMesh PointHandle, ScalarType Position[3]) const
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
SetPointPosition(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix)
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
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColCount())
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
SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<ScalarType>& PointPositionMatrix)
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
GetPointPosition(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList, DenseMatrix<ScalarType>& PointPositionMatrix) const
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
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColCount())
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
GetPointPosition(const DenseVector<int_max>& PointIDList, DenseMatrix<ScalarType>& PointPositionMatrix) const
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
Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(Handle_Of_Face_Of_MembraneMesh FaceHandle)
{
    return m_MeshData->FaceList[FaceHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(Handle_Of_Face_Of_MembraneMesh FaceHandle) const
{
    return m_MeshData->FaceList[FaceHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(int_max FaceID)
{
	auto FaceHandle = this->GetFaceHandleByID(FaceID);
	return this->Face(FaceHandle)
}

template<typename MeshAttributeType>
inline
const Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(int_max FaceID) const
{
	auto FaceHandle = this->GetFaceHandleByID(FaceID);
	return this->Face(FaceHandle)
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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(Handle_Of_Face_Of_MembraneMesh FaceHandle) const
{
    auto FaceIndex = FaceHandle.GetIndex();
    if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceValidityFlagList.GetLength())
    {
        return false;
    }
    else
    {
        return (m_MeshData->FaceValidityFlagList[FaceIndex] == 1);
    }

    // slow
    //if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceList.GetLength())
    //{
    //    return false;
    //}
    //else
    //{
    //    return m_MeshData->FaceList[FaceIndex].IsValid();
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
    OutputHandleList.SetCapacity(m_MeshData->PointList.GetLength());
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
    OutputHandleList.SetCapacity(m_MeshData->EdgeList.GetLength());
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
    OutputHandleList.SetCapacity(2*m_MeshData->EdgeList.GetLength());    
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
DenseVector<Handle_Of_Face_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetFaceHandleList() const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> OutputHandleList;
    this->GetFaceHandleList(OutputHandleList);
    return OutputHandleList;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(m_MeshData->FaceList.GetLength());
    Handle_Of_Face_Of_MembraneMesh FaceHandle;
    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
            FaceHandle.SetIndex(k);
            OutputHandleList.Append(FaceHandle);
        }
    }
}

//----------- get PointHandle by Position, ID, ----------------------------------------------//

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(const DenseVector<ScalarType, 3>& Position, ScalarType DistanceThreshold) const
{
    return this->GetPointHandleByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(const ScalarType Position[3], ScalarType DistanceThreshold) const
{
    return this->GetPointHandleByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
GetPointHandleByPosition(ScalarType x, ScalarType y, ScalarType z, ScalarType DistanceThreshold) const
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

    for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColCount(); ++k)
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

//----------- get FaceHandle by ID, PointHandleList, PointIDList, EdgeHandleList, or EdgeIDList ----------//

template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetFaceHandleByID(int_max FaceID) const
{
    Handle_Of_Face_Of_MembraneMesh FaceHandle;

    if (FaceID < 0)
    {
        //MDK_Warning("Invalid FaceID (< 0) @ MembraneMesh::GetFaceHandle(...)")
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }

    auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
    if (it != m_MeshData->Map_FaceID_to_FaceIndex.end())
    {
        FaceHandle.SetIndex(it->second);
    }
    else
    {
        //MDK_Warning("Invalid FaceID @ MembraneMesh::GetFaceHandle(...)")
        FaceHandle.SetToInvalid();
    }

    return FaceHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetFaceHandleByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList) const
{
    Handle_Of_Face_Of_MembraneMesh FaceHandle;

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            //MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetFaceHandleByEdge(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

    DenseVector<int_max> FaceIndexList;
    FaceIndexList.Resize(EdgeHandleList.GetLength());
    FaceIndexList.Fill(-1);

    for (int_max k = 0; k < EdgeHandleList.GetLength() - 1; ++k)
    {
        auto AdjacentFaceIndexList_a = m_MeshData->EdgeList[EdgeHandleList[k].GetIndex()].GetAdjacentFaceIndexList();
        auto AdjacentFaceIndexList_b = m_MeshData->EdgeList[EdgeHandleList[k + 1].GetIndex()].GetAdjacentFaceIndexList();

        int_max Counter = 0;

        for (int_max n = 0; n < AdjacentFaceIndexList_a.GetLength(); ++n)
        {            
            for (int_max m = 0; m < AdjacentFaceIndexList_b.GetLength(); ++m)
            {
                if (AdjacentFaceIndexList_a[n] == AdjacentFaceIndexList_b[m])
                {
                    FaceIndexList[k] = AdjacentFaceIndexList_a[n];
                    Counter += 1;
                }
            }
        }

        if (Counter > 1)
        {
            MDK_Warning("EdgeHandleList is invalid : more than one copy of a handle @ MembraneMesh::GetFaceHandle(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

    auto tempList = FaceIndexList.FindUnique();
    if (tempList.GetLength() != 1)
    {
        //MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetFaceHandleByEdge(...)")
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }
    else
    {
        FaceHandle.SetIndex(FaceIndexList[tempList[0]]);
        return FaceHandle;
    }
}

template<typename MeshAttributeType>
inline
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetFaceHandleByEdge(const DenseVector<int_max>& EdgeIDList) const
{
    if (EdgeIDList.IsEmpty() == true)
    {
        Handle_Of_Face_Of_MembraneMesh FaceHandle;
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }

    DenseVector<Handle_Of_Edge_Of_MembraneMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->GetFaceHandleByEdge(EdgeHandleList);
}


template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh 
MembraneMesh<MeshAttributeType>::GetFaceHandleByPoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList) const
{
    Handle_Of_Face_Of_MembraneMesh FaceHandle;

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            //MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetFaceHandleByPoint(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

    DenseVector<int_max> FaceIndexList;
    FaceIndexList.Resize(PointHandleList.GetLength());
    FaceIndexList.Fill(-1);

    for (int_max k = 0; k < PointHandleList.GetLength() - 1; ++k)
    {
        const auto& AdjacentFaceIndexList_a = m_MeshData->PointList[PointHandleList[k].GetIndex()].AdjacentFaceIndexList();
        const auto& AdjacentFaceIndexList_b = m_MeshData->PointList[PointHandleList[k + 1].GetIndex()].AdjacentFaceIndexList();

        int_max Counter = 0;

        for (int_max n = 0; n < AdjacentFaceIndexList_a.GetLength(); ++n)
        {
            for (int_max m = 0; m < AdjacentFaceIndexList_b.GetLength(); ++m)
            {
                if (AdjacentFaceIndexList_a[n] == AdjacentFaceIndexList_b[m])
                {
                    FaceIndexList[k] = AdjacentFaceIndexList_a[n];
                    Counter += 1;
                }
            }
        }

        if (Counter > 1)
        {
            MDK_Warning("PointHandleList is invalid : more than one copy of a handle @ MembraneMesh::GetFaceHandleByPoint(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

    auto tempList = FaceIndexList.FindUnique();
    if (tempList.GetLength() != 1)
    {
        //MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetFaceHandleByPoint(...)")
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }
    else
    {
        FaceHandle.SetIndex(FaceIndexList[tempList[0]]);
        return FaceHandle;
    }
}

template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh
MembraneMesh<MeshAttributeType>::GetFaceHandleByPoint(const DenseVector<int_max>& PointIDList) const
{
    if (PointIDList.IsEmpty() == true)
    {
        Handle_Of_Face_Of_MembraneMesh FaceHandle;
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }

    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);
    }

    return this->GetFaceHandleByPoint(PointHandleList);
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
bool MembraneMesh<MeshAttributeType>::IsValidFaceID(int_max FaceID) const
{
    if (FaceID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
    return (it != m_MeshData->Map_FaceID_to_FaceIndex.end());
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
    OutputIDList.SetCapacity(int_max(m_MeshData->Map_PointID_to_PointIndex.size()));
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
    OutputIDList.SetCapacity(int_max(m_MeshData->Map_EdgeID_to_EdgeIndex.size()));
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
    OutputIDList.SetCapacity(int_max(m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.size()));
    for (auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end(); ++it)
    {
        OutputIDList.Append(it->first);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetFaceIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetFaceIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetFaceIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
    OutputIDList.SetCapacity(int_max(m_MeshData->Map_FaceID_to_FaceIndex.size()));
    for (auto it = m_MeshData->Map_FaceID_to_FaceIndex.begin(); it != m_MeshData->Map_FaceID_to_FaceIndex.end(); ++it)
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
int_max MembraneMesh<MeshAttributeType>::GetPointIDByPosition(ScalarType Position[3]) const
{
    auto PointHandle = this->GetPointHandleByPosition(Position);
    return this->GetPointID(PointHandle);
}

template<typename MeshAttributeType>
inline int_max MembraneMesh<MeshAttributeType>::
GetPointIDByPosition(ScalarType x, ScalarType y, ScalarType z) const
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

//----------- get FaceID by FaceHandle, EdgeHandleList, EdgeIDList, PointHandleList, PointIDList --------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetFaceIDByHandle(Handle_Of_Face_Of_MembraneMesh FaceHandle) const
{
    if (this->IsValidHandle(FaceHandle) == true)
    {
        return m_MeshData->FaceList[FaceHandle.GetIndex()].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetFaceIDByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList) const
{
    auto FaceHandle = this->GetFaceHandleByEdge(EdgeHandleList);
    return this->GetFaceIDByHandle(FaceHandle);
}

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetFaceIDByEdge(const DenseVector<int_max>& EdgeIDList) const
{
    DenseVector<EdgeHandleType> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->GetFaceIDByEdge(EdgeHandleList);
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
Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfFace()
{
    Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfFace() const
{
    Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> it(*this);
    return it;
}

//------------ SetCapacity, ReleaseUnusedCapacity -------------------------------------//
template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount)
{
	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
	}

	m_MeshData->PointPositionTable.SetCapacity(3 * PointCount);
	m_MeshData->PointList.SetCapacity(PointCount);
	m_MeshData->PointValidityFlagList.SetCapacity(PointCount);
	m_MeshData->EdgeList.SetCapacity(EdgeCount);
	m_MeshData->EdgeValidityFlagList.SetCapacity(EdgeCount);
	m_MeshData->FaceList.SetCapacity(FaceCount);
	m_MeshData->FaceValidityFlagList.SetCapacity(FaceCount);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::ReleaseUnusedCapacity()
{
	if (this->IsPureEmpty() == true)
	{
		return;
	}

	m_MeshData->PointPositionTable->ReleaseUnusedCapacity();
	m_MeshData->PointList->ReleaseUnusedCapacity();
	m_MeshData->PointValidityFlagList->ReleaseUnusedCapacity();
	m_MeshData->EdgeList->ReleaseUnusedCapacity();
	m_MeshData->EdgeValidityFlagList->ReleaseUnusedCapacity();
	m_MeshData->FaceList->ReleaseUnusedCapacity();
	m_MeshData->FaceValidityFlagList->ReleaseUnusedCapacity();
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
        if (Position.GetElementCount() != 3)
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
    auto PointIndex = m_MeshData->PointPositionTable.GetColCount() - 1;

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
        if (PointSet.GetElementCount() != 3)
        {
            MDK_Error("PointSet is a vector but length != 3 @ MembraneMesh::AddPointSet(...)")
            return PointHandleList;
        }

        PointHandleList.Resize(1);
        PointHandleList[0] = this->AddPoint(PointSet.GetPointer());
        return PointHandleList;
    }

    if (PointSet.GetRowCount() != 3)
    {
        MDK_Error("PointSet is a matrix but RowCount != 3 @ MembraneMesh::AddPointSet(...)")
        return PointHandleList;
    }

    PointHandleList.Resize(PointSet.GetColCount());
    for (int_max k = 0; k < PointSet.GetColCount(); ++k)
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
		if (PointSet.GetElementCount() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ MembraneMesh::AddPointSet(...)")
			return PointHandleList;
		}

		PointHandleList.Resize(1);
		PointHandleList[0] = this->AddPoint(PointSet.GetPointer());
		return PointHandleList;
	}

	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("PointSet is a matrix but RowCount != 3 @ MembraneMesh::AddPointSet(...)")
		return PointHandleList;
	}

	if (this->IsEmpty() == true)
	{
		m_MeshData->PointPositionTable = std::move(PointSet);
		PointHandleList.Resize(m_MeshData->PointPositionTable.GetColCount());
		for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColCount(); ++k)
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
		PointHandleList.Resize(PointSet.GetColCount());
		for (int_max k = 0; k < PointSet.GetColCount(); ++k)
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
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<Handle_Of_Edge_Of_MembraneMesh>& EdgeHandleList)
{
    // Input: the input EdgeIndexList must have been ordered
    // the order determine the direction/sign of the normal vector
    // it also determine the direction of each DirectedEdge
    //
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

    Handle_Of_Face_Of_MembraneMesh FaceHandle;

    if (EdgeHandleList.GetLength() < 2)
    {
        MDK_Error("length of EdgeHandleList < 2 @ MembraneMesh::AddFaceByEdge(...)")
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            MDK_Error("Invalid EdgeHandleList @ MembraneMesh::AddFaceByEdge(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

    auto FaceHandle_temp = this->GetFaceHandleByEdge(EdgeHandleList);
    if (this->IsValidHandle(FaceHandle_temp) == true)
    {
        //MDK_Warning("The cell has been added already @ MembraneMesh::AddFaceByEdge(...)")
        FaceHandle = FaceHandle_temp;
        return FaceHandle;
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
        MDK_Error("EdgeHandleList is invalid : more than one copy of a handle @ MembraneMesh::AddFaceByEdge(...)")
        FaceHandle.SetToInvalid();
        return FaceHandle;
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
            MDK_Error("EdgeIndexList is not ordered or invalid @ MembraneMesh::AddFaceByEdge(...)")
            FaceHandle.SetIndex(-1);
            return FaceHandle;
        }
    }

    // create DirectedEdge  -------------------------------------------------------
    auto FaceIndex = m_MeshData->FaceList.GetLength();

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
        tempDirectedEdge.SetFaceIndex(FaceIndex);
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

    Face_Of_MembraneMesh<MeshAttributeType> Face;
    Face.SetParentMesh(*this);
    Face.SetIndex(FaceIndex);
    Face.DirectedEdgeIndexList() = DirectedEdgeIndexList;

    m_MeshData->FaceList.Append(std::move(Face));
    m_MeshData->FaceValidityFlagList.Append(1);

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
        m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().Append(FaceIndex);
    }

    //-----------------------------
    FaceHandle.SetIndex(FaceIndex);
    return FaceHandle;
}

template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<int_max>& EdgeIDList)
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> EdgeHandleList;
    EdgeHandleList.Resize(EdgeIDList.GetLength());
    for (int_max k = 0; k < EdgeIDList.GetLength(); ++k)
    {
        EdgeHandleList[k] = this->GetEdgeHandle(EdgeIDList[k]);
    }

    return this->AddFaceByEdge(EdgeIDList);    
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList)
{
    if (PointHandleList.GetLength() < 3)
    {        
        MDK_Error("PointHandleList length < 3 @ MembraneMesh::AddFaceByPoint(...)")
        Handle_Of_Face_Of_MembraneMesh FaceHandle;
        FaceHandle.SetToInvalid();
        return FaceHandle;
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

    return this->AddFaceByEdge(EdgeHandleList);
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<int_max>& PointIDList)
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
    PointHandleList.Resize(PointIDList.GetLength());
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		PointHandleList[k] = this->GetPointHandleByID(PointIDList[k]);
    }

    return this->AddFaceByPoint(PointHandleList);
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteFace(Handle_Of_Face_Of_MembraneMesh FaceHandle)
{
    // this function will modify each DirectedEdge of the Face, and modify any information related to the cell
    // FaceHandle and FaceID of the cell become invalid after the cell is deleted

    // check input 
    if (this->IsValidHandle(FaceHandle) == false)
    {
        MDK_Warning("Invalid FaceHandle @ MembraneMesh::DeleteFace(...)")
        return;
    }

    //--------------------------------------------------------------------------------------
    auto FaceIndex = FaceHandle.GetIndex();

    // auto& will get reference
    const auto& DirectedEdgeIndexList = m_MeshData->FaceList[FaceIndex].DirectedEdgeIndexList();
    const auto& PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();

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
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().ExactMatch(FaceIndex);
        m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().Delete(tempIndex);
    }

    // delete DirectedEdge
    for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); k++)
    {
		this->InternalFuction_DeleteDirectedEdge(DirectedEdgeIndexList[k]);
    }

    // delete Face
    this->InternalFuction_DeleteFace(FaceIndex);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteFace(int_max FaceID)
{
    auto FaceHandle = this->GetFaceHandle(FaceID);
    return this->DeleteFace(FaceHandle);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteEdge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle)
{    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        MDK_Warning("Invalid EdgeHandle @ MembraneMesh::DeleteEdge(...)")
        return true;
    }

    // check AdjacentFace: the edge can not be deleted if any adjacent cell exit

    auto EdgeIndex = EdgeHandle.GetIndex();

    auto AdjacentFaceCount = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceCount();
    if (AdjacentFaceCount > 0)
    {
        MDK_Error("AdjacentFaceIndexList is not empty, so this edge can not be deleted @ MembraneMesh::DeleteEdge(...)")
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
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteEdge(int_max EdgeID)
{
    auto EdgeHandle = this->GetEdgeHandle(EdgeID);
    this->DeleteEdge(EdgeHandle);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeletePoint(Handle_Of_Point_Of_MembraneMesh PointHandle)
{
    if (this->IsValidHandle(PointHandle) == false)
    {
        MDK_Warning("Invalid PointHandle @ MembraneMesh::DeletePoint(...)")
        return;
    }

    auto PointIndex = PointHandle.GetIndex();

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ MembraneMesh::DeletePoint(...)")
        return;
    }

    this->InternalFuction_DeletePoint(PointIndex);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeletePoint(const DenseVector<Handle_Of_Point_Of_MembraneMesh>& PointHandleList)
{
    if (PointHandleList.IsEmpty() == true)
    {
        //MDK_Warning("PointHandleList is empty @ MembraneMesh::DeletePoint(...)")
        return;
    }

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
		this->DeletePoint(PointHandleList[k]);
    }
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeletePoint(int_max PointID)
{
	auto PointHandle = this->GetPointHandleByID(PointID);
    this->DeletePoint(PointHandle);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeletePoint(const DenseVector<int_max>& PointIDList)
{
    for (int_max k = 0; k < PointIDList.GetLength(); ++k)
    {
		this->DeletePoint(PointIDList[k]);
    }
}

//----------------- remove deleted item from Point/Edge/DirectedEdgeList/Face list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index and handle may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when InvalidPointHandleCount/ValidPointCount( GetPointCount() ) > 0.5

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::CleanDataStructure()
{
    //-------------------------- clean PolintList and update Map_PointID_to_PointIndex -----------------------------------------------------------//
    DenseVector<int_max> ValidPointIndexList;
    ValidPointIndexList.SetCapacity(m_MeshData->PointList.GetLength());

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
        // remove deleted-item from PointList
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
    ValidEdgeIndexList.SetCapacity(m_MeshData->EdgeList.GetLength());

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

                // modify DirectedEdgeIndex.EdgeIndex in m_MeshData->FaceList
				for (int_max n = 0; n < m_MeshData->EdgeList[k].DirectedEdgeList().GetLength(); ++n)
                {
					if (m_MeshData->EdgeList[k].DirectedEdgeList()[n].IsValid() == true)
					{
						auto FaceIndex_n = m_MeshData->EdgeList[k].DirectedEdgeList()[n].GetFaceIndex();
						if (FaceIndex_n >= 0)
						{
							// auto& : get reference and modify
							auto& DirectedEdgeIndexList_n = m_MeshData->FaceList[FaceIndex_n].DirectedEdgeIndexList();
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

    //------------------ clean FaceList and Update Map_FaceID_to_FaceIndex ---------------------------------------------------------------------//

    DenseVector<int_max> ValidFaceIndexList;
    ValidFaceIndexList.SetCapacity(m_MeshData->FaceList.GetLength());

    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k) // k is FaceIndex
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
            ValidFaceIndexList.Append(k);

            auto FaceIndex_k_new = ValidFaceIndexList.GetLength() - 1;
            if (FaceIndex_k_new != k)// need to modify information related to the cell
            {
                // modify AdjacentFace information in m_MeshData->PointList
                auto PointIndexList_k = m_MeshData->FaceList[k].GetPointIndexList();
                for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
                {
                    // auto& : get reference and modify
                    auto& tempAdjacentFaceIndexList = m_MeshData->PointList[PointIndexList_k[n]].AdjacentFaceIndexList();
                    auto tempIndex = tempAdjacentFaceIndexList.Find([&](int_max Index) {return Index == k; });
                    if (tempIndex.GetLength() != 1)
                    {
                        MDK_Error("Something is wrong about AdjacentFaceIndexList @ MembraneMesh::CleanDataStructure()")
                        return;
                    }
                    tempAdjacentFaceIndexList[tempIndex[0]] = FaceIndex_k_new;
                }

                // modify FaceIndex Information in m_MeshData->EdgeList.DirectedEdgeList() 
                const auto& DirectedEdgeIndexList_k = m_MeshData->FaceList[k].DirectedEdgeIndexList();
                for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
                {
                    auto EdgeIndex_n = DirectedEdgeIndexList_k[n].EdgeIndex;
                    auto RelativeIndex_n = DirectedEdgeIndexList_k[n].RelativeIndex;
					m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeList()[RelativeIndex_n].SetFaceIndex(FaceIndex_k_new);
                }
            }
        }
    }// for each FaceIndex k

    // remove deleted item from FaceList
    if (ValidFaceIndexList.GetLength() != m_MeshData->FaceList.GetLength())
    {
		FaceListType FaceList_new;
        FaceList_new.Resize(ValidFaceIndexList.GetLength());
        for (int_max k = 0; k < ValidFaceIndexList.GetLength(); ++k)
        {
            FaceList_new[k] = std::move(m_MeshData->FaceList[ValidFaceIndexList[k]]);
            FaceList_new[k].SetIndex(k);
        }
        m_MeshData->FaceList = std::move(FaceList_new);

        m_MeshData->FaceValidityFlagList.FastResize(ValidFaceIndexList.GetLength());
        m_MeshData->FaceValidityFlagList.Fill(1);

        // update Map_FaceID_to_FaceIndex
        m_MeshData->Map_FaceID_to_FaceIndex.clear();
        for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
        {
            auto FaceID_k = m_MeshData->FaceList[k].GetID();
            if (FaceID_k >= 0) // ID is invalid if < 0 
            {
				m_MeshData->Map_FaceID_to_FaceIndex[FaceID_k] = k;
            }
        }
    }
    //ValidFaceIndexList.Clear();

    //--------------- done clean FaceList and Update Map_FaceID_to_FaceIndex ----------------------------------------------------//

	//--------------- clean DirectedEdgeList and update Map_DirectedEdgeID_to_DirectedEdgeIndex ---------------------------------//

	for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k) // k is EdgeIndex_new
	{
		auto& DirectedEdgeList_old = m_MeshData->EdgeList[k].DirectedEdgeList();

		DenseVector<int_max> ValidDirectedEdgeRelativeIndexList;
		ValidDirectedEdgeRelativeIndexList.SetCapacity(DirectedEdgeList_old.GetLength());

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
					// modify information in m_MeshData->FaceList
					{
						auto FaceIndex_n = DirectedEdgeList_old[n].GetFaceIndex();
						// auto& take reference
						auto& DirectedEdgeIndexList = m_MeshData->FaceList[FaceIndex_n].DirectedEdgeIndexList();
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
			for (int_max n = 0; n < ValidFaceIndexList.GetLength(); ++n)
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

template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::Check_If_DataStructure_is_Clean() const
{
	auto Count1 = this->GetDeletedPointHandleCount();
	auto Count2 = this->GetDeletedEdgeHandleCount();
	auto Count3 = this->GetDeletedFaceHandleCount();
	return (Count1 + Count2 + Count3 == 0);
}


template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedPointHandleCount() const
{
	auto ValidPointCount = this->GetPointCount();
	auto PointCountOfList = m_MeshData->PointList.GetLength();
	return  PointCountOfList - ValidPointCount;
}


template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedEdgeHandleCount() const
{
	auto ValidEdgeCount = this->GetEdgeCount();
	auto EdgeCountOfList = m_MeshData->EdgeList.GetLength();
	return  EdgeCountOfList - ValidEdgeCount;
}

template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedFaceHandleCount() const
{
	auto ValidFaceCount = this->GetFaceCount();
	auto FaceCountOfList = m_MeshData->FaceList.GetLength();
	return  FaceCountOfList - ValidFaceCount;
}

//-------------------- get a sub mesh by FaceHandleList or FaceIDList -----------------------------------------//

template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType> 
MembraneMesh<MeshAttributeType>::GetSubMeshByFace(const DenseVector<Handle_Of_Face_Of_MembraneMesh>& FaceHandleList) const
{
    MembraneMesh<MeshAttributeType> OutputMesh;
    
    if (FaceHandleList.IsEmpty() == true)
    {
        return OutputMesh;
    }

    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ MembraneMesh::GetSubMeshByFace(...)")
        return OutputMesh;
    }

    for (int_max k = 0; k < FaceHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(FaceHandleList[k]) == false)
        {
            MDK_Error("FaceHandleList is invalid @ MembraneMesh::GetSubMeshByFace(...)")
            return OutputMesh;
        }
    }
    //-----------------------------------------------------------

    // point index on this mesh-> point index on output mesh
    std::unordered_map<int_max, int_max> Map_PointIndex_OuputIndex;        
    // edge index on this mesh-> edge index on output mesh
    std::unordered_map<int_max, int_max> Map_EdgeIndex_OuputIndex;

    // add cell one by one
    for (int_max k = 0; k < FaceHandleList.GetLength(); ++k)
    {
        auto FaceID_k = this->Face(FaceHandleList[k]).GetID();
        const auto& FaceAttribute_k = this->Face(FaceHandleList[k]).Attribute();
        auto PointHandleList_k = this->Face(FaceHandleList[k]).GetPointHandleList();
        auto EdgeHandleList_k = this->Face(FaceHandleList[k]).GetEdgeHandleList();
        auto DirectedEdgeHandleList_k = this->Face(FaceHandleList[k]).GetDirectedEdgeHandleList();

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
        auto FaceHandle_OutputMesh = OutputMesh.AddFaceByPoint(PointHandleList_OutputMesh);
        // copy cell ID if it is valid
        if (FaceID_k >= 0)
        {
            OutputMesh.Face(FaceHandle_OutputMesh).SetID(FaceID_k);
        }
        // copy Attribute
        OutputMesh.Face(FaceHandle_OutputMesh).Attribute() = FaceAttribute_k;

        // copy Edge ID and Attribute
        auto EdgeHandleList_OutputMesh = OutputMesh.Face(FaceHandle_OutputMesh).GetEdgeHandleList();
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
        auto DirectedEdgeHandleList_OutputMesh = OutputMesh.Face(FaceHandle_OutputMesh).GetDirectedEdgeHandleList();
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
MembraneMesh<MeshAttributeType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIDList) const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> FaceHandleList;
    for (int_max k = 0; k < FaceIDList.GetLength(); ++k)
    {
        FaceHandleList[k] = this->GetFaceHandle(FaceIDList[k]);
    }
    return this->GetSubMeshByFace(FaceHandleList);
}


// Change Topology ----------------------------------------------------------------------------------------

template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::DilatePointToFace(Handle_Of_Point_Of_MembraneMesh PointHandle)
{
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::DilatePointToFace(int_max PointID)
{
    auto PointHandle = this->GetPointHandleByID(PointID);
    return this->DilatePointToFace(PointHandle);
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

    // get AdjacentFaceIndexList
    auto AdjacentFaceIndexList = m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetAdjacentFaceIndexList();
    int_max FaceIndex0 = -1;
    int_max FaceIndex1 = -1;    
    if (AdjacentFaceIndexList.GetLength() == 2)
    {
        FaceIndex0 = AdjacentFaceIndexList[0];
        FaceIndex1 = AdjacentFaceIndexList[1];
    }
    else if (AdjacentFaceIndexList.GetLength() == 1)
    {
        FaceIndex0 = AdjacentFaceIndexList[0];
    }

    for (int_max k = 0; k < AdjacentFaceIndexList.GetLength(); ++k)
    {
        if (m_MeshData->FaceList[AdjacentFaceIndexList[k]].GetEdgeCount() <= 3)
        {
            MDK_Error("EdgeCount <= 3 in AdjacentFace @ MembraneMesh::ShrinkEdgeToPoint(...)")
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
    const auto& AdjacentFaceIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentFaceIndexList();
    //----------
    const auto& AdjacentPointIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList();
    const auto& AdjacentEdgeIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList();
    const auto& OutgoingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].OutgoingDirectedEdgeIndexList();
    const auto& IncomingDirectedEdgeIndexList1 = m_MeshData->PointList[PointIndex1].IncomingDirectedEdgeIndexList();
    const auto& AdjacentFaceIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentFaceIndexList();
    //--------------------------------------------------------------------------------------------------------
	auto& AdjacentPointIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentPointIndexList();
	auto& AdjacentEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentEdgeIndexList();
	auto& OutgoingDirectedEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].OutgoingDirectedEdgeIndexList();
	auto& IncomingDirectedEdgeIndexList = m_MeshData->PointList[MiddlePointIndex].IncomingDirectedEdgeIndexList();
	auto& AdjacentFaceIndexList = m_MeshData->PointList[MiddlePointIndex].AdjacentFaceIndexList();

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

    AdjacentFaceIndexList = { &AdjacentFaceIndexList0, &AdjacentFaceIndexList1 };
    {
        auto tempList = AdjacentFaceIndexList.FindUnique();
		AdjacentFaceIndexList = AdjacentFaceIndexList.GetSubSet(tempList);
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
    for (int_max k = 0; k < AdjacentFaceIndexList.GetLength(); ++k)
    {
        auto& DirectedEdgeIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexList[k]].DirectedEdgeIndexList();
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

    auto AdjacentFaceIndexListA = m_MeshData->EdgeList[EdgeIndexA].GetAdjacentFaceIndexList();
    auto AdjacentFaceIndexListB = m_MeshData->EdgeList[EdgeIndexB].GetAdjacentFaceIndexList();

    if (AdjacentFaceIndexListA.GetLength() != AdjacentFaceIndexListB.GetLength())
    {
		MDK_Error("AdjacentFaceCount is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
        EdgeHandle_new.SetToInvalid();
        return EdgeHandle_new;
    }

    // get AdjacentFaceIndexList
    int_max FaceIndex0 = -1;
    int_max FaceIndex1 = -1;

    if (AdjacentFaceIndexListA.GetLength() == 1)
    {
        if (AdjacentFaceIndexListA[0] != AdjacentFaceIndexListB[0])
        {
			MDK_Error("AdjacentFaceIndex is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
            EdgeHandle_new.SetToInvalid();
            return EdgeHandle_new;
        }

        FaceIndex0 = AdjacentFaceIndexListA[0];
    }
    else if (AdjacentFaceIndexListA.GetLength() == 2)
    {
        bool tempFlag = false;

        if ((AdjacentFaceIndexListA[0] == AdjacentFaceIndexListB[0] && AdjacentFaceIndexListA[1] == AdjacentFaceIndexListB[1])
            || (AdjacentFaceIndexListA[0] == AdjacentFaceIndexListB[1] && AdjacentFaceIndexListA[1] == AdjacentFaceIndexListB[0]))
        {
            tempFlag = true;
        }

        if (tempFlag == false)
        {
			MDK_Error("AdjacentFaceIndex is not the same @ MembraneMesh::MergeTwoAdjacentEdge(...)")
            EdgeHandle.SetToInvalid();
            return EdgeHandle;
        }

        FaceIndex0 = AdjacentFaceIndexListA[0];
        FaceIndex1 = AdjacentFaceIndexListA[1];
    }

	// check if AdjacentFace is triangle
	if (m_MeshData->FaceList[FaceIndex0].GetEdgeCount() <= 3)
	{
		MDK_Error("EdgeCount <= 3 in AdjacentFace: FaceIndex0 @ MembraneMesh::MergeTwoAdjacentEdge(...)")
		EdgeHandle.SetToInvalid();
		return EdgeHandle;
	}
	if (FaceIndex1 >= 0)
	{
		if (m_MeshData->FaceList[FaceIndex1].GetEdgeCount() <= 3)
		{
			MDK_Error("EdgeCount <= 3 in AdjacentFace: FaceIndex1 @ MembraneMesh::MergeTwoAdjacentEdge(...)")
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
			DirectedEdge_k.SetFaceIndex(FaceIndex0);
		}
		DirectedEdge0_new.SetFaceIndex(FaceIndex0);
	}
    auto& DirectedEdge0_new = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgePair()[0];
    DirectedEdge0_new.SetFaceIndex(FaceIndex0);

    auto& DirectedEdge1_new = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgePair()[1];
    DirectedEdge1_new.SetFaceIndex(FaceIndex1);

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
        if (tempDirectedEdge.GetFaceIndex() >= 0)
        {
            if (tempDirectedEdge.GetFaceIndex() == FaceIndex0)
            {
                OutgoingDirectedEdgeIndexList0.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetFaceIndex() == FaceIndex1)
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
        if (tempDirectedEdge.GetFaceIndex() >= 0)
        {
            if (tempDirectedEdge.GetFaceIndex() == FaceIndex0)
            {
                IncomingDirectedEdgeIndexList0.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetFaceIndex() == FaceIndex1)
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
        if (tempDirectedEdge.GetFaceIndex() >= 0)
        {
            if (tempDirectedEdge.GetFaceIndex() == FaceIndex0)
            {
                OutgoingDirectedEdgeIndexList1.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetFaceIndex() == FaceIndex1)
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
        if (tempDirectedEdge.GetFaceIndex() >= 0)
        {
            if (tempDirectedEdge.GetFaceIndex() == FaceIndex0)
            {
                IncomingDirectedEdgeIndexList1.Append(DirectedEdgeIndex0_new);
            }
            else if (tempDirectedEdge.GetFaceIndex() == FaceIndex1)
            {
                IncomingDirectedEdgeIndexList1.Append(DirectedEdgeIndex1_new);
            }
        }
    }
    //--------------------------------------------------------------

    for (int_max k = 0; k < AdjacentFaceIndexListA.GetLength(); ++k)
    {
        auto AdjacentFaceIndex_k = AdjacentFaceIndexListA[k];

        auto& DirectedEdgeIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexListA[k]].DirectedEdgeIndexList();
        
        auto tempIndex_replace = DirectedEdgeIndexList_k.Match([&](DirectedEdgeIndex_Of_MembraneMesh Index) {return Index.EdgeIndex == EdgeIndexA; });        
        if (AdjacentFaceIndex_k == FaceIndex0)
        {
            DirectedEdgeIndexList_k[tempIndex_replace] = DirectedEdgeIndex0_new;
        }
        else if(AdjacentFaceIndex_k == FaceIndex1)
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
                    if (AdjacentFaceIndex_k == FaceIndex0)
                    {
                        tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndex0_new);
                    }
                    else if (AdjacentFaceIndex_k == FaceIndex1)
                    {
                        tempDirectedEdge.SetNextDirectedEdgeIndex(DirectedEdgeIndex1_new);
                    }
                }

                if (tempEdgeIndex_prev == EdgeIndexA || tempEdgeIndex_prev == EdgeIndexB)
                {
                    if (AdjacentFaceIndex_k == FaceIndex0)
                    {
                        tempDirectedEdge.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex0_new);
                    }
                    else if (AdjacentFaceIndex_k == FaceIndex1)
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
		MDK_Error("Invalid PointHandle @ MembraneMesh::SplitFaceByEdge(...)")
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
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkFaceToPoint(Handle_Of_Face_Of_MembraneMesh FaceHandle)
{

}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkFaceToPoint(int_max FaceID)
{
	auto FaceHandle = this->GetFaceHandleByID(FaceID);
	return this->ShrinkFaceToPoint(FaceHandle);
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentFace(Handle_Of_Face_Of_MembraneMesh FaceHandleA, 
                                                                                     Handle_Of_Face_Of_MembraneMesh FaceHandleB)
{

}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeTwoAdjacentFace(int_max FaceIDA, int_max FaceIDB)
{

}


template<typename MeshAttributeType>
std::pair<Handle_Of_Face_Of_MembraneMesh, Handle_Of_Face_Of_MembraneMesh> 
MembraneMesh<MeshAttributeType>::SplitFaceByTwoPoint(PointHandleType PointHandleA, PointHandleType PointHandleB)
{
	std::pair<FaceHandleType, FaceHandleType> FaceHandlePair;
     return FaceHandlePair;
}


template<typename MeshAttributeType>
std::pair<Handle_Of_Face_Of_MembraneMesh, Handle_Of_Face_Of_MembraneMesh> 
MembraneMesh<MeshAttributeType>::SplitFaceByTwoPoint(int_max PointIDA, int_max PointIDB)
{
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
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteFace(int_max FaceIndex)
{
    // delete FaceID record if the map has it 
    auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(m_MeshData->FaceList[FaceIndex].GetID());
    if (it != m_MeshData->Map_FaceID_to_FaceIndex.end())
    {
        m_MeshData->Map_FaceID_to_FaceIndex.erase(it);
    }

    // Delete Face : only clear memory, not remove from FaceList
	m_MeshData->FaceList[FaceIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->FaceValidityFlagList[FaceIndex] = 0;
}

}// namespace mdk

#endif