#pragma once

namespace mdk
{
//=========================================================== Point_Of_PolygonMesh ===========================================================//

template<typename MeshAttributeType>
inline
Point_Of_PolygonMesh<MeshAttributeType>::Point_Of_PolygonMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Point_Of_PolygonMesh<MeshAttributeType>::Point_Of_PolygonMesh(const Point_Of_PolygonMesh<MeshAttributeType>& InputPoint)
{
    (*this) = InputPoint;
}

template<typename MeshAttributeType>
inline
Point_Of_PolygonMesh<MeshAttributeType>::Point_Of_PolygonMesh(Point_Of_PolygonMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
Point_Of_PolygonMesh<MeshAttributeType>::~Point_Of_PolygonMesh()
{
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::operator=(const Point_Of_PolygonMesh<MeshAttributeType>& InputPoint)
{
    if (!InputPoint.m_Data)
    {
		m_Data.reset();
        return;
    }
	
	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Point_Of_PolygonMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputPoint.m_Data->Mesh);
    m_Data->Index = InputPoint.m_Data->Index; 
    m_Data->ID = InputPoint.m_Data->ID;
	m_Data->Name = InputPoint.m_Data->Name;
    m_Data->AdjacentEdgeIndexList = InputPoint.m_Data->AdjacentEdgeIndexList;
    m_Data->Attribute = InputPoint.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::operator=(Point_Of_PolygonMesh<MeshAttributeType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_PolygonMesh<MeshAttributeType>>();
    }

    m_Data->Index = -1;
    m_Data->ID = -1;
	m_Data->Name.Clear();
    m_Data->AdjacentEdgeIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetParentMesh(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max>& Point_Of_PolygonMesh<MeshAttributeType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<int_max>& Point_Of_PolygonMesh<MeshAttributeType>::AdjacentEdgeIndexList() const
{
	return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline
bool Point_Of_PolygonMesh<MeshAttributeType>::IsValid() const
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
bool Point_Of_PolygonMesh<MeshAttributeType>::IsOnEdge() const
{
    return (m_Data->AdjacentEdgeIndexList.IsEmpty() == false);
}

template<typename MeshAttributeType>
inline
bool Point_Of_PolygonMesh<MeshAttributeType>::IsOnBoundaryEdge() const
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
PolygonMesh<MeshAttributeType>& Point_Of_PolygonMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const PolygonMesh<MeshAttributeType>& Point_Of_PolygonMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetIndex() const
{
    return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetID(int_max PointID)
{
	if (!m_Data)
	{
		MDK_Error("this point has been deleted @ Point_Of_PolygonMesh::SetID(...)")
		return;
	}

    if (PointID < 0)
    {
        MDK_Error("PointID < 0 @ Point_Of_PolygonMesh::SetID(...)")
        return;
    }

    // check record
    auto PointID_old = m_Data->ID;
    if (PointID_old != PointID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.find(PointID);
        if (it != m_Data->Mesh.m_MeshData->Map_PointID_to_PointIndex.end())
        {
            MDK_Error("Input PointID has already been used for another point @ Point_Of_PolygonMesh::SetID(...)")
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
void Point_Of_PolygonMesh<MeshAttributeType>::EraseID()
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
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetName(String PointName)
{
	if (!m_Data)
	{
		MDK_Error("this point has been deleted @ Point_Of_PolygonMesh::SetName(...)")
		return;
	}

	if (PointName.IsEmpty() == true)
	{
		MDK_Error("PointName is empty @ Point_Of_PolygonMesh::SetName(...)")
		return;
	}

	// check record
	auto PointName_old = m_Data->Name;
	if (PointName_old != PointName)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.find(PointName);
		if (it != m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.end())
		{
			MDK_Error("Input PointName has already been used for another point @ Point_Of_PolygonMesh::SetID(...)")
			return;
		}

		it = m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.find(PointName_old);
		if (it != m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.erase(it);
		}

		m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex[PointName] = m_Data->Index;
		m_Data->Name =std::move(PointName);
	}
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.find(m_Data->Name);
		if (it != m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_PointName_to_PointIndex.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename MeshAttributeType>
inline
String Point_Of_PolygonMesh<MeshAttributeType>::GetName() const
{
	return m_Data->Name;
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetPosition(const DenseVector<ScalarType, 3>& Pos)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos.GetPointer());
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetPosition(const ScalarType Pos[3])
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::SetPosition(ScalarType x, ScalarType y, ScalarType z)
{
    m_Data->Mesh.m_MeshData->PointPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> Point_Of_PolygonMesh<MeshAttributeType>::GetPosition() const
{
    DenseVector<ScalarType, 3> Pos;
    this->GetPosition(Pos.GetPointer());
    return Pos;
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename MeshAttributeType>
inline
void Point_Of_PolygonMesh<MeshAttributeType>::GetPosition(ScalarType Pos[3]) const
{
    m_Data->Mesh.m_MeshData->PointPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename MeshAttributeType>
inline
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentPointCount() const
{
	return m_Data->AdjacentEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentPointIndexList() const
{
	DenseVector<int_max> AdjacentPointIndexList;
	AdjacentPointIndexList.Resize(m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->AdjacentEdgeIndexList[k];
		int_max PointIndex0, PointIndex1;
		m_Data->Mesh.m_MeshData->EdgeList[EdgeIndex_k].GetPointIndexList(PointIndex0, PointIndex1);
		if (PointIndex0 == m_Data->Index)
		{
			AdjacentPointIndexList[k] = PointIndex1;
		}
		else if (PointIndex1 == m_Data->Index)
		{
			AdjacentPointIndexList[k] = PointIndex0;
		}
		else
		{
			MDK_Error("Wrong @ Point_Of_PolygonMesh::GetAdjacentPointIndexList()")
		}
	}
	return AdjacentPointIndexList;
}

template<typename MeshAttributeType>
inline
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentEdgeCount() const
{
	return m_Data->AdjacentEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentEdgeIndexList() const
{
	return m_Data->AdjacentEdgeIndexList;
}

template<typename MeshAttributeType>
inline
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetOutgoingDirectedEdgeCount() const
{
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
	return OutgoingDirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> Point_Of_PolygonMesh<MeshAttributeType>::GetOutgoingDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_PolygonMesh> OutgoingDirectedEdgeIndexList;
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
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetIncomingDirectedEdgeCount() const
{
	auto IncomingDirectedEdgeIndexList = this->GetIncomingDirectedEdgeIndexList();
	return IncomingDirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> Point_Of_PolygonMesh<MeshAttributeType>::GetIncomingDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_PolygonMesh> IncomingDirectedEdgeIndexList;
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
int_max Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	auto AdjacentFaceIndexList = this->GetAdjacentFaceIndexList();
	return AdjacentFaceIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Point_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
	DenseVector<int_max> AdjacentFaceIndexList;
	auto OutgoingDirectedEdgeIndexList = this->GetOutgoingDirectedEdgeIndexList();
	AdjacentFaceIndexList.Resize(OutgoingDirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexList.GetLength(); ++k)
	{
		AdjacentFaceIndexList[k] = OutgoingDirectedEdgeIndexList[k].FaceIndex;
	}
	return AdjacentFaceIndexList;
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::PointAttributeType& Point_Of_PolygonMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::PointAttributeType& Point_Of_PolygonMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Edge_Of_PolygonMesh ===========================================================//

template<typename MeshAttributeType>
inline
Edge_Of_PolygonMesh<MeshAttributeType>::Edge_Of_PolygonMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Edge_Of_PolygonMesh<MeshAttributeType>::Edge_Of_PolygonMesh(const Edge_Of_PolygonMesh<MeshAttributeType>& InputEdge)
{
    (*this) = InputEdge;
}

template<typename MeshAttributeType>
inline
Edge_Of_PolygonMesh<MeshAttributeType>::Edge_Of_PolygonMesh(Edge_Of_PolygonMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
Edge_Of_PolygonMesh<MeshAttributeType>::~Edge_Of_PolygonMesh()
{
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::operator=(const Edge_Of_PolygonMesh<MeshAttributeType>& InputEdge)
{
    if (!InputEdge.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Edge_Of_PolygonMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputEdge.m_Data->Mesh);
    m_Data->Index = InputEdge.m_Data->Index;
    m_Data->ID = InputEdge.m_Data->ID;
	m_Data->Name = InputEdge.m_Data->Name;
    m_Data->PointIndex0 = InputEdge.m_Data->PointIndex0;
    m_Data->PointIndex1 = InputEdge.m_Data->PointIndex1;
	m_Data->DirectedEdgeIndexList = InputEdge.m_Data->DirectedEdgeIndexList;
    m_Data->Attribute = InputEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::operator=(Edge_Of_PolygonMesh<MeshAttributeType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_PolygonMesh<MeshAttributeType>>();
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
	m_Data->Name.Clear();
    m_Data->PointIndex0 = -1;
    m_Data->PointIndex1 = -1;
	m_Data->DirectedEdgeIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetParentMesh(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetPointIndexList(const int_max PointIndexList[2])
{
    this->SetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh>& Edge_Of_PolygonMesh<MeshAttributeType>::DirectedEdgeIndexList()
{
	return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
const DenseVector<DirectedEdgeIndex_Of_PolygonMesh>& Edge_Of_PolygonMesh<MeshAttributeType>::DirectedEdgeIndexList() const
{
	return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetPointIndexList(int_max PointIndex0, int_max PointIndex1)
{
    m_Data->PointIndex0 = PointIndex0;
    m_Data->PointIndex1 = PointIndex1;
}

template<typename MeshAttributeType>
inline 
void Edge_Of_PolygonMesh<MeshAttributeType>::SetPointIndexList(const DenseVector<int_max, 2>& PointIndexList)
{
	m_Data->PointIndex0 = PointIndexList[0];
	m_Data->PointIndex1 = PointIndexList[1];
}


template<typename MeshAttributeType>
inline
bool Edge_Of_PolygonMesh<MeshAttributeType>::IsValid() const
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
bool Edge_Of_PolygonMesh<MeshAttributeType>::IsBoundary() const
{
	return (m_Data->DirectedEdgeIndexList.GetLength() <= 1);
}

template<typename MeshAttributeType>
inline
PolygonMesh<MeshAttributeType>& Edge_Of_PolygonMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const PolygonMesh<MeshAttributeType>& Edge_Of_PolygonMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetIndex() const
{
	return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetID(int_max EdgeID)
{
	if (!m_Data)
	{
		MDK_Error("Edge has been deleted @ Edge_Of_PolygonMesh::SetID(...)")
		return;
	}

    if (EdgeID < 0)
    {
        MDK_Error("EdgeID < 0 @ Edge_Of_PolygonMesh::SetID(...)")
        return;
    }

    // check record
    auto EdgeID_old = m_Data->ID;
    if (EdgeID_old != EdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_EdgeID_to_EdgeIndex.end())
        {
            MDK_Error("Input EdgeID has already been used for another edge @ Edge_Of_PolygonMesh::SetID(...)")
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
void Edge_Of_PolygonMesh<MeshAttributeType>::EraseID()
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
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::SetName(String EdgeName)
{
	if (!m_Data)
	{
		MDK_Error("Edge has been deleted @ Edge_Of_PolygonMesh::SetName(...)")
		return;
	}

	if (EdgeName.IsEmpty() == true)
	{
		MDK_Error("EdgeName is empty @ Edge_Of_PolygonMesh::SetName(...)")
		return;
	}

	// check record
	auto EdgeName_old = m_Data->Name;
	if (EdgeName_old != EdgeName)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.find(EdgeName);
		if (it != m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.end())
		{
			MDK_Error("Input EdgeName has already been used for another edge @ Edge_Of_PolygonMesh::SetID(...)")
			return;
		}

		it = m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.find(EdgeName_old);
		if (it != m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.erase(it);
		}

		m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex[EdgeName] = m_Data->Index;
		m_Data->Name = std::move(EdgeName);
	}
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.find(m_Data->Name);
		if (it != m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_EdgeName_to_EdgeIndex.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename MeshAttributeType>
inline
String Edge_Of_PolygonMesh<MeshAttributeType>::GetName() const
{
	return m_Data->Name;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const
{
	PointIndex0 = m_Data->PointIndex0;
	PointIndex1 = m_Data->PointIndex1;
}

template<typename MeshAttributeType>
inline
void Edge_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList(int_max PointIndexList[2]) const
{
	this->GetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename MeshAttributeType>
inline
DenseVector<int_max, 2> Edge_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList() const
{
	DenseVector<int_max, 2> PointIndexList;
	PointIndexList[0] = m_Data->PointIndex0;
	PointIndexList[1] = m_Data->PointIndex1;
	return PointIndexList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetDirectedEdgeCount() const
{
	return DirectedEdgeIndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> Edge_Of_PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexList() const
{
	return m_Data->DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentEdgeCount() const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();
	return AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentEdgeIndexList() const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetAdjacentEdgeIndexList();

	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

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

	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	return m_Data->DirectedEdgeIndexList.GetLength();
}

// Face share this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(2);
	for (int_max k = 0; k < m_Data->DirectedEdgeIndexList.GetLength(); ++k)
	{
		OutputIndexList.Append(m_Data->DirectedEdgeIndexList[k].FaceIndex);
	}
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
int_max Edge_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{
	// wrong, may share more than 2 face
	//auto Counter0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].GetAdjacentFaceCount();
	//auto Counter1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].GetAdjacentFaceCount();
	//return Counter0 + Counter1 - 2;

	auto IndexList = this->GetNeighbourFaceIndexList();
	return IndexList.GetLength();
}

// Face share any  point of this edge
template<typename MeshAttributeType>
inline
DenseVector<int_max> Edge_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;

	const auto& AdjacentFaceIndexList0 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex0].AdjacentFaceIndexList();
	const auto& AdjacentFaceIndexList1 = m_Data->Mesh.m_MeshData->PointList[m_Data->PointIndex1].AdjacentFaceIndexList();

	if (AdjacentFaceIndexList0.IsEmpty() == true && AdjacentFaceIndexList1.IsEmpty() == true)
	{
		return OutputIndexList;
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

	return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::EdgeAttributeType& Edge_Of_PolygonMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::EdgeAttributeType& Edge_Of_PolygonMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== DirectedEdge_Of_PolygonMesh ===========================================================//

template<typename MeshAttributeType>
inline
DirectedEdge_Of_PolygonMesh<MeshAttributeType>::DirectedEdge_Of_PolygonMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_PolygonMesh<MeshAttributeType>::DirectedEdge_Of_PolygonMesh(const DirectedEdge_Of_PolygonMesh<MeshAttributeType>& InputDirectedEdge)
{
    (*this) = InputDirectedEdge;
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_PolygonMesh<MeshAttributeType>::DirectedEdge_Of_PolygonMesh(DirectedEdge_Of_PolygonMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_PolygonMesh<MeshAttributeType>::~DirectedEdge_Of_PolygonMesh()
{
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::operator=(const DirectedEdge_Of_PolygonMesh<MeshAttributeType>& InputDirectedEdge)
{
    if (!InputDirectedEdge.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_DirectedEdge_Of_PolygonMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputDirectedEdge.m_Data->Mesh);
    m_Data->Index = InputDirectedEdge.m_Data->Index;
    m_Data->ID = InputDirectedEdge.m_Data->ID;
	m_Data->Name = InputDirectedEdge.m_Data->Name;
    m_Data->EdgeIndex = InputDirectedEdge.m_Data->EdgeIndex;
    m_Data->PointIndex_start = InputDirectedEdge.m_Data->PointIndex_start;
    m_Data->PointIndex_end = InputDirectedEdge.m_Data->PointIndex_end;
    m_Data->Attribute = InputDirectedEdge.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::operator=(DirectedEdge_Of_PolygonMesh<MeshAttributeType>&& InputDirectedEdge)
{
    m_Data = std::move(InputDirectedEdge.m_Data);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdge_Of_PolygonMesh<MeshAttributeType>>();        
    }    

    m_Data->ID = -1;
	m_Data->Name.Clear();
    m_Data->Index.FaceIndex = -1;
    m_Data->Index.RelativeIndex = -1;
    m_Data->EdgeIndex = -1;
    m_Data->PointIndex_start = -1;
    m_Data->PointIndex_end = -1;

    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetParentMesh(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetIndex(DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex)
{
    m_Data->Index = DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetIndex(int_max FaceIndex, int_max RelativeIndex)
{
    m_Data->Index.FaceIndex = FaceIndex;
    m_Data->Index.RelativeIndex = RelativeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetEdgeIndex(int_max EdgeIndex)
{
    m_Data->EdgeIndex = EdgeIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetStartPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_start = PointIndex;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetEndPointIndex(int_max PointIndex)
{
    m_Data->PointIndex_end = PointIndex;
}

template<typename MeshAttributeType>
inline
bool DirectedEdge_Of_PolygonMesh<MeshAttributeType>::IsValid() const
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
bool DirectedEdge_Of_PolygonMesh<MeshAttributeType>::IsBoundary() const
{
    return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].IsBoundary();
}

template<typename MeshAttributeType>
inline
PolygonMesh<MeshAttributeType>& DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const PolygonMesh<MeshAttributeType>& DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_PolygonMesh DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetIndex() const
{
	return m_Data->Index;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetFaceIndex() const
{
	return m_Data->Index.FaceIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetEdgeIndex() const
{
	return m_Data->EdgeIndex;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetStartPointIndex() const
{
	return m_Data->PointIndex_start;
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetEndPointIndex() const
{
	return m_Data->PointIndex_end;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetID(int_max DirectedEdgeID)
{
    if (DirectedEdgeID < 0)
    {
        MDK_Error("DirectedEdgeID < 0 @ DirectedEdge_Of_PolygonMesh::SetID(...)")
        return;
    }

    // check record
    auto DirectedEdgeID_old = m_Data->ID;
    if (DirectedEdgeID_old != DirectedEdgeID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
        if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
        {
            MDK_Error("Input DirectedEdgeID has already been used for another directed-edge @ DirectedEdge_Of_PolygonMesh::SetID(...)")
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
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::EraseID()
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
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::SetName(String DirectedEdgeName)
{
	if (!m_Data)
	{
		MDK_Error("DirectedEdge has been deleted @ DirectedEdge_Of_PolygonMesh::SetName(...)")
		return;
	}

	if (DirectedEdgeName.IsEmpty() == true)
	{
		MDK_Error("DirectedEdgeName is empty 0 @ DirectedEdge_Of_PolygonMesh::SetName(...)")
		return;
	}

	// check record
	auto DirectedEdgeName_old = m_Data->Name;
	if (DirectedEdgeName_old != DirectedEdgeName)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(DirectedEdgeName);
		if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
		{
			MDK_Error("Input DirectedEdgeID has already been used for another directed-edge @ DirectedEdge_Of_PolygonMesh::SetName(...)")
			return;
		}

		it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(DirectedEdgeName_old);
		if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.erase(it);
		}

		m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex[DirectedEdgeName] = m_Data->Index;
		m_Data->Name = std::move(DirectedEdgeName);
	}
}

template<typename MeshAttributeType>
inline
void DirectedEdge_Of_PolygonMesh<MeshAttributeType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(m_Data->Name);
		if (it != m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename MeshAttributeType>
inline
String DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetName() const
{
	return m_Data->Name;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_PolygonMesh DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetNextDirectedEdgeIndex() const
{
	auto DirectedEdgeCount = m_Data->Mesh.m_MeshData->FaceList[m_Data->Index.FaceIndex].DirectedEdgeList().GetLength();
	DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex_next;
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
DirectedEdgeIndex_Of_PolygonMesh DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetPreviousDirectedEdgeIndex() const
{
	auto DirectedEdgeCount = m_Data->Mesh.m_MeshData->FaceList[m_Data->Index.FaceIndex].DirectedEdgeList().GetLength();
	DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex_prev;
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
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetFriendDirectedEdgeCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].DirectedEdgeIndexList().GetLength() - 1;
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetFriendDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_PolygonMesh> OutputIndexList;
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
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceCount();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetAdjacentFaceIndexList();
}

template<typename MeshAttributeType>
inline
int_max DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceCount();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> DirectedEdge_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
	return m_Data->Mesh.m_MeshData->EdgeList[m_Data->EdgeIndex].GetNeighbourFaceIndexList();
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_PolygonMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline 
const typename MeshAttributeType::DirectedEdgeAttributeType& DirectedEdge_Of_PolygonMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Face_Of_PolygonMesh ===========================================================//

template<typename MeshAttributeType>
inline
Face_Of_PolygonMesh<MeshAttributeType>::Face_Of_PolygonMesh()
{
	this->ReCreate();
}

template<typename MeshAttributeType>
inline
Face_Of_PolygonMesh<MeshAttributeType>::Face_Of_PolygonMesh(const Face_Of_PolygonMesh<MeshAttributeType>& InputFace)
{
    (*this) = InputFace;
}

template<typename MeshAttributeType>
inline
Face_Of_PolygonMesh<MeshAttributeType>::Face_Of_PolygonMesh(Face_Of_PolygonMesh<MeshAttributeType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename MeshAttributeType>
inline
Face_Of_PolygonMesh<MeshAttributeType>::~Face_Of_PolygonMesh()
{
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::operator=(const Face_Of_PolygonMesh<MeshAttributeType>& InputFace)
{
    if (!InputFace.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Face_Of_PolygonMesh<MeshAttributeType>>();
	}

    m_Data->Mesh.ForceShare(InputFace.m_Data->Mesh);
    m_Data->Index = InputFace.m_Data->Index;
    m_Data->ID = InputFace.m_Data->ID;
	m_Data->Name = InputFace.m_Data->Name;
    m_Data->DirectedEdgeList = InputFace.m_Data->DirectedEdgeList;
    m_Data->Attribute = InputFace.m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::operator=(Face_Of_PolygonMesh<MeshAttributeType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Face_Of_PolygonMesh<MeshAttributeType>>();
    }    

    m_Data->Index = -1;
    m_Data->ID = -1;
	m_Data->Name.Clear();
    m_Data->DirectedEdgeList.Clear();
    m_Data->Attribute.Clear();
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::SetParentMesh(PolygonMesh<MeshAttributeType>& InputMesh)
{
    m_Data->Mesh.Share(InputMesh);
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		m_Data->DirectedEdgeList[k].SetParentMesh(InputMesh);
	}
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::SetIndex(int_max FaceIndex)
{
    m_Data->Index = FaceIndex;
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		m_Data->DirectedEdgeList[k].SetIndex(FaceIndex, k);
	}
}

template<typename MeshAttributeType>
inline
StdObjectVector<DirectedEdge_Of_PolygonMesh<MeshAttributeType>>& Face_Of_PolygonMesh<MeshAttributeType>::DirectedEdgeList()
{
	return m_Data->DirectedEdgeList;
}

template<typename MeshAttributeType>
inline
const StdObjectVector<DirectedEdge_Of_PolygonMesh<MeshAttributeType>>& Face_Of_PolygonMesh<MeshAttributeType>::DirectedEdgeList() const
{
	return m_Data->DirectedEdgeList;
}

template<typename MeshAttributeType>
inline
bool Face_Of_PolygonMesh<MeshAttributeType>::IsValid() const
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
PolygonMesh<MeshAttributeType>& Face_Of_PolygonMesh<MeshAttributeType>::GetParentMesh()
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
const PolygonMesh<MeshAttributeType>& Face_Of_PolygonMesh<MeshAttributeType>::GetParentMesh() const
{
    return m_Data->Mesh;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetIndex() const
{
	return m_Data->Index;
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::SetID(int_max FaceID)
{
    if (FaceID < 0)
    {
        MDK_Error("FaceID < 0 @ Face_Of_PolygonMesh::SetID(...)")
        return;
    }

    // check record
    auto FaceID_old = m_Data->ID;
    if (FaceID_old != FaceID)
    {
        auto it = m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
        if (it != m_Data->Mesh.m_MeshData->Map_FaceID_to_FaceIndex.end())
        {
            MDK_Error("Input FaceID has already been used for another cell @ Face_Of_PolygonMesh::SetID(...)")
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
void Face_Of_PolygonMesh<MeshAttributeType>::EraseID()
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
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetID() const
{
    return m_Data->ID;
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::SetName(String FaceName)
{
	if (!m_Data)
	{
		MDK_Error("Face has been deleted @ Face_Of_PolygonMesh::SetName(...)")
		return;
	}

	if (FaceName.IsEmpty() == true)
	{
		MDK_Error("FaceName is empty @ Face_Of_PolygonMesh::SetName(...)")
		return;
	}

	// check record
	auto FaceName_old = m_Data->Name;
	if (FaceName_old != FaceName)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.find(FaceName);
		if (it != m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.end())
		{
			MDK_Error("Input FaceName has already been used for another cell @ Face_Of_PolygonMesh::SetName(...)")
			return;
		}

		it = m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.find(FaceName_old);
		if (it != m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.erase(it);
		}

		m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex[FaceName] = m_Data->Index;
		m_Data->Name = std::move(FaceName);
	}
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.find(m_Data->Name);
		if (it != m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.end())
		{
			m_Data->Mesh.m_MeshData->Map_FaceName_to_FaceIndex.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename MeshAttributeType>
inline
String Face_Of_PolygonMesh<MeshAttributeType>::GetName() const
{
	return m_Data->Name;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetPointCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.Resize(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < OutputIndexList.GetLength(); ++k)
	{
		OutputIndexList[k] = m_Data->DirectedEdgeList[k].GetStartPointIndex();
	}
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline int_max Face_Of_PolygonMesh<MeshAttributeType>::GetEdgeCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetEdgeIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.Resize(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		OutputIndexList[k] = m_Data->DirectedEdgeList[k].GetEdgeIndex();
	}
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline int_max Face_Of_PolygonMesh<MeshAttributeType>::GetDirectedEdgeCount() const
{
	return m_Data->DirectedEdgeList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> Face_Of_PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndex_Of_PolygonMesh> OutputIndexList;
	OutputIndexList.Resize(m_Data->DirectedEdgeList.GetLength());
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		OutputIndexList[k].FaceIndex = m_Data->Index;
		OutputIndexList[k].RelativeIndex = k;
	}
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceCount() const
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
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetAdjacentFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;
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
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceCount() const
{// share a vertex point
	auto IndexList = this->GetNeighbourFaceIndexList();
	return IndexList.GetLength();
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetNeighbourFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;
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
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::FaceAttributeType& Face_Of_PolygonMesh<MeshAttributeType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::FaceAttributeType& Face_Of_PolygonMesh<MeshAttributeType>::Attribute() const
{
    return m_Data->Attribute;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetRelativeIndexOfPoint(int_max PointIndex) const
{
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		if (m_Data->DirectedEdgeList[k].GetStartPointIndex() == PointIndex)
		{
			return k;
		}
	}
	return -1;
}

template<typename MeshAttributeType>
inline
int_max Face_Of_PolygonMesh<MeshAttributeType>::GetEdgeIndexByPoint(int_max PointIndexA, int_max PointIndexB) const
{
	auto PointIndexList = this->GetPointIndexList();
	int_max tempIndexA = -1;
	int_max tempIndexB = -1;
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (PointIndexList[k] == PointIndexA)
		{
			tempIndexA = k;
		}
		else if (PointIndexList[k] == PointIndexB)
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
		return -1;
	}

	auto EdgeIndex = -1;
	auto EdgeIndexList = this->GetEdgeIndexList();
	auto PointCount = EdgeIndexList.GetLength();
	if (tempIndexB = tempIndexA + 1 || tempIndexB = tempIndexA + 1 - PointCount)
	{
		EdgeIndex = EdgeIndexList[tempIndexA];
	}
	else if (tempIndexA = tempIndexB + 1 || tempIndexA = tempIndexB + 1 - PointCount)
	{
		EdgeIndex = EdgeIndexList[tempIndexB];
	}
	return EdgeIndex;
}

template<typename MeshAttributeType>
DirectedEdgeIndex_Of_PolygonMesh Face_Of_PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexByPoint(int_max PointIndexA, int_max PointIndexB) const
{// get DirectedEdgeIndex from A  to B
	DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex;	
	for (int_max k = 0; k < m_Data->DirectedEdgeList.GetLength(); ++k)
	{
		auto StartPointIndex_k = m_Data->DirectedEdgeList[k].GetStartPointIndex();
		auto EndPointIndex_k = m_Data->DirectedEdgeList[k].GetEndPointIndex();
		if (StartPointIndex_k == PointIndexA && EndPointIndex_k == PointIndexB)
		{
			DirectedEdgeIndex = m_Data->DirectedEdgeList[k].GetIndex();
			break;
		}
	}
	return DirectedEdgeIndex;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList_LeadBy(int_max PointIndexA) const
{// output list {PointIndexA, ...}
	auto PointIndexList = this->GetPointIndexList();
	int_max tempIndexA = -1;
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (PointIndexList[k] == PointIndexA)
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
		return PointIndexList;
	}

	DenseVector<int_max> PointIndexList_output;

	if (tempIndexA < 0)
	{
		return PointIndexList_output;
	}
	
	//now tempIndexA > 0
	PointIndexList_output.SetCapacity(PointIndexList.GetLength());
	for (int_max k = tempIndexA; k < PointIndexList.GetLength(); ++k)
	{
		PointIndexList_output.Append(PointIndexList[k]);
	}
	for (int_max k = 0; k < tempIndexA; ++k)
	{
		PointIndexList_output.Append(PointIndexList[k]);
	}
	return PointIndexList_output;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> Face_Of_PolygonMesh<MeshAttributeType>::GetPointIndexList_LeadBy(int_max PointIndexA, int_max PointIndexB) const
{// output Index list {PointIndexA, PointIndexB, ...}

	auto PointIndexListA = this->GetPointIndexList_LeadBy(PointIndexA);
	
	if (PointIndexListA.IsEmpty() == true)
	{
		return PointIndexListA;
	}

	if (PointIndexListA[1] == PointIndexB)
	{
		return PointIndexListA;
	}

	DenseVector<Index_Of_Point_Of_PolygonMesh> PointIndexList_output;

	if (PointIndexListA[PointIndexListA.GetLength()-1] == PointIndexB)
	{
		PointIndexList_output.SetCapacity(PointIndexListA.GetLength());
		PointIndexList_output.Append(PointIndexListA[0]);
		for (int_max k = PointIndexListA.GetLength() - 1; k > 0; --k)
		{
			PointIndexList_output.Append(PointIndexListA[k]);
		}
	}

	return PointIndexList_output;
}

template<typename MeshAttributeType>
inline
void Face_Of_PolygonMesh<MeshAttributeType>::ReversePointOrder()
{
	m_Data->Mesh.ReversePointOrderOfFace(m_Data->Index);
}

}// namespace mdk
