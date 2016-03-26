#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"

using namespace mdk;

void Test_MeshConstruction()
{
    std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square.vtk";
    
	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName);

    auto PointHandle = InputMesh.AddPoint(0, 0, 0);

    InputMesh.DeletePoint(PointHandle);
    InputMesh.CleanDataStructure();

   // SavePolygonMeshAsJsonDataFile(InputMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square_save.vtk");
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

void Test_GlobalAttribute()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_PolygonMesh/TestData/Square";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh, SquareMesh;	
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	InputMesh.Attribute().ID = 1;
	InputMesh.Attribute().Name = "Square";
	InputMesh.Attribute().Map_PointName_to_PointHandle["0_name"].SetIndex(0);
	InputMesh.Attribute().Map_PointName_to_PointHandle["1_name"].SetIndex(1);
	InputMesh.Attribute().Map_FaceName_to_FaceHandle["0_name"].SetIndex(0);
	InputMesh.Attribute().Map_FaceName_to_FaceHandle["1_name"].SetIndex(1);
	InputMesh.Attribute().PointSetList.Resize(10);	
	InputMesh.Attribute().PointSetList[0] = InputMesh.ConvertPointIndexToPointHandle({ 1, 2, 3 });
	InputMesh.Attribute().PointSetList[1] = InputMesh.ConvertPointIndexToPointHandle({ 4, 5, 6 });
	InputMesh.Attribute().Map_PointSetName_to_PointSetIndex["0_name"]=0;
	InputMesh.Attribute().Map_PointSetName_to_PointSetIndex["1_name"]=1;
	InputMesh.Attribute().FaceSetList.Resize(2);
	InputMesh.Attribute().FaceSetList[0] = InputMesh.ConvertFaceIndexToFaceHandle({ 1, 2, 3 });
	InputMesh.Attribute().FaceSetList[1] = InputMesh.ConvertFaceIndexToFaceHandle({ 4, 5, 6 });
	InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex["0_name"]=0;
	InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex["1_name"]=1;

	SavePolygonMeshAsJsonDataFile(InputMesh, FilePathAndName + "_att.json");

	LoadPolygonMeshFromJsonDataFile(SquareMesh, FilePathAndName + "_att.json");

	SavePolygonMeshAsJsonDataFile(SquareMesh, FilePathAndName + "_att_re.json");

	auto Set1 = InputMesh.Attribute().GetPointSet("0_name");
	auto Set2 = InputMesh.Attribute().GetPointSet("a");

	auto Set3 = InputMesh.Attribute().GetFaceSet("0_name");
	auto Set4 = InputMesh.Attribute().GetFaceSet("b");
}