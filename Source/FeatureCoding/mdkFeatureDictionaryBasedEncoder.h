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
    FeatureDictionaryBasedEncoder() {}
    virtual ~FeatureDictionaryBasedEncoder() {}

public:
    //-----------------------------------------

    bool IsDenseEncoder() { return m_IsDenseEncoder };

    bool IsSparseEncoder() { return !m_IsDenseEncoder };

    //-----------------------------------------

    virtual void Clear() = 0;
    
    //-----------------------------------------

    virtual void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData) = 0;
    
    virtual void SetOutputCode(DenseMatrix<ElementType>* Code) = 0;

    virtual void SetMaxNumberOfThreads(int_max Number) = 0;

    //-----------------------------------------------------//

    virtual bool Update() = 0;

    //----------------------------------------------------//

    virtual inline void EncodingFunction(DenseMatrix<ElementType>& CodeInDenseColVector,
                                         const DenseMatrix<ElementType>& DataColVector,
                                         int_max ThreadIndex) = 0;

    //----------------------------------------------------//

    virtual DenseMatrix<ElementType>* GetOutputCode() = 0;

    //----------------------------------------------------//

private:
//deleted:
    FeatureDictionaryBasedEncoder(const FeatureDictionaryBasedEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedEncoder&) = delete;

};

}

#endif