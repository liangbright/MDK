#pragma once

namespace mdk
{

template<typename ScalarType>
IsotropicTriangleSurfaceRemesher<ScalarType>::IsotropicTriangleSurfaceRemesher()
{
	this->Reset();
}

template<typename ScalarType>
IsotropicTriangleSurfaceRemesher<ScalarType>::~IsotropicTriangleSurfaceRemesher()
{
}

template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Reset()
{
	Input.SourceMesh = nullptr;
	Input.FeaturePointIndexList.Clear();
	Input.FeatureEdgeIndexList.Clear();
	Input.TargetEdgeLength = -1;
	Input.MaxIter = 10;
	Input.Flag_ProcessBounary = true;
	Input.Flag_ProjectToSourceMesh = true;

	Internal.SourceMesh_vtk = nullptr;
	Internal.PointFlagList.Clear();
	Internal.EdgeFlagList.Clear();
	Internal.MaxEdgeLength = 0;
	Internal.MinEdgeLength = 0;

	Output.TargetMesh.Clear();
}

template<typename ScalarType>
bool IsotropicTriangleSurfaceRemesher<ScalarType>::CheckInput()
{
	if (Input.SourceMesh == nullptr)
	{
		MDK_Error("InputMesh is nullptr @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->IsEmpty() == true)
	{
		MDK_Error("InputMesh is empty @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->GetPointCount() < 3)
	{
		MDK_Error("InputMesh PointCount < 3 @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.SourceMesh->Check_If_DataStructure_is_Clean() == false)
	{
		//MDK_Warning("InputMesh DataStructure is NOT Clean @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		//return nothing;
	}

	if (Input.SourceMesh->CheckIfTriangleMesh() == false)
	{
		MDK_Error("InputMesh is NOT TriangleMesh @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.TargetEdgeLength <= 0)
	{
		MDK_Error("TargetEdgeLength is NOT specified @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	//check FeaturePointIndexList
	for (int_max k = 0; k < Input.FeaturePointIndexList.GetLength(); ++k)
	{
		if (Input.SourceMesh->IsValidPointIndex(Input.FeaturePointIndexList[k]) == false)
		{
			MDK_Error("Input FeaturePointIndexList has invalid PointIndex @ IsotropicTriangleSurfaceRemesher::CheckInput()")
			return false;
		}
	}

	for (int_max k = 0; k < Input.FeatureEdgeIndexList.GetLength(); ++k)
	{
		if (Input.SourceMesh->IsValidEdgeIndex(Input.FeatureEdgeIndexList[k]) == false)
		{
			MDK_Error("Input FeatureEdgeIndexList has invalid EdgeIndex @ IsotropicTriangleSurfaceRemesher::CheckInput()")
			return false;
		}
	}

	//check mesh
	for (int_max k = 0; k <= Input.SourceMesh->GetMaxValueOfEdgeIndex(); ++k)
	{
		if (Input.SourceMesh->IsValidEdgeIndex(k) == true)
		{
			auto AdjFaceIndexList = Input.SourceMesh->Edge(k).GetAdjacentFaceIndexList();
			auto AdjFaceCount = AdjFaceIndexList.GetLength();
			if (AdjFaceCount != 1 && AdjFaceCount != 2)
			{
				MDK_Error("AdjFaceCount = " << AdjFaceCount << " !=1 and != 2 @ IsotropicTriangleSurfaceRemesher::CheckInput()")
				return false;
			}
		}
	}

	return true;
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->Initialize();
	this->Remesh();	
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::CleanMesh()
{
	this->Initialize();
	this->RemoveIsolatedFace();
	this->RemoveIsolatedEdge();
	this->RemoveIsolatedPoint();
	if (Input.Flag_ProcessBounary == true)
	{
		this->ProcessBoundary();
	}
	this->RemoveDistortedFace();
	this->CollapseShortEdge();
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Initialize()
{
	if (Input.SourceMesh->Check_If_DataStructure_is_Clean() == false)
	{
		auto tempSurface = *Input.SourceMesh;
		tempSurface.CleanDataStructure();
		Internal.SourceMesh_vtk = ConvertMDKPolygonMeshToVTKPolyData(tempSurface);
	}
	else
	{
		Internal.SourceMesh_vtk = ConvertMDKPolygonMeshToVTKPolyData(*Input.SourceMesh);
	}
	Internal.CellLocator_vtk = vtkSmartPointer<vtkCellLocator>::New();
	Internal.CellLocator_vtk->SetDataSet(Internal.SourceMesh_vtk);
	Internal.CellLocator_vtk->BuildLocator();
	//--------------------------------------------------------------------
	Internal.MaxEdgeLength = ScalarType(1.4)*Input.TargetEdgeLength;
	Internal.MinEdgeLength = ScalarType(0.7)*Input.TargetEdgeLength;
	//--------------------------------------------------------------------
	auto PointIndex_MAX = Input.SourceMesh->GetMaxValueOfPointIndex();
	Internal.PointFlagList.Clear();
	Internal.PointFlagList.Resize(PointIndex_MAX+1);
	Internal.PointFlagList.Fill(0);
	for (int_max k = 0; k < Input.FeaturePointIndexList.GetLength(); ++k)
	{
		auto PointIndex_k = Input.FeaturePointIndexList[k];
		if (Input.SourceMesh->IsValidPointIndex(PointIndex_k) == true)
		{
			Internal.PointFlagList[PointIndex_k] = 1;
		}
	}
	//--------------------------------------------------------------------
	auto EdgeIndex_MAX = Input.SourceMesh->GetMaxValueOfEdgeIndex();
	Internal.EdgeFlagList.Clear();
	Internal.EdgeFlagList.Resize(EdgeIndex_MAX+1);
	Internal.EdgeFlagList.Fill(0);
	for (int_max k = 0; k < Input.FeatureEdgeIndexList.GetLength(); ++k)
	{
		auto EdgeIndex_k = Input.FeatureEdgeIndexList[k];
		if (Input.SourceMesh->IsValidEdgeIndex(EdgeIndex_k) == true)
		{
			Internal.EdgeFlagList[EdgeIndex_k] = 1;
			//Point of Feature Edge is also Feature Point
			auto PointIndexList_k = Input.SourceMesh->Edge(EdgeIndex_k).GetPointIndexList();
			Internal.PointFlagList[PointIndexList_k[0]] = 1;
			Internal.PointFlagList[PointIndexList_k[1]] = 1;
		}
	}
	//--------------------------------------------------------------------
	Output.TargetMesh = *Input.SourceMesh;
}


template<typename ScalarType>
bool IsotropicTriangleSurfaceRemesher<ScalarType>::IsFeatureEdge(int_max EdgeIndex)
{
	bool Flag_Feature_Edge = false;
	if (EdgeIndex >= 0 && EdgeIndex < Internal.EdgeFlagList.GetLength())
	{
		if (Internal.EdgeFlagList[EdgeIndex] > 0)
		{
			Flag_Feature_Edge = true;
		}
	}
	return Flag_Feature_Edge;
}


template<typename ScalarType>
bool IsotropicTriangleSurfaceRemesher<ScalarType>::IsFeaturePoint(int_max PointIndex)
{
	bool Flag_Feature_Point = false;
	if (PointIndex >= 0 && PointIndex < Internal.PointFlagList.GetLength())
	{
		if (Internal.PointFlagList[PointIndex] > 0)
		{
			Flag_Feature_Point = true;
		}
	}
	return Flag_Feature_Point;
}

template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::ProcessBoundary()
{	
	this->SplitLongBoundaryEdge();
	this->CollapseShortBoundaryEdge();
	this->TangentialRelaxation_BoundaryEdge();
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::SplitLongBoundaryEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	int_max EdgeIndex = -1;
	int_max MaxIter = Surface.GetMaxValueOfEdgeIndex()+1;
	//while (true): it may not converge
	for (int_max iter = 0; iter < MaxIter; ++iter)
	{
		EdgeIndex = EdgeIndex + 1;
		if (EdgeIndex > Surface.GetMaxValueOfEdgeIndex())
		{
			break;
		}
		if (Surface.IsValidEdgeIndex(EdgeIndex) == true)
		{
			if (Surface.Edge(EdgeIndex).IsOnPolygonMeshBoundary() == true)
			{
				bool Flag_Feature_Edge = this->IsFeatureEdge(EdgeIndex);
				if (Flag_Feature_Edge == false)
				{
					auto PointIndexList = Surface.Edge(EdgeIndex).GetPointIndexList();
					auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
					auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
					auto EdgeLength = (Pos0 - Pos1).L2Norm();
					if (EdgeLength > MaxEdgeLength)
					{
						Surface.SplitFaceAtEdge(EdgeIndex);
					}
				}
			}
		}
	}
}


template<typename ScalarType>
bool IsotropicTriangleSurfaceRemesher<ScalarType>::TryToCollapseSingleEdge(int_max EdgeIndex, bool Flag_ProcessBoundary)
{
	auto& Surface = Output.TargetMesh;
	if (Surface.IsValidEdgeIndex(EdgeIndex) == false)
	{
		return false;
	}
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	auto MinEdgeLength = Internal.MinEdgeLength;
	auto PointIndexList = Surface.Edge(EdgeIndex).GetPointIndexList();
	auto PointIndex0 = PointIndexList[0];
	auto PointIndex1 = PointIndexList[1];
	auto Pos0 = Surface.GetPointPosition(PointIndex0);
	auto Pos1 = Surface.GetPointPosition(PointIndex1);
	auto EdgeLength = (Pos0 - Pos1).L2Norm();
	if (EdgeLength > MinEdgeLength)
	{
		return false;
	}
	bool Flag_Feature_Edge = this->IsFeatureEdge(EdgeIndex);
	if (Flag_ProcessBoundary == false && Flag_Feature_Edge == false)
	{
		Flag_Feature_Edge = Surface.Edge(EdgeIndex).IsOnPolygonMeshBoundary();
	}
	if (Flag_Feature_Edge == true)
	{
		return false;
	}
	bool Flag_Feature_Point0 = this->IsFeaturePoint(PointIndex0);
	bool Flag_Feature_Point1 = this->IsFeaturePoint(PointIndex1);
	if (Flag_ProcessBoundary == false)
	{
		if (Flag_Feature_Point0 == false)
		{
			Flag_Feature_Point0 = Surface.Point(PointIndex0).IsOnPolygonMeshBoundary();
		}
		if (Flag_Feature_Point1 == false)
		{
			Flag_Feature_Point1 = Surface.Point(PointIndex1).IsOnPolygonMeshBoundary();
		}
	}
	if (Flag_Feature_Point0 == true && Flag_Feature_Point1 == true)
	{
		return false;
	}
	//Now: EdgeLength<=MinEdgeLength, Flag_Edge_Feature is false, and Flag_Point0_Feature or Flag_Point1_Feature is true
	auto PosMean = (Pos0 + Pos1) * ScalarType(0.5);
	auto AdjPointIndexList_P0 = Surface.Point(PointIndex0).GetAdjacentPointIndexList();
	auto AdjPointIndexList_P1 = Surface.Point(PointIndex1).GetAdjacentPointIndexList();
	//remove P1 from AdjPointIndexList_P0
	auto tempSetA = AdjPointIndexList_P0.ExactMatch(PointIndex1);
	AdjPointIndexList_P0 = SetDiff(AdjPointIndexList_P0, tempSetA);
	//remove P0 from AdjPointIndexList_P1
	auto tempSetB = AdjPointIndexList_P1.ExactMatch(PointIndex0);
	AdjPointIndexList_P1 = SetDiff(AdjPointIndexList_P1, tempSetB);

	//check if it is possible to move P0 to PosMean
	bool Flag_P0_to_PosMean = true;
	for (int_max n = 0; n < AdjPointIndexList_P0.GetLength(); ++n)
	{
		if (Flag_ProcessBoundary == true)
		{
			if (Surface.Point(AdjPointIndexList_P0[n]).IsOnPolygonMeshBoundary() == false)
			{
				continue;
			}
		}
		auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_P0[n]);
		auto La = (Pos0 - Pos_n).L2Norm();
		auto Lb = (PosMean - Pos_n).L2Norm();
		if ((La >= MinEdgeLength && Lb < MinEdgeLength)|| (La <= MaxEdgeLength && Lb > MaxEdgeLength))
		{
			Flag_P0_to_PosMean = false;
			break;
		}
	}
	//check if it is possible to move P1 to PosMean
	bool Flag_P1_to_PosMean = true;
	for (int_max n = 0; n < AdjPointIndexList_P1.GetLength(); ++n)
	{
		if(Flag_ProcessBoundary == true)
		{
			if (Surface.Point(AdjPointIndexList_P1[n]).IsOnPolygonMeshBoundary() == false)
			{
				continue;
			}
		}
		auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_P1[n]);
		auto La = (Pos1 - Pos_n).L2Norm();
		auto Lb = (PosMean - Pos_n).L2Norm();
		if ((La >= MinEdgeLength && Lb < MinEdgeLength) || (La <= MaxEdgeLength && Lb > MaxEdgeLength))
		{
			Flag_P1_to_PosMean = false;
			break;
		}
	}
	//check if it is possible to move P0 to P1
	bool Flag_P0_to_P1 = true;
	for (int_max n = 0; n < AdjPointIndexList_P0.GetLength(); ++n)
	{
		if(Flag_ProcessBoundary == true)
		{
			if (Surface.Point(AdjPointIndexList_P0[n]).IsOnPolygonMeshBoundary() == false)
			{
				continue;
			}
		}
		auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_P0[n]);
		auto La = (Pos0 - Pos_n).L2Norm();
		auto Lb = (Pos1 - Pos_n).L2Norm();
		if ((La >= MinEdgeLength && Lb < MinEdgeLength) || (La <= MaxEdgeLength && Lb > MaxEdgeLength))
		{
			Flag_P0_to_P1 = false;
			break;
		}
	}
	//check if it is possible to move P1 to P0
	bool Flag_P1_to_P0 = true;
	for (int_max n = 0; n < AdjPointIndexList_P1.GetLength(); ++n)
	{
		if(Flag_ProcessBoundary == true)
		{
			if (Surface.Point(AdjPointIndexList_P1[n]).IsOnPolygonMeshBoundary() == false)
			{
				continue;
			}
		}
		auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_P1[n]);
		auto La = (Pos1 - Pos_n).L2Norm();
		auto Lb = (Pos0 - Pos_n).L2Norm();
		if ((La >= MinEdgeLength && Lb < MinEdgeLength) || (La <= MaxEdgeLength && Lb > MaxEdgeLength))
		{
			Flag_P1_to_P0 = false;
			break;
		}
	}
	//do collapse
	bool Flag_collapse = false;
	if (Flag_Feature_Point0 == false && Flag_Feature_Point1 == false)
	{
		if (Flag_P0_to_PosMean == true && Flag_P1_to_PosMean == true)
		{
			Flag_collapse = Surface.CollapseEdge(EdgeIndex, PointIndex0, false, false);
			if (Flag_collapse == true)
			{
				Surface.SetPointPosition(PointIndex0, PosMean);
			}
		}
		else
		{
			if (Flag_P0_to_P1 == true)
			{
				Flag_collapse = Surface.CollapseEdge(EdgeIndex, PointIndex1, false, false);
			}
			else if (Flag_P1_to_P0 == true)
			{
				Flag_collapse = Surface.CollapseEdge(EdgeIndex, PointIndex0, false, false);
			}
		}
	}
	else if (Flag_Feature_Point0 == false && Flag_Feature_Point1 == true)
	{
		if (Flag_P0_to_P1 == true)
		{
			Flag_collapse = Surface.CollapseEdge(EdgeIndex, PointIndex1, false, false);
		}
	}
	else if (Flag_Feature_Point1 == false && Flag_Feature_Point0 == true)
	{
		if (Flag_P1_to_P0 == true)
		{
			Flag_collapse = Surface.CollapseEdge(EdgeIndex, PointIndex0, false, false);
		}
	}
	return Flag_collapse;
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::CollapseShortBoundaryEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	auto MinEdgeLength = Internal.MinEdgeLength;
	//sort edge by length
	DenseVector<int_max> EdgeLengthList;
	EdgeLengthList.Resize(Surface.GetMaxValueOfEdgeIndex() + 1);
	for (int_max k = 0; k < EdgeLengthList.GetLength(); ++k)
	{
		if (Surface.IsValidEdgeIndex(k) == true)
		{
			auto PointIndexList = Surface.Edge(k).GetPointIndexList();
			auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
			auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
			EdgeLengthList[k] = (Pos0 - Pos1).L2Norm();
		}
		else
		{
			EdgeLengthList[k] = 0;
		}
	}
	auto EdgeIndexList_sort = EdgeLengthList.Sort("ascend");
	for (int_max k = 0; k < EdgeIndexList_sort.GetLength(); ++k)
	{
		auto EdgeIndex_k = EdgeIndexList_sort[k];
		if (Surface.IsValidEdgeIndex(EdgeIndex_k) == true)
		{
			if (Surface.Edge(EdgeIndex_k).IsOnPolygonMeshBoundary() == true)
			{
				this->TryToCollapseSingleEdge(EdgeIndex_k, true);
			}
		}
	}
	//handle new edge
	int_max EdgeIndex = EdgeIndexList_sort.GetLength() - 1;
	//int_max MaxIter = Surface.GetMaxValueOfEdgeIndex() - EdgeIndex;
	int_max MaxIter = 0;//same some time...
	//while (true): it may not converge
	for (int_max iter = 0; iter < MaxIter; ++iter)
	{
		EdgeIndex = EdgeIndex + 1;
		if (EdgeIndex > Surface.GetMaxValueOfEdgeIndex())
		{
			break;
		}
		if (Surface.IsValidEdgeIndex(EdgeIndex) == true)
		{
			if (Surface.Edge(EdgeIndex).IsOnPolygonMeshBoundary() == true)
			{
				this->TryToCollapseSingleEdge(EdgeIndex, true);
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Remesh()
{
	this->RemoveIsolatedFace();	
	this->RemoveIsolatedEdge();
	this->RemoveIsolatedPoint();
	for (int_max iter = 0; iter < Input.MaxIter; ++iter)
	{
		if (Input.Flag_ProcessBounary == true)
		{
			this->ProcessBoundary();
		}
		//this->EqualizeValence(); not good
		this->SplitLongEdge();
		this->CollapseShortEdge();
		this->RemoveIsolatedFace();
		//this->RemoveIsolatedEdge(); unlikely
		//this->RemoveIsolatedPoint(); unlikely
		this->RemoveDistortedFace();
		this->TangentialRelaxation_InternalEdge();
	}	
	this->CollapseShortEdge();
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::RemoveIsolatedPoint()
{
	auto& Surface = Output.TargetMesh;
	auto PointIndex_MAX = Surface.GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_MAX; ++k)
	{
		if (Surface.IsValidPointIndex(k) == false)
		{
			continue;
		}
		if (Surface.Point(k).GetAdjacentPointCount() == 0)
		{
			if (this->IsFeaturePoint(k) == false)
			{
				Surface.DeletePoint(k);
			}
			else
			{
				MDK_Warning("Feature Point " << k << " is isolated")
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::RemoveIsolatedEdge()
{
	auto& Surface = Output.TargetMesh;
	auto EdgeIndex_MAX = Surface.GetMaxValueOfEdgeIndex();
	for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
	{
		if (Surface.IsValidEdgeIndex(k) == false)
		{
			continue;
		}
		if (Surface.Edge(k).GetAdjacentEdgeCount() == 0)
		{
			auto PointIndexList = Surface.Edge(k).GetPointIndexList();
			auto PointIndex0 = PointIndexList[0];
			auto PointIndex1 = PointIndexList[1];
			if (this->IsFeatureEdge(k) == false
				&& this->IsFeaturePoint(PointIndex0) == false
				&& this->IsFeaturePoint(PointIndex1) == false)
			{
				Surface.DeleteEdge(k);
				Surface.DeletePoint(PointIndex0);
				Surface.DeletePoint(PointIndex1);				
			}
			else
			{
				MDK_Warning("Feature Edge " << k << " is isolated")
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::RemoveIsolatedFace()
{
	auto& Surface = Output.TargetMesh;
	auto FaceIndex_MAX = Surface.GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_MAX; ++k)
	{
		if (Surface.IsValidFaceIndex(k) == false)
		{
			continue;
		}
		auto PointIndexList = Surface.Face(k).GetPointIndexList();
		auto P0 = PointIndexList[0];
		auto P1 = PointIndexList[1];
		auto P2 = PointIndexList[2];
		auto EdgeIndexList = Surface.Face(k).GetEdgeIndexList();
		auto E0 = EdgeIndexList[0];
		auto E1 = EdgeIndexList[1];
		auto E2 = EdgeIndexList[2];
		bool Face_can_be_deleted = true;
		if (this->IsFeatureEdge(E0) == true
			|| this->IsFeatureEdge(E1) == true
			|| this->IsFeatureEdge(E2) == true
			|| this->IsFeaturePoint(P0) == true
			|| this->IsFeaturePoint(P1) == true
			|| this->IsFeaturePoint(P2) == true)
		{
			Face_can_be_deleted = false;
		}
		//case-1 (total isolation), case-2 (connect to only one point)
		if (Surface.Edge(E0).IsOnPolygonMeshBoundary() == true
			&& Surface.Edge(E1).IsOnPolygonMeshBoundary() == true
			&& Surface.Edge(E2).IsOnPolygonMeshBoundary() == true)
		{			
			if (Face_can_be_deleted = true)
			{
				Surface.DeleteFace(k);
				Surface.DeleteEdge(E0);
				Surface.DeleteEdge(E1);
				Surface.DeleteEdge(E2);
				if (Surface.Point(P0).GetAdjacentPointCount() == 0)
				{
					Surface.DeletePoint(P0);
				}
				if (Surface.Point(P1).GetAdjacentPointCount() == 0)
				{
					Surface.DeletePoint(P1);
				}
				if (Surface.Point(P2).GetAdjacentPointCount() == 0)
				{
					Surface.DeletePoint(P2);
				}
			}
			else
			{
				MDK_Warning("Face (with Feature Edge/Point) " << k << " is isolated")
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::RemoveDistortedFace()
{
	auto& Surface = Output.TargetMesh;
	auto  FaceIndex_Max = Surface.GetMaxValueOfFaceIndex();
	for (int_max k = 0; k <= FaceIndex_Max; ++k)
	{
		if (Surface.IsValidFaceIndex(k) == false)
		{
			continue;
		}
		auto EdgeIndexList = Surface.Face(k).GetEdgeIndexList();
		auto E0 = EdgeIndexList[0];
		auto E1 = EdgeIndexList[1];
		auto E2 = EdgeIndexList[2];
		Surface.UpdateCornerAngleOfFace(k);
		auto CornerAngle = Surface.Face(k).Attribute().CornerAngle;
		ScalarType Threshold = (120.0 / 180.0) * 3.14;
		if (CornerAngle[0] > Threshold)
		{
			if (this->IsFeatureEdge(E1) == false && Surface.Edge(E1).IsOnPolygonMeshBoundary() == false)
			{
				//std::cout << "CornerAngle[0]=" << CornerAngle[0] << ", k=" << k << '\n';
				Surface.FlipEdge(E1);
			}
		}
		else if (CornerAngle[1] > Threshold)
		{
			if (this->IsFeatureEdge(E2) == false && Surface.Edge(E2).IsOnPolygonMeshBoundary() == false)
			{
				//std::cout << "CornerAngle[1]=" << CornerAngle[1] << ", k=" << k << '\n';
				Surface.FlipEdge(E2);
			}
		}
		else if (CornerAngle[2] > Threshold)
		{
			if (this->IsFeatureEdge(E0) == false && Surface.Edge(E0).IsOnPolygonMeshBoundary() == false)
			{
				//std::cout << "CornerAngle[2]=" << CornerAngle[2] << ", k=" << k << '\n';
				Surface.FlipEdge(E0);
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::EqualizeValence()
{
	auto& Surface = Output.TargetMesh;
	//
	auto EdgeIndex_MAX = Surface.GetMaxValueOfEdgeIndex();
	for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
	{
		if (Surface.IsValidEdgeIndex(k) == true)
		{
			bool Flag_Edge_Boundary = Surface.Edge(k).IsOnPolygonMeshBoundary();
			bool Flag_Edge_Feature = Flag_Edge_Boundary;
			if (k < Internal.EdgeFlagList.GetLength())
			{
				if (Internal.EdgeFlagList[k] > 0)
				{
					Flag_Edge_Feature = true;
				}
			}

			if (Flag_Edge_Feature == false)
			{
				int_max H0 = -1;
				int_max H1 = -1;
				int_max H2 = -1;
				int_max H3 = -1;
				Surface.Edge(k).GetPointIndexList(H0, H1);
				auto AdjFaceIndexList = Surface.Edge(k).GetAdjacentFaceIndexList();
				if (AdjFaceIndexList.GetLength() != 2)
				{
					std::cout << "AdjFaceIndexList.GetLength() = " << AdjFaceIndexList.GetLength() << " != 2" << '\n';
					int_max aaa = 1;
				}				
				auto PointIndexListOfFace0 = Surface.Face(AdjFaceIndexList[0]).GetPointIndexList();
				if (PointIndexListOfFace0[0] != H0 && PointIndexListOfFace0[0] != H1)
				{
					H2 = PointIndexListOfFace0[0];
				}
				else if (PointIndexListOfFace0[1] != H0 && PointIndexListOfFace0[1] != H1)
				{
					H2 = PointIndexListOfFace0[1];
				}
				else //if (PointIndexListOfFace0[2] != H0 && PointIndexListOfFace0[2] != H1)
				{
					H2 = PointIndexListOfFace0[2];
				}
				auto PointIndexListOfFace1 = Surface.Face(AdjFaceIndexList[1]).GetPointIndexList();
				if (PointIndexListOfFace1[0] != H0 && PointIndexListOfFace1[0] != H1)
				{
					H3 = PointIndexListOfFace1[0];
				}
				else if (PointIndexListOfFace1[1] != H0 && PointIndexListOfFace1[1] != H1)
				{
					H3 = PointIndexListOfFace1[1];
				}
				else //if (PointIndexListOfFace1[2] != H0 && PointIndexListOfFace1[2] != H1)
				{
					H3 = PointIndexListOfFace1[2];
				}

				int_max TargetValence0 = 6;
				int_max TargetValence1 = 6;
				int_max TargetValence2 = 6;
				int_max TargetValence3 = 6;
				if (Surface.Point(H0).IsOnPolygonMeshBoundary() == true)
				{
					TargetValence0 = 4;
				}
				if (Surface.Point(H1).IsOnPolygonMeshBoundary() == true)
				{
					TargetValence1 = 4;
				}
				if (Surface.Point(H2).IsOnPolygonMeshBoundary() == true)
				{
					TargetValence2 = 4;
				}
				if (Surface.Point(H3).IsOnPolygonMeshBoundary() == true)
				{
					TargetValence3 = 4;
				}
				int_max Valence0 = Surface.Point(H0).GetAdjacentPointCount();
				int_max Valence1 = Surface.Point(H1).GetAdjacentPointCount();
				int_max Valence2 = Surface.Point(H2).GetAdjacentPointCount();
				int_max Valence3 = Surface.Point(H3).GetAdjacentPointCount();
				int_max ValenceDiff = std::abs(TargetValence0 - Valence0) + std::abs(TargetValence1 - Valence1)
					                + std::abs(TargetValence2 - Valence2) + std::abs(TargetValence3 - Valence3);
				auto EdgeIndex01 = Surface.FlipEdge(k, true, true);
				if (EdgeIndex01 >= 0)
				{
					if (EdgeIndex01 != k)
					{
						std::cout << "EdgeIndex01 != k" << '\n';
						int_max aaa = 1;
					}
					auto AdjFaceIndexList_after = Surface.Edge(k).GetAdjacentFaceIndexList();
					if (AdjFaceIndexList_after.GetLength() != 2)
					{
						std::cout << "AdjFaceIndexList_after.GetLength() = " << AdjFaceIndexList_after.GetLength() << " != 2" << '\n';
						int_max aaa = 1;
					}
					int_max Valence0_after = Surface.Point(H0).GetAdjacentPointCount();
					int_max Valence1_after = Surface.Point(H1).GetAdjacentPointCount();
					int_max Valence2_after = Surface.Point(H2).GetAdjacentPointCount();
					int_max Valence3_after = Surface.Point(H3).GetAdjacentPointCount();
					int_max ValenceDiff_after = std::abs(TargetValence0 - Valence0_after) + std::abs(TargetValence1 - Valence1_after) 
						                      + std::abs(TargetValence2 - Valence2_after) + std::abs(TargetValence3 - Valence3_after);
					if (ValenceDiff_after > ValenceDiff)
					{
						Surface.FlipEdge(k, true, true);
					}
				}
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::SplitLongEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	int_max EdgeIndex = -1;
	int_max MaxIter = Surface.GetMaxValueOfEdgeIndex()+1;
	//while (true), it may not converge
	for (int_max iter = 0; iter < MaxIter; ++iter)
	{
		EdgeIndex = EdgeIndex + 1;
		if (EdgeIndex > Surface.GetMaxValueOfEdgeIndex())
		{
			break;
		}
		if (Surface.IsValidEdgeIndex(EdgeIndex) == true)
		{
			bool Flag_Feature_Edge = this->IsFeatureEdge(EdgeIndex);
			if (Flag_Feature_Edge == false)
			{
				Flag_Feature_Edge = Surface.Edge(EdgeIndex).IsOnPolygonMeshBoundary();
			}
			if (Flag_Feature_Edge == false)
			{
				auto PointIndexList = Surface.Edge(EdgeIndex).GetPointIndexList();
				auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
				auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
				auto EdgeLength = (Pos0 - Pos1).L2Norm();
				if (EdgeLength > MaxEdgeLength)
				{
					//std::cout << "EdgeLength=" << EdgeLength << ",MaxEdgeLength=" << MaxEdgeLength << '\n';
					Surface.SplitFaceAtEdge(EdgeIndex);
				}
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::CollapseShortEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	auto MinEdgeLength = Internal.MinEdgeLength;
	//sort edge by length
	DenseVector<int_max> EdgeLengthList;
	EdgeLengthList.Resize(Surface.GetMaxValueOfEdgeIndex() + 1);
	for (int_max k = 0; k < EdgeLengthList.GetLength(); ++k)
	{
		if (Surface.IsValidEdgeIndex(k) == true)
		{
			Surface.UpdateLengthOfEdge(k);
			EdgeLengthList[k] = Surface.Edge(k).Attribute().Length;
		}
		else
		{
			EdgeLengthList[k] = 0;
		}
	}
	auto EdgeIndexList_sort = EdgeLengthList.Sort("ascend");
	for (int_max k = 0; k < EdgeIndexList_sort.GetLength(); ++k)
	{
		auto EdgeIndex_k = EdgeIndexList_sort[k];
		if (Surface.IsValidEdgeIndex(EdgeIndex_k) == true)
		{
			if (Surface.Edge(EdgeIndex_k).IsOnPolygonMeshBoundary() == false)
			{
				this->TryToCollapseSingleEdge(EdgeIndex_k, false);
			}
		}
	}
	//handle new edge
	int_max EdgeIndex = EdgeIndexList_sort.GetLength() - 1;
	//int_max MaxIter = Surface.GetMaxValueOfEdgeIndex() - EdgeIndex;
	int_max MaxIter = 0;//same some time...
	//while (true), it may not converge
	for (int_max iter = 0; iter < MaxIter; ++iter)
	{
		EdgeIndex = EdgeIndex + 1;
		if (EdgeIndex > Surface.GetMaxValueOfEdgeIndex())
		{
			break;
		}
		if (Surface.IsValidEdgeIndex(EdgeIndex) == true)
		{
			if (Surface.Edge(EdgeIndex).IsOnPolygonMeshBoundary() == false)
			{
				this->TryToCollapseSingleEdge(EdgeIndex, false);
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::TangentialRelaxation_BoundaryEdge()
{
	auto& Surface = Output.TargetMesh;
	//
	auto PointIndex_MAX = Surface.GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_MAX; ++k)
	{
		if (Surface.IsValidPointIndex(k) == true)
		{
			bool Flag_Feature_Point = this->IsFeaturePoint(k);
			if (Flag_Feature_Point == false && Surface.Point(k).IsOnPolygonMeshBoundary() == true)
			{//find the two neighbor points on boundary
				DenseVector<int_max> NeighborBoundaryPoint;
				auto AdjPointIndexList_k = Surface.Point(k).GetAdjacentPointIndexList();
				for (int_max n = 0; n < AdjPointIndexList_k.GetLength(); ++n)
				{
					if (Surface.Point(AdjPointIndexList_k[n]).IsOnPolygonMeshBoundary() == true)
					{
						NeighborBoundaryPoint.Append(AdjPointIndexList_k[n]);
					}
				}
				if (NeighborBoundaryPoint.GetLength() == 2)
				{
					auto Pos = Surface.GetPointPosition(k);
					auto Pos0 = Surface.GetPointPosition(NeighborBoundaryPoint[0]);
					auto Pos1 = Surface.GetPointPosition(NeighborBoundaryPoint[1]);
					auto NewPos = (Pos + Pos0 + Pos1) / ScalarType(3);
					if (Input.Flag_ProjectToSourceMesh == true)
					{
						NewPos = this->Project_A_Point_to_SourceMesh(NewPos);
					}
					Surface.SetPointPosition(k, NewPos);
				}
			}
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::TangentialRelaxation_InternalEdge()
{
	auto& Surface = Output.TargetMesh;
	//
	auto PointIndex_MAX = Surface.GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_MAX; ++k)
	{
		if (Surface.IsValidPointIndex(k) == true)
		{
			bool Flag_Feature_Point = this->IsFeaturePoint(k);
			if (Flag_Feature_Point == false)
			{
				Flag_Feature_Point = Surface.Point(k).IsOnPolygonMeshBoundary();
			}
			if (Flag_Feature_Point == false)
			{
				auto Pos_k = Surface.GetPointPosition(k);
				auto AdjPointIndexList_k = Surface.Point(k).GetAdjacentPointIndexList();
				if (AdjPointIndexList_k.GetLength() >= 3)
				{
					DenseVector<ScalarType, 3> NewPos;
					NewPos.Fill(0);
					for (int_max n = 0; n < AdjPointIndexList_k.GetLength(); ++n)
					{
						auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_k[n]);
						NewPos += Pos_n;
					}
					NewPos /= ScalarType(AdjPointIndexList_k.GetLength());
					if (Input.Flag_ProjectToSourceMesh == true)
					{
						NewPos = this->Project_A_Point_to_SourceMesh(NewPos);
					}
					Surface.SetPointPosition(k, NewPos);
				}
			}
		}
	}
}


template<typename ScalarType>
DenseVector<ScalarType, 3> IsotropicTriangleSurfaceRemesher<ScalarType>::Project_A_Point_to_SourceMesh(const DenseVector<ScalarType, 3>& Point)
{	
	double testPoint[3] = { double(Point[0]), double(Point[1]), double(Point[2]) };
	double closestPoint[3];//the coordinates of the closest point will be returned here
	double closestPointDist2; //the squared distance to the closest point will be returned here
	vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
	int subId; //this is rarely used (in triangle strips only, I believe)
	Internal.CellLocator_vtk->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);
	//output
	DenseVector<ScalarType, 3> Point_proj;
	Point_proj[0] = ScalarType(closestPoint[0]);
	Point_proj[1] = ScalarType(closestPoint[1]);
	Point_proj[2] = ScalarType(closestPoint[2]);	
	return Point_proj;
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Refine()
{// this function can not be applied for a mesh with sharp angles in mesh boundary (e.g., mesh of a spear)
 //TODO:
 //It can be used if the boundary is segmented, and each segment is smooth, i.e., no sharp angles
 //apply refine to each segment
	//flip edge near boundary point if necessary
	//      3
	//    /   \
	//  1/_____\2
	//   \     /
	//    \   /
	//      0
	// Boundary Point0, Point1 and Point2 
	// Angle102 is big, Edge10/Edge20 is small
	//-------------------------------------------------------------------------------
	
	auto& Surface = Output.TargetMesh;
	
	auto BoundarySet = TraceMeshBoundaryCurve(Surface);	
	if (BoundarySet.IsEmpty() == true)
	{
		return;
	}

	auto EdgeIndex_MAX = Surface.GetMaxValueOfEdgeIndex();
	DenseVector<ScalarType> EdgeScoreList;	
	EdgeScoreList.Resize(EdgeIndex_MAX+1);
	EdgeScoreList.Fill(0);
	for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
	{
		if (Surface.IsValidEdgeIndex(k) == true)
		{
			bool Flag_Edge_Boundary = Surface.Edge(k).IsOnPolygonMeshBoundary();
			bool Flag_Edge_Feature = Flag_Edge_Boundary;
			if (k < Internal.EdgeFlagList.GetLength())
			{
				if (Internal.EdgeFlagList[k] > 0)
				{
					Flag_Edge_Feature = true;
				}
			}

			if (Flag_Edge_Feature == false)
			{
				auto PointIndexList = Surface.Edge(k).GetPointIndexList();				
				for (int_max n = 0; n < BoundarySet.GetLength(); ++n)
				{
					auto tempIdx0 = BoundarySet[n].ExactMatch("first", PointIndexList[0]);
					auto tempIdx1 = BoundarySet[n].ExactMatch("first", PointIndexList[1]);
					if (tempIdx0 >= 0 && tempIdx1 >= 0)
					{		
						//score is the edge count between the two point on the boundary
						auto Score_a = std::abs(tempIdx1 - tempIdx0);
						auto Score_b = BoundarySet[n].GetLength() - Score_a;
						if (Score_a <= Score_b)
						{
							EdgeScoreList[k] = Score_a;
						}
						else
						{
							EdgeScoreList[k] = Score_b;
						}
						break;
					}					
				}
			}
		}
	}

	auto EdgeIndexList_sort = EdgeScoreList.Sort("descend");
	for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
	{
		if (EdgeScoreList[EdgeIndexList_sort[k]] > 1)
		{
			Surface.FlipEdge(EdgeIndexList_sort[k], true, true);
		}
	}

}


}//namespace mdk

