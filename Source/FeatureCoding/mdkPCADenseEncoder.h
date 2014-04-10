#ifndef __mdkPCADenseEncoder_h
#define __mdkPCADenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedDenseEncoder.h"


namespace mdk
{

template<typename ElementType>
class PCADenseEncoder : public FeatureDictionaryBasedDenseEncoder<ElementType>
{

protected:
    PCADenseEncoder();
    ~PCADenseEncoder();

public:
    
    void EncodingFunction(const DenseMatrix<ElementType>& DataColVector,  DenseMatrix<ElementType>& CodeInDenseColVector);

private:
//deleted:
    PCADenseEncoder(const PCADenseEncoder&) = delete;

    void operator=(const PCADenseEncoder&) = delete;

};

}

#include "mdkPCADenseEncoder.hpp"

#endif