#pragma once

namespace mdk
{
template<typename ScalarType>
TriangleMesh<ScalarType> ResampleMeshOpenBoundary(const TriangleMesh<ScalarType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList,
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
		if (InputMesh.Point(BounaryPointIndexList[k]).IsOnBoundaryEdge() == false)
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
	//modify the position of the start/end point
	OutputMesh.SetPointPosition(BounaryPointIndexList[0], Boundary_output(0, 0), Boundary_output(1, 0), Boundary_output(2, 0));
	OutputMesh.SetPointPosition(BounaryPointIndexList[BounaryPointIndexList.GetLength()-1], 
		                        Boundary_output(0, Boundary_output.GetColCount()-1), Boundary_output(1, Boundary_output.GetColCount() - 1), Boundary_output(2, Boundary_output.GetColCount() - 1));
	//add new boundary
	DenseVector<int_max> BounaryPointIndexList_output;
	BounaryPointIndexList_output.Append(BounaryPointIndexList[0]);
	for (int_max k = 1; k < Boundary_output.GetColCount() - 1; ++k)
	{
		auto PointIndex_new = OutputMesh.AddPoint(Boundary_output(0, k), Boundary_output(1, k), Boundary_output(2, k));
		BounaryPointIndexList_output.Append(PointIndex_new);
	}
	BounaryPointIndexList_output.Append(BounaryPointIndexList[BounaryPointIndexList.GetLength() - 1]);
	//divide Curve_near_boundary
	auto CurvePosition_near_boundary = OutputMesh.GetPointPosition(Curve_near_boundary);
	DenseVector<int_max> NearestPoint_on_Curve_near_boundary;
	//NearestPoint_on_Curve_near_boundary[k] is the nearest point of Boundary_output(:,k), the relative index on curve, 0~length(curve)-1
	NearestPoint_on_Curve_near_boundary.Append(0);
	for (int_max k = 1; k < Boundary_output.GetColCount() - 1; ++k)
	{
		auto Index_k = FindNearestPointOnCurve(CurvePosition_near_boundary, Boundary_output(0, k), Boundary_output(1, k), Boundary_output(2, k));
		auto Index_max = NearestPoint_on_Curve_near_boundary.Max();
		if (Index_k < Index_max)
		{
			Index_k = Index_max;
		}
		NearestPoint_on_Curve_near_boundary.Append(Index_k);
	}
	NearestPoint_on_Curve_near_boundary.Append(Curve_near_boundary.GetLength() - 1);
	//add new face
	DenseVector<int_max> BounaryFaceIndexList_output;
	for (int_max k = 0; k < BounaryPointIndexList_output.GetLength() - 1; ++k)
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
	//change point order of new face if necessary
	{ //Curve_near_boundary could also contain some boundary point
		bool Flag_reorder = false;
		int_max EdgeIndex_inner = -1;
		for (int_max k = 0; k < Curve_near_boundary.GetLength()-1; ++k)
		{
			auto EdgeIndex = OutputMesh.GetEdgeIndexByPoint(Curve_near_boundary[k], Curve_near_boundary[k+1]);
			if (OutputMesh.Edge(EdgeIndex).IsBoundary() == false)
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
TriangleMesh<ScalarType> ResampleMeshOpenBoundary(const TriangleMesh<ScalarType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList, 
	                                              const int_max PointCountOfBounary_output)
{
	//BounaryPointIndexList: an open bounary curve, BounaryPointIndexList[0] is the start point, BounaryPointIndexList[end] is the end point
	//position of the start/end point will not be changed
	//PointCountOfBounary_output is the target	
	
	if (PointCountOfBounary_output < 3)
	{
		MDK_Error("PointCountOfBounary_output < 3, abort @ ResampleMeshOpenBoundary(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}

	auto Boundary_input = InputMesh.GetPointPosition(BounaryPointIndexList);
	auto Boundary_output= ResampleOpen3DCurveByCardinalSpline(Boundary_input, PointCountOfBounary_output);
	return ResampleMeshOpenBoundary(InputMesh, BounaryPointIndexList, Boundary_output);
}

}//namespace mdk

