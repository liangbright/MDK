#pragma once

#include "SurfaceRemesher1.h"
#include "mdkTextFile.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_a1()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceRemesher1/TestData/";

	SurfaceRemesher1<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh1.vtk");
	Remesher.FindMiddlePointOfEdgeOfInputMesh();
	Remesher.GenerateQuadCandidate();
	Remesher.GenerateConstraint();
	Remesher.EvaluateQuadCandidate();

	TextFile OutputFile_a(TestDataPath + "candidate_score.txt");
	OutputFile_a.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidateScoreList.GetLength(); ++k)
	{
		OutputFile_a << std::to_string(Remesher.m_QuadCandidateScoreList[k]);	
		if (k < Remesher.m_QuadCandidateScoreList.GetLength() - 1)
		{
			OutputFile_a << '\n';
		}
	}
	OutputFile_a.Close();

	TextFile OutputFile_b(TestDataPath + "candidate_constraint.txt");
	OutputFile_b.Clear();
	for (int_max k = 0; k < Remesher.m_Constraint.GetLength(); ++k)
	{
		OutputFile_b << std::to_string(Remesher.m_Constraint[k][0]) << ", " << std::to_string(Remesher.m_Constraint[k][1]);
		if (k < Remesher.m_Constraint.GetLength() - 1)
		{
			OutputFile_b << '\n';
		}
	}
	OutputFile_b.Close();
}

void Test_a2()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceRemesher1/TestData/";

	SurfaceRemesher1<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh1.vtk");
	Remesher.FindMiddlePointOfEdgeOfInputMesh();
	Remesher.GenerateQuadCandidate();
	Remesher.EvaluateQuadCandidate();

	DenseMatrix<double> FlagList;
	LoadDenseMatrixFromJsonDataFile(FlagList, TestDataPath + "FlagList.json");

	Remesher.m_QuadCandidateFlagList.Resize(FlagList.GetElementCount());
	for (int_max k = 0; k < FlagList.GetElementCount(); ++k)
	{
		if (FlagList[k] > 0.1)
		{
			Remesher.m_QuadCandidateFlagList[k] = 1;
		}
		else
		{
			Remesher.m_QuadCandidateFlagList[k] = 0;
		}
	}

	Remesher.BuildOutputMesh();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh, TestDataPath + "outputmesh_quad.vtk");
}