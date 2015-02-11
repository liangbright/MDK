#ifndef mdk_SparseGlueMatrixForLinearCombination_hpp
#define mdk_SparseGlueMatrixForLinearCombination_hpp

#include <string>
#include <cmath>
#include <algorithm>

//#include "mdkSparseGlueMatrixForLinearCombination.h"

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
SparseGlueMatrixForLinearCombination<ElementType>::SparseGlueMatrixForLinearCombination()
{
    this->Reset();
}


template<typename ElementType>
inline
SparseGlueMatrixForLinearCombination<ElementType>::SparseGlueMatrixForLinearCombination(SparseGlueMatrixForLinearCombination<ElementType>&& GlueMatrix)
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
SparseGlueMatrixForLinearCombination<ElementType>::~SparseGlueMatrixForLinearCombination()
{
}


template<typename ElementType>
inline 
void 
SparseGlueMatrixForLinearCombination<ElementType>::Reset()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_SparseGlueMatrixForLinearCombination_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_SparseGlueMatrixForLinearCombination_ReservedCapacity);

    m_IndependentElement = m_IndependentElement - m_IndependentElement;
}


template<typename ElementType>
inline 
int64 
SparseGlueMatrixForLinearCombination<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
int64 
SparseGlueMatrixForLinearCombination<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
MatrixSize
SparseGlueMatrixForLinearCombination<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.ColNumber = m_ColNumber;

    Size.RowNumber = m_RowNumber;

    return Size;
}


template<typename ElementType>
inline
int64 
SparseGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}



template<typename ElementType>
inline
bool SparseGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    if (m_RowNumber <= 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
SparseMatrix<ElementType> 
SparseGlueMatrixForLinearCombination<ElementType>::CreateSparseMatrix() const
{
    SparseMatrix<ElementType> tempMatrix;

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool 
SparseGlueMatrixForLinearCombination<ElementType>::CreateSparseMatrix(SparseMatrix<ElementType>& OutputMatrix) const
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
            MDK_Error << "Size does not match @ SparseGlueMatrixForLinearCombination::CreateMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    int64 MatrixNumber = m_SourceMatrixSharedCopyList.size();

    if (MatrixNumber == 0)
    {
        MDK_Error << "Self is empty @ SparseGlueMatrixForLinearCombination::CreateMatrix(OutputMatrix)" << '\n';
        return false;
    }

    std::vector<const SparseMatrix<ElementType>*> MatrixPtrList(MatrixNumber);

    for (int64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixPtrList[k] = &m_SourceMatrixSharedCopyList[k];
    }

    return MatrixLinearCombine(OutputMatrix, m_ElementList_Coef, MatrixPtrList, m_IndependentElement);
}


template<typename ElementType>
inline
SparseMatrix<ElementType> 
SparseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const SparseMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> 
SparseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> 
SparseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const SparseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> 
SparseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
SparseMatrix<ElementType>
SparseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}

}//end namespace mdk

#endif
