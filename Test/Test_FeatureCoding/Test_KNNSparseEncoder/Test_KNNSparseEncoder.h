#ifndef __Test_KNNSparseEncoder_h
#define __Test_KNNSparseEncoder_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include "mdkTypeToOpenCVTranslation.h"

#include "mdkDebugConfig.h"

#include "mdkDenseMatrix.h"

#include "mdkFeatureDictionaryForSparseCoding.h"

#include "mdkFeatureDictionaryBuilder.h"
#include "mdkKMeansDictionaryBuilder.h"

#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkKNNReconstructionSparseEncoder.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNSoftAssignAndReconstructionSparseEncoder.h"

#include "mdkSPAMSOnlineDictionaryBuilder.h"
#include "mdkSPAMSSparseEncoder.h"

#include "mdkFeatureCoding_Common_Function.h"


namespace mdk
{

void Test_DisplayMatrix()
{
    DenseMatrix<double> A = { 1, 0.00001, 0.00001, 0.00002, 0.0000000003 };
    DisplayMatrix("A", A, 3);

}


void Test_KNNReconstructionSparseEncoder()
{
    int_max FeatureDimension = 10;

    int_max FeatureVectorNumber = 3;

    DenseMatrix<float> FeatureData(FeatureDimension, FeatureVectorNumber);

    auto t0 = std::chrono::system_clock::now();

    for (int_max i = 0; i < FeatureVectorNumber; ++i)
    {
        auto value = float(i) / float(FeatureVectorNumber);

        FeatureData.Col(i) = value;
    }

    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "Fill FeatureData  time " << raw_time.count() << '\n';

    FeatureDictionaryForSparseCoding<float> Dictionary;

    int_max K = 10;

    Dictionary.BasisMatrix().FastResize(FeatureDimension, K);

    for (int_max i = 0; i < K; ++i)
    {
        Dictionary.BasisMatrix().Col(i) = i;
    }

    DenseMatrix<float> DtD = Dictionary.BasisMatrix().Transpose()*Dictionary.BasisMatrix();

    DisplayMatrix("DtD", DtD, 0);

    KNNReconstructionSparseEncoder<float> Encoder;

    Encoder.m_Parameter.NeighbourNumber = 3;

    Encoder.m_Parameter.CodeNonnegative = true;
    Encoder.m_Parameter.CodeSumToOne = true;

    Encoder.SetInputDictionary(&Dictionary);

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetMaxNumberOfThreads(1);

    std::cout << "Encoder.Update()  start " << '\n';

    t0 = std::chrono::system_clock::now();

    //for (int_max i = 0; i < 10; ++i)
    //{
        Encoder.Update();
   // }

    t1 = std::chrono::system_clock::now();
    raw_time = t1 - t0;
    std::cout << "Encoder.Update()  time " << raw_time.count() << '\n';

    auto Code = Encoder.GetOutputCodeInDenseMatrix();

    DisplayMatrix("X", FeatureData, 6);

    DisplayMatrix("D", Dictionary.BasisMatrix(), 6);

    DisplayMatrix("Alpha", *Code, 6);

}


void Test_KNNReconstructionAndSoftAssignSparseEncoder()
{

    int_max FeatureDimension = 10;

    int_max FeatureVectorNumber = 3;

    DenseMatrix<float> FeatureData(FeatureDimension, FeatureVectorNumber);

    auto t0 = std::chrono::system_clock::now();

    for (int_max i = 0; i < FeatureVectorNumber; ++i)
    {
        auto value = float(i) / float(FeatureVectorNumber);

        FeatureData.Col(i) = value;
    }

    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "Fill FeatureData  time " << raw_time.count() << '\n';

    FeatureDictionaryForSparseCoding<float> Dictionary;

    int_max K = 10;

    Dictionary.BasisMatrix().FastResize(FeatureDimension, K);

    Dictionary.VarianceOfL2Distance().FastResize(1, K);

    for (int_max i = 0; i < K; ++i)
    {
        Dictionary.BasisMatrix().Col(i) = i;
        Dictionary.VarianceOfL2Distance()[i] = 1;
    }

    DenseMatrix<float> DtD = Dictionary.BasisMatrix().Transpose()*Dictionary.BasisMatrix();

    DisplayMatrix("DtD", DtD, 0);

    KNNSoftAssignAndReconstructionSparseEncoder<float> Encoder;

    Encoder.m_Parameter.NeighbourNumber = 3;

    Encoder.m_Parameter.CodePositive = true;
    Encoder.m_Parameter.CodeSumToOne = true;

    Encoder.m_Parameter.SimilarityType = VectorSimilarityTypeEnum::L2Distance;

    Encoder.SetInputDictionary(&Dictionary);

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetMaxNumberOfThreads(1);

    std::cout << "Encoder.Update()  start " << '\n';

    t0 = std::chrono::system_clock::now();

    //for (int_max i = 0; i < 10; ++i)
    //{
    Encoder.Update();
    // }

    t1 = std::chrono::system_clock::now();
    raw_time = t1 - t0;
    std::cout << "Encoder.Update()  time " << raw_time.count() << '\n';

    auto Code = Encoder.GetOutputCodeInDenseMatrix();

    DisplayMatrix("X", FeatureData, 6);

    DisplayMatrix("D", Dictionary.BasisMatrix(), 6);

    DisplayMatrix("Alpha", *Code, 6);
}


}//namespace mdk

#endif