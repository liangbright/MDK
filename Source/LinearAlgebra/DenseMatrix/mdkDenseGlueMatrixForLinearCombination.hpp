#ifndef __mdkDenseGlueMatrixForLinearCombination_hpp
#define __mdkDenseGlueMatrixForLinearCombination_hpp

#include <string>
#include <cmath>
#include <algorithm>

//#include "mdkDenseGlueMatrixForLinearCombination.h"

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkDenseGlueMatrixForLinearCombination<ElementType>::mdkDenseGlueMatrixForLinearCombination()
{
    this->Reset();
}


template<typename ElementType>
inline
mdkDenseGlueMatrixForLinearCombination<ElementType>::mdkDenseGlueMatrixForLinearCombination(mdkDenseGlueMatrixForLinearCombination<ElementType>&& GlueMatrix)
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
mdkDenseGlueMatrixForLinearCombination<ElementType>::~mdkDenseGlueMatrixForLinearCombination()
{
}


template<typename ElementType>
inline 
void 
mdkDenseGlueMatrixForLinearCombination<ElementType>::Reset()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity);

    m_IndependentElement = m_IndependentElement - m_IndependentElement;
}


template<typename ElementType>
inline 
int64 
mdkDenseGlueMatrixForLinearCombination<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
int64 
mdkDenseGlueMatrixForLinearCombination<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
mdkMatrixSize
mdkDenseGlueMatrixForLinearCombination<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.ColNumber = m_ColNumber;

    Size.RowNumber = m_RowNumber;

    return Size;
}


template<typename ElementType>
inline
int64 
mdkDenseGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}



template<typename ElementType>
inline
bool mdkDenseGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    if (m_RowNumber <= 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
mdkDenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix() const
{
    mdkDenseMatrix<ElementType> tempMatrix;

    this->CreateDenseMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool 
mdkDenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const
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
            mdkError << "Size does not match @ mdkDenseGlueMatrixForLinearCombination::CreateDenseMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    int64 MatrixNumber = m_SourceMatrixSharedCopyList.size();

    if (MatrixNumber == 0)
    {
        mdkError << "Self is empty @ mdkDenseGlueMatrixForLinearCombination::CreateDenseMatrix(OutputMatrix)" << '\n';
        return false;
    }

    std::vector<const mdkDenseMatrix<ElementType>*> MatrixPtrList(MatrixNumber);

    for (int64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixPtrList[k] = &m_SourceMatrixSharedCopyList[k];
    }

    return MatrixLinearCombine(OutputMatrix, m_ElementList_Coef, MatrixPtrList, m_IndependentElement);
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> 
mdkDenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkDenseMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> 
mdkDenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> 
mdkDenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> 
mdkDenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType>
mdkDenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}

}//end namespace mdk

#endif
