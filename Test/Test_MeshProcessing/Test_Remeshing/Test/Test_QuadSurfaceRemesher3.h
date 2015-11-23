#pragma once

#include "QuadSurfaceRemesher3.h"
#include "mdkTextFile.h"
#include "mdkDenseMatrix_FileIO.h"

void Test_a1()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/TestData/";

	QuadSurfaceRemesher3<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.Update();
	SavePolygonMeshAsVTKFile(Remesher.m_CandidateMesh, TestDataPath + "QuadSurfaceRemesher3_CandidateMesh_greedy.vtk");
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh_Mixed, TestDataPath + "QuadSurfaceRemesher3_OutputMesh_mixed_greedy.vtk");

	TextFile OutputFile_a(TestDataPath + "candidate_score.txt");
	OutputFile_a.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidatePairScoreList.GetLength(); ++k)
	{
		OutputFile_a << std::to_string(Remesher.m_QuadCandidatePairScoreList[k]);
		if (k < Remesher.m_QuadCandidatePairScoreList.GetLength() - 1)
		{
			OutputFile_a << '\n';
		}
	}
	OutputFile_a.Close();

	TextFile OutputFile_b(TestDataPath + "candidate_conflict_table.txt");
	OutputFile_b.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidatePairConflictTable.GetLength(); ++k)
	{
		const auto& List = Remesher.m_QuadCandidatePairConflictTable[k];

		for (int_max n = 0; n < List.GetLength(); ++n)
		{
			OutputFile_b << std::to_string(List[n]);
			if (n < List.GetLength() - 1)
			{
				OutputFile_b << ", ";
			}
		}
		if (k < Remesher.m_QuadCandidatePairConflictTable.GetLength() - 1)
		{
			OutputFile_b << '\n';
		}
	}
	OutputFile_b.Close();

	TextFile OutputFile_c(TestDataPath + "candidate_indicator.txt");
	OutputFile_c.Clear();
	for (int_max k = 0; k < Remesher.m_QuadCandidatePairIndicatorList.GetLength(); ++k)
	{
		OutputFile_c << std::to_string(Remesher.m_QuadCandidatePairIndicatorList[k]);
		if (k < Remesher.m_QuadCandidatePairIndicatorList.GetLength() - 1)
		{
			OutputFile_c << '\n';
		}
	}
	OutputFile_c.Close();
}

void Test_a2()
{
	using namespace mdk;

	String TestDataPath = "C:/Research/Mesh/Remeshing/TestData/";

	QuadSurfaceRemesher3<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.InitializeCandidateMesh();
	Remesher.BuildSmallTriangleMesh();
	Remesher.GenerateCandidate();

	DenseMatrix<double> FlagList;
	LoadDenseMatrixFromJsonDataFile(FlagList, TestDataPath + "FlagList.json");

	Remesher.m_QuadCandidatePairIndicatorList.Resize(FlagList.GetElementCount());
	for (int_max k = 0; k < FlagList.GetElementCount(); ++k)
	{
		if (FlagList[k] > 0.1)
		{
			Remesher.m_QuadCandidatePairIndicatorList[k] = 1;
		}
		else
		{
			Remesher.m_QuadCandidatePairIndicatorList[k] = 0;
		}
	}

	Remesher.BuildCandidateMesh();
	Remesher.BuildMixedTriQuadMesh();

	SavePolygonMeshAsVTKFile(Remesher.m_CandidateMesh, TestDataPath + "QuadSurfaceRemesher3_CandidateMesh.vtk");
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh_Mixed, TestDataPath + "QuadSurfaceRemesher3_outputmesh_mixed.vtk");

	for (auto it = Remesher.m_CandidateMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnEdge() == false)
		{
			Remesher.m_CandidateMesh.DeletePoint(it.GetPointHandle());
		}
	}
	Remesher.m_CandidateMesh.CleanDataStructure();
	SavePolygonMeshAsVTKFile(Remesher.m_CandidateMesh, TestDataPath + "QuadSurfaceRemesher3_CandidateMesh_clean.vtk");

	for (auto it = Remesher.m_OutputMesh_Mixed.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().IsOnEdge() == false)
		{
			Remesher.m_OutputMesh_Mixed.DeletePoint(it.GetPointHandle());
		}
	}
	//Remesher.m_OutputMesh_Mixed.CleanDataStructure();
	SavePolygonMeshAsVTKFile(Remesher.m_OutputMesh_Mixed, TestDataPath + "QuadSurfaceRemesher3_outputmesh_mixed_clean.vtk");

}


void Test_QuadSurfaceRemesher3()
{
	//Test_a1();
	Test_a2();
}
