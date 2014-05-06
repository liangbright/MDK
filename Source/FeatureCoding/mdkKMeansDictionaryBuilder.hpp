#ifndef __mdkKMeansDictionaryBuilder_hpp
#define __mdkKMeansDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KMeansDictionaryBuilder<ElementType>::KMeansDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KMeansDictionaryBuilder<ElementType>::~KMeansDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::Clear()
{
    m_KMeansLibraryName = "OpenCV";

    m_ClusterNumber = -1;

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::ClearPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.ForceShare(m_Dictionary);
    }
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData)
{
    m_FeatureData = FeatureData;
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionary<ElementType>* Dictionary)
{
    m_InitialDictionary = Dictionary;
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary)
{
    m_Dictionary = Dictionary;
}


template<typename ElementType>
bool KMeansDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_KMeansLibraryName != "OpenCV" || m_KMeansLibraryName != "VLFeat")
    {
        MDK_Error("Invalid input @ KMeansDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_ClusterNumber <= 0)
    {
        MDK_Error("m_ClusterNumber <= 0 @ KMeansDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::GenerateDictionary()
{
    bool IsOK = false;

    if (m_InitialDictionary == nullptr) // build a brand new dictionary
    {
        this->KMeansFirstTimeBuild();
    }
    else // update the Dictionary
    {
        this->KMeansOnlineUpdate();
    }
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild()
{
    if (m_KMeansLibraryName == "OpenCV")
    {
        this->KMeansFirstTimeBuild_using_OpenCV();
    }
    else if (m_KMeansLibraryName == "VLFeat")
    {
        this->KMeansFirstTimeBuild_using_VLFeat();
    }
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild_using_OpenCV()
{
    //call OpenCV kmeans function
    //OpenCV KMeans only supports float ! not double!
    
    auto OpenCVElemetType = GetOpenCVSingleChannelMatrixElementType<ElementType>();

    if (OpenCVElemetType != OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32)
    {
        MDK_Warning("ElemetType is not float @ mdkKMeansDictionaryBuilder::KMeansFirstTimeBuild_using_OpenCV()")
    }

    //---- transpose the data ---- OpenCV : each row is a feature vector
    //
    DenseMatrix<float> tempFeatureData(m_FeatureData->GetColNumber(), m_FeatureData->GetRowNumber());

    for (int_max i = 0; i < tempFeatureData.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < tempFeatureData.GetColNumber(); ++j)
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

    auto Center = cv::Mat(int(m_ClusterNumber), int(m_FeatureData->GetColNumber()), OpenCVSingleChannelMatrixElementTypeEnum::FLOAT32);

    cv::kmeans(FeatureData, int(m_ClusterNumber), FeatureLabel,
               cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
               3, cv::KMEANS_PP_CENTERS, Center);

    // convert Center to mdkDenseMatrix  --------------------------------------------------------------         
    
    DenseMatrix<ElementType>& D = m_Dictionary->BasisMatrix();

    for (int i = 0; i < int(m_FeatureData->GetRowNumber()); ++i)
    {
        for (int j = 0; j < int(m_ClusterNumber); ++j)
        {
            D(i, j) = ElementType(Center.at<float>(j, i));
        }
    }
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::KMeansFirstTimeBuild_using_VLFeat()
{
}


template<typename ElementType>
void KMeansDictionaryBuilder<ElementType>::KMeansOnlineUpdate()
{
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KMeansDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


}// namespace mdk


#endif