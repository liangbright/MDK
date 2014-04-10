#ifndef __mdkFeatureDictionaryBasedEncoder_h
#define __mdkFeatureDictionaryBasedEncoder_h


#include "mdkProcessObject.h"
#include "mdkFeatureDictionary.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedEncoder : public ProcessObject
{
protected:

    bool m_IsDenseEncoder;
    
protected:
    FeatureDictionaryBasedEncoder();
    virtual ~FeatureDictionaryBasedEncoder();

public:
    //-----------------------------------------

    bool IsDenseEncoder();

    bool IsSparseEncoder();

    //-----------------------------------------

    virtual void Clear() = 0;
    
    //-----------------------------------------

    virtual void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData) = 0;
    
    virtual void SetOutputCode(DenseMatrix<ElementType>* Code) = 0;

    virtual void SetMaxNumberOfThreads(int_max Number) = 0;

    //-----------------------------------------------------//

    virtual bool Update();

    //-----------------------------------------------------//

    virtual bool CheckInput() = 0;

    virtual bool Preprocess();

    virtual bool Postprocess();

    //----------------------------------------------------//

    virtual inline void EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                         DenseMatrix<ElementType>& CodeInDenseColVector) = 0;

    //----------------------------------------------------//

    virtual DenseMatrix<ElementType>* GetOutputCode() = 0;

    //----------------------------------------------------//

protected:

    virtual int_max GetTotalNumberOfInputFeatureVectors() = 0;

    virtual int_max GetMaxNumberOfThreads() = 0;

    virtual int_max GetMinNumberOfDataPerThread() = 0;

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureDataVector_start, int_max IndexOfFeatureDataVector_end) = 0;

    virtual void SetupDefaultPipelineOutput() = 0;

    virtual void UpdatePipelineOutput() = 0;

private:
//deleted:
    FeatureDictionaryBasedEncoder(const FeatureDictionaryBasedEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedEncoder.hpp"

#endif