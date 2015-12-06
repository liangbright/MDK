#pragma once

#include "mdkTriangleMeshProcessing.h"

using namespace mdk;

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

