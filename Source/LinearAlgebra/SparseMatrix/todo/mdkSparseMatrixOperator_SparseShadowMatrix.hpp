#ifndef mdk_SparseMatrixOperator_SparseShadowMatrix_hpp
#define mdk_SparseMatrixOperator_SparseShadowMatrix_hpp

namespace mdk
{

//============================================================================================================================================//
//
//                                          {SparseShadowMatrix} v.s {SparseMatrix, Element, SparseShadowMatrix}
//
//============================================================================================================================================//

// ------------------------------------------ SparseShadowMatrix {+ - *}  SparseMatrix ----------------------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator+(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] + MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + MatrixB(0);
    }

    mdkSparseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkWarning << "ShadowMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator-(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - MatrixB(0);
    }

    mdkSparseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkWarning << "ShadowMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//



    return  tempMatrix;
}


template<typename ElementType>
inline mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() * MatrixB;
}

// ---------------------------------------------- Matrix {+ - *}  ShadowMatrix -----------------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator+(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ShadowMatrixA.CreateSparseMatrix();
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> operator-(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA - ShadowMatrixB[0];
    }

    mdkSparseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkWarning << "MatrixA or ShadowMatrixB is empty @ mdkSparseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA * ShadowMatrixB.CreateSparseMatrix();
}

// ------------------------------------------ SparseShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

template<typename ElementType>
inline
mdkSparseMatrix<ElementType> operator+(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber <= 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkSparseMatrixOperator: +(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------------------//

    tempMatrix.DeepCopy(ShadowMatrixA.m_SourceMatrixSharedCopy);

    auto ptrTemp = tempMatrix.GetNonZeroElementPointer();

    auto ptrA = ShadowMatrixA.GetNonZeroElementPointer();

    auto NonZeroElementNumber = ShadowMatrixA.GetNonZeroElementNumber();

    for (int64 i = 0; i < NonZeroElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] + ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> operator-(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber <= 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkSparseMatrixOperator: -(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator*(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber <= 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkSparseMatrixOperator: *(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator/(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber <= 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkSparseMatrixOperator: /(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}

// ----------------------------------------- Element {+ - *} ShadowMatrix --------------------------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator+(const ElementType& ElementA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ElementA;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator-(const ElementType& ElementA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber <= 0)
    {
        mdkError << "ShadowMatrixB is empty @ mdkSparseMatrixOperator: -(ElementA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator*(const ElementType& ElementA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB * ElementA;
}


// ---------------------------------------- SparseShadowMatrix {+ - *}  SparseShadowMatrix -------------------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator+(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] + ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + ShadowMatrixB[0];
    }

    mdkSparseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkSparseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //------------------------------------------------------

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> operator-(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - ShadowMatrixB[0];
    }

    mdkSparseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkSparseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //------------------------------------------------------


    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() * ShadowMatrixB.CreateSparseMatrix();
}

} // namespace mdk

#endif
