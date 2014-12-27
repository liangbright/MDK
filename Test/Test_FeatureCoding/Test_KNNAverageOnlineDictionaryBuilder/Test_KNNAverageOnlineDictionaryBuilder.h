#include "mdkFileIO.h"
#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNAverageOnlineDictionaryBuilder.h"

void Test_SimpleCase()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/TestData/SimpleCase/";

	std::string FeatureDataFilePathAndName = FilePath + "DataSample.json";

	DenseMatrix<double> FeatureData;
	LoadDenseMatrixFromJsonDataFile(FeatureData, FeatureDataFilePathAndName);

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

    InitialDictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0.5;

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

    KNNAverageOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = 10;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::L1Distance;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.1;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

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

    std::string FilePath = "C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/TestData/GaussianObjectImage/";

	std::string FeatureDataFilePathAndName = FilePath + "GaussianObjectImage.json";

	DenseMatrix<double> FeatureData;
	LoadDenseMatrixFromJsonDataFile(FeatureData, FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

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

    //--------------------------------------------------------------------------------------------

    KNNAverageOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = 0.0000001;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 100;

    DictionaryBuilder.m_Parameter.WhetherToUseScaleFactor = false;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetInitialDictionary(InitialDictionaryPtr);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "GaussianObjectImage_BasisMatrix.json");
}


void Test_ImageDenoising()
{
    using namespace mdk;

	std::string FilePath = "C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/TestData/ImageDenoising/";

	std::string FeatureDataFilePathAndName = FilePath + "NoisyImagePatch.json";

	DenseMatrix<double> FeatureData;
	LoadDenseMatrixFromJsonDataFile(FeatureData, FeatureDataFilePathAndName);

    int_max FeatureDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max NeighbourNumber = 3;

    auto SimilarityType = VectorSimilarityTypeEnum::L1Distance;
	//auto SimilarityType = VectorSimilarityTypeEnum::L2Distance;

    double SimilarityThreshold = 0.1;

    double Variance_L1 = 0.287;

    double Variance_L2 = 0.287;

    int_max BasisNumber = 256;

    int_max MaxNumberOfThread = 8;

    //-------------------------------------------------------------------------------------------

    std::cout << "run KNNBasisSelectionOnlineDictionaryBuilder" << '\n';

    KNNBasisSelectionOnlineDictionaryBuilder<double> InitialDictionaryBuilder;

    InitialDictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;
	InitialDictionaryBuilder.m_Parameter.BasisNormalizedWithL1Norm = true;
	//InitialDictionaryBuilder.m_Parameter.BasisNormalizedWithL2Norm = true;

	InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = SimilarityThreshold;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = Variance_L1;
    InitialDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = Variance_L2;

    InitialDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    InitialDictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection = 0.5;

    InitialDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = 5000;

    InitialDictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = true;
    InitialDictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = FilePath;

    InitialDictionaryBuilder.m_Parameter.MaxNumberOfThread = MaxNumberOfThread;

    InitialDictionaryBuilder.SetInputFeatureData(&FeatureData);

    InitialDictionaryBuilder.Update();

    auto InitialDictionaryPtr = InitialDictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->BasisMatrix(), FilePath + "NoisyImage_BasisMatrix_init.json");

    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->BasisExperience(), FilePath + "NoisyImage_BasisExperience_init.json");

    SaveDenseMatrixAsJsonDataFile(InitialDictionaryPtr->SimilarityMatrix(), FilePath + "NoisyImage_SimilarityMatrix_init.json");
    
    //--------------------------------------------------------------------------------------------

    std::cout << "run KNNAverageOnlineDictionaryBuilder" << '\n';

    KNNAverageOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.m_Parameter.BasisNumber = BasisNumber;
	DictionaryBuilder.m_Parameter.BasisNormalizedWithL1Norm = true;
	//DictionaryBuilder.m_Parameter.BasisNormalizedWithL2Norm = true;

    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = NeighbourNumber;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = SimilarityType;
    DictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = SimilarityThreshold;

    DictionaryBuilder.m_Parameter.ExperienceDiscountFactor = 0;

    DictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = NeighbourNumber * BasisNumber;

    DictionaryBuilder.m_Parameter.WhetherToUseScaleFactor = true;

    DictionaryBuilder.m_Parameter.MaxNumberOfInteration = 1;

    DictionaryBuilder.m_Parameter.MaxNumberOfThread = MaxNumberOfThread;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetInitialDictionary(InitialDictionaryPtr);

    DictionaryBuilder.Update();

    auto DictionaryPtr = DictionaryBuilder.GetOutputDictionary();

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisMatrix(), FilePath + "NoisyImage_BasisMatrix.json");

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->BasisExperience(), FilePath + "NoisyImage_BasisExperience.json");

    SaveDenseMatrixAsJsonDataFile(DictionaryPtr->SimilarityMatrix(), FilePath + "NoisyImage_SimilarityMatrix.json");

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
