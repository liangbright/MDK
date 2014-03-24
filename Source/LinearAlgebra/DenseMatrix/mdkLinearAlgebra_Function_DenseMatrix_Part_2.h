#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_2_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_2_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <complex>


#include "mdkObject.h"
//#include "mdkDenseMatrix.h"
#include "mdkMatrix_Common.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class DenseMatrix;
//-------------------------------------//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixTranspose(const DenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int64 MatrixRank(const DenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixInv(const DenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixEigenResult
{
    DenseMatrix<ElementType> EigenVector;    // eigenvector : full matrix
    DenseMatrix<ElementType> EigenValue;     // eigenvalue  : col vector

    mdkDenseMatrixEigenResult(){};

    // move constructor
    mdkDenseMatrixEigenResult(mdkDenseMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    ~mdkDenseMatrixEigenResult(){};

    void operator=(const mdkDenseMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const mdkDenseMatrixEigenResult&) = delete;
};

template<typename ElementType>
inline mdkDenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const DenseMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkDenseMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const DenseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixPCAResult
{
    DenseMatrix<ElementType> Mean;           // mean
    DenseMatrix<ElementType> EigenVector;    // eigenvector
    DenseMatrix<ElementType> EigenValue;     // col vector

    mdkDenseMatrixPCAResult(){};

    // move constructor
    mdkDenseMatrixPCAResult(mdkDenseMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    ~mdkDenseMatrixPCAResult(){};

    void operator=(const mdkDenseMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const mdkDenseMatrixPCAResult&) = delete;
};

template<typename ElementType>
inline mdkDenseMatrixPCAResult<ElementType> MatrixPCA(const DenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixSVDResult
{
    // Matrix = U*S*V;
    DenseMatrix<ElementType> U;  // matrix
    DenseMatrix<ElementType> S;  // matrix  : change to vector?
    DenseMatrix<ElementType> V;  // matrix

    mdkDenseMatrixSVDResult(){};

    // move constructor
    mdkDenseMatrixSVDResult(mdkDenseMatrixSVDResult&& Result)
    {
        // this will call copy "=" not move "="
        //U = Result.U;

        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    ~mdkDenseMatrixSVDResult(){};

    void operator=(const mdkDenseMatrixSVDResult&& Result)
    {
        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    //------------------------------------------------------
    void operator=(const mdkDenseMatrixSVDResult&) = delete;

};


template<typename ElementType>
inline mdkDenseMatrixSVDResult<ElementType> MatrixSVD(const DenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixConvolution(const DenseMatrix<ElementType>& Matrix, const DenseMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_2.hpp"

#endif
