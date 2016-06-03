#pragma once

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkDenseImage3D_FileIO.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"
#include "mdkHistogram.h"
#include "mdkString.h"

namespace mdk
{

void test_ScalarDenseImageGaussianFilter3D()
{
    String FilePath = "C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ScalarImageFilter/TestData/3/";

	DenseImage3D<double> InputImage;
	//Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");
	Load3DScalarImageFromDICOMSeries(InputImage, FilePath);

	DenseImage3D<double> InputImage2;
	Load3DScalarImageFromSingleDICOMFile(InputImage2, "Z:/sun-lab/Liang_Liang/DrPadala-3D Echo Segmentation/3DEcho_DrPadala/Ring.dcm");//wrong spacing etc

	//InputImage.SetSpacing(1.0, 1.0, 1.0);
	//InputImage.SetSize(100, 100, 30);

	ScalarDenseImageGaussianFilter3D<double> imfilter;

    imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImageInfo(InputImage.GetInfo());

    DenseMatrix<double> RoationMatrix(3, 3);
    RoationMatrix.FillDiagonal(1);
	DenseVector <double, 3> Sigma;
	Sigma = { 3, 3, 3 };
	imfilter.SetGaussianParameter(Sigma, RoationMatrix, 3);

	auto Option = imfilter.GetImageInterpolationOption();
	Option.MethodType = Image3DInterpolationMethodEnum::Nearest;
	Option.BoundaryOption = Image3DInterpolationBoundaryOptionEnum::Constant;
	Option.Pixel_OutsideImage = 0;
	imfilter.SetImageInterpolationOption(Option);

	imfilter.SetMaxThreadCount(8);

    imfilter.Update();

    auto& OutputImage = *imfilter.GetOutputImage();

    Save3DScalarImageAsJsonDataFile(OutputImage, FilePath + "Data.json");

    std::system("pause");
}

}//namespace mdk
