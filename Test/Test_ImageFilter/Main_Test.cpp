#include <iostream>

#include "mdkType.h"
#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"

#include "TestImageFilter.h"

using namespace mdk;

int main(int argc, char** argv)
{
	//test();

	//Test_FilterFunction();

	//Test_FunctionPointer();

	//Test_MultiThread();

	//Test_ConvolutionFilter_VirtualFilterFunction();

	//Test_FunctionTemplate();

	//Test_FunctionTemplate_InputFilterFunction();

	Test_ConvolutionFilter_ScalarOutput();

	//Test_ConvolutionFilter_VectorOutput();
}