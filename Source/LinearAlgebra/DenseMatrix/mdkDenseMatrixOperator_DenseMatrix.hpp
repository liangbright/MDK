#pragma once

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
DenseGlueMatrixForLinearCombination<ElementType> operator+(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixA(0) + MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixA + MatrixB(0);
    }

    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: +(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }


    tempGlueMatrix.m_RowCount = SizeA.RowCount;

    tempGlueMatrix.m_ColCount = SizeA.ColCount;

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);    
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(2);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);
    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceShare(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixA(0) - MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixA - MatrixB(0);
    }

    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: -(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }
  
    tempGlueMatrix.m_RowCount = SizeA.RowCount;

    tempGlueMatrix.m_ColCount = SizeA.ColCount;

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(2);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceShare(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(-1));


    return tempGlueMatrix;
}


template<typename ElementType>
inline
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixA(0) * MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixA * MatrixB(0);
    }

    DenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkDenseMatrixOperator: *(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }

    if (SizeA.ColCount != SizeB.RowCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: *(MatrixA, MatrixB)")

        return  tempGlueMatrix;
    }

    tempGlueMatrix.m_RowCount = SizeA.RowCount;

    tempGlueMatrix.m_ColCount = SizeB.ColCount;

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(2);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(2);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[1].ForceShare(MatrixB);

    return tempGlueMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> operator/(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(MatrixA, MatrixB);
}

// ---------------------------------------------------- Matrix  {+ - * /}  Element ------------------------------------------------------//

// note: Matrix {+ -}  Element return GlueMatrixForLinearCombination
// note: Matrix {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixA.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixA.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);
    
    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixA.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixA.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);
 
    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementType(0) - ElementB;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixA.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixA.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

    tempGlueMatrix.m_Element_Coef = ElementB;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator/(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixA.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixA.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

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
DenseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixB.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixB.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(1));

    tempGlueMatrix.m_IndependentElement = ElementA;

    return tempGlueMatrix;
}


template<typename ElementType>
inline
DenseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixB.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixB.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixB);

    tempGlueMatrix.m_ElementList_Coef.push_back(ElementType(-1));

    tempGlueMatrix.m_IndependentElement = ElementA;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseGlueMatrixForMultiplication<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowCount = MatrixB.GetRowCount();

    tempGlueMatrix.m_ColCount = MatrixB.GetColCount();

    //tempGlueMatrix.m_SourceMatrixSharedCopyList.resize(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.reserve(1);
    tempGlueMatrix.m_SourceMatrixSharedCopyList.emplace_back(MDK_PURE_EMPTY_MATRIX);

    tempGlueMatrix.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixB);

    tempGlueMatrix.m_Element_Coef = ElementA;

    tempGlueMatrix.m_Is_m_Element_Coef_Equal_to_One = false;

    return tempGlueMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> operator/(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(ElementA, MatrixB);
}

} // namespace mdk

