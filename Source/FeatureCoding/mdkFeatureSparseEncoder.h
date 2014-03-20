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

    mdkMatrix<ElementType>* m_FeatureSparseCode;

    mdkMatrix<ElementType> m_FeatureSparseCode_SharedCopy;


public:

    mdkFeatureSparseEncoder();

    ~mdkFeatureSparseEncoder();

    //-----------------------------------------

    virtual void Reset();
    
    //-----------------------------------------

    bool SetOutputFeatureSparseCode(mdkMatrix<ElementType>* FeatureSparseCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    const mdkMatrix<ElementType>* GetOutputFeatureSparseCode();


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