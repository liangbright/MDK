#ifndef __mdkLinearAlgebra_Part_2_hpp
#define __mdkLinearAlgebra_Part_2_hpp

#include "mdkLinearAlgebra_Part_2.h"

namespace mdk
{

template<typename ElementType>
inline 
uint64 MatrixRank(const mdkMatrix<ElementType>& Matrix)
{
    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        mdkError << "Matrix is empty  @ mdkLinearAlgebra MatrixRank(Matrix)" << '\n';
        return 0;
    }

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(RowNumber), arma::uword(ColNumber), false);

    uint64 value = arma::rank(tempMat);

    return value;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixInv(const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrix<ElementType> tempMatrix;

    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    if (RowNumber != ColNumber)
    {
        mdkError << "Matrix is not squre @ mdkLinearAlgebra MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    tempMatrix.SetSize(RowNumber, ColNumber);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(RowNumber), arma::uword(ColNumber), false);

    arma::Mat<ElementType> tempInv(tempMatrix.GetElementDataRawPointer(), arma::uword(RowNumber), arma::uword(ColNumber), false);

    tempInv = arma::inv(tempMat);

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrixEigenResult<std::complex<ElementType>> Result;

    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (RowNumber != ColNumber)
    {
        mdkError << "Matrix is not squre @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    Result.EigenVector.SetSize(RowNumber, RowNumber);

    Result.EigenValue.SetSize(RowNumber, 1);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(RowNumber), arma::uword(RowNumber), false);

    arma::Mat<std::complex<ElementType>> tempEigenVector(Result.EigenVector.GetElementDataRawPointer(), arma::uword(RowNumber), arma::uword(RowNumber), false);

    arma::Col<std::complex<ElementType>> tempEigenValue(Result.EigenValue.GetElementDataRawPointer(), arma::uword(RowNumber), false);

    arma::eig_gen(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline 
mdkMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const mdkMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false)
{
    mdkMatrixEigenResult<ElementType> Result;

    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (RowNumber != ColNumber)
    {
        mdkError << "Matrix is not squre @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        mdkMatrix<ElementType> tempMatrix_2 = Matrix - Matrix.Transpose();

        tempMatrix_2.ElementOperationInPlace("abs");

        double tempsum = tempMatrix_2.Sum();

        if (std::abs(tempsum) > 0.00000001)
        {
            mdkError << "Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';
        }
    }
  
    Result.EigenVector.SetSize(RowNumber, RowNumber);

    Result.EigenValue.SetSize(RowNumber, 1);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(RowNumber), arma::uword(RowNumber), false);

    arma::Mat<ElementType> tempEigenVector(Result.EigenVector.GetElementDataRawPointer(), arma::uword(RowNumber), arma::uword(RowNumber), false);

    arma::Col<ElementType> tempEigenValue(Result.EigenValue.GetElementDataRawPointer(), arma::uword(RowNumber), false);

    arma::eig_sym(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline
mdkMatrixPCAResult<ElementType> MatrixPCA(const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrixPCAResult<ElementType> PCAResult;

    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (ColNumber <= 1)
    {
        mdkError << "ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra MatrixPCA(Matrix)" << '\n';
        return PCAResult;
    }

    auto MeanCol = Matrix.SumToCol();
    MeanCol /= ColNumber;

    mdkMatrix<ElementType> CovarianceMatrix(RowNumber, RowNumber);

    CovarianceMatrix.Fill(0);

    for (uint64 i = 0; i < ColNumber; ++i)
    {
        // auto tempCol = Matrix(ALL, { i }) - MeanCol;

        auto tempCol = Matrix.GetCol(i);
        
        tempCol -= MeanCol;

        CovarianceMatrix += tempCol * tempCol.Transpose();
    }

    CovarianceMatrix /= ColNumber;

    auto EigenResult = SymmetricRealMatrixEigen(CovarianceMatrix);

    //--------------------------------------------------------------//

    PCAResult.Mean = std::move(MeanCol);

    PCAResult.EigenVector = std::move(EigenResult.EigenVector);

    PCAResult.EigenValue = std::move(EigenResult.EigenValue);

    return PCAResult;
}


template<typename ElementType>
inline
mdkMatrixSVDResult<ElementType> MatrixSVD(const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrixSVDResult<ElementType> Result;

    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        mdkError << "Matrix is empty  @ mdkLinearAlgebra MatrixSVD(Matrix)" << '\n';
        return Result;
    }

    Result.U.SetSize(RowNumber, ColNumber);
    Result.S.SetSize(RowNumber, ColNumber);
    Result.V.SetSize(RowNumber, ColNumber);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> X(ptrData, arma::uword(RowNumber), arma::uword(ColNumber), false);

    arma::Mat<ElementType> U(Result.U.GetElementDataRawPointer(), arma::uword(RowNumber), arma::uword(ColNumber), false);
    arma::Col<ElementType> S;
    arma::Mat<ElementType> V(Result.V.GetElementDataRawPointer(), arma::uword(RowNumber), arma::uword(ColNumber), false);

    arma::svd(U, S, V, X);

    Result.S.SetDiangonal(S.memptr());

    return Result;
}

}//end namespace mdk

#endif