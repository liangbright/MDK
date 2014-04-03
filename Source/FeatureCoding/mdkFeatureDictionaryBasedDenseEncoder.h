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

    //-----------------------------------------

    virtual inline void EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                         const FeatureDictionary<ElementType>& InputDictionary,
                                         DenseMatrix<ElementType>& SingleFeatureCode) {}

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCode();


protected:

    int_max GetMaximunNumberOfThreads();

    int_max GetFeatureVectorNumber();

    void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif