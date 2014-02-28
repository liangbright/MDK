#ifndef __mdkShadowMatrix_hpp
#define __mdkShadowMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "mdkMatrix.h"
#include "mdkShadowMatrix.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix()
{
    this->Clear();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& LinearIndexList)
{
    // assume all the elements in LinearIndexList are within bound
    // the ShadowMatrix is a col-vector

    if (LinearIndexList.size() == 0)
    {
        mdkWarning << "LinearIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector LinearIndexList)" << '\n';
    }

    m_RowNumber_source = sourceMatrix.GetRowNumber();

    m_ColNumber_source = sourceMatrix.GetColNumber();

    m_ElementNumber_source = m_RowNumber_source*m_ColNumber_source;

    m_ElementData_source = sourceMatrix.GetElementDataSharedPointer();

    m_LinearIndexList_source = LinearIndexList;

    m_IsLinearIndexListOnly = true;

    m_ElementNumber = LinearIndexList.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_EmptyElement = sourceMatrix.GetEmptyElement();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix,
                                              const std::vector<uint64>& RowIndexList,
                                              const std::vector<uint64>& ColIndexList)
{
    // assume all the elements in RowIndexList and ColIndexList are within bound

    if (RowIndexList.size() == 0 && ColIndexList.size() == 0)
    {
        mdkWarning << "RowIndexList and ColIndexList are empty @ mdkShadowMatrix(sourceMatrix, std::vector RowIndexList, std::vector ColIndexList)" << '\n';
    }

    m_RowNumber_source = sourceMatrix.GetRowNumber();

    m_ColNumber_source = sourceMatrix.GetColNumber();

    m_ElementNumber_source = m_RowNumber_source*m_ColNumber_source;

    m_ElementData_source = sourceMatrix.GetElementDataSharedPointer();

    if (RowIndexList.size() != 0 && ColIndexList.size() != 0)
    {
        m_RowIndexList_source = RowIndexList;

        m_ColIndexList_source = ColIndexList;
    }
    else if (RowIndexList.size() == 0 && ColIndexList.size() != 0)
    {
        m_RowIndexList_source.resize(m_RowNumber_source);
        for (uint64 i = 0; i < m_RowNumber_source; ++i)
        {
            m_RowIndexList_source[i] = i;
        }

        m_ColIndexList_source = ColIndexList;
    }
    else if (RowIndexList.size() != 0 && ColIndexList.size() == 0)
    {
        m_RowIndexList_source = RowIndexList;

        m_ColIndexList_source.resize(m_ColNumber_source);
        for (uint64 i = 0; i < m_ColNumber_source; ++i)
        {
            m_ColIndexList_source[i] = i;
        }
    }

    m_IsLinearIndexListOnly = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    m_RowNumber_source = ShadowMatrix.GetRowNumberOfSource();

    m_ColNumber_source = ShadowMatrix.GetColNumberOfSource();

    m_ElementNumber_source = ShadowMatrix.GetElementNumberOfSource();

    m_ElementData_source = ShadowMatrix.GetElementDataSharedPointerOfSource();

    m_RowIndexList_source = ShadowMatrix.GetRowIndexListOfSource();

    m_ColIndexList_source = ShadowMatrix.GetColIndexListOfSource();

    m_LinearIndexList_source = ShadowMatrix.GetLinearIndexListOfSource();

    m_IsLinearIndexListOnly = ShadowMatrix.IsLinearIndexListOnly();

    if (m_IsLinearIndexListOnly == true)
    {
        m_ElementNumber = m_LinearIndexList_source.size();
        m_RowNumber = m_ElementNumber;
        m_ColNumber = 1;
    }
    else
    {
        m_RowNumber = m_RowIndexList_source.size();

        m_ColNumber = m_ColIndexList_source.size();

        m_ElementNumber = m_RowNumber*m_ColNumber;
    }

    m_EmptyElement -= m_EmptyElement;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::~mdkShadowMatrix()
{
}


template<typename ElementType>
inline void mdkShadowMatrix<ElementType>::Clear()
{
    m_RowNumber_source = 0;

    m_ColNumber_source = 0;

    m_ElementNumber_source = 0;

    m_ElementData_source.reset();

    m_RowIndexList_source.resize(0);

    m_ColIndexList_source.resize(0);

    m_LinearIndexList_source.resize(0);

    m_IsLinearIndexListOnly = false;

    m_ElementNumber = 0;

    m_RowNumber = 0;

    m_ColNumber = 0;

    m_EmptyElement -= m_EmptyElement;
}


template<typename ElementType>
inline bool mdkShadowMatrix<ElementType>::IsLinearIndexListOnly() const
{
    return m_IsLinearIndexListOnly;
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
uint64 mdkShadowMatrix<ElementType>::GetRowNumberOfSource() const
{
    return m_RowNumber_source;
}


template<typename ElementType>
inline uint64 mdkShadowMatrix<ElementType>::GetColNumberOfSource() const
{
    return m_ColNumber_source;
}


template<typename ElementType>
inline 
uint64 mdkShadowMatrix<ElementType>::GetElementNumberOfSource() const
{
    return m_ElementNumber_source;
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
const std::shared_ptr<std::vector<ElementType>>& mdkShadowMatrix<ElementType>::GetElementDataSharedPointerOfSource() const
{
    return m_ElementData_source;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (m_ElementNumber == 0)
    {
        return tempMatrix;
    }

    if (m_IsLinearIndexListOnly == true)
    { // col vector
        tempMatrix.SetSize(m_ElementNumber, 1);

        auto RawPointer = tempMatrix.GetElementDataRawPointer();

        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            RawPointer[i] = (*m_ElementData_source)[m_LinearIndexList_source[i]];
        }
    }
    else
    {
        tempMatrix.SetSize(m_RowNumber, m_ColNumber);

        auto RawPointer = tempMatrix.GetElementDataRawPointer();

        for (uint64 j = 0; j < m_ColNumber; ++j)
        {
            for (uint64 i = 0; i < m_RowNumber; ++i)
            {
                auto LinearIndex_source = m_ColIndexList_source[j] * m_RowNumber_source + m_RowIndexList_source[i];

                RawPointer[0] = (*m_ElementData_source)[LinearIndex_source];

                ++RawPointer;
            }
        }
    }

    return tempMatrix;
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

    if (ShadowMatrix.IsLinearIndexListOnly() != m_IsLinearIndexListOnly)
    {
        mdkError << "sourceMatrix.IsLinearIndexListOnly() != m_IsLinearIndexListOnly @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
        return;
    }

    auto shadowElementNumber = ShadowMatrix.GetElementNumber();

    if (m_IsLinearIndexListOnly == true)
    {
        if (m_ElementNumber != shadowElementNumber)
        {
            mdkError << "m_ElementNumber != shadowElementNumber @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            (*m_ElementData_source)[m_LinearIndexList_source[i]] = ShadowMatrix(i);
        }
    }
    else
    {
        auto shadowRowNumber = ShadowMatrix.GetRowNumber();

        auto shadowColNumber = ShadowMatrix.GetColNumber();

        if (m_RowNumber != shadowRowNumber || m_ColNumber != shadowColNumber)
        {
            mdkError << "Size does not match @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_RowNumber; ++i)
        {
            for (uint64 j = 0; j < m_ColNumber; ++j)
            {
                uint64 LinearIndex_source = m_ColIndexList_source[j] * m_RowNumber_source + m_RowIndexList_source[i];

                (*m_ElementData_source)[LinearIndex_source] = ShadowMatrix(i, j);
            }
        }
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const mdkMatrix<ElementType>& sourceMatrix)
{
    // MatrixA = MatrixA
    if (this == &sourceMatrix)
    {
        return;
    }

    auto sourceElementNumber = sourceMatrix.GetElementNumber();

    if (m_IsLinearIndexListOnly == true)
    {
        if (m_ElementNumber != sourceElementNumber)
        {
            mdkError << "m_ElementNumber != sourceElementNumber @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            (*m_ElementData_source)[m_LinearIndexList_source[i]] = sourceMatrix(i);
        }
    }
    else
    {
        auto sourceRowNumber = sourceMatrix.GetRowNumber();

        auto sourceColNumber = sourceMatrix.GetColNumber();

        if (m_RowNumber != sourceRowNumber || m_ColNumber != sourceColNumber)
        {
            mdkError << "Size does not match @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_RowNumber; ++i)
        {
            for (uint64 j = 0; j < m_ColNumber; ++j)
            {
                uint64 LinearIndex_source = m_ColIndexList_source[j] * m_RowNumber_source + m_RowIndexList_source[i];

                (*m_ElementData_source)[LinearIndex_source] = sourceMatrix(i, j);
            }
        }
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const ElementType& Element)
{
	if (m_IsLinearIndexListOnly == true)
	{
		for (uint64 i = 0; i < m_ElementNumber; ++i)
	    {
            (*m_ElementData_source)[m_LinearIndexList_source[i]] = Element;
	    }
    }
	else
	{
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			for (uint64 j = 0; j < m_ColNumber; ++j)
			{
				uint64 LinearIndex_source = m_ColIndexList_source[j] * m_RowNumber_source + m_RowIndexList_source[i];

				(*m_ElementData_source)[LinearIndex_source] = Element;
			}
		}
	}
}


template<typename ElementType>
inline
ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 LinearIndex)
{
#if defined(MDK_ShadowMatrix_Operator_CheckBound)

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(LinearIndex)" <<'\n';
		return m_EmptyElement;
	}

#endif

	if (m_IsLinearIndexListOnly == true)
	{
		return (*m_ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 ColIndex = LinearIndex / m_RowNumber;

		uint64 RowIndex = LinearIndex % m_RowNumber;

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * m_RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*m_ElementData_source)[LinearIndex_source];
	}
}


template<typename ElementType>
inline
const ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 LinearIndex) const
{
#if defined(MDK_ShadowMatrix_Operator_CheckBound)

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(LinearIndex)" << '\n';
		return m_EmptyElement;
	}

#endif

	if (m_IsLinearIndexListOnly == true)
	{
		return (*m_ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 ColIndex = LinearIndex / m_RowNumber;

		uint64 RowIndex = LinearIndex % m_RowNumber;

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * m_RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*m_ElementData_source)[LinearIndex_source];
	}
}


template<typename ElementType>
inline
ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
#if defined(MDK_ShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(RowIndex, ColIndex)" << '\n';
		return m_EmptyElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

	if (m_IsLinearIndexListOnly == true)
	{
		return (*m_ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * m_RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*m_ElementData_source)[LinearIndex_source];
	}
}


template<typename ElementType>
inline
const ElementType& mdkShadowMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
#if defined(MDK_ShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkShadowMatrix::operator(RowIndex, ColIndex) const" << '\n';
		return m_EmptyElement;
	}

#endif

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

	if (m_IsLinearIndexListOnly == true)
	{
		return (*m_ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * m_RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*m_ElementData_source)[LinearIndex_source];
	}
}


template<typename ElementType>
inline	
mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        return ShadowMatrixA(0) + ShadowMatrixB;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        return ShadowMatrixA + ShadowMatrixB(0);
	}

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();


	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix operator+(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkShadowMatrix operator+(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrixA(i) + ShadowMatrixB(i);
	}

	return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        return ShadowMatrixA(0) - ShadowMatrixB;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        return ShadowMatrixA - ShadowMatrixB(0);
	}

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();


	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "ShadowMatrixA or ShadowMatrixB are empty @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrixA(i) - ShadowMatrixB(i);
	}

	return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        return ShadowMatrixA(0) * ShadowMatrixB;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        return ShadowMatrixA * ShadowMatrixB(0);
	}

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

	if (SizeA.ColNumber != SizeB.RowNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0 || SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
	{
		mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	uint64 IndexA = 0;

	uint64 IndexB = 0;

	ElementType sum = 0;

	for (uint64 j = 0; j < SizeB.ColNumber; ++j)
	{
		for (uint64 i = 0; i < SizeA.RowNumber; ++i)
		{
			sum = 0;

			IndexA = 0;

			for (uint64 k = 0; k < SizeA.ColNumber; ++k)
			{
				//sum += ShadowMatrixA(i,k) * ShadowMatrixB(k,j);
                sum += ShadowMatrixA(IndexA + i) * ShadowMatrixB(IndexB + k);

				IndexA += SizeA.RowNumber;
			}

			tempRawPointer[0] = sum;

			++tempRawPointer;
		}

		IndexB += SizeB.RowNumber;
	}

	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        return ShadowMatrixA(0) / ShadowMatrixB;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        return ShadowMatrixA / ShadowMatrixB(0);
	}

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrixA(i) / ShadowMatrixB(i);
	}

	return  tempMatrix;
}


//element-wise multiplication
template<typename ElementType>
inline
mdkMatrix<ElementType> operator%(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        return ShadowMatrixA(0) * ShadowMatrixB;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        return ShadowMatrixA * ShadowMatrixB(0);
	}

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrixA(i) * ShadowMatrixB(i);
	}

	return  tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator+(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkShadowMatrix operator+(Element, ShadowMatrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = Element + ShadowMatrix(i);
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator-(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = Element - ShadowMatrix(i);
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator*(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = Element * ShadowMatrix(i);
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator/(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = Element / ShadowMatrix(i);
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element)
{
    return Element + ShadowMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator-(Matrix, Element)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrix(i) - Element;
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element)
{
    return Element * ShadowMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

    auto Size = ShadowMatrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator-(Matrix, Element)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
        tempRawPointer[i] = ShadowMatrix(i) / Element;
	}

	return tempMatrix;
}


//---------------------------------------------
template<typename ElementType>
inline
mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) + MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA + MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();


    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix operator+(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB is empty @ mdkShadowMatrix operator+(ShadowMatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) + MatrixB(i);
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();


    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB are empty @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) - MatrixB(i);
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) * MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA * MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0 || SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    uint64 IndexA = 0;

    uint64 IndexB = 0;

    ElementType sum = 0;

    for (uint64 j = 0; j < SizeB.ColNumber; ++j)
    {
        for (uint64 i = 0; i < SizeA.RowNumber; ++i)
        {
            sum = 0;

            IndexA = 0;

            for (uint64 k = 0; k < SizeA.ColNumber; ++k)
            {
                //sum += ShadowMatrixA(i,k) * MatrixB(k,j);
                sum += ShadowMatrixA(IndexA + i) * MatrixB(IndexB + k);

                IndexA += SizeA.RowNumber;
            }

            tempRawPointer[0] = sum;

            ++tempRawPointer;
        }

        IndexB += SizeB.RowNumber;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) / MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA / MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) / MatrixB(i);
    }

    return  tempMatrix;
}


//element-wise multiplication
template<typename ElementType>
inline
mdkMatrix<ElementType> operator%(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) * MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA * MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) * MatrixB(i);
    }

    return  tempMatrix;
}

//----------------------------------------------

template<typename ElementType>
inline
mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    return  ShadowMatrixB + MatrixA;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = ShadowMatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return ShadowMatrixA(0) - MatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return ShadowMatrixA - MatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();


    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "ShadowMatrixA or ShadowMatrixB are empty @ mdkShadowMatrix operator-(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = ShadowMatrixA(i) - MatrixB(i);
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) * ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA * ShadowMatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0 || SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
    {
        mdkError << "ShadowMatrixA or ShadowMatrixB is empty @ mdkShadowMatrix operator*(ShadowMatrixA, ShadowMatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    uint64 IndexA = 0;

    uint64 IndexB = 0;

    ElementType sum = 0;

    for (uint64 j = 0; j < SizeB.ColNumber; ++j)
    {
        for (uint64 i = 0; i < SizeA.RowNumber; ++i)
        {
            sum = 0;

            IndexA = 0;

            for (uint64 k = 0; k < SizeA.ColNumber; ++k)
            {
                //sum += MatrixA(i,k) * ShadowMatrixB(k,j);
                sum += MatrixA(IndexA + i) * ShadowMatrixB(IndexB + k);

                IndexA += SizeA.RowNumber;
            }

            tempRawPointer[0] = sum;

            ++tempRawPointer;
        }

        IndexB += SizeB.RowNumber;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) / ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA / ShadowMatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = MatrixA(i) / ShadowMatrixB(i);
    }

    return  tempMatrix;
}


//element-wise multiplication
template<typename ElementType>
inline
mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = ShadowMatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixA(0) * ShadowMatrixB;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixA * ShadowMatrixB(0);
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = MatrixA(i) * ShadowMatrixB(i);
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

    ShadowMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber == 1 && ColNumber == 1)
	{
        (*this) += ShadowMatrix(0);

		return;
	}

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber) 
	{
		mdkError << "Size does not match @ mdkShadowMatrix::operator+=(ShadowMatrix)" << '\n';
		return;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "ShadowMatrix is empty @ mdkShadowMatrix::operator+=(ShadowMatrix)" << '\n';
		return;
	}

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
        (*this)(i) += ShadowMatrix(i);
	}
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

    ShadowMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber == 1 && ColNumber == 1)
	{
        (*this) -= ShadowMatrix(0);

		return;
	}

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix::operator-=(ShadowMatrix)" << '\n';
		return;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "ShadowMatrix is empty @ mdkShadowMatrix::operator-=(ShadowMatrix)" << '\n';
		return;
	}

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
        (*this)(i) -= ShadowMatrix(i);
	}
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

    ShadowMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber == 1 && ColNumber == 1)
	{
        (*this) *= ShadowMatrix(0);

		return;
	}

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix::operator*=(ShadowMatrix)" << '\n';
		return;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "ShadowMatrix is empty @ mdkShadowMatrix::operator*=(ShadowMatrix)" << '\n';
		return;
	}

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) *= ShadowMatrix(i);
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

    ShadowMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber == 1 && ColNumber == 1)
	{
		(*this) /= targetMatrix(0, 0);

		return;
	}

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkShadowMatrix::operator/=(ShadowMatrix)" << '\n';
		return;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "ShadowMatrix is empty @ mdkShadowMatrix::operator/=(ShadowMatrix)" << '\n';
		return;
	}

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
        (*this)(i) /= ShadowMatrix(i);
	}
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator+=(const mdkMatrix<ElementType>& targetMatrix)
{
    uint64 RowNumber;
    uint64 ColNumber;

    targetMatrix.GetSize(&RowNumber, &ColNumber);

    if (RowNumber == 1 && ColNumber == 1)
    {
        (*this) += targetMatrix(0, 0);

        return;
    }

    if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) += targetRawPointer[i];
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(const mdkMatrix<ElementType>& targetMatrix)
{
    uint64 RowNumber;
    uint64 ColNumber;

    targetMatrix.GetSize(&RowNumber, &ColNumber);

    if (RowNumber == 1 && ColNumber == 1)
    {
        (*this) -= targetMatrix(0, 0);

        return;
    }

    if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) -= targetRawPointer[i];
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(const mdkMatrix<ElementType>& targetMatrix)
{
    uint64 RowNumber;
    uint64 ColNumber;

    targetMatrix.GetSize(&RowNumber, &ColNumber);

    if (RowNumber == 1 && ColNumber == 1)
    {
        (*this) *= targetMatrix(0);

        return;
    }

    if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
    {
        mdkError << "Size can not change @ mdkShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkShadowMatrix::operator*=(Matrix)" << '\n';
        return;
    }

    (*this) = (*this) * targetMatrix;
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(const mdkMatrix<ElementType>& targetMatrix)
{
    uint64 RowNumber;
    uint64 ColNumber;

    targetMatrix.GetSize(&RowNumber, &ColNumber);

    if (RowNumber == 1 && ColNumber == 1)
    {
        (*this) /= targetMatrix(0, 0);

        return;
    }

    if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkShadowMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) /= targetRawPointer[i];
    }
}


template<typename ElementType>
inline	
void mdkShadowMatrix<ElementType>::operator+=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkShadowMatrix::operator+=(Element)" << '\n';
		return;
	}

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) += Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator-=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkShadowMatrix::operator-=(Element)" << '\n';
		return;
	}

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) -= Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator*=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkShadowMatrix::operator*=(Element)" << '\n';
		return;
	}

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) *= Element;
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator/=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkShadowMatrix::operator/=(Element)" << '\n';
		return;
	}

	if (double(abs(Element)) < 0.0000000001)
	{
		mdkWarning << " abs(Element) < 0.0000000001 @ mdkShadowMatrix::operator/=(Element)" << '\n';
	}

    for (uint64 i = 0; i < m_ElementNumber; ++i)
    {
        (*this)(i) /= Element;
    }
}


}//end namespace mdk

#endif