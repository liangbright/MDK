#pragma once

namespace mdk
{
template<typename ScalarType>
SurfaceRemesher2<ScalarType>::SurfaceRemesher2()
{
	this->Clear();
}


template<typename ScalarType>
SurfaceRemesher2<ScalarType>::~SurfaceRemesher2()
{
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::Clear()
{
	m_InputMesh.Clear();
	m_FeatureEdgeOfInputMesh.Clear();
	m_FeaturePointOfInputMesh.Clear();
	this->ClearInternalData();
	m_OutputMesh.Clear();
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::ClearInternalData()
{
	m_MiddlePointOfEdgeOfInputMesh.Clear();
	m_CandidateList.Clear();
	m_CandidateTypeList.Clear();
	m_CandidateIndex_On_BigTriangle.Clear();
	m_CandidateConflictTable.Clear();
	m_CandidateScoreList.Clear();
	m_CandidateIndicatorList.Clear();
}

template<typename ScalarType>
bool SurfaceRemesher2<ScalarType>::CheckInput()
{
	if (m_InputMesh.CheckIfTriangleMesh() == false)
	{
		MDK_Error("m_InputMesh is NOT TriangleMesh @ SurfaceRemesher2::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ SurfaceRemesher2::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->FindMiddlePointOfEdgeOfInputMesh();
	this->GenerateCandidate();
	this->EvaluateCandidate();
	this->SelectCandidate();
	this->BuildOutputMesh();
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::FindMiddlePointOfEdgeOfInputMesh()
{// add middle point of each edge
	m_MiddlePointOfEdgeOfInputMesh.Clear();
	m_MiddlePointOfEdgeOfInputMesh.Resize(m_InputMesh.GetEdgeCount() + m_InputMesh.GetDeletedEdgeCount());

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Edge().GetPointHandleList();
		auto Point0 = m_InputMesh.GetPointPosition(PointHandleList[0]);
		auto Point1 = m_InputMesh.GetPointPosition(PointHandleList[1]);
		auto PointM = (Point0 + Point1) / ScalarType(2);

		auto EdgeIndex = it.GetEdgeHandle().GetIndex();
		m_MiddlePointOfEdgeOfInputMesh[EdgeIndex] = m_InputMesh.AddPoint(PointM);
	}
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::GenerateCandidate()
{
	auto FaceCount_input = m_InputMesh.GetFaceCount();

	auto CandidateCount_output_max = FaceCount_input + 6*FaceCount_input + FaceCount_input;

	m_CandidateList.Clear();
	m_CandidateList.SetCapacity(CandidateCount_output_max);

	m_CandidateTypeList.Clear();
	m_CandidateTypeList.SetCapacity(CandidateCount_output_max);

	m_CandidateConflictTable.Clear();
	m_CandidateConflictTable.Resize(10*CandidateCount_output_max);

	m_CandidateIndex_On_BigTriangle.Clear();
	m_CandidateIndex_On_BigTriangle.Resize(FaceCount_input + m_InputMesh.GetDeletedFaceCount());

	// candidate must be generated in this order, so m_CandidateConflictTable can be easily constructed
	this->GenerateTriangleCandidate_Type1();
	this->GenerateTriangleCandidate_Type2();
	//this->GenerateQuadCandidate_Type3();
	this->GenerateQuadCandidate_Type4();
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::GenerateTriangleCandidate_Type1()
{
	int_max FaceCandidateIndex = -1;
	for (auto it = m_InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		m_CandidateList.Append(it.Face().GetPointHandleList());
		m_CandidateTypeList.Append(1);
		FaceCandidateIndex += 1;//may not be it.GetFaceHandle().GetIndex();
		m_CandidateIndex_On_BigTriangle[FaceCandidateIndex] = FaceCandidateIndex;
		DenseVector<int_max> Conflict;
		m_CandidateConflictTable.Append(Conflict);
	}
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::GenerateTriangleCandidate_Type2()
{
	int_max FaceCandidateIndex = -1;
	for (auto it = m_InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{   //-------------------------
		//        P0
		//      /    \
		//     Pa     Pc
		//    /         \
		//   P1---Pb----P2
		//-------------------------		

		FaceCandidateIndex += 1;//may not be it.GetFaceHandle().GetIndex();

		auto PointHandleList = it.Face().GetPointHandleList();
		auto EdgeHandleList = it.Face().GetEdgeHandleList();
		auto PointH0 = PointHandleList[0];
		auto PointH1 = PointHandleList[1];
		auto PointH2 = PointHandleList[2];
		auto PointHa = m_MiddlePointOfEdgeOfInputMesh[EdgeHandleList[0].GetIndex()];
		auto PointHb = m_MiddlePointOfEdgeOfInputMesh[EdgeHandleList[1].GetIndex()];
		auto PointHc = m_MiddlePointOfEdgeOfInputMesh[EdgeHandleList[2].GetIndex()];

		DenseVector<PointHandleType> Candidate0, Candidate1, Candidate2, Candidate3, Candidate4, Candidate5;
		Candidate0 = { PointH0, PointH1, PointHb };
		Candidate1 = { PointH0, PointHb, PointH2 };
		Candidate2 = { PointH1, PointHc, PointH0 };
		Candidate3 = { PointH1, PointH2, PointHc };
		Candidate4 = { PointH2, PointH0, PointHa };
		Candidate5 = { PointH2, PointHa, PointH1 };

		auto tempIndex0 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeHandleList[0]);
		auto tempIndex1 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeHandleList[1]);
		auto tempIndex2 = m_FeatureEdgeOfInputMesh.ExactMatch("first", EdgeHandleList[2]);

		DenseVector<int_max> NewCandiateIndexList;
		NewCandiateIndexList.SetCapacity(6);

		if (tempIndex1 < 0)// no need to preserve edge P1P2
		{
			m_CandidateList.Append(Candidate0);		
			NewCandiateIndexList.Append(m_CandidateList.GetLength()-1);
			m_CandidateList.Append(Candidate1);
			NewCandiateIndexList.Append(m_CandidateList.GetLength() - 1);
			m_CandidateTypeList.Append(2);
			m_CandidateTypeList.Append(2);
		}

		if (tempIndex2 < 0)// no need to preserve edge P2P0
		{
			m_CandidateList.Append(Candidate2);
			NewCandiateIndexList.Append(m_CandidateList.GetLength() - 1);
			m_CandidateList.Append(Candidate3);
			NewCandiateIndexList.Append(m_CandidateList.GetLength() - 1);
			m_CandidateTypeList.Append(2);
			m_CandidateTypeList.Append(2);
		}

		if (tempIndex0 < 0)// no need to preserve edge P0P1
		{
			m_CandidateList.Append(Candidate4);
			NewCandiateIndexList.Append(m_CandidateList.GetLength() - 1);
			m_CandidateList.Append(Candidate5);
			NewCandiateIndexList.Append(m_CandidateList.GetLength() - 1);
			m_CandidateTypeList.Append(2);
			m_CandidateTypeList.Append(2);
		}

		m_CandidateIndex_On_BigTriangle[FaceCandidateIndex].Append(NewCandiateIndexList);

		if (tempIndex1 < 0 && tempIndex2 < 0 && tempIndex0 < 0)
		{// all six candidate added
			DenseVector<int_max> Conflict0, Conflict1, Conflict2, Conflict3, Conflict4, Conflict5;
			Conflict0 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3], NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict1 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3], NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict2 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1], NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict3 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1], NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict4 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1], NewCandiateIndexList[2], NewCandiateIndexList[3] };
			Conflict5 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1], NewCandiateIndexList[2], NewCandiateIndexList[3] };
			m_CandidateConflictTable.Append(Conflict0);
			m_CandidateConflictTable.Append(Conflict1);
			m_CandidateConflictTable.Append(Conflict2);
			m_CandidateConflictTable.Append(Conflict3);
			m_CandidateConflictTable.Append(Conflict4);
			m_CandidateConflictTable.Append(Conflict5);
			m_CandidateConflictTable[FaceCandidateIndex].Append(NewCandiateIndexList);
		}
		else if (tempIndex1 >= 0 && tempIndex2 < 0 && tempIndex0 < 0)
		{// Candidate0 and Candidate1 NOT added
			DenseVector<int_max> Conflict2, Conflict3, Conflict4, Conflict5;
			Conflict2 = { FaceCandidateIndex, NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict3 = { FaceCandidateIndex, NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict4 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3] };
			Conflict5 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3] };
			m_CandidateConflictTable.Append(Conflict2);
			m_CandidateConflictTable.Append(Conflict3);
			m_CandidateConflictTable.Append(Conflict4);
			m_CandidateConflictTable.Append(Conflict5);
			m_CandidateConflictTable[FaceCandidateIndex].Append(NewCandiateIndexList);
		}
		else if (tempIndex1 < 0 && tempIndex2 >= 0 && tempIndex0 < 0)
		{// Candidate2 and Candidate3 NOT added
			DenseVector<int_max> Conflict0, Conflict1, Conflict4, Conflict5;
			Conflict0 = { FaceCandidateIndex, NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict1 = { FaceCandidateIndex, NewCandiateIndexList[4], NewCandiateIndexList[5] };
			Conflict4 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1] };
			Conflict5 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1] };
			m_CandidateConflictTable.Append(Conflict0);
			m_CandidateConflictTable.Append(Conflict1);
			m_CandidateConflictTable.Append(Conflict4);
			m_CandidateConflictTable.Append(Conflict5);
			m_CandidateConflictTable[FaceCandidateIndex].Append(NewCandiateIndexList);
		}
		else if (tempIndex1 < 0 && tempIndex2 < 0 && tempIndex0 >= 0)
		{// Candidate4 and Candidate5 NOT added
			DenseVector<int_max> Conflict0, Conflict1, Conflict2, Conflict3;
			Conflict0 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3] };
			Conflict1 = { FaceCandidateIndex, NewCandiateIndexList[2], NewCandiateIndexList[3] };
			Conflict2 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1] };
			Conflict3 = { FaceCandidateIndex, NewCandiateIndexList[0], NewCandiateIndexList[1] };
			m_CandidateConflictTable.Append(Conflict0);
			m_CandidateConflictTable.Append(Conflict1);
			m_CandidateConflictTable.Append(Conflict2);
			m_CandidateConflictTable.Append(Conflict3);
			m_CandidateConflictTable[FaceCandidateIndex].Append(NewCandiateIndexList);
		}
		//else if (tempIndex1 >= 0 && tempIndex2 >= 0 && tempIndex0 < 0)
		//{
		//	//do nothing
		//}
		//else if (tempIndex1 >= 0 && tempIndex2 < 0 && tempIndex0 >= 0)
		//{
		//	//do nothing
		//}
		//else if (tempIndex1 < 0 && tempIndex2 >= 0 && tempIndex0 >= 0)
		//{
		//	//do nothing
		//}
		//else //if (tempIndex1 >= 0 && tempIndex2 >= 0 && tempIndex0 >= 0)
		//{
		//	//do nothing
		//}
	}
}


template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::GenerateQuadCandidate_Type3()
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
void SurfaceRemesher2<ScalarType>::GenerateQuadCandidate_Type3(EdgeHandleType EdgeHandle_input)
{
	auto AdjacentFaceHandleList = m_InputMesh.Edge(EdgeHandle_input).GetAdjacentFaceHandleList();
	if (AdjacentFaceHandleList.GetLength() <= 1)
	{
		return;
	}

	//----------------------------	
	//         P0
	//       / a  \
    //     P1-----P2
	//       \ b  /
	//         P3
	// Triangle Normal:(P0, P1, P2), (P1, P3, P2)
	//----------------------------
	auto FaceH_a = AdjacentFaceHandleList[0];
	auto FaceH_b = AdjacentFaceHandleList[1];
	auto PointHandleList_a = m_InputMesh.Face(FaceH_a).GetPointHandleList();
	auto PointHandleList_b = m_InputMesh.Face(FaceH_b).GetPointHandleList();

	PointHandleType PointH0;
	for (int_max k = 0; k < 3; ++k)
	{
		if (PointHandleList_a[k] != PointHandleList_b[0] && PointHandleList_a[k] != PointHandleList_b[1] && PointHandleList_a[k] != PointHandleList_b[2])
		{
			PointH0 = PointHandleList_a[k];
			break;
		}
	}

	PointHandleType PointH1, PointH2;
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
			break;
		}
	}

	PointHandleType PointH3;
	for (int_max k = 0; k < 3; ++k)
	{
		if (PointHandleList_b[k] != PointHandleList_a[0] && PointHandleList_b[k] != PointHandleList_a[1] && PointHandleList_b[k] != PointHandleList_a[2])
		{
			PointH3 = PointHandleList_b[k];
			break;
		}
	}

	DenseVector<PointHandleType, 4> Candidate = { PointH0, PointH1, PointH3, PointH2 };
	m_CandidateList.Append(Candidate);	
	m_CandidateTypeList.Append(3);	
	auto CandidateIndex = m_CandidateList.GetLength() - 1;

	//update m_CandidateConflictTable
	DenseVector<int_max> Conflict;
	Conflict.Append(m_CandidateIndex_On_BigTriangle[FaceH_a.GetIndex()]);
	Conflict.Append(m_CandidateIndex_On_BigTriangle[FaceH_b.GetIndex()]);
	for (int_max k = 0; k < Conflict.GetLength(); ++k)
	{
		m_CandidateConflictTable[Conflict[k]].Append(CandidateIndex);
	}
	m_CandidateConflictTable.Append(Conflict);
	
	// the last thing
	m_CandidateIndex_On_BigTriangle[FaceH_a.GetIndex()].Append(CandidateIndex);
	m_CandidateIndex_On_BigTriangle[FaceH_b.GetIndex()].Append(CandidateIndex);
}


template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::GenerateQuadCandidate_Type4()
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
void SurfaceRemesher2<ScalarType>::GenerateQuadCandidate_Type4(EdgeHandleType EdgeHandle_input)
{
	auto AdjacentFaceHandleList = m_InputMesh.Edge(EdgeHandle_input).GetAdjacentFaceHandleList();
	if (AdjacentFaceHandleList.GetLength() <= 0)
	{
		return;
	}

	// split and merge many triangle to get a quad
	for (int_max k = 0; k < AdjacentFaceHandleList.GetLength(); ++k)
	{
		//----------------------------	
		// P3: middle point of edge e3
		// P4: middle point of edge e4
		//
		//       P4          P3   
		//   ----e4--- P0 ---e3------
		//    \   |  /    \   |   /
		//     \  | /e2  e1\  |  /
		//      \ |/        \ |/
		//       P1 ---e0----P2          
		//
		// Triangle Normal:(P0, P1, P2)
		//----------------------------

		EdgeHandleType EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4;
		PointHandleType PointH0, PointH1, PointH2, PointH3, PointH4;

		EdgeH0 = EdgeHandle_input;

		auto FaceH_middle = AdjacentFaceHandleList[k];
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
			PointH3 = m_MiddlePointOfEdgeOfInputMesh[EdgeH3.GetIndex()];
		}
		PointH4.SetToInvalid();
		if (EdgeH4.GetIndex() >= 0)
		{
			PointH4 = m_MiddlePointOfEdgeOfInputMesh[EdgeH4.GetIndex()];
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

		//check if e3 or e4 is preserved
		bool Flag_Edge3_preserved = false;
		bool Flag_Edge4_preserved = false;
		{
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

		//----------------------------	
		//       P4          P3   
		//   ----e4--- P0 ---e3------
		//    \   |  /    \   |   /
		//     \  | /e2  e1\  |  /
		//      \ |/        \ |/
		//       P1 ---e0----P2          
		//----------------------------

		if (m_InputMesh.Point(PointH0).GetAdjacentPointCount() > 0)// == 6) //add quad {P1, P2, P3, P4}
		{
			if (PointH3.GetIndex() >= 0 && PointH4.GetIndex() >= 0)
			{
				if (Flag_P0_preserved == false && Flag_Edge3_preserved == false && Flag_Edge4_preserved == false)
				{
					DenseVector<PointHandleType, 4> Candidate = { PointH1, PointH2, PointH3, PointH4 };
					m_CandidateList.Append(Candidate);
					auto CandidateIndex = m_CandidateList.GetLength() - 1;
					m_CandidateTypeList.Append(4);
					//update m_CandidateConflictTable
					DenseVector<int_max> Conflict;
					Conflict.SetCapacity(10);
					{
						auto PotentialConflict_middle = m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()];
						auto PotentialConflict_left = m_CandidateIndex_On_BigTriangle[FaceH_left.GetIndex()];
						auto PotentialConflict_right = m_CandidateIndex_On_BigTriangle[FaceH_right.GetIndex()];
						for (int_max k = 0; k < PotentialConflict_middle.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_a(CandidateIndex, PotentialConflict_middle[k], 
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_middle[k]);
							}
						}
						for (int_max k = 0; k < PotentialConflict_left.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_a(CandidateIndex, PotentialConflict_left[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_left[k]);
							}
						}
						for (int_max k = 0; k < PotentialConflict_right.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_a(CandidateIndex, PotentialConflict_right[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_right[k]);
							}
						}
					}
					for (int_max k = 0; k < Conflict.GetLength(); ++k)
					{
						m_CandidateConflictTable[Conflict[k]].Append(CandidateIndex);
					}
					m_CandidateConflictTable.Append(Conflict);
					//the last thing
					m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateIndex);
					m_CandidateIndex_On_BigTriangle[FaceH_left.GetIndex()].Append(CandidateIndex);
					m_CandidateIndex_On_BigTriangle[FaceH_right.GetIndex()].Append(CandidateIndex);					
				}
			}
		}
		else
		{
			if (PointH3.GetIndex() >= 0)//add quad {P1, P2, P3, P0} 
			{
				if (Flag_Edge3_preserved == false)
				{
					DenseVector<PointHandleType, 4> Candidate = { PointH1, PointH2, PointH3, PointH0 };
					m_CandidateList.Append(Candidate);
					auto CandidateIndex = m_CandidateList.GetLength() - 1;
					m_CandidateTypeList.Append(4);
					//update m_CandidateConflictTable
					DenseVector<int_max> Conflict;
					Conflict.SetCapacity(10);
					{
						auto PotentialConflict_middle = m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()];
						auto PotentialConflict_right = m_CandidateIndex_On_BigTriangle[FaceH_right.GetIndex()];
						for (int_max k = 0; k < PotentialConflict_middle.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_b(CandidateIndex, PotentialConflict_middle[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_middle[k]);
							}
						}
						for (int_max k = 0; k < PotentialConflict_right.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_b(CandidateIndex, PotentialConflict_right[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_right[k]);
							}
						}
					}
					for (int_max k = 0; k < Conflict.GetLength(); ++k)
					{
						m_CandidateConflictTable[Conflict[k]].Append(CandidateIndex);
					}
					m_CandidateConflictTable.Append(Conflict);
					//the last thing
					m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateIndex);
					m_CandidateIndex_On_BigTriangle[FaceH_right.GetIndex()].Append(CandidateIndex);					
				}
			}
			if (PointH4.GetIndex() >= 0)//add quad {P1, P2, P0, P4}
			{
				if (Flag_Edge4_preserved == false)
				{
					DenseVector<PointHandleType, 4> Candidate = { PointH1, PointH2, PointH0, PointH4 };
					m_CandidateList.Append(Candidate);
					auto CandidateIndex = m_CandidateList.GetLength() - 1;
					m_CandidateTypeList.Append(4);
					//update m_CandidateConflictTable
					DenseVector<int_max> Conflict;
					Conflict.SetCapacity(10);
					{
						auto PotentialConflict_middle = m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()];
						auto PotentialConflict_left = m_CandidateIndex_On_BigTriangle[FaceH_left.GetIndex()];
						for (int_max k = 0; k < PotentialConflict_middle.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_c(CandidateIndex, PotentialConflict_middle[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_middle[k]);
							}
						}
						for (int_max k = 0; k < PotentialConflict_left.GetLength(); ++k)
						{
							if (this->CheckConflict_QuadCandidate_Type4_c(CandidateIndex, PotentialConflict_left[k],
								                                          PointH0, PointH1, PointH2, PointH3, PointH4, 
																		  EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4,
																		  FaceH_left, FaceH_middle, FaceH_right) == true)
							{
								Conflict.Append(PotentialConflict_left[k]);
							}
						}
					}
					for (int_max k = 0; k < Conflict.GetLength(); ++k)
					{
						m_CandidateConflictTable[Conflict[k]].Append(CandidateIndex);
					}
					m_CandidateConflictTable.Append(Conflict);
					//the last thing
					m_CandidateIndex_On_BigTriangle[FaceH_middle.GetIndex()].Append(CandidateIndex);
					m_CandidateIndex_On_BigTriangle[FaceH_left.GetIndex()].Append(CandidateIndex);					
				}
			}
		}
	}
}


template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::EvaluateCandidate()
{
	m_CandidateScoreList.Clear();
	m_CandidateScoreList.Resize(m_CandidateList.GetLength());
	for (int_max k = 0; k < m_CandidateList.GetLength(); ++k)
	{
		if (m_CandidateList[k].GetLength() == 4)
		{
			auto Point0 = m_InputMesh.GetPointPosition(m_CandidateList[k][0]);
			auto Point1 = m_InputMesh.GetPointPosition(m_CandidateList[k][1]);
			auto Point2 = m_InputMesh.GetPointPosition(m_CandidateList[k][2]);
			auto Point3 = m_InputMesh.GetPointPosition(m_CandidateList[k][3]);
			m_CandidateScoreList[k] = this->EvaluateQuad(Point0, Point1, Point2, Point3);
		}
		else //if (m_CandidateList[k].GetLength() == 3)
		{
			auto Point0 = m_InputMesh.GetPointPosition(m_CandidateList[k][0]);
			auto Point1 = m_InputMesh.GetPointPosition(m_CandidateList[k][1]);
			auto Point2 = m_InputMesh.GetPointPosition(m_CandidateList[k][2]);
			auto Score = this->EvaluateTriangle(Point0, Point1, Point2);
			if (m_CandidateTypeList[k] == 1)
			{
				Score *= ScalarType(0.5);//1 quad ~ 2 big triangle
			}
			else //if (m_CandidateTypeList[k] == 2)
			{
				Score *= ScalarType(0.25);//1 quad ~ 4 small triangle
			}
			Score *= ScalarType(0.6);//quad is preferred
			m_CandidateScoreList[k] = Score;
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::SelectCandidate()
{// call Gurobi 

}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::BuildOutputMesh()
{
	m_OutputMesh.Clear();
	m_OutputMesh.SetCapacity(m_InputMesh.GetPointCount(), m_InputMesh.GetEdgeCount(), m_InputMesh.GetFaceCount());
	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto PointHandle = m_OutputMesh.AddPoint(it.Point().GetPosition());
		auto ID = it.Point().GetID();
		if (ID >= 0)
		{
			m_OutputMesh.Point(PointHandle).SetID(ID);
		}
	}

	for (int_max k = 0; k < m_CandidateIndicatorList.GetLength(); ++k)
	{
		if (m_CandidateIndicatorList[k] == 1)
		{
			m_OutputMesh.AddFaceByPoint(m_CandidateList[k]);
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher2<ScalarType>::RefineOutputMesh()
{//smooth the output mesh and project it to the input mesh

}


template<typename ScalarType>
bool SurfaceRemesher2<ScalarType>::
CheckConflict_QuadCandidate_Type4_a(int_max QuadCandidateIndex, int_max CandidateIndex,
		                            PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3, PointHandleType PointH4,
		                            EdgeHandleType EdgeH0, EdgeHandleType EdgeH1, EdgeHandleType EdgeH2, EdgeHandleType EdgeH3, EdgeHandleType EdgeH4,
		                            FaceHandleType FaceH_left, FaceHandleType FaceH_middle, FaceHandleType FaceH_right)
{// true if Conflict exit, false if not 

	//----------------------------	
	//       P4          P3   
	//   ----e4--- P0 ---e3------
	//    \   |  /    \   |   /
	//     \  | /e2  e1\  |  /
	//      \ |/        \ |/
	//       P1 ---e0----P2          
	//             P5
	//----------------------------
	// Quad is {P1, P2, P3, P4}
	// Candidate overlap with left/middle/right big triangle

	if (m_CandidateTypeList[CandidateIndex] == 1)//one big triangle
	{
		return true;
	}

	if (m_CandidateTypeList[CandidateIndex] == 3)//one quad from two big triangle
	{
		return true;
	}

	const auto& Candidate = m_CandidateList[CandidateIndex];

	if (m_CandidateTypeList[CandidateIndex] == 2)//one small triangle
	{//check if the small triangle is inside the quad
		//get middle point of e0, e1, e2
		auto PointH_e0 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		auto PointH_e1 = m_MiddlePointOfEdgeOfInputMesh[EdgeH1.GetIndex()];
		auto PointH_e2 = m_MiddlePointOfEdgeOfInputMesh[EdgeH2.GetIndex()];
		DenseVector<PointHandleType> PointHandleList = { PointH0, PointH1, PointH2, PointH3, PointH4, PointH_e0, PointH_e1, PointH_e2 };
		auto tempIndex0 = PointHandleList.ExactMatch("first", Candidate[0]);
		auto tempIndex1 = PointHandleList.ExactMatch("first", Candidate[1]);
		auto tempIndex2 = PointHandleList.ExactMatch("first", Candidate[2]);
		if (tempIndex0 >= 0 && tempIndex1 >= 0 && tempIndex2 >= 0)
		{
			return true;
		}
		return false;
	}

	if (m_CandidateTypeList[CandidateIndex] == 4)// one quad from small triangle + big triangle
	{
		auto PointH5 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		bool Flag_P0 = (PointH0 == Candidate[0]) || (PointH0 == Candidate[1]) || (PointH0 == Candidate[2]) || (PointH0 == Candidate[3]);
		bool Flag_P1 = (PointH1 == Candidate[0]) || (PointH1 == Candidate[1]) || (PointH1 == Candidate[2]) || (PointH1 == Candidate[3]);
		bool Flag_P2 = (PointH2 == Candidate[0]) || (PointH2 == Candidate[1]) || (PointH2 == Candidate[2]) || (PointH2 == Candidate[3]);
		bool Flag_P3 = (PointH3 == Candidate[0]) || (PointH3 == Candidate[1]) || (PointH3 == Candidate[2]) || (PointH3 == Candidate[3]);
		bool Flag_P4 = (PointH4 == Candidate[0]) || (PointH4 == Candidate[1]) || (PointH4 == Candidate[2]) || (PointH4 == Candidate[3]);
		bool Flag_P5 = (PointH5 == Candidate[0]) || (PointH5 == Candidate[1]) || (PointH5 == Candidate[2]) || (PointH5 == Candidate[3]);
		if (Flag_P0 == true)//P0 is not in QuadCandidate
		{
			return true;
		}
		if (Flag_P5 == true)//P5 is not in QuadCandidate
		{
			return true;
		}
		if (Flag_P3 == true && Flag_P4 == true)
		{
			if (Flag_P1 == true || Flag_P2 == true)
			{
				return true;
			}
		}
		if (Flag_P1 == true && Flag_P2 == true)
		{
			if (Flag_P3 == true || Flag_P4 == true)
			{
				return true;
			}
		}
		if (Flag_P1 == true && Flag_P4 == true)
		{
			if (Flag_P2 == true || Flag_P3 == true)
			{
				return true;
			}
		}
		if (Flag_P2 == true && Flag_P3 == true)
		{
			if (Flag_P1 == true || Flag_P4 == true)
			{
				return true;
			}
		}
		return false;
	}

	MDK_Error("something is wrong here @ SurfaceRemesher2::CheckConflict_QuadCandidate_Type4_a(...)")
	return false;
}


template<typename ScalarType>
bool SurfaceRemesher2<ScalarType>::
CheckConflict_QuadCandidate_Type4_b(int_max QuadCandidateIndex, int_max CandidateIndex,
		                            PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3, PointHandleType PointH4,
		                            EdgeHandleType EdgeH0, EdgeHandleType EdgeH1, EdgeHandleType EdgeH2, EdgeHandleType EdgeH3, EdgeHandleType EdgeH4,
		                            FaceHandleType FaceH_left, FaceHandleType FaceH_middle, FaceHandleType FaceH_right)
{	//----------------------------	
	//       P4          P3   
	//   ----e4--- P0 ---e3------
	//    \   |  /    \   |   /
	//     \  | /e2  e1\  |  /
	//      \ |/        \ |/
	//       P1 ---e0----P2          
	//             P5
	//----------------------------
	//Quad is {P1, P2, P3, P0} 
	//Candidate overlap with left/middle/right big triangle

	if (m_CandidateTypeList[CandidateIndex] == 1)//big triangle
	{
		return true;
	}

	if (m_CandidateTypeList[CandidateIndex] == 3)//one quad from two big triangle
	{
		return true;
	}

	const auto& Candidate = m_CandidateList[CandidateIndex];

	if (m_CandidateTypeList[CandidateIndex] == 2)//one small triangle
	{//check if the small triangle is inside the quad
		//get middle point of e0, e1, e2
		auto PointH_e0 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		auto PointH_e1 = m_MiddlePointOfEdgeOfInputMesh[EdgeH1.GetIndex()];
		auto PointH_e2 = m_MiddlePointOfEdgeOfInputMesh[EdgeH2.GetIndex()];
		DenseVector<PointHandleType> PointHandleList = { PointH0, PointH1, PointH2, PointH3, PointH_e0, PointH_e1, PointH_e2 };
		auto tempIndex0 = PointHandleList.ExactMatch("first", Candidate[0]);
		auto tempIndex1 = PointHandleList.ExactMatch("first", Candidate[1]);
		auto tempIndex2 = PointHandleList.ExactMatch("first", Candidate[2]);
		if (tempIndex0 >= 0 && tempIndex1 >= 0 && tempIndex2 >= 0)
		{
			return true;
		}
		return false;
	}

	if (m_CandidateTypeList[CandidateIndex] == 4)// one quad from small triangle + big triangle
	{
		auto PointH5 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		bool Flag_P0 = (PointH0 == Candidate[0]) || (PointH0 == Candidate[1]) || (PointH0 == Candidate[2]) || (PointH0 == Candidate[3]);
		bool Flag_P1 = (PointH1 == Candidate[0]) || (PointH1 == Candidate[1]) || (PointH1 == Candidate[2]) || (PointH1 == Candidate[3]);
		bool Flag_P2 = (PointH2 == Candidate[0]) || (PointH2 == Candidate[1]) || (PointH2 == Candidate[2]) || (PointH2 == Candidate[3]);
		bool Flag_P3 = (PointH3 == Candidate[0]) || (PointH3 == Candidate[1]) || (PointH3 == Candidate[2]) || (PointH3 == Candidate[3]);
		bool Flag_P5 = (PointH5 == Candidate[0]) || (PointH5 == Candidate[1]) || (PointH5 == Candidate[2]) || (PointH5 == Candidate[3]);		
		if (Flag_P5 == true)// T-junction
		{
			return true;
		}
		if (Flag_P0 == true && Flag_P1 == true)
		{
			return true;
		}
		if (Flag_P0 == true && Flag_P2 == true)
		{
			return true;
		}
		if (Flag_P1 == true && Flag_P2 == true)
		{
			if (Flag_P0 == true || Flag_P3 == true)
			{
				return true;
			}
		}
		if (Flag_P2 == true && Flag_P3 == true)
		{
			if (Flag_P0 == true || Flag_P1 == true || Flag_P5 == true)
			{
				return true;
			}
		}
		if (Flag_P0 == true && Flag_P3 == true)
		{
			if (Flag_P1 == true || Flag_P2 == true || Flag_P5 == true)
			{
				return true;
			}
		}
		return false;
	}

	MDK_Error("something is wrong here @ SurfaceRemesher2::CheckConflict_QuadCandidate_Type4_b(...)")
	return false;
}


template<typename ScalarType>
bool SurfaceRemesher2<ScalarType>::
CheckConflict_QuadCandidate_Type4_c(int_max QuadCandidateIndex, int_max CandidateIndex,
		                            PointHandleType PointH0, PointHandleType PointH1, PointHandleType PointH2, PointHandleType PointH3, PointHandleType PointH4,
		                            EdgeHandleType EdgeH0, EdgeHandleType EdgeH1, EdgeHandleType EdgeH2, EdgeHandleType EdgeH3, EdgeHandleType EdgeH4,
		                            FaceHandleType FaceH_left, FaceHandleType FaceH_middle, FaceHandleType FaceH_right)
{	//----------------------------	
	//       P4          P3   
	//   ----e4--- P0 ---e3------
	//    \   |  /    \   |   /
	//     \  | /e2  e1\  |  /
	//      \ |/        \ |/
	//       P1 ---e0----P2          
	//             P5
	//----------------------------
	//Quad is quad {P1, P2, P0, P4}
	//Candidate overlap with left/middle/right big triangle

	if (m_CandidateTypeList[CandidateIndex] == 1)//big triangle
	{
		return true;
	}

	if (m_CandidateTypeList[CandidateIndex] == 3)//one quad from two big triangle
	{
		return true;
	}

	const auto& Candidate = m_CandidateList[CandidateIndex];

	if (m_CandidateTypeList[CandidateIndex] == 2)//one small triangle
	{//check if the small triangle is inside the quad
		//get middle point of e0, e1, e2
		auto PointH_e0 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		auto PointH_e1 = m_MiddlePointOfEdgeOfInputMesh[EdgeH1.GetIndex()];
		auto PointH_e2 = m_MiddlePointOfEdgeOfInputMesh[EdgeH2.GetIndex()];
		DenseVector<PointHandleType> PointHandleList = { PointH0, PointH1, PointH2, PointH4, PointH_e0, PointH_e1, PointH_e2 };
		auto tempIndex0 = PointHandleList.ExactMatch("first", Candidate[0]);
		auto tempIndex1 = PointHandleList.ExactMatch("first", Candidate[1]);
		auto tempIndex2 = PointHandleList.ExactMatch("first", Candidate[2]);
		if (tempIndex0 >= 0 && tempIndex1 >= 0 && tempIndex2 >= 0)
		{
			return true;
		}
		return false;
	}

	if (m_CandidateTypeList[CandidateIndex] == 4)// one quad from small triangle + big triangle
	{
		auto PointH5 = m_MiddlePointOfEdgeOfInputMesh[EdgeH0.GetIndex()];
		bool Flag_P0 = (PointH0 == Candidate[0]) || (PointH0 == Candidate[1]) || (PointH0 == Candidate[2]) || (PointH0 == Candidate[3]);
		bool Flag_P1 = (PointH1 == Candidate[0]) || (PointH1 == Candidate[1]) || (PointH1 == Candidate[2]) || (PointH1 == Candidate[3]);
		bool Flag_P2 = (PointH2 == Candidate[0]) || (PointH2 == Candidate[1]) || (PointH2 == Candidate[2]) || (PointH2 == Candidate[3]);		
		bool Flag_P4 = (PointH4 == Candidate[0]) || (PointH4 == Candidate[1]) || (PointH4 == Candidate[2]) || (PointH4 == Candidate[3]);
		bool Flag_P5 = (PointH5 == Candidate[0]) || (PointH5 == Candidate[1]) || (PointH5 == Candidate[2]) || (PointH5 == Candidate[3]);		
		if (Flag_P5 == true)// T-junction
		{
			return true;
		}
		if (Flag_P0 == true && Flag_P1 == true)
		{
			return true;
		}
		if (Flag_P0 == true && Flag_P2 == true)
		{
			return true;
		}
		if (Flag_P1 == true && Flag_P2 == true)
		{
			if (Flag_P0 == true || Flag_P4 == true)
			{
				return true;
			}
		}
		if (Flag_P1 == true && Flag_P4 == true)
		{
			if (Flag_P0 == true || Flag_P2 == true || Flag_P5 == true)
			{
				return true;
			}
		}
		if (Flag_P0 == true && Flag_P4 == true)
		{
			if (Flag_P1 == true || Flag_P2 == true || Flag_P5 == true)
			{
				return true;
			}
		}
		return false;
	}

	MDK_Error("something is wrong here @ SurfaceRemesher2::CheckConflict_QuadCandidate_Type4_c(...)")
	return false;
}

template<typename ScalarType>
ScalarType SurfaceRemesher2<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
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
		Score = 0;
	}
	Score = 1;
	return Score;
}

template<typename ScalarType>
ScalarType SurfaceRemesher2<ScalarType>::EvaluateTriangle(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2)
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