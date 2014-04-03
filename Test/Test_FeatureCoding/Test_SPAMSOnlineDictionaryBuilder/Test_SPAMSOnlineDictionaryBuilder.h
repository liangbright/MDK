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

template<typename ElementType>
void DisplayMatrix(const std::string& Name, const DenseMatrix<ElementType>& InputMatrix, int_max precision = 0, bool Flag_scientific = false)
{
    std::cout << Name << " = " << '\n';

    if (Flag_scientific == true)
    {
        std::cout << std::scientific << std::setprecision(precision);
    }
    else
    {
        std::cout << std::setprecision(precision) << std::fixed;
    }

    for (int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            std::cout << std::setw(6 + precision) << InputMatrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}


void Test_DisplayMatrix()
{
    DenseMatrix<double> A = { 1, 0.00001, 0.00001, 0.00002, 0.0000000003 };
    DisplayMatrix("A", A, 3);

}

void Test_FindKNNByDistanceList()
{
    DenseMatrix<double> DistanceList = {1, 2, 3, 4 ,5, 1, 2, 3, 4, 5 };

    int_max K = 10;

    auto NeighbourIndexList = FindKNNByDistanceList(K, DistanceList);

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
    DenseMatrix<double> FeatureData(10, 1000);

    for (int64 i = 0; i < 1000; ++i)
    {
        FeatureData.FillCol(i, i);
        FeatureData.FillRow(i / 100, i);
    }

    //DisplayMatrix("FeatureData", FeatureData);

    KNNReconstructionSparseEncoder<double> Encoder;

    Encoder.SetMaxNumberOfThreads(4);

    Encoder.SetNeighbourNumber(1);

    SPAMSOnlineDictionaryBuilder<double> DictionaryBuilder;

    DictionaryBuilder.SetInputFeatureData(&FeatureData);

    DictionaryBuilder.SetSparseEncoder(&Encoder);

    DictionaryBuilder.m_Parameter.mode = -1;
    DictionaryBuilder.m_Parameter.lambda = 1;
    DictionaryBuilder.m_Parameter.K = 3;
    DictionaryBuilder.m_Parameter.posD = true;
    DictionaryBuilder.m_Parameter.verbose = false;

    DictionaryBuilder.Update();

    auto Dictionary = DictionaryBuilder.GetOutputDictionary();

    DisplayMatrix("Dictionary", Dictionary->m_Record, 6);
}

}//namespace mdk

#endif