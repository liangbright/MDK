﻿#ifndef __TestScalarImageFilter3D_h
#define __TestScalarImageFilter3D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkDenseImageResampler3D.h"

#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"
#include "itkGaussianBlurImageFunction.h"
#include "itkImageRegionIterator.h"

namespace mdk
{

void test_a()
{
	std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	std::string FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json";

	auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';

	DenseImageResampler3D<double> Resampler;
	Resampler.SetInputImage(&InputImage);

	auto OutputImageInfo = InputImage.GetInfo();
	OutputImageInfo.Spacing *= 2.0;
	OutputImageInfo.Size = {256, 256, 43};
	//Resampler.SetOutputImageInfo(OutputImageInfo);

	Resampler.SetOutputImageInfoBySize(256, 256, 43);

	auto InterpolationOption = Resampler.GetImageInterpolationOption();
	InterpolationOption.MethodType = DenseImageResampler3D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = DenseImageResampler3D<double>::ImageInterpolationBoundaryOptionEnum::Nearest;
	InterpolationOption.Pixel_OutsideImage = 0;
	Resampler.SetImageInterpolationOption(InterpolationOption);

	Resampler.EnableGaussianSmoothWhenDownSampling();

	auto Sigma = OutputImageInfo.Spacing;
	Sigma[0] *= 3;
	Sigma[1] *= 3;
	Resampler.SetParameterOfGaussianSmooth(Sigma, 2);

	Resampler.SetMaxNumberOfThread(4);
	Resampler.Update();
	const auto& ResampledImage = *Resampler.GetOutputImage();

	std::cout << "done" << '\n';

	Save3DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);
}

void test_b()
{
	std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	std::string FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json";

	auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	auto input = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);
	auto inputSize = input->GetLargestPossibleRegion().GetSize();

	std::cout << "start" << '\n';

	typedef itk::Image<double, 3> ITKImageType;

	// Resize
	ITKImageType::SizeType outputSize;
	outputSize[0] = 256;
	outputSize[1] = 256;
	outputSize[2] = 43;

	ITKImageType::SpacingType outputSpacing;
	outputSpacing[0] = input->GetSpacing()[0] * (static_cast<double>(inputSize[0]) / static_cast<double>(outputSize[0]));
	outputSpacing[1] = input->GetSpacing()[1] * (static_cast<double>(inputSize[1]) / static_cast<double>(outputSize[1]));
	outputSpacing[2] = input->GetSpacing()[2] * (static_cast<double>(inputSize[2]) / static_cast<double>(outputSize[2]));

	typedef itk::IdentityTransform<double, 3> TransformType;
	typedef itk::ResampleImageFilter<ITKImageType, ITKImageType> ResampleImageFilterType;
	auto resample = ResampleImageFilterType::New();
	resample->SetInput(input);
	resample->SetSize(outputSize);
	resample->SetOutputSpacing(outputSpacing);
	resample->SetTransform(TransformType::New());
	resample->UpdateLargestPossibleRegion();
	resample->Update();
	auto output = resample->GetOutput();

	std::cout << "done" << '\n';
}

void test_c()
{
	std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//std::string FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	std::string FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json";

	auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	auto inputImage = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);

	std::cout << "start" << '\n';

	typedef itk::Image<double, 3> ImageType;
	typedef itk::ImageRegionIterator< ImageType > IteratorType;
	typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;

	ImageType::RegionType region = inputImage->GetBufferedRegion();

	ConstIteratorType it(inputImage, region);

	ImageType::Pointer output = ImageType::New();

	output->SetRegions(region);
	output->SetOrigin(inputImage->GetOrigin());
	output->SetSpacing(inputImage->GetSpacing());
	output->Allocate();

	IteratorType out(output, region);

	typedef itk::GaussianBlurImageFunction< ImageType > GFunctionType;
	GFunctionType::Pointer gaussianFunction = GFunctionType::New();
	gaussianFunction->SetInputImage(inputImage);

	GFunctionType::ErrorArrayType setError;
	setError.Fill(0.01);
	gaussianFunction->SetMaximumError(setError);
	gaussianFunction->SetSigma(2.5);
	gaussianFunction->SetMaximumKernelWidth(6);

	it.GoToBegin();
	out.GoToBegin();

	while (!it.IsAtEnd())
	{
		out.Set(gaussianFunction->EvaluateAtIndex(it.GetIndex()));
		++it;
		++out;
	}

	std::cout << "done" << '\n';
}
}//namespace mdk

#endif