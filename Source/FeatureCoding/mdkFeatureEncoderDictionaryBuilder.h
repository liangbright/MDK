#ifndef __mdkFeatureEncoderDictionaryBuilder_h
#define __mdkFeatureEncoderDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
class FeatureEncoderDictionaryBuilder : public Object
{
protected:

    // input:

    const DenseMatrix<ElementType>* m_FeatureData;

    // output:

    FeatureDictionary<ElementType>* m_Dictionary;

    FeatureDictionary<ElementType>  m_Dictionary_SharedCopy;

public:

    FeatureEncoderDictionaryBuilder();

    ~FeatureEncoderDictionaryBuilder();

    //---------------------------------------------------//

    virtual void Clear();

    //---------------------------------------------------//

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    //---------------------------------------------------//

    bool SetOutputDictionary(FeatureDictionary<ElementType>* Dictionary);

    //----------------------------------------------------//

    virtual bool Update();

    //----------------------------------------------------//

    FeatureDictionary<ElementType>* GetOutputDictionary();

protected:

    virtual bool GenerateDictionary();

private:
//deleted
    FeatureEncoderDictionaryBuilder(const FeatureEncoderDictionaryBuilder&) = delete;

    void operator=(const FeatureEncoderDictionaryBuilder&) = delete;

    FeatureEncoderDictionaryBuilder(FeatureEncoderDictionaryBuilder&&) = delete;

    void operator=(FeatureEncoderDictionaryBuilder&&) = delete;

};

}// namespace mdk


#include "mdkFeatureEncoderDictionaryBuilder.hpp"

#endif