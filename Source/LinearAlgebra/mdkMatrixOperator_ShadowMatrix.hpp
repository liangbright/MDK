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
        return ShadowMatrixA(0) + MatrixB;
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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] + ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] + ptrB[i];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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
        return ShadowMatrixA(0) - MatrixB;
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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] - ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] - ptrB[i];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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
        return ShadowMatrixA(0) / MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA / MatrixB(0);
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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] / ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] / ptrB[i];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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
        return MatrixA - ShadowMatrixB(0);
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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto sptrB = ShadowMatrixB.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixB.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = ptrA[i] - (*sptrB)[LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixB.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixB.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixB.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = ptrA[i] - (*sptrB)[LinearIndexList_source[i]];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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
        return MatrixA / ShadowMatrixB(0);
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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto sptrB = ShadowMatrixB.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixB.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = ptrA[i] / (*sptrB)[LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixB.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixB.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixB.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = ptrA[i] / (*sptrB)[LinearIndexList_source[i]];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] + ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] + ElementB;
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] - ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] - ElementB;
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] * ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] * ElementB;
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = (*sptrA)[LinearIndexList_source[i]] / ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = (*sptrA)[LinearIndex_source] / ElementB;
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrB = ShadowMatrixB.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixB.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = ElementA - (*sptrB)[LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixB.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixB.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixB.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = ElementA - (*sptrB)[LinearIndex_source];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto sptrB = ShadowMatrixB.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixB.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrTemp[i] = ElementA / (*sptrB)[LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixB.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixB.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixB.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                ptrTemp[0] = ElementA / (*sptrB)[LinearIndex_source];
                ++ptrTemp;
            }
        }
    }

    //------------------------------------------------------//

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
        return ShadowMatrixA(0) + ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + ShadowMatrixB(0);
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

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) + ShadowMatrixB(i);
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
        return ShadowMatrixA(0) - ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - ShadowMatrixB(0);
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

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) - ShadowMatrixB(i);
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
        return ShadowMatrixA(0) / ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA / ShadowMatrixB(0);
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

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) / ShadowMatrixB(i);
    }

    return  tempMatrix;
}

//--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

template<typename ElementType>
inline void operator+=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: +=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA += MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] += ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] += ptrB[i];                
            }
        }
    }
}


template<typename ElementType>
inline 
void operator-=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: -=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA -= MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] -= ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] -= ptrB[i];
            }
        }
    }
}


template<typename ElementType>
inline 
void operator*=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: *=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA *= MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] *= ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] *= ptrB[i];
            }
        }
    }
}


template<typename ElementType>
inline void operator/=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or MatrixB is empty @ mdkMatrixOperator: /=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA /= MatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /=(ShadowMatrixA, MatrixB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] /= ptrB[i];
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] /= ptrB[i];
            }
        }
    }
}

//--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline 
void operator+=(mdkShadowMatrix<ElementType> ShadowMatrixA, const ElementType& ElementB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: +=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] += ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] += ElementB;
            }
        }
    }
}


template<typename ElementType>
inline 
void operator-=(mdkShadowMatrix<ElementType> ShadowMatrixA, const ElementType& ElementB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: -=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] -= ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] -= ElementB;
            }
        }
    }
}


template<typename ElementType>
inline 
void operator*=(mdkShadowMatrix<ElementType> ShadowMatrixA, const ElementType& ElementB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: *=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] *= ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] *= ElementB;
            }
        }
    }
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType> ShadowMatrixA, const ElementType& ElementB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA is empty @ mdkMatrixOperator: /=(ShadowMatrixA, ElementB)" << '\n';
        return;
    }

    if (double(std::abs(ElementB)) < 0.0000000001)
    {
        mdkWarning << " abs(ElementB) < 0.0000000001 @ mdkMatrixOperator: /=(ShadowMatrixA, ElementB)" << '\n';
    }

    //----------------------------------------------------//

    auto sptrA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetElementDataSharedPointer();

    auto LinearIndexList_source = ShadowMatrixA.GetLinearIndexListOfSource();

    //----------------------------------------------------//

    if (LinearIndexList_source.empty() == false)
    {
        auto ElementNumber = SizeA.ColNumber * SizeA.RowNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            (*sptrA)[LinearIndexList_source[i]] /= ElementB;
        }
    }
    else
    {
        auto RowNumber_source = ShadowMatrixA.GetSourceMatrixSharedCopy().GetRowNumber();

        auto ColIndexList_source = ShadowMatrixA.GetColIndexListOfSource();

        auto RowIndexList_source = ShadowMatrixA.GetRowIndexListOfSource();

        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            auto Index = ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                uint64 LinearIndex_source = Index + RowIndexList_source[i];

                (*sptrA)[LinearIndex_source] /= ElementB;
            }
        }
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

template<typename ElementType>
inline
void operator+=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA += ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: +=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeA.RowNumber || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: +=(ShadowMatrixA, ShadowMatrixB)" << '\n';
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
void operator-=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA -= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: -=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeA.RowNumber || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: -=(ShadowMatrixA, ShadowMatrixB)" << '\n';
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
void operator*=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA -= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: *=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeA.RowNumber || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: *=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    ShadowMatrixA = ShadowMatrixA.CreateMatrix() * ShadowMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        ShadowMatrixA /= ShadowMatrixB(0);

        return;
    }

    if (SizeB.RowNumber != SizeA.RowNumber || SizeB.ColNumber != SizeA.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrixOperator: /=(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return;
    }

    if (SizeA.RowNumber || SizeB.RowNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkMatrixOperator: /=(ShadowMatrixA, ShadowMatrixB)" << '\n';
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
void operator+=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA += GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator-=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
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
void operator/=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB)
{
    ShadowMatrixA /= GlueMatrixB.CreateMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void operator+=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA += GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator-=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA -= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator*=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA *= GlueMatrixB.CreateMatrix()
}


template<typename ElementType>
inline
void operator/=(mdkShadowMatrix<ElementType> ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB)
{
    ShadowMatrixA /= GlueMatrixB.CreateMatrix()
}


} // namespace mdk

#endif