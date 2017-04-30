#pragma once

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_start)
{
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


template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& InputMesh)
{
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


template<typename MeshAttributeType>
int_max FindNearestPointOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    typedef typename MeshAttributeType::ScalarType ScalarType;

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


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SmoothMeshByVTKSmoothPolyDataFilter(const PolygonMesh<MeshAttributeType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
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

	PolygonMesh<MeshAttributeType> OutputMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SmoothMeshByVTKWindowedSincPolyDataFilter(const PolygonMesh<MeshAttributeType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
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

	PolygonMesh<MeshAttributeType> OutputMesh;
	ConvertVTKPolyDataToMDKPolygonMesh(SmoothFilter->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename MeshAttributeType>
SparseVector<int_max> FindNeighbourPointOfPointOnMesh(const PolygonMesh<MeshAttributeType>& InputMesh, int_max PointIndex_input, int_max MaxGraphDistance)
{
	SparseVector<int_max> Output;
	//Output.IndexList[n] is PointIndex
	//Output.ElementList[n] is GraphDistance between Point(PointIndex) and Point(PointIndex_input)

	int_max PointCount = InputMesh.GetPointCount();

	//--------------- check input ------------------------//
	if (PointIndex_input < 0 || PointIndex_input > PointCount)
	{
		return Output;
	}
	
	if (InputMesh.IsEmpty() == true)
	{
		return Output;
	}

	if (MaxGraphDistance <= 0)
	{
		return Output;
	}
	else if (MaxGraphDistance == 1)
	{
		auto AdjPointIndexList = InputMesh.Point(PointIndex_input).GetAdjacentPointIndexList();
		Output.Resize(PointCount);
		for (int_max k = 0; k < AdjPointIndexList.GetLength(); ++k)
		{
			Output.SetElement(AdjPointIndexList[k], 1);
			return Output;
		}
	}
	//----------------------------------------------------//

	DenseVector<int> PointFlagList;
	PointFlagList.Resize(PointCount);
	PointFlagList.Fill(0); // 0 ~ not selected, 1 ~ selected as ouput

	DenseVector<int_max> PointIndexList_boarder;	
	PointIndexList_boarder = InputMesh.Point(PointIndex_input).GetAdjacentPointIndexList();

	Output.Resize(PointCount);
	for (int_max k = 0; k < PointIndexList_boarder.GetLength(); ++k)
	{
		Output.SetElement(PointIndexList_boarder[k], 1);
		PointFlagList[PointIndexList_boarder[k]] = 1;
	}	

	for(int_max GraphDistance = 2; GraphDistance <= MaxGraphDistance; ++GraphDistance)
	{
		DenseVector<int_max> PointIndexList_boarder_next;
		PointIndexList_boarder_next.SetCapacity(PointIndexList_boarder.GetLength() + 100);
		for (int_max k = 0; k < PointIndexList_boarder.GetLength(); ++k)
		{
			auto AdjPointIndexList = InputMesh.Point(PointIndexList_boarder[k]).GetAdjacentPointIndexList();
			for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
			{
				if (PointFlagList[AdjPointIndexList[n]] == 0)
				{
					Output.SetElement(AdjPointIndexList[n], GraphDistance);
					PointIndexList_boarder_next.Append(AdjPointIndexList[n]);
					PointFlagList[AdjPointIndexList[n]] = 1;
				}				
			}
		}
		PointIndexList_boarder = std::move(PointIndexList_boarder_next);
	}

	return Output;
}


template<typename MeshAttributeType>
DenseVector<int_max> FindFaceEnclosedByEdgeCurve(const PolygonMesh<MeshAttributeType>& Surface, const DenseVector<int_max>& ClosedEdgeCurve_EdgeIndexList, const int_max FaceIndex_seed)
{
	DenseVector<int> FaceFlagList;//1: the same group as FaceIndex_seed
	FaceFlagList.Resize(Surface.GetFaceCount());
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
			auto EdgeIndexList_k = Surface.Face(FaceIndexList_front[k]).GetEdgeIndexList();
			for (int_max n = 0; n < EdgeIndexList_k.GetLength(); ++n)
			{
				auto tempIndex = ClosedEdgeCurve_EdgeIndexList.ExactMatch("first", EdgeIndexList_k[n]);
				if (tempIndex < 0)
				{
					auto FaceIndexList_n = Surface.Edge(EdgeIndexList_k[n]).GetAdjacentFaceIndexList();
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

}//namespace mdk
