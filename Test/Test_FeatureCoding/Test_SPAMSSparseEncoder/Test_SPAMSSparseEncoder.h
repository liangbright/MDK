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

    FeatureDictionaryForSparseCoding<double> Dictionary;

    Dictionary.BasisMatrix().FastResize(FeatureDimension, 3);

    for (int_max i = 0; i < 3; ++i)
    {
        Dictionary.BasisMatrix().Col(i) = i;
        Dictionary.BasisMatrix().Row(i) = i;
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

    DisplayMatrix("D", Dictionary.BasisMatrix(), 6);

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