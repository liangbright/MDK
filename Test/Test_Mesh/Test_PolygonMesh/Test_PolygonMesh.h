#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"

void Test_MeshConstruction()
{
    using namespace mdk;

   // CharString FilePathAndName = "H:/AorticValve/Data/Yale_CT/P1940650/P1940650_Leaflet_A-new-phase2.vtk";

    CharString FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";
    
    auto LeafletMesh = LoadPolygonMeshFromVTKFile<PolygonMeshAttributeType<double>>(FilePathAndName);

   // SavePolygonMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.vtk");
}
