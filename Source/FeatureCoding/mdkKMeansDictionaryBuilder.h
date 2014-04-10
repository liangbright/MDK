#ifndef __mdkKMeansDictionaryBuilder_h
#define __mdkKMeansDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"

namespace mdk
{

template<typename ElementType>
class KMeansDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    int_max m_ClusterNumber;

    std::string m_KMeansLibraryName;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType> m_Dictionary_SharedCopy;

public:

    KMeansDictionaryBuilder();

    ~KMeansDictionaryBuilder();

    void Clear();
  
    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetInitialDictionary(const FeatureDictionary<ElementType>* InitialDictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

private:

    void GenerateDictionary();

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    void KMeansFirstTimeBuild();

    void KMeansFirstTimeBuild_using_OpenCV();

    void KMeansFirstTimeBuild_using_VLFeat();

    void KMeansOnlineUpdate();

private:
    KMeansDictionaryBuilder(const KMeansDictionaryBuilder&) = delete;

    void operator=(const KMeansDictionaryBuilder&) = delete;

    KMeansDictionaryBuilder(KMeansDictionaryBuilder&&) = delete;

    void operator=(KMeansDictionaryBuilder&&) = delete;
};

}// namespace mdk


#include "mdkKMeansDictionaryBuilder.hpp"

#endif