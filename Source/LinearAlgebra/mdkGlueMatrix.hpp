#ifndef __mdkGlueMatrix_hpp
#define __mdkGlueMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "mdkGlueMatrix.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkGlueMatrix<ElementType>::mdkGlueMatrix()
{
    this->Clear();
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType>::mdkGlueMatrix(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    m_RowNumber = GlueMatrix.m_RowNumber;

    m_ColNumber = GlueMatrix.m_ColNumber;

    m_MatrixElementDataSharedPointerList = GlueMatrix.m_MatrixElementDataSharedPointerList;

    m_ElementList_Coef = GlueMatrix.m_ElementList_Coef;

    m_IndependentElement = GlueMatrix.m_IndependentElement;
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType>::~mdkGlueMatrix()
{
}


template<typename ElementType>
inline void mdkGlueMatrix<ElementType>::Clear()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_MatrixElementDataSharedPointerList.resize(0);

    m_MatrixElementDataSharedPointerList.reserve(MDK_GlueMatrix_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_GlueMatrix_ReservedCapacity);

    m_IndependentElement -= m_IndependentElement;
}


template<typename ElementType>
inline uint64 mdkGlueMatrix<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline uint64 mdkGlueMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}

template<typename ElementType>
inline mdkMatrix<ElementType> mdkGlueMatrix<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (m_RowNumber == 0 || m_ColNumber == 0)
    {
        return tempMatrix;
    }

    tempMatrix.SetSize(m_RowNumber, m_ColNumber);

    auto RawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 LinearIndex = 0; LinearIndex < m_ColNumber*m_RowNumber; ++LinearIndex)
    {
        ElementType temp = m_IndependentElement;

        for (uint64 k = 0; k < m_MatrixElementDataSharedPointerList.size(); ++k)
        {
            temp += m_ElementList_Coef[k] * m_MatrixElementDataSharedPointerList[k]->at(LinearIndex);
        }

        RawPointer[LinearIndex] = temp;
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) + MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA + MatrixB(0);
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(MatrixA, MatrixB)" << '\n';
        mdkGlueMatrix<ElementType> tempGlueMatrix;
        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA or MatrixB is empty @ mdkGlueMatrix operator+(MatrixA, MatrixB)" << '\n';
        mdkGlueMatrix<ElementType> tempGlueMatrix;
        return  tempGlueMatrix;
    }

    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = SizeA.RowNumber;

    tempGlueMatrix.m_ColNumber = SizeA.ColNumber;

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(1);

    tempGlueMatrix.m_ElementList_Coef.push_back(1);

    return tempGlueMatrix;
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA - MatrixB(0);
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(MatrixA, MatrixB)" << '\n';
        mdkGlueMatrix<ElementType> tempGlueMatrix;
        return  tempGlueMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA or MatrixB is empty @ mdkGlueMatrix operator+(MatrixA, MatrixB)" << '\n';
        mdkGlueMatrix<ElementType> tempGlueMatrix;
        return  tempGlueMatrix;
    }

    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = SizeA.RowNumber;

    tempGlueMatrix.m_ColNumber = SizeA.ColNumber;

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(1);

    tempGlueMatrix.m_ElementList_Coef.push_back(-1);

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    if (GlueMatrixA.m_RowNumber != MatrixB.GetRowNumber() || GlueMatrixA.m_ColNumber != MatrixB.GetColNumber())
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(GlueMatrixA, MatrixB)" << '\n';
        return GlueMatrixA;
    }

    GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    GlueMatrixA.m_ElementList_Coef.push_back(1);

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    if (GlueMatrixA.m_RowNumber != MatrixB.GetRowNumber() || GlueMatrixA.m_ColNumber != MatrixB.GetColNumber())
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator-(GlueMatrixA, MatrixB)" << '\n';
        return GlueMatrixA;
    }

    GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(MatrixB.GetElementDataSharedPointer());

    GlueMatrixA.m_ElementList_Coef.push_back(-1);

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    if (GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber() || GlueMatrixB.m_ColNumber != MatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(MatrixA, GlueMatrixB)" << '\n';
        return GlueMatrixB;
    }

    GlueMatrixB.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    GlueMatrixB.m_ElementList_Coef.push_back(1);

    return GlueMatrixB;
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    if (GlueMatrixB.m_RowNumber != MatrixA.GetRowNumber() || GlueMatrixB.m_ColNumber != MatrixA.GetColNumber())
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator-(MatrixA, GlueMatrixB)" << '\n';
        return GlueMatrixB;
    }

    GlueMatrixB.m_MatrixElementDataSharedPointerList.push_back(MatrixA.GetElementDataSharedPointer());

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixB.m_ElementList_Coef[i] = -GlueMatrixB.m_ElementList_Coef[i];
    }

    GlueMatrixB.m_ElementList_Coef.push_back(1);

    return GlueMatrixB;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    if (GlueMatrixA.m_RowNumber != GlueMatrixB.m_RowNumber || GlueMatrixA.m_ColNumber != GlueMatrixB.m_ColNumber)
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(GlueMatrixA, GlueMatrixB)" << '\n';
        return GlueMatrixA;
    }

    for (uint64 i = 0; i < GlueMatrixB.m_MatrixElementDataSharedPointerList.size(); ++i)
    {
        GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_MatrixElementDataSharedPointerList[i]);
    }

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement += GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    if (GlueMatrixA.m_RowNumber != GlueMatrixB.m_RowNumber || GlueMatrixA.m_ColNumber != GlueMatrixB.m_ColNumber)
    {
        mdkError << "Size does not match @ mdkGlueMatrix operator+(GlueMatrixA, GlueMatrixB)" << '\n';
        return GlueMatrixA;
    }

    for (uint64 i = 0; i < GlueMatrixB.m_MatrixElementDataSharedPointerList.size(); ++i)
    {
        GlueMatrixA.m_MatrixElementDataSharedPointerList.push_back(GlueMatrixB.m_MatrixElementDataSharedPointerList[i]);
    }

    for (uint64 i = 0; i < GlueMatrixB.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrixA.m_ElementList_Coef.push_back(-GlueMatrixB.m_ElementList_Coef[i]);
    }

    GlueMatrixA.m_IndependentElement -= GlueMatrixB.m_IndependentElement;

    return GlueMatrixA;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = Matrix.GetRowNumber();

    tempGlueMatrix.m_ColNumber = Matrix.GetColNumber();

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(Matrix.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(1);

    tempGlueMatrix.m_IndependentElement = Element;

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = Matrix.GetRowNumber();

    tempGlueMatrix.m_ColNumber = Matrix.GetColNumber();

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(Matrix.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(-1);

    tempGlueMatrix.m_IndependentElement = Element;

    return tempGlueMatrix;
}

template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = Matrix.GetRowNumber();

    tempGlueMatrix.m_ColNumber = Matrix.GetColNumber();

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(Matrix.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(Element);

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator+(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix)
{
    GlueMatrix.m_IndependentElement += Element;

    return GlueMatrix;
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix)
{
    GlueMatrix.m_IndependentElement = Element - GlueMatrix.m_IndependentElement;

    for (uint64 i = 0; i < GlueMatrix.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrix.m_ElementList_Coef[i] *= -1;
    }

    return GlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator*(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix)
{
    GlueMatrix.m_IndependentElement *= Element;

    for (uint64 i = 0; i < GlueMatrix.m_ElementList_Coef.size(); ++i)
    {
        GlueMatrix.m_ElementList_Coef[i] *= Element;
    }

    return GlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    return Element + Matrix;
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = Matrix.GetRowNumber();

    tempGlueMatrix.m_ColNumber = Matrix.GetColNumber();

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(Matrix.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(1);

    tempGlueMatrix.m_IndependentElement = -Element;

    return tempGlueMatrix;
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator*(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    mdkGlueMatrix<ElementType> tempGlueMatrix;

    tempGlueMatrix.m_RowNumber = Matrix.GetRowNumber();

    tempGlueMatrix.m_ColNumber = Matrix.GetColNumber();

    tempGlueMatrix.m_MatrixElementDataSharedPointerList.push_back(Matrix.GetElementDataSharedPointer());

    tempGlueMatrix.m_ElementList_Coef.push_back(Element);

    return tempGlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType> operator/(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    return Matrix * (1 / Element);
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element)
{
    return Element + GlueMatrix;
}


template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element)
{
    GlueMatrix.m_IndependentElement -= Element;

    return GlueMatrix;
}

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator*(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element)
{
    return Element * GlueMatrix;
}


template<typename ElementType>
inline 
mdkGlueMatrix<ElementType>& operator/(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element)
{
    return (1/Element) * GlueMatrix;
}


}//end namespace mdk

#endif