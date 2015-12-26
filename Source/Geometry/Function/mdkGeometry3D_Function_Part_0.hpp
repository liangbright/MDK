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
DenseMatrix<ScalarType> Subdivide3DCurve_Linear(const DenseMatrix<ScalarType>& InputCurve, int_max SubdivisionNumber)
{
	DenseMatrix<ScalarType> OutputCurve;

	if (InputCurve.IsEmpty() == true)
	{
		return OutputCurve;
	}

	OutputCurve = InputCurve;

	DenseMatrix<ScalarType> tempCurve;

	for (int_max SubdivisionIndex = 0; SubdivisionIndex < SubdivisionNumber; ++SubdivisionIndex)
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
DenseMatrix<ScalarType> Resample3DCurveWithBoundedSegmentLength(const DenseMatrix<ScalarType>& InputCurve, ScalarType MaxSegmentLength)
{
	DenseMatrix<ScalarType> OutputCurve;

	if (InputCurve.IsEmpty() == true)
	{
		return OutputCurve;
	}

	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	if (MaxSegmentLength < EPS)
	{
		MDK_Error("MaxSegmentLength is too small < EPS  @ Resample3DCurveWithBoundedSegmentLength(...)")
		return OutputCurve;
	}

	auto InputCurveLength = Compute3DCurveLength(InputCurve);
	auto MeanSegmentLength_input = InputCurveLength / ScalarType(InputCurve.GetColCount() - 1);

	OutputCurve = InputCurve;

	DenseMatrix<ScalarType> tempCurve;
	tempCurve.SetCapacity(3, 2 * (1 + int_max(MaxSegmentLength / MeanSegmentLength_input))*InputCurve.GetColCount());

	while (true)
	{
		bool Flag_AllUnit_WithinLength = true;

		tempCurve.FastResize(0, 0);
		tempCurve.AppendCol(OutputCurve.GetPointerOfCol(0), 3);

		for (int_max k = 1; k < OutputCurve.GetColCount(); ++k)
		{
			auto Pos_prev = OutputCurve.GetPointerOfCol(k - 1);
			auto Pos_k = OutputCurve.GetPointerOfCol(k);
			auto Distance = std::sqrt((Pos_prev[0] - Pos_k[0])*(Pos_prev[0] - Pos_k[0]) + (Pos_prev[1] - Pos_k[1])*(Pos_prev[1] - Pos_k[1]) + (Pos_prev[2] - Pos_k[2])*(Pos_prev[2] - Pos_k[2]));
			if (Distance > MaxSegmentLength)
			{
				Flag_AllUnit_WithinLength = false;

				auto Ratio = Distance / MaxSegmentLength;
				if (Ratio > 1)
				{
					DenseMatrix<ScalarType> Segment(3, 2);
					Segment.SetCol(0, Pos_prev);
					Segment.SetCol(1, Pos_k);
					while (true)
					{						
						Segment = Subdivide3DCurve_Linear(Segment);
						if (Segment.GetColCount() >= 2 * int_max(Ratio + 1))
						{
							break;
						}
					}

					for (int_max n = 1; n < Segment.GetColCount() - 1; ++n)// not include Pos_prev and Pos_k here
					{
						tempCurve.AppendCol(Segment.GetPointerOfCol(n));
					}
				}
				else
				{
					ScalarType Pos[3];
					Pos[0] = (Pos_prev[0] + Pos_k[0]) / ScalarType(2);
					Pos[1] = (Pos_prev[1] + Pos_k[1]) / ScalarType(2);
					Pos[2] = (Pos_prev[2] + Pos_k[2]) / ScalarType(2);
					tempCurve.AppendCol(Pos);
				}
			}
			tempCurve.AppendCol(Pos_k);
		}
		OutputCurve = std::move(tempCurve);

		if (Flag_AllUnit_WithinLength == true)
		{
			break;
		}
	}

	return OutputCurve;
}


template<typename ScalarType>
DenseMatrix<ScalarType> Resample3DCurveWithEqualSegmentLength(const DenseMatrix<ScalarType>& Curve, int_max PointCount_resampled, int_max MaxIterCount, double Tolerance)
{
	DenseMatrix<ScalarType> Curve_resampled;// output

	//------------------------ check input --------------------------------------//

	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	if (Tolerance < EPS)
	{
		MDK_Error("Tolerance is too small (< eps) @ Resample3DCurveWithEqualSegmentLength(...)")
        return Curve_resampled;
	}

	if (MaxIterCount < 1)
	{
		MDK_Error("MaxIterCount < 1 @ Resample3DCurveWithEqualSegmentLength(...)")
		return Curve_resampled;
	}

	auto PointCount_input = Curve.GetColCount();
	auto CurveLength_input = Compute3DCurveLength(Curve);

	if (PointCount_resampled <= 0)
	{
		return Curve_resampled;
	}
	else if (PointCount_resampled == 1)
	{
		MDK_Error("PointCount_resampled is 1 @ Resample3DCurveWithEqualSegmentLength(...)")
		Curve_resampled = Curve.MeanOfEachRow();
		return Curve_resampled;
	}
	else if (PointCount_resampled == 2)
	{
		Curve_resampled = Curve.GetSubMatrix(ALL, { 0, PointCount_input - 1 });
		return Curve_resampled;
	}
	
	if (CurveLength_input < EPS*(PointCount_input - 1))// check if curve shrink to a point
	{
		MDK_Error("curve shrink to a point @ Resample3DCurveWithEqualSegmentLength(...)")
		return Curve_resampled;
	}

	//------------------------------------------------------------------------//
	auto UnitLength_input = CurveLength_input / ScalarType(PointCount_resampled - 1);

	auto Curve_Interpolated = Resample3DCurveWithBoundedSegmentLength(Curve, UnitLength_input*Tolerance*0.5); // InterpolateCurve input curve

	auto PointCount_Interpolated = Curve_Interpolated.GetColCount();

	auto CumulativeCurveLengthList_Interpolated = ComputeCumulative3DCurveLengthList(Curve_Interpolated);

	//----------------------------- Initilization ------------------------------------------------------------//
	Curve_resampled.Resize(3, PointCount_resampled);
	Curve_resampled.SetCol(0, Curve_Interpolated.GetPointerOfCol(0));
	Curve_resampled.SetCol(PointCount_resampled-1, Curve_Interpolated.GetPointerOfCol(PointCount_Interpolated - 1));
	{	
	    int_max PointIndex_n = 1;
		for (int_max n = 1; n < PointCount_resampled-1; ++n)
		{
			//auto Length_n = ScalarType(1-Tolerance)*UnitLength_input*n;
			auto Length_n = UnitLength_input*n;
			for (int_max k = PointIndex_n+1; k < PointCount_Interpolated-1; ++k)
			{
				if (Length_n >= CumulativeCurveLengthList_Interpolated[k - 1] && Length_n <= CumulativeCurveLengthList_Interpolated[k])
				{
					PointIndex_n = k;
					break;
				}
			}
			Curve_resampled.SetCol(n, Curve_Interpolated.GetPointerOfCol(PointIndex_n));
			PointIndex_n += 1;
		}		
	}
	//----------------------------- Iteration to refine --------------------------------------------------------------------//
	// find the optimal UnitLength
	ScalarType UnitLength = Compute3DCurveLength(Curve_resampled) / ScalarType(PointCount_resampled - 1);
	auto Curve_resampled_prev = Curve_resampled;
	for (int_max IterIndex = 1; IterIndex <= MaxIterCount; ++IterIndex)
	{
		int_max PointIndex_n = 1;	
		bool Flag = true;
		for (int_max n = 1; n < PointCount_resampled-1; ++n)
		{
			DenseVector<ScalarType, 3> Pos_n1;
			Curve_resampled.GetCol(n - 1, Pos_n1);
			bool Flag_n = false;
			for (int_max k = PointIndex_n+1; k < PointCount_Interpolated-1; ++k)
			{
				DenseVector<ScalarType, 3> Pos_k;
				Curve_Interpolated.GetCol(k, Pos_k);
				auto Distance_n1_k = (Pos_n1 - Pos_k).L2Norm();
				if (Distance_n1_k >= UnitLength)
				{
					PointIndex_n = k;
					Flag_n = true;
					break;
				}
			}
			if (Flag_n == true)
			{
				Curve_resampled.SetCol(n, Curve_Interpolated.GetPointerOfCol(PointIndex_n));
				PointIndex_n += 1;
			}
			else
			{
				Curve_resampled = Curve_resampled_prev;
				Flag = false;
				break;
			}
		}

		if (Flag == false)
		{// UnitLength is too large, can not construct a full resample curve
			UnitLength *= 1 - Tolerance;
		}
		else
		{//check Curve_resampled to decide wheter to increase or decrease UnitLength: only need to check the distance between last two point -> usually the largest error is in it
			DenseVector<ScalarType, 3> Pos_a, Pos_b;
			Curve_resampled.GetCol(PointCount_resampled - 2, Pos_a);
			Curve_resampled.GetCol(PointCount_resampled - 1, Pos_b);
			auto Distance = (Pos_a - Pos_b).L2Norm();
			auto Ratio = std::abs(Distance - UnitLength) / UnitLength;
			if (Ratio <= Tolerance)
			{
				break;//iteration
			}
			UnitLength = Compute3DCurveLength(Curve_resampled) / ScalarType(PointCount_resampled - 1);

			if (IterIndex == MaxIterCount && Ratio > Tolerance)
			{
				//MDK_Warning(" output is inacurate @ Resample3DCurveWithEqualSegmentLength")
			}

			Curve_resampled_prev = Curve_resampled;
		}
	}

	return Curve_resampled;
}


template<typename ScalarType>
DenseMatrix<ScalarType> Resample3DCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType>& SegmentRelativeLengthList_resampled, int_max MaxIterCount, double Tolerance)
{
	DenseMatrix<ScalarType> Curve_resampled;// output

	//------------------------ check input --------------------------------------//
	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	auto SegmentCount_resampled = SegmentRelativeLengthList_resampled.GetLength();
	auto PointCount_resampled = SegmentCount_resampled + 1;

	auto tempSum = SegmentRelativeLengthList_resampled.Sum();
	if (tempSum > 1 + EPS*PointCount_resampled || tempSum < 1 - EPS*PointCount_resampled)
	{
		MDK_Error("SegmentRelativeLengthList_resampled.Sum() is NOT 1 @ Resample3DCurve(...)")
		return Curve_resampled;
	}

	if (Tolerance < EPS)
	{
		MDK_Error("Tolerance is too small (< eps) @ Resample3DCurve(...)")
		return Curve_resampled;
	}

	if (MaxIterCount < 1)
	{
		MDK_Error("MaxIterCount < 1 @ Resample3DCurve(...)")
		return Curve_resampled;
	}

	auto PointCount_input = Curve.GetColCount();
	auto CurveLength_input = Compute3DCurveLength(Curve);

	if (PointCount_resampled <= 0)
	{
		return Curve_resampled;
	}
	else if (PointCount_resampled == 1)
	{
		MDK_Error("PointCount_resampled is 1 @ Resample3DCurve(...)")
			Curve_resampled = Curve.MeanOfEachRow();
		return Curve_resampled;
	}
	else if (PointCount_resampled == 2)
	{
		Curve_resampled = Curve.GetSubMatrix(ALL, { 0, PointCount_input - 1 });
		return Curve_resampled;
	}

	if (CurveLength_input < EPS*(PointCount_input - 1))//check if curve shrink to a point
	{
		MDK_Error("curve shrink to a point @ Resample3DCurve(...)")
		return Curve_resampled;
	}
	//------------------------------------------------------------------------//
	auto UnitLength_input = CurveLength_input / ScalarType(PointCount_resampled - 1);

	auto Curve_Interpolated = Resample3DCurveWithBoundedSegmentLength(Curve, UnitLength_input*Tolerance); // InterpolateCurve input curve

	auto PointCount_Interpolated = Curve_Interpolated.GetColCount();

	auto CumulativeCurveLengthList_Interpolated = ComputeCumulative3DCurveLengthList(Curve_Interpolated);

	DenseVector<ScalarType> CumulativeCurveRelativeLengthList_resampled;
	CumulativeCurveRelativeLengthList_resampled.Resize(PointCount_resampled);
	CumulativeCurveRelativeLengthList_resampled[0] = 0;
	for (int_max k = 1; k < PointCount_resampled; ++k)
	{
		CumulativeCurveRelativeLengthList_resampled[k] += CumulativeCurveRelativeLengthList_resampled[k-1] + SegmentRelativeLengthList_resampled[k - 1];
	}
	//----------------------------- Initilization ------------------------------------------------------------//
	Curve_resampled.Resize(3, PointCount_resampled);
	Curve_resampled.SetCol(0, Curve_Interpolated.GetPointerOfCol(0));
	Curve_resampled.SetCol(PointCount_resampled - 1, Curve_Interpolated.GetPointerOfCol(PointCount_Interpolated - 1));
	{
	    int_max PointIndex_n = 1;
		for (int_max n = 1; n < PointCount_resampled - 1; ++n)
		{
			auto Length_n = ScalarType(0.99)*CumulativeCurveRelativeLengthList_resampled[n] * CurveLength_input;
			for (int_max k = PointIndex_n; k <= PointCount_Interpolated - 2; ++k)
			{
				if (Length_n >= CumulativeCurveLengthList_Interpolated[k - 1] && Length_n <= CumulativeCurveLengthList_Interpolated[k])
				{
					PointIndex_n = k;
					break;
				}
			}
			Curve_resampled.SetCol(n, Curve_Interpolated.GetPointerOfCol(PointIndex_n));
			PointIndex_n += 1;
		}
	}
	//----------------------------- Iteration to refine --------------------------------------------------------------------//
	// find the optimal UnitLength
	ScalarType UnitLength = Compute3DCurveLength(Curve_resampled) / ScalarType(PointCount_resampled - 1);
	for (int_max IterIndex = 1; IterIndex <= MaxIterCount; ++IterIndex)
	{
		int_max PointIndex_n = 1;
		bool Flag = true;
		for (int_max n = 1; n < PointCount_resampled - 1; ++n)
		{
			DenseVector<ScalarType, 3> Pos_n1;
			Curve_resampled.GetCol(n - 1, Pos_n1);
			auto SegmentLength_prefered = SegmentRelativeLengthList_resampled[n - 1] * ScalarType(PointCount_resampled-1)*UnitLength;
			bool Flag_n = false;
			for (int_max k = PointIndex_n; k <= PointCount_Interpolated - 2; ++k)
			{
				DenseVector<ScalarType, 3> Pos_k;
				Curve_Interpolated.GetCol(k, Pos_k);
				auto Distance_n1_k = (Pos_n1 - Pos_k).L2Norm();
				if (Distance_n1_k >= SegmentLength_prefered)
				{
					PointIndex_n = k;
					Flag_n = true;
					break;
				}
			}
			if (Flag_n == true)
			{
				Curve_resampled.SetCol(n, Curve_Interpolated.GetPointerOfCol(PointIndex_n));
				PointIndex_n += 1;
			}
			else
			{
				Flag = false;
				break;
			}
		}

		if (Flag == false)
		{// UnitLength is too large, can not construct a full resample curve
			UnitLength *= 0.9;
		}
		else
		{//check Curve_resampled to decide wheter to increase or decrease UnitLength: only need to check the distance between last two point -> usually the largest error is in it
			DenseVector<ScalarType, 3> Pos_a, Pos_b;
			Curve_resampled.GetCol(PointCount_resampled - 2, Pos_a);
			Curve_resampled.GetCol(PointCount_resampled - 1, Pos_b);
			auto Distance = (Pos_a - Pos_b).L2Norm();
			auto Ratio = std::abs(Distance - UnitLength) / UnitLength;
			if (Ratio <= Tolerance)
			{
				break;//iteration
			}
			UnitLength = Compute3DCurveLength(Curve_resampled) / ScalarType(PointCount_resampled - 1);
		}
	}

	return Curve_resampled;
}


}// namespace mdk
