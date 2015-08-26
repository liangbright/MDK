#include "mdkDenseMatrix_FileIO.h"
#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"

void Test_Matrix()
{
    using namespace mdk;

    DenseMatrix<double> A = { { 1, 2, 3, 4, 5 },
                              { 5, 4, 0, 2, 1 } };

    DenseMatrix<double> B = { { 6, 7, 8, 9, 10 },
                              { 1, 2, 3, 4, 5 } };

    DenseMatrix<double> C = { &A, &B };

    DisplayMatrix("C", C);
}

void Test_plus_space_plus()
{
    int a = 1;

    int b = 2;

    int c = a + b + + 1;
}

void Test_SimpleCase()
{
    using namespace mdk;

    String FilePath = "C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/TestData/SimpleCase/";

    String FeatureDataFilePathAndName = FilePath + "DataSample.json";

	DenseMatrix<double> FeatureData;
	LoadDenseMatrixFromJsonDataFile(FeatureData, FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColCount();

    KNNBasisSelectionOnlineDictionaryBuilder<double> DictionaryBuilder;

    int_max NeighbourNumber = 5;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::L1Distance;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = 10;
    
    DictionaryBuilder.m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
    DictionaryBuilder.m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne = false;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    DictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.m_Parameter.MaxNumberOfThread = 4;

    DictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    DictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    DisplayMatrix("D", DictionaryPtr->BasisMatrix(), 2);

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "BasisMatrix.json");
    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance.json");

}


void Test_GaussianObjectImage()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/TestData/GaussianObjectImage/";

    std::string FeatureDataFilePathAndName = FilePath + "GaussianObjectImage.json";

	DenseMatrix<double> FeatureData;
	LoadDenseMatrixFromJsonDataFile(FeatureData, FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColCount();

    KNNBasisSelectionOnlineDictionaryBuilder<double> DictionaryBuilder;

    int_max NeighbourNumber = 3;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::L2Distance;
    
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.01;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = 0.287;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 1;

    DictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0.5;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.m_Parameter.MaxNumberOfThread = 1;

    DictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    DictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix.json");
//    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance.json");

}
