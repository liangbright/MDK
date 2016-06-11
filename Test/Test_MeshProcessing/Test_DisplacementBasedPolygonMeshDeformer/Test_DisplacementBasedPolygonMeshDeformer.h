#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_DisplacementBasedPolygonMeshDeformer/TestData/";

	TriangleMesh<TriangleMeshStandardAttributeType<double>> InputMesh;	
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

	DisplacementBasedPolygonMeshDeformer<TriangleMeshStandardAttributeType<double>> Deformer;
	Deformer.InputMesh().Share(InputMesh);
	Deformer.InputDisplacementField().Share(InputDisplacementField);
	Deformer.ConfidenceOfInputDisplacementField().Share(ConfidenceOfInputDisplacementField);
	Deformer.ConfidenceOfSmoothness().Share(ConfidenceOfSmoothness);
	Deformer.Update();

	SavePolygonMeshAsVTKFile(Deformer.OutputMesh(), TestDataPath + "Test_a_OutputMesh.vtk");
}
