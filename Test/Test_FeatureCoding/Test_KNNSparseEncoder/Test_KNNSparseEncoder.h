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
#include "mdkSPAMSSparseEncoder.h"
#include "mdkKNNReconstructionSparseEncoder.h"

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


void Test_KNNReconstructionSparseEncoder()
{
    int_max FeatureDimension = 10000;

    int_max FeatureVectorNumber = 10000;

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

    FeatureDictionary<float> Dictionary;

    int_max K = 10;

    Dictionary.m_Record.FastResize(FeatureDimension, K);

    for (int_max i = 0; i < K; ++i)
    {
        Dictionary.m_Record.Col(i) = i;
    }

    KNNReconstructionSparseEncoder<float> Encoder;

    Encoder.m_Parameter.LsqlinMethodName = "Normal";
    Encoder.m_Parameter.NeighbourNumber = 3;

    Encoder.SetInputDictionary(&Dictionary);

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetMaxNumberOfThreads(1);

    std::cout << "Encoder.Update()  start " << '\n';

    t0 = std::chrono::system_clock::now();

    for (int_max i = 0; i < 10; ++i)
    {
        Encoder.Update();
    }

    t1 = std::chrono::system_clock::now();
    raw_time = t1 - t0;
    std::cout << "Encoder.Update()  time " << raw_time.count() << '\n';

    auto Code = Encoder.GetOutputCodeInDenseMatrix();

   // DisplayMatrix("X", FeatureData, 6);

  //  DisplayMatrix("D", Dictionary.m_Record, 6);

  //  DisplayMatrix("Alpha", *Code, 6);

}

}//namespace mdk

#endif