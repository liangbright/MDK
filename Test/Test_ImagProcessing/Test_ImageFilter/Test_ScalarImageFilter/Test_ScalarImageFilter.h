#pragma once

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage3D_FileIO.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"
#include "mdkHistogram.h"
#include "mdkString.h"

namespace mdk
{

void test_ScalarDenseImageGaussianFilter3D()
{
    String FilePath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ScalarImageFilter/TestData/3/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	//DenseImage3D<double> InputImage2;
	//Load3DScalarImageFromSingleDICOMFile(InputImage2, "Z:/sun-lab/Liang_Liang/DrPadala-3D Echo Segmentation/3DEcho_DrPadala/Ring.dcm");//wrong spacing etc

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 30);

	ScalarDenseImageGaussianFilter3D<double> imfilter;

    imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImageInfo(InputImage.GetInfo());

    DenseMatrix<double> RoationMatrix(3, 3);
    RoationMatrix.FillDiagonal(1);
	DenseVector <double, 3> Sigma;
	Sigma = { 2, 1.2, 1.2 };
	imfilter.SetGaussianParameter(Sigma, RoationMatrix, 3);

	auto Option = imfilter.GetImageInterpolationOption();
	Option.MethodType = Image3DInterpolationMethodEnum::Nearest;
	Option.BoundaryOption = Image3DInterpolationBoundaryOptionEnum::Constant;
	Option.Pixel_OutsideImage = 0;
	imfilter.SetImageInterpolationOption(Option);

	imfilter.SetMaxThreadCount(8);

	auto t0 = std::chrono::system_clock::now();
    imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

    auto& OutputImage = *imfilter.GetOutputImage();

    Save3DScalarImageAsJsonDataFile(OutputImage, FilePath + "Data.json");

    std::system("pause");
}

}//namespace mdk
