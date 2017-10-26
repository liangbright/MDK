#pragma once

namespace mdk
{
//=========================================================== Point_Of_Mesh ===========================================================//

template<typename ScalarType>
inline
Point_Of_Mesh<ScalarType>::Point_Of_Mesh()
{
	this->ReCreate();
}

template<typename ScalarType>
inline
Point_Of_Mesh<ScalarType>::Point_Of_Mesh(const Point_Of_Mesh<ScalarType>& InputPoint)
{
    (*this) = InputPoint;
}

template<typename ScalarType>
inline
Point_Of_Mesh<ScalarType>::Point_Of_Mesh(Point_Of_Mesh<ScalarType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename ScalarType>
inline
Point_Of_Mesh<ScalarType>::~Point_Of_Mesh()
{
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::operator=(const Point_Of_Mesh<ScalarType>& InputPoint)
{
    if (!InputPoint.m_Data)
    {
		m_Data.reset();
        return;
    }
	
	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Point_Of_Mesh<ScalarType>>();
	}
	m_Data->MeshData_wp = InputPoint.m_Data->MeshData_wp;
    m_Data->MeshData = InputPoint.m_Data->MeshData;
    m_Data->Index = InputPoint.m_Data->Index;     
	m_Data->Name = InputPoint.m_Data->Name;
    m_Data->AdjacentEdgeIndexList = InputPoint.m_Data->AdjacentEdgeIndexList;
    m_Data->Attribute = InputPoint.m_Data->Attribute;
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::operator=(Point_Of_Mesh<ScalarType>&& InputPoint)
{
    m_Data = std::move(InputPoint.m_Data);
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Point_Of_Mesh<ScalarType>>();
    }
	m_Data->MeshData_wp.reset();
	m_Data->MeshData = nullptr;
    m_Data->Index = -1;    
	m_Data->Name.Clear();	
    m_Data->AdjacentEdgeIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetParentMesh(Mesh<ScalarType>& ParentMesh)
{
	m_Data->MeshData_wp = ParentMesh.m_MeshData;
    m_Data->MeshData = ParentMesh.m_MeshData.get();
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetIndex(int_max PointIndex)
{
    m_Data->Index = PointIndex;
}

template<typename ScalarType>
inline 
DenseVector<int_max>& Point_Of_Mesh<ScalarType>::AdjacentEdgeIndexList()
{
    return m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Point_Of_Mesh<ScalarType>::AdjacentEdgeIndexList() const
{
	return m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
bool Point_Of_Mesh<ScalarType>::IsValid() const
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

template<typename ScalarType>
inline
bool Point_Of_Mesh<ScalarType>::IsOnEdge() const
{
    return (m_Data->AdjacentEdgeIndexList.IsEmpty() == false);
}

template<typename ScalarType>
inline
bool Point_Of_Mesh<ScalarType>::IsOnPolygonMeshBoundary() const
{
    for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
    {
        auto EdgeIndex = m_Data->AdjacentEdgeIndexList[k];
        if (m_Data->MeshData->EdgeList[EdgeIndex].IsOnPolygonMeshBoundary() == true)
        {
            return true;
        }
    }
    return false;
}

template<typename ScalarType>
inline
bool Point_Of_Mesh<ScalarType>::IsOnPolyhedronMeshBoundary() const
{
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex = m_Data->AdjacentEdgeIndexList[k];
		if (m_Data->MeshData->EdgeList[EdgeIndex].IsOnPolyhedronMeshBoundary() == true)
		{
			return true;
		}
	}
	return false;
}

template<typename ScalarType>
inline
int_max Point_Of_Mesh<ScalarType>::GetIndex() const
{
    return m_Data->Index;
}


template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetName(String PointName)
{
	if (!m_Data)
	{
		MDK_Error("this point has been deleted @ Point_Of_Mesh::SetName(...)")
		return;
	}

	if (PointName.IsEmpty() == true)
	{
		MDK_Error("PointName is empty @ Point_Of_Mesh::SetName(...)")
		return;
	}

	auto it = m_Data->MeshData->Map_Point_Name_to_Index.find(PointName);
	if (it == m_Data->MeshData->Map_Point_Name_to_Index.end())//not found
	{
		auto it_old = m_Data->MeshData->Map_Point_Name_to_Index.find(m_Data->Name);
		if (it_old != m_Data->MeshData->Map_Point_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Point_Name_to_Index.erase(it_old);
		}
		m_Data->MeshData->Map_Point_Name_to_Index[PointName] = m_Data->Index;
		m_Data->Name = std::move(PointName);
	}
	else
	{
		if (it->second != m_Data->Index)
		{
			MDK_Error("Input PointName has already been used for another point @ Point_Of_Mesh::SetName(...)")
			return;
		}
	}	
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->MeshData->Map_Point_Name_to_Index.find(m_Data->Name);
		if (it != m_Data->MeshData->Map_Point_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Point_Name_to_Index.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename ScalarType>
inline
String Point_Of_Mesh<ScalarType>::GetName() const
{
	return m_Data->Name;
}

template<typename ScalarType>
inline 
void Point_Of_Mesh<ScalarType>::SetData(int_max Index, const DenseVector<ScalarType>& Data)
{
	m_Data->MeshData->PointDataSet[Index].SetCol(m_Data->Index, Data);
}

template<typename ScalarType>
inline 
DenseVector<ScalarType> Point_Of_Mesh<ScalarType>::GetData(int_max Index) const
{
	DenseVector<ScalarType> Data;
	m_Data->MeshData->PointDataSet[Index].GetCol(m_Data->Index, Data);
	return Data;
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetPosition(const DenseVector<ScalarType, 3>& Pos)
{
    m_Data->MeshData->PointPositionTable.SetCol(m_Data->Index, Pos.GetPointer());
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetPosition(const ScalarType Pos[3])
{
    m_Data->MeshData->PointPositionTable.SetCol(m_Data->Index, Pos);
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::SetPosition(ScalarType x, ScalarType y, ScalarType z)
{
    m_Data->MeshData->PointPositionTable.SetCol(m_Data->Index, { x, y, z });
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Point_Of_Mesh<ScalarType>::GetPosition() const
{
    DenseVector<ScalarType, 3> Pos;
    this->GetPosition(Pos.GetPointer());
    return Pos;
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::GetPosition(ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Pos[3] = { 0, 0, 0 };
    this->GetPosition(Pos);
    x = Pos[0];
    y = Pos[1];
    z = Pos[2];
}

template<typename ScalarType>
inline
void Point_Of_Mesh<ScalarType>::GetPosition(ScalarType Pos[3]) const
{
    m_Data->MeshData->PointPositionTable.GetCol(m_Data->Index, Pos);
}

template<typename ScalarType>
inline
int_max Point_Of_Mesh<ScalarType>::GetAdjacentPointCount() const
{
	return m_Data->AdjacentEdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Point_Of_Mesh<ScalarType>::GetAdjacentPointIndexList() const
{
	DenseVector<int_max> AdjacentPointIndexList;
	AdjacentPointIndexList.Resize(m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->AdjacentEdgeIndexList[k];
		int_max PointIndex0, PointIndex1;
		m_Data->MeshData->EdgeList[EdgeIndex_k].GetPointIndexList(PointIndex0, PointIndex1);
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
			MDK_Error("Wrong @ Point_Of_Mesh::GetAdjacentPointIndexList()")
		}
	}
	return AdjacentPointIndexList;
}

template<typename ScalarType>
inline
int_max Point_Of_Mesh<ScalarType>::GetAdjacentEdgeCount() const
{
	return m_Data->AdjacentEdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Point_Of_Mesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
	return m_Data->AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
int_max Point_Of_Mesh<ScalarType>::GetAdjacentFaceCount() const
{
	auto AdjacentFaceIndexList = this->GetAdjacentFaceIndexList();
	return AdjacentFaceIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Point_Of_Mesh<ScalarType>::GetAdjacentFaceIndexList() const
{
	DenseVector<int_max> AdjacentFaceIndexList;	
	AdjacentFaceIndexList.SetCapacity(2*m_Data->AdjacentEdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->AdjacentEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->AdjacentEdgeIndexList[k];
		AdjacentFaceIndexList.Append(m_Data->MeshData->EdgeList[EdgeIndex_k].AdjacentFaceIndexList());
	}
	AdjacentFaceIndexList = AdjacentFaceIndexList.GetSubSet(AdjacentFaceIndexList.FindUnique());
	return AdjacentFaceIndexList;
}

template<typename ScalarType>
inline 
int_max Point_Of_Mesh<ScalarType>::GetAdjacentCellCount() const
{
	auto AdjacentCellIndexList = this->GetAdjacentCellIndexList();
	return AdjacentCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Point_Of_Mesh<ScalarType>::GetAdjacentCellIndexList() const
{
	DenseVector<int_max> AdjacentCellIndexList;
	auto AdjacentFaceIndexList = this->GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjacentFaceIndexList.GetLength(); ++k)
	{
		AdjacentCellIndexList.Append(m_Data->MeshData->FaceList[AdjacentFaceIndexList[k]].GetAdjacentCellIndexList());
	}
	AdjacentCellIndexList = AdjacentCellIndexList.GetSubSet(AdjacentCellIndexList.FindUnique());
	return AdjacentCellIndexList;
}

template<typename ScalarType>
inline 
typename StandardAttribute_Of_Point_Of_Mesh<ScalarType>& Point_Of_Mesh<ScalarType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename ScalarType>
inline
const StandardAttribute_Of_Point_Of_Mesh<ScalarType>& Point_Of_Mesh<ScalarType>::Attribute() const
{
    return m_Data->Attribute;
}

template<typename ScalarType>
inline
DenseVector<int_max> Point_Of_Mesh<ScalarType>::GetNeighborPointIndexList(int_max MaxGraphDistance) const
{
	return m_Data->MeshData->GetNeighborPointOfPoint(m_Data->Index, MaxGraphDistance);
}

//=========================================================== Edge_Of_Mesh ===========================================================//

template<typename ScalarType>
inline
Edge_Of_Mesh<ScalarType>::Edge_Of_Mesh()
{
	this->ReCreate();
}

template<typename ScalarType>
inline
Edge_Of_Mesh<ScalarType>::Edge_Of_Mesh(const Edge_Of_Mesh<ScalarType>& InputEdge)
{
    (*this) = InputEdge;
}

template<typename ScalarType>
inline
Edge_Of_Mesh<ScalarType>::Edge_Of_Mesh(Edge_Of_Mesh<ScalarType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename ScalarType>
inline
Edge_Of_Mesh<ScalarType>::~Edge_Of_Mesh()
{
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::operator=(const Edge_Of_Mesh<ScalarType>& InputEdge)
{
    if (!InputEdge.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Edge_Of_Mesh<ScalarType>>();
	}

	m_Data->MeshData_wp = InputEdge.m_Data->MeshData_wp;
    m_Data->MeshData = InputEdge.m_Data->MeshData;
    m_Data->Index = InputEdge.m_Data->Index;    
	m_Data->Name = InputEdge.m_Data->Name;
    m_Data->PointIndex0 = InputEdge.m_Data->PointIndex0;
    m_Data->PointIndex1 = InputEdge.m_Data->PointIndex1;
	m_Data->AdjacentFaceIndexList = InputEdge.m_Data->AdjacentFaceIndexList;
    m_Data->Attribute = InputEdge.m_Data->Attribute;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::operator=(Edge_Of_Mesh<ScalarType>&& InputEdge)
{
    m_Data = std::move(InputEdge.m_Data);
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Edge_Of_Mesh<ScalarType>>();
    }    
	m_Data->MeshData_wp.reset();
	m_Data->MeshData = nullptr;
    m_Data->Index = -1;    
	m_Data->Name.Clear();
    m_Data->PointIndex0 = -1;
    m_Data->PointIndex1 = -1;
	m_Data->AdjacentFaceIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::Clear(const MDK_Symbol_PureEmpty&)
{   
	m_Data.reset();
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetParentMesh(Mesh<ScalarType>& ParentMesh)
{
	m_Data->MeshData_wp = ParentMesh.m_MeshData;
	m_Data->MeshData = ParentMesh.m_MeshData.get();
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetIndex(int_max EdgeIndex)
{
    m_Data->Index = EdgeIndex;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetPointIndexList(const int_max PointIndexList[2])
{
    this->SetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename ScalarType>
inline
DenseVector<int_max>& Edge_Of_Mesh<ScalarType>::AdjacentFaceIndexList()
{
	return m_Data->AdjacentFaceIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Edge_Of_Mesh<ScalarType>::AdjacentFaceIndexList() const
{
	return m_Data->AdjacentFaceIndexList;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetPointIndexList(int_max PointIndex0, int_max PointIndex1)
{
    m_Data->PointIndex0 = PointIndex0;
    m_Data->PointIndex1 = PointIndex1;
}

template<typename ScalarType>
inline 
void Edge_Of_Mesh<ScalarType>::SetPointIndexList(const DenseVector<int_max, 2>& PointIndexList)
{
	m_Data->PointIndex0 = PointIndexList[0];
	m_Data->PointIndex1 = PointIndexList[1];
}


template<typename ScalarType>
inline
bool Edge_Of_Mesh<ScalarType>::IsValid() const
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

template<typename ScalarType>
inline
bool Edge_Of_Mesh<ScalarType>::IsOnPolygonMeshBoundary() const
{
	return (m_Data->AdjacentFaceIndexList.GetLength() == 1);
	// if == 0, then it is an isolated edge, not boundary
}

template<typename ScalarType>
inline
bool Edge_Of_Mesh<ScalarType>::IsOnPolyhedronMeshBoundary() const
{
	for (int_max k = 0; k < m_Data->AdjacentFaceIndexList.GetLength(); ++k)
	{
		auto FaceIndex = m_Data->AdjacentFaceIndexList[k];
		if (m_Data->MeshData->FaceList[FaceIndex].IsOnPolyhedronMeshBoundary() == true)
		{
			return true;
		}
	}
	return false;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetIndex() const
{
	return m_Data->Index;
}


template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetName(String EdgeName)
{
	if (!m_Data)
	{
		MDK_Error("Edge has been deleted @ Edge_Of_Mesh::SetName(...)")
		return;
	}

	if (EdgeName.IsEmpty() == true)
	{
		MDK_Error("EdgeName is empty @ Edge_Of_Mesh::SetName(...)")
		return;
	}

	auto it = m_Data->MeshData->Map_Edge_Name_to_Index.find(EdgeName);
	if (it == m_Data->MeshData->Map_Edge_Name_to_Index.end())//not found
	{
		auto it_old = m_Data->MeshData->Map_Edge_Name_to_Index.find(m_Data->Name);
		if (it_old != m_Data->MeshData->Map_Edge_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Edge_Name_to_Index.erase(it_old);
		}
		m_Data->MeshData->Map_Edge_Name_to_Index[EdgeName] = m_Data->Index;
		m_Data->Name = std::move(EdgeName);
	}
	else
	{
		if (it->second != m_Data->Index)
		{
			MDK_Error("Input EdgeName has already been used for another point @ Edge_Of_Mesh::SetName(...)")
			return;
		}
	}	
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->MeshData->Map_Edge_Name_to_Index.find(m_Data->Name);
		if (it != m_Data->MeshData->Map_Edge_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Edge_Name_to_Index.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename ScalarType>
inline
String Edge_Of_Mesh<ScalarType>::GetName() const
{
	return m_Data->Name;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::SetData(int_max Index, const DenseVector<ScalarType>& Data)
{
	m_Data->MeshData->EdgeDataSet[Index].SetCol(m_Data->Index, Data);
}

template<typename ScalarType>
inline
DenseVector<ScalarType> Edge_Of_Mesh<ScalarType>::GetData(int_max Index) const
{
	DenseVector<ScalarType> Data;
	m_Data->MeshData->EdgeDataSet[Index].GetCol(m_Data->Index, Data);
	return Data;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::GetPointIndexList(int_max& PointIndex0, int_max& PointIndex1) const
{
	PointIndex0 = m_Data->PointIndex0;
	PointIndex1 = m_Data->PointIndex1;
}

template<typename ScalarType>
inline
void Edge_Of_Mesh<ScalarType>::GetPointIndexList(int_max PointIndexList[2]) const
{
	this->GetPointIndexList(PointIndexList[0], PointIndexList[1]);
}

template<typename ScalarType>
inline
DenseVector<int_max, 2> Edge_Of_Mesh<ScalarType>::GetPointIndexList() const
{
	DenseVector<int_max, 2> PointIndexList;
	PointIndexList[0] = m_Data->PointIndex0;
	PointIndexList[1] = m_Data->PointIndex1;
	return PointIndexList;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetAdjacentEdgeCount() const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->MeshData->PointList[m_Data->PointIndex0].AdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->MeshData->PointList[m_Data->PointIndex1].AdjacentEdgeIndexList();
	return AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2;
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_Mesh<ScalarType>::GetAdjacentEdgeIndexList() const
{
	const auto& AdjacentEdgeIndexList0 = m_Data->MeshData->PointList[m_Data->PointIndex0].GetAdjacentEdgeIndexList();
	const auto& AdjacentEdgeIndexList1 = m_Data->MeshData->PointList[m_Data->PointIndex1].GetAdjacentEdgeIndexList();

	DenseVector<int_max> AdjacentEdgeIndexList;
	AdjacentEdgeIndexList.SetCapacity(AdjacentEdgeIndexList0.GetLength() + AdjacentEdgeIndexList1.GetLength() - 2);

	for (int_max k = 0; k < AdjacentEdgeIndexList0.GetLength(); ++k)
	{
		auto tempIndex = AdjacentEdgeIndexList0[k];
		if (tempIndex != m_Data->Index)
		{
			AdjacentEdgeIndexList.Append(tempIndex);
		}
	}

	for (int_max k = 0; k < AdjacentEdgeIndexList1.GetLength(); ++k)
	{
		auto tempIndex = AdjacentEdgeIndexList1[k];
		if (tempIndex != m_Data->Index)
		{
			AdjacentEdgeIndexList.Append(tempIndex);
		}
	}

	return AdjacentEdgeIndexList;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetAdjacentFaceCount() const
{
	return m_Data->AdjacentFaceIndexList.GetLength();
}

// Face share this edge
template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_Mesh<ScalarType>::GetAdjacentFaceIndexList() const
{
	return m_Data->AdjacentFaceIndexList;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetNeighborFaceCount() const
{
	// wrong because one edge may be shared by more than 2 face
	//auto Counter0 = m_Data->MeshData->PointList[m_Data->PointIndex0].GetAdjacentFaceCount();
	//auto Counter1 = m_Data->MeshData->PointList[m_Data->PointIndex1].GetAdjacentFaceCount();
	//return Counter0 + Counter1 - 2;

	auto NeighborFaceIndexList = this->GetNeighborFaceIndexList();
	return NeighborFaceIndexList.GetLength();
}

// Face share any  point of this edge
template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_Mesh<ScalarType>::GetNeighborFaceIndexList() const
{
	DenseVector<int_max> NeighborFaceIndexList;

	const auto& AdjacentFaceIndexList0 = m_Data->MeshData->PointList[m_Data->PointIndex0].AdjacentFaceIndexList();
	const auto& AdjacentFaceIndexList1 = m_Data->MeshData->PointList[m_Data->PointIndex1].AdjacentFaceIndexList();

	if (AdjacentFaceIndexList0.IsEmpty() == true && AdjacentFaceIndexList1.IsEmpty() == true)
	{
		return NeighborFaceIndexList;
	}

	NeighborFaceIndexList.SetCapacity(AdjacentFaceIndexList0.GetLength() + AdjacentFaceIndexList1.GetLength());

	for (int_max k = 0; k < AdjacentFaceIndexList0.GetLength(); ++k)
	{
		NeighborFaceIndexList.Append(AdjacentFaceIndexList0[k]);
	}

	for (int_max k = 0; k < AdjacentFaceIndexList1.GetLength(); ++k)
	{
		NeighborFaceIndexList.Append(AdjacentFaceIndexList1[k]);
	}

	NeighborFaceIndexList = NeighborFaceIndexList.GetSubSet(OutputIndexList.FindUnique());

	return NeighborFaceIndexList;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetAdjacentCellCount() const
{
	auto AdjacentCellIndexList = this->GetAdjacentCellIndexList();
	return AdjacentCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_Mesh<ScalarType>::GetAdjacentCellIndexList() const
{
	DenseVector<int_max> AdjacentCellIndexList;	
	for (int_max k = 0; k < m_Data->AdjacentFaceIndexList.GetLength(); ++k)
	{
		AdjacentCellIndexList.Append(m_Data->MeshData->FaceList[m_Data->AdjacentFaceIndexList[k]].GetAdjacentCellIndexList());
	}
	AdjacentCellIndexList = AdjacentCellIndexList.GetSubSet(AdjacentCellIndexList.FindUnique());
	return AdjacentCellIndexList;
}

template<typename ScalarType>
inline
int_max Edge_Of_Mesh<ScalarType>::GetNeighborCellCount() const
{
	auto NeighborCellIndexList = this->GetNeighborCellIndexList();
	return NeighborCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Edge_Of_Mesh<ScalarType>::GetNeighborCellIndexList() const
{
	DenseVector<int_max> NeighborCellIndexList;

	const auto& AdjacentFaceIndexList0 = m_Data->MeshData->PointList[m_Data->PointIndex0].AdjacentFaceIndexList();
	const auto& AdjacentFaceIndexList1 = m_Data->MeshData->PointList[m_Data->PointIndex1].AdjacentFaceIndexList();

	if (AdjacentFaceIndexList0.IsEmpty() == true && AdjacentFaceIndexList1.IsEmpty() == true)
	{
		return OutputIndexList;
	}

	NeighborCellIndexList.SetCapacity(AdjacentFaceIndexList0.GetLength() + AdjacentFaceIndexList1.GetLength());

	for (int_max k = 0; k < AdjacentFaceIndexList0.GetLength(); ++k)
	{
		NeighborCellIndexList.Append(m_Data->MeshData->FaceList[AdjacentFaceIndexList0[k]].GetAdjacentCellIndexList());
	}

	for (int_max k = 0; k < AdjacentFaceIndexList1.GetLength(); ++k)
	{
		NeighborCellIndexList.Append(m_Data->MeshData->FaceList[AdjacentFaceIndexList1[k]].GetAdjacentCellIndexList());		
	}

	NeighborCellIndexList = OutputIndexList.GetSubSet(OutputIndexList.FindUnique());

	return NeighborCellIndexList;
}

template<typename ScalarType>
inline 
typename StandardAttribute_Of_Edge_Of_Mesh<ScalarType>& Edge_Of_Mesh<ScalarType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename ScalarType>
inline
const StandardAttribute_Of_Edge_Of_Mesh<ScalarType>& Edge_Of_Mesh<ScalarType>::Attribute() const
{
    return m_Data->Attribute;
}

//=========================================================== Face_Of_Mesh ===========================================================//

template<typename ScalarType>
inline
Face_Of_Mesh<ScalarType>::Face_Of_Mesh()
{
	this->ReCreate();
}

template<typename ScalarType>
inline
Face_Of_Mesh<ScalarType>::Face_Of_Mesh(const Face_Of_Mesh<ScalarType>& InputFace)
{
    (*this) = InputFace;
}

template<typename ScalarType>
inline
Face_Of_Mesh<ScalarType>::Face_Of_Mesh(Face_Of_Mesh<ScalarType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename ScalarType>
inline
Face_Of_Mesh<ScalarType>::~Face_Of_Mesh()
{
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::operator=(const Face_Of_Mesh<ScalarType>& InputFace)
{
    if (!InputFace.m_Data)
    {
		m_Data.reset();
        return;
    }

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Face_Of_Mesh<ScalarType>>();
	}

	m_Data->MeshData_wp = InputFace.m_Data->MeshData_wp;
    m_Data->MeshData = InputFace.m_Data->MeshData;
    m_Data->Index = InputFace.m_Data->Index;    
	m_Data->Name = InputFace.m_Data->Name;
    m_Data->PointIndexList = InputFace.m_Data->PointIndexList;
	m_Data->EdgeIndexList = InputFace.m_Data->EdgeIndexList;
    m_Data->Attribute = InputFace.m_Data->Attribute;
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::operator=(Face_Of_Mesh<ScalarType>&& InputFace)
{
    m_Data = std::move(InputFace.m_Data);
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::ReCreate()
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_Face_Of_Mesh<ScalarType>>();
    }    
	m_Data->MeshData_wp.reset();
	m_Data->MeshData = nullptr;
    m_Data->Index = -1;    
	m_Data->Name.Clear();
    m_Data->PointIndexList.Clear();
	m_Data->EdgeIndexList.Clear();
    m_Data->Attribute.Clear();
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::Clear(const MDK_Symbol_PureEmpty&)
{
    m_Data.reset();
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::SetParentMesh(Mesh<ScalarType>& ParentMesh)
{
	m_Data->MeshData_wp = ParentMesh.m_MeshData;
	m_Data->MeshData = ParentMesh.m_MeshData.get();
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::SetIndex(int_max FaceIndex)
{
    m_Data->Index = FaceIndex;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Face_Of_Mesh<ScalarType>::PointIndexList()
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Face_Of_Mesh<ScalarType>::PointIndexList() const
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Face_Of_Mesh<ScalarType>::EdgeIndexList()
{
	return m_Data->EdgeIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Face_Of_Mesh<ScalarType>::EdgeIndexList() const
{
	return m_Data->EdgeIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Face_Of_Mesh<ScalarType>::AdjacentCellIndexList()
{
	return m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Face_Of_Mesh<ScalarType>::AdjacentCellIndexList() const
{
	return m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
bool Face_Of_Mesh<ScalarType>::IsValid() const
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

template<typename ScalarType>
inline
bool Face_Of_Mesh<ScalarType>::IsOnPolygonMeshBoundary() const
{
	for (int_max k = 0; k < m_Data->EdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex = m_Data->EdgeIndexList[k];
		if (m_Data->MeshData->EdgeList[EdgeIndex].IsOnPolygonMeshBoundary() == true)
		{
			return true;
		}
	}
	return false;
}

template<typename ScalarType>
inline
bool Face_Of_Mesh<ScalarType>::IsOnPolyhedronMeshBoundary() const
{
	return (m_Data->AdjacentCellIndexList.GetLength() == 1);
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetIndex() const
{
	return m_Data->Index;
}


template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::SetName(String FaceName)
{
	if (!m_Data)
	{
		MDK_Error("Face has been deleted @ Face_Of_Mesh::SetName(...)")
		return;
	}

	if (FaceName.IsEmpty() == true)
	{
		MDK_Error("FaceName is empty @ Face_Of_Mesh::SetName(...)")
		return;
	}

	auto it = m_Data->MeshData->Map_Face_Name_to_Index.find(FaceName);
	if (it == m_Data->MeshData->Map_Face_Name_to_Index.end())//not found
	{
		auto it_old = m_Data->MeshData->Map_Face_Name_to_Index.find(m_Data->Name);
		if (it_old != m_Data->MeshData->Map_Face_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Face_Name_to_Index.erase(it_old);
		}
		m_Data->MeshData->Map_Face_Name_to_Index[FaceName] = m_Data->Index;
		m_Data->Name = std::move(FaceName);
	}
	else
	{
		if (it->second != m_Data->Index)
		{
			MDK_Error("Input FaceName has already been used for another point @ Face_Of_Mesh::SetName(...)")
			return;
		}
	}
}

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->MeshData->Map_Face_Name_to_Index.find(m_Data->Name);
		if (it != m_Data->MeshData->Map_Face_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Face_Name_to_Index.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename ScalarType>
inline
String Face_Of_Mesh<ScalarType>::GetName() const
{
	return m_Data->Name;
}

template<typename ScalarType>
inline MeshFaceTypeEnum Face_Of_Mesh<ScalarType>::GetType() const
{
	auto PointCountInCell = m_Data->PointIndexList.GetLength();
	if (PointCountInCell == 3)
	{
		return MeshFaceTypeEnum::Triangle;
	}
	else if (PointCountInCell == 4)
	{
		return MeshFaceTypeEnum::Quad;
	}
	else
	{
		return MeshFaceTypeEnum::Polygon;
	}
}

template<typename ScalarType>
inline void Face_Of_Mesh<ScalarType>::SetData(int_max Index, const DenseVector<ScalarType>& Data)
{
	m_Data->MeshData->FaceDataSet[Index].SeCol(m_Data->Index, Data);
}

template<typename ScalarType>
inline DenseVector<ScalarType> Face_Of_Mesh<ScalarType>::GetData(int_max Index) const
{
	DenseVector<ScalarType> Data;
	m_Data->MeshData->FaceDataSet[Index].GetCol(m_Data->Index, Data);
	return Data;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetPointCount() const
{
	return m_Data->PointIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetPointIndexList() const
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline int_max Face_Of_Mesh<ScalarType>::GetEdgeCount() const
{
	return m_Data->EdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetEdgeIndexList() const
{
	return m_Data->EdgeIndexList;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetAdjacentFaceCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_Data->EdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->EdgeIndexList[k];
		Counter += m_Data->MeshData->EdgeList[EdgeIndex_k].AdjacentFaceIndexList().GetLength() - 1;
	}
	return Counter;
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetAdjacentFaceIndexList() const
{
	DenseVector<int_max> AdjacentFaceIndexList;
	AdjacentFaceIndexList.SetCapacity(m_Data->EdgeIndexList.GetLength());
	for (int_max k = 0; k < m_Data->EdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = m_Data->EdgeIndexList[k];
		const auto& TempList = m_Data->MeshData->EdgeList[EdgeIndex_k].AdjacentFaceIndexList();
		for (int_max n = 0; n < TempList.GetLength(); ++n)
		{
			if (TempList[n] != m_Data->Index)
			{
				AdjacentFaceIndexList.Append(TempList[n]);
			}
		}	
	}
	return AdjacentFaceIndexList;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetNeighborFaceCount() const
{// share a vertex point
	auto NeighborFaceIndexList = this->GetNeighborFaceIndexList();
	return NeighborFaceIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetNeighborFaceIndexList() const
{
	DenseVector<int_max> NeighborFaceIndexList;
	NeighborFaceIndexList.SetCapacity(m_Data->PointIndexList.GetLength());
	for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		const auto& FaceIndexList_k = m_Data->MeshData->PointList[m_Data->PointIndexList[k]].AdjacentFaceIndexList();
		for (int_max n = 0; n < FaceIndexList_k.GetLength(); ++n)
		{
			if (FaceIndexList_k[n] != m_Data->Index)
			{
				NeighborFaceIndexList.Append(FaceIndexList_k[n]);
			}
		}
	}
	NeighborFaceIndexList = NeighborFaceIndexList.GetSubSet(NeighborFaceIndexList.FindUnique());
	return NeighborFaceIndexList;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetAdjacentCellCount() const
{
	return m_Data->AdjacentCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetAdjacentCellIndexList() const
{
	return m_Data->AdjacentCellIndexList;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetNeighborCellCount() const
{
	auto NeighborCellIndexList = this->GetNeighborCellIndexList();
	return NeighborCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetNeighborCellIndexList() const
{
	DenseVector<int_max> NeighborCellIndexList;
	NeighborCellIndexList.SetCapacity(6);
	auto NeighborFaceIndexList = this->GetNeighborFaceIndexList();
	for (int_max k = 0; k < NeighborFaceIndexList.GetLength(); ++k)
	{
		NeighborCellIndexList.Append(m_Data->MeshData->FaceList[NeighborFaceIndexList[k]].AdjacentCellIndexList());
	}
	NeighborCellIndexList = NeighborCellIndexList.GetSubSet(NeighborCellIndexList.FindUnique());
	return NeighborCellIndexList;
}

template<typename ScalarType>
inline 
typename StandardAttribute_Of_Face_Of_Mesh<ScalarType>& Face_Of_Mesh<ScalarType>::Attribute()
{
    return m_Data->Attribute;
}

template<typename ScalarType>
inline
const StandardAttribute_Of_Face_Of_Mesh<ScalarType>& Face_Of_Mesh<ScalarType>::Attribute() const
{
    return m_Data->Attribute;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetRelativeIndexOfPoint(int_max PointIndex) const
{
	for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		if (m_Data->PointIndexList[k] == PointIndex)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
inline
int_max Face_Of_Mesh<ScalarType>::GetEdgeIndexByPoint(int_max PointIndexA, int_max PointIndexB) const
{
	int_max tempIndexA = -1;
	int_max tempIndexB = -1;
	for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		if (m_Data->PointIndexList[k] == PointIndexA)
		{
			tempIndexA = k;
		}
		else if (m_Data->PointIndexList[k] == PointIndexB)
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
	auto PointCount = m_Data->EdgeIndexList.GetLength();
	if (tempIndexB = tempIndexA + 1 || tempIndexB = tempIndexA + 1 - PointCount)
	{
		EdgeIndex = m_Data->EdgeIndexList[tempIndexA];
	}
	else if (tempIndexA = tempIndexB + 1 || tempIndexA = tempIndexB + 1 - PointCount)
	{
		EdgeIndex = m_Data->EdgeIndexList[tempIndexB];
	}
	return EdgeIndex;
}

template<typename ScalarType>
bool Face_Of_Mesh<ScalarType>::CheckPointOrder(int_max PointIndexA, int_max PointIndexB) const
{// true: A -> B ; false: B->A
	int_max tempIndexA = -1;
	int_max tempIndexB = -1;
	for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		if (m_Data->PointIndexList[k] == PointIndexA)
		{
			tempIndexA = k;
		}
		else if (m_Data->PointIndexList[k] == PointIndexB)
		{
			tempIndexB = k;
		}

		if (tempIndexA >= 0 && tempIndexB >= 0)
		{
			break;
		}
	}

	if (tempIndexB > tempIndexA)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetPointIndexList_LeadBy(int_max PointIndexA) const
{// output list {PointIndexA, ...}	
	int_max tempIndexA = -1;
	for (int_max k = 0; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		if (m_Data->PointIndexList[k] == PointIndexA)
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
		return m_Data->PointIndexList;
	}

	DenseVector<int_max> PointIndexList_output;

	if (tempIndexA < 0)
	{
		return PointIndexList_output;
	}
	
	//now tempIndexA > 0
	PointIndexList_output.SetCapacity(m_Data->PointIndexList.GetLength());
	for (int_max k = tempIndexA; k < m_Data->PointIndexList.GetLength(); ++k)
	{
		PointIndexList_output.Append(m_Data->PointIndexList[k]);
	}
	for (int_max k = 0; k < tempIndexA; ++k)
	{
		PointIndexList_output.Append(m_Data->PointIndexList[k]);
	}
	return PointIndexList_output;
}

template<typename ScalarType>
inline
DenseVector<int_max> Face_Of_Mesh<ScalarType>::GetPointIndexList_LeadBy(int_max PointIndexA, int_max PointIndexB) const
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

	DenseVector<Index_Of_Point_Of_Mesh> PointIndexList_output;

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

template<typename ScalarType>
inline
void Face_Of_Mesh<ScalarType>::ReversePointOrder()
{
	auto PointIndexList_old = m_Data->PointIndexList;
	auto L = PointIndexList_old.GetLength();
	for (int_max k = 0; k < L; ++k)
	{
		m_Data->PointIndexList[k] = PointIndexList_old[L-1-k];
	}
}

//====================================== Cell_Of_Mesh ==============================================================//

template<typename ScalarType>
inline
Cell_Of_Mesh<ScalarType>::Cell_Of_Mesh()
{
	this->ReCreate();
}

template<typename ScalarType>
inline
Cell_Of_Mesh<ScalarType>::Cell_Of_Mesh(const Cell_Of_Mesh<ScalarType>& InputCell)
{
	(*this) = InputCell;
}

template<typename ScalarType>
inline
Cell_Of_Mesh<ScalarType>::Cell_Of_Mesh(Cell_Of_Mesh<ScalarType>&& InputCell)
{
	m_Data = std::move(InputCell.m_Data);
}

template<typename ScalarType>
inline
Cell_Of_Mesh<ScalarType>::~Cell_Of_Mesh()
{
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::operator=(const Cell_Of_Mesh<ScalarType>& InputCell)
{
	if (!InputCell.m_Data)
	{
		m_Data.reset();
		return;
	}

	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Cell_Of_Mesh<ScalarType>>();
	}

	m_Data->MeshData_wp = InputCell.m_Data->MeshData_wp;
	m_Data->MeshData = InputCell.m_Data->MeshData;
	m_Data->Index = InputCell.m_Data->Index;
	m_Data->Name = InputCell.m_Data->Name;
	m_Data->Type = InputCell.m_Data->Type;
	m_Data->PointIndexList = InputCell.m_Data->PointIndexList;
	m_Data->FaceIndexList = InputCell.m_Data->FaceIndexList;
	m_Data->Attribute = InputCell.m_Data->Attribute;
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::operator=(Cell_Of_Mesh<ScalarType>&& InputCell)
{
	m_Data = std::move(InputCell.m_Data);
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::ReCreate()
{
	if (!m_Data)
	{
		m_Data = std::make_unique<Data_Of_Cell_Of_Mesh<ScalarType>>();
	}
	m_Data->MeshData_wp.reset();
	m_Data->MeshData = nullptr;
	m_Data->Index = -1;
	m_Data->Name.Clear();
	m_Data->Type = MeshCellTypeEnum::Polyhedron;
	m_Data->PointIndexList.Clear();
	m_Data->FaceIndexList.Clear();
	m_Data->Attribute.Clear();
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::SetParentMesh(Mesh<ScalarType>& ParentMesh)
{
	m_Data->MeshData_wp = ParentMesh.m_MeshData;
	m_Data->MeshData = ParentMesh.m_MeshData.get();
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::SetIndex(int_max CellIndex)
{
	m_Data->Index = CellIndex;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Cell_Of_Mesh<ScalarType>::PointIndexList()
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Cell_Of_Mesh<ScalarType>::PointIndexList() const
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& Cell_Of_Mesh<ScalarType>::FaceIndexList()
{
	return m_Data->FaceIndexList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& Cell_Of_Mesh<ScalarType>::FaceIndexList() const
{
	return m_Data->FaceIndexList;
}

template<typename ScalarType>
inline 
void Cell_Of_Mesh<ScalarType>::SetType(MeshCellTypeEnum CellType)
{
	m_Data->Type = CellType;
}

template<typename ScalarType>
inline
bool Cell_Of_Mesh<ScalarType>::IsValid() const
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

template<typename ScalarType>
inline
bool Cell_Of_Mesh<ScalarType>::IsOnPolyhedronMeshBoundary() const
{
	for (int_max k = 0; k < m_Data->FaceIndexList.GetLength(); ++k)
	{
		auto FaceIndex = m_Data->FaceIndexList[k];
		if (m_Data->MeshData->FaceList[FaceIndex].IsOnPolyhedronMeshBoundary() == true)
		{
			return true;
		}
	}
	return false;
}

template<typename ScalarType>
inline
int_max Cell_Of_Mesh<ScalarType>::GetIndex() const
{
	return m_Data->Index;
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::SetName(String CellName)
{
	if (!m_Data)
	{
		MDK_Error("Cell has been deleted @ Cell_Of_Mesh::SetName(...)")
			return;
	}

	if (CellName.IsEmpty() == true)
	{
		MDK_Error("CellName is empty @ Cell_Of_Mesh::SetName(...)")
		return;
	}

	auto it = m_Data->MeshData->Map_Cell_Name_to_Index.find(CellName);
	if (it == m_Data->MeshData->Map_Cell_Name_to_Index.end())//not found
	{
		auto it_old = m_Data->MeshData->Map_Cell_Name_to_Index.find(m_Data->Name);
		if (it_old != m_Data->MeshData->Map_Cell_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Cell_Name_to_Index.erase(it_old);
		}
		m_Data->MeshData->Map_Cell_Name_to_Index[CellName] = m_Data->Index;
		m_Data->Name = std::move(CellName);
	}
	else
	{
		if (it->second != m_Data->Index)
		{
			MDK_Error("Input CellName has already been used for another point @ Cell_Of_Mesh::SetName(...)")
			return;
		}
	}	
}

template<typename ScalarType>
inline
void Cell_Of_Mesh<ScalarType>::EraseName()
{
	if (m_Data->Name.IsEmpty() == false)
	{
		auto it = m_Data->MeshData->Map_Cell_Name_to_Index.find(m_Data->Name);
		if (it != m_Data->MeshData->Map_Cell_Name_to_Index.end())
		{
			m_Data->MeshData->Map_Cell_Name_to_Index.erase(it);
		}

		m_Data->Name.Clear();
	}
}

template<typename ScalarType>
inline
String Cell_Of_Mesh<ScalarType>::GetName() const
{
	return m_Data->Name;
}

template<typename ScalarType>
inline
MeshCellTypeEnum Cell_Of_Mesh<ScalarType>::GetType() const
{
	return m_Data->Type;
}

template<typename ScalarType>
inline void Cell_Of_Mesh<ScalarType>::SetData(int_max Index, const DenseVector<ScalarType>& Data)
{
	m_Data->MeshData->CellDataSet[Index].SetCol(m_Data->Index, Data);
}

template<typename ScalarType>
inline DenseVector<ScalarType> Cell_Of_Mesh<ScalarType>::GetData(int_max Index) const
{
	DenseVector<ScalarType> Data;
	m_Data->MeshData->CellDataSet[Index].GetCol(m_Data->Index, Data);
	return Data;
}

template<typename ScalarType>
inline
int_max Cell_Of_Mesh<ScalarType>::GetPointCount() const
{
	return m_Data->PointIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_Mesh<ScalarType>::GetPointIndexList() const
{
	return m_Data->PointIndexList;
}

template<typename ScalarType>
inline 
int_max Cell_Of_Mesh<ScalarType>::GetEdgeCount() const
{
	auto EdgeIndexList = this->GetEdgeIndexList();
	return EdgeIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_Mesh<ScalarType>::GetEdgeIndexList() const
{
	DenseVector<int_max> EdgeIndexList;
	for (int_max k = 0; k < m_Data->FaceIndexList.GetLength(); ++k)
	{
		EdgeIndexList.Append(m_Data > MeshData->FaceList[m_Data->FaceIndexList[k]].EdgeIndexList());
	}
	EdgeIndexList = EdgeIndexList.GetSubSet(EdgeIndexList.FindUnique());
	return EdgeIndexList;
}

template<typename ScalarType>
inline
int_max Cell_Of_Mesh<ScalarType>::GetFaceCount() const
{
	return m_Data->FaceIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_Mesh<ScalarType>::GetFaceIndexList() const
{
	return m_Data->FaceIndexList;
}

template<typename ScalarType>
inline
int_max Cell_Of_Mesh<ScalarType>::GetAdjacentCellCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_Data->FaceIndexList.GetLength(); ++k)
	{
		auto FaceIndex_k = m_Data->FaceIndexList[k];
		Counter += m_Data->MeshData->FaceIndexList[FaceIndex_k].AdjacentCellIndexList().GetLength() - 1;
	}
	return Counter;
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_Mesh<ScalarType>::GetAdjacentCellIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(m_Data->FaceIndexList.GetLength());
	for (int_max k = 0; k < m_Data->FaceIndexList.GetLength(); ++k)
	{
		auto FaceIndex_k = m_Data->FaceIndexList[k];
		const auto& TempList = m_Data->MeshData->FaceList[FaceIndex_k].AdjacentCellIndexList();
		for (int_max n = 0; n < TempList.GetLength(); ++n)
		{
			if (TempList[n] != m_Data->Index)
			{
				OutputIndexList.Append(TempList[n]);
			}
		}
	}
	return OutputIndexList;
}

template<typename ScalarType>
inline
int_max Cell_Of_Mesh<ScalarType>::GetNeighborCellCount() const
{
	auto NeighborCellIndexList = this->GetNeighborCellIndexList();
	return NeighborCellIndexList.GetLength();
}

template<typename ScalarType>
inline
DenseVector<int_max> Cell_Of_Mesh<ScalarType>::GetNeighborCellIndexList() const
{
	DenseVector<int_max> NeighborCellIndexList;
	NeighborCellIndexList.SetCapacity(m_Data->FaceIndexList.GetLength());
	for (int_max k = 0; k < m_Data->FaceIndexList.GetLength(); ++k)
	{
		NeighborCellIndexList.Append(m_Data->MeshData->FaceList[m_Data->FaceIndexList[k]].GetNeighborCellIndexList());
	}
	NeighborCellIndexList = NeighborCellIndexList.GetSubSet(NeighborCellIndexList.FindUnique());
	return NeighborCellIndexList;
}

template<typename ScalarType>
inline
typename StandardAttribute_Of_Cell_Of_Mesh<ScalarType>& Cell_Of_Mesh<ScalarType>::Attribute()
{
	return m_Data->Attribute;
}

template<typename ScalarType>
inline
const StandardAttribute_Of_Cell_Of_Mesh<ScalarType>& Cell_Of_Mesh<ScalarType>::Attribute() const
{
	return m_Data->Attribute;
}

}// namespace mdk
