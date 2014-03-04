#ifndef __mdkShadowMatrix_hpp
#define __mdkShadowMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "mdkLinearAlgebra.h"
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

    m_SourceMatrixSharedCopy.Share(sourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_IsLinearIndexListOnly = true;

    m_ElementNumber = LinearIndexList.size();

    m_RowNumber = m_ElementNumber;

    m_ColNumber = 1;

    m_NaNElement = sourceMatrix.GetNaNElement();
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix,
                                              const std::vector<uint64>& RowIndexList,
                                              const std::vector<uint64>& ColIndexList)
{
    // assume all the elements in RowIndexList and ColIndexList are within bound

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkWarning << "RowIndexList or ColIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector RowIndexList, std::vector ColIndexList)" << '\n';        
    }

    m_SourceMatrixSharedCopy.Share(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source = ColIndexList;

    m_IsLinearIndexListOnly = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix,
                                              const std::vector<uint64>& RowIndexList,
                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    // assume all the elements in RowIndexList and ColIndexList are within bound

    if (RowIndexList.size() == 0)
    {
        mdkWarning << "RowIndexList is empty @ mdkShadowMatrix(sourceMatrix, std::vector RowIndexList, ALL)" << '\n';
    }

    m_SourceMatrixSharedCopy.Share(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColNumber_source);

    for (uint64 i = 0; i < ColNumber_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_IsLinearIndexListOnly = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix,
                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                              const std::vector<uint64>& ColIndexList)
{
    // assume all the elements in RowIndexList and ColIndexList are within bound

    if (ColIndexList.size() == 0)
    {
        mdkWarning << "ColIndexList is empty @ mdkShadowMatrix(sourceMatrix, ALL, std::vector ColIndexList)" << '\n';
    }

    m_SourceMatrixSharedCopy.Share(sourceMatrix);

    auto RowNumber_source = sourceMatrix.GetRowNumber();

    auto ColNumber_source = sourceMatrix.GetColNumber();

    m_RowIndexList_source.resize(RowNumber_source);

    for (uint64 i = 0; i < RowNumber_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_IsLinearIndexListOnly = false;

    m_RowNumber = m_RowIndexList_source.size();

    m_ColNumber = m_ColIndexList_source.size();

    m_ElementNumber = m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType>::mdkShadowMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    m_SourceMatrixSharedCopy.Share(ShadowMatrix.GetSourceMatrixSharedCopy());

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

    m_NaNElement -= m_NaNElement;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>::~mdkShadowMatrix()
{
}


template<typename ElementType>
inline void mdkShadowMatrix<ElementType>::Clear()
{
    m_SourceMatrixSharedCopy.Clear(); // this will not clear the data of SourceMatrix

    m_RowIndexList_source.resize(0);

    m_ColIndexList_source.resize(0);

    m_LinearIndexList_source.resize(0);

    m_IsLinearIndexListOnly = false;

    m_ElementNumber = 0;

    m_RowNumber = 0;

    m_ColNumber = 0;

    m_NaNElement -= m_NaNElement;
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
bool mdkShadowMatrix<ElementType>::IsEmpty() const
{
    if (m_RowNumber == 0 || m_ColNumber == 0)
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
    return m_SourceMatrixSharedCopy;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkShadowMatrix<ElementType>::CreateMatrix() const
{
    return m_SourceMatrixSharedCopy.GetSubMatrix(m_RowIndexList_source, m_ColIndexList_source);
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
{
    return m_SourceMatrixSharedCopy.GetSubMatrix(OutputMatrix, m_RowIndexList_source, m_ColIndexList_source);
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    if (ShadowMatrix.IsLinearIndexListOnly() != m_IsLinearIndexListOnly)
    {
        mdkError << "sourceMatrix.IsLinearIndexListOnly() != m_IsLinearIndexListOnly @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
        return;
    }

    auto targetElementNumber = ShadowMatrix.GetElementNumber();

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

    if (m_IsLinearIndexListOnly == true)
    {
        if (m_ElementNumber != targetElementNumber)
        {
            mdkError << "m_ElementNumber != shadowElementNumber @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            (*ElementData_source)[m_LinearIndexList_source[i]] = ShadowMatrix(i);
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

        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

        for (uint64 i = 0; i < m_RowNumber; ++i)
        {
            for (uint64 j = 0; j < m_ColNumber; ++j)
            {
                uint64 LinearIndex_source = m_ColIndexList_source[j] * RowNumber_source + m_RowIndexList_source[i];

                (*ElementData_source)[LinearIndex_source] = ShadowMatrix(i, j);
            }
        }
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const mdkMatrix<ElementType>& sourceMatrix)
{
    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementDataRawPointer() == sourceMatrix.GetElementDataRawPointer())
    {
        return;
    }

    auto sourceElementNumber = sourceMatrix.GetElementNumber();

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

    if (m_IsLinearIndexListOnly == true)
    {
        if (m_ElementNumber != sourceElementNumber)
        {
            mdkError << "m_ElementNumber != sourceElementNumber @ mdkShadowMatrix::operator=(mdkShadowMatrix)" << '\n';
            return;
        }

        for (uint64 i = 0; i < m_ElementNumber; ++i)
        {
            (*ElementData_source)[m_LinearIndexList_source[i]] = sourceMatrix(i);
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

        for (uint64 j = 0; j < m_ColNumber; ++j)
        {
            for (uint64 i = 0; i < m_RowNumber; ++i)
            {
                uint64 LinearIndex_source = m_ColIndexList_source[j] * m_RowNumber_source + m_RowIndexList_source[i];

                (*ElementData_source)[LinearIndex_source] = sourceMatrix(i, j);
            }
        }
    }
}


template<typename ElementType>
inline
void mdkShadowMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

	if (m_IsLinearIndexListOnly == true)
	{
		for (uint64 i = 0; i < m_ElementNumber; ++i)
	    {
            (*ElementData_source)[m_LinearIndexList_source[i]] = Element;
	    }
    }
	else
	{
        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

        for (uint64 j = 0; j < m_ColNumber; ++j)
		{
            for (uint64 i = 0; i < m_RowNumber; ++i)
			{
				uint64 LinearIndex_source = m_ColIndexList_source[j] * RowNumber_source + m_RowIndexList_source[i];

				(*ElementData_source)[LinearIndex_source] = Element;
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
		return m_NaNElement;
	}

#endif

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

	if (m_IsLinearIndexListOnly == true)
	{
		return (*ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 ColIndex = LinearIndex / m_RowNumber;

		uint64 RowIndex = LinearIndex % m_RowNumber;

        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*ElementData_source)[LinearIndex_source];
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
		return m_NaNElement;
	}

#endif

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

	if (m_IsLinearIndexListOnly == true)
	{
		return (*ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
		uint64 ColIndex = LinearIndex / m_RowNumber;

		uint64 RowIndex = LinearIndex % m_RowNumber;

        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*ElementData_source)[LinearIndex_source];
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
		return m_NaNElement;
	}

#endif

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

	if (m_IsLinearIndexListOnly == true)
	{
		return (*ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*ElementData_source)[LinearIndex_source];
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
		return m_NaNElement;
	}

#endif

    auto ElementData_source = m_SourceMatrixSharedCopy.GetElementDataSharedPointer();

	auto LinearIndex = ColIndex *m_RowNumber + RowIndex;

	if (m_IsLinearIndexListOnly == true)
	{
		return (*ElementData_source)[m_LinearIndexList_source[LinearIndex]];
	}
	else
	{
        auto RowNumber_source = m_SourceMatrixSharedCopy.GetRowNumber();

		uint64 LinearIndex_source = m_ColIndexList_source[ColIndex] * RowNumber_source + m_RowIndexList_source[RowIndex];

		return (*ElementData_source)[LinearIndex_source];
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

	tempMatrix.SetTobeTemporaryBeforeReturn();


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

	tempMatrix.SetTobeTemporaryBeforeReturn();


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

    auto tempMatrixA = ShadowMatrixA.GetSourceMatrixSharedCopy().GetSubMatrix(ShadowMatrixA.GetRowIndexListOfSource(), ShadowMatrixA.GetColIndexListOfSource());

    auto tempMatrixB = ShadowMatrixB.GetSourceMatrixSharedCopy().GetSubMatrix(ShadowMatrixB.GetRowIndexListOfSource(), ShadowMatrixB.GetColIndexListOfSource());

    return MatrixMultiply(tempMatrixA, tempMatrixB);

    /*  too slow

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporaryBeforeReturn();

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
    */
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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

	tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();


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

    tempMatrix.SetTobeTemporaryBeforeReturn();


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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();


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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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

    tempMatrix.SetTobeTemporaryBeforeReturn();

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