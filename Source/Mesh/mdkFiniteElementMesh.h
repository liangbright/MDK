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
	VTK_PYRAMID,
	VTK_WEDGE,
	VTK_HEXAHEDRON,
	VTK_CONVEX_POINT_SET,
	Abaqus_S3,
	Abaqus_S4,
	Abaqus_C3D4,
	Abaqus_C3D5,
	Abaqus_C3D6,
	Abaqus_C3D8
};

template<typename ScalarType>
struct Data_Of_FiniteElementMesh
{
	String Name;// name of the object

	DenseMatrix<ScalarType> NodeList;        //col = [x, y, z]
	ObjectArray<String>     NodeNameList;    // every node has a name (may be empty)

	ObjectArray<DenseVector<int_max>> ElementList;     //{ Element1=[PointIndex1, PointIndex2, ...], ... }
	ObjectArray<String>               ElementNameList; // every element has a name (may be empty)
	ObjectArray<FiniteElementType>    ElementTypeList; // ElementTypeList[0] is the type of the first element ElementList[0]	

	ObjectArray<DenseMatrix<ScalarType>> NodeDataSet;        // NodeDataSet[k][NodeIndex] ~ Data of a Node
	ObjectArray<String>                  NodeDataSetName;    // NodeDataSetName[k] is the name of NodeDataSet[k]

	ObjectArray<DenseMatrix<ScalarType>> ElementDataSet;     // ElementDataSet[k][ElementIndex] ~ Data of an Element
	ObjectArray<String>                  ElementDataSetName; // ElementDataSetName[k] is the name of ElementDataSet[k]

	ObjectArray<DenseVector<int_max>> NodeSet;        // NodeSet[k] is NodeIndexList Of NodeSet-k
	ObjectArray<String>               NodeSetName;    // every node-set has a name (may be empty)

	ObjectArray<DenseVector<int_max>> ElementSet;     // ElementSet[k] is ElementIndexList Of ElementSet-k
	ObjectArray<String>               ElementSetName; // every element-set has a name (may be empty)	
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

	void SetName(const String& Name) { m_MeshData->Name = Name; }
	String GetName() const { return m_MeshData->Name; }

	void SetCapacity(int_max NodeCount, int_max ElementCount);

	void Resize(int_max NodeCount, int_max ElementCount);
	//Resize(...,...) then use SetNode() and SetElement()
	 
	void Construct(DenseMatrix<ScalarType> NodeList, ObjectArray<DenseVector<int_max>> ElementList);//unknown ElementType

	int_max AddNode(const DenseVector<ScalarType, 3>& Position);//return NodeIndex
	int_max AddNode(const ScalarType Position[3]);
	int_max AddNode(ScalarType x, ScalarType y, ScalarType z);
	DenseVector<int_max> AddNode_batch(const DenseMatrix<ScalarType>& PointSet);
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

	int_max AddElement(FiniteElementType ElementType, const DenseVector<int_max>& NodeIndexList);//return ElementIndex
	int_max AddElement(const String& ElementType, const DenseVector<int_max>& NodeIndexList);//return ElementIndex
	void SetElement(int_max ElementIndex, const DenseVector<int_max>& NodeIndexLis);
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

	int_max AddNodeDataSet(const String& Name, int_max ScalarCountPerNode);//return DataSetIndex or -1
	int_max AddNodeDataSet(const String& Name, DenseMatrix<ScalarType> DataSet);
	int_max GetNodeDataSetCount() const;
	int_max GetNodeDataSetIndex(const String& Name) const;
	String GetNodeDataSetName(int_max Index) const;
	void SetNodeDataSet(const String& Name, DenseMatrix<ScalarType> DataSet);
	void SetNodeDataSet(int_max Index, DenseMatrix<ScalarType> DataSet);
	const DenseMatrix<ScalarType>& GetNodeDataSet(const String& Name) const;
	const DenseMatrix<ScalarType>& GetNodeDataSet(int_max Index) const;

	int_max AddElementDataSet(const String& Name, int_max ScalarCountPerElement);//return DataSetIndex or -1
	int_max AddElementDataSet(const String& Name, DenseMatrix<ScalarType> DataSet);
	int_max GetElementDataSetCount() const;
	int_max GetElementDataSetIndex(const String& Name) const;
	String GetElementDataSetName(int_max Index) const;
	void SetElementDataSet(const String& Name, DenseMatrix<ScalarType> DataSet);
	void SetElementDataSet(int_max Index, DenseMatrix<ScalarType> DataSet);
	const DenseMatrix<ScalarType>& GetElementDataSet(const String& Name) const;
	const DenseMatrix<ScalarType>& GetElementDataSet(int_max Index) const;

	// can not use the same name for different node-set
	int_max AddNodeSet(const String& Name);//empty node-set
	int_max AddNodeSet(const String& Name, const DenseVector<int_max>& NodeIndexList);
	void SetNodeSet(const String& Name, const DenseVector<int_max>& NodeIndexList);
	void SetNodeSet(int_max Index, const DenseVector<int_max>& NodeIndexList);	
	void SetNodeSetName(int_max Index, const String& Name);
	int_max GetNodeSetCount() const;
	String GetNodeSetName(int_max Index) const;
	int_max GetNodeSetIndex(const String& Name) const;
	DenseVector<int_max> GetNodeSet(int_max Index) const;
	DenseVector<int_max> GetNodeSet(const String& Name) const;

	// can not use the same name for different node-set or element-set
	int_max AddElementSet(const String& Name);//empty element-set
	int_max AddElementSet(const String& Name, const DenseVector<int_max>& ElementIndexList);
	void SetElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexList);
	void SetElementSet(int_max Index, const DenseVector<int_max>& ElementIndexList);	
	void SetElementSetName(int_max Index, const String& Name);
	int_max GetElementSetCount() const;
	String GetElementSetName(int_max Index) const;
	int_max GetElementSetIndex(const String& Name) const;
	DenseVector<int_max> GetElementSet(int_max Index) const;
	DenseVector<int_max> GetElementSet(const String& Name) const;

	bool IsShellElement(int_max ElementIndex) const;
	bool IsSolidElement(int_max ElementIndex) const;

	bool IsShellMesh() const;
	bool IsSolidMesh() const;	

	FiniteElementMesh<ScalarType> GetSubMeshByElement(const DenseVector<int_max>& ElementIndexList) const;

	void Append(const FiniteElementMesh<ScalarType>& InputMesh);
};


}//namespace mdk

#include "mdkFiniteElementMesh.hpp"
#include "mdkFiniteElementMesh_FileIO.h"
