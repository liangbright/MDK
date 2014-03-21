#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp

//#include "mdkLinearAlgebra_DenseMatrix_Part_2.h"

namespace mdk
{

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixTranspose(const mdkDenseMatrix<ElementType>& Matrix)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "Matrix is empty @ mdkLinearAlgebra MatrixTranspose(Matrix)" << '\n';

        return tempMatrix;
    }

    if (Size.ColNumber == 1)
    {
        tempMatrix.DeepCopy(Matrix.GetElementPointer(), 1, Size.RowNumber);

        return tempMatrix;
    }

    if (Size.RowNumber == 1)
    {
        tempMatrix.DeepCopy(Matrix.GetElementPointer(), Size.ColNumber, 1);

        return tempMatrix;
    }

    tempMatrix.Resize(Size.ColNumber, Size.RowNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = Matrix.GetElementPointer();

    for (int64 i = 0; i < Size.RowNumber; ++i)
    {
        int64 Index = 0;

        for (int64 j = 0; j < Size.ColNumber; ++j)
        {
            tempRawPointer[0] = RawPointer[Index + i];

            Index += Size.RowNumber;

            ++tempRawPointer;
        }
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
int64 MatrixRank(const mdkDenseMatrix<ElementType>& Matrix)
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

    int64 value = arma::rank(tempMat);

    return value;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixInv(const mdkDenseMatrix<ElementType>& Matrix)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Matrix is not square @ mdkLinearAlgebra MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    tempMatrix.Resize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(Matrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::Mat<ElementType> tempInv(tempMatrix.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    tempInv = arma::inv(tempMat);

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkDenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const mdkDenseMatrix<ElementType>& Matrix)
{
    mdkDenseMatrixEigenResult<std::complex<ElementType>> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Matrix is not square @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    Result.EigenVector.Resize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.Resize(Size.RowNumber, 1);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Mat<std::complex<ElementType>> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Col<std::complex<ElementType>> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowNumber), false);

    arma::eig_gen(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline 
mdkDenseMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const mdkDenseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false)
{
    mdkDenseMatrixEigenResult<ElementType> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "Matrix is empty matrix @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Matrix is not square @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        mdkDenseMatrix<ElementType> tempMatrix_2 = Matrix - Matrix.Transpose();

        tempMatrix_2.ElementOperationInPlace("abs");

        double tempsum = tempMatrix_2.Sum();

        if (std::abs(tempsum) > 0.00000001)
        {
            mdkError << "Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra MatrixEigen(Matrix)" << '\n';
        }
    }
  
    Result.EigenVector.Resize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.Resize(Size.RowNumber, 1);

    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Mat<ElementType> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Col<ElementType> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowNumber), false);

    arma::eig_sym(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline
mdkDenseMatrixPCAResult<ElementType> MatrixPCA(const mdkDenseMatrix<ElementType>& Matrix)
{
    mdkDenseMatrixPCAResult<ElementType> PCAResult;

    auto Size = Matrix.GetSize();

    if (Size.ColNumber <= 1)
    {
        mdkError << "ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra MatrixPCA(Matrix)" << '\n';
        return PCAResult;
    }

    auto MeanCol = Matrix.SumToCol();
    MeanCol /= Size.ColNumber;

    mdkDenseMatrix<ElementType> CovarianceMatrix(Size.RowNumber, Size.RowNumber);

    CovarianceMatrix.Fill(0);

    for (int64 i = 0; i < Size.ColNumber; ++i)
    {
        // auto tempCol = Matrix(ALL, { i }) - MeanCol;

        auto tempCol = Matrix.GetCol(i);
        
        tempCol -= MeanCol;

        CovarianceMatrix += tempCol * tempCol.Transpose();
    }

    CovarianceMatrix /= Size.ColNumber;

    auto EigenResult = SymmetricRealMatrixEigen(CovarianceMatrix);

    //--------------------------------------------------------------//

    PCAResult.Mean = std::move(MeanCol);

    PCAResult.EigenVector = std::move(EigenResult.EigenVector);

    PCAResult.EigenValue = std::move(EigenResult.EigenValue);

    return PCAResult;
}


template<typename ElementType>
inline
mdkDenseMatrixSVDResult<ElementType> MatrixSVD(const mdkDenseMatrix<ElementType>& Matrix)
{
    mdkDenseMatrixSVDResult<ElementType> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "Matrix is empty  @ mdkLinearAlgebra MatrixSVD(Matrix)" << '\n';
        return Result;
    }

    Result.U.Resize(Size.RowNumber, Size.ColNumber);
    Result.S.Resize(Size.RowNumber, Size.ColNumber);
    Result.V.Resize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(Matrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> X(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::Mat<ElementType> U(Result.U.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
    arma::Col<ElementType> S;
    arma::Mat<ElementType> V(Result.V.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::svd(U, S, V, X);

    Result.S.SetDiangonal(S.memptr());

    return Result;
}

}//end namespace mdk

#endif
