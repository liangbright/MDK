#pragma once

#include "mdkDenseMatrix.h"
#include "mdkString.h"

namespace mdk
{

//=====================================================================================================//
// This class represent a FE mesh
// node, element, node-set, element-set, node-data-set, element-data-set can not be deleted after added
//=====================================================================================================//

//follow the definition in VTK and Abaqus
enum struct FiniteElementType
{
	UNKNOWN,
	VTK_TRIANGLE,
	VTK_QUAD,
	VTK_POLYGON,
	VTK_TETRA,
	VTK_WEDGE,
	VTK_HEXAHEDRON,
	VTK_CONVEX_POINT_SET,
	Abaqus_S3,
	Abaqus_S4,
	Abaqus_C3D4,
	Abaqus_C3D6,
	Abaqus_C3D8
};

template<typename ScalarType>
struct Data_Of_FiniteElementMesh
{
	int_max ID; // ID of the object
	String Name;// name of the object

	DenseMatrix<ScalarType>            NodeList;        //col = [x, y, z]
	ObjectArray<String>                NodeNameList;    // every node has a name (may be empty)

	ObjectArray<DenseVector<int_max>>  ElementList;     //{ Element1=[PointIndex1, PointIndex2, ...], ... }
	ObjectArray<String>                ElementNameList; // every element has a name (may be empty)
	ObjectArray<FiniteElementType>     ElementTypeList; // ElementTypeList[0] is the type of the first element ElementList[0]	

	ObjectArray<DenseVector<int_max>>  NodeSetList;        // NodeSetList[k] is NodeIndexList Of NodeSet-k
	ObjectArray<String>                NodeSetNameList;    // every node-set has a name (may be empty)

	ObjectArray<DenseVector<int_max>>  ElementSetList;     // ElementSetList[k] is ElementIndexList Of ElementSet-k
	ObjectArray<String>                ElementSetNameList; // every element-set has a name (may be empty)

	ObjectArray<DenseVector<DenseVector<ScalarType>>>  NodeDataSetList;        // NodeDataSetList[k][NodeIndex] ~ Data of a Node
	ObjectArray<String>                                NodeDataSetNameList;    //NodeDataSetNameList[k] is the name of NodeDataSetList[k]

	ObjectArray<DenseVector<DenseVector<ScalarType>>>  ElementDataSetList;     // ElementDataSetList[k][ElementIndex] ~ Data of an Element
	ObjectArray<String>                                ElementDataSetNameList; //ElementDataSetNameList[k] is the name of ElementDataSetList[k]
};


template<typename Scalar_Type>
class FiniteElementMesh : public Object
{
public: 
	typedef Scalar_Type ScalarType;

private:
	std::shared_ptr<Data_Of_FiniteElementMesh<ScalarType>> m_MeshData;

public:
	FiniteElementMesh();
	FiniteElementMesh(const FiniteElementMesh& Input);
	FiniteElementMesh(FiniteElementMesh&& Input);
	~FiniteElementMesh();
	void operator=(const FiniteElementMesh& Input);
	void operator=(FiniteElementMesh&& Input);
	void Copy(const FiniteElementMesh& Input);
	void Copy(FiniteElementMesh&& Input);	
	void Share(FiniteElementMesh& Input);
	void ForceShare(const FiniteElementMesh& Input);
	void Recreate();
	void Clear();
	bool IsEmpty() const;

	void SetID(int_max ID) { m_MeshData->ID = ID; }
	int_max GetID() const { return m_MeshData->ID; }
	void SetName(const String& Name) { m_MeshData->Name = Name; }
	String GetName() const { return m_MeshData->Name; }

	void SetCapacity(int_max NodeCount, int_max ElementCount);	
	void SetCapacity(int_max NodeCount, int_max ElementCount, int_max NodeSetCount, int_max ElementSetCount);

	void SetSize(int_max NodeCount, int_max ElementCount);

	void Construct(DenseMatrix<ScalarType> NodeList, ObjectArray<DenseVector<int_max>> ElementList);//unknown ElementType

	int_max AddNode(const DenseVector<ScalarType, 3>& Position);//return NodeIndex
	int_max AddNode(const ScalarType Position[3]);
	int_max AddNode(ScalarType x, ScalarType y, ScalarType z);
	DenseVector<int_max> AddNode_batch(const DenseMatrix<ScalarType>& PointSet);
	DenseVector<int_max> AddNode_batch(DenseMatrix<ScalarType>&& PointSet);
	void SetNode(int_max NodeIndex, const DenseVector<ScalarType, 3>& Position);
	void SetNode(int_max NodeIndex, const ScalarType Position[3]);
	void SetNode(int_max NodeIndex, ScalarType x, ScalarType y, ScalarType z);
	void SetNode_batch(const DenseVector<int_max>& NodeIndexList, const DenseMatrix<ScalarType>& PointSet);
	// can not use the same name for different node
	void SetNodeName(int_max NodeIndex, const String& NodeName);
	int_max GetNodeCount() const;
	int_max GetNodeIndex(const String& NodeName) const;
	String GetNodeName(int_max NodeIndex) const;
	DenseVector<ScalarType, 3> GetNode(int_max NodeIndex) const;
	DenseVector<ScalarType, 3> GetNode(const String& NodeName) const;
	DenseMatrix<ScalarType> GetNode(const DenseVector<int_max>& NodeIndexList) const;
	DenseMatrix<ScalarType> GetNode(const MDK_Symbol_ALL&) const;

	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement);//return ElementIndex, Type is UNKNOWN
	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement, FiniteElementType Type);//return ElementIndex
	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement, const String& ElementType);//return ElementIndex
	void SetElement(int_max ElementIndex, const DenseVector<int_max>& NodeIndexListOfElement);
	// can not use the same name for element
	void SetElementName(int_max ElementIndex, const String& ElementName);	
	inline void SetElementType(int_max ElementIndex, FiniteElementType Type);
	inline void SetElementType(int_max ElementIndex, const String& Type);	
	int_max GetElementCount() const;
	int_max GetElementIndex(const String& ElementName) const;
	String GetElementName(int_max ElementIndex) const;
	DenseVector<int_max> GetElement(int_max ElementIndex) const;
	DenseVector<int_max> GetElement(const String& ElementName) const;	
	ObjectArray<DenseVector<int_max>> GetElement(const DenseVector<int_max>& ElementIndexList) const;
	ObjectArray<DenseVector<int_max>> GetElement(const MDK_Symbol_ALL&) const;
	FiniteElementType GetElementType(int_max ElementIndex) const;
	String GetElementTypeAsString(int_max ElementIndex) const;
	String ConvertElementTypeToString(FiniteElementType Type) const;
	String GetElementTypeAsString_Abaqus(int_max ElementIndex) const;	
	String ConvertElementTypeToString_Abaqus(FiniteElementType Type) const;
	FiniteElementType ConvertStringToElementType(const String& Type) const;

	// can not use the same name for different node-set
	int_max AddNodeSet(const String& NodeSetName);//empty node-set
	int_max AddNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	void SetNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	void SetNodeSet(int_max NodeSetIndex, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	void ChangeNodeSetName(const String& NodeSetName_old, const String& NodeSetName_new);
	void SetNodeSetName(int_max NodeSetIndex, const String& NodeSetName);
	int_max GetNodeSetCount() const;
	String GetNodeSetName(int_max NodeSetIndex) const;
	int_max GetNodeSetIndex(const String& NodeSetName) const;
	DenseVector<int_max> GetNodeSet(int_max NodeSetIndex) const;
	DenseVector<int_max> GetNodeSet(const String& NodeSetName) const;

	// can not use the same name for different node-set or element-set
	int_max AddElementSet(const String& ElementSetName);//empty element-set
	int_max AddElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet);
	void SetElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet);
	void SetElementSet(int_max ElementSetIndex, const DenseVector<int_max>& ElementIndexListOfElementSet);
	void ChangeElementSetName(const String& ElementSetName_old, const String& ElementSetName_new);
	void SetElementSetName(int_max ElementSetIndex, const String& ElementSetName);
	int_max GetElementSetCount() const;	
	String GetElementSetName(int_max ElementSetIndex) const;
	int_max GetElementSetIndex(const String& ElementSetName) const;	
	DenseVector<int_max> GetElementSet(int_max ElementSetIndex) const;
	DenseVector<int_max> GetElementSet(const String& ElementSetName) const;

	int_max InitializeNodeDataSet(const String& Name);//return DataSetIndex or -1
	int_max GetNodeDataSetCount() const;
	int_max GetNodeDataSetIndex(const String& Name) const;
	String GetNodeDataSetName(int_max DataSetIndex) const;
	void SetNodeDataSet(const String& Name, DenseVector<DenseVector<ScalarType>> DataSet);
	void SetNodeDataSet(int_max DataSetIndex, DenseVector<DenseVector<ScalarType>> DataSet);
	DenseVector<DenseVector<ScalarType>> GetNodeDataSet(const String& Name) const;
	DenseVector<DenseVector<ScalarType>> GetNodeDataSet(int_max DataSetIndex) const;

	int_max InitializeElementDataSet(const String& Name);//return DataSetIndex or -1
	int_max GetElementDataSetCount() const;
	int_max GetElementDataSetIndex(const String& Name) const;
	String GetElementDataSetName(int_max DataSetIndex) const;
	void SetElementDataSet(const String& Name, DenseVector<DenseVector<ScalarType>> DataSet);
	void SetElementDataSet(int_max DataSetIndex, DenseVector<DenseVector<ScalarType>> DataSet);
	DenseVector<DenseVector<ScalarType>> GetElementDataSet(const String& Name) const;
	DenseVector<DenseVector<ScalarType>> GetElementDataSet(int_max DataSetIndex) const;

	bool IsShellElement(int_max ElementIndex) const;
	bool IsSolidElement(int_max ElementIndex) const;

	bool IsShellMesh() const;
	bool IsSolidMesh() const;	

	FiniteElementMesh<ScalarType> GetSubMesh(const DenseVector<int_max>& ElementIndexList) const;
};


}//namespace mdk

#include "mdkFiniteElementMesh.hpp"
#include "mdkFiniteElementMesh_FileIO.h"
