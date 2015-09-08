#ifndef Test_IntegralImageBuilder_h
#define Test_IntegralImageBuilder_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkIntegralImageBuilder2D.h"
#include "mdkIntegralImageBasedImageAverageFilter2D.h"
#include "mdkScalarDenseImageAverageFilter2D.h"
#include "mdkDenseImage2D_FileIO.h"

#include "mdkIntegralImageBuilder3D.h"
#include "mdkIntegralImageBasedImageAverageFilter3D.h"
#include "mdkScalarDenseImageAverageFilter3D.h"
#include "mdkDenseImage3D_FileIO.h"

void test_2D()
{
	using namespace mdk;

	String InputFile = "C:/Research/SpineAnalysis/TestData/Patient1/T2W/T2W0005.dcm";
	String OutputPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_IntegralImageBuilder/TestData/";

	DenseImage2D<double> InputImage;
	Load2DScalarImageFromSingleDICOMFile(InputImage, InputFile);

	std::cout << "start IntegralImageBuilder2D" << '\n';
	IntegralImageBuilder2D<double> imbuilder;
	imbuilder.SetInputImage(&InputImage);	
	imbuilder.Update();
	auto IntegralImageA = imbuilder.GetOutputImage();
	std::cout << "OK" << '\n';

	std::cout << "start IntegralImageBasedImageAverageFilter2D" << '\n';
	IntegralImageBasedImageAverageFilter2D<double> AverageFilter1;
	AverageFilter1.SetInputImage(&InputImage);
	//AverageFilter1.SetIntegralImage(&IntegralImage);
	AverageFilter1.SetOutputImageInfo(InputImage.GetInfo());
	AverageFilter1.EnableOutputImage();
	AverageFilter1.SetRadius(2, 2);
	AverageFilter1.SetMaxThreadCount(4);
	AverageFilter1.Update();
	auto& AverageImage1 = *AverageFilter1.GetOutputImage();
	auto IntegralImageB = AverageFilter1.GetIntegralImage();
	std::cout << "OK" << '\n';

	Save2DScalarImageAsJsonDataFile(*IntegralImageB, OutputPath + "IntegralImage.json");

	Save2DScalarImageAsJsonDataFile(AverageImage1, OutputPath + "AverageImage1.json");

	std::cout << "start ScalarDenseImageAverageFilter2D" << '\n';
	ScalarDenseImageAverageFilter2D<double> AverageFilter2;
	auto InterpolationOption_GF = AverageFilter2.GetImageInterpolationOption();
	InterpolationOption_GF.MethodType = ScalarDenseImageAverageFilter2D<double>::ImageInterpolationMethodEnum::Nearest;
	InterpolationOption_GF.BoundaryOption = ScalarDenseImageAverageFilter2D<double>::ImageInterpolationBoundaryOptionEnum::Replicate;// must use this option
	InterpolationOption_GF.Pixel_OutsideImage = 0;
	AverageFilter2.SetImageInterpolationOption(InterpolationOption_GF);
	AverageFilter2.SetInputImage(&InputImage);
	AverageFilter2.SetOutputImageInfo(InputImage.GetInfo());
	AverageFilter2.SetPoolingRadius(3);
	AverageFilter2.SetMaxThreadCount(1);
	AverageFilter2.Update();
	auto& AverageImage2 = *AverageFilter2.GetOutputImage();
	std::cout << "OK" << '\n';
	Save2DScalarImageAsJsonDataFile(AverageImage2, OutputPath + "AverageImage2.json");

}

void test_3D()
{
	using namespace mdk;

    String FilePath = "G:/AorticValveData/2014_7_25/P2115937/phase0/";
	String TestPath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_IntegralImageBuilder/TestData/";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath);

	std::cout << "start IntegralImageBuilder3D" << '\n';
	IntegralImageBuilder3D<double> imbuilder;
	imbuilder.SetInputImage(&InputImage);
	imbuilder.SetMaxThreadCount(6);
	imbuilder.Update();
	auto IntegralImageA = imbuilder.GetOutputImage();
	std::cout << "OK" << '\n';

	std::cout << "start IntegralImageBasedImageAverageFilter3D" << '\n';
	IntegralImageBasedImageAverageFilter3D<double> AverageFilter1;
	AverageFilter1.SetInputImage(&InputImage);
	//AverageFilter1.SetIntegralImage(&IntegralImage);
	AverageFilter1.SetOutputImageInfo(InputImage.GetInfo());
	AverageFilter1.EnableOutputImage();
	AverageFilter1.SetRadius(2, 2, 3);
	AverageFilter1.SetMaxThreadCount(6);
	AverageFilter1.Update();
	auto& AverageImage1 = *AverageFilter1.GetOutputImage();
	auto IntegralImageB = AverageFilter1.GetIntegralImage();
	std::cout << "OK" << '\n';

	Save3DScalarImageAsJsonDataFile(*IntegralImageB, TestPath + "IntegralImage.json");

	Save3DScalarImageAsJsonDataFile(AverageImage1, TestPath + "AverageImage1.json");

	std::cout << "start ScalarDenseImageAverageFilter3D" << '\n';
	ScalarDenseImageAverageFilter3D<double> AverageFilter2;
	auto InterpolationOption_GF = AverageFilter2.GetImageInterpolationOption();
	InterpolationOption_GF.MethodType = ScalarDenseImageAverageFilter3D<double>::ImageInterpolationMethodEnum::Nearest;
	InterpolationOption_GF.BoundaryOption = ScalarDenseImageAverageFilter3D<double>::ImageInterpolationBoundaryOptionEnum::Replicate;// must use this option
	InterpolationOption_GF.Pixel_OutsideImage = 0;
	AverageFilter2.SetImageInterpolationOption(InterpolationOption_GF);
	AverageFilter2.SetInputImage(&InputImage);
	AverageFilter2.SetOutputImageInfo(InputImage.GetInfo());
	AverageFilter2.SetPoolingRadius(3);
	AverageFilter2.SetMaxThreadCount(6);
	AverageFilter2.Update();
	auto& AverageImage2 = *AverageFilter2.GetOutputImage();
	std::cout << "OK" << '\n';		
	Save3DScalarImageAsJsonDataFile(AverageImage2, TestPath + "AverageImage2.json");

}

#endif