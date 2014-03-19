#ifndef __mdkKMeansDictionaryBuilder_h
#define __mdkKMeansDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureEncoderDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
class mdkKMeansDictionaryBuilder : public mdkFeatureEncoderDictionaryBuilder<ElementType>
{
protected:

    std::string m_KMeansLibrary;

public:

    mdkKMeansDictionaryBuilder();


    ~mdkKMeansDictionaryBuilder();

    bool SetKMeansLibrary(const std::string& KMeansLibrary);

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
    mdkKMeansDictionaryBuilder(const mdkKMeansDictionaryBuilder&) = delete;

    void operator=(const mdkKMeansDictionaryBuilder&) = delete;

    mdkKMeansDictionaryBuilder(mdkKMeansDictionaryBuilder&&) = delete;

    void operator=(mdkKMeansDictionaryBuilder&&) = delete;


};

}// namespace mdk


#include "mdkKMeansDictionaryBuilder.hpp"

#endif