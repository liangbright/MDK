#ifndef mdk_PolygonMeshProcessing_Function_Part_0_hpp
#define mdk_PolygonMeshProcessing_Function_Part_0_hpp

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh, Handle_Of_Point_Of_MembraneMesh PointHandle_start)
{
    typedef Handle_Of_Point_Of_MembraneMesh PointHandleType;

    DenseVector<PointHandleType> PointHandleListOfBoundaryCurve;

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
        return PointHandleListOfBoundaryCurve;
    }

    PointHandleListOfBoundaryCurve.SetCapacity(BoundaryEdgeCountOfInputMesh);

    PointHandleListOfBoundaryCurve.Append(PointHandle_start);

    auto BoundaryPointHandle_prev = PointHandle_start;
    PointHandleType BoundaryPointHandle_current;

    const auto& AdjacentPointHandleList_start = TargetMesh.Point(PointHandle_start).GetAdjacentPointHandleList();
    for (int_max k = 0; k < AdjacentPointHandleList_start.GetLength(); ++k)
    {
        if (TargetMesh.Point(AdjacentPointHandleList_start[k]).IsOnBoundaryEdge() == true)
        {
            BoundaryPointHandle_current = AdjacentPointHandleList_start[k];
            break;
        }
    }

    if (BoundaryPointHandle_current.GetIndex() < 0)
    {
        MDK_Error("Something is wrong with input mesh @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
        return PointHandleListOfBoundaryCurve;
    }

    PointHandleListOfBoundaryCurve.Append(BoundaryPointHandle_current);

    int_max Counter = 2;

    while (true)
    {
        auto tempAdjacentEdgeHandleList_current = TargetMesh.Point(BoundaryPointHandle_current).GetAdjacentEdgeHandleList();

        PointHandleType BoundaryPointHandle_next;

        for (int_max n = 0; n < tempAdjacentEdgeHandleList_current.GetLength(); ++n)
        {
            if (TargetMesh.Edge(tempAdjacentEdgeHandleList_current[n]).IsBoundary() == true)
            {
                auto tempPointHandleList = TargetMesh.Edge(tempAdjacentEdgeHandleList_current[n]).GetPointHandleList();
                if (tempPointHandleList[0] != BoundaryPointHandle_current && tempPointHandleList[0] != BoundaryPointHandle_prev)
                {
                    BoundaryPointHandle_next = tempPointHandleList[0];
                }
                else if (tempPointHandleList[1] != BoundaryPointHandle_current && tempPointHandleList[1] != BoundaryPointHandle_prev)
                {
                    BoundaryPointHandle_next = tempPointHandleList[1];
                }
            }
        }

        if (BoundaryPointHandle_next.GetIndex() < 0)
        {
            MDK_Error("BoundaryPointHandle.GetIndex() < 0 @ mdkPolygonMeshProcessing TraceMeshBoundaryCurvey(...)")
            break;
        }

        if (BoundaryPointHandle_next == PointHandle_start)
        {
            break; // while
        }

		//check if BoundaryPointHandle_next is already in PointHandleListOfBoundaryCurve
		auto tempIndex = PointHandleListOfBoundaryCurve.ExactMatch("first", BoundaryPointHandle_next);
		if (tempIndex >= 0)
		{
			MDK_Warning("Boundary Curve selfintersect ! @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
			break;//while
		}

		PointHandleListOfBoundaryCurve.Append(BoundaryPointHandle_next);

        BoundaryPointHandle_prev = BoundaryPointHandle_current;
        BoundaryPointHandle_current = BoundaryPointHandle_next;
        Counter += 1;

        if (Counter > BoundaryEdgeCountOfInputMesh)
        {
            MDK_Error("Counter > BoundaryEdgeCountOfInputMesh @ mdkPolygonMeshProcessing TraceMeshBoundaryCurve(...)")
            break; // while
        }
    }
    //-----------------------------------------------------------------------------------------------------

    return PointHandleListOfBoundaryCurve;
}


template<typename MeshAttributeType>
ObjectArray<DenseVector<Handle_Of_Point_Of_MembraneMesh>> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh)
{
    typedef Handle_Of_Point_Of_MembraneMesh PointHandleType;

    // find boundary point
    DenseVector<PointHandleType> BoundaryPointHandleList;
    for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        if (it.Point().IsOnBoundaryEdge() == true)
        {
            BoundaryPointHandleList.Append(it.GetPointHandle());
        }
    }
    auto BoundaryPointCount = BoundaryPointHandleList.GetLength();

    DenseVector<int_max> FlagList;
    FlagList.Resize(BoundaryPointCount);
    FlagList.Fill(0);
    // 0: not checked
    // 1: checked

	ObjectArray<DenseVector<PointHandleType>> BoundaryCurveTable;

    while (true)
    {
        // pick a boundary point and track boundary
        PointHandleType BoundaryPointHandle;
        for (int_max k = 0; k < FlagList.GetLength(); ++k)
        {
            if (FlagList[k] == 0)
            {
                BoundaryPointHandle = BoundaryPointHandleList[k];
                break;
            }
        }

        if (BoundaryPointHandle.GetIndex() < 0)
        {
            break;
        }

        auto BoundaryCurve = TraceMeshBoundaryCurve(TargetMesh, BoundaryPointHandle);

        // set flag
        for (int_max k = 0; k < BoundaryCurve.GetLength(); ++k)
        {
			auto tempIndex = BoundaryPointHandleList.ExactMatch("first", BoundaryCurve[k]);
            FlagList[tempIndex] = 1;
        }

        BoundaryCurveTable.Append(std::move(BoundaryCurve));
    }

    return BoundaryCurveTable;
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh FindNearestPointOnMesh(const PolygonMesh<MeshAttributeType>& TargetMesh,
                                                       const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    typedef Handle_Of_Point_Of_MembraneMesh PointHandleType;
    typedef typename MeshAttributeType::ScalarType ScalarType;

    PointHandleType OutputPointHandle;
    OutputPointHandle.SetToInvalid();

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
			OutputPointHandle = it.GetPointHandle();
            IsFirstPoint = false;
        }
        else
        {
            if (Distance_sq < Distance_sq_min)
            {
                Distance_sq_min = Distance_sq;
                OutputPointHandle = it.GetPointHandle();
            }
        }
    }

    return OutputPointHandle;
}

}//namespace mdk


#endif