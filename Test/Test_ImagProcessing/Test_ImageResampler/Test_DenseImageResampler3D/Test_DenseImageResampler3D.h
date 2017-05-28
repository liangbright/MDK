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
#include "itkLinearInterpolateImageFunction.h"

namespace mdk
{

void test_a()
{
	//std::cout << "FilePath_InputImage?" << '\n';
	//std::string name;
	//std::cin >> name;

	//String FilePath_InputImage = "C:/Research/AorticValve/Data/CT/Normal/2014_7_25/P2115937/phase0/";
	String FilePath_InputImage =  "G:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler3D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);
	auto InputImageInfo = InputImage.GetInfo();

	std::cout << "test_a start" << '\n';
	
	auto t0 = std::chrono::system_clock::now();

	DenseImageResampler3D<double> Resampler;
	Resampler.Input.Image = &InputImage;
	
	//Resampler.SetOutputImageInfo(OutputImageInfo);
	//OutputImageInfo.Spacing *= 2.0;
	//OutputImageInfo.Size = {256, 256, 43};
	Resampler.SetOutputImageInfoBySize(256, 256, 43);

	Resampler.Input.ImageInterpolationOption.MethodType = DenseImageResampler3D<double>::ImageInterpolationMethodEnum::Linear;
	Resampler.Input.ImageInterpolationOption.BoundaryOption = DenseImageResampler3D<double>::ImageInterpolationBoundaryOptionEnum::Constant;
	Resampler.Input.ImageInterpolationOption.Pixel_OutsideImage = 0;
	//Resampler.EnableTriangleSmoothingWhenDownsampling();
	Resampler.Input.MaxThreadCount = 8;
	Resampler.Update();
	auto& ResampledImage = Resampler.Output.Image;

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	Save3DScalarImageAsJsonDataFile(ResampledImage, FileNameAndPath_OutputImage);

	//
	
	t0 = std::chrono::system_clock::now();

	DenseImage3D<double> TestImage;
	TestImage.SetInfo(ResampledImage.GetInfo());	

	//for (int_max k = 0; k <= ResampledImage.GetPixelCount()-1; ++k)
	auto TempFunction=[&](int_max k)
	{
		auto Pos3D = TestImage.TransformLinearIndexTo3DPosition<double>(k);		
		//auto PosW = InputImage.Transform3DPositionTo3DWorldPosition<double>(Pos3D);
		//auto Index = InputImage.Transform3DWorldPositionTo3DIndex(PosW);
		TestImage[k] = InputImage.GetPixelAt3DPosition(Pos3D, Resampler.Input.ImageInterpolationOption);
		//TestImage[k] = InputImage.GetPixelNearestTo3DPosition(Pos3D);
        //TestImage[k] = 1;
	};
	ParallelForLoop(TempFunction, 0, ResampledImage.GetPixelCount() - 1, 8);

	t1 = std::chrono::system_clock::now();
	raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	auto Size = ResampledImage.GetSize();

	//---------------------------------------------------------------------
	TestImage.Clear();

	t0 = std::chrono::system_clock::now();

	TestImage.SetInfo(ResampledImage.GetInfo());

	//for (int_max z = 0; z <= Size[2]-1; ++z)
	auto TempFunctionZ = [&](int_max z)	
	{
		for (int_max y = 0; y < Size[1]; ++y)
		{
			for (int_max x = 0; x < Size[0]; ++x)
			{
				auto Pos3D = TestImage.Transform3DIndexTo3DPosition<double>(x, y, z);
				TestImage(x,y,z) = InputImage.GetPixelAt3DPosition(Pos3D, Resampler.Input.ImageInterpolationOption);
			}
		}
	};
	ParallelForLoop(TempFunctionZ, 0, Size[2] - 1, 8);

	t1 = std::chrono::system_clock::now();
	raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';
}

void test_b()
{
	String FilePath_InputImage = "G:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String Test_Path = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler3D/TestData/";

	String FileNameAndPath_OutputImage = Test_Path + "test_b_ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	//auto InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath_InputImage);

	auto input = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);
	auto inputSize = input->GetLargestPossibleRegion().GetSize();

	std::cout << "test_b start" << '\n';

	auto t0 = std::chrono::system_clock::now();

	typedef itk::Image<double, 3> ITKImageType;

	auto Origin = InputImage.GetOrigin();
	// Resize
	ITKImageType::SizeType outputSize;
	outputSize[0] = 256;
	outputSize[1] = 256;
	outputSize[2] = 43;

	ITKImageType::SpacingType outputSpacing;
	outputSpacing[0] = input->GetSpacing()[0] * (static_cast<double>(inputSize[0]-1) / static_cast<double>(outputSize[0]-1));
	outputSpacing[1] = input->GetSpacing()[1] * (static_cast<double>(inputSize[1]-1) / static_cast<double>(outputSize[1]-1));
	outputSpacing[2] = input->GetSpacing()[2] * (static_cast<double>(inputSize[2]-1) / static_cast<double>(outputSize[2]-1));

	typedef itk::IdentityTransform<double, 3> TransformType;
	typedef itk::ResampleImageFilter<ITKImageType, ITKImageType> ResampleImageFilterType;
	
	ResampleImageFilterType::OriginPointType outputOrigin;
	outputOrigin[0] = Origin[0];
	outputOrigin[1] = Origin[1];
	outputOrigin[2] = Origin[2];

	auto Interpolator = itk::LinearInterpolateImageFunction<ITKImageType, double>::New();

	auto resample = ResampleImageFilterType::New();
	resample->SetInput(input);
	resample->SetSize(outputSize);
	resample->SetOutputOrigin(outputOrigin);
	resample->SetOutputSpacing(outputSpacing);
	resample->SetTransform(TransformType::New());
	resample->UpdateLargestPossibleRegion();
	resample->SetInterpolator(Interpolator);
	//resample->Update();
	auto output = resample->GetOutput();

	auto t1 = std::chrono::system_clock::now();
	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << "time " << raw_time.count() << '\n';

	std::cout << "done" << '\n';

	DenseImage3D<double> OutputImage;
	ConvertITK3DScalarImageToMDK3DScalarImage(output, OutputImage);
	Save3DScalarImageAsJsonDataFile(OutputImage, FileNameAndPath_OutputImage);
}

void test_c()
{
	String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/phase0";
	//String FilePath_InputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/TestImage.json";

	String FileNameAndPath_OutputImage = "H:/AorticValveData/2014_7_25/P2115937/Detection/test_c_ResampledImage.json";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath_InputImage);
	Load3DScalarImageFromJsonDataFile(InputImage, FilePath_InputImage);

	auto inputImage = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage);

	std::cout << "test_c start" << '/n';

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
