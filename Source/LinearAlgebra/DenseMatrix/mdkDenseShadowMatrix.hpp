#ifndef __mdkDenseShadowMatrix_hpp
#define __mdkDenseShadowMatrix_hpp

//#include "mdkDenseShadowMatrix.h"

namespace mdk
{

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix()
{
    m_Flag_OutputVector = false;

    m_ElementNumber = 0;

    m_RowNumber = 0;

    m_ColNumber = 0;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}

//>>>
template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const std::initializer_list<int64>& LinearIndexList)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    m_ElementNumber = m_LinearIndexList_source.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const std::initializer_list<int64>& RowIndexList,
                                                  const std::initializer_list<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-----------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const std::initializer_list<int64>& RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (int64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_Flag_OutputVector = false;

    //------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                  const std::initializer_list<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}
//>>>

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    m_ElementNumber = m_LinearIndexList_source.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const std::vector<int64>& RowIndexList,
                                                  const std::vector<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-----------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const std::vector<int64>& RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (int64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_Flag_OutputVector = false;

    //------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                  const std::vector<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}



template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const DenseMatrix<int64>& LinearIndexList)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_LinearIndexList_source.resize(LinearIndexList.GetElementNumber());

    for (int64 i = 0; i < LinearIndexList.GetElementNumber(); ++i)
    {
        m_LinearIndexList_source[i] = LinearIndexList[i];
    }

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    m_ElementNumber = m_LinearIndexList_source.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const DenseMatrix<int64>& RowIndexList,
                                                  const DenseMatrix<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowIndexList.GetElementNumber());

    for (int64 i = 0; i < RowIndexList.GetElementNumber(); ++i)
    {
        m_RowIndexList_source[i] = RowIndexList[i];
    }

    m_ColIndexList_source.resize(ColIndexList.GetElementNumber());

    for (int64 i = 0; i < ColIndexList.GetElementNumber(); ++i)
    {
        m_ColIndexList_source[i] = ColIndexList[i];
    }

    m_Flag_OutputVector = false;

    //-----------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const DenseMatrix<int64>& RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowIndexList.GetElementNumber());

    for (int64 i = 0; i < RowIndexList.GetElementNumber(); ++i)
    {
        m_RowIndexList_source[i] = RowIndexList[i];
    }

    m_ColIndexList_source.resize(ColNumber_source);

    for (int64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_Flag_OutputVector = false;

    //------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = false;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                  const DenseMatrix<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source.resize(ColIndexList.GetElementNumber());

    for (int64 i = 0; i < ColIndexList.GetElementNumber(); ++i)
    {
        m_ColIndexList_source[i] = ColIndexList[i];
    }

    m_Flag_OutputVector = false;

    //-------------------------------------------

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}



template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_Flag_OutputVector = true;

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_ColIndexList_source.resize(ColNumber_source);

    for (int64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_RowIndexList_source.resize(RowNumber_source);

    for (int64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }
    //-----------------------------------------------

    m_ElementNumber = RowNumber_source*ColNumber_source;

    m_RowNumber = RowNumber_source;

    m_ColNumber = ColNumber_source;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline 
DenseShadowMatrix<ElementType>::DenseShadowMatrix(DenseShadowMatrix<ElementType>&& ShadowMatrix)
{
    m_SourceMatrixSharedCopy = std::move(ShadowMatrix.m_SourceMatrixSharedCopy);

    m_RowIndexList_source = std::move(ShadowMatrix.m_RowIndexList_source);

    m_ColIndexList_source = std::move(ShadowMatrix.m_ColIndexList_source);

    m_LinearIndexList_source = std::move(ShadowMatrix.m_LinearIndexList_source);

    m_Flag_OutputVector = ShadowMatrix.m_Flag_OutputVector;

    //-----------------------------------------------------

    m_RowNumber = ShadowMatrix.m_RowNumber;

    m_ColNumber = ShadowMatrix.m_ColNumber;

    m_ElementNumber = ShadowMatrix.m_ElementNumber;

    m_Flag_All_Row = ShadowMatrix.m_Flag_All_Row;

    m_Flag_All_Col = ShadowMatrix.m_Flag_All_Col;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::~DenseShadowMatrix()
{
}


template<typename ElementType>
inline bool DenseShadowMatrix<ElementType>::IsLinearIndexListOnly() const
{
    return m_Flag_OutputVector;
}


template<typename ElementType>
inline
int64 DenseShadowMatrix<ElementType>::GetElementNumber() const
{
    return m_ElementNumber;
}


template<typename ElementType>
inline
int64 DenseShadowMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
int64 DenseShadowMatrix<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline
MatrixSize DenseShadowMatrix<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.RowNumber = m_RowNumber;
    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
bool DenseShadowMatrix<ElementType>::IsEmpty() const
{
    return (m_ElementNumber == 0);
}


template<typename ElementType>
inline 
const std::vector<int64>& DenseShadowMatrix<ElementType>::GetRowIndexListOfSource() const
{
    return m_RowIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int64>& DenseShadowMatrix<ElementType>::GetColIndexListOfSource() const
{
    return m_ColIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int64>& DenseShadowMatrix<ElementType>::GetLinearIndexListOfSource() const
{
    return m_LinearIndexList_source;
}


template<typename ElementType>
inline 
const DenseMatrix<ElementType>& DenseShadowMatrix<ElementType>::GetSourceMatrixSharedCopy() const
{
    return m_SourceMatrixSharedCopy;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::CreateDenseMatrix() const
{
    DenseMatrix<ElementType> tempMatrix;

    this->CreateDenseMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseShadowMatrix<ElementType>::CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const
{
    if (m_SourceMatrixSharedCopy.GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        OutputMatrix = this->CreateDenseMatrix();

        return true;
    }

    if (m_RowNumber != OutputMatrix.GetRowNumber() || m_ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            if (m_Flag_OutputVector == true)
            {
                OutputMatrix.Resize(m_ElementNumber, 1);
            }
            else
            {
                OutputMatrix.Resize(m_RowNumber, m_ColNumber);
            }
        }
        else
        {
            MDK_Error << "Size does not match @ mdkDenseShadowMatrix::CreateDenseMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    if (m_ElementNumber == 0)
    {
        MDK_Warning << "Self is empty @ mdkDenseShadowMatrix::CreateDenseMatrix(OutputMatrix)" << '\n';
        return true;
    }

    auto ptrTemp = OutputMatrix.GetElementPointer();

    auto ptrSource = m_SourceMatrixSharedCopy.GetElementPointer();

    if (m_LinearIndexList_source.empty() == false)
    {
        for (int64 i = 0; i < m_ElementNumber; ++i)
        {
            ptrTemp[i] = ptrSource[m_LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

        for (int64 j = 0; j < m_ColNumber; ++j)
        {
            auto Index = m_ColIndexList_source[j] * RowNumber_source;

            for (int64 i = 0; i < m_RowNumber; ++i)
            {
                auto LinearIndex_source = Index + m_RowIndexList_source[i];

                ptrTemp[0] = ptrSource[LinearIndex_source];
                ++ptrTemp;
            }
        }
    }

    return true;
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseMatrix<ElementType>& InputMatrix)
{
    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputMatrix.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != InputMatrix.GetElementNumber())
        {
            MDK_Error << "m_ElementNumber != InputMatrix.GetElementNumber() @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != InputMatrix.GetRowNumber() || m_ColNumber != InputMatrix.GetColNumber())
        {
            MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }


    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {        
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputMatrix);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputMatrix);
            return;
        }
    }

    //-------------------------------------------------

    auto ptrMatrix = InputMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = ptrMatrix[i];
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const ElementType& Element)
{
    //------------------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = Element
        {
            m_SourceMatrixSharedCopy.FillRow(m_RowIndexList_source[0], Element);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) = Element
        {
            m_SourceMatrixSharedCopy.FillCol(m_ColIndexList_source[0], Element);
            return;
        }
    }

    //------------------------------------------------------------

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    // MatrixA = MatrixA
    if (this == &ShadowMatrix)
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != ShadowMatrix.GetElementNumber())
        {
            MDK_Error << "m_ElementNumber != ShadowMatrix.GetElementNumber() @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != ShadowMatrix.GetRowNumber() || m_ColNumber != ShadowMatrix.GetColNumber())
        {
            MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }

    //--------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) = ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int64 i = 0; i < m_ElementNumber; ++i)
        {
            (*this)[i] = ShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateMatrix();
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateMatrix();
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator[](int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator[i]" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int64 ColIndex = LinearIndex / m_RowNumber;

    int64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator[](int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator[i] const" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int64 ColIndex = LinearIndex / m_RowNumber;

    int64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
	{
		MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator(i)" <<'\n';
        return m_NaNElement;
	}

#endif

	if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
   	}
	
    int64 ColIndex = LinearIndex / m_RowNumber;

	int64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
    {
        MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator(i) const" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int64 ColIndex = LinearIndex / m_RowNumber;

    int64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator(i, j)" << '\n';
        return m_NaNElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid input @ mdkDenseShadowMatrix::operator(i, j) const" << '\n';
        return m_NaNElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}

//-------------------------------------------- DenseShadowMatrix {+= -= *= /=} DenseMatrix ------------------------------------------------//

template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator+=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator+=(InputMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += InputMatrix
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '+', InputMatrix, false);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) += InputMatrix
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_ColIndexList_source[0], '+', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += ptrInput[i];
    }
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator-=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or InputMatrix is empty @ mdkDenseShadowMatrix::operator-=(InputMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator-=(InputMatrix)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) -= Matrix
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_ColIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= ptrInput[i];
    }
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator*=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator*=(InputMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= InputMatrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    (*this) = this->CreateMatrix() * InputMatrix;
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator/=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator/=(InputMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator/=(InputMatrix)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '/', InputMatrix, false);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_ColIndexList_source[0], '/', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= ptrInput[i];
    }
}

//--------------------------------------------- DenseShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseShadowMatrix::operator+=(Element)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += Element
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '+', Element);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) += Element
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_RowIndexList_source[0], '+', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseShadowMatrix::operator-=(Element)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= Element
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '-', Element);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) -= Element
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_ColIndexList_source[0], '-', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseShadowMatrix::operator*=(Element)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) *= Element
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '*', Element);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) *= Element
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_ColIndexList_source[0], '*', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] *= Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseShadowMatrix::operator/=(Element)" << '\n';
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= Element
        {
            m_SourceMatrixSharedCopy.RowNamedOperationInPlace(m_RowIndexList_source[0], '/', Element);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) /= Element
        {
            m_SourceMatrixSharedCopy.ColNamedOperationInPlace(m_RowIndexList_source[0], '/', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= Element;
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) += ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int64 i = 0; i < m_ElementNumber; ++i)
        {
            (*this)[i] += ShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operato-=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) -= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int64 i = 0; i < m_ElementNumber; ++i)
        {
            (*this)[i] -= ShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= ShadowMatrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    //-----------------------------------------------------------

    (*this) = this->CreateMatrix() * ShadowMatrix.CreateMatrix();
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::operato/=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) /= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int64 i = 0; i < m_ElementNumber; ++i)
        {
            (*this)[i] /= ShadowMatrix[i];
        }
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& Matrix) const
{
    auto Size = Matrix.GetSize();

    if (m_ColNumber == 1 && m_RowNumber == 1)
    {
        return (*this)[0] * Matrix;
    }

    if (Size.ColNumber == 1 && Size.RowNumber == 1)
    {
        return (*this) * Matrix[0];
    }

    DenseMatrix<ElementType> tempMatrix;

    if (m_RowNumber != Size.RowNumber || m_ColNumber != Size.ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Warning << "Self or Matrix is empty @ mdkDenseShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ptrInput[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    return (*this) * Element;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        return this->ElementMultiply(ShadowMatrix(0));
    }

    DenseMatrix<ElementType> tempMatrix;

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error << "Size does not match @ mdkDenseShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
        return tempMatrix;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
        return tempMatrix;
    }

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ShadowMatrix[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


}//end namespace mdk

#endif
