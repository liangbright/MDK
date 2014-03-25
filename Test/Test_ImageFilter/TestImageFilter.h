﻿#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkFileIO.h"
#include "mdkDenseMatrix.h"
#include "mdkImage.h"
#include "mdkImageFilter.h"
#include "mdkImageConvolutionFilter.h"
#include "mdkImageGaussianFilter.h"
#include "mdkIntegralImageBuilder.h"

namespace mdk
{

template<typename T>
class TestClass
{
public:
	TestClass(){};
	~TestClass(){};

	T  t;

	template<typename U>
	void fun(U u)
	{
		std::cout << "fun: " << u << '\n';
	}
};

void test()
{
	TestClass<double>  A;

	//auto aaa = std::thread(&TestClass<double>::fun<int>, &A, 1);

	auto aaa = std::thread([&]{ A.fun(1); });
}

template<typename T>
void Templatefunction(T a)
{
	a = a;
}

void Tempfunction(double& a, int N)
{
	a = 0;

	double b = 0;

	for (double i = 0; i < N; ++i)
	{
		b += i;
		//a += i;
	}

	a = b;
}


inline void FilterFunction(int64 xIndex, int64 yIndex, int64 zIndex, const Image<double>& InputImage, double& Output)
{
	//std::cout << "FilterFuntion " << '\n';

	Output = 0;

	double N = 1000;

	//auto x = double(xIndex);
	//auto y = double(yIndex);
	//auto z = double(zIndex);

	double Value = 0;

	for (double i = 0; i < N; ++i)
	{
		Value += i * InputImage(xIndex, yIndex, zIndex);

		 //Output += i * InputImage(xIndex, yIndex, zIndex);

		//Output += i * InputImage(int64(x), int64(y), int64(z));
	}

	Output = Value;
}


void Test_FilterFunction()
{
	int64 Lx = 512;
	int64 Ly = 512;
	int64 Lz = 512;

	Image<double> InputImage;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	std::cout << "FilterFunction function " << '\n';

	auto t0 = std::time(0);

	double Output = 0;

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	auto t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::system("pause");
}


void Test_FunctionPointer()
{
	int N = 8000;

	double Value = 0;
	auto t0 = std::time(0);
	for (int64 i = 0; i < 1000000; ++i)
	{
		Tempfunction(Value, N);
	}
	auto t1 = std::time(0);
	std::cout << "Tempfunction_time = " << t1 - t0 << '\n';

	auto TempfunctionPtr = &Tempfunction;

	Value = 0;
	auto t2 = std::time(0);
	for (int64 i = 0; i < 1000000; ++i)
	{
		(*TempfunctionPtr)(Value, N);
	}
	auto t3 = std::time(0);
	std::cout << "TempfunctionPtr_time = " << t3 - t2 << '\n';

	std::system("pause");
}


void Test_FunctionTemplate_InputFilterFunction()
{
	int64 Lx = 512;
	int64 Ly = 512;
	int64 Lz = 512;

	Image<double> InputImage;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	Image<double> OutputImage;

	OutputImage.ReInitialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	ImageFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);

	imfilter.SetInputFilterFunctionAt3DIndex(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Update();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter.Update() time " << t1 - t0 << '\n';

	t0 = std::time(0);

//	imfilter.Update(FilterFunction);

	t1 = std::time(0);

	std::cout << "imfilter.Update(FilterFunction) time " << t1 - t0 << '\n';

	std::cout << "FilterFunction function " << '\n';

	t0 = std::time(0);

	double Output = 0;

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::system("pause");
}


void Test_MultiThread()
{
	int64 Lx = 512;
	int64 Ly = 512;
	int64 Lz = 512;

	Image<double> InputImage;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	Image<double> OutputImage;

	OutputImage.ReInitialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	ImageFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);

    imfilter.SetInputFilterFunctionAt3DIndex(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Update();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';


	std::cout << "FilterFunction function " << '\n';
	
	t0 = std::time(0);

	double Output = 0;

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	/*
	std::function<void(int64, int64, int64, const Image<double>&, double&)> stdFunction = FilterFunction;

	std::cout << "std function " << '\n';

	t0 = std::time(0);

	Output = 0;

	Value = 0;

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
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

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		(*FunctionPtr)(0, 0, 0, InputImage, Output);

		Value += Output;
	}

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	*/

	std::system("pause");
 }


void Test_ConvolutionFilter_VirtualFilterFunction()
{
	int64 Lx = 512;
	int64 Ly = 512;
	int64 Lz = 512;

	Image<double> InputImage;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	Image<double> OutputImage;

	OutputImage.ReInitialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	ImageFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);

    imfilter.SetInputFilterFunctionAt3DIndex(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Update();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter time " << t1 - t0 << '\n';

	ImageConvolutionFilter<double, double>  imconvfilter;

	imconvfilter.SetInputImage(&InputImage);

	imconvfilter.SetOutputImage(&OutputImage);

    imconvfilter.SetInputFilterFunctionAt3DIndex(FilterFunction);

	imconvfilter.SetMaxThreadNumber(1);

	imconvfilter.EnableBoundCheck(false);

	DenseMatrix<double> Mask(4, 1000 * 36 * 36 / 2);

	Mask.Fill(0);

	imconvfilter.SetMaskOf3DIndex(Mask);

	t0 = std::time(0);

	imfilter.Update();

	t1 = std::time(0);

	std::cout << "ImageConvolutionFilter time " << t1 - t0 << '\n';

	std::cout << "FilterFunction " << '\n';

	t0 = std::time(0);

	double Output = 0;

	for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	t1 = std::time(0);

	std::cout << "FilterFunction time " << t1 - t0 << '\n';

	std::system("pause");
}


void Test_ConvolutionFilter_ScalarOutput()
{
	int64 Lx = 100;
	int64 Ly = 100;
	int64 Lz = 100;

	Image<double> InputImage;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	Image<double> OutputImage;

	OutputImage.ReInitialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	ImageConvolutionFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(4);

	//imfilter.EnableBoundCheck(false);

    int64 NeighbourVoxelNumber = 10000;

    DenseMatrix<double> Mask(4, NeighbourVoxelNumber);

	Mask.Fill(0);

    imfilter.SetMaskOf3DIndex(Mask);

    auto t0 = std::chrono::system_clock::now();

	imfilter.Update();
	
    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;

    std::cout << "time = " << raw_time.count() << '\n';

	std::cout << "FilterFunction " << '\n';

    t0 = std::chrono::system_clock::now();

	double Output = 0;

    for (int64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

    t1 = std::chrono::system_clock::now();

    raw_time = t1 - t0;

    std::cout << "FilterFunction time = " << raw_time.count() << '\n';

	std::system("pause");
}


void Test_ConvolutionFilter_VectorOutput()
{
	
	/*
	std::vector<double> ZeroVoxel;
	std::cout << "ZeroVoxel Memory (Byte) is " << ZeroVoxel.capacity()*sizeof(double)+sizeof(ZeroVoxel) << '\n';

	std::cout << "sizeof(double)" << sizeof(double) << '\n';

	std::cout << "sizeof(std::vector<double>)" << sizeof(std::vector<double>) << '\n';

	std::vector<std::vector<double>> v(512*512*512);
	std::cout << "v Memory (G) is " << double(v.capacity()*sizeof(std::vector<double>) + sizeof(v)) / (1024 * 1024 * 1024) << '\n';

	for (int64 i = 0; i < v.size(); ++i)
	{ 
		v[i] = ZeroVoxel;
	}
	*/

	/*

	std::vector<std::array<double, 5>> tempdata;

	tempdata.resize(512 * 512 * 512);

	std::cout << "sizeof(tempdata)" << sizeof(tempdata)*tempdata.size() << '\n';

	auto tempdataptr = new std::vector<std::array<double, 5>>;

	tempdataptr->resize(512 * 512 * 512);

	std::cout << "sizeof(tempdata)" << sizeof(*tempdataptr)*tempdataptr->size() << '\n';
	*/

    /*

	int64 OuputVoxelDimension = 2;

	Image<double> InputImage;

	int64 Lx = 512;
	int64 Ly = 512;
	int64 Lz = 512;

	InputImage.ReInitialize(Lx, Ly, Lz);

	InputImage.Fill(1.0);

	Image<std::array<double, 2>> OutputImage;

	OutputImage.ReInitialize(Lx, Ly, Lz);

	std::array<double, 2> OutputZeroVoxel = { 0, 0 };

	std::time_t t01 = std::time(0);

	OutputImage.Fill(OutputZeroVoxel);

	std::time_t t02 = std::time(0);

	std::cout << "OutputImage.Fill time " << t02 - t01 << '\n';

	auto ArrayPtr = OutputImage.GetVoxelDataArrayPointer();

	std::cout << "(*ArrayPtr).capacity()" << (*ArrayPtr).capacity() << '\n';

	ImageConvolutionFilter<double, std::array<double, 2>, 2>  imfilter;
	
	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);
	
	std::vector<DenseMatrix<double>> MaskList;
	MaskList.resize(OuputVoxelDimension);

	for (int64 i = 0; i < OuputVoxelDimension; ++i)
	{
		MaskList[i].SetSize(4, 1000);
		MaskList[i].Fill(0);
	}

    imfilter.SetMaskOf3DIndex(MaskList);
	
	std::time_t t0 = std::time(0);

	imfilter.Update();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter time " << t1 - t0 << '\n';

	std::system("pause");
    */
}


void test_Valve_Filter()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto InputImage = LoadGrayScaleImageFromDICOMFile<double>(FilePath);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0_OutputImage");

    auto InputDimension = InputImage.GetDimension();

    Image<double> OutputImage;

    OutputImage.ReInitialize(InputDimension.Lx, InputDimension.Ly, InputDimension.Lz);

    OutputImage.Fill(0);

    ImageConvolutionFilter<double, double>  imfilter;

    imfilter.SetInputImage(&InputImage);

    imfilter.SetOutputImage(&OutputImage);

    imfilter.SetMaxThreadNumber(4);

    DenseMatrix<double> Mask(4, 7);

    Mask = { -100,  0,   0,    0,   1,   1,   10,
             -1, -1,   0,    0,   0,   1,   1,
             -1, -1,   -1,   0,   0,   0,   1,
             0.1, 0.1, 0.1, -0.6, 0.1, 0.1, 0.1 };

    imfilter.SetMaskOf3DIndex(Mask);

    std::cout << "start: " << '\n';

    std::time_t t0 = std::time(0);

    imfilter.Update();

    std::time_t t1 = std::time(0);

    std::cout << "time " << t1 - t0 << '\n';

    SaveGrayScaleImageAsDataFile(OutputFilePathAndName, OutputImage);

    std::system("pause");
}

void test_GaussianFilter()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto InputImage = LoadGrayScaleImageFromDICOMFile<double>(FilePath);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0_OutputImage");

    auto InputDimension = InputImage.GetDimension();

    Image<double> OutputImage;

    OutputImage.ReInitialize(InputDimension.Lx, InputDimension.Ly, InputDimension.Lz);

    OutputImage.Fill(0);

    ImageGaussianFilter<double, double>  imfilter;

    imfilter.SetInputImage(&InputImage);

    imfilter.SetOutputImage(&OutputImage);

    imfilter.SetMaxThreadNumber(4);

    imfilter.SetSigmaList(3, 4, 5);

    imfilter.SetCutOffRatio(2);

    std::cout << "start: " << '\n';

    std::time_t t0 = std::time(0);

    imfilter.Update();

    std::time_t t1 = std::time(0);

    std::cout << "time " << t1 - t0 << '\n';


    SaveGrayScaleImageAsDataFile(OutputFilePathAndName, OutputImage);

    std::system("pause");
}


void test_IntegralImageBuider()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto InputImage = LoadGrayScaleImageFromDICOMFile<double>(FilePath);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0_OutputImage");

    auto InputDimension = InputImage.GetDimension();

    Image<double> OutputImage;

    OutputImage.ReInitialize(InputDimension.Lx, InputDimension.Ly, InputDimension.Lz);

    OutputImage.Fill(0);

    IntegralImageBuilder<double, double>  imbuilder;

    imbuilder.SetInputImage(&InputImage);

    imbuilder.SetOutputImage(&OutputImage);

    imbuilder.SetMaxThreadNumber(4);

    std::cout << "start: " << '\n';

    std::time_t t0 = std::time(0);

    imbuilder.Update();

    std::time_t t1 = std::time(0);

    std::cout << "time " << t1 - t0 << '\n';

    SaveGrayScaleImageAsDataFile(OutputFilePathAndName, OutputImage);

    std::system("pause");
}

}//namespace mdk

#endif