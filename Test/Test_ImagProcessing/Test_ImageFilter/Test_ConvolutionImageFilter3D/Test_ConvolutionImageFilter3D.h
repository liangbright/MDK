#pragma once

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage3D_FileIO.h"
#include "mdkDiscreteConvolutionDenseImageFilter3D.h"
#include "mdkGenericConvolutionDenseImageFilter3D.h"

using namespace mdk;

void test_DiscreteGaussianFilter3D()
{
    String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	Save3DScalarImageAsJsonDataFile(InputImage, TestDataPath + "test_DiscreteGaussianFilter3D_InputImage.json");

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 100);

	DiscreteConvolutionDenseImageFilter3D<double> imfilter;
    imfilter.SetInputImage(&InputImage);
	imfilter.SetBoundaryOptionAsReplicate();	
	//imfilter.CreateGaussianMask(9, 9, 6, 1.8);
	imfilter.CreateGaussianMask(InputImage.GetSpacing(), 3, 3, 3, 1);
	imfilter.SetMaxThreadCount(8);
	
	std::cout << "Point in Mask " << imfilter.ConvolutionMask().GetColCount() << '\n';

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();
    imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

    auto& OutputImage = imfilter.OutputImage();

    Save3DScalarImageAsJsonDataFile(OutputImage, TestDataPath + "test_DiscreteGaussianFilter3D_OutputImage.json");

    std::system("pause");
}

void test_DiscreteLoGFilter3D()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 100);

	DiscreteConvolutionDenseImageFilter3D<double> imfilter;
	imfilter.SetInputImage(&InputImage);
	imfilter.SetBoundaryOptionAsReplicate();
	imfilter.CreateLaplacianOfGaussianMask(InputImage.GetSpacing(), 1.5, 2);
	imfilter.SetMaxThreadCount(8);

	std::cout << "Point in Mask " << imfilter.ConvolutionMask().GetColCount() << '\n';

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();
	imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	auto& OutputImage = imfilter.OutputImage();

	Save3DScalarImageAsJsonDataFile(OutputImage, TestDataPath + "test_DiscreteLoGFilter3D_OutputImage.json");

	std::system("pause");
}


void test_GenericGaussianFilter3D()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	Save3DScalarImageAsJsonDataFile(InputImage, TestDataPath + "test_GenericGaussianFilter3D_InputImage.json");

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 100);

	GenericConvolutionDenseImageFilter3D<double> imfilter;
	imfilter.SetInputImage(&InputImage);	
	imfilter.SetOutputImageInfo(InputImage.GetInfo());
	//imfilter.CreateGaussianMask(9, 9, 6, 1.8);
	imfilter.CreateGaussianMask(InputImage.GetSpacing(), 3, 3, 3, 1);
	imfilter.SetMaxThreadCount(8);

	auto InterpolationOption = imfilter.GetImageInterpolationOption();
	InterpolationOption.MethodType = GenericConvolutionDenseImageFilter3D<double>::ImageInterpolationMethodEnum::Nearest;
	imfilter.SetImageInterpolationOption(InterpolationOption);

	std::cout << "Point in Mask " << imfilter.ConvolutionMask().GetColCount() << '\n';

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();
	imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	auto& OutputImage = imfilter.OutputImage();

	Save3DScalarImageAsJsonDataFile(OutputImage, TestDataPath + "test_GenericGaussianFilter3D_OutputImage.json");

	std::system("pause");
}

void test_GenericLoGFilter3D()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 100);

	GenericConvolutionDenseImageFilter3D<double> imfilter;
	imfilter.SetInputImage(&InputImage);
	imfilter.SetOutputImageInfo(InputImage.GetInfo());
	imfilter.CreateLaplacianOfGaussianMask(InputImage.GetSpacing(), 1.5, 2);
	imfilter.SetMaxThreadCount(8);

	auto InterpolationOption = imfilter.GetImageInterpolationOption();
	InterpolationOption.MethodType = GenericConvolutionDenseImageFilter3D<double>::ImageInterpolationMethodEnum::Nearest;
	imfilter.SetImageInterpolationOption(InterpolationOption);

	std::cout << "Point in Mask " << imfilter.ConvolutionMask().GetColCount() << '\n';

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();
	imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	auto& OutputImage = imfilter.OutputImage();

	Save3DScalarImageAsJsonDataFile(OutputImage, TestDataPath + "test_GenericLoGFilter3D_OutputImage.json");

	std::system("pause");
}


void test_DiscreteTriangleFilter3D()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/";

	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "C:/Research/AorticValve/Data/CT/Normal/2014_7_25/P2115937/phase0";

	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);

	Save3DScalarImageAsJsonDataFile(InputImage, TestDataPath + "test_DiscreteTriangleFilter3D_InputImage.json");

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 100);

	DiscreteConvolutionDenseImageFilter3D<double> imfilter;
	imfilter.SetInputImage(&InputImage);
	imfilter.SetBoundaryOptionAsReplicate();
	imfilter.CreateTriangleMask(InputImage.GetSpacing(), 3, 3, 3);
	imfilter.SetMaxThreadCount(8);

	std::cout << "Point in Mask " << imfilter.ConvolutionMask().GetColCount() << '\n';

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();
	imfilter.Update();
	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	auto& OutputImage = imfilter.OutputImage();

	Save3DScalarImageAsJsonDataFile(OutputImage, TestDataPath + "test_DiscreteTriangleFilter3D_OutputImage.json");

	std::system("pause");
}