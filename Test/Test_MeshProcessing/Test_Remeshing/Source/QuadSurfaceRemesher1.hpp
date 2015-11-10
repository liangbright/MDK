#pragma once

namespace mdk
{
template<typename ScalarType>
QuadSurfaceRemesher1<ScalarType>::QuadSurfaceRemesher1()
{
	this->Clear();
}


template<typename ScalarType>
QuadSurfaceRemesher1<ScalarType>::~QuadSurfaceRemesher1()
{
}

template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::Clear()
{
	m_InputMesh.Clear();
	m_FeatureEdgeOfInputMesh.Clear();
	m_FeaturePointOfInputMesh.Clear();
	m_Threshold_Quad_WarpAngleScore = 0.9659; // 15/360
	m_Threshold_Quad_EdgeAngleScore = 0.2929; // 45/360
	m_Threshold_Quad_AspectScore = 0.2;       // 1/5		
	this->ClearInternalData();
	m_OutputMesh_Mixed.Clear();
	m_OutputMesh_Quad.Clear();
}

template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::ClearInternalData()
{
	m_MiddlePointHandleList_on_InputMesh.Clear();
}

template<typename ScalarType>
bool QuadSurfaceRemesher1<ScalarType>::CheckInput()
{
	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("m_InputMesh is NOT TriangleMesh @ QuadSurfaceRemesher1::CheckInput()")
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ QuadSurfaceRemesher1::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ QuadSurfaceRemesher1::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->PreprocessInputMesh();
	this->TraceQuadLine();
	this->BuildMixedTriQuadMesh();
	this->AdjustMixedTriQuadMesh();
	this->BuildQuadMesh();
}

template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::PreprocessInputMesh()
{// add middle point of each edge

	m_MiddlePointHandleList_on_InputMesh.Clear();
	m_MiddlePointHandleList_on_InputMesh.Resize(m_InputMesh.GetEdgeCount());

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Edge().GetPointHandleList();
		auto Point0 = m_InputMesh.GetPointPosition(PointHandleList[0]);
		auto Point1 = m_InputMesh.GetPointPosition(PointHandleList[1]);
		auto PointM = (Point0 + Point1) / ScalarType(2);

		auto EdgeIndex = it.GetEdgeHandle().GetIndex();
		m_MiddlePointHandleList_on_InputMesh[EdgeIndex] = m_InputMesh.AddPoint(PointM);
	}
}



template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::TraceQuadLine()
{

}


template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::BuildMixedTriQuadMesh()
{
	m_OutputMesh_Mixed.Clear();
	m_OutputMesh_Mixed = m_InputMesh;

	for (int_max k = 0; k < m_QuadLineList.GetLength(); ++k)
	{
		this->BuildMixedTriQuadMesh(m_QuadLineList[k]);
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::BuildMixedTriQuadMesh(const DenseVector<PointHandleType>& QuadLine)
{
	//------------------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-P7--P0-P8-P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//-----------------------------
	//      P6----P5
	//     /|     | \
	//    / |     |  \
	//  P1 P7--P0-P8 P4
	//    \ |     |  /
	//     \|_____| /
	//      P2     P3
    //--------------------------------
	// QuadLine Direction: P1 -> P0
	//--------------------------------

	for (int_max k = 1; k < QuadLine.GetLength()-1; ++k)
	{
		PointHandleType PointH0, PointH1, PointH2, PointH3, PointH4, PointH5, PointH6, PointH7, PointH8;
		EdgeHandleType  EdgeH01, EdgeH02, EdgeH03, EdgeH04, EdgeH05, EdgeH06;
		FaceHandleType  TriangleH012, TriangleH023, TriangleH034, TriangleH045, TriangleH056, TriangleH061;
		int_max AdjacentPointCount_PointH1, AdjacentPointCount_PointH4;
		{
			//--------------------------- get P0 to P6 --------------------------------------------------//
			PointH0 = QuadLine[k];
			PointH1 = QuadLine[k-1];
			auto AdjacentPointHandleList_PointH0 = m_InputMesh.Point(PointH0).GetAdjacentPointHandleList();		
			auto AdjacentPointHandleList_PointH1 = m_InputMesh.Point(PointH1).GetAdjacentPointHandleList();
		
			if (AdjacentPointHandleList_PointH0.GetLength() != 6)
			{
				MDK_Error("AdjacentPointHandleList_PointH0.GetLength() != 6 @ QuadSurfaceRemesher1::BuildMixedTriQuadMesh(...)")
				return;
			}

			AdjacentPointCount_PointH1 = AdjacentPointHandleList_PointH1.GetLength();

			auto tempSet01 = this->Intersect(AdjacentPointHandleList_PointH0, AdjacentPointHandleList_PointH1);
			PointH2 = tempSet01[0];
			PointH6 = tempSet01[1];
			auto AdjacentPointHandleList_PointH2 = m_InputMesh.Point(PointH2).GetAdjacentPointHandleList();
			auto AdjacentPointHandleList_PointH6 = m_InputMesh.Point(PointH6).GetAdjacentPointHandleList();

			auto tempSet02 = this->Intersect(AdjacentPointHandleList_PointH0, AdjacentPointHandleList_PointH2);
			if (tempSet02[0] == PointH1)
			{
				PointH3 = tempSet02[1];
			}
			else
			{
				PointH3 = tempSet02[0];
			}
			auto AdjacentPointHandleList_PointH3 = m_InputMesh.Point(PointH3).GetAdjacentPointHandleList();
			
			auto tempSet06 = this->Intersect(AdjacentPointHandleList_PointH0, AdjacentPointHandleList_PointH6);
			if (tempSet06[0] == PointH1)
			{
				PointH5 = tempSet06[1];
			}
			else
			{
				PointH5 = tempSet06[0];
			}
			auto AdjacentPointHandleList_PointH5 = m_InputMesh.Point(PointH5).GetAdjacentPointHandleList();

			auto tempSet05 = this->Intersect(AdjacentPointHandleList_PointH0, AdjacentPointHandleList_PointH5);
			if (tempSet05[0] == PointH6)
			{
				PointH4 = tempSet05[1];
			}
			else
			{
				PointH4 = tempSet05[0];
			}
			AdjacentPointCount_PointH4 = m_InputMesh.Point(PointH4).GetAdjacentPointCount();

			//-------------------------------- Get Edge and P7, P8---------------------------------------------------//
			EdgeH01 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH1);
			EdgeH02 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH2);
			EdgeH03 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH3);
			EdgeH04 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH4);
			EdgeH05 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH5);
			EdgeH06 = m_InputMesh.GetEdgeHandleByPoint(PointH0, PointH6);

			PointH7 = m_MiddlePointHandleList_on_InputMesh[EdgeH01.GetIndex()];
			PointH8 = m_MiddlePointHandleList_on_InputMesh[EdgeH04.GetIndex()];

			//-------------------------------- Get Triangle ---------------------------------------------------//
			TriangleH012 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH1, PointH2});
			TriangleH023 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH2, PointH3});
			TriangleH034 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH3, PointH4});
			TriangleH045 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH4, PointH5});
			TriangleH056 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH5, PointH6});
			TriangleH061 = m_InputMesh.GetFaceHandleByPoint({PointH0, PointH6, PointH1});
		}


		//update m_OutputMesh_Mixed

		m_OutputMesh_Mixed.DeleteFace(TriangleH012);
		m_OutputMesh_Mixed.DeleteFace(TriangleH023);
		m_OutputMesh_Mixed.DeleteFace(TriangleH034);
		m_OutputMesh_Mixed.DeleteFace(TriangleH045);
		m_OutputMesh_Mixed.DeleteFace(TriangleH056);
		m_OutputMesh_Mixed.DeleteFace(TriangleH061);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH01);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH02);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH03);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH04);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH05);
		m_OutputMesh_Mixed.DeleteEdge(EdgeH06);

		DenseVector<FaceHandleType, 6> NewFaceHandleList;
		NewFaceHandleList[0] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH7, PointH8, PointH5, PointH6 });
		NewFaceHandleList[1] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH7, PointH2, PointH3, PointH8 });

		if (k == 1)
		{
			//if (AdjacentPointCount_PointH1 != 6)
			{
				NewFaceHandleList[2] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH1, PointH7, PointH6 });
				NewFaceHandleList[3] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH1, PointH2, PointH7 });
			}
		}
		
		if (k == QuadLine.GetLength() - 2)
		{
			//if (AdjacentPointCount_PointH4 != 6)
			{
				NewFaceHandleList[4] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH8, PointH4, PointH5 });
				NewFaceHandleList[5] = m_OutputMesh_Mixed.AddFaceByPoint({ PointH8, PointH3, PointH4 });
			}
		}
	}

}


template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::AdjustMixedTriQuadMesh()
{

}


template<typename ScalarType>
void QuadSurfaceRemesher1<ScalarType>::BuildQuadMesh()
{	// subdivide m_OutputMesh_Mixed to get m_OutputMesh_Quad



}


template<typename ScalarType>
DenseVector<typename QuadSurfaceRemesher1<ScalarType>::PointHandleType> QuadSurfaceRemesher1<ScalarType>::ConvertIndexToPointHandle(const DenseVector<int_max>& IndexList)
{
	DenseVector<PointHandleType> HandleList;
	HandleList.Resize(IndexList.GetLength());
	for (int_max k = 0; k < IndexList.GetLength(); ++k)
	{
		HandleList[k].SetIndex(IndexList[k]);
	}
	return HandleList;
}


template<typename ScalarType>
DenseVector<typename QuadSurfaceRemesher1<ScalarType>::FaceHandleType> QuadSurfaceRemesher1<ScalarType>::ConvertIndexToFaceHandle(const DenseVector<int_max>& IndexList)
{
	DenseVector<FaceHandleType> HandleList;
	HandleList.Resize(IndexList.GetLength());
	for (int_max k = 0; k < IndexList.GetLength(); ++k)
	{
		HandleList[k].SetIndex(IndexList[k]);
	}
	return HandleList;
}


template<typename ScalarType>
DenseVector<int_max> QuadSurfaceRemesher1<ScalarType>::ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList)
{
	DenseVector<int_max> IndexList;
	IndexList.Resize(HandleList.GetLength());
	for (int_max k = 0; k < HandleList.GetLength(); ++k)
	{
		IndexList[k] = HandleList[k].GetIndex();
	}
	return IndexList;
}


template<typename ScalarType>
DenseVector<int_max> QuadSurfaceRemesher1<ScalarType>::ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList)
{
	DenseVector<int_max> IndexList;
	IndexList.Resize(HandleList.GetLength());
	for (int_max k = 0; k < HandleList.GetLength(); ++k)
	{
		IndexList[k] = HandleList[k].GetIndex();
	}
	return IndexList;
}


template<typename ScalarType>
ScalarType QuadSurfaceRemesher1<ScalarType>::EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3)
{
	auto Point0 = TargetMesh.GetPointPosition(PointH0);
	auto Point1 = TargetMesh.GetPointPosition(PointH1);
	auto Point2 = TargetMesh.GetPointPosition(PointH2);
	auto Point3 = TargetMesh.GetPointPosition(PointH3);
	return this->EvaluateQuad(Point0, Point1, Point2, Point3);
}

template<typename ScalarType>
ScalarType QuadSurfaceRemesher1<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
{
	//================
	//  P3 -  P2 
	//  |     |   
	//  P0 -  P1 
	//================

	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	//warp angle
	ScalarType WarpAngleScore = 0;
	{
		auto Normal_023 = ComputeTriangleNormalIn3D(Point0, Point2, Point3);
		auto Normal_012 = ComputeTriangleNormalIn3D(Point0, Point1, Point2);
		auto CosAngle_023_012 = std::abs(ComputeVectorDotProductIn3D(Normal_012, Normal_023));

		auto Normal_013 = ComputeTriangleNormalIn3D(Point0, Point1, Point3);
		auto Normal_123 = ComputeTriangleNormalIn3D(Point1, Point2, Point3);
		auto CosAngle_013_123 = std::abs(ComputeVectorDotProductIn3D(Normal_013, Normal_123));

		WarpAngleScore = std::min(CosAngle_023_012, CosAngle_013_123);
	}

	auto P0P1 = Point1 - Point0;
	auto P1P2 = Point2 - Point1;
	auto P2P3 = Point3 - Point2;
	auto P3P0 = Point0 - Point3;
	auto P0P2 = Point2 - Point0;
	auto P1P3 = Point3 - Point1;
	auto Direction01 = P0P1 / (P0P1.L2Norm() + EPS);
	auto Direction12 = P1P2 / (P1P2.L2Norm() + EPS);
	auto Direction23 = P2P3 / (P2P3.L2Norm() + EPS);
	auto Direction30 = P3P0 / (P3P0.L2Norm() + EPS);

	ScalarType EdgeAngleScore = 0;
	DenseVector<ScalarType, 4> CosEdgeAngleList;
	CosEdgeAngleList[0] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction30));
	CosEdgeAngleList[1] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction12));
	CosEdgeAngleList[2] = std::abs(ComputeVectorDotProductIn3D(Direction12, Direction23));
	CosEdgeAngleList[3] = std::abs(ComputeVectorDotProductIn3D(Direction23, Direction30));
	EdgeAngleScore = ScalarType(1) - CosEdgeAngleList.Max();

	ScalarType AspectScore = 0;
	DenseVector<ScalarType, 6> DistanceList;
	DistanceList[0] = P0P1.L2Norm();
	DistanceList[1] = P1P2.L2Norm();
	DistanceList[2] = P2P3.L2Norm();
	DistanceList[3] = P3P0.L2Norm();
	DistanceList[4] = P0P2.L2Norm() / ScalarType(1.414);
	DistanceList[5] = P1P3.L2Norm() / ScalarType(1.414);
	auto Distance_min = DistanceList.Min();
	auto Distance_max = DistanceList.Max();
	AspectScore = Distance_min / (Distance_max + EPS);

	auto Score = WarpAngleScore + ScalarType(2)*EdgeAngleScore + AspectScore;
	Score /= ScalarType(4);
	if (WarpAngleScore < m_Threshold_Quad_WarpAngleScore || EdgeAngleScore < m_Threshold_Quad_EdgeAngleScore || AspectScore < m_Threshold_Quad_AspectScore)
	{
		//Score = 0;
	}
	return Score;
}

template<typename ScalarType>
ScalarType QuadSurfaceRemesher1<ScalarType>::EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2)
{
	//================
	//  P0  ---- P2 
	//     \    /   
	//	     P1   
	//================

	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	auto P0P1 = Point1 - Point0;
	auto P1P2 = Point2 - Point1;
	auto P2P0 = Point0 - Point2;
	auto Direction01 = P0P1 / (P0P1.L2Norm() + EPS);
	auto Direction12 = P1P2 / (P1P2.L2Norm() + EPS);
	auto Direction20 = P2P0 / (P2P0.L2Norm() + EPS);

	ScalarType EdgeAngleScore = 0;
	DenseVector<ScalarType, 3> CosEdgeAngledList;
	CosEdgeAngledList[0] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction20));
	CosEdgeAngledList[1] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction12));
	CosEdgeAngledList[2] = std::abs(ComputeVectorDotProductIn3D(Direction12, Direction20));
	EdgeAngleScore = ScalarType(1) - CosEdgeAngledList.Max();

	ScalarType Aspect = 0;
	DenseVector<ScalarType, 3> EdgeLength;
	EdgeLength[0] = P0P1.L2Norm();
	EdgeLength[1] = P1P2.L2Norm();
	EdgeLength[2] = P2P0.L2Norm();
	auto EdgeLength_min = EdgeLength.Min();
	auto EdgeLength_max = EdgeLength.Max();
	Aspect = EdgeLength_min / (EdgeLength_max + EPS);

	auto Score = (EdgeAngleScore + Aspect) / ScalarType(2);
	return Score;
}


template<typename ScalarType>
DenseVector<typename  QuadSurfaceRemesher1<ScalarType>::PointHandleType>
QuadSurfaceRemesher1<ScalarType>::Intersect(const DenseVector<PointHandleType>& HandleListA, const DenseVector<PointHandleType>& HandleListB)
{
	DenseVector<PointHandleType> HandleListC;

	if (HandleListA.IsEmpty() == true || HandleListB.IsEmpty() == true)
	{
		return HandleListC;
	}

	HandleListC.SetCapacity(std::min(HandleListA.GetLength(), HandleListB.GetLength()));

	for (int_max k = 0; k < HandleListA.GetLength(); ++k)
	{
		for (int_max n = 0; n < HandleListB.GetLength(); ++n)
		{
			if (HandleListA[k] == HandleListB[n])
			{
				HandleListC.Append(HandleListA[k]);
			}
		}
	}
	return HandleListC;
}

template<typename ScalarType>
DenseVector<typename  QuadSurfaceRemesher1<ScalarType>::FaceHandleType>
QuadSurfaceRemesher1<ScalarType>::Intersect(const DenseVector<FaceHandleType>& HandleListA, const DenseVector<FaceHandleType>& HandleListB)
{
	DenseVector<FaceHandleType> HandleListC;

	if (HandleListA.IsEmpty() == true || HandleListB.IsEmpty() == true)
	{
		return HandleListC;
	}

	HandleListC.SetCapacity(std::min(HandleListA.GetLength(), HandleListB.GetLength()));

	for (int_max k = 0; k < HandleListA.GetLength(); ++k)
	{
		for (int_max n = 0; n < HandleListB.GetLength(); ++n)
		{
			if (HandleListA[k] == HandleListB[n])
			{
				HandleListC.Append(HandleListA[k]);
			}
		}
	}
	return HandleListC;
}

}//namespace