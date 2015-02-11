#ifndef mdk_SparseMatrixOperator_SparseGlueMatrixForLinearCombination_h
#define mdk_SparseMatrixOperator_SparseGlueMatrixForLinearCombination_h

namespace mdk
{

//============================================================================================================================================//
//
//      {SparseGlueMatrixForLinearCombination} vs {SparseMatrix, Element, SparseShadowMatrix, SparseGlueMatrixForLinearCombination}
//
//============================================================================================================================================//

// ---------------------------------- SparseGlueMatrixForLinearCombination {+ - * /}  SparseMatrix ---------------------------------------------------------//

// note: SparseGlueMatrixForLinearCombination {+ -} SparseMatrix return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForLinearCombination {*}   SparseMatrix return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForLinearCombination {/}   SparseMatrix is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) + MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) + MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixB);

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: -(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) - MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) - MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_ForLinearCombination, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixB);

    GlueMatrixA.m_ElementList_Coef.push_back(ElementType(-1));

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateSparseMatrix() * MatrixB;
}

// ------------------------------------- SparseMatrix or {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

// note: SparseMatrix {+ -} SparseGlueMatrixForLinearCombination return SparseGlueMatrixForLinearCombination
// note: SparseMatrix {*}   SparseGlueMatrixForLinearCombination return SparseGlueMatrixForMultiplication
// note: SparseMatrix {/}   SparseGlueMatrixForLinearCombination is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const mdkSparseMatrix<ElementType>& MatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: +(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return MatrixA + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixB.GetMatrixNumber();

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixB.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixA);

    GlueMatrixB.m_ElementList_Coef.push_back(ElementType(1));

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseMatrix<ElementType>& MatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: -(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return MatrixA - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(MatrixA, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber = GlueMatrixB.GetMatrixNumber();

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    GlueMatrixB.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixA);

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
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return MatrixA * GlueMatrixB.CreateSparseMatrix();
}

// ----------------------------------- SparseGlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

// note: SparseGlueMatrixForLinearCombination {+ - * /}  Element return SparseGlueMatrixForLinearCombination

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement += ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement -= ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator*(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
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
mdkSparseGlueMatrixForLinearCombination<ElementType> operator/(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_IndependentElement /= ElementB;

    for (uint64 i = 0; i < GlueMatrixA.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef[i] /= ElementB;
    }

    return GlueMatrixA;
}

// ----------------------------------------------- Element {+ - *} SparseGlueMatrixForLinearCombination ----------------------------------------------//

// note: Element {+ - *} SparseGlueMatrixForLinearCombination return SparseGlueMatrixForLinearCombination
// note: Element {/}     SparseGlueMatrixForLinearCombination is x/0

template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement += ElementA;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
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
mdkSparseGlueMatrixForLinearCombination<ElementType> operator*(const ElementType& ElementA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_IndependentElement *= ElementA;

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] *= ElementA;
    }

    return GlueMatrixB;
}

// ----------------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseShadowMatrix -----------------------------------------------------//

// note: SparseGlueMatrixForLinearCombination {+ -} SparseShadowMatrix return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForLinearCombination {*}   SparseShadowMatrix return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForLinearCombination {/}   SparseShadowMatrix is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) + ShadowMatrixB.CreateSparseMatrix();
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) - ShadowMatrixB.CreateSparseMatrix();
}


template<typename ElementType>
inline mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateSparseMatrix() * ShadowMatrixB.CreateSparseMatrix();
}

// --------------------------------- SparseShadowMatrix {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

// note: SparseShadowMatrix {+ -} SparseGlueMatrixForLinearCombination return SparseGlueMatrixForLinearCombination
// note: SparseShadowMatrix {*}   SparseGlueMatrixForLinearCombination return SparseGlueMatrixForMultiplication
// note: SparseShadowMatrix {/}   SparseGlueMatrixForLinearCombination is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() + std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() - std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() * GlueMatrixB.CreateSparseMatrix();
}

// ---------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseGlueMatrixForLinearCombination ---------------------------------------//

// note: SparseGlueMatrixForLinearCombination {+ -} SparseGlueMatrixForLinearCombination return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForLinearCombination {*}   SparseGlueMatrixForLinearCombination return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForLinearCombination {/}   SparseGlueMatrixForLinearCombination is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber_A + i].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[i]);
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
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: -(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_ForLinearCombination, GlueMatrixB_ForLinearCombination)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix;
        return EmptyGlueMatrix;
    }

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (int64 i = 0; i < MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber_A + i].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[i]);
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
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateSparseMatrix() * GlueMatrixB.CreateSparseMatrix();
}

} // namespace mdk

#endif
