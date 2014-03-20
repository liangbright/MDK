#ifndef __mdkKMeansDictionaryBuilder_hpp
#define __mdkKMeansDictionaryBuilder_hpp


#include <opencv2/core/core.hpp>

#include "mdkKMeansDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
mdkKMeansDictionaryBuilder<ElementType>::mdkKMeansDictionaryBuilder()
{
    m_KMeansLibraryName = "OpenCV";
}


template<typename ElementType>
mdkKMeansDictionaryBuilder<ElementType>::~mdkKMeansDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::SelectKMeansLibrary(const std::string& KMeansLibraryName)
{
    if (KMeansLibraryName != "OpenCV" || KMeansLibraryName != "VLFeat")
    {
        mdkError << "Invalid input @ mdkKMeansDictionaryBuilder::SelectKMeansLibrary" << '\n';
        return false;
    }

    m_KMeansLibraryName = KMeansLibraryName;

    return true;
}


template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::GenerateDictionary()
{
    bool IsOK = false;

    if (m_InitialDictionary == nullptr) // build a brand new dictionary
    {
        IsOK = this->KMeansFirstTimeBuild();
    }
    else // update the Dictionary
    {
        IsOK = this->KMeansOnlineUpdate();
    }

    return IsOK;
}


template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild()
{
    if (m_KMeansLibraryName == "OpenCV")
    {
        return this->KMeansFirstTimeBuild_using_OpenCV();
    }
    else if (m_KMeansLibraryName == "VLFeat")
    {
        return this->KMeansFirstTimeBuild_using_VLFeat();
    }
    else
    {
        return false;
    }
}


template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild_using_OpenCV()
{
    //call OpenCV kmeans function
    //OpenCV KMeans only supports float ! not double!
    
    auto OpenCVElemetType = GetOpenCVSingleChannelMatrixElementType<ElementType>();

    if (OpenCVElemetType != OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32)
    {
        mdkWarning << "ElemetType is not float @ mdkKMeansDictionaryBuilder::KMeansFirstTimeBuild_using_OpenCV()" << '\n';
    }

    mdkMatrix<float> tempFeatureData(m_FeatureData->GetColNumber(), m_FeatureData->GetRowNumber());

    for (int64 i = 0; i < tempFeatureData.GetRowNumber(); ++i)
    {
        for (int64 j = 0; j < tempFeatureData.GetColNumber(); ++j)
        {
            tempFeatureData(i, j) = float((*m_FeatureData)(j, i));
        }
    }

    auto FeatureData = cv::Mat(int(m_FeatureData->GetRowNumber()), 
                               int(m_FeatureData->GetColNumber()), 
                               OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32,
                               (void*)tempFeatureData.GetElementPointer());

    // do not clear tempFeatureData, MatrixData is shared with FeatureData

    cv::Mat FeatureLabel;

    auto Center = cv::Mat(int(m_DictionaryLength), int(m_FeatureData->GetColNumber()), OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32);

    cv::kmeans(FeatureData, int(m_DictionaryLength), FeatureLabel,
               cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
               3, cv::KMEANS_PP_CENTERS, Center);

    // convert Center to mdkMatrix  --------------------------------------------------------------         

    for (int i = 0; i < int(m_FeatureData->GetRowNumber()); ++i)
    {
        for (int j = 0; j < int(m_DictionaryLength); ++j)
        {
            (*m_Dictionary).m_Record(i, j) = ElementType(Center.at<float>(j, i));
        }
    }

    return true;
}


template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild_using_VLFeat()
{

    return true;
}


template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::KMeansOnlineUpdate()
{
    return true;
}


}// namespace mdk


#endif