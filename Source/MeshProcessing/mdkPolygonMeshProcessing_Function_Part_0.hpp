#pragma once

namespace mdk
{

template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start)
{// work for none-clean DataStructure
    DenseVector<int_max> PointIndexListOfBoundaryCurve;

	if (InputMesh.IsValidPointIndex(PointIndex_start) == false)
	{
		MDK_Error("PointIndex_start is invalid @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	if (InputMesh.Point(PointIndex_start).IsOnBoundaryEdge() == false)
	{
		MDK_Error("PointIndex_start is NOT on boundary edge @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

    int_max BoundaryEdgeCountOfInputMesh = 0;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		if (it.Edge().IsBoundary() == true)
		{
			BoundaryEdgeCountOfInputMesh += 1;
		}
	}

    if (BoundaryEdgeCountOfInputMesh <= 0)
    {
        MDK_Error("BoundaryEdgeCountOfInputMesh  <= 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
        return PointIndexListOfBoundaryCurve;
    }

    PointIndexListOfBoundaryCurve.SetCapacity(BoundaryEdgeCountOfInputMesh);

    PointIndexListOfBoundaryCurve.Append(PointIndex_start);

    auto BoundaryPointIndex_prev = PointIndex_start;
	int_max BoundaryPointIndex_current = -1;

    const auto& AdjacentPointIndexList_start = InputMesh.Point(PointIndex_start).GetAdjacentPointIndexList();
    for (int_max k = 0; k < AdjacentPointIndexList_start.GetLength(); ++k)
    {
        if (InputMesh.Point(AdjacentPointIndexList_start[k]).IsOnBoundaryEdge() == true)
        {
            BoundaryPointIndex_current = AdjacentPointIndexList_start[k];
            break;
        }
    }

    if (BoundaryPointIndex_current < 0)
    {
        MDK_Error("Something is wrong with input mesh @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
        return PointIndexListOfBoundaryCurve;
    }

    PointIndexListOfBoundaryCurve.Append(BoundaryPointIndex_current);

    int_max Counter = 2;

    while (true)
    {
        auto tempAdjacentEdgeIndexList_current = InputMesh.Point(BoundaryPointIndex_current).GetAdjacentEdgeIndexList();

        int_max BoundaryPointIndex_next = -1;

        for (int_max n = 0; n < tempAdjacentEdgeIndexList_current.GetLength(); ++n)
        {
            if (InputMesh.Edge(tempAdjacentEdgeIndexList_current[n]).IsBoundary() == true)
            {
                auto tempPointIndexList = InputMesh.Edge(tempAdjacentEdgeIndexList_current[n]).GetPointIndexList();
                if (tempPointIndexList[0] != BoundaryPointIndex_current && tempPointIndexList[0] != BoundaryPointIndex_prev)
                {
                    BoundaryPointIndex_next = tempPointIndexList[0];
                }
                else if (tempPointIndexList[1] != BoundaryPointIndex_current && tempPointIndexList[1] != BoundaryPointIndex_prev)
                {
                    BoundaryPointIndex_next = tempPointIndexList[1];
                }
            }
        }

        if (BoundaryPointIndex_next < 0)
        {
            MDK_Error("BoundaryPointIndex < 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurvey(...)")
            break;
        }

        if (BoundaryPointIndex_next == PointIndex_start)
        {
            break; // while
        }

		//check if BoundaryPointIndex_next is already in PointIndexListOfBoundaryCurve
		auto tempIndex = PointIndexListOfBoundaryCurve.ExactMatch("first", BoundaryPointIndex_next);
		if (tempIndex >= 0)
		{
			MDK_Warning("Boundary Curve selfintersect ! @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
			break;//while
		}

		PointIndexListOfBoundaryCurve.Append(BoundaryPointIndex_next);

        BoundaryPointIndex_prev = BoundaryPointIndex_current;
        BoundaryPointIndex_current = BoundaryPointIndex_next;
        Counter += 1;

        if (Counter > BoundaryEdgeCountOfInputMesh)
        {
            MDK_Error("Counter > BoundaryEdgeCountOfInputMesh @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
            break; // while
        }
    }
    //-----------------------------------------------------------------------------------------------------

    return PointIndexListOfBoundaryCurve;
}


template<typename ScalarType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh)
{// work for none-clean DataStructure
    // find boundary point
    DenseVector<int_max> BoundaryPointIndexList;
    for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        if (it.Point().IsOnBoundaryEdge() == true)
        {
            BoundaryPointIndexList.Append(it.GetPointIndex());
        }
    }
    auto BoundaryPointCount = BoundaryPointIndexList.GetLength();

    DenseVector<int_max> FlagList;
    FlagList.Resize(BoundaryPointCount);
    FlagList.Fill(0);
    // 0: not checked
    // 1: checked

	ObjectArray<DenseVector<int_max>> BoundaryCurveTable;

    while (true)
    {
        // pick a boundary point and track boundary
		int_max BoundaryPointIndex = -1;
        for (int_max k = 0; k < FlagList.GetLength(); ++k)
        {
            if (FlagList[k] == 0)
            {
                BoundaryPointIndex = BoundaryPointIndexList[k];
                break;
            }
        }

        if (BoundaryPointIndex < 0)
        {
            break;
        }

        auto BoundaryCurve = TraceMeshBoundaryCurve(InputMesh, BoundaryPointIndex);

        // set flag
        for (int_max k = 0; k < BoundaryCurve.GetLength(); ++k)
        {
			auto tempIndex = BoundaryPointIndexList.ExactMatch("first", BoundaryCurve[k]);
            FlagList[tempIndex] = 1;
        }

        BoundaryCurveTable.Append(std::move(BoundaryCurve));
    }

    return BoundaryCurveTable;
}


template<typename ScalarType>
int_max FindNearestPointOnMesh(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition)
{// work for none-clean DataStructure
	int_max OutputPointIndex = -1;

    ScalarType Distance_sq_min = 0;
    bool IsFirstPoint = true;
    for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        ScalarType x, y, z;
        it.Point().GetPosition(x, y, z);
        auto Distance_sq =  (x - PointPosition[0])*(x - PointPosition[0]) 
                          + (y - PointPosition[1])*(y - PointPosition[1]) 
                          + (z - PointPosition[2])*(z - PointPosition[2]);

        if (IsFirstPoint == true)
        {
            Distance_sq_min = Distance_sq;
			OutputPointIndex = it.GetPointIndex();
            IsFirstPoint = false;
        }
        else
        {
            if (Distance_sq < Distance_sq_min)
            {
                Distance_sq_min = Distance_sq;
                OutputPointIndex = it.GetPointIndex();
            }
        }
    }

    return OutputPointIndex;
}


template<typename ScalarType>
PolygonMesh<ScalarType> SmoothMeshByVTKSmoothPolyDataFilter(const PolygonMesh<ScalarType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SmoothMeshByVTKSmoothPolyDataFilter(...)")
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);

	auto SmoothFilter =	vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
	SmoothFilter->SetInputData(VTKMesh);
	SmoothFilter->SetNumberOfIterations(Iter);
	SmoothFilter->SetRelaxationFactor(0.01);
	SmoothFilter->SetConvergence(0.0);
	if (Flag_FeatureEdgeSmoothing == true)
	{
		SmoothFilter->FeatureEdgeSmoothingOn();
	}
	else
	{
		SmoothFilter->FeatureEdgeSmoothingOff();
	}
	if (Flag_BoundarySmoothing == true)
	{
		SmoothFilter->BoundarySmoothingOn();
	}
	else
	{
		SmoothFilter->BoundarySmoothingOff();
	}
	SmoothFilter->Update();

	//double a = SmoothFilter->GetFeatureAngle();

	PolygonMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> SmoothMeshByVTKWindowedSincPolyDataFilter(const PolygonMesh<ScalarType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SmoothMeshByVTKWindowedSincPolyDataFilter(...)")
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);

	auto SmoothFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
	SmoothFilter->SetInputData(VTKMesh);
	SmoothFilter->SetPassBand(PassBand);
	SmoothFilter->SetNumberOfIterations(Iter);	
	if (Flag_FeatureEdgeSmoothing == true)
	{
		SmoothFilter->FeatureEdgeSmoothingOn();
	}
	else
	{
		SmoothFilter->FeatureEdgeSmoothingOff();
	}
	if (Flag_BoundarySmoothing == true)
	{
		SmoothFilter->BoundarySmoothingOn();
	}
	else
	{
		SmoothFilter->BoundarySmoothingOff();
	}
	SmoothFilter->NonManifoldSmoothingOn();
	SmoothFilter->NormalizeCoordinatesOn();	
	SmoothFilter->Update();

	PolygonMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename ScalarType>
DenseVector<int_max> SegmentMeshByEdgeCurve(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed)
{
	if (InputMesh.IsEmpty() == true || ClosedEdgeCurve_EdgeIndexList.IsEmpty() == true)
	{
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SegmentMeshByEdgeCurve(...)")
	}

	DenseVector<int> FaceFlagList;//1: the same group as FaceIndex_seed
	FaceFlagList.Resize(InputMesh.GetFaceCount());
	FaceFlagList.Fill(0);
	FaceFlagList[FaceIndex_seed] = 1;

	DenseVector<int_max> FaceIndexList_front;
	FaceIndexList_front.Append(FaceIndex_seed);

	//DenseVector<int_max> FaceIndexList_seg_a;
	//FaceIndexList_seg_a.Append(FaceIndex_seed);

	while (true)
	{
		//grow
		DenseVector<int_max> FaceIndexList_new;
		for (int_max k = 0; k < FaceIndexList_front.GetLength(); ++k)
		{
			auto EdgeIndexList_k = InputMesh.Face(FaceIndexList_front[k]).GetEdgeIndexList();
			for (int_max n = 0; n < EdgeIndexList_k.GetLength(); ++n)
			{
				auto tempIndex = ClosedEdgeCurve_EdgeIndexList.ExactMatch("first", EdgeIndexList_k[n]);
				if (tempIndex < 0)
				{
					auto FaceIndexList_n = InputMesh.Edge(EdgeIndexList_k[n]).GetAdjacentFaceIndexList();
					for (int_max m = 0; m < FaceIndexList_n.GetLength(); ++m)
					{
						if (FaceFlagList[FaceIndexList_n[m]] == 0)
						{
							FaceIndexList_new.Append(FaceIndexList_n[m]);
							FaceFlagList[FaceIndexList_n[m]] = 1;
						}
					}
				}
			}
		}
		//update front
		FaceIndexList_front = FaceIndexList_new.GetSubSet(FaceIndexList_new.FindUnique());
		//stop?
		if (FaceIndexList_front.GetLength() == 0)
		{
			break;
		}
		//FaceIndexList_seg_a.Append(FaceIndexList_front);
	}

	auto FaceIndexList_seg = FaceFlagList.Find([](int Flag) { return (Flag == 1); });
	return FaceIndexList_seg;
}


template<typename ScalarType>
DenseVector<DenseVector<int_max>> SegmentMeshByEdgeCurve(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList)
{
	DenseVector<DenseVector<int_max>> FastIndexList_output;

	if (InputMesh.IsEmpty() == true || ClosedEdgeCurve_EdgeIndexList.IsEmpty() == true)
	{
		return FastIndexList_output;
	}
	
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SegmentMeshByEdgeCurve(...)")
	}

	DenseVector<int_max> FaceFlagList;
	FaceFlagList.Resize(InputMesh.GetFaceCount());
	FaceFlagList.Fill(0);
	int_max FaceIndex_seed = 0;	
	while (true)
	{
		FaceFlagList[FaceIndex_seed] = 1;
		auto FastIndexList_temp = SegmentMeshByEdgeCurve(InputMesh, ClosedEdgeCurve_EdgeIndexList, FaceIndex_seed);
		FastIndexList_output.Append(FastIndexList_temp);
		for (int_max k = 0; k < FastIndexList_temp.GetLength(); ++k)
		{
			FaceFlagList[FastIndexList_temp[k]] = 1;
		}
		FaceIndex_seed = -1;
		for (int_max k = 0; k < FaceFlagList.GetLength(); ++k)
		{
			if (FaceFlagList[k] == 0)
			{
				FaceIndex_seed = k;
				break;//for
			}
		}
		if (FaceIndex_seed < 0)
		{
			break;//while
		}
	}
	return FastIndexList_output;
}


template<typename ScalarType>
PolygonMesh<ScalarType> ClipMeshByVTKClipPolyData(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& Origin, const DenseVector<ScalarType, 3>& Normal)
{
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh_vtk is nullptr @ mdkPolygonMeshProcessing ClipMeshByVTKClipPolyData(...)")
		PolygonMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}

	auto plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(Origin[0], Origin[1], Origin[2]);
	plane->SetNormal(Normal[0], Normal[1], Normal[2]);

	auto clipper = vtkSmartPointer<vtkClipPolyData>::New();
	clipper->SetInputData(InputMesh_vtk);
	clipper->SetClipFunction(plane);
	clipper->SetValue(0);
	clipper->Update();
	auto cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
	cleaner->SetInputData(clipper->GetOutput());
	cleaner->Update();
	auto OutputMesh_vtk = cleaner->GetOutput();

	PolygonMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(OutputMesh_vtk, OutputMesh);
	return OutputMesh;
}

template<typename ScalarType>
PolygonMesh<ScalarType> ClipMeshByVTKClipPolyData(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& Origin, const DenseVector<ScalarType, 3>& Normal)
{
	auto InputMesh_vtk = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return ClipMeshByVTKClipPolyData(InputMesh_vtk, Origin, Normal);
}


template<typename ScalarType>
DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(vtkPolyData* InputMesh_vtk, int_max PointIndex_start, int_max PointIndex_end)
{
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh_vtk is nullptr @ mdkPolygonMeshProcessing FindShortestPathByVTKDijkstraGraphGeodesicPath(...)")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto PathFinder = vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();	
	PathFinder->SetInputData(InputMesh_vtk);
	PathFinder->SetStartVertex(PointIndex_start);
	PathFinder->SetEndVertex(PointIndex_end);
	PathFinder->Update();
	auto IdList = PathFinder->GetIdList();

	DenseVector<int_max> ShortestPath;
	ShortestPath.SetCapacity(IdList->GetNumberOfIds());
	for (int_max k = IdList->GetNumberOfIds() - 1; k >= 0; --k)// id is in reverse order
	{
		int_max idx = IdList->GetId(k);
		ShortestPath.Append(idx);
	}
	return ShortestPath;
}

template<typename ScalarType>
DenseVector<int_max> FindShortestPathByVTKDijkstraGraphGeodesicPath(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start, int_max PointIndex_end)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing FindShortestPathByVTKDijkstraGraphGeodesicPath(...)")
	}

	auto InputMesh_vtk = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return FindShortestPathByVTKDijkstraGraphGeodesicPath(InputMesh_vtk, PointIndex_start, PointIndex_end);
}


template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshBoundary(const PolygonMesh<ScalarType>& InputMeshA, const PolygonMesh<ScalarType>& InputMeshB, ScalarType Threshold)
{// work for none-clean DataStructure, attribute info not retained
	auto PointCountA = InputMeshA.GetPointCount(); 	auto EdgeCountA = InputMeshA.GetEdgeCount(); auto FaceCountA = InputMeshA.GetFaceCount();
	auto PointCountB = InputMeshB.GetPointCount();	auto EdgeCountB = InputMeshB.GetEdgeCount(); auto FaceCountB = InputMeshB.GetFaceCount();

	PolygonMesh<ScalarType> OutputMesh;
	OutputMesh.SetCapacity(PointCountA + PointCountB, EdgeCountA + EdgeCountB, FaceCountA + FaceCountB);
	//copy InputMeshA, only point and face
	{
		DenseMatrix<ScalarType> PointPositionMatrix;
		ObjectArray<DenseVector<int_max>> FaceTable;
		InputMeshA.GetPointPositionMatrixAndFaceTable(PointPositionMatrix, FaceTable);
		OutputMesh.Construct(std::move(PointPositionMatrix), FaceTable);
	}

	DenseVector<int_max> BoundaryPointIndexListA;
	for (int_max k = 0; k < PointCountA; ++k)
	{
		if (OutputMesh.Point(k).IsOnBoundaryEdge() == true)
		{
			BoundaryPointIndexListA.Append(k);
		}
	}
	auto BoundaryA = OutputMesh.GetPointPosition(BoundaryPointIndexListA);

	PolygonMesh<ScalarType> InputMeshB_clean;
	if (InputMeshB.Check_If_DataStructure_is_Clean() == true)
	{
		InputMeshB_clean.ForceShare(InputMeshB);
	}
	else
	{
		DenseMatrix<ScalarType> PointPositionMatrix;
		ObjectArray<DenseVector<int_max>> FaceTable;
		InputMeshB.GetPointPositionMatrixAndFaceTable(PointPositionMatrix, FaceTable);
		InputMeshB_clean.Construct(std::move(PointPositionMatrix), FaceTable);
	}	
	
	//map PointIndex of InputMeshB_clean  to PointIndex of OutputMesh
	std::unordered_map<int_max, int_max> Map_PointIndexB_to_OutputIndex;	
	for (int_max k = 0; k < PointCountB; ++k)
	{
		auto PosB = InputMeshB_clean.GetPointPosition(k);
		if (InputMeshB_clean.Point(k).IsOnBoundaryEdge() == true)
		{			
			auto Idx_near = FindNearestPointOnCurve(BoundaryA, PosB);
			auto PosA = OutputMesh.GetPointPosition(BoundaryPointIndexListA[Idx_near]);
			auto dist = (PosA - PosB).L2Norm();
			if (dist <= Threshold)
			{
				auto PointIndexA = BoundaryPointIndexListA[Idx_near];
				Map_PointIndexB_to_OutputIndex[k] = PointIndexA;
				if (dist > 0)
				{
					auto PosM = (PosA + PosB) / ScalarType(2);
					OutputMesh.SetPointPosition(PointIndexA, PosM);
				}
			}
			else
			{
				auto PointIndex_out = OutputMesh.AddPoint(PosB);
				Map_PointIndexB_to_OutputIndex[k] = PointIndex_out;
			}
		}
		else
		{
			auto PointIndex_out = OutputMesh.AddPoint(PosB);
			Map_PointIndexB_to_OutputIndex[k] = PointIndex_out;
		}
	}

	for (int_max k = 0; k < FaceCountB; ++k)
	{
		auto PointIndexList = InputMeshB_clean.Face(k).GetPointIndexList();
		DenseVector<int_max> PointIndexList_out;
		PointIndexList_out.Resize(PointIndexList.GetLength());
		for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
		{
			PointIndexList_out[n] = Map_PointIndexB_to_OutputIndex[PointIndexList[n]];
		}
		OutputMesh.AddFaceByPoint(PointIndexList_out);
	}

	return OutputMesh;
}

}//namespace mdk
