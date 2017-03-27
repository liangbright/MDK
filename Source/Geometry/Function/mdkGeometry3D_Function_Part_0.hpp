#pragma once

namespace mdk
{
template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseMatrix<ScalarType>& Point, const DenseMatrix<ScalarType>& PlaneOrigin, const DenseMatrix<ScalarType>& PlaneNormal)
{
	if (Point.IsVector() == false || PlaneOrigin.IsVector() == false || PlaneNormal.IsVector() == false)
	{
		MDK_Error("Input is not vector @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	if (Point.GetElementNumber() != 3 || PlaneOrigin.GetElementNumber() != 3 || PlaneNormal.GetElementNumber() != 3)
	{
		MDK_Error("Input Vector length is not 3 @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}

	return ComputeSignedDistanceBetweenPointAndPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}


template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseVector<ScalarType, 3>& Point, const DenseVector<ScalarType, 3>& PlaneOrigin, const DenseVector<ScalarType, 3>& PlaneNormal)
{
	return ComputeSignedDistanceBetweenPointAndPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal)
{ // PlaneNormal must have unit length

	if (Point == nullptr || PlaneOrigin == nullptr || PlaneNormal == nullptr)
	{
		MDK_Error("Input is nullptr @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	 
	DenseVector<ScalarType, 3> Displacement;
	Displacement[0] = Point[0] - PlaneOrigin[0];
	Displacement[1] = Point[1] - PlaneOrigin[1];
	Displacement[2] = Point[2] - PlaneOrigin[2];

	// dot product between Displacement and PlaneNormal
	auto DotProduct = Displacement[0] * PlaneNormal[0] + Displacement[1] * PlaneNormal[1] + Displacement[2] * PlaneNormal[2];	
	return DotProduct;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, const DenseMatrix<ScalarType>& PlaneOrigin, const DenseMatrix<ScalarType>& PlaneNormal)
{
	if (Point.IsVector() == false || PlaneOrigin.IsVector() == false || PlaneNormal.IsVector() == false)
	{
		MDK_Error("Input is not vector @ mdkGeometry3D ProjectPointToPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	if (Point.GetElementNumber() != 3 || PlaneOrigin.GetElementNumber() != 3 || PlaneNormal.GetElementNumber() != 3)
	{
		MDK_Error("Input Vector length is not 3 @ mdkGeometry3D ProjectPointToPlane(...)")
		return GetNaNElement<ScalarType>();
	}

	return ProjectPointToPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, const DenseVector<ScalarType, 3>& PlaneOrigin, const DenseVector<ScalarType, 3>& PlaneNormal)
{
	return ProjectPointToPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal)
{
	DenseVector<ScalarType, 3> ProjectedPoint;

	if (Point == nullptr || PlaneOrigin == nullptr || PlaneNormal == nullptr)
	{
		MDK_Error("Input is nullptr @ mdkGeometry3D ProjectPointToPlane(...)")
		ProjectedPoint.Fill(GetNaNElement<ScalarType>());
		return ProjectedPoint;
	}

	DenseVector<ScalarType, 3> Displacement;
	Displacement[0] = Point[0] - PlaneOrigin[0];
	Displacement[1] = Point[1] - PlaneOrigin[1];
	Displacement[2] = Point[2] - PlaneOrigin[2];

	// dot product between Displacement and PlaneNormal
	auto DotProduct = Displacement[0] * PlaneNormal[0] + Displacement[1] * PlaneNormal[1] + Displacement[2] * PlaneNormal[2];

	ProjectedPoint[0] = Point[0] - DotProduct*PlaneNormal[0];
	ProjectedPoint[1] = Point[1] - DotProduct*PlaneNormal[1];
	ProjectedPoint[2] = Point[2] - DotProduct*PlaneNormal[2];

	return ProjectedPoint;
}

template<typename ScalarType>
int_max FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType, 3>& Point_ref)
{
	if (Curve.IsEmpty() == true)
	{
		MDK_Warning("Curve is empty @ FindNearestPointOnCurve(...)")
		return -1;
	}
	else if (Curve.GetColCount() == 1)
	{
		return 0;
	}

	DenseVector<ScalarType, 3> Point0;
	Curve.GetCol(0, Point0);
	ScalarType Distance_min = (Point_ref - Point0).L2Norm();
	int_max Index_min = 0;
	for (int_max k = 1; k < Curve.GetColCount(); ++k)
	{
		DenseVector<ScalarType, 3> Point_k;
		Curve.GetCol(k, Point_k);
		auto Distance = (Point_ref - Point_k).L2Norm();
		if (Distance < Distance_min)
		{
			Distance_min = Distance;
			Index_min = k;
		}
	}
	return Index_min;
}


template<typename ScalarType>
ScalarType Compute3DCurveLength(const DenseMatrix<ScalarType>& Curve)
{
	if (Curve.GetColCount() < 2)
	{
		return 0;
	}

	if (Curve.GetRowCount() != 3)
	{
		MDK_Error("Not 3D Curve @ Compute3DCurveLength(...)")
		return 0;
	}

	ScalarType CurveLength = 0;

	for (int_max k = 1; k < Curve.GetColCount(); ++k)
	{
		ScalarType Pos_a[3], Pos_b[3];
		Curve.GetCol(k, Pos_a);
		Curve.GetCol(k - 1, Pos_b);
		auto Distance = (Pos_a[0] - Pos_b[0])*(Pos_a[0] - Pos_b[0])
			          + (Pos_a[1] - Pos_b[1])*(Pos_a[1] - Pos_b[1])
			          + (Pos_a[2] - Pos_b[2])*(Pos_a[2] - Pos_b[2]);
		Distance = std::sqrt(Distance);
		CurveLength += Distance;
	}

	return CurveLength;
}


template<typename ScalarType>
DenseVector<ScalarType> ComputeCumulative3DCurveLengthList(const DenseMatrix<ScalarType>& Curve)
{//LengthList ColCount is the same as Curve ColCount
	DenseVector<ScalarType> LengthList;
	LengthList.Resize(Curve.GetColCount());
	LengthList.Fill(0);

	if (Curve.GetColCount() < 2)
	{
		MDK_Error(" ColCount < 2 @ ComputeCumulative3DCurveLengthList(...)")
			return LengthList;
	}

	for (int_max k = 1; k < Curve.GetColCount(); ++k)
	{
		ScalarType Pos_a[3], Pos_b[3];
		Curve.GetCol(k, Pos_a);
		Curve.GetCol(k - 1, Pos_b);
		auto Distance = (Pos_a[0] - Pos_b[0])*(Pos_a[0] - Pos_b[0])
			          + (Pos_a[1] - Pos_b[1])*(Pos_a[1] - Pos_b[1])
			          + (Pos_a[2] - Pos_b[2])*(Pos_a[2] - Pos_b[2]);
		Distance = std::sqrt(Distance);
		LengthList[k] = Distance + LengthList[k - 1];
	}

	return LengthList;
}



template<typename ScalarType>
DenseMatrix<ScalarType> Subdivide3DCurve_Linear(const DenseMatrix<ScalarType>& InputCurve, int_max SubdivisionCount)
{
	DenseMatrix<ScalarType> OutputCurve;

	if (InputCurve.IsEmpty() == true)
	{
		return OutputCurve;
	}

	OutputCurve = InputCurve;

	DenseMatrix<ScalarType> tempCurve;

	for (int_max SubdivisionIndex = 0; SubdivisionIndex < SubdivisionCount; ++SubdivisionIndex)
	{
		auto ColCount_new = 2 * OutputCurve.GetColCount() - 1;
		tempCurve.FastResize(3, ColCount_new);

		tempCurve.SetCol(0, OutputCurve.GetPointerOfCol(0));

		for (int_max k = 1; k < OutputCurve.GetColCount(); ++k)
		{
			auto Pos_prev = OutputCurve.GetPointerOfCol(k - 1);
			auto Pos_k = OutputCurve.GetPointerOfCol(k);

			ScalarType Pos[3];
			Pos[0] = (Pos_prev[0] + Pos_k[0]) / ScalarType(2);
			Pos[1] = (Pos_prev[1] + Pos_k[1]) / ScalarType(2);
			Pos[2] = (Pos_prev[2] + Pos_k[2]) / ScalarType(2);

			tempCurve.SetCol(2 * k - 1, Pos);
			tempCurve.SetCol(2 * k, Pos_k);
		}
		OutputCurve = std::move(tempCurve);
	}

	return OutputCurve;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ResampleOpen3DCurveByCardinalSpline(const DenseMatrix<ScalarType>& Curve, int_max PointCount)
{
	// output: SplineCurve(:, 0) is Curve(:, 0); SplineCurve(:, end) is Curve(:, end)

	auto PointCount_input = Curve.GetColCount();
	if (Curve.GetColCount() < 2 || PointCount < 2)
	{
		DenseMatrix<ScalarType> EmptyCurve;
		return EmptyCurve;
	}

	auto points = vtkSmartPointer<vtkPoints>::New();
	points->SetNumberOfPoints(PointCount_input);
	for (int_max k = 0; k < PointCount_input; ++k)
	{
		auto x = Curve(0, k);
		auto y = Curve(1, k);
		auto z = Curve(2, k);
		points->SetPoint(k, x, y, z);
	}
	auto lines =vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(PointCount_input);
	for (int_max k = 0; k < PointCount_input; ++k)
	{
		lines->InsertCellPoint(k);
	}
	auto spline_data = vtkSmartPointer<vtkPolyData>::New();
	spline_data->SetPoints(points);
	spline_data->SetLines(lines);

	auto spline = vtkSmartPointer<vtkCardinalSpline>::New();
	spline->SetLeftConstraint(2);
	spline->SetLeftValue(0.0);
	spline->SetRightConstraint(2);
	spline->SetRightValue(0.0);

	auto splineFilter =	vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInputData(spline_data);
	splineFilter->SetNumberOfSubdivisions(PointCount-1);
	splineFilter->SetSpline(spline);
	splineFilter->Update();
	auto SplineCurve_VTK = splineFilter->GetOutput();
	auto PointCount_out = SplineCurve_VTK->GetNumberOfPoints();
	if (PointCount_out != PointCount)
	{
		MDK_Error("PointCount_out (" << PointCount_out << ") != PointCount (" << PointCount << ") @ FitCardinalSplineToOpen3DCurve(...)")
		DenseMatrix<ScalarType> EmptyCurve;
		return EmptyCurve;
	}

	DenseMatrix<ScalarType> SplineCurve;
	SplineCurve.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		double pos[3];
		SplineCurve_VTK->GetPoint(k, pos);
		SplineCurve.SetCol(k, pos);
	}
	return SplineCurve;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ResampleClosed3DCurveByCardinalSpline(const DenseMatrix<ScalarType>& Curve, int_max PointCount)
{// Curve(:,1)=[x,y,z]	
	auto PointCount_input = Curve.GetColCount();
	if (PointCount_input < 2 || PointCount < 2)
	{
		DenseMatrix<ScalarType> EmptyCurve;
		return EmptyCurve;
	}

	auto points = vtkSmartPointer<vtkPoints>::New();
	auto lines = vtkSmartPointer<vtkCellArray>::New();	

	//check if Curve(:,0) = Curve(:,end)
	bool Flag_Start_End_Same = false;
	{
		auto x1 = Curve(0, 0);
		auto y1 = Curve(1, 0);
		auto z1 = Curve(2, 0);
		auto x2 = Curve(0, PointCount_input - 1);
		auto y2 = Curve(1, PointCount_input - 1);
		auto z2 = Curve(2, PointCount_input - 1);
		auto dist = std::abs(x1 - x2) + std::abs(y1 - y2) + std::abs(z1 - z2);		
		auto EPS = std::numeric_limits<ScalarType>::epsilon();
		if (dist <= 3*EPS)
		{
			Flag_Start_End_Same = true;
		}
	}

	if (Flag_Start_End_Same == true)
	{
		points->SetNumberOfPoints(PointCount_input);
		for (int_max k = 0; k < PointCount_input; ++k)
		{
			auto x = Curve(0, k);
			auto y = Curve(1, k);
			auto z = Curve(2, k);
			points->SetPoint(k, x, y, z);
		}
		lines->InsertNextCell(PointCount_input);
		for (int_max k = 0; k < PointCount_input; ++k)
		{
			lines->InsertCellPoint(k);
		}
	}
	else 
	{
		points->SetNumberOfPoints(PointCount_input+1);
		for (int_max k = 0; k < PointCount_input; ++k)
		{
			auto x = Curve(0, k);
			auto y = Curve(1, k);
			auto z = Curve(2, k);
			points->SetPoint(k, x, y, z);
		}		
		{//add the first point to the end
			auto x = Curve(0, 0);
			auto y = Curve(1, 0);
			auto z = Curve(2, 0);
			points->SetPoint(PointCount_input, x, y, z);
		}
		lines->InsertNextCell(PointCount_input+1);
		for (int_max k = 0; k < PointCount_input+1; ++k)
		{
			lines->InsertCellPoint(k);
		}
	}

	auto spline_data = vtkSmartPointer<vtkPolyData>::New();
	spline_data->SetPoints(points);
	spline_data->SetLines(lines);

	auto spline = vtkSmartPointer<vtkCardinalSpline>::New();
	spline->ClosedOn();

	auto splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	splineFilter->SetInputData(spline_data);
	splineFilter->SetNumberOfSubdivisions(PointCount);
	splineFilter->SetSpline(spline);
	splineFilter->Update();
	auto SplineCurve_VTK = splineFilter->GetOutput();
	auto PointCount_out = SplineCurve_VTK->GetNumberOfPoints();
	if (PointCount_out != PointCount+1)
	{
		MDK_Error("PointCount_out (" << PointCount_out << ") != PointCount (" << PointCount << ") + 1 @ FitCardinalSplineToClosed3DCurve(...)")
		DenseMatrix<ScalarType> EmptyCurve;
		return EmptyCurve;
	}

	DenseMatrix<ScalarType> SplineCurve;
	SplineCurve.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		double pos[3];
		SplineCurve_VTK->GetPoint(k, pos);
		SplineCurve.SetCol(k, pos);
	}
	return SplineCurve;
}


}// namespace mdk
