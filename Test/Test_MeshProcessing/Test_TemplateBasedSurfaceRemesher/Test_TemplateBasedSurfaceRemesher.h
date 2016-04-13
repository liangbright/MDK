#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;


void Test_0()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "Leaflet0.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "Template.vtk");

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = { span(48, 48), span(50, 50), span(348, 360), span(74, 88), span(46, 46) };
	LowerBoundary_input = { span(48, 49), span(6, 0), span(7, 45), span(47, 47), span(46, 46) };
	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = UpperBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = LowerBoundary_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = UpperBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "Leaflet0_quad.vtk");
}


void Test_1()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "Leaflet.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "Template.vtk");

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = {span(0,0), span(34, 48), span(3,3), span(63, 49), span(1,1)};
	LowerBoundary_input = {span(0,0), span(4,18), span(2,2), span(33, 19), span(1,1)};
	UpperBoundary_template = {span(25,0), span(705,729)};
	LowerBounary_template = {span(25,65), span(768, 729)};

	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = UpperBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = LowerBoundary_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = UpperBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "Leaflet_quad.vtk");
}

void Test_2()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "Sinus.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "Template.vtk");

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = { span(0, 1), span(230, -2, 202), span(96, -2, 66), span(34, 35)};
	LowerBoundary_input = { span(0, 2, 32), span(65, -2, 35)};
	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = UpperBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = LowerBoundary_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = UpperBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "Sinus_quad.vtk");
}

void Test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "Leaflet_a.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "Template.vtk");

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;

	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	int_max PointIndex_a = 3;
	auto Boundary_input = TraceMeshBoundaryCurve(Remesher.InputMesh(), PointIndex_a);
	Boundary_input = Boundary_input.GetSubSet(span(Boundary_input.GetLength() - 1, 0));
	Boundary_input.Insert(0, PointIndex_a);
	Boundary_input.Delete(Boundary_input.GetLength() - 1);

	int_max Index_k = 0;
	for (int_max k = 0; k < Boundary_input.GetLength(); ++k)
	{
		UpperBoundary_input.Append(Boundary_input[k]);
		if (Boundary_input[k] == 132)
		{
			Index_k = k;
			break;
		}
	}
	LowerBoundary_input.Append(UpperBoundary_input[0]);
	for (int_max k = Boundary_input.GetLength() - 1; k >= Index_k; --k)
	{
		LowerBoundary_input.Append(Boundary_input[k]);
	}
	
	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = UpperBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = LowerBoundary_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = UpperBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "Leaflet_a_quad.vtk");
}

void test_hole()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "sphere_hole.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "disk_hole.vtk");

	DenseVector<int_max> OuterBoundary_input, InnerBoundary_input, LandMark_input, OuterBoundary_template, InnerBounary_template, LandMark_template;
	OuterBoundary_input = {0, 3, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 73, 79, 85, 91, 0};
	InnerBoundary_input = {10, 11, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 10};
	LandMark_input = {8};
	OuterBoundary_template = {80, 71, 62, 53, 44, 35, 26, 17, 8, 287, 278, 269, 260, 251, 242, 233, 224, 215, 206, 197, 188, 179, 170, 161,152, 143, 134, 125, 116, 107, 98, 89, 80};
	InnerBounary_template = {72, 63, 54, 45, 36, 27, 18, 9, 0, 279, 270, 261, 252, 243, 234, 225, 216, 207, 198, 189, 180, 171, 162, 153, 144, 135, 126, 117, 108, 99, 90, 81, 72};
	LandMark_template = {73};

	Remesher.BoundarySegmentListOfInputMesh().Resize(3);
	Remesher.BoundarySegmentListOfInputMesh()[0] = OuterBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = InnerBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[2] = LandMark_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(3);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = OuterBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = InnerBounary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[2] = LandMark_template;

	//Remesher.SetDiffusionCoefficientOfMeshParameterization(0);
	//Remesher.EnableTPSTransformOfTemplateMesh();
	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "sphere_hole_remesh.vtk");
	SavePolygonMeshAsVTKFile(Remesher.TransfromedInputMesh(), TestDataPath + "sphere_hole_2D.vtk");
}

void test_tube()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";

	TemplateBasedSurfaceRemesher<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.InputMesh(), TestDataPath + "62_AortaModel_Pimg.vtk");
	LoadPolygonMeshFromVTKFile(Remesher.TemplateMesh(), TestDataPath + "disk_hole.vtk");

	DenseVector<int_max> OuterBoundary_input, InnerBoundary_input, LandMark_input, OuterBoundary_template, InnerBounary_template, LandMark_template;
	OuterBoundary_input = { span(0, 49), span(0,0) };
	InnerBoundary_input = { span(4950, 4999), span(4950, 4950) };
	
	OuterBoundary_template = { 80, 71, 62, 53, 44, 35, 26, 17, 8, 287, 278, 269, 260, 251, 242, 233, 224, 215, 206, 197, 188, 179, 170, 161,152, 143, 134, 125, 116, 107, 98, 89, 80 };
	InnerBounary_template = { 72, 63, 54, 45, 36, 27, 18, 9, 0, 279, 270, 261, 252, 243, 234, 225, 216, 207, 198, 189, 180, 171, 162, 153, 144, 135, 126, 117, 108, 99, 90, 81, 72 };
	
	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = OuterBoundary_input;
	Remesher.BoundarySegmentListOfInputMesh()[1] = InnerBoundary_input;

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = OuterBoundary_template;
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = InnerBounary_template;

	//Remesher.EnableTPSTransformOfTemplateMesh();
	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "62_AortaModel_Pimg_remesh.vtk");

	SavePolygonMeshAsVTKFile(Remesher.TransfromedInputMesh(), TestDataPath + "62_AortaModel_Pimg_2D.vtk");
}