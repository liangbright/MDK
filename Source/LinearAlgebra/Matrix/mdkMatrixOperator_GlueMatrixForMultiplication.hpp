#ifndef mdk_MatrixOperator_GlueMatrixForMultiplication_h
#define mdk_MatrixOperator_GlueMatrixForMultiplication_h


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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) + MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) + MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return GlueMatrixA.CreateMatrix() + MatrixB;
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) - MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) - MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return GlueMatrixA.CreateMatrix() - MatrixB;
    }
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or MatrixB is empty @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) * MatrixB;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return std::move(GlueMatrixA) * MatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    int64 MatrixNumber = GlueMatrixA.GetMatrixNumber();

    GlueMatrixA.m_ColNumber = SizeB.ColNumber;

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber+1);

    GlueMatrixA.m_SourceMatrixSharedCopyList[MatrixNumber].ForceSharedCopy(MatrixB);

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
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkMatrixOperator: +(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {      
        return MatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return MatrixA + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return MatrixA + GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or GlueMatrixB is empty @ mdkMatrixOperator: -(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return MatrixA - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA_ForMultiplication, MatrixB)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return MatrixA + GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = MatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        return MatrixA(0) * std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {      
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return MatrixA * tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(MatrixA, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

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
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    // check if GlueMatrixA only has one matrix

    if (GlueMatrixA.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixA.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixA.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixA.m_SourceMatrixSharedCopyList);

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

    GlueMatrixA.m_Is_m_Element_Coef_Equal_to_One = false;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator/(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB)
{
    GlueMatrixA.m_Element_Coef /= ElementB;

    GlueMatrixA.m_Is_m_Element_Coef_Equal_to_One = false;

    return GlueMatrixA;
}

// ---------------------------------------------- Element {+ - * /} GlueMatrixForMultiplication -----------------------------------------------------//

// note: Element {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Element {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Element {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

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
mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check if GlueMatrixB only has one matrix

    if (GlueMatrixB.GetMatrixNumber() == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

        tempGlueMatrix_L.m_RowNumber = GlueMatrixB.m_RowNumber;

        tempGlueMatrix_L.m_ColNumber = GlueMatrixB.m_ColNumber;

        tempGlueMatrix_L.m_SourceMatrixSharedCopyList = std::move(GlueMatrixB.m_SourceMatrixSharedCopyList);

        tempGlueMatrix_L.m_ElementList_Coef.push_back(ElementType(0) - GlueMatrixB.m_Element_Coef);

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

    GlueMatrixB.m_Is_m_Element_Coef_Equal_to_One = false;

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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) + ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return std::move(GlueMatrixA) - ShadowMatrixB.CreateMatrix();
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
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() + std::move(GlueMatrixB);
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    return ShadowMatrixA.CreateMatrix() - std::move(GlueMatrixB);
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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_M.GetSize();

    auto SizeB = GlueMatrixB_L.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_M or GlueMatrixB_L is empty @ mdkMatrixOperator: +(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_M.CreateMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB_L);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA_M) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
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
        return GlueMatrixA_M.CreateMatrix() + std::move(GlueMatrixB_L);
    }
}

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType>& operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_M.GetSize();

    auto SizeB = GlueMatrixB_L.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_M or GlueMatrixB_L is empty @ mdkMatrixOperator: -(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_M.CreateMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB_L);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA_M) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA_M, GlueMatrixB_L)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_L.GetSize();

    auto SizeB = GlueMatrixB_M.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_L or GlueMatrixB_L is empty @ mdkMatrixOperator: +(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_L.CreateMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB_M);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB_M.CreateMatrix();

        return std::move(GlueMatrixA_L) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
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
        return std::move(GlueMatrixA_L) + GlueMatrixB_M.CreateMatrix();
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA_L.GetSize();

    auto SizeB = GlueMatrixB_M.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA_L or GlueMatrixB_L is empty @ mdkMatrixOperator: -(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA_L.CreateMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB_M);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB_M.CreateMatrix();

        return std::move(GlueMatrixA_L) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA_L, GlueMatrixB_M)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
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
mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) + std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA) + tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if one matrix in GlueMatrixA and one matrix in GlueMatrixB

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    if (MatrixNumber_A == 1 && MatrixNumber_B == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return std::move(GlueMatrixA) + GlueMatrixB.CreateMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        return GlueMatrixA.CreateMatrix() + std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateMatrix() + GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
{
    // check size -----------------------------

    auto SizeA = GlueMatrixA.GetSize();

    auto SizeB = GlueMatrixB.GetSize();

    // empty -----------------------------------------------------------------------------------------------

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkMatrixOperator: -(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) - std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA) - tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_L;
        return EmptyGlueMatrix_L;
    }

    // check if one matrix in GlueMatrixA and one matrix in GlueMatrixB

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixNumber();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixNumber();

    if (MatrixNumber_A == 1 && MatrixNumber_B == 1)
    {
        mdkGlueMatrixForLinearCombination<ElementType> tempGlueMatrix_L;

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
        return std::move(GlueMatrixA) - GlueMatrixB.CreateMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        return GlueMatrixA.CreateMatrix() - std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateMatrix() - GlueMatrixB.CreateMatrix();
    }
}


template<typename ElementType>
inline
mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB)
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
        mdkError << "GlueMatrixA or GlueMatrixB is empty @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForMultiplication<ElementType> EmptyGlueMatrix_M;
        return EmptyGlueMatrix_M;
    }

    //scalar --------------------------------------------------------------------------------------------

    if (SizeA.RowNumber == 1 && SizeA.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixA = GlueMatrixA.CreateMatrix();

        return tempScalarMatrixA(0) * std::move(GlueMatrixB);
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        mdkMatrix<ElementType> tempScalarMatrixB = GlueMatrixB.CreateMatrix();

        return std::move(GlueMatrixA) * tempScalarMatrixB(0);
    }

    //matrix -------------------------------------------------------------------------------------------

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *(GlueMatrixA_ForMultiplication, GlueMatrixB_ForMultiplication)" << '\n';
        mdkGlueMatrixForLinearCombination<ElementType> EmptyGlueMatrix_M;
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
        std::move(GlueMatrixA) * GlueMatrixB.CreateMatrix();
    }
    else if (MatrixNumber_A > 1 && MatrixNumber_B == 1)
    {
        GlueMatrixA.CreateMatrix() * std::move(GlueMatrixB);
    }
    else
    {
        return GlueMatrixA.CreateMatrix() * GlueMatrixB.CreateMatrix();
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------
    // do NOT ignore user's grouping: (A*B)*(C*D),  treat it as A*B*C*D
    //  case 1: may lead to error, for example:
    //          A : 1x3, B: 3x1, C: 3x3, D: 3x3,  A*B is a scalar,  but B*C is meaningless
    //  case 2: numerical problem
    //          user may do this on purpose to ensure numerical accuracy
    /* -------------------------------------------------------------------------------------------------------------------------------------------

    int64 MatrixNumber_A = GlueMatrixA.GetMatrixSize();

    int64 MatrixNumber_B = GlueMatrixB.GetMatrixSize();

    GlueMatrixA.m_ColNumber = SizeB.ColNumber;

    GlueMatrixA.m_SourceMatrixSharedCopyList.resize(MatrixNumber_A + MatrixNumber_B);

    for (int64 i = MatrixNumber_A; i < MatrixNumber_A + MatrixNumber_B; ++i)
    {
        GlueMatrixA.m_SourceMatrixSharedCopyList[i].ForceSharedCopy(GlueMatrixB.m_SourceMatrixSharedCopyList[i - MatrixNumber_A]);
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
