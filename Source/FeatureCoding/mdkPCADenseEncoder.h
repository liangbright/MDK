#ifndef __mdkPCADenseEncoder_h
#define __mdkPCADenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedDenseEncoder.h"


namespace mdk
{

template<typename Element_Type>
class PCADenseEncoder : public FeatureDictionaryBasedDenseEncoder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    PCADenseEncoder();
    ~PCADenseEncoder();

public:    
    void EncodingFunction(DenseMatrix<ElementType>& CodeInDenseColVector, const DenseMatrix<ElementType>& DataColVector, int_max ThreadIndex);

private:
    PCADenseEncoder(const PCADenseEncoder&) = delete;
    void operator=(const PCADenseEncoder&) = delete;
};

}

#include "mdkPCADenseEncoder.hpp"

#endif