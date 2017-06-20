#include "mdkFiniteElementMesh.h"
#include "mdkPolygonMesh.h"

using namespace mdk;

void Test_CreateMesh()
{
	String FilePath = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_FiniteElementMesh/TestData/";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePath + "Leaflet.vtk");
	
	DenseMatrix<double> NodeList;
	ObjectArray<DenseVector<int_max>> ElementList;

	InputMesh.GetPointPositionMatrixAndFaceTable(NodeList, ElementList);

	FiniteElementMesh<double> MeshA, MeshB;

	MeshA.Construct(NodeList, ElementList);
	for (int_max k = 0; k < MeshA.GetElementCount(); ++k)
	{
		MeshA.SetElementType(k, "S3");
	}

	MeshA.SetID(1);
	MeshA.SetName("Leaflet");
	
	MeshA.SetNodeName(0, "Node_0_Name");
	MeshA.SetNodeName(1, "Node_1_Name");
	MeshA.SetElementName(0, "Element_0_Name");
	MeshA.SetElementName(1, "Element_1_Name");
	
	MeshA.AddNodeSet("0_NodeSet", { 1, 2, 3 });
	MeshA.AddNodeSet("1_NodeSet", { 4, 5, 6 });

	MeshA.SetNodeSet("0_NodeSet", { 1, 2, 3 });
	MeshA.SetNodeSet("1_NodeSet", { 4, 5, 6 });

	MeshA.AddElementSet("0_ElementSet", { 1, 2, 3 });
	MeshA.AddElementSet("1_ElementSet", { 4, 5, 6 });

	MeshA.SetElementSet("0_ElementSet", { 1, 2, 3 });
	MeshA.SetElementSet("1_ElementSet", { 4, 5, 6 });

	SaveFiniteElementMeshAsJsonDataFile(MeshA, FilePath + "_MeshA.json");

	SaveFiniteElementMeshAsAbaqusINPFile(MeshA, FilePath + "_MeshA.inp");

	LoadFiniteElementMeshFromJsonDataFile(MeshB, FilePath + "_MeshA.json");

	auto Set1 = MeshB.GetNodeSet("0_NodeSet");
	auto Set2 = MeshB.GetNodeSet("1_PointSet");

	auto Set3 = MeshB.GetElementSet("0_ElementSet");
	auto Set4 = MeshB.GetElementSet("1_FaceSet");
}