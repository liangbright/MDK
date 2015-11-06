#pragma once

#include "SurfaceRemesher1.h"
#include "SurfaceRemesher2.h"
#include "SurfaceRemesher3.h"
#include "mdkDenseMatrix_FileIO.h"

using namespace mdk;

void test_a()
{
	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/TestData/";
	
	SurfaceRemesher3<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.InitilizeCandidateMesh();
	Remesher.GenerateCandidate();
	Remesher.EvaluateCandidate();
	auto CandidateCount = Remesher.m_CandidateMesh.GetFaceCount();
	DenseMatrix<int> Constraint(CandidateCount, CandidateCount);
	DenseMatrix<double> ScoreList(CandidateCount, 1);
	Constraint.Fill(0);
	for (int_max k = 0; k < CandidateCount; ++k)
	{
		Constraint(k, k) = 1;
		for (int_max n = 0; n < Remesher.m_CandidateConflictTable[k].GetLength(); ++n)
		{
			Constraint(k, Remesher.m_CandidateConflictTable[k][n]) = 1;
		}
		ScoreList[k] = Remesher.m_CandidateScoreList[k];
	}
	SaveDenseMatrixAsJsonDataFile(Constraint, TestDataPath + "Constraint.json");
	SaveDenseMatrixAsJsonDataFile(ScoreList, TestDataPath + "ScoreList.json");
}

void test_b()
{
	String TestDataPath = "C:/Research/Mesh/Remeshing/Build/TestData/";

	SurfaceRemesher2<double> Remesher;
	LoadTriangleMeshFromVTKFile(Remesher.m_InputMesh, TestDataPath + "inputmesh.vtk");
	Remesher.FindMiddlePointOfEdgeOfInputMesh();
	Remesher.GenerateCandidate();
	Remesher.EvaluateCandidate();

	DenseMatrix<double> IndicatorList;
	LoadDenseMatrixFromJsonDataFile(IndicatorList, TestDataPath + "IndicatorList.json");
	Remesher.m_CandidateIndicatorList.Resize(IndicatorList.GetElementCount());
	for (int_max k = 0; k < IndicatorList.GetElementCount(); ++k)
	{
		if (IndicatorList[k]>0.1)
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
