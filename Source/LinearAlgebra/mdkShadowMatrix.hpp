#ifndef __mdkShadowMatrix_hpp
#define __mdkShadowMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkLinearAlgebra.h"
#include "mdkMatrix.h"
#include "mdkShadowMatrix.h"

namespace mdk
{

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix()
{
    this->Reset();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& LinearIndexList)
{
    // all the indexes in LinearIndexList are within bound
    // bound check is performed in mdkMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector LinearIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixShallowCopy.ForceShallowCopy(sourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_Flag_OutputVector = true;

    m_ElementNumber = LinearIndexList.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    m_SourceMatrixShallowCopy.ForceShallowCopy(sourceMatrix);

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_ColIndexList_source.resize(ColNumber_source);

    for (uint64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_RowIndexList_source.resize(RowNumber_source);

    for (uint64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }
    //-----------------------------------------------

    m_ElementNumber = RowNumber_source*ColNumber_source;

    m_RowNumber = RowNumber_source;

    m_ColNumber = ColNumber_source;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix,
                                              const std::vector<uint64>& RowIndexList,
                                              const std::vector<uint64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    


    if (RowIndexList.empty() == true || ColIndexList.empty() == true)
    {
        mdkError << "RowIndexList or ColIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector RowIndexList, std::vector ColIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixShallowCopy.ForceShallowCopy(sourceMatrix);

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
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix,
                                              const std::vector<uint64>& RowIndexList,
                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    if (RowIndexList.empty() == true)
    {
        mdkError << "RowIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector RowIndexList, ALL)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixShallowCopy.ForceShallowCopy(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (uint64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_Flag_OutputVector = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix,
                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                              const std::vector<uint64>& ColIndexList)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    if (ColIndexList.empty() == true)
    {
        mdkError << "ColIndexList is empty @ mdkShadowMatrix(sourceMatrix, ALL, std::vector ColIndexList)" << '\n';

        this->Reset();
        return;
    }

    m_SourceMatrixShallowCopy.ForceShallowCopy(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (uint64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkShadowMatrix<ElementType>&& ShadowMatrix)
{
    m_SourceMatrixShallowCopy.ForceShallowCopy(ShadowMatrix.m_SourceMatrixShallowCopy); // do not use std::move() !

    m_RowIndexList_source = std::move(ShadowMatrix.m_RowIndexList_source);

    m_ColIndexList_source = std::move(ShadowMatrix.m_ColIndexList_source);

    m_LinearIndexList_source = std::move(ShadowMatrix.m_LinearIndexList_source);

    m_Flag_OutputVector = ShadowMatrix.m_Flag_OutputVector;

    m_RowNumber = ShadowMatrix.m_RowNumber;

    m_ColNumber = ShadowMatrix.m_ColNumber;

    m_ElementNumber = ShadowMatrix.m_ElementNumber;

    m_NaNElement = ShadowMatrix.m_NaNElement;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::~mdkShadowMatrix()
{
}


template<typename ElementType>
inline void mdkShadowMatrix<ElementType>::Reset()
{
    m_SourceMatrixShallowCopy.Reset();

    m_RowIndexList_source.resize(0);

    m_ColIndexList_source.resize(0);

    m_LinearIndexList_source.resize(0);

    m_Flag_OutputVector = false;

    m_ElementNumber = 0;

    m_RowNumber = 0;

    m_ColNumber = 0;

    m_NaNElement = m_SourceMatrixShallowCopy.GetNaNElement();
}


template<typename ElementType>
inline bool mdkShadowMatrix<ElementType>::IsLinearIndexListOnly() const
{
    return m_Flag_OutputVector;
}


template<typename ElementType>
inline
uint64 mdkShadowMatrix<ElementType>::GetElementNumber() const
{
    return m_ElementNumber;
}


template<typename ElementType>
inline
uint64 mdkShadowMatrix<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
uint64 mdkShadowMatrix<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline
mdkMatrixSize mdkShadowMatrix<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.RowNumber = m_RowNumber;
    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
bool mdkShadowMatrix<ElementType>::IsEmpty() const
{
    if (m_ElementNumber == 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline 
const std::vector<uint64>& mdkShadowMatrix<ElementType>::GetRowIndexListOfSource() const
{
    return m_RowIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<uint64>& mdkShadowMatrix<ElementType>::GetColIndexListOfSource() const
{
    return m_ColIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<uint64>& mdkShadowMatrix<ElementType>::GetLinearIndexListOfSource() const
{
    return m_LinearIndexList_source;
}


template<typename ElementType>
inline 
const mdkMatrix<ElementType>& mdkShadowMatrix<ElementType>::GetSourceMatrixSharedCopy() const
{
    return m_SourceMatrixShallowCopy;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkShadowMatrix<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
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
            mdkError << "Size does not match @ mdkShadowMatrix::CreateMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    if (m_ElementNumber == 0)
    {
        mdkWarning << "Self is empty @ mdkShadowMatrix::CreateMatrix(OutputMatrix)" << '\n';
        return true;
    }

    auto ptrTemp = OutputMatrix.GetElementPointer();

    auto ptrSource = m_SourceMatrixShallowCopy.GetElementPointer();

    if (m_LinearIndexList_source.empty() == false)
    {
        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            ptrTemp[i] = ptrSource[m_LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

        for (uint64 j = 0; j < m_ColNumber; ++j)
        {
            auto Index = m_ColIndexList_source[j] * RowNumber_source;

            for (uint64 i = 0; i < m_RowNumber; ++i)
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
void mdkShadowMatrix<ElementType>::operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
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
            mdkError << "m_ElementNumber != ShadowMatrix.GetElementNumber() @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != ShadowMatrix.GetRowNumber() || m_ColNumber != ShadowMatrix.GetColNumber())
        {
            mdkError << "Size does not match @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }
    }

    //--------------------------------------------

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = ShadowMatrix[i];
    }

}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const mdkMatrix<ElementType>& targetMatrix)
{
    // MatrixA = MatrixA
    if (m_SourceMatrixShallowCopy.GetElementPointer() == targetMatrix.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementNumber != targetMatrix.GetElementNumber())
        {
            mdkError << "m_ElementNumber != targetMatrix.GetElementNumber() @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }
    }
    else
    {
        if (m_RowNumber != targetMatrix.GetRowNumber() || m_ColNumber != targetMatrix.GetColNumber())
        {
            mdkError << "Size does not match @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }
    }

    //--------------------------------------------

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = targetMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const ElementType& Element)
{
    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] = Element;
    }
}


template<typename ElementType>
inline
ElementType& mdkShadowMatrix<ElementType>::operator[](uint64 LinearIndex)
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        mdkError << "Invalid input @ mdkShadowMatrix::operator[i]" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
    }

    auto ColIndex = LinearIndex / m_RowNumber;

    auto RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& mdkShadowMatrix<ElementType>::operator[](uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        mdkError << "Invalid input @ mdkShadowMatrix::operator[i] const" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
    }

    auto ColIndex = LinearIndex / m_RowNumber;

    auto RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 LinearIndex)
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(i)" <<'\n';
		return m_NaNElement;
	}

#endif

	if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
   	}
	
    uint64 ColIndex = LinearIndex / m_RowNumber;

	uint64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

	uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementNumber)
    {
        mdkError << "Invalid input @ mdkShadowMatrix::operator(i) const" << '\n';
        return m_NaNElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
    }

    uint64 ColIndex = LinearIndex / m_RowNumber;

    uint64 RowIndex = LinearIndex % m_RowNumber;

    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

    uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(i, j)" << '\n';
		return m_NaNElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

    if (m_LinearIndexList_source.empty() == false)
	{
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

	uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
#if defined(MDK_DEBUG_ShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(i, j) const" << '\n';
		return m_NaNElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

    if (m_LinearIndexList_source.empty() == false)
	{
        return m_SourceMatrixShallowCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowNumber_source = m_SourceMatrixShallowCopy.GetRowNumber();

	uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixShallowCopy[LinearIndex_source];
}

//--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

template<typename ElementType>
inline 
void mdkShadowMatrix<ElementType>::operator+=(const mdkMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber == 0 || Size.RowNumber == 0)
    {
        mdkError << "Sefl or Matrix is empty @ mdkShadowMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += Matrix[i];
    }
}


template<typename ElementType>
inline 
void mdkShadowMatrix<ElementType>::operator-=(const mdkMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber == 0 || Size.RowNumber == 0)
    {
        mdkError << "Sefl or Matrix is empty @ mdkShadowMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= Matrix[i];
    }
}


template<typename ElementType>
inline 
void mdkShadowMatrix<ElementType>::operator*=(const mdkMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber == 0 || Size.RowNumber == 0)
    {
        mdkError << "Sefl or Matrix is empty @ mdkShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] *= Matrix[i];
    }
}


template<typename ElementType>
inline 
void mdkShadowMatrix<ElementType>::operator/=(const mdkMatrix<ElementType>& Matrix)
{
    auto Size = Matrix.GetSize();

    if (m_RowNumber == 0 || Size.RowNumber == 0)
    {
        mdkError << "Sefl or Matrix is empty @ mdkShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= Matrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= Matrix[i];
    }
}

//--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const ElementType& Element)
{
    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkShadowMatrix::operator+=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const ElementType& Element)
{
    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkShadowMatrix::operator-=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const ElementType& Element)
{
    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkShadowMatrix::operator*=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] *= Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const ElementType& Element)
{
    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkShadowMatrix::operator/=(Element)" << '\n';
        return;
    }

    //----------------------------------------------------//

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= Element;
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber || Size.RowNumber == 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkShadowMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] += ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operato-=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber || Size.RowNumber == 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkShadowMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] -= ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= ShadowMatrix[0];

        return;
    }

    if (m_ColNumber != Size.RowNumber || Size.RowNumber != Size.ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber || Size.RowNumber == 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkShadowMatrix::operator*=(ShadowMatrix)" << '\n';
        return;
    }

    //-----------------------------------------------------------

    (*this) = this->CreateMatrix() * ShadowMatrix.CreateMatrix();
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operato/=(ShadowMatrix)" << '\n';
        return;
    }

    if (m_RowNumber || Size.RowNumber == 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkShadowMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    //-------------------------------------------

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)[i] /= ShadowMatrix[i];
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateMatrix()
}

template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateMatrix()
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::ElementMultiply(const mdkMatrix<ElementType>& Matrix) const
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

    mdkMatrix<ElementType> tempMatrix;

    if (m_RowNumber != Size.RowNumber || m_ColNumber != Size.ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    if (m_RowNumber == 0 || Size.RowNumber == 0)
    {
        mdkWarning << "Self or Matrix is empty @ mdkShadowMatrix::ElementMultiply(Matrix)" << '\n';
        return  tempMatrix;
    }

    //----------------------------------------------------//

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    auto ptrInput = targetMatrix.GetElementPointer();

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ptrInput[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    return (*this) * Element;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix) const
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        return this->ElementMultiply(ShadowMatrix(0));
    }

    mdkMatrix<ElementType> tempMatrix;

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
        return tempMatrix;
    }

    if (m_RowNumber || Size.RowNumber == 0)
    {
        mdkError << "Self or ShadowMatrix is empty @ mdkShadowMatrix::ElementMultiply(ShadowMatrix)" << '\n';
        return tempMatrix;
    }

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    auto ptrTemp = tempMatrix.GetElementPointer();

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        ptrTemp[i] = (*this)[i] * ShadowMatrix[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    return this->ElementMultiply(GlueMatrix.CreateMatrix());
}


}//end namespace mdk

#endif
