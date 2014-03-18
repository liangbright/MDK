#ifndef __mdkKMeansDictionaryBuilder_h
#define __mdkKMeansDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
class mdkKMeansDictionaryBuilder : public mdkObject
{
protected:

    mdkFeatureCodebook<ElementType> m_Codebook;

    int64 m_CodebookLength;

    mdkFeatureEncoder<ElementType>* m_GenericEncoder;

    mdkMatrix<ElementType> m_FeatureData;

public:

    mdkKMeansDictionaryBuilder();

    mdkKMeansDictionaryBuilder(mdkKMeansDictionaryBuilder&&);

    ~mdkKMeansDictionaryBuilder();

    void operator=(mdkKMeansDictionaryBuilder&&);

    //---------------------------------------------------//

    void Reset();

    void Clear();

    //---------------------------------------------------//

    void SetInitialCodebook(const mdkMatrix<ElementType>& InitialCodebook);

    bool LoadCodebook(const std::string& FilePathAndName);

    bool SaveCodebook(const std::string& FilePathAndName);

    bool SetOutputCodebook(mdkFeatureCodebook<ElementType>& Codebook);

    bool SetCodebookLength(int64 Length);

    //---------------------------------------------------//

    void SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder);

    //---------------------------------------------------//

    bool SetInputFeatureData(const mdkMatrix<ElementType>& InputFeatureData);

    bool LoadFeatureData(const std::string& FilePathAndName);

    //----------------------------------------------------//

    virtual bool Run();

    virtual bool GenerateDictionary();

    //----------------------------------------------------//

    mdkFeatureCodebook<ElementType>& GetOutputCodebook();


private:
//deleted
    mdkKMeansDictionaryBuilder(const mdkKMeansDictionaryBuilder&) = delete;

    void operator=(const mdkKMeansDictionaryBuilder&) = delete;

};

}// namespace mdk


#include "mdkKMeansDictionaryBuilder.hpp"

#endif