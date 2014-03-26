#ifndef __mdkKMeansDictionaryBuilder_h
#define __mdkKMeansDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
class KMeansDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
protected:

    std::string m_KMeansLibraryName;

    const FeatureDictionary<ElementType>* m_InitialDictionary;

    int64 m_DictionaryLength;

public:

    KMeansDictionaryBuilder();

    ~KMeansDictionaryBuilder();

    void Clear();

    bool SelectKMeansLibrary(const std::string& KMeansLibraryName);
  
    bool SetInitialDictionary(const FeatureDictionary<ElementType>* InitialDictionary);

    bool SetOutputDictionaryLength(int64 DictionaryLength);

    //----------------------------------------------------//

protected:

    bool GenerateDictionary();

private:

    bool KMeansFirstTimeBuild();

    bool KMeansFirstTimeBuild_using_OpenCV();

    bool KMeansFirstTimeBuild_using_VLFeat();

    bool KMeansOnlineUpdate();

private:
//deleted
    KMeansDictionaryBuilder(const KMeansDictionaryBuilder&) = delete;

    void operator=(const KMeansDictionaryBuilder&) = delete;

    KMeansDictionaryBuilder(KMeansDictionaryBuilder&&) = delete;

    void operator=(KMeansDictionaryBuilder&&) = delete;
};

}// namespace mdk


#include "mdkKMeansDictionaryBuilder.hpp"

#endif