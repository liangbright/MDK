#pragma once

namespace mdk
{

template<typename ScalarType>
void ConvertPolygonMeshToTriangleMesh(const PolygonMesh<ScalarType>& InputMesh, TriangleMesh<ScalarType>& OutputMesh)
{
	if (InputMesh.IsEmpty() == true)
	{
		OutputMesh.Clear();
		return;
	}

	if (InputMesh.CheckIfTriangleMesh() == true)
	{
		typedef ScalarTypeA::ScalarType ScalarType;
		DenseMatrix<ScalarType> PointPositionMatrix;
		ObjectArray<DenseVector<int_max>> FaceTable;
		InputMesh.GetPointPositionMatrixAndFaceTable(PointPositionMatrix, FaceTable);
		OutputMesh.Construct(std::move(PointPositionMatrix), FaceTable);
		return;
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	ConvertVTKPolyDataToMDKTriangleMesh(VTKMesh, OutputMesh);

	//-----------------------------------------------------------
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	if (NamedPointCount > 0)
	{
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
			OutputMesh.Point(PointIndex).SetName(PointNameList[k]);			
		}
	}
	//----------------------------------------------------------
	auto PointSetCount = InputMesh.GetPointSetCount();
	if (PointSetCount > 0)
	{		
		auto PointSetNameList = InputMesh.GetPointSetName(ALL);
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			OutputMesh.SetPointSet(PointSetNameList[PointSetIndex], PointSet);
		}
	}
}

template<typename ScalarType>
DenseVector<int_max> TraceMeshBoundaryCurve(const TriangleMesh<ScalarType>& TargetMesh, int_max PointIndex_start)
{
    const PolygonMesh<ScalarType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref, PointIndex_start);
}

template<typename ScalarType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const TriangleMesh<ScalarType>& TargetMesh)
{
    const PolygonMesh<ScalarType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref);
}

template<typename ScalarType>
int_max FindNearestPointOnMesh(const TriangleMesh<ScalarType>& TargetMesh, const DenseVector<typename ScalarType::ScalarType, 3>& PointPosition)
{
    const PolygonMesh<ScalarType>& TargetMesh_ref = TargetMesh;
    return FindNearestPointOnMesh(TargetMesh_ref, PointPosition);
}


template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMesh_Linear(const TriangleMesh<ScalarType>& InputMesh)
{
	TriangleMesh<ScalarType> OutputMesh;
	
	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input;
	auto EdgeCount = EdgeCount_input * 2;
	auto FaceCount = FaceCount_input * 4;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	DenseVector<int_max> PointIndexList_init;
	PointIndexList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (int_max k = 0; k < PointCount_input; ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			auto Pos = InputMesh.GetPointPosition(k);
			PointIndexList_init.Append(OutputMesh.AddPoint(Pos));
			PointIndex_output_init += 1;
			PointIndexMap_init[k] = PointIndex_output_init;
		}
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<int_max> PointIndexList_new;
	PointIndexList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (int_max k = 0; k < EdgeCount_input; ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			auto TempList = InputMesh.Edge(k).GetPointIndexList();
			auto P0 = InputMesh.GetPointPosition(TempList[0]);
			auto P1 = InputMesh.GetPointPosition(TempList[1]);
			auto P3 = P0 + P1;
			P3 /= ScalarType(2);
			auto H3 = OutputMesh.AddPoint(P3);
			PointIndexList_new.Append(H3);
			PointIndex_output_new += 1;
			PointIndexMap_new[k] = PointIndex_output_new;
		}
	}
	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (int_max k = 0; k < FaceCount_input; ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			auto PointIndexList_input = InputMesh.Face(k).GetPointIndexList(); // P0, P1, P2
			auto EdgeIndexList_input = InputMesh.Face(k).GetEdgeIndexList();   // P0-P1, P1-P2, P2-P1
			//-----------------
			//      0
			//    3    5
			// 1    4     2
			//-----------------		
			auto H0 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[0]]];
			auto H1 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[1]]];
			auto H2 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[2]]];
			auto H3 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[0]]];
			auto H4 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[1]]];
			auto H5 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[2]]];
			OutputMesh.AddFaceByPoint(H0, H3, H5);
			OutputMesh.AddFaceByPoint(H3, H1, H4);
			OutputMesh.AddFaceByPoint(H3, H4, H5);
			OutputMesh.AddFaceByPoint(H5, H4, H2);
		}
	}
	//--------------------------------------------------------------------------//
	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMesh_Linear(const TriangleMesh<ScalarType>& InputMesh, int_max SubdivisionNumber)
{
	TriangleMesh<ScalarType> OutputMesh;
	if (SubdivisionNumber <= 0)
	{
		MDK_Error(" SubdivisionNumber <=0 @ SubdivideTriangleMesh_Linear(...,...)")
	}
	else
	{
		OutputMesh = SubdivideTriangleMesh_Linear(InputMesh);
		for (int_max k = 1; k < SubdivisionNumber; ++k)//k from 1
		{
			OutputMesh = SubdivideTriangleMesh_Linear(OutputMesh);
		}
	}
	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> SubdivideTriangleMeshByVTKLinearSubdivisionFilter(const TriangleMesh<ScalarType>& TargetMesh, int_max SubdivisionCount)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);

	auto subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	subdivisionFilter->SetInputData(VTKMesh);
	subdivisionFilter->SetNumberOfSubdivisions(SubdivisionCount);
	subdivisionFilter->Update();
	auto VTKMesh_new = subdivisionFilter->GetOutput();

	TriangleMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(VTKMesh_new, OutputMesh);
	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> SimplifyTriangleMeshByVTKDecimatePro(const TriangleMesh<ScalarType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkDecimatePro>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	TriangleMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(decimate->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> SimplifyTriangleMeshByVTKQuadricDecimation(const TriangleMesh<ScalarType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkQuadricDecimation>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	TriangleMesh<ScalarType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(decimate->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename ScalarType>
void SmoothTriangleMeshByMeanCurvature(TriangleMesh<ScalarType>& TargetMesh, double MaxDisplacementRatio, bool Flag_UpdateAttribute)
{
	if (Flag_UpdateAttribute == true)
	{
		TargetMesh.UpdateCornerAngleOfFace();
		TargetMesh.UpdateAreaOfFace();
		TargetMesh.UpdateMeanCurvatureAtPoint();
	}
	auto PointCount = TargetMesh.GetPointCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (TargetMesh.IsValidPointIndex(k) == true)
		{
			auto MC = TargetMesh.Point(k).Attribute().MeanCurvatureNormal;
			auto Displacement = MC*ScalarType(MaxDisplacementRatio);
			auto Pos = TargetMesh.GetPointPosition(k);
			Pos += Displacement;
			TargetMesh.SetPointPosition(k, Pos);
		}
	}
}

template<typename ScalarType>
void SmoothTriangleMeshByGaussianCurvature(TriangleMesh<ScalarType>& TargetMesh, double MaxDisplacement, bool Flag_UpdateAttribute)
{
	if (Flag_UpdateAttribute == true)
	{
		TargetMesh.UpdateCornerAngleOfFace();
		TargetMesh.UpdateAreaOfFace();
		TargetMesh.UpdateGaussianCurvatureAtPoint();
		TargetMesh.UpdateMeanCurvatureAtPoint();
	}

	const auto two_pi = 2.0*std::acos(-1.0);

	auto PointCount = TargetMesh.GetPointCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (TargetMesh.IsValidPointIndex(k) == true)
		{
			auto GC = TargetMesh.Point(k).Attribute().GaussianCurvature;
			auto MC = TargetMesh.Point(k).Attribute().MeanCurvatureNormal;
			auto Displacement = MC*ScalarType(std::abs(GC) / two_pi);
			Displacement = std::min(Displacement, MaxDisplacement);
			auto Pos = TargetMesh.GetPointPosition(k);
			Pos += Displacement;
			TargetMesh.SetPointPosition(k, Pos);
		}
	}
}


template<typename ScalarType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<ScalarType>& TargetMesh, int_max MaxIter, double Alpha, bool Flag_BoundarySmoothing, bool Flag_TerminateIfTotalCurvatureIncrease)
{
	DenseVector<int_max> PointIndexList_NOSmoothing;
	auto PointCount = TargetMesh.GetPointCount();
	PointIndexList_NOSmoothing.SetCapacity(PointCount/100);	
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (TargetMesh.IsValidPointIndex(k) == true)
		{
			if (TargetMesh.Point(k).IsOnBoundaryEdge() == true)
			{
				if (Flag_BoundarySmoothing == false)
				{
					PointIndexList_NOSmoothing.Append(k);
				}
			}
		}
	}
	SmoothTriangleMeshByNormalBasedCurvature(TargetMesh, MaxIter, Alpha, PointIndexList_NOSmoothing, Flag_TerminateIfTotalCurvatureIncrease);
}


template<typename ScalarType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<ScalarType>& TargetMesh, int_max MaxIter, double Alpha, const DenseVector<int_max>& PointIndexList_NOSmoothing, bool Flag_TerminateIfTotalCurvatureIncrease)
{	
	//-------------------- check input ----------------------------
	if (TargetMesh.IsEmpty() == true)
	{
		return;
	}

	if (MaxIter < 0)
	{
		MDK_Error("MaxIter < 0 @ SmoothTriangleMeshByNormalBasedCurvature(...)")
		return;
	}
	else if (MaxIter == 0)
	{
		return;
	}

	if (Alpha < 0)
	{
		MDK_Error("Alpha < 0 @ SmoothTriangleMeshByNormalBasedCurvature(...)")
		return;
	}
	else if (Alpha == 0)
	{
		return;
	}

	//-------------------------------------------------------------

	DenseVector<int_max> PointIndexList_Smoothing;
	auto PointCount = TargetMesh.GetPointCount();
	PointIndexList_Smoothing.SetCapacity(PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (TargetMesh.IsValidPointIndex(k) == true)
		{
			auto temp = PointIndexList_NOSmoothing.ExactMatch("first", k);
			if (temp < 0)
			{
				PointIndexList_Smoothing.Append(k);
			}
		}
	}
	//------------------------------------------------------------	
	TargetMesh.UpdateCornerAngleOfFace(ALL);
	TargetMesh.UpdateNormalAtFace(ALL);
	TargetMesh.UpdateAngleWeightedNormalAtPoint(ALL);
	TargetMesh.UpdateNormalBasedCurvatureAtPoint(ALL);
	//------------------------------------------------------------
	DenseVector<ScalarType> CurvatureList_before, CurvatureList_after;
	CurvatureList_before.Resize(PointIndexList_Smoothing.GetLength());
	CurvatureList_after.Resize(PointIndexList_Smoothing.GetLength());

	for (int_max iter = 0; iter < MaxIter; ++iter)
	{
		for(int_max k=0; k < PointIndexList_Smoothing.GetLength(); ++k)
		{
			auto PointIndex = PointIndexList_Smoothing(k);

			bool Flag_smooth = true;
			DenseVector<int_max> AdjacentPointIndexList;
			if (Flag_smooth == true)
			{
				AdjacentPointIndexList = TargetMesh.Point(PointIndex).GetAdjacentPointIndexList();
				auto AdjacentPointCount = AdjacentPointIndexList.GetLength();
				if (AdjacentPointCount <= 1)
				{
					Flag_smooth = false;
				}
			}		

			auto Curvature = TargetMesh.Point(PointIndex).Attribute().NormalBasedCurvature;
			CurvatureList_before[k] = Curvature;
			if (Curvature <= std::numeric_limits<ScalarType>::epsilon())
			{
				Flag_smooth = false;
			}

			if (Flag_smooth == true)
			{				
				auto Normal = TargetMesh.Point(PointIndex).Attribute().AngleWeightedNormal;
				auto AdjacentPointCount = AdjacentPointIndexList.GetLength();
				auto Position = TargetMesh.Point(PointIndex).GetPosition();
				// find direction to move
				ScalarType Projection = 0;
				for (int_max k = 0; k < AdjacentPointCount; ++k)
				{
					auto Pos_k = TargetMesh.GetPointPosition(AdjacentPointIndexList[k]);
					auto EdgeVector = Pos_k - Position;
					auto Proj_k = EdgeVector[0] * Normal[0] + EdgeVector[1] * Normal[1] + EdgeVector[2] * Normal[2];					
					if (k == 0)
					{
						Projection = Proj_k;
					}
					else
					{
						if (std::abs(Projection) < std::abs(Proj_k))
						{
							Projection = Proj_k;
						}
					}
				}
				// move the point along normal or negtive normal direction
				ScalarType delta = ScalarType(Alpha)*Curvature*Projection*(ScalarType(iter + 1) / ScalarType(MaxIter));
				auto NewPosition = Position + delta*Normal;
				// update
				TargetMesh.Point(PointIndex).SetPosition(NewPosition);
			}
		}

		TargetMesh.UpdateCornerAngleOfFace(ALL);
		TargetMesh.UpdateNormalAtFace(ALL);
		TargetMesh.UpdateAngleWeightedNormalAtPoint(ALL);
		TargetMesh.UpdateNormalBasedCurvatureAtPoint(ALL);

		for (int_max k = 0; k < PointIndexList_Smoothing.GetLength(); ++k)
		{
			auto PointIndex = PointIndexList_Smoothing(k);
			auto Curvature = TargetMesh.Point(PointIndex).Attribute().NormalBasedCurvature;
			CurvatureList_after[k] = Curvature;
		}

		if (Flag_TerminateIfTotalCurvatureIncrease == true)
		{
			auto total_curvature_before = CurvatureList_before.Sum();
			auto total_curvature_after = CurvatureList_after.Sum();
			//std::cout << "total_curvature_before=" << total_curvature_before << '\n';
			//std::cout << "total_curvature_after=" << total_curvature_after << '\n';
			auto max_curvature_before = CurvatureList_before.Max();
			auto max_curvature_after = CurvatureList_after.Max();
			//std::cout << "max_curvature_before=" << max_curvature_before << '\n';
			//std::cout << "max_curvature_after=" << max_curvature_after << '\n';

			if (max_curvature_after > max_curvature_before)
			{
				//MDK_Warning("max_curvature_after > max_curvature_before at iter=" + std::to_string(iter) + " @ SmoothTriangleMeshByNormalBasedCurvature(...)")
				//break;
			}

			if (total_curvature_after > total_curvature_before)
			{
				//MDK_Warning("total_curvature_after > total_curvature_before at iter=" + std::to_string(iter) + " @ SmoothTriangleMeshByNormalBasedCurvature(...)")
				break;
			}
		}
	}
}


}//namespace mdk
