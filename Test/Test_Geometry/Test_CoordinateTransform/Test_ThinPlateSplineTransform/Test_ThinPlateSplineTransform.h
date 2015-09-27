#ifndef __Test_ThinPlateSplineTransform_h
#define __Test_ThinPlateSplineTransform_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include "mdkThinPlateSplineTransform3D.h"


void Test_simple()
{
	using namespace mdk;

	ThinPlateSplineTransform3D<double> TPSTransform;

	DenseMatrix<double> SourcePointSet(3, 10);
	DenseMatrix<double> TargetPointSet(3, 10);

	for (int_max i = 0; i < 10; ++i)
	{
		int_max j = i + 1;

		SourcePointSet(0, i) = double(j);
		SourcePointSet(1, i) = double(j*j + 1);
		SourcePointSet(2, i) = double(j + 2);

		TargetPointSet(0, i) = double(j + 2);
		TargetPointSet(1, i) = double(j*j + 1);
		TargetPointSet(2, i) = double(j);
	}

	TPSTransform.SetSourceLandmarkPointSet(&SourcePointSet);
	TPSTransform.SetTargetLandmarkPointSet(&TargetPointSet);
	TPSTransform.EstimateParameter();

	DenseMatrix<double> TestPointSet(3, 10);

	for (int_max k = 0; k < 10; ++k)
	{
		auto Point_k = TPSTransform.TransformPoint(SourcePointSet(0, k), SourcePointSet(1, k), SourcePointSet(2,k));
		TestPointSet(0, k) = Point_k[0];
		TestPointSet(1, k) = Point_k[1];
		TestPointSet(2, k) = Point_k[2];
	}

	DisplayMatrix("SourcePointSet", SourcePointSet, 4);
	DisplayMatrix("TargetPointSet", TargetPointSet, 4);
	DisplayMatrix("TestPointSet", TestPointSet, 4);
}


#endif