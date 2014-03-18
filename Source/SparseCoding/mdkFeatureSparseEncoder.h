#ifndef __mdkFeatureSparseEncoder_h
#define __mdkFeatureSparseEncoder_h

#include <string>

#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureSparseEncoder : public mdkObject
{
protected:

    // input:

    const mdkFeatureDictionary<ElementType>* m_Dictionary;

    mdkFeatureDictionary<ElementType> m_Dictionary_SharedCopy;

    const mdkMatrix<ElementType>* m_FeatureData;

    // output:

    mdkMatrix<ElementType>* m_FeatureCode;

    mdkMatrix<ElementType> m_FeatureCode_SharedCopy;

public:

    mdkFeatureSparseEncoder();

    ~mdkFeatureSparseEncoder();

    //-----------------------------------------

    void Reset();
    
    //-----------------------------------------

    bool SetInputFeatureData(const mdkMatrix<ElementType>* InputFeatureData);

    bool SetDictionary(const mdkFeatureDictionary<ElementType>* Dictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    void SetOutputFeatureCode(mdkMatrix<ElementType>& FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    const mdkMatrix<ElementType>& GetOutputFeatureCode();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    mdkFeatureSparseEncoder(const mdkFeatureSparseEncoder&) = delete;

    void operator=(const mdkFeatureSparseEncoder&) = delete;

};

}

#include "mdkFeatureSparseEncoder.hpp"

#endif