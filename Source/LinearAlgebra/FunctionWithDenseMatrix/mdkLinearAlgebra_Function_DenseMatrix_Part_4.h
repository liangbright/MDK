#pragma

namespace mdk
{
//---------- forward declare ---------
template<typename ElementType>
class DenseMatrix;
//-----------------------------------

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_NeighbourCount);

// find KNN with Distance <= DistanceThreshold
template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_MaxNeighbourCount, ElementType DistanceThreshold);

//------------------------------------ Similarity based metric --------------------------------------------------------------//

template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_NeighbourCount);

// find KNN with Similarity >= SimilarityThreshold
template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_MaxNeighbourCount, ElementType SimilarityThreshold);

}

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_4.hpp"
