#pragma once

namespace mdk
{

template<typename ScalarType>
Mesh<ScalarType>::Mesh()
{
	this->Recreate();
}


template<typename ScalarType>
Mesh<ScalarType>::Mesh(const MDK_Symbol_PureEmpty&)
{
}


template<typename ScalarType>
Mesh<ScalarType>::Mesh(const Mesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
Mesh<ScalarType>::Mesh(Mesh<ScalarType>&& InputMesh)
{
	m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
Mesh<ScalarType>::~Mesh()
{
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->Copy(std::move(InputMesh));
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

	m_MeshData->Name = "";

    m_MeshData->PointPositionTable.Clear();
    m_MeshData->PointList.Clear();
    m_MeshData->PointValidityFlagList.Clear();

    m_MeshData->EdgeList.Clear();
    m_MeshData->EdgeValidityFlagList.Clear();

    m_MeshData->FaceList.Clear();
    m_MeshData->FaceValidityFlagList.Clear();
	
	m_MeshData->Map_Point_Name_to_Index.clear();
	m_MeshData->Map_Edge_Name_to_Index.clear();
	m_MeshData->Map_Face_Name_to_Index.clear();

	m_MeshData->PointSet.Clear();
	m_MeshData->PointSetName.Clear();
	m_MeshData->Map_PointSet_Name_to_Index.clear();

	m_MeshData->EdgeSet.Clear();
	m_MeshData->EdgeSetName.Clear();
	m_MeshData->Map_EdgeSet_Name_to_Index.clear();

	m_MeshData->FaceSet.Clear();
	m_MeshData->FaceSetName.Clear();
	m_MeshData->Map_FaceSet_Name_to_Index.clear();

	m_MeshData->PointDataSet.Clear();
	m_MeshData->PointDataSetName.Clear();
	m_MeshData->Map_PointDataSet_Name_to_Index.clear();
	
	m_MeshData->EdgeDataSet.Clear();
	m_MeshData->EdgeDataSetName.Clear();
	m_MeshData->Map_EdgeDataSet_Name_to_Index.clear();
	
	m_MeshData->FaceDataSet.Clear();
	m_MeshData->FaceDataSetName.Clear();
	m_MeshData->Map_FaceDataSet_Name_to_Index.clear();

	m_MeshData->CellDataSet.Clear();
	m_MeshData->CellDataSetName.Clear();
	m_MeshData->Map_CellDataSet_Name_to_Index.clear();

    m_MeshData->Attribute.Clear();
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Clear(const MDK_Symbol_PureEmpty&)
{
	if (m_MeshData)
	{
		m_MeshData.reset();
	}
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Copy(const Mesh<ScalarType>& InputMesh)
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
            m_MeshData->FaceList[k].SetParentMesh(*this);
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

	m_MeshData->Map_Point_Name_to_Index = InputMesh.m_MeshData->Map_Point_Name_to_Index;
	m_MeshData->Map_Edge_Name_to_Index = InputMesh.m_MeshData->Map_Edge_Name_to_Index;
	m_MeshData->Map_Face_Name_to_Index = InputMesh.m_MeshData->Map_Face_Name_to_Index;
	m_MeshData->Map_Cell_Name_to_Index = InputMesh.m_MeshData->Map_Cell_Name_to_Index;	

	m_MeshData->PointDataSet = InputMesh.m_MeshData->PointDataSet;
	m_MeshData->PointDataSetName = InputMesh.m_MeshData->PointDataSetName;
	m_MeshData->Map_PointDataSet_Name_to_Index = InputMesh.m_MeshData->Map_PointDataSet_Name_to_Index;

	m_MeshData->EdgeDataSet = InputMesh.m_MeshData->EdgeDataSet;
	m_MeshData->EdgeDataSetName = InputMesh.m_MeshData->EdgeDataSetName;
	m_MeshData->Map_EdgeDataSet_Name_to_Index = InputMesh.m_MeshData->Map_EdgeDataSet_Name_to_Index;

	m_MeshData->FaceDataSet = InputMesh.m_MeshData->FaceDataSet;
	m_MeshData->FaceDataSetName = InputMesh.m_MeshData->FaceDataSetName;
	m_MeshData->Map_FaceDataSet_Name_to_Index = InputMesh.m_MeshData->Map_FaceDataSet_Name_to_Index;

	m_MeshData->CellDataSet = InputMesh.m_MeshData->CellDataSet;
	m_MeshData->CellDataSetName = InputMesh.m_MeshData->CellDataSetName;
	m_MeshData->Map_CellDataSet_Name_to_Index = InputMesh.m_MeshData->Map_CellDataSet_Name_to_Index;

	m_MeshData->PointSet = InputMesh.m_MeshData->PointSet;
	m_MeshData->PointSetName = InputMesh.m_MeshData->PointSetName;
	m_MeshData->Map_PointSet_Name_to_Index = InputMesh.m_MeshData->Map_PointSet_Name_to_Index;

	m_MeshData->EdgeSet = InputMesh.m_MeshData->EdgeSet;
	m_MeshData->EdgeSetName = InputMesh.m_MeshData->EdgeSetName;
	m_MeshData->Map_EdgeSet_Name_to_Index = InputMesh.m_MeshData->Map_EdgeSet_Name_to_Index;

	m_MeshData->FaceSet = InputMesh.m_MeshData->FaceSet;
	m_MeshData->FaceSetName = InputMesh.m_MeshData->FaceSetName;
	m_MeshData->Map_FaceSet_Name_to_Index = InputMesh.m_MeshData->Map_FaceSet_Name_to_Index;

	m_MeshData->CellSet = InputMesh.m_MeshData->CellSet;
	m_MeshData->CellSetName = InputMesh.m_MeshData->CellSetName;
	m_MeshData->Map_CellSet_Name_to_Index = InputMesh.m_MeshData->Map_CellSet_Name_to_Index;

    m_MeshData->Attribute = InputMesh.m_MeshData->Attribute;
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Copy(Mesh<ScalarType>&& InputMesh)
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
			m_MeshData->FaceList[k].SetParentMesh(*this);
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

	m_MeshData->Map_Point_Name_to_Index = std::move(InputMesh.m_MeshData->Map_Point_Name_to_Index);
	m_MeshData->Map_Edge_Name_to_Index = std::move(InputMesh.m_MeshData->Map_Edge_Name_to_Index);
	m_MeshData->Map_Face_Name_to_Index = std::move(InputMesh.m_MeshData->Map_Face_Name_to_Index);
	m_MeshData->Map_Cell_Name_to_Index = std::move(InputMesh.m_MeshData->Map_Cell_Name_to_Index);

	m_MeshData->PointDataSet = std::move(InputMesh.m_MeshData->PointDataSet);
	m_MeshData->PointDataSetName = std::move(InputMesh.m_MeshData->PointDataSetName);
	m_MeshData->Map_PointDataSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_PointDataSet_Name_to_Index);

	m_MeshData->EdgeDataSet = std::move(InputMesh.m_MeshData->EdgeDataSet);
	m_MeshData->EdgeDataSetName = std::move(InputMesh.m_MeshData->EdgeDataSetName);
	m_MeshData->Map_EdgeDataSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_EdgeDataSet_Name_to_Index);

	m_MeshData->FaceDataSet = std::move(InputMesh.m_MeshData->FaceDataSet);
	m_MeshData->FaceDataSetName = std::move(InputMesh.m_MeshData->FaceDataSetName);
	m_MeshData->Map_FaceDataSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_FaceDataSet_Name_to_Index);

	m_MeshData->CellDataSet = std::move(InputMesh.m_MeshData->CellDataSet);
	m_MeshData->CellDataSetName = std::move(InputMesh.m_MeshData->CellDataSetName);
	m_MeshData->Map_CellDataSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_CellDataSet_Name_to_Index);

	m_MeshData->PointSet = std::move(InputMesh.m_MeshData->PointSet);
	m_MeshData->PointSetName = std::move(InputMesh.m_MeshData->PointSetName);
	m_MeshData->Map_PointSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_PointSet_Name_to_Index);

	m_MeshData->EdgeSet = std::move(InputMesh.m_MeshData->EdgeSet);
	m_MeshData->EdgeSetName = std::move(InputMesh.m_MeshData->EdgeSetName);
	m_MeshData->Map_EdgeSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_EdgeSet_Name_to_Index);

	m_MeshData->FaceSet = std::move(InputMesh.m_MeshData->FaceSet);
	m_MeshData->FaceSetName = std::move(InputMesh.m_MeshData->FaceSetName);
	m_MeshData->Map_FaceSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_FaceSet_Name_to_Index);

	m_MeshData->CellSet = std::move(InputMesh.m_MeshData->CellSet);
	m_MeshData->CellSetName = std::move(InputMesh.m_MeshData->CellSetName);
	m_MeshData->Map_CellSet_Name_to_Index = std::move(InputMesh.m_MeshData->Map_CellSet_Name_to_Index);

	m_MeshData->Attribute = std::move(InputMesh.m_MeshData->Attribute);

	InputMesh.Clear();
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Share(Mesh& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::ForceShare(const Mesh<ScalarType>& InputMesh)
{
	m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::Recreate()
{
	m_MeshData = std::make_shared<MeshData<ScalarType>>();
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline 
bool Mesh<ScalarType>::IsEmpty() const
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

template<typename ScalarType>
inline
bool Mesh<ScalarType>::IsPureEmpty() const
{
	return (!m_MeshData);
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetPointCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->PointValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetEdgeCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->EdgeValidityFlagList.Sum();	
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetFaceCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->FaceValidityFlagList.Sum();
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetCellCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->CellValidityFlagList.Sum();
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetNamedPointCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->Map_Point_Name_to_Index.size();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetNamedFaceCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->Map_Face_Name_to_Index.size();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetNamedCellCount() const
{
	if (!m_MeshData)
	{
		return 0;
	}
	return m_MeshData->Map_Cell_Name_to_Index.size();
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetMaxValueOfPointIndex() const
{
	if (!m_MeshData)
	{
		return -1;
	}
	return m_MeshData->PointPositionTable.GetColCount() - 1;
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetMaxValueOfEdgeIndex() const
{
	if (!m_MeshData)
	{
		return -1;
	}
	return m_MeshData->EdgeList.GetLength() - 1;
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetMaxValueOfFaceIndex() const
{
	if (!m_MeshData)
	{
		return -1;
	}
	return m_MeshData->FaceList.GetLength() - 1;
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetMaxValueOfCellIndex() const
{
	if (!m_MeshData)
	{
		return -1;
	}
	return m_MeshData->CellList.GetLength() - 1;
}


//------ Get/Set Attribute -----------------------------------//

template<typename ScalarType>
inline
StandardAttribute_Of_Mesh<ScalarType>& Mesh<ScalarType>::Attribute()
{
    return m_MeshData->Attribute;
}

template<typename ScalarType>
inline
const StandardAttribute_Of_Mesh<ScalarType>& Mesh<ScalarType>::Attribute() const
{
    return m_MeshData->Attribute;
}

//----------------------- ID ---------------------------------------//

template<typename ScalarType>
void Mesh<ScalarType>::SetName(String Name)
{
	m_MeshData->Name = std::move(Name);
}

template<typename ScalarType>
String Mesh<ScalarType>::GetName() const
{
	return m_MeshData->Name;
}

//----------------------- StructureType ---------------------------------------//

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfPolygonMesh() const
{
	if (this->IsEmpty() == true)
	{
		return false;
	}

	return (m_MeshData->CellList.IsEmpty() == true);
}


template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfTriangleMesh() const
{
	if (this->CheckIfPolygonMesh() == false)
	{
		return false;
	}

	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_max; ++k)
	{
		if (this->IsValidFaceIndex(k) == true)
		{
			auto PointCount = this->Face(k).GetPointCount();
			if (PointCount != 3)
			{
				return false;
			}
		}
	}

	return true;
}

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfQuadMesh() const
{
	if (this->CheckIfPolygonMesh() == false)
	{
		return false;
	}

	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_max; ++k)
	{
		if (this->IsValidFaceIndex(k) == true)
		{
			auto PointCount = this->Face(k).GetPointCount();
			if (PointCount != 4)
			{
				return false;
			}
		}
	}

	return true;
}

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfMixedTriangleQuadMesh() const
{
	if (this->CheckIfPolygonMesh() == false)
	{
		return false;
	}

	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_max; ++k)
	{
		if (this->IsValidFaceIndex(k) == true)
		{
			auto PointCount = this->Face(k).GetPointCount();
			if (PointCount != 3 && PointCount != 4)
			{
				return false;
			}
		}
	}

	return true;
}

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfPolyhedronMesh() const
{
	if (this->IsEmpty() == true)
	{
		return false;
	}

	return (m_MeshData->CellList.IsEmpty() == false);
}

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfTetrahedronMesh() const
{
	if (this->CheckIfPolyhedronMesh() == false)
	{
		return false;
	}

	int_max CellIndex_max = this->GetMaxValueOfCellIndex();
	for (int_max k = 0; k <= CellIndex_max; ++k)
	{
		if (this->IsValidCellIndex(k) == true)
		{
			auto PointCount = this->Cell(k).GetPointCount();
			if (PointCount != 4)
			{
				return false;
			}
		}
	}
	return true;
}


template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfHexahedronMesh() const
{
	if (this->CheckIfPolyhedronMesh() == false)
	{
		return false;
	}

	int_max CellIndex_max = this->GetMaxValueOfCellIndex();
	for (int_max k = 0; k <= CellIndex_max; ++k)
	{
		if (this->IsValidCellIndex(k) == true)
		{
			auto PointCount = this->Cell(k).GetPointCount();
			if (PointCount != 8)
			{
				return false;
			}
		}
	}
	return true;
}

template<typename ScalarType>
bool Mesh<ScalarType>::CheckIfMixedTetHexMesh() const
{
	if (this->CheckIfPolyhedronMesh() == false)
	{
		return false;
	}

	int_max CellIndex_max = this->GetMaxValueOfCellIndex();
	for (int_max k = 0; k <= CellIndex_max; ++k)
	{
		if (this->IsValidCellIndex(k) == true)
		{
			auto PointCount = this->Cell(k).GetPointCount();
			if (PointCount != 4 && PointCount != 8)
			{
				return false;
			}
		}
	}
	return true;
}

//------------- Get/Set All the position (valid and invalid point) --------------------------------//

template<typename ScalarType>
inline 
DenseMatrix<ScalarType> Mesh<ScalarType>::GetPointPosition(const MDK_Symbol_ALL&) const
{
	return m_MeshData->PointPositionTable;
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::GetPointPosition(const MDK_Symbol_ALL&, DenseMatrix<ScalarType>& PositionMatrix) const
{
	if (this->IsEmpty() == true)
	{
		PositionMatrix.Clear();
		return;
	}

	PositionMatrix = m_MeshData->PointPositionTable;
}

template<typename ScalarType>
inline void Mesh<ScalarType>::SetPointPosition(const MDK_Symbol_ALL&, DenseMatrix<ScalarType> PositionMatrix) const
{
	if (PositionMatrix.GetColCount() != m_MeshData->PointList.GetLength())
	{
		MDK_Error(" input size is wrong @ Mesh<ScalarType>::SetPointPosition(ALL, ...)")
		return;
	}
	m_MeshData->PointPositionTable = std::move(PositionMatrix);
}

//---- Get/Set 3D Position by PointIndex --------------------------------------------------------------------------//

template<typename ScalarType>
inline
void Mesh<ScalarType>::
SetPointPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.SetCol(PointIndex, { x, y, z });
}

template<typename ScalarType>
inline 
void Mesh<ScalarType>::SetPointPosition(int_max PointIndex, const ScalarType Position[3])
{
    m_MeshData->PointPositionTable.SetCol(PointIndex, Position);
}

template<typename ScalarType>
inline
void Mesh<ScalarType>::SetPointPosition(int_max PointIndex, const DenseVector<ScalarType,3>& Position)
{
	m_MeshData->PointPositionTable.SetCol(PointIndex, Position);
}

template<typename ScalarType>
inline
void Mesh<ScalarType>::
SetPointPosition(const DenseVector<int_max>& PointIndexList, const DenseMatrix<ScalarType>& PointPositionMatrix)
{
	if (PointIndexList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == true)
	{
		return;
	}
	else if (PointIndexList.IsEmpty() == true && PointPositionMatrix.IsEmpty() == false)
	{
		MDK_Error("Invalid input @ Mesh::SetPointPosition(...)")
		return;
	}
	else if (PointIndexList.IsEmpty() == false && PointPositionMatrix.IsEmpty() == true)
	{
		MDK_Error("Invalid input @ Mesh::SetPointPosition(...)")
		return;
	}

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		auto PointIndex = PointIndexList[k];
		if (PointIndex < 0 || PointIndex >= m_MeshData->PointPositionTable.GetColCount())
		{
			MDK_Error("Invalid PointIndex @ Mesh::SetPointPosition(...)")
			return;
		}

		m_MeshData->PointPositionTable.SetCol(PointIndex, PointPositionMatrix.GetPointerOfCol(k));
	}
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Mesh<ScalarType>::GetPointPosition(int_max PointIndex) const
{
    DenseVector<ScalarType, 3> Position;
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position.GetPointer());
    return Position;
}

template<typename ScalarType>
inline
void Mesh<ScalarType>::GetPointPosition(int_max PointIndex, DenseVector<ScalarType, 3>& Position) const
{
	m_MeshData->PointPositionTable.GetCol(PointIndex, Position.GetPointer());
}

template<typename ScalarType>
inline 
void Mesh<ScalarType>::GetPointPosition(int_max PointIndex, ScalarType& x, ScalarType& y, ScalarType& z) const
{
    ScalarType Position[3];
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position);
    x = Position[0];
    y = Position[1];
    z = Position[2];
}

template<typename ScalarType>
inline 
void Mesh<ScalarType>::GetPointPosition(int_max PointIndex, ScalarType Position[3]) const
{
    m_MeshData->PointPositionTable.GetCol(PointIndex, Position);
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType> Mesh<ScalarType>::GetPointPosition(const DenseVector<int_max>& PointIndexList) const
{
    DenseMatrix<ScalarType> PointPositionMatrix;
    this->GetPointPosition(PointIndexList, PointPositionMatrix);
    return PointPositionMatrix;
}


template<typename ScalarType>
inline
void Mesh<ScalarType>::GetPointPosition(const DenseVector<int_max>& PointIndexList, DenseMatrix<ScalarType>& PointPositionMatrix) const
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
            MDK_Error("Invalid PointIndex @ Mesh::GetPointPosition(...)")
            PointPositionMatrix.FastResize(0, 0);
            return;
        }
        PointPositionMatrix.SetCol(k, m_MeshData->PointPositionTable.GetPointerOfCol(PointIndex));
    }
}

//------------------------- Mesh Element ----------------------------------------------

template<typename ScalarType>
inline
Point_Of_Mesh<ScalarType>& Mesh<ScalarType>::Point(int_max PointIndex)
{
    return m_MeshData->PointList[PointIndex];
}

template<typename ScalarType>
inline
const Point_Of_Mesh<ScalarType>& Mesh<ScalarType>::Point(int_max PointIndex) const
{
    return m_MeshData->PointList[PointIndex];
}

template<typename ScalarType>
inline
Edge_Of_Mesh<ScalarType>& Mesh<ScalarType>::Edge(int_max EdgeIndex)
{
    return m_MeshData->EdgeList[EdgeIndex];
}

template<typename ScalarType>
inline
const Edge_Of_Mesh<ScalarType>& Mesh<ScalarType>::Edge(int_max EdgeIndex) const
{
    return m_MeshData->EdgeList[EdgeIndex];
}


template<typename ScalarType>
inline
Face_Of_Mesh<ScalarType>& Mesh<ScalarType>::Face(int_max FaceIndex)
{
    return m_MeshData->FaceList[FaceIndex];
}

template<typename ScalarType>
inline
const Face_Of_Mesh<ScalarType>& Mesh<ScalarType>::Face(int_max FaceIndex) const
{
    return m_MeshData->FaceList[FaceIndex];
}

template<typename ScalarType>
inline
Cell_Of_Mesh<ScalarType>& Mesh<ScalarType>::Cell(int_max CellIndex)
{
	return m_MeshData->CellList[CellIndex];
}

template<typename ScalarType>
inline
const Cell_Of_Mesh<ScalarType>& Mesh<ScalarType>::Cell(int_max CellIndex) const
{
	return m_MeshData->CellList[CellIndex];
}

//-------------- check index -------------------------------------------------------//

template<typename ScalarType>
bool Mesh<ScalarType>::IsValidPointIndex(int_max PointIndex) const
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

template<typename ScalarType>
bool Mesh<ScalarType>::IsValidEdgeIndex(int_max EdgeIndex) const
{
    if (EdgeIndex < 0 || EdgeIndex >= m_MeshData->EdgeValidityFlagList.GetLength())
    {
        return false;
    }
	return (m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1);    
}


template<typename ScalarType>
bool Mesh<ScalarType>::IsValidFaceIndex(int_max FaceIndex) const
{
    if (FaceIndex < 0 || FaceIndex >= m_MeshData->FaceValidityFlagList.GetLength())
    {
        return false;
    }    
	return (m_MeshData->FaceValidityFlagList[FaceIndex] == 1);  
}

template<typename ScalarType>
bool Mesh<ScalarType>::IsValidCellIndex(int_max CellIndex) const
{
	if (CellIndex < 0 || CellIndex >= m_MeshData->CellValidityFlagList.GetLength())
	{
		return false;
	}
	return (m_MeshData->CellValidityFlagList[CellIndex] == 1);
}

//--------- get Valid IndexList ------------------------------------------------------------//

template<typename ScalarType>
inline
DenseVector<int_max> Mesh<ScalarType>::GetValidPointIndexList() const
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

template<typename ScalarType>
inline 
DenseVector<int_max> Mesh<ScalarType>::GetValidEdgeIndexList() const
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


template<typename ScalarType>
inline
DenseVector<int_max> Mesh<ScalarType>::GetValidFaceIndexList() const
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


template<typename ScalarType>
inline
DenseVector<int_max> Mesh<ScalarType>::GetValidCellIndexList() const
{
	DenseVector<int_max> OutputIndexList;
	OutputIndexList.SetCapacity(m_MeshData->CellList.GetLength());
	for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
	{
		if (m_MeshData->CellValidityFlagList[k] == 1)
		{
			OutputIndexList.Append(k);
		}
	}
	return OutputIndexList;
}

//----------- get PointIndex by Position, ID, Name ----------------------------------------------//

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetPointIndexByPosition(const DenseVector<ScalarType, 3>& Position, ScalarType DistanceThreshold) const
{
    return this->GetPointIndexByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetPointIndexByPosition(const ScalarType Position[3], ScalarType DistanceThreshold) const
{
    return this->GetPointIndexByPosition(Position[0], Position[1], Position[2], DistanceThreshold);
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetPointIndexByPosition(ScalarType x, ScalarType y, ScalarType z, ScalarType DistanceThreshold) const
{
    if (this->IsEmpty() == true)
    {        
        return -1;
    }

    if (DistanceThreshold < 0)
    {
        MDK_Error("DistanceThreshold < 0 @ Mesh::GetPointIndexByPosition(...)")
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

            if (Distance_sq < Distance_sq_min)
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


template<typename ScalarType>
inline int_max Mesh<ScalarType>::GetPointIndexByName(const String& PointName) const
{
	if (PointName.IsEmpty() == true)
	{
		//MDK_Warning("PointName is empty @ Mesh::GetPointIndexByID(...)")
		return -1;
	}

	auto it = m_MeshData->Map_Point_Name_to_Index.find(PointName);
	if (it != m_MeshData->Map_Point_Name_to_Index.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid PointName @ Mesh::GetPointIndexByID(...)")
		return -1;
	}
}

//----------- get EdgeIndex by Point, ID, Name -------------------------------------//
template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetEdgeIndexByPoint(int_max PointIndex0, int_max PointIndex1) const
{
	if (this->IsValidPointIndex(PointIndex0) == false || this->IsValidPointIndex(PointIndex1) == false)
	{
		//MDK_Warning("PointIndex0 or PointIndex1 is invalid @ Mesh::GetEdgeIndexByPoint(...)")		
		return -1;
	}

	if (m_MeshData->PointList[PointIndex0].IsOnEdge() == true && m_MeshData->PointList[PointIndex1].IsOnEdge() == true)
	{
		const auto& AdjacentEdgeIndexList0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList();
		const auto& AdjacentEdgeIndexList1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList();
		auto EdgeIndexList = Intersect(AdjacentEdgeIndexList0, AdjacentEdgeIndexList1);
		if (EdgeIndexList.IsEmpty() == false)
		{
			if (EdgeIndexList.GetLength() > 1)
			{
				MDK_Warning("EdgeIndexList.GetLength() > 1 @ Mesh::GetEdgeIndexByPoint(...)")
			}
			return EdgeIndexList[0];
		}
	}

	//MDK_Warning("No Edge between PointIndex0 and PointIndex1 @ Mesh::GetEdgeIndexByPoint(...)")
	return -1;
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetEdgeIndexByName(const String& EdgeName) const
{
	if (EdgeName.IsEmpty() == true)
	{
		//MDK_Warning("EdgeName is empty @ Mesh::GetEdgeIndexByName(...)")        
		return -1;
	}

	auto it = m_MeshData->Map_Edge_Name_to_Index.find(EdgeName);
	if (it != m_MeshData->Map_Edge_Name_to_Index.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid EdgeName @ Mesh::GetEdgeIndexByName(...)")
		return -1;
	}
}

//----------- get FaceIndex by Point, Edge, Name ----------//
template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetFaceIndexByPoint(const DenseVector<int_max>& PointIndexList) const
{
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			//MDK_Warning("PointIndexList is invalid @ Mesh::GetFaceIndexByPoint(...)")
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
			//MDK_Warning("PointIndexList is invalid @ Mesh::GetFaceIndexByPoint(...)")
			return -1;
		}
	}
	return this->GetFaceIndexByEdge(EdgeIndexList);
}


template<typename ScalarType>
inline 
int_max Mesh<ScalarType>::GetFaceIndexByEdge(const DenseVector<int_max>& EdgeIndexList) const
{
    for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
    {
        if (this->IsValidEdgeIndex(EdgeIndexList[k]) == false)
        {
            //MDK_Warning("EdgeIndexList is invalid @ Mesh::GetFaceIndexByEdge(...)")            
            return -1;
        }
    }

	//---------------------------------------------------------------------------------------------------
	auto FaceIndexList_shared = m_MeshData->EdgeList[EdgeIndexList[0]].GetAdjacentFaceIndexList();
	for (int_max k = 1; k < EdgeIndexList.GetLength(); ++k)
	{
		FaceIndexList_shared = Intersect(FaceIndexList_shared, m_MeshData->EdgeList[EdgeIndexList[k]].GetAdjacentFaceIndexList());
	}
	//----------------------
	if (FaceIndexList_shared.GetLength() != 1)
	{
		//MDK_Warning("EdgeIndexList is invalid @ Mesh::GetFaceIndexByEdge(...)")
		return -1;
	}

	return FaceIndexList_shared[0];
}


template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetFaceIndexByName(const String& FaceName) const
{
	if (FaceName.IsEmpty() == true)
	{
		//MDK_Warning("FaceName is empty @ Mesh::GetFaceIndexByName(...)")
		return -1;
	}

	auto it = m_MeshData->Map_Face_Name_to_Index.find(FaceName);
	if (it != m_MeshData->Map_Face_Name_to_Index.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid FaceName @ Mesh::GetFaceIndexByName(...)")
		return -1;
	}
}

template<typename ScalarType>
inline 
int_max Mesh<ScalarType>::GetCellIndexByPoint(const DenseVector<int_max>& PointIndexList) const
{
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			//MDK_Warning("PointIndexList is invalid @ Mesh::GetCellIndexByPoint(...)")            
			return -1;
		}
	}
	//---------------------------------------------------------------------------------------------------
	DenseVector<int_max> EdgeIndexList;
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		EdgeIndexList.Append(m_MeshData->PointList[PointIndexList[k]].GetAdjacentEdgeIndexList());
	}
	EdgeIndexList = EdgeIndexList.GetSubSet(EdgeIndexList.FindUnique());
	return this->GetCellIndexByEdge(EdgeIndexList);
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetCellIndexByEdge(const DenseVector<int_max>& EdgeIndexList) const
{
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		if (this->IsValidEdgeIndex(EdgeIndexList[k]) == false)
		{
			//MDK_Warning("EdgeIndexList is invalid @ Mesh::GetCellIndexByEdge(...)")            
			return -1;
		}
	}
	//---------------------------------------------------------------------------------------------------
	DenseVector<int_max> FaceIndexList;
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		FaceIndexList.Append(m_MeshData->EdgeList[EdgeIndexList[k]].GetAdjacentFaceIndexList());
	}
	FaceIndexList = FaceIndexList.GetSubSet(FaceIndexList.FindUnique());
	return this->GetCellIndexByFace(FaceIndexList);
}

template<typename ScalarType>
inline
int_max Mesh<ScalarType>::GetCellIndexByFace(const DenseVector<int_max>& FaceIndexList) const
{
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		if (this->IsValidFaceIndex(FaceIndexList[k]) == false)
		{
			//MDK_Warning("FaceIndexList is invalid @ Mesh::GetCellIndexByFace(...)")            
			return -1;
		}
	}

	//---------------------------------------------------------------------------------------------------
	auto CellIndexList_shared = m_MeshData->FaceList[FaceIndexList[0]].GetAdjacentCellIndexList();
	for (int_max k = 1; k < FaceIndexList.GetLength(); ++k)
	{
		CellIndexList_shared = Intersect(CellIndexList_shared, m_MeshData->FaceList[FaceIndexList[k]].GetAdjacentCellIndexList());
	}
	//----------------------
	if (CellIndexList_shared.GetLength() != 1)
	{
		//MDK_Warning("FaceIndexList is invalid @ Mesh::GetCellIndexByFace(...)")
		return -1;
	}

	return CellIndexList_shared[0];
}

template<typename ScalarType>
inline 
int_max Mesh<ScalarType>::GetCellIndexByName(const String& CellName) const
{
	if (CellName.IsEmpty() == true)
	{
		//MDK_Warning("CellName is empty @ Mesh::GetCellIndexByName(...)")
		return -1;
	}

	auto it = m_MeshData->Map_Cell_Name_to_Index.find(CellName);
	if (it != m_MeshData->Map_Cell_Name_to_Index.end())
	{
		return it->second;
	}
	else
	{
		//MDK_Warning("Invalid CellName @ Mesh::GetCellIndexByName(...)")
		return -1;
	}
}

//-------------- check Name -------------------------------------------------------//

template<typename ScalarType>
inline bool Mesh<ScalarType>::IsValidPointName(const String& PointName) const
{
	if (PointName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_Point_Name_to_Index.find(PointName);
	return (it != m_MeshData->Map_Point_Name_to_Index.end());
}

template<typename ScalarType>
inline bool Mesh<ScalarType>::IsValidEdgeName(const String& EdgeName) const
{
	if (EdgeName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_Edge_Name_to_Index.find(EdgeName);
	return (it != m_MeshData->Map_Edge_Name_to_Index.end());
}


template<typename ScalarType>
inline bool Mesh<ScalarType>::IsValidFaceName(const String& FaceName) const
{
	if (FaceName.IsEmpty() == true)
	{
		return false;
	}
	auto it = m_MeshData->Map_Face_Name_to_Index.find(FaceName);
	return (it != m_MeshData->Map_Face_Name_to_Index.end());
}

//--------- get Valid NameList ------------------------------------------------------------//

template<typename ScalarType>
StdObjectVector<String> Mesh<ScalarType>::GetValidPointNameList() const
{
	StdObjectVector<String> NameList;
	for (int_max k = 0; k < m_MeshData->PointValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->PointValidityFlagList[k] == 1)
		{
			NameList.Append(m_MeshData->PointList[k].GetName());
		}
	}
	return NameList;

	//this is wrong, Map may not keep insertion order
	//NameList.SetCapacity(m_MeshData->Map_Point_Name_to_Index.size());
	//for (auto it = m_MeshData->Map_Point_Name_to_Index.begin(); it != m_MeshData->Map_Point_Name_to_Index.end(); ++it)
	//{
	//	NameList.Append(it->first);
	//}	
}


template<typename ScalarType>
StdObjectVector<String> Mesh<ScalarType>::GetValidEdgeNameList() const
{
	StdObjectVector<String> NameList;
	for (int_max k = 0; k < m_MeshData->EdgeValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->EdgeValidityFlagList[k] == 1)
		{
			NameList.Append(m_MeshData->EdgeList[k].GetName());
		}
	}
	return NameList;
}


template<typename ScalarType>
StdObjectVector<String> Mesh<ScalarType>::GetValidFaceNameList() const
{
	StdObjectVector<String> NameList;
	for (int_max k = 0; k < m_MeshData->FaceValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->FaceValidityFlagList[k] == 1)
		{
			NameList.Append(m_MeshData->FaceList[k].GetName());
		}
	}
	return NameList;
}


template<typename ScalarType>
StdObjectVector<String> Mesh<ScalarType>::GetValidCellNameList() const
{
	StdObjectVector<String> NameList;
	for (int_max k = 0; k < m_MeshData->CellValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->CellValidityFlagList[k] == 1)
		{
			NameList.Append(m_MeshData->CellList[k].GetName());
		}
	}
	return NameList;
}

//----------- get Point/Edge/Face Name by Index -----------------------------------------------------------//

template<typename ScalarType>
inline String Mesh<ScalarType>::GetPointName(int_max PointIndex) const
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

template<typename ScalarType>
inline StdObjectVector<String> Mesh<ScalarType>::GetPointName(const DenseVector<int_max>& PointIndexList) const
{
	StdObjectVector<String> NameList;
	NameList.Resize(PointIndexList.GetLength());
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetPointName(PointIndexList[k]);
	}
	return NameList;
}


template<typename ScalarType>
inline String Mesh<ScalarType>::GetEdgeName(int_max EdgeIndex) const
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

template<typename ScalarType>
inline StdObjectVector<String> Mesh<ScalarType>::GetEdgeName(const DenseVector<int_max>& EdgeIndexList) const
{
	StdObjectVector<String> NameList;
	NameList.Resize(EdgeIndexList.GetLength());
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetEdgeName(EdgeIndexList[k]);
	}
	return NameList;
}


template<typename ScalarType>
inline String Mesh<ScalarType>::GetFaceName(int_max FaceIndex) const
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

template<typename ScalarType>
inline StdObjectVector<String> Mesh<ScalarType>::GetFaceName(const DenseVector<int_max>& FaceIndexList) const
{
	StdObjectVector<String> NameList;
	NameList.Resize(FaceIndexList.GetLength());
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		NameList[k] = this->GetFaceName(FaceIndexList[k]);
	}
	return NameList;
}

//------------- PointDataSet, EdgeDataSet, FaceDataset, CellDataset ----------------------------------//
template<typename ScalarType>
int_max Mesh<ScalarType>::GetPointDataSetCount() const
{
	return m_MeshData->PointDataSet.GetLength();
}

template<typename ScalarType>
int_max Mesh<ScalarType>::InitializePointDataSet(const String& Name, int_max ScalarCountPerPoint)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("PointDataSet Name is Empty @ Mesh::InitializePointDataSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_PointDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_PointDataSet_Name_to_Index.end())
	{
		auto Index = it->second;
		if (m_MeshData->PointDataSet[Index].GetColCount() != m_MeshData->PointList.GetLength())
		{
			m_MeshData->PointDataSet[Index].Clear();
			m_MeshData->PointDataSet[Index].Resize(ScalarCountPerPoint, m_MeshData->PointList.GetLength());
		}
		return Index;
	}
	else
	{//new
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(ScalarCountPerPoint, m_MeshData->PointList.GetLength());
		m_MeshData->PointDataSet.Append(std::move(DataSet));
		m_MeshData->PointDataSetName.Append(Name);
		auto Index = m_MeshData->PointDataSet.GetLength() - 1;
		m_MeshData->Map_PointDataSet_Name_to_Index[Name] = Index;
		return Index;
	}
}

template<typename ScalarType>
int_max Mesh<ScalarType>::SetPointDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("PointDataSet Name is Empty @ Mesh::SetPointDataSet(...)")
		return -1;
	}

	if (DataSet.GetColCount() != m_MeshData->PointList.GetLength())
	{
		MDK_Error("size is wrong @ Mesh<ScalarType>::SetPointDataSet(...)")
		return -1;
	}

	auto DataSetIndex = this->InitializePointDataSet(Name, DataSet.GetRowCount());
	if (DataSetIndex < 0)
	{
		MDK_Error("Name is wrong @ Mesh<ScalarType>::SetPointDataSet(...)")
		return -1;
	}

	m_MeshData->PointDataSet[DataSetIndex] = std::move(DataSet);
	return DataSetIndex;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetPointDataSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_PointDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_PointDataSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("Name " << Name << " NOT exist @ Mesh::GetPointDataSetIndex(...)")
	return -1;
}

template<typename ScalarType>
String Mesh<ScalarType>::GetPointDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->PointDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetPointDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->PointDataSetName[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetPointDataSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->PointDataSet.GetLength())
	{
		MDK_Error("Invalid Index @ Mesh::GetPointDataSet(...)")
		DenseMatrix<ScalarType> EmptySet;
		return EmptySet;
	}	
	return m_MeshData->PointDataSet[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetPointDataSet(const String& Name) const
{
	auto Index = this->GetPointDataSetIndex(Name);
	return this->GetPointDataSet(Index);
}

//
template<typename ScalarType>
int_max Mesh<ScalarType>::GetEdgeDataSetCount() const
{
	return m_MeshData->EdgeDataSet.GetLength();
}

template<typename ScalarType>
int_max Mesh<ScalarType>::InitializeEdgeDataSet(const String& Name, int_max ScalarCountPerEdge)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("EdgeDataSet Name is Empty @ Mesh::InitializeEdgeDataSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_EdgeDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_EdgeDataSet_Name_to_Index.end())	
	{
		auto Index = it->second;
		if (m_MeshData->EdgeDataSet[Index].GetColCount() != m_MeshData->EdgeList.GetLength())
		{
			m_MeshData->EdgeDataSet[Index].Clear();
			m_MeshData->EdgeDataSet[Index].Resize(ScalarCountPerEdge, m_MeshData->EdgeList.GetLength());
		}
		return Index;
	}
	else
	{//new
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(ScalarCountPerEdge, m_MeshData->EdgeList.GetLength());
		m_MeshData->EdgeDataSet.Append(std::move(DataSet));
		m_MeshData->EdgeDataSetName.Append(Name);
		auto Index = m_MeshData->EdgeDataSet.GetLength() - 1;
		m_MeshData->Map_EdgeDataSet_Name_to_Index[Name] = Index;
		return Index;
	}
}

template<typename ScalarType>
int_max Mesh<ScalarType>::SetEdgeDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("EdgeDataSet Name is Empty @ Mesh::SetEdgeDataSet(...)")
		return -1;
	}

	if (DataSet.GetColCount() != m_MeshData->EdgeList.GetLength())
	{
		MDK_Error("size is wrong @ Mesh<ScalarType>::SetEdgeDataSet(...)")
		return -1;
	}

	auto DataSetIndex = this->InitializeEdgeDataSet(Name, DataSet.GetRowCount());
	if (DataSetIndex < 0)
	{
		MDK_Error("Name is wrong @ Mesh<ScalarType>::SetEdgeDataSet(...)")
		return -1;
	}

	m_MeshData->EdgeDataSet[DataSetIndex] = std::move(DataSet);
	return DataSetIndex;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetEdgeDataSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_EdgeDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_EdgeDataSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("Name " << Name << " NOT exist @ Mesh::GetEdgeDataSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetEdgeDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->EdgeDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetEdgeDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->EdgeDataSetName[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetEdgeDataSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->EdgeDataSet.GetLength())
	{
		MDK_Error("Invalid Index @ Mesh::GetEdgeDataSet(...)")
		DenseMatrix<ScalarType> EmptySet;
		return EmptySet;
	}
	return m_MeshData->EdgeDataSet[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetEdgeDataSet(const String& Name) const
{
	auto Index = this->GetEdgeDataSetIndex(Name);
	return this->GetEdgeDataSet(Index);
}

//
template<typename ScalarType>
int_max Mesh<ScalarType>::GetFaceDataSetCount() const
{
	return m_MeshData->FaceDataSet.GetLength();
}

template<typename ScalarType>
int_max Mesh<ScalarType>::InitializeFaceDataSet(const String& Name, int_max ScalarCountPerFace)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("FaceDataSet Name is Empty @ Mesh::InitializeFaceDataSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_FaceDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_FaceDataSet_Name_to_Index.end())
	{
		auto Index = it->second;	
		if (m_MeshData->FaceDataSet[Index].GetColCount() != m_MeshData->FaceList.GetLength())
		{
			m_MeshData->FaceDataSet[Index].Clear();
			m_MeshData->FaceDataSet[Index].Resize(ScalarCountPerFace, m_MeshData->FaceList.GetLength());
		}
		return Index;
	}
	else
	{//new
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(ScalarCountPerFace, m_MeshData->FaceList.GetLength());
		m_MeshData->FaceDataSet.Append(std::move(DataSet));
		m_MeshData->FaceDataSetName.Append(Name);
		auto Index = m_MeshData->FaceDataSet.GetLength() - 1;
		m_MeshData->Map_FaceDataSet_Name_to_Index[Name] = Index;
		return Index;
	}
}

template<typename ScalarType>
int_max Mesh<ScalarType>::SetFaceDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("FaceDataSet Name is Empty @ Mesh::SetFaceDataSet(...)")
		return -1;
	}

	if (DataSet.GetColCount() != m_MeshData->FaceList.GetLength())
	{
		MDK_Error("size is wrong @ Mesh<ScalarType>::SetFaceDataSet(...)")
		return -1;
	}

	auto DataSetIndex = this->InitializeFaceDataSet(Name, DataSet.GetRowCount());
	if (DataSetIndex < 0)
	{
		MDK_Error("Name is wrong @ Mesh<ScalarType>::SetFaceDataSet(...)")
		return -1;
	}

	m_MeshData->FaceDataSet[DataSetIndex] = std::move(DataSet);
	return DataSetIndex;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetFaceDataSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_FaceDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_FaceDataSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("Name " << Name << " NOT exist @ Mesh::GetFaceDataSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetFaceDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->FaceDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetFaceDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->FaceDataSetName[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetFaceDataSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->FaceDataSet.GetLength())
	{
		MDK_Error("Invalid Index @ Mesh::GetFaceDataSet(...)")
		DenseMatrix<ScalarType> EmptySet;
		return EmptySet;
	}
	return m_MeshData->FaceDataSet[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetFaceDataSet(const String& Name) const
{
	auto Index = this->GetFaceDataSetIndex(Name);
	return this->GetFaceDataSet(Index);
}

//
template<typename ScalarType>
int_max Mesh<ScalarType>::GetCellDataSetCount() const
{
	return m_MeshData->CellDataSet.GetLength();
}

template<typename ScalarType>
int_max Mesh<ScalarType>::InitializeCellDataSet(const String& Name, int_max ScalarCountPerCell)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("CellDataSet Name is Empty @ Mesh::InitializeCellDataSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_CellDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_CellDataSet_Name_to_Index.end())
	{
		auto Index = it->second;
		if (m_MeshData->CellDataSet[Index].GetColCount() != m_MeshData->CellList.GetLength())
		{
			m_MeshData->CellDataSet[Index].Clear();
			m_MeshData->CellDataSet[Index].Resize(ScalarCountPerCell, m_MeshData->CellList.GetLength());
		}
		return Index;
	}
	else
	{//new
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(ScalarCountPerCell, m_MeshData->CellList.GetLength());
		m_MeshData->CellDataSet.Append(std::move(DataSet));
		m_MeshData->CellDataSetName.Append(Name);
		auto Index = m_MeshData->CellDataSet.GetLength() - 1;
		m_MeshData->Map_CellDataSet_Name_to_Index[Name] = Index;
		return Index;
	}
}

template<typename ScalarType>
int_max Mesh<ScalarType>::SetCellDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("CellDataSet Name is Empty @ Mesh::SetCellDataSet(...)")
		return -1;
	}

	if (DataSet.GetColCount() != m_MeshData->CellList.GetLength())
	{
		MDK_Error("size is wrong @ Mesh<ScalarType>::SetCellDataSet(...)")
		return -1;
	}

	auto DataSetIndex = this->InitializeCellDataSet(Name, DataSet.GetRowCount());
	if (DataSetIndex < 0)
	{
		MDK_Error("Name is wrong @ Mesh<ScalarType>::SetCellDataSet(...)")
		return -1;
	}

	m_MeshData->CellDataSet[DataSetIndex] = std::move(DataSet);
	return DataSetIndex;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetCellDataSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_CellDataSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_CellDataSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("Name " << Name << " NOT exist @ Mesh::GetCellDataSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetCellDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->CellDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetCellDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->CellDataSetName[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetCellDataSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->CellDataSet.GetLength())
	{
		MDK_Error("Invalid Index @ Mesh::GetCellDataSet(...)")
		DenseMatrix<ScalarType> EmptySet;
		return EmptySet;
	}
	return m_MeshData->CellDataSet[Index];
}

template<typename ScalarType>
DenseMatrix<ScalarType> Mesh<ScalarType>::GetCellDataSet(const String& Name) const
{
	auto Index = this->GetCellDataSetIndex(Name);
	return this->GetCellDataSet(Index);
}

//------------ PointSet, EdgeSet, FaceSet, CellSet ------------------------------------------//

template<typename ScalarType>
int_max Mesh<ScalarType>::GetPointSetCount() const
{
	return m_MeshData->PointSet.GetLength();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::SetPointSet(const String& Name, DenseVector<int_max> PointIndexList)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("PointSet Name is Empty @ Mesh::SetPointSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_PointSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_PointSet_Name_to_Index.end())
	{
		auto Index = it->second;
		m_MeshData->PointSet[Index] = std::move(PointIndexList);
		return Index;
	}
	else
	{
		m_MeshData->PointSet.Append(std::move(PointIndexList));
		m_MeshData->PointSetName.Append(Name);
		auto Index = m_MeshData->PointSet.GetLength() - 1;
		m_MeshData->Map_PointSet_Name_to_Index[Name] = Index;
		return Index;
	}
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetPointSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_PointSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_PointSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("PointSet " << Name << " NOT exist @ Mesh::GetPointSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetPointSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->PointSet.GetLength())
	{
		MDK_Error("PointSet " << Index << " NOT exist @  Mesh::GetPointSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->PointSetName[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetPointSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->PointSet.GetLength())
	{
		MDK_Error("invaid Index @ Mesh::GetPointSet(...)")
		DenseVector<int_max> EmptySet;
		return EmptySet;
	}
	return m_MeshData->PointSet[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetPointSet(const String& Name) const
{
	auto it = m_MeshData->Map_PointSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_PointSet_Name_to_Index.end())
	{
		return m_MeshData->PointSet[it->second];
	}
	MDK_Error("Unknown PointSetName: " << Name << " @ Mesh::GetPointSet(...)")
	DenseVector<int_max> EmptySet;
	return EmptySet;
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetEdgeSetCount() const
{
	return m_MeshData->EdgeSet.GetLength();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::SetEdgeSet(const String& Name, DenseVector<int_max> EdgeIndexList)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("EdgeSet Name is Empty @ Mesh::SetEdgeSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_EdgeSet_Name_to_Index.find(EdgeSetName);
	if (it != m_MeshData->Map_EdgeSet_Name_to_Index.end())
	{
		auto Index = it->second;
		m_MeshData->EdgeSet[Index] = std::move(EdgeSet);
		return Index;
	}
	else
	{
		m_MeshData->EdgeSet.Append(std::move(EdgeSet));
		m_MeshData->EdgeSetName.Append(Name);
		auto Index = m_MeshData->EdgeSet.GetLength() - 1;
		m_MeshData->Map_EdgeSet_Name_to_Index[Name] = Index;
		return Index;
	}
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetEdgeSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_EdgeSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_EdgeSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("EdgeSet NOT exist @ Mesh::GetEdgeSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetEdgeSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->EdgeSet.GetLength())
	{
		MDK_Error("EdgeSet " << Index << " NOT exist @  Mesh::GetEdgeSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->EdgeSetName[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetEdgeSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->EdgeSet.GetLength())
	{
		MDK_Error("EdgeSet " << Index << " NOT exist @  Mesh::GetEdgeSetName(...)")
		DenseVector<int_max> EmptySet;
		return EmptySet;
	}
	return m_MeshData->EdgeSet[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetEdgeSet(const String& Name) const
{
	auto it = m_MeshData->Map_EdgeSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_EdgeSet_Name_to_Index.end())
	{
		return m_MeshData->EdgeSet[it->second];
	}
	MDK_Error("Unknown EdgeSetName: " << Name << " @ Mesh::GetEdgeSet()")
	DenseVector<int_max> EmptySet;
	return EmptySet;
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetFaceSetCount() const
{
	return m_MeshData->FaceSet.GetLength();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::SetFaceSet(const String& Name, DenseVector<int_max> FaceIndexList)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("FaceSet Name is Empty @ Mesh::SetFaceSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_FaceSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_FaceSet_Name_to_Index.end())
	{
		auto Index = it->second;
		m_MeshData->FaceSet[Index] = std::move(FaceIndexList);
		return Index;
	}
	else
	{
		m_MeshData->FaceSet.Append(std::move(FaceIndexList));
		m_MeshData->FaceSetName.Append(Name);
		auto Index = m_MeshData->FaceSet.GetLength() - 1;
		m_MeshData->Map_FaceSet_Name_to_Index[Name] = Index;
		return Index;
	}
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetFaceSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_FaceSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_FaceSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("FaceSet " << Name << " NOT exist @ Mesh::GetFaceSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetFaceSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->FaceSet.GetLength())
	{
		MDK_Error("FaceSet " << Index << "NOT exist @  Mesh::GetFaceSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->FaceSetName[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetFaceSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->FaceSet.GetLength())
	{
		MDK_Error("FaceSet " << Index << "NOT exist @  Mesh::GetFaceSet(...)")
		DenseVector<int_max> EmptySet;
		return EmptySet;
	}
	return m_MeshData->FaceSet[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetFaceSet(const String& Name) const
{
	auto it = m_MeshData->Map_FaceSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_FaceSet_Name_to_Index.end())
	{
		return m_MeshData->FaceSet[it->second];
	}
	MDK_Error("Unknown FaceSetName: " << Name << " @ Mesh::GetFaceSet()")
	DenseVector<int_max> EmptySet;
	return EmptySet;
}

//
template<typename ScalarType>
int_max Mesh<ScalarType>::GetCellSetCount() const
{
	return m_MeshData->CellSet.GetLength();
}


template<typename ScalarType>
int_max Mesh<ScalarType>::SetCellSet(const String& Name, DenseVector<int_max> CellIndexList)
{
	if (Name.IsEmpty() == true)
	{
		MDK_Error("CellSet Name is Empty @ Mesh::SetCellSet(...)")
		return -1;
	}

	auto it = m_MeshData->Map_CellSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_CellSet_Name_to_Index.end())
	{
		auto Index = it->second;
		m_MeshData->CellSet[Index] = std::move(CellIndexList);
		return Index;
	}
	else
	{
		m_MeshData->CellSet.Append(std::move(CellIndexList));
		m_MeshData->CellSetName.Append(Name);
		auto Index = m_MeshData->CellSet.GetLength() - 1;
		m_MeshData->Map_CellSet_Name_to_Index[Name] = Index;
		return Index;
	}
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetCellSetIndex(const String& Name) const
{
	auto it = m_MeshData->Map_CellSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_CellSet_Name_to_Index.end())
	{
		return it->second;
	}
	MDK_Error("CellSet " << Name << " NOT exist @ Mesh::GetCellSetIndex(...)")
	return -1;
}


template<typename ScalarType>
String Mesh<ScalarType>::GetCellSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->CellSet.GetLength())
	{
		MDK_Error("CellSet " << Index << "NOT exist @  Mesh::GetCellSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->CellSetName[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetCellSet(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->CellSet.GetLength())
	{
		MDK_Error("CellSet " << Index << "NOT exist @  Mesh::GetCellSet(...)")
		DenseVector<int_max> EmptySet;
		return EmptySet;
	}
	return m_MeshData->CellSet[Index];
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::GetCellSet(const String& Name) const
{
	auto it = m_MeshData->Map_CellSet_Name_to_Index.find(Name);
	if (it != m_MeshData->Map_CellSet_Name_to_Index.end())
	{
		return m_MeshData->CellSet[it->second];
	}
	MDK_Error("Unknown CellSetName: " << Name << " @ Mesh::GetCellSet()")
	DenseVector<int_max> EmptySet;
	return EmptySet;
}

//------------ SetCapacity, ReleaseUnusedCapacity -------------------------------------//
template<typename ScalarType>
void Mesh<ScalarType>::SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount)
{
	this->SetCapacity(PointCount, EdgeCount, FaceCount, 0);
}


template<typename ScalarType>
void Mesh<ScalarType>::SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount, int_max CellCount)
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
	m_MeshData->CellList.SetCapacity(CellCount);
	m_MeshData->CellValidityFlagList.SetCapacity(CellCount);
}


template<typename ScalarType>
void Mesh<ScalarType>::ReleaseUnusedCapacity()
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
	m_MeshData->CellList->ReleaseUnusedCapacity();
	m_MeshData->CellValidityFlagList->ReleaseUnusedCapacity();
}

//------------------------------ Add Mesh Item -------------------------------------------------------------------------//

template<typename ScalarType>
int_max Mesh<ScalarType>::AddPoint(const DenseVector<ScalarType, 3>& Position)
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddPoint(const DenseVector<ScalarType>& Position)
{
    if (PointSet.GetLength() != 3)
    {
        MDK_Error("Position is a vector but length != 3 @ Mesh::AddPoint(...)")
        return -1;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddPoint(const DenseMatrix<ScalarType>& Position)
{
    if (Position.IsVector() == true)
    {
        if (Position.GetElementCount() != 3)
        {
            MDK_Error("Position is a vector but length != 3 @ Mesh::AddPoint(...)")
            return -1;
        }
    }
    else
    {
        MDK_Error("Position is a not a vector @ Mesh::AddPoint(...)")
        return -1;
    }

    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddPoint(const ScalarType Position[3])
{
    return this->AddPoint(Position[0], Position[1], Position[2]);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddPoint(ScalarType x, ScalarType y, ScalarType z)
{
    m_MeshData->PointPositionTable.AppendCol({x, y, z});
    auto PointIndex = m_MeshData->PointPositionTable.GetColCount() - 1;

    Point_Of_Mesh<ScalarType> Point;
    Point.SetParentMesh(*this);
    Point.SetIndex(PointIndex);
   
    m_MeshData->PointList.Append(std::move(Point));
    m_MeshData->PointValidityFlagList.Append(1);

	for (int_max SetIndex = 0; SetIndex < m_MeshData->PointDataSet.GetLength(); ++SetIndex)
	{
		DenseVector<ScalarType> Data;
		Data.Resize(m_MeshData->PointDataSet[SetIndex].GetRowCount());
		Data.Fill(0);
		m_MeshData->PointDataSet[SetIndex].AppendCol(Data);
	}

    return PointIndex;
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::AddPoint_batch(const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList;

    if (PointSet.IsVector() == true)
    {
        if (PointSet.GetElementCount() != 3)
        {
            MDK_Error("PointSet is a vector but length != 3 @ Mesh::AddPoint_batch(...)")
            return PointIndexList;
        }

		PointIndexList.Resize(1);
		PointIndexList[0] = this->AddPoint(PointSet.GetPointer());
        return PointIndexList;
    }

    if (PointSet.GetRowCount() != 3)
    {
        MDK_Error("PointSet is a matrix but RowCount != 3 @ Mesh::AddPoint_batch(...)")
        return PointIndexList;
    }

	PointIndexList.Resize(PointSet.GetColCount());
    for (int_max k = 0; k < PointSet.GetColCount(); ++k)
    {
		PointIndexList[k] = this->AddPoint(PointSet.GetPointerOfCol(k));
    }
    return PointIndexList;
}


template<typename ScalarType>
DenseVector<int_max> Mesh<ScalarType>::AddPoint_batch(DenseMatrix<ScalarType>&& PointSet)
{
	DenseVector<int_max> PointIndexList;

	if (PointSet.IsVector() == true)
	{
		if (PointSet.GetElementCount() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ Mesh::AddPoint_batch(...)")
			return PointIndexList;
		}

		PointIndexList.Resize(1);
		PointIndexList[0] = this->AddPoint(PointSet.GetPointer());
		return PointIndexList;
	}

	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("PointSet is a matrix but RowCount != 3 @ Mesh::AddPoint_batch(...)")
		return PointIndexList;
	}

	if (this->IsEmpty() == true)
	{
		m_MeshData->PointPositionTable = std::move(PointSet);
		PointIndexList.Resize(m_MeshData->PointPositionTable.GetColCount());
		for (int_max k = 0; k < m_MeshData->PointPositionTable.GetColCount(); ++k)
		{
			Point_Of_Mesh<ScalarType> Point;
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


template<typename ScalarType>
int_max Mesh<ScalarType>::AddEdge(int_max PointIndex0, int_max PointIndex1)
{
	auto EdgeIndex = m_MeshData->EdgeList.GetLength();
	return this->AddEdge(PointIndex0, PointIndex1, EdgeIndex);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddEdge(int_max PointIndex0, int_max PointIndex1, int_max EdgeIndex_input)
{
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

	if (EdgeIndex_input < 0 || EdgeIndex_input > m_MeshData->EdgeList.GetLength())
	{
		MDK_Error("EdgeIndex_input is out of range @ Mesh::AddEdge(...)")
		return -1;
	}

	if (EdgeIndex_input < m_MeshData->EdgeValidityFlagList.GetLength())
	{
		if (m_MeshData->EdgeValidityFlagList[EdgeIndex_input] == 1)
		{
			MDK_Error("Edge exist at EdgeIndex_input @ Mesh::AddEdge(...)")
			return -1;
		}
	}

    if (this->IsValidPointIndex(PointIndex0) == false || this->IsValidPointIndex(PointIndex1) == false)
    {
        MDK_Error("PointIndex0 or PointIndex1 is invalid @ Mesh::AddEdge(...)")
        return -1;
    }	

    auto EdgeIndex_temp = this->GetEdgeIndexByPoint(PointIndex0, PointIndex1);
    if (this->IsValidEdgeIndex(EdgeIndex_temp) == true)
    {
        //MDK_Warning("The edge has been added already @ Mesh::AddEdge(...)")
		if (EdgeIndex_input == m_MeshData->EdgeList.GetLength() || EdgeIndex_input == EdgeIndex_temp)
		{
			return EdgeIndex_temp;
		}
		else
		{
			MDK_Error("Edge exist at EdgeIndex_input @ Mesh::AddEdge(...)")
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
	for (int_max SetIndex = 0; SetIndex < m_MeshData->EdgeDataSet.GetLength(); ++SetIndex)
	{
		if (EdgeIndex_input == m_MeshData->EdgeList.GetLength())
		{
			DenseVector<ScalarType> Data;
			Data.Resize(m_MeshData->EdgeDataSet[SetIndex].GetRowCount());
			Data.Fill(0);
			m_MeshData->EdgeDataSet[SetIndex].AppendCol(Data);
		}
		else
		{
			m_MeshData->EdgeDataSet[SetIndex].FillCol(EdgeIndex_input, 0);
		}
	}
	//------------
    return EdgeIndex_input;
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList)
{
	auto FaceIndex = m_MeshData->FaceList.GetLength();
	return this->AddFaceByEdge(EdgeIndexList, FaceIndex);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList, int_max FaceInex_input)
{
    // Input: the input EdgeIndexList must have been ordered
	// the first point of the first input edge may NOT be the first point of the face
    // the order determine the direction/sign of the normal vector
    //
    // pay attention to MDK_Error or MDK_Warning, they are made on purpose

	if (FaceInex_input < 0 && FaceInex_input > m_MeshData->FaceValidityFlagList.GetLength())
	{
		MDK_Error("FaceInex_input is out of range @ Mesh::AddFaceByEdge(...)")
		return -1;
	}

	if (FaceInex_input < m_MeshData->FaceValidityFlagList.GetLength())
	{
		if (m_MeshData->FaceValidityFlagList[FaceInex_input] == 1)
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceInex_input @ Mesh::AddFaceByEdge(...)")
			return -1;
		}
	}

    if (EdgeIndexList.GetLength() < 2)
    {
        MDK_Error("length of EdgeIndexList < 2 @ Mesh::AddFaceByEdge(...)")
        return -1;
    }

    for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
    {
        if (this->IsValidEdgeIndex(EdgeIndexList[k]) == false)
        {
            MDK_Error("Invalid EdgeIndexList @ Mesh::AddFaceByEdge(...)")
            return -1;
        }
    }

	auto tempList_unique = EdgeIndexList.FindUnique();
	if (tempList_unique.GetLength() != EdgeIndexList.GetLength())
	{
		MDK_Error("EdgeIndexList is invalid : more than one copy of an edge index @ Mesh::AddFaceByEdge(...)")
		return -1;
	}

    auto FaceIndex_temp = this->GetFaceIndexByEdge(EdgeIndexList);
    if (this->IsValidFaceIndex(FaceIndex_temp) == true)
    {
        //MDK_Warning("The face has been added already @ Mesh::AddFaceByEdge(...)")

		if (FaceInex_input == m_MeshData->FaceValidityFlagList.GetLength() || FaceInex_input == FaceIndex_temp)
		{//attempt to add new face, not necessary			
			return FaceIndex_temp;
		}
		else
		{// attempt to replace old&valid face with new face
			MDK_Error("Face arealdy exists at FaceIndex: " << FaceIndex_temp << " @ Mesh::AddFaceByEdge(...)")
			return -1;
		}
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
			MDK_Error("EdgeIndexList is not ordered or invalid @ Mesh::AddFaceByEdge(...)")			
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
	Face.PointIndexList() = PointIndexList;
	Face.EdgeIndexList() = EdgeIndexList;

	// update information in m_MeshData->EdgeList ------------------------------------------------------------------------
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		m_MeshData->EdgeList[EdgeIndexList[k]].AdjacentFaceIndexList().Append(FaceInex_input);
	}

	//------------
	for (int_max SetIndex = 0; SetIndex < m_MeshData->FaceDataSet.GetLength(); ++SetIndex)
	{
		if (FaceInex_input == m_MeshData->FaceList.GetLength())
		{
			DenseVector<ScalarType> Data;
			Data.Resize(m_MeshData->FaceDataSet[SetIndex].GetRowCount());
			Data.Fill(0);
			m_MeshData->FaceDataSet[SetIndex].AppendCol(Data);
		}
		else
		{
			m_MeshData->FaceDataSet[SetIndex].FillCol(FaceInex_input, 0);
		}
	}
	//------------
    return FaceInex_input;
}


template<typename ScalarType>
int_max Mesh<ScalarType>::AddFaceByPoint(const DenseVector<int_max>& PointIndexList)
{
	auto FaceIndex = m_MeshData->FaceList.GetLength();
	return this->AddFaceByPoint(PointIndexList, FaceIndex);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddFaceByPoint(const DenseVector<int_max>& PointIndexList, int_max FaceIndex_input)
{
    if (PointIndexList.GetLength() < 3)
    {        
        MDK_Error("PointIndexList length < 3 @ Mesh::AddFaceByPoint(...)")
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


template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByFace(MeshCellTypeEnum Type, const DenseVector<int_max>& FaceIndexList)
{
	auto CellIndex = m_MeshData->CellList.GetLength();
	return this->AddCellByFace(Type, FaceIndexList, CellIndex);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByFace(MeshCellTypeEnum Type, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input)
{
	if (CellIndex_input < 0 && CellIndex_input > m_MeshData->CellValidityFlagList.GetLength())
	{
		MDK_Error("CellIndex_input is out of range @ Mesh::AddCellByFace(...)")
		return -1;
	}

	if (CellIndex_input < m_MeshData->CellValidityFlagList.GetLength())
	{
		if (m_MeshData->CellValidityFlagList[CellIndex_input] == 1)
		{
			MDK_Error("CellIndex_input is in use @ Mesh::AddCellByFace(...)")
			return -1;
		}
	}

	if (FaceIndexList.GetLength() < 4)
	{
		MDK_Error("length of FaceIndexList < 4 @ Mesh::AddCellByFace(...)")
		return -1;
	}

	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		if (this->IsValidFaceIndex(FaceIndexList[k]) == false)
		{
			MDK_Error("Invalid FaceIndexList @ Mesh::AddCellByFace(...)")
			return -1;
		}
	}

	auto tempList_unique = FaceIndexList.FindUnique();
	if (tempList_unique.GetLength() != FaceIndexList.GetLength())
	{
		MDK_Error("FaceIndexList is invalid : more than one copy of a face index @ Mesh::AddCellByFace(...)")
		return -1;
	}

	auto CellIndex_temp = this->GetCellIndexByFace(FaceIndexList);
	if (this->IsValidCellIndex(CellIndex_temp) == true)
	{
		MDK_Warning("The cell has been added already @ Mesh::AddCellByFace(...)")

		if (CellIndex_input == m_MeshData->CellValidityFlagList.GetLength() || CellIndex_input == CellIndex_temp)
		{//attempt to add new face, not necessary			
			return CellIndex_temp;
		}
		else
		{// attempt to replace old&valid cell with new cell
			MDK_Error("Cell arealdy exists at CellIndex: " << CellIndex_temp << " @ Mesh::AddCellByFace(...)")
			return -1;
		}
	}

	//---------------------- input check is done ------------------------------------------------//

	//extract PointIndexList
	DenseVector<int_max> PointIndexList;
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		PointIndexList.Append(m_MeshData->FaceList[FaceIndexList[k]].PointIndexList());
	}
	PointIndexList = PointIndexList.GetSubSet(PointIndexList.FindUnique());

	// create cell ----------------------------------------------------------------------------------

	if (CellIndex_input == m_MeshData->CellList.GetLength())
	{
		CellType EmptyCell;
		m_MeshData->CellList.Append(std::move(EmptyCell));
		m_MeshData->CellValidityFlagList.Append(1);
	}
	else
	{
		m_MeshData->CellList[CellIndex_input].ReCreate();
		m_MeshData->CellValidityFlagList[CellIndex_input] = 1;
	}
	// take reference
	auto& Cell = m_MeshData->CellList[CellIndex_input];
	Cell.SetParentMesh(*this);
	Cell.SetIndex(CellIndex_input);
	Cell.SetType(Type);
	Cell.PointIndexList() = PointIndexList;
	Cell.FaceIndexList() = FaceIndexList;

	// update information in m_MeshData->FaceList ---------------------------------------------
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		m_MeshData->FaceList[FaceIndexList[k]].AdjacentCellIndexList().Append(CellIndex_input);
	}

	//------------
	
	for (int_max SetIndex = 0; SetIndex < m_MeshData->CellDataSet.GetLength(); ++SetIndex)
	{
		if (CellIndex_input == m_MeshData->CellList.GetLength())
		{
			DenseVector<ScalarType> Data;
			Data.Resize(m_MeshData->CellDataSet[SetIndex].GetRowCount());
			Data.Fill(0);
			m_MeshData->CellDataSet[SetIndex].AppendCol(Data);
		}
		else
		{
			m_MeshData->CellDataSet[SetIndex].FillCol(CellIndex_input, 0);
		}
	}
	//------------
	return CellIndex_input;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByFace(const DenseVector<int_max>& FaceIndexList)
{
	return this->AddCellByFace(MeshCellTypeEnum::Polyhedron, FaceIndexList);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByFace(const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input)
{
	return this->AddCellByFace(MeshCellTypeEnum::Polyhedron, FaceIndexList, CellIndex_input);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByPointAndFace(MeshCellTypeEnum Type, const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList)
{
	auto CellIndex = m_MeshData->CellList.GetLength();
	return this->AddCellByPointAndFace(Type, PointIndexList, FaceIndexList, CellIndex);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByPointAndFace(MeshCellTypeEnum Type, const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input)
{
	if (PointIndexList.GetLength() < 4)
	{
		MDK_Error("length of PointIndexList < 4 @ Mesh::AddCellByPointAndFace(...)")
		return -1;
	}

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			MDK_Error("Invalid PointIndexList @ Mesh::AddCellByPointAndFace(...)")
			return -1;
		}
	}

	auto tempList_unique = PointIndexList.FindUnique();
	if (tempList_unique.GetLength() != PointIndexList.GetLength())
	{
		MDK_Error("PointIndexList is invalid : more than one copy of a point index @ Mesh::AddCellByFace(...)")
		return -1;
	}

	//extract PointIndexList_from_face, and compare it to input PointIndexList
	DenseVector<int_max> PointIndexList_from_face;
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		PointIndexList_from_face.Append(m_MeshData->FaceList[FaceIndexList[k]].PointIndexList());
	}
	PointIndexList_from_face = PointIndexList_from_face.GetSubSet(PointIndexList_from_face.FindUnique());
	if (PointIndexList_from_face.GetLength() != PointIndexList.GetLength())
	{
		MDK_Error("Invalid PointIndexList @ Mesh::AddCellByPointAndFace(...)")
		return -1;
	}
	auto TempList = Intersect(PointIndexList_from_face, PointIndexList);
	if (TempList.GetLength() != PointIndexList.GetLength())
	{
		MDK_Error("Invalid PointIndexList @ Mesh::AddCellByPointAndFace(...)")
		return -1;
	}

	auto CellIndex_new = this->AddCellByFace(Type, FaceIndexList, CellIndex_input);
	m_MeshData->CellList[CellIndex_new].PointIndexList() = PointIndexList;
	//-----------------------------
	return CellIndex_new;

}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByPointAndFace(const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList)
{
	return this->AddCellByPointAndFace(MeshCellTypeEnum::Polyhedron, PointIndexList, FaceIndexList);
}

template<typename ScalarType>
int_max Mesh<ScalarType>::AddCellByPointAndFace(const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input)
{
	return this->AddCellByPointAndFace(MeshCellTypeEnum::Polyhedron, PointIndexList, FaceIndexList, CellIndex_input);
}

//------------------- Delete Mesh Item ----------------------------------------------------------------------------//

template<typename ScalarType>
void Mesh<ScalarType>::DeleteCell(int_max CellIndex)
{	// this function will modify any information related to the cell
	// CellIndex of the cell become invalid after the cell is deleted

	if (this->IsValidCellIndex(CellIndex) == false)
	{
		//MDK_Warning("Invalid CellIndex @ Mesh::DeleteCell(...)")
		return;
	}

	this->UpdateRecord_DeleteCell(CellIndex);
}

template<typename ScalarType>
void Mesh<ScalarType>::DeleteCell(const DenseVector<int_max>& CellIndexList)
{
	for (int_max k = 0; k < CellIndexList.GetLength(); ++k)
	{
		this->DeleteCell(CellIndexList[k]);
	}
}

template<typename ScalarType>
void Mesh<ScalarType>::DeleteFace(int_max FaceIndex)
{    // this function will modify any information related to the face
    // FaceIndex of the face become invalid after the face is deleted

    if (this->IsValidFaceIndex(FaceIndex) == false)
    {
        //MDK_Warning("Invalid FaceIndex @ Mesh::DeleteFace(...)")
        return;
    }

	auto AdjacentCellCount = m_MeshData->FaceList[FaceIndex].GetAdjacentCellCount();
	if (AdjacentCellCount > 0)
	{
		MDK_Error("AdjacentCellIndexList is not empty, so this face can not be deleted @ Mesh::DeleteFace(...)")
		return;
	}

    this->UpdateRecord_DeleteFace(FaceIndex);
}

template<typename ScalarType>
void Mesh<ScalarType>::DeleteFace(const DenseVector<int_max>& FaceIndexList)
{
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		this->DeleteFace(FaceIndexList[k]);
	}
}

template<typename ScalarType>
void Mesh<ScalarType>::DeleteEdge(int_max EdgeIndex)
{    
    if (this->IsValidEdgeIndex(EdgeIndex) == false)
    {
        //MDK_Warning("Invalid EdgeIndex @ Mesh::DeleteEdge(...)")
        return;
    }
    
    auto AdjacentFaceCount = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceCount();
    if (AdjacentFaceCount > 0)
    {
        MDK_Error("AdjacentFaceIndexList is not empty, so this edge can not be deleted @ Mesh::DeleteEdge(...)")
        return;
    }

    this->UpdateRecord_DeleteEdge(EdgeIndex);
}

template<typename ScalarType>
void Mesh<ScalarType>::DeleteEdge(const DenseVector<int_max>& EdgeIndexList)
{
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		this->DeleteEdge(EdgeIndexList[k]);
	}
}

template<typename ScalarType>
void Mesh<ScalarType>::DeletePoint(int_max PointIndex)
{
    if (this->IsValidPointIndex(PointIndex) == false)
    {
        //MDK_Warning("Invalid PointIndex @ Mesh::DeletePoint(...)")
        return;
    }

    if (m_MeshData->PointList[PointIndex].IsOnEdge() == true)
    {
        MDK_Error("The point is on an edge, so it can not be deleted @ Mesh::DeletePoint(...)")
        return;
    }

    this->UpdateRecord_DeletePoint(PointIndex);
}


template<typename ScalarType>
void Mesh<ScalarType>::DeletePoint(const DenseVector<int_max>& PointIndexList)
{
    for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
    {
		this->DeletePoint(PointIndexList[k]);
    }
}

//----------------- remove deleted item from Point/Edge/Face list ----------------------------------------------------------//
// attention: 
// after CleanDataStructure() is called, the size of each list will change, index may become invalid,
// but every valid ID will still be valid
// and there will be no "dead/deleted" item in any object list (e.g., m_MeshData->EdgeList)
// Only use CleanDataStructure() if memory is an issue, when DeletedPointIndexCount/ValidPointCount( GetPointCount() ) > 0.5

template<typename ScalarType>
void Mesh<ScalarType>::CleanDataStructure()
{
	DenseVector<int_max> PointIndexMap_Old_To_New;
	DenseVector<int_max> EdgeIndexMap_Old_To_New;
	DenseVector<int_max> FaceIndexMap_Old_To_New;
	DenseVector<int_max> CellIndexMap_Old_To_New;
	this->CleanDataStructure(PointIndexMap_Old_To_New, EdgeIndexMap_Old_To_New, FaceIndexMap_Old_To_New, CellIndexMap_Old_To_New);
}

template<typename ScalarType>
void Mesh<ScalarType>::CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New, DenseVector<int_max>& FaceIndexMap_Old_To_New)
{
	DenseVector<int_max> CellIndexMap_Old_To_New;
	this->CleanDataStructure(PointIndexMap_Old_To_New, EdgeIndexMap_Old_To_New, FaceIndexMap_Old_To_New, CellIndexMap_Old_To_New);
}

template<typename ScalarType>
void Mesh<ScalarType>::CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New,
   	                                      DenseVector<int_max>& FaceIndexMap_Old_To_New, DenseVector<int_max>& CellIndexMap_Old_To_New)
{
	if (this->GetDeletedPointCount() == 0 && this->GetDeletedEdgeCount() == 0 && this->GetDeletedFaceCount() == 0 && this->GetDeletedCellCount() == 0)
	{
		return;
	}

	//---------------- get IndexMap between old and new ---------------------------------------//
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

	CellIndexMap_Old_To_New.Clear();
	CellIndexMap_Old_To_New.Resize(m_MeshData->CellList.GetLength());
	DenseVector<int_max> CellIndexMap_New_To_Old;
	CellIndexMap_New_To_Old.SetCapacity(m_MeshData->CellList.GetLength());
	for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k) // k is CellIndex_old
	{
		CellIndexMap_Old_To_New[k] = -1;
		if (m_MeshData->CellValidityFlagList[k] == 1)
		{
			CellIndexMap_New_To_Old.Append(k);
			auto CellIndex_new = CellIndexMap_New_To_Old.GetLength() - 1;
			CellIndexMap_Old_To_New[k] = CellIndex_new;
		}
	}

	auto PointCount_old = PointIndexMap_Old_To_New.GetLength();// include deleted point
	auto PointCount_new = PointIndexMap_New_To_Old.GetLength();
	auto EdgeCount_old = EdgeIndexMap_Old_To_New.GetLength();// include deleted edge
	auto EdgeCount_new = EdgeIndexMap_New_To_Old.GetLength();
	auto FaceCount_old = FaceIndexMap_Old_To_New.GetLength();// include deleted face
	auto FaceCount_new = FaceIndexMap_New_To_Old.GetLength();
	auto CellCount_old = CellIndexMap_Old_To_New.GetLength();// include deleted cell
	auto CellCount_new = CellIndexMap_New_To_Old.GetLength();

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
		// regenerate Map_Point_Name_to_Index
		m_MeshData->Map_Point_Name_to_Index.clear();
		for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
		{
			auto PointName_k = m_MeshData->PointList[k].GetName();
			if (PointName_k.IsEmpty() == false)
			{
				m_MeshData->Map_Point_Name_to_Index[PointName_k] = k;
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
		// regenerate Map_Edge_Name_to_Index
		m_MeshData->Map_Edge_Name_to_Index.clear();
		for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
		{
			auto EdgeName_k = m_MeshData->EdgeList[k].GetName();
			if (EdgeName_k.IsEmpty() == false)
			{
				m_MeshData->Map_Edge_Name_to_Index[EdgeName_k] = k;
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
		// regenerate Map_Face_Name_to_Index
		m_MeshData->Map_Face_Name_to_Index.clear();
		for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
		{
			auto FaceName_k = m_MeshData->FaceList[k].GetName();
			if (FaceName_k.IsEmpty() == false)
			{
				m_MeshData->Map_Face_Name_to_Index[FaceName_k] = k;
			}
		}
	}

	if (CellCount_old != CellCount_new)
	{
		CellListType CellList_new;
		CellList_new.Resize(CellIndexMap_New_To_Old.GetLength());
		for (int_max k = 0; k < CellIndexMap_New_To_Old.GetLength(); ++k)
		{
			CellList_new[k] = std::move(m_MeshData->CellList[CellIndexMap_New_To_Old[k]]);
			CellList_new[k].SetIndex(k);
		}
		m_MeshData->CellList = std::move(CellList_new);
		m_MeshData->CellValidityFlagList.Resize(CellCount_new);
		m_MeshData->CellValidityFlagList.Fill(1);
		// regenerate Map_Cell_Name_to_Index
		m_MeshData->Map_Cell_Name_to_Index.clear();
		for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
		{
			auto CellName_k = m_MeshData->CellList[k].GetName();
			if (CellName_k.IsEmpty() == false)
			{
				m_MeshData->Map_Cell_Name_to_Index[CellName_k] = k;
			}
		}
	}

	//--------------------------- update adjacency information------------------------------------------------

	for (int_max CellIndex_new = 0; CellIndex_new < m_MeshData->CellList.GetLength(); ++CellIndex_new)
	{
		DenseVector<int_max> PointIndexList_old, PointIndexList_new;
		PointIndexList_old = std::move(m_MeshData->CellList[CellIndex_new].PointIndexList());
		for (int_max n = 0; n < PointIndexList_old.GetLength(); ++n)
		{
			auto PointIndex_new = PointIndexMap_Old_To_New[PointIndexList_old[n]];
			if (PointIndex_new >= 0)
			{
				PointIndexList_new.Append(PointIndex_new);
			}
		}
		m_MeshData->CellList[CellIndex_new].PointIndexList() = std::move(PointIndexList_new);

		DenseVector<int_max> FaceIndexList_old, FaceIndexList_new;
		FaceIndexList_old = std::move(m_MeshData->CellList[CellIndex_new].FaceIndexList());
		for (int_max n = 0; n < FaceIndexList_old.GetLength(); ++n)
		{
			auto FaceIndex_new = FaceIndexMap_Old_To_New[FaceIndexList_old[n]];
			if (FaceIndex_new >= 0)
			{
				FaceIndexList_new.Append(FaceIndex_new);
			}
		}
		m_MeshData->CellList[CellIndex_new].FaceIndexList() = std::move(FaceIndexList_new);
	}

	for (int_max FaceIndex_new = 0; FaceIndex_new < m_MeshData->FaceList.GetLength(); ++FaceIndex_new)
	{
		DenseVector<int_max> PointIndexList_old, PointIndexList_new;
		PointIndexList_old = std::move(m_MeshData->FaceList[FaceIndex_new].PointIndexList());
		for (int_max n = 0; n < PointIndexList_old.GetLength(); ++n)
		{
			auto PointIndex_new = PointIndexMap_Old_To_New[PointIndexList_old[n]];
			if (PointIndex_new >= 0)
			{
				PointIndexList_new.Append(PointIndex_new);
			}
		}		
		m_MeshData->FaceList[FaceIndex_new].PointIndexList() = std::move(PointIndexList_new);

		DenseVector<int_max> EdgeIndexList_old, EdgeIndexList_new;
		EdgeIndexList_old = std::move(m_MeshData->FaceList[FaceIndex_new].EdgeIndexList());
		for (int_max n = 0; n < EdgeIndexList_old.GetLength(); ++n)
		{
			auto EdgeIndex_new = EdgeIndexMap_Old_To_New[EdgeIndexList_old[n]];
			if (EdgeIndex_new >= 0)
			{
				EdgeIndexList_new.Append(EdgeIndex_new);
			}
		}
		m_MeshData->FaceList[FaceIndex_new].EdgeIndexList() = std::move(EdgeIndexList_new);

		DenseVector<int_max> AdjacentCellIndexList_old, AdjacentCellIndexList_new;
		AdjacentCellIndexList_old = std::move(m_MeshData->FaceList[FaceIndex_new].AdjacentCellIndexList());
		for (int_max n = 0; n < AdjacentCellIndexList_old.GetLength(); ++n)
		{
			auto Index_new = CellIndexMap_Old_To_New[AdjacentCellIndexList_old[n]];
			if (Index_new >= 0)
			{
				AdjacentCellIndexList_new.Append(Index_new);
			}
		}
		m_MeshData->FaceList[FaceIndex_new].AdjacentCellIndexList() = std::move(AdjacentCellIndexList_new);
	}

	for (auto EdgeIndex_new = 0; EdgeIndex_new < m_MeshData->EdgeList.GetLength(); ++EdgeIndex_new)
	{
		auto PointIndexList = m_MeshData->EdgeList[EdgeIndex_new].GetPointIndexList();
		PointIndexList[0] = PointIndexMap_Old_To_New[PointIndexList[0]];
		PointIndexList[1] = PointIndexMap_Old_To_New[PointIndexList[1]];
		m_MeshData->EdgeList[EdgeIndex_new].SetPointIndexList(PointIndexList);

		DenseVector<int_max> AdjacentFaceIndexList_old, AdjacentFaceIndexList_new;
		AdjacentFaceIndexList_old = std::move(m_MeshData->EdgeList[EdgeIndex_new].AdjacentFaceIndexList());
		for (int_max n = 0; n < AdjacentFaceIndexList_old.GetLength(); ++n)
		{
			auto Index_new = FaceIndexMap_Old_To_New[AdjacentFaceIndexList_old[n]];
			if (Index_new >= 0)
			{
				AdjacentFaceIndexList_new.Append(Index_new);
			}
		}
		m_MeshData->EdgeList[EdgeIndex_new].AdjacentFaceIndexList() = std::move(AdjacentFaceIndexList_new);
	}

	for (int_max PointIndex_new = 0; PointIndex_new < m_MeshData->PointList.GetLength(); ++PointIndex_new)
	{
		DenseVector<int_max> AdjacentEdgeIndexList_old, AdjacentEdgeIndexList_new;
		AdjacentEdgeIndexList_old = std::move(m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList());
		for (int_max n = 0; n < AdjacentEdgeIndexList_old.GetLength(); ++n)
		{
			auto Index_new = EdgeIndexMap_Old_To_New[AdjacentEdgeIndexList_old[n]];
			if (Index_new >= 0)
			{
				AdjacentEdgeIndexList_new.Append(Index_new);
			}
		}	
		m_MeshData->PointList[PointIndex_new].AdjacentEdgeIndexList() = std::move(AdjacentEdgeIndexList_new);
	}

	//---------------------- update PointDataSet/EdgeDataSet/FaceDataSet/CellDataSet -------------------------------

	for (int_max Index = 0; Index < m_MeshData->PointDataSet.GetLength(); ++Index)
	{
		DenseMatrix<ScalarType> DataSet_old, DataSet_new;
		DataSet_old = std::move(m_MeshData->PointDataSet[Index]);
		DataSet_new.Resize(DataSet_old.GetRowCount(), m_MeshData->PointList.GetLength());
		DataSet_new.Fill(0);
		int_max MaxIndexValue = (std::max)(DataSet_old.GetColCount(), PointIndexMap_Old_To_New.GetLength()) - 1;
		for (int_max PointIndex_old = 0; PointIndex_old <= MaxIndexValue; ++PointIndex_old)
		{
			auto PointIndex_new = PointIndexMap_Old_To_New[PointIndex_old];
			if (PointIndex_new >= 0)
			{
				DataSet_new.SetCol(PointIndex_new, DataSet_old.GetPointerOfCol(PointIndex_old));
			}
		}
		m_MeshData->PointDataSet[Index] = std::move(DataSet_new);
	}

	for (int_max Index = 0; Index < m_MeshData->EdgeDataSet.GetLength(); ++Index)
	{
		DenseMatrix<ScalarType> DataSet_old, DataSet_new;
		DataSet_old = std::move(m_MeshData->EdgeDataSet[Index]);
		DataSet_new.Resize(DataSet_old.GetRowCount(), m_MeshData->EdgeList.GetLength());
		DataSet_new.Fill(0);
		int_max MaxIndexValue = (std::max)(DataSet_old.GetColCount(), EdgeIndexMap_Old_To_New.GetLength()) - 1;
		for (int_max EdgeIndex_old = 0; EdgeIndex_old <= MaxIndexValue; ++EdgeIndex_old)
		{
			auto EdgeIndex_new = EdgeIndexMap_Old_To_New[EdgeIndex_old];
			if (EdgeIndex_new >= 0)
			{
				DataSet_new.SetCol(EdgeIndex_new, DataSet_old.GetPointerOfCol(EdgeIndex_old));
			}
		}
		m_MeshData->EdgeDataSet[Index] = std::move(DataSet_new);
	}

	for (int_max Index = 0; Index < m_MeshData->FaceDataSet.GetLength(); ++Index)
	{
		DenseMatrix<ScalarType> DataSet_old, DataSet_new;
		DataSet_old = std::move(m_MeshData->FaceDataSet[Index]);		
		DataSet_new.Resize(DataSet_old.GetRowCount(), m_MeshData->FaceList.GetLength());
		DataSet_new.Fill(0);
		int_max MaxIndexValue = (std::max)(DataSet_old.GetColCount(), FaceIndexMap_Old_To_New.GetLength()) - 1;
		for (int_max FaceIndex_old = 0; FaceIndex_old <= MaxIndexValue; ++FaceIndex_old)
		{
			auto FaceIndex_new = FaceIndexMap_Old_To_New[FaceIndex_old];
			if (FaceIndex_new >= 0)
			{
				DataSet_new.SetCol(FaceIndex_new, DataSet_old.GetPointerOfCol(FaceIndex_old));
			}
		}
		m_MeshData->FaceDataSet[Index] = std::move(DataSet_new);
	}

	for (int_max Index = 0; Index < m_MeshData->CellDataSet.GetLength(); ++Index)
	{
		DenseMatrix<ScalarType> DataSet_old, DataSet_new;
		DataSet_old = std::move(m_MeshData->CellDataSet[Index]);	
		DataSet_new.Resize(DataSet_old.GetRowCount(), m_MeshData->CellList.GetLength());
		DataSet_new.Fill(0);
		int_max MaxIndexValue = (std::max)(DataSet_old.GetColCount(), CellIndexMap_Old_To_New.GetLength()) - 1;
		for (int_max CellIndex_old = 0; CellIndex_old <= MaxIndexValue; ++CellIndex_old)
		{
			auto CellIndex_new = CellIndexMap_Old_To_New[CellIndex_old];
			if (CellIndex_new >= 0)
			{
				DataSet_new.SetCol(CellIndex_new, DataSet_old.GetPointerOfCol(CellIndex_old));
			}
		}
		m_MeshData->CellDataSet[Index] = std::move(DataSet_new);
	}

	//------------------------------------ update PointSet/EdgeSet/FaceSet/CellSet -----------------------------------------

	for (int_max k = 0; k < m_MeshData->PointSet.GetLength(); ++k)
	{
		DenseVector<int_max> PointSet_old, PointSet_new;
		PointSet_old = std::move(m_MeshData->PointSet[k]);
		for (int_max n = 0; n < PointSet_old.GetLength(); ++n)
		{
			auto PointIndex_old = PointSet_old[n];
			auto PointIndex_new = PointIndexMap_Old_To_New[PointIndex_old];
			if (PointIndex_new >= 0)
			{
				PointSet_new.Append(PointIndex_new);
			}
		}
		m_MeshData->PointSet[k] = std::move(PointSet_new);
	}

	for (int_max k = 0; k < m_MeshData->EdgeSet.GetLength(); ++k)
	{
		DenseVector<int_max> EdgeSet_old, EdgeSet_new;
		EdgeSet_old = std::move(m_MeshData->EdgeSet[k]);
		for (int_max n = 0; n < EdgeSet_old.GetLength(); ++n)
		{
			auto EdgeIndex_old = EdgeSet_old[n];
			auto EdgeIndex_new = EdgeIndexMap_Old_To_New[EdgeIndex_old];
			if (EdgeIndex_new >= 0)
			{
				EdgeSet_new.Append(EdgeIndex_new);
			}
		}
		m_MeshData->EdgeSet[k] = std::move(EdgeSet_new);
	}

	for (int_max k = 0; k < m_MeshData->FaceSet.GetLength(); ++k)
	{
		DenseVector<int_max> FaceSet_old, FaceSet_new;
		FaceSet_old = std::move(m_MeshData->FaceSet[k]);
		for (int_max n = 0; n < FaceSet_old.GetLength(); ++n)
		{
			auto FaceIndex_old = FaceSet_old[n];
			auto FaceIndex_new = FaceIndexMap_Old_To_New[FaceIndex_old];
			if (FaceIndex_new >= 0)
			{
				FaceSet_new.Append(FaceIndex_new);
			}
		}
		m_MeshData->FaceSet[k] = std::move(FaceSet_new);
	}

	for (int_max k = 0; k < m_MeshData->CellSet.GetLength(); ++k)
	{
		DenseVector<int_max> CellSet_old, CellSet_new;
		CellSet_old = std::move(m_MeshData->CellSet[k]);
		for (int_max n = 0; n < CellSet_old.GetLength(); ++n)
		{
			auto CellIndex_old = CellSet_old[n];
			auto CellIndex_new = CellIndexMap_Old_To_New[CellIndex_old];
			if (CellIndex_new >= 0)
			{
				CellSet_new.Append(CellIndex_new);
			}
		}
		m_MeshData->CellSet[k] = std::move(CellSet_new);
	}
}


template<typename ScalarType>
bool Mesh<ScalarType>::Check_If_DataStructure_is_Clean() const
{
	auto Count1 = this->GetDeletedPointCount();
	auto Count2 = this->GetDeletedEdgeCount();
	auto Count3 = this->GetDeletedFaceCount();
	return (Count1 + Count2 + Count3 == 0);
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetDeletedPointCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_MeshData->PointValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->PointValidityFlagList[k] == 0)
		{
			Counter += 1;
		}
	}
	return Counter;
}


template<typename ScalarType>
int_max Mesh<ScalarType>::GetDeletedEdgeCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_MeshData->EdgeValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->EdgeValidityFlagList[k] == 0)
		{
			Counter += 1;
		}
	}
	return Counter;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetDeletedFaceCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_MeshData->FaceValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->FaceValidityFlagList[k] == 0)
		{
			Counter += 1;
		}
	}
	return Counter;
}

template<typename ScalarType>
int_max Mesh<ScalarType>::GetDeletedCellCount() const
{
	int_max Counter = 0;
	for (int_max k = 0; k < m_MeshData->CellValidityFlagList.GetLength(); ++k)
	{
		if (m_MeshData->CellValidityFlagList[k] == 0)
		{
			Counter += 1;
		}
	}
	return Counter;
}

template<typename ScalarType>
std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Mesh<ScalarType>::GetPointPositionMatrixAndFaceTable(bool Flag_Clean) const
{
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Output;
	this->GetPointPositionMatrixAndFaceTable(Output.first, Output.second, Flag_Clean);
	return Output;
}

template<typename ScalarType>
void Mesh<ScalarType>::GetPointPositionMatrixAndFaceTable(DenseMatrix<ScalarType>& PointPositionMatrix, ObjectArray<DenseVector<int_max>>& FaceTable, bool Flag_Clean) const
{
	if (Flag_Clean == false)
	{
		PointPositionMatrix = m_MeshData->PointPositionTable;
		FaceTable.FastResize(m_MeshData->FaceList.GetLength());
		for (int_max n = 0; n <= m_MeshData->FaceList.GetLength(); ++n)
		{
			if (this->IsValidFaceIndex(n) == true)
			{
				FaceTable[n] = this->Face(n).GetPointIndexList();
			}
		}
		return;
	}

	auto PointCount = this->GetPointCount();
	auto FaceCount = this->GetFaceCount();
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();

	PointPositionMatrix.FastResize(3, PointCount);
	FaceTable.FastResize(FaceCount);

	// DataStructure may not be clean
	// Map PointIndex (PointIndex.GetIndex()) to OutputIndex (col index) in PointPositionMatrix
	std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex;

	int_max PointCounter = 0;
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			auto Pos = this->GetPointPosition(k);
			PointPositionMatrix.SetCol(PointCounter, Pos);
			Map_PointIndex_to_OutputIndex[k] = PointCounter;
			PointCounter += 1;
		}
	}

	int_max FaceCounter = 0;
	for (int_max n=0; n<= FaceIndex_max; ++n)
	{
		if (this->IsValidFaceIndex(n) == true)
		{
			auto PointIndexList = this->Face(n).GetPointIndexList();
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
					MDK_Error("tempPointIndex is invalid @ Mesh::GetPointPositionMatrixAndFaceTable(...)")
					return;
				}
			}
			FaceCounter += 1;
		}
	}
}


template<typename ScalarType>
void Mesh<ScalarType>::Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable)
{
	if (InputPointPositionMatrix.IsEmpty() == true || InputFaceTable.IsEmpty() == true)
	{
		MDK_Error("InputPointPositionMatrix or InputFaceTable is empty @ Mesh::Construct(...)")
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
			MDK_Error("Somthing is wrong with PointIndexList @ Mesh::Construct(...)")
			return;
		}
	}

	for (int_max k = 0; k < InputFaceTable.GetLength(); ++k)
	{
		auto PointIndexList_k = PointIndexList.GetSubSet(InputFaceTable[k]);
		if (PointIndexList_k.IsEmpty() == false)
		{
			this->AddFaceByPoint(PointIndexList_k);
		}
		else
		{
			MDK_Warning("PointIndexList is empty at Face " << k << ", the Face is ignored")
		}
	}
}



template<typename ScalarType>
std::pair<DenseVector<int_max>, DenseVector<int_max>> Mesh<ScalarType>::GetNeighborPointOfPoint(int_max PointIndex_ref, int_max MaxGraphDistance) const
{
	std::pair<DenseVector<int_max>, DenseVector<int_max>> Output;
	//Output.first[k] is PointIndex
	//Output.second[k] is GraphDistance between Point(PointIndex) and Point(PointIndex_ref)

	int_max PointCount = this->GetPointCount();

	//--------------- check input ------------------------//
	if (PointIndex_ref < 0 || PointIndex_ref >= PointCount)
	{
		return Output;
	}

	if (this->IsEmpty() == true)
	{
		return Output;
	}

	if (MaxGraphDistance <= 0)
	{
		return Output;
	}
	else if (MaxGraphDistance == 1)
	{
		auto AdjPointIndexList = this->Point(PointIndex_ref).GetAdjacentPointIndexList();
		Output.first.Resize(AdjPointIndexList.GetLength());
		Output.second.Resize(AdjPointIndexList.GetLength());
		for (int_max k = 0; k < AdjPointIndexList.GetLength(); ++k)
		{
			Output.first[k] = AdjPointIndexList[k];
			Output.second[k] = 1;
			return Output;
		}
	}
	//----------------------------------------------------//

	DenseVector<int8> PointFlagList;
	PointFlagList.Resize(PointCount);
	PointFlagList.Fill(0); // 0 ~ not selected, 1 ~ selected as ouput

	DenseVector<int_max> PointIndexList_boarder;
	PointIndexList_boarder = this->Point(PointIndex_ref).GetAdjacentPointIndexList();

	for (int_max k = 0; k < PointIndexList_boarder.GetLength(); ++k)
	{
		Output.first.Append(PointIndexList_boarder[k]);
		Output.second.Append(1);
		PointFlagList[PointIndexList_boarder[k]] = 1;
	}

	for (int_max GraphDistance = 2; GraphDistance <= MaxGraphDistance; ++GraphDistance)
	{
		DenseVector<int_max> PointIndexList_boarder_next;
		PointIndexList_boarder_next.SetCapacity(PointIndexList_boarder.GetLength() + 100);
		for (int_max k = 0; k < PointIndexList_boarder.GetLength(); ++k)
		{
			auto AdjPointIndexList = this->Point(PointIndexList_boarder[k]).GetAdjacentPointIndexList();
			for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
			{
				if (PointFlagList[AdjPointIndexList[n]] == 0)
				{
					Output.first.Append(AdjPointIndexList[k]);
					Output.second.Append(GraphDistance);					
					PointIndexList_boarder_next.Append(AdjPointIndexList[n]);
					PointFlagList[AdjPointIndexList[n]] = 1;
				}
			}
		}
		PointIndexList_boarder = std::move(PointIndexList_boarder_next);
	}

	return Output;
}

//-------------------- get a sub mesh by FaceIndexList  -----------------------------------------//

template<typename ScalarType>
Mesh<ScalarType> 
Mesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
    Mesh<ScalarType> OutputMesh;
    
    if (FaceIndexList.IsEmpty() == true)
    {
        return OutputMesh;
    }

    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ Mesh::GetSubMeshByFace(...)")
        return OutputMesh;
    }

    for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
    {
        if (this->IsValidFaceIndex(FaceIndexList[k]) == false)
        {
            MDK_Error("FaceIndexList is invalid @ Mesh::GetSubMeshByFace(...)")
            return OutputMesh;
        }
    }
    //-----------------------------------------------------------

    // point index on this mesh -> point index on output mesh
	DenseVector<int_max> PointIndexMap_In_Out;
	PointIndexMap_In_Out.Resize(m_MeshData->PointList.GetLength());
	PointIndexMap_In_Out.Fill(-1);
    
	// edge index on this mesh -> edge index on output mesh	
	DenseVector<int_max> EdgeIndexMap_In_Out;
	EdgeIndexMap_In_Out.Resize(m_MeshData->EdgeList.GetLength());
	EdgeIndexMap_In_Out.Fill(-1);

    // add face one by one
    for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
    {
		auto FaceName_k= this->Face(FaceIndexList[k]).GetName();
        const auto& FaceAttribute_k = this->Face(FaceIndexList[k]).Attribute();
        auto PointIndexList_k = this->Face(FaceIndexList[k]).GetPointIndexList();
        auto EdgeIndexList_k = this->Face(FaceIndexList[k]).GetEdgeIndexList();

        // add point
        DenseVector<int_max> PointIndexList_OutputFace;
		PointIndexList_OutputFace.Resize(PointIndexList_k.GetLength());
        for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
        {
            auto PointIndex_n = PointIndexList_k[n];
			auto PointName_n = this->Point(PointIndex_n).GetName();
            const auto& PointAttribute_n = this->Point(PointIndex_n).Attribute();
			auto Position_n = this->Point(PointIndex_n).GetPosition();

            // check if the point has already been added to OutputMesh			
			if (PointIndexMap_In_Out[PointIndex_n] < 0)
            {        
                // add Point Position
				PointIndexList_OutputFace[n] = OutputMesh.AddPoint(Position_n);               
				// copy PointName if it is valid
				if (PointName_n.IsEmpty() == false)
				{
					OutputMesh.Point(PointIndexList_OutputFace[n]).SetName(PointName_n);
				}
                // copy Attribute
				OutputMesh.Point(PointIndexList_OutputFace[n]).Attribute() = PointAttribute_n;
                // add point to map
				PointIndexMap_In_Out[PointIndex_n] = PointIndexList_OutputFace[n];
            }
            else
            {
				PointIndexList_OutputFace[n] = PointIndexMap_In_Out[PointIndex_n];
            }
        }

        // add face, FaceIndex_Output is k
		auto FaceIndex_Output = OutputMesh.AddFaceByPoint(PointIndexList_OutputFace);
		// copy face Name if it is valid
		if (FaceName_k.IsEmpty() == false)
		{
			OutputMesh.Face(FaceIndex_Output).SetName(FaceName_k);
		}
        // copy Attribute
		OutputMesh.Face(FaceIndex_Output).Attribute() = FaceAttribute_k;

        // copy Edge Name and Attribute
		auto EdgeIndexList_Output = OutputMesh.Face(FaceIndex_Output).GetEdgeIndexList();
        for (int_max n = 0; n < EdgeIndexList_k.GetLength(); ++n)
        {
            // check if the edge has already been added to OutputMesh			
			if (EdgeIndexMap_In_Out[EdgeIndexList_k[n]] < 0)
            {
				auto EdgeName_n = this->Edge(EdgeIndexList_k[n]).GetName();
				if (EdgeName_n.IsEmpty() == false)
				{
					OutputMesh.Edge(EdgeIndexList_Output[n]).SetName(EdgeName_n);
				}

				const auto& EdgeAttribute_n = this->Edge(EdgeIndexList_k[n]).Attribute();
				OutputMesh.Edge(EdgeIndexList_Output[n]).Attribute() = EdgeAttribute_n;

				EdgeIndexMap_In_Out[EdgeIndexList_k[n]] = EdgeIndexList_Output[n];
            }
        }
    }

	//add PointDataSet, EdgeDataSet, FaceDataset ---------------------------
	if (this->GetPointDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->PointDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->PointDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;			
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->PointList.GetLength());
			for (int_max PointIndex_in = 0; PointIndex_in < PointIndexMap_In_Out.GetLength(); ++PointIndex_in)
			{
				auto PointIndex_out = PointIndexMap_In_Out[PointIndex_in];
				if (PointIndex_out >= 0)
				{
					DataSet_out.SetCol(PointIndex_out, DataSet_in.GetPointerOfCol(PointIndex_in));
				}
			}
			OutputMesh.m_MeshData->PointDataSet[Index] = std::move(DataSet_out);
		}
	}

	if (this->GetEdgeDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->EdgeDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->EdgeDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->EdgeList.GetLength());
			for (int_max EdgeIndex_in = 0; EdgeIndex_in < EdgeIndexMap_In_Out.GetLength(); ++EdgeIndex_in)
			{
				auto EdgeIndex_out = EdgeIndexMap_In_Out[EdgeIndex_in];
				if (EdgeIndex_out >= 0)
				{
					DataSet_out.SetCol(EdgeIndex_out, DataSet_in.GetPointerOfCol(EdgeIndex_in));
				}
			}
			OutputMesh.m_MeshData->EdgeDataSet[Index] = std::move(DataSet_out);
		}
	}

	if (this->GetFaceDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->FaceDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->FaceDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->FaceList.GetLength());
			for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
			{
				auto FaceIndex_in = FaceIndexList[k];
				auto FaceIndex_out = k;
				DataSet_out.SetCol(FaceIndex_out, DataSet_in.GetPointerOfCol(FaceIndex_in));
			}
			OutputMesh.m_MeshData->FaceDataSet[Index] = std::move(DataSet_out);
		}
	}

    return OutputMesh;
}

template<typename ScalarType>
Mesh<ScalarType> Mesh<ScalarType>::GetSubMeshByCell(const DenseVector<int_max>& CellIndexList) const
{
	Mesh<ScalarType> OutputMesh;

	if (FaceIndexList.IsEmpty() == true)
	{
		return OutputMesh;
	}

	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ Mesh::GetSubMeshByCell(...)")
		return OutputMesh;
	}

	for (int_max k = 0; k < CellIndexList.GetLength(); ++k)
	{
		if (this->IsValidCellIndex(CellIndexList[k]) == false)
		{
			MDK_Error("CellIndexList is invalid @ Mesh::GetSubMeshByCell(...)")
			return OutputMesh;
		}
	}
	//-----------------------------------------------------------

	// point index on this mesh -> point index on output mesh
	DenseVector<int_max> PointIndexMap_In_Out;
	PointIndexMap_In_Out.Resize(m_MeshData->PointList.GetLength());
	PointIndexMap_In_Out.Fill(-1);

	// edge index on this mesh -> edge index on output mesh	
	DenseVector<int_max> EdgeIndexMap_In_Out;
	EdgeIndexMap_In_Out.Resize(m_MeshData->EdgeList.GetLength());
	EdgeIndexMap_In_Out.Fill(-1);

	// face index on this mesh -> face index on output mesh	
	DenseVector<int_max> FaceIndexMap_In_Out;
	FaceIndexMap_In_Out.Resize(m_MeshData->FaceList.GetLength());
	FaceIndexMap_In_Out.Fill(-1);

	// add cell one by one
	for (int_max k = 0; k < CellIndexList.GetLength(); ++k)
	{
		auto CellName_k = this->Cell(CellIndexList[k]).GetName();
		const auto& CellAttribute_k = this->Cell(CellIndexList[k]).Attribute();
		auto PointIndexList_k = this->Cell(CellIndexList[k]).GetPointIndexList();
		auto FaceIndexList_k = this->Cell(CellIndexList[k]).GetFaceIndexList();
		//add point
		DenseVector<int_max> PointIndexList_OutputCell;
		PointIndexList_OutputCell.Resize(PointIndexList_k.GetLength());
		for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
		{
			auto PointIndex_n = PointIndexList_k[n];
			auto PointName_n = this->Point(PointIndex_n).GetName();
			const auto& PointAttribute_n = this->Point(PointIndex_n).Attribute();
			auto Position_n = this->Point(PointIndex_n).GetPosition();

			// check if the point has already been added to OutputMesh			
			if (PointIndexMap_In_Out[PointIndex_n] < 0)
			{
				// add Point Position
				PointIndexList_OutputCell[n] = OutputMesh.AddPoint(Position_n);
				// copy PointName if it is valid
				if (PointName_n.IsEmpty() == false)
				{
					OutputMesh.Point(PointIndexList_OutputCell[n]).SetName(PointName_n);
				}
				// copy Attribute
				OutputMesh.Point(PointIndexList_OutputCell[n]).Attribute() = PointAttribute_n;
				// add point to map
				PointIndexMap_In_Out[PointIndex_n] = PointIndexList_OutputCell[n];
			}
			else
			{
				PointIndexList_OutputCell[n] = PointIndexMap_In_Out[PointIndex_n];
			}
		}
		//add face
		DenseVector<int_max> FaceIndexList_OutputCell;
		FaceIndexList_OutputCell.Resize(FaceIndexList_k.GetLength());
		for (int_max m = 0; m < FaceIndexList_k.GetLength(); ++m)
		{
			auto FaceName_m = this->Face(FaceIndexList_k[m]).GetName();
			const auto& FaceAttribute_m = this->Face(FaceIndexList_k[m]).Attribute();
			auto PointIndexList_m = this->Face(FaceIndexList_k[m]).GetPointIndexList();
			auto EdgeIndexList_m = this->Face(FaceIndexList_k[m]).GetEdgeIndexList();			

			DenseVector<int_max> PointIndexList_OutputFace;
			PointIndexList_OutputFace.Resize(PointIndexList_m.GetLength());
			for (int_max n = 0; n < PointIndexList_m.GetLength(); ++n)
			{
				auto PointIndex_n = PointIndexList_m[n];
				// check if the point has already been added to OutputMesh			
				if (PointIndexMap_In_Out[PointIndex_n] < 0)
				{
					MDK_Error("Something is wrong @ Mesh::GetSubMeshByCell(...)")
				}
				else
				{
					PointIndexList_OutputFace[n] = PointIndexMap_In_Out[PointIndex_n];
				}
			}
			
			auto FaceIndex_Output = OutputMesh.AddFaceByPoint(PointIndexList_OutputFace);
			FaceIndexList_OutputCell[m] = FaceIndex_Output;
			// copy face Name if it is valid
			if (FaceName_m.IsEmpty() == false)
			{
				OutputMesh.Face(FaceIndex_Output).SetName(FaceName_m);
			}
			// copy Attribute
			OutputMesh.Face(FaceIndex_Output).Attribute() = FaceAttribute_m;

			// copy Edge Name and Attribute
			auto EdgeIndexList_Output = OutputMesh.Face(FaceIndex_Output).GetEdgeIndexList();
			for (int_max n = 0; n < EdgeIndexList_m.GetLength(); ++n)
			{
				// check if the edge has already been added to OutputMesh			
				if (EdgeIndexMap_In_Out[EdgeIndexList_m[n]] < 0)
				{
					auto EdgeName_n = this->Edge(EdgeIndexList_m[n]).GetName();
					if (EdgeName_n.IsEmpty() == false)
					{
						OutputMesh.Edge(EdgeIndexList_Output[n]).SetName(EdgeName_n);
					}

					const auto& EdgeAttribute_n = this->Edge(EdgeIndexList_m[n]).Attribute();
					OutputMesh.Edge(EdgeIndexList_Output[n]).Attribute() = EdgeAttribute_n;

					EdgeIndexMap_In_Out[EdgeIndexList_m[n]] = EdgeIndexList_Output[n];
				}
			}
		}
		//add cell, CellIndex_Output is k
		auto CellIndex_Output = OutputMesh.AddCellByPointAndFace(PointIndexList_OutputCell, FaceIndexList_OutputCell);
		OutputMesh.Cell(CellIndex_Output).Attribute() = CellAttribute_k;
	}

	//add PointDataSet, EdgeDataSet, FaceDataset, CellDataSet --------------------------------------

	if (this->GetPointDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->PointDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->PointDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->PointList.GetLength());
			for (int_max PointIndex_in = 0; PointIndex_in < PointIndexMap_In_Out.GetLength(); ++PointIndex_in)
			{
				auto PointIndex_out = PointIndexMap_In_Out[PointIndex_in];
				if (PointIndex_out >= 0)
				{
					DataSet_out.SetCol(PointIndex_out, DataSet_in.GetPointerOfCol(PointIndex_in));
				}
			}
			OutputMesh.m_MeshData->PointDataSet[Index] = std::move(DataSet_out);
		}
	}

	if (this->GetEdgeDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->EdgeDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->EdgeDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->EdgeList.GetLength());
			for (int_max EdgeIndex_in = 0; EdgeIndex_in < EdgeIndexMap_In_Out.GetLength(); ++EdgeIndex_in)
			{
				auto EdgeIndex_out = EdgeIndexMap_In_Out[EdgeIndex_in];
				if (EdgeIndex_out >= 0)
				{
					DataSet_out.SetCol(EdgeIndex_out, DataSet_in.GetPointerOfCol(EdgeIndex_in));
				}
			}
			OutputMesh.m_MeshData->EdgeDataSet[Index] = std::move(DataSet_out);
		}
	}

	if (this->GetFaceDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->FaceDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->FaceDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->FaceList.GetLength());
			for (int_max FaceIndex_in = 0; FaceIndex_in < FaceIndexMap_In_Out.GetLength(); ++FaceIndex_in)
			{
				auto FaceIndex_out = FaceIndexMap_In_Out[FaceIndex_in];
				if (FaceIndex_out >= 0)
				{
					DataSet_out.SetCol(FaceIndex_out, DataSet_in.GetPointerOfCol(FaceIndex_in));
				}
			}
			OutputMesh.m_MeshData->FaceDataSet[Index] = std::move(DataSet_out);
		}
	}

	if (this->GetCellDataSetCount() > 0)
	{
		for (int_max Index = 0; Index < m_MeshData->CellDataSet.GetLength(); ++Index)
		{
			const auto& DataSet_in = m_MeshData->CellDataSet[Index];
			DenseMatrix<ScalarType> DataSet_out;
			DataSet_out.Resize(DataSet_in.GetRowCount(), OutputMesh.m_MeshData->CellList.GetLength());
			for (int_max k = 0; k < CellIndexList.GetLength(); ++k)
			{
				auto CellIndex_in = CellIndexList[k];
				auto CellIndex_out = k;
				DataSet_out.SetCol(CellIndex_out, DataSet_in.GetPointerOfCol(CellIndex_in));
			}
			OutputMesh.m_MeshData->CellDataSet[Index] = std::move(DataSet_out);
		}
	}

	return OutputMesh;
}

template<typename ScalarType>
void Mesh<ScalarType>::Append(const Mesh<ScalarType>& InputMesh)
{// append a mesh, duplication of face/edge/point will not be checked
// this is not checked: append non-triangle mesh to triangle mesh
    //prevent self append
	if (&m_MeshData == &InputMesh.m_MeshData)
	{
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Recreate();
	}

	if (this->IsEmpty() == true)
	{
		if (InputMesh.IsEmpty() == false)
		{
			this->Copy(InputMesh);
		}
		return;
	}

	//no change on Name
	//m_MeshData->Name

	auto MaxPointIndex_init = m_MeshData->PointList.GetLength() - 1;
	auto MaxEdgeIndex_init = m_MeshData->EdgeList.GetLength() - 1;
	auto MaxFaceIndex_init = m_MeshData->FaceList.GetLength() - 1;
	auto MaxCellIndex_init = m_MeshData->CellList.GetLength() - 1;

	if (InputMesh.m_MeshData->PointList.IsEmpty() == false)
	{
		m_MeshData->PointPositionTable = { &m_MeshData->PointPositionTable, &InputMesh.m_MeshData->PointPositionTable };
		m_MeshData->PointValidityFlagList.Append(InputMesh.m_MeshData->PointValidityFlagList);
		m_MeshData->PointList.Insert(MaxPointIndex_init + 1, InputMesh.m_MeshData->PointList);
		for (int_max k = 0; k < m_MeshData->PointList.GetLength(); ++k)
		{
			if (k > MaxPointIndex_init)
			{
				if (m_MeshData->PointValidityFlagList[k] == 1)
				{
					m_MeshData->PointList[k].SetParentMesh(*this);
					m_MeshData->PointList[k].SetIndex(k);
					m_MeshData->PointList[k].AdjacentEdgeIndexList() += MaxEdgeIndex_init + 1;
				}
			}
		}
	}

	if (InputMesh.m_MeshData->EdgeList.IsEmpty() == false)
	{
		m_MeshData->EdgeValidityFlagList.Append(InputMesh.m_MeshData->EdgeValidityFlagList);
		m_MeshData->EdgeList.Insert(MaxEdgeIndex_init + 1, InputMesh.m_MeshData->EdgeList);
		for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
		{
			if (k > MaxEdgeIndex_init)
			{
				if (m_MeshData->EdgeValidityFlagList[k] == 1)
				{
					m_MeshData->EdgeList[k].SetParentMesh(*this);
					m_MeshData->EdgeList[k].SetIndex(k);
					auto PIdxList = m_MeshData->EdgeList[k].GetPointIndexList();
					m_MeshData->EdgeList[k].SetPointIndexList(PIdxList[0] + MaxPointIndex_init + 1, PIdxList[1] + MaxPointIndex_init + 1);
					m_MeshData->EdgeList[k].AdjacentFaceIndexList() += MaxFaceIndex_init + 1;
				}
			}
		}
	}

	if (InputMesh.m_MeshData->FaceList.IsEmpty() == false)
	{
		m_MeshData->FaceValidityFlagList.Append(InputMesh.m_MeshData->FaceValidityFlagList);
		m_MeshData->FaceList.Insert(MaxFaceIndex_init + 1, InputMesh.m_MeshData->FaceList);
		for (int_max k = 0; k < m_MeshData->FaceList.GetLength(); ++k)
		{
			if (k > MaxFaceIndex_init)
			{
				if (m_MeshData->FaceValidityFlagList[k] == 1)
				{
					m_MeshData->FaceList[k].SetParentMesh(*this);
					m_MeshData->FaceList[k].SetIndex(k);
					m_MeshData->FaceList[k].PointIndexList() += MaxPointIndex_init + 1;
					m_MeshData->FaceList[k].EdgeIndexList() += MaxEdgeIndex_init + 1;
				}
			}
		}
	}

	if (InputMesh.m_MeshData->CellList.IsEmpty() == false)
	{
		m_MeshData->CellValidityFlagList.Append(InputMesh.m_MeshData->CellValidityFlagList);
		m_MeshData->CellList.Insert(MaxCellIndex_init + 1, InputMesh.m_MeshData->CellList);
		for (int_max k = 0; k < m_MeshData->CellList.GetLength(); ++k)
		{
			if (k > MaxCellIndex_init)
			{
				if (m_MeshData->CellValidityFlagList[k] == 1)
				{
					m_MeshData->CellList[k].SetParentMesh(*this);
					m_MeshData->CellList[k].SetIndex(k);
					m_MeshData->CellList[k].PointIndexList() += MaxPointIndex_init + 1;
					m_MeshData->CellList[k].FaceIndexList() += MaxFaceIndex_init + 1;
				}
			}
		}
	}

	//TODO:
	//m_MeshData->Map_Point_Name_to_Index
	//m_MeshData->Map_Edge_Name_to_Index
	//m_MeshData->Map_Face_Name_to_Index
	//
	//m_MeshData->PointSet
	//m_MeshData->Map_PointSet_Name_to_Index
	//
	//m_MeshData->EdgeSet
	//m_MeshData->Map_EdgeSet_Name_to_Index
	//
	//m_MeshData->FaceSet
	//m_MeshData->Map_FaceSet_Name_to_Index
	//
	//PointDataSet/EgeDataSet/FaceDataSet/PointDataSet

	//no change
	//m_MeshData->Attribute
}


// other basic operation ----------------------------------------------------------------------------------------

template<typename ScalarType>
bool Mesh<ScalarType>::SwapPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::SwapPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::SwapPoint(...)")
		return false;
	}

	//swap Position
	auto PositionA = m_MeshData->PointList[PointIndexA].GetPosition();
	auto PositionB = m_MeshData->PointList[PointIndexB].GetPosition();
	m_MeshData->PointList[PointIndexA].SetPosition(PositionB);
	m_MeshData->PointList[PointIndexB].SetPosition(PositionA);

	return this->SwapConnectivityOfPoint(PointIndexA, PointIndexB);
}


template<typename ScalarType>
bool Mesh<ScalarType>::SwapConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::SwapConnectivityOfPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::SwapConnectivityOfPoint(...)")
		return false;
	}

	const auto& AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
	const auto& AdjacentFaceIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentFaceIndexList();
	//to be updated
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();

	const auto& AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
	const auto& AdjacentFaceIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentFaceIndexList();
	//to be updated
	auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();

	//---------------- update adjacent point, edge ----------------------------------------------------

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

	//change PointIndexA -> PointIndexB for Face	
	for (int_max k = 0; k < AdjacentFaceIndexListA.GetLength(); ++k)
	{   //take reference
		auto& PointIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexListA[k]].PointIndexList();
		int_max TempIndex = PointIndexList_k.ExactMatch("first", PointIndexA);
		PointIndexList_k[TempIndex] = PointIndexB;
	}

	//change PointIndexB -> PointIndexA for Face	
	for (int_max k = 0; k < AdjacentFaceIndexListB.GetLength(); ++k)
	{   //take reference
		auto& PointIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexListB[k]].PointIndexList();
		int_max TempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
		PointIndexList_k[TempIndex] = PointIndexA;
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


template<typename ScalarType>
bool Mesh<ScalarType>::MergeConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle)
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

	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::MergeConnectivityOfPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::MergeConnectivityOfPoint(...)")		
		return false;
	}
	//triangle check
	if (Flag_CheckTriangle == true)
	{// A and B may belong to the same triangle face
		auto AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListAB = Intersect(AdjacentPointIndexListA, AdjacentPointIndexListB);
		if (AdjacentPointIndexListAB.IsEmpty() == false)
		{
			MDK_Error("Can not merge: triangle detected @ Mesh::MergeConnectivityOfPoint(...)")
			return false;
		}
	}

	//------- delete Edge AB if it exist --------------------------------------------------------------------

	auto EdgeIndex_AB = this->GetEdgeIndexByPoint(PointIndexA, PointIndexB);
	if (EdgeIndex_AB >= 0)
	{
		auto AdjacentFaceIndexList_AB = m_MeshData->EdgeList[EdgeIndex_AB].GetAdjacentFaceIndexList();
		for (int_max k = 0; k < AdjacentFaceIndexList_AB.GetLength(); ++k)
		{
			auto FaceIndex_k = AdjacentFaceIndexList_AB[k];
			auto PointCount_k = m_MeshData->FaceList[FaceIndex_k].GetPointCount();
			// remove EdgeAB and PointB in the Face
			//take reference
			auto& EdgeIndexList_k = m_MeshData->FaceList[FaceIndex_k].EdgeIndexList();
			auto tempIndex_edge = EdgeIndexList_k.ExactMatch("first", EdgeIndex_AB);
			EdgeIndexList_k.Delete(tempIndex_edge);
			//take reference
			auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
			auto tempIndex_point = PointIndexList_k.ExactMatch("first", PointIndexB);
			PointIndexList_k.Delete(tempIndex_point);

		}
		// delete edge AB
		this->UpdateRecord_DeleteEdge(EdgeIndex_AB);
	}

	//-------------------------- Merge connection of B to connection of A ------------------------------------

	//update face adjacent to B
	auto AdjacentFaceIndexListB= m_MeshData->PointList[PointIndexB].GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjacentFaceIndexListB.GetLength(); ++k)
	{
		auto FaceIndex_k = AdjacentFaceIndexListB[k];
		//take reference
		auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
		auto tempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
		if (tempIndex >= 0)
		{
			PointIndexList_k[tempIndex] = PointIndexA;
		}
	}

	//update edge adjacent to B
	const auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();	
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
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();//to be updated
	AdjacentEdgeIndexListA = { &AdjacentEdgeIndexListA, &AdjacentEdgeIndexListB };
	auto tempIndexList_edge_delete = AdjacentEdgeIndexListA.ExactMatch(EdgeIndex_AB);
	AdjacentEdgeIndexListA.Delete(tempIndexList_edge_delete);

	//delete any adjacency info of B
	m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList().Clear();

	//---------------------------------------------------------------	
	return true;
}


template<typename ScalarType>
bool Mesh<ScalarType>::ShrinkEdgeToPoint(int_max EdgeIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::ShrinkEdgeToPoint(...)")
		return false;
	}

	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ Mesh::ShrinkEdgeToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::ShrinkEdgeToPoint(...)")		
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


template<typename ScalarType>
DenseVector<int_max, 2> Mesh<ScalarType>::SplitEdgeByPoint(int_max EdgeIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::SplitEdgeByPoint(...)")
		return false;
	}

	DenseVector<int_max, 2> EdgeIndexPair;
	EdgeIndexPair[0] = -1;
	EdgeIndexPair[1] = -1;

	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ Mesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::SplitEdgeByPoint(...)")
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
		MDK_Error("Input Point is on Edge  @ Mesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	//------------------------- input check is done --------------------------------------------//

	auto EdgeIndex01 = EdgeIndex;
	auto EdgeIndex02 = this->AddEdge(PointIndex0, PointIndex2);
	auto EdgeIndex21 = this->AddEdge(PointIndex2, PointIndex1);

	auto AdjacentFaceIndexList = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjacentFaceIndexList.GetLength(); ++k)
	{// delete Edge01, add Edge02 Edge21 and Point2 in Face
		auto FaceIndex_k = AdjacentFaceIndexList[k];
		//   ----------            -------------
		//   \  Face  /       to   \    Face    /
		//    Pa-->--Pb             Pa--->P2-->Pb
		//   ------------------------------------------
		//   [Pa, Pb] may be [P0 P1] or [P1 P0];
		//-----------------------------------------------
		int_max PointIndexA, PointIndexB;	
		int_max EdgeIndexA2, EdgeIndex2B;
		auto Flag_order= m_MeshData->FaceList[FaceIndex_k].CheckPointOrder(PointIndex0, PointIndex1);
		if (Flag_order == true)
		{
			PointIndexA = PointIndex0;
			PointIndexB = PointIndex1;
			EdgeIndexA2 = EdgeIndex02;
			EdgeIndex2B = EdgeIndex21;
		}
		else
		{
			PointIndexB = PointIndex0;
			PointIndexA = PointIndex1;
			EdgeIndexA2 = EdgeIndex21;
			EdgeIndex2B = EdgeIndex02;
		}
		//update edge
		{
			//take reference
			auto& EdgeIndexList_k = m_MeshData->FaceList[FaceIndex_k].EdgeIndexList();
			//delete Edge01
			auto tempIndex = EdgeIndexList_k.ExactMatch("first", EdgeIndex01);
			EdgeIndexList_k.Delete(tempIndex);
			//add Edge
			EdgeIndexList_k.Insert(tempIndex, EdgeIndex2B);
			EdgeIndexList_k.Insert(tempIndex, EdgeIndexA2);
		}
		//update point
		{
			// take reference
			auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
			//find PointB
			auto tempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
			//add Point2
			PointIndexList_k.Insert(tempIndex, PointIndex2);
		}
	}

	// delete edge P0P1
	this->UpdateRecord_DeleteEdge(EdgeIndex01);

	//-------------------------------------------------------------------------
	EdgeIndexPair[0] = EdgeIndex02;
	EdgeIndexPair[1] = EdgeIndex21;
	return EdgeIndexPair;
}


template<typename ScalarType>
bool Mesh<ScalarType>::ShrinkFaceToPoint(int_max FaceIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::ShrinkFaceToPoint(...)")
		return false;
	}

	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ Mesh::ShrinkFaceToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::ShrinkFaceToPoint(...)")
		return false;
	}

	auto PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();
	//triangle check
	for(int_max k=0; k<PointIndexList.GetLength(); ++k)
	{
		int_max PointIndexA = PointIndexList[k];
		int_max PointIndexB = 0;
		if (k < PointIndexList.GetLength() - 1)
		{
			PointIndexB = k + 1;
		}
		auto AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListAB = Intersect(AdjacentPointIndexListA, AdjacentPointIndexListB);
		if (AdjacentPointIndexListAB.IsEmpty() == false)
		{
			MDK_Error("Can not shrink: triangle detected @ Mesh::ShrinkFaceToPoint(...)")
			return false;
		}
	}
	auto EdgeIndexList= m_MeshData->FaceList[FaceIndex].GetEdgeIndexList();
	auto AdjacentFaceIndexList = m_MeshData->FaceList[FaceIndex].GetAdjacentFaceIndexList();
	this->DeleteFace(FaceIndex);

	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		int_max PointIndexA, PointIndexB;
		m_MeshData->EdgeList[EdgeIndexList[k]].GetPointIndexList(PointIndexA, PointIndexB);

		auto tempList = m_MeshData->EdgeList[EdgeIndexList[k]].GetAdjacentFaceIndexList();
		for (int_max n = 0; n < tempList.GetLength(); ++n)
		{
			if (tempList[n] != FaceIndex)
			{
				int_max AdjFaceIndex = tempList[n];				
				//take reference
				auto& EdgeIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].EdgeIndexList();
				auto tempIndex_edge = EdgeIndexList_adj.ExactMatch("first", EdgeIndexList[k]);
				EdgeIndexList_adj.Delete(tempIndex_edge);
				
				//take reference
				auto& PointIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].PointIndexList();
				auto tempIndexA = PointIndexList_adj.ExactMatch("first", PointIndexA);
				PointIndexList_adj.Delete(tempIndexA);
				auto tempIndexB = PointIndexList_adj.ExactMatch("first", PointIndexB);					
				PointIndexList_adj[tempIndexB]= PointIndex;				
			}
		}
	}

	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		this->UpdateRecord_DeleteEdge(EdgeIndexList[k]);
	}

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		auto FaceIndexList_adj = m_MeshData->PointList[PointIndexList[k]].GetAdjacentFaceIndexList();
		auto TempFaceIndexList = SetDiff(FaceIndexList_adj, AdjacentFaceIndexList);
		for (int_max n = 0; n < TempFaceIndexList.GetLength(); ++n)
		{
			if (TempFaceIndexList[n] != FaceIndex)
			{
				auto AdjFaceIndex = TempFaceIndexList[n];
				//take reference
				auto& PointIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].PointIndexList();
				auto tempIndex = PointIndexList_adj.ExactMatch("first", PointIndexList[k]);
				PointIndexList_adj[tempIndex] = PointIndex;
			}
		}

		auto EdgeIndexList_adj = m_MeshData->PointList[PointIndexList[k]].GetAdjacentEdgeIndexList();
		for (int_max n = 0; n < EdgeIndexList_adj.GetLength(); ++n)
		{
			auto tempPointIndexList = m_MeshData->EdgeList[EdgeIndexList_adj[n]].GetPointIndexList();
			if (tempPointIndexList[0] == PointIndexList[k])
			{
				m_MeshData->EdgeList[EdgeIndexList_adj[n]].SetPointIndexList(PointIndexList[k], tempPointIndexList[1]);
			}
			else if (tempPointIndexList[1] == PointIndexList[k])
			{
				m_MeshData->EdgeList[EdgeIndexList_adj[n]].SetPointIndexList(tempPointIndexList[0], PointIndexList[k]);
			}
		}
	}
	//--------------------------------
	return true;
}


/*
template<typename ScalarType>
int_max Mesh<ScalarType>::MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB)
{

}
*/
/*
template<typename ScalarType>
DenseVector<int_max, 2> Mesh<ScalarType>::SplitFaceByEdge(int_max EdgeIndex)
{
}
*/

template<typename ScalarType>
DenseVector<int_max, 2> Mesh<ScalarType>::SplitFace(int_max FaceIndex, int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::SplitFace(...)")
		return false;
	}

	auto EdgeIndex = this->AddEdge(PointIndexA, PointIndexB);
	return this->SplitFaceByEdge(FaceIndex, EdgeIndex);
}


template<typename ScalarType>
DenseVector<int_max, 2> Mesh<ScalarType>::SplitFaceByEdge(int_max FaceIndex, int_max EdgeABIndex)
{
	//------------------------
	//  ---<--A------   
	//  |     |     |
	//  |     |     |
	//  '-----B-->--'
	// 
	// -> show the point order
	//EdgeAB not adjacent to the face
	//-----------------------

	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ Mesh::SplitFaceByEdge(...)")
		return false;
	}

	DenseVector<int_max, 2> NewFaceIndexList;
	NewFaceIndexList[0] = -1;
	NewFaceIndexList[1] = -1 ;

	//--------------------- check input --------------------------------------------------
	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	if (this->IsValidEdgeIndex(EdgeABIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	auto PointCountOftheFace = m_MeshData->FaceList[FaceIndex].GetPointCount();
	if (PointCountOftheFace <= 3)
	{
		MDK_Error("Can not split face with <= 3 point @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	//------------------------------------------------------------------------------------
	int_max PointIndexA, PointIndexB;
	m_MeshData->EdgeList[EdgeABIndex].GetPointIndexList(PointIndexA, PointIndexB);

	auto PointIndexList_A = m_MeshData->FaceList[FaceIndex].GetPointIndexList_LeadBy(PointIndexA);
	if (PointIndexList_A.IsEmpty() == true)
	{
		MDK_Error("Edge and Face do not share point-A @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	for (int_max k = 1; k < PointIndexList_A.GetLength(); ++k)
	{
		if (PointIndexList_A[k] == PointIndexB)
		{
			PointIndexList_A = PointIndexList_A.GetSubSet(span(0, k));
			break;
		}
	}

	auto PointIndexList_B = m_MeshData->FaceList[FaceIndex].GetPointIndexList_LeadBy(PointIndexB);
	if (PointIndexList_B.IsEmpty() == true)
	{
		MDK_Error("Edge and Face do not share point-B @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	for (int_max k = 1; k < PointIndexList_B.GetLength(); ++k)
	{
		if (PointIndexList_B[k] == PointIndexA)
		{
			PointIndexList_B = PointIndexList_B.GetSubSet(span(0, k));
			break;
		}
	}

	if (PointIndexList_A.GetLength() < 3 || PointIndexList_B.GetLength() < 3)
	{
		MDK_Error("less than 3 point for each split face @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	
	if (PointIndexList_A.GetLength() == PointCountOftheFace || PointIndexList_B.GetLength() == PointCountOftheFace)
	{
		MDK_Error("Can not split: PointA and PointB are adjacent @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	this->DeleteFace(FaceIndex);
	
	NewFaceIndexList[0] =this->AddFaceByPoint(PointIndexList_A);
	NewFaceIndexList[1] =this->AddFaceByPoint(PointIndexList_B);
	return NewFaceIndexList;
}


//----------------------------------- protected function ---------------------------------------------------------//

template<typename ScalarType>
void Mesh<ScalarType>::UpdateRecord_DeletePoint(int_max PointIndex)
{// Cell/Face/Edge asscociated with this point have been deleted
	auto Name = m_MeshData->PointList[PointIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_Point_Name_to_Index.find(Name);
		if (it != m_MeshData->Map_Point_Name_to_Index.end())
		{
			m_MeshData->Map_Point_Name_to_Index.erase(it);
		}
	}

	for (int_max k = 0; k < m_MeshData->PointDataSet.GetLength(); ++k)
	{
		m_MeshData->PointDataSet[k].FillCol(PointIndex, 0);
	}

    m_MeshData->PointPositionTable.FillCol(PointIndex, 0);

	m_MeshData->PointList[PointIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->PointValidityFlagList[PointIndex] = 0;
}


template<typename ScalarType>
void Mesh<ScalarType>::UpdateRecord_DeleteEdge(int_max EdgeIndex)
{// Cell/Face asscociated with this edge have been deleted
	int_max PointIndex0, PointIndex1;
	m_MeshData->EdgeList[EdgeIndex].GetPointIndexList(PointIndex0, PointIndex1);
    //
	auto tempIndex0 = m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().ExactMatch(EdgeIndex);
	m_MeshData->PointList[PointIndex0].AdjacentEdgeIndexList().Delete(tempIndex0);
	//
	auto tempIndex1 = m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().ExactMatch(EdgeIndex);
	m_MeshData->PointList[PointIndex1].AdjacentEdgeIndexList().Delete(tempIndex1);

	auto Name = m_MeshData->EdgeList[EdgeIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_Edge_Name_to_Index.find(Name);
		if (it != m_MeshData->Map_Edge_Name_to_Index.end())
		{
			m_MeshData->Map_Edge_Name_to_Index.erase(it);
		}
	}

	for (int_max k = 0; k < m_MeshData->EdgeDataSet.GetLength(); ++k)
	{
		m_MeshData->EdgeDataSet[k].FillCol(EdgeIndex, 0);
	}

    // Delete Edge: only release memory
	m_MeshData->EdgeList[EdgeIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->EdgeValidityFlagList[EdgeIndex] = 0;
}


template<typename ScalarType>
void Mesh<ScalarType>::UpdateRecord_DeleteFace(int_max FaceIndex)
{// Cell asscociated with this point have been deleted
	const auto& EdgeIndexList = m_MeshData->FaceList[FaceIndex].EdgeIndexList();
	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		auto tempIndex = m_MeshData->EdgeList[EdgeIndexList[k]].AdjacentFaceIndexList().ExactMatch(FaceIndex);
		m_MeshData->EdgeList[EdgeIndexList[k]].AdjacentFaceIndexList().Delete(tempIndex);
	}
    
	auto Name = m_MeshData->FaceList[FaceIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_Face_Name_to_Index.find(Name);
		if (it != m_MeshData->Map_Face_Name_to_Index.end())
		{
			m_MeshData->Map_Face_Name_to_Index.erase(it);
		}
	}

	for (int_max k = 0; k < m_MeshData->FaceDataSet.GetLength(); ++k)
	{
		m_MeshData->FaceDataSet[k].FillCol(FaceIndex, 0);
	}

    // Delete Face : only clear memory
	m_MeshData->FaceList[FaceIndex].Clear(MDK_PURE_EMPTY);
    m_MeshData->FaceValidityFlagList[FaceIndex] = 0;
}

template<typename ScalarType>
void Mesh<ScalarType>::UpdateRecord_DeleteCell(int_max CellIndex)
{
	const auto& FaceIndexList = m_MeshData->CellList[CellIndex].FaceIndexList();
	for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
	{
		auto tempIndex = m_MeshData->FaceList[FaceIndexList[k]].AdjacentCellIndexList().ExactMatch(CellIndex);
		m_MeshData->FaceList[FaceIndexList[k]].AdjacentCellIndexList().Delete(tempIndex);
	}

	auto Name = m_MeshData->CellList[CellIndex].GetName();
	if (Name.IsEmpty() == false)
	{
		auto it = m_MeshData->Map_Cell_Name_to_Index.find(Name);
		if (it != m_MeshData->Map_Cell_Name_to_Index.end())
		{
			m_MeshData->Map_Cell_Name_to_Index.erase(it);
		}
	}

	for (int_max k = 0; k < m_MeshData->CellDataSet.GetLength(); ++k)
	{
		m_MeshData->CellDataSet[k].FillCol(CellIndex, 0);
	}

	// Delete Cell : only clear memory
	m_MeshData->CellList[FaceIndex].Clear(MDK_PURE_EMPTY);
	m_MeshData->CellValidityFlagList[CellIndex] = 0;
}

}// namespace mdk
