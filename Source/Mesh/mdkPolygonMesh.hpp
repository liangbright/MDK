#pragma once

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh()
{
	this->Recreate();
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(const MDK_Symbol_PureEmpty&)
{
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
	this->Copy(std::move(InputMesh));
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

	m_MeshData->ID = -1;
	m_MeshData->Name = "";

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
	
	m_MeshData->Map_PointName_to_PointIndex.clear();
	m_MeshData->Map_EdgeName_to_EdgeIndex.clear();
	m_MeshData->Map_FaceName_to_FaceIndex.clear();

	m_MeshData->PointSetList.Clear();
	m_MeshData->Map_PointSetName_to_PointSetIndex.clear();

	m_MeshData->EdgeSetList.Clear();
	m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.clear();

	m_MeshData->FaceSetList.Clear();
	m_MeshData->Map_FaceSetName_to_FaceSetIndex.clear();

    m_MeshData->Attribute.Clear();
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Clear(const MDK_Symbol_PureEmpty&)
{
	if (m_MeshData)
	{
		m_MeshData.reset();
	}
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Copy(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	//prevent self copy
	if (&m_MeshData == &InputMesh.m_MeshData)
	{
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
	}

	if (InputMesh.IsEmpty() == true)
    {
		this->Clear();
        return;
    }

	m_MeshData->ID = InputMesh.m_MeshData->ID;
	m_MeshData->Name = InputMesh.m_MeshData->Name;

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

	m_MeshData->Map_PointName_to_PointIndex = InputMesh.m_MeshData->Map_PointName_to_PointIndex;
	m_MeshData->Map_EdgeName_to_EdgeIndex = InputMesh.m_MeshData->Map_EdgeName_to_EdgeIndex;
	m_MeshData->Map_FaceName_to_FaceIndex = InputMesh.m_MeshData->Map_FaceName_to_FaceIndex;

	m_MeshData->PointSetList = InputMesh.m_MeshData->PointSetList;
	m_MeshData->Map_PointSetName_to_PointSetIndex = InputMesh.m_MeshData->Map_PointSetName_to_PointSetIndex;

	m_MeshData->EdgeSetList = InputMesh.m_MeshData->EdgeSetList;
	m_MeshData->Map_EdgeSetName_to_EdgeSetIndex = InputMesh.m_MeshData->Map_EdgeSetName_to_EdgeSetIndex;

	m_MeshData->FaceSetList = InputMesh.m_MeshData->FaceSetList;
	m_MeshData->Map_FaceSetName_to_FaceSetIndex = InputMesh.m_MeshData->Map_FaceSetName_to_FaceSetIndex;

    m_MeshData->Attribute = InputMesh.m_MeshData->Attribute;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Copy(PolygonMesh<MeshAttributeType>&& InputMesh)
{
	//prevent self copy
	if (&m_MeshData == &InputMesh.m_MeshData)
	{
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
	}

	if (InputMesh.IsEmpty() == true)
	{
		this->Clear();
		return;
	}

	m_MeshData->ID = InputMesh.m_MeshData->ID;
	m_MeshData->Name = std::move(InputMesh.m_MeshData->Name);

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

	m_MeshData->Map_PointName_to_PointIndex = std::move(InputMesh.m_MeshData->Map_PointName_to_PointIndex);
	m_MeshData->Map_EdgeName_to_EdgeIndex = std::move(InputMesh.m_MeshData->Map_EdgeName_to_EdgeIndex);
	m_MeshData->Map_FaceName_to_FaceIndex = std::move(InputMesh.m_MeshData->Map_FaceName_to_FaceIndex);

	m_MeshData->PointSetList = std::move(InputMesh.m_MeshData->PointSetList);
	m_MeshData->Map_PointSetName_to_PointSetIndex = std::move(InputMesh.m_MeshData->Map_PointSetName_to_PointSetIndex);

	m_MeshData->EdgeSetList = std::move(InputMesh.m_MeshData->EdgeSetList);
	m_MeshData->Map_EdgeSetName_to_EdgeSetIndex = std::move(InputMesh.m_MeshData->Map_EdgeSetName_to_EdgeSetIndex);

	m_MeshData->FaceSetList = std::move(InputMesh.m_MeshData->FaceSetList);
	m_MeshData->Map_FaceSetName_to_FaceSetIndex = std::move(InputMesh.m_MeshData->Map_FaceSetName_to_FaceSetIndex);

	m_MeshData->Attribute = std::move(InputMesh.m_MeshData->Attribute);
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Share(PolygonMesh& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::ForceShare(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::Recreate()
{
	m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
}

//-------------------------------------------------------------------

template<typename MeshAttributeType>
inline 
bool PolygonMesh<MeshAttributeType>::IsEmpty() const
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
bool PolygonMesh<MeshAttributeType>::IsPureEmpty() const
{
	return (!m_MeshData);
}

template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetPointCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->PointValidityFlagList.Sum();
}

template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetEdgeCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->EdgeValidityFlagList.Sum();	
}

template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetDirectedEdgeCount() const
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
int_max PolygonMesh<MeshAttributeType>::GetFaceCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->FaceValidityFlagList.Sum();
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetNamedPointCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->Map_PointName_to_PointIndex.size();
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetNamedFaceCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->Map_FaceName_to_FaceIndex.size();
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetMaxValueOfPointIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->PointPositionTable.GetColCount();	
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetMaxValueOfEdgeIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->EdgeList.GetLength();
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetMaxValueOfFaceIndex() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->FaceList.GetLength();	
}

//------ Get/Set GlobalAttribute -----------------------------------//

template<typename MeshAttributeType>
inline
typename MeshAttributeType::GlobalAttribute& PolygonMesh<MeshAttributeType>::Attribute()
{
    return m_MeshData->Attribute;
}

template<typename MeshAttributeType>
inline
const typename MeshAttributeType::GlobalAttribute& PolygonMesh<MeshAttributeType>::Attribute() const
{
    return m_MeshData->Attribute;
}

//--------------------------------------------------------------//

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::SetID(int_max ID)
{
	m_MeshData->ID = ID;
}

template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetID() const
{
	return m_MeshData->ID;
}

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::SetName(String Name)
{
	m_MeshData->Name = std::move(Name);
}

template<typename MeshAttributeType>
String PolygonMesh<MeshAttributeType>::GetName() const
{
	return m_MeshData->Name;
}

//------------- Get All the position (valid point) --------------------------------//

template<typename MeshAttributeType>
inline 
DenseMatrix<typename MeshAttributeType::ScalarType> 
PolygonMesh<MeshAttributeType>::GetPointPosition(const MDK_Symbol_ALL&) const
{
	DenseMatrix<ScalarType> PointPositionMatrix;
	this->GetPointPosition(ALL, PointPositionMatrix);
	return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::GetPointPosition(const MDK_Symbol_ALL&, DenseMatrix<ScalarType>& PositionMatrix) const
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

//---- Get/Set 3D Position by PointIndex --------------------------------------------------------------------------//

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
SetPointPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointIndex, { x, y, z });
}

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::SetPointPosition(int_max PointIndex, const ScalarType Position[3])
{
    m_MeshData->PointPositionTable.SetCol(PointIndex, Position);
}

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::SetPointPosition(int_max PointIndex, const DenseVector<ScalarType,3>& Position)
{
	m_MeshData->PointPositionTable.SetCol(PointIndex, Position);
}

template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
SetPointPosition(const DenseVector<int_max>& PointIndexList, const DenseMatrix<ScalarType>& PointPositionMatrix)
{
	if (PointIndexList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == true)
	{
		return;
	}
	else if (PointIndexList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == false)
	{
		MDK_Error("Invalid input @ PolygonMesh::SetPointPosition(...)")
		return;
	}
	else if (PointIndexList.IsEmpty() == false && PointPositionMatrix.IsEmpty() == true)
	{
		MDK_Error("Invalid input @ PolygonMesh::SetPointPosition(...)")
		return;
	}

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		auto PointIndex = PointIndexList[k];
		if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColCount())
		{
			MDK_Error("Invalid PointIndex @ PolygonMesh::SetPointPosition(...)")
			return;
		}

		m_MeshData->PointPositionTable.SetCol(PointIndex, PointPositionMatrix.GetPointerOfCol(k));
	}
}

template<typename MeshAttributeType>
inline
DenseVector<typename MeshAttributeType::ScalarType, 3> 
PolygonMesh<MeshAttributeType>::GetPointPosition(int_max PointIndex) const
{
    DenseVector<ScalarType, 3> Position;
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position.GetPointer());
    return Position;
}

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::
GetPointPosition(int_max PointIndex, ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename MeshAttributeType>
inline 
void PolygonMesh<MeshAttributeType>::GetPointPosition(int_max PointIndex, ScalarType Position[3]) const
{
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position);
}


template<typename MeshAttributeType>
inline
DenseMatrix<typename MeshAttributeType::ScalarType> 
PolygonMesh<MeshAttributeType>::GetPointPosition(const DenseVector<int_max>& PointIndexList) const
{
    DenseMatrix<MeshAttributeType::ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointIndexList, PointPositionMatrix);
    return PointPositionMatrix;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPosition(const DenseVector<int_max>& PointIndexList, DenseMatrix<ScalarType>& PointPositionMatrix) const
{
    if (PointIndexList.IsEmpty() == true)
    {
        PointPositionMatrix.FastResize(0, 0);
        return;
    }

    PointPositionMatrix.FastResize(3, PointIndexList.GetLength());
    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        auto PointIndex = PointIndexList[k];
        if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColCount())
        {
            MDK_Error("Invalid PointIndex @ PolygonMesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex));
    }
}

//------------------------- Mesh Element ----------------------------------------------

template<typename MeshAttributeType>
inline
Point_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Point(int_max PointIndex)
{
    return m_MeshData->PointList[PointIndex];
}

template<typename MeshAttributeType>
inline
const Point_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Point(int_max PointIndex) const
{
    return m_MeshData->PointList[PointIndex];
}

template<typename MeshAttributeType>
inline
Edge_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Edge(int_max EdgeIndex)
{
    return m_MeshData->EdgeList[EdgeIndex];
}

template<typename MeshAttributeType>
inline
const Edge_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Edge(int_max EdgeIndex) const
{
    return m_MeshData->EdgeList[EdgeIndex];
}

template<typename MeshAttributeType>
inline
DirectedEdge_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::DirectedEdge(DirectedEdgeIndexType DirectedEdgeIndex)
{
    auto FaceIndex = DirectedEdgeIndex.FaceIndex;
    auto RelativeIndex = DirectedEdgeIndex.RelativeIndex;
	return m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex];
}

template<typename MeshAttributeType>
inline
const DirectedEdge_Of_PolygonMesh<MeshAttributeType>& 
PolygonMesh<MeshAttributeType>::DirectedEdge(DirectedEdgeIndexType DirectedEdgeIndex) const
{
	auto FaceIndex = DirectedEdgeIndex.FaceIndex;
	auto RelativeIndex = DirectedEdgeIndex.RelativeIndex;
    return m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[RelativeIndex];
}

template<typename MeshAttributeType>
inline
Face_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Face(int_max FaceIndex)
{
    return m_MeshData->FaceList[FaceIndex];
}

template<typename MeshAttributeType>
inline
const Face_Of_PolygonMesh<MeshAttributeType>& PolygonMesh<MeshAttributeType>::Face(int_max FaceIndex) const
{
    return m_MeshData->FaceList[FaceIndex];
}

//-------------- check index -------------------------------------------------------//

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidPointIndex(int_max PointIndex) const
{
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
bool PolygonMesh<MeshAttributeType>::IsValidEdgeIndex(int_max EdgeIndex) const
{
    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength())
    {
        return false;
    }
	return (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1);    
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::IsValidDirectedEdgeIndex(DirectedEdgeIndexType DirectedEdgeIndex) const
{
    auto FaceIndex = DirectedEdgeIndex.FaceIndex;
    auto RelativeIndex = DirectedEdgeIndex.RelativeIndex;

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
bool PolygonMesh<MeshAttributeType>::IsValidFaceIndex(int_max FaceIndex) const
{
    if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceValidityFlagList.GetLength())
    {
        return false;
    }    
	return (m_MeshData->FaceValidityFlagList[FaceIndex] == 1);  
}

//--------- get Valid IndexList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetPointIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(m_MeshData->PointList.GetLength());	
    for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_MeshData->PointValidityFlagList[k] == 1)
        {
			OutputIndexList.Append(k);
        }
    }
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline 
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetEdgeIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(m_MeshData->EdgeList.GetLength());	
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_MeshData->EdgeValidityFlagList[k] == 1)
        {
			OutputIndexList.Append(k);
        }
    }
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexList() const
{
	DenseVector<DirectedEdgeIndexType> OutputIndexList;
	OutputIndexList.SetCapacity(3*m_MeshData->FaceList.GetLength());
    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
			for (int_max n = 0; n < m_MeshData->FaceList[k].DirectedEdgeList().GetLength(); ++n)
			{
				DirectedEdgeIndexType DirectedEdgeIndex;
				DirectedEdgeIndex.FaceIndex = k;
				DirectedEdgeIndex.RelativeIndex = n;
				OutputIndexList.Append(DirectedEdgeIndex);
			}
        }
    }
	return OutputIndexList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetFaceIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(m_MeshData->FaceList.GetLength());
    for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_MeshData->FaceValidityFlagList[k] == 1)
        {
			OutputIndexList.Append(k);
        }
    }
	return OutputIndexList;
}

//----------- get PointIndex by Position, ID, Name ----------------------------------------------//

template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetPointIndexByPosition(const DenseVector<ScalarType, 3>& Position, ScalarType DistanceThreshold) const
{
    return this->GetPointIndexByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetPointIndexByPosition(const ScalarType Position[3], ScalarType DistanceThreshold) const
{
    return this->GetPointIndexByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetPointIndexByPosition(ScalarType x, ScalarType y, ScalarType z, ScalarType DistanceThreshold) const
{
    if (this->IsEmpty() == true)
    {        
        return -1;
    }

    if (DistanceThreshold < 0)
    {
        MDK_Error("DistanceThreshold < 0 @ PolygonMesh::GetPointIndexByPosition(...)")
        return -1;
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
                return k;
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
		return PointIndex_min;
    }
	else
	{
		return -1;
	}
}


template<typename MeshAttributeType>
inline int_max PolygonMesh<MeshAttributeType>::GetPointIndexByID(int_max PointID) const
{
    if (PointID < 0)
    {
        //MDK_Warning("Invalid PointID (< 0) @ PolygonMesh::GetPointIndexByID(...)")
        return -1;
    }

    auto it = m_MeshData->Map_PointID_to_PointIndex.find(PointID);
    if (it != m_MeshData->Map_PointID_to_PointIndex.end())
    {
		return it->second;
    }
    else
    {
        //MDK_Warning("Invalid PointID @ PolygonMesh::GetPointIndexByID(...)")
		return -1;
    }
}

template<typename MeshAttributeType>
inline int_max PolygonMesh<MeshAttributeType>::GetPointIndexByName(const String& PointName) const
{
	if (PointName.IsEmpty() == true)
	{
		//MDK_Warning("PointName is empty @ PolygonMesh::GetPointIndexByID(...)")
		return -1;
	}

	auto it = m_MeshData->Map_PointName_to_PointIndex.find(PointName);
	if (it != m_MeshData->Map_PointName_to_PointIndex.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid PointName @ PolygonMesh::GetPointIndexByID(...)")
		return -1;
	}
}

//----------- get EdgeIndex by Point, ID, Name -------------------------------------//
template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetEdgeIndexByPoint(int_max PointIndex0, int_max PointIndex1) const
{
	if (this->IsValidPointIndex(PointIndex0) == false || this->IsValidPointIndex(PointIndex1) == false)
	{
		//MDK_Warning("PointIndex0 or PointIndex1 is invalid @ PolygonMesh::GetEdgeIndexByPoint(...)")		
		return -1;
	}

	if (m_MeshData->PointList[PointIndex0].IsOnEdge() == true && m_MeshData->PointList[PointIndex1].IsOnEdge() == true)
	{
		const auto& tempAdjacentEdgeIndexList = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
		for (int_max k = 0; k < tempAdjacentEdgeIndexList.GetLength(); ++k)
		{
			int_max tempPointIdex_a, tempPointIdex_b;
			m_MeshData->EdgeList[tempAdjacentEdgeIndexList[k]].GetPointIndexList(tempPointIdex_a, tempPointIdex_b);
			if (PointIndex1 == tempPointIdex_a || PointIndex1 == tempPointIdex_b)
			{
				return tempAdjacentEdgeIndexList[k];
			}
		}
	}

	//MDK_Warning("No Edge between PointIndex0 and PointIndex1 @ PolygonMesh::GetEdgeIndexByPoint(...)")
	return -1;
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetEdgeIndexByID(int_max EdgeID) const
{
    if (EdgeID < 0)
    {
        //MDK_Warning("Invalid EdgeID (< 0) @ PolygonMesh::GetEdgeIndexByID(...)")        
        return -1;
    }

    auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.find(EdgeID);
    if (it != m_MeshData->Map_EdgeID_to_EdgeIndex.end())
    {
        return it->second;
    }
    else
    {
        //MDK_Warning("Invalid EdgeID @ PolygonMesh::GetEdgeIndexByID(...)")
		return -1;
    }
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetEdgeIndexByName(const String& EdgeName) const
{
	if (EdgeName.IsEmpty() == true)
	{
		//MDK_Warning("EdgeName is empty @ PolygonMesh::GetEdgeIndexByName(...)")        
		return -1;
	}

	auto it = m_MeshData->Map_EdgeName_to_EdgeIndex.find(EdgeName);
	if (it != m_MeshData->Map_EdgeName_to_EdgeIndex.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid EdgeName @ PolygonMesh::GetEdgeIndexByName(...)")
		return -1;
	}
}

//---------- - get DirectedEdgeIndex by Point, ID, Name ------------------------------------ -//
template<typename MeshAttributeType>
inline
DenseVector<DirectedEdgeIndex_Of_PolygonMesh> PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexListByPoint(int_max PointIndex_start, int_max PointIndex_end) const
{
	DenseVector<DirectedEdgeIndexType> DirectedEdgeIndexList;

	if (this->IsValidPointIndex(PointIndex_start) == false || this->IsValidPointIndex(PointIndex_end) == false)
	{
		return DirectedEdgeIndexList;
	}

	auto OutgoingDirectedEdgeIndexList_s = m_MeshData->PointList[PointIndex_start].GetOutgoingDirectedEdgeIndexList();
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexList_s.GetLength(); ++k)
	{
		auto tempFaceIndex = OutgoingDirectedEdgeIndexList_s[k].FaceIndex;
		auto tempRelativeIndex = OutgoingDirectedEdgeIndexList_s[k].RelativeIndex;
		auto tempPointIndex_end = m_MeshData->FaceList[tempFaceIndex].DirectedEdgeList()[tempRelativeIndex].GetEndPointIndex();
		if (tempPointIndex_end == PointIndex_end)
		{
			DirectedEdgeIndexType tempIndex;
			tempIndex.FaceIndex = tempFaceIndex;
			tempIndex.RelativeIndex = tempRelativeIndex;			
			DirectedEdgeIndexList.Append(tempIndex);
		}
	}

	return DirectedEdgeIndexList;
}

template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_PolygonMesh PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexByID(int_max DirectedEdgeID) const
{
	DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex_invalid;

    if (DirectedEdgeID < 0)
    {
        //MDK_Warning("Invalid DirectedEdgeID (< 0) @ PolygonMesh::GetDirectedEdgeIndexByID(...)")        
        return DirectedEdgeIndex_invalid;
    }

    auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(DirectedEdgeID);
    if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
    {
        return it->second;
    }
    else
    {
        //MDK_Warning("Invalid DirectedEdgeID @ PolygonMesh::GetDirectedEdgeIndex(...)")
		return DirectedEdgeIndex_invalid;
    }
}


template<typename MeshAttributeType>
inline
DirectedEdgeIndex_Of_PolygonMesh PolygonMesh<MeshAttributeType>::GetDirectedEdgeIndexByName(const String& DirectedEdgeName) const
{
	DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex_invalid;

	if (DirectedEdgeName.IsEmpty() == true)
	{
		//MDK_Warning("DirectedEdgeName is empty @ PolygonMesh::GetDirectedEdgeIndexByName(...)")        
		return DirectedEdgeIndex_invalid;
	}

	auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(DirectedEdgeName);
	if (it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("invalid DirectedEdgeName @ PolygonMesh::GetDirectedEdgeIndexByName(...)")
		return DirectedEdgeIndex_invalid;
	}
}


//----------- get FaceIndex by Point, Edge, ID, Name ----------//
template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetFaceIndexByPoint(const DenseVector<int_max>& PointIndexList) const
{
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			//MDK_Warning("PointIndexList is invalid @ PolygonMesh::GetFaceIndexByPoint(...)")
			return -1;
		}
	}

	DenseVector<int_max> EdgeIndexList;
	EdgeIndexList.Resize(PointIndexList.GetLength());
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (k < PointIndexList.GetLength() - 1)
		{
			EdgeIndexList[k] = this->GetEdgeIndexByPoint(PointIndexList[k], PointIndexList[k + 1]);
		}
		else
		{
			EdgeIndexList[k] = this->GetEdgeIndexByPoint(PointIndexList[k], PointIndexList[0]);
		}
		if (EdgeIndexList[k].GetIndex() < 0)
		{
			//MDK_Warning("PointIndexList is invalid @ PolygonMesh::GetFaceIndexByPoint(...)")
			return -1;
		}
	}
	return this->GetFaceIndexByEdge(EdgeIndexList);
}


template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetFaceIndexByEdge(const DenseVector<int_max>& EdgeIndexList) const
{
    for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
    {
        if (this->IsValidEdgeIndex(EdgeIndexList[k]) == false)
        {
            //MDK_Warning("EdgeIndexList is invalid @ PolygonMesh::GetFaceIndexByEdge(...)")            
            return -1;
        }
    }

	//---------------------------------------------------------------------------------------------------
	auto FaceIndexList = m_MeshData->EdgeList[EdgeIndexList[0]].GetAdjacentFaceIndexList();
	for (int_max k = 1; k < EdgeIndexList.GetLength(); ++k)
	{
		auto AdjacentFaceIndexList_k = m_MeshData->EdgeList[EdgeIndexList[k]].GetAdjacentFaceIndexList();
		FaceIndexList = Intersect(FaceIndexList, AdjacentFaceIndexList_k);
	}
	//----------------------
	if (FaceIndexList.GetLength() != 1)
	{
		//MDK_Warning("EdgeIndexList is invalid @ PolygonMesh::GetFaceIndexByEdge(...)")
		return -1;
	}

	return FaceIndexList[0];
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetFaceIndexByID(int_max FaceID) const
{
	if (FaceID < 0)
	{
		//MDK_Warning("Invalid FaceID (< 0) @ PolygonMesh::GetFaceIndexByID(...)")
		return -1;
	}

	auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
	if (it != m_MeshData->Map_FaceID_to_FaceIndex.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid FaceID @ PolygonMesh::GetFaceIndexByID(...)")
		return -1;
	}
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetFaceIndexByName(const String& FaceName) const
{
	if (FaceName.IsEmpty() == true)
	{
		//MDK_Warning("FaceName is empty @ PolygonMesh::GetFaceIndexByName(...)")
		return -1;
	}

	auto it = m_MeshData->Map_FaceName_to_FaceIndex.find(FaceName);
	if (it != m_MeshData->Map_FaceName_to_FaceIndex.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid FaceName @ PolygonMesh::GetFaceIndexByName(...)")
		return -1;
	}
}

//-------------- check ID -------------------------------------------------------//

template<typename MeshAttributeType>
inline
bool PolygonMesh<MeshAttributeType>::IsValidPointID(int_max PointID) const
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
bool PolygonMesh<MeshAttributeType>::IsValidEdgeID(int_max EdgeID) const
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
bool PolygonMesh<MeshAttributeType>::IsValidDirectedEdgeID(int_max DirectedEdgeID) const
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
bool PolygonMesh<MeshAttributeType>::IsValidFaceID(int_max FaceID) const
{
    if (FaceID < 0)
    {
        return false;
    }

    auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(FaceID);
    return (it != m_MeshData->Map_FaceID_to_FaceIndex.end());
}

//--------- get Valid IDList ------------------------------------------------------------//

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetValidPointIDList() const
{
	DenseVector<int_max> OutputIDList;
	OutputIDList.SetCapacity(int_max(m_MeshData->Map_PointID_to_PointIndex.size()));
	for (auto it = m_MeshData->Map_PointID_to_PointIndex.begin(); it != m_MeshData->Map_PointID_to_PointIndex.end(); ++it)
	{
		OutputIDList.Append(it->first);
	}
	return OutputIDList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetValidEdgeIDList() const
{
	DenseVector<int_max> OutputIDList;
	OutputIDList.SetCapacity(int_max(m_MeshData->Map_EdgeID_to_EdgeIndex.size()));
	for (auto it = m_MeshData->Map_EdgeID_to_EdgeIndex.begin(); it != m_MeshData->Map_EdgeID_to_EdgeIndex.end(); ++it)
	{
		OutputIDList.Append(it->first);
	}
	return OutputIDList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetValidDirectedEdgeIDList() const
{
	DenseVector<int_max> OutputIDList;
	OutputIDList.SetCapacity(int_max(m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.size()));
	for (auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end(); ++it)
	{
		OutputIDList.Append(it->first);
	}
	return OutputIDList;
}

template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetValidFaceIDList() const
{
	DenseVector<int_max> OutputIDList;
	OutputIDList.SetCapacity(int_max(m_MeshData->Map_FaceID_to_FaceIndex.size()));
	for (auto it = m_MeshData->Map_FaceID_to_FaceIndex.begin(); it != m_MeshData->Map_FaceID_to_FaceIndex.end(); ++it)
	{
		OutputIDList.Append(it->first);
	}
	return OutputIDList;
}


//----------- get Point/Edge/DirectedEdge/Face ID by Index -----------------------------------------------------------//

template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetPointID(int_max PointIndex) const
{
    if (this->IsValidPointIndex(PointIndex) == true)
    {
        return m_MeshData->PointList[PointIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetPointID(const DenseVector<int_max>& PointIndexList) const
{
	DenseVector<int_max> PointIDList;
	PointIDList.Resize(PointIndexList.GetLength());
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		PointIDList[k] = this->GetPointID(PointIndexList[k]);
	}
	return PointIDList;
}


template<typename MeshAttributeType>
inline
int_max PolygonMesh<MeshAttributeType>::GetEdgeID(int_max EdgeIndex) const
{
    if (this->IsValidEdgeIndex(EdgeIndex) == true)
    {
        return m_MeshData->EdgeList[EdgeIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetEdgeID(const DenseVector<int_max>& EdgeIndexList) const
{
	DenseVector<int_max> EdgeIDList;
	EdgeIDList.Resize(EdgeIndexList.GetLength());
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		EdgeIDList[k] = this->GetEdgeID(EdgeIndexList[k]);
	}
	return EdgeIDList;
}


template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetDirectedEdgeID(DirectedEdgeIndexType DirectedEdgeIndex) const
{
    if (this->IsValidDirectedEdgeIndex(DirectedEdgeIndex) == true)
    {
		return m_MeshData->FaceList[DirectedEdgeIndex.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex.RelativeIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetDirectedEdgeID(const DenseVector<DirectedEdgeIndexType>& DirectedEdgeIndexList) const
{
	DenseVector<int_max> DirectedEdgeIDList;
	DirectedEdgeIDList.Resize(DirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
	{
		DirectedEdgeIDList[k] = this->GetDirectedEdgeID(DirectedEdgeIndexList[k]);
	}
	return DirectedEdgeIDList;
}


template<typename MeshAttributeType>
inline 
int_max PolygonMesh<MeshAttributeType>::GetFaceID(int_max FaceIndex) const
{
    if (this->IsValidFaceIndex(FaceIndex) == true)
    {
        return m_MeshData->FaceList[FaceIndex].GetID();
    }
    else
    {
        return -1;
    }
}


template<typename MeshAttributeType>
inline
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetFaceID(const DenseVector<int_max>& FaceIndexList) const
{
	DenseVector<int_max> FaceIDList;
	FaceIDList.Resize(FaceIndexList.GetLength());
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		FaceIDList[k] = this->GetFaceID(FaceIndexList[k]);
	}
	return FaceIDList;
}

//-------------- check Name -------------------------------------------------------//

template<typename MeshAttributeType>
inline bool PolygonMesh<MeshAttributeType>::IsValidPointName(const String& PointName) const
{
	if (PointName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_PointName_to_PointIndex.find(PointName);
	return (it != m_MeshData->Map_PointName_to_PointIndex.end());
}

template<typename MeshAttributeType>
inline bool PolygonMesh<MeshAttributeType>::IsValidEdgeName(const String& EdgeName) const
{
	if (EdgeName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_EdgeName_to_EdgeIndex.find(EdgeName);
	return (it != m_MeshData->Map_EdgeName_to_EdgeIndex.end());
}

template<typename MeshAttributeType>
inline bool PolygonMesh<MeshAttributeType>::IsValidDirectedEdgeName(const String& DirectedEdgeName) const
{
	if (DirectedEdgeName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(DirectedEdgeName);
	return (it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end());
}

template<typename MeshAttributeType>
inline bool PolygonMesh<MeshAttributeType>::IsValidFaceName(const String& FaceName) const
{
	if (FaceName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_FaceName_to_FaceIndex.find(FaceName);
	return (it != m_MeshData->Map_FaceName_to_FaceIndex.end());
}

//--------- get Valid NameList ------------------------------------------------------------//

template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetValidPointNameList() const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_PointName_to_PointIndex.size());
	for (auto it = m_MeshData->Map_PointName_to_PointIndex.begin(); it != m_MeshData->Map_PointName_to_PointIndex.end(); ++it)
	{
		NameList.Append(it->first);
	}
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetValidEdgeNameList() const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_EdgeName_to_EdgeIndex.size());
	for (auto it = m_MeshData->Map_EdgeName_to_EdgeIndex.begin(); it != m_MeshData->Map_EdgeName_to_EdgeIndex.end(); ++it)
	{
		NameList.Append(it->first);
	}
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetValidDirectedEdgeNameList() const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.size());
	for (auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end(); ++it)
	{
		NameList.Append(it->first);
	}
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetValidFaceNameList() const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_FaceName_to_FaceIndex.size());
	for (auto it = m_MeshData->Map_FaceName_to_FaceIndex.begin(); it != m_MeshData->Map_FaceName_to_FaceIndex.end(); ++it)
	{
		NameList.Append(it->first);
	}
	return NameList;
}

//----------- get Point/Edge/DirectedEdge/Face Name by Index -----------------------------------------------------------//

template<typename MeshAttributeType>
inline String PolygonMesh<MeshAttributeType>::GetPointName(int_max PointIndex) const
{
	if (this->IsValidPointIndex(PointIndex) == true)
	{
		return m_MeshData->PointList[PointIndex].GetName();
	}
	else
	{
		String EmptyName;
		return EmptyName;
	}
}

template<typename MeshAttributeType>
inline ObjectArray<String> PolygonMesh<MeshAttributeType>::GetPointName(const DenseVector<int_max>& PointIndexList) const
{
	ObjectArray<String> NameList;
	NameList.Resize(PointIndexList.GetLength());
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetPointName(PointIndexList[k]);
	}
	return NameList;
}


template<typename MeshAttributeType>
inline String PolygonMesh<MeshAttributeType>::GetEdgeName(int_max EdgeIndex) const
{
	if (this->IsValidEdgeIndex(EdgeIndex) == true)
	{
		return m_MeshData->EdgeList[EdgeIndex].GetName();
	}
	else
	{
		String EmptyName;
		return EmptyName;
	}
}

template<typename MeshAttributeType>
inline ObjectArray<String> PolygonMesh<MeshAttributeType>::GetEdgeName(const DenseVector<int_max>& EdgeIndexList) const
{
	ObjectArray<String> NameList;
	NameList.Resize(EdgeIndexList.GetLength());
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetEdgeName(EdgeIndexList[k]);
	}
	return NameList;
}


template<typename MeshAttributeType>
inline String PolygonMesh<MeshAttributeType>::GetDirectedEdgeName(DirectedEdgeIndexType DirectedEdgeIndex) const
{
	if (this->IsValidDirectedEdgeIndex(DirectedEdgeIndex) == true)
	{
		return m_MeshData->FaceList[DirectedEdgeIndex.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex.RelativeIndex].GetName();
	}
	else
	{
		String EmptyName;
		return EmptyName;
	}
}

template<typename MeshAttributeType>
inline ObjectArray<String> PolygonMesh<MeshAttributeType>::GetDirectedEdgeName(const DenseVector<DirectedEdgeIndexType>& DirectedEdgeIndexList) const
{
	ObjectArray<String> NameList;
	NameList.Resize(DirectedEdgeIndexList.GetLength());
	for (int_max k = 0; k < DirectedEdgeIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetDirectedEdgeName(DirectedEdgeIndexList[k]);
	}
	return NameList;
}


template<typename MeshAttributeType>
inline String PolygonMesh<MeshAttributeType>::GetFaceName(int_max FaceIndex) const
{
	if (this->IsValidFaceIndex(FaceIndex) == true)
	{
		return m_MeshData->FaceList[FaceIndex].GetName();
	}
	else
	{
		String EmptyName;
		return EmptyName;
	}
}

template<typename MeshAttributeType>
inline ObjectArray<String> PolygonMesh<MeshAttributeType>::GetFaceName(const DenseVector<int_max>& FaceIndexList) const
{
	ObjectArray<String> NameList;
	NameList.Resize(FaceIndexList.GetLength());
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetFaceName(FaceIndexList[k]);
	}
	return NameList;
}

//------------ PointSet, EdgeSet, FaceSet ------------------------------------------//

template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetPointSetCount() const
{
	return m_MeshData->PointSetList.GetLength();
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::SetPointSet(const String& PointSetName, DenseVector<int_max> PointSet)
{
	auto it = m_MeshData->Map_PointSetName_to_PointSetIndex.find(PointSetName);
	if (it != m_MeshData->Map_PointSetName_to_PointSetIndex.end())
	{
		auto PointSetIndex = it->second;
		m_MeshData->PointSetList[PointSetIndex] = std::move(PointSet);
		return PointSetIndex;
	}
	else
	{
		m_MeshData->PointSetList.Append(std::move(PointSet));
		auto PointSetIndex = m_MeshData->PointSetList.GetLength() - 1;
		m_MeshData->Map_PointSetName_to_PointSetIndex[PointSetName] = PointSetIndex;
		return PointSetIndex;
	}
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetPointSetIndex(const String& PointSetName) const
{
	auto it = m_MeshData->Map_PointSetName_to_PointSetIndex.find(PointSetName);
	if (it != m_MeshData->Map_PointSetName_to_PointSetIndex.end())
	{
		return it->second;
	}
	else
	{
		MDK_Error("PointSet NOT exist @ PolygonMesh::GetPointSetIndex(...)")
		return -1;
	}
}


template<typename MeshAttributeType>
String PolygonMesh<MeshAttributeType>::GetPointSetName(int_max PointSetIndex) const
{
	for (auto it = m_MeshData->Map_PointSetName_to_PointSetIndex.begin(); it != m_MeshData->Map_PointSetName_to_PointSetIndex.end(); ++it)
	{
		if (it->second == PointSetIndex)
		{
			return it->first;
		}
	}

	MDK_Error("PointSet NOT exist @  PolygonMesh::GetPointSetName(...)")
	String EmptyName;
	return EmptyName;
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetPointSet(int_max PointSetIndex) const
{
	return m_MeshData->PointSetList[PointSetIndex];
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetPointSet(const String& PointSetName) const
{
	auto it = m_MeshData->Map_PointSetName_to_PointSetIndex.find(PointSetName);
	if (it != m_MeshData->Map_PointSetName_to_PointSetIndex.end())
	{
		return m_MeshData->PointSetList[it->second];
	}
	else
	{
		DenseVector<int_max> EmptySet;
		MDK_Error("Unknown PointSetName: " << PointSetName << " @ PolygonMesh::GetPointSet()")
		return EmptySet;
	}
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetPointSetName(MDK_Symbol_ALL&) const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_PointSetName_to_PointSetIndex.size());
	DenseVector<int_max> IndexList;
	IndexList.SetCapacity(m_MeshData->Map_PointSetName_to_PointSetIndex.size());
	for (auto it = m_MeshData->Map_PointSetName_to_PointSetIndex.begin(); it != m_MeshData->Map_PointSetName_to_PointSetIndex.end(); ++it)
	{
		NameList.Append(it->first);
		IndexList.Append(it->second);
	}
	auto IndexList_sort = IndexList.Sort("ascend");
	NameList = NameList.GetSubSet(IndexList_sort);
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> PolygonMesh<MeshAttributeType>::GetPointSet(MDK_Symbol_ALL&) const
{
	ObjectArray<DenseVector<int_max>> PointSetList;
	PointSetList = m_MeshData->PointSetList;
	return PointSetList;
}

template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetEdgeSetCount() const
{
	return m_MeshData->EdgeSetList.GetLength();
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::SetEdgeSet(const String& EdgeSetName, DenseVector<int_max> EdgeSet)
{
	auto it = m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.find(EdgeSetName);
	if (it != m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.end())
	{
		auto EdgeSetIndex = it->second;
		m_MeshData->EdgeSetList[EdgeSetIndex] = std::move(EdgeSet);
		return EdgeSetIndex;
	}
	else
	{
		m_MeshData->EdgeSetList.Append(std::move(EdgeSet));
		auto EdgeSetIndex = m_MeshData->EdgeSetList.GetLength() - 1;
		m_MeshData->Map_EdgeSetName_to_EdgeSetIndex[EdgeSetName] = EdgeSetIndex;
		return EdgeSetIndex;
	}
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetEdgeSetIndex(const String& EdgeSetName) const
{
	auto it = m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.find(EdgeSetName);
	if (it != m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.end())
	{
		return it->second;
	}
	else
	{
		MDK_Error("EdgeSet NOT exist @ PolygonMesh::GetEdgeSetIndex(...)")
		return -1;
	}
}


template<typename MeshAttributeType>
String PolygonMesh<MeshAttributeType>::GetEdgeSetName(int_max EdgeSetIndex) const
{
	for (auto it = m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.begin(); it != m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.end(); ++it)
	{
		if (it->second == EdgeSetIndex)
		{
			return it->first;
		}
	}

	MDK_Error("EdgeSet NOT exist @  PolygonMesh::GetEdgeSetName(...)")
	String EmptyName;
	return EmptyName;
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetEdgeSet(int_max EdgeSetIndex) const
{
	return m_MeshData->EdgeSetList[EdgeSetIndex];
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetEdgeSet(const String& EdgeSetName) const
{
	auto it = m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.find(EdgeSetName);
	if (it != m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.end())
	{
		return m_MeshData->EdgeSetList[it->second];
	}
	else
	{
		DenseVector<int_max> EmptySet;
		MDK_Error("Unknown EdgeSetName: " << EdgeSetName << " @ PolygonMesh::GetEdgeSet()")
		return EmptySet;
	}
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetEdgeSetName(MDK_Symbol_ALL&) const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.size());
	DenseVector<int_max> IndexList;
	IndexList.SetCapacity(m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.size());
	for (auto it = m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.begin(); it != m_MeshData->Map_EdgeSetName_to_EdgeSetIndex.end(); ++it)
	{
		NameList.Append(it->first);
		IndexList.Append(it->second);
	}
	auto IndexList_sort = IndexList.Sort("ascend");
	NameList = NameList.GetSubSet(IndexList_sort);
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> PolygonMesh<MeshAttributeType>::GetEdgeSet(MDK_Symbol_ALL&) const
{
	ObjectArray<DenseVector<int_max>> EdgeSetList;
	EdgeSetList = m_MeshData->EdgeSetList;
	return EdgeSetList;
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetFaceSetCount() const
{
	return m_MeshData->FaceSetList.GetLength();
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::SetFaceSet(const String& FaceSetName, DenseVector<int_max> FaceSet)
{
	auto it = m_MeshData->Map_FaceSetName_to_FaceSetIndex.find(FaceSetName);
	if (it != m_MeshData->Map_FaceSetName_to_FaceSetIndex.end())
	{
		auto FaceSetIndex = it->second;
		m_MeshData->FaceSetList[FaceSetIndex] = std::move(FaceSet);
		return FaceSetIndex;
	}
	else
	{
		m_MeshData->FaceSetList.Append(std::move(FaceSet));
		auto FaceSetIndex = m_MeshData->FaceSetList.GetLength() - 1;
		m_MeshData->Map_FaceSetName_to_FaceSetIndex[FaceSetName] = FaceSetIndex;
		return FaceSetIndex;
	}
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetFaceSetIndex(const String& FaceSetName) const
{
	auto it = m_MeshData->Map_FaceSetName_to_FaceSetIndex.find(FaceSetName);
	if (it != m_MeshData->Map_FaceSetName_to_FaceSetIndex.end())
	{
		return it->second;
	}
	else
	{
		MDK_Error("FaceSet NOT exist @ PolygonMesh::GetFaceSetIndex(...)")
		return -1;
	}
}


template<typename MeshAttributeType>
String PolygonMesh<MeshAttributeType>::GetFaceSetName(int_max FaceSetIndex) const
{
	for (auto it = m_MeshData->Map_FaceSetName_to_FaceSetIndex.begin(); it != m_MeshData->Map_FaceSetName_to_FaceSetIndex.end(); ++it)
	{
		if (it->second == FaceSetIndex)
		{
			return it->first;
		}
	}

	MDK_Error("FaceSet NOT exist @  PolygonMesh::GetFaceSetName(...)")
	String EmptyName;
	return EmptyName;
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetFaceSet(int_max FaceSetIndex) const
{
	return m_MeshData->FaceSetList[FaceSetIndex];
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::GetFaceSet(const String& FaceSetName) const
{
	auto it = m_MeshData->Map_FaceSetName_to_FaceSetIndex.find(FaceSetName);
	if (it != m_MeshData->Map_FaceSetName_to_FaceSetIndex.end())
	{
		return m_MeshData->FaceSetList[it->second];
	}
	else
	{
		DenseVector<int_max> EmptySet;
		MDK_Error("Unknown FaceSetName: " << FaceSetName << " @ PolygonMesh::GetFaceSet()")
		return EmptySet;
	}
}


template<typename MeshAttributeType>
ObjectArray<String> PolygonMesh<MeshAttributeType>::GetFaceSetName(MDK_Symbol_ALL&) const
{
	ObjectArray<String> NameList;
	NameList.SetCapacity(m_MeshData->Map_FaceSetName_to_FaceSetIndex.size());
	DenseVector<int_max> IndexList;
	IndexList.SetCapacity(m_MeshData->Map_FaceSetName_to_FaceSetIndex.size());
	for (auto it = m_MeshData->Map_FaceSetName_to_FaceSetIndex.begin(); it != m_MeshData->Map_FaceSetName_to_FaceSetIndex.end(); ++it)
	{
		NameList.Append(it->first);
		IndexList.Append(it->second);
	}
	auto IndexList_sort = IndexList.Sort("ascend");
	NameList = NameList.GetSubSet(IndexList_sort);
	return NameList;
}


template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> PolygonMesh<MeshAttributeType>::GetFaceSet(MDK_Symbol_ALL&) const
{
	ObjectArray<DenseVector<int_max>> FaceSetList;
	FaceSetList = m_MeshData->FaceSetList;
	return FaceSetList;
}

//------------- Iterator --------------------------------------------------------------//

template<typename MeshAttributeType>
inline
Iterator_Of_Point_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfPoint()
{
    PointIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Point_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfPoint() const
{
	PointIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Edge_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfEdge()
{
    EdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Edge_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfEdge() const
{
	EdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfDirectedEdge()
{
    DirectedEdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfDirectedEdge() const
{
	DirectedEdgeIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
Iterator_Of_Face_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfFace()
{
    FaceIteratorType it(*this);
    return it;
}

template<typename MeshAttributeType>
inline
const Iterator_Of_Face_Of_PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetIteratorOfFace() const
{
	FaceIteratorType it(*this);
    return it;
}

//------------ SetCapacity, ReleaseUnusedCapacity -------------------------------------//
template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount)
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
void PolygonMesh<MeshAttributeType>::ReleaseUnusedCapacity()
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
int_max PolygonMesh<MeshAttributeType>::AddPoint(const DenseVector<ScalarType, 3>& Position)
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddPoint(const DenseVector<ScalarType>& Position)
{
    if (PointSet.GetLength() != 3)
    {
        MDK_Error("Position is a vector but length != 3 @ PolygonMesh::AddPoint(...)")
        return -1;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddPoint(const DenseMatrix<ScalarType>& Position)
{
    if (Position.IsVector() == true)
    {
        if (Position.GetElementCount() != 3)
        {
            MDK_Error("Position is a vector but length != 3 @ PolygonMesh::AddPoint(...)")
            return -1;
        }
    }
    else
    {
        MDK_Error("Position is a not a vector @ PolygonMesh::AddPoint(...)")
        return -1;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddPoint(const ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddPoint(ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColCount() - 1;

    Point_Of_PolygonMesh<MeshAttributeType> Point;
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
   
    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

    return PointIndex;
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::AddPoint_batch(const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList;

    if (PointSet.IsVector() == true)
    {
        if (PointSet.GetElementCount() != 3)
        {
            MDK_Error("PointSet is a vector but length != 3 @ PolygonMesh::AddPoint_batch(...)")
            return PointIndexList;
        }

		PointIndexList.Resize(1);
		PointIndexList[0] = this->AddPoint(PointSet.GetPointer());
        return PointIndexList;
    }

    if (PointSet.GetRowCount() != 3)
    {
        MDK_Error("PointSet is a matrix but RowCount != 3 @ PolygonMesh::AddPoint_batch(...)")
        return PointIndexList;
    }

	PointIndexList.Resize(PointSet.GetColCount());
    for (int_max k = 0; k < PointSet.GetColCount(); ++k)
    {
		PointIndexList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointIndexList;
}


template<typename MeshAttributeType>
DenseVector<int_max> PolygonMesh<MeshAttributeType>::AddPoint_batch(DenseMatrix<ScalarType>&& PointSet)
{
	DenseVector<int_max> PointIndexList;

	if (PointSet.IsVector() == true)
	{
		if (PointSet.GetElementCount() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ PolygonMesh::AddPoint_batch(...)")
			return PointIndexList;
		}

		PointIndexList.Resize(1);
		PointIndexList[0] = this->AddPoint(PointSet.GetPointer());
		return PointIndexList;
	}

	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("PointSet is a matrix but RowCount != 3 @ PolygonMesh::AddPoint_batch(...)")
		return PointIndexList;
	}

	if (this->IsEmpty() == true)
	{
		m_MeshData->PointPositionTable = std::move(PointSet);
		PointIndexList.Resize(m_MeshData->PointPositionTable.GetColCount());
		for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColCount(); ++k)
		{
			Point_Of_PolygonMesh<MeshAttributeType> Point;
			Point.SetParentMesh(*this);
			Point.SetIndex(k);

			m_MeshData->PointList.Append(std::move(Point));
			m_MeshData->PointValidityFlagList.Append(1);

			PointIndexList[k]=k;
		}
	}
	else
	{
		PointIndexList.Resize(PointSet.GetColCount());
		for (int_max k = 0; k < PointSet.GetColCount(); ++k)
		{
			PointIndexList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
		}
	}
	return PointIndexList;
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddEdge(int_max PointIndex0, int_max PointIndex1)
{
	auto EdgeIndex = m_MeshData->EdgeList.GetLength();
	return this->AddEdge(PointIndex0, PointIndex1, EdgeIndex);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddEdge(int_max PointIndex0, int_max PointIndex1, int_max EdgeIndex_input)
{
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

    if (this->IsValidPointIndex(PointIndex0) == false || this->IsValidPointIndex(PointIndex1) == false)
    {
        MDK_Error("PointIndex0 or PointIndex1 is invalid @ PolygonMesh::AddEdge(...)")
        return -1;
    }

	if (EdgeIndex_input < 0 || EdgeIndex_input > m_MeshData->EdgeList.GetLength())
	{
		MDK_Error("EdgeIndex_input is out of range @ PolygonMesh::AddEdge(...)")
		return -1;
	}

    auto EdgeIndex_temp = this->GetEdgeIndexByPoint(PointIndex0, PointIndex1);
    if (this->IsValidEdgeIndex(EdgeIndex_temp) == true)
    {
        //MDK_Warning("The edge has been added already @ PolygonMesh::AddEdge(...)")
		if (EdgeIndex_input == m_MeshData->EdgeList.GetLength() || EdgeIndex_input == EdgeIndex_temp)
		{
			return EdgeIndex_temp;
		}
		else
		{
			MDK_Error("Edge exist at EdgeIndex_input @ PolygonMesh::AddEdge(...)")
			return -1;
		}
    }

	if (EdgeIndex_input < m_MeshData->EdgeValidityFlagList.GetLength())
	{
		if (m_MeshData->EdgeValidityFlagList[EdgeIndex_input] == 1)
		{
			MDK_Error("Edge exist at EdgeIndex_input @ PolygonMesh::AddEdge(...)")
			return -1;
		}
	}

	//Create Edge ----------------------------------------------------------------

	if (EdgeIndex_input == m_MeshData->EdgeList.GetLength())
	{
		EdgeType EmptyEdge;
		m_MeshData->EdgeList.Append(std::move(EmptyEdge));
		m_MeshData->EdgeValidityFlagList.Append(1);
	}
	else
	{
		m_MeshData->EdgeList[EdgeIndex_input].ReCreate();
		m_MeshData->EdgeValidityFlagList[EdgeIndex_input] = 1;
	}
    // take reference
	auto& Edge = m_MeshData->EdgeList[EdgeIndex_input];
    Edge.SetParentMesh(*this);
    Edge.SetIndex(EdgeIndex_input);
    Edge.SetPointIndexList(PointIndex0, PointIndex1);

    // update AdjacentPoint information in m_MeshData->PointList
    m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().Append(EdgeIndex_input);
    m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().Append(EdgeIndex_input);
    
    //------------
    return EdgeIndex_input;
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList)
{
	auto FaceIndex = m_MeshData->FaceList.GetLength();
	return this->AddFaceByEdge(EdgeIndexList, FaceIndex);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList, int_max FaceInex_input)
{
    // Input: the input EdgeIndexList must have been ordered
	// the first point of the first input edge may NOT be the first point of the face
    // the order determine the direction/sign of the normal vector
    // it also determine the direction of each DirectedEdge
    //
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

    if (EdgeIndexList.GetLength() < 2)
    {
        MDK_Error("length of EdgeIndexList < 2 @ PolygonMesh::AddFaceByEdge(...)")
        return -1;
    }

    for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
    {
        if (this->IsValidEdgeIndex(EdgeIndexList[k]) == false)
        {
            MDK_Error("Invalid EdgeIndexList @ PolygonMesh::AddFaceByEdge(...)")
            return -1;
        }
    }

	if (FaceInex_input < 0 && FaceInex_input > m_MeshData->FaceValidityFlagList.GetLength())
	{
		MDK_Error("FaceInex_input is out of range @ PolygonMesh::AddFaceByEdge(...)")
		return -1;
	}

    auto FaceIndex_temp = this->GetFaceIndexByEdge(EdgeIndexList);
    if (this->IsValidFaceIndex(FaceIndex_temp) == true)
    {
        //MDK_Warning("The face has been added already @ PolygonMesh::AddFaceByEdge(...)")

		if (FaceInex_input == m_MeshData->FaceValidityFlagList.GetLength() || FaceInex_input == FaceIndex_temp)
		{//attempt to add new face, not necessary			
			return FaceIndex_temp;
		}
		else
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceIndex: " << FaceIndex_temp << " @ PolygonMesh::AddFaceByEdge(...)")
			return -1;
		}
    }

	if (FaceInex_input < m_MeshData->FaceValidityFlagList.GetLength())
	{
		if (m_MeshData->FaceValidityFlagList[FaceInex_input] == 1)
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceInex_input @ PolygonMesh::AddFaceByEdge(...)")			
			return -1;
		}
	}

    // check EdgeIndexList
    auto tempList_unique = EdgeIndexList.FindUnique();
    if (tempList_unique.GetLength() != EdgeIndexList.GetLength())
    {
        MDK_Error("EdgeIndexList is invalid : more than one copy of an index @ PolygonMesh::AddFaceByEdge(...)")
        return -1;
    }

	//-------------------- input check is done ---------------------------------------------------//

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
			MDK_Error("EdgeIndexList is not ordered or invalid @ PolygonMesh::AddFaceByEdge(...)")			
            return -1;
        }
    }

    // create face --------------------------------------------------------------------------------------------------

	if (FaceInex_input == m_MeshData->FaceList.GetLength())
	{
		FaceType EmptyFace;
		m_MeshData->FaceList.Append(std::move(EmptyFace));
		m_MeshData->FaceValidityFlagList.Append(1);
	}
	else
	{
		m_MeshData->FaceList[FaceInex_input].ReCreate();
		m_MeshData->FaceValidityFlagList[FaceInex_input] = 1;
	}
	// take reference
	auto& Face = m_MeshData->FaceList[FaceInex_input];
	Face.SetParentMesh(*this);
	Face.SetIndex(FaceInex_input);

	// create DirectedEdge in Face	
	Face.DirectedEdgeList().Resize(EdgeIndexList.GetLength());
	for (int_max k = 0; k < Face.DirectedEdgeList().GetLength(); ++k)
	{
		// attention: auto& will get reference, auto will copy
		auto& DirectedEdge_k = Face.DirectedEdgeList()[k];		
		DirectedEdge_k.SetParentMesh(*this);
		DirectedEdge_k.SetIndex(FaceInex_input, k);
		DirectedEdge_k.SetEdgeIndex(EdgeIndexList[k]);
		DirectedEdge_k.SetStartPointIndex(PointIndexList[k]);

		if (k < Face.DirectedEdgeList().GetLength() - 1)
		{
			DirectedEdge_k.SetEndPointIndex(PointIndexList[k + 1]);			
		}
		else // if (k == Face.DirectedEdgeList().GetLength() - 1)
		{
			DirectedEdge_k.SetEndPointIndex(PointIndexList[0]);
		}
	}

	// update information in m_MeshData->EdgeList ------------------------------------------------------------------------
	for (int_max k = 0; k < Face.DirectedEdgeList().GetLength(); ++k)
	{
		const auto& DirectedEdge_k = Face.DirectedEdgeList()[k];
		auto EdgeIndex_k = DirectedEdge_k.GetEdgeIndex();
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().Append(DirectedEdge_k.GetIndex());
	}

    //-----------------------------
    return FaceInex_input;
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<int_max>& PointIndexList)
{
	auto FaceIndex = m_MeshData->FaceList.GetLength();
	return this->AddFaceByPoint(PointIndexList, FaceIndex);
}

template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::AddFaceByPoint(const DenseVector<int_max>& PointIndexList, int_max FaceIndex_input)
{
    if (PointIndexList.GetLength() < 3)
    {        
        MDK_Error("PointIndexList length < 3 @ PolygonMesh::AddFaceByPoint(...)")
        return -1;
    }

	DenseVector<int_max> EdgeIndexList;
	EdgeIndexList.Resize(PointIndexList.GetLength());
    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
        if (k < PointIndexList.GetLength() - 1)
        {
			EdgeIndexList[k] = this->AddEdge(PointIndexList[k], PointIndexList[k + 1]);
        }
        else
        {
			EdgeIndexList[k] = this->AddEdge(PointIndexList[k], PointIndexList[0]);
        }
    }

	return this->AddFaceByEdge(EdgeIndexList, FaceIndex_input);
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::DeleteFace(int_max FaceIndex)
{
    // this function will delete each DirectedEdge of the Face, and modify any information related to the face
    // FaceIndex and FaceID of the face become invalid after the face is deleted

    // check input 
    if (this->IsValidFaceIndex(FaceIndex) == false)
    {
        //MDK_Warning("Invalid FaceIndex @ PolygonMesh::DeleteFace(...)")
        return;
    }

    this->UpdateRecord_DeleteFace(FaceIndex);
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::DeleteEdge(int_max EdgeIndex)
{    
    if (this->IsValidEdgeIndex(EdgeIndex) == false)
    {
        //MDK_Warning("Invalid EdgeIndex @ PolygonMesh::DeleteEdge(...)")
        return;
    }
    
    auto AdjacentFaceCount = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceCount();
    if (AdjacentFaceCount > 0)
    {
        MDK_Error("AdjacentFaceIndexList is not empty, so this edge can not be deleted @ PolygonMesh::DeleteEdge(...)")
        return;
    }

    this->UpdateRecord_DeleteEdge(EdgeIndex);
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::DeletePoint(int_max PointIndex)
{
    if (this->IsValidPointIndex(PointIndex) == false)
    {
        //MDK_Warning("Invalid PointIndex @ PolygonMesh::DeletePoint(...)")
        return;
    }

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ PolygonMesh::DeletePoint(...)")
        return;
    }

    this->UpdateRecord_DeletePoint(PointIndex);
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::DeletePoint(const DenseVector<int_max>& PointIndexList)
{
    if (PointIndexList.IsEmpty() == true)
    {
        //MDK_Warning("PointIndexList is empty @ PolygonMesh::DeletePoint(...)")
        return;
    }

    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
		this->DeletePoint(PointIndexList[k]);
    }
}

//----------------- remove deleted item from Point/Edge/DirectedEdgeList/Face list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when DeletedPointIndexCount/ValidPointCount( GetPointCount() ) > 0.5

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::CleanDataStructure()
{
	DenseVector<int_max> PointIndexMap_Old_To_New;
	DenseVector<int_max> EdgeIndexMap_Old_To_New;
	DenseVector<int_max> FaceIndexMap_Old_To_New;
	this->CleanDataStructure(PointIndexMap_Old_To_New, EdgeIndexMap_Old_To_New, FaceIndexMap_Old_To_New);
}

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New,	DenseVector<int_max>& FaceIndexMap_Old_To_New)
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

	//--------------------------- update adjacency information------------------

	for (int_max FaceIndex_new = 0; FaceIndex_new < m_MeshData->FaceList.GetLength(); ++FaceIndex_new)
	{
		// take reference
		auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex_new].DirectedEdgeList();
		for (int_max n = 0; n < DirectedEdgeList.GetLength(); ++n)
		{
			//DirectedEdgeList[n].SetIndex(FaceIndex_new, n); Index has been updated in Face.SetIndex()
			DirectedEdgeList[n].SetEdgeIndex(EdgeIndexMap_Old_To_New[DirectedEdgeList[n].GetEdgeIndex()]);
			DirectedEdgeList[n].SetStartPointIndex(PointIndexMap_Old_To_New[DirectedEdgeList[n].GetStartPointIndex()]);
			DirectedEdgeList[n].SetEndPointIndex(PointIndexMap_Old_To_New[DirectedEdgeList[n].GetEndPointIndex()]);

			// update Map_DirectedEdgeID_to_DirectedEdgeIndex
			auto DirectedEdgeID_n = DirectedEdgeList[n].GetID();
			if (DirectedEdgeID_n >= 0) // ID is invalid if < 0 
			{
				m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[DirectedEdgeID_n] = DirectedEdgeList[n].GetIndex();
			}
		}
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

	for (int_max PointIndex_new = 0; PointIndex_new < m_MeshData->PointList.GetLength(); ++PointIndex_new)
	{
		const auto& AdjacentEdgeIndexList_old = m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList();
		DenseVector<int_max> AdjacentEdgeIndexList_new;
		for (int_max n = 0; n < AdjacentEdgeIndexList_old.GetLength(); ++n)
		{
			auto Index_new = EdgeIndexMap_Old_To_New[AdjacentEdgeIndexList_old[n]];
			AdjacentEdgeIndexList_new.Append(Index_new);
		}
		m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList() = std::move(AdjacentEdgeIndexList_new);
	}

	// update additional info
	for (auto it = m_MeshData->Map_PointName_to_PointIndex.begin(); it != m_MeshData->Map_PointName_to_PointIndex.end(); ++it)
	{
		auto Index_old = it->second;
		auto Index_new = PointIndexMap_Old_To_New[Index_old];
		it->second = Index_new;
	}

	for (auto it = m_MeshData->Map_EdgeName_to_EdgeIndex.begin(); it != m_MeshData->Map_EdgeName_to_EdgeIndex.end(); ++it)
	{
		auto Index_old = it->second;
		auto Index_new = EdgeIndexMap_Old_To_New[Index_old];
		it->second = Index_new;
	}

	for (auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.begin(); it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end(); ++it)
	{
		auto FaceIndex_old = it->second.FaceIndex;
		auto FaceIndex_new = FaceIndexMap_Old_To_New[FaceIndex_old];
		it->second.FaceIndex = FaceIndex_new;
	}

	for (auto it = m_MeshData->Map_FaceName_to_FaceIndex.begin(); it != m_MeshData->Map_FaceName_to_FaceIndex.end(); ++it)
	{
		auto Index_old = it->second;
		auto Index_new = FaceIndexMap_Old_To_New[Index_old];
		it->second = Index_new;
	}

	for (int_max k = 0; k < m_MeshData->PointSetList.GetLength(); ++k)
	{
		auto& PointSet_k = m_MeshData->PointSetList[k];
		for (int_max n = 0; n < PointSet_k.GetLength(); ++n)
		{
			auto PointIndex_old = PointSet_k[n];
			auto PointIndex_new = PointIndexMap_Old_To_New[PointIndex_old];
			PointSet_k[n] = PointIndex_new;
		}
	}

	for (int_max k = 0; k < m_MeshData->EdgeSetList.GetLength(); ++k)
	{
		auto& EdgeSet_k = m_MeshData->EdgeSetList[k];
		for (int_max n = 0; n < EdgeSet_k.GetLength(); ++n)
		{
			auto EdgeIndex_old = EdgeSet_k[n];
			auto EdgeIndex_new = EdgeIndexMap_Old_To_New[EdgeIndex_old];
			EdgeSet_k[n] = EdgeIndex_new;
		}
	}

	for (int_max k = 0; k < m_MeshData->FaceSetList.GetLength(); ++k)
	{
		auto& FaceSet_k = m_MeshData->FaceSetList[k];
		for (int_max n = 0; n < FaceSet_k.GetLength(); ++n)
		{
			auto FaceIndex_old = FaceSet_k[n];
			auto FaceIndex_new = FaceIndexMap_Old_To_New[FaceIndex_old];
			FaceSet_k[n] = FaceIndex_new;
		}
	}
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::Check_If_DataStructure_is_Clean() const
{
	auto Count1 = this->GetDeletedPointCount();
	auto Count2 = this->GetDeletedEdgeCount();
	auto Count3 = this->GetDeletedFaceCount();
	return (Count1 + Count2 + Count3 == 0);
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetDeletedPointCount() const
{
	auto ValidPointCount = this->GetPointCount();
	auto PointCountOfList = m_MeshData->PointList.GetLength();
	return  PointCountOfList - ValidPointCount;
}


template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetDeletedEdgeCount() const
{
	auto ValidEdgeCount = this->GetEdgeCount();
	auto EdgeCountOfList = m_MeshData->EdgeList.GetLength();
	return  EdgeCountOfList - ValidEdgeCount;
}

template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::GetDeletedFaceCount() const
{
	auto ValidFaceCount = this->GetFaceCount();
	auto FaceCountOfList = m_MeshData->FaceList.GetLength();
	return  FaceCountOfList - ValidFaceCount;
}

//------------ Construct from input data ------------------------------------//

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable)
{
	if (InputPointPositionMatrix.IsEmpty() == true || InputFaceTable.IsEmpty() == true)
	{
		MDK_Error("InputPointPositionMatrix or InputFaceTable is empty @ PolygonMesh::Construct(...)")
		return;
	}

	if (InputPointPositionMatrix.GetRowCount() != 3 || 3 * InputFaceTable.GetElementCount() < InputPointPositionMatrix.GetColCount())
	{
		MDK_Error("InputPointPositionMatrix or InputFaceTable is invalid @ PolygonMesh::Construct(...)")
		return;
	}
	//--------------------------------------------------------------------------------------------------
	this->Clear(); // attribute will get lost

	auto PointIndexList = this->AddPoint_batch(std::move(InputPointPositionMatrix));

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			MDK_Error("Somthing is wrong with PointIndexList @ PolygonMesh::Construct(...)")
			return;
		}
	}

	for (int_max k = 0; k < InputFaceTable.GetLength(); ++k)
	{
		auto PointIndexList_k = PointIndexList.GetSubSet(InputFaceTable[k]);
		this->AddFaceByPoint(PointIndexList_k);
	}
}


template<typename MeshAttributeType>
inline
std::pair<DenseMatrix<typename MeshAttributeType::ScalarType>, ObjectArray<DenseVector<int_max>>>
PolygonMesh<MeshAttributeType>::GetPointPositionMatrixAndFaceTable() const
{
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Output;
	this->GetPointPositionMatrixAndFaceTable(Output.first, Output.second);
	return Output;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPositionMatrixAndFaceTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable, ObjectArray<DenseVector<int_max>>& FaceTable) const
{
	auto PointCount = this->GetPointCount();
	auto FaceCount = this->GetFaceCount();

	PointPositionTable.FastResize(3, PointCount);
	FaceTable.FastResize(FaceCount);

	// Map PointIndex (PointIndex.GetIndex()) to OutputIndex (col index) in PointPositionTable
	std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex;

	int_max PointCounter = 0;

	for (auto it = this->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto PointIndex = it.GetPointIndex();
		it.Point().GetPosition(PointPositionTable.GetPointerOfCol(PointCounter));
		Map_PointIndex_to_OutputIndex[PointIndex] = PointCounter;
		PointCounter += 1;
	}

	int_max FaceCounter = 0;

	for (auto it = this->GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList = it.Face().GetPointIndexList();
		FaceTable[FaceCounter].FastResize(PointIndexList.GetLength());
		for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
		{
			auto tempPointIndex = PointIndexList[k];
			auto it_map = Map_PointIndex_to_OutputIndex.find(tempPointIndex);
			if (it_map != Map_PointIndex_to_OutputIndex.end())
			{
				FaceTable[FaceCounter][k] = it_map->second;
			}
			else
			{
				MDK_Error("tempPointIndex is invalid @ PolygonMesh::GetPointPositionTableAndFaceTable(...)")
				return;
			}
		}
		FaceCounter += 1;
	}
}

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
	if (this->IsEmpty() == true)
	{
		return false;
	}

	for (auto it = this->GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto FaceIndex = it.GetFaceIndex();
		auto PointCount = this->Face(FaceIndex).GetPointCount();
		if (PointCount != 3)
		{
			return false;
		}
	}

	return true;
}

//-------------------- get a sub mesh by FaceIndexList  -----------------------------------------//

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> 
PolygonMesh<MeshAttributeType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
    PolygonMesh<MeshAttributeType> OutputMesh;
    
    if (FaceIndexList.IsEmpty() == true)
    {
        return OutputMesh;
    }

    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ PolygonMesh::GetSubMeshByFace(...)")
        return OutputMesh;
    }

    for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
    {
        if (this->IsValidFaceIndex(FaceIndexList[k]) == false)
        {
            MDK_Error("FaceIndexList is invalid @ PolygonMesh::GetSubMeshByFace(...)")
            return OutputMesh;
        }
    }
    //-----------------------------------------------------------

    // point index on this mesh -> point index on output mesh
	DenseVector<int_max> Map_PointIndex_OuputIndex;
	Map_PointIndex_OuputIndex.Resize(m_MeshData->PointList.GetLength());
	Map_PointIndex_OuputIndex.Fill(-1);
    
	// edge index on this mesh -> edge index on output mesh	
	DenseVector<int_max> Map_EdgeIndex_OuputIndex;
	Map_EdgeIndex_OuputIndex.Resize(m_MeshData->EdgeList.GetLength());
	Map_EdgeIndex_OuputIndex.Fill(-1);

    // add face one by one
    for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
    {
        auto FaceID_k = this->Face(FaceIndexList[k]).GetID();
		auto FaceName_k= this->Face(FaceIndexList[k]).GetName();
        const auto& FaceAttribute_k = this->Face(FaceIndexList[k]).Attribute();
        auto PointIndexList_k = this->Face(FaceIndexList[k]).GetPointIndexList();
        auto EdgeIndexList_k = this->Face(FaceIndexList[k]).GetEdgeIndexList();
        auto DirectedEdgeIndexList_k = this->Face(FaceIndexList[k]).GetDirectedEdgeIndexList();

        // add point
        DenseVector<int_max> PointIndexList_OutputFace;
		PointIndexList_OutputFace.Resize(PointIndexList_k.GetLength());
        for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
        {
            auto PointIndex_n = PointIndexList_k[n];
            auto PointID_n = this->Point(PointIndex_n).GetID();
			auto PointName_n = this->Point(PointIndex_n).GetName();
            const auto& PointAttribute_n = this->Point(PointIndex_n).Attribute();
			auto Position_n = this->Point(PointIndex_n).GetPosition();

            // check if the point has already been added to OutputMesh			
			if (Map_PointIndex_OuputIndex[PointIndex_n] < 0)
            {        
                // add Point Position
				PointIndexList_OutputFace[n] = OutputMesh.AddPoint(Position_n);
                // copy PointID if it is valid
                if (PointID_n >= 0)
                {
					OutputMesh.Point(PointIndexList_OutputFace[n]).SetID(PointID_n);
                }
				// copy PointName if it is valid
				if (PointName_n.IsEmpty() == false)
				{
					OutputMesh.Point(PointIndexList_OutputFace[n]).SetName(PointName_n);
				}
                // copy Attribute
				OutputMesh.Point(PointIndexList_OutputFace[n]).Attribute() = PointAttribute_n;
                // add point to map
				Map_PointIndex_OuputIndex[PointIndex_n] = PointIndexList_OutputFace[n];
            }
            else
            {
				PointIndexList_OutputFace[n] = Map_PointIndex_OuputIndex[PointIndex_n];
            }
        }

        // add face
		auto FaceIndex_Output = OutputMesh.AddFaceByPoint(PointIndexList_OutputFace);
        // copy face ID if it is valid
        if (FaceID_k >= 0)
        {
			OutputMesh.Face(FaceIndex_Output).SetID(FaceID_k);
        }
		// copy face Name if it is valid
		if (FaceName_k.IsEmpty() == false)
		{
			OutputMesh.Face(FaceIndex_Output).SetName(FaceName_k);
		}
        // copy Attribute
		OutputMesh.Face(FaceIndex_Output).Attribute() = FaceAttribute_k;

        // copy Edge ID, Name and Attribute
		auto EdgeIndexList_Output = OutputMesh.Face(FaceIndex_Output).GetEdgeIndexList();
        for (int_max n = 0; n < EdgeIndexList_k.GetLength(); ++n)
        {
            // check if the edge has already been added to OutputMesh			
			if (Map_EdgeIndex_OuputIndex[EdgeIndexList_k[n].GetIndex()] < 0)
            {
                auto EdgeID_n = this->Edge(EdgeIndexList_k[n]).GetID();             
                if (EdgeID_n >= 0)
                {
					OutputMesh.Edge(EdgeIndexList_Output[n]).SetID(EdgeID_n);
                }

				auto EdgeName_n = this->Edge(EdgeIndexList_k[n]).GetName();
				if (EdgeName_n.IsEmpty() == false)
				{
					OutputMesh.Edge(EdgeIndexList_Output[n]).SetName(EdgeName_n);
				}

				const auto& EdgeAttribute_n = this->Edge(EdgeIndexList_k[n]).Attribute();
				OutputMesh.Edge(EdgeIndexList_Output[n]).Attribute() = EdgeAttribute_n;

				Map_EdgeIndex_OuputIndex[EdgeIndexList_k[n]] = EdgeIndexList_Output[n];
            }
        }

        // copy DirectedEdge ID, Name and Attribute
        auto DirectedEdgeIndexList_Output = OutputMesh.Face(FaceIndex_Output).GetDirectedEdgeIndexList();
        for (int_max n = 0; n < DirectedEdgeIndexList_k.GetLength(); ++n)
        {
            auto DirectedEdgeID_n = this->DirectedEdge(DirectedEdgeIndexList_k[n]).GetID();
            if (DirectedEdgeID_n >= 0)
            {
				OutputMesh.DirectedEdge(DirectedEdgeIndexList_Output[n]).SetID(DirectedEdgeID_n);
            }
			auto DirectedEdgeName_n = this->DirectedEdge(DirectedEdgeIndexList_k[n]).GetName();
			if (DirectedEdgeName_n.IsEmpty() == false)
			{
				OutputMesh.DirectedEdge(DirectedEdgeIndexList_Output[n]).SetName(DirectedEdgeName_n);
			}
			const auto& DirectedEdgeAttribute_n = this->DirectedEdge(DirectedEdgeIndexList_k[n]).Attribute();
			OutputMesh.DirectedEdge(DirectedEdgeIndexList_Output[n]).Attribute() = DirectedEdgeAttribute_n;
        }
    }

    return OutputMesh;
}


// other basic operation ----------------------------------------------------------------------------------------

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::ReversePointOrderOfFace(int_max FaceIndex)
{
	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ PolygonMesh::ReversePointOrderOfFace(...)")
		return;
	}
	
	auto&  DirectedEdgeList = m_MeshData->FaceList[FaceIndex].DirectedEdgeList();
	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)
	{
		auto PointIndex_start = DirectedEdgeList[k].GetStartPointIndex();
		auto PointIndex_end = DirectedEdgeList[k].GetEndPointIndex();
		DirectedEdgeList[k].SetStartPointIndex(PointIndex_end);
		DirectedEdgeList[k].SetEndPointIndex(PointIndex_start);
	}

	StdObjectVector<DirectedEdge_Of_PolygonMesh<MeshAttributeType>> DirectedEdgeList_new;
	DirectedEdgeList_new.Resize(DirectedEdgeList.GetLength());
	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)// k is RelativeIndex_old
	{
		int_max RelativeIndex_new = -1;
		if (k == DirectedEdgeList.GetLength() - 1)
		{
			RelativeIndex_new = k;
		}
		else
		{
			RelativeIndex_new = DirectedEdgeList.GetLength() - 2 - k;
		}

		//update adjacency info
		auto EdgeIndex_k = DirectedEdgeList[k].GetEdgeIndex();
		auto tempIndex = m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().ExactMatch("first", DirectedEdgeList[k].GetIndex());
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList()[tempIndex].RelativeIndex = RelativeIndex_new;
		
		DirectedEdgeList_new[RelativeIndex_new] = std::move(DirectedEdgeList[k]);
	}

	DirectedEdgeList = std::move(DirectedEdgeList_new);	
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::SwapPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SwapPoint(...)")
		return false;
	}

	//swap Position
	auto PositionA = m_MeshData->PointList[PointIndexA].GetPosition();
	auto PositionB = m_MeshData->PointList[PointIndexB].GetPosition();
	m_MeshData->PointList[PointIndexA].SetPosition(PositionB);
	m_MeshData->PointList[PointIndexB].SetPosition(PositionA);

	return this->SwapConnectivityOfPoint(PointIndexA, PointIndexB);
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::SwapConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SwapConnectivityOfPoint(...)")
		return false;
	}

	const auto& AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();//to be updated
	auto OutgoingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].GetOutgoingDirectedEdgeIndexList();
	auto IncomingDirectedEdgeIndexListA = m_MeshData->PointList[PointIndexA].GetIncomingDirectedEdgeIndexList();

	const auto& AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
	auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();//to be updated
	auto OutgoingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].GetOutgoingDirectedEdgeIndexList();
	auto IncomingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].GetIncomingDirectedEdgeIndexList();

	//---------------- update adjacent point and edge ----------------------------------------------------

	auto tempPointIndexA = m_MeshData->PointList.GetLength();

	//change PointIndexA -> tempPointIndexA
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
	for (int_max k = 0; k < AdjacentEdgeIndexListB.GetLength(); ++k)
	{
		if (AdjacentPointIndexListB[k] != tempPointIndexA)
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
	}

	//change tempPointIndexA -> PointIndexB
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

	//change PointIndexA -> PointIndexB for DirectedEdge	
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto FaceIndex_k = OutgoingDirectedEdgeIndexListA[k].FaceIndex;
		auto RelativeIndex_k = OutgoingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList()[RelativeIndex_k].SetStartPointIndex(PointIndexB);
	}
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListA.GetLength(); ++k)
	{
		auto FaceIndex_k = IncomingDirectedEdgeIndexListA[k].FaceIndex;
		auto RelativeIndex_k = IncomingDirectedEdgeIndexListA[k].RelativeIndex;
		m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList()[RelativeIndex_k].SetEndPointIndex(PointIndexB);
	}

	//change PointIndexB -> PointIndexA for DirectedEdge
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto FaceIndex_k = OutgoingDirectedEdgeIndexListB[k].FaceIndex;
		auto RelativeIndex_k = OutgoingDirectedEdgeIndexListB[k].RelativeIndex;
		m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList()[RelativeIndex_k].SetStartPointIndex(PointIndexA);
	}
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto FaceIndex_k = IncomingDirectedEdgeIndexListB[k].FaceIndex;
		auto RelativeIndex_k = IncomingDirectedEdgeIndexListB[k].RelativeIndex;
		m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList()[RelativeIndex_k].SetEndPointIndex(PointIndexA);
	}

	//swap AdjacentEdgeIndexList
	{
		auto tempList = AdjacentEdgeIndexListA;
		AdjacentEdgeIndexListA = AdjacentEdgeIndexListB;
		AdjacentEdgeIndexListB = tempList;
	}

	//----------
	return true;
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::MergeConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB)
{// merge connection of B to connection of A, B become isolated
	//-------------------------
	//   \         /
	// ---A-------B---
	//   /         \
	//-------------------------
	//   \ /    
	// ---A---   (B)
	//   / \     
    //-------------------------

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::MergePoint(...)")		
		return false;
	}

	//------- delete Edge AB if it exist --------------------------------------------------------------------

	auto EdgeIndex_AB = this->GetEdgeIndexByPoint(PointIndexA, PointIndexB);
	if (EdgeIndex_AB >= 0)
	{// remove DirectedEdge on EdgeIndex_AB
		auto AdjacentFaceIndexList_AB = m_MeshData->EdgeList[EdgeIndex_AB].GetAdjacentFaceIndexList();
		for (int_max k = 0; k < AdjacentFaceIndexList_AB.GetLength(); ++k)
		{
			auto FaceIndex_k = AdjacentFaceIndexList_AB[k];
			// take reference			
			auto& DirectedEdgeList_k = m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList();
			if (DirectedEdgeList_k.GetLength() > 3)
			{
				int_max RelativeIndex_delete = -1;
				for (int_max n = 0; n < DirectedEdgeList_k.GetLength(); ++n)
				{
					if (DirectedEdgeList_k[n].GetEdgeIndex() == EdgeIndex_AB)
					{
						RelativeIndex_delete = n;
						break;
					}
				}
				auto DirectedEdgeIndex_delete = DirectedEdgeList_k[RelativeIndex_delete].GetIndex();
				this->UpdateRecord_DeleteDirectedEdge(DirectedEdgeIndex_delete);				
			}
			else
			{
				MDK_Error("can NOT merge two point of a triangle face @ PolygonMesh::MergePoint(...)")
				return false;
			}		
		}
	}

	// delete edge AB if it exist
	if (EdgeIndex_AB >= 0)
	{
		this->UpdateRecord_DeleteEdge(EdgeIndex_AB);
	}

	//-------------------------- Merge connection of B to connection of A ------------------------------------

	const auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();
	auto OutgoingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].GetOutgoingDirectedEdgeIndexList();
	auto IncomingDirectedEdgeIndexListB = m_MeshData->PointList[PointIndexB].GetIncomingDirectedEdgeIndexList();

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
	//update OutgoingDirectedEdge of PointB
	for (int_max k = 0; k < OutgoingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto DirectedEdgeIndex_k = OutgoingDirectedEdgeIndexListB[k];
		auto& DirectedEdge_k = m_MeshData->FaceList[DirectedEdgeIndex_k.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
		DirectedEdge_k.SetStartPointIndex(PointIndexA);
	}
	//update IncomingDirectedEdge of PointB
	for (int_max k = 0; k < IncomingDirectedEdgeIndexListB.GetLength(); ++k)
	{
		auto DirectedEdgeIndex_k = IncomingDirectedEdgeIndexListB[k];
		auto& DirectedEdge_k = m_MeshData->FaceList[DirectedEdgeIndex_k.FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_k.RelativeIndex];
		DirectedEdge_k.SetEndPointIndex(PointIndexA);
	}

	//update adjacency info of A
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();//to be updated
	AdjacentEdgeIndexListA = { &AdjacentEdgeIndexListA, &AdjacentEdgeIndexListB };
	auto tempIndexList_edge_delete = AdjacentEdgeIndexListA.ExactMatch(EdgeIndex_AB);
	AdjacentEdgeIndexListA.Delete(tempIndexList_edge_delete);

	//delete any adjacency info of B
	m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList().Clear();

	//---------------------------------------------------------------	
	return true;
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::ShrinkEdgeToPoint(int_max EdgeIndex, int_max PointIndex)
{
	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ PolygonMesh::ShrinkEdgeToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::ShrinkEdgeToPoint(...)")		
		return false;
	}

	auto PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();
	if (PointIndexList[0] == PointIndex)
	{
		this->MergeConnectivityOfPoint(PointIndexList[0], PointIndexList[1]);
	}
	else if (PointIndexList[1] == PointIndex)
	{
		this->MergeConnectivityOfPoint(PointIndexList[1], PointIndexList[0]);
	}
	else
	{
		this->MergeConnectivityOfPoint(PointIndexList[0], PointIndexList[1]);
		this->SwapConnectivityOfPoint(PointIndexList[0], PointIndex);
	}
	//-------------------
	return true;
}


template<typename MeshAttributeType>
DenseVector<int_max, 2> PolygonMesh<MeshAttributeType>::SplitEdgeByPoint(int_max EdgeIndex, int_max PointIndex)
{
	DenseVector<int_max, 2> EdgeIndexPair;
	EdgeIndexPair[0] = -1;
	EdgeIndexPair[1] = -1;

	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	auto PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();
	auto PointIndex0 = PointIndexList[0];
	auto PointIndex1 = PointIndexList[1];
	auto PointIndex2 = PointIndex;
	//-------------------------
	//   \          /
    // ---P0---P2--P1---
	//   /          \
    //-------------------------

	if (PointIndex2 == PointIndex0 || PointIndex2 == PointIndex1)
	{
		MDK_Error("Input Point is on Edge  @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	//------------------------- input check is done --------------------------------------------//

	auto EdgeIndex01 = EdgeIndex;
	auto EdgeIndex02 = this->AddEdge(PointIndex0, PointIndex2);
	auto EdgeIndex21 = this->AddEdge(PointIndex2, PointIndex1);

	auto DirectedEdgeIndexList01 = this->GetDirectedEdgeIndexListByPoint(PointIndex0, PointIndex1);
	for (int_max k = 0; k < DirectedEdgeIndexList01.GetLength(); ++k)
	{
		auto DirectedEdgeIndex01_k = DirectedEdgeIndexList01[k];
		auto FaceIndex_k = DirectedEdgeIndex01_k.FaceIndex;
		auto& DirectedEdgeList_k = m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList();			
		
		// delete directed-edge between P0 and P1
		auto DirectedEdgeIndex_delete = DirectedEdgeList_k[DirectedEdgeIndex01_k.RelativeIndex].GetIndex();
		this->UpdateRecord_DeleteDirectedEdge(DirectedEdgeIndex_delete);

		//create directed-edge P0P2, P2P1 for FaceIndex_k
		DirectedEdgeType DirectedEdge02, DirectedEdge21;
		DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex02, DirectedEdgeIndex21;
		DirectedEdgeIndex02.FaceIndex = FaceIndex_k;
		DirectedEdgeIndex02.RelativeIndex = DirectedEdgeIndex01_k.RelativeIndex;
		DirectedEdgeIndex21.FaceIndex = FaceIndex_k;
		DirectedEdgeIndex21.RelativeIndex = DirectedEdgeIndex01_k.RelativeIndex+1;
		//
		DirectedEdge02.SetParentMesh(*this);
		DirectedEdge02.SetIndex(DirectedEdgeIndex02);
		DirectedEdge02.SetEdgeIndex(EdgeIndex02);
		DirectedEdge02.SetStartPointIndex(PointIndex0);
		DirectedEdge02.SetEndPointIndex(PointIndex2);	
		//
		DirectedEdge21.SetParentMesh(*this);
		DirectedEdge21.SetIndex(DirectedEdgeIndex21);
		DirectedEdge21.SetEdgeIndex(EdgeIndex21);
		DirectedEdge21.SetStartPointIndex(PointIndex2);
		DirectedEdge21.SetEndPointIndex(PointIndex1);
		//
		this->UpdateRecord_InsertDirectedEdge(std::move(DirectedEdge02));
		this->UpdateRecord_InsertDirectedEdge(std::move(DirectedEdge21));
	}

	auto DirectedEdgeIndexList_P1P0 = this->GetDirectedEdgeIndexListByPoint(PointIndex1, PointIndex0);
	for (int_max k = 0; k < DirectedEdgeIndexList_P1P0.GetLength(); ++k)
	{
		auto DirectedEdgeIndex10_k = DirectedEdgeIndexList01[k];
		auto FaceIndex_k = DirectedEdgeIndex10_k.FaceIndex;
		auto& DirectedEdgeList_k = m_MeshData->FaceList[FaceIndex_k].DirectedEdgeList();

		// delete directed-edge between P1 and P0
		auto DirectedEdgeIndex_delete = DirectedEdgeList_k[DirectedEdgeIndex10_k.RelativeIndex].GetIndex();
		this->UpdateRecord_DeleteDirectedEdge(DirectedEdgeIndex_delete);

		//create directed edge P1P2, P2P0
		DirectedEdgeType DirectedEdge12, DirectedEdge20;
		DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex12, DirectedEdgeIndex20;
		DirectedEdgeIndex12.FaceIndex = FaceIndex_k;
		DirectedEdgeIndex12.RelativeIndex = DirectedEdgeIndex10_k.RelativeIndex;
		DirectedEdgeIndex20.FaceIndex = FaceIndex_k;
		DirectedEdgeIndex20.RelativeIndex = DirectedEdgeIndex10_k.RelativeIndex+1;
		//
		DirectedEdge12.SetParentMesh(*this);
		DirectedEdge12.SetIndex(DirectedEdgeIndex12);
		DirectedEdge12.SetEdgeIndex(EdgeIndex21);
		DirectedEdge12.SetStartPointIndex(PointIndex1);
		DirectedEdge12.SetEndPointIndex(PointIndex2);
		//
		DirectedEdge20.SetParentMesh(*this);
		DirectedEdge20.SetIndex(DirectedEdgeIndex20);
		DirectedEdge20.SetEdgeIndex(EdgeIndex02);
		DirectedEdge20.SetStartPointIndex(PointIndex2);
		DirectedEdge20.SetEndPointIndex(PointIndex0);
		//
		this->UpdateRecord_InsertDirectedEdge(std::move(DirectedEdge12));
		this->UpdateRecord_InsertDirectedEdge(std::move(DirectedEdge20));
	}

	// delete edge P0P1
	this->UpdateRecord_DeleteEdge(EdgeIndex01);

	//-------------------------------------------------------------------------
	EdgeIndexPair[0] = EdgeIndex02;
	EdgeIndexPair[1] = EdgeIndex21;
	return EdgeIndexPair;
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::ShrinkFaceToPoint(int_max FaceIndex, int_max PointIndex)
{
	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ PolygonMesh::ShrinkFaceToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::ShrinkFaceToPoint(...)")
		return false;
	}

	auto PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();
	
	this->DeleteFace(FaceIndex);

	auto tempIndex = PointIndexList.ExactMatch("first", PointIndex);
	if (tempIndex >= 0)
	{
		for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
		{
			if (k != tempIndex)
			{
				this->MergeConnectivityOfPoint(PointIndexList[tempIndex], PointIndexList[k]);
			}
		}
	}
	else
	{
		for (int_max k = 1; k < PointIndexList.GetLength(); ++k)
		{
			this->MergeConnectivityOfPoint(PointIndexList[0], PointIndexList[k]);
		}

		this->SwapConnectivityOfPoint(PointIndexList[0], PointIndex);
	}
	//--------------------------------
	return true;
}


/*
template<typename MeshAttributeType>
int_max PolygonMesh<MeshAttributeType>::MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB)
{

}
*/
/*
template<typename MeshAttributeType>
DenseVector<int_max, 2> PolygonMesh<MeshAttributeType>::SplitFaceByEdge(int_max EdgeIndex)
{
}
*/

//----------------------------------- protected function ---------------------------------------------------------//

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateRecord_DeletePoint(int_max PointIndex)
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

	auto Name = m_MeshData->PointList[PointIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_PointName_to_PointIndex.find(Name);
		if (it != m_MeshData->Map_PointName_to_PointIndex.end())
		{
			m_MeshData->Map_PointName_to_PointIndex.erase(it);
		}
	}

    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

	m_MeshData->PointList[PointIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateRecord_DeleteEdge(int_max EdgeIndex)
{
	// update AdjacentEdge information in m_MeshData->PointList
	int_max PointIndex0, PointIndex1;
	m_MeshData->EdgeList[EdgeIndex].GetPointIndexList(PointIndex0, PointIndex1);
    //
	auto tempIndex0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().ExactMatch(EdgeIndex);
	m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().Delete(tempIndex0);
	//
	auto tempIndex1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().ExactMatch(EdgeIndex);
	m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().Delete(tempIndex1);

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

	auto Name = m_MeshData->EdgeList[EdgeIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_EdgeName_to_EdgeIndex.find(Name);
		if (it != m_MeshData->Map_EdgeName_to_EdgeIndex.end())
		{
			m_MeshData->Map_EdgeName_to_EdgeIndex.erase(it);
		}
	}

    // Delete Edge: only release memory, not remove from EdgeList
	m_MeshData->EdgeList[EdgeIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateRecord_DeleteFace(int_max FaceIndex)
{
	// auto& will get reference
	const auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex].DirectedEdgeList();
	const auto& PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();

	// update information in m_MeshData.EdgeList ------------------------------------------------------------------
	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)
	{
		const auto& DirectedEdge_k = DirectedEdgeList[k];
		auto EdgeIndex_k = DirectedEdge_k.GetEdgeIndex();
		auto tempIndex = m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().ExactMatch(DirectedEdge_k.GetIndex());
		m_MeshData->EdgeList[EdgeIndex_k].DirectedEdgeIndexList().Delete(tempIndex);
	}

	// delete ID, Name if the map has it 
	for (int_max k = 0; k < DirectedEdgeList.GetLength(); ++k)
	{
		// auto& take reference
		const auto& DirectedEdge_k = DirectedEdgeList[k];
		auto ID = DirectedEdge_k.GetID();
		if (ID >= 0)
		{
			auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(ID);
			if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
			{
				m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
			}
		}	
		auto Name = DirectedEdge_k.GetName();
		if (Name.IsEmpty() == false)
		{
			auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(Name);
			if (it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
			{
				m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.erase(it);
			}
		}
	}

    // delete ID, Name if the map has it 
	auto ID = m_MeshData->FaceList[FaceIndex].GetID();
	if (ID >= 0)
	{
		auto it = m_MeshData->Map_FaceID_to_FaceIndex.find(ID);
		if (it != m_MeshData->Map_FaceID_to_FaceIndex.end())
		{
			m_MeshData->Map_FaceID_to_FaceIndex.erase(it);
		}
	}
	auto Name = m_MeshData->FaceList[FaceIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_FaceName_to_FaceIndex.find(Name);
		if (it != m_MeshData->Map_FaceName_to_FaceIndex.end())
		{
			m_MeshData->Map_FaceName_to_FaceIndex.erase(it);
		}
	}

    // Delete Face : only clear memory, not change the length of FaceList
	m_MeshData->FaceList[FaceIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->FaceValidityFlagList[FaceIndex] = 0;
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateRecord_DeleteDirectedEdge(DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex_delete)
{
	auto FaceIndex = DirectedEdgeIndex_delete.FaceIndex;

	if (m_MeshData->FaceValidityFlagList[FaceIndex] == 0)
	{
		return;
	}

	auto EdgeIndex = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_delete.RelativeIndex].GetEdgeIndex();
	auto PointIndex_start = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_delete.RelativeIndex].GetStartPointIndex();
	auto PointIndex_end = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_delete.RelativeIndex].GetEndPointIndex();
	// erase ID, Name
	auto ID = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_delete.RelativeIndex].GetID();
	if (ID >= 0)
	{
		auto it = m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.find(ID);
		if (it != m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.end())
		{
			m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex.erase(it);
		}
	}
	auto Name = m_MeshData->FaceList[FaceIndex].DirectedEdgeList()[DirectedEdgeIndex_delete.RelativeIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.find(Name);
		if (it != m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.end())
		{
			m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex.erase(it);
		}
	}

	//delete the directed-edge
	m_MeshData->FaceList[FaceIndex].DirectedEdgeList().Delete(DirectedEdgeIndex_delete.RelativeIndex);

	// update adjacency information ----------------------------------------------------

	if (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
	{
		auto tempIndex_e = m_MeshData->EdgeList[EdgeIndex].DirectedEdgeIndexList().ExactMatch(DirectedEdgeIndex_delete);
		m_MeshData->EdgeList[EdgeIndex].DirectedEdgeIndexList().Delete(tempIndex_e);
	}
	
	auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex].DirectedEdgeList();
	for (int_max n = 0; n < DirectedEdgeList.GetLength(); ++n)
	{
		auto DirectedEdgeIndex_old = DirectedEdgeList[n].GetIndex();
		if (DirectedEdgeIndex_old.RelativeIndex != n)
		{
			auto EdgeIndex_n = DirectedEdgeList[n].GetEdgeIndex();
			auto tempIndex_e = m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeIndexList().ExactMatch("first", DirectedEdgeIndex_old);
			m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeIndexList()[tempIndex_e].RelativeIndex = n;

			DirectedEdgeList[n].SetIndex(FaceIndex, n);
		}
	}
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateRecord_InsertDirectedEdge(DirectedEdgeType DirectedEdge_insert)
{
	auto DirectedEdgeIndex_insert = DirectedEdge_insert.GetIndex();
	auto FaceIndex = DirectedEdgeIndex_insert.FaceIndex;
	auto EdgeIndex = DirectedEdge_insert.GetEdgeIndex();
	auto PointIndex_start = DirectedEdge_insert.GetStartPointIndex();
	auto PointIndex_end = DirectedEdge_insert.GetEndPointIndex();
	
	// insert ID, Name 
	auto ID = DirectedEdge_insert.GetID();
	if (ID >= 0)
	{
		m_MeshData->Map_DirectedEdgeID_to_DirectedEdgeIndex[ID] = DirectedEdgeIndex_insert;
	}
	auto Name = DirectedEdge_insert.GetName();
	if (Name.IsEmpty() == false)
	{
		m_MeshData->Map_DirectedEdgeName_to_DirectedEdgeIndex[Name] = DirectedEdgeIndex_insert;
	}

	// insert the directed-edge
	auto& DirectedEdgeList = m_MeshData->FaceList[FaceIndex].DirectedEdgeList();
	DirectedEdgeList.Insert(DirectedEdgeIndex_insert.RelativeIndex, std::move(DirectedEdge_insert));

	for (int_max n = 0; n < DirectedEdgeList.GetLength(); ++n)
	{
		auto DirectedEdgeIndex_old = DirectedEdgeList[n].GetIndex();
		if (DirectedEdgeIndex_old.RelativeIndex != n)
		{
			auto EdgeIndex_n = DirectedEdgeList[n].GetEdgeIndex();
			auto tempIndex_e = m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeIndexList().ExactMatch("first", DirectedEdgeIndex_old);
			m_MeshData->EdgeList[EdgeIndex_n].DirectedEdgeIndexList()[tempIndex_e].RelativeIndex = n;

			DirectedEdgeList[n].SetIndex(FaceIndex, n);
		}
	}

	m_MeshData->EdgeList[EdgeIndex].DirectedEdgeIndexList().Append(DirectedEdgeIndex_insert);
}

}// namespace mdk
