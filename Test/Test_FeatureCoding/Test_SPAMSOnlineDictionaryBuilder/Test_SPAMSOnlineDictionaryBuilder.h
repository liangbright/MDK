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
void DisplayMatrix(const std::string& Name, const DenseMatrix<ElementType>& InputMatrix, int_max value_std_setw = 6, int_max precision = 0)
{
    std::cout << Name << " = " << '\n';

    for (int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(value_std_setw + precision) << std::setprecision(precision) << InputMatrix(i, j);
        }
        std::cout << '\n';
    }
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

    DisplayMatrix<double>("NeighbourDistanceList", NeighbourDistanceList);
}


void Test_Train()
{
    DenseMatrix<double> FeatureData(10, 3);

    for (int64 i = 0; i < 3; ++i)
    {
        auto temp = i % 3;
        if (temp == 0)
        {
            FeatureData.FillCol(i, 0.0);
        }
        else if (temp == 1)
        {
            FeatureData.FillCol(i, 1.0);
        }
        else if (temp == 2)
        {
            FeatureData.FillCol(i, 2.0);
        }
    }

    DisplayMatrix<double>("FeatureData", FeatureData);

    FeatureDictionary<double> KMeansDictionary;

    KMeansDictionary.m_Record.Resize(10, 3);

    KMeansDictionary.m_Record.FillCol(0, 0.0);
    KMeansDictionary.m_Record.FillCol(1, 1.0);
    KMeansDictionary.m_Record.FillCol(2, 2.0);


    DenseMatrix<double> OutputFeatureCodeInCompactFormat(3, 3);

    KNNReconstructionSparseEncoder<double> Encoder;

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetInputDictionary(&KMeansDictionary);

    Encoder.SetNeighbourNumber(1);

    Encoder.SetOutputFeatureCodeInCompactFormat(&OutputFeatureCodeInCompactFormat);

    Encoder.Update();

    DisplayMatrix<double>("OutputFeatureCodeInCompactFormat", OutputFeatureCodeInCompactFormat);
}

}//namespace mdk

#endif