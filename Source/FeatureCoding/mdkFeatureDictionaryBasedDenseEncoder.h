#ifndef __mdkFeatureDictionaryBasedDenseEncoder_h
#define __mdkFeatureDictionaryBasedDenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedEncoder.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedDenseEncoder : public FeatureDictionaryBasedEncoder<ElementType>
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


protected:
    FeatureDictionaryBasedDenseEncoder();

public:
    virtual ~FeatureDictionaryBasedDenseEncoder();

    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    bool SetDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool LoadDictionary(const std::string& FilePathAndName);

    bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool CheckInputAndOutput();

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCode();


protected:

    int_max GetMaximunNumberOfThreads();

    int_max GetFeatureVectorNumber();

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif