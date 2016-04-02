#include "mdkLinearAlgebra.h"
#include "mdkPolygonMeshProcessing.h"
#include "mdkKNNBasisSelectionOnlineShapeDictionaryBuilder.h"

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
	ShapeAligner.SetMaxNeighbourCount(10);
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

	DenseMatrix<double> SimilarityTable_output;
	SimilarityTable_output.Resize(ShapeCount, ShapeCount);
	for (int_max k = 0; k < ShapeCount; ++k)
	{
		for (int_max n = 0; n < ShapeCount; ++n)
		{
			SimilarityTable_output(k, n) = SimilarityTable[k][n];
		}
	}
	SaveDenseMatrixAsJsonDataFile(SimilarityTable_output, TestDataPath + "SimilarityTable.json");

	for (int_max k = 0; k < ShapeCount; ++k)
	{
		auto R = ShapeAligner.OutputTransformList()[k].Rotation;
		auto S = ShapeAligner.OutputTransformList()[k].Scale;
		auto T= ShapeAligner.OutputTransformList()[k].Translation;

		RigidTransform3D<double> Transform;
		Transform.SetRotationMatrix(R);
		Transform.SetTranslation_AfterRotation(T);
		auto  AlignedShape = Transform.TransformPoint(ShapeList[k]);
		AortaMesh.SetPointPosition(ALL, AlignedShape);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg_align_from_transfrom.vtk");
	}
}