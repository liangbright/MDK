#pragma once

namespace mdk
{

template<typename ScalarType>
IsotropicTriangleSurfaceRemesher<ScalarType>::IsotropicTriangleSurfaceRemesher()
{
	this->Clear();
}

template<typename ScalarType>
IsotropicTriangleSurfaceRemesher<ScalarType>::~IsotropicTriangleSurfaceRemesher()
{
}

template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Clear()
{
	Input.SourceMesh = nullptr;
	Input.FeaturePointIndexList.Clear();
	Input.FeatureEdgeIndexList.Clear();
	Input.TargetEdgeLength = -1;
	Input.MaxIter = 10;
	Input.Flag_CleanDataStructureOfOutputMesh = true;
	Input.Flag_ProcessBounary = false;

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
		MDK_Warning("InputMesh DataStructure is NOT Clean @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		//return nothing;
	}

	if (Input.SourceMesh->CheckIfTriangleMesh() == false)
	{
		MDK_Error("InputMesh is NOT TriangleMesh @ IsotropicTriangleSurfaceRemesher::CheckInput()")
		return false;
	}

	if (Input.TargetEdgeLength < 0)
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
	//--------------------------------------------------------------------
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
	Internal.MaxEdgeLength = ScalarType(4.0 / 3.0)*Input.TargetEdgeLength;
	Internal.MinEdgeLength = ScalarType(4.0 / 5.0)*Input.TargetEdgeLength;
	//--------------------------------------------------------------------
	Output.TargetMesh = *Input.SourceMesh;
	//--------------------------------------------------------------------
	if (Input.Flag_ProcessBounary == true)
	{
		auto PointIndex_MAX = Output.TargetMesh.GetMaxValueOfPointIndex();
		Internal.PointFlagList.Clear();
		Internal.PointFlagList.Resize(PointIndex_MAX);
		Internal.PointFlagList.Fill(0);
		for (int_max k = 0; k <= PointIndex_MAX; ++k)
		{
			if (Output.TargetMesh.IsValidPointIndex(k) == true)
			{
				auto tempIdx = Input.FeaturePointIndexList.ExactMatch("first", k);
				if (tempIdx >= 0)
				{
					Internal.PointFlagList[k] = 1;
				}
			}
		}
		//--------------------------------------------------------------------
		auto EdgeIndex_MAX = Output.TargetMesh.GetMaxValueOfEdgeIndex();
		Internal.EdgeFlagList.Clear();
		Internal.EdgeFlagList.Resize(EdgeIndex_MAX);
		Internal.EdgeFlagList.Fill(0);
		for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
		{
			if (Output.TargetMesh.IsValidEdgeIndex(k) == true)
			{
				auto tempIdx = Input.FeatureEdgeIndexList.ExactMatch("first", k);
				if (tempIdx >= 0)
				{//Point of Feature Edge is also Feature Point
					Internal.EdgeFlagList[k] = 1;
					auto PointIndexList_k = Output.TargetMesh.Edge(k).GetPointIndexList();
					Internal.PointFlagList[PointIndexList_k[0]] = 1;
					Internal.PointFlagList[PointIndexList_k[1]] = 1;
				}
			}
		}
		//--------------------------------------------------------------------
		this->ProcessBoundary();		
	}
	//--------------------------------------------------------------------	
	//now: set boundary point and edge as feature
	//--------------------------------------------------------------------	
	auto PointIndex_MAX = Output.TargetMesh.GetMaxValueOfPointIndex();
	Internal.PointFlagList.Clear();
	Internal.PointFlagList.Resize(PointIndex_MAX);
	Internal.PointFlagList.Fill(0);
	for (int_max k = 0; k <= PointIndex_MAX; ++k)
	{
		if (Output.TargetMesh.IsValidPointIndex(k) == true)
		{
			if (Output.TargetMesh.Point(k).IsOnPolygonMeshBoundary() == true)
			{
				Internal.PointFlagList[k] = 1;
			}
			auto tempIdx = Input.FeaturePointIndexList.ExactMatch("first", k);
			if (tempIdx >= 0)
			{
				Internal.PointFlagList[k] = 1;
			}
		}
	}
	//--------------------------------------------------------------------
	auto EdgeIndex_MAX = Output.TargetMesh.GetMaxValueOfEdgeIndex();
	Internal.EdgeFlagList.Clear();
	Internal.EdgeFlagList.Resize(EdgeIndex_MAX);
	Internal.EdgeFlagList.Fill(0);
	for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
	{
		if (Output.TargetMesh.IsValidEdgeIndex(k) == true)
		{
			if (Output.TargetMesh.Edge(k).IsOnPolygonMeshBoundary() == true)
			{
				Internal.EdgeFlagList[k] = 1;
			}
			auto tempIdx = Input.FeatureEdgeIndexList.ExactMatch("first", k);
			if (tempIdx >= 0)
			{//Point of Feature Edge is also Feature Point
				Internal.EdgeFlagList[k] = 1;
				auto PointIndexList_k = Output.TargetMesh.Edge(k).GetPointIndexList();
				Internal.PointFlagList[PointIndexList_k[0]] = 1;
				Internal.PointFlagList[PointIndexList_k[1]] = 1;
			}
		}
	}
	//--------------------------------------------------------------------
	this->Remesh();
	this->Refine();
	//--------------------------------------------------------------------
	if (Input.Flag_CleanDataStructureOfOutputMesh == true)
	{
		Output.TargetMesh.CleanDataStructure();
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::ProcessBoundary()
{
	this->SplitLongBoundaryEdge();
	this->CollapseShortBoundaryEdge();
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::SplitLongBoundaryEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	//while(true), do not use while
	for (int_max iter = 0; iter < 100; ++iter)
	{
		bool Flag_go_on = false;
		auto EdgeIndex_Max = Surface.GetMaxValueOfEdgeIndex();
		for (int_max k = 0; k <= EdgeIndex_Max; ++k)
		{
			if (Surface.IsValidEdgeIndex(k) == true)
			{
				if (Surface.Edge(k).IsOnPolygonMeshBoundary() == true)
				{
					bool Flag_Edge_Feature = false;
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
						auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
						auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
						auto EdgeLength = (Pos0 - Pos1).L2Norm();
						if (EdgeLength > MaxEdgeLength)
						{
							Surface.SplitFaceAtEdge(k);
							Flag_go_on = true;
						}
					}
				}
			}
		}
		if (Flag_go_on == false)
		{
			break;
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::CollapseShortBoundaryEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	auto MinEdgeLength = Internal.MinEdgeLength;
	//while (true)
	for (int_max iter = 0; iter < 100; ++iter)
	{
		bool Flag_go_on = false;
		auto EdgeIndex_MAX = Surface.GetMaxValueOfEdgeIndex();
		for (int_max k = 0; k <= EdgeIndex_MAX; ++k)
		{
			if (Surface.IsValidEdgeIndex(k) == true)
			{
				if (Surface.Edge(k).IsOnPolygonMeshBoundary() == true)
				{
					bool Flag_Edge_Feature = false;
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
						bool Flag_Point0_Feature = false;
						bool Flag_Point1_Feature = false;
						if (PointIndexList[0] < Internal.PointFlagList.GetLength())
						{
							if (Internal.PointFlagList[PointIndexList[0]] > 0)
							{
								Flag_Point0_Feature = true;
							}
						}
						if (PointIndexList[1] < Internal.PointFlagList.GetLength())
						{
							if (Internal.PointFlagList[PointIndexList[1]] > 0)
							{
								Flag_Point1_Feature = true;
							}
						}

						if (Flag_Point0_Feature == false || Flag_Point1_Feature == false)//can not both be feature
						{
							auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
							auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
							auto PosMean = (Pos0 + Pos1)*ScalarType(0.5);
							auto EdgeLength = (Pos0 - Pos1).L2Norm();
							if (EdgeLength < MinEdgeLength)
							{
								//try Point1 -> Point0
								//check if any long edge (>MaxEdgeLength) is created after Surface.CollapseEdge(k, PointIndexList[0]);
								bool Flag_P1_to_P0 = false;
								if (Flag_Point1_Feature == false)
								{
									Flag_P1_to_P0 = true;
									auto AdjPointIndexList_H1 = Surface.Point(PointIndexList[1]).GetAdjacentPointIndexList();
									for (int_max n = 0; n < AdjPointIndexList_H1.GetLength(); ++n)
									{
										if (Surface.Point(AdjPointIndexList_H1[n]).IsOnPolygonMeshBoundary() == true)
										{
											auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_H1[n]);
											auto Ln = (Pos0 - Pos_n).L2Norm();
											if (Ln > MaxEdgeLength)
											{
												Flag_P1_to_P0 = false;
												break;
											}
										}
									}
								}
								if (Flag_P1_to_P0 == true)
								{
									auto Flag_collapse = Surface.CollapseEdge(k, PointIndexList[0], false, false);
									if (Flag_collapse == true)
									{
										if (Flag_Point0_Feature == false)
										{
											Surface.SetPointPosition(PointIndexList[0], PosMean);
										}
										Flag_go_on = true;
									}
								}
								else
								{//try Point0 -> Point1
									bool Flag_P0_to_P1 = false;
									if (Flag_Point0_Feature == false)
									{
										Flag_P0_to_P1 = true;
										auto AdjPointIndexList_H0 = Surface.Point(PointIndexList[0]).GetAdjacentPointIndexList();
										for (int_max n = 0; n < AdjPointIndexList_H0.GetLength(); ++n)
										{
											if (Surface.Point(AdjPointIndexList_H0[n]).IsOnPolygonMeshBoundary() == true)
											{
												auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_H0[n]);
												auto Ln = (Pos1 - Pos_n).L2Norm();
												if (Ln > MaxEdgeLength)
												{
													Flag_P0_to_P1 = false;
													break;
												}
											}
										}
									}
									if (Flag_P0_to_P1 == true)
									{
										auto Flag_collapse = Surface.CollapseEdge(k, PointIndexList[1], false, false);
										if (Flag_collapse == true)
										{
											if (Flag_Point1_Feature == false)
											{
												Surface.SetPointPosition(PointIndexList[1], PosMean);
											}
											Flag_go_on = true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (Flag_go_on == false)
		{
			break;
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::Remesh()
{
	for (int_max iter = 0; iter < Input.MaxIter; ++iter)
	{
		if (iter > 0 && iter <  Input.MaxIter / 2)
		{
			this->EqualizeValence();
		}		
		this->SplitLongEdge();
		this->CollapseShortEdge();
		this->TangentialRelaxation_ProjectToSurface();
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

				int_max TargetValance0 = 6;
				int_max TargetValance1 = 6;
				int_max TargetValance2 = 6;
				int_max TargetValance3 = 6;
				if (Surface.Point(H0).IsOnPolygonMeshBoundary() == true)
				{
					TargetValance0 = 4;
				}
				if (Surface.Point(H1).IsOnPolygonMeshBoundary() == true)
				{
					TargetValance1 = 4;
				}
				if (Surface.Point(H2).IsOnPolygonMeshBoundary() == true)
				{
					TargetValance2 = 4;
				}
				if (Surface.Point(H3).IsOnPolygonMeshBoundary() == true)
				{
					TargetValance3 = 4;
				}
				int_max Valance0 = Surface.Point(H0).GetAdjacentPointCount();
				int_max Valance1 = Surface.Point(H1).GetAdjacentPointCount();
				int_max Valance2 = Surface.Point(H2).GetAdjacentPointCount();
				int_max Valance3 = Surface.Point(H3).GetAdjacentPointCount();
				int_max ValanceDiff = std::abs(TargetValance0 - Valance0) + std::abs(TargetValance1 - Valance1) + std::abs(TargetValance2 - Valance2) + std::abs(TargetValance3 - Valance3);

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

					int_max Valance0_after = Surface.Point(H0).GetAdjacentPointCount();
					int_max Valance1_after = Surface.Point(H1).GetAdjacentPointCount();
					int_max Valance2_after = Surface.Point(H2).GetAdjacentPointCount();
					int_max Valance3_after = Surface.Point(H3).GetAdjacentPointCount();
					int_max ValanceDiff_after = std::abs(TargetValance0 - Valance0_after) + std::abs(TargetValance1 - Valance1_after) + std::abs(TargetValance2 - Valance2_after) + std::abs(TargetValance3 - Valance3_after);

					if (ValanceDiff_after > ValanceDiff)
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
	//while(true), do not use while
	for (int_max iter = 0; iter<100; ++iter)
	{
		bool Flag_go_on = false;
		auto EdgeIndex_Max = Surface.GetMaxValueOfEdgeIndex();
		for (int_max k = 0; k <= EdgeIndex_Max; ++k)
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
					auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
					auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
					auto EdgeLength = (Pos0 - Pos1).L2Norm();
					if (EdgeLength > MaxEdgeLength)
					{
						Surface.SplitFaceAtEdge(k);
						Flag_go_on = true;
					}
				}				
			}
		}

		if (Flag_go_on == false)
		{
			break;
		}
	}
}



template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::CollapseShortEdge()
{
	auto& Surface = Output.TargetMesh;
	auto MaxEdgeLength = Internal.MaxEdgeLength;
	auto MinEdgeLength = Internal.MinEdgeLength;
	//while (true)
	for (int_max iter = 0; iter<100; ++iter)
	{
		bool Flag_go_on = false;
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
					auto PointIndexList = Surface.Edge(k).GetPointIndexList();
					bool Flag_Point0_Boundary = Surface.Point(PointIndexList[0]).IsOnPolygonMeshBoundary();
					bool Flag_Point1_Boundary = Surface.Point(PointIndexList[1]).IsOnPolygonMeshBoundary();
					bool Flag_Point0_Feature = Flag_Point0_Boundary;
					bool Flag_Point1_Feature = Flag_Point1_Boundary;
					if (PointIndexList[0] < Internal.PointFlagList.GetLength())
					{
						if (Internal.PointFlagList[PointIndexList[0]] > 0)
						{
							Flag_Point0_Feature = true;
						}
					}
					if (PointIndexList[1] < Internal.PointFlagList.GetLength())
					{
						if (Internal.PointFlagList[PointIndexList[1]] > 0)
						{
							Flag_Point1_Feature = true;
						}
					}

					if (Flag_Point0_Feature == false || Flag_Point1_Feature == false)//can not both be feature
					{
						auto Pos0 = Surface.GetPointPosition(PointIndexList[0]);
						auto Pos1 = Surface.GetPointPosition(PointIndexList[1]);
						auto PosMean = (Pos0 + Pos1)*ScalarType(0.5);
						auto EdgeLength = (Pos0 - Pos1).L2Norm();
						if (EdgeLength < MinEdgeLength)
						{
							//try Point1 -> Point0
							//check if any long edge (>MaxEdgeLength) is created after Surface.CollapseEdge(k, PointIndexList[0]);
							bool Flag_P1_to_P0 = false;
							if (Flag_Point1_Feature == false)
							{
								Flag_P1_to_P0 = true;
								auto AdjPointIndexList_H1 = Surface.Point(PointIndexList[1]).GetAdjacentPointIndexList();
								for (int_max n = 0; n < AdjPointIndexList_H1.GetLength(); ++n)
								{
									auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_H1[n]);
									auto Ln = (Pos0 - Pos_n).L2Norm();
									if (Ln > MaxEdgeLength)
									{
										Flag_P1_to_P0 = false;
										break;
									}
								}
							}							
							if (Flag_P1_to_P0 == true)
							{								
								auto Flag_collapse = Surface.CollapseEdge(k, PointIndexList[0], false, false);
								if (Flag_collapse == true)
								{
									if (Flag_Point0_Feature == false)
									{
										Surface.SetPointPosition(PointIndexList[0], PosMean);
									}
									Flag_go_on = true;
								}								
							}
							else
							{//try Point0 -> Point1
								bool Flag_P0_to_P1 = false;
								if (Flag_Point0_Feature == false)
								{
									Flag_P0_to_P1 = true;
									auto AdjPointIndexList_H0 = Surface.Point(PointIndexList[0]).GetAdjacentPointIndexList();
									for (int_max n = 0; n < AdjPointIndexList_H0.GetLength(); ++n)
									{
										auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_H0[n]);
										auto Ln = (Pos1 - Pos_n).L2Norm();
										if (Ln > MaxEdgeLength)
										{
											Flag_P0_to_P1 = false;
											break;
										}
									}
								}
								if (Flag_P0_to_P1 == true)
								{
									auto Flag_collapse = Surface.CollapseEdge(k, PointIndexList[1], false, false);
									if (Flag_collapse == true)
									{
										if (Flag_Point1_Feature == false)
										{
											Surface.SetPointPosition(PointIndexList[1], PosMean);
										}
										Flag_go_on = true;
									}
								}
							}
						}
					}
				}
			}
		}

		if (Flag_go_on == false)
		{
			break;
		}
	}
}


template<typename ScalarType>
void IsotropicTriangleSurfaceRemesher<ScalarType>::TangentialRelaxation_ProjectToSurface()
{
	auto& Surface = Output.TargetMesh;
	//
	auto PointIndex_MAX = Surface.GetMaxValueOfPointIndex();
	for (int_max k = 0; k <= PointIndex_MAX; ++k)
	{
		if (Surface.IsValidPointIndex(k) == true)
		{
			bool Flag_Point_Boundary = Surface.Point(k).IsOnPolygonMeshBoundary();
			bool Flag_Point_Feature = Flag_Point_Boundary;
			if (k < Internal.PointFlagList.GetLength())
			{
				if (Internal.PointFlagList[k] > 0)
				{
					Flag_Point_Feature = true;
				}
			}

			if (Flag_Point_Feature == false)
			{
				auto Pos_k = Surface.GetPointPosition(k);
				auto AdjPointIndexList_k = Surface.Point(k).GetAdjacentPointIndexList();
				if (AdjPointIndexList_k.GetLength() >= 3)
				{
					DenseVector<ScalarType, 3> PosMean;
					PosMean.Fill(0);
					for (int_max n = 0; n < AdjPointIndexList_k.GetLength(); ++n)
					{
						auto Pos_n = Surface.GetPointPosition(AdjPointIndexList_k[n]);
						PosMean += Pos_n;
					}
					PosMean /= ScalarType(AdjPointIndexList_k.GetLength());
					auto Pos_k_proj = this->Project_A_Point_to_SourceMesh(PosMean);
					Surface.SetPointPosition(k, Pos_k_proj);
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
{
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

	auto EdgeIndex_Max = Surface.GetMaxValueOfEdgeIndex();
	DenseVector<ScalarType> EdgeScoreList;	
	EdgeScoreList.Resize(EdgeIndex_Max);
	EdgeScoreList.Fill(0);
	for (int_max k = 0; k <= EdgeIndex_Max; ++k)
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
	for (int_max k = 0; k <= EdgeIndex_Max; ++k)
	{
		if (EdgeScoreList[EdgeIndexList_sort[k]] > 1)
		{
			Surface.FlipEdge(EdgeIndexList_sort[k], true, true);
		}
	}

}


}//namespace mdk

