#pragma once

#include "mdkPolygonMeshProcessing.h"

using namespace mdk;


void Test_0()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "Leaflet0.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "Template.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;
	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = { span(48, 48), span(50, 50), span(348, 360), span(74, 88), span(46, 46) };
	LowerBoundary_input = { span(48, 49), span(6, 0), span(7, 45), span(47, 47), span(46, 46) };
	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = UpperBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = LowerBoundary_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = UpperBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "Leaflet0_quad.vtk");
}


void Test_1()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "Leaflet.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "Template.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = {span(0,0), span(34, 48), span(3,3), span(63, 49), span(1,1)};
	LowerBoundary_input = {span(0,0), span(4,18), span(2,2), span(33, 19), span(1,1)};
	UpperBoundary_template = {span(25,0), span(705,729)};
	LowerBounary_template = {span(25,65), span(768, 729)};

	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = UpperBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = LowerBoundary_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = UpperBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "Leaflet_quad.vtk");
}

void Test_2()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "Sinus.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "Template.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;
	UpperBoundary_input = { span(0, 1), span(230, -2, 202), span(96, -2, 66), span(34, 35)};
	LowerBoundary_input = { span(0, 2, 32), span(65, -2, 35)};
	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = UpperBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = LowerBoundary_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = UpperBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "Sinus_quad.vtk");
}

void Test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "Leaflet_a.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "Template.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;

	DenseVector<int_max> UpperBoundary_input, LowerBoundary_input, UpperBoundary_template, LowerBounary_template;

	UpperBoundary_template = { span(25, 0), span(705, 729) };
	LowerBounary_template = { span(25, 65), span(768, 729) };

	int_max PointIndex_a = 3;
	auto Boundary_input = TraceMeshBoundaryCurve(SourceMesh, PointIndex_a);
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
	
	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = UpperBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = LowerBoundary_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = UpperBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = LowerBounary_template;

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "Leaflet_a_quad.vtk");
}

void test_hole()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "sphere_hole.vtk");
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "disk_hole.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;

	DenseVector<int_max> OuterBoundary_input, InnerBoundary_input, LandMark_input, OuterBoundary_template, InnerBounary_template, LandMark_template;
	OuterBoundary_input = {0, 3, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 73, 79, 85, 91, 0};
	InnerBoundary_input = {10, 11, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 10};
	LandMark_input = {8};
	OuterBoundary_template = {80, 71, 62, 53, 44, 35, 26, 17, 8, 287, 278, 269, 260, 251, 242, 233, 224, 215, 206, 197, 188, 179, 170, 161,152, 143, 134, 125, 116, 107, 98, 89, 80};
	InnerBounary_template = {72, 63, 54, 45, 36, 27, 18, 9, 0, 279, 270, 261, 252, 243, 234, 225, 216, 207, 198, 189, 180, 171, 162, 153, 144, 135, 126, 117, 108, 99, 90, 81, 72};
	LandMark_template = {73};

	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(3);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = OuterBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = InnerBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[2] = LandMark_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(3);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = OuterBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = InnerBounary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[2] = LandMark_template;

	//Remesher.SetDiffusionCoefficientOfMeshParameterization(0);
	//Remesher.EnableTPSTransformOfTemplateMesh;
	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "sphere_hole_remesh.vtk");
	SavePolygonMeshAsVTKFile(Remesher.Output.ParameterizedSourceMesh, TestDataPath + "sphere_hole_2D.vtk");
}

void test_tube()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	TriangleMesh<double> SourceMesh, TemplateMesh;
	LoadPolygonMeshFromVTKFile(SourceMesh, TestDataPath + "62_AortaModel_Pimg.vtk");
	SourceMesh = ConvertQuadMeshToTriangleMesh(SourceMesh);
	LoadPolygonMeshFromVTKFile(TemplateMesh, TestDataPath + "disk_hole.vtk");

	TemplateBasedSurfaceRemesher<double> Remesher;
	Remesher.Input.SourceMesh = &SourceMesh;
	Remesher.Input.TemplateMesh = &TemplateMesh;

	DenseVector<int_max> OuterBoundary_input, InnerBoundary_input, LandMark_input, OuterBoundary_template, InnerBounary_template, LandMark_template;
	OuterBoundary_input = { span(0, 49), span(0,0) };
	InnerBoundary_input = { span(4950, 4999), span(4950, 4950) };
	
	OuterBoundary_template = { 80, 71, 62, 53, 44, 35, 26, 17, 8, 287, 278, 269, 260, 251, 242, 233, 224, 215, 206, 197, 188, 179, 170, 161,152, 143, 134, 125, 116, 107, 98, 89, 80 };
	InnerBounary_template = { 72, 63, 54, 45, 36, 27, 18, 9, 0, 279, 270, 261, 252, 243, 234, 225, 216, 207, 198, 189, 180, 171, 162, 153, 144, 135, 126, 117, 108, 99, 90, 81, 72 };
	
	Remesher.Input.BoundarySegmentListOfSourceMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfSourceMesh[0] = OuterBoundary_input;
	Remesher.Input.BoundarySegmentListOfSourceMesh[1] = InnerBoundary_input;

	Remesher.Input.BoundarySegmentListOfTemplateMesh.Resize(2);
	Remesher.Input.BoundarySegmentListOfTemplateMesh[0] = OuterBoundary_template;
	Remesher.Input.BoundarySegmentListOfTemplateMesh[1] = InnerBounary_template;

	//Remesher.EnableTPSTransformOfTemplateMesh;
	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.Output.DeformedTemplateMesh, TestDataPath + "62_AortaModel_Pimg_remesh.vtk");

	SavePolygonMeshAsVTKFile(Remesher.Output.ParameterizedSourceMesh, TestDataPath + "62_AortaModel_Pimg_2D.vtk");
}


void Test_template()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_TemplateBasedSurfaceRemesher/TestData/";
	PolygonMesh<double> Template;
	LoadPolygonMeshFromVTKFile(Template, TestDataPath + "Template.vtk");

	DenseVector<int_max> FreeEdge = { span(25, 0), span(705, 729) };
	DenseVector<int_max> Attachment = { span(25, 65), span(768, 729) };

	auto FreeEdgeCurve= Template.GetPointPosition(FreeEdge);
	auto AttachmentCurve_source = Template.GetPointPosition(Attachment);
	DenseMatrix<double> AttachmentCurve_target = AttachmentCurve_source;

	double delta = 3.141592654 / double(Attachment.GetLength()-1);
	for (int_max k = 1; k < Attachment.GetLength()-1; ++k)
	{
		double R = 0.453092;
		double theta = double(k)*delta;
		auto x = -R*std::cos(theta);
		auto y = 0.222382 - R*std::sin(theta);

		auto PointIndex = Attachment[k];
		AttachmentCurve_target(0, k) = x;
		AttachmentCurve_target(1, k) = y;
		AttachmentCurve_target(2, k) = 0;
	}

	DenseMatrix<double> Source, Target;
	Source = { &FreeEdgeCurve,  &AttachmentCurve_source };
	Target = { &FreeEdgeCurve, &AttachmentCurve_target };
	ThinPlateSplineTransform3D<double> TPSTransform;
	TPSTransform.SetSourceLandmarkPointSet(&Source);
	TPSTransform.SetTargetLandmarkPointSet(&Target);
	TPSTransform.EstimateParameter();
	for (auto it = Template.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		auto NewPos = TPSTransform.TransformPoint(Pos);
		it.Point().SetPosition(NewPos);
	}
	SavePolygonMeshAsVTKFile(Template, TestDataPath + "Template_half_disk.vtk");
}