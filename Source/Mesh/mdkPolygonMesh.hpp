#pragma once

namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh()
: Mesh()
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const MDK_Symbol_PureEmpty&)
: Mesh(MDK_PureEmpty)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const PolygonMesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(PolygonMesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const Mesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(Mesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::~PolygonMesh()
{
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const PolygonMesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
PolygonMesh<ScalarType> PolygonMesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
	PolygonMesh<ScalarType> OutputMesh = this->Mesh::GetSubMeshByFace(FaceIndexList);
	return OutputMesh;
}

//----------------- mesh editing ---------------------------------------------------------------------------------------

template<typename ScalarType>
bool PolygonMesh<ScalarType>::SwapPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ PolygonMesh::SwapPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SwapPoint(...)")
		return false;
	}

	//swap Position
	auto PositionA = m_MeshData->PointList[PointIndexA].GetPosition();
	auto PositionB = m_MeshData->PointList[PointIndexB].GetPosition();
	m_MeshData->PointList[PointIndexA].SetPosition(PositionB);
	m_MeshData->PointList[PointIndexB].SetPosition(PositionA);

	return this->SwapConnectivityOfPoint(PointIndexA, PointIndexB);
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::SwapConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ PolygonMesh::SwapConnectivityOfPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SwapConnectivityOfPoint(...)")
		return false;
	}

	const auto& AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
	const auto& AdjacentFaceIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentFaceIndexList();
	//to be updated
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();

	const auto& AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
	const auto& AdjacentFaceIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentFaceIndexList();
	//to be updated
	auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();

	//---------------- update adjacent point, edge ----------------------------------------------------

	auto tempPointIndexA = m_MeshData->PointList.GetLength();

	//change PointIndexA -> tempPointIndexA
	for (int_max k = 0; k < AdjacentEdgeIndexListA.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].GetPointIndexList();
		if (PointIndexList_k[0] == PointIndexA)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(tempPointIndexA, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexList_k[0], tempPointIndexA);
		}
	}

	//change  PointIndexB -> PointIndexA
	for (int_max k = 0; k < AdjacentEdgeIndexListB.GetLength(); ++k)
	{
		if (AdjacentPointIndexListB[k] != tempPointIndexA)
		{
			auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].GetPointIndexList();
			if (PointIndexList_k[0] == PointIndexB)
			{
				m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexA, PointIndexList_k[1]);
			}
			else
			{
				m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexList_k[0], PointIndexA);
			}
		}
	}

	//change tempPointIndexA -> PointIndexB
	for (int_max k = 0; k < AdjacentEdgeIndexListA.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].GetPointIndexList();
		if (PointIndexList_k[0] == tempPointIndexA)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexB, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListA[k]].SetPointIndexList(PointIndexList_k[0], PointIndexB);
		}
	}

	//change PointIndexA -> PointIndexB for Face	
	for (int_max k = 0; k < AdjacentFaceIndexListA.GetLength(); ++k)
	{   //take reference
		auto& PointIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexListA[k]].PointIndexList();
		int_max TempIndex = PointIndexList_k.ExactMatch("first", PointIndexA);
		PointIndexList_k[TempIndex] = PointIndexB;
	}

	//change PointIndexB -> PointIndexA for Face	
	for (int_max k = 0; k < AdjacentFaceIndexListB.GetLength(); ++k)
	{   //take reference
		auto& PointIndexList_k = m_MeshData->FaceList[AdjacentFaceIndexListB[k]].PointIndexList();
		int_max TempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
		PointIndexList_k[TempIndex] = PointIndexA;
	}

	//swap AdjacentEdgeIndexList
	{
		auto tempList = AdjacentEdgeIndexListA;
		AdjacentEdgeIndexListA = AdjacentEdgeIndexListB;
		AdjacentEdgeIndexListB = tempList;
	}

	//----------
	return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::MergeConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle)
{// merge connection of B to connection of A, B become isolated
	//-------------------------
	//   \         /
	// ---A-------B---
	//   /         \
	//-------------------------
	//   \ /    
	// ---A---   (B)
	//   / \     
    //-------------------------

	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh for now @ PolygonMesh::MergeConnectivityOfPoint(...)")
		return false;
	}

	if (PointIndexA == PointIndexB)
	{
		return true;
	}

	if (this->IsValidPointIndex(PointIndexA) == false || this->IsValidPointIndex(PointIndexB) == false)
	{
		MDK_Error("Invalid PointIndex @ Mesh::MergeConnectivityOfPoint(...)")		
		return false;
	}
	//triangle check
	if (Flag_CheckTriangle == true)
	{// A and B may belong to the same triangle face
		auto AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListAB = Intersect(AdjacentPointIndexListA, AdjacentPointIndexListB);
		if (AdjacentPointIndexListAB.IsEmpty() == false)
		{
			MDK_Error("Can not merge: triangle detected @ PolygonMesh::MergeConnectivityOfPoint(...)")
			return false;
		}
	}

	//------- delete Edge AB if it exist --------------------------------------------------------------------

	auto EdgeIndex_AB = this->GetEdgeIndexByPoint(PointIndexA, PointIndexB);
	if (EdgeIndex_AB >= 0)
	{
		auto AdjacentFaceIndexList_AB = m_MeshData->EdgeList[EdgeIndex_AB].GetAdjacentFaceIndexList();
		for (int_max k = 0; k < AdjacentFaceIndexList_AB.GetLength(); ++k)
		{
			auto FaceIndex_k = AdjacentFaceIndexList_AB[k];
			auto PointCount_k = m_MeshData->FaceList[FaceIndex_k].GetPointCount();
			// remove EdgeAB and PointB in the Face
			//take reference
			auto& EdgeIndexList_k = m_MeshData->FaceList[FaceIndex_k].EdgeIndexList();
			auto tempIndex_edge = EdgeIndexList_k.ExactMatch("first", EdgeIndex_AB);
			EdgeIndexList_k.Delete(tempIndex_edge);
			//take reference
			auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
			auto tempIndex_point = PointIndexList_k.ExactMatch("first", PointIndexB);
			PointIndexList_k.Delete(tempIndex_point);

		}
		// delete edge AB
		this->UpdateRecord_DeleteEdge(EdgeIndex_AB);
	}

	//-------------------------- Merge connection of B to connection of A ------------------------------------

	//update face adjacent to B
	auto AdjacentFaceIndexListB= m_MeshData->PointList[PointIndexB].GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjacentFaceIndexListB.GetLength(); ++k)
	{
		auto FaceIndex_k = AdjacentFaceIndexListB[k];
		//take reference
		auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
		auto tempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
		if (tempIndex >= 0)
		{
			PointIndexList_k[tempIndex] = PointIndexA;
		}
	}

	//update edge adjacent to B
	const auto& AdjacentEdgeIndexListB = m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList();	
	for (int_max k = 0; k < AdjacentEdgeIndexListB.GetLength(); ++k)
	{
		auto PointIndexList_k = m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].GetPointIndexList();
		if (PointIndexList_k[0] == PointIndexB)
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexA, PointIndexList_k[1]);
		}
		else
		{
			m_MeshData->EdgeList[AdjacentEdgeIndexListB[k]].SetPointIndexList(PointIndexList_k[0], PointIndexA);
		}
	}

	//update adjacency info of A
	auto& AdjacentEdgeIndexListA = m_MeshData->PointList[PointIndexA].AdjacentEdgeIndexList();//to be updated
	AdjacentEdgeIndexListA = { &AdjacentEdgeIndexListA, &AdjacentEdgeIndexListB };
	auto tempIndexList_edge_delete = AdjacentEdgeIndexListA.ExactMatch(EdgeIndex_AB);
	AdjacentEdgeIndexListA.Delete(tempIndexList_edge_delete);

	//delete any adjacency info of B
	m_MeshData->PointList[PointIndexB].AdjacentEdgeIndexList().Clear();

	//---------------------------------------------------------------	
	return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ShrinkEdgeToPoint(int_max EdgeIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::ShrinkEdgeToPoint(...)")
		return false;
	}

	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ PolygonMesh::ShrinkEdgeToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::ShrinkEdgeToPoint(...)")		
		return false;
	}

	auto PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();
	if (PointIndexList[0] == PointIndex)
	{
		this->MergeConnectivityOfPoint(PointIndexList[0], PointIndexList[1]);
	}
	else if (PointIndexList[1] == PointIndex)
	{
		this->MergeConnectivityOfPoint(PointIndexList[1], PointIndexList[0]);
	}
	else
	{
		this->MergeConnectivityOfPoint(PointIndexList[0], PointIndexList[1]);
		this->SwapConnectivityOfPoint(PointIndexList[0], PointIndex);
	}
	//-------------------
	return true;
}


template<typename ScalarType>
DenseVector<int_max, 2> PolygonMesh<ScalarType>::SplitEdgeByPoint(int_max EdgeIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::SplitEdgeByPoint(...)")
		return false;
	}

	DenseVector<int_max, 2> EdgeIndexPair;
	EdgeIndexPair[0] = -1;
	EdgeIndexPair[1] = -1;

	if (this->IsValidEdgeIndex(EdgeIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	auto PointIndexList = m_MeshData->EdgeList[EdgeIndex].GetPointIndexList();
	auto PointIndex0 = PointIndexList[0];
	auto PointIndex1 = PointIndexList[1];
	auto PointIndex2 = PointIndex;
	//-------------------------
	//   \          /
    // ---P0---P2--P1---
	//   /          \
    //-------------------------

	if (PointIndex2 == PointIndex0 || PointIndex2 == PointIndex1)
	{
		MDK_Error("Input Point is on Edge @ PolygonMesh::SplitEdgeByPoint(...)")
		return EdgeIndexPair;
	}

	//------------------------- input check is done --------------------------------------------//

	auto EdgeIndex01 = EdgeIndex;
	auto EdgeIndex02 = this->AddEdge(PointIndex0, PointIndex2);
	auto EdgeIndex21 = this->AddEdge(PointIndex2, PointIndex1);

	auto AdjacentFaceIndexList = m_MeshData->EdgeList[EdgeIndex].GetAdjacentFaceIndexList();
	for (int_max k = 0; k < AdjacentFaceIndexList.GetLength(); ++k)
	{// delete Edge01, add Edge02 Edge21 and Point2 in Face
		auto FaceIndex_k = AdjacentFaceIndexList[k];
		//   ----------            -------------
		//   \  Face  /       to   \    Face    /
		//    Pa-->--Pb             Pa--->P2-->Pb
		//   ------------------------------------------
		//   [Pa, Pb] may be [P0 P1] or [P1 P0];
		//-----------------------------------------------
		int_max PointIndexA, PointIndexB;	
		int_max EdgeIndexA2, EdgeIndex2B;
		auto Flag_order= m_MeshData->FaceList[FaceIndex_k].CheckPointOrder(PointIndex0, PointIndex1);
		if (Flag_order == true)
		{
			PointIndexA = PointIndex0;
			PointIndexB = PointIndex1;
			EdgeIndexA2 = EdgeIndex02;
			EdgeIndex2B = EdgeIndex21;
		}
		else
		{
			PointIndexB = PointIndex0;
			PointIndexA = PointIndex1;
			EdgeIndexA2 = EdgeIndex21;
			EdgeIndex2B = EdgeIndex02;
		}
		//update edge
		{
			//take reference
			auto& EdgeIndexList_k = m_MeshData->FaceList[FaceIndex_k].EdgeIndexList();
			//delete Edge01
			auto tempIndex = EdgeIndexList_k.ExactMatch("first", EdgeIndex01);
			EdgeIndexList_k.Delete(tempIndex);
			//add Edge
			EdgeIndexList_k.Insert(tempIndex, EdgeIndex2B);
			EdgeIndexList_k.Insert(tempIndex, EdgeIndexA2);
		}
		//update point
		{
			// take reference
			auto& PointIndexList_k = m_MeshData->FaceList[FaceIndex_k].PointIndexList();
			//find PointB
			auto tempIndex = PointIndexList_k.ExactMatch("first", PointIndexB);
			//add Point2
			PointIndexList_k.Insert(tempIndex, PointIndex2);
		}
	}

	// delete edge P0P1
	this->UpdateRecord_DeleteEdge(EdgeIndex01);

	//-------------------------------------------------------------------------
	EdgeIndexPair[0] = EdgeIndex02;
	EdgeIndexPair[1] = EdgeIndex21;
	return EdgeIndexPair;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ShrinkFaceToPoint(int_max FaceIndex, int_max PointIndex)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::ShrinkFaceToPoint(...)")
		return false;
	}

	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ PolygonMesh::ShrinkFaceToPoint(...)")		
		return false;
	}

	if (this->IsValidPointIndex(PointIndex) == false)
	{
		MDK_Error("Invalid PointIndex @ PolygonMesh::ShrinkFaceToPoint(...)")
		return false;
	}

	auto PointIndexList = m_MeshData->FaceList[FaceIndex].GetPointIndexList();
	//triangle check
	for(int_max k=0; k<PointIndexList.GetLength(); ++k)
	{
		int_max PointIndexA = PointIndexList[k];
		int_max PointIndexB = 0;
		if (k < PointIndexList.GetLength() - 1)
		{
			PointIndexB = k + 1;
		}
		auto AdjacentPointIndexListA = m_MeshData->PointList[PointIndexA].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListB = m_MeshData->PointList[PointIndexB].GetAdjacentPointIndexList();
		auto AdjacentPointIndexListAB = Intersect(AdjacentPointIndexListA, AdjacentPointIndexListB);
		if (AdjacentPointIndexListAB.IsEmpty() == false)
		{
			MDK_Error("Can not shrink: triangle detected @ PolygonMesh::ShrinkFaceToPoint(...)")
			return false;
		}
	}
	auto EdgeIndexList= m_MeshData->FaceList[FaceIndex].GetEdgeIndexList();
	auto AdjacentFaceIndexList = m_MeshData->FaceList[FaceIndex].GetAdjacentFaceIndexList();
	this->DeleteFace(FaceIndex);

	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		int_max PointIndexA, PointIndexB;
		m_MeshData->EdgeList[EdgeIndexList[k]].GetPointIndexList(PointIndexA, PointIndexB);

		auto tempList = m_MeshData->EdgeList[EdgeIndexList[k]].GetAdjacentFaceIndexList();
		for (int_max n = 0; n < tempList.GetLength(); ++n)
		{
			if (tempList[n] != FaceIndex)
			{
				int_max AdjFaceIndex = tempList[n];				
				//take reference
				auto& EdgeIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].EdgeIndexList();
				auto tempIndex_edge = EdgeIndexList_adj.ExactMatch("first", EdgeIndexList[k]);
				EdgeIndexList_adj.Delete(tempIndex_edge);
				
				//take reference
				auto& PointIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].PointIndexList();
				auto tempIndexA = PointIndexList_adj.ExactMatch("first", PointIndexA);
				PointIndexList_adj.Delete(tempIndexA);
				auto tempIndexB = PointIndexList_adj.ExactMatch("first", PointIndexB);					
				PointIndexList_adj[tempIndexB]= PointIndex;				
			}
		}
	}

	for (int_max k = 0; k < EdgeIndexList.GetLength(); ++k)
	{
		this->UpdateRecord_DeleteEdge(EdgeIndexList[k]);
	}

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		auto FaceIndexList_adj = m_MeshData->PointList[PointIndexList[k]].GetAdjacentFaceIndexList();
		auto TempFaceIndexList = SetDiff(FaceIndexList_adj, AdjacentFaceIndexList);
		for (int_max n = 0; n < TempFaceIndexList.GetLength(); ++n)
		{
			if (TempFaceIndexList[n] != FaceIndex)
			{
				auto AdjFaceIndex = TempFaceIndexList[n];
				//take reference
				auto& PointIndexList_adj = m_MeshData->FaceList[AdjFaceIndex].PointIndexList();
				auto tempIndex = PointIndexList_adj.ExactMatch("first", PointIndexList[k]);
				PointIndexList_adj[tempIndex] = PointIndex;
			}
		}

		auto EdgeIndexList_adj = m_MeshData->PointList[PointIndexList[k]].GetAdjacentEdgeIndexList();
		for (int_max n = 0; n < EdgeIndexList_adj.GetLength(); ++n)
		{
			auto tempPointIndexList = m_MeshData->EdgeList[EdgeIndexList_adj[n]].GetPointIndexList();
			if (tempPointIndexList[0] == PointIndexList[k])
			{
				m_MeshData->EdgeList[EdgeIndexList_adj[n]].SetPointIndexList(PointIndexList[k], tempPointIndexList[1]);
			}
			else if (tempPointIndexList[1] == PointIndexList[k])
			{
				m_MeshData->EdgeList[EdgeIndexList_adj[n]].SetPointIndexList(tempPointIndexList[0], PointIndexList[k]);
			}
		}
	}
	//--------------------------------
	return true;
}


/*
template<typename ScalarType>
int_max PolygonMesh<ScalarType>::MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB)
{

}
*/
/*
template<typename ScalarType>
DenseVector<int_max, 2> PolygonMesh<ScalarType>::SplitFaceByEdge(int_max EdgeIndex)
{
}
*/

template<typename ScalarType>
DenseVector<int_max, 2> PolygonMesh<ScalarType>::SplitFace(int_max FaceIndex, int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::SplitFace(...)")
		return false;
	}

	auto EdgeIndex = this->AddEdge(PointIndexA, PointIndexB);
	return this->SplitFaceByEdge(FaceIndex, EdgeIndex);
}


template<typename ScalarType>
DenseVector<int_max, 2> PolygonMesh<ScalarType>::SplitFaceByEdge(int_max FaceIndex, int_max EdgeABIndex)
{
	//------------------------
	//  ---<--A------   
	//  |     |     |
	//  |     |     |
	//  '-----B-->--'
	// 
	// -> show the point order
	//EdgeAB not adjacent to the face
	//-----------------------

	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::SplitFaceByEdge(...)")
		return false;
	}

	DenseVector<int_max, 2> NewFaceIndexList;
	NewFaceIndexList[0] = -1;
	NewFaceIndexList[1] = -1 ;

	//--------------------- check input --------------------------------------------------
	if (this->IsValidFaceIndex(FaceIndex) == false)
	{
		MDK_Error("Invalid FaceIndex @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	if (this->IsValidEdgeIndex(EdgeABIndex) == false)
	{
		MDK_Error("Invalid EdgeIndex @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	auto PointCountOftheFace = m_MeshData->FaceList[FaceIndex].GetPointCount();
	if (PointCountOftheFace <= 3)
	{
		MDK_Error("Can not split face with <= 3 point @ Mesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	//------------------------------------------------------------------------------------
	int_max PointIndexA, PointIndexB;
	m_MeshData->EdgeList[EdgeABIndex].GetPointIndexList(PointIndexA, PointIndexB);

	auto PointIndexList_A = m_MeshData->FaceList[FaceIndex].GetPointIndexList_LeadBy(PointIndexA);
	if (PointIndexList_A.IsEmpty() == true)
	{
		MDK_Error("Edge and Face do not share point-A @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	for (int_max k = 1; k < PointIndexList_A.GetLength(); ++k)
	{
		if (PointIndexList_A[k] == PointIndexB)
		{
			PointIndexList_A = PointIndexList_A.GetSubSet(span(0, k));
			break;
		}
	}

	auto PointIndexList_B = m_MeshData->FaceList[FaceIndex].GetPointIndexList_LeadBy(PointIndexB);
	if (PointIndexList_B.IsEmpty() == true)
	{
		MDK_Error("Edge and Face do not share point-B @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	for (int_max k = 1; k < PointIndexList_B.GetLength(); ++k)
	{
		if (PointIndexList_B[k] == PointIndexA)
		{
			PointIndexList_B = PointIndexList_B.GetSubSet(span(0, k));
			break;
		}
	}

	if (PointIndexList_A.GetLength() < 3 || PointIndexList_B.GetLength() < 3)
	{
		MDK_Error("less than 3 point for each split face @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	
	if (PointIndexList_A.GetLength() == PointCountOftheFace || PointIndexList_B.GetLength() == PointCountOftheFace)
	{
		MDK_Error("Can not split: PointA and PointB are adjacent @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}

	this->DeleteFace(FaceIndex);
	
	NewFaceIndexList[0] =this->AddFaceByPoint(PointIndexList_A);
	NewFaceIndexList[1] =this->AddFaceByPoint(PointIndexList_B);
	return NewFaceIndexList;
}

}// namespace mdk
