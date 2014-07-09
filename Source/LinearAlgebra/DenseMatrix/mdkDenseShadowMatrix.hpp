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
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const std::initializer_list<int_max>& LinearIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
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
                                                  const std::initializer_list<int_max>& RowIndexList,
                                                  const std::initializer_list<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
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
                                                  const std::initializer_list<int_max>& RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (int_max i = 0; i < ColNumber_source; ++i)
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
                                                  const std::initializer_list<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int_max i = 0; i < RowNumber_source; ++i)
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
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, std::vector<int_max> LinearIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_LinearIndexList_source = std::move(LinearIndexList);

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
                                                  std::vector<int_max> RowIndexList,
                                                  std::vector<int_max> ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = std::move(RowIndexList);

    m_ColIndexList_source = std::move(ColIndexList);

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
                                                  std::vector<int_max> RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = std::move(RowIndexList);

    m_ColIndexList_source.resize(ColNumber_source);

    for (int_max i = 0; i < ColNumber_source; ++i)
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
                                                  std::vector<int_max> ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_ColIndexList_source = std::move(ColIndexList);

    m_RowIndexList_source.resize(RowNumber_source);

    for (int_max i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
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
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& sourceMatrix, const DenseMatrix<int_max>& LinearIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_LinearIndexList_source.resize(LinearIndexList.GetElementNumber());

    for (int_max i = 0; i < LinearIndexList.GetElementNumber(); ++i)
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
                                                  const DenseMatrix<int_max>& RowIndexList,
                                                  const DenseMatrix<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowIndexList.GetElementNumber());

    for (int_max i = 0; i < RowIndexList.GetElementNumber(); ++i)
    {
        m_RowIndexList_source[i] = RowIndexList[i];
    }

    m_ColIndexList_source.resize(ColIndexList.GetElementNumber());

    for (int_max i = 0; i < ColIndexList.GetElementNumber(); ++i)
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
                                                  const DenseMatrix<int_max>& RowIndexList,
                                                  const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowIndexList.GetElementNumber());

    for (int_max i = 0; i < RowIndexList.GetElementNumber(); ++i)
    {
        m_RowIndexList_source[i] = RowIndexList[i];
    }

    m_ColIndexList_source.resize(ColNumber_source);

    for (int_max i = 0; i < ColNumber_source; ++i)
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
                                                  const DenseMatrix<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int_max i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source.resize(ColIndexList.GetElementNumber());

    for (int_max i = 0; i < ColIndexList.GetElementNumber(); ++i)
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
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    m_SourceMatrixSharedCopy.ForceShare(sourceMatrix);

    m_Flag_OutputVector = true;

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_ColIndexList_source.resize(ColNumber_source);

    for (int_max i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_RowIndexList_source.resize(RowNumber_source);

    for (int_max i = 0; i < RowNumber_source; ++i)
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

/*
// this function is not useful
template<typename ElementType>
inline 
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseShadowMatrix<ElementType>& InputShadowMatrix)
{
    // copy matrix data, not share
    m_SourceMatrixSharedCopy = InputShadowMatrix.CreateDenseMatrix();

    m_RowNumber = m_SourceMatrixSharedCopy.GetRowNumber();
    m_ColNumber = m_SourceMatrixSharedCopy.GetColNumber();
    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_Flag_All_Row = true;
    m_Flag_All_Col = true;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();
}
*/

template<typename ElementType>
inline 
DenseShadowMatrix<ElementType>::DenseShadowMatrix(DenseShadowMatrix<ElementType>&& InputShadowMatrix)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    m_SourceMatrixSharedCopy.ForceShare(InputShadowMatrix.m_SourceMatrixSharedCopy); // std::move will move the data, i.e., std::move = Take

    m_RowIndexList_source = std::move(InputShadowMatrix.m_RowIndexList_source);

    m_ColIndexList_source = std::move(InputShadowMatrix.m_ColIndexList_source);

    m_LinearIndexList_source = std::move(InputShadowMatrix.m_LinearIndexList_source);

    m_Flag_OutputVector = InputShadowMatrix.m_Flag_OutputVector;

    //-----------------------------------------------------

    m_RowNumber = InputShadowMatrix.m_RowNumber;

    m_ColNumber = InputShadowMatrix.m_ColNumber;

    m_ElementNumber = InputShadowMatrix.m_ElementNumber;

    m_Flag_All_Row = InputShadowMatrix.m_Flag_All_Row;

    m_Flag_All_Col = InputShadowMatrix.m_Flag_All_Col;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();

    // clear InputShadowMatrix :  not necessary
    // Do NOT Use InputShadowMatrix.m_SourceMatrixSharedCopy.clear() -> this will clear m_SourceMatrixSharedCopy of this ShadowMatrix
    //DenseMatrix<ElementType> EmptyMatrix(MDK_PURE_EMPTY_MATRIX);
    //InputShadowMatrix.m_SourceMatrixSharedCopy.SwapSmartPointer(EmptyMatrix);
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
int_max DenseShadowMatrix<ElementType>::GetElementNumber() const
{
    return m_ElementNumber;
}


template<typename ElementType>
inline
int_max DenseShadowMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
int_max DenseShadowMatrix<ElementType>::GetRowNumber() const
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
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetRowIndexListOfSource() const
{
    return m_RowIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetColIndexListOfSource() const
{
    return m_ColIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetLinearIndexListOfSource() const
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

    if (m_ElementNumber > 0)
    {
        this->CreateDenseMatrix(tempMatrix);
    }
    
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
        bool IsOK = false;

        if (m_Flag_OutputVector == true)
        {
            IsOK = OutputMatrix.FastResize(m_ElementNumber, 1);
        }
        else
        {
            IsOK = OutputMatrix.FastResize(m_RowNumber, m_ColNumber);
        }

        if (IsOK == false)
        {
            MDK_Error("Size does not match and can not change @ DenseShadowMatrix::CreateDenseMatrix(OutputMatrix)")
            return false;
        }
    }

    if (m_ElementNumber == 0)
    {
        MDK_Warning("Self is empty @ DenseShadowMatrix::CreateDenseMatrix(OutputMatrix)")
        return true;
    }

    auto ptrTemp = OutputMatrix.GetElementPointer();

    auto ptrSource = m_SourceMatrixSharedCopy.GetElementPointer();

    if (m_LinearIndexList_source.empty() == false)
    {
        for (int_max i = 0; i < m_ElementNumber; ++i)
        {
            ptrTemp[i] = ptrSource[m_LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

        for (int_max j = 0; j < m_ColNumber; ++j)
        {
            auto Index = m_ColIndexList_source[j] * RowNumber_source;

            for (int_max i = 0; i < m_RowNumber; ++i)
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
            MDK_Error("m_ElementNumber != InputMatrix.GetElementNumber() @ DenseShadowMatrix::operator=(DenseMatrix)")
            return;
        }
    }
    else
    {
        if (InputMatrix.IsVector() == true)
        {
            if (m_RowNumber != InputMatrix.GetElementNumber() && m_ColNumber != InputMatrix.GetElementNumber())
            {
                MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseMatrix)")
                return;
            }
        }
        else
        {
            if (m_RowNumber != InputMatrix.GetRowNumber() || m_ColNumber != InputMatrix.GetColNumber())
            {
                MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseMatrix)")
                return;
            }
        }
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {        
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputMatrix.GetElementPointer());
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputMatrix.GetElementPointer());
            return;
        }
    }

    //-------------------------------------------------

    auto ptrMatrix = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = ptrMatrix[i];
    }
}


template<typename ElementType>
template<int_max Length>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseVector<ElementType, Length>& InputVector)
{
    // note: template parameter Length may not = InputVector.GetElementNumber()

    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputVector.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != InputVector.GetElementNumber())
        {
            MDK_Error("m_ElementNumber != InputVector.GetElementNumber() @ DenseShadowMatrix::operator=(DenseVector)")
            return;
        }
    }
    else
    {
        if (m_RowNumber != InputVector.GetElementNumber() && m_ColNumber != InputVector.GetElementNumber())
        {
            MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseVector)")
            return;
        }
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputVector
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputVector.GetElementPointer());
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = InputVector
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputVector.GetElementPointer());
            return;
        }
    }

    //-------------------------------------------------

    auto ptrMatrix = InputVector.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = Element
        {
            m_SourceMatrixSharedCopy.FillCol(m_ColIndexList_source[0], Element);
            return;
        }
    }

    //------------------------------------------------------------

    for (int_max i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseShadowMatrix<ElementType>& InputShadowMatrix)
{
    // MatrixA = MatrixA
    if (this == &InputShadowMatrix)
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != InputShadowMatrix.GetElementNumber())
        {
            MDK_Error("m_ElementNumber != InputShadowMatrix.GetElementNumber() @ DenseShadowMatrix::operator=(mdkDenseShadowMatrix)")
            return;
        }
    }
    else
    {
        if (m_RowNumber != InputShadowMatrix.GetRowNumber() || m_ColNumber != InputShadowMatrix.GetColNumber())
        {
            MDK_Error("Size does not match @ DenseShadowMatrix::operator=(mdkDenseShadowMatrix)")
            return;
        }
    }

    //--------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) = InputShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementNumber; ++i)
        {
            (*this)[i] = InputShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateDenseMatrix();
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateDenseMatrix();
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator[](int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator[i]")
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowNumber;

    int_max RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator[](int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator[i] const")
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowNumber;

    int_max RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i)")
        return m_NaNElement;
	}

#endif

	if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
   	}
	
    int_max ColIndex = LinearIndex / m_RowNumber;

	int_max RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator(i) const")
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowNumber;

    int_max RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i, j)")
        return m_NaNElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i, j) const")
        return m_NaNElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

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
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator+=(InputMatrix)")
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator+=(Matrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '+', InputMatrix, false);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) += InputMatrix
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_ColIndexList_source[0], '+', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self or InputMatrix is empty @ DenseShadowMatrix::operator-=(InputMatrix)")
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator-=(InputMatrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) -= Matrix
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_ColIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator*=(InputMatrix)")
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= InputMatrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator*=(Matrix)")
        return;
    }

    //----------------------------------------------------//

    (*this) = this->CreateDenseMatrix() * InputMatrix;
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator/=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator/=(InputMatrix)")
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= InputMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator/=(InputMatrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '/', InputMatrix, false);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_ColIndexList_source[0], '/', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self is empty @ DenseShadowMatrix::operator+=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += Element
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '+', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) += Element
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_RowIndexList_source[0], '+', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self is empty @ DenseShadowMatrix::operator-=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= Element
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '-', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) -= Element
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_ColIndexList_source[0], '-', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self is empty @ DenseShadowMatrix::operator*=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) *= Element
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '*', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) *= Element
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_ColIndexList_source[0], '*', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Self is empty @ DenseShadowMatrix::operator/=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= Element
        {
            m_SourceMatrixSharedCopy.RowOperationInPlace(m_RowIndexList_source[0], '/', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) /= Element
        {
            m_SourceMatrixSharedCopy.ColOperationInPlace(m_RowIndexList_source[0], '/', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Size does not match @ DenseShadowMatrix::operator+=(ShadowMatrix)")
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator+=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) += ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Size does not match @ DenseShadowMatrix::operato-=(ShadowMatrix)")
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator-=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) -= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementNumber; ++i)
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
        MDK_Error("Size does not match @ DenseShadowMatrix::operator*=(ShadowMatrix)")
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator*=(ShadowMatrix)")
        return;
    }

    //-----------------------------------------------------------

    (*this) = this->CreateDenseMatrix() * ShadowMatrix.CreateDenseMatrix();
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
        MDK_Error("Size does not match @ DenseShadowMatrix::operato/=(ShadowMatrix)")
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator/=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) /= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementNumber; ++i)
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
    (*this) += GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateDenseMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateDenseMatrix()
}

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, InputMatrix);

    return tempMatrix;   
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
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
        MDK_Error("Size does not match @ DenseShadowMatrix::ElementMultiply(ShadowMatrix)")
        return tempMatrix;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::ElementMultiply(ShadowMatrix)")
        return tempMatrix;
    }

    tempMatrix.FastResize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ShadowMatrix[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    auto tempMatrixA = this->CreateDenseMatrix();

    auto tempMatrixB = GlueMatrix.CreateDenseMatrix();

    MatrixElementMultiply(tempMatrixA, tempMatrixA, tempMatrixB);

    return tempMatrixA;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    auto tempMatrixA = this->CreateDenseMatrix();

    auto tempMatrixB = GlueMatrix.CreateDenseMatrix();

    MatrixElementMultiply(tempMatrixA, tempMatrixA, tempMatrixB);

    return tempMatrixA;
}


}//end namespace mdk

#endif
