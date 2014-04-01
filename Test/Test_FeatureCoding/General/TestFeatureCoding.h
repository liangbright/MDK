#ifndef __TestFeatureCoding_h
#define __TestFeatureCoding_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


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

namespace mdk
{

template<typename T>
void DisplayMatrix(const std::string& Name, const DenseMatrix<T>& Matrix, uint32 value_std_setw = 6, uint32 precision = 0)
{
    std::cout << Name << " = " << '\n';

    for (int64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (int64 j = 0; j < Matrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(value_std_setw + precision) << std::setprecision(precision) << Matrix(i, j);
        }
        std::cout << '\n';
    }
}


void Test_OpenCVMatrix()
{
    DenseMatrix<double> A(2, 3);

    A = { { 1, 2, 3 },
          { 4, 5, 6 } };

    DisplayMatrix("A", A);

    auto B = A.Transpose();

    DisplayMatrix("B", B);

    auto OpenCVElemetType = GetOpenCVSingleChannelMatrixElementType<double>();

    auto Data = cv::Mat(2, 3, OpenCVElemetType, (void*)B.GetElementPointer());

   // B.Clear();

   // cv::Mat Data(2, 3, CV_64FC1);

    std::cout << "Data:" << '\n';
    
    for (int64 i = 0; i < 2; ++i)
    {
        for (int64 j = 0; j < 3; ++j)
        {
            std::cout << Data.at<double>(i, j) << "  ";
        }

        std::cout << '\n';
    }

}

void Test_KMeansDictionaryBuilder_using_OpenCV()
{
    //OpenCV KMeans only support float ! not double!

    KMeansDictionaryBuilder<double> KMeansBuilder;

    DenseMatrix<double> FeatureData(100, 100);

    for (int64 i = 0; i < 100; ++i)
    {
        auto temp = i % 3;
        if (temp == 0)
        {
            FeatureData.FillRow(i, 0);
        }
        else if (temp == 1)
        {
            FeatureData.FillRow(i, 1);
        }
        else if (temp == 2)
        {
            FeatureData.FillRow(i, 2);
        }
    }

    int64 DictionaryLength = 3;

    FeatureDictionary<double> KMeansDictionary;

    KMeansDictionary.m_Record.Resize(100, DictionaryLength);

    KMeansBuilder.SetInputFeatureData(&FeatureData);

    KMeansBuilder.SetOutputDictionary(&KMeansDictionary);

    KMeansBuilder.SetOutputDictionaryLength(3);

    KMeansBuilder.Update();

    DisplayMatrix("Dictionary", KMeansDictionary.m_Record);
}

void Test_FindKNNByDistanceList()
{
    DenseMatrix<double> DistanceList = {1, 2, 3, 4 ,5, 1, 2, 3, 4, 5 };

    int64 K = 10;

    auto NeighbourIndexList = FindKNNByDistanceList(K, DistanceList);

    DisplayMatrix("NeighbourIndexList", NeighbourIndexList);

    DenseMatrix<double> NeighbourDistanceList(1, K);

    for (int64 i = 0; i < K; ++i)
    {
        NeighbourDistanceList[i] = DistanceList[NeighbourIndexList[i]];
    }

    DisplayMatrix("NeighbourDistanceList", NeighbourDistanceList);
}


void Test_KNNReconstructionSparseEncoder()
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

    DisplayMatrix("FeatureData", FeatureData);

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

    DisplayMatrix("OutputFeatureCodeInCompactFormat", OutputFeatureCodeInCompactFormat);
}


void Test_FeatureCodeInCompactFormat()
{
    // index is stored as double or float from int64
    // it needs to be converted to int64

    // convert int64 to double or float
    // and convert back
    // all good

    int64 CodeLength = 1000000;

    std::vector<int64> intList(CodeLength);

    std::vector<double> doubleList(CodeLength);

    std::vector<float> floatList(CodeLength);


    for (int64 i = 0; i < CodeLength; ++i)
    {
        intList[i] = i;

        doubleList[i] = i;

        floatList[i] = i;
    }


    int64 diff_a = 0;
    int64 diff_b = 0;

    double diff_c = 0;
    float diff_d = 0;

    for (int64 i = 0; i < CodeLength; ++i)
    {
        diff_a += std::abs(intList[i] - int64(doubleList[i]));

        diff_b += std::abs(intList[i] - int64(floatList[i]));

        diff_a += std::abs(double(intList[i]) - doubleList[i]);

        diff_b += std::abs(float(intList[i]) - floatList[i]);
    }


}


}//namespace mdk

#endif