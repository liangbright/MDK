#ifndef Test_RotationTransform_h
#define Test_RotationTransform_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include "mdkRotationTransform2D.h"
#include "mdkRotationTransform3D.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_2D()
{
	using namespace mdk;

	RotationTransform2D<double> Transform;

	String File_Source = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/Source2D.json";
	String File_Target = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/Target2D.json";

	DenseMatrix<double> SourceControlPointSet, TargetControlPointSet;
	LoadDenseMatrixFromJsonDataFile(SourceControlPointSet, File_Source);
	LoadDenseMatrixFromJsonDataFile(TargetControlPointSet, File_Target);

	Transform.SetSourceLandmarkPointSet(&SourceControlPointSet);
	Transform.SetTargetLandmarkPointSet(&TargetControlPointSet);
	Transform.EstimateParameter();

	auto RotationMatrix = Transform.GetRotationMatrix();

	auto RotationCenter = Transform.GetRotationCenter();

	DenseMatrix<double> TargetPointSet;
	TargetPointSet.Resize(SourceControlPointSet.GetSize());
	for (int_max k = 0; k < SourceControlPointSet.GetColCount(); ++k)
	{
		DenseVector<double, 2> Pos;
		SourceControlPointSet.GetCol(k, Pos);
		auto Pos_new = Transform.TransformPoint(Pos);
		TargetPointSet.SetCol(k, Pos_new);
	}

	DisplayMatrix("RotationMatrix", RotationMatrix, 3);
	DisplayMatrix("SourceControlPointSet", SourceControlPointSet, 3);
	DisplayMatrix("TargetControlPointSet", TargetControlPointSet, 3);
	DisplayMatrix("TargetPointSet", TargetPointSet, 3);
}


void Test_3D()
{
	using namespace mdk;

	RotationTransform3D<double> Transform;

	String File_Source = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/Source3D.json";
	String File_Target = "C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/Target3D.json";

	DenseMatrix<double> SourceControlPointSet, TargetControlPointSet;
	LoadDenseMatrixFromJsonDataFile(SourceControlPointSet, File_Source);
	LoadDenseMatrixFromJsonDataFile(TargetControlPointSet, File_Target);

	Transform.SetSourceLandmarkPointSet(&SourceControlPointSet);
	Transform.SetTargetLandmarkPointSet(&TargetControlPointSet);
	Transform.EstimateParameter();

	auto RotationMatrix = Transform.GetRotationMatrix();

	auto RotationCenter = Transform.GetRotationCenter();

	DenseMatrix<double> TargetPointSet;
	TargetPointSet.Resize(SourceControlPointSet.GetSize());
	for (int_max k = 0; k < SourceControlPointSet.GetColCount(); ++k)
	{
		DenseVector<double, 3> Pos;
		SourceControlPointSet.GetCol(k, Pos);
		auto Pos_new = Transform.TransformPoint(Pos);
		TargetPointSet.SetCol(k, Pos_new);
	}

	DisplayMatrix("RotationMatrix", RotationMatrix, 3);
	DisplayMatrix("SourceControlPointSet", SourceControlPointSet, 3);
	DisplayMatrix("TargetControlPointSet", TargetControlPointSet, 3);
	DisplayMatrix("TargetPointSet", TargetPointSet, 3);
}


#endif