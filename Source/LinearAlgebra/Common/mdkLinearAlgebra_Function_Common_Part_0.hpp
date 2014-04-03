#ifndef __mdkLinearAlgebra_Function_Common_Part_0_hpp
#define __mdkLinearAlgebra_Function_Common_Part_0_hpp

//#include "mdkLinearAlgebra_Function_Common_Part_0.h"


namespace mdk
{

template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix)
{
    mdkDenseMatrix<ElementType> OutputDenseMatrix;

    ConvertSparseMatrixToDenseMatrix(InputSparseMatrix, OutputDenseMatrix);
}


template<typename ElementType>
inline
bool ConvertSparseMatrixToDenseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix, DenseMatrix<ElementType>& OutputDenseMatrix)
{
    auto Size = InputSparseMatrix.GetSize();

    if (OutputDenseMatrix.FastResize(Size.RowNumber, Size.ColNumber) == false)
    {
        return false;
    }

    for (int_max k = 0; k < Size.RowNumber*Size.ColNumber; ++k)
    {
        OutputDenseMatrix[k] = InputSparseMatrix[k];
    }

    return true;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold)
{
    mdkSparseMatrix<ElementType> OutputSparseMatrix;

    return ConvertDenseMatrixToSparseMatrix(InputDenseMatrix, ElementType absThreashold, OutputSparseMatrix);
}


template<typename ElementType>
inline
bool ConvertDenseMatrixToSparseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold,
                                      SparseMatrix<ElementType>& OutputSparseMatrix )
{
    return true;
}


}// namespace mdk


#endif
