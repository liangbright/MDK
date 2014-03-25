#ifndef __mdkLinearAlgebra_DenseMatrix_Function_Common_Part_0_h
#define __mdkLinearAlgebra_DenseMatrix_Function_Common_Part_0_h

namespace mdk
{

template<typename ElementType>
mdkDenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

template<typename ElementType>
bool CreateDenseMatrixFromSparseMatrix(mdkDenseMatrix<ElementType>& OutputDenseMatrix, const mdkSparseMatrix<ElementType>& InputSparseMatrix);

template<typename ElementType>
mdkSparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const mdkDenseMatrix<ElementType>& InputDenseMatrix);

template<typename ElementType>
bool CreateSparseMatrixFromDenseMatrix(mdkSparseMatrix<ElementType>& OutputSparseMatrix, const mdkDenseMatrix<ElementType>& InputDenseMatrix);

}

#include "mdkLinearAlgebra_Function_Common_Part_0.hpp"

#endif
