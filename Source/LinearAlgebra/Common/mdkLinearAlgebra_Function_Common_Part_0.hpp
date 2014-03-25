#ifndef __mdkLinearAlgebra_DenseMatrix_Function_Common_Part_0_hpp
#define __mdkLinearAlgebra_DenseMatrix_Function_Common_Part_0_hpp

//#include "mdkLinearAlgebra_Function_Common_Part_0.h"


namespace mdk
{

template<typename ElementType>
mdkDenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const mdkSparseMatrix<ElementType>& InputSparseMatrix)
{
    mdkDenseMatrix<ElementType> OutputDenseMatrix;

    CreateDenseMatrixFromSparseMatrix(OutputDenseMatrix, InputSparseMatrix);
}


template<typename ElementType>
bool CreateDenseMatrixFromSparseMatrix(mdkDenseMatrix<ElementType>& OutputDenseMatrix, const mdkSparseMatrix<ElementType>& InputSparseMatrix)
{
    return true;
}


template<typename ElementType>
mdkSparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const mdkDenseMatrix<ElementType>& InputDenseMatrix)
{
    mdkSparseMatrix<ElementType> OutputSparseMatrix;

    return CreateSparseMatrixFromDenseMatrix(OutputSparseMatrix, InputDenseMatrix);
}


template<typename ElementType>
bool CreateSparseMatrixFromDenseMatrix(mdkSparseMatrix<ElementType>& OutputSparseMatrix, const mdkDenseMatrix<ElementType>& InputDenseMatrix)
{
    return true;
}


}// namespace mdk


#endif
