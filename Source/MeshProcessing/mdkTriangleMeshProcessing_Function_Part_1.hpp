#pragma once

namespace mdk
{
template<typename ScalarType>
TriangleMesh<ScalarType> ResampleMeshOpenBoundary_old(const TriangleMesh<ScalarType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList,
	                                                  const DenseMatrix<ScalarType>& Boundary_output)
{
	//BounaryPointIndexList: an open bounary curve, BounaryPointIndexList[0] is the start point, BounaryPointIndexList[end] is the end point
	//position of the start/end point will not be changed
	//Boundary_output is the target, Boundary_output(:,k) is a 3D point
	//Boundary_output should be very close to the orignial bounary on InputMesh

	//------------------- check input ----------------------------------------//
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT Clean, abort @ ResampleMeshOpenBoundary(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	if (BounaryPointIndexList.GetLength() < 3)
	{
		MDK_Error("BounaryPointIndexList.GetLength() < 3, abort @ ResampleMeshOpenBoundary(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	for (int_max k = 0; k < BounaryPointIndexList.GetLength(); ++k)
	{
		if (InputMesh.IsValidPointIndex(BounaryPointIndexList[k]) == false)
		{
			MDK_Error("BounaryPointIndexList is invalid, abort @ ResampleMeshOpenBoundary(...)")
			TriangleMesh<ScalarType> EmptyMesh;
			return EmptyMesh;
		}
		if (InputMesh.Point(BounaryPointIndexList[k]).IsOnPolygonMeshBoundary() == false)
		{
			MDK_Error("BounaryPointIndexList is invalid, abort @ ResampleMeshOpenBoundary(...)")
			TriangleMesh<ScalarType> EmptyMesh;
			return EmptyMesh;
		}
	}
	if (BounaryPointIndexList[0] == BounaryPointIndexList[BounaryPointIndexList.GetLength() - 1])
	{
		MDK_Error("BounaryPointIndexList is a closed curve, abort @ ResampleMeshOpenBoundary(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	if (Boundary_output.GetColCount() < 3)
	{
		MDK_Error("Boundary_output.GetColCount() < 3, abort @ ResampleMeshOpenBoundary(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}

	//------------------------------------------------------------------------//

	DenseVector<int_max> Curve_near_boundary;
	DenseVector<int_max> Face_delete;
	DenseVector<int_max> Edge_delete;
	DenseVector<int_max> Point_delete;
	// on InputMesh:
    // ~~~~~~~~~~~~~~~~~~ Curve_near_boundary
	// |\/|\/\/\/\|/\/\/| Face_delete, Edge_delete
	// ~~~~~~~~~~~~~~~~~~ Boundary, Edge_delete
	// Curve_near_boundary do NOT share start/end point with Boundary
	{//get Curve_near_boundary
		int_max PointIndex_start = -1;
		{
			auto tempList0 = InputMesh.Point(BounaryPointIndexList[0]).GetAdjacentPointIndexList();
			tempList0 = SetDiff(tempList0, BounaryPointIndexList);
			auto tempList1 = InputMesh.Point(BounaryPointIndexList[1]).GetAdjacentPointIndexList();			
			tempList1 = SetDiff(tempList1, BounaryPointIndexList);
			auto tempList2 = InputMesh.Point(BounaryPointIndexList[2]).GetAdjacentPointIndexList();
			tempList2 = SetDiff(tempList2, BounaryPointIndexList);
			auto tempList01 = Intersect(tempList0, tempList1);
			auto tempList02 = Intersect(tempList0, tempList2);
			if (tempList01.GetLength() == 1)
			{
				PointIndex_start = tempList01[0];
			}
			else if (tempList02.GetLength() == 1)
			{
				PointIndex_start = tempList02[0];
			}
			else
			{
				MDK_Error("special case: PointIndex_start not exist, please modify BounaryPointIndexList, abort @ ResampleMeshOpenBoundary(...)")
				DisplayVector("BounaryPointIndexList", BounaryPointIndexList);
				TriangleMesh<ScalarType> EmptyMesh;
				return EmptyMesh;
			}
		}
		int_max PointIndex_end = -1;
		{
			auto Index0 = BounaryPointIndexList.GetLength() - 1;
			auto Index1 = BounaryPointIndexList.GetLength() - 2;
			auto Index2 = BounaryPointIndexList.GetLength() - 3;
			auto tempList0 = InputMesh.Point(BounaryPointIndexList[Index0]).GetAdjacentPointIndexList();
			tempList0 = SetDiff(tempList0, BounaryPointIndexList);
			auto tempList1 = InputMesh.Point(BounaryPointIndexList[Index1]).GetAdjacentPointIndexList();
			tempList1 = SetDiff(tempList1, BounaryPointIndexList);
			auto tempList2 = InputMesh.Point(BounaryPointIndexList[Index2]).GetAdjacentPointIndexList();
			tempList2 = SetDiff(tempList2, BounaryPointIndexList);
			auto tempList01 = Intersect(tempList0, tempList1);	
			auto tempList02 = Intersect(tempList0, tempList2);
			if (tempList01.GetLength() == 1)
			{
				PointIndex_end = tempList01[0];
			}
			else if (tempList02.GetLength() == 1)
			{
				PointIndex_end = tempList02[0];
			}
			else
			{
				MDK_Error("special case: PointIndex_end not exist, please modify BounaryPointIndexList, abort @ ResampleMeshOpenBoundary(...)")
				TriangleMesh<ScalarType> EmptyMesh;
				return EmptyMesh;
			}
		}
		Curve_near_boundary.Append(PointIndex_start);
		int_max PointIndex_current = PointIndex_start;
		int_max while_counter = 0;
		auto PointCount_InputMesh = InputMesh.GetPointCount();
		while (true)
		{
			while_counter += 1;
			if (while_counter > PointCount_InputMesh)
			{
				MDK_Error("infinit while loop, please modify BounaryPointIndexList, abort @ ResampleMeshOpenBoundary(...)")
				std::cout << "PointIndex_start " << PointIndex_start << ", PointIndex_end " << PointIndex_end << '\n';
				DisplayVector("Curve_near_boundary", Curve_near_boundary);
				TriangleMesh<ScalarType> EmptyMesh;
				return EmptyMesh;
			}

			auto CandidateList = InputMesh.Point(PointIndex_current).GetAdjacentPointIndexList();
			CandidateList = SetDiff(CandidateList, BounaryPointIndexList);
			CandidateList = SetDiff(CandidateList, Curve_near_boundary);
			DenseVector<int_max> IntersectCounterList;
			IntersectCounterList.Resize(CandidateList.GetLength());
			IntersectCounterList.Fill(0);
			for (int_max k = 0; k < CandidateList.GetLength(); ++k)
			{
				auto tempList = InputMesh.Point(CandidateList[k]).GetAdjacentPointIndexList();
				auto tempInter = Intersect(tempList, BounaryPointIndexList);
				IntersectCounterList[k] = tempInter.GetLength();
			}
			auto Index_max_count = IntersectCounterList.IndexOfMax();
			if (IntersectCounterList[Index_max_count] > 0)
			{
				auto PointIndex_next = CandidateList[Index_max_count];
				Curve_near_boundary.Append(PointIndex_next);
				PointIndex_current = PointIndex_next;
			}
			else
			{
				MDK_Error("special case: IntersectCounterList[Index_max_count] = 0  please modify BounaryPointIndexList, abort @ ResampleMeshOpenBoundary(...)")
				TriangleMesh<ScalarType> EmptyMesh;
				return EmptyMesh;
			}

			//must be here
			if (PointIndex_current == PointIndex_end)
			{
				break;//while
			}
		}
	}
	{//get Face_delete
		for (int_max k = 1; k < BounaryPointIndexList.GetLength() - 1; ++k)
		{
			auto FaceIndexList = InputMesh.Point(BounaryPointIndexList[k]).GetAdjacentFaceIndexList();
			Face_delete.Append(FaceIndexList);
		}
		Face_delete = Face_delete.GetSubSet(Face_delete.FindUnique());
	}
	{//get Edge_delete
		for (int_max k = 1; k < BounaryPointIndexList.GetLength() - 1; ++k)
		{
			auto EdgeIndexList = InputMesh.Point(BounaryPointIndexList[k]).GetAdjacentEdgeIndexList();
			Edge_delete.Append(EdgeIndexList);
		}		
	}
	Point_delete = BounaryPointIndexList.GetSubSet(span(1, BounaryPointIndexList.GetLength() - 2));

	TriangleMesh<ScalarType> OutputMesh;
	OutputMesh = InputMesh;//copy
	OutputMesh.DeleteFace(Face_delete);
	OutputMesh.DeleteEdge(Edge_delete);
	OutputMesh.DeletePoint(Point_delete);
	//modify the position of the start/end point of BounaryPointIndexList to be the same as the start/end point of Boundary_output
	OutputMesh.SetPointPosition(BounaryPointIndexList[0], Boundary_output(0, 0), Boundary_output(1, 0), Boundary_output(2, 0));
	OutputMesh.SetPointPosition(BounaryPointIndexList[BounaryPointIndexList.GetLength()-1], 
		                        Boundary_output(0, Boundary_output.GetColCount()-1), Boundary_output(1, Boundary_output.GetColCount() - 1), Boundary_output(2, Boundary_output.GetColCount() - 1));
	//add new boundary (do not need to add start/end point)
	DenseVector<int_max> BounaryPointIndexList_output;
	BounaryPointIndexList_output.Append(BounaryPointIndexList[0]);
	for (int_max k = 1; k < Boundary_output.GetColCount() - 1; ++k)
	{
		auto PointIndex_new = OutputMesh.AddPoint(Boundary_output(0, k), Boundary_output(1, k), Boundary_output(2, k));
		BounaryPointIndexList_output.Append(PointIndex_new);
	}
	BounaryPointIndexList_output.Append(BounaryPointIndexList[BounaryPointIndexList.GetLength() - 1]);
	//divide Curve_near_boundary	
	DenseVector<int_max> NearestPoint_on_Curve_near_boundary;
	{//NearestPoint_on_Curve_near_boundary[k] is the nearest point to Boundary_output(:,k), the relative index on Curve_near_boundary, 0~length(curve)-1		
		auto ParamBoundary_output = ComputeCumulative3DCurveLengthList(Boundary_output);
		ParamBoundary_output /= ParamBoundary_output[ParamBoundary_output.GetLength() - 1];

		auto CurvePosition_near_boundary = OutputMesh.GetPointPosition(Curve_near_boundary);
		auto ParamCurvePosition_near_boundary = ComputeCumulative3DCurveLengthList(CurvePosition_near_boundary);
		ParamCurvePosition_near_boundary /= ParamCurvePosition_near_boundary[ParamCurvePosition_near_boundary.GetLength() - 1];

		NearestPoint_on_Curve_near_boundary.Append(0);
		for (int_max k = 1; k < ParamBoundary_output.GetLength() - 1; ++k)
		{
			DenseVector<ScalarType> tempDistList;
			tempDistList.SetCapacity(ParamCurvePosition_near_boundary.GetLength());
			auto Index_prev = NearestPoint_on_Curve_near_boundary[k - 1];
			for (int_max m = Index_prev; m < ParamCurvePosition_near_boundary.GetLength(); ++m)
			{
				tempDistList.Append((std::abs)(ParamBoundary_output[k] - ParamCurvePosition_near_boundary[m]));
			}
			auto Index_min = tempDistList.IndexOfMin();
			auto Index_nearest = Index_prev + Index_min;
			NearestPoint_on_Curve_near_boundary.Append(Index_nearest);
		}
		NearestPoint_on_Curve_near_boundary.Append(Curve_near_boundary.GetLength() - 1);

		//NearestPoint_on_Curve_near_boundary.Append(0);
		//for (int_max k = 1; k < Boundary_output.GetColCount() - 1; ++k)
		//{
		//	auto Index_k = FindNearestPointOnCurve(CurvePosition_near_boundary, Boundary_output(0, k), Boundary_output(1, k), Boundary_output(2, k));
		//	auto Index_max = NearestPoint_on_Curve_near_boundary.Max();
		//	if (Index_k < Index_max)
		//	{
		//		Index_k = Index_max;
		//	}
		//	NearestPoint_on_Curve_near_boundary.Append(Index_k);
		//}
		//NearestPoint_on_Curve_near_boundary.Append(Curve_near_boundary.GetLength() - 1);
	}

	//add new face
	// ~~~~~~~~~~~~~~~~~~ Curve_near_boundary
	// |\|     |      |/|
	// ~~~~~~~~~~~~~~~~~~ Boundary_output
	DenseVector<int_max> BounaryFaceIndexList_output;
	{
		auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[0], Curve_near_boundary[0], BounaryPointIndexList_output[1]);
		BounaryFaceIndexList_output.Append(FaceIndex_new);
	}
	for (int_max Index = 0; Index < NearestPoint_on_Curve_near_boundary[1]; ++Index)
	{
		auto PointIndexA = Curve_near_boundary[Index];
		auto PointIndexB = Curve_near_boundary[Index + 1];
		auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[1], PointIndexA, PointIndexB);
		BounaryFaceIndexList_output.Append(FaceIndex_new);
	}
	for (int_max k = 1; k < BounaryPointIndexList_output.GetLength() - 2; ++k)
	{
		int_max Index_middle = NearestPoint_on_Curve_near_boundary[k] + int_max((NearestPoint_on_Curve_near_boundary[k + 1]- NearestPoint_on_Curve_near_boundary[k])/2);
		if (Index_middle >= NearestPoint_on_Curve_near_boundary[k + 1])
		{
			Index_middle = NearestPoint_on_Curve_near_boundary[k];
		}		

		for (int_max Index = NearestPoint_on_Curve_near_boundary[k]; Index < Index_middle; ++Index)
		{
			auto PointIndexA = Curve_near_boundary[Index];
			auto PointIndexB = Curve_near_boundary[Index + 1];
			auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[k], PointIndexA, PointIndexB);
			BounaryFaceIndexList_output.Append(FaceIndex_new);
		}
		auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[k], Curve_near_boundary[Index_middle], BounaryPointIndexList_output[k+1]);
		BounaryFaceIndexList_output.Append(FaceIndex_new);
		for (int_max Index = Index_middle; Index < NearestPoint_on_Curve_near_boundary[k + 1]; ++Index)
		{
			auto PointIndexA = Curve_near_boundary[Index];
			auto PointIndexB = Curve_near_boundary[Index + 1];
			auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[k + 1], PointIndexA, PointIndexB);
			BounaryFaceIndexList_output.Append(FaceIndex_new);
		}		
	}
	for (int_max Index = NearestPoint_on_Curve_near_boundary[BounaryPointIndexList_output.GetLength()-2]; Index < NearestPoint_on_Curve_near_boundary[BounaryPointIndexList_output.GetLength()-1]; ++Index)
	{
		auto PointIndexA = Curve_near_boundary[Index];
		auto PointIndexB = Curve_near_boundary[Index + 1];
		auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[BounaryPointIndexList_output.GetLength()-2], PointIndexA, PointIndexB);
		BounaryFaceIndexList_output.Append(FaceIndex_new);
	}
	{
		auto tempIdx = BounaryPointIndexList_output.GetLength() - 2;
		auto FaceIndex_new = OutputMesh.AddFaceByPoint(BounaryPointIndexList_output[tempIdx], Curve_near_boundary[Curve_near_boundary.GetLength()-1], BounaryPointIndexList_output[tempIdx+1]);
		BounaryFaceIndexList_output.Append(FaceIndex_new);
	}

	//change point order of new face if necessary
	{ //Curve_near_boundary could also contain some boundary point
		bool Flag_reorder = false;
		int_max EdgeIndex_inner = -1;
		for (int_max k = 0; k < Curve_near_boundary.GetLength()-1; ++k)
		{
			auto EdgeIndex = OutputMesh.GetEdgeIndexByPoint(Curve_near_boundary[k], Curve_near_boundary[k+1]);
			if (OutputMesh.Edge(EdgeIndex).IsOnPolygonMeshBoundary() == false)
			{
				EdgeIndex_inner = EdgeIndex;
			}
		}
		if (EdgeIndex_inner < 0)
		{
			std::cout << "special case that Curve_near_boundary is bounary, do nothing for now" << '\n';
			Flag_reorder = false;
		}
		else
		{
			auto PointIndexList = OutputMesh.Edge(EdgeIndex_inner).GetPointIndexList();
			auto FaceIndexList = OutputMesh.Edge(EdgeIndex_inner).GetAdjacentFaceIndexList();
			//should be only two face
			auto PointIndexListA = OutputMesh.Face(FaceIndexList[0]).GetPointIndexList_LeadBy(PointIndexList[0]);
			auto PointIndexListB = OutputMesh.Face(FaceIndexList[1]).GetPointIndexList_LeadBy(PointIndexList[0]);
			if ((PointIndexListA[1] == PointIndexList[1] && PointIndexListB[1] != PointIndexList[1])
				||(PointIndexListA[1] != PointIndexList[1] && PointIndexListB[1] == PointIndexList[1]))
			{
				Flag_reorder = false;
			}
			else
			{
				Flag_reorder = true;
			}
		}
		if (Flag_reorder == true)
		{
			for (int_max k = 0; k < BounaryFaceIndexList_output.GetLength(); ++k)
			{
				OutputMesh.Face(BounaryFaceIndexList_output[k]).ReversePointOrder();
			}
		}
	}
	//clean garbage
	OutputMesh.CleanDataStructure();
	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> ResampleOpenBoundaryCurveOfSurface(const TriangleMesh<ScalarType>& Surface_input, const DenseVector<int_max>& BounaryPointIndexList_input, const DenseMatrix<ScalarType>& Boundary_output)
{
	//BounaryPointIndexList_input: an open bounary curve, BounaryPointIndexList_input[0] is the start point, BounaryPointIndexList_input[end] is the end point	
	//point order in BounaryPointIndexList_input must align with normal direction, so new face will have consistant normal direction
	//Boundary_output is the target, Boundary_output(:,k) is a 3D point
	//Boundary_output should be close to the orignial bounary on InputMesh

	//------------------- check input ----------------------------------------//
	if (Surface_input.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("Surface_input DataStructure is NOT Clean, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	if (BounaryPointIndexList_input.GetLength() < 3)
	{
		MDK_Error("BounaryPointIndexList_input.GetLength() < 3, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	for (int_max k = 0; k < BounaryPointIndexList_input.GetLength(); ++k)
	{
		if (Surface_input.IsValidPointIndex(BounaryPointIndexList_input[k]) == false)
		{
			MDK_Error("BounaryPointIndexList_input is invalid, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
			TriangleMesh<ScalarType> EmptyMesh;
			return EmptyMesh;
		}
		if (Surface_input.Point(BounaryPointIndexList_input[k]).IsOnPolygonMeshBoundary() == false)
		{
			MDK_Error("BounaryPointIndexList_input is invalid, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
			TriangleMesh<ScalarType> EmptyMesh;
			return EmptyMesh;
		}
	}
	if (BounaryPointIndexList_input[0] == BounaryPointIndexList_input[BounaryPointIndexList_input.GetLength() - 1])
	{
		MDK_Error("BounaryPointIndexList is a closed curve, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	if (Boundary_output.GetColCount() < 3)
	{
		MDK_Error("Boundary_output.GetColCount() < 3, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
	//-------------------- input check done ------------------------------------//

	auto Boundary_input = Surface_input.GetPointPosition(BounaryPointIndexList_input);
	auto ParamBoundary_input = ComputeCumulative3DCurveLengthList(Boundary_input);
	ParamBoundary_input /= ParamBoundary_input[ParamBoundary_input.GetLength() - 1];

	auto ParamBoundary_output = ComputeCumulative3DCurveLengthList(Boundary_output);
	ParamBoundary_output /= ParamBoundary_output[ParamBoundary_output.GetLength() - 1];

	auto Surface_output = Surface_input;
	
	//start and end point of input bounary and output bounary share the same index
	{
		DenseVector<ScalarType, 3> Pos_start, Pos_end;
		Boundary_output.GetCol(0, Pos_start);
		Boundary_output.GetCol(Boundary_output.GetColCount()-1, Pos_end);
		Surface_output.SetPointPosition(BounaryPointIndexList_input[0], Pos_start);
		Surface_output.SetPointPosition(BounaryPointIndexList_input[BounaryPointIndexList_input.GetLength()-1], Pos_end);
	}

	DenseVector<DenseVector<int_max>> PointIndexList_PerInputEdge;
	PointIndexList_PerInputEdge.Resize(BounaryPointIndexList_input.GetLength() - 1);
	DenseVector<int_max> BounaryPointIndexList_input_output;
	BounaryPointIndexList_input_output.SetCapacity(Boundary_output.GetColCount()+ BounaryPointIndexList_input.GetLength());
	DenseVector<int_max> BounaryPointFlagList_input_output;//0: input, 1: output (including start end point)
	BounaryPointFlagList_input_output.SetCapacity(Boundary_output.GetColCount() + BounaryPointIndexList_input.GetLength());	
	DenseVector<int_max> BounaryPointIndexList_output;
	BounaryPointIndexList_output.SetCapacity(Boundary_output.GetColCount());
	BounaryPointIndexList_output.Append(BounaryPointIndexList_input[0]);//share the same index for the start point
	for (int_max k = 0; k < BounaryPointIndexList_input.GetLength() - 1; ++k)
	{
		PointIndexList_PerInputEdge[k].Append(BounaryPointIndexList_input[k]);
		BounaryPointIndexList_input_output.Append(BounaryPointIndexList_input[k]);
		BounaryPointFlagList_input_output.Append(0);
		for (int_max n = 1; n < ParamBoundary_output.GetLength() - 1; ++n)
		{
			if (ParamBoundary_output[n] >= ParamBoundary_input[k] && ParamBoundary_output[n] < ParamBoundary_input[k + 1])
			{
				auto PointIndex_new = Surface_output.AddPoint(Boundary_output(0, n), Boundary_output(1, n), Boundary_output(2, n));
				PointIndexList_PerInputEdge[k].Append(PointIndex_new);
				BounaryPointIndexList_output.Append(PointIndex_new);
				BounaryPointIndexList_input_output.Append(PointIndex_new);
				BounaryPointFlagList_input_output.Append(1);
			}
		}
		PointIndexList_PerInputEdge[k].Append(BounaryPointIndexList_input[k + 1]);
	}
	BounaryPointIndexList_output.Append(BounaryPointIndexList_input[BounaryPointIndexList_input.GetLength()-1]);//share the same index for the end point
	BounaryPointIndexList_input_output.Append(BounaryPointIndexList_input[BounaryPointIndexList_input.GetLength() - 1]);
	BounaryPointFlagList_input_output.Append(1);
	BounaryPointFlagList_input_output[0] = 1;

	DenseVector<DenseVector<int_max>> PointIndexList_PerOutputEdge;
	{
		int_max Index_a = 0;
		int_max Index_b = 0;
		while (true)
		{
			for (int_max k = Index_a + 1; k < BounaryPointFlagList_input_output.GetLength(); ++k)
			{
				if (BounaryPointFlagList_input_output[k] == 1)
				{
					Index_b = k;
					break;
				}
			}
			DenseVector<int_max> tempPointIndexList;
			tempPointIndexList.Append(BounaryPointIndexList_input_output[Index_a]);
			for (int_max k = Index_a + 1; k < Index_b; ++k)
			{
				tempPointIndexList.Append(BounaryPointIndexList_input_output[k]);
			}
			tempPointIndexList.Append(BounaryPointIndexList_input_output[Index_b]);
			PointIndexList_PerOutputEdge.Append(tempPointIndexList);
			if (Index_b == BounaryPointIndexList_input_output.GetLength() - 1)
			{
				break;
			}
			else if (Index_b == Index_a)
			{
				MDK_Error("Somthing is wrong, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
				TriangleMesh<ScalarType> EmptyMesh;
				return EmptyMesh;
			}
			Index_a = Index_b;
		}		
	}

	//split input edge and add new face
	for (int_max k = 0; k < PointIndexList_PerInputEdge.GetLength(); ++k)
	{   //--------------
		//    2 
		// /    \
		// 0-----1		
		//-------------
		if (PointIndexList_PerInputEdge[k].GetLength() > 2)
		{
			auto PointIndexList_k = PointIndexList_PerInputEdge[k];
			auto H0 = PointIndexList_k[0];
			auto H1 = PointIndexList_k[PointIndexList_k.GetLength() - 1];
			auto EdgeIndex01 = Surface_output.GetEdgeIndexByPoint(H0, H1);
			auto tempFaceIndexList = Surface_output.Edge(EdgeIndex01).GetAdjacentFaceIndexList();
			auto FaceIndex_k = tempFaceIndexList[0];
			auto PointIndexListOfFace = Surface_output.Face(FaceIndex_k).GetPointIndexList();//only one face because it is an edge
			int_max H2 = -1;
			if (PointIndexListOfFace[0] != H0 && PointIndexListOfFace[0] != H1)
			{
				H2 = PointIndexListOfFace[0];
			}
			else if (PointIndexListOfFace[1] != H0 && PointIndexListOfFace[1] != H1)
			{
				H2 = PointIndexListOfFace[1];
			}
			else
			{
				H2 = PointIndexListOfFace[2];
			}

			//debug
			if (Surface_output.Edge(EdgeIndex01).IsOnPolygonMeshBoundary() == false)
			{
				std::cout << "EdgeIndex01=" << EdgeIndex01 << ", H0=" << H0 << ", H1=" << H1 << ", H2=" << H2 << '\n';
			}

			Surface_output.DeleteFace(FaceIndex_k);
			Surface_output.DeleteEdge(EdgeIndex01);
			//add new face
			for (int_max n = 0; n < PointIndexList_k.GetLength() - 1; ++n)
			{
				Surface_output.AddFaceByPoint(PointIndexList_k[n], PointIndexList_k[n + 1], H2);
			}
		}
	}

	//debug
	std::cout << "collapse edge @ ResampleOpenBoundaryCurveOfSurface" << '\n';

	//collapse edge
	for (int_max k = 0; k < PointIndexList_PerOutputEdge.GetLength(); ++k)
	{
		if (PointIndexList_PerOutputEdge[k].GetLength() > 2)
		{
			auto PointIndexList_k = PointIndexList_PerOutputEdge[k];
			auto H0 = PointIndexList_k[0];
			for (int_max n = 1; n < PointIndexList_k.GetLength() - 1; ++n)
			{
				auto Hn = PointIndexList_k[n];
				auto EdgeIndex_n0 = Surface_output.GetEdgeIndexByPoint(Hn, H0);
				Surface_output.CollapseEdge(EdgeIndex_n0, H0);
			}
		}
	}	
    //clean and output
	Surface_output.CleanDataStructure();
	return Surface_output;
}

template<typename ScalarType>
TriangleMesh<ScalarType> ResampleOpenBoundaryCurveOfSurface(const TriangleMesh<ScalarType>& Surface_input, const DenseVector<int_max>& BounaryPointIndexList_input, const int_max PointCountOfBounary_output)
{
	//BounaryPointIndexList: an open bounary curve, BounaryPointIndexList[0] is the start point, BounaryPointIndexList[end] is the end point
	//position of the start/end point will not be changed
	//PointCountOfBounary_output is the target	
	
	if (PointCountOfBounary_output < 3)
	{
		MDK_Error("PointCountOfBounary_output < 3, abort @ ResampleOpenBoundaryCurveOfSurface(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}

	auto Boundary_input = Surface_input.GetPointPosition(BounaryPointIndexList_input);
	auto Boundary_output= ResampleOpen3DCurveByCardinalSpline(Boundary_input, PointCountOfBounary_output);
	return ResampleOpenBoundaryCurveOfSurface(Surface_input, BounaryPointIndexList_input, Boundary_output);
}


template<typename ScalarType>
void ProjectPointToSurface(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& Surface, DenseVector<ScalarType, 3>& Point_proj, int_max& FaceIndex_proj)
{
	if (Surface.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("DataStructure of input mesh is not clean, abort @ ProjectPointToSurface(...)")
		FaceIndex_proj = -1;
		return;
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(Surface);
	//https://www.vtk.org/Wiki/VTK/Examples/Cxx/PolyData/CellLocator
	auto CellLocator = vtkSmartPointer<vtkCellLocator>::New();
	CellLocator->SetDataSet(VTKMesh);
	CellLocator->BuildLocator();
	double testPoint[3] = { double(Point[0]), double(Point[1]), double(Point[2]) };
	double closestPoint[3];//the coordinates of the closest point will be returned here
	double closestPointDist2; //the squared distance to the closest point will be returned here
	vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
	int subId; //this is rarely used (in triangle strips only, I believe)
	CellLocator->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);
	//output
	Point_proj[0] = ScalarType(closestPoint[0]);
	Point_proj[1] = ScalarType(closestPoint[1]);
	Point_proj[2] = ScalarType(closestPoint[2]);
	FaceIndex_proj = int_max(cellId);
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const DenseVector<ScalarType, 3>& Point, const TriangleMesh<ScalarType>& Surface)
{
	DenseVector<ScalarType, 3> Point_proj;
	int_max FaceIndex_proj;
	ProjectPointToSurface(Point, Surface, Point_proj, FaceIndex_proj);
	return Point_proj;
}


template<typename ScalarType>
void ProjectPointToSurface(const DenseMatrix<ScalarType>& PointSet, const TriangleMesh<ScalarType>& Surface, DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj)
{
	if (Surface.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("DataStructure of input mesh is not clean, abort @ ProjectPointToSurface(...)")
		return;
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(Surface);
	//https://www.vtk.org/Wiki/VTK/Examples/Cxx/PolyData/CellLocator
	auto CellLocator = vtkSmartPointer<vtkCellLocator>::New();
	CellLocator->SetDataSet(VTKMesh);
	CellLocator->BuildLocator();

	PointSet_proj.Resize(PointSet.GetSize());
	PointSet_proj.Fill(0);
	FaceIndexList_proj.Resize(PointSet.GetColCount());
	FaceIndexList_proj.Fill(-1);

	for (int_max k = 0; k < PointSet.GetColCount(); ++k)
	{
		DenseVector<ScalarType, 3> Point;
		PointSet.GetCol(k, Point);
		double testPoint[3] = { double(Point[0]), double(Point[1]), double(Point[2]) };
		double closestPoint[3];//the coordinates of the closest point will be returned here
		double closestPointDist2; //the squared distance to the closest point will be returned here
		vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
		int subId; //this is rarely used (in triangle strips only, I believe)
		CellLocator->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);
		//output
		DenseVector<ScalarType, 3> Point_proj;
		Point_proj[0] = ScalarType(closestPoint[0]);
		Point_proj[1] = ScalarType(closestPoint[1]);
		Point_proj[2] = ScalarType(closestPoint[2]);
		PointSet_proj.SetCol(k, Point_proj);
		FaceIndexList_proj[k]= int_max(cellId);
	}
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const DenseMatrix<ScalarType>& PointSet, const TriangleMesh<ScalarType>& Surface)
{
	DenseMatrix<ScalarType> PointSet_proj;
	DenseVector<int_max> FaceIndexList_proj;
	ProjectPointToSurface(PointSet, Surface, PointSet_proj, FaceIndexList_proj);
	return PointSet_proj;
}


template<typename ScalarType>
int_max Project_Add_Point_to_Surface(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point)
{	
	int_max FaceIndex_proj;
	DenseVector<ScalarType, 3> Point_proj;
	ProjectPointToSurface(Point, Surface, Point_proj, FaceIndex_proj);

	if (FaceIndex_proj == -1)
	{
		return -1;
	}

	auto PointIndexList = Surface.Face(FaceIndex_proj).GetPointIndexList();
	//check if no need to add new point
	{
		DenseVector<ScalarType, 3> DistToPoint;
		for (int_max n = 0; n < 3; ++n)
		{
			auto Pn = Surface.GetPointPosition(PointIndexList[n]);
			DistToPoint[n] = (Pn - Point).L2Norm();
		}

		ScalarType DistThreshold = 1e-6;
		DenseVector<ScalarType, 3> EdgeLength;
		for (int_max n = 0; n < 3; ++n)
		{
			DenseVector<ScalarType, 3> Pos1, Pos2;
			Pos1 = Surface.GetPointPosition(PointIndexList[n]);
			if (n < PointIndexList.GetLength() - 1)
			{
				Pos1 = Surface.GetPointPosition(PointIndexList[n + 1]);
			}
			else
			{
				Pos1 = Surface.GetPointPosition(PointIndexList[0]);
			}
			EdgeLength[n] = (Pos1 - Pos2).L2Norm();
		}
		DistThreshold = EdgeLength.Min() / 10;
	
		auto dist_min = DistToPoint.Min();
		if (dist_min < DistThreshold)
		{
			auto PointIndex_ref = PointIndexList[DistToPoint.IndexOfMin()];
			Surface.SetPointPosition(PointIndex_ref, Point);
			return PointIndex_ref;
		}
	}

	// add new point -------------------------------------------------------
	auto H0 = PointIndexList[0];
	auto H1 = PointIndexList[1];
	auto H2 = PointIndexList[2];
	auto H3 = Surface.AddPoint(Point);
	//-----------------
	//     2 
	//     3      
	// 0       1
	//-----------------		

	//check if Point_proj is on an edge or inside the triangle face (FaceIndex_proj)

	auto P0 = Surface.GetPointPosition(H0);
	auto P1 = Surface.GetPointPosition(H1);
	auto P2 = Surface.GetPointPosition(H2);
	auto P3 = Surface.GetPointPosition(H3);
	auto FaceNormal = ComputeTriangleNormalIn3D(P0, P1, P2);
	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	/*
	for (int_max n = 0; n < 3; ++n)
	{
		int_max Ha, Hb, Hc;
		DenseVector<ScalarType, 3> Pa, Pb, Pc;
		if (n == 0)
		{	//---------
			//    2c 
			// 0a   1b
			//---------
			Pa = P0; Ha = H0;
			Pb = P1; Hb = H1;
			Pc = P2; Hc = H2;
		}
		else if (n == 1)
		{   //----------
			//    0c 			     
			// 1a    2b
			//----------
			Pa = P1; Ha = H1;
			Pb = P2; Hb = H2;
			Pc = P0; Hc = H0;
		}
		else
		{   //----------
		    //    1c 
		    // 2a    0b
		    //----------
			Pa = P2; Ha = H2;
			Pb = P0; Hb = H0;
			Pc = P1; Hc = H1;
		}

		//check if Point_proj is on edge_ab
		auto Disp_ab = Pb - Pa;
		auto Direction = ComputeVectorCrossProductIn3D(FaceNormal, Disp_ab);
		Direction /= (Direction.L2Norm() + EPS);
		auto Disp_ac = Pc - Pa;
		auto Disp_a3 = P3 - Pa;
		auto prod_ac = ComputeVectorDotProductIn3D(Direction, Disp_ac);
		auto prod_a3 = ComputeVectorDotProductIn3D(Direction, Disp_a3);
		if (std::abs(prod_a3) <= 0.1*std::abs(prod_ac))
		{// on edge_ab
			auto EdgeIndex_ab = Surface.GetEdgeIndexByPoint(Ha, Hb);
			auto FaceIndexList_adj = Surface.Edge(EdgeIndex_ab).GetAdjacentFaceIndexList();
			if (FaceIndexList_adj.GetLength() == 2)
			{  //-----------------
			   //     c 
			   //           
			   // a   3    b
			   //
			   //     d
			   //-----------------	
				int_max FaceIndex_adb = -1;
				if (FaceIndexList_adj[0] == FaceIndex_proj)
				{
					FaceIndex_adb = FaceIndexList_adj[1];
				}
				else
				{
					FaceIndex_adb = FaceIndexList_adj[0];
				}
				auto PointIndexList_adb = Surface.Face(FaceIndex_adb).GetPointIndexList();
				auto tempHd = SetDiff(PointIndexList_adb, Surface.Edge(EdgeIndex_ab).GetPointIndexList());
				auto Hd = tempHd[0];
				Surface.DeleteFace(FaceIndexList_adj);
				Surface.DeleteEdge(EdgeIndex_ab);
				Surface.AddFaceByPoint(H3, Hc, Ha);
				Surface.AddFaceByPoint(H3, Hb, Hc);
				Surface.AddFaceByPoint(H3, Ha, Hd);
				Surface.AddFaceByPoint(H3, Hd, Hb);
			}
			else if(FaceIndexList_adj.GetLength() == 1)
			{	//-----------------
				//     c 
				//           
				// a   3    b
				//-----------------	
				Surface.DeleteFace(FaceIndexList_adj);
				Surface.DeleteEdge(EdgeIndex_ab);
				Surface.AddFaceByPoint(H3, Hc, Ha);
				Surface.AddFaceByPoint(H3, Hb, Hc);
			}
			else
			{
				MDK_Error("Input is not TriangleMesh @ Project_Add_Point_to_Surface(...)")				
			}
			return H3;
		}
	}
	*/
	{// inside
		//-----------------
		//     2 
		//     3      
		// 0       1
		//-----------------		
		Surface.DeleteFace(FaceIndex_proj);
		Surface.AddFaceByPoint({ H3, H2, H0 });
		Surface.AddFaceByPoint({ H3, H0, H1 });
		Surface.AddFaceByPoint({ H3, H1, H2 });
		return H3;
	}
}


}//namespace mdk

