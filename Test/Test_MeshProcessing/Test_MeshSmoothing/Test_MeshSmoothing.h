#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test_Aorta()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_MeshSmoothing/TestData/Aorta";

	TriangleMesh<PolygonMeshStandardAttributeType<double>> InputMesh;
	LoadTriangleMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	auto OutputMesh1 = SmoothMeshByVTKSmoothPolyDataFilter(InputMesh, 100, false);
	auto OutputMesh2 = SmoothMeshByVTKWindowedSincPolyDataFilter(InputMesh, 0.01, 10);
	auto OutputMesh3 = InputMesh;
	SmoothTriangleMeshByNormalBasedCurvature(OutputMesh3, 10, 1, false, false);
	SavePolygonMeshAsVTKFile(OutputMesh1, FilePathAndName + "_VTKSmoothPolyDataFilter.vtk");
	SavePolygonMeshAsVTKFile(OutputMesh2, FilePathAndName + "_VTKWindowedSincPolyDataFilter.vtk");
	SavePolygonMeshAsVTKFile(OutputMesh3, FilePathAndName + "_SmoothTriangleMeshByNormalBasedCurvature.vtk");
}

