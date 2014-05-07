#include "mdkFileIO.h"
#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNReconstructionOnlineDictionaryBuilder.h"

void Test_SimpleCase()
{
    using namespace mdk;

    CharString FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionOnlineDictionaryBuilder/Debug/";

    CharString FeatureDataFilePathAndName = FilePath + "DataSample.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max NeighbourNumber = 5;

    //-------------------------------------------------------------------------------------------

    KNNBasisSelectionOnlineDictionaryBuilder<double> InitialDictionaryBuilder;

    InitialDictionaryBuilder.m_Parameter.BasisNumber = 10;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;

    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::L1Distance;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = 10;

    InitialDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    InitialDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    InitialDictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    InitialDictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    InitialDictionaryBuilder.SetInputFeatureData(&FeatureData);

    InitialDictionaryBuilder.Update();

    auto InitialDictionaryPtr = InitialDictionaryBuilder.GetOutputDictionary();

    DisplayMatrix("D_init", InitialDictionaryPtr->BasisMatrix(), 2);

    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->BasisMatrix(), FilePath + "BasisMatrix_init.json");
    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance_init.json");

    //--------------------------------------------------------------------------------------------

    KNNReconstructionOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.SimilarityType = VectorSimilarityTypeEnum::Correlation;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative = true;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne = false;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetInitialDictionary(InitialDictionaryPtr);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    DisplayMatrix("D", DictionaryPtr->BasisMatrix(), 2);

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "BasisMatrix.json");
    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance.json");

}


void Test_GaussianObjectImage()
{
    using namespace mdk;

    CharString FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionOnlineDictionaryBuilder/Debug/";

    CharString FeatureDataFilePathAndName = FilePath + "GaussianObjectImage.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max NeighbourNumber = 5;

    auto SimilarityType = VectorSimilarityTypeEnum::L2Distance;

    double Variance_L1 = 1;

    double Variance_L2 = 1;

    int_max BasisNumber = 10;

    //-------------------------------------------------------------------------------------------

    KNNBasisSelectionOnlineDictionaryBuilder<double> InitialDictionaryBuilder;

    InitialDictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;

    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = Variance_L1;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = Variance_L2;

    InitialDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    InitialDictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0.5;

    InitialDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    InitialDictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    InitialDictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    InitialDictionaryBuilder.SetInputFeatureData(&FeatureData);

    InitialDictionaryBuilder.Update();

    auto InitialDictionaryPtr = InitialDictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix_init.json");
    //SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance_init.json");

    //--------------------------------------------------------------------------------------------

    KNNReconstructionOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;

    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber = 3;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.SimilarityType = SimilarityType;

    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative = true;
    DictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne    = false;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.00000001;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetInitialDictionary(InitialDictionaryPtr);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix.json");
    //SaveDenseMatrixAsJsonDataFile(DictionaryPtr->VarianceOfL1Distance(), FilePath + "VarianceOfL1Distance.json");

}