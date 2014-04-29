#include "mdkFileIO.h"
#include "mdkKNNSoftAssignOnlineDictionaryBuilder.h"

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

void Test_SimpleCase()
{
    using namespace mdk;

    CharString FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNSoftAssignOnlineDictionaryBuiler/Debug/";

    CharString FeatureDataFilePathAndName = FilePath + "DataSample.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    KNNSoftAssignOnlineDictionaryBuilder<double> DictionaryBuilder;

    int_max NeighbourNumber = 5;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = "L1Distance";
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.0;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1 = 20;
    
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative = false;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne = false;

    DictionaryBuilder.m_Parameter.weigth_s = 1;
    DictionaryBuilder.m_Parameter.weigth_past = 0;

    DictionaryBuilder.m_Parameter.NumberOfDataInEachBatch = 5;
    DictionaryBuilder.m_Parameter.MaxNumberOfIteration = 1000;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    DisplayMatrix("D", DictionaryPtr->BasisMatrix());

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "BasisMatrix.json");
    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->StandardDeviationOfL1Distance(), FilePath + "L1DistanceStd.json");

}