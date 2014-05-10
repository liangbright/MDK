#include "mdkFileIO.h"
#include "mdkKNNReconstructionSparseEncoder.h"

void Test_ComputeKNNCode()
{
    using namespace mdk;

    CharString FilePath = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionSparseEncoder/Debug/";

    CharString FeatureDataFilePathAndName = FilePath + "FeatureData.json";

    auto FeatureData = LoadDenseMatrixFromJsonDataFile<double>(FeatureDataFilePathAndName);

    CharString KNNBasisMatrixFilePathAndName = FilePath + "KNNBasisMatrix.json";

    auto KNNBasisMatrix = LoadDenseMatrixFromJsonDataFile<double>(KNNBasisMatrixFilePathAndName);

    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    bool CodeNonnegative = true;

    bool CodeSumToOne = false;

    //-------------------------------------------------------------------------------------------------

    DenseMatrix<double> KNNCodeMatrix(KNNBasisNumber, DataNumber);

    DenseMatrix<double> DataVector;

    for (int_max k = 0; k < DataNumber; ++k)
    {
        DataVector.Share(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

        auto KNNCode = KNNReconstructionSparseEncoder<double>::ComputeKNNCode(DataVector, KNNBasisMatrix, CodeNonnegative, CodeSumToOne);

        KNNCodeMatrix.SetCol(k, KNNCode);
    }

    SaveDenseMatrixAsJsonDataFile(KNNCodeMatrix, FilePath + "KNNCodeMatrix.json");    
}
