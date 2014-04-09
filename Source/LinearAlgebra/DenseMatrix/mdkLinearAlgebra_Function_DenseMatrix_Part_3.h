#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h

#include <string>

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



}

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.hpp"

#endif