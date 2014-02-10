#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"
#include "mdk3DImageConvolutionFilter.h"

using namespace mdk;

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


inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, const mdk3DImage<double>& InputImage, double& Output)
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

		//Output += i * InputImage(uint64(x), uint64(y), uint64(z));
	}

	Output = Value;
}


void Test_FilterFunction()
{
	uint64 Lx = 512;
	uint64 Ly = 512;
	uint64 Lz = 512;

	mdk3DImage<double> InputImage;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	std::cout << "FilterFunction function " << '\n';

	auto t0 = std::time(0);

	double Output = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
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

void Test_FunctionTemplate()
{
	mdkMatrix<double> M(1,3);
	M = { 1, 2, 3 };

	M.ElementOperation("sqrt");

	std::system("pause");
}

void Test_FunctionTemplate_InputFilterFunction()
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

	imfilter.SetInputFilterFunction(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter.Run() time " << t1 - t0 << '\n';

	t0 = std::time(0);

	imfilter.Run(FilterFunction);

	t1 = std::time(0);

	std::cout << "imfilter.Run(FilterFunction) time " << t1 - t0 << '\n';

	std::cout << "FilterFunction function " << '\n';

	t0 = std::time(0);

	double Output = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
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

	imfilter.SetInputFilterFunction(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';


	std::cout << "FilterFunction function " << '\n';
	
	t0 = std::time(0);

	double Output = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	/*
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

	*/

	std::system("pause");
 }


void Test_ConvolutionFilter_VirtualFilterFunction()
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

	imfilter.SetInputFilterFunction(FilterFunction);

	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter time " << t1 - t0 << '\n';


	mdk3DImageConvolutionFilter<double, double>  imconvfilter;

	imconvfilter.SetInputImage(&InputImage);

	imconvfilter.SetOutputImage(&OutputImage);

	imconvfilter.SetInputFilterFunction(FilterFunction);

	imconvfilter.SetMaxThreadNumber(1);

	imconvfilter.EnableBoundCheck(false);

	mdkMatrix<double> Mask(4, 1000 * 36 * 36 / 2);

	Mask.Fill(0);

	imconvfilter.SetMask(Mask);

	t0 = std::time(0);

	imfilter.Run();

	t1 = std::time(0);

	std::cout << "mdk3DImageConvolutionFilter time " << t1 - t0 << '\n';

	std::cout << "FilterFunction " << '\n';

	t0 = std::time(0);

	double Output = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
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
	uint64 Lx = 100;
	uint64 Ly = 100;
	uint64 Lz = 100;

	mdk3DImage<double> InputImage;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1);

	mdk3DImage<double> OutputImage;

	OutputImage.Initialize(Lx, Ly, Lz);

	OutputImage.Fill(0);

	mdk3DImageConvolutionFilter<double, double>  imfilter;

	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);

	imfilter.EnableBoundCheck(false);

	mdkMatrix<double> Mask(4, 1000*36*36/2);

	Mask.Fill(0);

	imfilter.SetMask(Mask);

	std::time_t t0 = std::time(0);

	imfilter.Run();
	
	std::time_t t1 = std::time(0);

	std::cout << "time " << t1 - t0 << '\n';

	std::cout << "FilterFunction " << '\n';

	t0 = std::time(0);

	double Output = 0;

	for (uint64 i = 0; i < Lx*Ly*Lz; ++i)
	{
		FilterFunction(0, 0, 0, InputImage, Output);
	}

	std::cout << "Output " << Output << '\n';

	t1 = std::time(0);

	std::cout << "FilterFunction time " << t1 - t0 << '\n';

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

	for (uint64 i = 0; i < v.size(); ++i)
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

	uint64 OuputVoxelDimension = 2;

	mdk3DImage<double> InputImage;

	uint64 Lx = 512;
	uint64 Ly = 512;
	uint64 Lz = 512;

	InputImage.Initialize(Lx, Ly, Lz);

	InputImage.Fill(1.0);

	mdk3DImage<std::array<double, 2>> OutputImage;

	OutputImage.Initialize(Lx, Ly, Lz);

	std::array<double, 2> OutputZeroVoxel = { 0, 0 };

	std::time_t t01 = std::time(0);

	OutputImage.Fill(OutputZeroVoxel);

	std::time_t t02 = std::time(0);

	std::cout << "OutputImage.Fill time " << t02 - t01 << '\n';

	auto ArrayPtr = OutputImage.GetVoxelDataArrayPointer();

	std::cout << "(*ArrayPtr).capacity()" << (*ArrayPtr).capacity() << '\n';

	mdk3DImageConvolutionFilter<double, std::array<double, 2>, 2>  imfilter;
	
	imfilter.SetInputImage(&InputImage);

	imfilter.SetOutputImage(&OutputImage);

	imfilter.SetMaxThreadNumber(1);
	
	std::vector<mdkMatrix<double>> MaskList;
	MaskList.resize(OuputVoxelDimension);

	for (uint64 i = 0; i < OuputVoxelDimension; ++i)
	{
		MaskList[i].SetSize(4, 1000);
		MaskList[i].Fill(0);
	}

	imfilter.SetMask(MaskList);
	
	std::time_t t0 = std::time(0);

	imfilter.Run();

	std::time_t t1 = std::time(0);

	std::cout << "imfilter time " << t1 - t0 << '\n';

	std::system("pause");
}


void test_Valve_Filter()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto InputImage = ReadGrayScale3DImageFromDICOMFile(FilePath);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0_OutputImage");

    auto InputSize = InputImage.GetImageSize();


    mdk3DImage<double> OutputImage;

    OutputImage.Initialize(InputSize.Lx, InputSize.Ly, InputSize.Lz);

    OutputImage.Fill(0);

    mdk3DImageConvolutionFilter<double, double>  imfilter;

    imfilter.SetInputImage(&InputImage);

    imfilter.SetOutputImage(&OutputImage);

    imfilter.SetMaxThreadNumber(4);

    imfilter.EnableBoundCheck(true);

    mdkMatrix<double> Mask(4, 7);

    Mask = { -1,  0,   0,    0,   1,   1,   1,
             -1, -1,   0,    0,   0,   1,   1,
             -1, -1,   -1,   0,   0,   0,   1,
             0.1, 0.1, 0.1, -0.6, 0.1, 0.1, 0.1 };

    imfilter.SetMask(Mask);

    std::cout << "start: " << '\n';

    std::time_t t0 = std::time(0);

    imfilter.Run();

    std::time_t t1 = std::time(0);

    std::cout << "time " << t1 - t0 << '\n';


    SaveGrayScale3DImageAsBinaryFile(OutputFilePathAndName, OutputImage);

    std::system("pause");
}

#endif