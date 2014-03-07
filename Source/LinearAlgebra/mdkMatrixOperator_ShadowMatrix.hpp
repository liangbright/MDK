#ifndef __mdkMatrixOperator_ShadowMatrix_hpp
#define __mdkMatrixOperator_ShadowMatrix_hpp

namespace mdk
{

//============================================================================================================================================//
//
//                                          {ShadowMatrix} v.s {Matrix, Element, ShadowMatrix}
//
//============================================================================================================================================//

// ------------------------------------------ ShadowMatrix {+ - * /}  Matrix ----------------------------------------------------------//

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateMatrix() + MatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateMatrix() - MatrixB;
}


template<typename ElementType>
inline GlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateMatrix() * MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateMatrix() / MatrixB;
}

// ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>&, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA + ShadowMatrixA.CreateMatrix();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA - ShadowMatrixA.CreateMatrix();
}


template<typename ElementType>
inline 
GlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA * ShadowMatrixA.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA / ShadowMatrixA.CreateMatrix();
}

// ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, ElementType ElementB)
{
    return ShadowMatrixA.CreateMatrix() + ElementB;
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, ElementType ElementB)
{
    return ShadowMatrixA.CreateMatrix() - ElementB;
}


template<typename ElementType>
inline 
GlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, ElementType ElementB)
{
    return ShadowMatrixA.CreateMatrix() * ElementB;
}


template<typename ElementType>
inline 
GlueMatrixForMultiplication<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, ElementType ElementB)
{
    return ShadowMatrixA.CreateMatrix() / ElementB;
}

// ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(ElementType ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ElementA + ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(ElementType ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ElementA - ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
GlueMatrixForMultiplication<ElementType> operator*(ElementType ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ElementA * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(ElementType ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ElementA / ShadowMatrixB.CreateMatrix();
}

// ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateMatrix() + ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateMatrix() - ShadowMatrixB.CreateMatrix();
}

template<typename ElementType>
inline 
GlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateMatrix() * ShadowMatrixB.CreateMatrix();
}

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateMatrix() / ShadowMatrixB.CreateMatrix();
}

//--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

template<typename ElementType>
inline void operator+=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkMatrixOperator: +=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA += MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) += ptrB[i];
    }
}


template<typename ElementType>
inline 
void operator-=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "MatrixB is empty @ mdkMatrixOperator: -=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA -= MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) -= ptrB[i];
    }
}


template<typename ElementType>
inline 
void operator*=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    ShadowMatrixA = ShadowMatrixA.CreateMatrix() * MatrixB;
}


template<typename ElementType>
inline void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "MatrixB is empty @ mdkMatrixOperator: /=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA /= MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) /= ptrB[i];
    }
}

//--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    auto ElementNumber = ShadowMatrixA.GetElementNumber()

    if (ElementNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: +=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) += ElementB;
    }
}


template<typename ElementType>
inline 
void operator-=(EmdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    auto ElementNumber = ShadowMatrixA.GetElementNumber()

    if (ElementNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: -=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) -= ElementB;
    }
}


template<typename ElementType>
inline 
void operator*=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    auto ElementNumber = ShadowMatrixA.GetElementNumber()

    if (ElementNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: *=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) *= ElementB;
    }
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    auto ElementNumber = ShadowMatrixA.GetElementNumber()

    if (ElementNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: /=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    if (double(abs(ElementB)) < 0.0000000001)
    {
        mdkWarning << " abs(ElementB) < 0.0000000001 @ mdkMatrixOperator: /=(ShadowMatrixA, ElementB)" << '\n';
    }

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) /= ElementB;
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

template<typename ElementType>
inline
void operator+=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA += ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixB is empty @ mdkMatrixOperator: +=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) += ShadowMatrixB(i);
    }
}


template<typename ElementType>
inline
void operator-=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA -= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixB is empty @ mdkMatrixOperator: -=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) -= ShadowMatrixB(i);
    }
}


template<typename ElementType>
inline
void operator*=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA *= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixB is empty @ mdkMatrixOperator: *=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    ShadowMatrixA = ShadowMatrixA.CreateMatrix() * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA /= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != ShadowMatrixA.GetRowNumber() || SizeB.ColNumber != ShadowMatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixB is empty @ mdkMatrixOperator: /=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ShadowMatrixA(i) /= ShadowMatrixB(i);
    }
}


//----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void operator+=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA += GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator-=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA -= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator*=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA *= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA /= GlueMatrixB.CreateMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void operator+=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA += GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator-=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA -= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator*=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA *= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA /= GlueMatrixB.CreateMatrix()
}

} // namespace mdk

#endif