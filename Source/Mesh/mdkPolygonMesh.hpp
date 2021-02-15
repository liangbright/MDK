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


template<typename ScalarType>
std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>>
PolygonMesh<ScalarType>::GetPointPositionMatrixAndFaceTable(bool Flag_Clean) const
{
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Output;
	this->GetPointPositionMatrixAndFaceTable(Output.first, Output.second, Flag_Clean);
	return Output;
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::GetPointPositionMatrixAndFaceTable(DenseMatrix<ScalarType>& PointPositionMatrix, ObjectArray<DenseVector<int_max>>& FaceTable, bool Flag_Clean) const
{
	if (Flag_Clean == false)
	{
		PointPositionMatrix.Copy(m_MeshData->PointPositionTable);
		for (int_max n = 0; n <= this->GetMaxValueOfFaceIndex(); ++n)
		{
			DenseVector<int_max> PointIndexList;
			if (this->IsValidFaceIndex(n) == true)
			{
				PointIndexList = this->Face(n).GetPointIndexList();				
			}
			FaceTable.Append(PointIndexList);
		}
		return;
	}

	auto PointCount = this->GetPointCount();
	auto FaceCount = this->GetFaceCount();
	int_max PointIndex_max = this->GetMaxValueOfPointIndex();
	int_max FaceIndex_max = this->GetMaxValueOfFaceIndex();

	PointPositionMatrix.FastResize(3, PointCount);
	FaceTable.Resize(FaceCount);

	// DataStructure may not be clean
	// Map PointIndex (PointIndex.GetIndex()) to OutputIndex (col index) in PointPositionMatrix
	std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex;

	int_max PointCounter = 0;
	for (int_max k = 0; k <= PointIndex_max; ++k)
	{
		if (this->IsValidPointIndex(k) == true)
		{
			auto Pos = this->GetPointPosition(k);
			PointPositionMatrix.SetCol(PointCounter, Pos);
			Map_PointIndex_to_OutputIndex[k] = PointCounter;
			PointCounter += 1;
		}
		else
		{
			Map_PointIndex_to_OutputIndex[k] = -1;
		}
	}

	int_max FaceCounter = 0;
	for (int_max n = 0; n <= FaceIndex_max; ++n)
	{
		if (this->IsValidFaceIndex(n) == true)
		{
			auto PointIndexList = this->Face(n).GetPointIndexList();
			FaceTable[FaceCounter].Resize(PointIndexList.GetLength());
			for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
			{
				auto tempPointIndex = PointIndexList[k];
				auto it_map = Map_PointIndex_to_OutputIndex.find(tempPointIndex);
				if (it_map != Map_PointIndex_to_OutputIndex.end())
				{
					FaceTable[FaceCounter][k] = it_map->second;
				}
				else
				{
					MDK_Error("tempPointIndex is invalid @ PolygonMesh::GetPointPositionMatrixAndFaceTable(...)")
					return;
				}
			}
			FaceCounter += 1;
		}
	}
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::Construct(const std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>>& InputData)
{
	this->Construct(InputData.first, InputData.second);
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable)
{
	if (InputPointPositionMatrix.IsEmpty() == true || InputFaceTable.IsEmpty() == true)
	{
		MDK_Error("InputPointPositionMatrix or InputFaceTable is empty @ PolygonMesh::Construct(...)")
		return;
	}

	if (InputPointPositionMatrix.GetRowCount() != 3 || 3 * InputFaceTable.GetElementCount() < InputPointPositionMatrix.GetColCount())
	{
		MDK_Error("InputPointPositionMatrix or InputFaceTable is invalid @ PolygonMesh::Construct(...)")
		return;
	}
	//--------------------------------------------------------------------------------------------------
	this->Clear(); // attribute will get lost

	auto PointIndexList = this->AddPoint_batch(std::move(InputPointPositionMatrix));

	for (int_max k = 0; k < PointIndexList.GetLength(); ++k)
	{
		if (this->IsValidPointIndex(PointIndexList[k]) == false)
		{
			MDK_Error("Somthing is wrong with PointIndexList @ PolygonMesh::Construct(...)")
			return;
		}
	}

	for (int_max k = 0; k < InputFaceTable.GetLength(); ++k)
	{
		auto PointIndexList_k = PointIndexList.GetSubSet(InputFaceTable[k]);
		if (PointIndexList_k.IsEmpty() == false)
		{
			this->AddFaceByPoint(PointIndexList_k);
		}
		else
		{
			MDK_Warning("PointIndexList is empty at Face " << k << ", the Face is ignored")
		}
	}
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
bool PolygonMesh<ScalarType>::MergePoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle)
{
	auto Flag = this->MergeConnectivityOfPoint(PointIndexA, PointIndexB, Flag_CheckTriangle);
	if (Flag == false)
	{
		return false;
	}
	this->DeletePoint(PointIndexB);
	return true;
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::MergeDuplicatedEdgeBetweenPoint(int_max PointIndex0, int_max PointIndex1)
{
	if (this->IsValidPointIndex(PointIndex0) == false || this->IsValidPointIndex(PointIndex1) == false)
	{
		return;
	}
	auto& Ref_AdjacentEdgeIndexList0 = this->Point(PointIndex0).AdjacentEdgeIndexList();
	auto& Ref_AdjacentEdgeIndexList1 = this->Point(PointIndex1).AdjacentEdgeIndexList();
	auto EdgeIndex01_List = Intersect(Ref_AdjacentEdgeIndexList0, Ref_AdjacentEdgeIndexList1);
	if (EdgeIndex01_List.GetLength() <= 1)
	{
		return;
	}
	//now: EdgeIndex is not unique, only keep EdgeIndexList[0]
	auto EdgeIndex01 = EdgeIndex01_List[0];
	//modify Point0 and Point1
	Ref_AdjacentEdgeIndexList0 = SetDiff(Ref_AdjacentEdgeIndexList0, EdgeIndex01_List);
	Ref_AdjacentEdgeIndexList0.Append(EdgeIndex01);
	Ref_AdjacentEdgeIndexList1 = SetDiff(Ref_AdjacentEdgeIndexList1, EdgeIndex01_List);
	Ref_AdjacentEdgeIndexList1.Append(EdgeIndex01);
	//modify face adj to edge k=1,2,... and delete edge
	for (int_max k = 1; k < EdgeIndex01_List.GetLength(); ++k)
	{
		auto AdjFaceIndexList = this->Edge(EdgeIndex01_List[k]).GetAdjacentFaceIndexList();
		for (int_max n = 0; n < AdjFaceIndexList.GetLength(); ++n)
		{
			auto& Ref_EdgeIndexList = this->Face(AdjFaceIndexList[n]).EdgeIndexList();
			auto tempIdx = Ref_EdgeIndexList.ExactMatch("first", EdgeIndex01_List[k]);
			Ref_EdgeIndexList[tempIdx] = EdgeIndex01;
		}
		this->Edge(EdgeIndex01_List[k]).AdjacentFaceIndexList().Clear();
		this->DeleteEdge(EdgeIndex01_List[k]);
	}
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
DenseVector<int_max, 2> PolygonMesh<ScalarType>::SplitFaceByPoint(int_max FaceIndex, int_max PointIndexA, int_max PointIndexB)
{
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::SplitFaceByPoint(...)")
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

	DenseVector<int_max, 2> NewFaceIndexList;
	NewFaceIndexList[0] = -1;
	NewFaceIndexList[1] = -1;

	//--------------------- check input --------------------------------------------------
	if (this->CheckIfPolygonMesh() == false)
	{
		MDK_Error("Only support PolygonMesh @ PolygonMesh::SplitFaceByEdge(...)")
		return NewFaceIndexList;
	}
	
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


template<typename ScalarType>
int_max PolygonMesh<ScalarType>::CutAlongEdge(int_max EdgeIndex01)
{
//------------------------------------------
//case 1
//-----------------
// 4   1  5                4  1  5
//   \ | /        cut =>    \/ \/
// 2---0---3             2--0a  0---3
//input:
//Edge20, Edge03 on boundary
//Edge01 NOT no boundary
//Point0 on boundary, Point1 NOT on boundary
//output:
//New Point 0a
//New Edge 0a~1
//return: EdgeIndex0a1
//Delete nothing
//------------------------------------------
//case 2
//-----------------
// 4---1---5            4---1a 1---5
//     |        cut =>      |  |
// 2---0---3            2---0a 0---3  
//input:
//Edge20, Edge03, Edge41, Edge15 on boundary
//Edge01 NOT on boundary
//Point0, Point 1 on boundary
//note: Point2, Point 4 may be the same
//note: Point3, Point 5 may be the same
//output:
//New Point 0a, 1a
//New Edge 1a~0a
//return: EdgeIndex0a1a
//Delete nothing
//------------------------------------------		
	//=============================================================================================================================
	auto TempFunction_FindSequenceOfFace = [&](int_max FaceIndex_start, const DenseVector<int_max>& FaceIndexList)
	{//FaceIndex_seed is inside FaceIndexList
		DenseVector<int_max> SequenceOfFace;
		DenseVector<int_max> FlagList;//0: initial, 1: used in SequenceOfFace
		FlagList.Resize(FaceIndexList.GetLength());
		FlagList.Fill(0);
		auto tempIdx_s = FaceIndexList.ExactMatch("first", FaceIndex_start);
		FlagList[tempIdx_s] = 1;
		SequenceOfFace.Append(FaceIndex_start);
		while (true)
		{
			bool Flag_go_on = false;
			for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
			{
				if (FlagList[k] == 0)
				{//check if SequenceOfFace[end] is adjacent to FaceIndex_k
					auto FaceIndex_end = SequenceOfFace[SequenceOfFace.GetLength() - 1];
					auto AdjFaceIndexList_end = this->Face(FaceIndex_end).GetAdjacentFaceIndexList();
					auto FaceIndex_k = FaceIndexList[k];
					auto tempIdx_k = AdjFaceIndexList_end.ExactMatch("first", FaceIndex_k);
					if (tempIdx_k >= 0)
					{
						SequenceOfFace.Append(FaceIndex_k);
						FlagList[k] = 1;
						Flag_go_on = true;
						break;
					}
				}
			}
			if (Flag_go_on == false)
			{
				break;
			}
		}
		return SequenceOfFace;
	};
	//=============================================================================================================================
	auto TempFunction_CutA1 = [&](int_max P0, int_max P1, int_max Edge01, int_max Face01A, const DenseVector<int_max>& FaceSetA)
	{//-------------------------------------
	 // 4   1  5                4  1  5
	 //   \ | /        cut =>    \/ \/
	 // 2---0---3             2--0a  0---3
	 //-------------------------------------
	 // Face01A in AdjFaceIndexList_Edge01 and FaceSetA
	 // add new point 0a, new edge0a1
	 // update adjacency info	 
	 //-------------------------------------		
		//add new point and edge
		auto P0a = this->AddPoint(this->GetPointPosition(P0));
		auto EdgeIndex0a1 = this->AddEdge(P0a, P1);
		//modify face in FaceSetA: update point info
		for (int_max k = 0; k < FaceSetA.GetLength(); ++k)
		{
			auto& Ref_PointIndexList = this->Face(FaceSetA[k]).PointIndexList();
			auto tempIdx_point = Ref_PointIndexList.ExactMatch("first", P0);
			Ref_PointIndexList[tempIdx_point] = P0a;
		}
		//modify Face01A (e.g. Face104): update edge info		
		{
			auto& Ref_EdgeIndexList = this->Face(Face01A).EdgeIndexList();
			auto tempIdx_edge = Ref_EdgeIndexList.ExactMatch("first", Edge01);
			Ref_EdgeIndexList[tempIdx_edge] = EdgeIndex0a1;
		}
		//modify edge01: delete Face01A
		{
			auto& Ref_AdjFaceIndexList = this->Edge(Edge01).AdjacentFaceIndexList();
			auto tempIdx = Ref_AdjFaceIndexList.ExactMatch("first", Face01A);
			Ref_AdjFaceIndexList.Delete(tempIdx);
		}
		//modify Edge01a: update face info
		this->Edge(EdgeIndex0a1).AdjacentFaceIndexList().Append(Face01A);
		//find edge that should connect to to P0a
		DenseVector<int_max> AdjEdgeIndexList_P0a;
		{
			auto AdjEdgeIndexList_P0 = this->Point(P0).GetAdjacentEdgeIndexList();
			for (int_max k = 0; k < AdjEdgeIndexList_P0.GetLength(); ++k)
			{
				auto AdjFaceIndexList_k = this->Edge(AdjEdgeIndexList_P0[k]).GetAdjacentFaceIndexList();
				bool Flag_n = false;
				for (int_max n = 0; n < AdjFaceIndexList_k.GetLength(); ++n)
				{
					auto FaceIndex_kn = AdjFaceIndexList_k[n];
					auto tempIdx = FaceSetA.ExactMatch("first", FaceIndex_kn);
					if (tempIdx >= 0)
					{
						Flag_n = true;
						break;
					}
				}
				if (AdjEdgeIndexList_P0[k] != Edge01)
				{
					AdjEdgeIndexList_P0a.Append(AdjEdgeIndexList_P0[k]);
				}
			}
			AdjEdgeIndexList_P0a.Append(EdgeIndex0a1);
		}
		//modify Edge in AdjEdgeIndexList_P0a: update point info
		for (int_max k = 0; k < AdjEdgeIndexList_P0a.GetLength(); ++k)
		{
			auto PointIndexList_k = this->Edge(AdjEdgeIndexList_P0a[k]).GetPointIndexList();
			if (PointIndexList_k[0] == P0)
			{
				PointIndexList_k[0] = P0a;
			}
			else if (PointIndexList_k[1] == P0)//must do this check because Edge0a1 is in AdjEdgeIndexList_P0a
			{
				PointIndexList_k[1] = P0a;
			}
			this->Edge(AdjEdgeIndexList_P0a[k]).SetPointIndexList(PointIndexList_k);
		}
		//modify Point0a: update edge info
		this->Point(P0a).AdjacentEdgeIndexList() = AdjEdgeIndexList_P0a;
		//modify Point0: update edge info
		auto AdjEdgeIndexList_P0_old = this->Point(P0).GetAdjacentEdgeIndexList();
		this->Point(P0).AdjacentEdgeIndexList() = SetDiff(AdjEdgeIndexList_P0_old, AdjEdgeIndexList_P0a);
		//output
		return EdgeIndex0a1;
	};
	//=============================================================================================================================
	auto TempFunction_CutA2 = [&](int_max P1, const DenseVector<int_max>& FaceSetA)
	{//---------------------------------------------------------
	 // 4---1---5            4---1---5             4---1a  1---5
	 //     |     cutA1 =>    \ / \ /     cutA2 =>     |   |
	 // 2---0---3          2---0a  0---3           2---0a  0---3  
	 //---------------------------------------------------------	 
	 // add new point 1a and update adjacency info
	 //---------------------------------------------------------
		//add new point
		auto P1a = this->AddPoint(this->GetPointPosition(P1));		
		//modify face in FaceSetA: update point info
		for (int_max k = 0; k < FaceSetA.GetLength(); ++k)
		{
			auto& Ref_PointIndexList = this->Face(FaceSetA[k]).PointIndexList();
			auto tempIdx_point = Ref_PointIndexList.ExactMatch("first", P1);
			Ref_PointIndexList[tempIdx_point] = P1a;
		}
		//find edge that should connect to P1a
		DenseVector<int_max> AdjEdgeIndexList_P1a;
		{
			auto AdjEdgeIndexList_P1 = this->Point(P1).GetAdjacentEdgeIndexList();
			for (int_max k = 0; k < AdjEdgeIndexList_P1.GetLength(); ++k)
			{
				auto AdjFaceIndexList_k = this->Edge(AdjEdgeIndexList_P1[k]).GetAdjacentFaceIndexList();
				bool Flag_n = false;
				for (int_max n = 0; n < AdjFaceIndexList_k.GetLength(); ++n)
				{
					auto FaceIndex_kn = AdjFaceIndexList_k[n];
					auto tempIdx = FaceSetA.ExactMatch("first", FaceIndex_kn);
					if (tempIdx >= 0)
					{
						Flag_n = true;
						break;
					}
				}
				AdjEdgeIndexList_P1a.Append(AdjEdgeIndexList_P1[k]);
			}
		}
		//modify Edge in AdjEdgeIndexList_P1a: update point info
		for (int_max k = 0; k < AdjEdgeIndexList_P1a.GetLength(); ++k)
		{
			auto PointIndexList_k = this->Edge(AdjEdgeIndexList_P1a[k]).GetPointIndexList();
			if (PointIndexList_k[0] == P1)
			{
				PointIndexList_k[0] = P1a;
			}
			else if (PointIndexList_k[1] == P1)
			{
				PointIndexList_k[1] = P1a;
			}
			this->Edge(AdjEdgeIndexList_P1a[k]).SetPointIndexList(PointIndexList_k);
		}
		//modify Point1a: update edge info
		this->Point(P1a).AdjacentEdgeIndexList() = AdjEdgeIndexList_P1a;
		//modify Point1: update edge info
		auto AdjEdgeIndexList_P1_old = this->Point(P1).GetAdjacentEdgeIndexList();
		this->Point(P1).AdjacentEdgeIndexList() = SetDiff(AdjEdgeIndexList_P1_old, AdjEdgeIndexList_P1a);		
	};
	//=============================================================================================================================
	if (this->IsValidEdgeIndex(EdgeIndex01) == false)
	{
		MDK_Error("Invalid EdgeIndex01 @ PolygonMesh::CutAlongEdge(...)")
		return -1;
	}
	if (this->Edge(EdgeIndex01).IsOnPolygonMeshBoundary() == true)
	{
		MDK_Error("EdgeIndex01 is on boundary @ PolygonMesh::CutAlongEdge(...)")
		return -1;
	}
	//-----------------------------------------------------------
	auto PointIndexList_Edge01 = this->Edge(EdgeIndex01).GetPointIndexList();
	if (this->Point(PointIndexList_Edge01[0]).IsOnPolygonMeshBoundary() == false && this->Point(PointIndexList_Edge01[1]).IsOnPolygonMeshBoundary() == false)
	{
		MDK_Error("One Point of the edge must be on boundary @ PolygonMesh::CutAlongEdge(...)")
		return -1;
	}
	//-----------------------------------------------------------
	//find Point0 and Point1
	int_max P0 = -1;
	int_max P1 = -1;
	if (this->Point(PointIndexList_Edge01[0]).IsOnPolygonMeshBoundary() == true)
	{
		P0 = PointIndexList_Edge01[0];
		P1 = PointIndexList_Edge01[1];
	}
	else
	{
		P0 = PointIndexList_Edge01[1];
		P1 = PointIndexList_Edge01[0];
	}
	bool Flag_P1_on_Boundary = this->Point(P1).IsOnPolygonMeshBoundary();
	//-----------------------------------------------------------	
	auto AdjFaceIndexList_Edge01 = this->Edge(EdgeIndex01).GetAdjacentFaceIndexList();
	if (AdjFaceIndexList_Edge01.GetLength() != 2)
	{
		MDK_Error("AdjacentFaceCount of this edge is NOT 2 @ PolygonMesh::CutAlongEdge(...)")
		return -1;
	}
	//find two disjoint faceset adjacent to P0
	DenseVector<int_max> FaceSetA, FaceSetB;
	int_max FaceIndex01A = AdjFaceIndexList_Edge01[0];//in FaceSetA
	int_max FaceIndex01B = AdjFaceIndexList_Edge01[1];//in FaceSetB
	{
		auto AdjFaceIndexList_P0 = this->Point(P0).GetAdjacentFaceIndexList();
		auto AdjFaceIndexList_P0_less = AdjFaceIndexList_P0;
		{
			auto tempIdx = AdjFaceIndexList_P0_less.ExactMatch("first", FaceIndex01B);
			AdjFaceIndexList_P0_less.Delete(tempIdx);
		}
		FaceSetA = TempFunction_FindSequenceOfFace(FaceIndex01A, AdjFaceIndexList_P0_less);		
		FaceSetB = SetDiff(AdjFaceIndexList_P0, FaceSetA);
	}
	//-----------------------------------------------------------		
	auto EdgeIndex_output = TempFunction_CutA1(P0, P1, EdgeIndex01, FaceIndex01A, FaceSetA);
	if (Flag_P1_on_Boundary == true)
	{
		TempFunction_CutA2(P1, FaceSetA);		
	}
	return EdgeIndex_output;
}


template<typename ScalarType>
DenseVector<int_max, 2> PolygonMesh<ScalarType>::CutAlongEdge(int_max EdgeIndex01, int_max EdgeIndex12)
{
//---------------------------------------
// 3  2  4             3    2  4
//  \ | /               \  / \ /
//5---1---6   cut =>  5---1a  1 ---6  
//  / | \                / \ / \
// 7  0  8              7   0   8
//
//{2, 1a, 0, 1b} is a hole
//---------------------------
//after cut:
//New Point 1a
//New Edge 1a~2, 1a~0
//return:EdgeIndexList_output ={ Edge01a, Edge1a2 }
//-------------------------------------------------	
	//=============================================================================================================================
	auto TempFunction_FindSequenceOfFace = [&](int_max FaceIndex_start, const DenseVector<int_max>& FaceIndexList)
	{//FaceIndex_seed is inside FaceIndexList
		DenseVector<int_max> SequenceOfFace;
		DenseVector<int_max> FlagList;//0: initial, 1: used in SequenceOfFace
		FlagList.Resize(FaceIndexList.GetLength());
		FlagList.Fill(0);
		auto tempIdx_s = FaceIndexList.ExactMatch("first", FaceIndex_start);
		FlagList[tempIdx_s] = 1;
		SequenceOfFace.Append(FaceIndex_start);
		while (true)
		{
			bool Flag_go_on = false;
			for (int_max k = 0; k < FaceIndexList.GetLength(); ++k)
			{
				if (FlagList[k] == 0)
				{//check if SequenceOfFace[end] is adjacent to FaceIndex_k
					auto FaceIndex_end = SequenceOfFace[SequenceOfFace.GetLength() - 1];
					auto AdjFaceIndexList_end = this->Face(FaceIndex_end).GetAdjacentFaceIndexList();
					auto FaceIndex_k = FaceIndexList[k];
					auto tempIdx_k = AdjFaceIndexList_end.ExactMatch("first", FaceIndex_k);
					if (tempIdx_k >= 0)
					{
						SequenceOfFace.Append(FaceIndex_k);
						FlagList[k] = 1;
						Flag_go_on = true;
						break;
					}
				}
			}
			if (Flag_go_on == false)
			{
				break;
			}
		}
		return SequenceOfFace;
	};
	//=============================================================================================================================
	DenseVector<int_max, 2> EdgeIndexList_output;
	EdgeIndexList_output.Fill(-1);
	if (this->IsValidEdgeIndex(EdgeIndex01) == false)
	{
		MDK_Error("Invalid EdgeIndex01 @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	if (this->IsValidEdgeIndex(EdgeIndex12) == false)
	{
		MDK_Error("Invalid EdgeIndex12 @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	if (this->Edge(EdgeIndex01).IsOnPolygonMeshBoundary() == true)
	{
		MDK_Error("EdgeIndex01 is on boundary @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	if (this->Edge(EdgeIndex12).IsOnPolygonMeshBoundary() == true)
	{
		MDK_Error("EdgeIndex12 is on boundary @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	//-----------------------------------------------------------
	auto PointIndexList_Edge01 = this->Edge(EdgeIndex01).GetPointIndexList();
	if (this->Point(PointIndexList_Edge01[0]).IsOnPolygonMeshBoundary() == true || this->Point(PointIndexList_Edge01[1]).IsOnPolygonMeshBoundary() == true)
	{
		MDK_Error("One Point of the edge is on boundary @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	//-----------------------------------------------------------
	auto PointIndexList_Edge12 = this->Edge(EdgeIndex12).GetPointIndexList();
	if (this->Point(PointIndexList_Edge12[0]).IsOnPolygonMeshBoundary() == true || this->Point(PointIndexList_Edge12[1]).IsOnPolygonMeshBoundary() == true)
	{
		MDK_Error("One Point of the edge is on boundary @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	//-----------------------------------------------------------
	// find Point0, Point1, Point2
	int_max P0, P1, P2;
	if (PointIndexList_Edge01[0] == PointIndexList_Edge12[0])
	{
		P0 = PointIndexList_Edge01[1];
		P1 = PointIndexList_Edge01[0];
		P2 = PointIndexList_Edge12[1];
	}
	else if (PointIndexList_Edge01[0] == PointIndexList_Edge12[1])
	{
		P0 = PointIndexList_Edge01[1];
		P1 = PointIndexList_Edge01[0];
		P2 = PointIndexList_Edge12[0];
	}
	else if (PointIndexList_Edge01[1] == PointIndexList_Edge12[0])
	{
		P0 = PointIndexList_Edge01[0];
		P1 = PointIndexList_Edge01[1];
		P2 = PointIndexList_Edge12[1];
	}
	else if (PointIndexList_Edge01[1] == PointIndexList_Edge12[1])
	{
		P0 = PointIndexList_Edge01[0];
		P1 = PointIndexList_Edge01[1];
		P2 = PointIndexList_Edge12[0];
	}
	else
	{
		MDK_Error("Two edge do not share a point @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	//-----------------------------------------------------------	
	auto AdjFaceIndexList_Edge01 = this->Edge(EdgeIndex01).GetAdjacentFaceIndexList();
	if (AdjFaceIndexList_Edge01.GetLength() != 2)
	{
		MDK_Error("AdjacentFaceCount of edge01 is NOT 2 @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	auto AdjFaceIndexList_Edge12 = this->Edge(EdgeIndex12).GetAdjacentFaceIndexList();
	if (AdjFaceIndexList_Edge12.GetLength() != 2)
	{
		MDK_Error("AdjacentFaceCount of edge12 is NOT 2 @ PolygonMesh::CutAlongEdge(...)")
		return EdgeIndexList_output;
	}
	//find two disjoint faceset adjacent to P1
	DenseVector<int_max> FaceSetA, FaceSetB;
	int_max FaceIndex01A = AdjFaceIndexList_Edge01[0];//in FaceSetA
	int_max FaceIndex01B = AdjFaceIndexList_Edge01[1];//in FaceSetB
	int_max FaceIndex12A = -1;//in FaceSetA
	int_max FaceIndex12B = -1;//in FaceSetB	
	{
		auto AdjFaceIndexList_P1 = this->Point(P1).GetAdjacentFaceIndexList();
		auto AdjFaceIndexList_P1_less = AdjFaceIndexList_P1;
		{
			auto tempIdx = AdjFaceIndexList_P1_less.ExactMatch("first", FaceIndex01B);
			AdjFaceIndexList_P1_less.Delete(tempIdx);
		}
		auto FaceSquence = TempFunction_FindSequenceOfFace(FaceIndex01A, AdjFaceIndexList_P1_less);
		auto tempIdx0 = FaceSquence.ExactMatch("first", AdjFaceIndexList_Edge12[0]);
		auto tempIdx1 = FaceSquence.ExactMatch("first", AdjFaceIndexList_Edge12[1]);
		if (tempIdx0 >= 0 && tempIdx1 >= 0)
		{
			if (tempIdx0 < tempIdx1)
			{
				FaceIndex12A = AdjFaceIndexList_Edge12[0];
				FaceIndex12B = AdjFaceIndexList_Edge12[1];
				FaceSetA = FaceSquence.GetSubSet(0, tempIdx0);
			}
			else
			{
				FaceIndex12A = AdjFaceIndexList_Edge12[1];
				FaceIndex12B = AdjFaceIndexList_Edge12[0];
				FaceSetA = FaceSquence.GetSubSet(0, tempIdx1);
			}
		}
		else if (tempIdx0 >= 0)
		{
			FaceIndex12A = AdjFaceIndexList_Edge12[0];
			FaceIndex12B = AdjFaceIndexList_Edge12[1];
			FaceSetA = FaceSquence.GetSubSet(0, tempIdx0);
		}
		else if (tempIdx1 >= 0)
		{
			FaceIndex12A = AdjFaceIndexList_Edge12[1];
			FaceIndex12B = AdjFaceIndexList_Edge12[0];
			FaceSetA = FaceSquence.GetSubSet(0, tempIdx1);
		}
		else
		{
			MDK_Error("Somthing is wrong in the input mesh @ PolygonMesh::CutAlongEdge(...)")
			return EdgeIndexList_output;
		}
		FaceSetB = SetDiff(AdjFaceIndexList_P1, FaceSetA);
	}
	//create a new Point1a and make sure Point1a and FaceSetA are connected
	auto P1a = this->AddPoint(this->GetPointPosition(P1));
	//create new Edge1a0 and Edge1a2
	auto EdgeIndex1a0 = this->AddEdge(P1a, P0);
	auto EdgeIndex1a2 = this->AddEdge(P1a, P2);
	//modify face in FaceSetA: update point
	for (int_max k = 0; k < FaceSetA.GetLength(); ++k)
	{
		auto& Ref_PointIndexList = this->Face(FaceSetA[k]).PointIndexList();
		auto tempIdx = Ref_PointIndexList.ExactMatch("first", P1);
		Ref_PointIndexList[tempIdx] = P1a;
	}
	//modify FaceIndex01A: update edge
	{
		auto& Ref_EdgeIndexList = this->Face(FaceIndex01A).EdgeIndexList();
		auto tempIdx = Ref_EdgeIndexList.ExactMatch("first", EdgeIndex01);
		Ref_EdgeIndexList.Delete(tempIdx);
	}
	//modify FaceIndex12A: update edge
	{
		auto& Ref_EdgeIndexList = this->Face(FaceIndex12A).EdgeIndexList();
		auto tempIdx = Ref_EdgeIndexList.ExactMatch("first", EdgeIndex12);
		Ref_EdgeIndexList.Delete(tempIdx);
	}
	//modify Edge1a0
	this->Edge(EdgeIndex1a0).AdjacentFaceIndexList().Append(FaceIndex01A);
	//modify Edge1a2
	this->Edge(EdgeIndex1a2).AdjacentFaceIndexList().Append(FaceIndex12A);
    //modify Edge01
	{
		auto& Ref_AdjFaceIndexList = this->Edge(EdgeIndex01).AdjacentFaceIndexList();
		auto tempIdx = Ref_AdjFaceIndexList.ExactMatch("first", FaceIndex01A);
		Ref_AdjFaceIndexList.Delete(tempIdx);
	}
	//modify Edge12
	{
		auto& Ref_AdjFaceIndexList = this->Edge(EdgeIndex12).AdjacentFaceIndexList();
		auto tempIdx = Ref_AdjFaceIndexList.ExactMatch("first", FaceIndex12A);
		Ref_AdjFaceIndexList.Delete(tempIdx);
	}
	//find edge that should connect to P1a
	DenseVector<int_max> AdjEdgeIndexList_P1a;
	{
		auto AdjEdgeIndexList_P1 = this->Point(P1).GetAdjacentEdgeIndexList();
		for (int_max k = 0; k < AdjEdgeIndexList_P1.GetLength(); ++k)
		{
			auto AdjFaceIndexList_k = this->Edge(AdjEdgeIndexList_P1[k]).GetAdjacentFaceIndexList();
			bool Flag_n = false;
			for (int_max n = 0; n < AdjFaceIndexList_k.GetLength(); ++n)
			{
				auto FaceIndex_kn = AdjFaceIndexList_k[n];
				auto tempIdx = FaceSetA.ExactMatch("first", FaceIndex_kn);
				if (tempIdx >= 0)
				{
					Flag_n = true;
					break;
				}
			}
			if (AdjEdgeIndexList_P1[k] != EdgeIndex01 && AdjEdgeIndexList_P1[k] != EdgeIndex12)
			{
				AdjEdgeIndexList_P1a.Append(AdjEdgeIndexList_P1[k]);
			}
		}
		AdjEdgeIndexList_P1a.Append(EdgeIndex1a0);
		AdjEdgeIndexList_P1a.Append(EdgeIndex1a2);
	}
	//modify Edge in AdjEdgeIndexList_P1a: update point info
	for (int_max k = 0; k < AdjEdgeIndexList_P1a.GetLength(); ++k)
	{
		auto PointIndexList_k = this->Edge(AdjEdgeIndexList_P1a[k]).GetPointIndexList();
		if (PointIndexList_k[0] == P1)
		{
			PointIndexList_k[0] = P1a;
		}
		else if (PointIndexList_k[1] == P1)//must do this check because Edge1a0 and Edge1a2 is in AdjEdgeIndexList_P1a
		{
			PointIndexList_k[1] = P1a;
		}
		this->Edge(AdjEdgeIndexList_P1a[k]).SetPointIndexList(PointIndexList_k);
	}
	//modify Point1a: update edge info
	this->Point(P1a).AdjacentEdgeIndexList() = AdjEdgeIndexList_P1a;
	//modify Point1: update edge info
	auto AdjEdgeIndexList_P1_old = this->Point(P1).GetAdjacentEdgeIndexList();
	this->Point(P1).AdjacentEdgeIndexList() = SetDiff(AdjEdgeIndexList_P1_old, AdjEdgeIndexList_P1a);
	//output
	EdgeIndexList_output[0] = EdgeIndex1a0;
	EdgeIndexList_output[1] = EdgeIndex1a2;
	return EdgeIndexList_output;
}

}// namespace mdk
