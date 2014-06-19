#include "mdkFileIO.h"
#include "mdkTriangleMesh.h"

void Test_MeshConstruction()
{
    using namespace mdk;

    //CharString FilePathAndName = "H:/AorticValve/Data/Yale_CT/P1940650/P1940650_Leaflet_A-new-phase2.vtk";

    CharString FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";

    auto LeafletMesh = LoadTriangleMeshFromVTKFile<TriangleMeshAttributeType<double>>(FilePathAndName);

    //SaveTriangleMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.json");
    SaveTriangleMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle_test.vtk");
}
