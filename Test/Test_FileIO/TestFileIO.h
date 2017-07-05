#ifndef __TestFileIO_h
#define __TestFileIO_h

#include <iostream>

#include "mdkDenseImage3D.h"
#include "mdkDenseImage3D_FileIO.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseMatrix_FileIO.h"
#include "mdkPolygonMesh.h"
#include "mdkTriangleMesh.h"

#include "mdkVTKDataObject_FileIO.h"
#include "mdkITKDataObject_FileIO.h"

namespace mdk
{

void Test_MatrixIO()
{
	DenseMatrix<double> InputMatrix(3, 4);
	InputMatrix = { { 1.01, 2.02, 3.03, 4.04 },
					{ 5.05, 6.06, 7.07, 8.08 },
				    { 9.09, 10.1, 11.1, 12.2 } };

    String FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Matrix.json";

	SaveDenseMatrixAsJsonDataFile(InputMatrix, FilePathAndName);

	DenseMatrix<double> OutputMatrix;
	LoadDenseMatrixFromJsonDataFile(OutputMatrix, FilePathAndName);

	DisplayMatrix("InputMatrix", InputMatrix, 4);
	DisplayMatrix("OutputMatrix", OutputMatrix, 4);
}


void Test_ImageIO()
{
	String FilePath = "H:/AorticValveData/2014_7_25/P2115937/phase0";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath);

	String OutputFilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Image.json";

	Save3DScalarImageAsJsonDataFile(InputImage, OutputFilePathAndName);

	DenseImage3D<double> OutputImage;
	Load3DScalarImageFromJsonDataFile(OutputImage, OutputFilePathAndName);
}


void Test_PolygonMeshIO()
{
	String VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	String JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.json";

	PolygonMesh<double> InputMesh;

	LoadPolygonMeshFromVTKFile(InputMesh, VTKFileNameAndPath_input);

	SavePolygonMeshAsJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	PolygonMesh<double> OutputMesh;
	LoadPolygonMeshFromJsonDataFile(OutputMesh, JsonFileNameAndPath_input);

	String VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh_output.vtk";

	SavePolygonMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}

void Test_TriangleMeshIO()
{
	String VTKFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/PolygonMesh.vtk";
	String JsonFileNameAndPath_input = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh.json";

	TriangleMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, VTKFileNameAndPath_input);

	LoadPolygonMeshFromJsonDataFile(InputMesh, JsonFileNameAndPath_input);

	TriangleMesh<double> OutputMesh;
	LoadPolygonMeshFromJsonDataFile(OutputMesh, JsonFileNameAndPath_input);

	String VTKFileNameAndPath_output = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/TriangleMesh_output.vtk";

	SavePolygonMeshAsVTKFile(OutputMesh, VTKFileNameAndPath_output);
}

/*
void Test_VTK_Json()
{
	String FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Image.json";

	auto VTKImage = vtkSmartPointer<vtkImageData>::New();	
	vtkImageData* VTKImage_ptr = VTKImage.Get();
	auto IsOK = LoadVTK3DScalarImageFromJsonDataFile(VTKImage_ptr, VTKDataTypeEnum::VALUE_DOUBLE, FilePathAndName);
}
*/

/*
void Test_ITK_Json()
{
	String FilePathAndName = "C:/Research/MDK/MDK_Build/Test/Test_FileIO/TestData/Image.json";
	typedef itk::Image<double, 3> ITKImageType;
	auto ITKImage = ITKImageType::New();

	auto IsOK = LoadITK3DScalarImageFromJsonDataFile(ITKImage.GetPointer(), FilePathAndName);
}
*/
}//end of namespace


#endif