#ifndef __mdkGlueMatrixForLinearCombination_hpp
#define __mdkGlueMatrixForLinearCombination_hpp

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

    m_SourceMatrixShallowCopyList = std::move(GlueMatrix.m_SourceMatrixShallowCopyList);

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

    m_SourceMatrixShallowCopyList.resize(0);

    m_SourceMatrixShallowCopyList.reserve(MDK_GlueMatrixForLinearCombination_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_GlueMatrixForLinearCombination_ReservedCapacity);

    m_IndependentElement = ElementType(0);
}


template<typename ElementType>
inline 
uint64 mdkGlueMatrixForLinearCombination<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
uint64 mdkGlueMatrixForLinearCombination<ElementType>::GetColNumber() const
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
uint64 mdkGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixShallowCopyList.size();
}



template<typename ElementType>
inline
bool mdkGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    if (m_RowNumber == 0)
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

    if (m_RowNumber == 0)
    {
        return tempMatrix;
    }

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

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

    auto OutputRawPointer = OutputMatrix.GetElementDataRawPointer();

    auto MatrixNumber = m_SourceMatrixShallowCopyList.size();

    std::vector<const ElementType*> MatrixElementDataRawPtrList(MatrixNumber);

    for (uint64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixElementDataRawPtrList[k] = m_SourceMatrixShallowCopyList[k].GetElementDataRawPointer();
    }

    auto ElementNumber = m_RowNumber*m_ColNumber;

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
    else if (MatrixNumber == 9)
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
                                            + m_ElementList_Coef[7] * MatrixElementDataRawPtrList[7][LinearIndex]
                                            + m_ElementList_Coef[8] * MatrixElementDataRawPtrList[8][LinearIndex];
        }
    }
    else if (MatrixNumber == 10)
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
                                            + m_ElementList_Coef[7] * MatrixElementDataRawPtrList[7][LinearIndex]
                                            + m_ElementList_Coef[8] * MatrixElementDataRawPtrList[8][LinearIndex]
                                            + m_ElementList_Coef[9] * MatrixElementDataRawPtrList[9][LinearIndex];
        }
    }
    else
    {
        // two kinds of for-loop ------------------------------------------------------------

        // for-loop #1: faster than #2

        for (auto tempPtr = OutputRawPointer; tempPtr < OutputRawPointer + ElementNumber; ++tempPtr)
        {
            tempPtr[0] = m_IndependentElement;
        }

        for (uint64 k = 0; k < MatrixNumber; ++k)
        {
            auto Coef_k = m_ElementList_Coef[k];

            auto RawPtr_k = MatrixElementDataRawPtrList[k];

            for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
            {
                OutputRawPointer[LinearIndex] += Coef_k * RawPtr_k[LinearIndex];
            }

        }
        
        // for-loop #2:
        /*
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
        */

    }

    return true;
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
