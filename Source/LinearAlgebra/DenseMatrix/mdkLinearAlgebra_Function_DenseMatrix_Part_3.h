#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h

#include <string>
#include <limits>


#include <Eigen/Dense>

#include "mdkType.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class DenseMatrix;
//-------------------------------------//

// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and ColVectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeL2DistanceListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                             const DenseMatrix<ElementType>& ColVectorSet);

template<typename ElementType>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput = true);


template<typename ElementType>
DenseMatrix<ElementType> ComputeL1DistanceListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                             const DenseMatrix<ElementType>& ColVectorSet);

template<typename ElementType>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput = true);


// sum((x-mean(x)).*(y-mean(y))/(sum((x-mean(x)).^2)*sum((y-mean(y)).^2))
template<typename ElementType>
DenseMatrix<ElementType> ComputeCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                              const DenseMatrix<ElementType>& ColVectorSet);

template<typename ElementType>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput = true);

// sum(x.*y)/(sum(x.^2)*sum(y.^2))
// UncenteredCorrelation = angle between two vectors
template<typename ElementType>
DenseMatrix<ElementType> ComputeUncenteredCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                                        const DenseMatrix<ElementType>& ColVectorSet);

template<typename ElementType>
inline
ElementType ComputeUncenteredCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeUncenteredCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length);

// sum((x-mean(x)).*(y-mean(y))
template<typename ElementType>
DenseMatrix<ElementType> ComputeUnnormalizedCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                                          const DenseMatrix<ElementType>& ColVectorSet);

template<typename ElementType>
inline
ElementType ComputeUnnormalizedCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeUnnormalizedCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput = true);



template<typename ElementType>
DenseMatrix<ElementType> ComputeKLDivergenceListOfSingleVectorFromColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                               const DenseMatrix<ElementType>& ColVectorSet);

// D(A||B), B is considered as "ground truth", basis  
template<typename ElementType>
inline
ElementType ComputeKLDivergenceOfVectorAFromVectorB(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeKLDivergenceOfVectorAFromVectorB(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput = true);


}

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.hpp"

#endif