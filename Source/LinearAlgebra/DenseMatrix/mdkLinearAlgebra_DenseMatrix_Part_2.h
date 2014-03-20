#ifndef __mdkLinearAlgebra_DenseMatrix_Part_2_h
#define __mdkLinearAlgebra_DenseMatrix_Part_2_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <complex>


#include "mdkObject.h"
//#include "mdkDenseMatrix.h"
#include "mdkMatrixCommon.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkDenseMatrix;
//-------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixTranspose(const mdkDenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int64 MatrixRank(const mdkDenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixInv(const mdkDenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixEigenResult
{
    mdkDenseMatrix<ElementType> EigenVector;    // eigenvector : full matrix
    mdkDenseMatrix<ElementType> EigenValue;     // eigenvalue  : col vector

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
inline mdkDenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const mdkDenseMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkDenseMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const mdkDenseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixPCAResult
{
    mdkDenseMatrix<ElementType> Mean;           // mean
    mdkDenseMatrix<ElementType> EigenVector;    // eigenvector
    mdkDenseMatrix<ElementType> EigenValue;     // col vector

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
inline mdkDenseMatrixPCAResult<ElementType> MatrixPCA(const mdkDenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixSVDResult
{
    // Matrix = U*S*V;
    mdkDenseMatrix<ElementType> U;  // matrix
    mdkDenseMatrix<ElementType> S;  // matrix  : change to vector?
    mdkDenseMatrix<ElementType> V;  // matrix

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
inline mdkDenseMatrixSVDResult<ElementType> MatrixSVD(const mdkDenseMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixConvolution(const mdkDenseMatrix<ElementType>& Matrix, const mdkDenseMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_DenseMatrix_Part_2.hpp"

#endif
