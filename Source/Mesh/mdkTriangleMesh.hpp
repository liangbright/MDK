﻿#ifndef mdk_TriangleMesh_hpp
#define mdk_TriangleMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh()
: PolygonMesh()
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(const TriangleMesh<MeshAttributeType>& InputMesh)
: PolygonMesh(InputMesh)
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(TriangleMesh<MeshAttributeType>&& InputMesh)
: PolygonMesh(std::move(InputMesh))
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::~TriangleMesh()
{
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->PolygonMesh::operator=(InputMesh);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    this->PolygonMesh::operator=(std::move(InputMesh));
}

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_MembraneMesh
TriangleMesh<MeshAttributeType>::AddCellByEdge(Handle_Of_Edge_Of_MembraneMesh EdgeHandle0,
                                               Handle_Of_Edge_Of_MembraneMesh EdgeHandle1, 
                                               Handle_Of_Edge_Of_MembraneMesh EdgeHandle2)
{
    DenseVector<EdgeHandleType> EdgeHandleList = { EdgeHandle0, EdgeHandle1, EdgeHandle2 };
    return this->PolygonMesh::AddCellByEdge(EdgeHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_MembraneMesh
TriangleMesh<MeshAttributeType>::AddCellByEdge(int_max EdgeID0, int_max EdgeID1, int_max EdgeID2)
{
    DenseVector<int_max> EdgeIDList = { EdgeID0, EdgeID1, EdgeID2 };
    return this->PolygonMesh::AddCellByEdge(EdgeIDList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_MembraneMesh
TriangleMesh<MeshAttributeType>::AddCellByPoint(Handle_Of_Point_Of_MembraneMesh PointHandle0,
                                                Handle_Of_Point_Of_MembraneMesh PointHandle1, 
                                                Handle_Of_Point_Of_MembraneMesh PointHandle2)
{
    DenseVector<PointHandleType> PointHandleList = { PointHandle0, PointHandle1, PointHandle2 };
    return this->PolygonMesh::AddCellByPoint(PointHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_MembraneMesh
TriangleMesh<MeshAttributeType>::AddCellByPoint(int_max PointID0, int_max PointID1, int_max PointID2)
{
    DenseVector<int_max> PointIDList = { PointID0, PointID1, PointID2 };
    return this->PolygonMesh::AddCellByPoint(PointIDList);
}

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh)
{
    auto InputMeshPtr = static_cast<TriangleMesh<MeshAttributeType>*>(&InputPolygonMesh);
    m_MeshData = std::move(InputMeshPtr->m_MeshData);
}


template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
    if (this->IsEmpty() == true)
    {
        return false;
    }

    for (auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        auto CellHandle = it.GetCellHandle();
        auto PointCount = this->Cell(CellHandle).GetPointCount();
        if (PointCount != 3)
        {
            return false;
        }
    }

    return true;
}

// get a sub mesh by CellHandleList or CellIDList----------------------------//

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const
{
    TriangleMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->PolygonMesh<MeshAttributeType>::GetSubMeshByCell(CellHandleList));
    return OutputMesh;
}

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const
{
    TriangleMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->PolygonMesh<MeshAttributeType>::GetSubMeshByCell(CellIDList));
    return OutputMesh;
}

//------------- Function optimized for TriangleMesh --------------------------------------------------//

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell() // all
{ 
    for (auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        this->UpdateNormalAtCell(it.GetCellHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell(CellHandleType CellHandle)
{
    if (this->IsValidHandle(CellHandle) == false)
    {
        MDK_Warning("CellHandle is invalid @ TriangleMesh::UpdateNormalAtCell()")
        return;
    }

    auto PointHandleList = this->Cell(CellHandle).GetPointHandleList();
	auto PointPositionA = this->GetPointPosition(PointHandleList[0]);
	auto PointPositionB = this->GetPointPosition(PointHandleList[1]);
	auto PointPositionC = this->GetPointPosition(PointHandleList[2]);
	auto Normal = ComputeTriangleNormalIn3D(PointPositionA, PointPositionB, PointPositionC);
    if (this->Cell(CellHandle).Attribute().Flag_ReverseNormalDirection == true)
    {
        Normal[0] = -Normal[0];
        Normal[1] = -Normal[1];
        Normal[2] = -Normal[2];
    }
    this->Cell(CellHandle).Attribute().Normal = Normal;
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell(int_max CellID)
{
    this->UpdateNormalAtCell(this->GetCellHandleByID(CellID));
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAreaOfCell() // all
{
    for (auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        this->UpdateAreaOfCell(it.GetCellHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAreaOfCell(CellHandleType CellHandle)
{
    if (this->IsValidHandle(CellHandle) == false)
    {
        MDK_Warning("CellHandle is invalid @ TriangleMesh::UpdateAreaOfCell()")
        return;
    }

    auto PointHandleList = this->Cell(CellHandle).GetPointHandleList();
	auto PointPositionA = this->GetPointPosition(PointHandleList[0]);
	auto PointPositionB = this->GetPointPosition(PointHandleList[1]);
	auto PointPositionC = this->GetPointPosition(PointHandleList[2]);
	auto Area = ComputeTriangleAreaIn3D(PointPositionA, PointPositionB, PointPositionC);
	this->Cell(CellHandle).Attribute().Area = Area;
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAreaOfCell(int_max CellID)
{
    this->UpdateAreaOfCell(this->GetCellHandleByID(CellID));
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateCornerAngleOfCell() // all
{
    for (auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        this->UpdateCornerAngleOfCell(it.GetCellHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateCornerAngleOfCell(CellHandleType CellHandle)
{
    if (this->IsValidHandle(CellHandle) == false)
    {
        MDK_Warning("CellHandle is invalid @ TriangleMesh::UpdateCornerAngleOfCell()")
        return;
    }

    auto PointHandleList = this->Cell(CellHandle).GetPointHandleList();
	auto PointPositionA = this->GetPointPosition(PointHandleList[0]);
	auto PointPositionB = this->GetPointPosition(PointHandleList[1]);
	auto PointPositionC = this->GetPointPosition(PointHandleList[2]);

    DenseVector<ScalarType, 3> CornerAngle;
    //----------------------------------------------
    auto Vector_AB = PointPositionB - PointPositionA;
    auto Vector_AC = PointPositionC - PointPositionA;
    CornerAngle[0] = ComputeAngleBetweenTwoVectorIn3D(Vector_AB, Vector_AC);
    //----------------------------------------------
    auto Vector_BA = PointPositionA - PointPositionB;
    auto Vector_BC = PointPositionC - PointPositionB;
    CornerAngle[1] = ComputeAngleBetweenTwoVectorIn3D(Vector_BA, Vector_BC);
    //---------------------------------    
    auto Vector_CA = PointPositionA - PointPositionC;
    auto Vector_CB = PointPositionB - PointPositionC;
    CornerAngle[2] = ComputeAngleBetweenTwoVectorIn3D(Vector_CA, Vector_CB);
    //---------------------------------

    this->Cell(CellHandle).Attribute().CornerAngle = CornerAngle;
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateCornerAngleOfCell(int_max CellID)
{
    this->UpdateCornerAngleOfCell(this->GetCellHandleByID(CellID));
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAngleWeightedNormalAtPoint() // all
{// CellNormal must be available: call UpdateNormalAtCell() and UpdateCornerAngleOfCell()
    for (auto it = this->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        this->UpdateAngleWeightedNormalAtPoint(it.GetPointHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAngleWeightedNormalAtPoint(PointHandleType PointHandle)
{ // CellNormal and CellCornerAngle must be available: call UpdateNormalAtCell() and UpdateCornerAngleOfCell()

    if (this->IsValidHandle(PointHandle) == false)
    {
        MDK_Warning("PointHandle is invalid @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
        return;
    }

    // compute angle weighted Normal

    auto AdjacentCellHandleList = this->Point(PointHandle).GetAdjacentCellHandleList();
    if (AdjacentCellHandleList.IsEmpty() == true)
    {
		MDK_Warning("The point (PointIndex = " << PointHandle.GetIndex() << ") has NO adjacent cell @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
        this->Point(PointHandle).Attribute().AngleWeightedNormal.Fill(0);
        return;
    }

	auto AdjacentCellCount = AdjacentCellHandleList.GetLength();
	if (AdjacentCellCount > 1)
	{
		auto PointPosition = this->Point(PointHandle).GetPosition();

		DenseVector<ScalarType> CornerAngleList;
		CornerAngleList.Resize(AdjacentCellCount);

		ObjectArray<DenseVector<ScalarType>> CellNormalTable;
		CellNormalTable.Resize(AdjacentCellCount);

		for (int_max k = 0; k < AdjacentCellCount; ++k)
		{
			auto PointRelativeIndex_k = this->Cell(AdjacentCellHandleList[k]).GetRelativeIndexOfPoint(PointHandle);
			CornerAngleList[k] = this->Cell(AdjacentCellHandleList[k]).Attribute().CornerAngle[PointRelativeIndex_k];
			CellNormalTable[k] = this->Cell(AdjacentCellHandleList[k]).Attribute().Normal;
		}

		// calculate angle weighted normal at point
		auto AngleWeight = CornerAngleList;
		auto AngleSum = AngleWeight.Sum();
		if (AngleSum <= std::numeric_limits<ScalarType>::epsilon())
		{
			MDK_Warning("AngleSum <= eps @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
			AngleWeight.Fill(ScalarType(1)/ScalarType(AdjacentCellCount));
		}
		else
		{
			AngleWeight /= AngleSum;
		}

		DenseVector<ScalarType, 3> Normal;
		Normal.Fill(0);
		for (int_max k = 0; k < AdjacentCellCount; ++k)
		{
			Normal[0] += AngleWeight[k] * CellNormalTable[k][0];
			Normal[1] += AngleWeight[k] * CellNormalTable[k][1];
			Normal[2] += AngleWeight[k] * CellNormalTable[k][2];
		}		

		this->Point(PointHandle).Attribute().AngleWeightedNormal = Normal;
	}
	else// edge point
	{
		this->Point(PointHandle).Attribute().AngleWeightedNormal = this->Cell(AdjacentCellHandleList[0]).Attribute().Normal;
	}    
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateAngleWeightedNormalAtPoint(int_max PointID)
{
    this->UpdateAngleWeightedNormalAtPoint(this->GetPointHandleByID(PointID));
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateGaussianCurvatureAtPoint() // all
{// run UpdateCornerAngleOfCell and UpdateAreaOfCell first
    for (auto it = this->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        this->UpdateGaussianCurvatureAtPoint(it.GetPointHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle)
{// run UpdateCornerAngleOfCell and UpdateAreaOfCell first
    if (this->IsValidHandle(PointHandle) == false)
    {
        MDK_Warning("PointHandle is invalid @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
        return;
    }

    auto AdjacentCellHandleList = this->Point(PointHandle).GetAdjacentCellHandleList();
    auto AdjacentCellCount = AdjacentCellHandleList.GetLength();
    if (AdjacentCellCount <= 1)
    {
        //MDK_Warning("This point only has one or zero adjacent cell @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
        this->Point(PointHandle).Attribute().GaussianCurvature = 0;
		this->Point(PointHandle).Attribute().WeightedGaussianCurvature = 0;
        return;
    }

	auto AdjacentPointCount = this->Point(PointHandle).GetAdjacentPointCount();

    DenseVector<ScalarType> AreaList;
	AreaList.FastResize(AdjacentPointCount);

    DenseVector<ScalarType> CornerAngleList;
	CornerAngleList.FastResize(AdjacentPointCount);

    for (int_max k = 0; k < AdjacentCellCount; ++k)
    {
		auto PointRelativeIndex_k = this->Cell(AdjacentCellHandleList[k]).GetRelativeIndexOfPoint(PointHandle);
        CornerAngleList[k] = this->Cell(AdjacentCellHandleList[k]).Attribute().CornerAngle[PointRelativeIndex_k];
        AreaList[k] = this->Cell(AdjacentCellHandleList[k]).Attribute().Area;
    }

    if (AdjacentCellCount != AdjacentPointCount)
    {// this point (e.g. b) is on boundary edge, an angle must be computed  (big angle a_b_c vs small angle a_b_c)
     //  a
	 //  |  /c
	 //  |/
     //  b

        if (AdjacentCellCount != AdjacentPointCount - 1)
        {
            MDK_Error("AdjacentCellCount != AdjacentPointCount -1 @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
            return;
        }

        // check any edge boundary
        DenseVector<PointHandleType> AdjacentBoundaryPointHandleList;

        auto AdjacentEdgeHandleList = this->Point(PointHandle).GetAdjacentEdgeHandleList();
        for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
        {
            if (this->Edge(AdjacentEdgeHandleList[k]).IsBoundary() == true)
            {
                PointHandleType tempPointHandle_0, tempPointHandle_1;
                this->Edge(AdjacentEdgeHandleList[k]).GetPointHandleList(tempPointHandle_0, tempPointHandle_1);
                if (tempPointHandle_0 == PointHandle)
                {
                    AdjacentBoundaryPointHandleList.Append(tempPointHandle_1);
                }
                else
                {
                    AdjacentBoundaryPointHandleList.Append(tempPointHandle_0);
                }
            }
        }

        if (AdjacentBoundaryPointHandleList.GetLength() != 2) // point is isolated or is a endpoint of a curve
        {
            MDK_Error("AdjacentBoundaryPointHandleList length is not 2 @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
            return;
        }

		auto PointPosition = this->GetPointPosition(PointHandle);
		auto Position_a = this->GetPointPosition(AdjacentBoundaryPointHandleList[0]);
		auto Position_b = this->GetPointPosition(AdjacentBoundaryPointHandleList[1]);
        auto Vector_a = Position_a - PointPosition;
        auto Vector_b = Position_b - PointPosition;
        CornerAngleList[AdjacentPointCount - 1] = ComputeAngleBetweenTwoVectorIn3D(Vector_a, Vector_b);
        AreaList[AdjacentPointCount - 1] = ComputeTriangleAreaIn3D(PointPosition, Position_a, Position_b);
    }

    // calculate Gaussian curvature
	auto constant_pi = ScalarType(std::acos(-1.0));
	auto GaussianCurvature = 2 * constant_pi - CornerAngleList.Sum();
	auto WeightedGaussianCurvature = GaussianCurvature / AreaList.Sum();
    //------------------------------
	this->Point(PointHandle).Attribute().GaussianCurvature = GaussianCurvature;
	this->Point(PointHandle).Attribute().WeightedGaussianCurvature = WeightedGaussianCurvature;
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateGaussianCurvatureAtPoint(int_max PointID)
{
    this->UpdateGaussianCurvatureAtPoint(this->GetPointHandleByID(PointID));
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateMeanCurvatureAtPoint() // all
{ // run UpdateAreaOfCell() first
    for (auto it = this->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        this->UpdateMeanCurvatureAtPoint(it.GetPointHandle());
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateMeanCurvatureAtPoint(PointHandleType PointHandle)
{ // run UpdateAreaOfCell() first

    if (this->IsValidHandle(PointHandle) == false)
    {
        MDK_Warning("PointHandle is invalid @ TriangleMesh::UpdateMeanCurvatureAtPoint()")
        return;
    }

    // cotangent Laplace Beltrami Operator "Polygon Mesh Processing: page 46"

    auto AdjacentPointCount = this->Point(PointHandle).GetAdjacentPointCount();
    if (AdjacentPointCount < 3)
    {
        this->Point(PointHandle).Attribute().MeanCurvatureNormal.Fill(0);
        this->Point(PointHandle).Attribute().MeanCurvature = 0;
        return;
    }

	auto PointPosition = this->GetPointPosition(PointHandle);

    DenseVector<ScalarType, 3> MeanCurvatureNormal;
    MeanCurvatureNormal.Fill(0);

    DenseVector<ScalarType> AreaList;
    AreaList.ReserveCapacity(AdjacentPointCount);

	DenseVector<ScalarType> CotSumList;
	CotSumList.ReserveCapacity(AdjacentPointCount);

    auto AdjacentEdgeHandleList = this->Point(PointHandle).GetAdjacentEdgeHandleList();
    for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
    {
        auto AdjacentCellHandleList_k = this->Edge(AdjacentEdgeHandleList[k]).GetAdjacentCellHandleList();
        if (AdjacentCellHandleList_k.GetLength() == 2)
        {
            auto CellHandle_a = AdjacentCellHandleList_k[0];
            auto CellHandle_b = AdjacentCellHandleList_k[1];

            PointHandleType tempPointHandle_0, tempPointHandle_1, PointHandle_k;
            this->Edge(AdjacentEdgeHandleList[k]).GetPointHandleList(tempPointHandle_0, tempPointHandle_1);
            if (tempPointHandle_0 == PointHandle)
            {
                PointHandle_k = tempPointHandle_1;
            }
            else
            {
                PointHandle_k = tempPointHandle_0;
            }

			auto Position_k = this->GetPointPosition(PointHandle_k);

            // get cotangent of the angle at PointHandle_a in CellHandle_a
            PointHandleType PointHandle_a;
            int_max PointRelativeIndex_a;
            auto PointHandleList_a = this->Cell(CellHandle_a).GetPointHandleList();
            if (PointHandleList_a[0] != PointHandle && PointHandleList_a[0] != PointHandle_k)
            {
                PointHandle_a = PointHandleList_a[0];
                PointRelativeIndex_a = 0;
            }
            else if (PointHandleList_a[1] != PointHandle && PointHandleList_a[1] != PointHandle_k)
            {
                PointHandle_a = PointHandleList_a[1];
                PointRelativeIndex_a = 1;
            }
            else
            {
                PointHandle_a = PointHandleList_a[2];
                PointRelativeIndex_a = 2;
            }

            auto CornerAngle_a = this->Cell(CellHandle_a).Attribute().CornerAngle[PointRelativeIndex_a];

			const auto pi = ScalarType(std::acos(-1.0));
			auto constant_pi_half = pi / ScalarType(2);
            auto Cot_a = std::tan(constant_pi_half - CornerAngle_a);

            // get cotangent of the angle at PointHandle_b in CellHandle_b
            PointHandleType PointHandle_b;
            int_max PointRelativeIndex_b;
            auto PointHandleList_b = this->Cell(CellHandle_b).GetPointHandleList();
            if (PointHandleList_b[0] != PointHandle && PointHandleList_b[0] != PointHandle_k)
            {
                PointHandle_b = PointHandleList_b[0];
                PointRelativeIndex_b = 0;
            }
            else if (PointHandleList_b[1] != PointHandle && PointHandleList_b[1] != PointHandle_k)
            {
                PointHandle_b = PointHandleList_b[1];
                PointRelativeIndex_b = 1;
            }
            else
            {
                PointHandle_b = PointHandleList_b[2];
                PointRelativeIndex_b = 2;
            }

            auto CornerAngle_b = this->Cell(CellHandle_b).Attribute().CornerAngle[PointRelativeIndex_b];
            auto Cot_b = std::tan(constant_pi_half - CornerAngle_b);

            auto SumOfCot = Cot_a + Cot_b;// read "Polygon Mesh Processing: page 46"
            if (SumOfCot > 0)
            {
                MeanCurvatureNormal += SumOfCot * (Position_k - PointPosition);
				CotSumList.Append(SumOfCot);
                AreaList.Append(this->Cell(CellHandle_a).Attribute().Area);
                AreaList.Append(this->Cell(CellHandle_b).Attribute().Area);
            }
        }
    }

    if (AreaList.GetLength() > 0)
    {
        // note: theoretically, mixed region should be used instead of the total region
		auto MeanCurvature = MeanCurvatureNormal.L2Norm() / AreaList.Sum();
		MeanCurvatureNormal /= CotSumList.Sum();
        //-------------------
        this->Point(PointHandle).Attribute().MeanCurvatureNormal = MeanCurvatureNormal;
        this->Point(PointHandle).Attribute().MeanCurvature = MeanCurvature;
    }
    else
    {
        this->Point(PointHandle).Attribute().MeanCurvatureNormal.Fill(0);
        this->Point(PointHandle).Attribute().MeanCurvature = 0;
    }
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateMeanCurvatureAtPoint(int_max PointID)
{
    this->UpdateMeanCurvatureAtPoint(this->GetPointHandleByID(PointID));
}

}// namespace mdk

#endif