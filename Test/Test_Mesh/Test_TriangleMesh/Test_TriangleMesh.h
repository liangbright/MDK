#include "mdkFileIO.h"
#include "mdkTriangleMesh.h"

using namespace mdk;

void Test_MeshConstruction()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet.vtk";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName);	

	SavePolygonMeshAsVTKFile(LeafletMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet_Test_Load_Save.vtk");
}

void Test_PointDataSet()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet.vtk";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName);

	LeafletMesh.AddPointDataSet("Stress", 10);	
	LeafletMesh.AddFaceDataSet("Stress", 12);
	SavePolygonMeshAsVTKFile(LeafletMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet_Test_PointDataSet.vtk");

	TriangleMesh<double> LeafletMesh_a;
	LoadPolygonMeshFromVTKFile(LeafletMesh_a, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet_Test_PointDataSet.vtk");
	SavePolygonMeshAsVTKFile(LeafletMesh_a, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet_Test_PointDataSet_a.vtk");
}

void Test_MeshAttribute()
{
	std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName);

	auto Normal = LeafletMesh.Point(0).Attribute().AngleWeightedNormal;
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = LeafletMesh.GetValidEdgeIndexList();

	LeafletMesh.ShrinkEdgeToPoint(EdgeHandleList[0], 0);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkEdge.vtk");
}


void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = LeafletMesh.GetValidFaceIndexList();

	LeafletMesh.ShrinkFaceToPoint(FaceHandleList[1], 0);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkFace.vtk");
}

void Test_GlobalAttribute()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Square";

	TriangleMesh<double> InputMesh, SquareMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	InputMesh.SetName("Square");
	InputMesh.Point(0).SetName("0_name");
	InputMesh.Face(0).SetName("0_name");
	InputMesh.SetPointSet("1", { 0,1 });

	SavePolygonMeshAsJsonDataFile(InputMesh, FilePathAndName + "_att.json");
	LoadPolygonMeshFromJsonDataFile(SquareMesh, FilePathAndName + "_att.json");
	SavePolygonMeshAsJsonDataFile(SquareMesh, FilePathAndName + "_att_re.json");
}


void Test_CollapseEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = LeafletMesh.GetValidEdgeIndexList();
	auto PointIndexList = LeafletMesh.Edge(EdgeHandleList[10]).GetPointIndexList();
	LeafletMesh.CollapseEdge(EdgeHandleList[10], PointIndexList[0]);
	LeafletMesh.CleanDataStructure();
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_CollapseEdge.vtk");
}

void Test_FlipEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<double> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = LeafletMesh.GetValidEdgeIndexList();	
	LeafletMesh.FlipEdge(EdgeHandleList[10]);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_FlipEdge.vtk");
}