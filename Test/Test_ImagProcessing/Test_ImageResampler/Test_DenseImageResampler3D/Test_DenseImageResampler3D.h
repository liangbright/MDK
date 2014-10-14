#ifndef __TestScalarImageFilter3D_h
#define __TestScalarImageFilter3D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkDenseImageResampler3D.h"

namespace mdk
{

void test_a()
{
	std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	std::string FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json";

	auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	DenseImageResampler3D<double> Resampler;
	Resampler.SetInputImage(&InputImage);

	auto OutputImageInfo = InputImage.GetInfo();
	OutputImageInfo.Spacing *= 2.0;
	OutputImageInfo.Size = {256, 256, 43};
	Resampler.SetOutputImageInfo(OutputImageInfo);

	auto InterpolationOption = Resampler.GetImageInterpolationOption();
	InterpolationOption.MethodType = DenseImageResampler3D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = DenseImageResampler3D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	InterpolationOption.Pixel_OutsideImage = 0;
	Resampler.SetImageInterpolationOption(InterpolationOption);

	//Resampler.EnableGaussianSmoothWhenDownSmapling(false);

	Resampler.SetMaxNumberOfThread(4);
	Resampler.Update();
	const auto& ResampledImage = *Resampler.GetOutputImage();

	Save3DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);
}

}//namespace mdk

#endif