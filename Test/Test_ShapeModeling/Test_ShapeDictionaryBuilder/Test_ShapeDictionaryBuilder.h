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
	DictionaryBuilder.Parameter().MiniBatchSize = 125;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&ShapeList);
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.OutputDictionary();
	std::cout << "done build dictionary" << '\n';

	SaveDenseMatrixAsJsonDataFile(Dictionary.BasisSimilarity(), TestDataPath + "BasisSimilarity_select.json");

	KNNSoftAssignBasedSparseShapeEncoder<double> Encoder;
	Encoder.SetInputDictionary(&Dictionary.Basis());
	Encoder.SetInputShapeData(&ShapeList);
	Encoder.Parameter().MaxNeighbourCount = 1;
	Encoder.Parameter().MaxThreadCount = 8;
	Encoder.Parameter().SimilarityThreshold = 0;
	Encoder.Parameter().TransformName= "SimilarityTransform";
	Encoder.Update();
	auto& Code = Encoder.OutputSimilarityCode();

	DenseVector<int_max> ShapeIndexList_Basis;
	ShapeIndexList_Basis.SetCapacity(Dictionary.GetBasisCount());
	for (int_max k = 0; k < Code.GetLength(); ++k)
	{
		if (Code[k].IndexList()[0] >= 0.9999999999999)
		{
			ShapeIndexList_Basis.Append(k);
		}
	}
	ShapeIndexList_Basis.SortInPlace("ascend");


	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, ShapeList[ShapeIndexList_Basis[k]]);
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

	std::cout << "done read mesh" << '\n';

	KNNBasisSelectionBasedShapeDictionaryBuilder<double> DictionaryBuilder;
	DictionaryBuilder.Parameter().BasisCount = 10;
	DictionaryBuilder.Parameter().MaxNeighbourCount = 5;	
	DictionaryBuilder.Parameter().ExperienceDiscountFactor = 0.5;
	DictionaryBuilder.Parameter().TransformName = "SimilarityTransform";
	DictionaryBuilder.Parameter().MaxEpochCount = 1;
	DictionaryBuilder.Parameter().MiniBatchSize = 125;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&TrainingShapeList);	
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.OutputDictionary();
	std::cout << "done build dictionary" << '\n';

	KNNSoftAssignBasedSparseShapeEncoder<double> Encoder;
	Encoder.SetInputDictionary(&Dictionary.Basis());
	Encoder.SetInputShapeData(&TrainingShapeList);
	Encoder.Parameter().MaxNeighbourCount = 1;
	Encoder.Parameter().MaxThreadCount = 8;
	Encoder.Parameter().SimilarityThreshold = 0;
	Encoder.Parameter().TransformName = "SimilarityTransform";
	Encoder.Update();
	auto& Code = Encoder.OutputSimilarityCode();

	DenseVector<int_max> ShapeIndexList_Basis;
	ShapeIndexList_Basis.SetCapacity(Dictionary.GetBasisCount());
	for (int_max k = 0; k < Code.GetLength(); ++k)
	{
		if (Code[k].IndexList()[0] >= 0.9999999999999)
		{
			ShapeIndexList_Basis.Append(k);
		}
	}
	ShapeIndexList_Basis.SortInPlace("ascend");

	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, TrainingShapeList[ShapeIndexList_Basis[k]]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_knn_select.vtk");
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

	ShapeDictionary<double> Dictionary_init;
	Dictionary_init.Initialize(std::move(BasisShape));

	std::cout << "done read mesh" << '\n';	

	KNNAverageBasedShapeDictionaryBuilder<double> DictionaryBuilder;	
	DictionaryBuilder.Parameter().MaxNeighbourCount = 5;
	DictionaryBuilder.Parameter().SimilarityThreshold = 0.3;
	DictionaryBuilder.Parameter().ExperienceDiscountFactor = 0.5;
	DictionaryBuilder.Parameter().TransformName = "SimilarityTransform";
	DictionaryBuilder.Parameter().MaxEpochCount = 1;
	DictionaryBuilder.Parameter().MiniBatchSize = 125;
	DictionaryBuilder.Parameter().MaxThreadCount = 8;
	DictionaryBuilder.Parameter().Debug_Flag = true;
	DictionaryBuilder.Parameter().Debug_FilePath = TestDataPath;
	DictionaryBuilder.SetTrainingShapeData(&TrainingShapeList);
	DictionaryBuilder.SetInitialDictionary(&Dictionary_init);
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.OutputDictionary();
	std::cout << "done build dictionary" << '\n';

	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{		
		AortaMesh.SetPointPosition(ALL, Dictionary.Basis()[k]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_avg.vtk");
	}
}
