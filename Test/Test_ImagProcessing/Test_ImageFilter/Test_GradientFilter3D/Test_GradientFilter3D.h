#pragma once

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkDenseImage3D_FileIO.h"
#include "mdkGradientDenseImageFilter3D.h"

using namespace mdk;


void test_a()
{
	String FilePath_InputImage = "C:/Research/AorticValve/Data/AorticValveData/2014_7_25/P2115937/phase0/";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter3D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "GradientMagImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';

	GradientDenseImageFilter3D<double> GFilter;
	GFilter.SetInputImage(&InputImage);
    
	double CosT = 0.707;
	double SinT = 0.707;
	DenseMatrix<double> R = { {1.0, 0.0, 0.0},
							  {0.0, CosT, -SinT },
	                          {0.0, SinT, CosT} };

	auto OutputImageInfo = InputImage.GetInfo();
	//OutputImageInfo.Spacing *= 2.0;
	//OutputImageInfo.Size = {256, 256};
	//OutputImageInfo.Orientation = R*OutputImageInfo.Orientation;
	GFilter.SetOutputImageInfo(OutputImageInfo);

	//Resampler.SetOutputImageInfoBySize(256, 256);

	auto InterpolationOption = GFilter.GetImageInterpolationOption();
	InterpolationOption.MethodType = GradientDenseImageFilter3D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = GradientDenseImageFilter3D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	InterpolationOption.Pixel_OutsideImage = 0;
	GFilter.SetImageInterpolationOption(InterpolationOption);
	GFilter.SetMaxThreadCount(8);

	GFilter.SetRadius(2.0);
	GFilter.SetSphereResolution(3600);

	//DenseVector<double, 2> GradientPrior = { 1.0, 0.0 };
	//DenseVector<int_max> MaskCountPerLevel = { 8, 32 };
	//GFilter.BuildMaskWithGradientPrior(GradientPrior, MaskCountPerLevel);

	std::cout << "Update" << '\n';

	GFilter.Update();
	const auto& GradientImage = GFilter.OutputImage();

	std::cout << "done" << '\n';

	DenseImage3D<double> GradientMagImage;
	GradientMagImage.SetInfo(GradientImage.GetInfo());
	for (int_max k = 0; k < GradientMagImage.GetPixelCount(); ++k)
	{
		GradientMagImage[k] = GradientImage[k].L2Norm();
	}

	Save3DScalarImageAsJsonDataFile(GradientMagImage, FileNameAndPath_OutputImage);
}

