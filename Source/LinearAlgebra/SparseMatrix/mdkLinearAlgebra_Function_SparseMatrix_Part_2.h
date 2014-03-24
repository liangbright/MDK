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
class SparseMatrix;
//-------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> MatrixTranspose(const SparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int64 MatrixRank(const SparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> MatrixInv(const SparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using SparseMatrixEigenResult = mdkDenseMatrixEigenResult<ElementType>;

template<typename ElementType>
inline SparseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const SparseMatrix<ElementType>& Matrix);

template<typename ElementType>
inline SparseMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const SparseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using SparseMatrixPCAResult = mdkDenseMatrixPCAResult<ElementType>;

template<typename ElementType>
inline SparseMatrixPCAResult<ElementType> MatrixPCA(const SparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
using SparseMatrixSVDResult = mdkDenseMatrixSVDResult<ElementType>;

template<typename ElementType>
inline SparseMatrixSVDResult<ElementType> MatrixSVD(const SparseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> MatrixConvolution(const SparseMatrix<ElementType>& Matrix, const SparseMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_Function_SparseMatrix_Part_2.hpp"

#endif
