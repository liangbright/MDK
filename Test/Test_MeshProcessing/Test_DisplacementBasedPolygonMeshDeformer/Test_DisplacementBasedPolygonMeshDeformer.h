#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_DisplacementBasedPolygonMeshDeformer/TestData/";

	TriangleMesh<double> InputMesh;	
	LoadPolygonMeshFromVTKFile(InputMesh, TestDataPath + "Leaflet.vtk");

	auto PointCount = InputMesh.GetPointCount();
	InputMesh.UpdateNormalAtFace(ALL);
	InputMesh.UpdateCornerAngleOfFace(ALL);
	InputMesh.UpdateAngleWeightedNormalAtPoint(ALL);

	DenseMatrix<double> InputDisplacementField, ConfidenceOfInputDisplacementField, ConfidenceOfSmoothness;
	InputDisplacementField.Resize(3, PointCount);	
	ConfidenceOfInputDisplacementField.Resize(1, PointCount);	
	ConfidenceOfSmoothness.Resize(1, PointCount);
	
	ConfidenceOfSmoothness.Fill(1);

	InputDisplacementField.Fill(0);
	ConfidenceOfInputDisplacementField.Fill(0);
	for (int_max k = 176; k < 177; ++k)
	{
		auto Normal = InputMesh.Point(k).Attribute().AngleWeightedNormal;
		InputDisplacementField(0, k) = -10*Normal[0];
		InputDisplacementField(1, k) = -10*Normal[1];
		InputDisplacementField(2, k) = -10*Normal[2];
		ConfidenceOfInputDisplacementField[k] = 1;
		ConfidenceOfSmoothness[k] = 10;
	}

	for (int_max k = 0; k < PointCount; ++k)
	{
		if (InputMesh.Point(k).IsOnBoundaryEdge() == true)
		{
			ConfidenceOfSmoothness[k] = 0;
			ConfidenceOfInputDisplacementField[k] = 1;
		}
	}

	DisplacementBasedPolygonMeshDeformer<double> Deformer;
	Deformer.Input.SourceMesh = &InputMesh;
	Deformer.Input.DisplacementField.Share(InputDisplacementField);
	Deformer.Input.ConfidenceOfDisplacementField.Share(ConfidenceOfInputDisplacementField);
	Deformer.Input.ConfidenceOfSmoothness.Share(ConfidenceOfSmoothness);
	Deformer.Update();

	SavePolygonMeshAsVTKFile(Deformer.Output.DeformedMesh, TestDataPath + "Test_a_OutputMesh.vtk");
}


void Test_b()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_DisplacementBasedPolygonMeshDeformer/TestData/";

	PolygonMesh<double> InputMesh, TemplateMesh, TemplateMesh_TPS;
	LoadPolygonMeshFromVTKFile(InputMesh, TestDataPath + "Leaflet_quad.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "Template.vtk");

	auto Boundary = TraceMeshBoundaryCurve(InputMesh, 25);
	auto Source = TemplateMesh.GetPointPosition(Boundary);
	auto Target = InputMesh.GetPointPosition(Boundary);
	{
		ThinPlateSplineTransform3D<double> TPS;
		TPS.SetSourceLandmarkPointSet(&Source);
		TPS.SetTargetLandmarkPointSet(&Target);
		TPS.EstimateParameter();
		TemplateMesh_TPS = TemplateMesh;
		auto PositionMatrix = TPS.TransformPoint(TemplateMesh.GetPointPosition(ALL));
		TemplateMesh_TPS.SetPointPosition(ALL, PositionMatrix);
	}
	{
		AffineTransform3D<double> Transform;
		//SimilarityTransform3D<double> Transform;
		Transform.SetSourceLandmarkPointSet(&Source);
		Transform.SetTargetLandmarkPointSet(&Target);
		Transform.EstimateParameter();
		auto PositionMatrix = Transform.TransformPoint(TemplateMesh.GetPointPosition(ALL));
		TemplateMesh.SetPointPosition(ALL, PositionMatrix);
	}
	auto PointCount = InputMesh.GetPointCount();

	DenseMatrix<double> InputDisplacementField, ConfidenceOfInputDisplacementField, ConfidenceOfSmoothness;
	InputDisplacementField.Resize(3, PointCount);
	ConfidenceOfInputDisplacementField.Resize(1, PointCount);
	ConfidenceOfSmoothness.Resize(1, PointCount);

	ConfidenceOfSmoothness.Fill(1);
	InputDisplacementField.Fill(0);
	ConfidenceOfInputDisplacementField.Fill(0);
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (InputMesh.Point(k).IsOnBoundaryEdge() == true)
		{
			auto Pos_in = InputMesh.GetPointPosition(k);
			auto Pos_temp = TemplateMesh.GetPointPosition(k);
			auto Displacement = Pos_in - Pos_temp;
			InputDisplacementField(0, k) = Displacement[0];
			InputDisplacementField(1, k) = Displacement[1];
			InputDisplacementField(2, k) = Displacement[2];
			ConfidenceOfInputDisplacementField[k] = 100;
			ConfidenceOfSmoothness[k] = 1;
		}
		else
		{
			ConfidenceOfInputDisplacementField[k] = 0;
			ConfidenceOfSmoothness[k] = 1;			
		}
	}

	DisplacementBasedPolygonMeshDeformer<double> Deformer;
	Deformer.Input.SourceMesh = &TemplateMesh;
	Deformer.Input.DisplacementField.Share(InputDisplacementField);
	Deformer.Input.ConfidenceOfDisplacementField.Share(ConfidenceOfInputDisplacementField);
	Deformer.Input.ConfidenceOfSmoothness.Share(ConfidenceOfSmoothness);
	Deformer.Update();

	SavePolygonMeshAsVTKFile(TemplateMesh_TPS, TestDataPath + "Test_b_TemplateMesh_TPS_transform.vtk");

	SavePolygonMeshAsVTKFile(TemplateMesh, TestDataPath + "Test_b_TemplateMesh_A_transform.vtk");

	SavePolygonMeshAsVTKFile(Deformer.Output.DeformedMesh, TestDataPath + "Test_b_OutputMesh.vtk");
}

void Test_c()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_DisplacementBasedPolygonMeshDeformer/TestData/";

	PolygonMesh<double> P4Mesh, P15Mesh, P4Mesh_A, P4Mesh_TPS, P4Mesh_D;
	LoadPolygonMeshFromVTKFile(P4Mesh, TestDataPath + "P4_remesh_134p.json.vtk");
	LoadPolygonMeshFromVTKFile(P15Mesh, TestDataPath + "P15_remesh_134p.json.vtk");

	auto PointCount = P4Mesh.GetPointCount();

	DenseMatrix<double> Source, Target;
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (P4Mesh.Point(k).IsOnBoundaryEdge() == true)
		{
			auto Pos_p4 = P4Mesh.GetPointPosition(k);
			auto Pos_p15 = P15Mesh.GetPointPosition(k);
			Source.AppendCol(Pos_p4);
			Target.AppendCol(Pos_p15);
		}
	}

	{
		ThinPlateSplineTransform3D<double> TPS;
		TPS.SetSourceLandmarkPointSet(&Source);
		TPS.SetTargetLandmarkPointSet(&Target);
		TPS.EstimateParameter();
		P4Mesh_TPS = P4Mesh;
		auto PositionMatrix = TPS.TransformPoint(P4Mesh.GetPointPosition(ALL));
		P4Mesh_TPS.SetPointPosition(ALL, PositionMatrix);
	}
	{
		AffineTransform3D<double> Transform;		
		Transform.SetSourceLandmarkPointSet(&Source);
		Transform.SetTargetLandmarkPointSet(&Target);
		Transform.EstimateParameter();
		P4Mesh_A = P4Mesh;
		auto PositionMatrix = Transform.TransformPoint(P4Mesh.GetPointPosition(ALL));
		P4Mesh_A.SetPointPosition(ALL, PositionMatrix);
	}
	
	DenseMatrix<double> InputDisplacementField, ConfidenceOfInputDisplacementField, ConfidenceOfSmoothness;
	InputDisplacementField.Resize(3, PointCount);
	ConfidenceOfInputDisplacementField.Resize(1, PointCount);
	ConfidenceOfSmoothness.Resize(1, PointCount);

	ConfidenceOfSmoothness.Fill(1);
	InputDisplacementField.Fill(0);
	ConfidenceOfInputDisplacementField.Fill(0);
	for (int_max k = 0; k < PointCount; ++k)
	{
		if (P4Mesh.Point(k).IsOnBoundaryEdge() == true)
		{
			auto Pos_p4 = P4Mesh_A.GetPointPosition(k);
			auto Pos_p15 = P15Mesh.GetPointPosition(k);
			auto Displacement = Pos_p15 - Pos_p4;
			InputDisplacementField(0, k) = Displacement[0];
			InputDisplacementField(1, k) = Displacement[1];
			InputDisplacementField(2, k) = Displacement[2];
			ConfidenceOfInputDisplacementField[k] = 1000;
			ConfidenceOfSmoothness[k] = 1;
		}
		else
		{
			ConfidenceOfInputDisplacementField[k] = 0;
			ConfidenceOfSmoothness[k] = 1;
		}
	}

	DisplacementBasedPolygonMeshDeformer<double> Deformer;
	typedef DisplacementBasedPolygonMeshDeformer<double>::WeightTypeEnum WeightTypeEnum;
	Deformer.Input.SourceMesh = &P4Mesh_A;
	Deformer.Input.DisplacementField.Share(InputDisplacementField);
	Deformer.Input.ConfidenceOfDisplacementField.Share(ConfidenceOfInputDisplacementField);
	Deformer.Input.ConfidenceOfSmoothness.Share(ConfidenceOfSmoothness);
	//Deformer.SetWeightType(WeightTypeEnum::One);
	Deformer.Update();

	SavePolygonMeshAsVTKFile(P4Mesh_TPS, TestDataPath + "Test_c_P4Mesh_TPS_transform_134p.vtk");
	SavePolygonMeshAsVTKFile(Deformer.Output.DeformedMesh, TestDataPath + "Test_c_OutputMesh_134p.vtk");
}
