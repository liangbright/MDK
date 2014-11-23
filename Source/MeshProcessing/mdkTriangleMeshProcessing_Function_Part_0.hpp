#ifndef __mdkTriangleMeshProcessing_Function_Part_0_hpp
#define __mdkTriangleMeshProcessing_Function_Part_0_hpp

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh,
                                                                    Handle_Of_Point_Of_MembraneMesh PointHandle_start)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref, PointHandle_start);
}

template<typename MeshAttributeType>
ObjectArray<DenseVector<Handle_Of_Point_Of_MembraneMesh>> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref);
}

template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, 
                                                       const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return FindNearestPointOnMesh(TargetMesh_ref, PointPosition);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKDecimatePro(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkDecimatePro>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	return ConvertVTKPolyDataToMDKTriangleMesh<MeshAttributeType>(decimate->GetOutput());
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> SimplifyTriangleMeshByVTKQuadricDecimation(const TriangleMesh<MeshAttributeType>& TargetMesh, double TargetReduction)
{
	auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(TargetMesh);
	auto decimate = vtkSmartPointer<vtkQuadricDecimation>::New();
	decimate->SetInputData(VTKMesh);
	decimate->SetTargetReduction(TargetReduction);
	decimate->Update();
	return ConvertVTKPolyDataToMDKTriangleMesh<MeshAttributeType>(decimate->GetOutput());
}


template<typename MeshAttributeType>
void SmoothTriangleMeshByMeanCurvature(TriangleMesh<MeshAttributeType>& TargetMesh, double MaxDisplacementRatio, bool Flag_UpdateAttribute)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;

	if (Flag_UpdateAttribute == true)
	{
		TargetMesh.UpdateCornerAngleOfCell();
		TargetMesh.UpdateAreaOfCell();
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
		TargetMesh.UpdateCornerAngleOfCell();
		TargetMesh.UpdateAreaOfCell();
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

}//namespace mdk


#endif