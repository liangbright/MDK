﻿#ifndef mdk_GlueMatrixForLinearCombination_hpp
#define mdk_GlueMatrixForLinearCombination_hpp

#include <string>
#include <cmath>
#include <algorithm>

//#include "mdkGlueMatrixForLinearCombination.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType>::mdkGlueMatrixForLinearCombination()
{
    this->Reset();
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType>::mdkGlueMatrixForLinearCombination(mdkGlueMatrixForLinearCombination<ElementType>&& GlueMatrix)
{
    m_RowNumber = GlueMatrix.m_RowNumber;

    m_ColNumber = GlueMatrix.m_ColNumber;

    m_SourceMatrixSharedCopyList = std::move(GlueMatrix.m_SourceMatrixSharedCopyList);

    m_ElementList_Coef = std::move(GlueMatrix.m_ElementList_Coef);

    m_IndependentElement = GlueMatrix.m_IndependentElement;

    // clear the counter 
    GlueMatrix.m_RowNumber = 0;
    GlueMatrix.m_ColNumber = 0;
    //
}


template<typename ElementType>
inline
mdkGlueMatrixForLinearCombination<ElementType>::~mdkGlueMatrixForLinearCombination()
{
}


template<typename ElementType>
inline void mdkGlueMatrixForLinearCombination<ElementType>::Reset()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_GlueMatrixForLinearCombination_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_GlueMatrixForLinearCombination_ReservedCapacity);

    m_IndependentElement = m_IndependentElement - m_IndependentElement;
}


template<typename ElementType>
inline 
int64 mdkGlueMatrixForLinearCombination<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
int64 mdkGlueMatrixForLinearCombination<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
mdkMatrixSize mdkGlueMatrixForLinearCombination<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.ColNumber = m_ColNumber;

    Size.RowNumber = m_RowNumber;

    return Size;
}


template<typename ElementType>
inline
int64 mdkGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}



template<typename ElementType>
inline
bool mdkGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    if (m_RowNumber <= 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkGlueMatrixForLinearCombination<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
{
    if (m_RowNumber != OutputMatrix.GetRowNumber() || m_ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            OutputMatrix.Resize(m_RowNumber, m_ColNumber);            
        }
        else
        {
            mdkError << "Size does not match @ mdkGlueMatrixForLinearCombination::CreateMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    int64 MatrixNumber = m_SourceMatrixSharedCopyList.size();

    if (MatrixNumber == 0)
    {
        mdkError << "Self is empty @ mdkGlueMatrixForLinearCombination::CreateMatrix(OutputMatrix)" << '\n';
        return false;
    }

    std::vector<const mdkMatrix<ElementType>*> MatrixPtrList(MatrixNumber);

    for (int64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixPtrList[k] = &m_SourceMatrixSharedCopyList[k];
    }

    return MatrixLinearCombine(OutputMatrix, m_ElementList_Coef, MatrixPtrList, m_IndependentElement);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}

}//end namespace mdk

#endif
