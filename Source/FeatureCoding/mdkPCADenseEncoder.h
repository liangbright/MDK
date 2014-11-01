#ifndef __mdkPCADenseEncoder_h
#define __mdkPCADenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedDenseEncoder.h"


namespace mdk
{

template<typename Scalar_Type>
class PCADenseEncoder : public FeatureDictionaryBasedDenseEncoder<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

public:
    PCADenseEncoder();
    ~PCADenseEncoder();

public:
	DenseMatrix<ScalarType> EncodeSingleFeatureVector(const DenseMatrix<ScalarType>& FeatureVector);

protected:
	DenseMatrix<ScalarType> EncodeSingleFeatureVector(int_max DataIndex, const DenseMatrix<ScalarType>& FeatureVector, int_max ThreadIndex);

private:
    PCADenseEncoder(const PCADenseEncoder&) = delete;
    void operator=(const PCADenseEncoder&) = delete;
};

}

#include "mdkPCADenseEncoder.hpp"

#endif