#include "mdkFileIO.h"
#include "mdkKNNBasisSelectMergeOnlineDictionaryBuilder.h"


void Test_GaussianObjectImage()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectMergeOnlineDictionaryBuilder/Debug/";

    std::string FeatureDataFilePathAndName = FilePath + "GaussianObjectImage.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    KNNBasisSelectMergeOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = 3;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::L2Distance;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = 0.287;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = 0.287;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 1;

    DictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0.0;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.m_Parameter.MaxNumberOfThread = 1;

    DictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    DictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtrA = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtrA->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix_init.json");
    //----------------------------------------------------------------------------------------------------------------------
    
    DictionaryBuilder.m_Parameter.BasisNumber = 10;

    DictionaryBuilder.SetInitialDictionary(DictionaryPtrA);

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtrB = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtrB->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix.json");
}
