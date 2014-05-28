#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp



namespace mdk
{

//================================================================================================================================//

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, InputMatrix.GetElementNumber(), 0, InputMatrix.GetElementNumber(), MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, MaxOutputNumber, 0, InputMatrix.GetElementNumber(), MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, 
                                         int_max LinearIndex_start, int_max LinearIndex_end, MatchFunctionType MatchFunction)
{
    DenseMatrix<int_max> LinearIndexList;

    auto InputElementNumber = InputMatrix.GetElementNumber();

    if (MaxOutputNumber <= 0 || MaxOutputNumber > InputElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (LinearIndex_start < 0 || LinearIndex_start >= InputElementNumber || LinearIndex_start > LinearIndex_end)
    {
        MDK_Error("LinearIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (LinearIndex_end < 0 || LinearIndex_end >= InputElementNumber)
    {
        MDK_Error("LinearIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (InputElementNumber == 0)
    {
        return LinearIndexList;
    }

    if (LinearIndex_start == LinearIndex_end)
    {
        LinearIndex_end.AppendCol({ LinearIndex_start });
        return LinearIndexList;
    }

    LinearIndexList.ReserveCapacity(MaxOutputNumber);

    for (int_max i = LinearIndex_start; i <= LinearIndex_end; ++i)
    {
        if (MatchFunction(InputMatrix[i]) == true)
        {
            LinearIndexList.AppendCol({ i });

            auto CurrentNumber = LinearIndexList.GetElementNumber();

            if (CurrentNumber == MaxOutputNumber)
            {
                break;
            }
        }
    }

    return LinearIndexList;
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, InputMatrix.GetColNumber(), 0, InputMatrix.GetColNumber(), MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, MaxOutputColNumber, 0, InputMatrix.GetColNumber(), MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber, 
                                     int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction)
{
    DenseMatrix<int_max> ColIndexList;

    auto InputSize = InputMatrix.GetSize();

    if (MaxOutputColNumber <= 0 || MaxOutputColNumber > InputSize.ColNumber)
    {
        MDK_Error("MaxOutputColNumber is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColNumber || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColNumber)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColNumber == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.AppendCol({ ColIndex_start });
        return ColIndexList;
    }

    ColIndexList.ReserveCapacity(MaxOutputColNumber);

    DenseMatrix<ElementType> ColVector;

    for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
    {
        ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowNumber, 1);

        if (MatchFunction(ColVector) == true)
        {
            ColIndexList.AppendCol({ i });

            auto CurrentNumber = ColIndexList.GetElementNumber();

            if (CurrentNumber == MaxOutputColNumber)
            {
                break;
            }
        }
    }

    return ColIndexList;
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix, CompareFunctionType CompareFunction)
{
    return SortColInMatrix(InputMatrix, 0, InputMatrix.GetColNumber(), CompareFunction);
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix, 
                                     int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction)
{
    DenseMatrix<int_max> ColIndexList;

    auto InputSize = InputMatrix.GetSize();

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColNumber || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColNumber)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColNumber == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.AppendCol({ ColIndex_start });
        return ColIndexList;
    }

    ColIndexList.FastResize(1, InputSize.ColNumber);

    for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
    {
        ColIndexList[i] = i;
    }

    DenseMatrix<ElementType> ColVector_a, ColVector_b;

    std::sort(ColIndexList.begin(), ColIndexList.end(), [&](int_max a, int_max b)
    {
        ColVector_a.ForceShare(InputMatrix.GetElementPointerOfCol(a), InputSize.RowNumber, 1);
        ColVector_b.ForceShare(InputMatrix.GetElementPointerOfCol(b), InputSize.RowNumber, 1);
        return CompareFunction(ColVector_a, ColVector_b);
    });

    return ColIndexList;
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(DenseMatrix<ElementType>& OutputMatrix, const DenseMatrix<ElementType>& InputMatrix, CompareFunctionType CompareFunction)
{
    auto ColIndexList = SortColInMatrix(InputMatrix, CompareFunction);

    if (ColIndexList.IsEmpty() == true)
    {
        return ColIndexList;
    }

    OutputMatrix = InputMatrix(ALL, ColIndexList);

    return ColIndexList;
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(DenseMatrix<ElementType>& OutputMatrix, const DenseMatrix<ElementType>& InputMatrix, 
                                     int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction)
{
    auto ColIndexList = SortColInMatrix(InputMatrix, ColIndex_start, ColIndex_end, CompareFunction);

    if (ColIndexList.IsEmpty() == true)
    {
        return ColIndexList;
    }

    OutputMatrix = InputMatrix(span(ColIndex_start, ColIndex_end), ColIndexList);

    return ColIndexList;
}

//================================================================================================================================//

template<typename ElementType>
inline
ElementType MatrixMean(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber <= 0)
    {
        MDK_Error("Input is an empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMean(InputMatrix)")
        return InputMatrix.GetNaNElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

    value /= ElementType(ElementNumber);

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
        {
            value += RawPointer[0];

            ++RawPointer;
        }

        tempRawPointer[j] = value / ElementType(InputSize.RowNumber);
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[0];

        int_max Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value / ElementType(InputSize.ColNumber);
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixMax(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMax(InputMatrix)")
        return InputMatrix.GetNaNElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Input_ElementNumber; ++Ptr)
    {
        value = std::max(value, Ptr[0]);
    }

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
        {
            value = std::max(value, RawPointer[0]);

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value = std::max(value, RawPointer[Index + i]);

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixMin(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMin(InputMatrix)")
        return InputMatrix.GetNaNElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Input_ElementNumber; ++Ptr)
    {
        value = std::min(value, Ptr[0]);
    }

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
        {
            value = std::min(value, RawPointer[0]);

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value = std::min(value, RawPointer[Index + i]);

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixSum(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSum(InputMatrix)")
        return InputMatrix.GetNaNElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Input_ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("InputMatrix is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
        {
            value += RawPointer[0];

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        int_max Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


//===================================================================================================================================//

template<typename ElementType>
inline
ElementType MatrixNorm_L1(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L1(InputMatrix)")
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
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L2(InputMatrix)")
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
        MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixTranspose(InputMatrix)")

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

    for (int_max i = 0; i < Size.RowNumber; ++i)
    {
        int_max Index = 0;

        for (int_max j = 0; j < Size.ColNumber; ++j)
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
void MatrixTransposeInPlace(DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        return;
    }

    if (Size.ColNumber == 1 || Size.RowNumber == 1)
    {
        InputMatrix.Reshape(Size.ColNumber, Size.RowNumber);
        return;
    }

    //--------------------- call armadillo

    arma::Mat<ElementType> A(InputMatrix.GetElementPointer(), arma::uword(InputMatrix.GetRowNumber()), arma::uword(InputMatrix.GetColNumber()), false);

    arma::inplace_trans(A);

    InputMatrix.Reshape(Size.ColNumber, Size.RowNumber);
}


template<typename ElementType>
inline 
int_max MatrixRank(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error("InputMatrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixRank(InputMatrix)")
        return 0;
    }

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    int_max value = arma::rank(tempMat);

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
        MDK_Error("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixInv(InputMatrix)")

        return tempMatrix;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixInv(InputMatrix)")

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
        MDK_Error("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

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
        MDK_Error("Matrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("Matrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        DenseMatrix<ElementType> tempMatrix_2 = InputMatrix - InputMatrix.Transpose();

        tempMatrix_2.ElementOperationInPlace("abs");

        double tempsum = double(tempMatrix_2.Sum());

        if (std::abs(tempsum) > EPS(tempsum))
        {
            MDK_Error("Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")
        }
    }
  
    Result.EigenVector.FastResize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.FastResize(Size.RowNumber, 1);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

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
        MDK_Error("ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra_DenseMatrix MatrixPCA(...)")
        return PCAResult;
    }

    auto MeanCol = InputMatrix.SumToCol();
    MeanCol /= Size.ColNumber;

    DenseMatrix<ElementType> CovarianceMatrix(Size.RowNumber, Size.RowNumber);

    CovarianceMatrix.Fill(0);

    // reference only
    //for (int_max i = 0; i < Size.ColNumber; ++i)
    //{
    //    DenseMatrix<ElementType> tempCol = InputMatrix(ALL, { i }) - MeanCol;
       
    //    CovarianceMatrix += tempCol * tempCol.Transpose();
    //}

    DenseMatrix<ElementType> tempMatrix(Size.RowNumber, Size.RowNumber);

    DenseMatrix<ElementType> tempCol(Size.RowNumber, 1);

    DenseMatrix<ElementType> tempColTranspose(1, Size.RowNumber);

    for (int_max i = 0; i < Size.ColNumber; ++i)
    {
        InputMatrix.GetCol(i, tempCol);

        MatrixSubtract(tempCol, tempCol, MeanCol);

        tempColTranspose = tempCol.Transpose();

        MatrixMultiply(tempMatrix, tempCol, tempColTranspose);

        CovarianceMatrix += tempMatrix;
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
        MDK_Error("Matrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixSVD(InputMatrix)")
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

    Result.S.SetDiagonal(S.memptr());

    return Result;
}

}//end namespace mdk

#endif
