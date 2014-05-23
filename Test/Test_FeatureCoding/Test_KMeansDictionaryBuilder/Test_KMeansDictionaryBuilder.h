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
#include "mdkFeatureCoding_Common_Function.h"

#include "mdkTypeToOpenCVTranslation.h"

namespace mdk
{

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
    
    for (int_max i = 0; i < 2; ++i)
    {
        for (int_max j = 0; j < 3; ++j)
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

    for (int_max i = 0; i < 100; ++i)
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

    int_max DictionaryLength = 3;

    FeatureDictionaryForSparseCoding<double> KMeansDictionary;

    KMeansDictionary.BasisMatrix().FastResize(100, DictionaryLength);

    KMeansBuilder.SetInputFeatureData(&FeatureData);

    KMeansBuilder.SetOutputDictionary(&KMeansDictionary);

    KMeansBuilder.m_ClusterNumber = 3;

    KMeansBuilder.Update();

    DisplayMatrix("Dictionary", KMeansDictionary.BasisMatrix());
}


}//namespace mdk

#endif