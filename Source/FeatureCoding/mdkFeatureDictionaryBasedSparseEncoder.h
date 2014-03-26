#ifndef __mdkFeatureDictionaryBasedSparseEncoder_h
#define __mdkFeatureDictionaryBasedSparseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedEncoder.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedSparseEncoder : public FeatureDictionaryBasedEncoder<ElementType>
{
protected:

    // additional output:

    DenseMatrix<ElementType>* m_FeatureCodeInCompactFormat;

    DenseMatrix<ElementType>  m_FeatureCodeInCompactFormat_SharedCopy;
   
    // Example:
    //
    // MaxNumberOfNonZeroValue = 2
    //
    // Code in Dense Format (a column is a code vector)
    // 0 1 0 1 0
    // 1 0 1 0 0
    // 1 1 0 0 1
    // 0 0 0 1 0
    // 0 0 1 0 0
    //
    // Code in Compact Format
    // row index  1  0  1  0   2
    // value      1  1  1  1   1
    // row index  2  2  4  3  -1
    // value      1  1  1  1  -1

public:

    FeatureDictionaryBasedSparseEncoder();

    ~FeatureDictionaryBasedSparseEncoder();

    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    bool SetOutputFeatureCodeInCompactFormat(DenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    const DenseMatrix<ElementType>* GetOutputFeatureCodeInCompactFormat();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif