#pragma once

namespace mdk
{

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh()
: PolygonMesh()
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const MDK_Symbol_PureEmpty&)
: Mesh(MDK_PureEmpty)
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const TriangleMesh<ScalarType>& InputMesh)
: PolygonMesh(InputMesh)
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(TriangleMesh<ScalarType>&& InputMesh)
: PolygonMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const PolygonMesh<ScalarType>& InputMesh)
: PolygonMesh(InputMesh)
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(PolygonMesh<ScalarType>&& InputMesh)
: PolygonMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const Mesh<ScalarType>& InputMesh)
: PolygonMesh(InputMesh)
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(Mesh<ScalarType>&& InputMesh)
: PolygonMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TriangleMesh<ScalarType>::~TriangleMesh()
{
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(const TriangleMesh<ScalarType>& InputMesh)
{
	this->PolygonMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(TriangleMesh<ScalarType>&& InputMesh)
{
	this->PolygonMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(const PolygonMesh<ScalarType>& InputMesh)
{
	this->PolygonMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
	this->PolygonMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
	this->PolygonMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->PolygonMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::AddFaceByEdge(int_max EdgeIndex0, int_max EdgeIndex1, int_max EdgeIndex2)
{
    DenseVector<EdgeIndexType> EdgeIndexList = { EdgeIndex0, EdgeIndex1, EdgeIndex2 };
    return this->PolygonMesh::AddFaceByEdge(EdgeIndexList);
}

template<typename ScalarType>
inline
int_max TriangleMesh<ScalarType>::AddFaceByPoint(int_max PointIndex0, int_max PointIndex1, int_max PointIndex2)
{
    DenseVector<int_max> PointIndexList = { PointIndex0, PointIndex1, PointIndex2 };
    return this->PolygonMesh::AddFaceByPoint(PointIndexList);
}

template<typename ScalarType>
TriangleMesh<ScalarType> TriangleMesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
	TriangleMesh<ScalarType> OutputMesh = this->PolygonMesh::GetSubMeshByFace(FaceIndexList);
	return OutputMesh;
}

//------------- Function optimized for TriangleMesh --------------------------------------------------//

template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalAtFace(const MDK_Symbol_ALL&)
{ 
	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
    for (int_max k=0; k<= FaceIndex_max; ++k)
    {
		if (this->IsValidFaceIndex(k) == true)
		{
			this->UpdateNormalAtFace(k);
		}
    }
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalAtFace(int_max FaceIndex)
{
    if (this->IsValidFaceIndex(FaceIndex) == false)
    {
        MDK_Warning("FaceIndex is invalid @ TriangleMesh::UpdateNormalAtFace()")
        return;
    }

    auto PointIndexList = this->Face(FaceIndex).GetPointIndexList();
	auto PointPositionA = this->GetPointPosition(PointIndexList[0]);
	auto PointPositionB = this->GetPointPosition(PointIndexList[1]);
	auto PointPositionC = this->GetPointPosition(PointIndexList[2]);
	auto Normal = ComputeTriangleNormalIn3D(PointPositionA, PointPositionB, PointPositionC);
    this->Face(FaceIndex).Attribute().Normal = Normal;
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAreaOfFace(const MDK_Symbol_ALL&)
{
	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_max; ++k)
	{
		if (this->IsValidFaceIndex(k) == true)
		{
			this->UpdateAreaOfFace(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAreaOfFace(int_max FaceIndex)
{
    if (this->IsValidFaceIndex(FaceIndex) == false)
    {
        MDK_Warning("FaceIndex is invalid @ TriangleMesh::UpdateAreaOfFace()")
        return;
    }

    auto PointIndexList = this->Face(FaceIndex).GetPointIndexList();
	auto PointPositionA = this->GetPointPosition(PointIndexList[0]);
	auto PointPositionB = this->GetPointPosition(PointIndexList[1]);
	auto PointPositionC = this->GetPointPosition(PointIndexList[2]);
	auto Area = ComputeTriangleAreaIn3D(PointPositionA, PointPositionB, PointPositionC);
	this->Face(FaceIndex).Attribute().Area = Area;
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateCornerAngleOfFace(const MDK_Symbol_ALL&)
{
	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_max; ++k)
	{
		if (this->IsValidFaceIndex(k) == true)
		{
			this->UpdateCornerAngleOfFace(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateCornerAngleOfFace(int_max FaceIndex)
{
    if (this->IsValidFaceIndex(FaceIndex) == false)
    {
        MDK_Warning("FaceIndex is invalid @ TriangleMesh::UpdateCornerAngleOfFace()")
        return;
    }

    auto PointIndexList = this->Face(FaceIndex).GetPointIndexList();
	auto PointPositionA = this->GetPointPosition(PointIndexList[0]);
	auto PointPositionB = this->GetPointPosition(PointIndexList[1]);
	auto PointPositionC = this->GetPointPosition(PointIndexList[2]);

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

    this->Face(FaceIndex).Attribute().CornerAngle = CornerAngle;
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAngleWeightedNormalAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo)
{//if Flag_Update_RelativeInfo is false, then FaceNormal must be available: call UpdateNormalAtFace() and UpdateCornerAngleOfFace()
	if (Flag_Update_RelativeInfo == true)
	{
		this->UpdateNormalAtFace(ALL);
		this->UpdateCornerAngleOfFace(ALL);
	}
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			this->UpdateAngleWeightedNormalAtPoint(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAngleWeightedNormalAtPoint(int_max PointIndex)
{ // FaceNormal and FaceCornerAngle must be available: call UpdateNormalAtFace() and UpdateCornerAngleOfFace()

    if (this->IsValidPointIndex(PointIndex) == false)
    {
        MDK_Warning("PointIndex is invalid @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
        return;
    }

    // compute angle weighted Normal

    auto AdjacentFaceIndexList = this->Point(PointIndex).GetAdjacentFaceIndexList();
    if (AdjacentFaceIndexList.IsEmpty() == true)
    {
		MDK_Warning("The point (PointIndex = " << PointIndex << ") has NO adjacent face @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
        this->Point(PointIndex).Attribute().AngleWeightedNormal.Fill(0);
        return;
    }

	auto AdjacentFaceCount = AdjacentFaceIndexList.GetLength();
	if (AdjacentFaceCount > 1)
	{
		auto PointPosition = this->Point(PointIndex).GetPosition();

		DenseVector<ScalarType> CornerAngleList;
		CornerAngleList.Resize(AdjacentFaceCount);

		ObjectArray<DenseVector<ScalarType>> FaceNormalTable;
		FaceNormalTable.Resize(AdjacentFaceCount);

		for (int_max k = 0; k < AdjacentFaceCount; ++k)
		{
			auto PointRelativeIndex_k = this->Face(AdjacentFaceIndexList[k]).GetRelativeIndexOfPoint(PointIndex);
			CornerAngleList[k] = this->Face(AdjacentFaceIndexList[k]).Attribute().CornerAngle[PointRelativeIndex_k];
			FaceNormalTable[k] = this->Face(AdjacentFaceIndexList[k]).Attribute().Normal;
		}

		// calculate angle weighted normal at point
		auto AngleWeight = CornerAngleList;
		auto AngleSum = AngleWeight.Sum();
		if (AngleSum <= std::numeric_limits<ScalarType>::epsilon())
		{
			MDK_Warning("AngleSum <= eps @ TriangleMesh::UpdateAngleWeightedNormalAtPoint(...)")
			AngleWeight.Fill(ScalarType(1)/ScalarType(AdjacentFaceCount));
		}
		else
		{
			AngleWeight /= AngleSum;
		}

		DenseVector<ScalarType, 3> Normal;
		Normal.Fill(0);
		for (int_max k = 0; k < AdjacentFaceCount; ++k)
		{
			Normal[0] += AngleWeight[k] * FaceNormalTable[k][0];
			Normal[1] += AngleWeight[k] * FaceNormalTable[k][1];
			Normal[2] += AngleWeight[k] * FaceNormalTable[k][2];
		}		

		this->Point(PointIndex).Attribute().AngleWeightedNormal = Normal;
	}
	else// edge point
	{
		this->Point(PointIndex).Attribute().AngleWeightedNormal = this->Face(AdjacentFaceIndexList[0]).Attribute().Normal;
	}    
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateGaussianCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo)
{//if Flag_Update_RelativeInfo is false, run UpdateCornerAngleOfFace and UpdateAreaOfFace first
	if (Flag_Update_RelativeInfo == true)
	{
		this->UpdateCornerAngleOfFace(ALL);
		this->UpdateAreaOfFace(ALL);
	}
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			this->UpdateGaussianCurvatureAtPoint(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateGaussianCurvatureAtPoint(int_max PointIndex)
{// run UpdateCornerAngleOfFace and UpdateAreaOfFace first
    if (this->IsValidPointIndex(PointIndex) == false)
    {
        MDK_Warning("PointIndex is invalid @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
        return;
    }

	//initialize
	this->Point(PointIndex).Attribute().GaussianCurvature = 0;
	this->Point(PointIndex).Attribute().WeightedGaussianCurvature = 0;

    auto AdjacentFaceIndexList = this->Point(PointIndex).GetAdjacentFaceIndexList();
    auto AdjacentFaceCount = AdjacentFaceIndexList.GetLength();
    if (AdjacentFaceCount <= 1)
    {
        //MDK_Warning("This point only has one or zero adjacent face @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
        return;
    }

	auto AdjacentPointCount = this->Point(PointIndex).GetAdjacentPointCount();

    DenseVector<ScalarType> AreaList;
	AreaList.FastResize(AdjacentPointCount);

    DenseVector<ScalarType> CornerAngleList;
	CornerAngleList.FastResize(AdjacentPointCount);

    for (int_max k = 0; k < AdjacentFaceCount; ++k)
    {
		auto PointRelativeIndex_k = this->Face(AdjacentFaceIndexList[k]).GetRelativeIndexOfPoint(PointIndex);
        CornerAngleList[k] = this->Face(AdjacentFaceIndexList[k]).Attribute().CornerAngle[PointRelativeIndex_k];
        AreaList[k] = this->Face(AdjacentFaceIndexList[k]).Attribute().Area;
    }

    if (AdjacentFaceCount != AdjacentPointCount)
    {// this point (PointIndex~b) is on boundary edge, an angle must be computed  (big angle a_b_c vs small angle a_b_c)
     //  a
	 //  |  /c
	 //  |/
     //  b
	 //-------------
        if (AdjacentFaceCount != AdjacentPointCount - 1)
        {
            MDK_Error("AdjacentFaceCount != AdjacentPointCount -1 @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
            return;
        }

        // check any edge boundary
        DenseVector<int_max> AdjacentBoundaryPointIndexList;// point a and c

        auto AdjacentEdgeIndexList = this->Point(PointIndex).GetAdjacentEdgeIndexList();
        for (int_max k = 0; k < AdjacentEdgeIndexList.GetLength(); ++k)
        {
            if (this->Edge(AdjacentEdgeIndexList[k]).IsOnPolygonMeshBoundary() == true)
            {
                int_max tempPointIndex_0, tempPointIndex_1;
                this->Edge(AdjacentEdgeIndexList[k]).GetPointIndexList(tempPointIndex_0, tempPointIndex_1);
                if (tempPointIndex_0 == PointIndex)
                {
                    AdjacentBoundaryPointIndexList.Append(tempPointIndex_1);
                }
                else
                {
                    AdjacentBoundaryPointIndexList.Append(tempPointIndex_0);
                }
            }
        }

        if (AdjacentBoundaryPointIndexList.GetLength() != 2) // point is isolated or is a endpoint of a curve
        {
			MDK_Error("AdjacentBoundaryPointIndexList length is not 2 @ TriangleMesh::UpdateGaussianCurvatureAtPoint()")
            return;
        }

		auto PointPosition = this->GetPointPosition(PointIndex);
		auto Position_a = this->GetPointPosition(AdjacentBoundaryPointIndexList[0]);
		auto Position_b = this->GetPointPosition(AdjacentBoundaryPointIndexList[1]);
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
	this->Point(PointIndex).Attribute().GaussianCurvature = GaussianCurvature;
	this->Point(PointIndex).Attribute().WeightedGaussianCurvature = WeightedGaussianCurvature;
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateMeanCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo)
{ //if Flag_Update_RelativeInfo is false, run UpdateAreaOfFace() first
	if (Flag_Update_RelativeInfo == true)
	{
		this->UpdateAreaOfFace(ALL);
	}
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			this->UpdateMeanCurvatureAtPoint(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateMeanCurvatureAtPoint(int_max PointIndex)
{ // run UpdateAreaOfFace() first

    if (this->IsValidPointIndex(PointIndex) == false)
    {
        MDK_Warning("PointIndex is invalid @ TriangleMesh::UpdateMeanCurvatureAtPoint()")
        return;
    }

    // cotangent Laplace Beltrami Operator "Polygon Mesh Processing: page 46"

    auto AdjacentPointCount = this->Point(PointIndex).GetAdjacentPointCount();
    if (AdjacentPointCount < 3)
    {
        this->Point(PointIndex).Attribute().MeanCurvatureNormal.Fill(0);
        this->Point(PointIndex).Attribute().MeanCurvature = 0;
        return;
    }

	auto PointPosition = this->GetPointPosition(PointIndex);

    DenseVector<ScalarType, 3> MeanCurvatureNormal;
    MeanCurvatureNormal.Fill(0);

    DenseVector<ScalarType> AreaList;
    AreaList.SetCapacity(AdjacentPointCount);

	DenseVector<ScalarType> CotSumList;
	CotSumList.SetCapacity(AdjacentPointCount);

    auto AdjacentEdgeIndexList = this->Point(PointIndex).GetAdjacentEdgeIndexList();
    for (int_max k = 0; k < AdjacentEdgeIndexList.GetLength(); ++k)
    {
        auto AdjacentFaceIndexList_k = this->Edge(AdjacentEdgeIndexList[k]).GetAdjacentFaceIndexList();
        if (AdjacentFaceIndexList_k.GetLength() == 2)
        {
            auto FaceIndex_a = AdjacentFaceIndexList_k[0];
            auto FaceIndex_b = AdjacentFaceIndexList_k[1];

            int_max tempPointIndex_0, tempPointIndex_1, PointIndex_k;
            this->Edge(AdjacentEdgeIndexList[k]).GetPointIndexList(tempPointIndex_0, tempPointIndex_1);
            if (tempPointIndex_0 == PointIndex)
            {
                PointIndex_k = tempPointIndex_1;
            }
            else
            {
                PointIndex_k = tempPointIndex_0;
            }

			auto Position_k = this->GetPointPosition(PointIndex_k);

            // get cotangent of the angle at PointIndex_a in FaceIndex_a
            int_max PointIndex_a;
            int_max PointRelativeIndex_a;
            auto PointIndexList_a = this->Face(FaceIndex_a).GetPointIndexList();
            if (PointIndexList_a[0] != PointIndex && PointIndexList_a[0] != PointIndex_k)
            {
                PointIndex_a = PointIndexList_a[0];
                PointRelativeIndex_a = 0;
            }
            else if (PointIndexList_a[1] != PointIndex && PointIndexList_a[1] != PointIndex_k)
            {
                PointIndex_a = PointIndexList_a[1];
                PointRelativeIndex_a = 1;
            }
            else
            {
                PointIndex_a = PointIndexList_a[2];
                PointRelativeIndex_a = 2;
            }

            auto CornerAngle_a = this->Face(FaceIndex_a).Attribute().CornerAngle[PointRelativeIndex_a];

			const auto pi = ScalarType(std::acos(-1.0));
			auto constant_pi_half = pi / ScalarType(2);
            auto Cot_a = std::tan(constant_pi_half - CornerAngle_a);

            // get cotangent of the angle at PointIndex_b in FaceIndex_b
            int_max PointIndex_b;
            int_max PointRelativeIndex_b;
            auto PointIndexList_b = this->Face(FaceIndex_b).GetPointIndexList();
            if (PointIndexList_b[0] != PointIndex && PointIndexList_b[0] != PointIndex_k)
            {
                PointIndex_b = PointIndexList_b[0];
                PointRelativeIndex_b = 0;
            }
            else if (PointIndexList_b[1] != PointIndex && PointIndexList_b[1] != PointIndex_k)
            {
                PointIndex_b = PointIndexList_b[1];
                PointRelativeIndex_b = 1;
            }
            else
            {
                PointIndex_b = PointIndexList_b[2];
                PointRelativeIndex_b = 2;
            }

            auto CornerAngle_b = this->Face(FaceIndex_b).Attribute().CornerAngle[PointRelativeIndex_b];
            auto Cot_b = std::tan(constant_pi_half - CornerAngle_b);

            auto SumOfCot = Cot_a + Cot_b;// read "Polygon Mesh Processing: page 46"
            if (SumOfCot > 0)
            {
                MeanCurvatureNormal += SumOfCot * (Position_k - PointPosition);
				CotSumList.Append(SumOfCot);
                AreaList.Append(this->Face(FaceIndex_a).Attribute().Area);
                AreaList.Append(this->Face(FaceIndex_b).Attribute().Area);
            }
        }
    }

    if (AreaList.GetLength() > 0)
    {
        // note: theoretically, mixed region should be used instead of the total region
		auto MeanCurvature = MeanCurvatureNormal.L2Norm() / AreaList.Sum();
		MeanCurvatureNormal /= CotSumList.Sum();
        //-------------------
        this->Point(PointIndex).Attribute().MeanCurvatureNormal = MeanCurvatureNormal;
        this->Point(PointIndex).Attribute().MeanCurvature = MeanCurvature;
    }
    else
    {
        this->Point(PointIndex).Attribute().MeanCurvatureNormal.Fill(0);
        this->Point(PointIndex).Attribute().MeanCurvature = 0;
    }
}

template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalBasedCurvatureAtPoint(const MDK_Symbol_ALL&, bool Flag_Update_RelativeInfo)
{ //if Flag_Update_RelativeInfo is false, run UpdateNormalAtFace first
	if (Flag_Update_RelativeInfo == true)
	{
		this->UpdateNormalAtFace(ALL);
	}
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			this->UpdateNormalBasedCurvatureAtPoint(k);
		}
	}
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalBasedCurvatureAtPoint(int_max PointIndex)
{ // run UpdateNormalAtFace first

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Warning("PointIndex is invalid @ TriangleMesh::UpdateNormalBasedCurvatureAtPoint()")
		return;
	}

	// measure the diviation of adjacent face normal 
	// if all the adjacent face normal  point to the same direction, then curvature is zero
	// NormalBasedCurvature is from 0 to 1
	// 0: __ __ two adjacent face in a flat plane 
	// 0.5: |__   two adjacent face 90' angle
	// 1:   || two adjacent face fold to be parallel to each other
	//
	// Attention:
	// if FaceA and FaceB is in the same plane and not fold, then Normal_of_FaceA and Normal_of_FaceB must be the same

	auto AdjacentFaceIndexList = this->Point(PointIndex).GetAdjacentFaceIndexList();
	auto AdjacentFaceCount = AdjacentFaceIndexList.GetLength();
	if (AdjacentFaceCount <= 1)
	{
		this->Point(PointIndex).Attribute().NormalBasedCurvature = 0;
		return;
	}

	ObjectArray<DenseVector<ScalarType, 3>> NormalList;
	NormalList.Resize(AdjacentFaceCount);
	for (int_max k = 0; k < AdjacentFaceCount; ++k)
	{
		NormalList[k] = this->Face(AdjacentFaceIndexList(k)).Attribute().Normal;
	}

	ScalarType MinProjection = 1;
	for (int_max k = 0; k < AdjacentFaceCount-1; ++k)
	{
		auto Normal_k = NormalList[k];
		for (int_max n = k+1; n < AdjacentFaceCount; ++n)
		{
			auto Normal_n = NormalList[n];
			auto projection = Normal_k[0] * Normal_n[0] + Normal_k[1] * Normal_n[1] + Normal_k[2] * Normal_n[2];
			if (MinProjection > projection)
			{
				MinProjection = projection;
			}
		}
	}
	
	this->Point(PointIndex).Attribute().NormalBasedCurvature = (ScalarType(1) - MinProjection)/ ScalarType(2);
}

//-------------------------------------- mesh editing -----------------------------------------------------//

template<typename ScalarType>
bool TriangleMesh<ScalarType>::CollapseEdge(int_max EdgeIndex01, int_max PointIndex0, bool Flag_HandleSpecialCase1, bool Flag_HandleSpecialCase2)
{
	if (this->IsValidEdgeIndex(EdgeIndex01) == false)
	{
		MDK_Error("Invalid EdgeIndex01 @ TriangleMesh::CollapseEdge(...)")
		return false;
	}
	if (this->IsValidPointIndex(PointIndex0) == false)
	{
		MDK_Error("Invalid PointIndex0 @ TriangleMesh::CollapseEdge(...)")
		return false;
	}
	auto PointIndexList_Edge01 = this->Edge(EdgeIndex01).GetPointIndexList();
	if (PointIndexList_Edge01[0] != PointIndex0 && PointIndexList_Edge01[1] != PointIndex0)
	{
		MDK_Error("Point must be on the Edge @ TriangleMesh::CollapseEdge(...)")
		return false;
	}
	//-----------------------------------------------------------------------------------
	//Upper: Triangle A
	//Lower: Triangle B
	//   2
	//0 /_\ 1    /
	//  \ /  =>  \
    //   3
	// Triangle012 Triangle031, Point1, Edge01, Edge21, Edge31, will be deleted
	//-----------------------------------------------------------------------------------
	//we can just operater on each triangle adj to Edge01
	//Triangle A
	//    2
	// 0 /_\ 1  =>  /		
	// Triangle012, Point1, Edge01, Edge21 will be deleted
	//-----------------------------------------------------------------------------------
	//special case 1: Trangle {0,1,2}, {0,2,3}, {1, 3, 2}
	//Edge30 and Edge31 are NOT on mesh boundary
	//collapse Edge01 will produce 2 idential triangle {0, 2, 3} and {0, 3, 2}	
	//      3
	//     /|\ 
	//	  / 2 \
	//   / /_\ \
	//   0     1 
	//-----------------------------------------------------------------------------------
	//special case 2a: Edge20 and Edge21 are on boundary
	//collapse Edge01 will produce an Edge connecting 2 and 0_1		
	//   2         2
	//  /_\   =>   |
	// 0   1       0
	//------------------------------------------------------------------------------------	
	//special case 2b : Edge30, Edge31 on boundary
	//      3
	//     /|\ 
	//	  / 2 \
	//   / /_\ \
	//   0     1 
    //------------------------------------------------------------------------------------	
	// nested special case 1 or 2
	//      4
	//     /|\
	//    / 3 \
	//   / /|\ \ 
	//  / / 2 \ \
	// / / /_\ \ \
	//  0       1 
	// Point3, 4, ... in H3_PointIndexList
	// if Edge40 and Edge41 are NOT on mesh bounary, then it is special case 1
	// else,  then it is special case 2
	//------------------------------------------------------------------------------------
	//This function can handle an isolated face connecting to the rest by only one point	
	//Collapse 1->0
	// mesh       mesh 
	//  \ /        \ / 
	//   2          2  
	//  /_\   => (no edge)    
	// 0   1        0
	//------------------------------------------------------------------------------------
	//Collapse Edge01 (1->0) may generate isolated face
	//if the Edge01 is not on bounary, but Point0 and Point1 on bounary
	//avoid this in application
	//------------------------------------------------------------------------------------
	int_max H0, H1;
	if (PointIndexList_Edge01[0] == PointIndex0)
	{
		H0 = PointIndexList_Edge01[0];
		H1 = PointIndexList_Edge01[1];
	}
	else
	{
		H0 = PointIndexList_Edge01[1];
		H1 = PointIndexList_Edge01[0];
	}
	//------------------------------------------------------------------------------------
	auto AdjFaceIndexList_H1 = this->Point(H1).GetAdjacentFaceIndexList();
	auto AdjEdgeIndexList_H1 = this->Point(H1).GetAdjacentEdgeIndexList();
	auto AdjFaceIndexList_Edge01 = this->Edge(EdgeIndex01).GetAdjacentFaceIndexList();		
	//------------------------------------------------------------------------------------
	DenseVector<int_max> H2_PointIndexList;
	//H2_PointIndexList[k] is H2 in AdjFaceIndexList_Edge01[k]
	//(H2, FaceIndex012) = (H2_PointIndexList[k], AdjFaceIndexList_Edge01[k])
	for (int_max k = 0; k < AdjFaceIndexList_Edge01.GetLength(); ++k)
	{
		int_max FaceIndex012 = AdjFaceIndexList_Edge01[k];
		auto PointIndexList_k = this->Face(FaceIndex012).GetPointIndexList();
		int_max H2 = -1;
		for (int_max n = 0; n < PointIndexList_k.GetLength(); ++n)
		{
			if (PointIndexList_k[n] != PointIndexList_Edge01[0] && PointIndexList_k[n] != PointIndexList_Edge01[1])
			{
				H2 = PointIndexList_k[n];
				break;
			}
		}
		if (H2 < 0)
		{
			MDK_Error("H2 < 0 @ TriangleMesh::CollapseEdge(...)")
			return false;
		}
		H2_PointIndexList.Append(H2);
	}

	//debug
	{
		auto IdxList = H2_PointIndexList.FindUnique();
		if (IdxList.GetLength() != H2_PointIndexList.GetLength())
		{
			auto AdjFaceIndexList_H2 = this->Point(H2_PointIndexList[0]).GetAdjacentFaceIndexList();
			auto PointIndexList0 = this->Face(AdjFaceIndexList_Edge01[0]).GetPointIndexList();
			auto PointIndexList1 = this->Face(AdjFaceIndexList_Edge01[1]).GetPointIndexList();
			auto Flag_Edge01_Boundary = this->Edge(EdgeIndex01).IsOnPolygonMeshBoundary();
			MDK_Error("H2_PointIndexList has duplicated element, please check input mesh @ TriangleMesh::CollapseEdge(...)")
			//return false;
		}
	}

	//------------------------------------------------------------------------------------
	auto TempFunction_FindNeighborPoint = [&](int_max H, const DenseVector<int_max>& PointIndexSet)
	{
		for (int_max k = 0; k < PointIndexSet.GetLength(); ++k)
		{
			if (H != PointIndexSet[k])
			{
				auto EdgeIndex_k = this->GetEdgeIndexByPoint(H, PointIndexSet[k]);
				if (EdgeIndex_k >= 0)
				{
					return PointIndexSet[k];
				}
			}
		}
		return int_max(-1);
	};
	//------------------------------------------------------------------------------------
	auto TempFunction_FindNeighborChain = [&](int_max H, const DenseVector<int_max>& PointIndexSet)
	{
		DenseVector<int_max> Chain;
		auto H_start = TempFunction_FindNeighborPoint(H, PointIndexSet);
		if (H_start < 0)
		{
			return Chain;
		}
		Chain.Append(H_start);
		int_max H_pre = H;
		int_max H_next = H_start;		
		for (int_max n = 0; n < PointIndexSet.GetLength(); ++n)
		{
			auto Hn = TempFunction_FindNeighborPoint(H_next, PointIndexSet);
			if (Hn >= 0 && Hn != H_pre)
			{
				Chain.Append(Hn);
				H_pre = H_next;
				H_next = Hn;
			}
			else
			{
				break;
			}
		}
		return Chain;
	};
	//------------------------------------------------------------------------------------
	//      4     
	//     /|\    
	//    / 3 \   
	//   / /|\ \  
    //  / / 2 \ \ 
	// / / /_\ \ \
	//  0       1  
	//---------------------------------
	// case 1 or 2
	//----------------------------------
	// find H3H4Chain[k]={H3, H4,...} for each H2=H2_PointIndexList[k]
	//--------------------------
	DenseVector<int_max> H2H3H4_PointIndexSet;//contain H2, H3, H4...
	{
		//case1 and case2
		auto AdjPointIndexList_H0 = this->Point(H0).GetAdjacentPointIndexList();//include H1		
		auto AdjPointIndexList_H1 = this->Point(H1).GetAdjacentPointIndexList();//include H0		
		H2H3H4_PointIndexSet = Intersect(AdjPointIndexList_H0, AdjPointIndexList_H1);		
		//delete H0 and H1
		auto tempIdx0 = H2H3H4_PointIndexSet.ExactMatch(H0);
		H2H3H4_PointIndexSet.Delete(tempIdx0);
		auto tempIdx1 = H2H3H4_PointIndexSet.ExactMatch(H1);
		H2H3H4_PointIndexSet.Delete(tempIdx1);
	}
	//------------------------------------------------------------------------------------
	auto H3H4_PointIndexSet = SetDiff(H2H3H4_PointIndexSet, H2_PointIndexList);	
	DenseVector<DenseVector<int_max>> H3H4Chain;
	H3H4Chain.Resize(H2_PointIndexList.GetLength());
	for (int_max k = 0; k < H2_PointIndexList.GetLength(); ++k)
	{
		auto H2 = H2_PointIndexList[k];
		H3H4Chain[k] = TempFunction_FindNeighborChain(H2, H3H4_PointIndexSet);
	}
	//------------------------------------------------------------------------------------
	DenseVector<int_max> EdgeIndexList_delete;
	EdgeIndexList_delete.Append(EdgeIndex01);
	for (int_max k = 0; k < H2H3H4_PointIndexSet.GetLength(); ++k)
	{
		auto tempEdgeIndex = this->GetEdgeIndexByPoint(H2H3H4_PointIndexSet[k], H1);
		EdgeIndexList_delete.Append(tempEdgeIndex);
	}
	//------------------------------------------------------------------------------------
	auto TempFunction_HandleNormalCase = [&](int_max H2, int_max FaceIndex012)
	{//    2
	 // 0 /_\ 1  =>  /		
	 // Face012, Edge21 will be deleted; 
	 // Point1 and Edge01 will NOT be deleted in this function (deleted in TempFunction_End)
	 // Face (not Face012) adj to Point1 will NOT be modified in this function
	 //-----------------------------------------------------------------------------------	 
		this->DeleteFace(FaceIndex012);
        //
		auto EdgeIndex20 = this->GetEdgeIndexByPoint(H2, H0);
		auto EdgeIndex21 = this->GetEdgeIndexByPoint(H2, H1);		
		//modify Face (Face012 is deleted) adjacent to Edge21: replace Edge21 with Edge20
		auto AdjFaceIndexList_Edge21 = this->Edge(EdgeIndex21).GetAdjacentFaceIndexList();
		for (int_max k = 0; k < AdjFaceIndexList_Edge21.GetLength(); ++k)
		{
			auto& EdgeIndexList_k = this->Face(AdjFaceIndexList_Edge21[k]).EdgeIndexList();
			auto tempIdx = EdgeIndexList_k.ExactMatch("first", EdgeIndex21);
			EdgeIndexList_k[tempIdx] = EdgeIndex20;
		}
		//modify Edge20: add face (Face012 is deleted) adjacent to Edge21
		auto& AdjFaceIndexList_Edge20_ref = this->Edge(EdgeIndex20).AdjacentFaceIndexList();
		{
			AdjFaceIndexList_Edge20_ref.Append(AdjFaceIndexList_Edge21);
			AdjFaceIndexList_Edge20_ref = AdjFaceIndexList_Edge20_ref.GetSubSet(AdjFaceIndexList_Edge20_ref.FindUnique());
		}
		//delete Edge21
		this->Edge(EdgeIndex21).AdjacentFaceIndexList().Clear();
		this->DeleteEdge(EdgeIndex21);
	};
	//------------------------------------------------------------------------------------
	auto TempFunction_HandleSpecialCase1 = [&](int_max H2, int_max FaceIndex012, const DenseVector<int_max>& H3H4List)
	{//     4                    4
	//     /|\                  / \
	//    / 3 \                /   \
	//   / /|\ \     =>       /     \
	//  / / 2 \ \            /       \
	// / / /_\ \ \          /_________\
	//  0       1           0         1
	//---------------------------------
	//H3H4List={H3, H4, ...}=H3H4Chain[n]
	//---------------------------------		
		//Handle Point2
		{
			auto AdjFaceIndex_H2 = this->Point(H2).GetAdjacentFaceIndexList();
			auto AdjEdgeIndex_H2 = this->Point(H2).GetAdjacentEdgeIndexList();
			this->DeleteFace(AdjFaceIndex_H2);
			this->DeleteEdge(AdjEdgeIndex_H2);
			this->DeletePoint(H2);
		}
		for (int_max k = 0; k < H3H4List.GetLength() - 1; ++k)
		{
			auto AdjFaceIndex_k = this->Point(H3H4List[k]).GetAdjacentFaceIndexList();
			auto AdjEdgeIndex_k = this->Point(H3H4List[k]).GetAdjacentEdgeIndexList();
			this->DeleteFace(AdjFaceIndex_k);
			this->DeleteEdge(AdjEdgeIndex_k);
			this->DeletePoint(H3H4List[k]);
		}
		auto PointIndex_end = H3H4List[H3H4List.GetLength() - 1];
		auto FaceIndex_end = this->GetFaceIndexByPoint({ H0, H1, PointIndex_end });
		TempFunction_HandleNormalCase(PointIndex_end, FaceIndex_end);
	};
	//------------------------------------------------------------------------------------
	auto TempFunction_HandleSpecialCase2 = [&](int_max H2, int_max FaceIndex012, const DenseVector<int_max>& H3H4List)
	{//H3H4List is the same as that in TempFunction_HandleSpecialCase1
		if (H3H4List.IsEmpty() == true)//Case2a
		{	//-----------------------
			//   2        2      (2)
			//  /_\   =>  |   =>
			// 0   1      0       0
			//-----------------------
			TempFunction_HandleNormalCase(H2, FaceIndex012);
			auto EdgeIndex20 = this->GetEdgeIndexByPoint(H2, H0);
			this->DeleteEdge(EdgeIndex20);
			if (this->Point(H2).GetAdjacentEdgeCount() == 0)
			{//just in case Face012 is connected to a mesh only by Point2
				this->DeletePoint(H2);
			}
		}
		else //H3H4List is not empty, Case2b
		{
			{
				auto AdjFaceIndex_H2 = this->Point(H2).GetAdjacentFaceIndexList();
				auto AdjEdgeIndex_H2 = this->Point(H2).GetAdjacentEdgeIndexList();
				this->DeleteFace(AdjFaceIndex_H2);
				this->DeleteEdge(AdjEdgeIndex_H2);
				this->DeletePoint(H2);
			}
			for (int_max k = 0; k < H3H4List.GetLength() - 1; ++k)
			{
				auto AdjFaceIndex_k = this->Point(H3H4List[k]).GetAdjacentFaceIndexList();
				auto AdjEdgeIndex_k = this->Point(H3H4List[k]).GetAdjacentEdgeIndexList();
				this->DeleteFace(AdjFaceIndex_k);
				this->DeleteEdge(AdjEdgeIndex_k);
				this->DeletePoint(H3H4List[k]);
			}
			auto H_end = H3H4List[H3H4List.GetLength() - 1];
			auto FaceIndex_end = this->GetFaceIndexByPoint({ H0, H1, H_end });
			//-----------------------
			//  end       end     (end)
			//  /_\   =>   |   => 
			// 0   1       0       0
			//-----------------------
			TempFunction_HandleNormalCase(H_end, FaceIndex_end);
			auto EdgeIndex_end = this->GetEdgeIndexByPoint(H_end, H0);
			this->DeleteEdge(EdgeIndex_end);
			if (this->Point(H_end).GetAdjacentEdgeCount() == 0)
			{//just in case Face_end is connected to a mesh only by Point_end
				this->DeletePoint(H_end);
			}
		}
	};
	//------------------------------------------------------------------------------------
	auto TempFunction_End = [&]()
	{		
		//modify Edge (not Edge_delete) adjacent to Point1: replace H1 with H0		
		auto AdjEdgeIndexList_H1_valid = SetDiff(AdjEdgeIndexList_H1, EdgeIndexList_delete);
		for (int_max k = 0; k < AdjEdgeIndexList_H1_valid.GetLength(); ++k)
		{
			auto PointIndexList_k = this->Edge(AdjEdgeIndexList_H1_valid[k]).GetPointIndexList();
			if (PointIndexList_k[0] == H1)
			{
				PointIndexList_k[0] = H0;
			}
			else
			{
				PointIndexList_k[1] = H0;
			}
			this->Edge(AdjEdgeIndexList_H1_valid[k]).SetPointIndexList(PointIndexList_k);
		}
		
		//modify Point0: add edge (not deleted) adjacent to Point1			
		auto& AdjEdgeIndexList_H0_ref = this->Point(H0).AdjacentEdgeIndexList();
		AdjEdgeIndexList_H0_ref.Append(AdjEdgeIndexList_H1_valid);

		//modify Face (not AdjFaceIndexList_Edge01) adjacent to Point1: replace H1 with H0						
		auto AdjFaceIndexList_H1_valid = SetDiff(AdjFaceIndexList_H1, AdjFaceIndexList_Edge01);			
		for (int_max k = 0; k < AdjFaceIndexList_H1_valid.GetLength(); ++k)
		{
			auto& PointIndexList_k_ref = this->Face(AdjFaceIndexList_H1_valid[k]).PointIndexList();
			auto tempIdx = PointIndexList_k_ref.ExactMatch("first", H1);
			PointIndexList_k_ref[tempIdx] = H0;		
		}		

		//delete Edge01
		auto Empty_AdjacentFaceIndexList_Edge01 = this->Edge(EdgeIndex01).AdjacentFaceIndexList();
		this->DeleteEdge(EdgeIndex01);

		//delete Point1
		this->Point(H1).AdjacentEdgeIndexList().Clear();
		this->DeletePoint(H1);
	};
	//------------------------------------------------------------------------------------
	//------------------------------- end of TempFunction---------------------------------------------------------//	
	if (Flag_HandleSpecialCase1 == false || Flag_HandleSpecialCase2 == false)
	{
		for (int_max n = 0; n < H2_PointIndexList.GetLength(); ++n)
		{
			int_max H2 = H2_PointIndexList[n];
			int_max FaceIndex012 = AdjFaceIndexList_Edge01[n];
			auto H3H4Chain_n = H3H4Chain[n];
			if (H3H4Chain_n.GetLength() > 0)
			{
				bool Flag_SpecialCase1 = false;
				bool Flag_SpecialCase2 = false;
				int_max H_end = H3H4Chain_n[H3H4Chain_n.GetLength() - 1];
				if (this->Point(H_end).IsOnPolygonMeshBoundary() == true)
				{
					int_max EdgeIndex_0 = this->GetEdgeIndexByPoint(H_end, H0);
					int_max EdgeIndex_1 = this->GetEdgeIndexByPoint(H_end, H1);
					if (this->Edge(EdgeIndex_0).IsOnPolygonMeshBoundary() == true && this->Edge(EdgeIndex_1).IsOnPolygonMeshBoundary() == true)
					{
						Flag_SpecialCase2 = true;
					}
					else
					{
						Flag_SpecialCase1 = true;
					}
				}
				else
				{
					Flag_SpecialCase1 = true;
				}

				if (Flag_SpecialCase1 == true)
				{
					int_max H3 = H3H4Chain_n[0];
					MDK_Warning("H0=" << H0 << ", H1=" << H1 << ", H2=" << H2 << ", H3=" << H3)
					MDK_Warning("Special Case 1 is detected, Flag_HandleSpecialCase is false, abort @ TriangleMesh::CollapseEdge(...)")
					return false;
				}

				if (Flag_SpecialCase2 == true)
				{
					MDK_Warning("H0=" << H0 << ", H1=" << H1 << ", H2=" << H2)
					MDK_Warning("Special Case 2 is detected, Flag_HandleSpecialCase is false, abort @ TriangleMesh::CollapseEdge(...)")
					return false;
				}
			}
			else
			{
				bool Flag_SpecialCase2 = false;
				if (this->Point(H2).IsOnPolygonMeshBoundary() == true)
				{
					int_max EdgeIndex_0 = this->GetEdgeIndexByPoint(H2, H0);
					int_max EdgeIndex_1 = this->GetEdgeIndexByPoint(H2, H1);
					if (this->Edge(EdgeIndex_0).IsOnPolygonMeshBoundary() == true && this->Edge(EdgeIndex_1).IsOnPolygonMeshBoundary() == true)
					{
						Flag_SpecialCase2 = true;
					}
				}
				if (Flag_SpecialCase2 == true)
				{
					MDK_Warning("H0=" << H0 << ", H1=" << H1 << ", H2=" << H2)
					MDK_Warning("Special Case 2 is detected, Flag_HandleSpecialCase is false, abort @ TriangleMesh::CollapseEdge(...)")
					return false;
				}
			}
		}

		for (int_max n = 0; n < H2_PointIndexList.GetLength(); ++n)
		{
			int_max H2 = H2_PointIndexList[n];
			int_max FaceIndex012 = AdjFaceIndexList_Edge01[n];			
			TempFunction_HandleNormalCase(H2, FaceIndex012);
		}
		TempFunction_End();
		return true;
	}

	//now: Flag_HandleSpecialCase1 is true and Flag_HandleSpecialCase2 is true ---------------------------------------------------------------------

	for (int_max n = 0; n < H2_PointIndexList.GetLength(); ++n)
	{
		int_max H2 = H2_PointIndexList[n];
		int_max FaceIndex012 = AdjFaceIndexList_Edge01[n];
		auto H3H4Chain_n = H3H4Chain[n];
		if (H3H4Chain_n.GetLength() > 0)
		{
			bool Flag_SpecialCase1 = false;
			bool Flag_SpecialCase2 = false;
			int_max H_end = H3H4Chain_n[H3H4Chain_n.GetLength() - 1];
			if (this->Point(H_end).IsOnPolygonMeshBoundary() == true)
			{
				int_max EdgeIndex_0 = this->GetEdgeIndexByPoint(H_end, H0);
				int_max EdgeIndex_1 = this->GetEdgeIndexByPoint(H_end, H1);
				if (this->Edge(EdgeIndex_0).IsOnPolygonMeshBoundary() == true && this->Edge(EdgeIndex_1).IsOnPolygonMeshBoundary() == true)
				{
					Flag_SpecialCase2 = true;
				}
				else
				{
					Flag_SpecialCase1 = true;
				}
			}
			else
			{
				Flag_SpecialCase1 = true;
			}

			if (Flag_SpecialCase1 == true)
			{
				TempFunction_HandleSpecialCase1(H2, FaceIndex012, H3H4Chain_n);
			}

			if (Flag_SpecialCase2 == true)
			{
				if (this->Point(H_end).GetAdjacentEdgeCount() > 2)
				{
					MDK_Warning("isolated face may be generated @ TriangleMesh::CollapseEdge(...)")
				}
				TempFunction_HandleSpecialCase2(H2, FaceIndex012, H3H4Chain_n);
			}
		}
		else
		{			
			bool Flag_SpecialCase2 = false;
			if (this->Point(H2).IsOnPolygonMeshBoundary() == true)
			{
				int_max EdgeIndex_0 = this->GetEdgeIndexByPoint(H2, H0);
				int_max EdgeIndex_1 = this->GetEdgeIndexByPoint(H2, H1);
				if (this->Edge(EdgeIndex_0).IsOnPolygonMeshBoundary() == true && this->Edge(EdgeIndex_1).IsOnPolygonMeshBoundary() == true)
				{
					Flag_SpecialCase2 = true;
				}				
			}
			if (Flag_SpecialCase2 == true)
			{
				if (this->Point(H2).GetAdjacentEdgeCount() > 2)
				{
					MDK_Warning("isolated face may be generated @ TriangleMesh::CollapseEdge(...)")
				}
				TempFunction_HandleSpecialCase2(H2, FaceIndex012, H3H4Chain_n);
			}
			else
			{
				TempFunction_HandleNormalCase(H2, FaceIndex012);
			}
		}
	}
	TempFunction_End();
	return true;
}


template<typename ScalarType>
int_max TriangleMesh<ScalarType>::FlipEdge(int_max EdgeIndex01, bool Flag_ReuseEdgeIndex, bool Flag_ReuseFaceIndex)
{//only support 2 triangle face sharing an endge
	if (this->IsValidEdgeIndex(EdgeIndex01) == false)
	{
		MDK_Error("Invalid EdgeIndex01 @ TriangleMesh::FlipEdge(...)")
		return -1;
	}
	//--------------------
	//Upper: Triangle A
	//Lower: Triangle B
	//flip edge01
	//   2
	//0 /_\ 1    / | \
	//  \ /  =>  \ | /
	//   3
	//-------------------
	//special case 1 : do not flip
	//Trangle {0,1,2}, {0,2,3}, {1, 3, 2}
	//flip Edge01 will produce at least a floating triangle {0, 2, 3}
	//detection: check if point 2 and 3 are already connected
	//      2
	//     /|\ 
	//	  / 0 \
	//   / /_\ \
	//   3     1 
	//-------------------
	//special case 2 : do not flip
	// Edge has less/more than 2 adj face (e.g. on boundary)
	//---------------------

	auto PointIndexList01 = this->Edge(EdgeIndex01).GetPointIndexList();
	int_max H0 = PointIndexList01[0];
	int_max H1 = PointIndexList01[1];
	//check special case 2
	auto FaceIndexListAB = this->Edge(EdgeIndex01).GetAdjacentFaceIndexList();
	if (FaceIndexListAB.GetLength() != 2)
	{
		MDK_Warning("AdjacentFaceCount is " << FaceIndexListAB.GetLength() << " != 2, abort @ TriangleMesh::FlipEdge(...)")
		return false;
	}
	int_max FaceIndexA = -1;
	int_max FaceIndexB = -1;
	{//assume normal direction is consistant
		auto tempList = this->Face(FaceIndexListAB[0]).GetPointIndexList_LeadBy(H0);
		if (tempList[1] == H1)
		{// assume (0,1,2) and (0,3,1)
			FaceIndexA = FaceIndexListAB[0];
			FaceIndexB = FaceIndexListAB[1];
		}
		else
		{
			FaceIndexA = FaceIndexListAB[1];
			FaceIndexB = FaceIndexListAB[0];
		}
	}
	int_max H2 = -1;
	int_max H3 = -1;
	{
		auto tempListA = this->Face(FaceIndexA).GetPointIndexList();
		for (int_max k = 0; k < tempListA.GetLength(); ++k)
		{
			if (tempListA[k] != H0 && tempListA[k] != H1)
			{
				H2 = tempListA[k];
				break;
			}
		}
		auto tempListB = this->Face(FaceIndexB).GetPointIndexList();
		for (int_max k = 0; k < tempListB.GetLength(); ++k)
		{
			if (tempListB[k] != H0 && tempListB[k] != H1)
			{
				H3 = tempListB[k];
				break;
			}
		}
		if (H2 == H3)
		{
			MDK_Error("H2 and H3 are the same @ TriangleMesh::FlipEdge(...)")
			return -1;
		}
	}
	
	//check special case 1
	bool Flag_SpecialCase1 = false;
	{
		auto EdgeIndex23 = this->GetEdgeIndexByPoint(H2, H3);
		if (EdgeIndex23 >= 0)
		{
			Flag_SpecialCase1 = true;
			MDK_Warning("Special Case 1 is detected, abort @ TriangleMesh::FlipEdge(...)")
			return -1;
		}
	}

	//now, ok to go --------------------------------------------------

	this->DeleteFace(FaceIndexA);
	this->DeleteFace(FaceIndexB);
	this->DeleteEdge(EdgeIndex01);

	int_max EdgeIndex_flipped = -1;
	if (Flag_ReuseEdgeIndex == true)
	{
		EdgeIndex_flipped = this->AddEdge(H2, H3, EdgeIndex01);
	}
	else
	{
		EdgeIndex_flipped = this->AddEdge(H2, H3);
	}

	if (Flag_ReuseFaceIndex == true)
	{
		this->AddFaceByPoint({ H0, H3, H2 }, FaceIndexA);
		this->AddFaceByPoint({ H1, H2, H3 }, FaceIndexB);
	}
	else
	{
		this->AddFaceByPoint({ H0, H3, H2 });
		this->AddFaceByPoint({ H1, H2, H3 });
	}
	return EdgeIndex_flipped;

}


template<typename ScalarType>
DenseVector<int_max> TriangleMesh<ScalarType>::SplitFaceAtEdge(int_max EdgeIndex12, int_max PointIndex0)
{
//one/two/more face share Edge12
//split Edge12 at Point0
//       3             3
//      / \           /|\ 
//    1/_0_\2  =>   1/_0_\2
//     \   /         \ | /
//      \ /           \|/
//       4             4
//-------------------------------
//operator on each face 
//       3             3
//      / \           /|\ 
//    1/_0_\2  =>   1/_0_\2
//point order 1->2->3 in Face012
//------------------------------

	DenseVector<int_max> FaceIndexList_output;

	if (this->IsValidEdgeIndex(EdgeIndex12) == false)
	{
		MDK_Error("Invalid EdgeIndex12, abort @ TriangleMesh::SplitFaceAtEdge(...)")
		return FaceIndexList_output;
	}

	if (this->IsValidPointIndex(PointIndex0) == false)
	{
		MDK_Error("Invalid PointIndex0, abort @ TriangleMesh::SplitFaceAtEdge(...)")
		return FaceIndexList_output;
	}

	auto PointIndexList_Edge12 = this->Edge(EdgeIndex12).GetPointIndexList();
	if (PointIndexList_Edge12[0] == PointIndex0 || PointIndexList_Edge12[1] == PointIndex0)
	{
		MDK_Error("Input Edge contain Input Point @ TriangleMesh::SplitFaceAtEdge(...)")
		return FaceIndexList_output;
	}

	auto AdjFaceIndexList_Edge12 = this->Edge(EdgeIndex12).GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjFaceIndexList_Edge12.GetLength(); ++k)
	{
		auto FaceIndex123 = AdjFaceIndexList_Edge12[k];
		auto H0 = PointIndex0;		
		int_max H1 = -1;
		int_max H2 = -1;
		auto PointIndexList_Face123 = this->Face(FaceIndex123).GetPointIndexList_LeadBy(PointIndexList_Edge12[0]);
		if (PointIndexList_Face123[1] == PointIndexList_Edge12[1])
		{
			H1 = PointIndexList_Edge12[0];
			H2 = PointIndexList_Edge12[1];
		}
		else
		{
			H1 = PointIndexList_Edge12[1];
			H2 = PointIndexList_Edge12[0];
		}
		int_max H3 = -1;
		if (PointIndexList_Face123[0] != H1 && PointIndexList_Face123[0] != H2)
		{
			H3 = PointIndexList_Face123[0];
		}
		else if (PointIndexList_Face123[1] != H1 && PointIndexList_Face123[1] != H2)
		{
			H3 = PointIndexList_Face123[1];
		}
		else
		{
			H3 = PointIndexList_Face123[2];
		}		
		this->DeleteFace(FaceIndex123);
		auto FaceIndex103 = this->AddFaceByPoint(H1, H0, H3);
		auto FaceIndex023 = this->AddFaceByPoint(H0, H2, H3);
		FaceIndexList_output.Append(FaceIndex103);
		FaceIndexList_output.Append(FaceIndex023);
	}
	this->DeleteEdge(EdgeIndex12);
	return FaceIndexList_output;
}


template<typename ScalarType>
DenseVector<int_max> TriangleMesh<ScalarType>::SplitFaceAtEdge(int_max EdgeIndex12)
{
	DenseVector<int_max> FaceIndexList_output;
	if (this->IsValidEdgeIndex(EdgeIndex12) == false)
	{
		MDK_Error("Invalid EdgeIndex12 @ TriangleMesh::SplitFaceAtEdge(...)")
		return FaceIndexList_output;
	}
	auto PointIndexList = this->Edge(EdgeIndex12).GetPointIndexList();
	auto PosA = this->GetPointPosition(PointIndexList[0]);
	auto PosB = this->GetPointPosition(PointIndexList[1]);
	auto PosC = (PosA + PosB)*ScalarType(0.5);
	auto PointIndex0 = this->AddPoint(PosC);
	FaceIndexList_output = this->SplitFaceAtEdge(EdgeIndex12, PointIndex0);
	return FaceIndexList_output;
}

}// namespace mdk
