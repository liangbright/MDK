#ifndef __mdkFeatureSparseEncoderDictionaryBuilder_h
#define __mdkFeatureSparseEncoderDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
class mdkFeatureSparseEncoderDictionaryBuilder : public mdkObject
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

    mdkFeatureSparseEncoderDictionaryBuilder();

    ~mdkFeatureSparseEncoderDictionaryBuilder();

    //---------------------------------------------------//

    void Reset();

    //---------------------------------------------------//

    bool SetInitialDictionary(const mdkMatrix<ElementType>& InitialDictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    bool SaveDictionary(const std::string& FilePathAndName);

    bool SetOutputDictionary(mdkFeatureDictionary<ElementType>& Dictionary);

    bool SetDictionaryLength(int64 Length);

    //---------------------------------------------------//

    bool SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder);

    //---------------------------------------------------//

    bool SetInputFeatureData(const mdkMatrix<ElementType>& InputFeatureData);

    bool LoadFeatureData(const std::string& FilePathAndName);

    //----------------------------------------------------//

    virtual bool Update();

    virtual bool GenerateDictionary();

    //----------------------------------------------------//

    const mdkFeatureDictionary<ElementType>& GetOutputDictionary();


private:
//deleted
    mdkFeatureSparseEncoderDictionaryBuilder(const mdkFeatureSparseEncoderDictionaryBuilder&) = delete;

    void operator=(const mdkFeatureSparseEncoderDictionaryBuilder&) = delete;

    mdkFeatureSparseEncoderDictionaryBuilder(mdkFeatureSparseEncoderDictionaryBuilder&&) = delete;

    void operator=(mdkFeatureSparseEncoderDictionaryBuilder&&) = delete;

};

}// namespace mdk


#include "mdkFeatureSparseEncoderDictionaryBuilder.hpp"

#endif