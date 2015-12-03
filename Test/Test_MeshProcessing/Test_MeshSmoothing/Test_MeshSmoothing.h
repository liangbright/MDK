#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test_Aorta()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_MeshSmoothing/TestData/Aorta";

	PolygonMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto OutputMesh1 = SmoothMeshByVTKSmoothPolyDataFilter(InputMesh, 100, false);
	auto OutputMesh2 = SmoothMeshByVTKWindowedSincPolyDataFilter(InputMesh, 0.01, 10);

	SavePolygonMeshAsVTKFile(OutputMesh1, FilePathAndName + "_VTKSmoothPolyDataFilter.vtk");
	SavePolygonMeshAsVTKFile(OutputMesh1, FilePathAndName + "_VTKWindowedSincPolyDataFilter.vtk");
}

