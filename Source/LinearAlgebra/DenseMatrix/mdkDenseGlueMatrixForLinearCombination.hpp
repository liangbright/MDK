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
DenseGlueMatrixForLinearCombination<ElementType>::DenseGlueMatrixForLinearCombination()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity);

    m_ElementList_Coef.resize(0);

    m_ElementList_Coef.reserve(MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity);

    m_IndependentElement = ElementType(0);
}


template<typename ElementType>
inline
DenseGlueMatrixForLinearCombination<ElementType>::DenseGlueMatrixForLinearCombination(DenseGlueMatrixForLinearCombination<ElementType>&& GlueMatrix)
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
DenseGlueMatrixForLinearCombination<ElementType>::~DenseGlueMatrixForLinearCombination()
{
}


template<typename ElementType>
inline 
int_max 
DenseGlueMatrixForLinearCombination<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline 
int_max 
DenseGlueMatrixForLinearCombination<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
MatrixSize
DenseGlueMatrixForLinearCombination<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.ColNumber = m_ColNumber;

    Size.RowNumber = m_RowNumber;

    return Size;
}


template<typename ElementType>
inline
int_max 
DenseGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}



template<typename ElementType>
inline
bool DenseGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    return (m_RowNumber <= 0);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix() const
{
    DenseMatrix<ElementType> tempMatrix;

    if (this->IsEmpty() == false)
    {
        this->CreateDenseMatrix(tempMatrix);
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
bool 
DenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const
{
    if (m_RowNumber != OutputMatrix.GetRowNumber() || m_ColNumber != OutputMatrix.GetColNumber())
    {
        auto IsOK = OutputMatrix.FastResize(m_RowNumber, m_ColNumber);

        if (IsOK == false)
        {
            MDK_Error("Size does not match and can not be changed @ mdkDenseGlueMatrixForLinearCombination::CreateDenseMatrix(OutputMatrix)")
            return false;
        }
    }

    int_max MatrixNumber = m_SourceMatrixSharedCopyList.size();

    if (MatrixNumber == 0)
    {
        MDK_Error("Self is empty @ mdkDenseGlueMatrixForLinearCombination::CreateDenseMatrix(OutputMatrix)")
        return false;
    }

    std::vector<const DenseMatrix<ElementType>*> MatrixPtrList(MatrixNumber);

    for (int_max k = 0; k < MatrixNumber; ++k)
    {
        MatrixPtrList[k] = &m_SourceMatrixSharedCopyList[k];
    }

    return MatrixLinearCombine(OutputMatrix, m_ElementList_Coef, MatrixPtrList, m_IndependentElement);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseGlueMatrixForLinearCombination<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}

}//end namespace mdk

#endif
