#ifndef __Test_ThinPlateSplineTransform_h
#define __Test_ThinPlateSplineTransform_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include "mdkThinPlateSplineTransform2D.h"
#include "mdkThinPlateSplineTransform3D.h"
#include "mdkDenseMatrix_FileIO.h"


void Test_2D()
{
	using namespace mdk;

	ThinPlateSplineTransform2D<double> TPSTransform;

	String File_Source = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/Source2D.json";
	String File_Target = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/Target2D.json";

	DenseMatrix<double> SourceControlPointSet, TargetControlPointSet;
	LoadDenseMatrixFromJsonDataFile(SourceControlPointSet, File_Source);
	LoadDenseMatrixFromJsonDataFile(TargetControlPointSet, File_Target);

	TPSTransform.SetSourceLandmarkPointSet(&SourceControlPointSet);
	TPSTransform.SetTargetLandmarkPointSet(&TargetControlPointSet);
	//TPSTransform.UseTPS3D();
	TPSTransform.UseStandardTPS2D();
	TPSTransform.EstimateParameter();

	DenseMatrix<double> TargetPointSet;
	TargetPointSet.Resize(TargetControlPointSet.GetSize());
	for (int_max k = 0; k < TargetControlPointSet.GetColCount(); ++k)
	{
		auto Point_k = TPSTransform.TransformPoint(SourceControlPointSet(0, k), SourceControlPointSet(1, k));
		TargetPointSet(0, k) = Point_k[0];
		TargetPointSet(1, k) = Point_k[1];
	}

	auto Parameter = TPSTransform.GetParameter();

	DisplayMatrix("Parameter", Parameter, 6);
	DisplayMatrix("TargetControlPointSet", TargetControlPointSet, 6);
	DisplayMatrix("TargetPointSet", TargetPointSet, 6);
}


void Test_3D()
{
	using namespace mdk;

	ThinPlateSplineTransform3D<double> TPSTransform;

	String File_Source = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/Source.json";
	String File_Target = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/Target.json";

	DenseMatrix<double> SourceControlPointSet, TargetControlPointSet;
	LoadDenseMatrixFromJsonDataFile(SourceControlPointSet, File_Source);
	LoadDenseMatrixFromJsonDataFile(TargetControlPointSet, File_Target);

	TPSTransform.SetSourceLandmarkPointSet(&SourceControlPointSet);
	TPSTransform.SetTargetLandmarkPointSet(&TargetControlPointSet);
	TPSTransform.EstimateParameter();

	DenseMatrix<double> TargetPointSet;
	TargetPointSet.Resize(TargetControlPointSet.GetSize());
	for (int_max k = 0; k < TargetControlPointSet.GetColCount(); ++k)
	{
		auto Point_k = TPSTransform.TransformPoint(SourceControlPointSet(0, k), SourceControlPointSet(1, k), SourceControlPointSet(2, k));
		TargetPointSet(0, k) = Point_k[0];
		TargetPointSet(1, k) = Point_k[1];
		TargetPointSet(2, k) = Point_k[2];
	}

	auto Parameter = TPSTransform.GetParameter();

	DisplayMatrix("Parameter", Parameter, 4);
	DisplayMatrix("TargetControlPointSet", TargetControlPointSet, 4);
	DisplayMatrix("TargetPointSet", TargetPointSet, 4);
}


#endif