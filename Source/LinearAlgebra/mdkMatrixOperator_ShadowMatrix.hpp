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
mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: +(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ptrB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: -(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ptrB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return ShadowMatrixA.CreateMatrix() * MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: /(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrB = MatrixB.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ptrB[i];
    }

    return  tempMatrix;
}

// ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ShadowMatrixA.CreateMatrix();
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA - ShadowMatrixB[0];
    }

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "MatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return MatrixA * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) / ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA / ShadowMatrixB[0];
    }

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "MatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: /(MatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

// ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

template<typename ElementType>
inline
mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: +(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: -(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: *(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] * ElementB;
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: /(ShadowMatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ElementB;
    }

    return  tempMatrix;
}

// ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB + ElementA;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixB is empty @ mdkMatrixOperator: -(ElementA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixB * ElementA;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixB is empty @ mdkMatrixOperator: /(ElementA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeB.RowNumber * SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

// ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: +(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] + ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: -(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] - ShadowMatrixB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return ShadowMatrixA.CreateMatrix() * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
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

    mdkMatrix<ElementType> tempMatrix;

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: /(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.Resize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ShadowMatrixA[i] / ShadowMatrixB[i];
    }

    return  tempMatrix;
}

} // namespace mdk

#endif
