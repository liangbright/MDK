#ifndef __TestFileIO_h
#define __TestFileIO_h

#include <iostream>

#include "mdkFileIO.h"
#include "mdkImage3D.h"
#include "mdkDenseMatrix.h"
#include "mdkPolygonMesh.h"
#include "mdkTriangleMesh.h"

namespace mdk
{

void Test_MatrixIO()
{
	DenseMatrix<double> InputMatrix(3, 4);
	InputMatrix = { { 1.01, 2.02, 3.03, 4.04 },
					{ 5.05, 6.06, 7.07, 8.08 },
				    { 9.09, 10.1, 11.1, 12.2 } };

    std::string FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Matrix.json";

	SaveDenseMatrixAsJsonDataFile(InputMatrix, FilePathAndName);

	auto OutputMatrix = LoadDenseMatrixFromJsonDataFile<float>(FilePathAndName);

	DisplayMatrix("InputMatrix", InputMatrix, 4);
	DisplayMatrix("OutputMatrix", OutputMatrix, 4);
}


void Test_ImageIO()
{
	std::string FilePath = "H:/AorticValveData/2014_7_25/P2115937/phase0";

	auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath);

	std::string OutputFilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Image.json";

	Save3DScalarImageAsJsonDataFile(InputImage, OutputFilePathAndName);
}

void Test_PolygonMeshIO()
{
	std::string VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	std::string JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.json";

	auto InputMesh = LoadPolygonMeshFromVTKFile<PolygonMeshStandardAttributeType<double>>(VTKFileNameAndPath_input);

	SavePolygonMeshAsJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	auto OutputMesh = LoadPolygonMeshFromJsonDataFile<PolygonMeshStandardAttributeType<double>>(JsonFileNameAndPath_input);

	std::string VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh_output.vtk";

	SavePolygonMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}

void Test_TriangleMeshIO()
{
	std::string VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	std::string JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh.json";

	auto InputMesh = LoadTriangleMeshFromVTKFile<PolygonMeshStandardAttributeType<double>>(VTKFileNameAndPath_input);

	SaveTriangleMeshAsJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	auto OutputMesh = LoadTriangleMeshFromJsonDataFile<PolygonMeshStandardAttributeType<double>>(JsonFileNameAndPath_input);

	std::string VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh_output.vtk";

	SaveTriangleMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}

}//end of namespace


#endif