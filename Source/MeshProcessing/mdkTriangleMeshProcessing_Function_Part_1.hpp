#pragma once

namespace mdk
{

template<typename ScalarType>
DenseVector<int_max> ResampleOpenCurveOfSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<int_max>& CurvePointIndexList_input, const DenseMatrix<ScalarType>& Curve_output)
{
	//CurvePointIndexList_input: an open boundary curve on Surface, CurvePointIndexList_input[0] is the start point, CurvePointIndexList_input[end] is the end point	
	//point order in CurvePointIndexList_input must align with normal direction, so new face will have consistant normal direction
	//Curve_output is the target, Curve_output(:,k) is a 3D point
	//Curve_output should be close to the orignial bounary on InputMesh

	DenseVector<int_max> CurvePointIndexList_output;
	//------------------- check input ----------------------------------------//
	if (CurvePointIndexList_input.GetLength() < 2)
	{
		MDK_Error("CurvePointIndexList_input.GetLength() < 2, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")		
		return CurvePointIndexList_output;
	}
	for (int_max k = 0; k < CurvePointIndexList_input.GetLength(); ++k)
	{
		if (Surface.IsValidPointIndex(CurvePointIndexList_input[k]) == false)
		{
			MDK_Error("CurvePointIndexList_input is invalid, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")			
			return CurvePointIndexList_output;
		}
	}
	if (CurvePointIndexList_input[0] == CurvePointIndexList_input[CurvePointIndexList_input.GetLength() - 1])
	{
		MDK_Error("CurvePointIndexList_input is a closed curve, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")		
		return CurvePointIndexList_output;
	}
	if (Curve_output.GetColCount() < 2)
	{
		MDK_Error("Curve_output.GetColCount() < 2, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")		
		return CurvePointIndexList_output;
	}
	//-------------------- input check done ------------------------------------//

	//---------- handle a simple case : 2 points ----------------------------------------//
	if ((CurvePointIndexList_input.GetLength() == 2) && (Curve_output.GetColCount() == 2))
	{
		DenseVector<ScalarType, 3> Pos_start, Pos_end;
		Curve_output.GetCol(0, Pos_start);
		Curve_output.GetCol(1, Pos_end);
		Surface.SetPointPosition(CurvePointIndexList_input[0], Pos_start);
		Surface.SetPointPosition(CurvePointIndexList_input[1], Pos_end);
		return CurvePointIndexList_output;
	}
	//--------------------------- the simple case is done ------------------------------//

	auto Curve_input = Surface.GetPointPosition(CurvePointIndexList_input);
	auto ParamCurve_input = ComputeCumulative3DCurveLengthList(Curve_input);
	ParamCurve_input /= ParamCurve_input[ParamCurve_input.GetLength() - 1];

	auto ParamCurve_output = ComputeCumulative3DCurveLengthList(Curve_output);
	ParamCurve_output /= ParamCurve_output[ParamCurve_output.GetLength() - 1];
	
	//start/end point of input boundary and output boundary share the same index
	{
		DenseVector<ScalarType, 3> Pos_start, Pos_end;
		Curve_output.GetCol(0, Pos_start);
		Curve_output.GetCol(Curve_output.GetColCount()-1, Pos_end);
		Surface.SetPointPosition(CurvePointIndexList_input[0], Pos_start);
		Surface.SetPointPosition(CurvePointIndexList_input[CurvePointIndexList_input.GetLength()-1], Pos_end);
	}

	DenseVector<DenseVector<int_max>> PointIndexList_PerInputEdge;
	PointIndexList_PerInputEdge.Resize(CurvePointIndexList_input.GetLength() - 1);
	//
	DenseVector<int_max> CurvePointIndexList_input_output;
	CurvePointIndexList_input_output.SetCapacity(Curve_output.GetColCount()+ CurvePointIndexList_input.GetLength());
	//
	DenseVector<int_max> CurvePointFlagList_input_output;//0: input, 1: output (including start end point)
	CurvePointFlagList_input_output.SetCapacity(Curve_output.GetColCount() + CurvePointIndexList_input.GetLength());	
	//
	CurvePointIndexList_output.SetCapacity(Curve_output.GetColCount());
	CurvePointIndexList_output.Append(CurvePointIndexList_input[0]);//share the same index for the start point
	for (int_max k = 0; k < CurvePointIndexList_input.GetLength() - 1; ++k)
	{
		PointIndexList_PerInputEdge[k].Append(CurvePointIndexList_input[k]);
		CurvePointIndexList_input_output.Append(CurvePointIndexList_input[k]);
		CurvePointFlagList_input_output.Append(0);
		for (int_max n = 1; n < ParamCurve_output.GetLength() - 1; ++n)
		{
			if (ParamCurve_output[n] >= ParamCurve_input[k] && ParamCurve_output[n] < ParamCurve_input[k + 1])
			{
				auto PointIndex_new = Surface.AddPoint(Curve_output(0, n), Curve_output(1, n), Curve_output(2, n));
				PointIndexList_PerInputEdge[k].Append(PointIndex_new);
				CurvePointIndexList_output.Append(PointIndex_new);
				CurvePointIndexList_input_output.Append(PointIndex_new);
				CurvePointFlagList_input_output.Append(1);
			}
		}
		PointIndexList_PerInputEdge[k].Append(CurvePointIndexList_input[k + 1]);
	}
	CurvePointIndexList_output.Append(CurvePointIndexList_input[CurvePointIndexList_input.GetLength()-1]);//share the same index for the end point
	CurvePointIndexList_input_output.Append(CurvePointIndexList_input[CurvePointIndexList_input.GetLength() - 1]);
	CurvePointFlagList_input_output.Append(1);
	CurvePointFlagList_input_output[0] = 1;

	DenseVector<DenseVector<int_max>> PointIndexList_PerOutputEdge;
	{
		int_max Index_a = 0;
		int_max Index_b = 0;
		while (true)
		{
			for (int_max k = Index_a + 1; k < CurvePointFlagList_input_output.GetLength(); ++k)
			{
				if (CurvePointFlagList_input_output[k] == 1)
				{
					Index_b = k;
					break;
				}
			}
			DenseVector<int_max> tempPointIndexList;
			tempPointIndexList.Append(CurvePointIndexList_input_output[Index_a]);
			for (int_max k = Index_a + 1; k < Index_b; ++k)
			{
				tempPointIndexList.Append(CurvePointIndexList_input_output[k]);
			}
			tempPointIndexList.Append(CurvePointIndexList_input_output[Index_b]);
			PointIndexList_PerOutputEdge.Append(tempPointIndexList);
			if (Index_b == CurvePointIndexList_input_output.GetLength() - 1)
			{
				break;
			}
			else if (Index_b == Index_a)
			{
				MDK_Error("Somthing is wrong, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")				
				return CurvePointIndexList_output;
			}
			Index_a = Index_b;
		}		
	}

	//split input edge and add new face
	for (int_max k = 0; k < PointIndexList_PerInputEdge.GetLength(); ++k)
	{   //----------------------
		//InputEdge01
		//----------------------
		//    2            2
		//  /   \    or  /   \
		// 0-----1	    0-----1   
		//               \   /
		//                 3
		//---------------------
		if (PointIndexList_PerInputEdge[k].GetLength() > 2)
		{
			auto PointIndexList_k = PointIndexList_PerInputEdge[k];
			auto H0 = PointIndexList_k[0];
			auto H1 = PointIndexList_k[PointIndexList_k.GetLength() - 1];
			auto EdgeIndex01 = Surface.GetEdgeIndexByPoint(H0, H1);
			auto tempFaceIndexList = Surface.Edge(EdgeIndex01).GetAdjacentFaceIndexList();
			for (int_max m = 0; m < tempFaceIndexList.GetLength(); ++m)
			{
				auto FaceIndex_km = tempFaceIndexList[m];
				auto PointIndexListOfFace = Surface.Face(FaceIndex_km).GetPointIndexList_LeadBy(H0);
				int_max H2 = -1;//H2 or H3
				if (PointIndexListOfFace[1] != H1)
				{
					H2 = PointIndexListOfFace[1];
				}
				else
				{
					H2 = PointIndexListOfFace[2];
				}
				Surface.DeleteFace(FaceIndex_km);
				//add new face
				for (int_max n = 0; n < PointIndexList_k.GetLength() - 1; ++n)
				{
					if (PointIndexListOfFace[1] == H1)
					{
						Surface.AddFaceByPoint(PointIndexList_k[n], PointIndexList_k[n + 1], H2);
					}
					else
					{
						Surface.AddFaceByPoint(PointIndexList_k[n + 1], PointIndexList_k[n], H2);
					}
				}
			}
			Surface.DeleteEdge(EdgeIndex01);			
		}
	}

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
				auto EdgeIndex_n0 = Surface.GetEdgeIndexByPoint(Hn, H0);
				auto Flag = Surface.CollapseEdge(EdgeIndex_n0, H0, true, true);
				if (Flag == false)
				{
					MDK_Error("Please modify the mesh to remove special case, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")
					return CurvePointIndexList_output;
				}
			}
		}
	}

	return CurvePointIndexList_output;
}


template<typename ScalarType>
DenseVector<int_max> ResampleOpenCurveOfSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<int_max>& CurvePointIndexList_input, const int_max PointCountOfCurve_output)
{	//CurvePointIndexList_input: an open curve, CurvePointIndexList_input[0] is the start point, CurvePointIndexList_input[end] is the end point
	//position of the start/end point will not be changed
	//PointCountOfCurve_output is the target

	auto PointCountOfCurve_input = CurvePointIndexList_input.GetLength();
	if (PointCountOfCurve_input < 2)
	{
		MDK_Error("PointCountOfCurve_input < 2, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...) ")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	if (PointCountOfCurve_output < 2)
	{
		MDK_Error("PointCountOfCurve_output < 2, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...) ")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	if (PointCountOfCurve_output == 2)
	{
		auto Curve_output = Surface.GetPointPosition(CurvePointIndexList_input);
		return ResampleOpenCurveOfSurface(Surface, CurvePointIndexList_input, Curve_output);
	}
	else
	{
		auto Curve_input = Surface.GetPointPosition(CurvePointIndexList_input);
		auto Curve_output = ResampleOpen3DCurveByCardinalSpline(Curve_input, PointCountOfCurve_output);
		return ResampleOpenCurveOfSurface(Surface, CurvePointIndexList_input, Curve_output);
	}
}


template<typename ScalarType>
int_max ProjectPoint_AddProjectedPoint_ToSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, ScalarType DistanceThreshold)
{	
	DenseMatrix<ScalarType> PointSet;
	PointSet.AppendCol(Point);
	auto PointIndexList_proj = ProjectPoint_AddProjectedPoint_ToSurface(Surface, PointSet, DistanceThreshold);
	return PointIndexList_proj[0];	
}


template<typename ScalarType>
DenseVector<int_max> ProjectPoint_AddProjectedPoint_ToSurface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, ScalarType DistanceThreshold)
{
	DenseVector<int_max> PointIndexList_proj;//output
	DenseVector<int_max> FaceIndexList_proj;
	DenseMatrix<ScalarType> PointSet_proj;
	ProjectPointToFaceByVTKCellLocator(Surface, PointSet, PointSet_proj, FaceIndexList_proj);
	for (int_max Index = 0; Index < FaceIndexList_proj.GetLength(); ++Index)
	{
		auto FaceIndex_proj = FaceIndexList_proj[Index];
		auto PointIndexList = Surface.Face(FaceIndex_proj).GetPointIndexList();
		DenseVector<ScalarType, 3> Point_proj, Point_nearest;
		int_max PointIndex_nearest = -1;
		{
			PointSet_proj.GetCol(Index, Point_proj);			
			DenseVector<ScalarType, 3> DistToPoint;
			for (int_max n = 0; n < 3; ++n)
			{
				auto Pn = Surface.GetPointPosition(PointIndexList[n]);
				DistToPoint[n] = (Pn - Point_proj).L2Norm();
			}
			auto Idx_min = DistToPoint.IndexOfMin();
			PointIndex_nearest = PointIndexList[Idx_min];
			Point_nearest = Surface.GetPointPosition(PointIndex_nearest);
		}		
		bool Flag_AddNewPoint = false;
		{
			auto Distance = (Point_proj - Point_nearest).L2Norm();
			if (Distance > DistanceThreshold)
			{
				Flag_AddNewPoint = true;
			}
		}		
		if (Flag_AddNewPoint == false)
		{
			PointIndexList_proj.Append(PointIndex_nearest);
		}
		else
		{
			auto H0 = PointIndexList[0];
			auto H1 = PointIndexList[1];
			auto H2 = PointIndexList[2];
			auto H3 = Surface.AddPoint(Point_proj);
			PointIndexList_proj.Append(H3);
			//-----------------
			//     2 
			//     3      
			// 0       1
			//-----------------	
			//check if Point_proj is on some edge
			auto EdgeIndexList = Surface.Face(FaceIndex_proj).GetEdgeIndexList();
			DenseVector<ScalarType> CosAngleList;
			CosAngleList.Resize(EdgeIndexList.GetLength());
			for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
			{
				auto PointIndex_A_B = Surface.Edge(EdgeIndexList[k]).GetPointIndexList();
				auto PosA = Surface.GetPointPosition(PointIndex_A_B[0]);
				auto PosB = Surface.GetPointPosition(PointIndex_A_B[1]);
				auto VectorA = PosA - Point_proj;
				VectorA /= VectorA.L2Norm();
				auto VectorB = PosB - Point_proj;
				VectorB /= VectorB.L2Norm();
				CosAngleList[k] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
			}
			auto Idx_min = CosAngleList.IndexOfMin();
			if (CosAngleList[Idx_min] < -0.95) // cos(162)=-0.95
			{//on edge
				Surface.SplitFaceAtEdge(EdgeIndexList[Idx_min], H3);
			}
			else
			{//now, the input point is not on any edge
				Surface.DeleteFace(FaceIndex_proj);
				Surface.AddFaceByPoint({ H3, H2, H0 });
				Surface.AddFaceByPoint({ H3, H0, H1 });
				Surface.AddFaceByPoint({ H3, H1, H2 });
			}
		}
	}
	return PointIndexList_proj;
}


template<typename ScalarType>
int_max AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, ScalarType DistanceThreshold)
{
	auto PointIndex = ProjectPoint_AddProjectedPoint_ToSurface(Surface, Point, DistanceThreshold);
	Surface.SetPointPosition(PointIndex, Point);
	return PointIndex;
}


template<typename ScalarType>
DenseVector<int_max> AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, ScalarType DistanceThreshold)
{
	auto PointIndexList = ProjectPoint_AddProjectedPoint_ToSurface(Surface, PointSet, DistanceThreshold);
	Surface.SetPointPosition(PointIndexList, PointSet);
	return PointIndexList;
}


template<typename ScalarType>
DenseVector<int_max> AddPolyLineOnSurface(TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PolyLine)
{
//input:
//PolyLine is a 'continuous' curve (3xN matrix) on Surface, it can be from GeodesicPathFinder
//a point of PolyLine must be on an edge of Surface or a point of Surface
//PolyLine(:,0) is the start point, and it must be a point of Surface
//output:
//PointIndexOfPolyLine[k] is a point index on Surface
//---------------------------------------------------------------------------------
	DenseVector<int_max> PointIndexOfPolyLine;
	PointIndexOfPolyLine.Clear();
	DenseVector<ScalarType, 3> StartPoint;
	PolyLine.GetCol(0, StartPoint);
	auto StartPointIndex = FindNearestPointOnMesh(Surface, StartPoint);
	PointIndexOfPolyLine.Append(StartPointIndex);
	for (int_max Index = 1; Index < PolyLine.GetColCount(); ++Index)
	{
		DenseVector<ScalarType, 3> CurrentPoint;
		PolyLine.GetCol(Index, CurrentPoint);
		auto PointIndex_prev = PointIndexOfPolyLine[PointIndexOfPolyLine.GetLength() - 1];
		auto AdjPointIndexList = Surface.Point(PointIndex_prev).GetAdjacentPointIndexList();
		AdjPointIndexList = SetDiff(AdjPointIndexList, PointIndexOfPolyLine);
		//check if a point in PolyLine is in AdjPointIndexList
		bool Flag_NextPointInAdjPointList = false;
		for (int_max k = 0; k < AdjPointIndexList.GetLength(); ++k)
		{
			auto Pos_k = Surface.GetPointPosition(AdjPointIndexList[k]);
			auto Disp_k = CurrentPoint - Pos_k;
			auto Dist_k = Disp_k.L2Norm();
			if (Dist_k <= 3 * std::numeric_limits<ScalarType>::epsilon())
			{
				Flag_NextPointInAdjPointList = true;
				PointIndexOfPolyLine.Append(AdjPointIndexList[k]);
				break;
			}
		}
		if (Flag_NextPointInAdjPointList == false)
		{
			auto AdjEdgeIndexList = Surface.Point(PointIndex_prev).GetAdjacentEdgeIndexList();
			auto AdjFaceIndexList = Surface.Point(PointIndex_prev).GetAdjacentFaceIndexList();
			DenseVector<int_max> EdgeIndexListOfAdjFace;
			for (int_max k = 0; k < AdjFaceIndexList.GetLength(); ++k)
			{
				EdgeIndexListOfAdjFace.Append(Surface.Face(AdjFaceIndexList[k]).GetEdgeIndexList());
			}
			auto CandidateEdgeIndexList = SetDiff(EdgeIndexListOfAdjFace, AdjEdgeIndexList);
			DenseVector<ScalarType> CosAngleList;
			CosAngleList.Resize(CandidateEdgeIndexList.GetLength());
			for (int_max k = 0; k < CandidateEdgeIndexList.GetLength(); ++k)
			{
				auto PointIndex_A_B = Surface.Edge(CandidateEdgeIndexList[k]).GetPointIndexList();
				auto PosA = Surface.GetPointPosition(PointIndex_A_B[0]);
				auto PosB = Surface.GetPointPosition(PointIndex_A_B[1]);
				auto VectorA = PosA - CurrentPoint;
				VectorA /= VectorA.L2Norm();
				auto VectorB = PosB - CurrentPoint;
				VectorB /= VectorB.L2Norm();
				CosAngleList[k] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
			}
			auto Idx_min = CosAngleList.IndexOfMin();
			auto EdgeIndex_best = CandidateEdgeIndexList[Idx_min];
			auto PointIndex_cur = Surface.AddPoint(CurrentPoint);
			Surface.SplitFaceAtEdge(EdgeIndex_best, PointIndex_cur);
			PointIndexOfPolyLine.Append(PointIndex_cur);
		}
	}
	return PointIndexOfPolyLine;
}

}//namespace mdk

