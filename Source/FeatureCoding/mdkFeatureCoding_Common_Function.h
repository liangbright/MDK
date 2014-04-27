#ifndef __mdkFeatureCoding_Common_Function_h
#define __mdkFeatureCoding_Common_Function_h

#include "mdkFeatureDictionary.h"

#include "mdkSparseMatrix.h"

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(int_max K_NeighbourNumber, const DenseMatrix<ElementType>& DistanceList);


//------------------------------------ Similarity based metric --------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(int_max K_NeighbourNumber, const DenseMatrix<ElementType>& SimilarityList);

//---------------------- Compute Similarity Matrix Between Vectors Stored in DenseMatrix<ElementType> VecorSet ----------------------------//

template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVecorSet(const DenseMatrix<ElementType>& VecorSet, const char* SimilarityFunctionName);

template<typename ElementType>
void ComputeSimilarityMatrixOfVecorSet(DenseMatrix<ElementType> & SimilarityMatrix, 
                                       const DenseMatrix<ElementType>& VecorSet, 
                                       const char* SimilarityFunctionName);

template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVecorSet(const DenseMatrix<ElementType>& VecorSet, const std::string& SimilarityFunctionName);

template<typename ElementType>
void ComputeSimilarityMatrixOfVecorSet(DenseMatrix<ElementType>& SimilarityMatrix, 
                                       const DenseMatrix<ElementType>& VecorSet, 
                                       const std::string& SimilarityFunctionName);

template<typename ElementType, typename SimilarityFunctionType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVecorSet(const DenseMatrix<ElementType>& VecorSet, SimilarityFunctionType SimilarityFunction);


template<typename ElementType, typename SimilarityFunction>
void ComputeSimilarityMatrixOfVecorSet(DenseMatrix<ElementType>& SimilarityMatrix, 
                                       const DenseMatrix<ElementType>& VecorSet, 
                                       SimilarityFunctionType SimilarityFunction);

}

#include "mdkFeatureCoding_Common_Function.hpp"

#endif