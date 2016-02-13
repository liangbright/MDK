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
	m_InputMesh.Clear();
	m_BoundarySegmentListOfInputMesh.Clear();
	m_TemplateMesh.Clear();
	m_BoundarySegmentListOfTemplateMesh.Clear();

	m_BoundaryPointHandleListOfInputMesh.Clear();
	m_UVTalbleOfBoundaryOfInputMesh.Clear();

	m_DiffusionCoefficient = 0.5;
	m_MaxInteration = 10;

	this->ClearInternalData();

	m_OutputMesh.Clear();
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::ClearInternalData()
{
	m_BoundaryPointHandleListOfTemplateMesh.Clear();
	m_BoundaryPositionOfTemplateMesh.Clear();
	m_BoundaryPositionOfOutputMesh.Clear();
	m_TransfromedInputMesh.Clear();
}


template<typename ScalarType>
bool TemplateBasedSurfaceRemesher<ScalarType>::CheckInput()
{
	if (m_InputMesh.IsEmpty() == true)
	{
		MDK_Error("InputMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_InputMesh.GetPointCount() < 3)
	{
		MDK_Error("InputMesh PointCount < 3 @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT Clean @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("InputMesh is NOT TriangleMesh @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_BoundarySegmentListOfInputMesh.IsEmpty() == true)
	{
		MDK_Error("BoundarySegmentListOfInputMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_TemplateMesh.IsEmpty() == true)
	{
		MDK_Error("TemplateMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_BoundarySegmentListOfTemplateMesh.IsEmpty() == true)
	{
		MDK_Error("BoundarySegmentListOfTemplateMesh is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_BoundarySegmentListOfInputMesh.GetLength() != m_BoundarySegmentListOfTemplateMesh.GetLength())
	{
		MDK_Error("BoundarySegmentList not match @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	for (int_max k = 0; k < m_BoundarySegmentListOfInputMesh.GetLength(); ++k)
	{
		if (m_BoundarySegmentListOfInputMesh[k].GetLength() == 0 || m_BoundarySegmentListOfTemplateMesh[k].GetLength() == 0)
		{
			MDK_Error("BoundarySegment is empty @ TemplateBasedSurfaceRemesher::CheckInput()")
			return false;
		}
	}

	if (m_DiffusionCoefficient < 0 || m_DiffusionCoefficient > 1)
	{
		MDK_Error("DiffusionCoefficient is out of range [0, 1] @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_MaxInteration <= 0)
	{
		MDK_Error("MaxInteration <= 0 @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	//check if all the boundary point of inputmesh included in m_BoundarySegmentListOfInputMesh
	/*
	auto BoundarySet_input = TraceMeshBoundaryCurve(m_InputMesh);
	bool Flag_all_in = true;
	for (int_max k = 0; k < BoundarySet_input.GetLength(); ++k)
	{
		const auto& Boundary_k = BoundarySet_input[k];
		for (int_max n = 0; n < Boundary_k.GetLength(); ++n)
		{
			bool Flag_n_in = false;
			for (int_max m = 0; m < m_BoundarySegmentListOfInputMesh.GetLength(); ++m)
			{
				auto tempIndex = m_BoundarySegmentListOfInputMesh[m].ExactMatch("first", Boundary_k[n]);
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
	*/

	return true;
}

template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
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
	m_BoundaryPointHandleListOfInputMesh.Clear();
	m_BoundaryPointHandleListOfInputMesh.SetCapacity(100 * m_BoundarySegmentListOfInputMesh.GetLength());

	ObjectArray<DenseMatrix<ScalarType>> UVTalbleOfInputBoundary;
	UVTalbleOfInputBoundary.Resize(m_BoundarySegmentListOfInputMesh.GetLength());

	for (int_max k = 0; k < m_BoundarySegmentListOfInputMesh.GetLength(); ++k)
	{
		auto& UVTable_input = UVTalbleOfInputBoundary[k];
		UVTable_input.Resize(3, m_BoundarySegmentListOfInputMesh[k].GetLength());
		UVTable_input.Fill(0);

		const auto& CurveHandle_input = m_BoundarySegmentListOfInputMesh[k];
		const auto& CurveHandle_template = m_BoundarySegmentListOfTemplateMesh[k];
		auto Curve_input = m_InputMesh.GetPointPosition(CurveHandle_input);
		auto Curve_template = m_TemplateMesh.GetPointPosition(CurveHandle_template);
		auto CL_input = this->ComputeCumulativeCurveLength_Relative(Curve_input);
		auto CL_template = this->ComputeCumulativeCurveLength_Relative(Curve_template);

		int_max index_m = 0;
		for (int_max n = 0; n < CurveHandle_input.GetLength(); ++n)
		{
			for (int_max m = index_m; m < CurveHandle_template.GetLength()-1; ++m)
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

		m_BoundaryPointHandleListOfInputMesh.Append(CurveHandle_input);
	}

	m_BoundaryPointHandleListOfInputMesh = m_BoundaryPointHandleListOfInputMesh.GetSubSet(m_BoundaryPointHandleListOfInputMesh.FindUnique());
	
	//-----------------------------------------------------------------------------------------------//
	m_UVTalbleOfBoundaryOfInputMesh.Clear();
	m_UVTalbleOfBoundaryOfInputMesh.Resize(2, m_BoundaryPointHandleListOfInputMesh.GetLength());
	m_UVTalbleOfBoundaryOfInputMesh.Fill(0);

	for (int_max k = 0; k < m_BoundarySegmentListOfInputMesh.GetLength(); ++k)
	{
		const auto& CurveHandle = m_BoundarySegmentListOfInputMesh[k];
		const auto& UVCurve = UVTalbleOfInputBoundary[k];

		for (int_max n = 0; n < CurveHandle.GetLength(); ++n)
		{
			auto tempIndex = m_BoundaryPointHandleListOfInputMesh.ExactMatch("first", CurveHandle[n]);
			auto u = UVCurve(0, n);
			auto v = UVCurve(1, n);
			m_UVTalbleOfBoundaryOfInputMesh.SetCol(tempIndex, {u,v});
		}
	}
	//-----------------------------------------------------------------------------------------------//

	m_BoundaryPointHandleListOfTemplateMesh.Clear();
	m_BoundaryPointHandleListOfTemplateMesh.SetCapacity(100 * m_BoundarySegmentListOfInputMesh.GetLength());

	ObjectArray<DenseMatrix<ScalarType>> OutputBoundaryPosition;
	OutputBoundaryPosition.Resize(m_BoundarySegmentListOfTemplateMesh.GetLength());

	for (int_max k = 0; k < m_BoundarySegmentListOfTemplateMesh.GetLength(); ++k)
	{
		auto&Boundary_output = OutputBoundaryPosition[k];
		Boundary_output.Resize(3, m_BoundarySegmentListOfTemplateMesh[k].GetLength());
		Boundary_output.Fill(0);

		const auto& CurveHandle_input = m_BoundarySegmentListOfInputMesh[k];
		const auto& CurveHandle_template = m_BoundarySegmentListOfTemplateMesh[k];
		auto Curve_input = m_InputMesh.GetPointPosition(CurveHandle_input);
		auto Curve_template = m_TemplateMesh.GetPointPosition(CurveHandle_template);
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

		m_BoundaryPointHandleListOfTemplateMesh.Append(CurveHandle_template);
	}

	m_BoundaryPointHandleListOfTemplateMesh = m_BoundaryPointHandleListOfTemplateMesh.GetSubSet(m_BoundaryPointHandleListOfTemplateMesh.FindUnique());

	//-----------------------------------------------------------------------------------------------//
	m_BoundaryPositionOfOutputMesh.Clear();
	m_BoundaryPositionOfOutputMesh.Resize(3, m_BoundaryPointHandleListOfTemplateMesh.GetLength());
	m_BoundaryPositionOfOutputMesh.Fill(0);

	for (int_max k = 0; k < m_BoundarySegmentListOfTemplateMesh.GetLength(); ++k)
	{
		const auto& CurveHandle = m_BoundarySegmentListOfTemplateMesh[k];
		const auto& Curve = OutputBoundaryPosition[k];

		for (int_max n = 0; n < CurveHandle.GetLength(); ++n)
		{
			auto tempIndex = m_BoundaryPointHandleListOfTemplateMesh.ExactMatch("first", CurveHandle[n]);
			m_BoundaryPositionOfOutputMesh.SetCol(tempIndex, Curve.GetPointerOfCol(n));
		}
	}

	m_BoundaryPositionOfTemplateMesh = m_TemplateMesh.GetPointPosition(m_BoundaryPointHandleListOfTemplateMesh);
}


template<typename ScalarType>
bool TemplateBasedSurfaceRemesher<ScalarType>::CheckBoundaryConstraint()
{
	//check if all the boundary point of input mesh is included in m_BoundaryPointHandleListOfInputMesh
	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnBoundaryEdge() == true)
		{
			auto tempIndex = m_BoundaryPointHandleListOfInputMesh.ExactMatch("first", it.GetPointHandle());
			if (tempIndex < 0)
			{
				MDK_Error(" Some boundary point of input mesh is NOT included in BoundarySegmentListOfInputMesh @ TemplateBasedSurfaceRemesher::CheckBoundaryConstraint()")
				return false;
			}
		}
	}
	return true;
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransformInputMeshFrom3DTo2D()
{
	MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType> Mapper;
	Mapper.InputMesh().Share(m_InputMesh);
	Mapper.BoundaryPointHandleList() = m_BoundaryPointHandleListOfInputMesh;
	Mapper.UVTableOfBoundary() = m_UVTalbleOfBoundaryOfInputMesh;
	Mapper.SetDiffusionCoefficient(m_DiffusionCoefficient);
	Mapper.Update();
	m_TransfromedInputMesh = std::move(Mapper.OutputMesh());
}


template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransfromTemplateMeshFrom2Dto3D()
{
	if (m_Flag_Use_TPS_Transform == true)
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
	auto PointSet3D_input = m_InputMesh.GetPointPosition(ALL);    
	auto PointSet2D_input = m_TransfromedInputMesh.GetPointPosition(ALL);

	DenseMatrix<ScalarType> PointSet2D, PointSet3D;
	PointSet2D = { &PointSet2D_input, &m_BoundaryPositionOfTemplateMesh };
	PointSet3D = { &PointSet3D_input, &m_BoundaryPositionOfOutputMesh };

	ThinPlateSplineTransform3D<ScalarType> TPSWarper;
	TPSWarper.SetSourceLandmarkPointSet(&PointSet2D);
	TPSWarper.SetTargetLandmarkPointSet(&PointSet3D);
	TPSWarper.EstimateParameter();

	m_OutputMesh.Clear();
	m_OutputMesh = m_TemplateMesh;
	for (auto it = m_OutputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos2D = it.Point().GetPosition();
		auto Pos3D = TPSWarper.TransformPoint(Pos2D);
		it.Point().SetPosition(Pos3D);
	}
}

template<typename ScalarType>
void TemplateBasedSurfaceRemesher<ScalarType>::TransfromTemplateMeshFrom2Dto3D_Method1_Interpolation()
{
	m_OutputMesh.Clear();
	m_OutputMesh = m_TemplateMesh;
	for (auto it = m_OutputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto tempIndex = m_BoundaryPointHandleListOfTemplateMesh.ExactMatch("first", it.GetPointHandle());
		if (tempIndex >= 0)
		{
			DenseVector<ScalarType, 3> Pos3D;
			m_BoundaryPositionOfOutputMesh.GetCol(tempIndex, Pos3D);
			it.Point().SetPosition(Pos3D);
		}
		else
		{
			auto Pos2D = it.Point().GetPosition();
			auto PointHandleList_nearest = this->Find3PointOfNearestFace(Pos2D, m_TransfromedInputMesh);

			DenseVector<ScalarType, 3> Weight;
			Weight.Fill(0);
			{
				auto Point0 = m_TransfromedInputMesh.GetPointPosition(PointHandleList_nearest[0]);
				auto Point1 = m_TransfromedInputMesh.GetPointPosition(PointHandleList_nearest[1]);
				auto Point2 = m_TransfromedInputMesh.GetPointPosition(PointHandleList_nearest[2]);

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

			auto Point0 = m_InputMesh.GetPointPosition(PointHandleList_nearest[0]);
			auto Point1 = m_InputMesh.GetPointPosition(PointHandleList_nearest[1]);
			auto Point2 = m_InputMesh.GetPointPosition(PointHandleList_nearest[2]);
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

	if (CurvePosition.GetColCount() < 2)
	{
		MDK_Error("CurvePosition  ColCount < 2 @ TemplateBasedSurfaceRemesher::ComputeCumulativeCurveLength(...)")
		return LengthList;
	}

	LengthList.Fill(0);

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
typename TemplateBasedSurfaceRemesher<ScalarType>::PointHandleType
TemplateBasedSurfaceRemesher<ScalarType>::ConvertPointIndexToPointHandle(int_max Index)
{
	PointHandleType P;
	P.SetIndex(Index);
	return P;
}

template<typename ScalarType>
DenseVector<typename TemplateBasedSurfaceRemesher<ScalarType>::PointHandleType> 
TemplateBasedSurfaceRemesher<ScalarType>::ConvertPointIndexToPointHandle(const DenseVector<int_max>& IndexList)
{
	DenseVector<PointHandleType> HandleList;
	HandleList.Resize(IndexList.GetLength());
	for (int_max k = 0; k < HandleList.GetLength(); ++k)
	{
		HandleList[k].SetIndex(IndexList[k]);
	}
	return HandleList;
}

template<typename ScalarType>
DenseVector<typename TemplateBasedSurfaceRemesher<ScalarType>::PointHandleType, 3>
TemplateBasedSurfaceRemesher<ScalarType>::Find3PointOfNearestFace(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<InputMeshAttribute>& TargetMesh)
{
	DenseVector<PointHandleType, 3> PointHandleList_nearest;
    //-----------------------------------------------//
	DenseVector<FaceHandleType> CandidateFaceHandleList;
	PointHandleType PointHanle_nearest;
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
		auto PointIndexList_nearest = PointIndexList_sort[0];
		auto PointIndex_nearest = DistanceList.IndexOfMin();
		PointHanle_nearest.SetIndex(PointIndex_nearest);
		PointPosition_nearest = TargetMesh.GetPointPosition(PointHanle_nearest);

		for (int_max n = 0; n < 3; ++n)
		{
			PointHandleType PointHandle_n;
			PointHandle_n.SetIndex(PointIndexList_sort[n]);
			auto AdjPointHandleList = TargetMesh.Point(PointHandle_n).GetAdjacentPointHandleList();
			AdjPointHandleList.Insert(0, PointHandle_n);
			for (int_max k = 0; k < AdjPointHandleList.GetLength(); ++k)
			{
				auto AdjFaceHandleList = TargetMesh.Point(AdjPointHandleList[k]).GetAdjacentFaceHandleList();
				CandidateFaceHandleList.Append(AdjFaceHandleList);
			}
		}
		CandidateFaceHandleList = CandidateFaceHandleList.GetSubSet(CandidateFaceHandleList.FindUnique());
	}
	
	//-------------------------------------------------------------------
	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	auto PointDistance_nearest = (PointPosition_nearest - Point).L2Norm();
	if (PointDistance_nearest <= EPS)
	{
		auto AdjFaceHandleList = TargetMesh.Point(PointHanle_nearest).GetAdjacentFaceHandleList();
		PointHandleList_nearest = TargetMesh.Face(AdjFaceHandleList[0]).GetPointHandleList();
		return PointHandleList_nearest;
	}
	//-------------------------------------------------------------------

	auto TempFunction_det = [](const DenseVector<ScalarType, 3>& U, const DenseVector<ScalarType, 3>& V)
	{
		return U[0] * V[1] - U[1] * V[0];
	};

	auto FaceHandle_nearest = CandidateFaceHandleList[0];
	bool Flag = false;	
	for (int_max k = 0; k < CandidateFaceHandleList.GetLength(); ++k)
	{
		auto PointHandleList_k = TargetMesh.Face(CandidateFaceHandleList[k]).GetPointHandleList();//triangle: 3 point
		auto Point0 = TargetMesh.GetPointPosition(PointHandleList_k[0]);
		auto Point1 = TargetMesh.GetPointPosition(PointHandleList_k[1]);
		auto Point2 = TargetMesh.GetPointPosition(PointHandleList_k[2]);
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

		if (a >= -0.0001 && b >= -0.0001 && a+b <= 1.0001)
		{
			FaceHandle_nearest = CandidateFaceHandleList[k];
			Flag = true;
			break;
		}

	}

	if (Flag == false)
	{
		MDK_Warning("Flag == false @ TemplateBasedSurfaceRemesher::Find3PointOfNearestFace(...)")
	}

	PointHandleList_nearest = TargetMesh.Face(FaceHandle_nearest).GetPointHandleList();
	return PointHandleList_nearest;
}

}//namespace mdk