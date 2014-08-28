#include "mdkFileIO.h"
#include "mdkPolygonMesh.h"

void Test_MeshConstruction()
{
    using namespace mdk;

    std::string FilePathAndName = "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Triangle.vtk";
    
    auto LeafletMesh = LoadPolygonMeshFromVTKFile<PolygonMeshStandardAttributeType<double>>(FilePathAndName);

    auto PointHandle = LeafletMesh.AddPoint(0, 0, 0);

    LeafletMesh.DeletePoint(PointHandle);
    LeafletMesh.CleanDataStructure();

   // SavePolygonMeshAsJsonDataFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.json");

    SavePolygonMeshAsVTKFile(LeafletMesh, "C:/Research/Test_VTK/Subdivision/Build/Leaflet_A_Polygon.vtk");
}
