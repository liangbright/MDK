#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test_Aorta()
{
	std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_MeshSmoothing/TestData/Aorta";

	TriangleMesh<double> InputMesh, OutputMesh1, OutputMesh2, OutputMesh3;
	LoadPolygonMeshFromVTKFile(InputMesh, FilePathAndName + ".vtk");

	OutputMesh1 = InputMesh;
	OutputMesh2 = InputMesh;
	OutputMesh3 = InputMesh;
	SmoothMeshByVTKSmoothPolyDataFilter(OutputMesh1, 100, false);
	SmoothMeshByVTKWindowedSincPolyDataFilter(OutputMesh2, 0.01, 10);
	SmoothTriangleMeshByNormalBasedCurvature(OutputMesh3, 10, 1, false, false);
	std::cout << '1' << '\n';
	SavePolygonMeshAsVTKFile(OutputMesh1, FilePathAndName + "_VTKSmoothPoOutputMesh1lyDataFilter.vtk");
	std::cout << '2' << '\n';
	SavePolygonMeshAsVTKFile(OutputMesh2, FilePathAndName + "_VTKWindowedSincPolyDataFilter.vtk");
	std::cout << '3' << '\n';
	SavePolygonMeshAsVTKFile(OutputMesh3, FilePathAndName + "_SmoothTriangleMeshByNormalBasedCurvature.vtk");
	std::cout << '4' << '\n';
}

