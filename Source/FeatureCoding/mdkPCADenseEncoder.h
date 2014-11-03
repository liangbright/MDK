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
	inline DenseMatrix<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector);

protected:
	inline DenseMatrix<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex);

private:
    PCADenseEncoder(const PCADenseEncoder&) = delete;
    void operator=(const PCADenseEncoder&) = delete;
};

}

#include "mdkPCADenseEncoder.hpp"

#endif