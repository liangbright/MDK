#include "mdkGeodesicSphereBuilder.h"
#include "mdkTriangleMesh_FileIO.h"

void Test_a()
{
    using namespace mdk;

	GeodesicSphereBuilder<double> Builder;
	Builder.SetMaxDepth(2);
	Builder.Update();
	auto& Sphere = *Builder.GetSphereAtMaxDepth();

    std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_GeodesicSphereBuilder/TestData/Sphere_d2.vtk";

	SaveTriangleMeshAsVTKFile(Sphere, FilePathAndName);
}
