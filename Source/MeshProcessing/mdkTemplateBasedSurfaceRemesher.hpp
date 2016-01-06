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

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ TemplateBasedSurfaceRemesher::CheckInput()")
		return false;
	}

	if (m_InputMesh.GetPointCount() < 3)
	{
		MDK_Error("InputMesh PointCount < 3 @ TemplateBasedSurfaceRemesher::CheckInput()")
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
	auto PointSet3D_input = m_InputMesh.GetPointPosition(ALL);
	auto PointSet2D_input = m_TransfromedInputMesh.GetPointPosition(ALL);

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

			auto PointIndexList = this->Find3PointOfNearestFace(Pos2D, m_TransfromedInputMesh);
			//auto PointIndexList = this->FindNearestPoint(Pos2D, PointSet2D_input, 3);

			DenseVector<ScalarType, 3> Weight;
			Weight.Fill(0);
			{
				DenseVector<ScalarType, 3> Point0, Point1, Point2;
				PointSet2D_input.GetCol(PointIndexList[0], Point0);
				PointSet2D_input.GetCol(PointIndexList[1], Point1);
				PointSet2D_input.GetCol(PointIndexList[2], Point2);

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

			DenseVector<ScalarType, 3> Pos3D, Point0, Point1, Point2;
			PointSet3D_input.GetCol(PointIndexList[0], Point0);
			PointSet3D_input.GetCol(PointIndexList[1], Point1);
			PointSet3D_input.GetCol(PointIndexList[2], Point2);
			Pos3D = Weight[0] * Point0 + Weight[1] * Point1 + Weight[2] * Point2;

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
DenseVector<int_max> TemplateBasedSurfaceRemesher<ScalarType>::FindNearestPoint(const DenseVector<ScalarType, 3> Point, const DenseMatrix<ScalarType>& PointSet, int_max OutputPointCount)
{
	//---------------------------------------------------------------------------------------------
	DenseVector<ScalarType> DistanceList;
	DistanceList.Resize(PointSet.GetColCount());
	for (int_max k = 0; k < PointSet.GetColCount(); ++k)
	{
		DenseVector<ScalarType, 3> Pos_k;
		PointSet.GetCol(k, Pos_k);
		DistanceList[k] = (Point - Pos_k).L2Norm();
	}
	auto IndexList_sort = DistanceList.Sort("ascend");	
	return IndexList_sort.GetSubSet(0, OutputPointCount-1);
}


template<typename ScalarType>
DenseVector<int_max> TemplateBasedSurfaceRemesher<ScalarType>::Find3PointOfNearestFace(const DenseVector<ScalarType, 3> Point, const TriangleMesh<InputMeshAttribute>& TargetMesh)
{
	DenseVector<ScalarType> DistanceList;
	DistanceList.SetCapacity(TargetMesh.GetFaceCount());
	for (auto it = TargetMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Face().GetPointHandleList();
		ScalarType Distance = 0;
		for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
		{
			auto Pos_k = TargetMesh.Point(PointHandleList[k]).GetPosition();
			Distance += (Pos_k - Point).L2Norm();
		}
		DistanceList.Append(Distance);
	}

	auto FaceIndex_min = DistanceList.IndexOfMin();
	FaceHandleType FaceHandle_min;
	FaceHandle_min.SetIndex(FaceIndex_min);

	DenseVector<int_max> IndexList;
	IndexList.SetCapacity(3);
	auto PointHandleList = TargetMesh.Face(FaceHandle_min).GetPointHandleList();
	for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
	{
		IndexList.Append(PointHandleList[k].GetIndex());
	}
	return IndexList;
}


template<typename ScalarType>
ScalarType TemplateBasedSurfaceRemesher<ScalarType>::ComputeSignedTriangleAreaIn2D(const DenseVector<ScalarType, 3> PointA, const DenseVector<ScalarType, 3> PointB, const DenseVector<ScalarType, 3> PointC)
{
	auto x1 = PointA[0];
	auto y1 = PointA[1];
	auto x2 = PointB[0];
	auto y2 = PointB[1];
	auto x3 = PointC[0];
	auto y3 = PointC[1];

	auto Area = 0.5*(-x2*y1+x3*y1+x1*y2-x3*y2-x1*y3+x2*y3);
	return Area;
}

}//namespace mdk