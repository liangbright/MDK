#include "mdkPolygonMeshProcessing.h"
#include "mdkDistanceMinimizationBasedShapeAligner3D.h"

using namespace mdk;

void test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_DistanceMinimizationBasedShapeAligner/TestData/";
	
	int_max ShapeCount = 125;

	ObjectArray<DenseMatrix<double>> ShapeList;
	ShapeList.Resize(ShapeCount);
	PolygonMesh<PolygonMeshEmptyAttributeType<double>> AortaMesh;
	for (int_max k = 0; k < ShapeCount; ++k)
	{	
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg_rand.vtk");
		ShapeList[k] = AortaMesh.GetPointPosition(ALL);
	}
	std::cout << "done read mesh" << '\n';

	DistanceMinimizationBasedShapeAligner3D<double> ShapeAligner;
	
	ObjectArray<SparseVector<double>> SimilarityTable;
	SimilarityTable = ShapeAligner.ComputeSimilarityBetweenShape(ShapeList, false, 8);
	std::cout << "done ComputeSimilarityBetweenShape" << '\n';

	ShapeAligner.SetInputShapeList(&ShapeList);
	ShapeAligner.SetInputSimilarityTable(&SimilarityTable);
	ShapeAligner.SelectRigidTransform();
	ShapeAligner.SetMaxNeighbourCount(5);
	ShapeAligner.SetMaxIterCount(10);
	ShapeAligner.EnableObjectiveFunctionEvaluation();
	ShapeAligner.EnableParallelUpdateTransform();
	ShapeAligner.SetMaxThreadCount(8);
	ShapeAligner.Update();
	auto& AlignedShapeList = ShapeAligner.OutputShapeList();
	std::cout << "done DistanceMinimizationBasedShapeAligner3D" << '\n';

	for (int_max k = 0; k < ShapeCount; ++k)
	{
		AortaMesh.SetPointPosition(ALL, AlignedShapeList[k]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg_align.vtk");		
	}
	
	DisplayVector("ObjectiveFunctionValue", ShapeAligner.GetObjectiveFunctionValue(), 6);
}