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

	TriangleMesh<TriangleMeshStandardAttributeType<double>>::PointHandleType PointHandle;
	PointHandle.SetIndex(0);

	auto Normal = LeafletMesh.Point(PointHandle).Attribute().AngleWeightedNormal;
}

void Test_ShrinkEdge()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	TriangleMesh<TriangleMeshStandardAttributeType<double>>::PointHandleType PointHandle;
	PointHandle.SetIndex(0);

	auto EdgeHandleList = LeafletMesh.GetEdgeHandleList();

	LeafletMesh.ShrinkEdgeToPoint(EdgeHandleList[0], PointHandle);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkEdge.vtk");
}


void Test_ShrinkFace()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Leaflet";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName + ".vtk");

	TriangleMesh<TriangleMeshStandardAttributeType<double>>::PointHandleType PointHandle;
	PointHandle.SetIndex(0);

	auto FaceHandleList = LeafletMesh.GetFaceHandleList();

	LeafletMesh.ShrinkFaceToPoint(FaceHandleList[1], PointHandle);
	SaveTriangleMeshAsVTKFile(LeafletMesh, FilePathAndName + "_ShrinkFace.vtk");
}

void Test_GlobalAttribute()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_Mesh/Test_TriangleMesh/TestData/Square";

	TriangleMesh<PolygonMeshStandardAttributeType<double>> InputMesh, SquareMesh;
	LoadTriangleMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	InputMesh.Attribute().ID = 1;
	InputMesh.Attribute().Name = "Square";
	InputMesh.Attribute().Map_PointName_to_PointHandle["0_name"].SetIndex(0);
	InputMesh.Attribute().Map_PointName_to_PointHandle["1_name"].SetIndex(1);
	InputMesh.Attribute().Map_FaceName_to_FaceHandle["0_name"].SetIndex(0);
	InputMesh.Attribute().Map_FaceName_to_FaceHandle["1_name"].SetIndex(1);
	InputMesh.Attribute().PointSetList.Resize(10);
	InputMesh.Attribute().PointSetList[0] = InputMesh.ConvertPointIndexToPointHandle({ 1, 2, 3 });
	InputMesh.Attribute().PointSetList[1] = InputMesh.ConvertPointIndexToPointHandle({ 4, 5, 6 });
	InputMesh.Attribute().Map_PointSetName_to_PointSetIndex["0_name"] = 0;
	InputMesh.Attribute().Map_PointSetName_to_PointSetIndex["1_name"] = 1;
	InputMesh.Attribute().FaceSetList.Resize(2);
	InputMesh.Attribute().FaceSetList[0] = InputMesh.ConvertFaceIndexToFaceHandle({ 1, 2, 3 });
	InputMesh.Attribute().FaceSetList[1] = InputMesh.ConvertFaceIndexToFaceHandle({ 4, 5, 6 });
	InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex["0_name"] = 0;
	InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex["1_name"] = 1;

	SaveTriangleMeshAsJsonDataFile(InputMesh, FilePathAndName + "_att.json");

	LoadTriangleMeshFromJsonDataFile(SquareMesh, FilePathAndName + "_att.json");

	SaveTriangleMeshAsJsonDataFile(SquareMesh, FilePathAndName + "_att_re.json");
}