#pragma once

#include "SurfaceRemesher3.h"
#include "mdkTextFile.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_a1()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/TestData/";

	SurfaceRemesher3<double> Remesher;
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

	TextFile OutputFile_b(TestDataPath + "candidate_conflict_table_TJunction.txt");
	OutputFile_b.Clear();
	for (int_max k = 0; k < Remesher.m_CandidateConflictTable_TJunction.GetLength(); ++k)
	{
		const auto& List = Remesher.m_CandidateConflictTable_TJunction[k];

		for (int_max n = 0; n < List.GetLength(); ++n)
		{
			OutputFile_b << std::to_string(List[n]);
			if (n < List.GetLength() - 1)
			{
				OutputFile_b << ", ";
			}
		}
		if (k < Remesher.m_CandidateConflictTable_TJunction.GetLength() - 1)
		{
			OutputFile_b << '\n';
		}
	}
	OutputFile_b.Close();

	TextFile OutputFile_c(TestDataPath + "candidate_indicator.txt");
	OutputFile_c.Clear();
	for (int_max k = 0; k < Remesher.m_CandidateIndicatorList.GetLength(); ++k)
	{
		OutputFile_c << std::to_string(Remesher.m_CandidateIndicatorList[k]);
		if (k < Remesher.m_CandidateScoreList.GetLength() - 1)
		{
			OutputFile_c << '\n';
		}
	}
	OutputFile_c.Close();

	TextFile OutputFile_d(TestDataPath + "QuadCandidateIndexSet_At_SmallTriangle.txt");
	OutputFile_d.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidateIndexSet_At_SmallTriangle.GetLength(); ++k)
	{
		const auto& Set = Remesher.m_QuadCandidateIndexSet_At_SmallTriangle[k];
		for (int_max n = 0; n < Set.GetLength(); ++n)
		{
			OutputFile_d << std::to_string(Set[n]);
			if (n < Set.GetLength() - 1)
			{
				OutputFile_d << ", ";
			}
		}
		if (k < Remesher.m_QuadCandidateIndexSet_At_SmallTriangle.GetLength() - 1)
		{
			OutputFile_d << '\n';
		}
	}
	OutputFile_d.Close();

	TextFile OutputFile_e(TestDataPath + "QuadCandidateIndexSet_At_BigTriangle.txt");
	OutputFile_e.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidateIndexSet_At_BigTriangle.GetLength(); ++k)
	{
		const auto& Set = Remesher.m_QuadCandidateIndexSet_At_BigTriangle[k];
		for (int_max n = 0; n < Set.GetLength(); ++n)
		{
			OutputFile_e << std::to_string(Set[n]);
			if (n < Set.GetLength() - 1)
			{
				OutputFile_e << ", ";
			}
		}
		if (k < Remesher.m_QuadCandidateIndexSet_At_BigTriangle.GetLength() - 1)
		{
			OutputFile_e << '\n';
		}
	}
	OutputFile_e.Close();


}

void Test_a2()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/TestData/";

	SurfaceRemesher3<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.InitilizeCandidateMesh();
	Remesher.GenerateCandidate();
	Remesher.EvaluateCandidate();
	
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
	
	Remesher.BuildOutputMesh();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh, TestDataPath + "outputmesh.vtk");
}