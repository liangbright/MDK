#ifndef __TestScalarImageFilter3D_h
#define __TestScalarImageFilter3D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"
#include "mdkHistogram.h"
#include "mdkString.h"

namespace mdk
{

void test_ScalarImageGaussianFilter3D()
{
    std::string FilePath = "C:/Research/MDK_Build/Test/Test_ImageFilter/Test_ScalarImageFilter/Debug/";

	DenseImage3D<double> InputImage;
	Load3DScalarImageFromJsonDataFile(InputImage, FilePath + "TestImage.json");

	ScalarDenseImageGaussianFilter3D<double> imfilter;

    imfilter.SetInputImage(&InputImage);

    DenseMatrix<double> RoationMatrix(3, 3);
    RoationMatrix.FillDiagonal(1);
	DenseVector <double, 3> Sigma;
	Sigma = { 3, 3, 3 };
	imfilter.SetGaussianParameter(Sigma, RoationMatrix, 3);

	auto Option = imfilter.GetImageInterpolationOption();
	Option.MethodType = Image3DInterpolationMethodEnum::Linear;
	Option.BoundaryOption = Image3DInterpolationBoundaryOptionEnum::Constant;
	Option.Pixel_OutsideImage = 0;
	imfilter.SetImageInterpolationOption(Option);

    imfilter.Update();

    auto& OutputImage = *imfilter.GetOutputImage();

    Save3DScalarImageAsJsonDataFile(OutputImage, FilePath + "FilteredTestImage.json");

    std::system("pause");
}

}//namespace mdk

#endif