#pragma once

#include "QuadSurfaceRemesher1.h"
#include "mdkTextFile.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_QuadSurfaceRemesher1()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/TestData/";

	DenseVector<int_max> QuadLine0 = { 451, 473, 486, 496, 220, 530, 284 };
	DenseVector<int_max> QuadLine1 = { 456, 90, 471, 397, 126 };

	QuadSurfaceRemesher1<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.PreprocessInputMesh();
	//Remesher.TraceQuadLine();
	Remesher.m_QuadLineList.Resize(2);
	Remesher.m_QuadLineList[0] = Remesher.ConvertIndexToPointHandle(QuadLine0);
	Remesher.m_QuadLineList[1] = Remesher.ConvertIndexToPointHandle(QuadLine1);
	Remesher.BuildMixedTriQuadMesh();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh_Mixed, TestDataPath + "QuadSurfaceRemesher1_outputmesh_mixed.vtk");
}
