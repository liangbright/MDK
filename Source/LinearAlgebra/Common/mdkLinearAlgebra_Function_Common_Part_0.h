#pragma once

namespace mdk
{
//----------- forward declare ----------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class SparseMatrix;

template<typename ElementType>
class SparseVector;
//-----------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix);

template<typename ElementType>
inline
bool ConvertSparseMatrixToDenseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix, DenseMatrix<ElementType>& OutputDenseMatrix);

template<typename ElementType>
inline
SparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold);

template<typename ElementType>
inline
bool ConvertDenseMatrixToSparseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold,
                                      SparseMatrix<ElementType>& OutputSparseMatrix);
}

#include "mdkLinearAlgebra_Function_Common_Part_0.hpp"
