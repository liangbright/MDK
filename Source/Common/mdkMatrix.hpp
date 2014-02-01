#ifndef __mdkMatrix_hpp
#define __mdkMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "armadillo.h"

#include "mdkMatrix.h"
#include "mdkDebug.h"

namespace mdk
{

template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix()
{
	this->Clear();
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(mdkMatrix<ElementType>& sourceMatrix)
{
	this->Clear();

	(*this) = sourceMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false)
{
	this->Clear();

	this->SetSize(RowNumber, ColNumber, IsSizeFixed);
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::~mdkMatrix()
{
}


template<typename ElementType>
template<typename ElementType_target>
inline
mdkMatrixElementTypeEnum mdkMatrix<ElementType>::FindElementType(ElementType_target Element)
{
	std::string TypeName(typeid(Element).name());

	if (TypeName == "double")
	{
		return mdkMatrixElementTypeEnum::MDK_DOUBLE64;
	}
	else if (TypeName == "float")
	{
		return mdkMatrixElementTypeEnum::MDK_FLOAT32;
	}
	else if (TypeName == "signed char")
	{
		return mdkMatrixElementTypeEnum::MDK_INT8;
	}
	else if (TypeName == "short")
	{
		return mdkMatrixElementTypeEnum::MDK_INT16;
	}
	else if (TypeName == "int")
	{
		return mdkMatrixElementTypeEnum::MDK_INT32;
	}
	else if (TypeName == "__int64")
	{
		return mdkMatrixElementTypeEnum::MDK_INT64;
	}
	else if (TypeName == "unsigned char")
	{
		return mdkMatrixElementTypeEnum::MDK_UINT8;
	}
	else if (TypeName == "unsigned short")
	{
		return mdkMatrixElementTypeEnum::MDK_UINT16;
	}
	else if (TypeName == "unsigned int")
	{
		return mdkMatrixElementTypeEnum::MDK_UINT32;
	}
	else if (TypeName == "unsigned __int64")
	{
		return mdkMatrixElementTypeEnum::MDK_UINT64;
	}

	return mdkMatrixElementTypeEnum::MDK_UNKNOWN;
}


template<typename ElementType>
inline
mdkMatrixElementTypeEnum mdkMatrix<ElementType>::GetElementType()
{
	return m_ElementType;
}


template<typename ElementType>
template<typename ElementType_target>
inline 
uint64 mdkMatrix<ElementType>::ByteNumberOfElement(ElementType_target Element)
{
	std::string TypeName(typeid(Element).name());

	if (TypeName == "double")
	{
		return 8;
	}
	else if (TypeName == "float")
	{
		return 4;
	}
	else if (TypeName == "signed char")
	{
		return 1;
	}
	else if (TypeName == "short")
	{
		return 2;
	}
	else if (TypeName == "int")
	{
		return 4;
	}
	else if (TypeName == "__int64")
	{
		return 8;
	}
	else if (TypeName == "unsigned char")
	{
		return 1;
	}
	else if (TypeName == "unsigned short")
	{
		return 2;
	}
	else if (TypeName == "unsigned int")
	{
		return 4;
	}
	else if (TypeName == "unsigned __int64")
	{
		return 8;
	}

	mdkError << "Unknown ElementType_target @ mdkMatrix::ByteNumberOfElement" << '\n';
	return 0;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Clear()
{
	m_RowNumber = 0;
	m_ColNumber = 0;

	m_ElementNumber = 0;

	m_ElementData.reset(new std::vector<ElementType>);

	m_IsSizeFixed = false;

	ElementType Element = 0;

	m_ElementType = this->FindElementType(Element);

	m_ColExpansionStep = 100;

	m_RowExpansionStep = 100;

	m_EmptyElement = 0;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false)
{
	if (m_ElementData->size() > 0)
	{
		mdkError << "must call Clear before SetSize if the matrix is not empty @ mdkMatrix::SetSize" << '\n';
		return false;
	}

	m_RowNumber = RowNumber;
	
	m_ColNumber = ColNumber;

	m_ElementNumber = RowNumber*ColNumber;

	m_ElementData->resize(m_ElementNumber);

	m_IsSizeFixed = IsSizeFixed;

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsSizeFixed()
{
	return m_IsSizeFixed;
}


template<typename ElementType>
inline
MatrixSize mdkMatrix<ElementType>::GetSize()
{
	MatrixSize Size;

	Size.RowNumber = m_RowNumber;

	Size.ColNumber = m_ColNumber;

	return Size;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::GetSize(uint64* RowNumber, uint64* ColNumber)
{
	RowNumber[0]=m_RowNumber;

	ColNumber[0]=m_ColNumber;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ReSize(uint64 RowNumber, uint64 ColNumber)
{
	if (RowNumber*ColNumber != m_ElementNumber)
	{
		mdkError << "Matrix size can not be changed @ mdkMatrix::ReSize" << '\n';
		return false;
	}

	m_RowNumber = RowNumber;

	m_ColNumber = ColNumber;

	m_ElementNumber = m_RowNumber*m_ColNumber;

	return true;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::FixSize(bool Fix = true)
{
	m_IsSizeFixed = Fix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsEmpty()
{
	if (m_ElementNumber > 0)
	{
		return false;
	}

	return true;
}


template<typename ElementType>
inline
std::shared_ptr<std::vector<ElementType>> mdkMatrix<ElementType>::GetElementDataSharedPointer()
{
	return m_ElementData;
}


template<typename ElementType>
inline 
ElementType* mdkMatrix<ElementType>::GetElementDataRawPointer()
{
	return m_ElementData->data();
}


template<typename ElementType>
template<typename ElementType_target>
inline
void mdkMatrix<ElementType>::operator=(mdkMatrix<ElementType_target>& targetMatrix)
{
	// MatrixA = MatrixA
	if (this == &targetMatrix) 
	{
		return;
	}

	uint64 RowNumber_target;
	uint64 ColNumber_target;
	targetMatrix.GetSize(&RowNumber_target, &ColNumber_target);

	if (m_IsSizeFixed == true)
	{	
		if (RowNumber_target != m_RowNumber || ColNumber_target != m_ColNumber)
		{
			mdkError << "Matrix size can not be changed @ mdkMatrix::operator=(mdkMatrix)" << '\n';
			return;
		}
	}

	auto mdkElementType_target = targetMatrix.GetElementType();

	if (m_ElementType == mdkElementType_target)
	{
		m_ElementData = targetMatrix.GetElementDataSharedPointer();

		m_RowNumber = RowNumber_target;
		m_ColNumber = ColNumber_target;

		m_ElementNumber = m_RowNumber*m_ColNumber;
	}
	else // copy data
	{
		this->Copy(targetMatrix);
	}

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(ElementType Element)
{
	if (m_FixedSize == true)
	{
		if (1 != m_RowNumber || 1 != m_ColNumber)
		{
			mdkError << "Can not change matrix size @ mdkMatrix::operator=(Element)" << '\n';
			return;
		}
	}

	m_ElementData.reset(new std::vector<ElementType>);

	m_RowNumber = 1;
	m_ColNumber = 1;
	m_ElementNumber = 1;

	m_ElementData->push_back(Element);

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& list)
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "operator=(list) can only be called after SetSzie() @ mdkMatrix::operator=(list)" << '\n';
		return;
	}

	if (m_RowNumber*m_ColNumber != list.size())
	{
		mdkError << "Size does not match @ mdkMatrix::operator=(list)" << '\n';
		return;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Counter = 0;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			RawPointer[Index + i] = list.begin()[Counter];

			Index += m_RowNumber;

			Counter += 1;
		}
	}

	m_ElementNumber = m_RowNumber*m_ColNumber;

	return;
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::CopyOnWrite()
{
	if (m_ElementData.unique() == true || m_ElementNumber == 0)
	{
		return;
	}

	auto tempData = new std::vector<ElementType>;

	tempData->resize(m_ElementNumber);

	auto RawPointer = m_ElementData->data();

	ElementType tempElement = 0;

	memcpy(tempData->data(), RawPointer, this->ByteNumberOfElement(tempElement)*m_ElementNumber);

	m_ElementData.reset(tempData);
}


template<typename ElementType>
template<typename ElementType_target>
inline
bool mdkMatrix<ElementType>::Copy(mdkMatrix<ElementType_target>& targetMatrix)
{
	if (targetMatrix.IsEmpty() == true)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::Copy" << '\n';
		this->Clear();
		return true;
	}

	m_ElementData.reset(new std::vector<ElementType>);

	auto Size = targetMatrix.GetSize();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	return this->Copy(targetRawPointer, Size.RowNumber, Size.ColNumber);
}


template<typename ElementType>
template<typename ElementType_target>
inline 
bool mdkMatrix<ElementType>::Copy(ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber)
{
	if (ElementPointer == nullptr)
	{
		mdkWarning << "ElementPointer is nullptr @ mdkMatrix::Copy" << '\n';
		this->Clear();
		return true;
	}

	ElementType_target tempElement = 0;

	auto tempElementType = this->FindElementType(tempElement);

	if (tempElementType == mdkMatrixElementTypeEnum::MDK_UNKNOWN)
	{
		mdkError << "input type is unknown @ mdkMatrix::Copy(ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber)" << '\n';
		return false;
	}

	m_ElementData.reset(new std::vector<ElementType>);

	m_RowNumber = RowNumber;

	m_ColNumber = ColNumber;

	m_ElementNumber = m_RowNumber*m_ColNumber;

	m_ElementData->resize(m_ElementNumber);

	auto RawPointer = m_ElementData->data();

	if (tempElementType == m_ElementType)
	{
		memcpy(RawPointer, ElementPointer, this->ByteNumberOfElement(tempElement)*m_ElementNumber);
	}
	else
	{
		for (uint64 i = 0; i < m_ElementNumber; ++i)
		{
			RawPointer[i] = ElementType(ElementPointer[i]);
		}
	}

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Fill(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::Fill" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] = Element;
	}

	return true;
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex)
{
	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex)" <<'\n';
		m_EmptyElement = 0;
		return m_EmptyElement;
	}

	this->CopyOnWrite();

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex) const
{
	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex)" << '\n';
		return m_EmptyElement;
	}

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex)
{
	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "LinearIndex >= m_ElementNumber @ mdkMatrix::operator[LinearIndex]" << '\n';
		m_EmptyElement = 0;
		return m_EmptyElement;
	}

	this->CopyOnWrite();

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex) const
{
	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "LinearIndex >= m_ElementNumber @ mdkMatrix::operator[LinearIndex]" << '\n';
		return m_EmptyElement;
	}

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::Element(uint64 LinearIndex)
{
	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Element(i)" << '\n';
		return m_EmptyElement;
	}

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(i,j)" << '\n';
		return m_EmptyElement;
	}

	this->CopyOnWrite();

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(i,j)" << '\n';
		return m_EmptyElement;
	}

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::Element(uint64 RowIndex, uint64 ColIndex)
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Element(i,j)" << '\n';
		return m_EmptyElement;
	}

	return m_ElementData->operator[](LinearIndex);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::SubMatrix(uint64 RowIndex_s, uint64 RowIndex_e, uint64 ColIndex_s, uint64 ColIndex_e)
{
	mdkMatrix<ElementType> tempMatrix; // empty matrix

	if (RowIndex_s >= m_RowNumber || RowIndex_e >= m_RowNumber || ColIndex_s >= m_ColNumber || ColIndex_e >= m_ColNumber
		|| RowIndex_s > RowIndex_e || ColIndex_s > ColIndex_e)
	{
		mdkError << "Invalid input @ mdkMatrix::SubMatrix" << '\n';
		return tempMatrix; 
	}

	auto RowNumber = RowIndex_e - RowIndex_s + 1;
	auto ColNumber = ColIndex_e - ColIndex_s + 1;

	tempMatrix.SetSize(RowNumber, ColNumber);

	auto tempRawPointer = tempMatrix->GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	for (uint64 j = ColIndex_s; j <= ColIndex_e; ++j)
	{
		for (uint64 i = RowIndex_s; i <= RowIndex_e; ++i)
		{
			tempRawPointer[0] = RawPointer[j*m_RowNumber + i];
			
			++tempRawPointer;
		}
	}

	return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::Col(uint64 ColIndex)
{
	mdkMatrix<ElementType> tempColMatrix; // empty matrix

	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Col(uint64 ColIndex)" << '\n';
		return tempColMatrix;
	}

	tempColMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempColMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return tempColMatrix;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData)" << '\n';
		return false;
	}

	ColData.resize(m_RowNumber);

	auto tempRawPointer = ColData.data();

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, ElementType* ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, ElementType* ColData)" << '\n';
		return false;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		ColData[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, mdkMatrix<ElementType_input>& ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	auto Size = ColData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	return this->SetCol(ColIndex, ColData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const ElementType_input* ColData)
{
	if (ColIndex >= m_ColNumber || ColData == nullptr)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, const ElementType_input* ColData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[Index + i] = ElementType(ColData[i]);
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const std::vector<ElementType_input>& ColData)
{
	if (ColIndex >= m_ColNumber || ColData.size() == 0)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, const std::vector<ElementType_input>& ColData)" << '\n';
		return false;
	}

	return this->SetCol(ColIndex, ColData.data());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const mdkMatrix<ElementType_input>& ColData)
{
	if (m_RowNumber == 0)
	{
		mdkError << "Self Size is 0 x ? @ mdkMatrix::AppendCol(mdkMatrix)" << '\n';
		return false;
	}

	auto Size = ColData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendCol(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendCol(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendCol(mdkMatrix)" << '\n';
		return false;
	}

	return this->AppendCol(ColData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool  mdkMatrix<ElementType>::AppendCol(const ElementType_input* ColData)
{
	if (m_RowNumber == 0 || ColData == nullptr)
	{
		mdkError << "Self Size is 0 x ? or Invalid input @ mdkMatrix::AppendCol(const ElementType_input* ColData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	m_ElementData->resize((m_ColNumber + m_ColExpansionStep)*m_RowNumber);

	auto RawPointer = m_ElementData->data();

	uint64 Index = m_ElementNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[Index + i] = ElementType(ColData[i]);
	}

	m_ColNumber += 1;

	m_ElementNumber += m_RowNumber;

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool  mdkMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
	if (m_RowNumber == 0 || ColData.size() != m_RowNumber)
	{
		mdkError << "Self Size is 0 x ? or Invalid input @ mdkMatrix::AppendCol(const std::vector<ElementType_Input>& ColData)" << '\n';
		return false;
	}

	return this->AppendCol(ColData.data());
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::Row(uint64 RowIndex)
{
	mdkMatrix<ElementType> tempRowMatrix;

	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Row(RowIndex)" << '\n';
		return tempRowMatrix;
	}

	tempRowMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempRowMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return tempRowMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, std::vector<ElementType>& RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetRow(RowIndex, std::vector<ElementType>& RowData)" << '\n';
		return false;
	}

	RowData.resize(m_ColNumber);

	auto tempRawPointer = RowData.data();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, ElementType* RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix GetRow(RowIndex, ElementType* RowData)" << '\n';
		return false;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, mdkMatrix<ElementType_input>& RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex, mdkMatrix)" << '\n';
		return false;
	}

	auto Size = RowData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

	return this->SetRow(RowIndex, RowData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const ElementType_input* RowData)
{
	if (RowIndex >= m_RowNumber || RowData == nullptr)
	{
		mdkError << "Invalid input @ mdkMatrix::SetRow(RowIndex, const ElementType_input* RowData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + RowIndex] = ElementType(RowData[j]);

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData)
{
	if (RowIndex >= m_RowNumber || RowData.size() == 0)
	{
		mdkError << "Invalid input @ mdkMatrix::SetRow(RowIndex, const std::vector<ElementType_input>& RowData)" << '\n';
		return false;
	}

	return this->SetRow(RowIndex, RowData.data());	
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::AppendRow(mdkMatrix<ElementType_input>& RowData)
{
	auto Size = RowData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendRow(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendRow(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::AppendRow(mdkMatrix)" << '\n';
		return false;
	}

	this->AppendRow(RowData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::AppendRow(const ElementType_input* RowData)
{
	if (m_ColNumber == 0 || RowData == nullptr) // must use SetSize before calling this function
	{
		mdkError << "Self Size is ? x 0 or Invalid input @ mdkMatrix::AppendRow(const ElementType_input* RowData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto tempData = new std::vector<ElementType>;

	tempData->resize((m_RowNumber + m_RowExpansionStep)*m_ColNumber);

	auto tempRawPointer = tempData->data();

	auto RawPointer = m_ElementData->data();

	uint64 tempIndex = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			tempRawPointer[tempIndex+i] = RawPointer[0];

			++RawPointer;
		}

		tempRawPointer[tempIndex + m_RowNumber] = ElementType(RowData[j]);

		tempIndex += m_RowNumber+1;
	}

	m_RowNumber += 1;

	m_ElementNumber += m_ColNumber;

	m_ElementData.reset(tempData);

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::vector<ElementType_input>& RowData)
{
	if (m_ColNumber == 0 || RowData.size() != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix AppendRow(const std::vector<ElementType_input>& RowData)" << '\n';
		return false;
	}

	return this->AppendRow(RowData.data());
}


template<typename ElementType>
inline bool mdkMatrix<ElementType>::SetDiangonal(ElementType Element)
{
	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber)
	{
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + j] = Element;

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetDiangonal(mdkMatrix<ElementType_input>& DiangonalData)
{
	auto Size = DiangonalData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	return this->SetDiangonal(DiangonalData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetDiangonal(const ElementType_input* DiangonalData)
{
	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber)
	{
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + j] = ElementType(DiangonalData[j]);

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::vector<ElementType_input>& DiangonalData)
{
	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber || DiangonalData.size() != m_RowNumber)
	{
		mdkError << " Self is empty or not square or Input is empty @ mdkMatrix::SetDiangonal" << '\n';
		return false;
	}

	return this->SetDiangonal(DiangonalData.data());
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::Diangonal()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix::Diangonal" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(std::vector<ElementType>& DiangonalData)
{
	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix GetDiangonal" << '\n';
		return  false;
	}

	DiangonalData.resize(m_RowNumber);

	auto outputRawPointer = DiangonalData.data();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		outputRawPointer[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(ElementType* DiangonalData)
{
	if (m_ElementNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix GetDiangonal" << '\n';
		return  false;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		DiangonalData[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
inline	
mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB)
{
	mdkMatrix<ElementType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator+(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator+(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointerA = MatrixA.GetElementDataRawPointer();

	auto RawPointerB = MatrixB.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointerA[i] + RawPointerB[i];
	}

	return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB)
{
	mdkMatrix<ElementType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator-(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator-(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointerA = MatrixA.GetElementDataRawPointer();

	auto RawPointerB = MatrixB.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointerA[i] - RawPointerB[i];
	}

	return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB)
{
	mdkMatrix<ElementType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixB.GetSize();

	if (SizeA.ColNumber != SizeB.RowNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator*(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0 || SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
	{
		mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator*(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointerA = MatrixA.GetElementDataRawPointer();

	auto RawPointerB = MatrixB.GetElementDataRawPointer();

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
				//sum += MatrixA(i,k) * MatrixB(k,j);
				sum += RawPointerA[IndexA + i] * RawPointerB[IndexB + k];

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
mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB)
{
	mdkMatrix<ElementType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

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

	auto RawPointerA = MatrixA.GetElementDataRawPointer();

	auto RawPointerB = MatrixB.GetElementDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointerA[i] / RawPointerB[i];
	}

	return  tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator+(ElementType Element, mdkMatrix<ElementType>& Matrix)
{
	mdkMatrix<ElementType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator+(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = Matrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Element + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(ElementType Element, mdkMatrix<ElementType>& Matrix)
{
	mdkMatrix<ElementType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator-(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = Matrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Element + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(ElementType Element, mdkMatrix<ElementType>& Matrix)
{
	mdkMatrix<ElementType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator*(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = Matrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Element + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(ElementType Element, mdkMatrix<ElementType>& Matrix)
{
	mdkMatrix<ElementType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "Matrix is empty @ mdkMatrix operator/(Element, Matrix)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = Matrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Element / RawPointer[i];
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& Matrix, ElementType Element)
{
	return Element + Matrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& Matrix, ElementType Element)
{
	return Element - Matrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& Matrix, ElementType Element)
{
	return Element * Matrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& Matrix, ElementType Element)
{
	return Element / Matrix;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator+=(mdkMatrix<ElementType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber) 
	{
		mdkError << "Size does not match @ mdkMatrix::operator+=(Matrix)" << '\n';
		return false;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::operator+=(Matrix)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] += targetRawPointer[i];
	}

	return true;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(mdkMatrix<ElementType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator-=(Matrix)" << '\n';
		return false;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::operator-=(Matrix)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] -= targetRawPointer[i];
	}

	return true;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(mdkMatrix<ElementType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator*=(Matrix)" << '\n';
		return false;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::operator*=(Matrix)" << '\n';
		return;
	}

	(*this) = (*this)*targetMatrix;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(mdkMatrix<ElementType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator/=(Matrix)" << '\n';
		return false;
	}

	if (RowNumber == 0 || ColNumber == 0)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::operator/=(Matrix)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] /= targetRawPointer[i];
	}

	return;
}


template<typename ElementType>
inline	
void mdkMatrix<ElementType>::operator+=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator+=(Element)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] += Element;
	}

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator-=(Element)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] -= Element;
	}

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator*=(Element)" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] *= Element;
	}

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(ElementType Element)
{
	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator/=(Element)" << '\n';
		return;
	}

	if (double(abs(Element)) < 0.0000000001)
	{
		mdkWarning << " abs(Element) < 0.0000000001 @ mdkMatrix::operator/=(Element)" << '\n';
	}

	this->CopyOnWrite();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		RawPointer[i] /= Element;
	}

	return;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctorName)
{
	std::string str(FunctorName);

	return this->ElementOperation(str);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::string FunctorName)
{
	mdkMatrix<ElementType> tempMatrix;

	if (FunctorName == "abs")
	{
		return this->ElementOperation([](ElementType a){return std::abs(a); });
	}
	else if (FunctorName == "sin")
	{
		return this->ElementOperation([](ElementType a){return std::sin(a); });
	}
	else if (FunctorName == "cos")
	{
		return this->ElementOperation([](ElementType a){return std::cos(a); });
	}
	else if (FunctorName == "tan")
	{
		return this->ElementOperation([](ElementType a){return std::tan(a); });
	}
	else if (FunctorName == "sqrt")
	{
		return this->ElementOperation([](ElementType a){return std::sqrt(a); });
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string FunctorName)" << '\n';
		return tempMatrix;
	}
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(ElementType)> Functor)
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::ElementOperation(Functor)" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		tempRawPointer[i] = Functor(RawPointer[i]);
	}

	return tempMatrix;
}



template<typename ElementType>
template<typename ElementType_target>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctorName, mdkMatrix<ElementType_target>& targetMatrix)
{
	std::string str(FunctorName);

	return this->ElementOperation(FunctorName, targetMatrix);
}


template<typename ElementType>
template<typename ElementType_target>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::string FunctorName, mdkMatrix<ElementType_target>& targetMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	if (FunctorName == "+")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a + b;}, targetMatrix);
	}
	else if (FunctorName == "-")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a - b; }, targetMatrix);
	}
	else if (FunctorName == "*")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a * b; }, targetMatrix);
	}
	else if (FunctorName == "/")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a / b; }, targetMatrix);
	}
	else if (FunctorName == "^")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a^b; }, targetMatrix);
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string FunctorName, targetMatrix)" << '\n';
		return tempMatrix;
	}
}


template<typename ElementType>
template<typename ElementType_target>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(ElementType, ElementType)> Functor, mdkMatrix<ElementType_target>& targetMatrix)
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix ElementOperation" << '\n';
		return tempMatrix;
	}

	auto targetMatrixSize = targetMatrix.GetSize();

	if	(targetMatrixSize.ColNumber == 0 || targetMatrixSize.RowNumber == 0)
	{
		mdkError << "empty targetMatrix @ mdkMatrix ElementOperation" << '\n';
		return tempMatrix;
	}

	auto Flag_row = 0;

	auto Flag_col = 0;

	auto Flag_full = 0;

	if (targetMatrixSize.ColNumber == m_ColNumber && targetMatrixSize.RowNumber == m_RowNumber)
	{
		Flag_full = 1;
	}
	else if (targetMatrixSize.ColNumber == 1 && targetMatrixSize.RowNumber == m_RowNumber)
	{
		Flag_col = 1;
	}
	else if (targetMatrixSize.ColNumber == m_ColNumber && targetMatrixSize.RowNumber == 1)
	{
		Flag_row = 1;
	}
	else
	{
		mdkError << "Size does not match @ mdkMatrix::ElementOperation(Functor, targetMatrix)" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	if (Flag_full == 1)
	{
		for (uint64 i = 0; i < m_ElementNumber; ++i)
		{
			tempRawPointer[i] = Functor(RawPointer[i], ElementType(targetRawPointer[i]));
		}
	}
	else if (Flag_col == 1)
	{
		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			for (uint64 i = 0; i < m_RowNumber; ++i)
			{
				tempRawPointer[0] = Functor(RawPointer[0], ElementType(targetRawPointer[i]));

				++tempRawPointer;
				++RawPointer;
			}
		}
	}
	else if (Flag_row == 1)
	{
		uint64 tempIndex = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			tempIndex = 0;

			for (uint64 j = 0; j < m_ColNumber; ++j)
			{
				tempRawPointer[tempIndex + i] = Functor(RawPointer[tempIndex + i], ElementType(targetRawPointer[i]));

				tempIndex += m_RowNumber;
			}
		}
	}

	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctorName, ElementType Element)
{
	std::string str(FunctorName);

	return this->ElementOperation(str, Element);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::string FunctorName, ElementType Element)
{
	mdkMatrix<ElementType> tempMatrix;

	if (FunctorName == "+")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a + b; }, Element);
	}
	else if (FunctorName == "-")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a - b; }, Element);
	}
	else if (FunctorName == "*")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a * b; }, Element);
	}
	else if (FunctorName == "/")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return a / b; }, Element);
	}
	else if (FunctorName == "^")
	{
		return this->ElementOperation([](ElementType a, ElementType b){return std::pow(a, b); }, Element);
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string Operator, Element)" << '\n';
		return tempMatrix;
	}
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(ElementType, ElementType)> Functor, ElementType Element)
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::ElementOperation(Functor, Element)" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		tempRawPointer[i] = Functor(RawPointer[i], ElementType(Element));
	}
	
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Mean()
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		value += RawPointer[i];
	}

	value /= m_ColNumber*m_RowNumber;

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanAlongRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value += RawPointer[Index];
			Index += 1;
		}

		value /= m_RowNumber;

		tempRawPointer[j] = value;
	}
	
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanAlongCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		value = 0;

		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += m_RowNumber;
		}

		value /= m_ColNumber;

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Sum()
{
	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	for (uint64 i = 0; i < m_ElementNumber; ++i)
	{
		value += RawPointer[i];
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumAlongRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value += RawPointer[Index];
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumAlongCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		value = 0;

		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Max()
{
	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < m_ElementNumber; ++i)
	{
		value = std::max(value, RawPointer[i]);
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxAlongRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = RawPointer[Index];

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value = std::max(value, RawPointer[Index]);
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxAlongCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		value = RawPointer[Index + i];

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value = std::max(value, RawPointer[Index + i]);

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Min()
{
	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < m_ElementNumber; ++i)
	{
		value = std::min(value, RawPointer[i]);
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinAlongRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = RawPointer[Index];

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value = std::min(value, RawPointer[Index]);
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinAlongCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	ElementType value = 0;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		value = RawPointer[Index+i];

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value = std::min(value, RawPointer[Index + i]);

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetTranspose()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix GetTranspose" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, m_RowNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			tempRawPointer[0] = RawPointer[Index + i];

			Index += m_RowNumber;

			++tempRawPointer;
		}
	}

	return tempMatrix;
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::Rank()
{
	auto RawPointer = m_ElementData->data();

	arma::Mat<ElementType> tempMat(RawPointer, m_RowNumber, m_ColNumber);

	uint64 value = arma::rank(tempMat);

	return value;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::Inv()
{
	// call Armadillo 

	auto RawPointer = m_ElementData->data();

	arma::Mat<ElementType> tempMat(RawPointer, arma::uword(m_RowNumber), arma::uword(m_ColNumber));

	arma::Mat<ElementType> tempInv = arma::inv(tempMat);

	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.Copy(tempInv.memptr(), tempInv.n_rows, tempInv.n_cols);

	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::PseudoInv()
{
	// call Armadillo 


}


template<typename ElementType>
inline
mdkMatrixSVDResult<ElementType> mdkMatrix<ElementType>::SVD()
{
	// call Armadillo 

	auto RawPointer = m_ElementData->data();

	arma::Mat<ElementType> X(RawPointer, arma::uword(m_RowNumber), arma::uword(m_ColNumber));
	
	arma::Mat<ElementType> U;
	arma::Col<ElementType> S;
	arma::Mat<ElementType> V;

	arma::svd(U, S, V, X);

	mdkMatrixSVDResult<ElementType> Result;

	Result.U.Copy(U.memptr(), U.n_rows, U.n_cols);

	Result.S.SetSize(X.n_rows, X.n_cols);

	Result.S.SetDiangonal(S.memptr());

	Result.V.Copy(V.memptr(), V.n_rows, V.n_cols);

	return Result;
}

}//end namespace mdk

#endif