#pragma once

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage2D_FileIO.h"
#include "mdkGradientDenseImageFilter2D.h"

namespace mdk
{

void test_a()
{
	String FilePath_InputImage = "C:/Research/LumbarSpine/TestData/Patient1/T2W/T2W0005.dcm";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter2D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "GradientMagImage.json";

	DenseImage2D<double> InputImage;
	Load2DScalarImageFromSingleDICOMFile(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';
	auto t0 = std::chrono::system_clock::now();

	GradientDenseImageFilter2D<double> GFilter;
	GFilter.SetInputImage(&InputImage);
    
	auto OutputImageInfo = InputImage.GetInfo();
	//OutputImageInfo.Spacing *= 2.0;
	//OutputImageInfo.Size = {256, 256};
	GFilter.SetOutputImageInfo(OutputImageInfo);

	//Resampler.SetOutputImageInfoBySize(256, 256);

	auto InterpolationOption = GFilter.GetImageInterpolationOption();
	InterpolationOption.MethodType = GradientDenseImageFilter2D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = GradientDenseImageFilter2D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	InterpolationOption.Pixel_OutsideImage = 0;
	GFilter.SetImageInterpolationOption(InterpolationOption);
	GFilter.SetMaxThreadCount(1);

	GFilter.SetRadius(1.0);

	GFilter.SetAngleResolution(0.0175);
	
	//GFilter.BuildMask();

	//DenseVector<double, 2> GradientPrior = { 1.0, 0.0 };
	//GFilter.SetGradientDirectionPrior(GradientPrior);

	GFilter.Update();
	const auto& GradientImage = GFilter.OutputImage();

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	DenseImage2D<double> GradientMagImage;
	GradientMagImage.SetInfo(GradientImage.GetInfo());
	for (int_max k = 0; k < GradientMagImage.GetPixelCount(); ++k)
	{
		GradientMagImage[k] = GradientImage[k].L2Norm();
	}

	Save2DScalarImageAsJsonDataFile(GradientMagImage, FileNameAndPath_OutputImage);
}

}//namespace mdk
