#ifndef __Test_SPAMSSparseEncoder_h
#define __Test_SPAMSSparseEncoder_h

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

void Test_OMP()
{
    int_max FeatureDimension = 3;

    int_max FeatureVectorNumber = 3;

    DenseMatrix<double> FeatureData(FeatureDimension, FeatureVectorNumber);

    for (int_max i = 0; i < FeatureVectorNumber; ++i)
    {
        auto value = double(i) / double(FeatureVectorNumber);

        FeatureData.Col(i) = value;

        FeatureData.Row(i) = value;
    }

    FeatureDictionary<double> Dictionary;

    Dictionary.m_Record.FastResize(FeatureDimension, 3);

    for (int_max i = 0; i < 3; ++i)
    {
        Dictionary.m_Record.Col(i) = i;
        Dictionary.m_Record.Row(i) = i;
    }

    SPAMSSparseEncoder<double> Encoder;

    Encoder.m_MethodName = "OMP";

    Encoder.m_Parameter_OMP.L = 3;
    Encoder.m_Parameter_OMP.eps = 0;
    Encoder.m_Parameter_OMP.lambda = 0;

    Encoder.SetInputDictionary(&Dictionary);

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetMaxNumberOfThreads(1);

    Encoder.Update();

    auto Code = Encoder.GetOutputCodeInDenseMatrix();

    DisplayMatrix("X", FeatureData, 6);

    DisplayMatrix("D", Dictionary.m_Record, 6);

    DisplayMatrix("Alpha", *Code, 6);

}

void Test_Lasso()
{
    int_max FeatureDimension = 3;

    int_max FeatureVectorNumber = 3;

    DenseMatrix<double> FeatureData(FeatureDimension, FeatureVectorNumber);

    for (int_max i = 0; i < FeatureVectorNumber; ++i)
    {
        auto value = double(i) / double(FeatureVectorNumber);

        FeatureData.Col(i) = value;

        FeatureData.Row(i) = value;
    }

    FeatureDictionary<double> Dictionary;

    Dictionary.m_Record.FastResize(FeatureDimension, 3);

    for (int_max i = 0; i < 3; ++i)
    {
        Dictionary.m_Record.Col(i) = i;
        Dictionary.m_Record.Row(i) = i;
    }

    SPAMSSparseEncoder<double> Encoder;

    Encoder.m_MethodName = "Lasso";

    Encoder.m_Parameter_Lasso.mode = 0;
    Encoder.m_Parameter_Lasso.lambda = 1;
    Encoder.m_Parameter_Lasso.lambda2 = 0;
    Encoder.m_Parameter_Lasso.pos = true;
    Encoder.m_Parameter_Lasso.ols = false;


    Encoder.SetInputDictionary(&Dictionary);

    Encoder.SetInputFeatureData(&FeatureData);

    Encoder.SetMaxNumberOfThreads(1);

    Encoder.Update();

    auto Code = Encoder.GetOutputCodeInDenseMatrix();

    DisplayMatrix("X", FeatureData, 6);

    DisplayMatrix("D", Dictionary.m_Record, 6);

    DisplayMatrix("Alpha", *Code, 6);

}

}//namespace mdk

#endif