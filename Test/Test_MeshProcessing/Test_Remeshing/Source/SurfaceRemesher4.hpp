#pragma once

namespace mdk
{
template<typename ScalarType>
SurfaceRemesher4<ScalarType>::SurfaceRemesher4()
{
	this->Clear();
}


template<typename ScalarType>
SurfaceRemesher4<ScalarType>::~SurfaceRemesher4()
{
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::Clear()
{
	m_InputMesh.Clear();
	m_FeatureEdgeOfInputMesh.Clear();
	m_FeaturePointOfInputMesh.Clear();
	m_Threshold_Quad_WarpAngleScore = 0.9659; // 15/360
	m_Threshold_Quad_EdgeAngleScore = 0.2929; // 45/360
	m_Threshold_Quad_AspectScore = 0.2;       // 1/5		
	this->ClearInternalData();
	m_OutputMesh.Clear();
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::ClearInternalData()
{
	m_CandidateMesh.Clear();
	m_CandidateRegionList.Clear();
	m_MiddlePointList_on_CandidateMesh.Clear();
	m_CandidateTypeList.Clear();
	m_CandidateIndexSet_Overlap_SmallTriangle.Clear();
	m_QuadCandidateIndexSet_At_SmallTriangle.Clear();
	m_QuadCandidateIndexSet_At_BigTriangle.Clear();
	m_CandidateConflictTable_TJunction.Clear();
	m_CandidateScoreList.Clear();
	m_CandidateIndicatorList.Clear();
}

template<typename ScalarType>
bool SurfaceRemesher4<ScalarType>::CheckInput()
{
	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("m_InputMesh is NOT TriangleMesh @ SurfaceRemesher4::CheckInput()")
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ SurfaceRemesher4::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ SurfaceRemesher4::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->InitilizeCandidateMesh();
	this->GenerateCandidate();
	this->EvaluateCandidate();
	this->SelectCandidate();
	this->BuildOutputMesh();
	this->BuildMixedTriQuadMesh();
	this->AdjustMixedTriQuadMesh();
	this->BuildQuadMesh();
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::InitilizeCandidateMesh()
{// add middle point of each edge

	m_CandidateMesh = m_InputMesh;

	m_MiddlePointList_on_CandidateMesh.Clear();
	m_MiddlePointList_on_CandidateMesh.Resize(m_InputMesh.GetEdgeCount());

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Edge().GetPointHandleList();
		auto Point0 = m_CandidateMesh.GetPointPosition(PointHandleList[0]);
		auto Point1 = m_CandidateMesh.GetPointPosition(PointHandleList[1]);
		auto PointM = (Point0 + Point1) / ScalarType(2);

		auto EdgeIndex = it.GetEdgeHandle().GetIndex();
		m_MiddlePointList_on_CandidateMesh[EdgeIndex] = m_CandidateMesh.AddPoint(PointM);
	}
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateCandidate()
{
	auto FaceCount_input = m_CandidateMesh.GetFaceCount();

	auto CandidateCount_output_max = FaceCount_input + 6*FaceCount_input + FaceCount_input;

	m_CandidateRegionList.Clear();
	m_CandidateRegionList.SetCapacity(CandidateCount_output_max);

	m_CandidateTypeList.Clear();
	m_CandidateTypeList.SetCapacity(CandidateCount_output_max);

	m_QuadCandidateIndexSet_At_BigTriangle.Clear();
	m_QuadCandidateIndexSet_At_BigTriangle.Resize(FaceCount_input);

	m_QuadCandidateIndexSet_At_SmallTriangle.Clear();
	m_QuadCandidateIndexSet_At_SmallTriangle.Resize(6 * FaceCount_input);

	m_CandidateIndexSet_Overlap_SmallTriangle.Clear();
	m_CandidateIndexSet_Overlap_SmallTriangle.Resize(6 * FaceCount_input);

	m_CandidateConflictTable_TJunction.Clear();
	m_CandidateConflictTable_TJunction.SetCapacity(10 * CandidateCount_output_max);

	// candidate must be generated in this order
	this->GenerateTriangleCandidate_Type1();
	this->GenerateTriangleCandidate_Type2();
	this->GenerateQuadCandidate_Type3();
	this->GenerateQuadCandidate_Type4();
	this->Build_CandidateConflictTable_Overlap();
	this->Build_CandidateConflictTable_TJunction();
	this->Preserve_FeatureEdge();
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateTriangleCandidate_Type1()
{
	for (auto it = m_InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto EdgeHandleList = it.Face().GetEdgeHandleList();
		auto PointHandleList = it.Face().GetPointHandleList();
		m_CandidateRegionList.Append(PointHandleList);
		m_CandidateTypeList.Append(1); 
	}
	m_BigTriangleCandidateCount = m_InputMesh.GetFaceCount();
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateTriangleCandidate_Type2()
{
	//Attention: must generate all 6 small-triangle-candidate for each big triangle 
	//           otherwise m_CandidateIndexSet_Overlap_SmallTriangle, m_QuadCandidateIndexSet_At_SmallTriangle will be wrong
	//----------------------------------------------------------------------------------------------------------------------

	for (auto it = m_InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{   //-------------------------
		//        P0
		//      /    \
		//     Pa     Pc
		//    /         \
		//   P1---Pb----P2
		//-------------------------		
		//generate small triangle candidate

		auto BigTriangleCandidateIndex = it.GetFaceHandle().GetIndex();

		auto EdgeHandleList = it.Face().GetEdgeHandleList();
		auto PointHandleList = it.Face().GetPointHandleList();		
		auto PointH0 = PointHandleList[0];
		auto PointH1 = PointHandleList[1];
		auto PointH2 = PointHandleList[2];
		auto PointHa = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[0].GetIndex()];
		auto PointHb = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[1].GetIndex()];
		auto PointHc = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[2].GetIndex()];
		
		//small triangle candidate
		DenseVector<PointHandleType> Candidate0, Candidate1, Candidate2, Candidate3, Candidate4, Candidate5;
		Candidate0 = { PointH0, PointH1, PointHb };
		Candidate1 = { PointH0, PointHb, PointH2 };
		Candidate2 = { PointH1, PointHc, PointH0 };
		Candidate3 = { PointH1, PointH2, PointHc };
		Candidate4 = { PointH2, PointH0, PointHa };
		Candidate5 = { PointH2, PointHa, PointH1 };

		auto CandidateHandle0 = m_CandidateMesh.AddFaceByPoint(Candidate0);
		auto CandidateHandle1 = m_CandidateMesh.AddFaceByPoint(Candidate1);
		auto CandidateHandle2 = m_CandidateMesh.AddFaceByPoint(Candidate2);
		auto CandidateHandle3 = m_CandidateMesh.AddFaceByPoint(Candidate3);
		auto CandidateHandle4 = m_CandidateMesh.AddFaceByPoint(Candidate4);
		auto CandidateHandle5 = m_CandidateMesh.AddFaceByPoint(Candidate5);

		m_CandidateRegionList.Append(Candidate0);
		m_CandidateRegionList.Append(Candidate1);
		m_CandidateRegionList.Append(Candidate2);
		m_CandidateRegionList.Append(Candidate3);
		m_CandidateRegionList.Append(Candidate4);
		m_CandidateRegionList.Append(Candidate5);

		m_CandidateTypeList.Append(2);
		m_CandidateTypeList.Append(2);		
		m_CandidateTypeList.Append(2);
		m_CandidateTypeList.Append(2);		
		m_CandidateTypeList.Append(2);
		m_CandidateTypeList.Append(2);

		auto Index0 = CandidateHandle0.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(CandidateHandle0.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(CandidateHandle2.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(CandidateHandle3.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(CandidateHandle4.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index0].Append(CandidateHandle5.GetIndex());
		
		auto Index1 = CandidateHandle1.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(CandidateHandle1.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(CandidateHandle2.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(CandidateHandle3.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(CandidateHandle4.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index1].Append(CandidateHandle5.GetIndex());
		
		auto Index2 = CandidateHandle2.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(CandidateHandle2.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(CandidateHandle0.GetIndex());
 	    m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(CandidateHandle1.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(CandidateHandle4.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index2].Append(CandidateHandle5.GetIndex());
		
		auto Index3 = CandidateHandle3.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(CandidateHandle3.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(CandidateHandle0.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(CandidateHandle1.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(CandidateHandle4.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index3].Append(CandidateHandle5.GetIndex());
		
		auto Index4 = CandidateHandle4.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(CandidateHandle4.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(CandidateHandle0.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(CandidateHandle1.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(CandidateHandle2.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index4].Append(CandidateHandle3.GetIndex());

		auto Index5 = CandidateHandle5.GetIndex() - m_BigTriangleCandidateCount;
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(BigTriangleCandidateIndex);
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(CandidateHandle5.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(CandidateHandle0.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(CandidateHandle1.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(CandidateHandle2.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[Index5].Append(CandidateHandle3.GetIndex());	
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateQuadCandidate_Type3()
{
	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto tempIndex_preseve_edge = m_FeatureEdgeOfInputMesh.ExactMatch("first", it.GetEdgeHandle());
		if (tempIndex_preseve_edge < 0)
		{
			this->GenerateQuadCandidate_Type3(it.GetEdgeHandle());
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateQuadCandidate_Type3(EdgeHandleType EdgeHandle_input)
{
	auto AdjacentFaceHandleList = m_InputMesh.Edge(EdgeHandle_input).GetAdjacentFaceHandleList();
	if (AdjacentFaceHandleList.GetLength() != 2)
	{
		return;
	}

	//-----------------------------------------------------------	
	//         P0
	//       / a  \
    //     P1-----P2
	//       \ b  /
	//         P3
	//
	// assume  P0->P1->P2->P0 order in Face_a 
	// in Face_b, order may be P1->P3->P2->P3 or P1->P2->P3->P1
	//----------------------------------------------------------
	auto FaceH_a = AdjacentFaceHandleList[0];
	auto EdgeHandleList_a = m_InputMesh.Face(FaceH_a).GetEdgeHandleList();
	auto PointHandleList_a = m_InputMesh.Face(FaceH_a).GetPointHandleList();

	auto FaceH_b = AdjacentFaceHandleList[1];
	auto EdgeHandleList_b = m_InputMesh.Face(FaceH_b).GetEdgeHandleList();
	auto PointHandleList_b = m_InputMesh.Face(FaceH_b).GetPointHandleList();

	PointHandleType PointH0, PointH1, PointH2, PointH3;
	EdgeHandleType EdgeH_P0P1, EdgeH_P1P2, EdgeH_P2P0, EdgeH_P1P3, EdgeH_P3P2;

	//get PointH0
	for (int_max k = 0; k < 3; ++k)
	{
		if (PointHandleList_a[k] != PointHandleList_b[0] && PointHandleList_a[k] != PointHandleList_b[1] && PointHandleList_a[k] != PointHandleList_b[2])
		{
			PointH0 = PointHandleList_a[k];
			break;
		}
	}

	//get PointH1, PointH2, EdgeH_P0P1, EdgeH_P1P2, EdgeH_P2P0
	for (int_max k = 0; k < 3; ++k)
	{
		if (PointHandleList_a[k] == PointH0)
		{
			auto Index1 = k + 1;
			auto Index2 = k + 2;
			if (Index1 >= 3)
			{
				Index1 -= 3;
			}
			if (Index2 >= 3)
			{
				Index2 -= 3;
			}
			PointH1 = PointHandleList_a[Index1];
			PointH2 = PointHandleList_a[Index2];
			EdgeH_P0P1 = EdgeHandleList_a[k];
			EdgeH_P1P2 = EdgeHandleList_a[Index1];
			EdgeH_P2P0 = EdgeHandleList_a[Index2];
			break;
		}
	}

	//get PointH3
	for (int_max k = 0; k < 3; ++k)
	{
		if (PointHandleList_b[k] != PointHandleList_a[0] && PointHandleList_b[k] != PointHandleList_a[1] && PointHandleList_b[k] != PointHandleList_a[2])
		{
			PointH3 = PointHandleList_b[k];
			break;
		}
	}

	//------------------------- check the quality of the quad ---------------
	auto Score = this->EvaluateQuad(m_InputMesh, PointH0, PointH1, PointH2, PointH3);
	if (Score < 0.1)
	{
		return;//do not generate this quad
	}
	//------------------------------------------------------------------------

	EdgeH_P1P3 = m_InputMesh.GetEdgeHandleByPoint(PointH1, PointH3);//same as using m_CandidateMesh
	EdgeH_P3P2 = m_InputMesh.GetEdgeHandleByPoint(PointH3, PointH2);//same as using m_CandidateMesh
	
	// middle point on each edge
	auto PointH_m_P0P1 = m_MiddlePointList_on_CandidateMesh[EdgeH_P0P1.GetIndex()];
	auto PointH_m_P1P2 = m_MiddlePointList_on_CandidateMesh[EdgeH_P1P2.GetIndex()];
	auto PointH_m_P2P0 = m_MiddlePointList_on_CandidateMesh[EdgeH_P2P0.GetIndex()];
	auto PointH_m_P1P3 = m_MiddlePointList_on_CandidateMesh[EdgeH_P1P3.GetIndex()];
	auto PointH_m_P3P2 = m_MiddlePointList_on_CandidateMesh[EdgeH_P3P2.GetIndex()];

	DenseVector<PointHandleType, 4> Candidate = { PointH0, PointH1, PointH3, PointH2 };	
	auto CandidateHandle = m_CandidateMesh.AddFaceByPoint(Candidate); 	
	m_CandidateRegionList.Append(Candidate);
	m_CandidateTypeList.Append(3);	

	m_QuadCandidateIndexSet_At_BigTriangle[FaceH_a.GetIndex()].Append(CandidateHandle.GetIndex());
	m_QuadCandidateIndexSet_At_BigTriangle[FaceH_b.GetIndex()].Append(CandidateHandle.GetIndex());

	for (int_max k = 0; k < 6; ++k)
	{
		m_CandidateIndexSet_Overlap_SmallTriangle[6*FaceH_a.GetIndex()+k].Append(CandidateHandle.GetIndex());
		m_CandidateIndexSet_Overlap_SmallTriangle[6*FaceH_b.GetIndex()+k].Append(CandidateHandle.GetIndex());
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateQuadCandidate_Type4()
{
	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto tempIndex_preseve_edge = m_FeatureEdgeOfInputMesh.ExactMatch("first", it.GetEdgeHandle());
		if (tempIndex_preseve_edge < 0)
		{
			this->GenerateQuadCandidate_Type4(it.GetEdgeHandle());
		}
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::GenerateQuadCandidate_Type4(EdgeHandleType EdgeHandle_input)
{
	auto AdjacentFaceHandleList = m_InputMesh.Edge(EdgeHandle_input).GetAdjacentFaceHandleList();
	if (AdjacentFaceHandleList.GetLength() <= 0)
	{
		return;
	}

	// split and merge many triangle to get a quad
	for (int_max i = 0; i < AdjacentFaceHandleList.GetLength(); ++i)
	{
		//----------------------------	
		// P3: middle point of edge e3
		// P4: middle point of edge e4
		// P5: middle point of edge e0
		//
		//       P4          P3   
		//   ----e4--- P0 ---e3------
		//    \   |  /    \   |   /
		//     \  | /e2  e1\  |  /
		//      \ |/        \ |/
		//       P1 ---e0----P2          
        //             P5
		//----------------------------

		EdgeHandleType EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4;
		PointHandleType PointH0, PointH1, PointH2, PointH3, PointH4, PointH5;

		EdgeH0 = EdgeHandle_input;
		PointH5 = m_MiddlePointList_on_CandidateMesh[EdgeH0.GetIndex()];

		auto FaceH_middle = AdjacentFaceHandleList[i];
		{
			auto tempPointHandleList = m_InputMesh.Face(FaceH_middle).GetPointHandleList();
			auto tempEdgeHandleList = m_InputMesh.Face(FaceH_middle).GetEdgeHandleList();
			if (EdgeH0 == tempEdgeHandleList[0])
			{
				PointH0 = tempPointHandleList[2];
				PointH1 = tempPointHandleList[0];
				PointH2 = tempPointHandleList[1];

				EdgeH1 = tempEdgeHandleList[1];
				EdgeH2 = tempEdgeHandleList[2];
			}
			else if (EdgeH0 == tempEdgeHandleList[1])
			{
				PointH0 = tempPointHandleList[0];
				PointH1 = tempPointHandleList[1];
				PointH2 = tempPointHandleList[2];

				EdgeH1 = tempEdgeHandleList[2];
				EdgeH2 = tempEdgeHandleList[0];
			}
			else//(EdgeH0 == tempEdgeHandleList[2])
			{
				PointH0 = tempPointHandleList[1];
				PointH1 = tempPointHandleList[2];
				PointH2 = tempPointHandleList[0];

				EdgeH1 = tempEdgeHandleList[0];
				EdgeH2 = tempEdgeHandleList[1];
			}
		}

		FaceHandleType FaceH_left;
		FaceH_left.SetToInvalid();
		{
			auto tempAdjacentFaceHandleList = m_InputMesh.Edge(EdgeH2).GetAdjacentFaceHandleList();
			if (tempAdjacentFaceHandleList.GetLength() > 1)
			{
				if (tempAdjacentFaceHandleList[0] != FaceH_middle)
				{
					FaceH_left = tempAdjacentFaceHandleList[0];
				}
				else
				{
					FaceH_left = tempAdjacentFaceHandleList[1];
				}
			}
		}

		FaceHandleType FaceH_right;
		FaceH_right.SetToInvalid();
		{
			auto tempAdjacentFaceHandleList = m_InputMesh.Edge(EdgeH1).GetAdjacentFaceHandleList();
			if (tempAdjacentFaceHandleList.GetLength() > 1)
			{
				if (tempAdjacentFaceHandleList[0] != FaceH_middle)
				{
					FaceH_right = tempAdjacentFaceHandleList[0];
				}
				else
				{
					FaceH_right = tempAdjacentFaceHandleList[1];
				}
			}
		}

		//get EdgeH3 and EdgeH4
		EdgeH3.SetToInvalid();
		EdgeH4.SetToInvalid();
		{// Known: P0, P1, e1, e2
			auto tempEdgeHandleList = m_InputMesh.Point(PointH0).GetAdjacentEdgeHandleList();
			for (int_max m = 0; m < tempEdgeHandleList.GetLength(); ++m)
			{
				if (tempEdgeHandleList[m] != EdgeH1 && tempEdgeHandleList[m] != EdgeH2)
				{
					bool Flag_e3 = false;
					bool Flag_e4 = false;
					auto tempFaceHandleList = m_InputMesh.Edge(tempEdgeHandleList[m]).GetAdjacentFaceHandleList();
					for (int_max n = 0; n < tempFaceHandleList.GetLength(); ++n)
					{
						if (tempFaceHandleList[n] == FaceH_left)
						{
							EdgeH4 = tempEdgeHandleList[m];
							Flag_e4 = true;

						}
						else if (tempFaceHandleList[n] == FaceH_right)
						{
							EdgeH3 = tempEdgeHandleList[m];
							Flag_e3 = true;
						}
					}

					if (Flag_e3 == true && Flag_e4 == true)
					{
						break;
					}
				}
			}
		}

		//Get PointH3, PointH4
		PointH3.SetToInvalid();
		if (EdgeH3.GetIndex() >= 0)
		{
			PointH3 = m_MiddlePointList_on_CandidateMesh[EdgeH3.GetIndex()];
		}
		PointH4.SetToInvalid();
		if (EdgeH4.GetIndex() >= 0)
		{
			PointH4 = m_MiddlePointList_on_CandidateMesh[EdgeH4.GetIndex()];
		}

		//check if P0 is preserved
		bool Flag_P0_preserved = false;
		{
			auto tempIndex = m_FeaturePointOfInputMesh.ExactMatch("first", PointH0);
			if (tempIndex >= 0)
			{
				Flag_P0_preserved = true;
			}
		}

		//check if e1/e2/e3/e4 is preserved
		bool Flag_Edge1_preserved = false;
		bool Flag_Edge2_preserved = false;
		bool Flag_Edge3_preserved = false;
		bool Flag_Edge4_preserved = false;
		{
			auto tempIndex1 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeH1);
			if (tempIndex1 >= 0)
			{
				Flag_Edge1_preserved = true;
			}
			auto tempIndex2 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeH2);
			if (tempIndex2 >= 0)
			{
				Flag_Edge2_preserved = true;
			}
			auto tempIndex3 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeH3);
			if (tempIndex3 >= 0)
			{
				Flag_Edge3_preserved = true;
			}
			auto tempIndex4 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeH4);
			if (tempIndex4 >= 0)
			{
				Flag_Edge4_preserved = true;
			}
		}

		//add the quad {P1, P2, P3, P4} = {P1 P5 P2 P3 P0 P4} whenever possible
		if (PointH3.GetIndex() >= 0 && PointH4.GetIndex() >= 0)
		{
			if (Flag_P0_preserved == false && Flag_Edge1_preserved == false && Flag_Edge2_preserved == false && Flag_Edge3_preserved == false && Flag_Edge4_preserved == false)
			{
				//----------------------------	
				//       P4          P3   
				//   ----e4--- P0 ---e3------
				//    \   |  /    \   |   /
				//     \  | /e2  e1\  |  /
				//      \ |/        \ |/
				//       P1 ---e0----P2     
				//             P5
				//----------------------------

				//-------------------------check the quality of the quad ---------------
				auto Score = this->EvaluateQuad(m_InputMesh, PointH1, PointH2, PointH3, PointH4);
				if (Score < 0.1)
				{
					return;//do not generate this quad
				}
				//------------------------------------------------------------------------

				DenseVector<PointHandleType> Candidate = { PointH1, PointH2, PointH3, PointH4 };
				DenseVector<PointHandleType> CandidateRegion = { PointH1, PointH5, PointH2, PointH3, PointH0, PointH4 };
				auto CandidateHandle = m_CandidateMesh.AddFaceByPoint(Candidate);
				m_CandidateRegionList.Append(CandidateRegion);
				m_CandidateTypeList.Append(4);

				DenseVector<PointHandleType> Triangle_P1P0P4 = { PointH1, PointH0, PointH4 };
				auto TriangleH_P1P0P4 = m_CandidateMesh.GetFaceHandleByPoint(Triangle_P1P0P4);
				m_QuadCandidateIndexSet_At_SmallTriangle[TriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());

				DenseVector<PointHandleType> Triangle_P2P3P0 = { PointH2, PointH3, PointH0 };
				auto TriangleH_P2P3P0 = m_CandidateMesh.GetFaceHandleByPoint(Triangle_P2P3P0);
				m_QuadCandidateIndexSet_At_SmallTriangle[TriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());

				m_QuadCandidateIndexSet_At_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateHandle.GetIndex());

				for (int_max k = 0; k < 6; ++k)
				{
					m_CandidateIndexSet_Overlap_SmallTriangle[6 * FaceH_middle.GetIndex() + k].Append(CandidateHandle.GetIndex());
				}
				m_CandidateIndexSet_Overlap_SmallTriangle[TriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());
				m_CandidateIndexSet_Overlap_SmallTriangle[TriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());
			}
		}
		
		// If AdjacentPointCount_P0 is 5 or 7 or less, add special quad
		if (m_InputMesh.Point(PointH0).GetAdjacentPointCount() != 6)
		{
			if (PointH3.GetIndex() >= 0)//add quad {P1, P2, P3, P0} ={P1 P5 P2 P3 P0}
			{
				if (Flag_Edge1_preserved == false && Flag_Edge3_preserved == false)
				{
					//----------------------------	
					//       P4          P3   
					//   ----e4--- P0 ---e3------
					//    \   |  /    \   |   /
					//     \  | /e2  e1\  |  /
					//      \ |/        \ |/
					//       P1 ---e0----P2     
					//             P5
					//----------------------------

					//-------------------------check the quality of the quad ---------------
					auto Score = this->EvaluateQuad(m_InputMesh, PointH1, PointH2, PointH3, PointH0);
					if (Score < 0.1)
					{
						return;//do not generate this quad
					}
					//------------------------------------------------------------------------

					DenseVector<PointHandleType> Candidate = { PointH1, PointH2, PointH3, PointH0 };
					DenseVector<PointHandleType> CandidateRegion = { PointH1, PointH5, PointH2, PointH3, PointH0 };
					auto CandidateHandle = m_CandidateMesh.AddFaceByPoint(Candidate);
					m_CandidateRegionList.Append(CandidateRegion);
					m_CandidateTypeList.Append(4);

					DenseVector<PointHandleType> Triangle_P2P3P0 = { PointH2, PointH3, PointH0 };
					auto TriangleH_P2P3P0 = m_CandidateMesh.GetFaceHandleByPoint(Triangle_P2P3P0);
					m_QuadCandidateIndexSet_At_SmallTriangle[TriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());

					m_QuadCandidateIndexSet_At_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateHandle.GetIndex());

					for (int_max k = 0; k < 6; ++k)
					{
						m_CandidateIndexSet_Overlap_SmallTriangle[6*FaceH_middle.GetIndex() + k].Append(CandidateHandle.GetIndex());
					}
					m_CandidateIndexSet_Overlap_SmallTriangle[TriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());
				}
			}
			if (PointH4.GetIndex() >= 0)//add quad {P1, P2, P0, P4} = {P1 P5 P2 P0 P4}
			{
				if (Flag_Edge2_preserved == false && Flag_Edge4_preserved == false)
				{
					//----------------------------	
					//       P4          P3   
					//   ----e4--- P0 ---e3------
					//    \   |  /    \   |   /
					//     \  | /e2  e1\  |  /
					//      \ |/        \ |/
					//       P1 ---e0----P2     
					//             P5
					//----------------------------

					//-------------------------check the quality of the quad ---------------
					auto Score = this->EvaluateQuad(m_InputMesh, PointH1, PointH2, PointH0, PointH4);
					if (Score < 0.1)
					{
						return;//do not generate this quad
					}
					//------------------------------------------------------------------------

					DenseVector<PointHandleType> Candidate = { PointH1, PointH2, PointH0, PointH4 };
					DenseVector<PointHandleType> CandidateRegion = { PointH1, PointH5, PointH2, PointH0, PointH4 };
					auto CandidateHandle = m_CandidateMesh.AddFaceByPoint(Candidate);
					m_CandidateRegionList.Append(CandidateRegion);
					m_CandidateTypeList.Append(4);

					DenseVector<PointHandleType> Triangle_P1P0P4 = { PointH1, PointH0, PointH4 };
					auto TriangleH_P1P0P4 = m_CandidateMesh.GetFaceHandleByPoint(Triangle_P1P0P4);
					m_QuadCandidateIndexSet_At_SmallTriangle[TriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());

					m_QuadCandidateIndexSet_At_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateHandle.GetIndex());

					for (int_max k = 0; k < 6; ++k)
					{
						m_CandidateIndexSet_Overlap_SmallTriangle[6*FaceH_middle.GetIndex() + k].Append(CandidateHandle.GetIndex());
					}
					m_CandidateIndexSet_Overlap_SmallTriangle[TriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount].Append(CandidateHandle.GetIndex());
				}
			}
		}
	}
}



template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::Build_CandidateConflictTable_Overlap()
{
	auto PointCount_input = m_InputMesh.GetPointCount();
	auto EdgeCount_input = m_InputMesh.GetEdgeCount();
	auto FaceCount_input = m_InputMesh.GetFaceCount();
	auto CandidateCount = m_CandidateMesh.GetFaceCount();
	m_CandidateConflictTable_Overlap.Clear();
	m_CandidateConflictTable_Overlap.Resize(CandidateCount);
	
	//update m_CandidateIndexSet_Overlap_SmallTriangle
	for (int_max k = 0; k < FaceCount_input; ++k)
	{
		DenseVector<int_max> IndexList_small = {6*k, 6*k+1, 6*k+2, 6*k+3, 6*k+4, 6*k+5};
		const auto& CandidateSet0 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[0]];
		const auto& CandidateSet1 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[1]];
		const auto& CandidateSet2 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[2]];
		const auto& CandidateSet3 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[3]];
		const auto& CandidateSet4 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[4]];
		const auto& CandidateSet5 = m_QuadCandidateIndexSet_At_SmallTriangle[IndexList_small[5]];		
		
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[0]].Append(CandidateSet2);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[0]].Append(CandidateSet3);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[0]].Append(CandidateSet4);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[0]].Append(CandidateSet5);

		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[1]].Append(CandidateSet2);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[1]].Append(CandidateSet3);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[1]].Append(CandidateSet4);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[1]].Append(CandidateSet5);

		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[2]].Append(CandidateSet0);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[2]].Append(CandidateSet1);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[2]].Append(CandidateSet4);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[2]].Append(CandidateSet5);

		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[3]].Append(CandidateSet0);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[3]].Append(CandidateSet1);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[3]].Append(CandidateSet4);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[3]].Append(CandidateSet5);

		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[4]].Append(CandidateSet0);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[4]].Append(CandidateSet1);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[4]].Append(CandidateSet2);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[4]].Append(CandidateSet3);

		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[5]].Append(CandidateSet0);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[5]].Append(CandidateSet1);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[5]].Append(CandidateSet2);
		m_CandidateIndexSet_Overlap_SmallTriangle[IndexList_small[5]].Append(CandidateSet3);
	}

	for (int_max k = 0; k < CandidateCount; ++k)
	{
		const auto& CandidateRegion = m_CandidateRegionList[k];
		DenseVector<int_max> CandidateIndexList_Overlap;
		if (CandidateRegion.GetLength() == 3)//small triangle or big triangle
		{
			if (m_CandidateTypeList[k] == 1)//big
			{
				for (int_max n = 0; n < 6; ++n)
				{
					CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[6*k + n]);
				}
				CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
			}
			else//small
			{
				CandidateIndexList_Overlap = m_CandidateIndexSet_Overlap_SmallTriangle[k - m_BigTriangleCandidateCount];
			}
			CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
		}
		else if (CandidateRegion.GetLength() == 4)//quad from two big-triangle
		{
			const auto& CandidateRegion = m_CandidateRegionList[k];
			auto BigTriangleHa = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[1], CandidateRegion[3] });
			auto BigTriangleHb = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[1], CandidateRegion[2], CandidateRegion[3] });
			for (int_max n = 0; n < 6; ++n)
			{
				CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[BigTriangleHa.GetIndex() * 6 + n]);
				CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[BigTriangleHb.GetIndex() * 6 + n]);
			}
			CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
		}
		else if (CandidateRegion.GetLength() == 5)//quad from one big-triangle + one small-triangle
		{
			//----------------------------	
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2     
			//             P5
			//----------------------------
			const auto& CandidateRegion = m_CandidateRegionList[k];
			auto PointH_0_or_3 = CandidateRegion[3];//PointH0 or PointH3
			if (PointH_0_or_3.GetIndex() >= PointCount_input)
			{//PointH3, quad{P1 P2 P3 P0}={P1 P5 P2 P3 P0}
				auto SmallTriangleH_P2P3P0 = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[2], CandidateRegion[3], CandidateRegion[4] });
				auto BigTriangleH = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[2], CandidateRegion[4] });
				CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[SmallTriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount]);
				for (int_max n = 0; n < 6; ++n)
				{
					CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[BigTriangleH.GetIndex() * 6 + n]);
				}
				CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
			}
			else
			{//PointH0, quad{P1 P2 P0 P4}={P1 P5 P2 P0 P4}
				const auto& CandidateRegion = m_CandidateRegionList[k];
				auto SmallTriangleH_P1P0P4 = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[3], CandidateRegion[4] });
				auto BigTriangleH = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[2], CandidateRegion[3] });
				CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[SmallTriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount]);
				for (int_max n = 0; n < 6; ++n)
				{
					CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[BigTriangleH.GetIndex() * 6 + n]);
				}
				CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
			}
		}
		else if (CandidateRegion.GetLength() == 6)//quad from one big-triangle + two small-triangle
		{
			//----------------------------	
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2     
			//             P5
			//----------------------------
			// quad{P1 P2 P3 P4}={P1 P5 P2 P3 P0 P4}
			const auto& CandidateRegion = m_CandidateRegionList[k];
			auto SmallTriangleH_P1P0P4 = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[4], CandidateRegion[5] });
			auto SmallTriangleH_P2P3P0 = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[2], CandidateRegion[3], CandidateRegion[4] });
			auto BigTriangleH = m_CandidateMesh.GetFaceHandleByPoint({ CandidateRegion[0], CandidateRegion[2], CandidateRegion[4] });
			CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[SmallTriangleH_P1P0P4.GetIndex() - m_BigTriangleCandidateCount]);
			CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[SmallTriangleH_P2P3P0.GetIndex() - m_BigTriangleCandidateCount]);
			for (int_max n = 0; n < 6; ++n)
			{
				CandidateIndexList_Overlap.Append(m_CandidateIndexSet_Overlap_SmallTriangle[BigTriangleH.GetIndex() * 6 + n]);
			}
			CandidateIndexList_Overlap = CandidateIndexList_Overlap.GetSubSet(CandidateIndexList_Overlap.FindUnique());
		}
		else
		{
			MDK_Error("somthing is wrong @ SurfaceRemesher4::Build_CandidateConflictTable_Overlap() ")
		}

		for (int_max n = 0; n < CandidateIndexList_Overlap.GetLength(); ++n)
		{
			if (CandidateIndexList_Overlap[n] == k)
			{
				CandidateIndexList_Overlap.Delete(n);
				break;
			}
		}

		m_CandidateConflictTable_Overlap[k] = CandidateIndexList_Overlap;
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::Build_CandidateConflictTable_TJunction()
{
	auto PointCount_input = m_InputMesh.GetPointCount();
	auto EdgeCount_input = m_InputMesh.GetEdgeCount();
	auto CandidateCount = m_CandidateMesh.GetFaceCount();
	m_CandidateConflictTable_TJunction.Clear();
	m_CandidateConflictTable_TJunction.Resize(CandidateCount);

	for (int_max k = 0; k < CandidateCount; ++k)
	{	
		const auto& CandidateRegion = m_CandidateRegionList[k];
		DenseVector<int_max> CandidateIndexList_Conflict;
		if (CandidateRegion.GetLength() == 3)//small triangle or big triangle
		{
			FaceHandleType CandidateHandle;
			CandidateHandle.SetIndex(k);
			auto EdgeHandleList = m_CandidateMesh.Face(CandidateHandle).GetEdgeHandleList();

			if (m_CandidateTypeList[k] == 1)//big
			{
				for (int_max n = 0; n < EdgeHandleList.GetLength(); ++n)
				{
					auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[n].GetIndex()];
					auto IndexList = this->ConvertHandleToIndex(m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList());
					CandidateIndexList_Conflict.Append(IndexList);
				}
				CandidateIndexList_Conflict = CandidateIndexList_Conflict.GetSubSet(CandidateIndexList_Conflict.FindUnique());
			}
			else//small
			{
				for (int_max n = 0; n < EdgeHandleList.GetLength(); ++n)
				{
					if (EdgeHandleList[n].GetIndex() < EdgeCount_input)
					{
						auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[n].GetIndex()];
						auto IndexList = this->ConvertHandleToIndex(m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList());
						CandidateIndexList_Conflict.Append(IndexList);
					}
				}
				CandidateIndexList_Conflict = CandidateIndexList_Conflict.GetSubSet(CandidateIndexList_Conflict.FindUnique());
			}
		}
		else if (CandidateRegion.GetLength() == 4)//quad from two big-triangle
		{
			FaceHandleType CandidateHandle;
			CandidateHandle.SetIndex(k);
			auto EdgeHandleList = m_CandidateMesh.Face(CandidateHandle).GetEdgeHandleList();

			for (int_max n = 0; n < EdgeHandleList.GetLength(); ++n)
			{
				auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[EdgeHandleList[n].GetIndex()];
				auto IndexList = this->ConvertHandleToIndex(m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList());
				CandidateIndexList_Conflict.Append(IndexList);
			}
			CandidateIndexList_Conflict = CandidateIndexList_Conflict.GetSubSet(CandidateIndexList_Conflict.FindUnique());
		}
		else if (CandidateRegion.GetLength() == 5)//quad from one big-triangle + one small-triangle
		{
			//----------------------------	
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2     
			//             P5
			//----------------------------
			auto PointH5 = CandidateRegion[1];//middle point of input edge
			auto IndexList_PointH5 = this->ConvertHandleToIndex(m_CandidateMesh.Point(PointH5).GetAdjacentFaceHandleList());
			CandidateIndexList_Conflict.Append(IndexList_PointH5);
			auto PointH_0_or_3 = CandidateRegion[3];//PointH0 or PointH3
			if (PointH_0_or_3.GetIndex() >= PointCount_input)
			{//PointH3, quad{P1 P2 P3 P0}={P1 P5 P2 P3 P0}
				auto EdgeH_P0P1 = m_CandidateMesh.GetEdgeHandleByPoint(CandidateRegion[4], CandidateRegion[0]);
				auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[EdgeH_P0P1.GetIndex()];
				auto IndexList = this->ConvertHandleToIndex(m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList());
				CandidateIndexList_Conflict.Append(IndexList);
			}
			else
			{//PointH0, quad{P1 P2 P0 P4}={P1 P5 P2 P0 P4}
				auto Edge_P2P0 = m_CandidateMesh.GetEdgeHandleByPoint(CandidateRegion[2], CandidateRegion[3]);
				auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[Edge_P2P0.GetIndex()];
				auto IndexList = this->ConvertHandleToIndex(m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList());
				CandidateIndexList_Conflict.Append(IndexList);
			}
			CandidateIndexList_Conflict = CandidateIndexList_Conflict.GetSubSet(CandidateIndexList_Conflict.FindUnique());
		}
		else if (CandidateRegion.GetLength() == 6)//quad from one big-triangle + two small-triangle
		{			
			//----------------------------	
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2     
			//             P5
			//----------------------------
			//quad{P1 P2 P3 P4}={P1 P5 P2 P3 P0 P4}
			auto PointH5 = CandidateRegion[1];//middle point of input edge
			auto PointH0 = CandidateRegion[4];//middle point of input edge
			auto IndexList_PointH5 = this->ConvertHandleToIndex(m_CandidateMesh.Point(PointH5).GetAdjacentFaceHandleList());
			auto IndexList_PointH0 = this->ConvertHandleToIndex(m_CandidateMesh.Point(PointH0).GetAdjacentFaceHandleList());
			CandidateIndexList_Conflict.Append(IndexList_PointH5);
			CandidateIndexList_Conflict.Append(IndexList_PointH0);
			CandidateIndexList_Conflict = CandidateIndexList_Conflict.GetSubSet(CandidateIndexList_Conflict.FindUnique());		
		}
		else
		{
			MDK_Error("somthing is wrong @ SurfaceRemesher4::EvaluateCandidate() ")
		}

		const auto& CandidateIndexList_Overlap = m_CandidateConflictTable_Overlap[k];
		for (int_max n = 0; n < CandidateIndexList_Conflict.GetLength(); ++n)
		{
			auto tempIndex = CandidateIndexList_Overlap.ExactMatch("first", CandidateIndexList_Conflict[n]);
			if (tempIndex < 0)
			{// no overlap					
				m_CandidateConflictTable_TJunction[k].Append(CandidateIndexList_Conflict[n]);
			}
		}
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::Preserve_FeatureEdge()
{
	// only need to check small triangle : may split feature edge
	// quad candidate, that could split feature edge, is not generated

	m_CandidateIndicatorList.Clear();
	m_CandidateIndicatorList.Resize(m_CandidateMesh.GetFaceCount());
	m_CandidateIndicatorList.Fill(-1);

	for (int_max k = 0; k < m_FeatureEdgeOfInputMesh.GetLength(); ++k)
	{
		auto EdgeIndex = m_FeatureEdgeOfInputMesh[k].GetIndex();
		auto MiddlePointH = m_MiddlePointList_on_CandidateMesh[EdgeIndex];
		auto CandidateHandleList_k = m_CandidateMesh.Point(MiddlePointH).GetAdjacentFaceHandleList();
		for (int_max n = 0; n < CandidateHandleList_k.GetLength(); ++n)
		{
			auto CandidateIndex = CandidateHandleList_k[n].GetIndex();
			if (m_CandidateTypeList[CandidateIndex] == 2)//small triangle candidate
			{
				m_CandidateIndicatorList[CandidateIndex] = 0;
			}
		}
	}
}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::EvaluateCandidate()
{
	m_CandidateScoreList.Clear();
	m_CandidateScoreList.Resize(m_CandidateMesh.GetFaceCount());
	for (int_max k = 0; k < m_CandidateScoreList.GetLength(); ++k)
	{
		FaceHandleType CandidateHandle;
		CandidateHandle.SetIndex(k);
		auto Candidate = m_CandidateMesh.Face(CandidateHandle).GetPointHandleList();
		if (Candidate.GetLength() == 3)//triangle
		{			
			auto Point0 = m_CandidateMesh.GetPointPosition(Candidate[0]);
			auto Point1 = m_CandidateMesh.GetPointPosition(Candidate[1]);
			auto Point2 = m_CandidateMesh.GetPointPosition(Candidate[2]);
			auto Score = this->EvaluateTriangle(Point0, Point1, Point2);
			
			if (m_CandidateTypeList[k] == 1)
			{
				Score *= ScalarType(0.5);//1 quad ~ 2 big triangle				
				Score *= ScalarType(0.5);//quad is better
			}
			else //if (m_CandidateTypeList[k] == 2)
			{
				//Score *= ScalarType(0.5);// big triangle is better than small one

				Score *= ScalarType(0.25);//1 quad ~ 4 small triangle				
				Score = Score - ScalarType(0.25);//use small one if absolute necessary
			}			
			
			m_CandidateScoreList[k] = Score;
		}
		else if (Candidate.GetLength() == 4)//quad from two big-triangle
		{
			auto Point0 = m_CandidateMesh.GetPointPosition(Candidate[0]);
			auto Point1 = m_CandidateMesh.GetPointPosition(Candidate[1]);
			auto Point2 = m_CandidateMesh.GetPointPosition(Candidate[2]);
			auto Point3 = m_CandidateMesh.GetPointPosition(Candidate[3]);
			m_CandidateScoreList[k] = ScalarType(0.8)*this->EvaluateQuad(Point0, Point1, Point2, Point3);
		}
		else if (Candidate.GetLength() == 5)//quad from one big-triangle + one small-triangle
		{
			auto Point0 = m_CandidateMesh.GetPointPosition(Candidate[0]);
			auto Point1 = m_CandidateMesh.GetPointPosition(Candidate[2]);
			auto Point2 = m_CandidateMesh.GetPointPosition(Candidate[3]);
			auto Point3 = m_CandidateMesh.GetPointPosition(Candidate[4]);
			m_CandidateScoreList[k] = this->EvaluateQuad(Point0, Point1, Point2, Point3);
		}
		else if (Candidate.GetLength() == 6)//quad from one big-triangle + two small-triangle
		{
			auto Point0 = m_CandidateMesh.GetPointPosition(Candidate[0]);
			auto Point1 = m_CandidateMesh.GetPointPosition(Candidate[2]);
			auto Point2 = m_CandidateMesh.GetPointPosition(Candidate[3]);
			auto Point3 = m_CandidateMesh.GetPointPosition(Candidate[5]);
			m_CandidateScoreList[k] = this->EvaluateQuad(Point0, Point1, Point2, Point3);
		}
		else
		{
			MDK_Error("somthing is wrong @ SurfaceRemesher4::EvaluateCandidate() ")
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::SelectCandidate()
{ 
	auto CandidateIndexList_sort = m_CandidateScoreList.Sort("descend");
	m_CandidateIndicatorList.Resize(m_CandidateScoreList.GetLength());
	m_CandidateIndicatorList.Fill(-1);
	for (int_max k = 0; k < m_CandidateScoreList.GetLength(); ++k)
	{
		auto Index_k = CandidateIndexList_sort[k];
		if (m_CandidateIndicatorList[Index_k] == -1)
		{
			m_CandidateIndicatorList[Index_k] = 1;
			for (int_max n = 0; n < m_CandidateConflictTable_Overlap[Index_k].GetLength(); ++n)
			{
				auto Index_n = m_CandidateConflictTable_Overlap[Index_k][n];
				m_CandidateIndicatorList[Index_n] = 0;
			}
			for (int_max n = 0; n < m_CandidateConflictTable_TJunction[Index_k].GetLength(); ++n)
			{
				auto Index_n = m_CandidateConflictTable_TJunction[Index_k][n];
				m_CandidateIndicatorList[Index_n] = 0;
			}
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::BuildMixedTriQuadMesh()
{
	m_OutputMesh_Mixed.Clear();
	m_OutputMesh_Mixed.SetCapacity(m_CandidateMesh.GetPointCount(), m_CandidateMesh.GetEdgeCount(), m_CandidateMesh.GetFaceCount());
	for (auto it = m_CandidateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto PointHandle = m_OutputMesh_Mixed.AddPoint(it.Point().GetPosition());
		auto ID = it.Point().GetID();
		if (ID >= 0)
		{
			m_OutputMesh.Point(PointHandle).SetID(ID);
		}
	}

	auto SelectedCandidateCount = m_CandidateIndicatorList.Sum();

	m_Map_FaceOfMixedMesh_to_Candidate.Clear();
	m_Map_FaceOfMixedMesh_to_Candidate.Resize(SelectedCandidateCount);

	for (int_max k = 0; k < m_CandidateIndicatorList.GetLength(); ++k)//k is Candidate Index
	{
		if (m_CandidateIndicatorList[k] == 1)
		{
			FaceHandleType CandidateHandle;
			CandidateHandle.SetIndex(k);
			auto Candidate = m_CandidateMesh.Face(CandidateHandle).GetPointHandleList();
			auto FaceHandle = m_OutputMesh_Mixed.AddFaceByPoint(Candidate);
			m_Map_FaceOfMixedMesh_to_Candidate[FaceHandle.GetIndex()] = k;
		}
	}
	/*
	for (auto it = m_OutputMesh_Mixed.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnEdge() == false)
		{
			m_OutputMesh_Mixed.DeletePoint(it.GetPointHandle());
		}
	}
	m_OutputMesh_Mixed.CleanDataStructure();
	*/
}

template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::AdjustMixedTriQuadMesh()
{
	// merge small-triangle-pair on m_OutputMesh_Mixed
	//----------------------------------   
	//   P1__P2
	//    |\ |
	//    |_\|
	//   P4  P3
	//---------------
	// P1P3 is an edge of input mesh
	//-------------------------

	// merge small-triangle-pair on m_OutputMesh_Mixed
	//-------------------------------------------------------------
	//  quad, small triangle  to quad, big triangle   to all quad
	//  quad, small triangle     quad, big triangle      
	//     ____              ____            ____
	//     |__|_\       to  |__/_\    to    |__/ \   
	//     |__|/            |__\ / 	        |__\ / 
	//----------------------------------------------------------      
	//    P7___P3      
	//     |__|_\      
	//     P6 P1 P2 
	//     |__|/          
	//    P5  P4
	//------------------------
	// move P1 toward P6
	// P1 is not on input mesh, so it can not be a feature point
	//------------------------------

	// adjust small-triangle (surrounded by quad) on m_OutputMesh_Mixed
	//-------------------------------------------------------------
	//  quad, small triangle 
	//  quad, quad triangle   
	//     ____              ____  
	//     |__|_\       to   |_/_\ 
	//     |__|_|            |_\_|
	//----------------------------------------------------------      
	//    P4___P3      
	//     |__|_\      
	//     P5 P1 P2 
	//     |__|__|          
	//    P6  P7 P8
	//------------------------
	// move P1 toward P5
	// P1 is not on input mesh, so it can not be a feature point
	//------------------------------

}


template<typename ScalarType>
void SurfaceRemesher4<ScalarType>::BuildQuadMesh()
{	// subdivide m_OutputMesh_Mixed to get m_OutputMesh_Quad



}

template<typename ScalarType>
DenseVector<int_max> SurfaceRemesher4<ScalarType>::ConvertHandleToIndex(const DenseVector<PointHandleType>& HandleList)
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
DenseVector<int_max> SurfaceRemesher4<ScalarType>::ConvertHandleToIndex(const DenseVector<FaceHandleType>& HandleList)
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
ScalarType SurfaceRemesher4<ScalarType>::EvaluateQuad(const PolygonMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3)
{
	auto Point0 = TargetMesh.GetPointPosition(PointH0);
	auto Point1 = TargetMesh.GetPointPosition(PointH1);
	auto Point2 = TargetMesh.GetPointPosition(PointH2);
	auto Point3 = TargetMesh.GetPointPosition(PointH3);
	return this->EvaluateQuad(Point0, Point1, Point2, Point3);
}

template<typename ScalarType>
ScalarType SurfaceRemesher4<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
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
ScalarType SurfaceRemesher4<ScalarType>::EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2)
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

}//namespace