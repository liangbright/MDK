#ifndef __mdkCombinatorialMath_Function_Part0_h
#define __mdkCombinatorialMath_Function_Part0_h

#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"

namespace mdk
{

int_max ComputeBinomialCoefficient(int_max N, int_max K);

ObjectArray<DenseVector<int_max>> NChooseK(int_max N, int_max K);

}//name space mdk

#include "mdkCombinatorialMath_Function_Part0.hpp"

#endif