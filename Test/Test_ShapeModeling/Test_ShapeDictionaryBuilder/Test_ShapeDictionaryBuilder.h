#include "mdkLinearAlgebra.h"
#include "mdkPolygonMeshProcessing.h"
#include "mdkBasisSelectionBasedShapeDictionaryBuilder.h"
#include "mdkKNNBasisSelectionBasedShapeDictionaryBuilder.h"
#include "mdkKNNAverageBasedShapeDictionaryBuilder.h"

using namespace mdk;

void test_a()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/";

	ObjectArray<DenseMatrix<double>> ShapeList;
	ShapeList.SetCapacity(125);
	PolygonMesh<PolygonMeshEmptyAttributeType<double>> AortaMesh;
	for (int_max k = 0; k < 125; ++k)
	{
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg.vtk");
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		/*
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		ShapeList.Append(AortaMesh.GetPointPosition(ALL));
		*/

	}
	std::cout << "done read mesh" << '\n';

	BasisSelectionBasedShapeDictionaryBuilder<double> DictionaryBuilder;
	DictionaryBuilder.Parameter().BasisCount = 10;
	DictionaryBuilder.Parameter().TransformName = "SimilarityTransform";
	DictionaryBuilder.Parameter().MaxEpochCount = 1;
	DictionaryBuilder.Parameter().MiniBatchSize = 12;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&ShapeList);
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.OutputDictionary();
	std::cout << "done build dictionary" << '\n';

	SaveDenseMatrixAsJsonDataFile(Dictionary.BasisSimilarity(), TestDataPath + "BasisSimilarity_select.json");

	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, Dictionary.Basis()[k]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_select.vtk");
	}
}

void test_b()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/";

	PolygonMesh<PolygonMeshEmptyAttributeType<double>> AortaMesh;

	ObjectArray<DenseMatrix<double>> TrainingShapeList;
	TrainingShapeList.SetCapacity(125);
	for (int_max k = 0; k < 125; ++k)
	{
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg.vtk");
		TrainingShapeList.Append(AortaMesh.GetPointPosition(ALL));
	}

	ShapeDictionary<double> Dictioanry_init;
	Dictioanry_init.Initialize(TrainingShapeList);

	std::cout << "done read mesh" << '\n';

	KNNBasisSelectionBasedShapeDictionaryBuilder<double> DictionaryBuilder;
	DictionaryBuilder.Parameter().BasisCount = 10;
	DictionaryBuilder.Parameter().MaxNeighbourCount = 5;
	DictionaryBuilder.Parameter().SimilarityThreshold = 0.3;
	DictionaryBuilder.Parameter().ExperienceDiscountFactor = 0.2;
	DictionaryBuilder.Parameter().TransformName = "SimilarityTransform";
	DictionaryBuilder.Parameter().MaxEpochCount = 1;
	DictionaryBuilder.Parameter().MiniBatchSize = 125;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&TrainingShapeList);
	DictionaryBuilder.SetInitialDictionary(&Dictioanry_init);
	DictionaryBuilder.Update();
	auto& SelectedBasisIndexList = DictionaryBuilder.OutputSelectedBasisIndexList();
	std::cout << "done build dictionary" << '\n';

	for (int_max k = 0; k < SelectedBasisIndexList.GetLength(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, Dictioanry_init.Basis()[SelectedBasisIndexList[k]]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_select2.vtk");
	}
}


void test_c()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/";

	PolygonMesh<PolygonMeshEmptyAttributeType<double>> AortaMesh;

	ObjectArray<DenseMatrix<double>> TrainingShapeList;
	TrainingShapeList.SetCapacity(125);	
	for (int_max k = 0; k < 125; ++k)
	{	
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg.vtk");
		TrainingShapeList.Append(AortaMesh.GetPointPosition(ALL));
	}

	ObjectArray<DenseMatrix<double>> BasisShape;
	BasisShape.SetCapacity(10);
	for (int_max k = 0; k < 10; ++k)
	{		
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_select.vtk");
		BasisShape.Append(AortaMesh.GetPointPosition(ALL));
	}

	ShapeDictionary<double> Dictioanry_init;
	Dictioanry_init.Initialize(std::move(BasisShape));

	std::cout << "done read mesh" << '\n';	

	KNNAverageBasedShapeDictionaryBuilder<double> DictionaryBuilder;	
	DictionaryBuilder.Parameter().MaxNeighbourCount = 5;
	DictionaryBuilder.Parameter().SimilarityThreshold = 0.3;
	DictionaryBuilder.Parameter().ExperienceDiscountFactor = 0.2;
	DictionaryBuilder.Parameter().TransformName = "SimilarityTransform";
	DictionaryBuilder.Parameter().MaxEpochCount = 1;
	DictionaryBuilder.Parameter().MiniBatchSize = 125;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&TrainingShapeList);
	DictionaryBuilder.SetInitialDictionary(&Dictioanry_init);
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.OutputDictionary();
	std::cout << "done build dictionary" << '\n';

	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{		
		AortaMesh.SetPointPosition(ALL, Dictionary.Basis()[k]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_avg.vtk");
	}
}
