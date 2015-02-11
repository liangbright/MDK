#ifndef mdk_LinearAlgebra_Function_SparseMatrix_Part_2_hpp
#define mdk_LinearAlgebra_Function_SparseMatrix_Part_2_hpp


namespace mdk
{

template<typename ElementType>
inline 
SparseMatrix<ElementType> MatrixTranspose(const SparseMatrix<ElementType>& Matrix)
{
    SparseMatrix<ElementType> tempMatrix;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty @ mdkLinearAlgebra_SparseMatrix MatrixTranspose(Matrix)" << '\n';

        return tempMatrix;
    }

    if (Size.ColNumber == 1)
    {
        tempMatrix.Copy(Matrix.GetElementPointer(), 1, Size.RowNumber);

        return tempMatrix;
    }

    if (Size.RowNumber == 1)
    {
        tempMatrix.Copy(Matrix.GetElementPointer(), Size.ColNumber, 1);

        return tempMatrix;
    }

//-----------------------------------

    return tempMatrix;
}


template<typename ElementType>
inline 
int64 MatrixRank(const SparseMatrix<ElementType>& Matrix)
{
    auto RowNumber = Matrix.GetRowNumber();

    auto ColNumber = Matrix.GetColNumber();

    if (RowNumber == 0)
    {
        MDK_Error << "Matrix is empty  @ mdkLinearAlgebra_SparseMatrix MatrixRank(Matrix)" << '\n';
        return 0;
    }

    // call Armadillo 

    int64 value = 0;

    return value;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> MatrixInv(const SparseMatrix<ElementType>& Matrix)
{
    SparseMatrix<ElementType> tempMatrix;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty matrix @ mdkLinearAlgebra_SparseMatrix MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Matrix is not square @ mdkLinearAlgebra_SparseMatrix MatrixINV(Matrix)" << '\n';

        return tempMatrix;
    }

    //------------------------------------------------------

    return tempMatrix;
}


template<typename ElementType>
inline 
SparseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const SparseMatrix<ElementType>& Matrix)
{
    SparseMatrixEigenResult<std::complex<ElementType>> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty matrix @ mdkLinearAlgebra_SparseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Matrix is not square @ mdkLinearAlgebra_SparseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    Result.EigenVector.Resize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.Resize(Size.RowNumber, 1);

    //--------------------------------------------------------------------
    auto ptrData = Matrix.GetElementDataSharedPointer()->data();

    return Result;
}


template<typename ElementType>
inline 
SparseMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const SparseMatrix<ElementType>& Matrix, bool CheckIfSymmetric = false)
{
    SparseMatrixEigenResult<ElementType> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty matrix @ mdkLinearAlgebra_SparseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Matrix is not square @ mdkLinearAlgebra_SparseMatrix MatrixEigen(Matrix)" << '\n';

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        SparseMatrix<ElementType> tempMatrix_2 = Matrix - Matrix.Transpose();

        tempMatrix_2.ElementOperationInPlace("abs");

        double tempsum = tempMatrix_2.Sum();

        if (std::abs(tempsum) > 0.00000001)
        {
            MDK_Error << "Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra_SparseMatrix MatrixEigen(Matrix)" << '\n';
        }
    }
  
    Result.EigenVector.Resize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.Resize(Size.RowNumber, 1);

    //---------------------------------------------------------------------------


    return Result;
}


template<typename ElementType>
inline
SparseMatrixPCAResult<ElementType> MatrixPCA(const SparseMatrix<ElementType>& Matrix)
{
    SparseMatrixPCAResult<ElementType> PCAResult;

    auto Size = Matrix.GetSize();

    if (Size.ColNumber <= 1)
    {
        MDK_Error << "ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra_SparseMatrix MatrixPCA(Matrix)" << '\n';
        return PCAResult;
    }

    auto MeanCol = Matrix.SumToCol();
    MeanCol /= Size.ColNumber;

    SparseMatrix<ElementType> CovarianceMatrix(Size.RowNumber, Size.RowNumber);

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
SparseMatrixSVDResult<ElementType> MatrixSVD(const SparseMatrix<ElementType>& Matrix)
{
    SparseMatrixSVDResult<ElementType> Result;

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error << "Matrix is empty  @ mdkLinearAlgebra_SparseMatrix MatrixSVD(Matrix)" << '\n';
        return Result;
    }

    Result.U.Resize(Size.RowNumber, Size.ColNumber);
    Result.S.Resize(Size.RowNumber, Size.ColNumber);
    Result.V.Resize(Size.RowNumber, Size.ColNumber);

    //----------------------------------------------------------------------

    return Result;
}

}//end namespace mdk

#endif
