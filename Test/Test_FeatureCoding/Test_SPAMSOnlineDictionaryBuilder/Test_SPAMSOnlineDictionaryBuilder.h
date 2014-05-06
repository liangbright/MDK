#ifndef __Test_SPAMSOnlineDictionaryBuilder_h
#define __Test_SPAMSOnlineDictionaryBuilder_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include "mdkDebugConfig.h"
#include "mdkType.h"

#include "mdkDenseMatrix.h"
#include "mdkFeatureDictionary.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkKMeansDictionaryBuilder.h"
#include "mdkKNNReconstructionSparseEncoder.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkTypeToOpenCVTranslation.h"
#include "mdkSPAMSOnlineDictionaryBuilder.h"

namespace mdk
{

void Test_DisplayMatrix()
{
    DenseMatrix<double> A = { 1, 0.00001, 0.00001, 0.00002, 0.0000000003 };
    DisplayMatrix("A", A, 3);

}

void Test_FindKNNByDistanceList()
{
    DenseMatrix<double> DistanceList = {1, 2, 3, 4 ,5, 1, 2, 3, 4, 5 };

    int_max K = 10;

    auto NeighbourIndexList = FindKNNByDistanceList(DistanceList, K);

    DisplayMatrix<int_max>("NeighbourIndexList", NeighbourIndexList);

    DenseMatrix<double> NeighbourDistanceList(1, K);

    for (int_max i = 0; i < K; ++i)
    {
        NeighbourDistanceList[i] = DistanceList[NeighbourIndexList[i]];
    }

    DisplayMatrix("NeighbourDistanceList", NeighbourDistanceList);
}


void Test_Train()
{
    int_max FeatureDimension = 10000;

    int_max FeatureVectorNumber = FeatureDimension*10;

    DenseMatrix<double> FeatureData(FeatureDimension, FeatureVectorNumber);

    auto t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < FeatureVectorNumber; ++i)
    {
        auto value = double(i) / double(FeatureVectorNumber);

        FeatureData.FillCol(i, value);

//        FeatureData.FillRow(i/1000, value);
    }

    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "Fill FeatureData  time " << raw_time.count() << '\n';

    //DisplayMatrix("FeatureData", FeatureData);

    KNNReconstructionSparseEncoder<double> Encoder;

    Encoder.SetMaxNumberOfThreads(1);

    Encoder.m_Parameter.NeighbourNumber = 3;

    SPAMSOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetSparseEncoder(&Encoder);

    DictionaryBuilder.m_Parameter.mode = -1;
    DictionaryBuilder.m_Parameter.lambda = 1;
    DictionaryBuilder.m_Parameter.K = 100;
    DictionaryBuilder.m_Parameter.posD = false;
    DictionaryBuilder.m_Parameter.verbose = false;

    std::cout << "DictionaryBuilder.Update()  start: " << '\n';

    t0 = std::chrono::system_clock::now();

    DictionaryBuilder.Update();

    t1 = std::chrono::system_clock::now();

    raw_time = t1 - t0;
    std::cout << "DictionaryBuilder.Update()  time " << raw_time.count() << '\n';

    auto Dictionary = DictionaryBuilder.GetOutputDictionary();

//    DisplayMatrix("Dictionary", Dictionary->m_Record, 6);
}

}//namespace mdk

#endif