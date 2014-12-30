#ifndef __TestImage_h
#define __TestImage_h

#include <ctime>
#include <cstdlib>
#include <array>
#include <chrono>
#include <ctime>

#include "mdkDenseImage3D.h"
#include "mdkDenseImage3D_FileIO.h"
#include "mdkCurve_FileIO.h"

namespace mdk
{

void testCurve()
{
	std::string FileNameA = "C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveA.vtk";
	std::string FileNameB = "C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveB.vtk";
	std::string FileNameAB = "C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveAandB.vtk";

	DenseMatrix<double> CurveA(3,3);
	CurveA = { { 1, 2, 3 },
			 { 4, 5, 6 },
			 { 7, 8, 9 } };
	SaveSingle3DCurveAsVTKFile(CurveA, FileNameA);

	DenseMatrix<double> CurveB;
	CurveB = {  { 10, 20, 30, 40 },
				{ 4,  5,  6,  7 },
	            { 7,  8,  9,  10 } };

	SaveSingle3DCurveAsVTKFile(CurveB, FileNameB);

	ObjectArray<DenseMatrix<double>> CurveList;
	CurveList.Resize(2);
	CurveList[0] = CurveA;
	CurveList[1] = CurveB;
	SaveMultiple3DCurveAsVTKFile(CurveList, FileNameAB);

	DenseMatrix<double> InputCurveA;
	LoadSingle3DCurveFromVTKFile(InputCurveA, FileNameA);
	DisplayMatrix("InputCurveA", InputCurveA);

	DenseMatrix<double> InputCurveB;
	LoadSingle3DCurveFromVTKFile(InputCurveB, FileNameB);
	DisplayMatrix("InputCurveB", InputCurveB);

	ObjectArray<DenseMatrix<double>> InputCurveAB;
	LoadMultiple3DCurveFromVTKFile(InputCurveAB, FileNameAB);
	DisplayMatrix("InputCurveAB[0]", InputCurveAB[0]);
	DisplayMatrix("InputCurveAB[1]", InputCurveAB[1]);

}

void testA()
{
	DenseImage3D<double> ScalarImage_double;
	ScalarImage_double.SetSize(100, 100, 100);
	ScalarImage_double.SetOrigin(0, 0, 0);
	ScalarImage_double.SetSpacing(1, 1, 1);

	DenseImage3D<double>::InterpolationOptionType Option_double;
	Option_double.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_double.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_double.Pixel_OutsideImage = 0;
	auto a1 = ScalarImage_double.GetPixelAt3DIndex(1.0, 1.5, 1.0, Option_double);
	auto a2 = ScalarImage_double.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0, Option_double);
	auto a3 = ScalarImage_double(1, 1, 1);

	DenseImage3D<float> ScalarImage_float;
	ScalarImage_float.SetSize(100, 100, 100);
	ScalarImage_float.SetOrigin(0, 0, 0);
	ScalarImage_float.SetSpacing(1, 1, 1);

	DenseImage3D<float>::InterpolationOptionType Option_float;
	Option_float.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_float.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_float.Pixel_OutsideImage = 0;
	auto b1 = ScalarImage_float.GetPixelAt3DIndex(1.0, 1.5, 1.0, Option_float);
	auto b2 = ScalarImage_float.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0, Option_float);
	auto b3 = ScalarImage_float(1, 1, 1);
	
	// do not use int, precision loss
	DenseImage3D<int> ScalarImage_int;
	ScalarImage_int.SetSize(100, 100, 100);
	ScalarImage_int.SetOrigin(0, 0, 0);
	ScalarImage_int.SetSpacing(1, 1, 1);

	DenseImage3D<int>::InterpolationOptionType Option_int;
	Option_int.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_int.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_int.Pixel_OutsideImage = 0;
	auto aa1 = ScalarImage_int.GetPixelAt3DIndex(1.0, 1.5, 1.0, Option_int);
	auto aa2 = ScalarImage_int.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0, Option_int);
	auto aa3 = ScalarImage_int(1, 1, 1);

	DenseImage3D<DenseVector<double, 3>> VectorImage_double;
	VectorImage_double.SetSize(10, 10, 10);
	VectorImage_double.SetOrigin(0, 0, 0);
	VectorImage_double.SetSpacing(1, 1, 1);

	DenseImage3D<DenseVector<double, 3>>::InterpolationOptionType Option_vd;
	Option_vd.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_vd.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_vd.Pixel_OutsideImage = 0;
	auto c1 = VectorImage_double.GetPixelAt3DIndex(1.0, 1.5, 1.0, Option_vd);
	auto c2 = VectorImage_double.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0, Option_vd);
	auto c3 = VectorImage_double(1, 1, 1);

	DenseImage3D<DenseVector<float, 3>> VectorImage_float;
	VectorImage_float.SetSize(10, 10, 10);
	VectorImage_float.SetOrigin(0, 0, 0);
	VectorImage_float.SetSpacing(1, 1, 1);

	DenseImage3D<DenseVector<float, 3>>::InterpolationOptionType Option_vf;
	Option_vf.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_vf.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_vf.Pixel_OutsideImage = 0;
	auto d1 = VectorImage_float.GetPixelAt3DIndex(float(1.0), float(1.5), float(1.0), Option_vf);
	auto d2 = VectorImage_float.GetPixelAt3DPhysicalPosition(float(1.0), float(1.5), float(1.0), Option_vf);
	auto d3 = VectorImage_float(1, 1, 1);
	// this is incorrect, must use float()
	//auto d = VectorImage_float.GetPixelAt3DIndex(1.0, 1.5, 1.0);

	// do not use int, precision loss
	DenseImage3D<DenseVector<int, 3>> VectorImage_int;
	VectorImage_int.SetSize(10, 10, 10);
	VectorImage_int.SetOrigin(0, 0, 0);
	VectorImage_int.SetSpacing(1, 1, 1);

	DenseImage3D<DenseVector<int, 3>>::InterpolationOptionType Option_vi;
	Option_vi.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option_vi.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Replicate;
	Option_vi.Pixel_OutsideImage = 0;
	auto e1 = VectorImage_int.GetPixelAt3DIndex(int(1.0), int(1.5), int(1.0), Option_vi);
}

void testB()
{
	DenseImage3D<double> ScalarImage_double;
	ScalarImage_double.SetSize(100, 100, 100);
	ScalarImage_double.SetSpacing(1, 1, 1);

	auto Index3D_a = ScalarImage_double.TransformLinearIndexTo3DIndex(10);
	auto a1 = ScalarImage_double.GetPixelAt3DIndex(0.1, 0.1, 0.1);
	DenseImage3D<double>::InterpolationOptionType Option_a;
	auto a2 = ScalarImage_double.GetPixelAt3DIndex<double>(0.1, 0.1, 0.1, Option_a);

	auto b1 = ScalarImage_double.TransformLinearIndexTo3DPhysicalPosition(10);

	auto b2 = ScalarImage_double.TransformLinearIndexTo3DPhysicalPosition<float>(10);

	auto c1 = ScalarImage_double.Transform3DIndexTo3DPhysicalPosition(10, 10, 10);
	auto c2 = ScalarImage_double.Transform3DIndexTo3DPhysicalPosition(10.0, 10.0, 10.0);

}

void Test_Image3D()
{// use Image3D and virtual function

	DenseImage3D<int> ScalarImage;
	ScalarImage.SetSize(1000, 1000, 1000);
	ScalarImage.SetSpacing(1, 1, 1);

	//Image3D<int>* Ptr = &ScalarImage;

	auto t0 = std::chrono::system_clock::now();
	for (int n = 1; n < 10; ++n)
	{
		for (int_max k = 0; k < ScalarImage.GetPixelNumber(); ++k)
		{
			ScalarImage.SetPixelAtLinearIndex(k, 1);
			//Ptr->SetPixelAtLinearIndex(k, 1);
		}
	}
	auto t1 = std::chrono::system_clock::now();

	std::chrono::duration<double> raw_time = t1 - t0;
	std::cout << " time " << raw_time.count() << '\n';

	// result
	// 11.6376s : derive from Image3D and use ScalarImage.SetPixelAtLinearIndex(k, 1);
	// 31.7932s : derive from Image3D and use Ptr->SetPixelAtLinearIndex(k, 1);
	// 10.6966s : not from Image3D

	BoxRegionOf3DPhysicalPositionInImage3D Region;
	ScalarImage.GetSubImage(Region);
}

}


#endif