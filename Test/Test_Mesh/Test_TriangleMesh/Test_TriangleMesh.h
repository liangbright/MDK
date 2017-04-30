#include "mdkFileIO.h"
#include "mdkTriangleMesh.h"
#include "mdkTriangleMesh_FileIO.h"

using namespace mdk;

void Test_MeshConstruction()
{
	std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName);

    //SaveTriangleMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.json");
    SaveTriangleMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle_test.vtk");	
}

void Test_MeshAttribute()
{
	std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName);

	auto Normal = LeafletMesh.Point(0).Attribute().AngleWeightedNormal;
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = LeafletMesh.GetEdgeIndexList();

	LeafletMesh.ShrinkEdgeToPoint(EdgeHandleList[0], 0);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkEdge.vtk");
}


void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = LeafletMesh.GetFaceIndexList();

	LeafletMesh.ShrinkFaceToPoint(FaceHandleList[1], 0);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkFace.vtk");
}

void Test_GlobalAttribute()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Square";

	TriangleMesh<PolygonMeshStandardAttributeType<double>> InputMesh, SquareMesh;
	LoadTriangleMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	InputMesh.SetID(1);
	InputMesh.SetName("Square");
	InputMesh.Point(0).SetName("0_name");
	InputMesh.Face(0).SetName("0_name");
	InputMesh.SetPointSet("1", { 0,1 });

	SaveTriangleMeshAsJsonDataFile(InputMesh, FilePathAndName + "_att.json");

	LoadTriangleMeshFromJsonDataFile(SquareMesh, FilePathAndName + "_att.json");

	SaveTriangleMeshAsJsonDataFile(SquareMesh, FilePathAndName + "_att_re.json");
}