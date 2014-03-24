#ifndef __mdkFeatureEncoder_h
#define __mdkFeatureEncoder_h

#include <string>

#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class FeatureEncoder : public mdkObject
{
protected:

    // input:

    const DenseMatrix<ElementType>* m_FeatureData;

    // input, and can also be internal data, 

    const FeatureDictionary<ElementType>* m_Dictionary;

    FeatureDictionary<ElementType> m_Dictionary_SharedCopy;

    // output:

    DenseMatrix<ElementType>* m_FeatureCode;

    DenseMatrix<ElementType> m_FeatureCode_SharedCopy;

public:

    FeatureEncoder();

    ~FeatureEncoder();

    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    bool SetDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCode();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    FeatureEncoder(const FeatureEncoder&) = delete;

    void operator=(const FeatureEncoder&) = delete;

};

}

#include "mdkFeatureEncoder.hpp"

#endif