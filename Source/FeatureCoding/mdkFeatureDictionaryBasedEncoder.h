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
    FeatureDictionaryBasedEncoder() {}
    virtual ~FeatureDictionaryBasedEncoder() {}

public:
    //-----------------------------------------

    virtual bool IsDenseEncoder() const = 0;

    virtual bool IsSparseEncoder() const = 0;

    //-----------------------------------------

    virtual void Clear() = 0;
    
    //-----------------------------------------

    virtual void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData) = 0;
    
    virtual void SetOutputCode(DenseMatrix<ElementType>* Code) = 0;

    virtual void SetMaxNumberOfThreads(int_max Number) = 0;

    //-----------------------------------------------------//

    virtual bool Update() = 0;

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