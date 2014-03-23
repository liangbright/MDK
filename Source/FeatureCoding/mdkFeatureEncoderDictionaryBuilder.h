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

    const mdkDenseMatrix<ElementType>* m_FeatureData;

    mdkFeatureEncoder<ElementType>* m_GenericEncoder;

    // output:

    mdkFeatureDictionary<ElementType>* m_Dictionary;

    mdkFeatureDictionary<ElementType> m_Dictionary_SharedCopy;

public:

    mdkFeatureEncoderDictionaryBuilder();

    ~mdkFeatureEncoderDictionaryBuilder();

    //---------------------------------------------------//

    virtual void Clear();

    //---------------------------------------------------//

    bool SetInputFeatureData(const mdkDenseMatrix<ElementType>* InputFeatureData);

    //---------------------------------------------------//

    bool SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder);

    //---------------------------------------------------//

    bool SetOutputDictionary(mdkFeatureDictionary<ElementType>* Dictionary);

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