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
	PolygonMesh<double> AortaMesh;
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
	DictionaryBuilder.Input.Parameter.BasisCount = 10;
	DictionaryBuilder.Input.Parameter.TransformName = "SimilarityTransform";
	DictionaryBuilder.Input.Parameter.MaxEpochCount = 1;
	DictionaryBuilder.Input.Parameter.BatchSize = 125;
	DictionaryBuilder.Input.Parameter.MaxThreadCount = 8;
	DictionaryBuilder.Input.Parameter.Debug_Flag = true;
	DictionaryBuilder.Input.Parameter.Debug_FilePath = TestDataPath;
	DictionaryBuilder.Input.TrainingShapeData = &ShapeList;
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.Output.Dictionary;
	std::cout << "done build dictionary" << '\n';

	SaveDenseMatrixAsJsonDataFile(Dictionary.BasisSimilarity(), TestDataPath + "BasisSimilarity_select.json");

	KNNSoftAssignBasedSparseShapeEncoder<double> Encoder;
	Encoder.Input.Dictionary = &Dictionary.Basis();
	Encoder.Input.ShapeData = &ShapeList;
	Encoder.Input.Parameter.MaxNeighborCount = 1;
	Encoder.Input.Parameter.MaxThreadCount = 8;
	Encoder.Input.Parameter.SimilarityThreshold = 0;
	Encoder.Input.Parameter.TransformName= "RigidTransform";
	Encoder.Update();
	auto& Code = Encoder.Output.SimilarityCode;

	DenseVector<int_max> ShapeIndexList_Basis;
	ShapeIndexList_Basis.SetCapacity(Dictionary.GetBasisCount());
	for (int_max k = 0; k < Code.GetLength(); ++k)
	{
		if (Code[k].ElementList()[0] >= 0.9999999999999)
		{
			ShapeIndexList_Basis.Append(k);
		}
	}
	ShapeIndexList_Basis.SortInPlace("ascend");

	for (int_max k = 0; k < ShapeIndexList_Basis.GetLength(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, ShapeList[ShapeIndexList_Basis[k]]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_select.vtk");
	}
}

void test_b()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/";

	PolygonMesh<double> AortaMesh;

	ObjectArray<DenseMatrix<double>> TrainingShapeList;
	TrainingShapeList.SetCapacity(125);
	for (int_max k = 0; k < 125; ++k)
	{
		LoadPolygonMeshFromVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Pimg.vtk");
		TrainingShapeList.Append(AortaMesh.GetPointPosition(ALL));
	}

	std::cout << "done read mesh" << '\n';

	KNNBasisSelectionBasedShapeDictionaryBuilder<double> DictionaryBuilder;
	DictionaryBuilder.Input.Parameter.BasisCount = 10;
	DictionaryBuilder.Input.Parameter.MaxNeighborCount = 5;	
	DictionaryBuilder.Input.Parameter.ExperienceDiscountFactor = 0.5;
	DictionaryBuilder.Input.Parameter.TransformName = "SimilarityTransform";
	DictionaryBuilder.Input.Parameter.MaxEpochCount = 1;
	DictionaryBuilder.Input.Parameter.BatchSize = 125;
	DictionaryBuilder.Input.Parameter.MaxThreadCount = 8;
	DictionaryBuilder.Input.Parameter.Debug_Flag = true;
	DictionaryBuilder.Input.Parameter.Debug_FilePath = TestDataPath;
	DictionaryBuilder.Input.TrainingShapeData = &TrainingShapeList;
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.Output.Dictionary;
	std::cout << "done build dictionary" << '\n';

	KNNSoftAssignBasedSparseShapeEncoder<double> Encoder;
	Encoder.Input.Dictionary = &Dictionary.Basis();
	Encoder.Input.ShapeData = &TrainingShapeList;
	Encoder.Input.Parameter.MaxNeighborCount = 1;
	Encoder.Input.Parameter.MaxThreadCount = 8;
	Encoder.Input.Parameter.SimilarityThreshold = 0;
	Encoder.Input.Parameter.TransformName = "RigidTransform";
	Encoder.Update();
	auto& Code = Encoder.Output.SimilarityCode;

	DenseVector<int_max> ShapeIndexList_Basis;
	ShapeIndexList_Basis.SetCapacity(Dictionary.GetBasisCount());
	for (int_max k = 0; k < Code.GetLength(); ++k)
	{
		if (Code[k].ElementList()[0] >= 0.9999999999999)
		{
			ShapeIndexList_Basis.Append(k);
		}
	}
	ShapeIndexList_Basis.SortInPlace("ascend");

	for (int_max k = 0; k < ShapeIndexList_Basis.GetLength(); ++k)
	{
		AortaMesh.SetPointPosition(ALL, TrainingShapeList[ShapeIndexList_Basis[k]]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_knn_select.vtk");
	}
}


void test_c()
{
	String TestDataPath = "C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/";

	PolygonMesh<double> AortaMesh;

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
	DictionaryBuilder.Input.Parameter.MaxNeighborCount = 5;
	DictionaryBuilder.Input.Parameter.SimilarityThreshold = 0.3;
	DictionaryBuilder.Input.Parameter.ExperienceDiscountFactor = 0.5;
	DictionaryBuilder.Input.Parameter.TransformName = "SimilarityTransform";
	DictionaryBuilder.Input.Parameter.MaxEpochCount = 1;
	DictionaryBuilder.Input.Parameter.BatchSize = 125;
	DictionaryBuilder.Input.Parameter.MaxThreadCount = 8;
	DictionaryBuilder.Input.Parameter.Debug_Flag = true;
	DictionaryBuilder.Input.Parameter.Debug_FilePath = TestDataPath;
	DictionaryBuilder.Input.TrainingShapeData = &TrainingShapeList;
	DictionaryBuilder.Input.InitialDictionary = &Dictionary_init;
	DictionaryBuilder.Update();
	auto& Dictionary = DictionaryBuilder.Output.Dictionary;
	std::cout << "done build dictionary" << '\n';

	for (int_max k = 0; k < Dictionary.GetBasisCount(); ++k)
	{		
		AortaMesh.SetPointPosition(ALL, Dictionary.Basis()[k]);
		SavePolygonMeshAsVTKFile(AortaMesh, TestDataPath + std::to_string(k) + "_AortaModel_Basis_avg.vtk");
	}
}
