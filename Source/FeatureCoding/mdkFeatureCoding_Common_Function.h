#ifndef __mdkFeatureCoding_Common_Function_h
#define __mdkFeatureCoding_Common_Function_h

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
DenseMatrix<ElementType> ComputeListOfL2DistanceFromOneFeatureToFeatureDictionary(const DenseMatrix<ElementType>& Feature, 
                                                                                  const DenseMatrix<ElementType>& DictionaryRecord);


template<typename ElementType>
DenseMatrix<int64> FindKNNFromDistanceList(int64 K_NeighbourNumber, const DenseMatrix<ElementType>& DistanceList);

}


#endif