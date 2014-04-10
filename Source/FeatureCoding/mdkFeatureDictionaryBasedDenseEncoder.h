#ifndef __mdkFeatureDictionaryBasedDenseEncoder_h
#define __mdkFeatureDictionaryBasedDenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryForDenseCoding.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedDenseEncoder : public FeatureDictionaryBasedEncoder<ElementType>
{
protected:

    // input:

    const DenseMatrix<ElementType>* m_FeatureData;

    // input 

    const FeatureDictionaryForDenseCoding<ElementType>* m_Dictionary;

    // output:

    DenseMatrix<ElementType>* m_Code;

    // other
    int_max m_MaxNumberOfThreads;

    int_max m_MinNumberOfDataPerThread;

private:
    DenseMatrix<ElementType> m_Code_SharedCopy;

protected:
    FeatureDictionaryBasedDenseEncoder();
    virtual ~FeatureDictionaryBasedDenseEncoder();

public:
    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInputDictionary(const FeatureDictionaryForDenseCoding<ElementType>* Dictionary);

    void SetOutputCode(DenseMatrix<ElementType>* Code);

    void SetMaxNumberOfThreads(int_max Number);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputCode();

protected:

    int_max GetMaximunNumberOfThreads();

    int_max GetMinNumberOfDataPerThread();

    int_max GetFeatureVectorNumber();

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif