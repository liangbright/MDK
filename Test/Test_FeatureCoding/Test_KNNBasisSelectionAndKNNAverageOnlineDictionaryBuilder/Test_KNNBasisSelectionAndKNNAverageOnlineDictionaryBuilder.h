#include "mdkFileIO.h"
#include "mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder.h"


void Test_GaussianObjectImage()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBSABasedOnlineDictionaryBuilder/Debug/";

    std::string FeatureDataFilePathAndName = FilePath + "GaussianObjectImage.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max NeighbourNumber = 5;

    auto SimilarityType = VectorSimilarityTypeEnum::L2Distance;

    double Variance_L1 = 1;

    double Variance_L2 = 1;

    int_max BasisNumber = 10;

    //-------------------------------------------------------------------------------------------

    KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = Variance_L1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = Variance_L2;

    DictionaryBuilder.m_Parameter.ParameterOfKNNAverage.ExperienceDiscountFactor = 0;
    
    DictionaryBuilder.m_Parameter.ParameterOfKNNBasisSelection.ExperienceDiscountFactor = 1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection = 0.5;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = BasisNumber*NeighbourNumber;

    DictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    DictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix.json");
}


void Test_ImageDenoising()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBSABasedOnlineDictionaryBuilder/Debug/";

    std::string FeatureDataFilePathAndName = FilePath + "NoisyImagePatch.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max NeighbourNumber = 3;

    auto SimilarityType = VectorSimilarityTypeEnum::L2Distance;

    double SimilarityThreshold = 0.1;

    double Variance_L1 = 1;

    double Variance_L2 = 1;

    int_max BasisNumber = 128;

    int_max MaxNumberOfThread = 4;

    //-------------------------------------------------------------------------------------------

	KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = Variance_L1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = Variance_L2;

    DictionaryBuilder.m_Parameter.ParameterOfKNNAverage.ExperienceDiscountFactor = 0;
    DictionaryBuilder.m_Parameter.ParameterOfKNNAverage.WhetherToUseScaleFactor  = false;

    DictionaryBuilder.m_Parameter.ParameterOfKNNBasisSelection.ExperienceDiscountFactor = 1;
    DictionaryBuilder.m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection = 0.0;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = BasisNumber*NeighbourNumber;

    DictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    DictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "NoisyImage_BasisMatrix.json");

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisExperience(), FilePath + "NoisyImage_BasisExperience.json");

    //---------------------------------------------------------------------------------------------------

    KNNReconstructionSparseEncoder<double> SparseEncoder;

    SparseEncoder.m_Parameter.NeighbourNumber = NeighbourNumber;
    SparseEncoder.m_Parameter.SimilarityType = SimilarityType;
    SparseEncoder.SetMaxNumberOfThread(MaxNumberOfThread);

    SparseEncoder.SetInputFeatureData(&FeatureData);
    SparseEncoder.SetInputDictionary(DictionaryPtr);
    SparseEncoder.Update();

    auto& CodeSet = *SparseEncoder.GetOutputCode();

    DenseMatrix<double> ReconstructedFeatureData;

    SparseEncoder.GetReconstructedData(ReconstructedFeatureData);

    SaveDenseMatrixAsJsonDataFile(ReconstructedFeatureData, FilePath + "NoisyImage_ReconstructedPatch.json");

}
