#ifndef __TestScalarImageFilter3D_h
#define __TestScalarImageFilter3D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkScalarImageGaussianFilter3D.h"
#include "mdkImageFilter_Common_Function.h"

namespace mdk
{

void test_ScalarImageGaussianFilter3D()
{
    CharString FilePath = "C:/Research/MDK_Build/Test/Test_ImageFilter/Test_ScalarImageFilter/Debug/";

    ScalarImage3D<double> InputImage;

    InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath + "TestImage.json");

    ScalarImageGaussianFilter3D<double, double> imfilter;

    imfilter.SetInputImage(&InputImage);

    DenseMatrix<double> RoationMatrix(3, 3);
    RoationMatrix.FillDiagonal(1);

    imfilter.SetMaskParameter(3, 3, 3, RoationMatrix, 3);

    imfilter.Update();

    auto OutputImage = imfilter.GetOutputImage();

    Save3DScalarImageAsJsonDataFile(*OutputImage, FilePath + "FilteredTestImage.json");

    std::system("pause");
}

}//namespace mdk

#endif