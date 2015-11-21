#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"
#include "mdkPolygonMesh_FileIO.h"

using namespace mdk;

void Test_MeshConstruction()
{
    std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";
    
	PolygonMesh<PolygonMeshStandardAttributeType<double>> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName);

    auto PointHandle = LeafletMesh.AddPoint(0, 0, 0);

    LeafletMesh.DeletePoint(PointHandle);
    LeafletMesh.CleanDataStructure();

   // SavePolygonMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.vtk");
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = LeafletMesh.GetEdgeHandleList();

	LeafletMesh.ShrinkEdgeToPoint(EdgeHandleList[0], 0);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkEdge.vtk");

}

void Test_SplitEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto PointHandleList = LeafletMesh.GetPointHandleList();
	auto Point0 = LeafletMesh.GetPointPosition(PointHandleList[0]);
	auto Point1 = LeafletMesh.GetPointPosition(PointHandleList[1]);
	auto Point2 = (Point0 + Point1) / 2.0;
	auto PointHandle2 = LeafletMesh.AddPoint(Point2);
	auto EdgeHandle01 = LeafletMesh.GetEdgeHandleByPoint(PointHandleList[0], PointHandleList[1]);
	LeafletMesh.SplitEdgeByPoint(EdgeHandle01, PointHandle2);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_SplitEdge.vtk");
}

void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> LeafletMesh;
	LoadPolygonMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = LeafletMesh.GetFaceHandleList();

	LeafletMesh.ShrinkFaceToPoint(FaceHandleList[0], 0);
	SavePolygonMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkFace.vtk");

}