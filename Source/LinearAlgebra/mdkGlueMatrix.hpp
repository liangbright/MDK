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
    this->Reset();
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
mdkGlueMatrix<ElementType>::mdkGlueMatrix(mdkGlueMatrix<ElementType>&& GlueMatrix)
{
    m_RowNumber = GlueMatrix.m_RowNumber;

    m_ColNumber = GlueMatrix.m_ColNumber;

    m_MatrixElementDataSharedPointerList = std::move(GlueMatrix.m_MatrixElementDataSharedPointerList);

    m_ElementList_Coef = std::move(GlueMatrix.m_ElementList_Coef);

    m_IndependentElement = GlueMatrix.m_IndependentElement;
}


template<typename ElementType>
inline
mdkGlueMatrix<ElementType>::~mdkGlueMatrix()
{
}


template<typename ElementType>
inline void mdkGlueMatrix<ElementType>::Reset()
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
inline 
uint64 mdkGlueMatrix<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
uint64 mdkGlueMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
bool mdkGlueMatrix<ElementType>::IsEmpty() const
{
    if (m_RowNumber == 0 || m_ColNumber == 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrix<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    if (m_RowNumber == 0 || m_ColNumber == 0)
    {
        return tempMatrix;
    }

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
void mdkGlueMatrix<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
{
    auto OutputRawPointer = OutputMatrix.GetElementDataRawPointer();

    auto MatrixNumber = m_MatrixElementDataSharedPointerList.size();

    std::vector<ElementType*> MatrixElementDataRawPtrList(MatrixNumber);

    for (uint64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixElementDataRawPtrList[k] = m_MatrixElementDataSharedPointerList[k]->data();
    }

    auto ElementNumber = m_ColNumber*m_RowNumber;

    if (MatrixNumber == 1)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] =  m_IndependentElement
                                            + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex];
        }  
    }
    else if (MatrixNumber == 2)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                           + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                           + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex];
        }
    }
    else if (MatrixNumber == 3)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                           + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                           + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                           + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex];
        }
    }
    else if (MatrixNumber == 4)
    {        
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                           + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                           + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                           + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                                           + m_ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex];
        }
    }
    else if (MatrixNumber == 5)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                           + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                           + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                           + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                                           + m_ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                                           + m_ElementList_Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex];
        }
    }
    else if (MatrixNumber == 6)
    {        
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                           + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                           + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                           + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                                           + m_ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                                           + m_ElementList_Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                                           + m_ElementList_Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex];
        }
    }
    else if (MatrixNumber == 7)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                            + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                            + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                            + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                                            + m_ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                                            + m_ElementList_Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                                            + m_ElementList_Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                                            + m_ElementList_Coef[6] * MatrixElementDataRawPtrList[6][LinearIndex];
        }
    }
    else if (MatrixNumber == 8)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            OutputRawPointer[LinearIndex] = m_IndependentElement
                                            + m_ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                                            + m_ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                                            + m_ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                                            + m_ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                                            + m_ElementList_Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                                            + m_ElementList_Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                                            + m_ElementList_Coef[6] * MatrixElementDataRawPtrList[6][LinearIndex]
                                            + m_ElementList_Coef[7] * MatrixElementDataRawPtrList[7][LinearIndex];
        }
    }
    else
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            // must use a temp variable
            auto tempElement = m_IndependentElement;

            for (uint64 k = 0; k < MatrixNumber; ++k)
            {
                tempElement += m_ElementList_Coef[k] * MatrixElementDataRawPtrList[k][LinearIndex];
            }

            OutputRawPointer[LinearIndex] = tempElement;
        }
    }
}

// --------------------------------------- Matrix {+ -}  Matrix  -------------------------------------------------------------//

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

// ------------------------------------- Matrix {+ - * % /}  GlueMatrix ------------------------------------------------//

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
mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateMatrix();
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return MatrixA / GlueMatrixB.CreateMatrix();
}

// ---------------------------------------- GlueMatrix {+ - * % /}  Matrix ------------------------------------------------//

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
mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() * MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() % MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return GlueMatrixA.CreateMatrix() / MatrixB;
}


// ---------------------------------------- GlueMatrix {+ - * % /}  GlueMatrix ------------------------------------------------//

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
mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) * (GlueMatrixB.CreateMatrix());       
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) % (GlueMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) / (GlueMatrixB.CreateMatrix());
}

// ----------------------- GlueMatrix or ShadowMatrix {+ - * % /}  GlueMatrix or ShadowMatrix ------------------------------------//

// note: GlueMatrix {+ - * % /} ShadowMatrix returns Matrix

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) + (ShadowMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) - (ShadowMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) * (ShadowMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) % (ShadowMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return (GlueMatrixA.CreateMatrix()) / (ShadowMatrixB.CreateMatrix());
}

// note: ShadowMatrix {+ - * % /} GlueMatrix returns Matrix

template<typename ElementType>
inline 
mdkMatrix<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (ShadowMatrixA.CreateMatrix()) + (GlueMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (ShadowMatrixA.CreateMatrix()) - (GlueMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator*(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (ShadowMatrixA.CreateMatrix()) * (GlueMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator%(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (ShadowMatrixA.CreateMatrix()) % (GlueMatrixB.CreateMatrix());
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> operator/(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB)
{
    return (ShadowMatrixA.CreateMatrix()) / (GlueMatrixB.CreateMatrix());
}

// -------------------------------------------- Element {+ - *} Matrix ------------------------------------------------//

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

// -------------------------------------------- Element {+ - * /} GlueMatrix ------------------------------------------------//

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
mdkMatrix<ElementType> operator/(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix)
{
    return Element / (GlueMatrix.CreateMatrix());
}


// ------------------------------------ Matrix {+ - * /}  Element ------------------------------------------------//

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
    return Matrix * (ElementType(1) / Element);
}


// ------------------------------------ GlueMatrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element)
{
    GlueMatrix.m_IndependentElement += Element;

    return GlueMatrix;
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
    return (ElementType(1) / Element) * GlueMatrix;
}


}//end namespace mdk

#endif