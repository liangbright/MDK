#pragma once

namespace mdk
{

template<typename ScalarType>
FiniteElementMesh<ScalarType>::FiniteElementMesh()
{
	m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
}

template<typename ScalarType>
FiniteElementMesh<ScalarType>::FiniteElementMesh(const FiniteElementMesh<ScalarType>& Input)
{
	this->Copy(Input);
}

template<typename ScalarType>
FiniteElementMesh<ScalarType>::FiniteElementMesh(FiniteElementMesh<ScalarType>&& Input)
{
	m_MeshData = std::move(Input.m_MeshData);
}

template<typename ScalarType>
FiniteElementMesh<ScalarType>::~FiniteElementMesh()
{
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::operator=(const FiniteElementMesh<ScalarType>& Input)
{
	this->Copy(Input);
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::operator=(FiniteElementMesh<ScalarType>&& Input)
{
	this->Copy(std::move(Input));
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Copy(const FiniteElementMesh<ScalarType>& Input)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}

	if (Input.IsEmpty() == true)
	{
		this->Clear();
		return;
	}

	m_MeshData->Name = Input.m_MeshData->Name;

	m_MeshData->NodeList = Input.m_MeshData->NodeList;
	m_MeshData->NodeNameList = Input.m_MeshData->NodeNameList;

	m_MeshData->ElementList = Input.m_MeshData->ElementList;
	m_MeshData->ElementNameList = Input.m_MeshData->ElementNameList;
	m_MeshData->ElementTypeList = Input.m_MeshData->ElementTypeList;
	
	m_MeshData->NodeDataSet = Input.m_MeshData->NodeDataSet;
	m_MeshData->NodeDataSetName = Input.m_MeshData->NodeDataSetName;

	m_MeshData->ElementDataSet = Input.m_MeshData->ElementDataSet;
	m_MeshData->ElementDataSetName = Input.m_MeshData->ElementDataSetName;

	m_MeshData->NodeSet = Input.m_MeshData->NodeSet;
	m_MeshData->NodeSetName = Input.m_MeshData->NodeSetName;

	m_MeshData->ElementSet = Input.m_MeshData->ElementSet;
	m_MeshData->ElementSetName = Input.m_MeshData->ElementSetName;
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Copy(FiniteElementMesh<ScalarType>&& Input)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}

	if (Input.IsEmpty() == true)
	{
		this->Clear();
		return;
	}

	m_MeshData->Name = std::move(Input.m_MeshData->Name);

	m_MeshData->NodeList = std::move(Input.m_MeshData->NodeList);
	m_MeshData->NodeNameList = std::move(Input.m_MeshData->NodeNameList);

	m_MeshData->ElementList = std::move(Input.m_MeshData->ElementList);
	m_MeshData->ElementNameList = std::move(Input.m_MeshData->ElementNameList);
	m_MeshData->ElementTypeList = std::move(Input.m_MeshData->ElementTypeList);	

	m_MeshData->NodeDataSet = std::move(Input.m_MeshData->NodeDataSet);
	m_MeshData->NodeDataSetName = std::move(Input.m_MeshData->NodeDataSetName);

	m_MeshData->ElementDataSet = std::move(Input.m_MeshData->ElementDataSet);
	m_MeshData->ElementDataSetName = std::move(Input.m_MeshData->ElementDataSetName);

	m_MeshData->NodeSet = std::move(Input.m_MeshData->NodeSet);
	m_MeshData->NodeSetName = std::move(Input.m_MeshData->NodeSetName);

	m_MeshData->ElementSet = std::move(Input.m_MeshData->ElementSet);
	m_MeshData->ElementSetName = std::move(Input.m_MeshData->ElementSetName);

	Input.Clear();
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Share(FiniteElementMesh<ScalarType>& Input)
{
	m_MeshData = Input.m_MeshData
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::ForceShare(const FiniteElementMesh<ScalarType>& Input)
{
	m_MeshData = Input.m_MeshData
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Recreate()
{
	m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Clear()
{
	if (m_MeshData)
	{
		m_MeshData->Name.Clear();
		m_MeshData->NodeList.Clear();
		m_MeshData->NodeNameList.Clear();
		m_MeshData->ElementList.Clear();
		m_MeshData->ElementNameList.Clear();
		m_MeshData->ElementTypeList.Clear();		
		m_MeshData->NodeDataSet.Clear();
		m_MeshData->NodeDataSetName.Clear();
		m_MeshData->ElementDataSet.Clear();
		m_MeshData->ElementDataSetName.Clear();
		m_MeshData->NodeSet.Clear();
		m_MeshData->NodeSetName.Clear();
		m_MeshData->ElementSet.Clear();
		m_MeshData->ElementSetName.Clear();
	}
}

template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsEmpty() const
{
	if (!m_MeshData)
	{
		return true;
	}
	else
	{
		return m_MeshData->NodeList.IsEmpty();
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetCapacity(int_max NodeCount, int_max ElementCount)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}
	m_MeshData->NodeList.SetCapacity(3, NodeCount);
	m_MeshData->NodeNameList.SetCapacity(NodeCount);
	m_MeshData->ElementList.SetCapacity(ElementCount);
	m_MeshData->ElementNameList.SetCapacity(ElementCount);
	m_MeshData->ElementTypeList.SetCapacity(ElementCount);

	for (int_max k = 0; k < m_MeshData->NodeDataSet.GetLength(); ++k)
	{
		m_MeshData->NodeDataSet[k].SetCapacity(NodeCount);
	}
	for (int_max k = 0; k < m_MeshData->ElementDataSet.GetLength(); ++k)
	{
		m_MeshData->ElementDataSet[k].SetCapacity(ElementCount);
	}	
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Resize(int_max NodeCount, int_max ElementCount)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}
	m_MeshData->NodeList.Resize(3, NodeCount);
	m_MeshData->NodeNameList.Resize(NodeCount);
	m_MeshData->ElementList.Resize(ElementCount);
	m_MeshData->ElementNameList.Resize(ElementCount);
	m_MeshData->ElementTypeList.Resize(ElementCount);
	for (int_max k = 0; k < m_MeshData->NodeDataSet.GetLength(); ++k)
	{
		m_MeshData->NodeDataSet[k].Resize(NodeCount);
	}
	for (int_max k = 0; k < m_MeshData->ElementDataSet.GetLength(); ++k)
	{
		m_MeshData->ElementDataSet[k].Resize(ElementCount);
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Construct(DenseMatrix<ScalarType> NodeList, ObjectArray<DenseVector<int_max>> ElementList)
{// no input check
	auto NodeCount = NodeList.GetColCount();
	auto ElementCount = ElementList.GetLength();

	this->Clear();
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}

	m_MeshData->NodeList = std::move(NodeList);
	m_MeshData->ElementList = std::move(ElementList);	
	m_MeshData->NodeNameList.Resize(NodeCount);
	m_MeshData->ElementNameList.Resize(ElementCount);
	m_MeshData->ElementTypeList.Resize(ElementCount);
	for (int_max k = 0; k < ElementCount; ++k)
	{
		m_MeshData->ElementTypeList[k] = FiniteElementType::UNKNOWN;
	}
}

//------------------------------------------ Node -----------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNode(const DenseVector<ScalarType, 3>& Position)
{
	return this->AddNode(Position[0], Position[1], Position[2]);
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNode(const ScalarType Position[3])
{
	return this->AddNode(Position[0], Position[1], Position[2]);
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNode(ScalarType x, ScalarType y, ScalarType z)
{
	m_MeshData->NodeList.AppendCol({ x, y, z });
	String EmptyName;
	m_MeshData->NodeNameList.Append(EmptyName);
	return m_MeshData->NodeList.GetColCount()-1;
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::AddNode_batch(const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> NodeIndexList;

	if (PointSet.IsVector() == true)
	{
		if (PointSet.GetElementCount() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ FiniteElementMesh::AddPoint_batch(...)")
			return NodeIndexList;
		}
		NodeIndexList.Resize(1);
		NodeIndexList[0] = this->AddNode(PointSet.GetPointer());
		return NodeIndexList;
	}

	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("PointSet is a matrix but RowCount != 3 @ FiniteElementMesh::AddPoint_batch(...)")
		return NodeIndexList;
	}
	
	NodeIndexList.Resize(PointSet.GetColCount());
	for (int_max k = 0; k < PointSet.GetColCount(); ++k)
	{
		NodeIndexList[k] = this->AddNode(PointSet(0, k), PointSet(1, k), PointSet(2, k));
	}
	return NodeIndexList;
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNode(int_max NodeIndex, const DenseVector<ScalarType, 3>& Position)
{
	m_MeshData->NodeList.SetCol(NodeIndex, Position);
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNode(int_max NodeIndex, const ScalarType Position[3])
{
	m_MeshData->NodeList.SetCol(NodeIndex, Position);
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNode(int_max NodeIndex, ScalarType x, ScalarType y, ScalarType z)
{
	m_MeshData->NodeList.SetCol(NodeIndex, { x, y, z });
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNode_batch(const DenseVector<int_max>& NodeIndexList, const DenseMatrix<ScalarType>& PointSet)
{
	for (int_max k = 0; k < NodeIndexList.GetLength(); ++k)
	{
		m_MeshData->NodeList.SetCol(NodeIndexList[k], PointSet.GetPointerOfCol(k));
	}
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeName(int_max NodeIndex, const String& NodeName)
{// no input check
	m_MeshData->NodeNameList[NodeIndex] = NodeName;
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeCount() const
{
	return m_MeshData->NodeList.GetColCount();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeIndex(const String& NodeName) const
{
	for (int_max k = 0; k < m_MeshData->NodeNameList.GetLength(); ++k)
	{
		if (m_MeshData->NodeNameList[k] == NodeName)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetNodeName(int_max NodeIndex) const
{
	return m_MeshData->NodeNameList[NodeIndex];
}

template<typename ScalarType>
DenseVector<ScalarType, 3> FiniteElementMesh<ScalarType>::GetNode(int_max NodeIndex) const
{
	DenseVector<ScalarType, 3> Pos;
	m_MeshData->NodeList.GetCol(NodeIndex, Pos);
	return Pos;
}

template<typename ScalarType>
DenseVector<ScalarType, 3> FiniteElementMesh<ScalarType>::GetNode(const String& NodeName) const
{
	auto NodeIndex = this->GetNodeIndex(NodeName);
	return this->GetNode(NodeIndex);
}

template<typename ScalarType>
DenseMatrix<ScalarType> FiniteElementMesh<ScalarType>::GetNode(const DenseVector<int_max>& NodeIndexList) const
{
	DenseMatrix<ScalarType> NodeSet;
	NodeSet = m_MeshData->NodeList.GetSubMatrix(ALL, NodeIndexList);
	return NodeSet;
}


template<typename ScalarType>
DenseMatrix<ScalarType> FiniteElementMesh<ScalarType>::GetNode(const MDK_Symbol_ALL&) const
{
	return m_MeshData->NodeList;
}

//------------------------------------------ Element -----------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElement(FiniteElementType ElementType, const DenseVector<int_max>& NodeIndexList)
{
	m_MeshData->ElementList.Append(NodeIndexList);
	m_MeshData->ElementTypeList.Append(ElementType);
	String EmptyName;
	m_MeshData->ElementNameList.Append(EmptyName);
	return m_MeshData->ElementList.GetLength()-1;
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElement(const String& ElementType, const DenseVector<int_max>& NodeIndexList)
{
	auto Type = this->ConvertStringToElementType(ElementType);
	return this->AddElement(Type, NodeIndexList);
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElement(int_max ElementIndex, const DenseVector<int_max>& NodeIndexList)
{
	m_MeshData->ElementList[ElementIndex] = NodeIndexList;
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementName(int_max ElementIndex, const String& ElementName)
{
	m_MeshData->ElementNameList[ElementIndex] = ElementName;
}

template<typename ScalarType>
inline
void FiniteElementMesh<ScalarType>::SetElementType(int_max ElementIndex, FiniteElementType Type)
{
	m_MeshData->ElementTypeList[ElementIndex] = Type;
}

template<typename ScalarType>
inline
void FiniteElementMesh<ScalarType>::SetElementType(int_max ElementIndex, const String& Type)
{
	m_MeshData->ElementTypeList[ElementIndex] = this->ConvertStringToElementType(Type);
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementCount() const
{
	return m_MeshData->ElementList.GetLength();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementIndex(const String& ElementName) const
{
	for (int_max k = 0; k < m_MeshData->ElementNameList.GetLength(); ++k)
	{
		if (m_MeshData->ElementNameList[k] == ElementName)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementName(int_max ElementIndex) const
{
	return m_MeshData->ElementNameList[ElementIndex];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElement(int_max ElementIndex) const
{
	return m_MeshData->ElementList[ElementIndex];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElement(const String& ElementName) const
{
	auto ElementIndex = this->GetElementIndex(ElementName);
	return this->GetElement(ElementIndex);
}

template<typename ScalarType>
ObjectArray<DenseVector<int_max>> FiniteElementMesh<ScalarType>::GetElement(const DenseVector<int_max>& ElementIndexList) const
{
	ObjectArray<DenseVector<int_max>> ElementList;
	ElementList.Resize(ElementIndexList.GetLength());
	for (int_max k = 0; k < ElementIndexList.GetLength(); ++k)
	{
		ElementList[k] = this->GetElement(ElementIndexList[k]);
	}
	return ElementList;
}

template<typename ScalarType>
ObjectArray<DenseVector<int_max>> FiniteElementMesh<ScalarType>::GetElement(const MDK_Symbol_ALL&) const
{
	return m_MeshData->ElementList;
}

template<typename ScalarType>
FiniteElementType FiniteElementMesh<ScalarType>::GetElementType(int_max ElementIndex) const
{
	return m_MeshData->ElementTypeList[ElementIndex];
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementTypeAsString(int_max ElementIndex) const
{
	return this->ConvertElementTypeToString(this->GetElementType(ElementIndex));
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::ConvertElementTypeToString(FiniteElementType Type) const
{
	String ElementTypeName;
	switch (Type)
	{
	case FiniteElementType::VTK_TRIANGLE:
		ElementTypeName = "VTK_TRIANGLE";
		break;
	case FiniteElementType::VTK_QUAD:
		ElementTypeName = "VTK_QUAD";
		break;
	case FiniteElementType::VTK_POLYGON:
		ElementTypeName = "VTK_POLYGON";
	case FiniteElementType::VTK_TETRA:
		ElementTypeName = "VTK_TETRA";
		break;
	case FiniteElementType::VTK_WEDGE:
		ElementTypeName = "VTK_WEDGE";
		break;
	case FiniteElementType::VTK_HEXAHEDRON:
		ElementTypeName = "VTK_HEXAHEDRON";
		break;
	case FiniteElementType::Abaqus_S3:
		ElementTypeName = "Abaqus_S3";
		break;
	case FiniteElementType::Abaqus_S4:
		ElementTypeName = "Abaqus_S4";
		break;
	case FiniteElementType::Abaqus_C3D4:
		ElementTypeName = "Abaqus_C3D4";
		break;
	case FiniteElementType::Abaqus_C3D6:
		ElementTypeName = "Abaqus_C3D6";
		break;
	case FiniteElementType::Abaqus_C3D8:
		ElementTypeName = "Abaqus_C3D8";
		break;
	default:
		ElementTypeName = "UNKNOWN";
	}
	return ElementTypeName;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementTypeAsString_Abaqus(int_max ElementIndex) const
{
	return this->ConvertElementTypeToString_Abaqus(this->GetElementType(ElementIndex));
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::ConvertElementTypeToString_Abaqus(FiniteElementType Type) const
{
	String ElementTypeName;
	switch (Type)
	{
	case FiniteElementType::VTK_TRIANGLE:
		ElementTypeName = "S3";
		break;
	case FiniteElementType::VTK_QUAD:
		ElementTypeName = "S4";
		break;
	case FiniteElementType::VTK_TETRA:
		ElementTypeName = "C3D4";
		break;
	case FiniteElementType::VTK_PYRAMID:
		ElementTypeName = "C3D5";
		break;
	case FiniteElementType::VTK_WEDGE:
		ElementTypeName = "C3D6";
		break;
	case FiniteElementType::VTK_HEXAHEDRON:
		ElementTypeName = "C3D8";
		break;
	case FiniteElementType::Abaqus_S3:
		ElementTypeName = "S3";
		break;
	case FiniteElementType::Abaqus_S4:
		ElementTypeName = "S4";
		break;
	case FiniteElementType::Abaqus_C3D4:
		ElementTypeName = "C3D4";
		break;
	case FiniteElementType::Abaqus_C3D5:
		ElementTypeName = "C3D5";
		break;
	case FiniteElementType::Abaqus_C3D6:
		ElementTypeName = "C3D6";
		break;
	case FiniteElementType::Abaqus_C3D8:
		ElementTypeName = "C3D8";
		break;
	default:
		ElementTypeName = "UNKNOWN";
	}
	return ElementTypeName;
}

template<typename ScalarType>
FiniteElementType FiniteElementMesh<ScalarType>::ConvertStringToElementType(const String& Type) const
{
	FiniteElementType ElementType;
	if (Type == "VTK_TRIANGLE")
	{
		ElementType = FiniteElementType::VTK_TRIANGLE;
	}
	else if (Type == "VTK_QUAD")
	{
		ElementType = FiniteElementType::VTK_QUAD;
	}
	else if (Type == "VTK_POLYGON")
	{
		ElementType = FiniteElementType::VTK_POLYGON;
	}
	else if (Type == "VTK_TETRA")
	{
		ElementType = FiniteElementType::VTK_TETRA;
	}
	else if (Type == "VTK_PYRAMID")
	{
		ElementType = FiniteElementType::VTK_PYRAMID;
	}
	else if (Type == "VTK_WEDGE")
	{
		ElementType = FiniteElementType::VTK_WEDGE;
	}
	else if (Type == "VTK_HEXAHEDRON")
	{
		ElementType = FiniteElementType::VTK_HEXAHEDRON;
	}
	else if (Type == "VTK_CONVEX_POINT_SET")
	{
		ElementType = FiniteElementType::VTK_CONVEX_POINT_SET;
	}
	else if (Type == "Abaqus_S3")
	{
		ElementType = FiniteElementType::Abaqus_S3;
	}
	else if (Type == "Abaqus_S4")
	{
		ElementType = FiniteElementType::Abaqus_S4;
	}
	else if (Type == "Abaqus_C3D4")
	{
		ElementType = FiniteElementType::Abaqus_C3D4;
	}
	else if (Type == "Abaqus_C3D5")
	{
		ElementType = FiniteElementType::Abaqus_C3D5;
	}
	else if (Type == "Abaqus_C3D6")
	{
		ElementType = FiniteElementType::Abaqus_C3D6;
	}
	else if (Type == "Abaqus_C3D8")
	{
		ElementType = FiniteElementType::Abaqus_C3D8;
	}
	else
	{
		MDK_Warning("FiniteElementType is unknown @ FiniteElementMesh::ConvertStringToElementType(...)")
		ElementType = FiniteElementType::UNKNOWN;
	}
	return ElementType;
}

//-------------------------------------------- NodeDataSet -----------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNodeDataSet(const String& Name, int_max ScalarCountPerNode)
{
	DenseMatrix<ScalarType> DataSet;
	DataSet.Resize(ScalarCountPerNode, this->GetNodeCount());
	DataSet.Fill(0);
	return this->AddNodeDataSet(Name, std::move(DataSet));
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNodeDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	auto Index = this->GetNodeDataSetIndex(Name);
	if (Index < 0)
	{
		m_MeshData->NodeDataSetName.Append(Name);		
		m_MeshData->NodeDataSet.Append(std::move(DataSet));
		Index = m_MeshData->NodeDataSet.GetLength() - 1;
		return Index;
	}
	else
	{
		MDK_Error("NodeDataSet has already been added @ FiniteElementMesh::AddElementDataSet(...)")
		return -1;
	}	
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeDataSetCount() const
{
	return m_MeshData->NodeDataSet.GetLength();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeDataSetIndex(const String& Name) const
{
	for (int_max k = 0; k < m_MeshData->NodeDataSetName.GetLength(); ++k)
	{
		if (Name == m_MeshData->NodeDataSetName[k])
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetNodeDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->NodeDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetNodeDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->NodeDataSetName[Index];
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	auto Index = this->GetNodeDataSetIndex(Name);
	this->SetNodeDataSet(Index, std::move(DataSet));
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeDataSet(int_max Index, DenseMatrix<ScalarType> DataSet)
{
	if (Index < 0 || Index >= m_MeshData->NodeDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::SetNodeDataSet(...)")
		return;
	}

	if (DataSet.GetColCount() != this->GetNodeCount())
	{
		MDK_Error("invalid DataSet @ FiniteElementMesh::SetNodeDataSet(...,...)")
		return;
	}
	m_MeshData->NodeDataSet[Index] = std::move(DataSet);
}

template<typename ScalarType>
const DenseMatrix<ScalarType>& FiniteElementMesh<ScalarType>::GetNodeDataSet(const String& Name) const
{
	auto Index = this->GetNodeDataSetIndex(Name);
	return this->GetNodeDataSet(Index);
}

template<typename ScalarType>
const DenseMatrix<ScalarType>& FiniteElementMesh<ScalarType>::GetNodeDataSet(int_max Index) const
{
	return m_MeshData->NodeDataSet[Index];
}

//-------------------------------------------- ElementDataSet -----------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementDataSet(const String& Name, int_max ScalarCountPerElement)
{
	DenseMatrix<ScalarType> DataSet;
	DataSet.Resize(ScalarCountPerElement, this->GetElementCount());
	DataSet.Fill(0);
	return this->AddElementDataSet(Name, std::move(DataSet));
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	auto Index = this->GetElementDataSetIndex(Name);
	if (Index < 0)
	{
		m_MeshData->ElementDataSetName.Append(Name);
		m_MeshData->ElementDataSet.Append(std::move(DataSet));
		Index = m_MeshData->ElementDataSet.GetLength() - 1;
		return Index;
	}
	else
	{
		MDK_Error("ElementDataSet has already been added @ FiniteElementMesh::AddElementDataSet(...)")
		return -1;
	}	
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementDataSetCount() const
{
	return m_MeshData->ElementDataSet.GetLength();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementDataSetIndex(const String& Name) const
{
	for (int_max k = 0; k < m_MeshData->ElementDataSetName.GetLength(); ++k)
	{
		if (Name == m_MeshData->ElementDataSetName[k])
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementDataSetName(int_max Index) const
{
	if (Index < 0 || Index >= m_MeshData->ElementDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::GetElementDataSetName(...)")
		String EmptyName;
		return EmptyName;
	}
	return m_MeshData->ElementDataSetName[Index];
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementDataSet(const String& Name, DenseMatrix<ScalarType> DataSet)
{
	auto Index = this->GetElementDataSetIndex(Name);
	this->SetElementDataSet(Index, std::move(DataSet));
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementDataSet(int_max Index, DenseMatrix<ScalarType> DataSet)
{
	if (Index < 0 || Index >= m_MeshData->ElementDataSet.GetLength())
	{
		MDK_Error("Index " << Index << "NOT exist @ Mesh::SetElementDataSet(...)")
		return;
	}

	if (DataSet.GetColCount() != this->GetElementCount())
	{
		MDK_Error("invalid DataSet @ FiniteElementMesh::SetElementDataSet(...,...)")
		return;
	}
	m_MeshData->ElementDataSet[Index] = std::move(DataSet);
}

template<typename ScalarType>
const DenseMatrix<ScalarType>& FiniteElementMesh<ScalarType>::GetElementDataSet(const String& Name) const
{
	auto Index = this->GetElementDataSetIndex(Name);
	return this->GetElementDataSet(Index);
}

template<typename ScalarType>
const DenseMatrix<ScalarType>& FiniteElementMesh<ScalarType>::GetElementDataSet(int_max Index) const
{
	return m_MeshData->ElementDataSet[Index];
}

//-------------------------------------------- NodeSet -----------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNodeSet(const String& Name)
{
	if (this->GetNodeSetIndex(Name) < 0)
	{
		DenseVector<int_max> EmptyList;
		m_MeshData->NodeSet.Append(EmptyList);
		m_MeshData->NodeSetName.Append(Name);
		return m_MeshData->NodeSet.GetLength() - 1;
	}
	else
	{
		MDK_Error("Name already exist @ FiniteElementMesh::AddNodeSet()")
		return -1;
	}
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNodeSet(const String& Name, const DenseVector<int_max>& NodeIndexList)
{
	if (this->GetNodeSetIndex(Name) < 0)
	{
		m_MeshData->NodeSet.Append(NodeIndexList);
		m_MeshData->NodeSetName.Append(Name);
		return m_MeshData->NodeSet.GetLength() - 1;
	}
	else
	{
		MDK_Error("Name already exist @ FiniteElementMesh::AddNodeSet()")
		return -1;
	}	
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSet(const String& Name, const DenseVector<int_max>& NodeIndexList)
{
	auto Index = this->GetNodeSetIndex(Name);
	if (Index >= 0)
	{
		m_MeshData->NodeSet[Index] = NodeIndexList;
		m_MeshData->NodeSetName[Index] = Name;
	}
	else
	{
		MDK_Error("NodeSet NOT exist @ FiniteElementMesh::SetNodeSet()")
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSet(int_max Index, const DenseVector<int_max>& NodeIndexList)
{
	m_MeshData->NodeSet[Index] = NodeIndexList;
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSetName(int_max Index, const String& Name)
{
	m_MeshData->NodeSetName[Index] = Name;
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeSetCount() const
{
	return m_MeshData->NodeSet.GetLength();
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetNodeSet(int_max Index) const
{
	return m_MeshData->NodeSet[Index];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetNodeSet(const String& Name) const
{
	auto NodeSetIndex = this->GetNodeSetIndex(Name);
	return m_MeshData->NodeSet[NodeSetIndex];
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeSetIndex(const String& Name) const
{
	for (int_max k = 0; k < m_MeshData->NodeSetName.GetLength(); ++k)
	{
		if (m_MeshData->NodeSetName[k] == Name)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetNodeSetName(int_max Index) const
{
	return m_MeshData->NodeSetName[Index];
}

//------------------------------------------------ ElementSet -------------------------------------------------//

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementSet(const String& Name)
{
	if (this->GetElementSetIndex(Name) < 0)
	{
		DenseVector<int_max> EmptyList;
		m_MeshData->ElementSet.Append(EmptyList);		
		m_MeshData->ElementSetName.Append(Name);
		return m_MeshData->ElementSet.GetLength() - 1;
	}
	else
	{
		MDK_Error("Name already exist @ FiniteElementMesh::AddElementSet()")
		return -1;
	}
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementSet(const String& Name, const DenseVector<int_max>& ElementIndexList)
{
	if (this->GetElementSetIndex(Name) < 0)
	{
		m_MeshData->ElementSet.Append(ElementIndexList);
		m_MeshData->ElementSetName.Append(Name);
		return m_MeshData->ElementSet.GetLength() - 1;
	}
	else
	{
		MDK_Error("Name already exist @ FiniteElementMesh::AddElementSet()")
		return -1;
	}
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSet(const String& Name, const DenseVector<int_max>& ElementIndexList)
{
	auto Index = this->GetElementSetIndex(Name);
	if (Index >= 0)
	{
		m_MeshData->ElementSet[Index] = ElementIndexList;
		m_MeshData->ElementSetName[Index] = Name;
	}
	else
	{
		MDK_Error("ElementSet NOT exist @ FiniteElementMesh::SetElementSet()")
	}
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSet(int_max Index, const DenseVector<int_max>& ElementIndexList)
{
	m_MeshData->ElementSet[Index] = ElementIndexList;
}

template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSetName(int_max Index, const String& Name)
{
	m_MeshData->ElementSetName[Index] = Name;
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementSetCount() const
{
	return m_MeshData->ElementSet.GetLength();
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElementSet(int_max Index) const
{
	return m_MeshData->ElementSet[Index];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElementSet(const String& Name) const
{
	auto Index = this->GetElementSetIndex(Name);
	return m_MeshData->ElementSet[Index];
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementSetIndex(const String& Name) const
{
	for (int_max k = 0; k < m_MeshData->ElementSetName.GetLength(); ++k)
	{
		if (m_MeshData->ElementSetName[k] == Name)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementSetName(int_max Index) const
{
	return m_MeshData->ElementSetName[Index];
}

//------------------------------ other ---------------------------------------------------//

template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsShellElement(int_max ElementIndex) const
{
	auto Type = this->GetElementType(ElementIndex);
	if (Type == FiniteElementType::UNKNOWN)
	{
		return false;
	}
	switch (Type)
	{
	case FiniteElementType::VTK_TRIANGLE:
		return true;
	case FiniteElementType::VTK_QUAD:
		return true;
	case FiniteElementType::VTK_POLYGON:
		return true;
	case FiniteElementType::Abaqus_S3:
		return true;
	case FiniteElementType::Abaqus_S4:
		return true;
	default:
		return false;
	}
}

template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsSolidElement(int_max ElementIndex) const
{
	if (Type == FiniteElementType::UNKNOWN)
	{
		return false;
	}
	return !(this->IsShellElement(ElementIndex));
}


template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsShellMesh() const
{
	auto ElementCount = this->GetElementCount();
	for (int_max k = 0; k < ElementCount; ++k)
	{
		if (this->IsShellElement(k) == false)
		{
			return false;
		}
	}
	return true;
}

template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsSolidMesh() const
{
	auto ElementCount = this->GetElementCount();
	for (int_max k = 0; k < ElementCount; ++k)
	{
		if (this->IsSolidElement(k) == false)
		{
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
FiniteElementMesh<ScalarType> FiniteElementMesh<ScalarType>::GetSubMeshByElement(const DenseVector<int_max>& ElementIndexList) const
{
	FiniteElementMesh<ScalarType> SubMesh;
	if (ElementIndexList.IsEmpty() == true || this->IsEmpty() == true)
	{
		return SubMesh;
	}

	SubMesh.m_MeshData->ElementList = m_MeshData->ElementList.GetSubSet(ElementIndexList);
	SubMesh.m_MeshData->ElementTypeList = m_MeshData->ElementTypeList.GetSubSet(ElementIndexList);
	SubMesh.m_MeshData->ElementNameList = m_MeshData->ElementNameList.GetSubSet(ElementIndexList);

	if (m_MeshData->ElementDataSet.IsEmpty() == false)
	{
		SubMesh.m_MeshData->ElementDataSet = m_MeshData->ElementDataSet.GetSubSet(ElementIndexList);
		SubMesh.m_MeshData->ElementDataSetName = m_MeshData->ElementDataSetName.GetSubSet(ElementIndexList);
	}

	DenseVector<int_max> NodeIndexMap_Old_to_New;
	NodeIndexMap_Old_to_New = span(0, m_MeshData->NodeList.GetColCount() - 1);

	DenseVector<int_max> NodeIndexMap_New_to_Old;
	NodeIndexMap_New_to_Old.SetCapacity(m_MeshData->NodeList.GetColCount());
	
	DenseVector<int_max> NodeFlagList;
	NodeFlagList.Resize(m_MeshData->NodeList.GetColCount());
	NodeFlagList.Fill(0);//0: not used, 1: used in SubMesh

	int_max NewNodeIndex = -1;
	for (int_max k = 0; k < SubMesh.m_MeshData->ElementList.GetLength(); ++k)
	{
		const auto& NodeIndexList = SubMesh.m_MeshData->ElementList[k];
		for (int_max n = 0; n < NodeIndexList.GetLength(); ++n)
		{
			auto NodeIndex = NodeIndexList[n];
			if (NodeFlagList[NodeIndex] == 0)
			{
				NodeFlagList[NodeIndex] = 1;
				NewNodeIndex += 1;
				NodeIndexMap_Old_to_New[NodeIndex] = NewNodeIndex;
				NodeIndexMap_New_to_Old.Append(NodeIndex);
			}
		}
	}

	SubMesh.m_MeshData->NodeList.Resize(3, NodeIndexMap_New_to_Old.GetLength());
	SubMesh.m_MeshData->NodeNameList.Resize(NodeIndexMap_New_to_Old.GetLength());
	for (int_max k = 0; k < NodeIndexMap_New_to_Old.GetLength(); ++k)
	{
		int_max OldNodeIndex = NodeIndexMap_New_to_Old[k];
		SubMesh.m_MeshData->NodeList.SetCol(k, m_MeshData->NodeList.GetPointerOfCol(OldNodeIndex));
		SubMesh.m_MeshData->NodeNameList[k] = m_MeshData->NodeNameList[OldNodeIndex];
	}

	for (int_max k = 0; k < SubMesh.m_MeshData->ElementList.GetLength(); ++k)
	{
		//take reference
		auto& NodeIndexList = SubMesh.m_MeshData->ElementList[k];
		for (int_max n = 0; n < NodeIndexList.GetLength(); ++n)
		{
			NodeIndexList[n] = NodeIndexMap_Old_to_New[NodeIndexList[n]];
		}
	}

	if (m_MeshData->NodeDataSet.IsEmpty() == false)
	{
		SubMesh.m_MeshData->NodeDataSetName = m_MeshData->NodeDataSetName;
		SubMesh.m_MeshData->NodeDataSet.Resize(m_MeshData->NodeDataSet.GetLength());
		for (int_max k = 0; k < SubMesh.m_MeshData->NodeDataSet.GetLength(); ++k)
		{
			SubMesh.m_MeshData->NodeDataSet[k] = m_MeshData->NodeDataSet[k].GetSubMatrix(ALL, NodeIndexMap_New_to_Old);
		}
	}

	return SubMesh;
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::Append(const FiniteElementMesh<ScalarType>& InputMesh)
{
	auto NodeCount_init = m_MeshData->NodeList.GetColCount();
	auto ElementCount_init = m_MeshData->ElementList.GetLength();
	m_MeshData->NodeList = { &m_MeshData->NodeList, &InputMesh.m_MeshData->NodeList };
	m_MeshData->NodeNameList.Append(InputMesh.m_MeshData->NodeNameList.GetPointer(), InputMesh.m_MeshData->NodeNameList.GetLength());	
	m_MeshData->ElementList.Append(InputMesh.m_MeshData->ElementList.GetPointer(), InputMesh.m_MeshData->ElementList.GetLength());
	for (int_max k = ElementCount_init; k < m_MeshData->ElementList.GetLength(); ++k)
	{
		m_MeshData->ElementList[k] += NodeCount_init;
	}
	m_MeshData->ElementNameList.Append(InputMesh.m_MeshData->ElementNameList.GetPointer(), InputMesh.m_MeshData->ElementNameList.GetLength());
	m_MeshData->ElementTypeList.Append(InputMesh.m_MeshData->ElementTypeList.GetPointer(), InputMesh.m_MeshData->ElementTypeList.GetLength());	
	for (int_max k = 0; k < InputMesh.m_MeshData->NodeSet.GetLength(); ++k)
	{
		auto NodeSet_k = InputMesh.m_MeshData->NodeSet[k];
		NodeSet_k += NodeCount_init;
		this->AddNodeSet(InputMesh.m_MeshData->NodeSetName[k], NodeSet_k);		
	}
	for (int_max k = 0; k < InputMesh.m_MeshData->ElementSet.GetLength(); ++k)
	{
		auto ElementSet_k = InputMesh.m_MeshData->ElementSet[k];
		ElementSet_k += ElementCount_init;
		this->AddElementSet(InputMesh.m_MeshData->ElementSetName[k], ElementSet_k);		
	}
	//clear DataSet
	if (m_MeshData->NodeDataSet.IsEmpty() == false)
	{
		MDK_Warning("NodeDataSet is cleared @ FiniteElementMesh::Append(...)")
		m_MeshData->NodeDataSet.Clear();
		m_MeshData->NodeDataSetName.Clear();
	}
	if (m_MeshData->ElementDataSet.IsEmpty() == false)
	{
		MDK_Warning("ElementDataSet is cleared @ FiniteElementMesh::Append(...)")
		m_MeshData->ElementDataSet.Clear();
		m_MeshData->ElementDataSetName.Clear();
	}
}

}//namespace mdk
