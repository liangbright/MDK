#ifndef __mdkDenseMatrixOperator_DenseMatrix_hpp
#define __mdkDenseMatrixOperator_DenseMatrix_hpp

namespace mdk
{

//============================================================================================================================================//
//
//                                                    {Matrix} v.s {Matrix, Element}
//
//============================================================================================================================================//

// ----------------------------------------------------- Matrix  {+ - * /}  Matrix ---------------------------------------------------------//

// note: Matrix {+ -} Matrix return GlueMatrixForLinearCombination
// note: Matrix {*}   Matrix return MultiplyGlueMatrix
// note: Matrix {/}   Matrix return Matrix

template<typename ElementType>
inline 
mdkDenseGlueMatrixForLinearCombination<ElementType> operator+(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
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

    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseMatrixOperator: +(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(MatrixA, MatrixB)" << '\n';

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
mdkDenseGlueMatrixForLinearCombination<ElementType> operator-(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
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

    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseMatrixOperator: -(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(MatrixA, MatrixB)" << '\n';

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
mdkDenseGlueMatrixForMultiplication<ElementType> operator*(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
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

    mdkDenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkDenseMatrixOperator: *(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: *(MatrixA, MatrixB)" << '\n';

        return  tempGlueMatrix;
    }

    tempGlueMatrix.m_RowNumber = SizeA.RowNumber;

    tempGlueMatrix.m_ColNumber = SizeB.ColNumber;

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

    return tempGlueMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> operator/(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(MatrixA, MatrixB);
}

// ---------------------------------------------------- Matrix  {+ - * /}  Element ------------------------------------------------------//

// note: Matrix {+ -}  Element return GlueMatrixForLinearCombination
// note: Matrix {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline 
mdkDenseGlueMatrixForLinearCombination<ElementType> operator+(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixA.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixA.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkDenseGlueMatrixForLinearCombination<ElementType> operator-(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixA.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixA.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementType(0) - ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkDenseGlueMatrixForMultiplication<ElementType> operator*(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

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
mdkDenseGlueMatrixForMultiplication<ElementType> operator/(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixA.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixA.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    tempGlueMatrix.m_Element_Coef = ElementType(1) / ElementB;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}

// --------------------------------------------------- Element {+ - * /} Matrix  ---------------------------------------------------------//

// note: Element {+ -} Matrix return GlueMatrixForLinearCombination
// note: Element {*}   Matrix return GlueMatrixForMultiplication
// note: Element {/}   Matrix return Matrix

template<typename ElementType>
inline 
mdkDenseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

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
mdkDenseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

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
mdkDenseGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = MatrixB.GetRowNumber();

    tempGlueMatrix.m_ColNumber = MatrixB.GetColNumber();

    tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixB);

    tempGlueMatrix.m_Element_Coef = ElementA;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> operator/(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(ElementA, MatrixB);
}

} // namespace mdk

#endif
