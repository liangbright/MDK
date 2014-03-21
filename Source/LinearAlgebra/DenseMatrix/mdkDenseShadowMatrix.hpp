#ifndef __mdkDenseShadowMatrix_hpp
#define __mdkDenseShadowMatrix_hpp

//#include "mdkDenseShadowMatrix.h"

namespace mdk
{

template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix()
{
    this->Reset();
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList)
{
    // all the indexes in LinearIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkDenseShadowMatrix(sourceMatrix, std::vector LinearIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixSharedCopy.ForceSharedCopy(sourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_Flag_OutputVector = true;

    m_ElementNumber = LinearIndexList.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    m_SourceMatrixSharedCopy.ForceSharedCopy(sourceMatrix);

    m_Flag_OutputVector = true;

    //-----------------------------------------------

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

    m_NaNElement = sourceMatrix.GetNaNElement();

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix,
                                                        const std::vector<int64>& RowIndexList,
                                                        const std::vector<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    


    if (RowIndexList.empty() == true || ColIndexList.empty() == true)
    {
        mdkError << "RowIndexList or ColIndexList is empty @ mdkDenseShadowMatrix(sourceMatrix, std::vector RowIndexList, std::vector ColIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixSharedCopy.ForceSharedCopy(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix,
                                                        const std::vector<int64>& RowIndexList,
                                                        const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    if (RowIndexList.empty() == true)
    {
        mdkError << "RowIndexList is empty @ mdkDenseShadowMatrix(sourceMatrix, std::vector RowIndexList, ALL)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixSharedCopy.ForceSharedCopy(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (int64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_Flag_OutputVector = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_NaNElement = sourceMatrix.GetNaNElement();

    m_Flag_All_Col = true;
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix,
                                                        const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                        const std::vector<int64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    if (ColIndexList.empty() == true)
    {
        mdkError << "ColIndexList is empty @ mdkDenseShadowMatrix(sourceMatrix, ALL, std::vector ColIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixSharedCopy.ForceSharedCopy(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (int64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_NaNElement = sourceMatrix.GetNaNElement();

    m_Flag_All_Row = true;
}


template<typename ElementType>
inline 
mdkDenseShadowMatrix<ElementType>::mdkDenseShadowMatrix(mdkDenseShadowMatrix<ElementType>&& ShadowMatrix)
{
    m_SourceMatrixSharedCopy.ForceSharedCopy(ShadowMatrix.m_SourceMatrixSharedCopy); // do not use std::move() !

    m_RowIndexList_source = std::move(ShadowMatrix.m_RowIndexList_source);

    m_ColIndexList_source = std::move(ShadowMatrix.m_ColIndexList_source);

    m_LinearIndexList_source = std::move(ShadowMatrix.m_LinearIndexList_source);

    m_Flag_OutputVector = ShadowMatrix.m_Flag_OutputVector;

    m_RowNumber = ShadowMatrix.m_RowNumber;

    m_ColNumber = ShadowMatrix.m_ColNumber;

    m_ElementNumber = ShadowMatrix.m_ElementNumber;

    m_NaNElement = ShadowMatrix.m_NaNElement;

    m_Flag_All_Row = ShadowMatrix.m_Flag_All_Row;

    m_Flag_All_Col = ShadowMatrix.m_Flag_All_Col;
}


template<typename ElementType>
inline
mdkDenseShadowMatrix<ElementType>::~mdkDenseShadowMatrix()
{
}


template<typename ElementType>
inline void mdkDenseShadowMatrix<ElementType>::Reset()
{
    m_SourceMatrixSharedCopy.Reset();

    m_RowIndexList_source.resize(0);

    m_ColIndexList_source.resize(0);

    m_LinearIndexList_source.resize(0);

    m_Flag_OutputVector = false;

    m_ElementNumber = 0;

    m_RowNumber = 0;

    m_ColNumber = 0;

    m_NaNElement = m_SourceMatrixSharedCopy.GetNaNElement();

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;
}


template<typename ElementType>
inline bool mdkDenseShadowMatrix<ElementType>::IsLinearIndexListOnly() const
{
    return m_Flag_OutputVector;
}


template<typename ElementType>
inline
int64 mdkDenseShadowMatrix<ElementType>::GetElementNumber() const
{
    return m_ElementNumber;
}


template<typename ElementType>
inline
int64 mdkDenseShadowMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
int64 mdkDenseShadowMatrix<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline
mdkMatrixSize mdkDenseShadowMatrix<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.RowNumber = m_RowNumber;
    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
bool mdkDenseShadowMatrix<ElementType>::IsEmpty() const
{
    if (m_ElementNumber == 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
const std::vector<int64>& mdkDenseShadowMatrix<ElementType>::GetRowIndexListOfSource() const
{
    return m_RowIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int64>& mdkDenseShadowMatrix<ElementType>::GetColIndexListOfSource() const
{
    return m_ColIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int64>& mdkDenseShadowMatrix<ElementType>::GetLinearIndexListOfSource() const
{
    return m_LinearIndexList_source;
}


template<typename ElementType>
inline 
const mdkDenseMatrix<ElementType>& mdkDenseShadowMatrix<ElementType>::GetSourceMatrixSharedCopy() const
{
    return m_SourceMatrixSharedCopy;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::CreateDenseMatrix() const
{
    mdkDenseMatrix<ElementType> tempMatrix;

    this->CreateDenseMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkDenseShadowMatrix<ElementType>::CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const
{
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
            mdkError << "Size does not match @ mdkDenseShadowMatrix::CreateDenseMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    if (m_ElementNumber == 0)
    {
        mdkWarning << "Self is empty @ mdkDenseShadowMatrix::CreateDenseMatrix(OutputMatrix)" << '\n';
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
void mdkDenseShadowMatrix<ElementType>::operator=(const mdkDenseMatrix<ElementType>& targetMatrix)
{
    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementPointer() == targetMatrix.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != targetMatrix.GetElementNumber())
        {
            mdkError << "m_ElementNumber != targetMatrix.GetElementNumber() @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != targetMatrix.GetRowNumber() || m_ColNumber != targetMatrix.GetColNumber())
        {
            mdkError << "Size does not match @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }


    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {        
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = targetMatrix
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], targetMatrix);
            return;
        }
        else if (m_Flag_All_Row == true && m_ColIndexList_source.size() == 1) // SourceMatrix(:,j) = targetMatrix
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], targetMatrix);
            return;
        }
    }

    //-------------------------------------------------

    auto ptrMatrix = targetMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = ptrMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator=(const ElementType& Element)
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
void mdkDenseShadowMatrix<ElementType>::operator=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
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
            mdkError << "m_ElementNumber != ShadowMatrix.GetElementNumber() @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != ShadowMatrix.GetRowNumber() || m_ColNumber != ShadowMatrix.GetColNumber())
        {
            mdkError << "Size does not match @ mdkDenseShadowMatrix::operator=(mdkDenseShadowMatrix)" << '\n';
            return;
        }
    }

    //--------------------------------------------

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = ShadowMatrix[i];
    }

}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateMatrix();
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateMatrix();
}


template<typename ElementType>
inline
ElementType& mdkDenseShadowMatrix<ElementType>::operator[](int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        mdkError << "Invalid input @ mdkDenseShadowMatrix::operator[i]" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    auto ColIndex = LinearIndex / m_RowNumber;

    auto RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& mdkDenseShadowMatrix<ElementType>::operator[](int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        mdkError << "Invalid input @ mdkDenseShadowMatrix::operator[i] const" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    auto ColIndex = LinearIndex / m_RowNumber;

    auto RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& mdkDenseShadowMatrix<ElementType>::operator()(int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
	{
		mdkError << "Invalid input @ mdkDenseShadowMatrix::operator(i)" <<'\n';
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
const ElementType& mdkDenseShadowMatrix<ElementType>::operator()(int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber || LinearIndex < 0)
    {
        mdkError << "Invalid input @ mdkDenseShadowMatrix::operator(i) const" << '\n';
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
ElementType& mdkDenseShadowMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		mdkError << "Invalid input @ mdkDenseShadowMatrix::operator(i, j)" << '\n';
		return m_NaNElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

    if (m_LinearIndexList_source.empty() == false)
	{
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& mdkDenseShadowMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || RowIndex < 0 || ColIndex >= m_ColNumber || ColIndex < 0)
	{
		mdkError << "Invalid input @ mdkDenseShadowMatrix::operator(i, j) const" << '\n';
		return m_NaNElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

    if (m_LinearIndexList_source.empty() == false)
	{
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

	int64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}

//--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

template<typename ElementType>
inline 
void mdkDenseShadowMatrix<ElementType>::operator+=(const mdkDenseMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrInput = Matrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += ptrInput[i];
    }
}


template<typename ElementType>
inline 
void mdkDenseShadowMatrix<ElementType>::operator-=(const mdkDenseMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrInput = Matrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= ptrInput[i];
    }
}


template<typename ElementType>
inline 
void mdkDenseShadowMatrix<ElementType>::operator*=(const mdkDenseMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= Matrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    (*this) = this->CreateMatrix() * Matrix;
}


template<typename ElementType>
inline 
void mdkDenseShadowMatrix<ElementType>::operator/=(const mdkDenseMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or Matrix is empty @ mdkDenseShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    auto ptrInput = Matrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= ptrInput[i];
    }
}

//--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator+=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        mdkError << "Self is empty @ mdkDenseShadowMatrix::operator+=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += Element;
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator-=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        mdkError << "Self is empty @ mdkDenseShadowMatrix::operator-=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= Element;
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator*=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        mdkError << "Self is empty @ mdkDenseShadowMatrix::operator*=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] *= Element;
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator/=(const ElementType& Element)
{
    if (m_RowNumber <= 0)
    {
        mdkError << "Self is empty @ mdkDenseShadowMatrix::operator/=(Element)" << '\n';
        return;
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
void mdkDenseShadowMatrix<ElementType>::operator+=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator-=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operato-=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator*=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= ShadowMatrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    //-----------------------------------------------------------

    (*this) = this->CreateMatrix() * ShadowMatrix.CreateMatrix();
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator/=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::operato/=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= ShadowMatrix[i];
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator+=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator-=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator*=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator/=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator+=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator-=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator*=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}

template<typename ElementType>
inline
void mdkDenseShadowMatrix<ElementType>::operator/=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::ElementMultiply(const mdkDenseMatrix<ElementType>& Matrix) const
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

    mdkDenseMatrix<ElementType> tempMatrix;

    if (m_RowNumber != Size.RowNumber || m_ColNumber != Size.ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkWarning << "Self or Matrix is empty @ mdkDenseShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ptrInput = targetMatrix.GetElementPointer();

    for (int64 i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ptrInput[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    return (*this) * Element;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix) const
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        return this->ElementMultiply(ShadowMatrix(0));
    }

    mdkDenseMatrix<ElementType> tempMatrix;

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkDenseShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
        return tempMatrix;
    }

    if (m_RowNumber <= 0 || Size.RowNumber <= 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkDenseShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
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
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseShadowMatrix<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


}//end namespace mdk

#endif
