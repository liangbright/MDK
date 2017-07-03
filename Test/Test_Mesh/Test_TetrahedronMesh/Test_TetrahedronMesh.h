#include "mdkFileIO.h"
#include "mdkTetrahedronMesh.h"

using namespace mdk;

void Test_MeshConstruction()
{
	TetrahedronMesh<double> TestMesh;
	LoadPolyhedronMeshFromVTKFile(TestMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1.vtk");

	SavePolyhedronMeshAsJsonDataFile(TestMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Tetra1_save.json");
}

