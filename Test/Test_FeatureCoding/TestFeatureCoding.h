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
#include "mdkFeatureEncoder.h"
#include "mdkFeatureEncoderDictionaryBuilder.h"
#include "mdkFeatureSparseEncoder.h"
#include "mdkKMeansDictionaryBuilder.h"
#include "mdkKNNReconstructionSparseEncoder.h"
#include "mdkKNNSoftAssignSparseEncoder.h"

#include "mdkTypeToOpenCVTranslation.h"

using namespace mdk;

template<typename T>
void DisplayMatrix(const std::string& Name, const DenseMatrix<T>& Matrix, uint32 value_std_setw = 6, uint32 precision = 0)
{
    std::cout << Name << " = " << '\n';

    for (mdk::int64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (mdk::int64 j = 0; j < Matrix.GetColNumber(); ++j)
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

    A = { 1, 2, 3,
          4, 5, 6 };

    DisplayMatrix("A", A);

    auto B = A.Transpose();

    DisplayMatrix("B", B);

    auto OpenCVElemetType = GetOpenCVSingleChannelMatrixElementType<double>();

   // cv::Mat img(500, 500, CV_8UC3);

    auto Data = cv::Mat(2, 3, OpenCVElemetType, (void*)B.GetElementPointer());

   // B.Clear();

   // cv::Mat Data(2, 3, CV_64FC1);

    std::cout << "Data:" << '\n';
    
    for (mdk::int64 i = 0; i < 2; ++i)
    {
        for (mdk::int64 j = 0; j < 3; ++j)
        {
            std::cout << Data.at<double>(i, j) << "  ";
        }

        std::cout << '\n';
    }

}

void Test_mdkKMeansDictionaryBuilder_using_OpenCV()
{
    //OpenCV KMeans only support float ! not double!

    KMeansDictionaryBuilder<double> KMeansBuilder;

    DenseMatrix<double> FeatureData(100, 100);

    for (mdk::int64 i = 0; i < 100; ++i)
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

    mdk::int64 DictionaryLength = 3;

    FeatureDictionary<double> KMeansDictionary;

    KMeansDictionary.m_Record.Resize(100, DictionaryLength);

    KMeansBuilder.SetInputFeatureData(&FeatureData);

    KMeansBuilder.SetOutputDictionary(&KMeansDictionary);

    KMeansBuilder.SetOutputDictionaryLength(3);

    KMeansBuilder.Update();

    DisplayMatrix("Dictionary", KMeansDictionary.m_Record);
}

#endif