#include "mdkFileIO.h"
#include "mdkTriangleMesh.h"
#include "mdkTriangleMesh_FileIO.h"

void Test_MeshConstruction()
{
    using namespace mdk;

    std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> LeafletMesh;
	LoadTriangleMeshFromVTKFile(LeafletMesh, FilePathAndName);

    //SaveTriangleMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.json");
    SaveTriangleMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle_test.vtk");
}
