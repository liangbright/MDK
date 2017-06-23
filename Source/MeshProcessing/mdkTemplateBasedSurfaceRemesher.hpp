#pragma once

namespace mdk
{
template<typename ScalarType>
TemplateBasedSurfaceRemesher<ScalarType>::TemplateBasedSurfaceRemesher()
{
	this->Clear();
}

template<typename ScalarType>
TemplateBasedSurfaceRemesher<ScalarType>::~TemplateBasedSurfaceRemesher()
{
}

template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::Clear()
{
	// dot NOT use Clear for input
	Input.SourceMesh = nullptr;
	Input.BoundarySegmentListOfSourceMesh.Recreate();
	Input.TemplateMesh = nullptr;
	Input.BoundarySegmentListOfTemplateMesh.Recreate();
	Input.DiffusionCoefficientOfMeshParameterization = 0.5;
	Input.MaxIterationOfMeshParameterization = 10;

	Internal.BoundaryPointIndexListOfSourceMesh.Clear();
	Internal.UVTalbleOfBoundaryOfSourceMesh.Clear();
	Internal.BoundaryPointIndexListOfTemplateMesh.Clear();
	Internal.BoundaryPositionOfTemplateMesh.Clear();
	Internal.BoundaryPositionOfDeformedTemplateMesh.Clear();

	Output.ParameterizedSourceMesh.Clear();
	Output.DeformedTemplateMesh.Clear();
}


template<typename ScalarType>
bool TemplateBasedSurfaceRemesher<ScalarType>::CheckInput()
{
	if (Input.SourceMesh == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->IsEmpty() == true)
	{
		MDK_Error("InputMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->GetPointCount() < 3)
	{
		MDK_Error("InputMesh PointCount < 3 @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT Clean @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->CheckIfTriangleMesh() == false)
	{
		MDK_Error("InputMesh is NOT TriangleMesh @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.BoundarySegmentListOfSourceMesh.IsEmpty() == true)
	{
		MDK_Error("BoundarySegmentListOfInputMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.TemplateMesh == nullptr)
	{
		MDK_Error("TemplateMesh is nullptr @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.TemplateMesh->IsEmpty() == true)
	{
		MDK_Error("TemplateMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.BoundarySegmentListOfTemplateMesh.IsEmpty() == true)
	{
		MDK_Error("BoundarySegmentListOfTemplateMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.BoundarySegmentListOfSourceMesh.GetLength() != Input.BoundarySegmentListOfTemplateMesh.GetLength())
	{
		MDK_Error("BoundarySegmentList not match @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	for (int_max k = 0; k < Input.BoundarySegmentListOfSourceMesh.GetLength(); ++k)
	{
		if (Input.BoundarySegmentListOfSourceMesh[k].GetLength() == 0 || Input.BoundarySegmentListOfTemplateMesh[k].GetLength() == 0)
		{
			MDK_Error("BoundarySegment is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
			return false;
		}
	}

	if (Input.DiffusionCoefficientOfMeshParameterization < 0 || Input.DiffusionCoefficientOfMeshParameterization > 1)
	{
		MDK_Error("DiffusionCoefficientOfMeshParameterization is out of range [0, 1] @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.MaxIterationOfMeshParameterization <= 0)
	{
		MDK_Error("MaxIterationOfMeshParameterization <= 0 @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	//Input.SourceMesh: every point should belong to a face
	auto PointCount = Input.SourceMesh->GetPointCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto AdjPointCount = Input.SourceMesh->Point(k).GetAdjacentPointCount();
		if (AdjPointCount < 2)
		{
			MDK_Error("AdjPointCount < 2 @ TemplateBasedSurfaceRemesher::CheckInput()")
			return false;
		}
	}

	//check if all the boundary point of inputmesh included in Input.BoundarySegmentListOfSourceMesh	
	auto BoundarySet_input = TraceMeshBoundaryCurve(*Input.SourceMesh);
	bool Flag_all_in = true;
	for (int_max k = 0; k < BoundarySet_input.GetLength(); ++k)
	{
		const auto& Boundary_k = BoundarySet_input[k];
		for (int_max n = 0; n < Boundary_k.GetLength(); ++n)
		{
			bool Flag_n_in = false;
			for (int_max m = 0; m < Input.BoundarySegmentListOfSourceMesh.GetLength(); ++m)
			{
				auto tempIndex = Input.BoundarySegmentListOfSourceMesh[m].ExactMatch("first", Boundary_k[n]);
				if (tempIndex >= 0)
				{
					Flag_n_in = true;
					break;
				}
			}
			if (Flag_n_in == false)
			{
				Flag_all_in = false;
				break;
			}
		}
	}
	if (Flag_all_in == false)
	{
		MDK_Error("Some Boundary Point NOT included in BoundarySegmentListOfInputMesh @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}	

	return true;
}

template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	
	this->FindBoundaryConstraint();

	if (this->CheckBoundaryConstraint() == false)
	{
		return;
	}

	this->TransformInputMeshFrom3DTo2D();
	this->TransfromTemplateMeshFrom2Dto3D();
}

template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::FindBoundaryConstraint()
{
	Internal.BoundaryPointIndexListOfSourceMesh.Clear();
	Internal.BoundaryPointIndexListOfSourceMesh.SetCapacity(100 * Input.BoundarySegmentListOfSourceMesh.GetLength());

	ObjectArray<DenseMatrix<ScalarType>> UVTalbleOfInputBoundary;
	UVTalbleOfInputBoundary.Resize(Input.BoundarySegmentListOfSourceMesh.GetLength());

	for (int_max k = 0; k < Input.BoundarySegmentListOfSourceMesh.GetLength(); ++k)
	{
		auto& UVTable_input = UVTalbleOfInputBoundary[k];
		UVTable_input.Resize(3, Input.BoundarySegmentListOfSourceMesh[k].GetLength());
		UVTable_input.Fill(0);

		const auto& CurveHandle_input = Input.BoundarySegmentListOfSourceMesh[k];
		const auto& CurveHandle_template = Input.BoundarySegmentListOfTemplateMesh[k];
		auto Curve_input = Input.SourceMesh->GetPointPosition(CurveHandle_input);
		auto Curve_template = Input.TemplateMesh->GetPointPosition(CurveHandle_template);
		
		if (CurveHandle_input.GetLength() >= 2 && CurveHandle_template.GetLength() >= 2)
		{
			auto CL_input = this->ComputeCumulativeCurveLength_Relative(Curve_input);
			auto CL_template = this->ComputeCumulativeCurveLength_Relative(Curve_template);

			int_max index_m = 0;
			for (int_max n = 0; n < CurveHandle_input.GetLength(); ++n)
			{
				for (int_max m = index_m; m < CurveHandle_template.GetLength() - 1; ++m)
				{
					if (CL_input[n] >= CL_template[m] && CL_input[n] <= CL_template[m + 1])
					{
						DenseVector<ScalarType, 3> TemplatePos2D_m, TemplatePos2D_m1;
						Curve_template.GetCol(m, TemplatePos2D_m);
						Curve_template.GetCol(m + 1, TemplatePos2D_m1);

						auto Ratio = (CL_input[n] - CL_template[m]) / (CL_template[m + 1] - CL_template[m]);
						auto InputPos2D = TemplatePos2D_m + (TemplatePos2D_m1 - TemplatePos2D_m)*Ratio;
						UVTable_input.SetCol(n, InputPos2D);

						index_m = m;
						break;
					}
				}
			}
		}
		else if (CurveHandle_input.GetLength() == 1 && CurveHandle_template.GetLength() == 1)
		{
			UVTable_input = Curve_template;
		}
		else
		{
			MDK_Error("Something is wrong @ TemplateBasedSurfaceRemesher::FindBoundaryConstraint(...)")
			return;
		}

		Internal.BoundaryPointIndexListOfSourceMesh.Append(CurveHandle_input);
	}

	Internal.BoundaryPointIndexListOfSourceMesh = Internal.BoundaryPointIndexListOfSourceMesh.GetSubSet(Internal.BoundaryPointIndexListOfSourceMesh.FindUnique());
	
	//-----------------------------------------------------------------------------------------------//
	Internal.UVTalbleOfBoundaryOfSourceMesh.Clear();
	Internal.UVTalbleOfBoundaryOfSourceMesh.Resize(2, Internal.BoundaryPointIndexListOfSourceMesh.GetLength());
	Internal.UVTalbleOfBoundaryOfSourceMesh.Fill(0);

	for (int_max k = 0; k < Input.BoundarySegmentListOfSourceMesh.GetLength(); ++k)
	{
		const auto& CurveHandle = Input.BoundarySegmentListOfSourceMesh[k];
		const auto& UVCurve = UVTalbleOfInputBoundary[k];

		for (int_max n = 0; n < CurveHandle.GetLength(); ++n)
		{
			auto tempIndex = Internal.BoundaryPointIndexListOfSourceMesh.ExactMatch("first", CurveHandle[n]);
			auto u = UVCurve(0, n);
			auto v = UVCurve(1, n);
			Internal.UVTalbleOfBoundaryOfSourceMesh.SetCol(tempIndex, {u,v});
		}
	}
	//-----------------------------------------------------------------------------------------------//

	Internal.BoundaryPointIndexListOfTemplateMesh.Clear();
	Internal.BoundaryPointIndexListOfTemplateMesh.SetCapacity(100 * Input.BoundarySegmentListOfSourceMesh.GetLength());

	ObjectArray<DenseMatrix<ScalarType>> OutputBoundaryPosition;
	OutputBoundaryPosition.Resize(Input.BoundarySegmentListOfTemplateMesh.GetLength());

	for (int_max k = 0; k < Input.BoundarySegmentListOfTemplateMesh.GetLength(); ++k)
	{
		auto& Boundary_output = OutputBoundaryPosition[k];
		Boundary_output.Resize(3, Input.BoundarySegmentListOfTemplateMesh[k].GetLength());
		Boundary_output.Fill(0);

		const auto& CurveHandle_input = Input.BoundarySegmentListOfSourceMesh[k];
		const auto& CurveHandle_template = Input.BoundarySegmentListOfTemplateMesh[k];
		auto Curve_input = Input.SourceMesh->GetPointPosition(CurveHandle_input);
		auto Curve_template = Input.TemplateMesh->GetPointPosition(CurveHandle_template);
				
		if (CurveHandle_input.GetLength() >= 2 && CurveHandle_template.GetLength() >= 2)
		{
			auto CL_input = this->ComputeCumulativeCurveLength_Relative(Curve_input);
			auto CL_template = this->ComputeCumulativeCurveLength_Relative(Curve_template);

			int_max index_m = 0;
			for (int_max n = 0; n < CurveHandle_template.GetLength(); ++n)
			{
				for (int_max m = index_m; m < CurveHandle_input.GetLength() - 1; ++m)
				{
					if (CL_template[n] >= CL_input[m] && CL_template[n] <= CL_input[m + 1])
					{
						DenseVector<ScalarType, 3> InputPos3D_m, InputPos3D_m1;
						Curve_input.GetCol(m, InputPos3D_m);
						Curve_input.GetCol(m + 1, InputPos3D_m1);

						auto Ratio = (CL_template[n] - CL_input[m]) / (CL_input[m + 1] - CL_input[m]);
						auto Pos3D = InputPos3D_m + (InputPos3D_m1 - InputPos3D_m)*Ratio;
						Boundary_output.SetCol(n, Pos3D);

						index_m = m;
						break;
					}
				}
			}
		}
		else if (CurveHandle_input.GetLength() == 1 && CurveHandle_template.GetLength() == 1)
		{
			Boundary_output = Curve_input;
		}

		Internal.BoundaryPointIndexListOfTemplateMesh.Append(CurveHandle_template);
	}

	Internal.BoundaryPointIndexListOfTemplateMesh = Internal.BoundaryPointIndexListOfTemplateMesh.GetSubSet(Internal.BoundaryPointIndexListOfTemplateMesh.FindUnique());

	//-----------------------------------------------------------------------------------------------//
	Internal.BoundaryPositionOfDeformedTemplateMesh.Clear();
	Internal.BoundaryPositionOfDeformedTemplateMesh.Resize(3, Internal.BoundaryPointIndexListOfTemplateMesh.GetLength());
	Internal.BoundaryPositionOfDeformedTemplateMesh.Fill(0);

	for (int_max k = 0; k < Input.BoundarySegmentListOfTemplateMesh.GetLength(); ++k)
	{
		const auto& CurveHandle = Input.BoundarySegmentListOfTemplateMesh[k];
		const auto& Curve = OutputBoundaryPosition[k];

		for (int_max n = 0; n < CurveHandle.GetLength(); ++n)
		{
			auto tempIndex = Internal.BoundaryPointIndexListOfTemplateMesh.ExactMatch("first", CurveHandle[n]);
			Internal.BoundaryPositionOfDeformedTemplateMesh.SetCol(tempIndex, Curve.GetPointerOfCol(n));
		}
	}

	Internal.BoundaryPositionOfTemplateMesh = Input.TemplateMesh->GetPointPosition(Internal.BoundaryPointIndexListOfTemplateMesh);
}


template<typename ScalarType>
bool TemplateBasedSurfaceRemesher<ScalarType>::CheckBoundaryConstraint()
{
	//check if all the boundary point of input mesh is included in Internal.BoundaryPointIndexListOfSourceMesh
	for (auto it = Input.SourceMesh->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnBoundaryEdge() == true)
		{
			auto tempIndex = Internal.BoundaryPointIndexListOfSourceMesh.ExactMatch("first", it.GetPointIndex());
			if (tempIndex < 0)
			{
				MDK_Error(" Some boundary point of input mesh is NOT included in BoundarySegmentListOfInputMesh @ TemplateBasedSurfaceRemesher::CheckBoundaryConstraint()")
				return false;
			}
		}
	}

	for (int_max k = 0; k < Input.BoundarySegmentListOfSourceMesh.GetLength(); ++k)
	{
		const auto& CurveHandle_input = Input.BoundarySegmentListOfSourceMesh[k];
		const auto& CurveHandle_template = Input.BoundarySegmentListOfTemplateMesh[k];

		if (CurveHandle_input.GetLength() == 1 && CurveHandle_template.GetLength() > 1 || CurveHandle_input.GetLength() > 1 && CurveHandle_template.GetLength() == 1)
		{
			MDK_Error(" input boundary NOT match output boundary @ TemplateBasedSurfaceRemesher::CheckBoundaryConstraint()")
			return false;
		}
	}

	return true;
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransformInputMeshFrom3DTo2D()
{	
	MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType> Mapper;
	Mapper.Input.SourceMesh = Input.SourceMesh;
	Mapper.Input.BoundaryPointIndexList = Internal.BoundaryPointIndexListOfSourceMesh;
	Mapper.Input.UVTableOfBoundary = Internal.UVTalbleOfBoundaryOfSourceMesh;
	Mapper.Input.DiffusionCoefficient = Input.DiffusionCoefficientOfMeshParameterization;
	Mapper.Update();
	Output.ParameterizedSourceMesh = std::move(Mapper.Output.ParameterizedSourceMesh);
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransfromTemplateMeshFrom2Dto3D()
{	
	if (Input.Flag_EnableTPSTransformOfTemplateMesh == true)
	{
		this->TransfromTemplateMeshFrom2Dto3D_Method0_TPS();
	}
	else
	{
		this->TransfromTemplateMeshFrom2Dto3D_Method1_Interpolation();
	}
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransfromTemplateMeshFrom2Dto3D_Method0_TPS()
{		
	auto PointSet3D_input = Input.SourceMesh->GetPointPosition(ALL);    
	auto PointSet2D_input = Output.ParameterizedSourceMesh.GetPointPosition(ALL);

	DenseMatrix<ScalarType> PointSet2D, PointSet3D;
	PointSet2D = { &PointSet2D_input, &Internal.BoundaryPositionOfTemplateMesh };
	PointSet3D = { &PointSet3D_input, &Internal.BoundaryPositionOfDeformedTemplateMesh };

	ThinPlateSplineTransform3D<ScalarType> TPSWarper;
	TPSWarper.SetSourceLandmarkPointSet(&PointSet2D);
	TPSWarper.SetTargetLandmarkPointSet(&PointSet3D);
	TPSWarper.EstimateParameter();

	Output.DeformedTemplateMesh.Clear();
	Output.DeformedTemplateMesh = *Input.TemplateMesh;
	for (auto it = Output.DeformedTemplateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos2D = it.Point().GetPosition();
		auto Pos3D = TPSWarper.TransformPoint(Pos2D);
		it.Point().SetPosition(Pos3D);
	}
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransfromTemplateMeshFrom2Dto3D_Method1_Interpolation()
{
	Output.DeformedTemplateMesh.Clear();
	Output.DeformedTemplateMesh = *Input.TemplateMesh;
	for (auto it = Output.DeformedTemplateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto tempIndex = Internal.BoundaryPointIndexListOfTemplateMesh.ExactMatch("first", it.GetPointIndex());
		if (tempIndex >= 0)
		{
			DenseVector<ScalarType, 3> Pos3D;
			Internal.BoundaryPositionOfDeformedTemplateMesh.GetCol(tempIndex, Pos3D);
			it.Point().SetPosition(Pos3D);
		}
		else
		{
			auto Pos2D = it.Point().GetPosition();
			auto PointIndexList_nearest = this->Find3PointOfNearestFace(Pos2D, Output.ParameterizedSourceMesh);

			DenseVector<ScalarType, 3> Weight;
			Weight.Fill(0);
			{
				auto Point0 = Output.ParameterizedSourceMesh.GetPointPosition(PointIndexList_nearest[0]);
				auto Point1 = Output.ParameterizedSourceMesh.GetPointPosition(PointIndexList_nearest[1]);
				auto Point2 = Output.ParameterizedSourceMesh.GetPointPosition(PointIndexList_nearest[2]);

				auto x = Pos2D[0];
				auto y = Pos2D[1];
				auto x1 = Point0[0];
				auto y1 = Point0[1];
				auto x2 = Point1[0];
				auto y2 = Point1[1];
				auto x3 = Point2[0];
				auto y3 = Point2[1];

				//https://en.wikipedia.org/wiki/Barycentric_coordinate_system

				auto det = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);

				if (std::abs(det) > 0.00000001)
				{
					Weight[0] = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / det;
					Weight[1] = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / det;
					Weight[2] = 1 - Weight[0] - Weight[1];
				}
				else
				{
					Weight[0] = (x - x2) / (x1 - x2);
					Weight[1] = 1 - Weight[0];
					Weight[2] = 0;
				}
				
			}

			auto Point0 = Input.SourceMesh->GetPointPosition(PointIndexList_nearest[0]);
			auto Point1 = Input.SourceMesh->GetPointPosition(PointIndexList_nearest[1]);
			auto Point2 = Input.SourceMesh->GetPointPosition(PointIndexList_nearest[2]);
			auto Pos3D = Weight[0] * Point0 + Weight[1] * Point1 + Weight[2] * Point2;
			it.Point().SetPosition(Pos3D);
		}
	}

}


template<typename ScalarType>
DenseVector<ScalarType> TemplateBasedSurfaceRemesher<ScalarType>::ComputeCumulativeCurveLength(const DenseMatrix<ScalarType>& CurvePosition)
{
	DenseVector<ScalarType> LengthList;
	LengthList.Resize(CurvePosition.GetColCount());
	LengthList.Fill(0);

	if (CurvePosition.GetColCount() < 2)
	{
		MDK_Error("CurvePosition  ColCount < 2 @ TemplateBasedSurfaceRemesher::ComputeCumulativeCurveLength(...)")
		return LengthList;
	}	

	for (int_max k = 1; k < CurvePosition.GetColCount(); ++k)
	{
		ScalarType Pos_a[3], Pos_b[3];
		CurvePosition.GetCol(k, Pos_a);
		CurvePosition.GetCol(k - 1, Pos_b);
		auto Distance = (Pos_a[0] - Pos_b[0])*(Pos_a[0] - Pos_b[0])
			           + (Pos_a[1] - Pos_b[1])*(Pos_a[1] - Pos_b[1])
			           + (Pos_a[2] - Pos_b[2])*(Pos_a[2] - Pos_b[2]);
		Distance = std::sqrt(Distance);
		LengthList[k] = Distance + LengthList[k - 1];
	}

	return LengthList;
}


template<typename ScalarType>
DenseVector<ScalarType> TemplateBasedSurfaceRemesher<ScalarType>::ComputeCumulativeCurveLength_Relative(const DenseMatrix<ScalarType>& CurvePosition)
{
	auto CL = this->ComputeCumulativeCurveLength(CurvePosition);
	DenseVector<ScalarType> CL_relative;
	CL_relative.Resize(CL.GetLength());
	for (int_max k = 0; k < CL.GetLength(); ++k)
	{
		CL_relative[k] = CL[k] / CL[CL.GetLength()-1];
	}
	return CL_relative;
}


template<typename ScalarType>
DenseVector<int_max, 3> TemplateBasedSurfaceRemesher<ScalarType>::Find3PointOfNearestFace(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& TargetMesh)
{
	DenseVector<int_max, 3> PointIndexList_nearest;
	//-------------------------------------------------------------------
	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	EPS = EPS * 10;
	//-------------------------------------------------------------------
	DenseVector<int_max> CandidateFaceIndexList;
	int_max FaceIndex_nearest = -1;
	int_max PointIndex_nearest = -1;
	DenseVector<ScalarType, 3> PointPosition_nearest;
	{
		DenseVector<ScalarType> DistanceList;
		DistanceList.SetCapacity(TargetMesh.GetFaceCount());
		for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
		{
			auto Pos = it.Point().GetPosition();
			auto Distance = (Point - Pos).L2Norm();
			DistanceList.Append(Distance);
		}
		auto PointIndexList_sort = DistanceList.Sort("ascend");
		PointIndex_nearest = DistanceList.IndexOfMin();
		PointPosition_nearest = TargetMesh.GetPointPosition(PointIndex_nearest);

		auto tempFaceInexList = TargetMesh.Point(PointIndex_nearest).GetAdjacentFaceIndexList();
		FaceIndex_nearest = tempFaceInexList[0];

		for (int_max n = 0; n < 3; ++n)
		{
			int_max PointIndex_n = PointIndexList_sort[n];
			auto AdjPointIndexList = TargetMesh.Point(PointIndex_n).GetAdjacentPointIndexList();
			AdjPointIndexList.Insert(0, PointIndex_n);
			for (int_max k = 0; k < AdjPointIndexList.GetLength(); ++k)
			{
				auto AdjFaceIndexList = TargetMesh.Point(AdjPointIndexList[k]).GetAdjacentFaceIndexList();
				CandidateFaceIndexList.Append(AdjFaceIndexList);
			}
		}
		CandidateFaceIndexList = CandidateFaceIndexList.GetSubSet(CandidateFaceIndexList.FindUnique());
	}
	//-------------------------------------------------------------------
	auto PointDistance_nearest = (PointPosition_nearest - Point).L2Norm();
	if (PointDistance_nearest <= EPS)
	{
		auto AdjFaceIndexList = TargetMesh.Point(PointIndex_nearest).GetAdjacentFaceIndexList();
		PointIndexList_nearest = TargetMesh.Face(AdjFaceIndexList[0]).GetPointIndexList();
		return PointIndexList_nearest;
	}
	//-------------------------------------------------------------------
	auto TempFunction_det = [](const DenseVector<ScalarType, 3>& U, const DenseVector<ScalarType, 3>& V)
	{
		return U[0] * V[1] - U[1] * V[0];
	};
	//-------------------------------------------------------------------
	bool Flag = false;	
	for (int_max k = 0; k < CandidateFaceIndexList.GetLength(); ++k)
	{
		auto PointIndexList_k = TargetMesh.Face(CandidateFaceIndexList[k]).GetPointIndexList();//triangle: 3 point
		auto Point0 = TargetMesh.GetPointPosition(PointIndexList_k[0]);
		auto Point1 = TargetMesh.GetPointPosition(PointIndexList_k[1]);
		auto Point2 = TargetMesh.GetPointPosition(PointIndexList_k[2]);
		//http://mathworld.wolfram.com/TriangleInterior.html
		//auto V = Point;
		//auto V0 = Point0;		
		//auto V1 = Point1 - Point0;
		//auto V2 = Point2 - Point0;
		//auto a = (TempFunction_det(V, V2) - TempFunction_det(V0, V2)) / TempFunction_det(V1, V2);
		//auto b = (TempFunction_det(V, V1) - TempFunction_det(V0, V1)) / TempFunction_det(V2, V1);

		//http://stackoverflow.com/questions/2049582/how-to-determine-a-point-in-a-2d-triangle
		auto x = Point[0];
		auto y = Point[1];
		auto x0 = Point0[0];
		auto y0 = Point0[1];
		auto x1 = Point1[0];
		auto y1 = Point1[1];
		auto x2 = Point2[0];
		auto y2 = Point2[1];
		auto Area = 0.5*(-y1*x2 + y0*(-x1 + x2) + x0*(y1 - y2) + x1*y2);
		auto a = 1/(2*Area)*(y0*x2 - x0*y2 + (y2 - y0)*x + (x0 - x2)*y);
		auto b = 1/(2*Area)*(x0*y1 - y0*x1 + (y0 - y1)*x + (x1 - x0)*y);

		//if (a >= -0.0001 && b >= -0.0001 && a+b <= 1.0001)
		if (a >= -EPS && b >= -EPS && 1-(a + b) >= -EPS)
		{
			FaceIndex_nearest = CandidateFaceIndexList[k];
			Flag = true;
			break;
		}

	}

	if (Flag == false)
	{
		MDK_Warning("Flag == false @ TemplateBasedSurfaceRemesher::Find3PointOfNearestFace(...)")
	}

	PointIndexList_nearest = TargetMesh.Face(FaceIndex_nearest).GetPointIndexList();
	return PointIndexList_nearest;
}

}//namespace mdk