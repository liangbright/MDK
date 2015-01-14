#ifndef mdk_LinearAlgebra_Function_DenseMatrix_Part_2_h
#define mdk_LinearAlgebra_Function_DenseMatrix_Part_2_h

#include <algorithm>
#include <complex>

#include "mdkConstant.h"
#include "mdkMatrix_Common.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class DenseMatrix;
//-------------------------------------//

//-----------------------------------------------------------------------------------------------//

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber,
                                         int_max LinearIndex_start, int_max LinearIndex_end, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
int_max FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, MatchFunctionType MatchFunction);
//------------------------------------------------
template<typename ElementType>
inline DenseMatrix<int_max> ExactMatchElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const ElementType& InputElement);

template<typename ElementType>
inline DenseMatrix<int_max> ExactMatchElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, 
												      const ElementType& InputElement);
//---------------------------------------------------
template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber,
                                     int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction);

template<typename ElementType, typename MatchFunctionType>
int_max FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, MatchFunctionType MatchFunction);
//---------------------------------------------------
template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix, CompareFunctionType CompareFunction);

template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix,
                                     int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction);
//---------------------------------------------------
template<typename ElementType>
DenseMatrix<int_max> FindUniqueElementInMatrix(const DenseMatrix<ElementType>& InputMatrix);

//SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
template<typename ElementType, typename SpecialCompareFunctionType>
DenseMatrix<int_max> FindUniqueElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, SpecialCompareFunctionType SpecialCompareFunction);

template<typename ElementType>
DenseMatrix<int_max> FindUniqueColInMatrix(const DenseMatrix<ElementType>& InputMatrix);

//SpecialCompareFunction(VectorA, VectorB) return {-1, 0, 1} as {VectorA < VectorB, VectorA = VectorB, VectorA > VectorB}
template<typename ElementType, typename SpecialCompareFunctionType>
DenseMatrix<int_max> FindUniqueColInMatrix(const DenseMatrix<ElementType>& InputMatrix, SpecialCompareFunctionType SpecialCompareFunction);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
ElementType MatrixMean(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanOfEachCol(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanOfEachRow(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
int_max FindLinearIndexOfMaxInMatrix(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
ElementType MatrixMax(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxOfEachCol(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxOfEachRow(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
int_max FindLinearIndexOfMinInMatrix(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
ElementType MatrixMin(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinOfEachCol(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinOfEachRow(const DenseMatrix<ElementType>& InputMatrix);


template<typename ElementType>
inline
ElementType MatrixSum(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumOfEachCol(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumOfEachRow(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
ElementType MatrixNorm_L1(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
ElementType MatrixNorm_L2(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixTranspose(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline void MatrixTransposeInPlace(DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline int_max MatrixRank(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline ElementType MatrixDeterminant(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//
template<typename ElementType>
inline DenseMatrix<ElementType> MatrixInverse(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixPseudoInverse(const DenseMatrix<ElementType>& InputMatrix);
//-----------------------------------------------------------------------------------------------//
// solve A*X=B
template<typename ElementType>
inline DenseMatrix<ElementType> SolveMatrixLinearEquation(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);
//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct DenseMatrixEigenResult
{
    DenseMatrix<ElementType> EigenVector;    // eigenvector : full matrix
    DenseMatrix<ElementType> EigenValue;     // eigenvalue  : col vector

    DenseMatrixEigenResult(){};

    // move constructor
    DenseMatrixEigenResult(DenseMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue  = std::move(Result.EigenValue);
    }

    ~DenseMatrixEigenResult(){};

    void operator=(const DenseMatrixEigenResult&& Result)
    {
        EigenVector = std::move(Result.EigenVector);
        EigenValue  = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const DenseMatrixEigenResult&) = delete;
};

template<typename ElementType>
inline DenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline DenseMatrixEigenResult<ElementType> SymmetricRealMatrixEigen(const DenseMatrix<ElementType>& InputMatrix, bool CheckIfSymmetric = false);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct DenseMatrixPCAResult
{
    DenseMatrix<ElementType> Mean;           // mean
    DenseMatrix<ElementType> EigenVector;    // eigenvector
    DenseMatrix<ElementType> EigenValue;     // col vector

    DenseMatrixPCAResult(){};

    // move constructor
    DenseMatrixPCAResult(DenseMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    ~DenseMatrixPCAResult(){};

    void operator=(const DenseMatrixPCAResult&& Result)
    {
        Mean = std::move(Result.Mean);
        EigenVector = std::move(Result.EigenVector);
        EigenValue = std::move(Result.EigenValue);
    }

    //------------------------------------------------------
    void operator=(const DenseMatrixPCAResult&) = delete;
};

template<typename ElementType>
inline DenseMatrixPCAResult<ElementType> MatrixPCA(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct DenseMatrixSVDResult
{
    // Matrix = U*S*V;
    DenseMatrix<ElementType> U;  // matrix
    DenseMatrix<ElementType> S;  // matrix  : change to vector?
    DenseMatrix<ElementType> V;  // matrix

    DenseMatrixSVDResult(){};

    // move constructor
    DenseMatrixSVDResult(DenseMatrixSVDResult&& Result)
    {
        // this will call copy "=" not move "="
        //U = Result.U;

        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    ~DenseMatrixSVDResult(){};

    void operator=(const DenseMatrixSVDResult&& Result)
    {
        U = std::move(Result.U);
        V = std::move(Result.V);
        S = std::move(Result.S);
    }

    //------------------------------------------------------
    void operator=(const DenseMatrixSVDResult&) = delete;

};


template<typename ElementType>
inline DenseMatrixSVDResult<ElementType> MatrixSVD(const DenseMatrix<ElementType>& InputMatrix);

//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixConvolution(const DenseMatrix<ElementType>& InputMatrix, const DenseMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_2.hpp"

#endif
