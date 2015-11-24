#pragma once

namespace mdk
{
template<typename ScalarType>
QuadSurfaceRemesher3<ScalarType>::QuadSurfaceRemesher3()
{
	this->Clear();
}


template<typename ScalarType>
QuadSurfaceRemesher3<ScalarType>::~QuadSurfaceRemesher3()
{
}

template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::Clear()
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
void QuadSurfaceRemesher3<ScalarType>::ClearInternalData()
{
	m_CandidateMesh.Clear();
	m_SmallTriangleMesh.Clear();
	m_MiddlePointHandleList_on_CandidateMesh.Clear();
	m_QuadCandidatePairList.Clear();
	m_QuadCandidatePairScoreList.Clear();
	m_SmallTriangleList_Near_QuadCandidatePair.Clear();
	m_QuadCandidatePairCenterPointHandleList.Clear();
	m_QuadCandidatePairIndexSet_Overlap_SmallTriangle.Clear();
	m_QuadCandidatePairConflictTable.Clear();
}

template<typename ScalarType>
bool QuadSurfaceRemesher3<ScalarType>::CheckInput()
{
	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("m_InputMesh is NOT TriangleMesh @ QuadSurfaceRemesher3::CheckInput()")
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ QuadSurfaceRemesher3::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ QuadSurfaceRemesher3::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->InitializeCandidateMesh();
	this->BuildSmallTriangleMesh();
	this->GenerateCandidate();
	this->BuildConflictTable();
	this->SelectCandidate();
	this->BuildCandidateMesh();
	this->BuildMixedTriQuadMesh();
	this->BuildQuadMesh();
}

template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::InitializeCandidateMesh()
{// add middle point of each edge

	m_CandidateMesh = m_InputMesh;

	m_MiddlePointHandleList_on_CandidateMesh.Clear();
	m_MiddlePointHandleList_on_CandidateMesh.Resize(m_InputMesh.GetEdgeCount());

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Edge().GetPointHandleList();
		auto Point0 = m_InputMesh.GetPointPosition(PointHandleList[0]);
		auto Point1 = m_InputMesh.GetPointPosition(PointHandleList[1]);
		auto PointM = (Point0 + Point1) / ScalarType(2);

		auto EdgeIndex = it.GetEdgeHandle().GetIndex();
		m_MiddlePointHandleList_on_CandidateMesh[EdgeIndex] = m_CandidateMesh.AddPoint(PointM);
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildSmallTriangleMesh()
{// add middle point of each edge

	m_SmallTriangleMesh.Clear();
	m_SmallTriangleMesh.SetCapacity(m_CandidateMesh.GetPointCount(), m_CandidateMesh.GetEdgeCount() + 3 * m_CandidateMesh.GetFaceCount(), 6 * m_CandidateMesh.GetFaceCount());

	for (auto it = m_CandidateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		m_SmallTriangleMesh.AddPoint(it.Point().GetPosition());
	}

	for (auto it = m_CandidateMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
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
		auto PointHa = m_MiddlePointHandleList_on_CandidateMesh[EdgeHandleList[0].GetIndex()];
		auto PointHb = m_MiddlePointHandleList_on_CandidateMesh[EdgeHandleList[1].GetIndex()];
		auto PointHc = m_MiddlePointHandleList_on_CandidateMesh[EdgeHandleList[2].GetIndex()];

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
void QuadSurfaceRemesher3<ScalarType>::GenerateCandidate()
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
			bool Flag_generate_candidate = true;

			auto tempIndex = m_FeaturePointOfInputMesh.ExactMatch("first", it.GetPointHandle());
			if (tempIndex >= 0)
			{
				Flag_generate_candidate = false;
			}
			else
			{
				auto AdjacentEdgeHandleList = it.Point().GetAdjacentEdgeHandleList();
				for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
				{
					auto tempIndex_k = m_FeatureEdgeOfInputMesh.ExactMatch("first", AdjacentEdgeHandleList[k]);
					if (tempIndex_k >= 0)
					{
						Flag_generate_candidate = false;
						break;
					}
				}
			}

			if (Flag_generate_candidate == true)
			{
				this->GenerateCandidate(it.GetPointHandle());
			}
		}
	}

	m_QuadCandidatePairIndicatorList.Clear();
	m_QuadCandidatePairIndicatorList.Resize(m_QuadCandidatePairList.GetLength());
	m_QuadCandidatePairIndicatorList.Fill(-1);
}

template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::GenerateCandidate(PointHandleType PointHandle_input)
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

	//get P2, P6 , make sure P0->P1->P2 and P0->P6->P1 the righ order to define triangle normal
	auto tempSet01 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P1);
	{
		P2 = tempSet01[0];
		P6 = tempSet01[1];
		auto tempTriangle012 = m_InputMesh.GetFaceHandleByPoint({P0, P1, P2});
		if (m_InputMesh.Face(tempTriangle012).GetDirectedEdgeHandleByPoint(P0, P1).GetFaceIndex() < 0)
		{
			P2 = tempSet01[1];
			P6 = tempSet01[0];
		}
	}
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

	Pm01 = m_MiddlePointHandleList_on_CandidateMesh[E01.GetIndex()];
	Pm02 = m_MiddlePointHandleList_on_CandidateMesh[E02.GetIndex()];
	Pm03 = m_MiddlePointHandleList_on_CandidateMesh[E03.GetIndex()];
	Pm04 = m_MiddlePointHandleList_on_CandidateMesh[E04.GetIndex()];
	Pm05 = m_MiddlePointHandleList_on_CandidateMesh[E05.GetIndex()];
	Pm06 = m_MiddlePointHandleList_on_CandidateMesh[E06.GetIndex()];
	Pm12 = m_MiddlePointHandleList_on_CandidateMesh[E12.GetIndex()];
	Pm23 = m_MiddlePointHandleList_on_CandidateMesh[E23.GetIndex()];
	Pm34 = m_MiddlePointHandleList_on_CandidateMesh[E34.GetIndex()];
	Pm45 = m_MiddlePointHandleList_on_CandidateMesh[E45.GetIndex()];
	Pm56 = m_MiddlePointHandleList_on_CandidateMesh[E56.GetIndex()];
	Pm61 = m_MiddlePointHandleList_on_CandidateMesh[E61.GetIndex()];

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
	ScoreList[0] = this->EvaluateQuad(m_CandidateMesh, P5, P6, Pm01, Pm04);
	ScoreList[1] = this->EvaluateQuad(m_CandidateMesh, P2, P3, Pm04, Pm01);
	ScoreList[2] = this->EvaluateQuad(m_CandidateMesh, P6, P1, Pm02, Pm05);
	ScoreList[3] = this->EvaluateQuad(m_CandidateMesh, P3, P4, Pm05, Pm02);
	ScoreList[4] = this->EvaluateQuad(m_CandidateMesh, P4, P5, Pm06, Pm03);
	ScoreList[5] = this->EvaluateQuad(m_CandidateMesh, P1, P2, Pm03, Pm06);

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
void QuadSurfaceRemesher3<ScalarType>::BuildConflictTable()
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
void QuadSurfaceRemesher3<ScalarType>::SelectCandidate()
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
void QuadSurfaceRemesher3<ScalarType>::BuildCandidateMesh()
{
	for (int_max k = 0; k < m_QuadCandidatePairIndicatorList.GetLength(); ++k)
	{
		if (m_QuadCandidatePairIndicatorList[k] == 1)
		{
			this->BuildCandidateMesh_AddQuad(m_QuadCandidatePairList[k], m_QuadCandidatePairCenterPointHandleList[k]);
		}
	}

	for (int_max k = 0; k < m_QuadCandidatePairIndicatorList.GetLength(); ++k)
	{
		if (m_QuadCandidatePairIndicatorList[k] == 1)
		{
			this->BuildCandidateMesh_FillGap(m_SmallTriangleList_Near_QuadCandidatePair[k]);
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildCandidateMesh_AddQuad(const DenseVector<DenseVector<PointHandleType>, 2>& QuadPair, PointHandleType CenterPoint)
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

	auto FaceHandleList = m_CandidateMesh.Point(CenterPoint).GetAdjacentFaceHandleList();
	auto EdgeHandleList = m_CandidateMesh.Point(CenterPoint).GetAdjacentEdgeHandleList();

	for (int_max k = 0; k < FaceHandleList.GetLength(); ++k)
	{
		m_CandidateMesh.DeleteFace(FaceHandleList[k]);
	}

	for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
	{
		m_CandidateMesh.DeleteEdge(EdgeHandleList[k]);
	}

	m_CandidateMesh.AddFaceByPoint({ QuadPair[0][0], QuadPair[0][1], QuadPair[0][2], QuadPair[0][3] });
	m_CandidateMesh.AddFaceByPoint({ QuadPair[1][0], QuadPair[1][1], QuadPair[1][2], QuadPair[1][3] });
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildCandidateMesh_FillGap(const DenseVector<DenseVector<PointHandleType>, 4>& SmallTriangleSet_Near_QuadPair)
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
		if (m_CandidateMesh.Point(MiddlePoint).IsOnBoundaryEdge() == true)
		{
			m_CandidateMesh.AddFaceByPoint(SmallTriangleSet_Near_QuadPair[k]);
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh()
{
	m_OutputMesh_Mixed.Clear();
	m_OutputMesh_Mixed = m_CandidateMesh;
	// must in this order
	this->BuildMixedTriQuadMesh_5PointStarRegionA();	
	this->BuildMixedTriQuadMesh_MergeSmallTriangleAndBigTriangle();		
	this->BuildMixedTriQuadMesh_MergeTwoAdjacentSmallTriangle();	
	//this->BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle();
	this->BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special();
	//this->BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary();
	this->BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle();

	this->BuildMixedTriQuadMesh_MergeTwoBigTriangle_If_necessary();
	this->BuildMixedTriQuadMesh_SplitQuadToTwoBigTriangle_If_necessary();
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_5PointStarRegionA()
{
	auto PointCount_input = m_InputMesh.GetPointCount();
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto PointHandle = it.GetPointHandle();
		if (it.Point().IsOnBoundaryEdge() == false && it.Point().GetAdjacentPointCount() == 5 && PointHandle.GetIndex() < PointCount_input)
		{// check if 4 small triangle and 1 big triangle on m_OutputMesh_Mixed		
			auto AdjacentFaceHandleList = m_OutputMesh_Mixed.Point(PointHandle).GetAdjacentFaceHandleList();
			int_max Counter_tri = 0;
			int_max Counter_quad = 0;
			for (int_max k = 0; k < AdjacentFaceHandleList.GetLength(); ++k)
			{
				auto PointCount_k = m_OutputMesh_Mixed.Face(AdjacentFaceHandleList[k]).GetPointCount();
				if (PointCount_k == 3)
				{
					Counter_tri += 1;
				}
				else if (PointCount_k == 4)
				{
					Counter_quad += 1;
				}
			}

			if (Counter_tri == 5 && Counter_quad == 0)
			{
				auto AdjacentPointHandleList = m_OutputMesh_Mixed.Point(PointHandle).GetAdjacentPointHandleList();
				int_max Counter_a = 0;
				int_max Counter_b = 0;
				for (int_max k = 0; k < AdjacentPointHandleList.GetLength(); ++k)
				{
					auto PointIndex_k = AdjacentPointHandleList[k].GetIndex();
					if (PointIndex_k < PointCount_input)
					{
						Counter_a += 1;//input point on big triangle
					}
					else if (PointIndex_k < PointCount_input_and_middle)// >= PointCount_input
					{
						Counter_b += 1; //middle point
					}
				}

				if (Counter_a == 3 && Counter_b == 2)
				{
					this->BuildMixedTriQuadMesh_5PointStarRegionA(PointHandle, PointCount_input);
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_5PointStarRegionA(PointHandleType PointHandle_center, int_max PointCount_input)
{
// 5PointStar Region-A: 
//--------------------------------------
//       P1
//      /|\
//     / | \
// P2 /__P0_\P5
//    | / \ |
//    |/___\|
//    P3    P4
//--------------------------------------
// P0 is PointHandle_center
//--------------------------------------

	PointHandleType P0, P1, P2, P3, P4, P5;
	P0 = PointHandle_center;
	auto AdjacentPointHandleList_P0 = m_OutputMesh_Mixed.Point(P0).GetAdjacentPointHandleList();
    // get P2, P5
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();
	for (int_max k = 0; k < AdjacentPointHandleList_P0.GetLength(); ++k)
	{
		auto PointIndex_k = AdjacentPointHandleList_P0[k].GetIndex();
		if (PointIndex_k >= PointCount_input && PointIndex_k < PointCount_input_and_middle)
		{// PointIndex_k is P2 or P5
			if (P2.GetIndex() < 0)
			{
				P2.SetIndex(PointIndex_k);
			}
			else
			{
				P5.SetIndex(PointIndex_k);
			}
		}
	}
	// re-check to make sure P0->P5 define the tirangle normal
	auto tempHandleList = m_OutputMesh_Mixed.GetDirectedEdgeHandleListByPoint(P0, P5);
	if (tempHandleList.IsEmpty() == true)
	{
		auto temp = P2;
		P2 = P5;
		P5 = temp;
	}
	auto AdjacentPointHandleList_P2 = m_OutputMesh_Mixed.Point(P2).GetAdjacentPointHandleList();
	auto AdjacentPointHandleList_P5 = m_OutputMesh_Mixed.Point(P5).GetAdjacentPointHandleList();

	//get P1, P3, P4
	auto Set02 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P2);//{P1, P3}
	auto Set05 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P5);//{P1, P4}	
	auto tempSet = this->Intersect(Set02, Set05);
	P1 = tempSet[0];
	if (Set02[0] == P1)
	{
		P3 = Set02[1];
	}
	else
	{
		P3 = Set02[0];
	}
	if (Set05[0] == P1)
	{
		P4 = Set05[1];
	}
	else
	{
		P4 = Set05[0];
	}

	// delete edge, face
	auto AdjacentFaceHandleList = m_OutputMesh_Mixed.Point(P0).GetAdjacentFaceHandleList();
	auto AdjacentEdgeHandleList = m_OutputMesh_Mixed.Point(P0).GetAdjacentEdgeHandleList();
	for (int_max k = 0; k < AdjacentFaceHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteFace(AdjacentFaceHandleList[k]);
	}
	for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteEdge(AdjacentEdgeHandleList[k]);
	}
	// do NOT delete any point here

	// add  a triangle and a quad
	m_OutputMesh_Mixed.AddFaceByPoint({ P1, P2, P5 });
	m_OutputMesh_Mixed.AddFaceByPoint({ P2, P3, P4, P5 });
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_MergeSmallTriangleAndBigTriangle()
{
	auto PointCount_input = m_InputMesh.GetPointCount();
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		if (it.Edge().IsBoundary() == false)
		{
			auto AdjacentFaceHandleList = it.Edge().GetAdjacentFaceHandleList();
			auto FaceH0 = AdjacentFaceHandleList[0];
			auto FaceH1 = AdjacentFaceHandleList[1];
			if (m_OutputMesh_Mixed.Face(FaceH0).GetPointCount() == 3 && m_OutputMesh_Mixed.Face(FaceH1).GetPointCount() == 3)
			{
				auto PointIndexList0 = this->ConvertHandleToIndex(m_OutputMesh_Mixed.Face(FaceH0).GetPointHandleList());
				auto PointIndexList1 = this->ConvertHandleToIndex(m_OutputMesh_Mixed.Face(FaceH1).GetPointHandleList());
				// 3 input point on big-triangle, 2 input point and 1 middle point on small-triangle
				int_max Counter0_input = 0;
				int_max Counter0_middle = 0;
				int_max Counter1_input = 0;
				int_max Counter1_middle = 0;
				for (int_max k = 0; k < 3; ++k)
				{
					if (PointIndexList0[k] < PointCount_input)
					{
						Counter0_input += 1;
					}
					else if (PointIndexList0[k] < PointCount_input_and_middle)// >= PointCount_input
					{
						Counter0_middle += 1;
					}

					if (PointIndexList1[k] < PointCount_input)
					{
						Counter1_input += 1;
					}
					else if (PointIndexList1[k] < PointCount_input_and_middle)// >= PointCount_input
					{
						Counter1_middle += 1;
					}
				}

				if (Counter0_input == 3 && Counter1_input == 2 && Counter1_middle == 1 || Counter1_input == 3 && Counter0_input == 2 && Counter0_middle == 1)
				{
					this->BuildMixedTriQuadMesh_MergeTwoAdjacentTriangle(it.GetEdgeHandle());
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_MergeTwoAdjacentSmallTriangle()
{
	//------------------------
    //      P0 
	//     /|\ 
	//  P1/ | \P3
	//     \|/ 
	//      P2
	//------------------------

	auto PointCount_input = m_InputMesh.GetPointCount();
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		if (it.Edge().IsBoundary() == false)
		{
			auto PointHandleList = it.Edge().GetPointHandleList();
			if (PointHandleList[0].GetIndex() < PointCount_input && PointHandleList[1].GetIndex() < PointCount_input)
			{ 
				auto AdjacentPointIndexList_P0 = this->ConvertHandleToIndex(m_OutputMesh_Mixed.Point(PointHandleList[0]).GetAdjacentPointHandleList());
				auto AdjacentPointIndexList_P1 = this->ConvertHandleToIndex(m_OutputMesh_Mixed.Point(PointHandleList[1]).GetAdjacentPointHandleList());
				auto Set01 = this->Intersect(AdjacentPointIndexList_P0, AdjacentPointIndexList_P1);
				if (Set01.GetLength() == 2)
				{
					if (Set01[0] >= PointCount_input && Set01[0] < PointCount_input_and_middle && Set01[1] >= PointCount_input && Set01[1] < PointCount_input_and_middle)
					{
						this->BuildMixedTriQuadMesh_MergeTwoAdjacentTriangle(it.GetEdgeHandle());
					}
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_MergeTwoAdjacentTriangle(EdgeHandleType EdgeHandle_shared)
{
	//---------------------------------------
	//     P0___ P3
	//     / \ | 
	//    /___\|
	//    P1   P2     
	//---------------------------------------
	//     P0___P3
	//     / \  /
	//    /___\/
	//    P1   P2
	//---------------------------------------
	// P0->P1->P2 define triangle normal
	//----------------------------------------

	PointHandleType P0, P1, P2, P3;
	m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetPointHandleList(P0, P2);
	auto AdjacentPointHandleList_P0 = m_OutputMesh_Mixed.Point(P0).GetAdjacentPointHandleList();
	auto AdjacentPointHandleList_P2 = m_OutputMesh_Mixed.Point(P2).GetAdjacentPointHandleList();
	auto tempSet13 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P2);
	P1 = tempSet13[0];
	P3 = tempSet13[1];

	//check the point order
	auto Triangle012 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P1, P2 });
	if (m_OutputMesh_Mixed.Face(Triangle012).GetDirectedEdgeHandleByPoint(P2, P0).GetFaceIndex() < 0)
	{// no directed-edge from P2 to P0, then switch
		auto temp = P0;
		P0 = P2;
		P2 = temp;
	}

	auto Triangle023 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P2, P3 });

	//delete edge, face
	m_OutputMesh_Mixed.DeleteFace(Triangle012);
	m_OutputMesh_Mixed.DeleteFace(Triangle023);
	m_OutputMesh_Mixed.DeleteEdge(EdgeHandle_shared);

	//new face
	m_OutputMesh_Mixed.AddFaceByPoint({ P0, P1, P2, P3 });
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special()
{// Collapse two small/big triangle surrounded by quad
// identify the  two small triangle: any middle point of input mesh

	//---------------------------------------
	//   P4_______P3________P9
	//    |      / \       |
	// P5 |___P0/___\P1____| P8      
	//    |     \   /      |
	//    |_____ \ /_______|
	//   P6       P2       P7
	//---------------------------------------

	auto PointCount_input = m_InputMesh.GetPointCount();

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_Edge = it.Edge().GetPointHandleList();
		auto PointH0 = PointHandleList_Edge[0];
		auto PointH1 = PointHandleList_Edge[1];

		bool Flag_go_on = true;
		if (it.Edge().IsBoundary() == false)
		{			
			auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();
			//if (PointH0.GetIndex() >= PointCount_input && PointH0.GetIndex() < PointCount_input_and_middle
			//	|| PointH1.GetIndex() >= PointCount_input && PointH1.GetIndex() < PointCount_input_and_middle)
			if (PointH0.GetIndex() > PointCount_input_and_middle || PointH1.GetIndex() > PointCount_input_and_middle)
			{
				Flag_go_on = false;
			}

			auto AdjacentFaceHandleList = it.Edge().GetAdjacentFaceHandleList();
			auto FaceH0 = AdjacentFaceHandleList[0];
			auto FaceH1 = AdjacentFaceHandleList[1];

			if (m_OutputMesh_Mixed.Face(FaceH0).GetPointCount() != 3 || m_OutputMesh_Mixed.Face(FaceH1).GetPointCount() != 3)
			{
				Flag_go_on = false;
			}
		}

		if (Flag_go_on == true)
		{// check if two triangle and two quad around each point of the edge
			Flag_go_on = false;
			auto AdjacentFaceHandleList0 = m_OutputMesh_Mixed.Point(PointH0).GetAdjacentFaceHandleList();
			auto AdjacentFaceHandleList1 = m_OutputMesh_Mixed.Point(PointH1).GetAdjacentFaceHandleList();
			if (AdjacentFaceHandleList0.GetLength() == 4 && AdjacentFaceHandleList1.GetLength() == 4)
			{
				int_max Count0_tri = 0;
				int_max Count0_quad = 0;
				for (int_max k = 0; k < 4; ++k)
				{
					auto tempPointCount0 = m_OutputMesh_Mixed.Face(AdjacentFaceHandleList0[k]).GetPointCount();
					if (tempPointCount0 == 3)
					{
						Count0_tri += 1;
					}
					else if (tempPointCount0 == 4)
					{
						Count0_quad += 1;
					}
				}

				int_max Count1_tri = 0;
				int_max Count1_quad = 0;
				for (int_max k = 0; k < 4; ++k)
				{
					auto tempPointCount1 = m_OutputMesh_Mixed.Face(AdjacentFaceHandleList1[k]).GetPointCount();
					if (tempPointCount1 == 3)
					{
						Count1_tri += 1;
					}
					else if (tempPointCount1 == 4)
					{
						Count1_quad += 1;
					}
				}

				if (Count0_tri == 2 && Count0_quad == 2 && Count1_tri == 2 && Count1_quad == 2)
				{
					Flag_go_on = true;
				}
			}
		}

		if (Flag_go_on == true)
		{
			this->BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special(it.GetEdgeHandle());
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTwoAdjacentTriangle_Special(EdgeHandleType EdgeHandle_shared)
{// Collapse two small triangle surrounded by quad

	//---------------------------------------
	//   P4_______P3________P9
	//    |      / \       |
	// P5 |___P0/___\P1____| P8      
	//    |     \   /      |
	//    |_____ \ /_______|
	//   P6       P2       P7
	//---------------------------------------
	// E01 is EdgeHandle_shared
	//---------------------------------------
    // Pm is middle point between P0 and P1
	//---------------------------------------
	//   P4_______P3________P9
	//    |       |        |
	// P5 |_______Pm_______| P8      
	//    |       |        |
	//    |_______|________|
	//   P6       P2       P7
	//---------------------------------------

	PointHandleType P0, P1, Pm, P2, P3, P4, P5, P6, P7, P8, P9;

	// get P0, P1
	auto PointHandleList_E01 = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetPointHandleList();
	P0 = PointHandleList_E01[0];
	P1 = PointHandleList_E01[1];

	//add Pm to m_OutputMesh_Mixed
	auto P0_Position = m_OutputMesh_Mixed.GetPointPosition(P0);
	auto P1_Position = m_OutputMesh_Mixed.GetPointPosition(P1);
	auto Pm_Position = (P0_Position + P1_Position) / ScalarType(2);
	Pm = m_OutputMesh_Mixed.AddPoint(Pm_Position);

	//get P2, P3,  order P0->P1->P3 define triangle normal
	auto AdjacentPointHandleList_P0 = m_OutputMesh_Mixed.Point(P0).GetAdjacentPointHandleList();
	auto AdjacentPointHandleList_P1 = m_OutputMesh_Mixed.Point(P1).GetAdjacentPointHandleList();
	{
		auto tempSet01 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P1);
		P2 = tempSet01[0];
		P3 = tempSet01[1];
		auto tempTriangle013 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P1, P3 });
		if (m_OutputMesh_Mixed.Face(tempTriangle013).GetDirectedEdgeHandleByPoint(P0, P1).GetFaceIndex() < 0)
		{
			P2 = tempSet01[1];
			P3 = tempSet01[0];
		}
	}

	//get P4, P5
	auto E03 = m_OutputMesh_Mixed.GetDirectedEdgeHandleListByPoint(P0, P3);
	auto Quad0345 = m_OutputMesh_Mixed.DirectedEdge(E03[0]).GetFaceHandle();
	auto PointHandleList_Quad0345 = m_OutputMesh_Mixed.Face(Quad0345).GetPointHandleList_LeadBy(P0, P3);
	auto PointHandleList_Quad0345_a = m_OutputMesh_Mixed.Face(Quad0345).GetPointHandleList_LeadBy(P0);
	P4 = PointHandleList_Quad0345[2];
	P5 = PointHandleList_Quad0345[3];

	// get P5, P6
	auto E20 = m_OutputMesh_Mixed.GetDirectedEdgeHandleListByPoint(P2, P0);
	auto Quad2056 = m_OutputMesh_Mixed.DirectedEdge(E20[0]).GetFaceHandle();
	auto PointHandleList_Quad2056 = m_OutputMesh_Mixed.Face(Quad2056).GetPointHandleList_LeadBy(P2, P0);
	auto PointHandleList_Quad2056_a = m_OutputMesh_Mixed.Face(Quad2056).GetPointHandleList_LeadBy(P2);
	P5 = PointHandleList_Quad2056[2];
	P6 = PointHandleList_Quad2056[3];

	//get P7, P8
	auto E12 = m_OutputMesh_Mixed.GetDirectedEdgeHandleListByPoint(P1, P2);
	auto Quad1278 = m_OutputMesh_Mixed.DirectedEdge(E12[0]).GetFaceHandle();
	auto PointHandleList_Quad1278 = m_OutputMesh_Mixed.Face(Quad1278).GetPointHandleList_LeadBy(P1, P2);
	auto PointHandleList_Quad1278_a = m_OutputMesh_Mixed.Face(Quad1278).GetPointHandleList_LeadBy(P1);
	P7 = PointHandleList_Quad1278[2];
	P8 = PointHandleList_Quad1278[3];

	//get P8, P9
	auto E31 = m_OutputMesh_Mixed.GetDirectedEdgeHandleListByPoint(P3, P1);
	auto Quad3189 = m_OutputMesh_Mixed.DirectedEdge(E31[0]).GetFaceHandle();
	auto PointHandleList_Quad3189 = m_OutputMesh_Mixed.Face(Quad3189).GetPointHandleList_LeadBy(P3, P1);
	auto PointHandleList_Quad3189_a = m_OutputMesh_Mixed.Face(Quad3189).GetPointHandleList_LeadBy(P3);
	P8 = PointHandleList_Quad3189[2];
	P9 = PointHandleList_Quad3189[3];

	// delete edge, face, and re-create quad
	auto NeighbourFaceHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetNeighbourFaceHandleList();
	auto AdjacentEdgeHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetAdjacentEdgeHandleList();
	for (int_max k = 0; k < NeighbourFaceHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteFace(NeighbourFaceHandleList[k]);
	}
	for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteEdge(AdjacentEdgeHandleList[k]);
	}
	m_OutputMesh_Mixed.DeleteEdge(EdgeHandle_shared);
	
	//---------------------------------------
	//   P4_______P3________P9
	//    |       |        |
	// P5 |_______Pm_______| P8      
	//    |       |        |
	//    |_______|________|
	//   P6       P2       P7
	//---------------------------------------

	m_OutputMesh_Mixed.AddFaceByPoint({ Pm, P3, P4, P5 });
	m_OutputMesh_Mixed.AddFaceByPoint({ Pm, P5, P6, P2 });
	m_OutputMesh_Mixed.AddFaceByPoint({ Pm, P2, P7, P8 });
	m_OutputMesh_Mixed.AddFaceByPoint({ Pm, P8, P9, P3 });
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle()
{// Collapse two small triangle 

	//---------------------------------------
	//   _______P3________
	//  |      / \    /  |
	//  |___P0/___\P1/___|      
	//  |    / \  /  \   |
	//  |___/___\/____\__|
	//          P2       
	//---------------------------------------

	auto PointCount_input = m_InputMesh.GetPointCount();
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		if (it.Edge().IsBoundary() == false)
		{
			auto AdjacentFaceHandleList = it.Edge().GetAdjacentFaceHandleList();
			auto FaceH0 = AdjacentFaceHandleList[0];
			auto FaceH1 = AdjacentFaceHandleList[1];
			if (m_OutputMesh_Mixed.Face(FaceH0).GetPointCount() == 3 && m_OutputMesh_Mixed.Face(FaceH1).GetPointCount() == 3)
			{
				auto PointHandleList_Edge = it.Edge().GetPointHandleList();
				auto P0 = PointHandleList_Edge[0].GetIndex();
				auto P1 = PointHandleList_Edge[1].GetIndex();
				if (P0 >= PointCount_input && P0 < PointCount_input_and_middle || P1 >= PointCount_input && P1 < PointCount_input_and_middle)
				{
					this->BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle(it.GetEdgeHandle());
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle(EdgeHandleType EdgeHandle_shared)
{// Collapse two small triangle surrounded by many face

	//---------------------------------------
	//   _______P3________
	//  |      / \    /  |
	//  |___P0/___\P1/___|      
	//  |    / \  /  \   |
	//  |___/___\/____\__|
	//          P2       
	//---------------------------------------
	// Edge P0P1 is EdgeHandle_shared
	// Pm is middle point between P0 and P1
	//---------------------------------------

	PointHandleType P0, P1, P2, P3, Pm;

	// get P0, P1
	auto PointHandleList_E01 = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetPointHandleList();
	P0 = PointHandleList_E01[0];
	P1 = PointHandleList_E01[1];

	//add Pm to m_OutputMesh_Mixed
	auto P0_Position = m_OutputMesh_Mixed.GetPointPosition(P0);
	auto P1_Position = m_OutputMesh_Mixed.GetPointPosition(P1);
	auto Pm_Position = (P0_Position + P1_Position) / ScalarType(2);		
	if (m_OutputMesh_Mixed.Point(P0).IsOnBoundaryEdge() == true)
	{// if P0 or P1 is on boundary, move Pm to P0 or P1
		Pm_Position = P0_Position;
	}
	else if (m_OutputMesh_Mixed.Point(P1).IsOnBoundaryEdge() == true)
	{
		Pm_Position = P1_Position;
	}
	Pm = m_OutputMesh_Mixed.AddPoint(Pm_Position);

	//get P2, P3,  order P0->P1->P3 define triangle normal
	auto AdjacentPointHandleList_P0 = m_OutputMesh_Mixed.Point(P0).GetAdjacentPointHandleList();
	auto AdjacentPointHandleList_P1 = m_OutputMesh_Mixed.Point(P1).GetAdjacentPointHandleList();
	{
		auto tempSet01 = this->Intersect(AdjacentPointHandleList_P0, AdjacentPointHandleList_P1);
		P2 = tempSet01[0];
		P3 = tempSet01[1];
		auto tempTriangle013 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P1, P3 });
		if (m_OutputMesh_Mixed.Face(tempTriangle013).GetDirectedEdgeHandleByPoint(P0, P1).GetFaceIndex() < 0)
		{
			P2 = tempSet01[1];
			P3 = tempSet01[0];
		}
	}

	auto Triangle013 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P1, P3 });
	auto Triangle021 = m_OutputMesh_Mixed.GetFaceHandleByPoint({ P0, P2, P1 });

	//prepare
	auto AdjacentFaceHandleList_P0 = m_OutputMesh_Mixed.Point(P0).GetAdjacentFaceHandleList();
	auto AdjacentFaceHandleList_P1 = m_OutputMesh_Mixed.Point(P1).GetAdjacentFaceHandleList();
	ObjectArray<DenseVector<PointHandleType>> PointHandleTable_FaceAdjacentToP0;
	PointHandleTable_FaceAdjacentToP0.SetCapacity(AdjacentFaceHandleList_P0.GetLength());
	ObjectArray<DenseVector<PointHandleType>> PointHandleTable_FaceAdjacentToP1;
	PointHandleTable_FaceAdjacentToP1.SetCapacity(AdjacentFaceHandleList_P1.GetLength());
	for (int_max k = 0; k < AdjacentFaceHandleList_P0.GetLength(); ++k)
	{
		auto FaceHandle_k = AdjacentFaceHandleList_P0[k];
		if (FaceHandle_k != Triangle013 && FaceHandle_k != Triangle021)
		{
			PointHandleTable_FaceAdjacentToP0.Append(m_OutputMesh_Mixed.Face(FaceHandle_k).GetPointHandleList_LeadBy(P0));
			PointHandleTable_FaceAdjacentToP0[PointHandleTable_FaceAdjacentToP0.GetLength()-1][0] = Pm;//move P0 to Pm
		}
	}
	for (int_max k = 0; k < AdjacentFaceHandleList_P1.GetLength(); ++k)
	{
		auto FaceHandle_k = AdjacentFaceHandleList_P1[k];
		if (FaceHandle_k != Triangle013 && FaceHandle_k != Triangle021)
		{
			PointHandleTable_FaceAdjacentToP1.Append(m_OutputMesh_Mixed.Face(FaceHandle_k).GetPointHandleList_LeadBy(P1));
			PointHandleTable_FaceAdjacentToP1[PointHandleTable_FaceAdjacentToP1.GetLength() - 1][0] = Pm;//move P1 to Pm
		}
	}

	// delete edge, face
	auto NeighbourFaceHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetNeighbourFaceHandleList();
	auto AdjacentEdgeHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_shared).GetAdjacentEdgeHandleList();
	for (int_max k = 0; k < NeighbourFaceHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteFace(NeighbourFaceHandleList[k]);
	}
	for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteEdge(AdjacentEdgeHandleList[k]);
	}
	m_OutputMesh_Mixed.DeleteEdge(EdgeHandle_shared);

	//re-create face and edge
	for (int_max k = 0; k < PointHandleTable_FaceAdjacentToP0.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.AddFaceByPoint(PointHandleTable_FaceAdjacentToP0[k]);
	}
	for (int_max k = 0; k < PointHandleTable_FaceAdjacentToP1.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.AddFaceByPoint(PointHandleTable_FaceAdjacentToP1[k]);
	}

}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary()
{
	for (auto it = m_OutputMesh_Mixed.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		if (it.Face().GetPointCount() == 3)
		{
			this->BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary(it.GetFaceHandle());
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTriangle_If_Necessary(FaceHandleType FaceHandle)
{
	auto PointHandleList = m_OutputMesh_Mixed.Face(FaceHandle).GetPointHandleList();
	auto P0_Position = m_OutputMesh_Mixed.GetPointPosition(PointHandleList[0]);
	auto P1_Position = m_OutputMesh_Mixed.GetPointPosition(PointHandleList[1]);
	auto P2_Position = m_OutputMesh_Mixed.GetPointPosition(PointHandleList[2]);

	auto L01 = (P0_Position - P1_Position).L2Norm();
	auto L12 = (P1_Position - P2_Position).L2Norm();
	auto L20 = (P2_Position - P0_Position).L2Norm();

	PointHandleType Pa, Pb, Pc;
	//     Pa
	//     /\
	//    /  \
	//   Pb--Pc
	//---------------------------

	ScalarType Threshold = 0.6;
	if (L01 / L12 < Threshold && L01 / L20 < Threshold)
	{
		Pa = PointHandleList[2];
		Pb = PointHandleList[0];
		Pc = PointHandleList[1];
	}
	else if (L12/L01 < Threshold && L12/L20 < Threshold)
	{
		Pa = PointHandleList[0];
		Pb = PointHandleList[1];
		Pc = PointHandleList[2];
	}
	else if (L20 / L01 < Threshold && L20 / L12 < Threshold)
	{
		Pa = PointHandleList[1];
		Pb = PointHandleList[2];
		Pc = PointHandleList[0];
	}
	else
	{
		return;
	}

	this->BuildMixedTriQuadMesh_CollapseTriangle(FaceHandle, Pa, Pb, Pc);
}

template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_CollapseTriangle(FaceHandleType Triangle_abc, PointHandleType Pa, PointHandleType Pb, PointHandleType Pc)
{
	//---------------------------
	// ----Pa -----
	//     /\
	//    /  \
	//---Pb--Pc----
	//---------------------------
	// Pa->Pb->Pc determine triangle normal
	//---------------------------
	// ----Pa------
	//     |
	//     |
	//-----Pm------
	//-------------------------

	PointHandleType Pm;
	DenseVector<ScalarType, 3> Pm_Position;
	auto Pb_Position = m_OutputMesh_Mixed.GetPointPosition(Pb);
	auto Pc_Position = m_OutputMesh_Mixed.GetPointPosition(Pc);
	if (m_OutputMesh_Mixed.Point(Pb).IsOnBoundaryEdge() == true)
	{
		Pm_Position = Pb_Position;
		Pm = Pb;
	}
	else if (m_OutputMesh_Mixed.Point(Pc).IsOnBoundaryEdge() == true)
	{
		Pm_Position = Pc_Position;
		Pm = Pc;
	}
	else
	{
		Pm_Position = (Pb_Position + Pc_Position) / ScalarType(2);
		Pm = m_OutputMesh_Mixed.AddPoint(Pm_Position);
	}
	

	auto AdjacentFaceHandleList_Pb = m_OutputMesh_Mixed.Point(Pb).GetAdjacentFaceHandleList();
	auto AdjacentFaceHandleList_Pc = m_OutputMesh_Mixed.Point(Pc).GetAdjacentFaceHandleList();
	auto AdjacentFaceHandleList_Pb_Pc = this->Intersect(AdjacentFaceHandleList_Pb, AdjacentFaceHandleList_Pc);

	ObjectArray<DenseVector<PointHandleType>> PointHandleTable_FaceAdjacentToPb;
	PointHandleTable_FaceAdjacentToPb.SetCapacity(AdjacentFaceHandleList_Pb.GetLength());

	ObjectArray<DenseVector<PointHandleType>> PointHandleTable_FaceAdjacentToPc;
	PointHandleTable_FaceAdjacentToPc.SetCapacity(AdjacentFaceHandleList_Pc.GetLength());

	ObjectArray<DenseVector<PointHandleType>> PointHandleTable_FaceAdjacentToPbPc;
	PointHandleTable_FaceAdjacentToPbPc.SetCapacity(3);

	for (int_max k = 0; k < AdjacentFaceHandleList_Pb.GetLength(); ++k)
	{
		auto FaceHandle_k = AdjacentFaceHandleList_Pb[k];
		if (FaceHandle_k != Triangle_abc)
		{
			auto tempIndex = AdjacentFaceHandleList_Pb_Pc.ExactMatch("first", FaceHandle_k);
			if (tempIndex < 0)
			{
				PointHandleTable_FaceAdjacentToPb.Append(m_OutputMesh_Mixed.Face(FaceHandle_k).GetPointHandleList_LeadBy(Pb));
				PointHandleTable_FaceAdjacentToPb[PointHandleTable_FaceAdjacentToPb.GetLength() - 1][0] = Pm;//move Pb to Pm
			}
		}
	}
	for (int_max k = 0; k < AdjacentFaceHandleList_Pc.GetLength(); ++k)
	{
		auto FaceHandle_k = AdjacentFaceHandleList_Pc[k];
		if (FaceHandle_k != Triangle_abc)
		{
			auto tempIndex = AdjacentFaceHandleList_Pb_Pc.ExactMatch("first", FaceHandle_k);
			if (tempIndex < 0)
			{
				PointHandleTable_FaceAdjacentToPc.Append(m_OutputMesh_Mixed.Face(FaceHandle_k).GetPointHandleList_LeadBy(Pc));
				PointHandleTable_FaceAdjacentToPc[PointHandleTable_FaceAdjacentToPc.GetLength() - 1][0] = Pm;//move Pc to Pm
			}
		}
	}
	for (int_max k = 0; k < AdjacentFaceHandleList_Pb_Pc.GetLength(); ++k)
	{
		auto FaceHandle_k = AdjacentFaceHandleList_Pb_Pc[k];
		if (FaceHandle_k != Triangle_abc)
		{
			auto tempList = m_OutputMesh_Mixed.Face(FaceHandle_k).GetPointHandleList_LeadBy(Pc, Pb);
			if (tempList.GetLength() == 3)
			{
				MDK_Error("wrong @ BuildMixedTriQuadMesh_CollapseTriangle(...)")
				return;
			}
			PointHandleTable_FaceAdjacentToPbPc.Append(tempList.GetSubSet(1, tempList.GetLength() - 1));
			PointHandleTable_FaceAdjacentToPbPc[PointHandleTable_FaceAdjacentToPbPc.GetLength() - 1][0] = Pm;//move Pc to Pm
		}
	}

	// delete edge, face
	auto EdgeHandle_bc = m_OutputMesh_Mixed.GetEdgeHandleByPoint(Pb, Pc);
	auto NeighbourFaceHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_bc).GetNeighbourFaceHandleList();
	auto AdjacentEdgeHandleList = m_OutputMesh_Mixed.Edge(EdgeHandle_bc).GetAdjacentEdgeHandleList();	
	for (int_max k = 0; k < NeighbourFaceHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteFace(NeighbourFaceHandleList[k]);
	}
	for (int_max k = 0; k < AdjacentEdgeHandleList.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.DeleteEdge(AdjacentEdgeHandleList[k]);
	}
	m_OutputMesh_Mixed.DeleteEdge(EdgeHandle_bc);

	//re-create face and edge
	for (int_max k = 0; k < PointHandleTable_FaceAdjacentToPb.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.AddFaceByPoint(PointHandleTable_FaceAdjacentToPb[k]);
	}
	for (int_max k = 0; k < PointHandleTable_FaceAdjacentToPc.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.AddFaceByPoint(PointHandleTable_FaceAdjacentToPc[k]);
	}
	for (int_max k = 0; k < PointHandleTable_FaceAdjacentToPbPc.GetLength(); ++k)
	{
		m_OutputMesh_Mixed.AddFaceByPoint(PointHandleTable_FaceAdjacentToPbPc[k]);
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle()
{
	auto FaceCount_input = m_InputMesh.GetFaceCount();
	for (auto it = m_OutputMesh_Mixed.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		if (it.GetFaceHandle().GetIndex() >= FaceCount_input)
		{
			this->BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle(it.GetFaceHandle());
		}
	}
}

template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_ChangeSmallTriangleToBigTriangle(FaceHandleType SmallTriangle)
{
//-----------------------------
//	  P0 ____P4
//     /|    | 
// P1 /_|____|  
//      P2   P3
//--------------------------
//	   P0____P4
//     / \   |
// P1 /___\__| 
//        P5 P3
//---------------------------
// merge connectivity of P2 to connectivity of P5
// P2 is the middle point on input edge P1P5
// P5 is isolated point on m_CandidateMesh
// P0, P1, P4 on input mesh
// P3 may be a middle point on input edge
// P3 may be a point created in BuildMixedTriQuadMesh_CollapseTwoAdjacentSmallTriangle
//--------------------------

	PointHandleType P0, P1, P2, P5;

	auto PointHandleList = m_OutputMesh_Mixed.Face(SmallTriangle).GetPointHandleList();
	auto EdgeHandleList = m_OutputMesh_Mixed.Face(SmallTriangle).GetEdgeHandleList();
	//------------------- input check --------------------------------------------------//
	auto PointCount_input = m_InputMesh.GetPointCount();
	auto PointCount_input_and_middle = PointCount_input + m_MiddlePointHandleList_on_CandidateMesh.GetLength();

	bool Flag_input = false;
	DenseMatrix<int_max> IndexTable;
	IndexTable = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 1, 0 }, { 2, 0, 1 } };
	for (int_max k = 0; k < 6; ++k)
	{
		P0 = PointHandleList[IndexTable(k, 0)];
		P1 = PointHandleList[IndexTable(k, 1)];
		P2 = PointHandleList[IndexTable(k, 2)];
		if (P0.GetIndex() < PointCount_input && P1.GetIndex() < PointCount_input && P2.GetIndex() >= PointCount_input && P2.GetIndex() < PointCount_input_and_middle)
		{
			Flag_input = true;
			break;
		}
	}
	if (Flag_input == false)
	{
		return;
	}
	
	// may need to switch P0 and P1
	auto tempEdgeIndex_P1P5 = m_MiddlePointHandleList_on_CandidateMesh.ExactMatch("first", P2);
	if (tempEdgeIndex_P1P5 < 0)
	{
		return;
	}
	EdgeHandleType EdgeH_P1P5;
	EdgeH_P1P5.SetIndex(tempEdgeIndex_P1P5);
	auto PointHandleList_P1P5 = m_InputMesh.Edge(EdgeH_P1P5).GetPointHandleList();
	if (PointHandleList_P1P5[0] == P0)
	{
		auto temp = P0;
		P0 = P1;
		P1 = temp;
		P5 = PointHandleList_P1P5[1];
	}
	else if (PointHandleList_P1P5[1] == P0)
	{
		auto temp = P0;
		P0 = P1;
		P1 = temp;
		P5 = PointHandleList_P1P5[0];
	}
	else if (PointHandleList_P1P5[0] == P1)
	{
		P5 = PointHandleList_P1P5[1];
	}
	else if (PointHandleList_P1P5[1] == P1)
	{
		P5 = PointHandleList_P1P5[0];
	}
	else
	{
		return;
	}
	//------------------ input check is done -------------------------------------------//

	m_OutputMesh_Mixed.MergeConnectivityOfPoint(P5, P2);

}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_SplitQuadToTwoBigTriangle_If_necessary()
{// Quad Score <= 0.5 then split
	auto PointCount_input = m_InputMesh.GetPointCount();

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		if (it.Face().GetPointCount() == 4)
		{
			auto PointHandleList = it.Face().GetPointHandleList();
			if (PointHandleList[0].GetIndex() < PointCount_input && PointHandleList[1].GetIndex() < PointCount_input
				&& PointHandleList[2].GetIndex() < PointCount_input && PointHandleList[3].GetIndex() < PointCount_input)
			{
				auto Score = this->EvaluateQuad(m_OutputMesh_Mixed, PointHandleList[0], PointHandleList[1], PointHandleList[2], PointHandleList[3]);
				if (Score < 0.5)
				{
					m_OutputMesh_Mixed.DeleteFace(it.GetFaceHandle());
					m_OutputMesh_Mixed.AddFaceByPoint({ PointHandleList[0], PointHandleList[1], PointHandleList[2] });
					m_OutputMesh_Mixed.AddFaceByPoint({ PointHandleList[0], PointHandleList[2], PointHandleList[3] });
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildMixedTriQuadMesh_MergeTwoBigTriangle_If_necessary()
{// Quad Score > 0.5 then merge

	auto PointCount_input = m_InputMesh.GetPointCount();

	DenseVector<EdgeHandleType> CandidateList_Edge;
	DenseVector<DenseVector<FaceHandleType, 2>> CandidateList_TrianglePair;
	DenseVector<DenseVector<PointHandleType>> CandidateList_Quad;

	DenseVector<ScalarType> ScoreList;

	for (auto it = m_OutputMesh_Mixed.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto AdjacentFaceHandleList = it.Edge().GetAdjacentFaceHandleList();
		if (AdjacentFaceHandleList.GetLength() == 2)
		{
			auto FaceH0 = AdjacentFaceHandleList[0];
			auto FaceH1 = AdjacentFaceHandleList[1];
			if (m_OutputMesh_Mixed.Face(FaceH0).GetPointCount() == 3 && m_OutputMesh_Mixed.Face(FaceH1).GetPointCount() == 3)
			{
				DenseVector<PointHandleType> PointHandleList0 = m_OutputMesh_Mixed.Face(FaceH0).GetPointHandleList();
				DenseVector<PointHandleType> PointHandleList1 = m_OutputMesh_Mixed.Face(FaceH1).GetPointHandleList();
				if (PointHandleList0[0].GetIndex() < PointCount_input && PointHandleList0[1].GetIndex() < PointCount_input && PointHandleList0[2].GetIndex() < PointCount_input
					&&PointHandleList1[0].GetIndex() < PointCount_input && PointHandleList1[1].GetIndex() < PointCount_input && PointHandleList1[2].GetIndex() < PointCount_input)
				{				
					DenseVector<PointHandleType> PointHandleList_Edge = it.Edge().GetPointHandleList();
					auto Set0 = this->SetDiff(PointHandleList0, PointHandleList_Edge);
					auto P0 = Set0[0];					
					auto PointHandleList_012 = m_OutputMesh_Mixed.Face(FaceH0).GetPointHandleList_LeadBy(P0);
					auto P1 = PointHandleList_012[1];
					auto P2 = PointHandleList_012[2];
					auto Set3 = this->SetDiff(PointHandleList1, PointHandleList_Edge);
					auto P3 = Set3[0];
					auto Score = this->EvaluateQuad(m_OutputMesh_Mixed, P0, P1, P3, P2);
					ScoreList.Append(Score);
					CandidateList_Edge.Append(it.GetEdgeHandle());
					CandidateList_TrianglePair.Append({ FaceH0, FaceH1 });
					CandidateList_Quad.Append({ P0, P1, P3, P2 });
				}
			}
		}
	}

	auto IndexList_sort = ScoreList.Sort("descend");
	for (int_max k = 0; k < IndexList_sort.GetLength(); ++k)
	{
		auto Index = IndexList_sort[k];
		if (ScoreList[Index] > 0.5)
		{
			auto EdgeH = CandidateList_Edge[Index];
			if (m_OutputMesh_Mixed.IsValidHandle(EdgeH) == true)
			{
				auto TrianglePair = CandidateList_TrianglePair[Index];
				if (m_OutputMesh_Mixed.IsValidHandle(TrianglePair[0]) == true && m_OutputMesh_Mixed.IsValidHandle(TrianglePair[1]) == true)
				{
					m_OutputMesh_Mixed.DeleteFace(TrianglePair[0]);
					m_OutputMesh_Mixed.DeleteFace(TrianglePair[1]);
					m_OutputMesh_Mixed.DeleteEdge(EdgeH);
					m_OutputMesh_Mixed.AddFaceByPoint(CandidateList_Quad[Index]);
				}
			}
		}
	}
}


template<typename ScalarType>
void QuadSurfaceRemesher3<ScalarType>::BuildQuadMesh()
{	// subdivide m_OutputMesh_Mixed to get m_OutputMesh_Quad



}


template<typename ScalarType>
DenseVector<typename QuadSurfaceRemesher3<ScalarType>::PointHandleType> QuadSurfaceRemesher3<ScalarType>::ConvertIndexToPointHandle(const DenseVector<int_max>& IndexList)
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
DenseVector<typename QuadSurfaceRemesher3<ScalarType>::FaceHandleType> QuadSurfaceRemesher3<ScalarType>::ConvertIndexToFaceHandle(const DenseVector<int_max>& IndexList)
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
DenseVector<int_max> QuadSurfaceRemesher3<ScalarType>::ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList)
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
DenseVector<int_max> QuadSurfaceRemesher3<ScalarType>::ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList)
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
ScalarType QuadSurfaceRemesher3<ScalarType>::EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3)
{
	auto Point0 = TargetMesh.GetPointPosition(PointH0);
	auto Point1 = TargetMesh.GetPointPosition(PointH1);
	auto Point2 = TargetMesh.GetPointPosition(PointH2);
	auto Point3 = TargetMesh.GetPointPosition(PointH3);
	return this->EvaluateQuad(Point0, Point1, Point2, Point3);
}

template<typename ScalarType>
ScalarType QuadSurfaceRemesher3<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
{
	//================
	//  P3 -  P2 
	//  |     |   
	//  P0 -  P1 
	//================

	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	//warp angle
	ScalarType WarpAngleScore = 0;
	auto Normal_023 = ComputeTriangleNormalIn3D(Point0, Point2, Point3);
	auto Normal_012 = ComputeTriangleNormalIn3D(Point0, Point1, Point2);
	auto CosAngle_023_012 = std::abs(ComputeVectorDotProductIn3D(Normal_012, Normal_023));
	auto Normal_013 = ComputeTriangleNormalIn3D(Point0, Point1, Point3);
	auto Normal_123 = ComputeTriangleNormalIn3D(Point1, Point2, Point3);
	auto CosAngle_013_123 = std::abs(ComputeVectorDotProductIn3D(Normal_013, Normal_123));
	WarpAngleScore = std::min(CosAngle_023_012, CosAngle_013_123);
	

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
ScalarType QuadSurfaceRemesher3<ScalarType>::EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2)
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
template<typename ElementType>
DenseVector<ElementType> QuadSurfaceRemesher3<ScalarType>::Intersect(const DenseVector<ElementType>& SetA, const DenseVector<ElementType>& SetB)
{
	DenseVector<ElementType> SetC;

	if (SetA.IsEmpty() == true || SetB.IsEmpty() == true)
	{
		return SetC;
	}

	SetC.SetCapacity(std::min(SetA.GetLength(), SetB.GetLength()));

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] == SetB[n])
			{
				SetC.Append(SetA[k]);
			}
		}
	}
	return SetC;
}

template<typename ScalarType>
template<typename ElementType>
DenseVector<ElementType> QuadSurfaceRemesher3<ScalarType>::SetDiff(const DenseVector<ElementType>& SetA, const DenseVector<ElementType>& SetB) const
{// in A, NOT in B

	if (SetB.IsEmpty() == true)
	{
		return SetA;
	}

	DenseVector<ElementType> SetC;
	SetC.SetCapacity(SetA.GetLength());

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		bool Flag = false;
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] == SetB[n])
			{
				Flag = true;
				break;
			}
		}
		if (Flag == false)
		{
			SetC.Append(SetA[k]);
		}
	}
	return SetC;
}

}//namespace