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
		DenseMatrix<ScalarType> PointPositionMatrix;
		ObjectArray<DenseVector<int_max>> FaceTable;
		InputMesh.GetPointPositionMatrixAndFaceTable(PointPositionMatrix, FaceTable);
		OutputMesh.Construct(std::move(PointPositionMatrix), FaceTable);
		return;
	}

	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	ConvertVTKPolyDataToMDKTriangleMesh(VTKMesh, OutputMesh);
}

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_PolygonMesh> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh, Handle_Of_Point_Of_PolygonMesh PointHandle_start)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref, PointHandle_start);
}

template<typename MeshAttributeType>
ObjectArray<DenseVector<Handle_Of_Point_Of_PolygonMesh>> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref);
}

template<typename MeshAttributeType>
Handle_Of_Point_Of_PolygonMesh FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
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
	DenseVector<Handle_Of_Point_Of_PolygonMesh> PointHandleList_init;
	PointHandleList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointHandleList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointHandle().GetIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<Handle_Of_Point_Of_PolygonMesh> PointHandleList_new;
	PointHandleList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointHandleList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointHandleList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeHandle().GetIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_input = it.Face().GetPointHandleList(); // P0, P1, P2
		auto EdgeHandleList_input = it.Face().GetEdgeHandleList();   // P0-P1, P1-P2, P2-P1
		//-----------------
		//      0
		//    3    5
		// 1    4     2
		//-----------------		
		auto H0 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[0].GetIndex()]];
		auto H1 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[1].GetIndex()]];
		auto H2 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[2].GetIndex()]];
		auto H3 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[0].GetIndex()]];
		auto H4 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[1].GetIndex()]];
		auto H5 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[2].GetIndex()]];
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
TriangleMesh<MeshAttributeType> SubdivideTriangleMeshByVTKLinearSubdivisionFilter(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max SubdivisionNumber)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);

	auto subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	subdivisionFilter->SetInputData(VTKMesh);
	subdivisionFilter->SetNumberOfSubdivisions(SubdivisionNumber);
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
TriangleMesh<MeshAttributeType> SmoothMeshByVTKSmoothPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	const PolygonMesh<MeshAttributeType>& InputMesh_ref = InputMesh;
	auto OutputMesh = SmoothMeshByVTKSmoothPolyDataFilter(InputMesh_ref, Iter, Flag_FeatureEdgeSmoothing, Flag_BoundarySmoothing);
	TriangleMesh<MeshAttributeType> OutputMesh_tri;
	OutputMesh_tri.Construct(std::move(OutputMesh));
	return OutputMesh_tri;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SmoothMeshByVTKWindowedSincPolyDataFilter(const TriangleMesh<MeshAttributeType>& InputMesh, double PassBand, int_max Iter, bool Flag_FeatureEdgeSmoothing, bool Flag_BoundarySmoothing)
{
	const PolygonMesh<MeshAttributeType>& InputMesh_ref = InputMesh;
	auto OutputMesh = SmoothMeshByVTKWindowedSincPolyDataFilter(InputMesh_ref, PassBand, Iter, Flag_FeatureEdgeSmoothing, Flag_BoundarySmoothing);
	TriangleMesh<MeshAttributeType> OutputMesh_tri;
	OutputMesh_tri.Construct(std::move(OutputMesh));
	return OutputMesh_tri;
}

}//namespace mdk
