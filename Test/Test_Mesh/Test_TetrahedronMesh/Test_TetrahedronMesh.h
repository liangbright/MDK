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


void Test_Wedge()
{
	DenseVector<double, 3> Pos0, Pos1, Pos2, Pos3, Pos4, Pos5;
	Pos0 = {0.0, 0.0, 0.0};
	Pos1 = {1.0, 0.0, 0.0 };
	Pos2 = { 0.0, 1.0, 0.0 };
	Pos3 = { 0.0, 0.0, 1.0 };
	Pos4 = { 1.0, 0.0, 1.0 };
	Pos5 = { 0.0, 1.0, 1.0 };
	PolyhedronMesh<double> TestMesh;
	DenseVector<int_max> PointIndexList;
	PointIndexList.Append(TestMesh.AddPoint(Pos3));
	PointIndexList.Append(TestMesh.AddPoint(Pos4));
	PointIndexList.Append(TestMesh.AddPoint(Pos5));
	PointIndexList.Append(TestMesh.AddPoint(Pos0));
	PointIndexList.Append(TestMesh.AddPoint(Pos1));
	PointIndexList.Append(TestMesh.AddPoint(Pos2));

	TestMesh.AddCellByPoint(MeshCellTypeEnum::Wedge, PointIndexList);
	SavePolyhedronMeshAsJsonDataFile(TestMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TetrahedronMesh/TestData/Test_Wedge2.json");
}

