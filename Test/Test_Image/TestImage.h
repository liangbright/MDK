#ifndef __TestImage_h
#define __TestImager_h

#include <ctime>
#include <cstdlib>
#include <array>

#include "mdkImage3D.h"
#include "mdkFileIO.h"

namespace mdk
{

void testA()
{
	Image3D<double> ScalarImage_double;
	ScalarImage_double.SetSize(100, 100, 100);
	ScalarImage_double.SetOrigin(0, 0, 0);
	ScalarImage_double.SetSpacing(1, 1, 1);

	auto Option = ScalarImage_double.GetInterpolationOption();
	Option.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	Option.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Nearest;

	auto a1 = ScalarImage_double.GetPixelAt3DIndex(1.0, 1.5, 1.0, Option);
	auto a2 = ScalarImage_double.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0, Option);
	auto a3 = ScalarImage_double(1, 1, 1);

	Image3D<float> ScalarImage_float;
	ScalarImage_float.SetSize(100, 100, 100);
	ScalarImage_float.SetOrigin(0, 0, 0);
	ScalarImage_float.SetSpacing(1, 1, 1);
	auto b1 = ScalarImage_float.GetPixelAt3DIndex(1.0, 1.5, 1.0);
	auto b2 = ScalarImage_float.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0);
	auto b3 = ScalarImage_float(1, 1, 1);
	
	// do not use int, precision loss
	Image3D<int> ScalarImage_int;
	ScalarImage_int.SetSize(100, 100, 100);
	ScalarImage_int.SetOrigin(0, 0, 0);
	ScalarImage_int.SetSpacing(1, 1, 1);
	auto aa1 = ScalarImage_int.GetPixelAt3DIndex(1.0, 1.5, 1.0);
	auto aa2 = ScalarImage_int.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0);
	auto aa3 = ScalarImage_int(1, 1, 1);

	Image3D<DenseVector<double, 3>> VectorImage_double;
	VectorImage_double.SetSize(10, 10, 10);
	VectorImage_double.SetOrigin(0, 0, 0);
	VectorImage_double.SetSpacing(1, 1, 1);
	auto c1 = VectorImage_double.GetPixelAt3DIndex(1.0, 1.5, 1.0);
	auto c2 = VectorImage_double.GetPixelAt3DPhysicalPosition(1.0, 1.5, 1.0);
	auto c3 = VectorImage_double(1, 1, 1);

	Image3D<DenseVector<float, 3>> VectorImage_float;
	VectorImage_float.SetSize(10, 10, 10);
	VectorImage_float.SetOrigin(0, 0, 0);
	VectorImage_float.SetSpacing(1, 1, 1);
	auto d1 = VectorImage_float.GetPixelAt3DIndex<float>(1.0, 1.5, 1.0);
	auto d2 = VectorImage_float.GetPixelAt3DPhysicalPosition<float>(1.0, 1.5, 1.0);
	auto d3 = VectorImage_float(1, 1, 1);
	// this is incorrect, must use <float>
	//auto d = VectorImage_float.GetPixelAt3DIndex(1.0, 1.5, 1.0);

	// do not use int, precision loss
	Image3D<DenseVector<int, 3>> VectorImage_int;
	VectorImage_int.SetSize(10, 10, 10);
	VectorImage_int.SetOrigin(0, 0, 0);
	VectorImage_int.SetSpacing(1, 1, 1);
	auto e1 = VectorImage_int.GetPixelAt3DIndex<int>(1.0, 1.5, 1.0);
}

}


#endif