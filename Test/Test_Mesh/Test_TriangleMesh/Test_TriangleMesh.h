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

	auto EdgeHandleList = LeafletMesh.GetEdgeHandleList();

	LeafletMesh.ShrinkEdgeToPoint(EdgeHandleList[0], 0);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkEdge.vtk");

}

void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = LeafletMesh.GetFaceHandleList();

	LeafletMesh.ShrinkFaceToPoint(FaceHandleList[1], 0);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkFace.vtk");

}