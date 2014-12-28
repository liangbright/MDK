#ifndef __TestFileIO_h
#define __TestFileIO_h

#include <iostream>

#include "mdkFileIO.h"
#include "mdkDenseImage3D.h"
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

	DenseMatrix<double> OutputMatrix;
	LoadDenseMatrixFromJsonDataFile(OutputMatrix, FilePathAndName);

	DisplayMatrix("InputMatrix", InputMatrix, 4);
	DisplayMatrix("OutputMatrix", OutputMatrix, 4);
}

/*
void Test_ImageIO()
{
	std::string FilePath = "H:/AorticValveData/2014_7_25/P2115937/phase0";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath);

	std::string OutputFilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Image.json";

	Save3DScalarImageAsJsonDataFile(InputImage, OutputFilePathAndName);
}

void Test_PolygonMeshIO()
{
	std::string VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	std::string JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.json";

	typedef PolygonMesh<PolygonMeshStandardAttributeType<double>> PolygonMeshType;

	PolygonMeshType InputMesh;

	LoadPolygonMeshFromVTKFile(InputMesh, VTKFileNameAndPath_input);

	SavePolygonMeshAsJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	PolygonMeshType OutputMesh;
	LoadPolygonMeshFromJsonDataFile(OutputMesh, JsonFileNameAndPath_input);

	std::string VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh_output.vtk";

	SavePolygonMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}

void Test_TriangleMeshIO()
{
	std::string VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	std::string JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh.json";

	typedef TriangleMesh<TriangleMeshStandardAttributeType<double>> TriangleMeshType;

	TriangleMeshType InputMesh;
    LoadTriangleMeshFromVTKFile(InputMesh, VTKFileNameAndPath_input);

	SaveTriangleMeshAsJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	TriangleMeshType OutputMesh;
	LoadTriangleMeshFromJsonDataFile(OutputMesh, JsonFileNameAndPath_input);

	std::string VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh_output.vtk";

	SaveTriangleMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}
*/
}//end of namespace


#endif