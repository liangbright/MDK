#ifndef __mdkSparseMatrixOperator_SparseMatrix_hpp
#define __mdkSparseMatrixOperator_SparseMatrix_hpp

namespace mdk
{

//============================================================================================================================================//
//
//                                                    {SparseMatrix} v.s {SparseMatrix, Element}
//
//============================================================================================================================================//

// ----------------------------------------------------- SparseMatrix  {+ - *}  SparseMatrix ---------------------------------------------------------//

// note: SparseMatrix {+ -} SparseMatrix return SparseGlueMatrixForLinearCombination
// note: SparseMatrix {*}   SparseMatrix return SparseGlueMatrixForMultiplication
// note: SparseMatrix {/}   SparseMatrix is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) + MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA + MatrixB(0);
    }

    mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }


    tempGlueMatrix.m_RowNumber = SizeA.RowNumber;

    tempGlueMatrix.m_ColNumber = SizeA.ColNumber;

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA - MatrixB(0);
    }

    mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkSparseMatrixOperator: -(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }
  

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(-1));


    return tempGlueMatrix;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) * MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA * MatrixB(0);
    }

    mdkSparseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: *(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkSparseMatrixOperator: *(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    tempGlueMatrix.m_RowNumber = SizeA.RowNumber;

    tempGlueMatrix.m_ColNumber = SizeB.ColNumber;

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

    return tempGlueMatrix;
}

// ---------------------------------------------------- SparseMatrix  {+ - * /}  Element ------------------------------------------------------//

// note: SparseMatrix {+ -}  Element return DenseMatrix
// note: SparseMatrix {* /}  Element return SparseGlueMatrixForMultiplication

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> operator+(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    tempMatrix = MatrixA.CreateDenseMatrix();

    tempMatrix += ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> operator-(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    tempMatrix = MatrixA.CreateDenseMatrix();

    tempMatrix -= ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixA.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixA.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_Element_Coef = ElementB;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator/(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixA.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixA.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_Element_Coef = ElementType(1) / ElementB;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}

// --------------------------------------------------- Element {+ - *} SparseMatrix  ---------------------------------------------------------//

// note: Element {+ -} SparseMatrix return SparseGlueMatrixForLinearCombination
// note: Element {*}   SparseMatrix return SparseGlueMatrixForMultiplication
// note: Element {/}   SparseMatrix is x/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixB.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixB.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementA;

    return tempGlueMatrix;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixB.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixB.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(-1));

    tempGlueMatrix.m_IndependentElement = ElementA;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixB.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixB.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_Element_Coef = ElementA;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}

} // namespace mdk

#endif
