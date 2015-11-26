#ifndef mdk_MembraneMeshItem_hpp
#define mdk_MembraneMeshItem_hpp


namespace mdk
{
//=========================================================== Point_Of_MembraneMesh ===========================================================//

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>::Point_Of_MembraneMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>::Point_Of_MembraneMesh(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint)
{
    (*this) = InputPoint;
}

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>::Point_Of_MembraneMesh(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
Point_Of_MembraneMesh<MeshAttributeType>::~Point_Of_MembraneMesh()
{
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::operator=(const Point_Of_MembraneMesh<MeshAttributeType>& InputPoint)
{
    if (!InputPoint.m_Data)
    {
		m_Data.reset();
        return;
    }
	
	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Point_Of_MembraneMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputPoint.m_Data->Mesh);
    m_Data->Index = InputPoint.m_Data->Index; 
    m_Data->ID = InputPoint.m_Data->ID;
    m_Data->AdjacentPointIndexList = InputPoint.m_Data->AdjacentPointIndexList;
    m_Data->AdjacentEdgeIndexList = InputPoint.m_Data->AdjacentEdgeIndexList;
    m_Data->AdjacentFaceIndexList = InputPoint.m_Data->AdjacentFaceIndexList;
    m_Data->Attribute = InputPoint.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::operator=(Point_Of_MembraneMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_MembraneMesh<MeshAttributeType>>();
    }

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->AdjacentPointIndexList.Clear();
    m_Data->AdjacentEdgeIndexList.Clear();
    m_Data->AdjacentFaceIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetParentMesh(MembraneMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentPointIndexList()
{
    return m_Data->AdjacentPointIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentPointIndexList() const
{
	return m_Data->AdjacentPointIndexList;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentEdgeIndexList() const
{
	return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentFaceIndexList()
{
    return m_Data->AdjacentFaceIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<int_max>& Point_Of_MembraneMesh<MeshAttributeType>::AdjacentFaceIndexList() const
{
	return m_Data->AdjacentFaceIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<DirectedEdgeIndex_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutgoingDirectedEdgeIndexList;
	OutgoingDirectedEdgeIndexList.SetCapacity(m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		const auto& DirectedEdgeIndexList_k = m_Data->Mesh.m_MeshData->EdgeList[m_Data->AdjacentEdgeIndexList[k]].DirectedEdgeIndexList();
		for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
		{
			auto DirectedEdgeIndex_n = DirectedEdgeIndexList_k[n];
			const auto& DirectedEdge_n = m_Data->Mesh.m_MeshData->FaceList[DirectedEdgeIndex_n.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_n.RelativeIndex];
			auto PointIndex_start = DirectedEdge_n.GetStartPointIndex();			
			if (PointIndex_start == m_Data->Index)
			{
				OutgoingDirectedEdgeIndexList.Append(DirectedEdgeIndex_n);
			}
		}
	}
	return OutgoingDirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<DirectedEdgeIndex_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_MembraneMesh> IncomingDirectedEdgeIndexList;
	IncomingDirectedEdgeIndexList.SetCapacity(m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		const auto& DirectedEdgeIndexList_k = m_Data->Mesh.m_MeshData->EdgeList[m_Data->AdjacentEdgeIndexList[k]].DirectedEdgeIndexList();
		for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
		{
			auto DirectedEdgeIndex_n = DirectedEdgeIndexList_k[n];
			const auto& DirectedEdge_n = m_Data->Mesh.m_MeshData->FaceList[DirectedEdgeIndex_n.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_n.RelativeIndex];
			auto PointIndex_end = DirectedEdge_n.GetEndPointIndex();
			if (PointIndex_end == m_Data->Index)
			{
				IncomingDirectedEdgeIndexList.Append(DirectedEdgeIndex_n);
			}
		}
	}
	return IncomingDirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::Update_AdjacentPointIndexList_Given_AdjacentEdgeIndexList()
{
	m_Data->AdjacentPointIndexList.Clear();
	m_Data->AdjacentPointIndexList.Resize(m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->AdjacentEdgeIndexList[k];
		int_max PointIndex0, PointIndex1;
		m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].GetPointIndexList(PointIndex0, PointIndex1);
		if (PointIndex0 == m_Data->Index)
		{
			m_Data->AdjacentPointIndexList[k] = PointIndex1;
		}
		else if (PointIndex1 == m_Data->Index)
		{
			m_Data->AdjacentPointIndexList[k] = PointIndex0;
		}
		else
		{
			MDK_Error("Wrong @ Point_Of_MembraneMesh::Update_AdjacentPointIndexList_Given_AdjacentEdgeIndexList()")
		}
	}
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::Update_AdjacentFaceIndexList_Given_AdjacentEdgeIndexList()
{
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
	m_Data->AdjacentFaceIndexList.Clear();
	m_Data->AdjacentFaceIndexList.Resize(OutgoingDirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexList.GetLength(); ++k)
	{
		m_Data->AdjacentFaceIndexList[k] = OutgoingDirectedEdgeIndexList[k].FaceIndex;
	}
}

template<typename MeshAttributeType>
inline
bool Point_Of_MembraneMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool Point_Of_MembraneMesh<MeshAttributeType>::IsOnEdge() const
{
    return (m_Data->AdjacentEdgeIndexList.IsEmpty() == false);
}

template<typename MeshAttributeType>
inline
bool Point_Of_MembraneMesh<MeshAttributeType>::IsOnBoundaryEdge() const
{
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        auto EdgeIndex = m_Data->AdjacentEdgeIndexList[k];
        if (m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex].IsBoundary() == true)
        {
            return true;
        }
    }
    return false;
}

template<typename MeshAttributeType>
inline
MembraneMesh<MeshAttributeType>& Point_Of_MembraneMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const MembraneMesh<MeshAttributeType>& Point_Of_MembraneMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh Point_Of_MembraneMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;
    PointHandle.SetIndex(m_Data->Index);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetID(int_max PointID)
{
	if (!m_Data)
	{
		MDK_Error("this point has been deleted @ Point_Of_MembraneMesh::SetID(...)")
		return;
	}

    if (PointID < 0)
    {
        MDK_Error("PointID < 0 @ Point_Of_MembraneMesh::SetID(...)")
        return;
    }

    // check record
    auto PointID_old = m_Data->ID;
    if (PointID_old != PointID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(PointID);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            MDK_Error("Input PointID has already been used for another point @ Point_Of_MembraneMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(PointID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex[PointID] = m_Data->Index;
        m_Data->ID = PointID;
    }
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::EraseID()
{   
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.erase(it);
        }

        m_Data->ID = -1;
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetPosition(const DenseVector<ScalarType, 3>& Pos)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos.GetPointer());
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetPosition(const ScalarType Pos[3])
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::SetPosition(ScalarType x, ScalarType y, ScalarType z)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> Point_Of_MembraneMesh<MeshAttributeType>::GetPosition() const
{
    DenseVector<ScalarType, 3> Pos;
    this->GetPosition(Pos.GetPointer());
    return Pos;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetPosition(ScalarType Pos[3]) const
{
    m_Data->Mesh.m_MeshData->PointPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentPointCount() const
{
	return m_Data->AdjacentPointIndexList.GetLength();
}


template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentPointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentPointIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentPointIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentPointIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentPointIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentPointIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(m_Data->AdjacentPointIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentPointIndexList.GetLength(); ++k)
    {
        OutputIDList[k] = m_Data->Mesh.m_MeshData->PointList[m_Data->AdjacentPointIndexList[k]].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeCount() const
{
	return m_Data->AdjacentEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentEdgeIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        OutputIDList[k]= m_Data->Mesh.m_MeshData->EdgeList[m_Data->AdjacentEdgeIndexList[k]].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeCount() const
{
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
	return OutgoingDirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
    this->GetOutgoingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const
{
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
    OutputHandleList.FastResize(OutgoingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(OutgoingDirectedEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetOutgoingDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
    OutputHandleList.FastResize(OutgoingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < OutgoingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto FaceIndex = OutgoingDirectedEdgeIndexList[k].FaceIndex;
        auto RelativeIndex = OutgoingDirectedEdgeIndexList[k].RelativeIndex;
        OutputIDList[k] = m_Data->Mesh.m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeCount() const
{
	auto IncomingDirectedEdgeIndexList = this->GetIncomingDirectedEdgeIndexList();
	return IncomingDirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
    this->GetIncomingDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const
{
	auto IncomingDirectedEdgeIndexList = this->GetIncomingDirectedEdgeIndexList();
    OutputHandleList.FastResize(IncomingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(IncomingDirectedEdgeIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetIncomingDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetIncomingDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
	auto IncomingDirectedEdgeIndexList = this->GetIncomingDirectedEdgeIndexList();
    OutputHandleList.FastResize(IncomingDirectedEdgeIndexList.GetLength());
    for (int_max k = 0; k < IncomingDirectedEdgeIndexList.GetLength(); ++k)
    {
        auto FaceIndex = IncomingDirectedEdgeIndexList[k].FaceIndex;
        auto RelativeIndex = IncomingDirectedEdgeIndexList[k].RelativeIndex;
        OutputIDList[k] = m_Data->Mesh.m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	return m_Data->AdjacentFaceIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList() const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentFaceHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentFaceIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentFaceIndexList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->AdjacentFaceIndexList[k]);
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentFaceIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Point_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputHandleList.FastResize(m_Data->AdjacentFaceIndexList.GetLength());
    for (int_max k = 0; k < m_Data->AdjacentFaceIndexList.GetLength(); ++k)
    {
        OutputIDList[k] = m_Data->Mesh.m_MeshData->FaceList[m_Data->AdjacentFaceIndexList[k]].GetID();
    }
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::PointAttributeType& Point_Of_MembraneMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::PointAttributeType& Point_Of_MembraneMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Edge_Of_MembraneMesh ===========================================================//

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>::Edge_Of_MembraneMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>::Edge_Of_MembraneMesh(const Edge_Of_MembraneMesh<MeshAttributeType>& InputEdge)
{
    (*this) = InputEdge;
}

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>::Edge_Of_MembraneMesh(Edge_Of_MembraneMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
Edge_Of_MembraneMesh<MeshAttributeType>::~Edge_Of_MembraneMesh()
{
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::operator=(const Edge_Of_MembraneMesh<MeshAttributeType>& InputEdge)
{
    if (!InputEdge.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Edge_Of_MembraneMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputEdge.m_Data->Mesh);
    m_Data->Index = InputEdge.m_Data->Index;
    m_Data->ID = InputEdge.m_Data->ID;
    m_Data->PointIndex0 = InputEdge.m_Data->PointIndex0;
    m_Data->PointIndex1 = InputEdge.m_Data->PointIndex1;
	m_Data->DirectedEdgeIndexList = InputEdge.m_Data->DirectedEdgeIndexList;
    m_Data->Attribute = InputEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::operator=(Edge_Of_MembraneMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_MembraneMesh<MeshAttributeType>>();
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->PointIndex0 = -1;
    m_Data->PointIndex1 = -1;
	m_Data->DirectedEdgeIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::SetParentMesh(MembraneMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::SetPointIndexList(const int_max PointIndexList[2])
{
    this->SetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& Edge_Of_MembraneMesh<MeshAttributeType>::DirectedEdgeIndexList()
{
	return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& Edge_Of_MembraneMesh<MeshAttributeType>::DirectedEdgeIndexList() const
{
	return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const
{
    PointIndex0 = m_Data->PointIndex0;
    PointIndex1 = m_Data->PointIndex1;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetPointIndexList(int_max PointIndexList[2]) const
{
    this->GetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max, 2> Edge_Of_MembraneMesh<MeshAttributeType>::GetPointIndexList() const
{
	DenseVector<int_max, 2> PointIndexList;
	PointIndexList[0] = m_Data->PointIndex0;
	PointIndexList[1] = m_Data->PointIndex1;
	return PointIndexList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::SetPointIndexList(int_max PointIndex0, int_max PointIndex1)
{
    m_Data->PointIndex0 = PointIndex0;
    m_Data->PointIndex1 = PointIndex1;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_MembraneMesh<MeshAttributeType>::SetPointIndexList(const DenseVector<int_max, 2>& PointIndexList)
{
	m_Data->PointIndex0 = PointIndexList[0];
	m_Data->PointIndex1 = PointIndexList[1];
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetAdjacentEdgeIndexList();

    OutputIndexList.FastResize(0);
    OutputIndexList.SetCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIndexList.Append(tempIndex);
        }
    }
}

// Face share this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentFaceIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(0);
    OutputIndexList.SetCapacity(2);
	for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
	{
		OutputIndexList.Append(m_Data->DirectedEdgeIndexList[k].FaceIndex);
	}
}

// Face share any  point of this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetNeighbourFaceIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIndexList(DenseVector<int_max>& OutputIndexList) const
{
    const auto& AdjacentFaceIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentFaceIndexList();
	const auto& AdjacentFaceIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentFaceIndexList();

    OutputIndexList.FastResize(0);

    if (AdjacentFaceIndexList0.IsEmpty() == true && AdjacentFaceIndexList1.IsEmpty() == true)
    {
        return;
    }

    OutputIndexList.SetCapacity(AdjacentFaceIndexList0.GetLength() + AdjacentFaceIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentFaceIndexList0.GetLength(); ++k)
    {
        OutputIndexList.Append(AdjacentFaceIndexList0[k]);
    }

    for (int_max k = 0; k < AdjacentFaceIndexList1.GetLength(); ++k)
    {
        OutputIndexList.Append(AdjacentFaceIndexList1[k]);
    }

    auto tempIndexList = OutputIndexList.FindUnique();
    OutputIndexList = OutputIndexList.GetSubSet(tempIndexList);
}

template<typename MeshAttributeType>
inline
bool Edge_Of_MembraneMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool Edge_Of_MembraneMesh<MeshAttributeType>::IsBoundary() const
{
	return (m_Data->DirectedEdgeIndexList.GetLength() <= 1);
}

template<typename MeshAttributeType>
inline
MembraneMesh<MeshAttributeType>& Edge_Of_MembraneMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const MembraneMesh<MeshAttributeType>& Edge_Of_MembraneMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh Edge_Of_MembraneMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->Index);
    return EdgeHandle;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::SetID(int_max EdgeID)
{
	if (!m_Data)
	{
		MDK_Error("Edge has been deleted @ Edge_Of_MembraneMesh::SetID(...)")
		return;
	}

    if (EdgeID < 0)
    {
        MDK_Error("EdgeID < 0 @ Edge_Of_MembraneMesh::SetID(...)")
        return;
    }

    // check record
    auto EdgeID_old = m_Data->ID;
    if (EdgeID_old != EdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            MDK_Error("Input EdgeID has already been used for another edge @ Edge_Of_MembraneMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex[EdgeID] = m_Data->Index;
        m_Data->ID = EdgeID;
    }
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.erase(it);
        }

        m_Data->ID = -1;
    }
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_MembraneMesh, 2> Edge_Of_MembraneMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh, 2> OutputHandleList;
    OutputHandleList[0].SetIndex(m_Data->PointIndex0);
    OutputHandleList[1].SetIndex(m_Data->PointIndex1);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::
GetPointHandleList(Handle_Of_Point_Of_MembraneMesh& PointHandle0, Handle_Of_Point_Of_MembraneMesh& PointHandle1) const
{
    PointHandle0.SetIndex(m_Data->PointIndex0);
    PointHandle1.SetIndex(m_Data->PointIndex1);
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max, 2> Edge_Of_MembraneMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max, 2> PointIDList;
    this->GetPointIDList(PointIDList[0], PointIDList[1]);
    return PointIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetPointIDList(int_max& PointID0, int_max& PointID1) const
{
    PointID0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetID();
    PointID1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetID();
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeCount() const
{
	return DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline 
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> Edge_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
	this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
	{
		OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k]);
	}
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeCount() const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();
	return AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_MembraneMesh> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentEdgeHandleList(OutputHandleList);
	return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();

    OutputHandleList.FastResize(0);
    OutputHandleList.SetCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            EdgeHandle.SetIndex(tempIndex);
            OutputHandleList.Append(EdgeHandle);
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            EdgeHandle.SetIndex(tempIndex);
            OutputHandleList.Append(EdgeHandle);
        }
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
    const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();

    OutputIDList.FastResize(0);
    OutputIDList.SetCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;

    for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList0[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIDList.Append(m_Data->Mesh.m_MeshData->EdgeList[tempIndex].GetID());
        }
    }

    for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
    {
        auto tempIndex = AdjacentEdgeIndexList1[k];
        if (tempIndex != m_Data->Index)
        {
            OutputIDList.Append(m_Data->Mesh.m_MeshData->EdgeList[tempIndex].GetID());
        }
    }
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	return m_Data->DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList() const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentFaceHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{    
	OutputHandleList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
	{				
		OutputHandleList[k].SetIndex(m_Data->DirectedEdgeIndexList[k].FaceIndex);
	}
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentFaceIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(m_Data->DirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
	{
		auto FaceIndex_k = m_Data->DirectedEdgeIndexList[k].GetFaceIndex();		
		OutputIDList[k] = m_Data->Mesh.FaceList[FaceIndex_k].GetID();
	}
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{
	// wrong, may share more than 2 face
	//auto Counter0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentFaceCount();
	//auto Counter1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetAdjacentFaceCount();
	//return Counter0 + Counter1 - 2;

	auto HandleList = this->GetNeighbourFaceHandleList();
	return HandleList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceHandleList() const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> OutputHandleList;
    this->GetNeighbourFaceHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{
    const auto& AdjacentFaceIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentFaceIndexList();
    const auto& AdjacentFaceIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentFaceIndexList();

    OutputHandleList.FastResize(0);

    if (AdjacentFaceIndexList0.IsEmpty() == true && AdjacentFaceIndexList1.IsEmpty() == true)
    {
        return;
    }

    Handle_Of_Face_Of_MembraneMesh FaceHandle;

    OutputHandleList.SetCapacity(AdjacentFaceIndexList0.GetLength() + AdjacentFaceIndexList1.GetLength());

    for (int_max k = 0; k < AdjacentFaceIndexList0.GetLength(); ++k)
    {
        FaceHandle.SetIndex(AdjacentFaceIndexList0[k]);
        OutputHandleList.Append(FaceHandle);
    }

    for (int_max k = 0; k < AdjacentFaceIndexList1.GetLength(); ++k)
    {
        FaceHandle.SetIndex(AdjacentFaceIndexList1[k]);
        OutputHandleList.Append(FaceHandle);
    }

    auto tempIndexList = OutputHandleList.FindUnique();
    OutputHandleList = OutputHandleList.GetSubSet(tempIndexList);
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetNeighbourFaceIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIDList(DenseVector<int_max>& OutputIDList) const
{
	auto NeighbourFaceIndexList = this->GetNeighbourFaceIndexList();
	OutputIDList.FastResize(NeighbourFaceIndexList.GetLength());
	for (int_max k = 0; k < NeighbourFaceIndexList.GetLength(); ++k)
	{
		OutputIDList[k] = m_Data->Mesh.m_MeshData->FaceList[NeighbourFaceIndexList[k]].GetID();
	}
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::EdgeAttributeType& Edge_Of_MembraneMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::EdgeAttributeType& Edge_Of_MembraneMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== DirectedEdge_Of_MembraneMesh ===========================================================//

template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>::DirectedEdge_Of_MembraneMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>::DirectedEdge_Of_MembraneMesh(const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& InputDirectedEdge)
{
    (*this) = InputDirectedEdge;
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>::DirectedEdge_Of_MembraneMesh(DirectedEdge_Of_MembraneMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_MembraneMesh<MeshAttributeType>::~DirectedEdge_Of_MembraneMesh()
{
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::operator=(const DirectedEdge_Of_MembraneMesh<MeshAttributeType>& InputDirectedEdge)
{
    if (!InputDirectedEdge.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputDirectedEdge.m_Data->Mesh);
    m_Data->Index = InputDirectedEdge.m_Data->Index;
    m_Data->ID = InputDirectedEdge.m_Data->ID;
    m_Data->EdgeIndex = InputDirectedEdge.m_Data->EdgeIndex;
    m_Data->PointIndex_start = InputDirectedEdge.m_Data->PointIndex_start;
    m_Data->PointIndex_end = InputDirectedEdge.m_Data->PointIndex_end;
    m_Data->Attribute = InputDirectedEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::operator=(DirectedEdge_Of_MembraneMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>>();        
    }    

    m_Data->ID = -1;
    m_Data->Index.FaceIndex = -1;
    m_Data->Index.RelativeIndex = -1;
    m_Data->EdgeIndex = -1;
    m_Data->PointIndex_start = -1;
    m_Data->PointIndex_end = -1;

    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetParentMesh(MembraneMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetIndex(int_max FaceIndex, int_max RelativeIndex)
{
    m_Data->Index.FaceIndex = FaceIndex;
    m_Data->Index.RelativeIndex = RelativeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_Data->EdgeIndex = EdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetStartPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_start = PointIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetEndPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_end = PointIndex;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFaceIndex() const
{
	return m_Data->Index.FaceIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEdgeIndex() const
{
	return m_Data->EdgeIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetStartPointIndex() const
{
    return m_Data->PointIndex_start;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEndPointIndex() const
{
    return m_Data->PointIndex_end;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNextDirectedEdgeIndex() const
{
	auto DirectedEdgeCount = m_Data->Mesh.m_MeshData->FaceList[m_Data->Index.FaceIndex].DirectedEdgeList().GetLength();
	DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex_next;
	DirectedEdgeIndex_next.FaceIndex = m_Data->Index.FaceIndex;
	DirectedEdgeIndex_next.RelativeIndex = m_Data->Index.RelativeIndex + 1;
	if (DirectedEdgeIndex_next.RelativeIndex >= DirectedEdgeCount)
	{
		DirectedEdgeIndex_next.RelativeIndex = 0;
	}
	return DirectedEdgeIndex_next;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetPreviousDirectedEdgeIndex() const
{
	auto DirectedEdgeCount = m_Data->Mesh.m_MeshData->FaceList[m_Data->Index.FaceIndex].DirectedEdgeList().GetLength();
	DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex_prev;
	DirectedEdgeIndex_prev.FaceIndex = m_Data->Index.FaceIndex;
	DirectedEdgeIndex_prev.RelativeIndex = m_Data->Index.RelativeIndex + 1;
	if (DirectedEdgeIndex_prev.RelativeIndex >= DirectedEdgeCount)
	{
		DirectedEdgeIndex_prev.RelativeIndex = 0;
	}
	return DirectedEdgeIndex_prev;
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_MembraneMesh> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFriendDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutputIndexList;
	auto DirectedEdgeCountMax = m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList().GetLength();
	OutputIndexList.SetCapacity(DirectedEdgeCountMax);
	for (int_max k = 0; k < DirectedEdgeCountMax; ++k)
	{
		auto DirectedEdgeIndex_k = m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList()[k];
		if (m_Data->Index != DirectedEdgeIndex_k)
		{
			OutputIndexList.Append(DirectedEdgeIndex_k);
		}
	}
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceIndexList();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceIndexList();
}

template<typename MeshAttributeType>
inline
bool DirectedEdge_Of_MembraneMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index.FaceIndex < 0 || m_Data->Index.RelativeIndex < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
bool DirectedEdge_Of_MembraneMesh<MeshAttributeType>::IsBoundary() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].IsBoundary();
}

template<typename MeshAttributeType>
inline
MembraneMesh<MeshAttributeType>& DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const MembraneMesh<MeshAttributeType>& DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline 
Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle;
    DirectedEdgeHandle.SetIndex(m_Data->Index);
    return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::SetID(int_max DirectedEdgeID)
{
    if (DirectedEdgeID < 0)
    {
        MDK_Error("DirectedEdgeID < 0 @ DirectedEdge_Of_MembraneMesh::SetID(...)")
        return;
    }

    // check record
    auto DirectedEdgeID_old = m_Data->ID;
    if (DirectedEdgeID_old != DirectedEdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            MDK_Error("Input DirectedEdgeID has already been used for another directed-edge @ DirectedEdge_Of_MembraneMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID] = m_Data->Index;
        m_Data->ID = DirectedEdgeID;
    }
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_MembraneMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
        }

        m_Data->ID = -1;
    }
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
Handle_Of_Face_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFaceHandle() const
{
    Handle_Of_Face_Of_MembraneMesh FaceHandle;
    FaceHandle.SetIndex(m_Data->Index.FaceIndex);
    return FaceHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFaceID() const
{
    return m_Data->Mesh.m_MeshData->FaceList[m_Data->Index.FaceIndex].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetStartPointHandle() const
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;
    PointHandle.SetIndex(m_Data->PointIndex_start);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetStartPointID() const
{
    return m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex_start].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_Point_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEndPointHandle() const
{
    Handle_Of_Point_Of_MembraneMesh PointHandle;
    PointHandle.SetIndex(m_Data->PointIndex_end);
    return PointHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEndPointID() const
{
    return m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex_end].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEdgeHandle() const
{
    Handle_Of_Edge_Of_MembraneMesh EdgeHandle;
    EdgeHandle.SetIndex(m_Data->EdgeIndex);
    return EdgeHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetEdgeID() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNextDirectedEdgeHandle() const
{
	Handle_Of_DirectedEdge_Of_MembraneMesh  DirectedEdgeHandle;
	DirectedEdgeHandle.SetIndex(m_Data->NextDirectedEdgeIndex);
	return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNextDirectedEdgeID() const
{
	auto FaceIndex = m_Data->NextDirectedEdgeIndex.FaceIndex;
	auto RelativeIndex = m_Data->NextDirectedEdgeIndex.RelativeIndex;
	return m_Data->Mesh.m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex].GetID();
}

template<typename MeshAttributeType>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetPreviousDirectedEdgeHandle() const
{
	Handle_Of_DirectedEdge_Of_MembraneMesh  DirectedEdgeHandle;
	DirectedEdgeHandle.SetIndex(m_Data->PreviousDirectedEdgeIndex);
	return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetPreviousDirectedEdgeID() const
{
	auto FaceIndex = m_Data->PreviousDirectedEdgeIndex.FaceIndex;
	auto RelativeIndex = m_Data->PreviousDirectedEdgeIndex.RelativeIndex;
	return m_Data->Mesh.m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex].GetID();
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFriendDirectedEdgeCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList().GetLength()-1;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFriendDirectedEdgeHandleList() const
{
	DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
	auto DirectedEdgeCountMax = m_Data->Mesh.m_MeshData->EdgeList[m_Data->Index.EdgeIndex].DirectedEdgeIndexList().GetLength();
	OutputIndexList.SetCapacity(DirectedEdgeCountMax);
	for (int_max k = 0; k < DirectedEdgeCountMax; ++k)
	{
		auto DirectedEdgeIndex_k = m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList()[k];
		if (m_Data->Index != DirectedEdgeIndex_k)
		{
			Handle_Of_DirectedEdge_Of_MembraneMesh  DirectedEdgeHandle;
			DirectedEdgeHandle.SetIndex(DirectedEdgeIndex_k);
			OutputHandleList.Append(DirectedEdgeHandle);
		}
	}
	return OutputHandleList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetFriendDirectedEdgeIDList() const
{
	DenseVector<int_max> OutputIDList;
	auto DirectedEdgeCountMax = m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList().GetLength();
	OutputIDList.SetCapacity(DirectedEdgeCountMax);
	for (int_max k = 0; k < DirectedEdgeCountMax; ++k)
	{
		auto DirectedEdgeIndex_k = m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList()[k];		
		if (m_Data->Index.RelativeIndex != DirectedEdgeIndex_k)
		{
			auto FaceIndex_k = DirectedEdgeIndex_k.FaceIndex;
			auto RelativeIndex_k = DirectedEdgeIndex_k.RelativeIndex;
			const auto& DirectedEdge_k = m_Data->Mesh.m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList()[RelativeIndex_k];
			OutputIDList.Append(DirectedEdge_k.GetID());
		}
	}
	return OutputIDList;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceCount();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceHandleList();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceHandleList();
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceCount();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceHandleList() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceHandleList();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIDList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceIDList();
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_MembraneMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline 
const typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_MembraneMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Face_Of_MembraneMesh ===========================================================//

template<typename MeshAttributeType>
inline
Face_Of_MembraneMesh<MeshAttributeType>::Face_Of_MembraneMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Face_Of_MembraneMesh<MeshAttributeType>::Face_Of_MembraneMesh(const Face_Of_MembraneMesh<MeshAttributeType>& InputFace)
{
    (*this) = InputFace;
}

template<typename MeshAttributeType>
inline
Face_Of_MembraneMesh<MeshAttributeType>::Face_Of_MembraneMesh(Face_Of_MembraneMesh<MeshAttributeType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename MeshAttributeType>
inline
Face_Of_MembraneMesh<MeshAttributeType>::~Face_Of_MembraneMesh()
{
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::operator=(const Face_Of_MembraneMesh<MeshAttributeType>& InputFace)
{
    if (!InputFace.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Face_Of_MembraneMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputFace.m_Data->Mesh);
    m_Data->Index = InputFace.m_Data->Index;
    m_Data->ID = InputFace.m_Data->ID;
    m_Data->DirectedEdgeList = InputFace.m_Data->DirectedEdgeList;
    m_Data->Attribute = InputFace.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::operator=(Face_Of_MembraneMesh<MeshAttributeType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Face_Of_MembraneMesh<MeshAttributeType>>();
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
    m_Data->DirectedEdgeList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::SetParentMesh(MembraneMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		m_Data->DirectedEdgeList[k].SetParentMesh(InputMesh);
	}
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::SetIndex(int_max FaceIndex)
{
    m_Data->Index = FaceIndex;
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		m_Data->DirectedEdgeList[k].SetIndex(FaceIndex, k);
	}
}

template<typename MeshAttributeType>
inline
StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& Face_Of_MembraneMesh<MeshAttributeType>::DirectedEdgeList()
{
	return m_Data->DirectedEdgeList;
}

template<typename MeshAttributeType>
inline
const StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>& Face_Of_MembraneMesh<MeshAttributeType>::DirectedEdgeList() const
{
	return m_Data->DirectedEdgeList;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetPointIndexList() const
{
    DenseVector<int_max> IndexList;
    this->GetPointIndexList(IndexList);
    return IndexList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetPointIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(m_Data->DirectedEdgeList.GetLength());
    for (int_max k = 0; k < OutputIndexList.GetLength(); ++k)
    {
        OutputIndexList[k] = m_Data->DirectedEdgeList[k].GetStartPointIndex();
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetEdgeIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeIndexList(DenseVector<int_max>& OutputIndexList) const
{
    int_max EdgeCount = m_Data->DirectedEdgeList.GetLength();
    OutputIndexList.FastResize(EdgeCount);
    for (int_max k = 0; k < EdgeCount; ++k)
    {
        OutputIndexList[k] = m_Data->DirectedEdgeList[k].GetEdgeIndex();
    }
}

template<typename MeshAttributeType>
inline 
DenseVector<DirectedEdgeIndex_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_MembraneMesh> OutputIndexList;
	this->GetDirectedEdgeIndexList(OutputIndexList);
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
void Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeIndexList(DenseVector<DirectedEdgeIndex_Of_MembraneMesh>& OutputIndexList) const
{
	OutputIndexList.FastResize(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		OutputIndexList[k].FaceIndex = m_Data->Index;
		OutputIndexList[k].RelativeIndex = k;
	}
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
    DenseVector<int_max> OutputIndexList;
    this->GetAdjacentFaceIndexList(OutputIndexList);
    return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIndexList(DenseVector<int_max>& OutputIndexList) const
{
    OutputIndexList.FastResize(0);
	OutputIndexList.SetCapacity(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
    {   
		auto EdgeIndex_k = m_Data->DirectedEdgeList[k].GetEdgeIndex();	
		int_max MaxCount = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().GetLength();
		for (int_max n = 0; n < MaxCount; ++n)
		{
			auto DirectedEdgeIndex_n = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList()[n];
			if (DirectedEdgeIndex_n.FaceIndex != m_Data->Index)
			{
				OutputIndexList.Append(DirectedEdgeIndex_n.FaceIndex);
			}
		}
    }    
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	this->GetNeighbourFaceIndexList(OutputIndexList);
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIndexList(DenseVector<int_max>& OutputIndexList) const
{
	OutputIndexList.FastResize(0);
	OutputIndexList.SetCapacity(6);
	auto PointIndexList = this->GetPointIndexList();
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		const auto& FaceIndexList_k = m_Data->Mesh.m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList();
		for (int_max n = 0; n < FaceIndexList_k.GetLength(); ++n)
		{
			if (FaceIndexList_k[n] != m_Data->Index)
			{
				OutputIndexList.Append(FaceIndexList_k[n]);
			}
		}
	}
	OutputIndexList = OutputIndexList.GetSubSet(OutputIndexList.FindUnique());
}

template<typename MeshAttributeType>
inline
bool Face_Of_MembraneMesh<MeshAttributeType>::IsValid() const
{
    if (!m_Data)
    {
        return false;
    }
    else
    {
        if (m_Data->Index < 0)
        {
            return false;
        }
    }

    return true;
}

template<typename MeshAttributeType>
inline
MembraneMesh<MeshAttributeType>& Face_Of_MembraneMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const MembraneMesh<MeshAttributeType>& Face_Of_MembraneMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
Handle_Of_Face_Of_MembraneMesh Face_Of_MembraneMesh<MeshAttributeType>::GetHandle() const
{
    Handle_Of_Face_Of_MembraneMesh FaceHandle;
    FaceHandle.SetIndex(m_Data->Index);
    return FaceHandle;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::SetID(int_max FaceID)
{
    if (FaceID < 0)
    {
        MDK_Error("FaceID < 0 @ Face_Of_MembraneMesh::SetID(...)")
        return;
    }

    // check record
    auto FaceID_old = m_Data->ID;
    if (FaceID_old != FaceID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
        if (it != m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.end())
        {
            MDK_Error("Input FaceID has already been used for another cell @ Face_Of_MembraneMesh::SetID(...)")
            return;
        }

        it = m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID_old);
        if (it != m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.erase(it);
        }

        m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex[FaceID] = m_Data->Index;
        m_Data->ID = FaceID;
    }
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::EraseID()
{
    if (m_Data->ID >= 0)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.find(m_Data->ID);
        if (it != m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.end())
        {
            m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.erase(it);
        }

        m_Data->ID = -1;
    }
}

template<typename MeshAttributeType>
inline 
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline int_max Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleList() const
{
    DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh> OutputHandleList;
    this->GetDirectedEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleList(DenseVector<Handle_Of_DirectedEdge_Of_MembraneMesh>& OutputHandleList) const
{
    OutputHandleList.FastResize(m_Data->DirectedEdgeList.GetLength());
    for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeList[k].GetIndex());
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetDirectedEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    OutputIDList.FastResize(m_Data->DirectedEdgeList.GetLength());
    for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
    {
		OutputIDList[k] = m_Data->DirectedEdgeList[k].GetID();
    }
}

template<typename MeshAttributeType>
inline
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetPointCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetPointHandleList() const
{
    DenseVector<Handle_Of_Point_Of_MembraneMesh> OutputHandleList;
    this->GetPointHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetPointHandleList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputHandleList) const
{    
    OutputHandleList.FastResize(m_Data->DirectedEdgeList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
		OutputHandleList[k].SetIndex(m_Data->DirectedEdgeList[k].GetStartPointIndex());
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetPointIDList() const
{
    DenseVector<int_max> OutputPointIDList;
    this->GetPointIDList(OutputPointIDList);
    return OutputPointIDList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetPointIDList(DenseVector<Handle_Of_Point_Of_MembraneMesh>& OutputPointIDList) const
{    
    OutputPointIDList.FastResize(m_Data->DirectedEdgeList.GetLength());
    for (int_max k = 0; k < OutputHandleList.GetLength(); ++k)
    {
		OutputPointIDList[k] = m_Data->DirectedEdgeList[k].GetStartPointID();
    }
}

template<typename MeshAttributeType>
inline 
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetRelativeIndexOfPoint(Handle_Of_Point_Of_MembraneMesh PointHandle) const
{
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		if (m_Data->DirectedEdgeList[k].GetStartPointHandle() == PointHandle)
		{
			return k;
		}
	}
	return -1;
}

template<typename MeshAttributeType>
inline int_max Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Edge_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeHandleList() const
{
    DenseVector<Handle_Of_Edge_Of_MembraneMesh> OutputHandleList;
    this->GetEdgeHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeHandleList(DenseVector<Handle_Of_Edge_Of_MembraneMesh>& OutputHandleList) const
{
    int_max EdgeCount = m_Data->DirectedEdgeList.GetLength();
    OutputHandleList.FastResize(EdgeCount);
    for (int_max k = 0; k < EdgeCount; ++k)
    {
        OutputHandleList[k].SetIndex(m_Data->DirectedEdgeList[k].GetEdgeIndex());
    }
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetEdgeIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeIDList(DenseVector<int_max>& OutputIDList) const
{
    int_max EdgeCount = m_Data->DirectedEdgeList.GetLength();
    OutputIDList.FastResize(EdgeCount);
    for (int_max k = 0; k < EdgeCount; ++k)
    {
        OutputIDList[k] = m_Data->Mesh.m_MeshData->EdgeList[m_Data->DirectedEdgeList[k].GetEdgeIndex()].GetID();
    }
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList() const
{
    DenseVector<Handle_Of_Face_Of_MembraneMesh> OutputHandleList;
    this->GetAdjacentFaceHandleList(OutputHandleList);
    return OutputHandleList;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->DirectedEdgeList[k].GetEdgeIndex();		
		int_max MaxCount = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().GetLength();
		for (int_max n = 0; n < MaxCount; ++n)
		{
			auto DirectedEdgeIndex_n = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList()[n];
			if (DirectedEdgeIndex_n.FaceIndex != m_Data->Index)
			{				
				Counter += 1;
			}
		}
	}
	return Counter;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{
	OutputHandleList.FastResize(0);
	OutputHandleList.SetCapacity(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->DirectedEdgeList[k].GetEdgeIndex();
		int_max MaxCount = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().GetLength();
		for (int_max n = 0; n < MaxCount; ++n)
		{
			auto DirectedEdgeIndex_n = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList()[n];
			if (DirectedEdgeIndex_n.FaceIndex != m_Data->Index)
			{
				Handle_Of_Face_Of_MembraneMesh FaceHandle;
				FaceHandle.SetIndex(DirectedEdgeIndex_n.FaceIndex);
				OutputHandleList.Append(FaceHandle);
			}
		}
	}
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList() const
{
    DenseVector<int_max> OutputIDList;
    this->GetAdjacentFaceIDList(OutputIDList);
    return OutputIDList;
}

template<typename MeshAttributeType>
inline
void Face_Of_MembraneMesh<MeshAttributeType>::GetAdjacentFaceIDList(DenseVector<int_max>& OutputIDList) const
{
	OutputIDList.FastResize(0);
	OutputIDList.SetCapacity(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->DirectedEdgeList[k].GetEdgeIndex();
		int_max MaxCount = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().GetLength();
		for (int_max n = 0; n < MaxCount; ++n)
		{
			auto DirectedEdgeIndex_n = m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList()[n];
			if (DirectedEdgeIndex_n.FaceIndex != m_Data->Index)
			{
				OutputIDList.Append(m_Data->Mesh.m_MeshData->FaceList[DirectedEdgeIndex_n.FaceIndex].GetID());
			}
		}
	}
}


template<typename MeshAttributeType>
inline
int_max Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{// share a vertex point
	auto HandleList = this->GetNeighbourFaceHandleList();
	return HandleList.GetLength();
}


template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Face_Of_MembraneMesh> Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceHandleList() const
{
	DenseVector<Handle_Of_Face_Of_MembraneMesh> HandleList;
	this->GetNeighbourFaceHandleList(HandleList);
	return HandleList;
}


template<typename MeshAttributeType>
inline 
void Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceHandleList(DenseVector<Handle_Of_Face_Of_MembraneMesh>& OutputHandleList) const
{
	auto PointIndexList = this->GetPointIndexList();
	OutputHandleList.FastResize(0);
	OutputHandleList.SetCapacity(PointIndexList.GetLength());
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		const auto& FaceIndexList_k = m_Data->Mesh.m_MeshData->PointList[PointIndexList[k]].AdjacentFaceIndexList();
		for (int_max n = 0; n < FaceIndexList_k.GetLength(); ++n)
		{
			if (FaceIndexList_k[n] != m_Data->Index)
			{
				Handle_Of_Face_Of_MembraneMesh FaceHandle;
				FaceHandle.SetIndex(FaceIndexList_k[n]);
				OutputHandleList.Append(FaceHandle);
			}
		}
	}
	OutputHandleList = OutputHandleList.GetSubSet(OutputHandleList.FindUnique());
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIDList() const
{
	DenseVector<int_max> IDList;
	this->GetNeighbourFaceIDList(IDList);
	return IDList;
}


template<typename MeshAttributeType>
inline 
void Face_Of_MembraneMesh<MeshAttributeType>::GetNeighbourFaceIDList(DenseVector<int_max>& OutputIDList) const
{
	auto NeighbourFaceIndexList = this->GetNeighbourFaceIndexList();
	OutputIDList.FastResize(NeighbourFaceIndexList.GetLength());
	for (int_max k = 0; k < NeighbourFaceIndexList.GetLength(); ++k)
	{
		OutputIDList[k] = m_Data->Mesh.m_MeshData->FaceList[NeighbourFaceIndexList[k]].GetID();
	}
}


template<typename MeshAttributeType>
inline 
typename MeshAttributeType::FaceAttributeType& Face_Of_MembraneMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::FaceAttributeType& Face_Of_MembraneMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
Handle_Of_Edge_Of_MembraneMesh Face_Of_MembraneMesh<MeshAttributeType>::GetEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const
{
	Handle_Of_Edge_Of_MembraneMesh EdgeHandle;
	EdgeHandle.SetToInvalid();
	auto PointHandleList = this->GetPointHandleList();
	int_max tempIndexA = -1;
	int_max tempIndexB = -1;
	for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
	{
		if (PointHandleList[k] == PointHandleA)
		{
			tempIndexA = k;
		}
		else if (PointHandleList[k] == PointHandleB)
		{
			tempIndexB = k;
		}

		if (tempIndexA >= 0 && tempIndexB >= 0)
		{
			break;
		}
	}
	if (tempIndexA < 0 || tempIndexB < 0)
	{
		return EdgeHandle;
	}
	auto EdgeHandleList = this->GetEdgeHandleList();
	auto PointCount = PointHandleList.GetLength();
	if (tempIndexB = tempIndexA + 1 || tempIndexB = tempIndexA + 1 - PointCount)
	{
		EdgeHandle = EdgeHandleList[tempIndexA];
	}
	else if (tempIndexA = tempIndexB + 1 || tempIndexA = tempIndexB + 1 - PointCount)
	{
		EdgeHandle = EdgeHandleList[tempIndexB];
	}
	return EdgeHandle;
}

template<typename MeshAttributeType>
Handle_Of_DirectedEdge_Of_MembraneMesh Face_Of_MembraneMesh<MeshAttributeType>::GetDirectedEdgeHandleByPoint(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const
{// get DirectedEdgeHandle from A  to B
	Handle_Of_DirectedEdge_Of_MembraneMesh DirectedEdgeHandle;
	DirectedEdgeHandle.SetToInvalid();
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		auto StartPointIndex_k = m_Data->DirectedEdgeList[k].GetStartPointIndex();
		auto EndPointIndex_k = m_Data->DirectedEdgeList[k].GetEndPointIndex();
		if (StartPointIndex_k == PointHandleA.GetIndex() && EndPointIndex_k == PointHandleB.GetIndex())
		{
			DirectedEdgeHandle.SetIndex(m_Data->DirectedEdgeList[k].GetIndex());
			break;
		}
	}
	return DirectedEdgeHandle;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_MembraneMesh> 
Face_Of_MembraneMesh<MeshAttributeType>::GetPointHandleList_LeadBy(Handle_Of_Point_Of_MembraneMesh PointHandleA) const
{// output handle list {PointHandleA, ...}
	auto PointHandleList = this->GetPointHandleList();
	int_max tempIndexA = -1;
	for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
	{
		if (PointHandleList[k] == PointHandleA)
		{
			tempIndexA = k;
		}
		if (tempIndexA >= 0)
		{
			break;
		}
	}

	if (tempIndexA == 0)
	{
		return PointHandleList;
	}

	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_output;

	if (tempIndexA < 0)
	{
		return PointHandleList_output;
	}
	
	//now tempIndexA > 0
	PointHandleList_output.SetCapacity(PointHandleList.GetLength());
	for (int_max k = tempIndexA; k < PointHandleList.GetLength(); ++k)
	{
		PointHandleList_output.Append(PointHandleList[k]);
	}
	for (int_max k = 0; k < tempIndexA; ++k)
	{
		PointHandleList_output.Append(PointHandleList[k]);
	}
	return PointHandleList_output;
}

template<typename MeshAttributeType>
inline
DenseVector<Handle_Of_Point_Of_MembraneMesh> 
Face_Of_MembraneMesh<MeshAttributeType>::GetPointHandleList_LeadBy(Handle_Of_Point_Of_MembraneMesh PointHandleA, Handle_Of_Point_Of_MembraneMesh PointHandleB) const
{// output handle list {PointHandleA, PointHandleB, ...}

	auto PointHandleListA = this->GetPointHandleList_LeadBy(PointHandleA);
	
	if (PointHandleListA.IsEmpty() == true)
	{
		return PointHandleListA;
	}

	if (PointHandleListA[1] == PointHandleB)
	{
		return PointHandleListA;
	}

	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_output;

	if (PointHandleListA[PointHandleListA.GetLength()-1] == PointHandleB)
	{
		PointHandleList_output.SetCapacity(PointHandleListA.GetLength());
		PointHandleList_output.Append(PointHandleListA[0]);
		for (int_max k = PointHandleListA.GetLength() - 1; k > 0; --k)
		{
			PointHandleList_output.Append(PointHandleListA[k]);
		}
	}

	return PointHandleList_output;
}


}// namespace mdk

#endif