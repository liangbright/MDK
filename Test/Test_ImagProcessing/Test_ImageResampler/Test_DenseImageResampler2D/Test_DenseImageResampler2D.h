#ifndef Test_ScalarImageFilter2D_h
#define Test_ScalarImageFilter2D_h

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage2D_FileIO.h"
#include "mdkDenseImageResampler2D.h"

namespace mdk
{

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
	Resampler.SetInputImage(&InputImage);

	double CosT = 0.707;
	double SinT = 0.707;
	DenseMatrix<double> R = { {1.0, 0.0, 0.0},
							  {0.0, CosT, -SinT },
	                          {0.0, SinT, CosT} };

	auto OutputImageInfo = InputImage.GetInfo();
	OutputImageInfo.Spacing *= 2.0;
	OutputImageInfo.Size = {256, 256};
	OutputImageInfo.Orientation = R*OutputImageInfo.Orientation;
	Resampler.SetOutputImageInfo(OutputImageInfo);

	//Resampler.SetOutputImageInfoBySize(256, 256);

	auto InterpolationOption = Resampler.GetImageInterpolationOption();
	InterpolationOption.MethodType = DenseImageResampler2D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = DenseImageResampler2D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	InterpolationOption.Pixel_OutsideImage = 0;
	Resampler.SetImageInterpolationOption(InterpolationOption);
	Resampler.EnableSmoothingWhenDownsampling();
	Resampler.SetMaxThreadCount(6);
	Resampler.Update();
	const auto& ResampledImage = Resampler.OutputImage();

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	Save2DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);
}

}//namespace mdk

#endif