#pragma once

#include <ctime>
#include <cstdlib>
#include <array>
#include <iostream>
#include <chrono>

#include "mdkDenseImage3D_FileIO.h"
#include "mdkDenseImageResampler3D.h"

#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"
#include "itkGaussianBlurImageFunction.h"
#include "itkImageRegionIterator.h"

namespace mdk
{

void test_a()
{
	//std::cout << "FilePath_InputImage?" << '\n';
	//std::string name;
	//std::cin >> name;

	String FilePath_InputImage = "C:/Research/AorticValve/Data/CT/Normal/2014_7_25/P2115937/phase0/";
	//String FilePath_InputImage =  "G:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler3D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	std::cout << "start" << '\n';
	
	auto t0 = std::chrono::system_clock::now();

	DenseImageResampler3D<double> Resampler;
	Resampler.SetInputImage(&InputImage);

	auto OutputImageInfo = InputImage.GetInfo();
	Resampler.SetOutputImageInfo(OutputImageInfo);
	//OutputImageInfo.Spacing *= 2.0;
	//OutputImageInfo.Size = {256, 256, 43};
	//Resampler.SetOutputImageInfoBySize(256, 256, 43);

	auto InterpolationOption = Resampler.GetImageInterpolationOption();
	InterpolationOption.MethodType = DenseImageResampler3D<double>::ImageInterpolationMethodEnum::Linear;
	InterpolationOption.BoundaryOption = DenseImageResampler3D<double>::ImageInterpolationBoundaryOptionEnum::Replicate;
	InterpolationOption.Pixel_OutsideImage = 0;
	Resampler.SetImageInterpolationOption(InterpolationOption);
	//Resampler.EnableSmoothingWhenDownsampling();
	Resampler.SetMaxThreadCount(8);
	Resampler.Update();
	const auto& ResampledImage = *Resampler.GetOutputImage();

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	Save3DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);

	//
	t0 = std::chrono::system_clock::now();

	DenseImage3D<double> CopyImage;
	CopyImage.SetInfo(InputImage.GetInfo());
	//for (int_max k = 0; k <= InputImage.GetPixelCount()-1; ++k)
	auto TempFunction=[&](int_max k)
	{
		CopyImage[k] = InputImage[k];
	};
	ParallelForLoop(TempFunction, 0, InputImage.GetPixelCount() - 1, 8);

	t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time2 = t1 - t0;
	std::cout << "time " << raw_time2.count() << '\n';
}

void test_b()
{
	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler3D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	auto input = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);
	auto inputSize = input->GetLargestPossibleRegion().GetSize();

	std::cout << "start" << '/n';

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

	std::cout << "done" << '/n';
}

void test_c()
{
	String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	Load3DScalarImageFromJsonDataFile(InputImage, FilePath_InputImage);

	auto inputImage = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);

	std::cout << "start" << '/n';

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

	std::cout << "done" << '/n';
}
}//namespace mdk
