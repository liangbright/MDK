#include "SurfaceMesh3DTo2DMapper1.h"
#include "mdkDenseMatrix_FileIO.h"

using namespace mdk;

void test1()
{
	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/";
	String File_InputMesh = TestDataPath + "Leaflet.vtk";
	String File_OutputMesh = TestDataPath + "Leaflet2D_output";

	String File_TemplateMesh = TestDataPath + "job-2.vtk";
	PolygonMesh<PolygonMeshStandardAttributeType<double>> TemplateMesh;
	LoadPolygonMeshFromVTKFile(TemplateMesh, File_TemplateMesh);
	PolygonMesh<PolygonMeshStandardAttributeType<double>>::PointHandleType PointH0, PointH1, PointH2;
	PointH0.SetIndex(25);
	PointH1.SetIndex(729);
	PointH2.SetIndex(65);
	auto Pos0 = TemplateMesh.GetPointPosition(PointH0);
	auto Pos1 = TemplateMesh.GetPointPosition(PointH1);
	auto Pos2 = TemplateMesh.GetPointPosition(PointH2);
	auto DirectionX = Pos1 - Pos0;
	DirectionX /= DirectionX.L2Norm();
	auto Normal = ComputeTriangleNormalIn3D(Pos0, Pos1, Pos2);
	auto DirectionY = ComputeVectorCrossProductIn3D(DirectionX, Normal);
	DirectionY /= DirectionY.L2Norm();
	for (auto it = TemplateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		auto x = ComputeVectorDotProductIn3D(Pos, DirectionX);
		auto y = ComputeVectorDotProductIn3D(Pos, DirectionY);
		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = 0;
		it.Point().SetPosition(Pos);
	}
	SavePolygonMeshAsVTKFile(TemplateMesh, TestDataPath + "TemplateMesh.vtk");

	SurfaceMesh3DTo2DMapper1<double> MeshMapper;
	LoadPolygonMeshFromVTKFile(MeshMapper.m_InputMesh, File_InputMesh);
	
	SurfaceMesh3DTo2DMapper1<double>::PointHandleType PointHandle0;
	PointHandle0.SetIndex(0);
	MeshMapper.m_BoundaryPointHandleList = TraceMeshBoundaryCurve(MeshMapper.m_InputMesh, PointHandle0);
	
	String File_UVTableOfBoundary = TestDataPath + "UVTableOfBoundary.json";
	LoadDenseMatrixFromJsonDataFile(MeshMapper.m_UVTableOfBoundary, File_UVTableOfBoundary);

	MeshMapper.Preprocess();

	MeshMapper.ApplyMeanValueBasedParameterization();
	SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh + "_mean_value.vtk");

	MeshMapper.ApplyStretchMinimizationBasedParameterization();
	SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh + "_stretch_minimization.vtk");
}

void test2()
{
	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/";
	String File_InputMesh = TestDataPath + "Leaflet.vtk";
	String File_OutputMesh = TestDataPath + "Leaflet2D_output";

	String File_TemplateMesh = TestDataPath + "TemplateMesh.vtk";
	PolygonMesh<PolygonMeshStandardAttributeType<double>> TemplateMesh;
	LoadPolygonMeshFromVTKFile(TemplateMesh, File_TemplateMesh);

	typedef SurfaceMesh3DTo2DMapper1<double>::PointHandleType PointHandleType;

	DenseMatrix<double> UVTableOfBoundary;
	UVTableOfBoundary.Resize(3, 64);
	{
		
		PointHandleType PointHandle_a;
		PointHandle_a.SetIndex(25);
		auto Boundary_template = TraceMeshBoundaryCurve(TemplateMesh, PointHandle_a);
		Boundary_template = Boundary_template.GetSubSet(span(Boundary_template.GetLength() - 1, 0));
		Boundary_template.Insert(0, PointHandle_a);
		Boundary_template.Delete(Boundary_template.GetLength() - 1);

		DenseVector<PointHandleType> UpperBoundary_template, LowerBoundary_template;
		int_max Index_k = 0;
		for (int_max k = 0; k < Boundary_template.GetLength(); ++k)
		{
			UpperBoundary_template.Append(Boundary_template[k]);
			if (Boundary_template[k].GetIndex() == 729)
			{
				Index_k = k;
				break;
			}
		}
		LowerBoundary_template = Boundary_template.GetSubSet(span(Index_k, Boundary_template.GetLength() - 1));
		LowerBoundary_template.Append(UpperBoundary_template[0]);
		auto UpperBoundary = TemplateMesh.GetPointPosition(UpperBoundary_template);
		auto LowerBoundary = TemplateMesh.GetPointPosition(LowerBoundary_template);
		UpperBoundary = Resample3DCurveWithEqualSegmentLength(UpperBoundary, 33);
		LowerBoundary = Resample3DCurveWithEqualSegmentLength(LowerBoundary, 33);
		UVTableOfBoundary(ALL, span(0, 32)) = UpperBoundary;
		UVTableOfBoundary(ALL, span(33, 63)) = LowerBoundary(ALL, span(1, 31));
	}
	String File_UVTableOfBoundary = TestDataPath + "UVTableOfBoundary.json";
	SaveDenseMatrixAsJsonDataFile(UVTableOfBoundary, File_UVTableOfBoundary);

	SurfaceMesh3DTo2DMapper1<double> MeshMapper;
	LoadPolygonMeshFromVTKFile(MeshMapper.m_InputMesh, File_InputMesh);

	MeshMapper.m_UVTableOfBoundary = UVTableOfBoundary;

	PointHandleType PointHandle0;
	PointHandle0.SetIndex(0);
	MeshMapper.m_BoundaryPointHandleList = TraceMeshBoundaryCurve(MeshMapper.m_InputMesh, PointHandle0);

	MeshMapper.Preprocess();

	MeshMapper.ApplyMeanValueBasedParameterization();
	SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh + "_mean_value.vtk");
	{
		PolygonMesh<PolygonMeshStandardAttributeType<double>> Leaflet_quad;
		auto PointPosition_3D = MeshMapper.m_InputMesh.GetPointPositionMatrix();
		auto PoingPosition_2D = MeshMapper.m_OutputMesh.GetPointPositionMatrix();
		ThinPlateSplineTransform3D<double> TPSTransform;
		TPSTransform.SetSourceLandmarkPointSet(&PoingPosition_2D);
		TPSTransform.SetTargetLandmarkPointSet(&PointPosition_3D);
		TPSTransform.EstimateParameter();
		Leaflet_quad = TemplateMesh;
		for (auto it = Leaflet_quad.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
		{
			auto Pos = it.Point().GetPosition();
			auto Pos_new = TPSTransform.TransformPoint(Pos);
			it.Point().SetPosition(Pos_new);
		}
		SavePolygonMeshAsVTKFile(Leaflet_quad, File_OutputMesh + "_quad_mean_value.vtk");
	}

	MeshMapper.ApplyStretchMinimizationBasedParameterization();
	SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh + "_stretch_minimization.vtk");
	{
		PolygonMesh<PolygonMeshStandardAttributeType<double>> Leaflet_quad;
		auto PointPosition_3D = MeshMapper.m_InputMesh.GetPointPositionMatrix();
		auto PoingPosition_2D = MeshMapper.m_OutputMesh.GetPointPositionMatrix();
		ThinPlateSplineTransform3D<double> TPSTransform;
		TPSTransform.SetSourceLandmarkPointSet(&PoingPosition_2D);
		TPSTransform.SetTargetLandmarkPointSet(&PointPosition_3D);
		TPSTransform.EstimateParameter();
		Leaflet_quad = TemplateMesh;
		for (auto it = Leaflet_quad.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
		{
			auto Pos = it.Point().GetPosition();
			auto Pos_new = TPSTransform.TransformPoint(Pos);
			it.Point().SetPosition(Pos_new);
		}
		SavePolygonMeshAsVTKFile(Leaflet_quad, File_OutputMesh + "_quad_stretch_minimization.vtk");
	}
}

void test3()
{
	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/Leaflet/";
	for (int_max k = 0; k < 10; ++k)
	{
		for (int_max n = 0; n < 3; ++n)
		{
			String File_InputMesh = TestDataPath + "Model_" + std::to_string(k) + "/Leaflet_" + std::to_string(n) + "_PCH17.vtk";
			String File_TemplateMesh = TestDataPath + "TemplateMesh.vtk";
			String File_UVTableOfBoundary = TestDataPath + "UVTableOfBoundary.json";
			String File_OutputMesh_MeanValue2D = TestDataPath + "LeafletRemesh_2D_mean_value" + std::to_string(k) + std::to_string(n) + ".vtk";
			String File_OutputMesh_MeanValue3D = TestDataPath + "LeafletRemesh_3D_mean_value" + std::to_string(k) + std::to_string(n) + ".vtk";
			String File_OutputMesh_MinStretch2D = TestDataPath + "LeafletRemesh_2D_min_stretch" + std::to_string(k) + std::to_string(n) + ".vtk";
			String File_OutputMesh_MinStretch3D = TestDataPath + "LeafletRemesh_3D_min_stretch" + std::to_string(k) + std::to_string(n) + ".vtk";

			PolygonMesh<PolygonMeshStandardAttributeType<double>> TemplateMesh;
			LoadPolygonMeshFromVTKFile(TemplateMesh, File_TemplateMesh);

			SurfaceMesh3DTo2DMapper1<double> MeshMapper;
			LoadDenseMatrixFromJsonDataFile(MeshMapper.m_UVTableOfBoundary, File_UVTableOfBoundary);
			LoadPolygonMeshFromVTKFile(MeshMapper.m_InputMesh, File_InputMesh);

			typedef SurfaceMesh3DTo2DMapper1<double>::PointHandleType PointHandleType;
			PointHandleType PointHandle0;
			PointHandle0.SetIndex(0);
			MeshMapper.m_BoundaryPointHandleList = TraceMeshBoundaryCurve(MeshMapper.m_InputMesh, PointHandle0);

			MeshMapper.Preprocess();

			MeshMapper.ApplyMeanValueBasedParameterization();
			SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh_MeanValue2D);
			{
				PolygonMesh<PolygonMeshStandardAttributeType<double>> Leaflet_quad;
				auto PointPosition_3D = MeshMapper.m_InputMesh.GetPointPositionMatrix();
				auto PoingPosition_2D = MeshMapper.m_OutputMesh.GetPointPositionMatrix();
				ThinPlateSplineTransform3D<double> TPSTransform;
				TPSTransform.SetSourceLandmarkPointSet(&PoingPosition_2D);
				TPSTransform.SetTargetLandmarkPointSet(&PointPosition_3D);
				TPSTransform.EstimateParameter();
				Leaflet_quad = TemplateMesh;
				for (auto it = Leaflet_quad.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
				{
					auto Pos = it.Point().GetPosition();
					auto Pos_new = TPSTransform.TransformPoint(Pos);
					it.Point().SetPosition(Pos_new);
				}
				SavePolygonMeshAsVTKFile(Leaflet_quad, File_OutputMesh_MeanValue3D);
			}

			MeshMapper.ApplyStretchMinimizationBasedParameterization();
			SavePolygonMeshAsVTKFile(MeshMapper.m_OutputMesh, File_OutputMesh_MinStretch2D);
			{
				PolygonMesh<PolygonMeshStandardAttributeType<double>> Leaflet_quad;
				auto PointPosition_3D = MeshMapper.m_InputMesh.GetPointPositionMatrix();
				auto PoingPosition_2D = MeshMapper.m_OutputMesh.GetPointPositionMatrix();
				ThinPlateSplineTransform3D<double> TPSTransform;
				TPSTransform.SetSourceLandmarkPointSet(&PoingPosition_2D);
				TPSTransform.SetTargetLandmarkPointSet(&PointPosition_3D);
				TPSTransform.EstimateParameter();
				Leaflet_quad = TemplateMesh;
				for (auto it = Leaflet_quad.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
				{
					auto Pos = it.Point().GetPosition();
					auto Pos_new = TPSTransform.TransformPoint(Pos);
					it.Point().SetPosition(Pos_new);
				}
				SavePolygonMeshAsVTKFile(Leaflet_quad, File_OutputMesh_MinStretch3D);
			}
		}
	}
}

void main()
{
	//test1();
	//test2();
	test3();
}