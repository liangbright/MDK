#pragma once

namespace mdk
{

template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertPolygonMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh)
{
	if (InputMesh.IsEmpty() == true)
	{
		OutputMesh.Clear();
		return;
	}

	if (InputMesh.CheckIfTriangleMesh() == true)
	{
		typedef MeshAttributeTypeA::ScalarType ScalarType;
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

template<typename MeshAttributeType>
DenseVector<int_max> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max PointIndex_start)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref, PointIndex_start);
}

template<typename MeshAttributeType>
ObjectArray<DenseVector<int_max>> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref);
}

template<typename MeshAttributeType>
int_max FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return FindNearestPointOnMesh(TargetMesh_ref, PointPosition);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMesh_Linear(const TriangleMesh<MeshAttributeType>& InputMesh)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;
	//----------------------------------------------------------------------------//
	TriangleMesh<MeshAttributeType> OutputMesh;
	
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
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointIndexList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<int_max> PointIndexList_new;
	PointIndexList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointIndexList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointIndexList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList_input = it.Face().GetPointIndexList(); // P0, P1, P2
		auto EdgeIndexList_input = it.Face().GetEdgeIndexList();   // P0-P1, P1-P2, P2-P1
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
	//--------------------------------------------------------------------------//
	return OutputMesh;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMesh_Linear(const TriangleMesh<MeshAttributeType>& InputMesh, int_max SubdivisionNumber)
{
	TriangleMesh<MeshAttributeType> OutputMesh;
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


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SubdivideTriangleMeshByVTKLinearSubdivisionFilter(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max SubdivisionCount)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);

	auto subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	subdivisionFilter->SetInputData(VTKMesh);
	subdivisionFilter->SetNumberOfSubdivisions(SubdivisionCount);
	subdivisionFilter->Update();
	auto VTKMesh_new = subdivisionFilter->GetOutput();

	TriangleMesh<MeshAttributeType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(VTKMesh_new, OutputMesh);
	return OutputMesh;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKDecimatePro(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkDecimatePro>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	TriangleMesh<MeshAttributeType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(decimate->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKQuadricDecimation(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkQuadricDecimation>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	TriangleMesh<MeshAttributeType> OutputMesh;
	ConvertVTKPolyDataToMDKTriangleMesh(decimate->GetOutput(), OutputMesh);
	return OutputMesh;
}


template<typename MeshAttributeType>
void SmoothTriangleMeshByMeanCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, double MaxDisplacementRatio, bool Flag_UpdateAttribute)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;

	if (Flag_UpdateAttribute == true)
	{
		TargetMesh.UpdateCornerAngleOfFace();
		TargetMesh.UpdateAreaOfFace();
		TargetMesh.UpdateMeanCurvatureAtPoint();
	}

	for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto MC = it.Point().Attribute().MeanCurvatureNormal;
		auto Displacement = MC*ScalarType(MaxDisplacementRatio);
		auto Pos = it.Point().GetPosition();
		Pos += Displacement;
		it.Point().SetPosition(Pos);
	}
}

template<typename MeshAttributeType>
void SmoothTriangleMeshByGaussianCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, double MaxDisplacement, bool Flag_UpdateAttribute)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;

	if (Flag_UpdateAttribute == true)
	{
		TargetMesh.UpdateCornerAngleOfFace();
		TargetMesh.UpdateAreaOfFace();
		TargetMesh.UpdateGaussianCurvatureAtPoint();
		TargetMesh.UpdateMeanCurvatureAtPoint();
	}

	const auto two_pi = 2.0*std::acos(-1.0);

	for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto GC = it.Point().Attribute().GaussianCurvature;
		auto MC = it.Point().Attribute().MeanCurvatureNormal;
		auto Displacement = MC*ScalarType(std::abs(GC) / two_pi);
		Displacement = std::min(Displacement, MaxDisplacement);
		auto Pos = it.Point().GetPosition();
		Pos += Displacement;
		it.Point().SetPosition(Pos);
	}
}

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SmoothMeshByVTKSmoothPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, int_max MaxIter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	const PolygonMesh<MeshAttributeType>& InputMesh_ref = InputMesh;
	auto OutputMesh = SmoothMeshByVTKSmoothPolyDataFilter(InputMesh_ref, MaxIter, Flag_FeatureEdgeSmoothing, Flag_BoundarySmoothing);
	TriangleMesh<MeshAttributeType> OutputMesh_tri;
	OutputMesh_tri.Construct(std::move(OutputMesh));
	return OutputMesh_tri;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SmoothMeshByVTKWindowedSincPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, double PassBand, int_max MaxIter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	const PolygonMesh<MeshAttributeType>& InputMesh_ref = InputMesh;
	auto OutputMesh = SmoothMeshByVTKWindowedSincPolyDataFilter(InputMesh_ref, PassBand, MaxIter, Flag_FeatureEdgeSmoothing, Flag_BoundarySmoothing);
	TriangleMesh<MeshAttributeType> OutputMesh_tri;
	OutputMesh_tri.Construct(std::move(OutputMesh));
	return OutputMesh_tri;
}

template<typename MeshAttributeType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, int_max MaxIter, double Alpha, bool Flag_BoundarySmoothing, bool Flag_TerminateIfTotalCurvatureIncrease)
{
	DenseVector<int_max> PointIndexList_NOSmoothing;
	auto PointCount = TargetMesh.GetPointCount();
	PointIndexList_NOSmoothing.SetCapacity(PointCount/100);
	for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnBoundaryEdge() == true)
		{
			if (Flag_BoundarySmoothing == false)
			{
				PointIndexList_NOSmoothing.Append(it.GetPointIndex());
			}
		}
	}
	SmoothTriangleMeshByNormalBasedCurvature(TargetMesh, MaxIter, Alpha, PointIndexList_NOSmoothing, Flag_TerminateIfTotalCurvatureIncrease);
}


template<typename MeshAttributeType>
void SmoothTriangleMeshByNormalBasedCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, int_max MaxIter, double Alpha, const DenseVector<int_max>& PointIndexList_NOSmoothing, bool Flag_TerminateIfTotalCurvatureIncrease)
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

	typedef MeshAttributeType::ScalarType ScalarType;
	//-------------------------------------------------------------

	DenseVector<int_max> PointIndexList_Smoothing;
	auto PointCount = TargetMesh.GetPointCount();
	PointIndexList_Smoothing.SetCapacity(PointCount);
	for (auto it = TargetMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto temp = PointIndexList_NOSmoothing.ExactMatch("first", it.GetPointIndex());
		if (temp < 0)
		{
			PointIndexList_Smoothing.Append(it.GetPointIndex());
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
