﻿#ifndef __TestScalarImageFilter3D_h
#define __TestScalarImageFilter3D_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkScalarImageGaussianFilter3D.h"
#include "mdkHistogram.h"
#include "mdkString.h"

namespace mdk
{

void test_ScalarImageGaussianFilter3D()
{
    std::string FilePath = "C:/Research/MDK_Build/Test/Test_ImageFilter/Test_ScalarImageFilter/Debug/";

    Image3D<double> InputImage;

    InputImage = Load3DScalarImageFromJsonDataFile<double>(FilePath + "TestImage.json");

    ScalarImageGaussianFilter3D<double> imfilter;

    imfilter.SetInputImage(&InputImage);

    DenseMatrix<double> RoationMatrix(3, 3);
    RoationMatrix.FillDiagonal(1);

    imfilter.SetMaskParameter(3, 3, 3, RoationMatrix, 3);
	imfilter.Use3DIndexInMask();

    imfilter.Update();

    auto& OutputImage = imfilter.OutputImage();

    Save3DScalarImageAsJsonDataFile(OutputImage, FilePath + "FilteredTestImage.json");

    std::system("pause");
}

}//namespace mdk

#endif