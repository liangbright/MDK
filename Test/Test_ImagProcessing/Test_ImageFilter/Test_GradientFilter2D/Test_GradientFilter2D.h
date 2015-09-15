#ifndef Test_GradientFilter2D_h
#define Test_GradientFilter2D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkDenseImage2D_FileIO.h"
#include "mdkScalarDenseImageGradientFilter2D.h"

namespace mdk
{

void test_a()
{
	String FilePath_InputImage = "C:/Research/SpineAnalysis/TestData/Patient1/T2W/T2W0005.dcm";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter2D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "GradientMagImage.json";

	DenseImage2D<double> InputImage;
	Load2DScalarImageFromSingleDICOMFile(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';

	ScalarDenseImageGradientFilter2D<double> GFilter;
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
	InterpolationOption.MethodType = ScalarDenseImageGradientFilter2D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = ScalarDenseImageGradientFilter2D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	InterpolationOption.Pixel_OutsideImage = 0;
	GFilter.SetImageInterpolationOption(InterpolationOption);
	GFilter.SetMaxThreadCount(4);

	GFilter.SetRadius(1.0);

	GFilter.SetAngleResolution(0.0175);
	
	//GFilter.BuildMask();

	DenseVector<double, 2> GradientPrior = { 1.0, 0.0 };
	GFilter.BuildMaskWithGradientPrior(GradientPrior);

	GFilter.Update();
	const auto& GradientImage = *GFilter.GetOutputImage();

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

#endif