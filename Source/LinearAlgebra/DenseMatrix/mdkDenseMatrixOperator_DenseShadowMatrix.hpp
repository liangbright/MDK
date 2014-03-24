#ifndef __mdkDenseMatrixOperator_DenseShadowMatrix_hpp
#define __mdkDenseMatrixOperator_DenseShadowMatrix_hpp

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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] + MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + MatrixB(0);
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - MatrixB(0);
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] / MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA / MatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: /(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or MatrixB is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA - ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "MatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA[0] / ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA / ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "MatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixA is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixA is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixA is empty @ mdkDenseMatrixOperator: *(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixA is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeB.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixB is empty @ mdkDenseMatrixOperator: -(ElementA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeB.RowNumber <= 0)
    {
        MDK_Error << "ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(ElementA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] + ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
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

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA[0] / ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA / ShadowMatrixB[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber <= 0 || SizeB.RowNumber <= 0)
    {
        MDK_Warning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkDenseMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

} // namespace mdk

#endif
