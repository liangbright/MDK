#pragma once

#include "mdkTriangleMeshProcessing.h"

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
	Remesher.BoundarySegmentListOfInputMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_input);
	Remesher.BoundarySegmentListOfInputMesh()[1] = Remesher.ConvertIndexToHandle(LowerBoundary_input);

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_template);
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = Remesher.ConvertIndexToHandle(LowerBounary_template);

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
	Remesher.BoundarySegmentListOfInputMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_input);
	Remesher.BoundarySegmentListOfInputMesh()[1] = Remesher.ConvertIndexToHandle(LowerBoundary_input);

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_template);
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = Remesher.ConvertIndexToHandle(LowerBounary_template);

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
	Remesher.BoundarySegmentListOfInputMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_input);
	Remesher.BoundarySegmentListOfInputMesh()[1] = Remesher.ConvertIndexToHandle(LowerBoundary_input);

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_template);
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = Remesher.ConvertIndexToHandle(LowerBounary_template);

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

	typedef TemplateBasedSurfaceRemesher<double>::PointHandleType PointHandleType;
	PointHandleType PointHandle_a;
	PointHandle_a.SetIndex(3);
	auto Boundary_input = TraceMeshBoundaryCurve(Remesher.InputMesh(), PointHandle_a);
	Boundary_input = Boundary_input.GetSubSet(span(Boundary_input.GetLength() - 1, 0));
	Boundary_input.Insert(0, PointHandle_a);
	Boundary_input.Delete(Boundary_input.GetLength() - 1);

	int_max Index_k = 0;
	for (int_max k = 0; k < Boundary_input.GetLength(); ++k)
	{
		UpperBoundary_input.Append(Boundary_input[k].GetIndex());
		if (Boundary_input[k].GetIndex() == 132)
		{
			Index_k = k;
			break;
		}
	}
	LowerBoundary_input.Append(UpperBoundary_input[0]);
	for (int_max k = Boundary_input.GetLength() - 1; k >= Index_k; --k)
	{
		LowerBoundary_input.Append(Boundary_input[k].GetIndex());
	}
	
	Remesher.BoundarySegmentListOfInputMesh().Resize(2);
	Remesher.BoundarySegmentListOfInputMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_input);
	Remesher.BoundarySegmentListOfInputMesh()[1] = Remesher.ConvertIndexToHandle(LowerBoundary_input);

	Remesher.BoundarySegmentListOfTemplateMesh().Resize(2);
	Remesher.BoundarySegmentListOfTemplateMesh()[0] = Remesher.ConvertIndexToHandle(UpperBoundary_template);
	Remesher.BoundarySegmentListOfTemplateMesh()[1] = Remesher.ConvertIndexToHandle(LowerBounary_template);

	Remesher.Update();

	SavePolygonMeshAsVTKFile(Remesher.OutputMesh(), TestDataPath + "Leaflet_a_quad.vtk");
}
