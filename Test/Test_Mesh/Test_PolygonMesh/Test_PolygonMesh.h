#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"
#include "mdkPolygonMesh_FileIO.h"

using namespace mdk;

void Test_MeshConstruction()
{
    std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";
    
	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName);

    auto PointHandle = InputMesh.AddPoint(0, 0, 0);

    InputMesh.DeletePoint(PointHandle);
    InputMesh.CleanDataStructure();

   // SavePolygonMeshAsJsonDataFile(InputMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(InputMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.vtk");
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto EdgeHandleList = InputMesh.GetEdgeHandleList();
	auto PointIndexList_Edge = InputMesh.Edge(EdgeHandleList[0]).GetPointHandleList();
	InputMesh.ShrinkEdgeToPoint(EdgeHandleList[0], PointIndexList_Edge[0]);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ShrinkEdge.vtk");

}

void Test_SplitEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto PointHandleList = InputMesh.GetPointHandleList();
	auto Point0 = InputMesh.GetPointPosition(PointHandleList[0]);
	auto Point1 = InputMesh.GetPointPosition(PointHandleList[1]);
	auto Point2 = (Point0 + Point1) / 2.0;
	auto PointHandle2 = InputMesh.AddPoint(Point2);
	auto EdgeHandle01 = InputMesh.GetEdgeHandleByPoint(PointHandleList[0], PointHandleList[1]);
	InputMesh.SplitEdgeByPoint(EdgeHandle01, PointHandle2);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_SplitEdge.vtk");
}

void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = InputMesh.GetFaceHandleList();
	auto PointHandleList_Face = InputMesh.Face(FaceHandleList[0]).GetPointHandleList();
	InputMesh.ShrinkFaceToPoint(FaceHandleList[0], PointHandleList_Face[0]);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ShrinkFace.vtk");
}

void Test_ReversePointOrder()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto FaceHandleList = InputMesh.GetFaceHandleList();
	InputMesh.ReversePointOrderOfFace(FaceHandleList[0]);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ReversePointOrder.vtk");
}