﻿#pragma once

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<int_max> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh, int_max PointIndex_start)
{
    DenseVector<int_max> PointIndexListOfBoundaryCurve;

    int_max BoundaryEdgeCountOfInputMesh = 0;
    for (auto it = TargetMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
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

    const auto& AdjacentPointIndexList_start = TargetMesh.Point(PointIndex_start).GetAdjacentPointIndexList();
    for (int_max k = 0; k < AdjacentPointIndexList_start.GetLength(); ++k)
    {
        if (TargetMesh.Point(AdjacentPointIndexList_start[k]).IsOnBoundaryEdge() == true)
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
        auto tempAdjacentEdgeIndexList_current = TargetMesh.Point(BoundaryPointIndex_current).GetAdjacentEdgeIndexList();

        int_max BoundaryPointIndex_next = -1;

        for (int_max n = 0; n < tempAdjacentEdgeIndexList_current.GetLength(); ++n)
        {
            if (TargetMesh.Edge(tempAdjacentEdgeIndexList_current[n]).IsBoundary() == true)
            {
                auto tempPointIndexList = TargetMesh.Edge(tempAdjacentEdgeIndexList_current[n]).GetPointIndexList();
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
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh)
{
    // find boundary point
    DenseVector<int_max> BoundaryPointIndexList;
    for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
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

        auto BoundaryCurve = TraceMeshBoundaryCurve(TargetMesh, BoundaryPointIndex);

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
int_max FindNearestPointOnMesh(const PolygonMesh<MeshAttributeType>& TargetMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    typedef typename MeshAttributeType::ScalarType ScalarType;

	int_max OutputPointIndex = -1;

    ScalarType Distance_sq_min = 0;
    bool IsFirstPoint = true;
    for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
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


}//namespace mdk
