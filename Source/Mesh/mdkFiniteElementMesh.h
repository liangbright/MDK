#pragma once

#include "mdkDenseMatrix.h"
#include "mdkString.h"

namespace mdk
{

//========================================================================//
// This class represent a FE mesh
// node and element can not be deleted
//
//========================================================================//

//follow the definition in Abaqus
enum struct FiniteElementType
{
	UNKNOWN,
	S3,
	S3R,
	S4,
	S4R,
	C3D4,
	C3D4R,
	C3D6,
	C3D6R,
	C3D8,
	C3D8R,
};

template<typename ScalarType>
struct Data_Of_FiniteElementMesh
{
	String Name;// name of the object

	DenseMatrix<ScalarType>            NodeList;        //col = [x, y, z]
	ObjectArray<DenseVector<int_max>>  ElementList;     //{ Element1=[PointIndex1, PointIndex2, ...], ... }
	ObjectArray<FiniteElementType>     ElementTypeList; // ElementTypeList[0] is the type of the first element ElementList[0]

	ObjectArray<DenseVector<int_max>>  NodeSetList;   // NodeSetList[k] is NodeIndexList Of NodeSet-k
	ObjectArray<DenseVector<int_max>>  ElementSetList;// ElementSetList[k] is ElementIndexList Of ElementSet-k

	ObjectArray<String> NodeNameList;       // every node has a name (may be empty)
	ObjectArray<String> ElementNameList;    // every element has a name (may be empty)
	ObjectArray<String> NodeSetNameList;    // every node-set has a name (may be empty)
	ObjectArray<String> ElementSetNameList; // every element-set has a name (may be empty)
};


template<typename Scalar_Type>
class FiniteElementMesh : public mdk::Object
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
	void SetCapacity(int_max NodeCount, int_max ElementCount, int_max NodeSetCount, int_max ElementSetCount);

	void Construct(DenseMatrix<ScalarType> NodeList, ObjectArray<DenseVector<int_max>> ElementList);

	int_max AddNode(const DenseVector<ScalarType, 3>& Position);//return NodeIndex
	int_max AddNode(const ScalarType Position[3]);
	int_max AddNode(ScalarType x, ScalarType y, ScalarType z);
	DenseVector<int_max> AddNode_batch(const DenseMatrix<ScalarType>& PointSet);
	DenseVector<int_max> AddNode_batch(DenseMatrix<ScalarType>&& PointSet);
	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement);//return ElementIndex, Type is UNKNOWN
	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement, FiniteElementType Type);//return ElementIndex
	int_max AddElement(const DenseVector<int_max>& NodeIndexListOfElement, const String& ElementType);//return ElementIndex

	void SetSize(int_max NodeCount, int_max ElementCount);
	void SetNode(int_max NodeIndex, const DenseVector<ScalarType, 3>& Position);
	void SetNode(int_max NodeIndex, const ScalarType Position[3]);
	void SetNode(int_max NodeIndex, ScalarType x, ScalarType y, ScalarType z);
	void SetNode_batch(const DenseVector<int_max>& NodeIndexList, const DenseMatrix<ScalarType>& PointSet);
	void SetElement(int_max ElementIndex, const DenseVector<int_max>& NodeIndexListOfElement);

	// can not use the same name for different node or element
	void SetNodeName(int_max NodeIndex, const String& NodeName);
	void SetElementName(int_max ElementIndex, const String& ElementName);
	inline void SetElementType(int_max ElementIndex, FiniteElementType Type);
	inline void SetElementType(int_max ElementIndex, const String& Type);

	// can not use the same name for different node-set or element-set
	int_max AddNodeSet(const String& NodeSetName);//empty node-set
	int_max AddNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	int_max AddElementSet(const String& ElementSetName);//empty element-set
	int_max AddElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet);

	//modify existing node-set, element-set
	void SetNodeSet(const String& NodeSetName, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	void SetNodeSet(int_max NodeSetIndex, const DenseVector<int_max>& NodeIndexListOfNodeSet);
	void SetElementSet(const String& ElementSetName, const DenseVector<int_max>& ElementIndexListOfElementSet);
	void SetElementSet(int_max ElementSetIndex, const DenseVector<int_max>& ElementIndexListOfElementSet);

	//change node-set name or element-set name
	void ChangeNodeSetName(const String& NodeSetName_old, const String& NodeSetName_new);
	void SetNodeSetName(int_max NodeSetIndex, const String& NodeSetName);
	void ChangeElementSetName(const String& ElementSetName_old, const String& ElementSetName_new);
	void SetElementSetName(int_max ElementSetIndex, const String& ElementSetName);

	int_max GetNodeCount() const;
	int_max GetElementCount() const;
	int_max GetNodeSetCount() const;
	int_max GetElementSetCount() const;

	DenseMatrix<ScalarType> GetNode(const MDK_Symbol_ALL&) const;
	DenseVector<ScalarType, 3> GetNode(int_max NodeIndex) const;
	DenseVector<ScalarType, 3> GetNode(const String& NodeName) const;
	DenseMatrix<ScalarType> GetNode(const DenseVector<int_max>& NodeIndexList) const;	
	DenseVector<int_max> GetNodeSet(int_max NodeSetIndex) const;
	DenseVector<int_max> GetNodeSet(const String& NodeSetName) const;

	ObjectArray<DenseVector<int_max>> GetElement(const MDK_Symbol_ALL&) const;
	DenseVector<int_max> GetElement(int_max ElementIndex) const;
	DenseVector<int_max> GetElement(const String& ElementName) const;
	DenseVector<int_max> GetElementSet(int_max ElementSetIndex) const;
	DenseVector<int_max> GetElementSet(const String& ElementSetName) const;

	int_max GetNodeIndex(const String& NodeName) const;
	int_max GetElementIndex(const String& ElementName) const;
	int_max GetNodeSetIndex(const String& NodeSetName) const;
	int_max GetElementSetIndex(const String& ElementSetName) const;

	String GetNodeName(int_max NodeIndex) const;
	String GetElementName(int_max ElementIndex) const;
	String GetNodeSetName(int_max NodeSetIndex) const;
	String GetElementSetName(int_max ElementSetIndex) const;

	bool IsShellElement(int_max ElementIndex) const;
	bool IsBrickElement(int_max ElementIndex) const;

	FiniteElementType GetElementType(int_max ElementIndex) const;
	String GetElementTypeAsString(int_max ElementIndex) const;
	
	String ConvertElementTypeToString(FiniteElementType Type) const;
	FiniteElementType ConvertStringToElementType(const String& Type) const;

	FiniteElementMesh<ScalarType> GetSubMesh(const DenseVector<int_max>& ElementIndexList) const;
};


}//namespace mdk

#include "mdkFiniteElementMesh.hpp"
#include "mdkFiniteElementMesh_FileIO.h"
