#include "mdkFileIO.h"
#include "mdkTetrahedronMesh.h"

using namespace mdk;

void Test_MeshConstruction()
{
	TetrahedronMesh<double> TestMesh;
	LoadPolyhedronMeshFromVTKFile(TestMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1.vtk");

	TestMesh.InitializePointDataSet("Stress", 6);
	TestMesh.InitializeCellDataSet("Stress", 12);
	SavePolyhedronMeshAsJsonDataFile(TestMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1_save.json");

	TetrahedronMesh<double> TestMesh_a;
	LoadPolyhedronMeshFromJsonDataFile(TestMesh_a, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1_save.json");
	SavePolyhedronMeshAsJsonDataFile(TestMesh_a, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1_save_a.json");
}

