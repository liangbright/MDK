#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"

using namespace mdk;

void Test_MeshConstruction()
{
    std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square.vtk";
    
	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName);

    auto PointIndex = InputMesh.AddPoint(0, 0, 0);

    InputMesh.DeletePoint(PointIndex);
    InputMesh.CleanDataStructure();

   // SavePolygonMeshAsJsonDataFile(InputMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square_save.vtk");
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto EdgeIndexList = InputMesh.GetValidEdgeIndexList();
	auto PointIndexList_Edge = InputMesh.Edge(EdgeIndexList[0]).GetPointIndexList();
	InputMesh.ShrinkEdgeToPoint(EdgeIndexList[0], PointIndexList_Edge[0]);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ShrinkEdge.vtk");

}

void Test_SplitEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto PointIndexList = InputMesh.GetValidPointIndexList();
	auto Point0 = InputMesh.GetPointPosition(PointIndexList[0]);
	auto Point1 = InputMesh.GetPointPosition(PointIndexList[1]);
	auto Point2 = (Point0 + Point1) / 2.0;
	auto PointIndex2 = InputMesh.AddPoint(Point2);
	auto EdgeIndex01 = InputMesh.GetEdgeIndexByPoint(PointIndexList[0], PointIndexList[1]);
	InputMesh.SplitEdgeByPoint(EdgeIndex01, PointIndex2);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_SplitEdge.vtk");
}

void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto FaceIndexList = InputMesh.GetValidFaceIndexList();
	auto PointIndexList_Face = InputMesh.Face(FaceIndexList[0]).GetPointIndexList();
	InputMesh.ShrinkFaceToPoint(FaceIndexList[0], PointIndexList_Face[0]);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ShrinkFace.vtk");
}

void Test_ReversePointOrder()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto FaceIndexList = InputMesh.GetValidFaceIndexList();
	InputMesh.Face(FaceIndexList[0]).ReversePointOrder();
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_ReversePointOrder.vtk");
}

void Test_AddtionalInfo()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh, SquareMesh;	
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	InputMesh.SetName("Square");

	InputMesh.Point(0).SetName("0_Name");	
	InputMesh.Point(1).SetName("1_Name");
	InputMesh.Face(0).SetName("0_Name");
	InputMesh.Face(1).SetName("1_Name");
	
	InputMesh.SetPointSet("0_PointSet", { 1, 2, 3 });
	InputMesh.SetPointSet("1_PointSet", { 4, 5, 6 });	
	InputMesh.SetFaceSet("0_FaceSet", { 1, 2, 3 });
	InputMesh.SetFaceSet("1_FaceSet", { 4, 5, 6 });

	SavePolygonMeshAsJsonDataFile(InputMesh, FilePathAndName + "_att.json");

	LoadPolygonMeshFromJsonDataFile(SquareMesh, FilePathAndName + "_att.json");

	SavePolygonMeshAsJsonDataFile(SquareMesh, FilePathAndName + "_att_re.json");

	auto Set1 = InputMesh.GetPointSet("0_PointSet");
	auto Set2 = InputMesh.GetPointSet("1_PointSet");

	auto Set3 = InputMesh.GetFaceSet("0_FaceSet");
	auto Set4 = InputMesh.GetFaceSet("1_FaceSet");
}

void Test_AddtionalInfo_Name()
{
	ObjectArray<String> NameList;
	NameList.Resize(10000);
	for (int_max k = 0; k < 10000; ++k)
	{
		NameList[k] = "LandMark_abcedfg_0123456789";
	}
}

void Test_SplitFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto FaceIndexList = InputMesh.GetValidFaceIndexList();
	auto PointIndexList_Face = InputMesh.Face(FaceIndexList[0]).GetPointIndexList();
	
	InputMesh.SplitFaceByPoint(FaceIndexList[0], 0, 11);

	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_SplitFaceByPoint.vtk");
}

void Test_CutAlongEdge1()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";
	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");
	auto EdgeIndexA = InputMesh.GetEdgeIndexByPoint(1, 11);
	auto EdgeIndexB = InputMesh.GetEdgeIndexByPoint(11, 21);
	InputMesh.CutAlongEdge(EdgeIndexA);
	InputMesh.CutAlongEdge(EdgeIndexB);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_CutAlongEdge1.vtk");
}

void Test_CutAlongEdge2()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";
	PolygonMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");
	auto EdgeIndexA = InputMesh.GetEdgeIndexByPoint(11, 21);
	auto EdgeIndexB = InputMesh.GetEdgeIndexByPoint(21, 22);
	InputMesh.CutAlongEdge(EdgeIndexA, EdgeIndexB);	
	auto Pos100 = InputMesh.GetPointPosition(100);
	Pos100[2] = 10.123456789;
	InputMesh.SetPointPosition(100, Pos100);
	SavePolygonMeshAsVTKFile(InputMesh, FilePathAndName + "_CutAlongEdge2.vtk");
}
