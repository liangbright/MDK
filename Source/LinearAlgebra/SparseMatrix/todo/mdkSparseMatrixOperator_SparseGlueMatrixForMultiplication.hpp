#ifndef mdk_SparseMatrixOperator_SparseGlueMatrixForMultiplication_h
#define mdk_SparseMatrixOperator_SparseGlueMatrixForMultiplication_h


namespace mdk
{

//====================================================================================================================================================//
//
// {SparseGlueMatrixForMultiplication} vs {SparseMatrix, Element, SparseShadowMatrix, SparseGlueMatrixForLinearCombination, SparseGlueMatrixForMultiplication}
//
//====================================================================================================================================================//

// ---------------------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseMatrix ---------------------------------------------------------//

// note: SparseGlueMatrixForMultiplication {+ -} SparseMatrix return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForMultiplication {*}   SparseMatrix return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForMultiplication {/}   SparseMatrix is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) + MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) + MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() + MatrixB;
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) - MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) - MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(MatrixB);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(-1));

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() - MatrixB;
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkSparseMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) * MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) * MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_ColNumber = SizeB.ColNumber;

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber+1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixB);

    return GlueMatrixA;
}

// ------------------------------- SparseMatrix or {+ - *} SparseGlueMatrixForMultiplication -------------------------------------------------------//

// note: SparseMatrix {+ -} SparseGlueMatrixForMultiplication return SparseGlueMatrixForLinearCombination
// note: SparseMatrix {*}   SparseGlueMatrixForMultiplication return SparseGlueMatrixForMultiplication
// note: SparseMatrix {/}   SparseGlueMatrixForMultiplication is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const mdkSparseMatrix<ElementType>& MatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: +(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {      
        return MatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return MatrixA + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(tempGlueMatrix_L.m_SourceMatrixSharedCopyList[0]);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else
    {
        return MatrixA + GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseMatrix<ElementType>& MatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: -(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return MatrixA - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(tempGlueMatrix_L.m_SourceMatrixSharedCopyList[0]);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(1));

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else
    {
        return MatrixA + GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseMatrix<ElementType>& MatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkSparseMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) * std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {      
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return MatrixA * tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }


    int64 MatrixNumber = GlueMatrixB.m_SourceMatrixSharedCopyList.size();

    GlueMatrixB.m_ColNumber = SizeA.ColNumber;

    GlueMatrixB.m_SourceMatrixSharedCopyList.resize(MatrixNumber + 1);

    for (int64 i = MatrixNumber; i >= 1; --i)
    {
        GlueMatrixB.m_SourceMatrixSharedCopyList[i].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[i - 1]);
    }    

    GlueMatrixB.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(MatrixA);

    return GlueMatrixB;
}

// ------------------------------- SparseGlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

// note: SparseGlueMatrixForMultiplication {+ -}  Element return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForMultiplication {* /}  Element return SparseGlueMatrixForMultiplication

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementB;

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() + ElementB;
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementType(0) - ElementB;

        return tempGlueMatrix_L;
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() - ElementB;
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_Element_Coef *= ElementB;

    GlueMatrixA.m_Is_m_Element_Coef_Equal_to_One = false;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator/(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_Element_Coef /= ElementB;

    GlueMatrixA.m_Is_m_Element_Coef_Equal_to_One = false;

    return GlueMatrixA;
}

// ---------------------------------------------- Element {+ - *} SparseGlueMatrixForMultiplication -----------------------------------------------------//

// note: Element {+ -} SparseGlueMatrixForMultiplication return SparseGlueMatrixForLinearCombination
// note: Element {*}   SparseGlueMatrixForMultiplication return SparseGlueMatrixForMultiplication
// note: Element {/}   SparseGlueMatrixForMultiplication is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementA;

        return tempGlueMatrix_L;
    }
    else
    {
        return ElementA + GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_Element_Coef);

        tempGlueMatrix_L.m_IndependentElement = ElementA;

        return tempGlueMatrix_L;
    }
    else
    {
        return ElementA - GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    GlueMatrixB.m_Element_Coef *= ElementA;

    GlueMatrixB.m_Is_m_Element_Coef_Equal_to_One = false;

    return GlueMatrixB;
}

// ------------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseShadowMatrix -----------------------------------------------------//

// note: SparseGlueMatrixForMultiplication {+ -} SparseShadowMatrix return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForMultiplication {*}   SparseShadowMatrix return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForMultiplication {/}   SparseShadowMatrix is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) + ShadowMatrixB.CreateSparseMatrix();
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) - ShadowMatrixB.CreateSparseMatrix();
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkSparseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) * ShadowMatrixB.CreateSparseMatrix();
}

// --------------------------- SparseShadowMatrix {+ - *} SparseGlueMatrixForMultiplication -------------------------------------------------------//

// note: SparseShadowMatrix {+ -} SparseGlueMatrixForMultiplication return SparseGlueMatrixForLinearCombination
// note: SparseShadowMatrix {*}   SparseGlueMatrixForMultiplication return SparseGlueMatrixForMultiplication
// note: SparseShadowMatrix {/}   SparseGlueMatrixForMultiplication is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() + std::move(GlueMatrixB);
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() - std::move(GlueMatrixB);
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseShadowMatrix<ElementType>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateSparseMatrix() * std::move(GlueMatrixB);
}

// -------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseGlueMatrixForLinearCombination -----------------------------------------//

// note: SparseGlueMatrixForMultiplication {+ -} SparseGlueMatrixForLinearCombination return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForMultiplication {*}   SparseGlueMatrixForLinearCombination return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForMultiplication {/}   SparseGlueMatrixForLinearCombination is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_M.GetSize();

    auto SizeB = GlueMatrixB_L.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_M or GlueMatrixB_L is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_M.CreateSparseMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB_L);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return std::move(GlueMatrixA_M) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA_M only has one matrix

    if (GlueMatrixA_M.GetMatrixNumber() == 1)
    {
        int64 MatrixNumber_B = GlueMatrixB_L.GetMatrixNumber();

        GlueMatrixB_L.m_SourceMatrixSharedCopyList.resize(MatrixNumber_B + 1);

        GlueMatrixB_L.m_SourceMatrixSharedCopyList[MatrixNumber_B].ForceSharedCopy(GlueMatrixA_M.m_SourceMatrixSharedCopyList[0]);

        GlueMatrixB_L.m_ElementList_Coef.push_back(GlueMatrixA_M.m_Element_Coef);

        return GlueMatrixB_L;
    }
    else
    {
        return GlueMatrixA_M.CreateSparseMatrix() + std::move(GlueMatrixB_L);
    }
}

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType>& operator-(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_M.GetSize();

    auto SizeB = GlueMatrixB_L.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_M or GlueMatrixB_L is empty @ mdkSparseMatrixOperator: -(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_M.CreateSparseMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB_L);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return std::move(GlueMatrixA_M) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA_M only has one matrix

    if (GlueMatrixA_M.GetMatrixNumber() == 1)
    {
        int64 MatrixNumber_B = GlueMatrixB_L.GetMatrixNumber();

        GlueMatrixB_L.m_SourceMatrixSharedCopyList.resize(MatrixNumber_B + 1);

        GlueMatrixB_L.m_SourceMatrixSharedCopyList[MatrixNumber_B].ForceSharedCopy(GlueMatrixA_M.m_SourceMatrixSharedCopyList[0]);

        for (int64 i = 0; i < MatrixNumber_B; ++i)
        {
            GlueMatrixB_L.m_ElementList_Coef[i] = ElementType(0) - GlueMatrixB_L.m_ElementList_Coef[i];
        }

        GlueMatrixB_L.m_ElementList_Coef.push_back(GlueMatrixA_M.m_Element_Coef);

        GlueMatrixB_L.m_IndependentElement = ElementType(0) - GlueMatrixB_L.m_IndependentElement;

        return GlueMatrixB_L;
    }
    else
    {
        return GlueMatrixA_M.CreateSparseMatrix() - std::move(GlueMatrixB_L);
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L)
{
    return std::move(GlueMatrixA_M) * GlueMatrixB_L.CreateSparseMatrix();
}

// ---------------------------- SparseGlueMatrixForLinearCombination {+ - *} SparseGlueMatrixForMultiplication -------------------------------------------//

// note: SparseGlueMatrixForLinearCombination {+ -} SparseGlueMatrixForMultiplication return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForLinearCombination {*}   SparseGlueMatrixForMultiplication return SparseGlueMatrixForMultiplication
// note: SparseGlueMatrixForLinearCombination {/}   SparseGlueMatrixForMultiplication is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_L.GetSize();

    auto SizeB = GlueMatrixB_M.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_L or GlueMatrixB_L is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_L.CreateSparseMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB_M);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB_M.CreateSparseMatrix();

        return std::move(GlueMatrixA_L) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB_M only has one matrix

    if (GlueMatrixB_M.GetMatrixNumber() == 1)
    {
        int64 MatrixNumber_A = GlueMatrixA_L.GetMatrixNumber();

        GlueMatrixA_L.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + 1);

        GlueMatrixA_L.m_SourceMatrixSharedCopyList[MatrixNumber_A].ForceSharedCopy(GlueMatrixB_M.m_SourceMatrixSharedCopyList[0]);

        GlueMatrixA_L.m_ElementList_Coef.push_back(GlueMatrixB_M.m_Element_Coef);

        return GlueMatrixA_L;
    }
    else
    {
        return std::move(GlueMatrixA_L) + GlueMatrixB_M.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_L.GetSize();

    auto SizeB = GlueMatrixB_M.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_L or GlueMatrixB_L is empty @ mdkSparseMatrixOperator: -(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_L.CreateSparseMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB_M);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB_M.CreateSparseMatrix();

        return std::move(GlueMatrixA_L) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB_M only has one matrix

    if (GlueMatrixB_M.GetMatrixNumber() == 1)
    {
        int64 MatrixNumber_A = GlueMatrixA_L.GetMatrixNumber();

        GlueMatrixA_L.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + 1);

        GlueMatrixA_L.m_SourceMatrixSharedCopyList[MatrixNumber_A].ForceSharedCopy(GlueMatrixB_M.m_SourceMatrixSharedCopyList[0]);

        GlueMatrixA_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB_M.m_Element_Coef);

        return GlueMatrixA_L;
    }
    else
    {
        return std::move(GlueMatrixA_L) - GlueMatrixB_M.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    return GlueMatrixA_L.CreateSparseMatrix() * std::move(GlueMatrixB_M);
}

// ------------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseGlueMatrixForMultiplication ---------------------------------------//

// note: SparseGlueMatrixForMultiplication {+ -} SparseGlueMatrixForMultiplication return SparseGlueMatrixForLinearCombination
// note: SparseGlueMatrixForMultiplication {*}   SparseGlueMatrixForMultiplication return SparseMatrix
// note: SparseGlueMatrixForMultiplication {/}   SparseGlueMatrixForMultiplication is 0/0

template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator+(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return std::move(GlueMatrixA) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if one matrix in GlueMatrixA and one matrix in GlueMatrixB

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    if (MatrixNumber_A == 1 && MatrixNumber_B == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        //tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);
        //tempGlueMatrix_L.m_SourceMatrixSharedCopyList[0].ForceSharedCopy(GlueMatrixA.m_SourceMatrixSharedCopyList[0]);
        //more efficient:
        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[0]);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else if (MatrixNumber_A == 1 && MatrixNumber_B > 1)
    {
        return std::move(GlueMatrixA) + GlueMatrixB.CreateSparseMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        return GlueMatrixA.CreateSparseMatrix() + std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() + GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline 
mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: -(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return std::move(GlueMatrixA) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: -(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if one matrix in GlueMatrixA and one matrix in GlueMatrixB

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    if (MatrixNumber_A == 1 && MatrixNumber_B == 1)
    {
        mdkSparseGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList.resize(2);

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[0]);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(GlueMatrixA.m_Element_Coef);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_Element_Coef);

        return tempGlueMatrix_L;
    }
    else if (MatrixNumber_A == 1 && MatrixNumber_B > 1)
    {
        return std::move(GlueMatrixA) - GlueMatrixB.CreateSparseMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        return GlueMatrixA.CreateSparseMatrix() - std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() - GlueMatrixB.CreateSparseMatrix();
    }
}


template<typename ElementType>
inline
mdkSparseGlueMatrixForMultiplication<ElementType> operator*(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    //-------------------------------------------------------------------------------------------------------------------------------------------
    // follow user's grouping: (A*B)*(C*D), treat it as Matrix * Matrix
    //-------------------------------------------------------------------------------------------------------------------------------------------

    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkSparseMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateSparseMatrix();

        return tempScalarMatrixA(0) * std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkSparseMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateSparseMatrix();

        return std::move(GlueMatrixA) * tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkSparseMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkSparseGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();
    
    if (MatrixNumber_A == 1 && MatrixNumber_B == 1)
    {
        GlueMatrixA.m_ColNumber = GlueMatrixB.m_ColNumber;

        GlueMatrixA.m_SourceMatrixSharedCopyList.resize(2);

        GlueMatrixA.m_SourceMatrixSharedCopyList[1].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[0]);

        GlueMatrixA.m_Element_Coef *= GlueMatrixB.m_Element_Coef;

        return GlueMatrixA;
    }
    else if (MatrixNumber_A == 1 && MatrixNumber_B > 1)
    {
        std::move(GlueMatrixA) * GlueMatrixB.CreateSparseMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        GlueMatrixA.CreateSparseMatrix() * std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateSparseMatrix() * GlueMatrixB.CreateSparseMatrix();
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------
    // do NOT ignore user's grouping: (A*B)*(C*D),  treat it as A*B*C*D
    //  case 1: may lead to error, for example:
    //          A : 1x3, B: 3x1, C: 3x3, D: 3x3,  A*B is a scalar,  but B*C is meaningless
    //  case 2: numerical problem
    //          user may do this on purpose to ensure numerical accuracy
    //-------------------------------------------------------------------------------------------------------------------------------------------
}

} // namespace mdk

#endif
