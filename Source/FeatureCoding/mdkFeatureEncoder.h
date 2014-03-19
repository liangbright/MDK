#ifndef __mdkFeatureEncoder_h
#define __mdkFeatureEncoder_h

#include <string>

#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureEncoder : public mdkObject
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

    mdkFeatureEncoder();

    ~mdkFeatureEncoder();

    //-----------------------------------------

    virtual void Reset();
    
    //-----------------------------------------

    bool SetInputFeatureData(const mdkMatrix<ElementType>* InputFeatureData);

    bool SetDictionary(const mdkFeatureDictionary<ElementType>* Dictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    bool SetOutputFeatureCode(mdkMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    const mdkMatrix<ElementType>& GetOutputFeatureCode();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    mdkFeatureEncoder(const mdkFeatureEncoder&) = delete;

    void operator=(const mdkFeatureEncoder&) = delete;

};

}

#include "mdkFeatureEncoder.hpp"

#endif