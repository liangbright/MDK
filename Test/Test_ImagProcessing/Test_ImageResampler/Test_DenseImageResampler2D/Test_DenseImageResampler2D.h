#pragma once

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage2D_FileIO.h"
#include "mdkDenseImageResampler2D.h"


using namespace mdk;

void test_a()
{
	String FilePath_InputImage = "C:/Research/LumbarSpine/TestData/Patient1/T2W/T2W0005.dcm";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler2D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "ResampledImage.json";

	DenseImage2D<double> InputImage;
	Load2DScalarImageFromSingleDICOMFile(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';

	auto t0 = std::chrono::system_clock::now();

	DenseImageResampler2D<double> Resampler;
	Resampler.Input.Image = &InputImage;

	double Theta = -3.141592654 / 4;
	double CosT = std::cos(Theta);
	double SinT = std::sin(Theta);
	DenseMatrix<double> Rx = { {1.0, 0.0, 0.0},
							  {0.0, CosT, -SinT },
	                          {0.0, SinT, CosT} };
	
	DenseMatrix<double> Ry = { { CosT, 0.0, SinT },
	                           { 0.0,  1.0, 0.0 },
	                           { -SinT, 0.0, CosT } };

	DenseMatrix<double> Rz = { { CosT, -SinT, 0.0 },
	                           { SinT, CosT,  0.0 },
	                           { 0.0,  0.0, 1.0 } };

	///*
	auto OutputImageInfo = InputImage.GetInfo();
	OutputImageInfo.Origin = InputImage.Transform2DIndexTo3DWorldPosition(185, 400);
	auto Index2D = InputImage.Transform3DWorldPositionTo2DIndex(OutputImageInfo.Origin);
	OutputImageInfo.Spacing = {2.5, 2.5};
	OutputImageInfo.Size = {21, 21};
	OutputImageInfo.Orientation = OutputImageInfo.Orientation*Rz;
	Resampler.SetOutputImageInfo(OutputImageInfo);
	//*/

	//Resampler.SetOutputImageInfoBySize(256, 256);

	//Resampler.SetOutputImageInfoBySpacing(2.5, 2.5);

	Resampler.Input.ImageInterpolationOption.MethodType = DenseImageResampler2D<double>::ImageInterpolationMethodEnum::Linear;
	Resampler.Input.ImageInterpolationOption.BoundaryOption = DenseImageResampler2D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	Resampler.Input.ImageInterpolationOption.Pixel_OutsideImage = 0;

	//Resampler.EnableTriangleSmoothingWhenDownsampling();
	Resampler.EnableAverageSmoothingWhenDownsampling();
	Resampler.Input.MaxThreadCount = 8;
	Resampler.Update();
	const auto& ResampledImage = Resampler.Output.Image;

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	Save2DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);
}
