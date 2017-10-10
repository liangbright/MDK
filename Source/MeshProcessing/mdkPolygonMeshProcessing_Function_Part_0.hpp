#pragma once

namespace mdk
{
template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start, int_max PointIndex_next)
{// work for none-clean DataStructure
	DenseVector<int_max> PointIndexListOfBoundaryCurve;

	if (InputMesh.IsValidPointIndex(PointIndex_start) == false)
	{
		MDK_Error("PointIndex_start is invalid @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	if (InputMesh.IsValidPointIndex(PointIndex_next) == false)
	{
		MDK_Error("PointIndex_next is invalid @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	if (InputMesh.Point(PointIndex_start).IsOnPolygonMeshBoundary() == false)
	{
		MDK_Error("PointIndex_start is NOT on boundary edge @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	if (InputMesh.Point(PointIndex_next).IsOnPolygonMeshBoundary() == false)
	{
		MDK_Error("PointIndex_next is NOT on boundary edge @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	bool Flag_start_neighbour_next = false;
	const auto& AdjacentPointIndexList_start = InputMesh.Point(PointIndex_start).GetAdjacentPointIndexList();
	for (int_max k = 0; k < AdjacentPointIndexList_start.GetLength(); ++k)
	{
		if (AdjacentPointIndexList_start[k] == PointIndex_next)
		{
			Flag_start_neighbour_next = true;
			break;
		}
	}
	if (Flag_start_neighbour_next == false)
	{
		MDK_Error("PointIndex_start and PointIndex_next are NOT neighbour to each other @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

    int_max BoundaryEdgeCountOfInputMesh = 0;
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfEdgeIndex(); ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			if (InputMesh.Edge(k).IsOnPolygonMeshBoundary() == true)
			{
				BoundaryEdgeCountOfInputMesh += 1;
			}
		}
	}

    if (BoundaryEdgeCountOfInputMesh <= 0)
    {
        MDK_Error("BoundaryEdgeCountOfInputMesh <= 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
        return PointIndexListOfBoundaryCurve;
    }

    PointIndexListOfBoundaryCurve.SetCapacity(BoundaryEdgeCountOfInputMesh);
    PointIndexListOfBoundaryCurve.Append(PointIndex_start);	
    PointIndexListOfBoundaryCurve.Append(PointIndex_next);

	int_max BoundaryPointIndex_prev = PointIndex_start;
	int_max BoundaryPointIndex_current = PointIndex_next;
    int_max Counter = 2;
    while (true)
    {  
        int_max BoundaryPointIndex_next = -1;
		auto tempAdjacentEdgeIndexList_current = InputMesh.Point(BoundaryPointIndex_current).GetAdjacentEdgeIndexList();
        for (int_max n = 0; n < tempAdjacentEdgeIndexList_current.GetLength(); ++n)
        {
            if (InputMesh.Edge(tempAdjacentEdgeIndexList_current[n]).IsOnPolygonMeshBoundary() == true)
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
            MDK_Error("BoundaryPointIndex_current=" << BoundaryPointIndex_current << ", BoundaryPointIndex < 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurvey(...)")
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
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh, int_max PointIndex_start)
{// work for none-clean DataStructure
    DenseVector<int_max> PointIndexListOfBoundaryCurve;

	if (InputMesh.IsValidPointIndex(PointIndex_start) == false)
	{
		MDK_Error("PointIndex_start is invalid @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

	if (InputMesh.Point(PointIndex_start).IsOnPolygonMeshBoundary() == false)
	{
		MDK_Error("PointIndex_start is NOT on boundary edge @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
		return PointIndexListOfBoundaryCurve;
	}

    int_max BoundaryEdgeCountOfInputMesh = 0;
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfEdgeIndex(); ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			if (InputMesh.Edge(k).IsOnPolygonMeshBoundary() == true)
			{
				BoundaryEdgeCountOfInputMesh += 1;
			}
		}
	}

    if (BoundaryEdgeCountOfInputMesh <= 0)
    {
        MDK_Error("BoundaryEdgeCountOfInputMesh  <= 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
        return PointIndexListOfBoundaryCurve;
    }
	
	int_max PointIndex_next = -1;
    const auto& AdjacentPointIndexList_start = InputMesh.Point(PointIndex_start).GetAdjacentPointIndexList();
    for (int_max k = 0; k < AdjacentPointIndexList_start.GetLength(); ++k)
    {
        if (InputMesh.Point(AdjacentPointIndexList_start[k]).IsOnPolygonMeshBoundary() == true)
        {
			PointIndex_next = AdjacentPointIndexList_start[k];
            break;
        }
    }

	PointIndexListOfBoundaryCurve= TraceMeshBoundaryCurve(InputMesh, PointIndex_start, PointIndex_next);
	return PointIndexListOfBoundaryCurve;
}


template<typename ScalarType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<ScalarType>& InputMesh)
{// work for none-clean DataStructure
    // find boundary point
    DenseVector<int_max> BoundaryPointIndexList;
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			if (InputMesh.Point(k).IsOnPolygonMeshBoundary() == true)
			{
				BoundaryPointIndexList.Append(k);
			}
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
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k))
		{
			ScalarType x, y, z;
			InputMesh.GetPointPosition(k, x, y, z);
			auto Distance_sq = (x - PointPosition[0])*(x - PointPosition[0]) + (y - PointPosition[1])*(y - PointPosition[1]) + (z - PointPosition[2])*(z - PointPosition[2]);
			if (IsFirstPoint == true)
			{
				Distance_sq_min = Distance_sq;
				OutputPointIndex = k;
				IsFirstPoint = false;
			}
			else
			{
				if (Distance_sq < Distance_sq_min)
				{
					Distance_sq_min = Distance_sq;
					OutputPointIndex = k;
				}
			}
		}
	}

    return OutputPointIndex;
}


template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKPointLocator(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& PointPosition)
{
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKPointLocator(...)")
		return -1;
	}
	auto PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	PointLocator->SetDataSet(InputMesh_vtk);
	PointLocator->AutomaticOn();
	int_max PointCountOfMesh = InputMesh_vtk->GetNumberOfPoints();
	PointLocator->SetNumberOfPointsPerBucket(std::max(int_max(2), PointCountOfMesh / 10));
	PointLocator->BuildLocator();
	auto TempMesh = vtkSmartPointer<vtkPolyData>::New();
	PointLocator->GenerateRepresentation(PointLocator->GetLevel(), TempMesh);
	double Pos[3];
	Pos[0] = double(PointPosition[0]);
	Pos[1] = double(PointPosition[1]);
	Pos[2] = double(PointPosition[2]);
	auto PointIndex = int_max(PointLocator->FindClosestPoint(Pos));
	return PointIndex;
}

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKPointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKPointLocator(...)")
		return -1;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return FindNearestPointOnMeshByVTKPointLocator(VTKMesh, PointPosition);
}

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKPointLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList_output;
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKPointLocator(...)")
		return -1;
	}
		
	auto PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	PointLocator->SetDataSet(InputMesh_vtk);
	PointLocator->AutomaticOn();
	int_max PointCountOfMesh = InputMesh_vtk->GetNumberOfPoints();
	PointLocator->SetNumberOfPointsPerBucket((std::max)(int_max(2), PointCountOfMesh / 10));
	PointLocator->BuildLocator();
	auto TempMesh = vtkSmartPointer<vtkPolyData>::New();
	PointLocator->GenerateRepresentation(PointLocator->GetLevel(), TempMesh);
	for (int_max k = 0; k < PointSet.GetColCount(); ++k)
	{
		double Pos[3];
		Pos[0] = double(PointSet(0, k));
		Pos[1] = double(PointSet(1, k));
		Pos[2] = double(PointSet(2, k));
		auto Index = int_max(PointLocator->FindClosestPoint(Pos));
		PointIndexList_output.Append(Index);
	}
	return PointIndexList_output;
}

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKPointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList_output;
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKPointLocator(...)")
		return PointIndexList_output;
	}
	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("Input PointSet is invalid @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKPointLocator(...)")
		return PointIndexList_output;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	PointIndexList_output = FindNearestPointOnMeshByVTKPointLocator(VTKMesh, PointSet);
	return PointIndexList_output;
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
{// InputMesh DataStructure does not need to be clean
	if (InputMesh.IsEmpty() == true || ClosedEdgeCurve_EdgeIndexList.IsEmpty() == true)
	{
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	DenseVector<int> FaceFlagList;
	//-1: deleted face
	//0: initial state
	//1: the same group as FaceIndex_seed
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex());
	for (int_max k = 0; k < FaceFlagList.GetLength(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			FaceFlagList[k] = 0;
		}
		else
		{
			FaceFlagList[k] = -1;
		}
	}
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
	{// InputMesh DataStructure does not need to be clean
		//MDK_Warning("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SegmentMeshByEdgeCurve(...)")
	}

	DenseVector<int_max> FaceFlagList;
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex());
	//-1: deleted face
	//0: initial state
	//1: segmented
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex());
	int_max FaceIndex_seed = -1;
	for (int_max k = 0; k < FaceFlagList.GetLength(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			FaceFlagList[k] = 0;
			FaceIndex_seed = k;
		}
		else
		{
			FaceFlagList[k] = -1;
		}
	}
	
	while (true)
	{		
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
		if (OutputMesh.Point(k).IsOnPolygonMeshBoundary() == true)
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
		if (InputMeshB_clean.Point(k).IsOnPolygonMeshBoundary() == true)
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
