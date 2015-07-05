#ifndef mdk_DenseGlueMatrixForLinearCombination_hpp
#define mdk_DenseGlueMatrixForLinearCombination_hpp

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
    m_RowCount = 0;

    m_ColCount = 0;

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
    m_RowCount = GlueMatrix.m_RowCount;

    m_ColCount = GlueMatrix.m_ColCount;

    m_SourceMatrixSharedCopyList = std::move(GlueMatrix.m_SourceMatrixSharedCopyList);

    m_ElementList_Coef = std::move(GlueMatrix.m_ElementList_Coef);

    m_IndependentElement = GlueMatrix.m_IndependentElement;

    // clear the counter 
    GlueMatrix.m_RowCount = 0;
    GlueMatrix.m_ColCount = 0;
    //
}


template<typename ElementType>
inline
DenseGlueMatrixForLinearCombination<ElementType>::~DenseGlueMatrixForLinearCombination()
{
}


template<typename ElementType>
inline 
int_max DenseGlueMatrixForLinearCombination<ElementType>::GetRowCount() const
{
    return m_RowCount;
}


template<typename ElementType>
inline 
int_max DenseGlueMatrixForLinearCombination<ElementType>::GetColCount() const
{
    return m_ColCount;
}


template<typename ElementType>
inline
MatrixSize DenseGlueMatrixForLinearCombination<ElementType>::GetSize() const
{
    MatrixSize Size;
    Size.ColCount = m_ColCount;
    Size.RowCount = m_RowCount;
    return Size;
}


template<typename ElementType>
inline
int_max DenseGlueMatrixForLinearCombination<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}



template<typename ElementType>
inline
bool DenseGlueMatrixForLinearCombination<ElementType>::IsEmpty() const
{
    return (m_RowCount <= 0);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix() const
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
bool DenseGlueMatrixForLinearCombination<ElementType>::CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const
{
	OutputMatrix.FastResize(m_RowCount, m_ColCount);

    int_max MatrixNumber = m_SourceMatrixSharedCopyList.size();
    if (MatrixNumber == 0)
    {
        MDK_Error("Self is empty @ mdkDenseGlueMatrixForLinearCombination::CreateDenseMatrix(OutputMatrix)")
        return false;
    }

	MatrixLinearCombine(OutputMatrix, m_ElementList_Coef, m_SourceMatrixSharedCopyList, m_IndependentElement);
	return true;
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
