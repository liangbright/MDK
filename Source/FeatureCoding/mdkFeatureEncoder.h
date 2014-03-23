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

    const mdkDenseMatrix<ElementType>* m_FeatureData;

    // input, and can also be internal data, 

    const mdkFeatureDictionary<ElementType>* m_Dictionary;

    mdkFeatureDictionary<ElementType> m_Dictionary_SharedCopy;

    // output:

    mdkDenseMatrix<ElementType>* m_FeatureCode;

    mdkDenseMatrix<ElementType> m_FeatureCode_SharedCopy;

public:

    mdkFeatureEncoder();

    ~mdkFeatureEncoder();

    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    bool SetInputFeatureData(const mdkDenseMatrix<ElementType>* InputFeatureData);

    bool SetDictionary(const mdkFeatureDictionary<ElementType>* Dictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    bool SetOutputFeatureCode(mdkDenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    mdkDenseMatrix<ElementType>* GetOutputFeatureCode();


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