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

	DenseMatrix<double> SourceControlPointSet(3, 10);
	DenseMatrix<double> TargetControlPointSet(3, 10);

	for (int_max i = 0; i < 10; ++i)
	{
		int_max j = i + 1;

		SourceControlPointSet(0, i) = double(j);
		SourceControlPointSet(1, i) = double(j*j + 1);
		SourceControlPointSet(2, i) = double(j + 2);

		TargetControlPointSet(0, i) = double(j + 2);
		TargetControlPointSet(1, i) = double(j*j + 1);
		TargetControlPointSet(2, i) = double(j);
	}

	TPSTransform.SetSourceControlPointSet(SourceControlPointSet);
	TPSTransform.SetTargetControlPointSet(TargetControlPointSet);
	TPSTransform.UpdateParameter();

	DenseMatrix<double> TestPointSet(3, 10);

	for (int_max k = 0; k < 10; ++k)
	{
		auto Point_k = TPSTransform.TransformPoint(SourceControlPointSet(0, k), SourceControlPointSet(1, k), SourceControlPointSet(2,k));
		TestPointSet(0, k) = Point_k[0];
		TestPointSet(1, k) = Point_k[1];
		TestPointSet(2, k) = Point_k[2];
	}

	DisplayMatrix("SourceControlPointSet", SourceControlPointSet, 4);
	DisplayMatrix("TargetControlPointSet", TargetControlPointSet, 4);
	DisplayMatrix("TestPointSet", TestPointSet, 4);
}


#endif