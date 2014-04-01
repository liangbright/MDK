#ifndef __mdkFeatureCoding_Common_Function_h
#define __mdkFeatureCoding_Common_Function_h

#include "mdkFeatureDictionary.h"

#include "mdkSparseMatrix.h"

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(int_max K_NeighbourNumber, const DenseMatrix<ElementType>& DistanceList);

}

#include "mdkFeatureCoding_Common_Function.hpp"

#endif