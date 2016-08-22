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
	m_MeshData->NodeSetList = Input.m_MeshData->NodeSetList;	
	m_MeshData->NodeSetNameList = Input.m_MeshData->NodeSetNameList;
	m_MeshData->ElementSetList = Input.m_MeshData->ElementSetList;
	m_MeshData->ElementSetNameList = Input.m_MeshData->ElementSetNameList;
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
	m_MeshData->NodeList = std:move(Input.m_MeshData->NodeList);
	m_MeshData->NodeNameList = std::move(Input.m_MeshData->NodeNameList);
	m_MeshData->ElementList = std::move(Input.m_MeshData->ElementList);
	m_MeshData->ElementNameList = std::move(Input.m_MeshData->ElementNameList);
	m_MeshData->ElementTypeList = std::move(Input.m_MeshData->ElementTypeList);
	m_MeshData->NodeSetList = std::move(Input.m_MeshData->NodeSetList);
	m_MeshData->NodeSetNameList = std::move(Input.m_MeshData->NodeSetNameList);
	m_MeshData->ElementSetList = std::move(Input.m_MeshData->ElementSetList);
	m_MeshData->ElementSetNameList = std::move(Input.m_MeshData->ElementSetNameList);

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
		m_MeshData->NodeSetList.Clear();
		m_MeshData->NodeSetNameList.Clear();
		m_MeshData->ElementSetList.Clear();
		m_MeshData->ElementSetNameList.Clear();
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
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetCapacity(int_max NodeCount, int_max ElementCount, int_max NodeSetCount, int_max ElementSetCount)
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
	m_MeshData->NodeSetList.SetCapacity(NodeSetCount);
	m_MeshData->NodeSetNameList.SetCapacity(NodeSetCount);
	m_MeshData->ElementSetList.SetCapacity(ElementSetCount);
	m_MeshData->ElementSetNameList.SetCapacity(ElementSetCount);
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
DenseVector<int_max> FiniteElementMesh<ScalarType>::AddNode_batch(DenseMatrix<ScalarType>&& PointSet)
{
	if (PointSet.IsVector() == true)
	{
		DenseVector<int_max> NodeIndexList;
		if (PointSet.GetElementCount() != 3)
		{
			MDK_Error("PointSet is a vector but length != 3 @ FiniteElementMesh::AddPoint_batch(...)")
			return NodeIndexList;
		}
		NodeIndexList.Resize(1);
		NodeIndexList[0] = this->AddPoint(PointSet.GetPointer());
		return NodeIndexList;
	}

	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("PointSet is a matrix but RowCount != 3 @ FiniteElementMesh::AddPoint_batch(...)")
		DenseVector<int_max> NodeIndexList;
		return NodeIndexList;
	}

	if (m_MeshData->NodeList.IsEmpty() == true)
	{
		m_MeshData->NodeList = std::move(PointSet);
		DenseVector<int_max> NodeIndexList = span(0, PointSet.GetColCount() - 1);
		return NodeIndexList;
	}
	else
	{
		return this->AddNode_batch(PointSet);
	}
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElement(const DenseVector<int_max>& NodeIndexListOfElement)
{
	return this->AddElement(NodeIndexListOfElement, FiniteElementType::UNKNOWN);
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElement(const DenseVector<int_max>& NodeIndexListOfElement, FiniteElementType Type)
{
	m_MeshData->ElementList.Append(NodeIndexListOfElement);
	m_MeshData->ElementTypeList.Append(Type);
	String EmptyName;
	m_MeshData->ElementNameList.Append(EmptyName);
	return m_MeshData->ElementList.GetLength()-1;
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElement(const DenseVector<int_max>& NodeIndexListOfElement, const String& ElementType)
{
	auto Type = this->ConvertStringToElementType(ElementType);
	return this->AddElement(NodeIndexListOfElement, Type);
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetSize(int_max NodeCount, int_max ElementCount)
{
	if (!m_MeshData)
	{
		m_MeshData = std::make_shared<Data_Of_FiniteElementMesh<ScalarType>>();
	}
	m_MeshData->NodeList.SetSize(3, NodeCount);
	m_MeshData->NodeNameList.SetSize(NodeCount);
	m_MeshData->ElementList.SetSize(ElementCount);
	m_MeshData->ElementNameList.SetSize(ElementCount);
	m_MeshData->ElementTypeList.SetSize(ElementCount);
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
	m_MeshData->NodeList.SetCol(NodeIndex, {x, y, z});
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
void FiniteElementMesh<ScalarType>::SetElement(int_max ElementIndex, const DenseVector<int_max>& NodeIndexListOfElement)
{
	m_MeshData->ElementList[ElementIndex] = NodeIndexListOfElement;
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeName(int_max NodeIndex, const String& NodeName)
{
	m_MeshData->NodeNameList[NodeIndex] = NodeName;
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
int_max FiniteElementMesh<ScalarType>::AddNodeSet(const String& NodeSetName)
{
	if (this->GetNodeSetIndex(NodeSetName) < 0)
	{
		DenseVector<int_max> EmptyList;
		m_MeshData->NodeSetList.Append(EmptyList);
		m_MeshData->NodeSetNameList.Append(NodeSetName);
		return m_MeshData->NodeSetList.GetLength() - 1;
	}
	else
	{
		MDK_Error("NodeSetName already exist @ FiniteElementMesh::AddNodeSet()")
		return -1;
	}
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeIndexListOfNodeSet)
{
	if (this->GetNodeSetIndex(NodeSetName) < 0)
	{
		m_MeshData->NodeSetList.Append(NodeIndexListOfNodeSet);
		m_MeshData->NodeSetNameList.Append(NodeSetName);
		return m_MeshData->NodeSetList.GetLength() - 1;
	}
	else
	{
		MDK_Error("NodeSetName already exist @ FiniteElementMesh::AddNodeSet()")
		return -1;
	}	
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementSet(const String& ElementSetName)
{
	if (this->GetElementSetIndex(ElementSetName) < 0)
	{
		DenseVector<int_max> EmptyList;
		m_MeshData->ElementSetList.Append(EmptyList);		
		m_MeshData->ElementSetNameList.Append(ElementSetName);
		return m_MeshData->ElementSetList.GetLength() - 1;
	}
	else
	{
		MDK_Error("ElementSetName already exist @ FiniteElementMesh::AddElementSet()")
		return -1;
	}
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::AddElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet)
{
	if (this->GetElementSetIndex(ElementSetName) < 0)
	{
		m_MeshData->ElementSetList.Append(ElementIndexListOfElementSet);
		m_MeshData->ElementSetNameList.Append(ElementSetName);
		return m_MeshData->ElementSetList.GetLength() - 1;
	}
	else
	{
		MDK_Error("ElementSetName already exist @ FiniteElementMesh::AddElementSet()")
		return -1;
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeSet)
{
	auto NodeSetIndex = this->GetNodeSetIndex(NodeSetName);
	if (NodeSetIndex >= 0)
	{
		m_MeshData->NodeSetList[NodeSetIndex] = NodeSet;
		m_MeshData->NodeSetNameList[NodeSetIndex] = NodeSetName;
	}
	else
	{
		MDK_Error("NodeSet NOT exist @ FiniteElementMesh::SetNodeSet()")
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSet(int_max NodeSetIndex, const DenseVector<int_max>& NodeSet)
{
	m_MeshData->NodeSetList[NodeSetIndex] = NodeSet;
	m_MeshData->NodeSetNameList[NodeSetIndex] = ElementName;
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet)
{
	auto ElementSetIndex = this->GetElementSetIndex(ElementSetName);
	if (ElementSetIndex >= 0)
	{
		m_MeshData->ElementSetList[ElementSetIndex] = ElementIndexListOfElementSet;
		m_MeshData->ElementSetNameList[ElementSetIndex] = ElementSetName;
	}
	else
	{
		MDK_Error("NodeSet NOT exist @ FiniteElementMesh::SetNodeSet()")
	}
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSet(int_max ElementSetIndex, const DenseVector<int_max>& ElementIndexListOfElementSet)
{
	m_MeshData->ElementSetList[ElementSetIndex] = ElementIndexListOfElementSet;
	m_MeshData->ElementSetNameList[ElementSetIndex] = ElementSetName;
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::ChangeNodeSetName(const String& NodeSetName_old, const String& NodeSetName_new)
{
	this->SetNodeSetName(this->GetNodeSetIndex(NodeSetName_old), NodeSetName_new);
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetNodeSetName(int_max NodeSetIndex, const String& NodeSetName)
{
	m_MeshData->NodeSetNameList[NodeSetIndex] = NodeSetName;
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::ChangeElementSetName(const String& ElementSetName_old, const String& ElementSetName_new)
{
	this->SetElementSetName(this->GetElementSetIndex(ElementSetName_old), ElementSetName_new);
}


template<typename ScalarType>
void FiniteElementMesh<ScalarType>::SetElementSetName(int_max ElementSetIndex, const String& ElementSetName)
{
	m_MeshData->ElementSetNameList[ElementSetIndex] = ElementSetName;
}


template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeCount() const
{
	return m_MeshData->NodeList.GetColCount();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementCount() const
{
	return m_MeshData->ElementList.GetLength();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetNodeSetCount() const
{
	return m_MeshData->NodeSetList.GetLength();
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementSetCount() const
{
	return m_MeshData->ElementSetList.GetLength();
}

template<typename ScalarType>
DenseMatrix<ScalarType> FiniteElementMesh<ScalarType>::GetNode(const MDK_Symbol_ALL&) const
{
	return m_MeshData->NodeList;
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
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetNodeSet(int_max NodeSetIndex) const
{
	return m_MeshData->NodeSetList[NodeSetIndex];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetNodeSet(const String& NodeSetName) const
{
	auto NodeSetIndex = this->GetNodeSetIndex(NodeSetName);
	return m_MeshData->NodeSetList[NodeSetIndex];
}

template<typename ScalarType>
ObjectArray<DenseVector<int_max>> FiniteElementMesh<ScalarType>::GetElement(const MDK_Symbol_ALL&) const
{
	return m_MeshData->ElementList;
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
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElementSet(int_max ElementSetIndex) const
{
	return m_MeshData->ElementSetList[ElementSetIndex];
}

template<typename ScalarType>
DenseVector<int_max> FiniteElementMesh<ScalarType>::GetElementSet(const String& ElementSetName) const
{
	auto ElementSetIndex = this->GetElementSetIndex(ElementSetName);
	return m_MeshData->ElementSetList[ElementSetIndex];
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
int_max FiniteElementMesh<ScalarType>::GetNodeSetIndex(const String& NodeSetName) const
{
	for (int_max k = 0; k < m_MeshData->NodeSetNameList.GetLength(); ++k)
	{
		if (m_MeshData->NodeSetNameList[k] == NodeSetName)
		{
			return k;
		}
	}
	return -1;
}

template<typename ScalarType>
int_max FiniteElementMesh<ScalarType>::GetElementSetIndex(const String& ElementSetName) const
{
	for (int_max k = 0; k < m_MeshData->ElementSetNameList.GetLength(); ++k)
	{
		if (m_MeshData->ElementSetNameList[k] == ElementSetName)
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
String FiniteElementMesh<ScalarType>::GetElementName(int_max ElementIndex) const
{
	return m_MeshData->ElementNameList[ElementIndex];
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetNodeSetName(int_max NodeSetIndex) const
{
	return m_MeshData->NodeSetNameList[NodeSetIndex];
}

template<typename ScalarType>
String FiniteElementMesh<ScalarType>::GetElementSetName(int_max ElementSetIndex) const
{
	return m_MeshData->ElementSetNameList[ElementSetIndex];
}


template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsShellElement(int_max ElementIndex) const
{
	auto Type = this->GetElementType(ElementIndex);
	switch (Type)
	{
	case FiniteElementType::S3:
		return true;
	case FiniteElementType::S3R:
		return true;
	case FiniteElementType::S4:
		return true;
	case FiniteElementType::S4R:
		return true;
	case FiniteElementType::C3D4:
		return false;
	case FiniteElementType::C3D4R:
		return false;
	case FiniteElementType::C3D6:
		return false;
	case FiniteElementType::C3D6R:
		return false;
	case FiniteElementType::C3D8:
		return false;
	case FiniteElementType::C3D8R:
		return false;
	default:
		return false;
	}
}

template<typename ScalarType>
bool FiniteElementMesh<ScalarType>::IsSolidElement(int_max ElementIndex) const
{
	auto Type = this->GetElementType(ElementIndex);
	switch (Type)
	{
	case FiniteElementType::S3:
		return false;
	case FiniteElementType::S3R:
		return false;
	case FiniteElementType::S4:
		return false;
	case FiniteElementType::S4R:
		return false;
	case FiniteElementType::C3D4:
		return true;
	case FiniteElementType::C3D4R:
		return true;
	case FiniteElementType::C3D6:
		return true;
	case FiniteElementType::C3D6R:
		return true;
	case FiniteElementType::C3D8:
		return true;
	case FiniteElementType::C3D8R:
		return true;
	default:
		return false;
	}
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
	case FiniteElementType::S3:
		ElementTypeName = "S3";
		break;
	case FiniteElementType::S3R:
		ElementTypeName = "S3R";
		break;
	case FiniteElementType::S4:
		ElementTypeName = "S4";
		break;
	case FiniteElementType::S4R:
		ElementTypeName = "S4R";
		break;
	case FiniteElementType::C3D4:
		ElementTypeName = "C3D4";
		break;
	case FiniteElementType::C3D4R:
		ElementTypeName = "C3D4R";
		break;
	case FiniteElementType::C3D6:
		ElementTypeName = "C3D6";
		break;
	case FiniteElementType::C3D6R:
		ElementTypeName = "C3D6R";
		break;
	case FiniteElementType::C3D8:
		ElementTypeName = "C3D8";
		break;
	case FiniteElementType::C3D8R:
		ElementTypeName = "C3D8R";
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
    if (Type == "S3")
	{
		ElementType = FiniteElementType::S3;
	}
	else if (Type == "S3R")
	{
		ElementType = FiniteElementType::S3R;
	}
	else if (Type == "S4")
	{
		ElementType = FiniteElementType::S4;
	}
	else if (Type == "S4R")
	{
		ElementType = FiniteElementType::S4R;
	}
	else if (Type == "C3D4")
	{
		ElementType = FiniteElementType::C3D4;
	}
	else if (Type == "C3D4R")
	{
		ElementType = FiniteElementType::C3D4R;
	}
	else if (Type == "C3D6")
	{
		ElementType = FiniteElementType::C3D6;
	}
	else if (Type == "C3D6R")
	{
		ElementType = FiniteElementType::C3D6R;
	}
	else if (Type == "C3D8")
	{
		ElementType = FiniteElementType::C3D8;
	}
	else if (Type == "C3D8R")
	{
		ElementType = FiniteElementType::C3D8R;
	}
	else
	{
		ElementType = FiniteElementType::UNKNOWN;
	}
	return ElementType;
}


template<typename ScalarType>
FiniteElementMesh<ScalarType> FiniteElementMesh<ScalarType>::GetSubMesh(const DenseVector<int_max>& ElementIndexList) const
{
	FiniteElementMesh<ScalarType> SubMesh;
	if (ElementIndexList.IsEmpty() == true || this->IsEmpty() == true)
	{
		return SubMesh;
	}

	SubMesh.m_MeshData->ElementList = m_MeshData->ElementList.GetSubSet(ElementIndexList);
	SubMesh.m_MeshData->ElementTypeList = m_MeshData->ElementTypeList.GetSubSet(ElementIndexList);
	SubMesh.m_MeshData->ElementNameList = m_MeshData->ElementNameList.GetSubSet(ElementIndexList);

	DenseVector<int_max> PointIndexMap_Old_to_New;// old -> new
	PointIndexMap_Old_to_New = span(0, m_MeshData->NodeList.GetColCount() - 1);

	DenseVector<int_max> PointIndexMap_New_to_Old;
	PointIndexMap_New_to_Old.SetCapacity(m_MeshData->NodeList.GetColCount());
	
	DenseVector<int_max> PointFlagList;
	PointFlagList.Resize(m_MeshData->NodeList.GetColCount());
	PointFlagList.Fill(0);//0: not used, 1: used in SubMesh

	int_max NewPointIndex = -1;
	for (int_max k = 0; k < SubMesh.m_MeshData->ElementList.GetLength(); ++k)
	{
		auto& PointIndexList = SubMesh.m_MeshData->ElementList[k];
		for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
		{
			auto PointIndex = PointIndexList[n];
			if (PointFlagList[PointIndex] == 0)
			{
				PointFlagList[PointIndex] = 1;
				NewPointIndex += 1;
				PointIndexMap_Old_to_New[PointIndex] = NewPointIndex;
				PointIndexMap_New_to_Old.Append(PointIndex);
			}
		}
	}

	SubMesh.m_MeshData->NodeList.Resize(3, PointIndexMap_New_to_Old.GetLength());
	SubMesh.m_MeshData->NodeNameList.Resize(PointIndexMap_New_to_Old.GetLength());
	for (int_max k = 0; k < PointIndexMap_New_to_Old.GetLength(); ++k)
	{
		int_max OldPointIndex = PointIndexMap_New_to_Old[k];
		SubMesh.m_MeshData->NodeList.SetCol(k, m_MeshData->NodeList.GetPointerOfCol(OldPointIndex));
		SubMesh.m_MeshData->NodeNameList[k] = m_MeshData->NodeNameList[OldPointIndex];
	}

	for (int_max k = 0; k < SubMesh.m_MeshData->ElementList.GetLength(); ++k)
	{
		//take reference
		auto& PointIndexList = SubMesh.m_MeshData->ElementList[k];
		for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
		{
			auto PointIndex = PointIndexList[n];
			auto NewPointIndex = PointIndexMap_Old_to_New[PointIndex];
			PointIndexList[n] = NewPointIndex;
		}
	}

	return SubMesh;
}

}//namespace mdk
