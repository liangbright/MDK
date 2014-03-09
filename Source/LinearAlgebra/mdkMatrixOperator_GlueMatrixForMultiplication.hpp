#ifndef __mdkMatrixOperator_GlueMatrixForMultiplication_h
#define __mdkMatrixOperator_GlueMatrixForMultiplication_h


namespace mdk
{

//====================================================================================================================================================//
//
//          {GlueMatrixForMultiplication} vs {Matrix, Element, ShadowMatrix, GlueMatrixForLinearCombination, GlueMatrixForMultiplication}
//
//====================================================================================================================================================//

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   Matrix return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() + MatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() - MatrixB;
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';

        return  GlueMatrixA;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';

        return  GlueMatrixA;
    }

    auto MatrixNumber = GlueMatrixA.m_SourceMatrixSharedCopyList.size();

    GlueMatrixA.m_ColNumber = SizeB.ColNumber;

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber+1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].SharedCopy(MatrixB);

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() / MatrixB;
}

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Matrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return MatrixA + GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return MatrixA - GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';

        return  GlueMatrixB;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';

        return  GlueMatrixB;
    }

    auto MatrixNumber = GlueMatrixB.m_SourceMatrixSharedCopyList.size();

    GlueMatrixB.m_ColNumber = SizeA.ColNumber;

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    if (MatrixNumber == 0)
    {
        mdkError << "MatrixNumber == 0 @ mdkMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';

        return GlueMatrixB;
    }

    for (uint64 i = MatrixNumber; i > 1; --i)
    {
        GlueMatrixB.m_SourceMatrixSharedCopyList[i].Eat(GlueMatrixB.m_SourceMatrixSharedCopyList[i - 1]);
    }    

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateMatrix();
}

// -------------------------------------------- GlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -}  Element return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const ElementType& ElementB)
{
    return GlueMatrixA.CreateMatrix() + ElementB;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const ElementType& ElementB)
{
    return GlueMatrixA.CreateMatrix() - ElementB;
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_Element_Coef *= ElementB;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType>& operator/(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_Element_Coef /= ElementB;

    return GlueMatrixA;
}

// ---------------------------------------------- Element {+ - * /} GlueMatrixForMultiplication -----------------------------------------------------//

// note: Element {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Element {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Element {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ElementA + GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ElementA - GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_Element_Coef *= ElementA;

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ElementA / GlueMatrixB.CreateMatrix();
}

// -------------------------------------------- GlueMatrixForMultiplication {+ - /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateMatrix() + ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateMatrix() - ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA.CreateMatrix() / ShadowMatrixB.CreateMatrix();
}

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() + GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() - GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() * std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() / GlueMatrixB.CreateMatrix();
}

// -------------------------------------------- GlueMatrixForMultiplication {+ - /}  GlueMatrixForLinearCombination -----------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    return GlueMatrixA_M.CreateMatrix() + std::move(GlueMatrixB_L);
}

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType>& operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    return GlueMatrixA_M.CreateMatrix() - std::move(GlueMatrixB_L);
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L)
{
    return std::move(GlueMatrixA_M) * GlueMatrixB_L.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L)
{
    return GlueMatrixA_M.CreateMatrix() / GlueMatrixB_L.CreateMatrix();
}

// ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M)
{
    return std::move(GlueMatrixA_L) + GlueMatrixB_M.CreateMatrix();
}

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M)
{
    return std::move(GlueMatrixA_L) - GlueMatrixB_M.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    return GlueMatrixA_L.CreateMatrix() * std::move(GlueMatrixB_M);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M)
{
    return GlueMatrixA_L.CreateMatrix() / GlueMatrixB_M.CreateMatrix();
}

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  GlueMatrixForMultiplication ---------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForMultiplication return Matrix
// note: GlueMatrixForMultiplication {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() + GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() - GlueMatrixB.CreateMatrix();
}




template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    //-------------------------------------------------------------------------------------------------------------------------------------------
    // follow user's grouping: (A*B)*(C*D), treat it as Matrix * Matrix
    //-------------------------------------------------------------------------------------------------------------------------------------------

    return GlueMatrixA.CreateMatrix() * GlueMatrixB.CreateMatrix();

    //-------------------------------------------------------------------------------------------------------------------------------------------
    // ignore user's grouping: (A*B)*(C*D),  treat it as A*B*C*D
    //-------------------------------------------------------------------------------------------------------------------------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';

        return  GlueMatrixA;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';

        return  GlueMatrixA;
    }

    auto MatrixNumber_A = GlueMatrixA.m_SourceMatrixSharedCopyList.size();

    auto MatrixNumber_B = GlueMatrixB.m_SourceMatrixSharedCopyList.size();

    GlueMatrixA.m_ColNumber = SizeB.ColNumber;

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (uint64 i = MatrixNumber_A; i < MatrixNumber_A + MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[i].SharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[i - MatrixNumber_A]);
    }

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() / GlueMatrixB.CreateMatrix();
}

} // namespace mdk

#endif
