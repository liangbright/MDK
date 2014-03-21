#ifndef __mdkLinearAlgebra_Function_SparseMatrix_Part_0_h
#define __mdkLinearAlgebra_Function_SparseMatrix_Part_0_h

#include <vector>
#include <array>
#include <memory>
#include <string>

//#include "mdkSparseMatrix.h"

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_0.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
bool Sort(const mdkSparseMatrix<ElementType>& InputDataArray, mdkSparseMatrix<ElementType>& OutputDataArray, mdkSparseMatrix<ElementType>& OutputIndexList, const std::string& Order);


}//end namespace mdk

#include "mdkLinearAlgebra_Function_SparseMatrix_Part_0.hpp"

#endif
