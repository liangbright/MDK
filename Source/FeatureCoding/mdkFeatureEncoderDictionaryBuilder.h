#ifndef __mdkFeatureEncoderDictionaryBuilder_h
#define __mdkFeatureEncoderDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
class mdkFeatureEncoderDictionaryBuilder : public mdkObject
{
protected:

    // input:

    mdkFeatureEncoder<ElementType>* m_GenericEncoder;

    const mdkMatrix<ElementType>* m_FeatureData;

    int64 m_DictionaryLength;

    const mdkFeatureDictionary<ElementType>* m_InitialDictionary;

    // output:

    mdkFeatureDictionary<ElementType>* m_Dictionary;

    mdkFeatureDictionary<ElementType> m_Dictionary_SharedCopy;

public:

    mdkFeatureEncoderDictionaryBuilder();

    ~mdkFeatureEncoderDictionaryBuilder();

    //---------------------------------------------------//

    virtual void Reset();

    //---------------------------------------------------//

    bool SetInitialDictionary(const mdkFeatureDictionary<ElementType>* InitialDictionary);

    bool SetOutputDictionary(mdkFeatureDictionary<ElementType>* Dictionary);

    bool SetDictionaryLength(int64 Length);

    //---------------------------------------------------//

    bool SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder);

    //---------------------------------------------------//

    bool SetInputFeatureData(const mdkMatrix<ElementType>* InputFeatureData);

    //----------------------------------------------------//

    virtual bool Update();

    //----------------------------------------------------//

    mdkFeatureDictionary<ElementType>* GetOutputDictionary();

protected:

    virtual bool GenerateDictionary();

private:
//deleted
    mdkFeatureEncoderDictionaryBuilder(const mdkFeatureEncoderDictionaryBuilder&) = delete;

    void operator=(const mdkFeatureEncoderDictionaryBuilder&) = delete;

    mdkFeatureEncoderDictionaryBuilder(mdkFeatureEncoderDictionaryBuilder&&) = delete;

    void operator=(mdkFeatureEncoderDictionaryBuilder&&) = delete;

};

}// namespace mdk


#include "mdkFeatureEncoderDictionaryBuilder.hpp"

#endif