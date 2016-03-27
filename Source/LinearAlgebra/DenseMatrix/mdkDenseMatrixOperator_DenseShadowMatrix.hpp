#pragma once

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
DenseMatrix<ElementType> operator+(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] + MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA + MatrixB(0);
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: +(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ptrB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator-(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] - MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA - MatrixB(0);
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: -(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ptrB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() * MatrixB;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] / MatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA / MatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: /(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, MatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ptrB[i];
    }

    return  tempMatrix;
}

// ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> operator+(const DenseMatrix<ElementType>& MatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ShadowMatrixA.CreateDenseMatrix();
}


template<typename ElementType>
inline
DenseMatrix<ElementType> operator-(const DenseMatrix<ElementType>& MatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixA - ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("MatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseMatrix<ElementType>& MatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA * ShadowMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseMatrix<ElementType>& MatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixA[0] / ShadowMatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixA / ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("MatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

// ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> operator+(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixA is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, ElementB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> operator-(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixA is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, ElementB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator*(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixA is empty @ mdkDenseMatrixOperator: *(ShadowMatrixA, ElementB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] * ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixA is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, ElementB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount * SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ElementB;
    }

    return  tempMatrix;
}

// ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> operator+(const ElementType& ElementA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ElementA;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator-(const ElementType& ElementA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixB is empty @ mdkDenseMatrixOperator: -(ElementA, ShadowMatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowCount * SizeB.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator*(const ElementType& ElementA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB * ElementA;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const ElementType& ElementA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    DenseMatrix<ElementType> tempMatrix;

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowCount <= 0)
    {
        MDK_Error("ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(ElementA, ShadowMatrixB)")
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowCount * SizeB.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

// ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> operator+(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] + ShadowMatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA + ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColCount * SizeA.RowCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator-(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA - ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColCount * SizeA.RowCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
DenseGlueMatrixForMultiplication<ElementType> operator*(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateDenseMatrix() * ShadowMatrixB.CreateDenseMatrix();
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> operator/(const DenseShadowMatrix<ElementType>& ShadowMatrixA, const DenseShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return ShadowMatrixA[0] / ShadowMatrixB;
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return ShadowMatrixA / ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("Size does not match @ mdkDenseMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    if (SizeA.RowCount <= 0 || SizeB.RowCount <= 0)
    {
        MDK_Warning("ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)")
        return  tempMatrix;
    }

    tempMatrix.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowCount*SizeA.ColCount;

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

} // namespace mdk

