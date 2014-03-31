#ifndef __mdkFeatureDictionaryBuilder_h
#define __mdkFeatureDictionaryBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBuilder : public Object
{
protected:

    // input:

    const DenseMatrix<ElementType>* m_FeatureData;

    // output:

    FeatureDictionary<ElementType>* m_Dictionary;

    FeatureDictionary<ElementType>  m_Dictionary_SharedCopy;

protected:
    FeatureDictionaryBuilder();
    ~FeatureDictionaryBuilder();

public:

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
    FeatureDictionaryBuilder(const FeatureDictionaryBuilder&) = delete;

    void operator=(const FeatureDictionaryBuilder&) = delete;

    FeatureDictionaryBuilder(FeatureDictionaryBuilder&&) = delete;

    void operator=(FeatureDictionaryBuilder&&) = delete;

};

}// namespace mdk


#include "mdkFeatureDictionaryBuilder.hpp"

#endif