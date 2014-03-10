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
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        // check size
        if (GlueMatrixA.m_ColNumber != MatrixB.GetColNumber() || GlueMatrixA.m_RowNumber != MatrixB.GetRowNumber())
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
            return tempGlueMatrix_L;
        }

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateMatrix() + MatrixB;
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        // check size
        if (GlueMatrixA.m_ColNumber != MatrixB.GetColNumber() || GlueMatrixA.m_RowNumber != MatrixB.GetRowNumber())
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
            return tempGlueMatrix_L;
        }

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(-1));

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateMatrix() - MatrixB;
    }
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

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceShare(MatrixB);

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
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        // check size
        if (GlueMatrixB.m_ColNumber != MatrixA.GetColNumber() || GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber())
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
            return tempGlueMatrix_L;
        }

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else
    {
        return MatrixA + GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        // check size
        if (GlueMatrixB.m_ColNumber != MatrixA.GetColNumber() || GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber())
        {
            mdkError << "Size does not match @ mdkMatrixOperator -(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
            return tempGlueMatrix_L;
        }

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixA.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else
    {
        return MatrixA - GlueMatrixB.CreateMatrix();
    }
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

    for (uint64 i = MatrixNumber; i >= 1; --i)
    {
        GlueMatrixB.m_SourceMatrixSharedCopyList[i].ForceShare(GlueMatrixB.m_SourceMatrixSharedCopyList[i - 1]);
    }    

    GlueMatrixB.m_SourceMatrixSharedCopyList[0].ForceShare(MatrixA);

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
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementB;

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateMatrix() + ElementB;
    }
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const ElementType& ElementB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementType(0) - ElementB;

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateMatrix() - ElementB;
    }
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
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementA;

        return tempGlueMatrix_L;
    }
    else
    {
        return ElementA + GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(-GlueMatrixB.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementA;

        return tempGlueMatrix_L;
    }
    else
    {
        return ElementA - GlueMatrixB.CreateMatrix();
    }
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

// -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA + ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return GlueMatrixA - ShadowMatrixB.CreateMatrix();
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
    return ShadowMatrixA.CreateMatrix() + GlueMatrixB;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() - GlueMatrixB;
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
    // check if GlueMatrixA_M only has one matrix

    if (GlueMatrixA_M.GetMatrixNumber() == 1)
    {
        // check size
        if (GlueMatrixA_M.m_ColNumber != GlueMatrixB_L.m_ColNumber || GlueMatrixA_M.m_RowNumber != GlueMatrixB_L.m_RowNumber)
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
            return GlueMatrixA_L;
        }

        GlueMatrixB_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA_M.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        GlueMatrixB_L.m_ElementList_Coef.push_back(GlueMatrixA_M.m_Element_Coef);

        return GlueMatrixB_L;
    }
    else
    {
        return GlueMatrixA_M.CreateMatrix() + std::move(GlueMatrixB_L);
    }
}

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType>& operator-(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    // check if GlueMatrixA_M only has one matrix

    if (GlueMatrixA_M.GetMatrixNumber() == 1)
    {
        // check size
        if (GlueMatrixA_M.m_ColNumber != GlueMatrixB_L.m_ColNumber || GlueMatrixA_M.m_RowNumber != GlueMatrixB_L.m_RowNumber)
        {
            mdkError << "Size does not match @ mdkMatrixOperator -(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
            return GlueMatrixA_L;
        }

        GlueMatrixB_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA_M.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        for (uint64 i = 0; i < GlueMatrixB_L.m_ElementList_Coef.size(); ++i)
        {
            GlueMatrixB_L.m_ElementList_Coef[i] = ElementType(0) - GlueMatrixB_L.m_ElementList_Coef[i];
        }

        GlueMatrixB_L.m_ElementList_Coef.push_back(GlueMatrixA_M.m_Element_Coef);

        GlueMatrixB_L.m_IndependentElement = ElementType(0) - GlueMatrixB_L.m_IndependentElement;

        return GlueMatrixB_L;
    }
    else
    {
        return GlueMatrixA_M.CreateMatrix() - std::move(GlueMatrixB_L);
    }
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
    // check if GlueMatrixB_M only has one matrix

    if (GlueMatrixB_M.GetMatrixNumber() == 1)
    {
        // check size
        if (GlueMatrixB_M.m_ColNumber != GlueMatrixA_L.m_ColNumber || GlueMatrixB_M.m_RowNumber != GlueMatrixA_L.m_RowNumber)
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
            return GlueMatrixA_L;
        }

        GlueMatrixA_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB_M.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        GlueMatrixA_L.m_ElementList_Coef.push_back(GlueMatrixB_M.m_Element_Coef);

        return GlueMatrixA_L;
    }
    else
    {
        return std::move(GlueMatrixA_L) + GlueMatrixB_M.CreateMatrix();
    }
}

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M)
{
    // check if GlueMatrixB_M only has one matrix

    if (GlueMatrixB_M.GetMatrixNumber() == 1)
    {
        // check size
        if (GlueMatrixB_M.m_ColNumber != GlueMatrixA_L.m_ColNumber || GlueMatrixB_M.m_RowNumber != GlueMatrixA_L.m_RowNumber)
        {
            mdkError << "Size does not match @ mdkMatrixOperator -(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
            return GlueMatrixA_L;
        }

        GlueMatrixA_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB_M.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        GlueMatrixA_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB_M.m_Element_Coef);

        return GlueMatrixA_L;
    }
    else
    {
        return std::move(GlueMatrixA_L) - GlueMatrixB_M.CreateMatrix();
    }
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
    // check if one matrix in GlueMatrixA and one matrix in GlueMatrixB

    if (GlueMatrixA.GetMatrixNumber() == 1 && GlueMatrixB.GetMatrixNumber() == 1)
    {
        auto SizeA = GlueMatrixA.GetSize();

        auto SizeB = GlueMatrixB.GetSize();

        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        // check size -----------------------------

        // empty -------------------------------

        if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
        {
            mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';

            return  tempGlueMatrix_L;
        }

        //scalar -----------------------
        
        if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
        {
            mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

            return  tempScalarMatrixA(0) + GlueMatrixB;
        }

        if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
        {
            mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

            return GlueMatrixA + tempScalarMatrixB(0);
        }

        //matrix -------------------

        if (GlueMatrixA.m_ColNumber != GlueMatrixB.m_ColNumber || GlueMatrixA.m_RowNumber != GlueMatrixB.m_RowNumber)
        {
            mdkError << "Size does not match @ mdkMatrixOperator +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
            return tempGlueMatrix_L;
        }

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixA.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_SourceMatrixSharedCopyList[0].GetElementDataSharedPointer());

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else if (GlueMatrixA.GetMatrixNumber() == 1 && GlueMatrixB.GetMatrixNumber() > 1)
    {
        return GlueMatrixA + GlueMatrixB.CreateMatrix();
    }
    else if (GlueMatrixA.GetMatrixNumber() > 1 && GlueMatrixB.GetMatrixNumber() == 1)
    {
        return GlueMatrixA.CreateMatrix() + GlueMatrixB;
    }
    else
    {
        return GlueMatrixA.CreateMatrix() + GlueMatrixB.CreateMatrix();
    }
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
    //  may lead to error, for example:
    //  A : 1x3, B: 3x1, C: 3x3, D: 3x3,  A*B is a scalar,  but B*C is meaningless
    
    /* -------------------------------------------------------------------------------------------------------------------------------------------

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

    GlueMatrixA.m_Element_Coef *= GlueMatrixB.m_Element_Coef;

    return GlueMatrixA;

    */ //-----------------------------------------------------------------------------------------------------------------------------
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    return GlueMatrixA.CreateMatrix() / GlueMatrixB.CreateMatrix();
}

} // namespace mdk

#endif
