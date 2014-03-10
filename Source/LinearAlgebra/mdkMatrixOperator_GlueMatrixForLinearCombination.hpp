#ifndef __mdkMatrixOperator_GlueMatrixForLinearCombination_h
#define __mdkMatrixOperator_GlueMatrixForLinearCombination_h

namespace mdk
{

//============================================================================================================================================//
//
//                          {GlueMatrixForLinearCombination} vs {Matrix, Element, ShadowMatrix, GlueMatrixForLinearCombination}
//
//============================================================================================================================================//

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   Matrix return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkMatrixOperator: +(GlueMatrixA, MatrixB)" << '\n';

        return GlueMatrixA;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 || SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) + MatrixB;
    }

    if (SizeB.RowNumber == 1 || SizeB.ColNumber == 1)
    {
        return GlueMatrixA + MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA, MatrixB)" << '\n';
        return GlueMatrixA;
    }

    GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    if (GlueMatrixA.m_RowNumber != MatrixB.GetRowNumber() || GlueMatrixA.m_ColNumber != MatrixB.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA, MatrixB)" << '\n';
        return GlueMatrixA;
    }

    GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(- 1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() * MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() / MatrixB;
}

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication (definition in mdkGlueMatrixForMultiplication.h)
// note: Matrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    if (GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber() || GlueMatrixB.m_ColNumber != MatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        return GlueMatrixB;
    }

    GlueMatrixB.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    GlueMatrixB.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    if (GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber() || GlueMatrixB.m_ColNumber != MatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        return GlueMatrixB;
    }

    GlueMatrixB.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] = ElementType(0) - GlueMatrixB.m_ElementList_Coef[i];
    }

    GlueMatrixB.m_ElementList_Coef.push_back(ElementType(1));

    GlueMatrixB.m_IndependentElement = ElementType(0) - GlueMatrixB.m_IndependentElement;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return MatrixA * GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateMatrix();
}


// ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

// note: GlueMatrixForLinearCombination {+ - * /}  Element return GlueMatrixForLinearCombination

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement += ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement -= ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator*(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement *= ElementB;

    for (uint64 i = 0; i < GlueMatrixA.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef[i] *= ElementB;
    }

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement /= ElementB;

    for (uint64 i = 0; i < GlueMatrixA.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef[i] /= ElementB;
    }

    return GlueMatrixA;
}

// ----------------------------------------------- Element {+ - * /} GlueMatrixForLinearCombination ----------------------------------------------//

// note: Element {+ - *} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Element {/}     GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement += ElementA;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement = ElementA - GlueMatrix.m_IndependentElement;

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] = ElementType(0) - GlueMatrixB.m_ElementList_Coef[i];
    }

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator*(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement *= ElementA;

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] *= ElementA;
    }

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ElementA / GlueMatrixB.CreateMatrix();
}

// -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) +ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) -ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateMatrix() * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateMatrix() / ShadowMatrixB.CreateMatrix();
}

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() + std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() - std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() * GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() / GlueMatrixB.CreateMatrix();
}

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    if (GlueMatrixA.m_RowNumber != GlueMatrixB.m_RowNumber || GlueMatrixA.m_ColNumber != GlueMatrixB.m_ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        return GlueMatrixA;
    }

    for (uint64 i = 0; i < GlueMatrixB.m_MatrixElementDataSharedPointerList.size(); ++i)
    {
        GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_MatrixElementDataSharedPointerList[i]);
    }

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement += GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    if (GlueMatrixA.m_RowNumber != GlueMatrixB.m_RowNumber || GlueMatrixA.m_ColNumber != GlueMatrixB.m_ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        return GlueMatrixA;
    }

    for (uint64 i = 0; i < GlueMatrixB.m_MatrixElementDataSharedPointerList.size(); ++i)
    {
        GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_MatrixElementDataSharedPointerList[i]);
    }

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement -= GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;

}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() * GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() / GlueMatrixB.CreateMatrix();
}

} // namespace mdk

#endif
