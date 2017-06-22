#include "mdkGeodesicSphereBuilder.h"
#include "mdkPolygonMeshProcessing.h"

void Test_a()
{
    using namespace mdk;

	GeodesicSphereBuilder<double> Builder;
	Builder.Input.MaxDepth = 6;
	Builder.Update();
	auto& SphereList = Builder.Output.SphereList;

	String File0 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d0.vtk";
	String File1 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d1.vtk";
	String File2 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d2.vtk";
	String File3 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d3.vtk";
	String File4 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d4.vtk";
	String File5 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d5.vtk";
    String File6 = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d6.vtk";

	SavePolygonMeshAsVTKFile(SphereList[0], File0);// Cell 20, Point 12
	SavePolygonMeshAsVTKFile(SphereList[1], File1);// Cell 80, Point 42
	SavePolygonMeshAsVTKFile(SphereList[2], File2);// Cell 320, Point 162
	SavePolygonMeshAsVTKFile(SphereList[3], File3);// Cell 1280, Point 642
	SavePolygonMeshAsVTKFile(SphereList[4], File4);// Cell 5120, Point 2562
	SavePolygonMeshAsVTKFile(SphereList[5], File5);// Cell 20480, Point 10242
	SavePolygonMeshAsVTKFile(SphereList[6], File6);// Cell 81920, Point 40962

	auto tempMesh = SubdivideTriangleMesh_Linear(SphereList[0], 2);
	String File_temp = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d0_sub.vtk";
	SavePolygonMeshAsVTKFile(tempMesh, File_temp);

}
