#ifndef __mdkFeatureCoding_Common_Function_h
#define __mdkFeatureCoding_Common_Function_h

#include "mdkFeatureCoding_Common_Type.h"

namespace mdk
{
//---------- forward declare ---------
template<typename ElementType>
class DenseMatrix;
//-----------------------------------

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_NeighbourNumber);

// find KNN with Distance <= DistanceThreshold
template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_MaxNumberOfNeighbours, ElementType DistanceThreshold);

//------------------------------------ Similarity based metric --------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_NeighbourNumber);

// find KNN with Similarity >= SimilarityThreshold
template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_NeighbourNumber, ElementType SimilarityThreshold);

//---------------------- Compute Similarity Matrix Between Vectors Stored in DenseMatrix<ElementType> VecorSet ----------------------------//

template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, const char* SimilarityFunctionName);

template<typename ElementType>
void ComputeSimilarityMatrixOfVectorSet(DenseMatrix<ElementType> & SimilarityMatrix, 
                                        const DenseMatrix<ElementType>& VecorSet, 
                                        const char* SimilarityFunctionName);

template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, const std::string& SimilarityFunctionName);

template<typename ElementType>
void ComputeSimilarityMatrixOfVectorSet(DenseMatrix<ElementType>& SimilarityMatrix, 
                                        const DenseMatrix<ElementType>& VecorSet, 
                                        const std::string& SimilarityFunctionName);

template<typename ElementType, typename SimilarityFunctionType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, SimilarityFunctionType SimilarityFunction);


template<typename ElementType, typename SimilarityFunctionType>
void ComputeSimilarityMatrixOfVectorSet(DenseMatrix<ElementType>& SimilarityMatrix, 
                                        const DenseMatrix<ElementType>& VecorSet, 
                                        SimilarityFunctionType SimilarityFunction);


}

#include "mdkFeatureCoding_Common_Function.hpp"

#endif