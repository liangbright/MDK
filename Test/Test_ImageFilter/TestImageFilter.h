#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>
#include <cstdlib>

#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"
#include "mdk3DImageConvolutionFilter.h"

namespace mdk
{

void Tempfunction(double& a, int N)
{
	a = 0;
	for (double i = 0; i < N; ++i)
	{
		a += i;
	}
}

inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, const mdk3DImage<double>& InputImage, double& Output)
{
	//std::cout << "FilterFuntion " << '\n';

	Output = 0;

	double N = 100;

	//auto x = double(xIndex);
	//auto y = double(yIndex);
	//auto z = double(zIndex);

	for (double i = 0; i < N; ++i)
	{
		//Output += i * InputImage(xIndex, yIndex, zIndex);
		//Output += i * InputImage(uint64(x), uint64(y), uint64(z));

		Output += i*(i+1);
	}
}

void Test_FunctionPointer()
{
	int N = 3000;

	double Value = 0;
	auto t0 = std::time(0);
	for (uint64 i = 0; i < 1000000; ++i)
	{
		Tempfunction(Value, N);
	}
	auto t1 = std::time(0);
	std::cout << "Tempfunction_time = " << t1 - t0 << '\n';

	auto TempfunctionPtr = &Tempfunction;

	Value = 0;
	auto t2 = std::time(0);
	for (uint64 i = 0; i < 1000000; ++i)
	{
		(*TempfunctionPtr)(Value, N);
	}
	auto t3 = std::time(0);
	std::cout << "TempfunctionPtr_time = " << t3 - t2 << '\n';

	std::system("pause");
}


void Test_MultiThread()
{
	uint64 Lx = 512;
	uint64 Ly = 512;
	uint64 Lz = 512;

	mdk3DImage<double> InputImage;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	mdk3DImage<double> OutputImage;

	OutputImage.Initialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	mdk3DImageFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);

	imfilter.SetFilterFunction(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';


	std::cout << "Pure function " << '\n';
	
	t0 = std::time(0);

	double Output = 0;

	double Value = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);

		Value += Output;
	}

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::function<void(uint64, uint64, uint64, const mdk3DImage<double>&, double&)> stdFunction = FilterFunction;

	
	std::cout << "std function " << '\n';

	t0 = std::time(0);

	Output = 0;

	Value = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		stdFunction(0, 0, 0, InputImage, Output);

		Value += Output;
	}

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';


	auto FunctionPtr = &FilterFunction;

	std::cout << "FunctionPtr " << '\n';

	t0 = std::time(0);

	Output = 0;

	Value = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		(*FunctionPtr)(0, 0, 0, InputImage, Output);

		Value += Output;
	}

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::system("pause");
 }


void Test_ConvolutionFilter_ScalarOutput()
{
	uint64 Lx = 512;
	uint64 Ly = 512;
	uint64 Lz = 512;

	mdk3DImage<double> InputImage;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	mdk3DImage<double> OutputImage;

	OutputImage.Initialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	mdk3DImageConvolutionFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetFilterFunction(FilterFunction);

	imfilter.SetMaxThreadNumber(1);

	imfilter.EnableBoundCheck(false);

	mdkMatrix<double> Mask(4, 1000*36*36/2);

	Mask.Fill(0);

	imfilter.SetMask(Mask);

	std::time_t t0 = std::time(0);

	imfilter.Run();
	
	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::system("pause");
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

	uint64 Lx = 70;
	uint64 Ly = 80;
	uint64 Lz = 60;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1.0);

	mdk3DImage<std::vector<double>> OutputImage;

	OutputImage.Initialize(Lx, Ly, Lz);

	
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
		MaskList[i].SetSize(4, 1000);
		MaskList[i].Fill(0);
	}

	imfilter.SetMask(MaskList);
	
	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::system("pause");
}


}//end of namespace


#endif