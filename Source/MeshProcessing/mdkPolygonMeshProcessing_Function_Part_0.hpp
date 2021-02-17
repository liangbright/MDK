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
//-------
template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKKdTreePointLocator(vtkPolyData* InputMesh_vtk, const DenseVector<ScalarType, 3>& PointPosition)
{
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKKdTreePointLocator(...)")
		return -1;
	}
	auto PointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	PointLocator->SetDataSet(InputMesh_vtk);
	PointLocator->BuildLocator();
	double Pos[3];
	Pos[0] = double(PointPosition[0]);
	Pos[1] = double(PointPosition[1]);
	Pos[2] = double(PointPosition[2]);
	auto PointIndex = int_max(PointLocator->FindClosestPoint(Pos));
	return PointIndex;
}

template<typename ScalarType>
int_max FindNearestPointOnMeshByVTKKdTreePointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<ScalarType, 3>& PointPosition)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKKdTreePointLocator(...)")
		return -1;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return FindNearestPointOnMeshByVTKPointLocator(VTKMesh, PointPosition);
}

template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnMeshByVTKKdTreePointLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList_output;
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKKdTreePointLocator(...)")
		return PointIndexList_output;
	}
	auto PointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	PointLocator->SetDataSet(InputMesh_vtk);
	PointLocator->BuildLocator();
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
DenseVector<int_max> FindNearestPointOnMeshByVTKKdTreePointLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet)
{
	DenseVector<int_max> PointIndexList_output;
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKKdTreePointLocator(...)")
			return PointIndexList_output;
	}
	if (PointSet.GetRowCount() != 3)
	{
		MDK_Error("Input PointSet is invalid @ mdkPolygonMeshProcessing FindNearestPointOnMeshByVTKKdTreePointLocator(...)")
		return PointIndexList_output;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	PointIndexList_output = FindNearestPointOnMeshByVTKPointLocator(VTKMesh, PointSet);
	return PointIndexList_output;
}

//--------

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
		return PointIndexList_output;
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
void ProjectPointToFaceByVTKCellLocator(vtkPolyData* InputMesh_vtk, const DenseMatrix<ScalarType>& PointSet,
	                                    DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj)
{
	if (InputMesh_vtk == nullptr)
	{
		MDK_Error("InputMesh_vtk is nullptr, abort @ PolygonMeshProcessing FindNearestCellOnMeshByVTKCellLocator(...)")
		return;
	}

	PointSet_proj.Resize(PointSet.GetSize());
	PointSet_proj.Fill(0);
	FaceIndexList_proj.Resize(PointSet.GetColCount());
	FaceIndexList_proj.Fill(-1);

	auto CellLocator = vtkSmartPointer<vtkCellLocator>::New();
	CellLocator->SetDataSet(InputMesh_vtk);
	CellLocator->BuildLocator();

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
		PointSet_proj.SetCol(k, closestPoint);
		FaceIndexList_proj[k] = int_max(cellId);
	}
}


template<typename ScalarType>
void ProjectPointToFaceByVTKCellLocator(const PolygonMesh<ScalarType>& InputMesh, const DenseMatrix<ScalarType>& PointSet,
	                                    DenseMatrix<ScalarType>& PointSet_proj, DenseVector<int_max>& FaceIndexList_proj)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("DataStructure of input mesh is not clean, abort @ ProjectPointToSurface(...)")
		return;
	}
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	ProjectPointToFaceByVTKCellLocator(VTKMesh, PointSet, PointSet_proj, FaceIndexList_proj);
}


template<typename ScalarType>
void SmoothMeshByVTKSmoothPolyDataFilter(PolygonMesh<ScalarType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SmoothMeshByVTKSmoothPolyDataFilter(...)")
		return;
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

	PolygonMesh<ScalarType> SmoothMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), SmoothMesh);	
	InputMesh.SetPointPosition(ALL, SmoothMesh.GetPointPosition(ALL));
}


template<typename ScalarType>
void SmoothMeshByVTKWindowedSincPolyDataFilter(PolygonMesh<ScalarType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	if (InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT clean @ mdkPolygonMeshProcessing SmoothMeshByVTKWindowedSincPolyDataFilter(...)")
		return;
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

	PolygonMesh<ScalarType> SmoothMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), SmoothMesh);
	InputMesh.SetPointPosition(ALL, SmoothMesh.GetPointPosition(ALL));
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
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex()+1);
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
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex()+1);
	//-1: deleted face
	//0: initial state
	//1: segmented
	FaceFlagList.Resize(InputMesh.GetMaxValueOfFaceIndex()+1);
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
PolygonMesh<ScalarType> MergeMeshOnBoundary(const PolygonMesh<ScalarType>& InputMesh, ScalarType PointDistanceThreshold)
{
	std::unordered_map<int_max, int_max> Map_InputIndex_to_OutputIndex;
	return MergeMeshOnBoundary(InputMesh, PointDistanceThreshold, Map_InputIndex_to_OutputIndex);
}

template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshOnBoundary(const PolygonMesh<ScalarType>& InputMesh, ScalarType PointDistanceThreshold,
											std::unordered_map<int_max, int_max>& Map_InputIndex_to_OutputIndex)
{
	DenseVector<int_max> PointIndexList;
	for (int_max k = 0; k < InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			if (InputMesh.Point(k).IsOnPolygonMeshBoundary() == true)
			{
				PointIndexList.Append(k);
			}
		}
	}
	return MergeMesh(InputMesh, PointIndexList, PointDistanceThreshold, Map_InputIndex_to_OutputIndex);
}


template<typename ScalarType>
PolygonMesh<ScalarType> MergeMesh(const PolygonMesh<ScalarType>& InputMesh, const DenseVector<int_max>& PointIndexList, ScalarType DistanceThreshold,
 								  DenseVector<int_max>& PointIndexMap_Input_to_Output)
{
	PolygonMesh<ScalarType> OutputMesh;
	PointIndexMap_Input_to_Output.Resize(InputMesh.GetMaxValueOfPointIndex()+1);
	PointIndexMap_Input_to_Output.Fill(-1);
	for (int_max k = 0; k < InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			bool Flag = false;
			for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
			{
				if (k == PointIndexList[n])
				{
					Flag = true;
					break;
				}
			}
			if (Flag == false)
			{
				auto PointIndex_out = OutputMesh.AddPoint(InputMesh.GetPointPosition(k));
				PointIndexMap_Input_to_Output[k] = PointIndex_out;
			}
		}
	}

	DenseVector<int_max> FlagList_merged;
	FlagList.Resize(PointIndexList.GetLength());
	FlagList_merged.Fill(0);//1: the point is merged with some other points
	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		auto PointIndex_k = PointIndexList[k];
		if (FlagList_merged[k] == 0)
		{
			auto PointIndex_out = OutputMesh.AddPoint(InputMesh.GetPointPosition(k));
			PointIndexMap_Input_to_Output[PointIndex_k] = PointIndex_out;
			auto Pos_k = InputMesh.GetPointPosition(PointIndex_k);
			for (int_max n = k + 1; n < PointIndexList.GetLength(); ++n)
			{
				auto PointIndex_n = PointIndexList[n];
				auto Pos_n = InputMesh.GetPointPosition(PointIndex_n);
				auto dist_kn = (Pos_k - Pos_n).L2Norm();
				if (dist_kn <= DistanceThreshold)
				{
					FlagList_merged[n] = 1;
					PointIndexMap_Input_to_Output[PointIndex_n] = PointIndex_out;
				}
			}
		}
	}

	for (int_max FaceIndex = 0; FaceIndex <= InputMesh.GetMaxValueOfFaceIndex(); ++FaceIndex)
	{
		if (InputMesh.IsValidFaceIndex(FaceIndex) == true)
		{
			DenseVector<int_max> PointIndexList_out;
			auto PointIndexList = InputMesh.Face(InputMesh).GetPointIndexList();
			for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
			{
				auto PointIndex_out = PointIndexMap_Input_to_Output[PointIndexList[n]];
				if (PointIndex_out >= 0)
				{
					PointIndexList_out.Append(PointIndex_out)
				}
			}
			OutputMesh.AddFaceByPoint(PointIndexList_out);
		}
	}

	//Update PointSet Only
	for (int_max k = 0; k < InputMesh.GetPointSetCount(); ++k)
	{	
		DenseVector<int_max> PointSet_out;
		auto PointSet = InputMesh.GetPointSet(k);		
		for (int_max n = 0; n < PointSet.GetLength(); ++n)
		{
			auto PointIndex = PointSet[n];
			auto PointIndex_out = PointIndexMap_Input_to_Output[PointIndex];
			if (PointIndex_out >= 0)
			{
				PointSet_out.Append(PointIndex_out);
			}
		}
		OutputMesh.AddPointSet(k, PointSet_out)
	}
	
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> MergeMeshOnBoundary(const PolygonMesh<ScalarType>& InputMeshA, const PolygonMesh<ScalarType>& InputMeshB, ScalarType PointDistanceThreshold)
{// work for none-clean DataStructure, attribute info not retained
	DenseVector<int_max> PointIndexListA;
	for (int_max k = 0; k < InputMeshA.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMeshA.IsValidPointIndex(k) == true)
		{
			if (InputMeshA.Point(k).IsOnPolygonMeshBoundary() == true)
			{
				PointIndexListA.Append(k);
			}
		}
	}
	DenseVector<int_max> PointIndexListB;
	for (int_max k = 0; k < InputMeshB.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMeshB.IsValidPointIndex(k) == true)
		{
			if (InputMeshB.Point(k).IsOnPolygonMeshBoundary() == true)
			{
				PointIndexListB.Append(k);
			}
		}
	}
	return MergeMesh(InputMeshA, PointIndexListA, InputMeshB, PointIndexListB, PointDistanceThreshold);
}

template<typename ScalarType>
PolygonMesh<ScalarType> MergeMesh(const PolygonMesh<ScalarType>& InputMeshA, const DenseVector<int_max>& PointIndexListA,
						          const PolygonMesh<ScalarType>& InputMeshB, const DenseVector<int_max>& PointIndexListB, ScalarType PointDistanceThreshold)
{
	DenseVector<int_max> PointIndexMap_InputA_to_Output, PointIndexMap_InputB_to_Output;
	return MergeMesh(InputMeshA, PointIndexListA, InputMeshB, PointIndexListB, PointDistanceThreshold,
					 PointIndexMap_InputA_to_Output, PointIndexMap_InputB_to_Output);
}


//Merge InputMeshA and InputMeshB
//The shared points are in PointIndexListA of InputMeshA and PointIndexListB of InputMeshB;
template<typename ScalarType>
PolygonMesh<ScalarType> MergeMesh(const PolygonMesh<ScalarType>& InputMeshA, const DenseVector<int_max>& PointIndexListA,
								  const PolygonMesh<ScalarType>& InputMeshB, const DenseVector<int_max>& PointIndexListB, ScalarType PointDistanceThreshold,
								  DenseVector<int_max>& PointIndexMap_InputA_to_Output, DenseVector<int_max>& PointIndexMap_InputB_to_Output)
{
	PolygonMesh<ScalarType> OutputMesh;

	PointIndexMap_InputA_to_Output.Resize(InputMeshA.GetMaxValueOfPointIndex() + 1);
	PointIndexMap_InputA_to_Output.Fill(-1);
	//PointIndexMap_InputA_to_Output[PointIndexA] is PointIndex on OutputMesh

	PointIndexMap_InputB_to_Output.Resize(InputMeshB.GetMaxValueOfPointIndex() + 1);
	PointIndexMap_InputB_to_Output.Fill(-1);
	//PointIndexMap_InputB_to_Output[PointIndexB] is PointIndex on OutputMesh

	if (InputMeshA.IsEmpty() && InputMeshB.IsEmpty())
	{
		return OutputMesh;
	}
	
	if (InputMeshA.IsEmpty() == true)
	{//copy InputMeshB
		for (int_max PointIndexB = 0; PointIndexB <= InputMeshB.GetMaxValueOfPointIndex(); ++PointIndexB)
		{
			if (InputMeshB.IsValidPointIndex(PointIndexB) == true)
			{
				auto PointIndex_out = OutputMesh.AddPoint(InputMeshB.GetPointPosition(PointIndexB));
				PointIndexMap_InputB_to_Output[PointIndexB] = PointIndex_out;
			}
		}
		for (int_max FaceIndexB = 0; FaceIndexB <= InputMeshB.GetMaxValueOfFaceIndex(); ++FaceIndexB)
		{
			if (InputMeshB.IsValidFaceIndex(FaceIndexB) == true)
			{
				auto PointIndexList = InputMeshB.Face(FaceIndexB).GetPointIndexList();
				DenseVector<int_max> PointIndexList_out;
				for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
				{
					auto PointIndex_out = PointIndexMap_InputB_to_Output[PointIndexList[n]];
					if (PointIndex_out >= 0)
					{
						PointIndexList_out.Append(PointIndex_out);
					}
				}
				OutputMesh.AddFaceByPoint(PointIndexList_out);
			}
		}
		return OutputMesh;
	}

	//now, InputMeshA is not empty and InputMeshB is not empty

	for (int_max PointIndexA = 0; PointIndexA <= InputMeshA.GetMaxValueOfPointIndex(); ++PointIndexA)
	{
			if (InputMeshA.IsValidPointIndex(PointIndexA) == true)
			{
				auto PointIndex_out = OutputMesh.AddPoint(InputMeshA.GetPointPosition(PointIndexA));
				PointIndexMap_InputA_to_Output[PointIndexA] = PointIndex_out;
			}
		}
	for (int_max FaceIndexA = 0; FaceIndexA <= InputMeshA.GetMaxValueOfFaceIndex(); ++FaceIndexA)
	{
		if (InputMeshA.IsValidFaceIndex(FaceIndexA) == true)
		{
			auto PointIndexList = InputMeshA.Face(FaceIndexA).GetPointIndexList();
			DenseVector<int_max> PointIndexList_out;
			for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
			{
				auto PointIndex_out = PointIndexMap_InputA_to_Output[PointIndexList[n]];
				if (PointIndex_out >= 0)
				{
					PointIndexList_out.Append(PointIndex_out);
				}
			}
			OutputMesh.AddFaceByPoint(PointIndexList_out);
		}
	}

	auto CurveA = InputMeshA.GetPointPosition(PointIndexListA);

	std::cout << "PointIndexListA " << PointIndexListA.GetLength() << '\n';
	std::cout << "CurveA " << CurveA.GetColCount() << '\n';

	for (int_max PointIndexB = 0; PointIndexB <= InputMeshB.GetMaxValueOfPointIndex(); ++PointIndexB)
	{
		if (InputMeshB.IsValidPointIndex(PointIndexB) == true)
		{
			bool Flag = false;
			for (int_max n = 0; n < PointIndexListB.GetLength(); ++n)
			{
				if (PointIndexB == PointIndexListB[n])
				{
					Flag = true;
					break;
				}
			}
			if (Flag == true)
			{
				auto PosB = InputMeshB.GetPointPosition(PointIndexB);
				auto Idx_near = FindNearestPointOnCurve(CurveA, PosB);
				auto PointIndexA = PointIndexListA[Idx_near];
				auto PosA = InputMeshA.GetPointPosition(PointIndexA);
				auto dist = (PosA - PosB).L2Norm();
				if (dist <= PointDistanceThreshold)
				{
					auto PointIndex_out = PointIndexMap_InputA_to_Output[PointIndexA];
					PointIndexMap_InputB_to_Output[PointIndexB] = PointIndex_out;
					auto Pos = 0.5 * (PosA + PosB);
					OutputMesh.SetPointPosition(PointIndex_out, Pos);
				}
				else
				{
					auto PointIndex_out = OutputMesh.AddPoint(PosB);
					PointIndexMap_InputB_to_Output[PointIndexB] = PointIndex_out;
				}
			}
			else
			{
				auto PosB = InputMeshB.GetPointPosition(PointIndexB);
				auto PointIndex_out = OutputMesh.AddPoint(PosB);
				PointIndexMap_InputB_to_Output[PointIndexB] = PointIndex_out;
			}
		}
	}

	for (int_max FaceIndexB = 0; FaceIndexB <= InputMeshB.GetMaxValueOfFaceIndex(); ++FaceIndexB)
	{
		if (InputMeshB.IsValidFaceIndex(FaceIndexB) == true)
		{
			auto PointIndexList = InputMeshB.Face(FaceIndexB).GetPointIndexList();
			DenseVector<int_max> PointIndexList_out;
			for (int_max n = 0; n < PointIndexList.GetLength(); ++n)
			{
				auto PointIndex_out = PointIndexMap_InputB_to_Output[PointIndexList[n]];
				if (PointIndex_out >= 0)
				{
					PointIndexList_out.Append(PointIndex_out);
				}
			}
			OutputMesh.AddFaceByPoint(PointIndexList_out);
		}
	}

	return OutputMesh;
}


}//namespace mdk
