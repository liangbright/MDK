#ifndef __mdkFeatureSparseEncoder_h
#define __mdkFeatureSparseEncoder_h

#include <string>

#include "mdkFeatureEncoder.h"

namespace mdk
{

template<typename ElementType>
class mdkFeatureSparseEncoder : public mdkFeatureEncoder<ElementType>
{
protected:

    // additional output:

    mdkDenseMatrix<ElementType>* m_FeatureCodeInCompactFormat;

    mdkDenseMatrix<ElementType>  m_FeatureCodeInCompactFormat_SharedCopy;
   
    // MaxNumberOfNonZeroValue = 2
    //
    // Code in Dense Format
    // 0 1 0 1 0
    // 1 0 1 0 0
    // 1 1 0 0 1
    // 0 0 0 1 0
    // 0 0 1 0 0
    //
    // Code in Compact Format
    // row index  1  0  2  0   2
    // value      1  1  1  1   1
    // row index  2  2  4  3  -1
    // value      1  1  1  1  -1

public:

    mdkFeatureSparseEncoder();

    ~mdkFeatureSparseEncoder();

    //-----------------------------------------

    virtual void Reset();
    
    //-----------------------------------------

    bool SetOutputFeatureCodeInCompactFormat(mdkDenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    const mdkDenseMatrix<ElementType>* GetOutputFeatureCodeInCompactFormat();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    mdkFeatureSparseEncoder(const mdkFeatureSparseEncoder&) = delete;

    void operator=(const mdkFeatureSparseEncoder&) = delete;

};

}

#include "mdkFeatureSparseEncoder.hpp"

#endif