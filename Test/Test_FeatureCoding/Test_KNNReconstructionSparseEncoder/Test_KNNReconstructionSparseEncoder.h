#include "mdkFileIO.h"
#include "mdkKNNReconstructionSparseEncoder.h"

void Test_ComputeKNNCode()
{
    using namespace mdk;

    std::string FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionSparseEncoder/Debug/";

    std::string FeatureDataFilePathAndName = FilePath + "FeatureData.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    std::string KNNBasisMatrixFilePathAndName = FilePath + "KNNBasisMatrix.json";

    auto KNNBasisMatrix = LoadDenseMatrixFromJsonDataFile<double>(KNNBasisMatrixFilePathAndName);

    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    bool CodeNonnegative = true;

    bool CodeSumToOne = false;

    //-------------------------------------------------------------------------------------------------

    DenseMatrix<double> KNNCodeMatrix(KNNBasisNumber, DataNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {
		auto DataVector = FeatureData.RefCol(k);

        auto KNNCode = KNNReconstructionSparseEncoder<double>::ComputeKNNCode(DataVector, KNNBasisMatrix, CodeNonnegative, CodeSumToOne);

        KNNCodeMatrix.SetCol(k, KNNCode);
    }

    SaveDenseMatrixAsJsonDataFile(KNNCodeMatrix, FilePath + "KNNCodeMatrix.json");    
}
