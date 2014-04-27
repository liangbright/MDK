#include <iostream>

#include "TestImageFilter.h"

using namespace mdk;

int main()
{
   
    // initializer_list sytax can be used for this!
    //double a;
    //a = { 1 };

    //error
    //std::vector<double> a;
    //std::initializer_list<double> b=a;

    //
    // attention !
    // this code can run!
    //double a[2];
    //a[1, 1] = 1;


    //test();

    //Test_FilterFunction();

    //Test_FunctionPointer();

    //Test_MultiThread();

    //Test_ConvolutionFilter_VirtualFilterFunction();

    //Test_FunctionTemplate();

    //Test_FunctionTemplate_InputFilterFunction();

    //Test_ConvolutionFilter_ScalarOutput();

    //Test_ConvolutionFilter_VectorOutput();

    //test_Valve_Filter();

    test_GaussianFilter();

    //test_IntegralImageBuider();

    //Test_ComputeHistogram();

    std::system("pause");
}