#pragma once

#include "SurfaceRemesher2.h"
#include "mdkTextFile.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_a1()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceRemesher2/TestData/";

	SurfaceRemesher2<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.InitilizeCandidateMesh();
	Remesher.GenerateCandidate();	
	Remesher.EvaluateCandidate();

	TextFile OutputFile_a(TestDataPath + "candidate_score.txt");
	OutputFile_a.Clear();
	for (int_max k = 0; k < Remesher.m_CandidateScoreList.GetLength(); ++k)
	{
		OutputFile_a << std::to_string(Remesher.m_CandidateScoreList[k]);	
		if (k < Remesher.m_CandidateScoreList.GetLength() - 1)
		{
			OutputFile_a << '\n';
		}
	}
	OutputFile_a.Close();

	TextFile OutputFile_b(TestDataPath + "candidate_conflict_table.txt");
	OutputFile_b.Clear();
	for (int_max k = 0; k < Remesher.m_CandidateConflictTable.GetLength(); ++k)
	{
		for (int_max n = 0; n < Remesher.m_CandidateConflictTable[k].GetLength(); ++n)
		{
			OutputFile_b << std::to_string(Remesher.m_CandidateConflictTable[k][n]);
			if (n < Remesher.m_CandidateConflictTable[k].GetLength() - 1)
			{
				OutputFile_b << " ";
			}
		}
		if (k < Remesher.m_CandidateConflictTable.GetLength() - 1)
		{
			OutputFile_b << '\n';
		}
	}
	OutputFile_b.Close();
}

void Test_a2()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/Test_SurfaceRemesher2/TestData/";

	SurfaceRemesher2<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.InitilizeCandidateMesh();
	Remesher.GenerateCandidate();
	Remesher.EvaluateCandidate();
	Remesher.SelectCandidate();
	Remesher.BuildOutputMesh();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh, TestDataPath + "outputmesh_greedy.vtk");
	/*
	DenseMatrix<double> FlagList;
	LoadDenseMatrixFromJsonDataFile(FlagList, TestDataPath + "FlagList.json");

	Remesher.m_CandidateIndicatorList.Resize(FlagList.GetElementCount());
	for (int_max k = 0; k < FlagList.GetElementCount(); ++k)
	{
		if (FlagList[k] > 0.1)
		{
			Remesher.m_CandidateIndicatorList[k] = 1;
		}
		else
		{
			Remesher.m_CandidateIndicatorList[k] = 0;
		}
	}
	*/
	Remesher.BuildOutputMesh();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh, TestDataPath + "outputmesh.vtk");
}