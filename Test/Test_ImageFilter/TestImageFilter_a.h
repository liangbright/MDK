#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>
#include <cstdlib>
#include <array>

//#include "mdkMatrix.h"
#include "mdk3DImage.h"

using namespace mdk;


void Test_ConvolutionFilter_VoxelType()
{
	uint64 Lx = 100;
	uint64 Ly = 100;
	uint64 Lz = 100;

    
	//mdk3DImage<double> InputImage;
    /*
	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1);
    
	mdk3DImage<double> OutputImage_scalar;

    OutputImage_scalar.Initialize(Lx, Ly, Lz);

    OutputImage_scalar.Fill(0);

	mdk3DImageConvolutionFilter<double, double>  imfilter_scalar;

    imfilter_scalar.SetInputImage(&InputImage);

    imfilter_scalar.SetOutputImage(&OutputImage_scalar);

   
    mdk3DImage<std::array<double, 2>> OutputImage_array;

    mdk3DImageConvolutionFilter<double, std::array<double, 2>, 2>  imfilter_array;

    imfilter_array.SetInputImage(&InputImage);

    imfilter_array.SetOutputImage(&OutputImage_array);

    imfilter_array.SetMaxThreadNumber(1);
    */
}


#endif