#ifndef __mdkLinearAlgebra_Function_SparseMatrix_Part_2_h
#define __mdkLinearAlgebra_Function_SparseMatrix_Part_2_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <complex>


#include "mdkObject.h"
//#include "mdkSparseMatrix.h"

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_2.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkSparseMatrix;
//-------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixTranspose(const mdkSparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int64 MatrixRank(const mdkSparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixInv(const mdkSparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using mdkSparseMatrixEigenResult = mdkDenseMatrixEigenResult<ElementType>;

template<typename ElementType>
inline mdkSparseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const mdkSparseMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkSparseMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const mdkSparseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using mdkSparseMatrixPCAResult = mdkDenseMatrixPCAResult<ElementType>;

template<typename ElementType>
inline mdkSparseMatrixPCAResult<ElementType> MatrixPCA(const mdkSparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using mdkSparseMatrixSVDResult = mdkDenseMatrixSVDResult<ElementType>;

template<typename ElementType>
inline mdkSparseMatrixSVDResult<ElementType> MatrixSVD(const mdkSparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixConvolution(const mdkSparseMatrix<ElementType>& Matrix, const mdkSparseMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_Function_SparseMatrix_Part_2.hpp"

#endif
