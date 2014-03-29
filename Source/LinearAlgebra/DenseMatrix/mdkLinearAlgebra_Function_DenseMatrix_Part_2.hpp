#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp

//#include "mdkLinearAlgebra_DenseMatrix_Part_2.h"

namespace mdk
{

template<typename ElementType>
inline
ElementType MatrixNorm_L1(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error << "empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L1(InputMatrix)" << '\n';
        return InputMatrix.GetNaNElement();
    }

    ElementType Value = ElementType(0);

    auto BeginPointer = InputMatrix.GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Value += std::abs(Ptr[0]);
    }

    return Value;
}


template<typename ElementType>
inline
ElementType MatrixNorm_L2(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error << "empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L2(InputMatrix)" << '\n';
        return InputMatrix.GetNaNElement();
    }

    ElementType Value = ElementType(0);

    auto BeginPointer = InputMatrix.GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Value += Ptr[0] * Ptr[0];
    }

    Value = std::sqrt(Value);

    return Value;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixTranspose(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixTranspose(InputMatrix)" << '\n';

        return tempMatrix;
    }

    if (Size.ColNumber == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), 1, Size.RowNumber);

        return tempMatrix;
    }

    if (Size.RowNumber == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), Size.ColNumber, 1);

        return tempMatrix;
    }

    tempMatrix.FastResize(Size.ColNumber, Size.RowNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

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
int64 MatrixRank(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "InputMatrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixRank(InputMatrix)" << '\n';
        return 0;
    }

    auto ptrData = InputMatrix.GetElementDataSharedPointer()->data();

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    int64 value = arma::rank(tempMat);

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixInv(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixInv(InputMatrix)" << '\n';

        return tempMatrix;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixInv(InputMatrix)" << '\n';

        return tempMatrix;
    }

    tempMatrix.FastResize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::Mat<ElementType> tempInv(tempMatrix.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    tempInv = arma::inv(tempMat);

    return tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixEigenResult<std::complex<ElementType>> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "InputMatrix is not square @ mdkLinearAlgebra MatrixEigen(InputMatrix)" << '\n';

        return Result;
    }

    Result.EigenVector.FastResize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.FastResize(Size.RowNumber, 1);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Mat<std::complex<ElementType>> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Col<std::complex<ElementType>> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowNumber), false);

    arma::eig_gen(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline 
DenseMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const DenseMatrix<ElementType>& InputMatrix, bool CheckIfSymmetric = false)
{
    DenseMatrixEigenResult<ElementType> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Matrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        DenseMatrix<ElementType> tempMatrix_2 = InputMatrix - InputMatrix.Transpose();

        tempMatrix_2.ElementNamedOperationInPlace("abs");

        double tempsum = tempMatrix_2.Sum();

        if (std::abs(tempsum) > 0.00000001)
        {
            MDK_Error << "Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)" << '\n';
        }
    }
  
    Result.EigenVector.FastResize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.FastResize(Size.RowNumber, 1);

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
DenseMatrixPCAResult<ElementType> MatrixPCA(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixPCAResult<ElementType> PCAResult;

    auto Size = InputMatrix.GetSize();

    if (Size.ColNumber <= 1)
    {
        MDK_Error << "ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra MatrixPCA(Matrix)" << '\n';
        return PCAResult;
    }

    auto MeanCol = InputMatrix.SumToCol();
    MeanCol /= Size.ColNumber;

    DenseMatrix<ElementType> CovarianceMatrix(Size.RowNumber, Size.RowNumber);

    CovarianceMatrix.Fill(0);

    for (int64 i = 0; i < Size.ColNumber; ++i)
    {
        // auto tempCol = Matrix(ALL, { i }) - MeanCol;

        auto tempCol = InputMatrix.GetCol(i);
        
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
DenseMatrixSVDResult<ElementType> MatrixSVD(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixSVDResult<ElementType> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty  @ mdkLinearAlgebra MatrixSVD(InputMatrix)" << '\n';
        return Result;
    }

    Result.U.FastResize(Size.RowNumber, Size.ColNumber);
    Result.S.FastResize(Size.RowNumber, Size.ColNumber);
    Result.V.FastResize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

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
