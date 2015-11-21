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
            m_MeshData->FaceList[k].SetParentMesh(*this);//DirectedEdge.SetParentMesh(...) is called inside this function
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
			m_MeshData->FaceList[k].SetParentMesh(*this);//DirectedEdge.SetParentMesh(...) is called inside this function
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
		for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
		{
			if (m_MeshData->FaceValidityFlagList[k] == 1)
			{
				Counter += m_MeshData->FaceList[k].DirectedEdgeList().GetLength();
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
SetPointPosition(PointHandleType PointHandle, ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointHandle.GetIndex(), { x, y, z });
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::SetPointPosition(PointHandleType PointHandle, const ScalarType Position[3])
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
MembraneMesh<MeshAttributeType>::GetPointPosition(PointHandleType PointHandle) const
{
    DenseVector<ScalarType, 3> Position;
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position.GetPointer());
    return Position;
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::
GetPointPosition(PointHandleType PointHandle, ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointHandle.GetIndex(), Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename MeshAttributeType>
inline 
void MembraneMesh<MeshAttributeType>::GetPointPosition(PointHandleType PointHandle, ScalarType Position[3]) const
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
SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix)
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

	DenseVector<PointHandleType> PointHandleList;
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
MembraneMesh<MeshAttributeType>::GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const
{
    DenseMatrix<MeshAttributeType::ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointHandleList, PointPositionMatrix);
    return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
GetPointPosition(const DenseVector<PointHandleType>& PointHandleList, DenseMatrix<ScalarType>& PointPositionMatrix) const
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

	DenseVector<PointHandleType> PointHandleList;
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
Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(PointHandleType PointHandle)
{
    return m_MeshData->PointList[PointHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Point_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Point(PointHandleType PointHandle) const
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
Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(EdgeHandleType EdgeHandle)
{
    return m_MeshData->EdgeList[EdgeHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Edge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Edge(EdgeHandleType EdgeHandle) const
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
DirectedEdge_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle)
{
    auto FaceIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
	return m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& 
MembraneMesh<MeshAttributeType>::DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const
{
    auto FaceIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();
    return m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex];
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
Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(FaceHandleType FaceHandle)
{
    return m_MeshData->FaceList[FaceHandle.GetIndex()];
}

template<typename MeshAttributeType>
inline
const Face_Of_MembraneMesh<MeshAttributeType>& MembraneMesh<MeshAttributeType>::Face(FaceHandleType FaceHandle) const
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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(PointHandleType PointHandle) const
{
    auto PointIndex = PointHandle.GetIndex();
    if (PointIndex < 0 || PointIndex >= m_MeshData->PointValidityFlagList.GetLength())
    {
        return false;
    }

	return (m_MeshData->PointValidityFlagList[PointIndex] == 1);    
    
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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(EdgeHandleType EdgeHandle) const
{
    auto EdgeIndex = EdgeHandle.GetIndex();
    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength())
    {
        return false;
    }

	return (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1);    

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
bool MembraneMesh<MeshAttributeType>::IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const
{
    auto FaceIndex = DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = DirectedEdgeHandle.GetRelativeIndex();

    if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceValidityFlagList.GetLength())
    {
        return false;
    }

	if (RelativeIndex < 0 || RelativeIndex >= m_MeshData->FaceList[FaceIndex].DirectedEdgeList().GetLength())
	{
		return false
	}
	
	return m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex].IsValid();   
}

template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::IsValidHandle(FaceHandleType FaceHandle) const
{
    auto FaceIndex = FaceHandle.GetIndex();
    if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceValidityFlagList.GetLength())
    {
        return false;
    }
    
	return (m_MeshData->FaceValidityFlagList[FaceIndex] == 1);
    
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
	DenseVector<PointHandleType> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(m_MeshData->PointList.GetLength());
	PointHandleType PointHandle;
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
	DenseVector<EdgeHandleType> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(m_MeshData->EdgeList.GetLength());
	EdgeHandleType EdgeHandle;
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
	DenseVector<DirectedEdgeHandleType> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::
GetDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(3*m_MeshData->FaceList.GetLength());    
    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
			for (int_max n = 0; n < m_MeshData->FaceList[k].DirectedEdgeList().GetLength(); ++n)
			{
				DirectedEdgeHandleType DirectedEdgeHandle;
				DirectedEdgeHandle.SetIndex(k, n);
				OutputHandleList.Append(DirectedEdgeHandle);
			}
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::GetFaceHandleList() const
{
	DenseVector<FaceHandleType> OutputHandleList;
    this->GetFaceHandleList(OutputHandleList);
    return OutputHandleList;
}


template<typename MeshAttributeType>
inline
void MembraneMesh<MeshAttributeType>::GetFaceHandleList(DenseVector<FaceHandleType>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(m_MeshData->FaceList.GetLength());
	FaceHandleType FaceHandle;
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
	PointHandleType PointHandle;
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
	PointHandleType PointHandle;

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
	EdgeHandleType EdgeHandle;

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
GetEdgeHandleByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const
{
	EdgeHandleType EdgeHandle;

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
	DirectedEdgeHandleType DirectedEdgeHandle;

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
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::
GetDirectedEdgeHandleListByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const
{
	DenseVector<DirectedEdgeHandleType> DirectedEdgeHandleList;

    if (this->IsValidHandle(PointHandle_start) == false || this->IsValidHandle(PointHandle_end) == false)
    {
		return DirectedEdgeHandleList;
    }
    
    auto PointIndex_start = PointHandle_start.GetIndex();
    auto PointIndex_end = PointHandle_end.GetIndex();

    const auto& OutgoingDirectedEdgeIndexList_s = m_MeshData->PointList[PointIndex_start].OutgoingDirectedEdgeIndexList();
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexList_s.GetLength(); ++k)
    {
		auto tempFaceIndex = OutgoingDirectedEdgeIndexList_s[k].FaceIndex;
		auto tempRelativeIndex = OutgoingDirectedEdgeIndexList_s[k].RelativeIndex;
        auto tempPointIndex_end = m_MeshData->FaceList[tempFaceIndex].DirectedEdgeList()[tempRelativeIndex].GetEndPointIndex();
        if (tempPointIndex_end == PointIndex_end)
        {
			DirectedEdgeHandleType tempHandle;
			tempHandle.SetIndex(tempEdgeIndex, tempRelativeIndex);
			DirectedEdgeHandleList.Append(tempHandle);
        }
    }
  
	return DirectedEdgeHandleList;
}

//----------- get FaceHandle by ID, PointHandleList, PointIDList, EdgeHandleList, or EdgeIDList ----------//

template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetFaceHandleByID(int_max FaceID) const
{
	FaceHandleType FaceHandle;

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
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::GetFaceHandleByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const
{
	FaceHandleType FaceHandle;

    for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(EdgeHandleList[k]) == false)
        {
            //MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetFaceHandleByEdge(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

	//---------------------------------------------------------------------------------------------------
	auto FaceIndexList = m_MeshData->EdgeList[EdgeHandleList[0].GetIndex()].GetAdjacentFaceIndexList();
	for (int_max k = 1; k < EdgeHandleList.GetLength(); ++k)
	{
		auto AdjacentFaceIndexList_k = m_MeshData->EdgeList[EdgeHandleList[k].GetIndex()].GetAdjacentFaceIndexList();
		FaceIndexList = this->Intersect(FaceIndexList, AdjacentFaceIndexList_k);
	}
	//----------------------
	if (FaceIndexList.GetLength() != 1)
	{
		//MDK_Warning("EdgeHandleList is invalid @ MembraneMesh::GetFaceHandleByEdge(...)")
		FaceHandle.SetToInvalid();
		return FaceHandle;
	}

	FaceHandle.SetIndex(FaceIndexList[0]);
	return FaceHandle;
}


template<typename MeshAttributeType>
inline 
Handle_Of_Face_Of_MembraneMesh 
MembraneMesh<MeshAttributeType>::GetFaceHandleByPoint(const DenseVector<PointHandleType>& PointHandleList) const
{
	FaceHandleType FaceHandle;

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            //MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetFaceHandleByPoint(...)")
            FaceHandle.SetToInvalid();
            return FaceHandle;
        }
    }

	DenseVector<EdgeHandleType> EdgeHandleList;
	EdgeHandleList.Resize(PointHandleList.GetLength());
	for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
	{
		if (k < PointHandleList.GetLength() - 1)
		{
			EdgeHandleList[k] = this->GetEdgeHandleByPoint(PointHandleList[k], PointHandleList[k + 1]);
		}
		else
		{
			EdgeHandleList[k] = this->GetEdgeHandleByPoint(PointHandleList[k], PointHandleList[0]);
		}
		if (EdgeHandleList[k].GetIndex() < 0)
		{
			//MDK_Warning("PointHandleList is invalid @ MembraneMesh::GetFaceHandleByPoint(...)")
			FaceHandle.SetToInvalid();
			return FaceHandle;
		}
	}
	return this->GetFaceHandleByEdge(EdgeHandleList);
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
int_max MembraneMesh<MeshAttributeType>::GetPointIDByHandle(PointHandleType PointHandle) const
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


template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetPointIDListByHandleList(const DenseVector<PointHandleType>& PointHandleList) const
{
	DenseVector<int_max> PointIDList;
	PointIDList.Resize(PointHandleList.GetLength());
	for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
	{
		PointIDList[k] = this->GetPointIDByHandle(PointHandleList[k]);
	}
	return PointIDList;
}

//----------- get Edge ID by EdgeHandle, PointHandleList, PointIDList -----------------------------------------------------------//

template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::GetEdgeIDByHandle(EdgeHandleType EdgeHandle) const
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
int_max MembraneMesh<MeshAttributeType>::GetEdgeIDByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const
{
    auto EdgeHandle = this->GetEdgeHandle(PointHandle0, PointHandle1);
    return this->GetEdgeID(EdgeHandle);
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetEdgeIDListByHandleList(const DenseVector<EdgeHandleType>& EdgeHandleList) const
{
	DenseVector<int_max> EdgeIDList;
	EdgeIDList.Resize(EdgeHandleList.GetLength());
	for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
	{
		EdgeIDList[k] = this->GetEdgeIDByHandle(EdgeHandleList[k]);
	}
	return EdgeIDList;
}

//----------- get DirectedEdge ID by DirectedEdgeHandle, PointHandle, PointID -------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDByHandle(DirectedEdgeHandleType DirectedEdgeHandle) const
{
    if (this->IsValidHandle(DirectedEdgeHandle) == true)
    {
        auto Index = DirectedEdgeHandle.GetIndex();
		return m_MeshData->FaceList[Index.FaceIndex].DirectedEdgeList()[Index.RelativeIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
int_max MembraneMesh<MeshAttributeType>::
GetDirectedEdgeIDByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const
{
    auto DirectedEdgeHandle = this->GetDirectedEdgeHandle(PointHandle_start, PointHandle_end);
    return this->GetDirectedEdgeIDByHandle(DirectedEdgeHandle);
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDListByHandleList(const DenseVector<DirectedEdgeHandleType>& DirectedEdgeHandleList) const
{
	DenseVector<int_max> DirectedEdgeIDList;
	DirectedEdgeIDList.Resize(DirectedEdgeHandleList.GetLength());
	for (int_max k = 0; k < DirectedEdgeHandleList.GetLength(); ++k)
	{
		DirectedEdgeIDList[k] = this->GetDirectedEdgeIDByHandle(DirectedEdgeHandleList[k]);
	}
	return DirectedEdgeIDList;
}

//----------- get FaceID by FaceHandle, EdgeHandleList, EdgeIDList, PointHandleList, PointIDList --------------------------//

template<typename MeshAttributeType>
inline 
int_max MembraneMesh<MeshAttributeType>::GetFaceIDByHandle(FaceHandleType FaceHandle) const
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
int_max MembraneMesh<MeshAttributeType>::GetFaceIDByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const
{
    auto FaceHandle = this->GetFaceHandleByEdge(EdgeHandleList);
    return this->GetFaceIDByHandle(FaceHandle);
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> MembraneMesh<MeshAttributeType>::GetFaceIDListByHandleList(const DenseVector<FaceHandleType>& FaceHandleList) const
{
	DenseVector<int_max> FaceIDList;
	FaceIDList.Resize(FaceHandleList.GetLength());
	for (int_max k = 0; k < FaceHandleList.GetLength(); ++k)
	{
		FaceIDList[k] = this->GetFaceIDByHandle(FaceHandleList[k]);
	}
	return FaceIDList;
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    PointIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
	PointIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    EdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
	EdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfDirectedEdge()
{
    DirectedEdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfDirectedEdge() const
{
	DirectedEdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfFace()
{
    FaceIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType> MembraneMesh<MeshAttributeType>::GetIteratorOfFace() const
{
	FaceIteratorType it(*this);
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
		PointHandleType PointHandle;
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
			PointHandleType PointHandle;
            PointHandle.SetToInvalid();
            return PointHandle;
        }
    }
    else
    {
        MDK_Error("Position is a not a vector @ MembraneMesh::AddPoint(...)")
		PointHandleType PointHandle;
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

	PointHandleType PointHandle;
    PointHandle.SetIndex(PointIndex);

    return PointHandle;
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> MembraneMesh<MeshAttributeType>::AddPointSet(const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<PointHandleType> PointHandleList;

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
	DenseVector<PointHandleType> PointHandleList;

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
AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1)
{
	EdgeHandleType EdgeHandle;
	EdgeHandle.SetIndex(m_MeshData->EdgeList.GetLength());
	return this->AddEdge(PointHandle0, PointHandle1, EdgeHandle);
}


template<typename MeshAttributeType>
Handle_Of_Edge_Of_MembraneMesh MembraneMesh<MeshAttributeType>::
AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1, EdgeHandleType EdgeHandle_input)
{
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

	EdgeHandleType EdgeHandle;

    if (this->IsValidHandle(PointHandle0) == false || this->IsValidHandle(PointHandle1) == false)
    {
        MDK_Error("PointHandle0 or PointHandle1 is invalid @ MembraneMesh::AddEdge(...)")
        EdgeHandle.SetToInvalid();
        return EdgeHandle;
    }

	auto EdgeIndex = EdgeHandle_input.GetIndex();
	if (EdgeIndex < 0 || EdgeIndex > m_MeshData->EdgeList.GetLength())
	{
		MDK_Error("EdgeHandle_input is out of range @ MembraneMesh::AddEdge(...)")
		EdgeHandle.SetToInvalid();
		return EdgeHandle;
	}

    auto EdgeHandle_temp = this->GetEdgeHandleByPoint(PointHandle0, PointHandle1);
    if (this->IsValidHandle(EdgeHandle_temp) == true)
    {
        //MDK_Warning("The edge has been added already @ MembraneMesh::AddEdge(...)")
		if (EdgeIndex == m_MeshData->EdgeList.GetLength() || EdgeIndex == EdgeHandle_temp.GetIndex())
		{
			EdgeHandle = EdgeHandle_temp;
			return EdgeHandle;
		}
		else
		{
			MDK_Error("Edge exist at EdgeHandle_input @ MembraneMesh::AddEdge(...)")
			EdgeHandle.SetToInvalid();
			return EdgeHandle;
		}
    }

	if (EdgeIndex < m_MeshData->EdgeList.GetLength())
	{
		if (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
		{
			MDK_Error("Edge exist at EdgeHandle_input @ MembraneMesh::AddEdge(...)")
			EdgeHandle.SetToInvalid();
			return EdgeHandle;
		}
	}

    //----------------------------------------------------------------
    auto PointIndex0 = PointHandle0.GetIndex();
    auto PointIndex1 = PointHandle1.GetIndex();
	
	//Create Edge ----------------------------------------------------------------

	if (EdgeIndex == m_MeshData->EdgeList.GetLength())
	{
		EdgeType EmptyEdge;
		m_MeshData->EdgeList.Append(std::move(EmptyEdge));
		m_MeshData->EdgeValidityFlagList.Append(1);
	}
	else
	{
		m_MeshData->EdgeList[EdgeIndex].ReCreate();
		m_MeshData->EdgeValidityFlagList[EdgeIndex] = 1;
	}
    // take reference
	auto& Edge = m_MeshData->EdgeList[EdgeIndex];
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex);
    Edge.SetPointIndexList(PointIndex0, PointIndex1);

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
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList)
{
	FaceHandleType FaceHandle;
	FaceHandle.SetIndex(m_MeshData->FaceList.GetLength());
	return this->AddFaceByEdge(EdgeHandleList, FaceHandle);
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList, FaceHandleType FaceHandle_input)
{
    // Input: the input EdgeIndexList must have been ordered
	// the first point of the first input edge may NOT be the first point of the face
    // the order determine the direction/sign of the normal vector
    // it also determine the direction of each DirectedEdge
    //
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

	FaceHandleType FaceHandle;

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

	auto FaceIndex = FaceHandle_input.GetIndex();
	if (FaceIndex < 0 && FaceIndex > m_MeshData->FaceValidityFlagList.GetLength())
	{
		MDK_Error("FaceIndex is out of range @ MembraneMesh::AddFaceByEdge(...)")
		FaceHandle.SetToInvalid();
		return FaceHandle;
	}

    auto FaceHandle_temp = this->GetFaceHandleByEdge(EdgeHandleList);
    if (this->IsValidHandle(FaceHandle_temp) == true)
    {
        //MDK_Warning("The face has been added already @ MembraneMesh::AddFaceByEdge(...)")

		if (FaceIndex == m_MeshData->FaceValidityFlagList.GetLength() || FaceIndex == FaceHandle_temp.GetIndex())
		{//attempt to add new face, not necessary
			FaceHandle = FaceHandle_temp;
			return FaceHandle;
		}
		else
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceIndex @ MembraneMesh::AddFaceByEdge(...)")
			FaceHandle.SetToInvalid();
			return FaceHandle;
		}
    }

	if (FaceIndex < m_MeshData->FaceValidityFlagList.GetLength())
	{
		if (m_MeshData->FaceValidityFlagList[FaceIndex] == 1)
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceIndex @ MembraneMesh::AddFaceByEdge(...)")
			FaceHandle.SetToInvalid();
			return FaceHandle;
		}
	}

	//-------------------- input check is done ---------------------------------------------------//

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

    // create face --------------------------------------------------------------------------------------------------

	if (FaceIndex == m_MeshData->FaceList.GetLength())
	{
		FaceType EmptyFace;
		m_MeshData->FaceList.Append(std::move(EmptyFace));
		m_MeshData->FaceValidityFlagList.Append(1);
	}
	else
	{
		m_MeshData->FaceList[FaceIndex].ReCreate();
		m_MeshData->FaceValidityFlagList[FaceIndex] = 1;
	}
	// take reference
	auto& Face = m_MeshData->FaceList[FaceIndex];
	Face.SetParentMesh(*this);
	Face.SetIndex(FaceIndex);

	// create DirectedEdge in Face	
	Face.DirectedEdgeList().Resize(EdgeIndexList.GetLength());
	for (int_max k = 0; k < Face.DirectedEdgeList().GetLength(); ++k)
	{
		// attention: auto& will get reference, auto will copy
		auto& DirectedEdge_k = Face.DirectedEdgeList()[k];		
		DirectedEdge_k.SetIndex(FaceIndex, k);
		DirectedEdge_k.SetParentMesh(*this);
		DirectedEdge_k.SetEdgeIndex(EdgeIndexList[k]);
		DirectedEdge_k.SetStartPointIndex(PointIndexList[k]);

		if (k < Face.DirectedEdgeList().GetLength() - 1)
		{
			DirectedEdge_k.SetEndPointIndex(PointIndexList[k + 1]);
			DirectedEdge_k.SetNextDirectedEdgeIndex(FaceIndex, k + 1);
		}
		else // if (k == Face.DirectedEdgeList().GetLength() - 1)
		{
			DirectedEdge_k.SetEndPointIndex(PointIndexList[0]);
			DirectedEdge_k.SetNextDirectedEdgeIndex(FaceIndex, 0);
		}

		if (k > 0)
		{
			DirectedEdge_k.SetPreviousDirectedEdgeIndex(FaceIndex, k - 1);
		}
		else // if (k == 0)
		{
			DirectedEdge_k.SetPreviousDirectedEdgeIndex(FaceIndex, Face.DirectedEdgeList().GetLength() - 1);
		}
	}

    // update information in m_MeshData->PointList ------------------------------------------------------------------

	for (int_max k = 0; k < Face.DirectedEdgeList().GetLength(); ++k)
    {
		const auto& DirectedEdge_k = Face.DirectedEdgeList()[k];
		m_MeshData->PointList[DirectedEdge_k.GetStartPointIndex()].OutgoingDirectedEdgeIndexList().Append(DirectedEdge_k.GetIndex());
		m_MeshData->PointList[DirectedEdge_k.GetEndPointIndex()].IncomingDirectedEdgeIndexList().Append(DirectedEdge_k.GetIndex());
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().Append(FaceIndex);
    }

	// update information in m_MeshData->EdgeList ------------------------------------------------------------------------
	for (int_max k = 0; k < Face.DirectedEdgeList().GetLength(); ++k)
	{
		const auto& DirectedEdge_k = Face.DirectedEdgeList()[k];
		auto EdgeIndex_k = DirectedEdge_k.GetEdgeIndex();
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().Append(DirectedEdge_k.GetIndex());
	}

    //-----------------------------
    FaceHandle.SetIndex(FaceIndex);
    return FaceHandle;
}


template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<PointHandleType>& PointHandleList)
{
	FaceHandleType FaceHandle;
	FaceHandle.SetIndex(m_MeshData->FaceList.GetLength());
	return this->AddFaceByPoint(PointHandleList, FaceHandle);
}

template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<PointHandleType>& PointHandleList, FaceHandleType FaceHandle_input)
{
    if (PointHandleList.GetLength() < 3)
    {        
        MDK_Error("PointHandleList length < 3 @ MembraneMesh::AddFaceByPoint(...)")
		FaceHandleType FaceHandle;
        FaceHandle.SetToInvalid();
        return FaceHandle;
    }

	DenseVector<EdgeHandleType> EdgeHandleList;
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

	return this->AddFaceByEdge(EdgeHandleList, FaceHandle_input);
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteFace(FaceHandleType FaceHandle)
{
    // this function will delete each DirectedEdge of the Face, and modify any information related to the face
    // FaceHandle and FaceID of the face become invalid after the face is deleted

    // check input 
    if (this->IsValidHandle(FaceHandle) == false)
    {
        //MDK_Warning("Invalid FaceHandle @ MembraneMesh::DeleteFace(...)")
        return;
    }

    //--------------------------------------------------------------------------------------
    auto FaceIndex = FaceHandle.GetIndex();

    // auto& will get reference
    const auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex].DirectedEdgeList();
    const auto& PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();

    // update information in m_MeshData->PointList ------------------------------------------------------------------

	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)
    {
		auto DirectedEdge_k = DirectedEdgeList[k];
		auto tempPointIndex_start = DirectedEdge_k.GetStartPointIndex();
		auto tempPointIndex_end = DirectedEdge_k.GetEndPointIndex();

		auto tempIndex_s = m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().ExactMatch(DirectedEdge_k.GetIndex());
		m_MeshData->PointList[tempPointIndex_start].OutgoingDirectedEdgeIndexList().Delete(tempIndex_s);

		auto tempIndex_e = m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().ExactMatch(DirectedEdge_k.GetIndex());
		m_MeshData->PointList[tempPointIndex_end].IncomingDirectedEdgeIndexList().Delete(tempIndex_e);
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        auto tempIndex = m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().ExactMatch(FaceIndex);
        m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList().Delete(tempIndex);
    }

	// update information in m_MeshData.EdgeList ------------------------------------------------------------------
	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)
	{
		const auto& DirectedEdge_k = DirectedEdgeList[k];
		auto EdgeIndex_k = DirectedEdge_k.GetEdgeIndex();
		auto tempIndex = m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().ExactMatch(DirectedEdge_k.GetIndex());
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().Delete(tempIndex);
	}
	
    // delete Face and DirectedEdge
    this->InternalFuction_DeleteFace(FaceIndex);
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::DeleteEdge(EdgeHandleType EdgeHandle)
{    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        //MDK_Warning("Invalid EdgeHandle @ MembraneMesh::DeleteEdge(...)")
        return;
    }

    // check AdjacentFace: the edge can not be deleted if any adjacent face exit

    auto EdgeIndex = EdgeHandle.GetIndex();

    auto AdjacentFaceCount = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceCount();
    if (AdjacentFaceCount > 0)
    {
        MDK_Error("AdjacentFaceIndexList is not empty, so this edge can not be deleted @ MembraneMesh::DeleteEdge(...)")
        return;
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
void MembraneMesh<MeshAttributeType>::DeletePoint(PointHandleType PointHandle)
{
    if (this->IsValidHandle(PointHandle) == false)
    {
        //MDK_Warning("Invalid PointHandle @ MembraneMesh::DeletePoint(...)")
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
void MembraneMesh<MeshAttributeType>::DeletePoint(const DenseVector<PointHandleType>& PointHandleList)
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

//----------------- remove deleted item from Point/Edge/DirectedEdgeList/Face list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index and handle may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when InvalidPointHandleCount/ValidPointCount( GetPointCount() ) > 0.5

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::CleanDataStructure()
{
	DenseVector<int_max> PointIndexMap_Old_To_New;
	DenseVector<int_max> EdgeIndexMap_Old_To_New;
	ObjectArray<DenseVector<int_max>> DirectedEdge_RelativeIndexMap_Old_To_New;
	DenseVector<int_max> FaceIndexMap_Old_To_New;
	this->CleanDataStructure(PointIndexMap_Old_To_New, EdgeIndexMap_Old_To_New, DirectedEdge_RelativeIndexMap_Old_To_New, FaceIndexMap_Old_To_New);
}

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New,	DenseVector<int_max>& FaceIndexMap_Old_To_New)
{
	if (this->GetDeletedPointCount() == 0 && this->GetDeletedEdgeCount() == 0 && this->GetDeletedFaceCount() == 0)
	{
		return;
	}

	//---------------- get IndexMap from old to new ---------------------------------------//
	PointIndexMap_Old_To_New.Clear();
	PointIndexMap_Old_To_New.Resize(m_MeshData->PointList.GetLength());

	DenseVector<int_max> PointIndexMap_New_To_Old;
	PointIndexMap_New_To_Old.SetCapacity(m_MeshData->PointList.GetLength());

	for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k) // k is PointIndex_old
	{
		PointIndexMap_Old_To_New[k] = -1;
		if (m_MeshData->PointValidityFlagList[k] == 1)
		{
			PointIndexMap_New_To_Old.Append(k);
			auto PointIndex_new = PointIndexMap_New_To_Old.GetLength() - 1;
			PointIndexMap_Old_To_New[k] = PointIndex_new;
		}
	}

	EdgeIndexMap_Old_To_New.Clear();
	EdgeIndexMap_Old_To_New.Resize(m_MeshData->EdgeList.GetLength());

	DenseVector<int_max> EdgeIndexMap_New_To_Old;
	EdgeIndexMap_New_To_Old.SetCapacity(m_MeshData->EdgeList.GetLength());

	for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k) // k is EdgeIndex_old
	{
		EdgeIndexMap_Old_To_New[k] = -1;
		if (m_MeshData->EdgeValidityFlagList[k] == 1)
		{
			EdgeIndexMap_New_To_Old.Append(k);
			auto EdgeIndex_new = EdgeIndexMap_New_To_Old.GetLength() - 1;
			EdgeIndexMap_Old_To_New[k] = EdgeIndex_new;
		}
	}

	FaceIndexMap_Old_To_New.Clear();
	FaceIndexMap_Old_To_New.Resize(m_MeshData->FaceList.GetLength());

	DenseVector<int_max> FaceIndexMap_New_To_Old;
	FaceIndexMap_New_To_Old.SetCapacity(m_MeshData->FaceList.GetLength());

	for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k) // k is FaceIndex_old
	{
		FaceIndexMap_Old_To_New[k] = -1;
		if (m_MeshData->FaceValidityFlagList[k] == 1)
		{
			FaceIndexMap_New_To_Old.Append(k);
			auto FaceIndex_new = FaceIndexMap_New_To_Old.GetLength() - 1;
			FaceIndexMap_Old_To_New[k] = FaceIndex_new;
		}
	}

	auto PointCount_old = PointIndexMap_Old_To_New.GetLength();// include deleted point
	auto PointCount_new = PointIndexMap_New_To_Old.GetLength();
	auto EdgeCount_old = EdgeIndexMap_Old_To_New.GetLength();// include deleted edge
	auto EdgeCount_new = EdgeIndexMap_New_To_Old.GetLength();
	auto FaceCount_old = FaceIndexMap_Old_To_New.GetLength();// include deleted face
	auto FaceCount_new = FaceIndexMap_New_To_Old.GetLength();

	//------------------------------------------ update -------------------------------------------------------------//

	if (PointCount_old != PointCount_new)
	{
		m_MeshData->PointPositionTable = m_MeshData->PointPositionTable.GetSubMatrix(ALL, PointIndexMap_New_To_Old);

		PointListType PointList_new;
		PointList_new.Resize(PointIndexMap_New_To_Old.GetLength());
		for (int_max k = 0; k < PointIndexMap_New_To_Old.GetLength(); ++k)// k is PointIndex_new
		{
			PointList_new[k] = std::move(m_MeshData->PointList[PointIndexMap_New_To_Old[k]]);
			PointList_new[k].SetIndex(k);
		}
		m_MeshData->PointList = std::move(PointList_new);
		m_MeshData->PointValidityFlagList.Resize(PointCount_new);
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

	if (EdgeCount_old != EdgeCount_new)
	{
		EdgeListType EdgeList_new;
		EdgeList_new.Resize(EdgeIndexMap_New_To_Old.GetLength());
		for (int_max k = 0; k < EdgeIndexMap_New_To_Old.GetLength(); ++k)// k is EdgeIndex_new
		{
			EdgeList_new[k] = std::move(m_MeshData->EdgeList[EdgeIndexMap_New_To_Old[k]]);
			EdgeList_new[k].SetIndex(k);
		}
		m_MeshData->EdgeList = std::move(EdgeList_new);
		m_MeshData->EdgeValidityFlagList.Resize(EdgeCount_new);
		m_MeshData->EdgeValidityFlagList.Fill(1);

		// update Map_EdgeID_to_EdgeIndex
		m_MeshData->Map_EdgeID_to_EdgeIndex.clear();
		m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.clear();
		for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
		{
			auto EdgeID_k = m_MeshData->EdgeList[k].GetID();
			if (EdgeID_k >= 0) // ID is invalid if < 0 
			{
				m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeID_k] = k;
			}
		}
	}

	if (FaceCount_old != FaceCount_new)
	{
		FaceListType FaceList_new;
		FaceList_new.Resize(FaceIndexMap_New_To_Old.GetLength());
		for (int_max k = 0; k < FaceIndexMap_New_To_Old.GetLength(); ++k)
		{
			FaceList_new[k] = std::move(m_MeshData->FaceList[FaceIndexMap_New_To_Old[k]]);
			FaceList_new[k].SetIndex(k);
		}
		m_MeshData->FaceList = std::move(FaceList_new);
		m_MeshData->FaceValidityFlagList.Resize(FaceCount_new);
		m_MeshData->FaceValidityFlagList.Fill(1);

		for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
		{
			for (int_max n = 0; n < m_MeshData->FaceList[k].DirectedEdgeList().GetLength(); ++n)
			{
				// auto& : get reference and modify
				auto& DirectedEdge_n = m_MeshData->FaceList[k].DirectedEdgeList()[n];
				DirectedEdge_n.SetIndex(k, n);
			}
		}

		// update Map_FaceID_to_FaceIndex and Map_DirectedEdgeID_to_DirectedEdgeIndex
		m_MeshData->Map_FaceID_to_FaceIndex.clear();
		for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
		{
			auto FaceID_k = m_MeshData->FaceList[k].GetID();
			if (FaceID_k >= 0) // ID is invalid if < 0 
			{
				m_MeshData->Map_FaceID_to_FaceIndex[FaceID_k] = k;

				for (int_max n = 0; n < m_MeshData->FaceList[k].DirectedEdgeList().GetLength(); ++n)
				{
					// auto& : get reference and modify
					auto& DirectedEdge_n = m_MeshData->FaceList[k].DirectedEdgeList()[n];
					auto DirectedEdgeID_n = DirectedEdge_n.GetID();
					if (DirectedEdgeID_n >= 0) // ID is invalid if < 0 
					{
						m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID_n] = DirectedEdge_n.GetIndex();					
					}
				}
			}
		}
	}

	//--------------------------- update adjacency information------------------

	for (int_max PointIndex_new = 0; PointIndex_new < m_MeshData->PointList.GetLength(); ++PointIndex_new)
	{
		const auto& AdjacentPointIndexList_old = m_MeshData->PointList[PointIndex_new].AdjacentPointIndexList();
		DenseVector<int_max> AdjacentPointIndexList_new;
		for (int_max n = 0; n < AdjacentPointIndexList_old.GetLength(); ++n)
		{
			auto Index_new = PointIndexMap_Old_To_New[AdjacentPointIndexList_old[n]];
			AdjacentPointIndexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].AdjacentPointIndexList() = std::move(AdjacentPointIndexList_new);

		const auto& AdjacentEdgeIndexList_old = m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList();
		DenseVector<int_max> AdjacentEdgeIndexList_new;
		for (int_max n = 0; n < AdjacentEdgeIndexList_old.GetLength(); ++n)
		{
			auto Index_new = EdgeIndexMap_Old_To_New[AdjacentEdgeIndexList_old[n]];
			AdjacentEdgeIndexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList() = std::move(AdjacentEdgeIndexList_new);

		const auto& OutgoingDirectedEdgeIndexList_old = m_MeshData->PointList[PointIndex_new].OutgoingDirectedEdgeIndexList();
		DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutgoingDirectedEdgeIndexList_new;
		for (int_max n = 0; n < OutgoingDirectedEdgeIndexList_old.GetLength(); ++n)
		{
			auto Index_old = OutgoingDirectedEdgeIndexList_old[n];
			auto Index_new = Index_old;
			Index_new.EdgeIndex = EdgeIndexMap_Old_To_New[Index_old.EdgeIndex];
			Index_new.RelativeIndex = DirectedEdge_RelativeIndexMap_Old_To_New[Index_old.EdgeIndex][Index_old.RelativeIndex];
			OutgoingDirectedEdgeIndexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].OutgoingDirectedEdgeIndexList() = std::move(OutgoingDirectedEdgeIndexList_new);

		const auto& IncomingDirectedEdgeIndexList_old = m_MeshData->PointList[PointIndex_new].IncomingDirectedEdgeIndexList();
		DenseVector<DirectedEdgeIndex_Of_MembraneMesh> IncomingDirectedEdgeIndexList_new;
		for (int_max n = 0; n < IncomingDirectedEdgeIndexList_old.GetLength(); ++n)
		{
			auto Index_old = IncomingDirectedEdgeIndexList_old[n];
			auto Index_new = Index_old;
			Index_new.EdgeIndex = EdgeIndexMap_Old_To_New[Index_old.EdgeIndex];
			Index_new.RelativeIndex = DirectedEdge_RelativeIndexMap_Old_To_New[Index_old.EdgeIndex][Index_old.RelativeIndex];
			IncomingDirectedEdgeIndexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].IncomingDirectedEdgeIndexList() = std::move(IncomingDirectedEdgeIndexList_new);

		const auto& AdjacentFaceIdnexList_old = m_MeshData->PointList[PointIndex_new].AdjacentFaceIndexList();
		DenseVector<int_max> AdjacentFaceIdnexList_new;
		for (int_max n = 0; n < AdjacentFaceIdnexList_old.GetLength(); ++n)
		{
			auto Index_new = FaceIndexMap_Old_To_New[AdjacentFaceIdnexList_old[n]];
			AdjacentFaceIdnexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].AdjacentFaceIndexList() = std::move(AdjacentFaceIdnexList_new);
	}

	for (auto EdgeIndex_new = 0; EdgeIndex_new < m_MeshData->EdgeList.GetLength(); ++EdgeIndex_new)
	{
		auto PointIndexList = m_MeshData->EdgeList[EdgeIndex_new].GetPointIndexList();
		PointIndexList[0] = PointIndexMap_Old_To_New[PointIndexList[0]];
		PointIndexList[1] = PointIndexMap_Old_To_New[PointIndexList[1]];
		m_MeshData->EdgeList[EdgeIndex_new].SetPointIndexList(PointIndexList);
		// take reference
		auto& DirectedEdgeIndexList = m_MeshData->EdgeList[EdgeIndex_new].DirectedEdgeIndexList();
		for (int_max n = 0; n < DirectedEdgeIndexList.GetLength(); ++n)
		{
			DirectedEdgeIndexList[n].FaceIndex = FaceIndexMap_Old_To_New[DirectedEdgeIndexList[n].FaceIndex];
		    //DirectedEdgeIndexList[n].RelativeIndex will not change
		}
	}

	for (int_max FaceIndex_new = 0; FaceIndex_new < m_MeshData->FaceList.GetLength(); ++FaceIndex_new)
	{
		// take reference
		auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex_new].DirectedEdgeList();
		for (int_max n = 0; n < DirectedEdgeList.GetLength(); ++n)
		{
			DirectedEdgeList[n].SetFaceIndex(FaceIndexMap_Old_To_New[DirectedEdgeList[n].GetFaceIndex()]);
			DirectedEdgeList[n].SetStartPointIndex(PointIndexMap_Old_To_New[DirectedEdgeList[n].GetStartPointIndex()]);
			DirectedEdgeList[n].SetEndPointIndex(PointIndexMap_Old_To_New[DirectedEdgeList[n].GetEndPointIndex()]);

			auto Index_next_old = DirectedEdgeList[n].GetNextDirectedEdgeIndex();
			auto Index_next_new = Index_next_old;
			Index_next_new.FaceIndex = FaceIndexMap_Old_To_New[Index_next_old.FaceIndex];
			//Index_next_new.RelativeIndex will not change
			DirectedEdgeList[n].SetNextDirectedEdgeIndex(Index_next_new);

			auto Index_prev_old = DirectedEdgeList[n].GetPreviousDirectedEdgeIndex();
			auto Index_prev_new = Index_prev_old;
			Index_prev_new.FaceIndex = EdgeIndexMap_Old_To_New[Index_prev_old.FaceIndex];
			//Index_prev_new.RelativeIndex will not change
			DirectedEdgeList[n].SetPreviousDirectedEdgeIndex(Index_prev_new);
		}
	}

}


template<typename MeshAttributeType>
bool MembraneMesh<MeshAttributeType>::Check_If_DataStructure_is_Clean() const
{
	auto Count1 = this->GetDeletedPointCount();
	auto Count2 = this->GetDeletedEdgeCount();
	auto Count3 = this->GetDeletedFaceCount();
	return (Count1 + Count2 + Count3 == 0);
}


template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedPointCount() const
{
	auto ValidPointCount = this->GetPointCount();
	auto PointCountOfList = m_MeshData->PointList.GetLength();
	return  PointCountOfList - ValidPointCount;
}


template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedEdgeCount() const
{
	auto ValidEdgeCount = this->GetEdgeCount();
	auto EdgeCountOfList = m_MeshData->EdgeList.GetLength();
	return  EdgeCountOfList - ValidEdgeCount;
}

template<typename MeshAttributeType>
int_max MembraneMesh<MeshAttributeType>::GetDeletedFaceCount() const
{
	auto ValidFaceCount = this->GetFaceCount();
	auto FaceCountOfList = m_MeshData->FaceList.GetLength();
	return  FaceCountOfList - ValidFaceCount;
}

//-------------------- get a sub mesh by FaceHandleList or FaceIDList -----------------------------------------//

template<typename MeshAttributeType>
MembraneMesh<MeshAttributeType> 
MembraneMesh<MeshAttributeType>::GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const
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

    // point index on this mesh -> point index on output mesh
	//std::unordered_map<int_max, int_max> Map_PointIndex_OuputIndex;        
	DenseVector<int_max> Map_PointIndex_OuputIndex;
	Map_PointIndex_OuputIndex.Resize(m_MeshData->PointList.GetLength());
	Map_PointIndex_OuputIndex.Fill(-1);
    
	// edge index on this mesh -> edge index on output mesh	
	//std::unordered_map<int_max, int_max> Map_EdgeIndex_OuputIndex;
	DenseVector<int_max> Map_EdgeIndex_OuputIndex;
	Map_EdgeIndex_OuputIndex.Resize(m_MeshData->EdgeList.GetLength());
	Map_EdgeIndex_OuputIndex.Fill(-1);

    // add face one by one
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
            //auto it = Map_PointIndex_OuputIndex.find(PointHandle_n.GetIndex());
			//if (it == Map_PointIndex_OuputIndex.end())
			auto tempIndex = Map_PointIndex_OuputIndex.ExactMatch("first", PointHandle_n.GetIndex());
			if (tempIndex < 0)
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

        // add face
        auto FaceHandle_OutputMesh = OutputMesh.AddFaceByPoint(PointHandleList_OutputMesh);
        // copy face ID if it is valid
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
            //auto it = Map_EdgeIndex_OuputIndex.find(EdgeHandleList_k[n].GetIndex());
            //if (it == Map_EdgeIndex_OuputIndex.end())
			auto tempIndex = Map_EdgeIndex_OuputIndex.ExactMatch("first", EdgeHandleList_k[n].GetIndex());
			if (tempIndex < 0)
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


// Change Topology ----------------------------------------------------------------------------------------

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::SwapPoint(PointHandleType PointHandleA, PointHandleType PointHandleB)
{
	if (this->IsValidHandle(PointHandleA) == false || this->IsValidHandle(PointHandleB) == false)
	{
		MDK_Error("Invalid PointHandle @ MembraneMesh::SwapPoint(...)")
		return;
	}

	if (PointHandleA == PointHandleB)
	{
		return;
	}

	auto PointIndexA = PointHandleA.GetIndex();
	auto& AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].AdjacentPointIndexList();//to be updated
	const auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();
	const auto& OutgoingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].OutgoingDirectedEdgeIndexList();
	const auto& IncomingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].IncomingDirectedEdgeIndexList();

	auto PointIndexB = PointHandleB.GetIndex();
	auto& AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].AdjacentPointIndexList();//to be updated
	const auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();
	const auto& OutgoingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].OutgoingDirectedEdgeIndexList();
	const auto& IncomingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].IncomingDirectedEdgeIndexList();

	// Update index and ID
	auto PointID_A = m_MeshData->PointList[PointIndexA].GetID();
	auto PointID_B = m_MeshData->PointList[PointIndexB].GetID();
	m_MeshData->PointList[PointIndexA].SetIndex(PointIndexB);
	if (PointID_B >= 0)
	{
		m_MeshData->PointList[PointIndexA].SetID(PointID_B);
	}

	m_MeshData->PointList[PointIndexB].SetIndex(PointIndexA);
	if (PointID_A >= 0)
	{
		m_MeshData->PointList[PointIndexB].SetID(PointID_A);
	}

	//update Position
	auto PositionA = m_MeshData->PointList[PointIndexA].GetPosition();
	auto PositionB = m_MeshData->PointList[PointIndexB].GetPosition();
	m_MeshData->PointList[PointIndexA].SetPosition(PositionB);
	m_MeshData->PointList[PointIndexB].SetPosition(PositionA);

	//Update Attribute
	auto Attribute_A = m_MeshData->PointList[PointIndexA].Attribute();
	auto Attribute_B = m_MeshData->PointList[PointIndexB].Attribute();
	m_MeshData->PointList[PointIndexA].Attribute() = std::move(Attribute_B);
	m_MeshData->PointList[PointIndexB].Attribute() = std::move(Attribute_A);

	// update adjacency information -------------------------

	auto tempPointIndexA = m_MeshData->PointList.GetLength();

	//change PointIndexA -> tempPointIndexA
	for (int_max k = 0; k < AdjacentPointIndexListA.GetLength(); ++k)
	{
		auto& AdjacentPointIndexList_k = m_MeshData->PointList[AdjacentPointIndexListA[k]].AdjacentPointIndexList();
		auto tempIndex = AdjacentPointIndexList_k.ExactMatch("first", PointIndexA);
		AdjacentPointIndexList_k[tempIndex] = tempPointIndexA;
	}
	for (int_max k = 0; k < AdjacentEdgeIndexListA.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].GetPointIndexList();
		if (PointIndexList_k[0] == PointIndexA)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(tempPointIndexA, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexList_k[0], tempPointIndexA);
		}
	}

	//change  PointIndexB -> PointIndexA
	for (int_max k = 0; k < AdjacentPointIndexListB.GetLength(); ++k)
	{
		auto& AdjacentPointIndexList_k = m_MeshData->PointList[AdjacentPointIndexListB[k]].AdjacentPointIndexList();
		auto tempIndex = AdjacentPointIndexList_k.ExactMatch("first", PointIndexB);
		AdjacentPointIndexList_k[tempIndex] = PointIndexA;
	}
	for (int_max k = 0; k < AdjacentEdgeIndexListB.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].GetPointIndexList();
		if (PointIndexList_k[0] == PointIndexB)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexA, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexList_k[0], PointIndexA);
		}
	}

	//change tempPointIndexA -> PointIndexB
	for (int_max k = 0; k < AdjacentPointIndexListA.GetLength(); ++k)
	{
		auto& AdjacentPointIndexList_k = m_MeshData->PointList[AdjacentPointIndexListA[k]].AdjacentPointIndexList();
		auto tempIndex = AdjacentPointIndexList_k.ExactMatch("first", tempPointIndexA);
		AdjacentPointIndexList_k[tempIndex] = PointIndexB;
	}
	for (int_max k = 0; k < AdjacentEdgeIndexListA.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].GetPointIndexList();
		if (PointIndexList_k[0] == tempPointIndexA)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexB, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexList_k[0], PointIndexB);
		}
	}

	//update AdjacentPointIndexListA
	{
		auto tempIndex = AdjacentPointIndexListA.ExactMatch("first", PointIndexB);
		if (tempIndex >= 0)
		{
			AdjacentPointIndexListA[tempIndex] = PointIndexA;
		}
	}

	//update AdjacentPointIndexListB
	{
		auto tempIndex = AdjacentPointIndexListB.ExactMatch("first", PointIndexA);
		if (tempIndex >= 0)
		{
			AdjacentPointIndexListB[tempIndex] = PointIndexB;
		}
	}

	//change PointIndexA -> PointIndexB for DirectedEdge
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto EdgeIndex_k = OutgoingDirectedEdgeIndexListA[k].EdgeIndex;
		auto RelativeIndex_k = OutgoingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetStartPointIndex(PointIndexB);
	}
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto EdgeIndex_k = IncomingDirectedEdgeIndexListA[k].EdgeIndex;
		auto RelativeIndex_k = IncomingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetEndPointIndex(PointIndexB);
	}

	//change PointIndexB -> PointIndexA for DirectedEdge
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto EdgeIndex_k = OutgoingDirectedEdgeIndexListB[k].EdgeIndex;
		auto RelativeIndex_k = OutgoingDirectedEdgeIndexListB[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetStartPointIndex(PointIndexA);
	}
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto EdgeIndex_k = IncomingDirectedEdgeIndexListB[k].EdgeIndex;
		auto RelativeIndex_k = IncomingDirectedEdgeIndexListB[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetEndPointIndex(PointIndexA);
	}
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergePoint(PointHandleType PointHandleA, PointHandleType PointHandleB)
{
	//-------------------------
	//   \      /
	// ---A----B---
	//   /      \
	//-------------------------
	//   \      /
	// ---A    B---
	//   /      \
    //-------------------------

	if (PointHandleA == PointHandleB)
	{
		return PointHandleA;
	}

	Handle_Of_Point_Of_MembraneMesh PointHandle_output;

	if (this->IsValidHandle(PointHandleA) == false || this->IsValidHandle(PointHandleB) == false)
	{
		MDK_Error("Invalid PointHandle @ MembraneMesh::MergePoint(...)")
		PointHandle_output.SetToInvalid();
		return PointHandle_output;
	}

	auto PointIndexA = PointHandleA.GetIndex();
	auto PointIndexB = PointHandleB.GetIndex();

	auto EdgeHandle_AB = this->GetEdgeHandleByPoint(PointHandleA, PointHandleB);
	if (EdgeHandle_AB.GetIndex() >= 0)
	{
		auto AdjacentFaceIndexList_AB = m_MeshData->EdgeList[EdgeHandle_AB.GetIndex()].GetAdjacentFaceIndexList();
		for (int_max k = 0; k < AdjacentFaceIndexList_AB.GetLength(); ++k)
		{
			auto& DirectedEdgeIndexList = m_MeshData->FaceList[AdjacentFaceIndexList_AB[k]].DirectedEdgeIndexList();//to be updated
			if (DirectedEdgeIndexList.GetLength() > 3)
			{
				int_max Index_delete = -1;
				for (int_max n = 0; n < DirectedEdgeIndexList.GetLength(); ++n)
				{
					if (DirectedEdgeIndexList[n].EdgeIndex == EdgeHandle_AB.GetIndex())
					{
						Index_delete = n;
						break;
					}
				}

				int_max Index_prev = Index_delete - 1;
				if (Index_prev < 0)
				{
					Index_prev = DirectedEdgeIndexList.GetLength() - 1;
				}

				int_max Index_next = Index_delete + 1;
				if (Index_next >= DirectedEdgeIndexList.GetLength())
				{
					Index_next = 0;
				}

				auto DirectedEdgeIndex_prev = DirectedEdgeIndexList[Index_prev];
				auto DirectedEdgeIndex_next = DirectedEdgeIndexList[Index_next];

				auto& DirectedEdge_prev = m_MeshData->EdgeList[DirectedEdgeIndex_prev.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_prev.RelativeIndex];
				DirectedEdge_prev.SetNextDirectedEdgeIndex(DirectedEdgeIndex_next);

				auto& DirectedEdge_next = m_MeshData->EdgeList[DirectedEdgeIndex_next.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_next.RelativeIndex];
				DirectedEdge_next.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_prev);

				DirectedEdgeIndexList.Delete(Index_delete);
			}
			else
			{
				MDK_Error("can NOT merge two point of a triangle face @ MembraneMesh::MergePoint(...)")
				PointHandle_output.SetToInvalid();
				return PointHandle_output;
			}		
		}
	}

	//-------------------------- Merge B to A ------------------------------------

	const auto& AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].AdjacentPointIndexList();
	const auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();
	const auto& OutgoingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].OutgoingDirectedEdgeIndexList();
	const auto& IncomingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].IncomingDirectedEdgeIndexList();
	//update point adjacent to B
	for (int_max k = 0; k < AdjacentPointIndexListB.GetLength(); ++k)
	{
		auto& AdjacentPointIndexList_k = m_MeshData->PointList[AdjacentPointIndexListB[k]].AdjacentPointIndexList();
		auto tempIndexB = AdjacentPointIndexList_k.ExactMatch("first", PointIndexB);
		AdjacentPointIndexList_k[tempIndexB] = PointIndexA;
	}
	//update edge adjacent to B
	for (int_max k = 0; k < AdjacentEdgeIndexListB.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].GetPointIndexList();
		if (PointIndexList_k[0] == PointIndexB)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexA, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexList_k[0], PointIndexA);
		}
	}

	//update adjacency info of A
	auto& AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].AdjacentPointIndexList();//to be updated
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();//to be updated
	auto& OutgoingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].OutgoingDirectedEdgeIndexList();//to be updated
	auto& IncomingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].IncomingDirectedEdgeIndexList();//to be updated
	AdjacentPointIndexListA = { &AdjacentPointIndexListA, &AdjacentPointIndexListB };
	AdjacentEdgeIndexListA = { &AdjacentEdgeIndexListA, &AdjacentEdgeIndexListB };
	OutgoingDirectedEdgeIndexListA = { &OutgoingDirectedEdgeIndexListA, &OutgoingDirectedEdgeIndexListB };
	IncomingDirectedEdgeIndexListA = { &IncomingDirectedEdgeIndexListA, &IncomingDirectedEdgeIndexListB };
	//update AdjacentPointIndexListA
	auto tempIndexListA_delete = AdjacentPointIndexListA.ExactMatch(PointIndexA);
	AdjacentPointIndexListA.Delete(tempIndexListA_delete);
	auto tempIndexListB_delete = AdjacentPointIndexListA.ExactMatch(PointIndexB);
	AdjacentPointIndexListA.Delete(tempIndexListB_delete);
	//update AdjacentEdgeIndexListA
	auto tempIndexList_edge_delete = AdjacentEdgeIndexListA.ExactMatch(EdgeHandle_AB.GetIndex());
	AdjacentEdgeIndexListA.Delete(tempIndexList_edge_delete);
	// update OutgoingDirectedEdgeIndexListA
	int_max Index_out_delete = -1;
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto EdgeIndex_k = OutgoingDirectedEdgeIndexListA[k].EdgeIndex;
		auto RelativeIndex_k = OutgoingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetStartPointIndex(PointIndexA);
		if (EdgeIndex_k == EdgeHandle_AB.GetIndex())
		{
			Index_out_delete = k;
		}
	}
	if (Index_out_delete >= 0)
	{
		OutgoingDirectedEdgeIndexListA.Delete(Index_out_delete);
	}
	// update IncomingDirectedEdgeIndexListA
	int_max Index_in_delete = -1;
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto EdgeIndex_k = IncomingDirectedEdgeIndexListA[k].EdgeIndex;
		auto RelativeIndex_k = IncomingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeList()[RelativeIndex_k].SetEndPointIndex(PointIndexA);
		if (EdgeIndex_k == EdgeHandle_AB.GetIndex())
		{
			Index_in_delete = k;
		}
	}
	if (Index_in_delete >= 0)
	{
		IncomingDirectedEdgeIndexListA.Delete(Index_in_delete);
	}

	//delete any adjacency info of B
	m_MeshData->PointList[PointIndexB].AdjacentPointIndexList().Clear();
	m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList().Clear();
	m_MeshData->PointList[PointIndexB].OutgoingDirectedEdgeIndexList().Clear();
	m_MeshData->PointList[PointIndexB].IncomingDirectedEdgeIndexList().Clear();
	m_MeshData->PointList[PointIndexB].AdjacentFaceIndexList().Clear();

	// delete edge AB if it exist
	if (EdgeHandle_AB.GetIndex() >= 0)
	{
		this->InternalFuction_DeleteEdge(EdgeHandle_AB.GetIndex());
	}
	//---------------------------------------------------------------
	PointHandle_output.SetIndex(PointIndexA);
	return PointHandle_output;
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkEdgeToPoint(EdgeHandleType EdgeHandle, int_max Point_RelativeIndex)
{
	PointHandleType PointHandle;
    
    if (this->IsValidHandle(EdgeHandle) == false)
    {
        MDK_Error("Invalid EdgeHandle @ MembraneMesh::ShrinkEdgeToPoint(...)")
        PointHandle.SetToInvalid();
        return PointHandle;
    }

	if (Point_RelativeIndex < 0 || Point_RelativeIndex > 1)
	{
		MDK_Error("Invalid Point_RelativeIndex @ MembraneMesh::ShrinkEdgeToPoint(...)")
		PointHandle.SetToInvalid();
		return PointHandle;	
	}

	auto PointHandleList = m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetPointHandleList();
	if (Point_RelativeIndex == 0)
	{
		return this->MergePoint(PointHandleList[0], PointHandleList[1]);
	}
	else
	{
		return this->MergePoint(PointHandleList[1], PointHandleList[0]);
	}
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkEdgeToPoint(EdgeHandleType EdgeHandle, PointHandleType PointHandle)
{
	if (this->IsValidHandle(EdgeHandle) == false)
	{
		MDK_Error("Invalid EdgeHandle @ MembraneMesh::ShrinkEdgeToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	if (this->IsValidHandle(PointHandle) == false)
	{
		MDK_Error("Invalid PointHandle @ MembraneMesh::ShrinkEdgeToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	auto PointHandleList = m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetPointHandleList();
	if (PointHandleList[0] == PointHandle)
	{
		return this->ShrinkEdgeToPoint(EdgeHandle, 0);
	}
	else if (PointHandleList[1] == PointHandle)
	{
		return this->ShrinkEdgeToPoint(EdgeHandle, 1);
	}
	else
	{
		auto PointHandle0 = this->ShrinkEdgeToPoint(EdgeHandle, 0);
		this->SwapPoint(PointHandle0, PointHandle);
	}
}


template<typename MeshAttributeType>
DenseVector<Handle_Of_Edge_Of_MembraneMesh, 2> MembraneMesh<MeshAttributeType>::SplitEdgeByPoint(EdgeHandleType EdgeHandle, PointHandleType PointHandle)
{
	DenseVector<EdgeHandleType, 2> EdgeHandlePair;
	EdgeHandlePair[0].SetToInvalid();
	EdgeHandlePair[1].SetToInvalid();

    if (this->IsValidHandle(PointHandle) == false)
    {
		MDK_Error("Invalid PointHandle @ MembraneMesh::SplitEdgeByPoint(...)")      
		return EdgeHandlePair;
    }

	if (this->IsValidHandle(EdgeHandle) == false)
	{
		MDK_Error("Invalid EdgeHandle @ MembraneMesh::SplitEdgeByPoint(...)")
		return EdgeHandlePair;
	}

	auto PointIndexList = m_MeshData->EdgeList[EdgeHandle.GetIndex()].GetPointIndexList();
	auto PointIndex0 = PointIndexList[0];
	auto PointIndex1 = PointIndexList[1];
	auto PointIndex2 = PointHandle.GetIndex();
	//-------------------------
	//   \          /
    // ---P0---P2--P1---
	//   /          \
    //-------------------------

	if (PointIndex2 == PointIndex0 || PointIndex2 == PointIndex1)
	{
		MDK_Error("Input Point is on Edge  @ MembraneMesh::SplitEdgeByPoint(...)")
		return EdgeHandlePair;
	}

	//------------------------- input check is done --------------------------------------------//

	PointHandleType PointHandle0, PointHandle1, PointHandle2;
	PointHandle0.SetIndex(PointIndex0);
	PointHandle1.SetIndex(PointIndex1);
	PointHandle2.SetIndex(PointIndex2);

	auto EdgeHandle01 = EdgeHandle;
	auto EdgeHandle02 = this->AddEdge(PointHandle0, PointHandle2);
	auto EdgeHandle21 = this->AddEdge(PointHandle2, PointHandle1);

	auto DirectedEdgeHandleList01 = this->GetDirectedEdgeHandleByPoint(PointHandle0, PointHandle1);
	for (int_max k = 0; k < DirectedEdgeHandleList01.GetLength(); ++k)
	{
		auto DirectedEdgeIndex_k = DirectedEdgeHandleList01[k].GetIndex();
		const auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
		auto FaceIndex_k = DirectedEdge_k.GetFaceIndex();
		auto& DirectedEdgeIndexList = m_MeshData->FaceList[FaceIndex_k].DirectedEdgeIndexList();//to be updated

		int_max Index_delete = -1;
		for (int_max n = 0; n < DirectedEdgeIndexList.GetLength(); ++n)
		{
			if (DirectedEdgeIndexList[n].EdgeIndex == EdgeHandle01.GetIndex())
			{
				Index_delete = n;
				break;
			}
		}

		int_max Index_prev = Index_delete - 1;
		if (Index_prev < 0)
		{
			Index_prev = DirectedEdgeIndexList.GetLength() - 1;
		}

		int_max Index_next = Index_delete + 1;
		if (Index_next >= DirectedEdgeIndexList.GetLength())
		{
			Index_next = 0;
		}

		auto DirectedEdgeIndex_prev = DirectedEdgeIndexList[Index_prev];
		auto DirectedEdgeIndex_next = DirectedEdgeIndexList[Index_next];
		auto& DirectedEdge_prev = m_MeshData->EdgeList[DirectedEdgeIndex_prev.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_prev.RelativeIndex];
		auto& DirectedEdge_next = m_MeshData->EdgeList[DirectedEdgeIndex_next.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_next.RelativeIndex];

		//create directed edge P0P2, P2P1 for FaceIndex_k
		DirectedEdgeType DirectedEdge02, DirectedEdge21;
		DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex02, DirectedEdgeIndex21;
		DirectedEdgeIndex02.EdgeIndex = EdgeHandle02.GetIndex();
		DirectedEdgeIndex02.RelativeIndex = 0;
		DirectedEdgeIndex21.EdgeIndex = EdgeHandle21.GetIndex();
		DirectedEdgeIndex21.RelativeIndex = 0;
		//
		DirectedEdge02.SetParentMesh(*this);
		DirectedEdge02.SetIndex(DirectedEdgeIndex02);
		DirectedEdge02.SetFaceIndex(FaceIndex_k);
		DirectedEdge02.SetStartPointIndex(PointIndex0);
		DirectedEdge02.SetEndPointIndex(PointIndex2);	
		DirectedEdge02.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_prev);
		DirectedEdge02.SetNextDirectedEdgeIndex(DirectedEdgeIndex21);
		//
		DirectedEdge21.SetParentMesh(*this);
		DirectedEdge21.SetIndex(DirectedEdgeIndex21);
		DirectedEdge21.SetFaceIndex(FaceIndex_k);
		DirectedEdge21.SetStartPointIndex(PointIndex2);
		DirectedEdge21.SetEndPointIndex(PointIndex1);
		DirectedEdge21.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex02);
		DirectedEdge21.SetNextDirectedEdgeIndex(DirectedEdgeIndex_next);

		DirectedEdge_prev.SetNextDirectedEdgeIndex(DirectedEdgeIndex02);

		DirectedEdge_next.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex21);

		DirectedEdgeIndexList[Index_delete] = DirectedEdgeIndex02;
		DirectedEdgeIndexList.Insert(Index_delete+1, DirectedEdgeIndex21);

		m_MeshData->EdgeList[DirectedEdgeIndex02.EdgeIndex].DirectedEdgeList().Append(std::move(DirectedEdge02));
		m_MeshData->EdgeList[DirectedEdgeIndex21.EdgeIndex].DirectedEdgeList().Append(std::move(DirectedEdge21));
	}

	auto DirectedEdgeHandleList_P1P0 = this->GetDirectedEdgeHandleByPoint(PointHandle1, PointHandle0);
	for (int_max k = 0; k < DirectedEdgeHandleList_P1P0.GetLength(); ++k)
	{
		auto DirectedEdgeIndex_k = DirectedEdgeHandleList_P1P0[k].GetIndex();
		const auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
		auto FaceIndex_k = DirectedEdge_k.GetFaceIndex();
		auto& DirectedEdgeIndexList = m_MeshData->FaceList[FaceIndex_k].DirectedEdgeIndexList();//to be updated

		int_max Index_delete = -1;
		for (int_max n = 0; n < DirectedEdgeIndexList.GetLength(); ++n)
		{
			if (DirectedEdgeIndexList[n].EdgeIndex == EdgeHandle01.GetIndex())
			{
				Index_delete = n;
				break;
			}
		}

		int_max Index_prev = Index_delete - 1;
		if (Index_prev < 0)
		{
			Index_prev = DirectedEdgeIndexList.GetLength() - 1;
		}

		int_max Index_next = Index_delete + 1;
		if (Index_next >= DirectedEdgeIndexList.GetLength())
		{
			Index_next = 0;
		}

		auto DirectedEdgeIndex_prev = DirectedEdgeIndexList[Index_prev];
		auto DirectedEdgeIndex_next = DirectedEdgeIndexList[Index_next];
		auto& DirectedEdge_prev = m_MeshData->EdgeList[DirectedEdgeIndex_prev.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_prev.RelativeIndex];
		auto& DirectedEdge_next = m_MeshData->EdgeList[DirectedEdgeIndex_next.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_next.RelativeIndex];

		//create directed edge P1P2, P2P0
		DirectedEdgeType DirectedEdge12, DirectedEdge20;
		DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex12, DirectedEdgeIndex20;
		DirectedEdgeIndex12.EdgeIndex = EdgeHandle21.GetIndex();
		DirectedEdgeIndex12.RelativeIndex = 0;
		DirectedEdgeIndex20.EdgeIndex = EdgeHandle02.GetIndex();
		DirectedEdgeIndex20.RelativeIndex = 0;
		//
		DirectedEdge12.SetParentMesh(*this);
		DirectedEdge12.SetIndex(DirectedEdgeIndex12);
		DirectedEdge12.SetFaceIndex(FaceIndex_k);
		DirectedEdge12.SetStartPointIndex(PointIndex1);
		DirectedEdge12.SetEndPointIndex(PointIndex2);
		DirectedEdge12.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex_prev);
		DirectedEdge12.SetNextDirectedEdgeIndex(DirectedEdgeIndex20);
		//
		DirectedEdge20.SetParentMesh(*this);
		DirectedEdge20.SetIndex(DirectedEdgeIndex20);
		DirectedEdge20.SetFaceIndex(FaceIndex_k);
		DirectedEdge20.SetStartPointIndex(PointIndex2);
		DirectedEdge20.SetEndPointIndex(PointIndex0);
		DirectedEdge20.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex12);
		DirectedEdge20.SetNextDirectedEdgeIndex(DirectedEdgeIndex_next);

		DirectedEdge_prev.SetNextDirectedEdgeIndex(DirectedEdgeIndex12);

		DirectedEdge_next.SetPreviousDirectedEdgeIndex(DirectedEdgeIndex20);

		DirectedEdgeIndexList[Index_delete] = DirectedEdgeIndex12;
		DirectedEdgeIndexList.Insert(Index_delete + 1, DirectedEdgeIndex20);

		m_MeshData->EdgeList[DirectedEdgeIndex12.EdgeIndex].DirectedEdgeList().Append(std::move(DirectedEdge12));
		m_MeshData->EdgeList[DirectedEdgeIndex20.EdgeIndex].DirectedEdgeList().Append(std::move(DirectedEdge20));
	}

	//update adjacency info of P0
	auto& AdjacentPointIndexList_P0 = m_MeshData->PointList[PointIndex0].AdjacentPointIndexList();
	{
		auto tempIndex = AdjacentPointIndexList_P0.ExactMatch("first", PointIndex1);
		AdjacentPointIndexList_P0[tempIndex] = PointIndex2;
	}
	auto& AdjacentEdgeIndexList_P0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
	{
		auto tempIndex = AdjacentEdgeIndexList_P0.ExactMatch("first", EdgeHandle01.GetIndex());
		AdjacentEdgeIndexList_P0[tempIndex] = EdgeHandle02.GetIndex();
	}
	auto& OutgoingDirectedEdgeIndexList_P0 = m_MeshData->PointList[PointIndex0].OutgoingDirectedEdgeIndexList();
	{
		for (int_max k = 0; k < OutgoingDirectedEdgeIndexList_P0.GetLength(); ++k)
		{
			auto DirectedEdgeIndex_k = OutgoingDirectedEdgeIndexList_P0[k];
			auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
			if (DirectedEdge_k.GetEndPointIndex() == PointIndex1)
			{
				DirectedEdge_k.SetEndPointIndex(PointIndex2);						
			}		
		}
	}
	auto& IncomingDirectedEdgeIndexList_P0 = m_MeshData->PointList[PointIndex0].IncomingDirectedEdgeIndexList();
	{
		for (int_max k = 0; k < IncomingDirectedEdgeIndexList_P0.GetLength(); ++k)
		{
			auto DirectedEdgeIndex_k = IncomingDirectedEdgeIndexList_P0[k];
			auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
			if (DirectedEdge_k.GetStartPointIndex() == PointIndex1)
			{
				DirectedEdge_k.SetStartPointIndex(PointIndex2);
			}
		}
	}

	//update adjacency info of P1	
	auto& AdjacentPointIndexList_P1 = m_MeshData->PointList[PointIndex1].AdjacentPointIndexList();
	{
		auto tempIndex = AdjacentPointIndexList_P1.ExactMatch("first", PointIndex0);
		AdjacentPointIndexList_P1[tempIndex] = PointIndex2;
	}
	auto& AdjacentEdgeIndexList_P1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList();
	{
		auto tempIndex = AdjacentEdgeIndexList_P1.ExactMatch("first", EdgeHandle01.GetIndex());
		AdjacentEdgeIndexList_P1[tempIndex] = EdgeHandle21.GetIndex();
	}
	auto& OutgoingDirectedEdgeIndexList_P1 = m_MeshData->PointList[PointIndex1].OutgoingDirectedEdgeIndexList();
	{
		for (int_max k = 0; k < OutgoingDirectedEdgeIndexList_P1.GetLength(); ++k)
		{
			auto DirectedEdgeIndex_k = OutgoingDirectedEdgeIndexList_P1[k];
			auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
			if (DirectedEdge_k.GetEndPointIndex() == PointIndex0)
			{
				DirectedEdge_k.SetEndPointIndex(PointIndex2);
			}
		}
	}
	auto& IncomingDirectedEdgeIndexList_P1 = m_MeshData->PointList[PointIndex1].IncomingDirectedEdgeIndexList();
	{
		for (int_max k = 0; k < IncomingDirectedEdgeIndexList_P1.GetLength(); ++k)
		{
			auto DirectedEdgeIndex_k = IncomingDirectedEdgeIndexList_P1[k];
			auto& DirectedEdge_k = m_MeshData->EdgeList[DirectedEdgeIndex_k.EdgeIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
			if (DirectedEdge_k.GetStartPointIndex() == PointIndex0)
			{
				DirectedEdge_k.SetStartPointIndex(PointIndex2);
			}
		}
	}

	// delete edge P0P1
	this->InternalFuction_DeleteEdge(EdgeHandle01.GetIndex());
	//-------------------------------------------------------------------------
	EdgeHandlePair[0] = EdgeHandle02;
	EdgeHandlePair[1] = EdgeHandle21;
	return EdgeHandlePair;
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkFaceToPoint(FaceHandleType FaceHandle, int_max Point_RelativeIndex)
{
	if (this->IsValidHandle(FaceHandle) == false)
	{
		MDK_Error("Invalid FaceHandle @ MembraneMesh::ShrinkFaceToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	if (Point_RelativeIndex < 0 || Point_RelativeIndex >= m_MeshData->FaceList[FaceHandle.GetIndex()].GetPointCount())
	{
		MDK_Error("Invalid Point_RelativeIndex @ MembraneMesh::ShrinkFaceToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	auto PointHandleList = m_MeshData->FaceList[FaceHandle.GetIndex()].GetPointHandleList();

	this->DeleteFace(FaceHandle);

	for (int_max k = 1; k < PointHandleList.GetLength(); ++k)
	{
		this->MergePoint(PointHandleList[0], PointHandleList[k]);
	}

	this->SwapPoint(PointHandleList[0], PointHandleList[Point_RelativeIndex]);

	return PointHandleList[Point_RelativeIndex];
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh MembraneMesh<MeshAttributeType>::ShrinkFaceToPoint(FaceHandleType FaceHandle, PointHandleType PointHandle)
{
	if (this->IsValidHandle(FaceHandle) == false)
	{
		MDK_Error("Invalid FaceHandle @ MembraneMesh::ShrinkFaceToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	if (this->IsValidHandle(PointHandle) == false)
	{
		MDK_Error("Invalid PointHandle @ MembraneMesh::ShrinkFaceToPoint(...)")
		PointHandleType PointHandle_empty;
		return PointHandle_empty;
	}

	auto PointHandleList = m_MeshData->FaceList[FaceHandle.GetIndex()].GetPointHandleList();
	auto tempIndex = PointHandleList.ExactMatch("first", PointHandle);
	if (tempIndex >= 0)
	{
		return this->ShrinkFaceToPoint(FaceHandle, tempIndex);
	}
	else
	{
		auto PointHandle0 = this->ShrinkFaceToPoint(FaceHandle, 0);
		this->SwapPoint(PointHandle0, PointHandle);
	}
}


/*
template<typename MeshAttributeType>
Handle_Of_Face_Of_MembraneMesh MembraneMesh<MeshAttributeType>::MergeAdjacentFace(FaceHandleType FaceHandleA, FaceHandleType FaceHandleB)
{

}
*/
/*
template<typename MeshAttributeType>
DenseVector<Handle_Of_Face_Of_MembraneMesh, 2> MembraneMesh<MeshAttributeType>::SplitFaceByEdge(EdgeHandleType EdgeHandle)
{
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Face_Of_MembraneMesh, 2> MembraneMesh<MeshAttributeType>::SplitFaceByEdge(int_max EdgeID)
{
}
*/

template<typename MeshAttributeType>
template<typename ElementType>
DenseVector<ElementType> MembraneMesh<MeshAttributeType>::Intersect(const DenseVector<ElementType>& SetA, const DenseVector<ElementType>& SetB) const
{
	DenseVector<ElementType> SetC;

	if (SetA.IsEmpty() == true || SetB.IsEmpty() == true)
	{
		return SetC;
	}

	SetC.SetCapacity(std::min(SetA.GetLength(), SetB.GetLength()));

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] == SetB[n])
			{
				SetC.Append(SetA[k]);
			}
		}
	}
	return SetC;
}


template<typename MeshAttributeType>
template<typename ElementType>
DenseVector<ElementType> MembraneMesh<MeshAttributeType>::SetDiff(const DenseVector<ElementType>& SetA, const DenseVector<ElementType>& SetB) const
{// in A, NOT in B

	if (SetB.IsEmpty() == true)
	{
		return SetA;
	}

	DenseVector<ElementType> SetC;
	SetC.SetCapacity(SetA.GetLength());

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] != SetB[n])
			{
				SetC.Append(SetA[k]);
			}
		}
	}
	return SetC;
}

//----------------------------------- private InternalFunction ---------------------------------------------------------//

template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeletePoint(int_max PointIndex)
{
	auto ID = m_MeshData->PointList[PointIndex].GetID();
	if (ID >= 0)
	{
		auto it = m_MeshData->Map_PointID_to_PointIndex.find(ID);
		if (it != m_MeshData->Map_PointID_to_PointIndex.end())
		{
			m_MeshData->Map_PointID_to_PointIndex.erase(it);
		}
	}

    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

	m_MeshData->PointList[PointIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteEdge(int_max EdgeIndex)
{
    // Delete EdgeID record if the map has it 
	auto ID = m_MeshData->EdgeList[EdgeIndex].GetID();
	if (ID >= 0)
	{
		auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(ID);
		if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
		{
			m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
		}
	}

    // Delete Edge: only release memory, not remove from EdgeList
	m_MeshData->EdgeList[EdgeIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
}


template<typename MeshAttributeType>
void MembraneMesh<MeshAttributeType>::InternalFuction_DeleteFace(int_max FaceIndex)
{
	// delete DirectedEdgeID record if the map has it 
	for (int_max k = 0; k < m_MeshData->FaceList[FaceIndex].DirectedEdgeList().GetLength(); ++k)
	{
		// auto& take reference
		auto& DirectedEdge_k = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[k];
		auto ID = DirectedEdge_k.GetID();
		if (ID >= 0)
		{
			auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(ID);
			if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
			{
				m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
			}
		}		
	}

    // delete FaceID record if the map has it 
	auto ID = m_MeshData->FaceList[FaceIndex].GetID();
	if (ID >= 0)
	{
		auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(ID);
		if (it != m_MeshData->Map_FaceID_to_FaceIndex.end())
		{
			m_MeshData->Map_FaceID_to_FaceIndex.erase(it);
		}
	}

    // Delete Face : only clear memory, not change the length of FaceList
	m_MeshData->FaceList[FaceIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->FaceValidityFlagList[FaceIndex] = 0;
}

}// namespace mdk

#endif