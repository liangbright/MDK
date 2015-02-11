#ifndef mdk_LinearAlgebra_Part_2_h
#define mdk_LinearAlgebra_Part_2_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <complex>


#include "mdkObject.h"
//#include "mdkMatrix.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkMatrix;
//-------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixTranspose(const mdkMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int64 MatrixRank(const mdkMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixInv(const mdkMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkMatrixEigenResult
{
    mdkMatrix<ElementType> EigenVector;    // eigenvector : full matrix
    mdkMatrix<ElementType> EigenValue;     // eigenvalue  : col vector

    mdkMatrixEigenResult(){};

    // move constructor
    mdkMatrixEigenResult(mdkMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue  = std::move(Result.EigenValue);
    }

    ~mdkMatrixEigenResult(){};

    void operator=(const mdkMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue  = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const mdkMatrixEigenResult&) = delete;
};


template<typename ElementType>
inline mdkMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const mdkMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkMatrixPCAResult
{
    mdkMatrix<ElementType> Mean;           // mean
    mdkMatrix<ElementType> EigenVector;    // eigenvector
    mdkMatrix<ElementType> EigenValue;     // col vector

    mdkMatrixPCAResult(){};

    // move constructor
    mdkMatrixPCAResult(mdkMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    ~mdkMatrixPCAResult(){};

    void operator=(const mdkMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const mdkMatrixPCAResult&) = delete;
};

template<typename ElementType>
inline mdkMatrixPCAResult<ElementType> MatrixPCA(const mdkMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkMatrixSVDResult
{
    // Matrix = U*S*V;
    mdkMatrix<ElementType> U;  // matrix
    mdkMatrix<ElementType> S;  // matrix  : change to vector?
    mdkMatrix<ElementType> V;  // matrix

    mdkMatrixSVDResult(){};

    // move constructor
    mdkMatrixSVDResult(mdkMatrixSVDResult&& Result)
    {
        // this will call copy "=" not move "="
        //U = Result.U;

        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    ~mdkMatrixSVDResult(){};

    void operator=(const mdkMatrixSVDResult&& Result)
    {
        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    //------------------------------------------------------
    void operator=(const mdkMatrixSVDResult&) = delete;

};

template<typename ElementType>
inline mdkMatrixSVDResult<ElementType> MatrixSVD(const mdkMatrix<ElementType>& Matrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixConvolution(const mdkMatrix<ElementType>& Matrix, const mdkMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_Part_2.hpp"

#endif
