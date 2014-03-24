#ifndef __mdkDenseMatrixOperator_DenseGlueMatrixForLinearCombination_h
#define __mdkDenseMatrixOperator_DenseGlueMatrixForLinearCombination_h

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
DenseGlueMatrixForLinearCombination<ElementType> operator+(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "GlueMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateDenseMatrix();

        return tempScalarMatrixA(0) + MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) + MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: +(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceShare(MatrixB);

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
DenseGlueMatrixForLinearCombination<ElementType> operator-(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "GlueMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateDenseMatrix();

        return tempScalarMatrixA(0) - MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) - MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: -(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceShare(MatrixB);

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(-1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() * MatrixB;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() / MatrixB;
}

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication (definition in mdkDenseGlueMatrixForMultiplication.h)
// note: Matrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(const DenseMatrix<ElementType>& MatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "MatrixA or GlueMatrixB is empty @ mdkDenseMatrixOperator: +(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateDenseMatrix();

        return MatrixA + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: +(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixB.GetMatrixNumber();

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixB.m_SourceMatrixSharedCopyList[MatrixNumber].ForceShare(MatrixA);

    GlueMatrixB.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixB;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(const DenseMatrix<ElementType>& MatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "MatrixA or GlueMatrixB is empty @ mdkDenseMatrixOperator: -(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateDenseMatrix();

        return MatrixA - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: -(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixB.GetMatrixNumber();

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixB.m_SourceMatrixSharedCopyList[MatrixNumber].Share(MatrixA);

    for (int64 i = 0; i < MatrixNumber; ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] = ElementType(0) - GlueMatrixB.m_ElementList_Coef[i];
    }

    GlueMatrixB.m_ElementList_Coef.push_back(ElementType(1));

    GlueMatrixB.m_IndependentElement = ElementType(0) - GlueMatrixB.m_IndependentElement;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseMatrix<ElementType>& MatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return MatrixA * GlueMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseMatrix<ElementType>& MatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateDenseMatrix();
}


// ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

// note: GlueMatrixForLinearCombination {+ - * /}  Element return GlueMatrixForLinearCombination

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement += ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement -= ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline
DenseGlueMatrixForLinearCombination<ElementType> operator*(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
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
DenseGlueMatrixForLinearCombination<ElementType> operator/(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
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
DenseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement += ElementA;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
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
DenseGlueMatrixForLinearCombination<ElementType> operator*(const ElementType& ElementA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
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
DenseMatrix<ElementType> operator/(const ElementType& ElementA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ElementA / GlueMatrixB.CreateDenseMatrix();
}

// -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) + ShadowMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) - ShadowMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() * ShadowMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() / ShadowMatrixB.CreateDenseMatrix();
}

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(const DenseShadowMatrix<ElementType>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() + std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(const DenseShadowMatrix<ElementType>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() - std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() * GlueMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline DenseMatrix<ElementType> operator/(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() / GlueMatrixB.CreateDenseMatrix();
}

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator+(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "GlueMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateDenseMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateDenseMatrix();

        return std::move(GlueMatrixA) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber_A + i].Share(GlueMatrixB.m_SourceMatrixSharedCopyList[i]);
    }

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement += GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
DenseGlueMatrixForLinearCombination<ElementType> operator-(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, DenseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Error << "GlueMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateDenseMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        DenseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateDenseMatrix();

        return std::move(GlueMatrixA) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: -(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        DenseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber_A + i].Share(GlueMatrixB.m_SourceMatrixSharedCopyList[i]);
    }

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement -= GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;

}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() * GlueMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateDenseMatrix() / GlueMatrixB.CreateDenseMatrix();
}

} // namespace mdk

#endif
