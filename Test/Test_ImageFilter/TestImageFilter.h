#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>

#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"
#include "mdk3DImageConvolutionFilter.h"

namespace mdk
{

void FilterFuntion(uint64 xIndex, uint64 yIndex, uint64 zIndex, const mdk3DImage<double>& InputImage, double& Output)
{
	Output = 0;

	double N = 10;

	for (double i = 0; i < N*N*N; ++i)
	{
		Output += i * InputImage(xIndex, yIndex, zIndex);
	}
}

void Test_MultiThread()
{
	mdk3DImage<double> InputImage;

	InputImage.Initialize(512, 512, 512);

	InputImage.Fill(1);

	mdk3DImage<double> OutputImage;

	OutputImage.Initialize(512, 512, 512);

	OutputImage.Fill(0);

	mdk3DImageFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(8);

	imfilter.SetFilterFunction(FilterFuntion);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';


	t0 = std::time(0);

	double value = 0;

	double Output = 0;

	uint64 N = 512*512*512;

	for (uint64 i = 0; i < N; ++i)
	{
		FilterFuntion(0, 0, 0, InputImage, Output);
		value += Output;
	}

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';
}


void Test_ConvolutionFilter_ScalarOutput()
{
	double a(1);

	mdk3DImage<double> InputImage;

	InputImage.Initialize(512, 512, 512);

	InputImage.Fill(1);

	mdk3DImage<double> OutputImage;

	OutputImage.Initialize(512, 512, 512);

	OutputImage.Fill(0);

	mdk3DImageConvolutionFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(8);

	mdkMatrix<double> Mask(4, 300);

	Mask.Fill(0);

	imfilter.SetMask(Mask);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';
}

void Test_ConvolutionFilter_VectorOutput()
{
	uint64 VoxelDimension = 1;

	std::vector<double> ZeroVoxel(VoxelDimension);
	std::cout << "ZeroVoxel Memory (Byte) is " << ZeroVoxel.capacity()*sizeof(double)+sizeof(ZeroVoxel) << '\n';

	std::cout << "sizeof(double)" << sizeof(double) << '\n';

	std::cout << "sizeof(std::vector<double>)" << sizeof(std::vector<double>) << '\n';

	std::vector<std::vector<double>> v(512*512*512);
	std::cout << "v Memory (G) is " << double(v.capacity()*sizeof(std::vector<double>) + sizeof(v)) / (1024 * 1024 * 1024) << '\n';

	for (uint64 i = 0; i < v.size(); ++i)
	{ 
		v[i] = ZeroVoxel;
	}

	mdk3DImage<double> InputImage;

	InputImage.Initialize(512, 512, 512);

	InputImage.Fill(1.0);

	mdk3DImage<std::vector<double>> OutputImage;

	OutputImage.Initialize(512, 512, 512);

	
	std::time_t t01 = std::time(0);

	OutputImage.Fill(ZeroVoxel);
	
	auto ArrayPtr = OutputImage.GetVoxelDataArrayPointer();

	std::cout << "(*ArrayPtr).capacity()" << (*ArrayPtr).capacity() << '\n';

	std::time_t t02 = std::time(0);

	std::cout << "OutputImage.Fill time " << t02 - t01 << '\n';

	mdk3DImageConvolutionFilter<double, std::vector<double>>  imfilter;
	
	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(8);
	
	std::vector<mdkMatrix<double>> MaskList;
	MaskList.resize(VoxelDimension);

	for (uint64 i = 0; i < VoxelDimension; ++i)
	{
		MaskList[i].SetSize(4, 100);
		MaskList[i].Fill(0);
	}

	imfilter.SetMask(MaskList);
	
	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';
}


}//end of namespace


#endif