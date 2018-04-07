#pragma once

namespace mdk
{

template<typename ScalarType>
DenseVector<int_max> ResampleOpenCurveOfSurface(TriangleMesh<ScalarType>& Surface, const DenseVector<int_max>& CurvePointIndexList_input, const DenseMatrix<ScalarType>& Curve_output)
{
	//CurvePointIndexList_input: an open bounary curve, CurvePointIndexList_input[0] is the start point, CurvePointIndexList_input[end] is the end point	
	//point order in CurvePointIndexList_input must align with normal direction, so new face will have consistant normal direction
	//Curve_output is the target, Curve_output(:,k) is a 3D point
	//Curve_output should be close to the orignial bounary on InputMesh

	DenseVector<int_max> CurvePointIndexList_output;
	//------------------- check input ----------------------------------------//
	if (CurvePointIndexList_input.GetLength() < 3)
	{
		MDK_Error("CurvePointIndexList_input.GetLength() < 3, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")		
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
	if (Curve_output.GetColCount() < 3)
	{
		MDK_Error("Curve_output.GetColCount() < 3, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")		
		return CurvePointIndexList_output;
	}
	//-------------------- input check done ------------------------------------//

	auto Curve_input = Surface.GetPointPosition(CurvePointIndexList_input);
	auto ParamCurve_input = ComputeCumulative3DCurveLengthList(Curve_input);
	ParamCurve_input /= ParamCurve_input[ParamCurve_input.GetLength() - 1];

	auto ParamCurve_output = ComputeCumulative3DCurveLengthList(Curve_output);
	ParamCurve_output /= ParamCurve_output[ParamCurve_output.GetLength() - 1];
	
	//start and end point of input bounary and output bounary share the same index
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
	if (PointCountOfCurve_output < 3)
	{
		MDK_Error("PointCountOfCurve_output < 3, abort @ TriangMeshProcessing ResampleOpenCurveOfSurface(...)")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}
	auto Curve_input = Surface.GetPointPosition(CurvePointIndexList_input);
	auto Curve_output= ResampleOpen3DCurveByCardinalSpline(Curve_input, PointCountOfCurve_output);
	return ResampleOpenCurveOfSurface(Surface, CurvePointIndexList_input, Curve_output);
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(vtkPolyData* Surface, const DenseVector<ScalarType, 3>& Point, int_max& FaceIndex_proj)
{
	DenseVector<ScalarType, 3> Point_proj;
	if (Surface == nullptr)
	{
		MDK_Error("Surface is nullptr, abort @ TriangMeshProcessing ProjectPointToSurface(...)")
		Point_proj.Fill(0);
		return Point_proj;
    }
	//https://www.vtk.org/Wiki/VTK/Examples/Cxx/PolyData/CellLocator
	auto CellLocator = vtkSmartPointer<vtkCellLocator>::New();
	CellLocator->SetDataSet(Surface);
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
	return Point_proj;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(vtkPolyData* Surface, const DenseVector<ScalarType, 3>& Point)
{
	int_max FaceIndex_proj;
	return ProjectPointToSurface(Surface, Point, FaceIndex_proj);
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point, int_max& FaceIndex_proj)
{
	DenseVector<ScalarType, 3> Point_proj;
	if (Surface.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("DataStructure of input mesh is not clean, abort @ TriangMeshProcessing ProjectPointToSurface(...)")
		Point_proj.Fill(0);
		return Point_proj;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(Surface);
	Point_proj = ProjectPointToSurface(VTKMesh, Point, FaceIndex_proj);
	return Point_proj;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point)
{
	int_max FaceIndex_proj;
	return ProjectPointToSurface(Surface, Point, FaceIndex_proj);
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(vtkPolyData* Surface, const DenseMatrix<ScalarType>& PointSet, DenseVector<int_max>& FaceIndexList_proj)
{
	DenseMatrix<ScalarType> PointSet_proj;
	if (Surface == nullptr)
	{
		MDK_Error("Surface is nullptr, abort @ TriangMeshProcessing ProjectPointToSurface(...)")
		return PointSet_proj;
	}

	auto CellLocator = vtkSmartPointer<vtkCellLocator>::New();
	CellLocator->SetDataSet(Surface);
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
		FaceIndexList_proj[k] = int_max(cellId);
	}
	return PointSet_proj;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(vtkPolyData* Surface, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> FaceIndexList_proj;
	return ProjectPointToSurface(Surface, PointSet, FaceIndexList_proj);
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet, DenseVector<int_max>& FaceIndexList_proj)
{
	DenseMatrix<ScalarType> PointSet_proj;
	if (Surface.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("DataStructure of input mesh is not clean, abort @ ProjectPointToSurface(...)")
		return PointSet_proj;
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(Surface);	
	PointSet_proj = ProjectPointToSurface(VTKMesh, PointSet, FaceIndexList_proj);
	return PointSet_proj;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToSurface(const TriangleMesh<ScalarType>& Surface, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> FaceIndexList_proj;
	return ProjectPointToSurface(Surface, PointSet, FaceIndexList_proj);
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
	//check if input Point is a point in PointIndexList
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
			return PointIndex_ref;
		}
	}

	// add new point -------------------------------------------------------
	auto H0 = PointIndexList[0];
	auto H1 = PointIndexList[1];
	auto H2 = PointIndexList[2];
	auto H3 = Surface.AddPoint(Point_proj);
	//-----------------
	//     2 
	//     3      
	// 0       1
	//-----------------		

	//check if input Point is on edge of FaceIndex_proj
	auto EdgeIndexList= Surface.Face(FaceIndex_proj).GetEdgeIndexList();
	{
		DenseVector<ScalarType> CosAngleList;
		CosAngleList.Resize(EdgeIndexList.GetLength());
		for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
		{
			auto PointIndex_A_B = Surface.Edge(EdgeIndexList[k]).GetPointIndexList();
			auto PosA = Surface.GetPointPosition(PointIndex_A_B[0]);
			auto PosB = Surface.GetPointPosition(PointIndex_A_B[1]);
			auto VectorA = PosA - Point;
			VectorA /= VectorA.L2Norm();
			auto VectorB = PosB - Point;
			VectorB /= VectorB.L2Norm();
			CosAngleList[k] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
		}
		auto Idx_min = CosAngleList.IndexOfMin();
		if (CosAngleList[Idx_min] < -0.99)
		{
			Surface.SplitFaceByEdge(EdgeIndexList[Idx_min], H3);
			return H3;
		}
	}	
	{//now, the input point is inside FaceIndex_proj
		Surface.DeleteFace(FaceIndex_proj);
		Surface.AddFaceByPoint({ H3, H2, H0 });
		Surface.AddFaceByPoint({ H3, H0, H1 });
		Surface.AddFaceByPoint({ H3, H1, H2 });
		return H3;
	}
}


template<typename ScalarType>
int_max AddPointToSurfaceByProjection(TriangleMesh<ScalarType>& Surface, const DenseVector<ScalarType, 3>& Point)
{
	auto PointIndex = Project_Add_Point_to_Surface(Surface, Point);
	Surface.SetPointPosition(PointIndex, Point);
	return PointIndex;
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

