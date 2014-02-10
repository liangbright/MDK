#include <iostream>

#include "mdkType.h"
#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"
#include "mdkFileIO.h"

#include "TestImageFilter.h"

using namespace mdk;

int main()
{
    double a[2];

    //
    // attention !
    // this code can run!
    a[1, 1] = 1;

    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto Image = ReadGrayScale3DImageFromDICOMFile(FilePath);

	//test();

	//Test_FilterFunction();

	//Test_FunctionPointer();

	//Test_MultiThread();

	//Test_ConvolutionFilter_VirtualFilterFunction();

	//Test_FunctionTemplate();

	//Test_FunctionTemplate_InputFilterFunction();

	//Test_ConvolutionFilter_ScalarOutput();

	//Test_ConvolutionFilter_VectorOutput();

}