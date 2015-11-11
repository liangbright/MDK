#pragma once

namespace mdk
{
template<typename ScalarType>
QuadSurfaceRemesher2<ScalarType>::QuadSurfaceRemesher2()
{
	this->Clear();
}


template<typename ScalarType>
QuadSurfaceRemesher2<ScalarType>::~QuadSurfaceRemesher2()
{
}

template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::Clear()
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
void QuadSurfaceRemesher2<ScalarType>::ClearInternalData()
{
	m_MiddlePointHandleList_on_InputMesh.Clear();
	m_QuadCandidatePairList.Clear();
	m_QuadCandidatePairScoreList.Clear();
	m_SmallTriangleList_Near_QuadCandidatePair.Clear();
	m_QuadCandidatePairCenterPointHandleList.Clear();
	m_QuadCandidatePairIndexSet_Overlap_SmallTriangle.Clear();
	m_QuadCandidatePairConflictTable.Clear();
}

template<typename ScalarType>
bool QuadSurfaceRemesher2<ScalarType>::CheckInput()
{
	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("m_InputMesh is NOT TriangleMesh @ QuadSurfaceRemesher2::CheckInput()")
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ QuadSurfaceRemesher2::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ QuadSurfaceRemesher2::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->PreprocessInputMesh();
	this->GenerateCandidate();
	this->EvaluateCandidate();
	this->SelectCandidate();
	this->BuildMixedTriQuadMesh();
	this->AdjustMixedTriQuadMesh();
	this->BuildQuadMesh();
}

template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::PreprocessInputMesh()
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
void QuadSurfaceRemesher2<ScalarType>::BuildSmallTriangleMesh()
{// add middle point of each edge

	m_SmallTriangleMesh.Clear();
	m_SmallTriangleMesh.SetCapacity(m_InputMesh.GetPointCount(), m_InputMesh.GetEdgeCount() + 3 * m_InputMesh.GetFaceCount(), 6 * m_InputMesh.GetFaceCount());

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		m_SmallTriangleMesh.AddPoint(it.Point().GetPosition());
	}

	for (auto it = m_InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{   //-------------------------
		//        P0
		//      /    \
		//     Pa     Pc
		//    /         \
		//   P1---Pb----P2
		//-------------------------		
		//generate small triangle candidate

		auto EdgeHandleList = it.Face().GetEdgeHandleList();
		auto PointHandleList = it.Face().GetPointHandleList();
		auto PointH0 = PointHandleList[0];
		auto PointH1 = PointHandleList[1];
		auto PointH2 = PointHandleList[2];
		auto PointHa = m_MiddlePointHandleList_on_InputMesh[EdgeHandleList[0].GetIndex()];
		auto PointHb = m_MiddlePointHandleList_on_InputMesh[EdgeHandleList[1].GetIndex()];
		auto PointHc = m_MiddlePointHandleList_on_InputMesh[EdgeHandleList[2].GetIndex()];

		//small triangle
		DenseVector<PointHandleType> SmallTriangle0, SmallTriangle1, SmallTriangle2, SmallTriangle3, SmallTriangle4, SmallTriangle5;
		SmallTriangle0 = { PointH0, PointH1, PointHb };
		SmallTriangle1 = { PointH0, PointHb, PointH2 };
		SmallTriangle2 = { PointH1, PointHc, PointH0 };
		SmallTriangle3 = { PointH1, PointH2, PointHc };
		SmallTriangle4 = { PointH2, PointH0, PointHa };
		SmallTriangle5 = { PointH2, PointHa, PointH1 };

		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle0);
		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle1);
		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle2);
		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle3);
		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle4);
		m_SmallTriangleMesh.AddFaceByPoint(SmallTriangle5);
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::GenerateCandidate()
{
	auto PointCount_input = m_InputMesh.GetPointCount();

	m_QuadCandidatePairList.Clear();
	m_QuadCandidatePairList.SetCapacity(PointCount_input);

	m_SmallTriangleList_Near_QuadCandidatePair.Clear();
	m_SmallTriangleList_Near_QuadCandidatePair.SetCapacity(PointCount_input);

	m_QuadCandidatePairCenterPointHandleList.Clear();
	m_QuadCandidatePairCenterPointHandleList.SetCapacity(PointCount_input);

	m_QuadCandidatePairScoreList.Clear();
	m_QuadCandidatePairScoreList.SetCapacity(PointCount_input);

	m_QuadCandidatePairIndexSet_Overlap_SmallTriangle.Clear();
	m_QuadCandidatePairIndexSet_Overlap_SmallTriangle.Resize(6*m_InputMesh.GetFaceCount());

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() == 6)
		{
			this->GenerateCandidate(it.GetPointHandle());
		}
	}

	m_QuadCandidatePairIndicatorList.Clear();
	m_QuadCandidatePairIndicatorList.Resize(m_QuadCandidatePairList.GetLength());
	m_QuadCandidatePairIndicatorList.Fill(-1);
}

template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::GenerateCandidate(PointHandleType PointHandle_input)
{
	// P0 is PointHandle_input
	//----------------------------------------------------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------------------------------------------------
	// Middle Point: Pm01, Pm02, Pm03, Pm94, Pm05, Pm06
	//               Pm12, Pm23, Pm34, Pm45, Pm56, Pm61
	// QuadPair-0: {P5, P6, Pm01, Pm04} and {P2, P3, Pm04, Pm01}
	// QuadPair-1: {P6, P1, Pm02, Pm05} and {P3, P4, Pm05, Pm02}         
	// QuadPair-2: {P4, P5, Pm06, Pm03} and {P1, P2, Pm03, Pm06}
	//----------------------------------------------------------------
	
	PointHandleType P0, P1, P2, P3, P4, P5, P6, Pm01, Pm02, Pm03, Pm04, Pm05, Pm06, Pm12, Pm23, Pm34, Pm45, Pm56, Pm61;

	//--------------------------- get P0 to P6 --------------------------------------------------//
	P0 = PointHandle_input;
	auto AdjacentPointHandleList_P0 = m_InputMesh.Point(P0).GetAdjacentPointHandleList();

	P1 = AdjacentPointHandleList_P0[0];
	auto AdjacentPointHandleList_P1 = m_InputMesh.Point(P1).GetAdjacentPointHandleList();

	auto tempSet01 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P1);
    P2 = tempSet01[0];
	P6 = tempSet01[1];
	auto AdjacentPointHandleList_P2 = m_InputMesh.Point(P2).GetAdjacentPointHandleList();
	auto AdjacentPointHandleList_P6 = m_InputMesh.Point(P6).GetAdjacentPointHandleList();

	auto tempSet02 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P2);
	if (tempSet02[0] == P1)
	{
		P3 = tempSet02[1];
	}
	else
	{
		P3 = tempSet02[0];
	}
	auto AdjacentPointHandleList_P3 = m_InputMesh.Point(P3).GetAdjacentPointHandleList();

	auto tempSet06 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P6);
	if (tempSet06[0] == P1)
	{
		P5 = tempSet06[1];
	}
	else
	{
		P5 = tempSet06[0];
	}
	auto AdjacentPointHandleList_P5 = m_InputMesh.Point(P5).GetAdjacentPointHandleList();

	auto tempSet05 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P5);
	if (tempSet05[0] == P6)
	{
		P4 = tempSet05[1];
	}
	else
	{
		P4 = tempSet05[0];
	}
	
	//-------------------------------- Get Edge and Middle Point---------------------------------------------------//
	auto E01 = m_InputMesh.GetEdgeHandleByPoint(P0, P1);
	auto E02 = m_InputMesh.GetEdgeHandleByPoint(P0, P2);
	auto E03 = m_InputMesh.GetEdgeHandleByPoint(P0, P3);
	auto E04 = m_InputMesh.GetEdgeHandleByPoint(P0, P4);
	auto E05 = m_InputMesh.GetEdgeHandleByPoint(P0, P5);
	auto E06 = m_InputMesh.GetEdgeHandleByPoint(P0, P6);
	auto E12 = m_InputMesh.GetEdgeHandleByPoint(P1, P2);
	auto E23 = m_InputMesh.GetEdgeHandleByPoint(P2, P3);
	auto E34 = m_InputMesh.GetEdgeHandleByPoint(P3, P4);
	auto E45 = m_InputMesh.GetEdgeHandleByPoint(P4, P5);
	auto E56 = m_InputMesh.GetEdgeHandleByPoint(P5, P6);
	auto E61 = m_InputMesh.GetEdgeHandleByPoint(P6, P1);

	Pm01 = m_MiddlePointHandleList_on_InputMesh[E01.GetIndex()];
	Pm02 = m_MiddlePointHandleList_on_InputMesh[E02.GetIndex()];
	Pm03 = m_MiddlePointHandleList_on_InputMesh[E03.GetIndex()];
	Pm04 = m_MiddlePointHandleList_on_InputMesh[E04.GetIndex()];
	Pm05 = m_MiddlePointHandleList_on_InputMesh[E05.GetIndex()];
	Pm06 = m_MiddlePointHandleList_on_InputMesh[E06.GetIndex()];
	Pm12 = m_MiddlePointHandleList_on_InputMesh[E12.GetIndex()];
	Pm23 = m_MiddlePointHandleList_on_InputMesh[E23.GetIndex()];
	Pm34 = m_MiddlePointHandleList_on_InputMesh[E34.GetIndex()];
	Pm45 = m_MiddlePointHandleList_on_InputMesh[E45.GetIndex()];
	Pm56 = m_MiddlePointHandleList_on_InputMesh[E56.GetIndex()];
	Pm61 = m_MiddlePointHandleList_on_InputMesh[E61.GetIndex()];

	//-------------------------- get triangle index --------------------------------------------//
	auto Triangle012 = m_InputMesh.GetFaceHandleByPoint({ P0, P1, P2 }).GetIndex();
	auto Triangle023 = m_InputMesh.GetFaceHandleByPoint({ P0, P2, P3 }).GetIndex();
	auto Triangle034 = m_InputMesh.GetFaceHandleByPoint({ P0, P3, P4 }).GetIndex();
	auto Triangle045 = m_InputMesh.GetFaceHandleByPoint({ P0, P4, P5 }).GetIndex();
	auto Triangle056 = m_InputMesh.GetFaceHandleByPoint({ P0, P5, P6 }).GetIndex();
	auto Triangle061 = m_InputMesh.GetFaceHandleByPoint({ P0, P6, P1 }).GetIndex();

	//----------------------------- generate quad candidate pair -------------------------//
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------------------------------------------------

	DenseVector<DenseVector<PointHandleType>,2> QuadPair0, QuadPair1, QuadPair2;
	QuadPair0[0] = { P5, P6, Pm01, Pm04 };
	QuadPair0[1] = { P2, P3, Pm04, Pm01 };
	QuadPair1[0] = { P6, P1, Pm02, Pm05 }; 
	QuadPair1[1] = { P3, P4, Pm05, Pm02 };
	QuadPair2[0] = { P4, P5, Pm06, Pm03 };
	QuadPair2[1] = { P1, P2, Pm03, Pm06 };

	DenseVector<ScalarType, 6> ScoreList;
	ScoreList[0] = this->EvaluateQuad(m_InputMesh, P5, P6, Pm01, Pm04);
	ScoreList[1] = this->EvaluateQuad(m_InputMesh, P2, P3, Pm04, Pm01);
	ScoreList[2] = this->EvaluateQuad(m_InputMesh, P6, P1, Pm02, Pm05);
	ScoreList[3] = this->EvaluateQuad(m_InputMesh, P3, P4, Pm05, Pm02);
	ScoreList[4] = this->EvaluateQuad(m_InputMesh, P4, P5, Pm06, Pm03);
	ScoreList[5] = this->EvaluateQuad(m_InputMesh, P1, P2, Pm03, Pm06);

	//----------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------
	if (ScoreList[0] > 0.1 && ScoreList[1] > 0.1)
	{// QuadPair-0: {P5, P6, Pm01, Pm04} and {P2, P3, Pm04, Pm01}
		m_QuadCandidatePairList.Append(QuadPair0);
		m_QuadCandidatePairScoreList.Append((ScoreList[0] + ScoreList[1])/ScalarType(2));
		auto CandidatePairIndex = m_QuadCandidatePairList.GetLength() - 1;

		auto SmallTriangle_P0_P6_Pm01 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P6, Pm01 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P6_Pm01].Append(CandidatePairIndex);

		auto SmallTriangle_P2_P0_Pm01 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P2, P0, Pm01 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P2_P0_Pm01].Append(CandidatePairIndex);

		auto SmallTriangle_P5_P0_Pm04 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P5, P0, Pm04 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P5_P0_Pm04].Append(CandidatePairIndex);

		auto SmallTriangle_P0_P3_Pm04 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P3, Pm04 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P3_Pm04].Append(CandidatePairIndex);

		for (int_max k = 0; k < 6; ++k)
		{
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle056 + k].Append(CandidatePairIndex);
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle023 + k].Append(CandidatePairIndex);
		}

		DenseVector<DenseVector<PointHandleType>, 4> SmallTriangleSet;
		SmallTriangleSet[0] = { Pm01, P6, P1 };
		SmallTriangleSet[1] = { Pm01, P1, P2 };
		SmallTriangleSet[2] = { Pm04, P4, P5 };
		SmallTriangleSet[3] = { Pm04, P3, P4 };
		m_SmallTriangleList_Near_QuadCandidatePair.Append(SmallTriangleSet);

		m_QuadCandidatePairCenterPointHandleList.Append(P0);
	}
	//----------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------
	if (ScoreList[2] > 0.1 && ScoreList[3] > 0.1)
	{// QuadPair-1: {P6, P1, Pm02, Pm05} and {P3, P4, Pm05, Pm02}        
		m_QuadCandidatePairList.Append(QuadPair1);
		m_QuadCandidatePairScoreList.Append((ScoreList[2] + ScoreList[3]) / ScalarType(2));
		auto CandidatePairIndex = m_QuadCandidatePairList.GetLength() - 1;
		
		auto SmallTriangle_P0_P1_Pm02 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P1, Pm02 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P1_Pm02].Append(CandidatePairIndex);

		auto SmallTriangle_P3_P0_Pm02 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P3, P0, Pm02 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P3_P0_Pm02].Append(CandidatePairIndex);

		auto SmallTriangle_P6_P0_Pm05 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P6, P0, Pm05 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P6_P0_Pm05].Append(CandidatePairIndex);

		auto SmallTriangle_P0_P4_Pm05 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P4, Pm05 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P4_Pm05].Append(CandidatePairIndex);

		for (int_max k = 0; k < 6; ++k)
		{
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle061 + k].Append(CandidatePairIndex);
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle034 + k].Append(CandidatePairIndex);
		}

		DenseVector<DenseVector<PointHandleType>, 4> SmallTriangleSet;
		SmallTriangleSet[0] = { Pm02, P1, P2 };
		SmallTriangleSet[1] = { Pm02, P2, P3 };
		SmallTriangleSet[2] = { Pm05, P5, P6 };
		SmallTriangleSet[3] = { Pm05, P4, P5 };
		m_SmallTriangleList_Near_QuadCandidatePair.Append(SmallTriangleSet);

		m_QuadCandidatePairCenterPointHandleList.Append(P0);
	}
	//----------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------
	if (ScoreList[4] > 0.1 && ScoreList[5] > 0.1)
	{// QuadPair-2: {P4, P5, Pm06, Pm03} and {P1, P2, Pm03, Pm06}
		m_QuadCandidatePairList.Append(QuadPair2);
		m_QuadCandidatePairScoreList.Append((ScoreList[4] + ScoreList[5]) / ScalarType(2));
		auto CandidatePairIndex = m_QuadCandidatePairList.GetLength() - 1;
		
		auto SmallTriangle_P0_P5_Pm06 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P5, Pm06 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P5_Pm06].Append(CandidatePairIndex);

		auto SmallTriangle_P1_P0_Pm06 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P1, P0, Pm06 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P1_P0_Pm06].Append(CandidatePairIndex);

		auto SmallTriangle_P4_P0_Pm03 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P4, P0, Pm03 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P4_P0_Pm03].Append(CandidatePairIndex);

		auto SmallTriangle_P0_P2_Pm03 = m_SmallTriangleMesh.GetFaceHandleByPoint({ P0, P2, Pm03 }).GetIndex();
		m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[SmallTriangle_P0_P2_Pm03].Append(CandidatePairIndex);

		for (int_max k = 0; k < 6; ++k)
		{
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle045 + k].Append(CandidatePairIndex);
			m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[6 * Triangle012 + k].Append(CandidatePairIndex);
		}

		DenseVector<DenseVector<PointHandleType>, 4> SmallTriangleSet;
		SmallTriangleSet[0] = { Pm06, P5, P6 };
		SmallTriangleSet[1] = { Pm06, P6, P1 };
		SmallTriangleSet[2] = { Pm03, P3, P4 };
		SmallTriangleSet[3] = { Pm03, P2, P3 };
		m_SmallTriangleList_Near_QuadCandidatePair.Append(SmallTriangleSet);

		m_QuadCandidatePairCenterPointHandleList.Append(P0);
	}
	//----------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//----------------------
	
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::BuildConflictTable()
{
	auto CandidatePairCount = m_QuadCandidatePairList.GetLength();
	m_QuadCandidatePairConflictTable.Clear();
	m_QuadCandidatePairConflictTable.Resize(CandidatePairCount);
	for (int_max k = 0; k < CandidatePairCount; ++k)
	{
		m_QuadCandidatePairConflictTable[k].SetCapacity(18);
	}

	for (int_max k = 0; k < m_QuadCandidatePairIndexSet_Overlap_SmallTriangle.GetLength(); ++k)
	{		
		const auto& Set = m_QuadCandidatePairIndexSet_Overlap_SmallTriangle[k];
		for (int_max n = 0; n < Set.GetLength(); ++n)
		{
			auto Index_n = Set[n];
			for (int_max m = 0; m < Set.GetLength(); ++m)
			{
				if (Index_n != Set[m])
				{
					m_QuadCandidatePairConflictTable[Index_n].Append(Set[m]);
				}
			}
		}
	}

	for (int_max k = 0; k < CandidatePairCount; ++k)
	{
		auto& Conflict = m_QuadCandidatePairConflictTable[k];
		Conflict = Conflict.GetSubSet(Conflict.FindUnique());
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::SelectCandidate()
{
	auto CandidatePairCount = m_QuadCandidatePairScoreList.GetLength();
	auto QuadCandidatePairIndexList_sort = m_QuadCandidatePairScoreList.Sort("descend");
	m_QuadCandidatePairIndicatorList.Resize(CandidatePairCount);
	m_QuadCandidatePairIndicatorList.Fill(-1);
	for (int_max k = 0; k < CandidatePairCount; ++k)
	{
		auto Index_k = QuadCandidatePairIndexList_sort[k];
		if (m_QuadCandidatePairIndicatorList[Index_k] == -1)
		{
			m_QuadCandidatePairIndicatorList[Index_k] = 1;
			for (int_max n = 0; n < m_QuadCandidatePairConflictTable[Index_k].GetLength(); ++n)
			{
				auto Index_n = m_QuadCandidatePairConflictTable[Index_k][n];
				m_QuadCandidatePairIndicatorList[Index_n] = 0;
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::BuildMixedTriQuadMesh()
{
	m_OutputMesh_Mixed.Clear();
	m_OutputMesh_Mixed = m_InputMesh;

	for (int_max k = 0; k < m_QuadCandidatePairIndicatorList.GetLength(); ++k)
	{
		if (m_QuadCandidatePairIndicatorList[k] == 1)
		{
			this->BuildMixedTriQuadMesh(m_QuadCandidatePairList[k], m_QuadCandidatePairCenterPointHandleList[k]);
		}
	}

	for (int_max k = 0; k < m_QuadCandidatePairIndicatorList.GetLength(); ++k)
	{
		if (m_QuadCandidatePairIndicatorList[k] == 1)
		{
			this->BuildMixedTriQuadMesh(m_SmallTriangleList_Near_QuadCandidatePair[k]);
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::BuildMixedTriQuadMesh(const DenseVector<DenseVector<PointHandleType>, 2>& QuadPair, PointHandleType CenterPoint)
{
	//------------------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//-------------------------------

	auto FaceHandleList = m_OutputMesh_Mixed.Point(CenterPoint).GetAdjacentFaceHandleList();
	auto EdgeHandleList = m_OutputMesh_Mixed.Point(CenterPoint).GetAdjacentEdgeHandleList();

	for (int_max k = 0; k < FaceHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteFace(FaceHandleList[k]);
	}

	for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteEdge(EdgeHandleList[k]);
	}

	m_OutputMesh_Mixed.AddFaceByPoint({ QuadPair[0][0], QuadPair[0][1], QuadPair[0][2], QuadPair[0][3] });
	m_OutputMesh_Mixed.AddFaceByPoint({ QuadPair[1][0], QuadPair[1][1], QuadPair[1][2], QuadPair[1][3] });
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::BuildMixedTriQuadMesh(const DenseVector<DenseVector<PointHandleType>, 4>& SmallTriangleSet_Near_QuadPair)
{
	//------------------------------
	//      P6----P5
	//     / \    / \
	//    /   \  /   \
	//  P1-----P0-----P4
	//    \   /  \   /
	//     \ /____\ /
	//      P2     P3
	//-------------------------------

	for (int_max k = 0; k < 4; ++k)
	{
		auto MiddlePoint = SmallTriangleSet_Near_QuadPair[k][0];
		if (m_OutputMesh_Mixed.Point(MiddlePoint).IsOnBoundaryEdge() == true)
		{
			m_OutputMesh_Mixed.AddFaceByPoint(SmallTriangleSet_Near_QuadPair[k]);
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::AdjustMixedTriQuadMesh()
{
	for (auto it = m_OutputMesh_Mixed.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto AdjacentPointCount = it.Point().GetAdjacentPointCount();
		auto AdjacentFaceHandleList = it.Point().GetAdjacentFaceHandleList();
		auto AdjacentFaceCount = AdjacentFaceHandleList.GetLength();
		if (AdjacentPointCount == 6 && AdjacentFaceCount == 6)
		{
			for (int_max k = 0; k < AdjacentFaceCount; ++k)
			{

			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher2<ScalarType>::BuildQuadMesh()
{	// subdivide m_OutputMesh_Mixed to get m_OutputMesh_Quad



}


template<typename ScalarType>
DenseVector<typename QuadSurfaceRemesher2<ScalarType>::PointHandleType> QuadSurfaceRemesher2<ScalarType>::ConvertIndexToPointHandle(const DenseVector<int_max>& IndexList)
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
DenseVector<typename QuadSurfaceRemesher2<ScalarType>::FaceHandleType> QuadSurfaceRemesher2<ScalarType>::ConvertIndexToFaceHandle(const DenseVector<int_max>& IndexList)
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
DenseVector<int_max> QuadSurfaceRemesher2<ScalarType>::ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList)
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
DenseVector<int_max> QuadSurfaceRemesher2<ScalarType>::ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList)
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
ScalarType QuadSurfaceRemesher2<ScalarType>::EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3)
{
	auto Point0 = TargetMesh.GetPointPosition(PointH0);
	auto Point1 = TargetMesh.GetPointPosition(PointH1);
	auto Point2 = TargetMesh.GetPointPosition(PointH2);
	auto Point3 = TargetMesh.GetPointPosition(PointH3);
	return this->EvaluateQuad(Point0, Point1, Point2, Point3);
}

template<typename ScalarType>
ScalarType QuadSurfaceRemesher2<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
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
ScalarType QuadSurfaceRemesher2<ScalarType>::EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2)
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
DenseVector<typename  QuadSurfaceRemesher2<ScalarType>::PointHandleType>
QuadSurfaceRemesher2<ScalarType>::Intersect(const DenseVector<PointHandleType>& HandleListA, const DenseVector<PointHandleType>& HandleListB)
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
DenseVector<typename  QuadSurfaceRemesher2<ScalarType>::FaceHandleType>
QuadSurfaceRemesher2<ScalarType>::Intersect(const DenseVector<FaceHandleType>& HandleListA, const DenseVector<FaceHandleType>& HandleListB)
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