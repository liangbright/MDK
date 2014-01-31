#include <string>
#include <cmath>
#include <algorithm>

#include "armadillo_bits/armadillo_header.h"

#include "mdkMatrix.h"
#include "mdkDebug.h"

namespace mdk
{

template<typename ScalarType>
inline
mdkMatrix<ScalarType>::mdkMatrix()
{
	this->Clear();
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType>::mdkMatrix(mdkMatrix<ScalarType>& sourceMatrix)
{
	this->Clear();

	(*this) = sourceMatrix;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType>::mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false)
{
	this->Clear();

	this->SetSize(RowNumber, ColNumber, IsSizeFixed);
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType>::~mdkMatrix()
{
}


template<typename ScalarType>
template<typename ScalarType_target>
inline
mdkScalarTypeEnum mdkMatrix<ScalarType>::FindScalarType(ScalarType_target Scalar)
{
	std::string TypeName(typeid(Scalar).name());

	if (TypeName == "double")
	{
		return mdkScalarTypeEnum::MDK_DOUBLE64;
	}
	else if (TypeName == "float")
	{
		return mdkScalarTypeEnum::MDK_FLOAT32;
	}
	else if (TypeName == "signed char")
	{
		return mdkScalarTypeEnum::MDK_INT8;
	}
	else if (TypeName == "short")
	{
		return mdkScalarTypeEnum::MDK_INT16;
	}
	else if (TypeName == "int")
	{
		return mdkScalarTypeEnum::MDK_INT32;
	}
	else if (TypeName == "__int64")
	{
		return mdkScalarTypeEnum::MDK_INT64;
	}
	else if (TypeName == "unsigned char")
	{
		return mdkScalarTypeEnum::MDK_UINT8;
	}
	else if (TypeName == "unsigned short")
	{
		return mdkScalarTypeEnum::MDK_UINT16;
	}
	else if (TypeName == "unsigned int")
	{
		return mdkScalarTypeEnum::MDK_UINT32;
	}
	else if (TypeName == "unsigned __int64")
	{
		return mdkScalarTypeEnum::MDK_UINT64;
	}

	return mdkScalarTypeEnum::MDK_UNKNOWN;
}


template<typename ScalarType>
inline
mdkScalarTypeEnum mdkMatrix<ScalarType>::GetScalarType()
{
	return m_ScalarType;
}


template<typename ScalarType>
template<typename ScalarType_target>
inline 
uint64 mdkMatrix<ScalarType>::ByteNumberOfScalar(ScalarType_target Scalar)
{
	std::string TypeName(typeid(Scalar).name());

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

	mdkError << "Unknown ScalarType_target @ mdkMatrix::ByteNumberOfScalar" << '\n';
	return 0;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::Clear()
{
	m_RowNumber = 0;
	m_ColNumber = 0;

	m_ScalarNumber = 0;

	m_ScalarData.reset(new std::vector<ScalarType>);

	m_IsSizeFixed = false;

	ScalarType Scalar = 0;

	m_ScalarType = this->FindScalarType(Scalar);

	m_ColExpansionStep = 100;

	m_RowExpansionStep = 100;

	m_EmptyScalar = 0;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false)
{
	if (m_ScalarData->size() > 0)
	{
		mdkError << "must call Clear before SetSize if the matrix is not empty @ mdkMatrix::SetSize" << '\n';
		return false;
	}

	m_RowNumber = RowNumber;
	
	m_ColNumber = ColNumber;

	m_ScalarNumber = RowNumber*ColNumber;

	m_ScalarData->resize(m_ScalarNumber);

	m_IsSizeFixed = IsSizeFixed;

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::IsSizeFixed()
{
	return m_IsSizeFixed;
}


template<typename ScalarType>
inline
MatrixSize mdkMatrix<ScalarType>::GetSize()
{
	MatrixSize Size;

	Size.RowNumber = m_RowNumber;

	Size.ColNumber = m_ColNumber;

	return Size;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::GetSize(uint64* RowNumber, uint64* ColNumber)
{
	RowNumber[0]=m_RowNumber;

	ColNumber[0]=m_ColNumber;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::ReSize(uint64 RowNumber, uint64 ColNumber)
{
	if (RowNumber*ColNumber != m_ScalarNumber)
	{
		mdkError << "Matrix size can not be changed @ mdkMatrix::ReSize" << '\n';
		return false;
	}

	m_RowNumber = RowNumber;

	m_ColNumber = ColNumber;

	m_ScalarNumber = m_RowNumber*m_ColNumber;

	return true;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::FixSize(bool Fix = true)
{
	m_IsSizeFixed = Fix;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::IsEmpty()
{
	if (m_ScalarNumber > 0)
	{
		return false;
	}

	return true;
}


template<typename ScalarType>
inline
std::shared_ptr<std::vector<ScalarType>> mdkMatrix<ScalarType>::GetScalarDataSharedPointer()
{
	return m_ScalarData;
}


template<typename ScalarType>
inline 
ScalarType* mdkMatrix<ScalarType>::GetScalarDataRawPointer()
{
	return m_ScalarData->data();
}


template<typename ScalarType>
template<typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator=(mdkMatrix<ScalarType_target>& targetMatrix)
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

	auto mdkScalarType_target = targetMatrix.GetScalarType();

	if (m_ScalarType == mdkScalarType_target)
	{
		m_ScalarData = targetMatrix.GetScalarDataSharedPointer();

		m_RowNumber = RowNumber_target;
		m_ColNumber = ColNumber_target;

		m_ScalarNumber = m_RowNumber*m_ColNumber;
	}
	else // copy data
	{
		this->Copy(targetMatrix);
	}

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator=(ScalarType Scalar)
{
	if (m_FixedSize == true)
	{
		if (1 != m_RowNumber || 1 != m_ColNumber)
		{
			mdkError << "Can not change matrix size @ mdkMatrix::operator=(Scalar)" << '\n';
			return;
		}
	}

	m_ScalarData.reset(new std::vector<ScalarType>);

	m_RowNumber = 1;
	m_ColNumber = 1;
	m_ScalarNumber = 1;

	m_ScalarData->push_back(Scalar);

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator=(const std::initializer_list<ScalarType>& list)
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

	auto RawPointer = m_ScalarData->data();

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

	m_ScalarNumber = m_RowNumber*m_ColNumber;

	return;
}


template<typename ScalarType>
inline 
void mdkMatrix<ScalarType>::CopyOnWrite()
{
	if (m_ScalarData.unique() == true || m_ScalarNumber == 0)
	{
		return;
	}

	auto tempData = new std::vector<ScalarType>;

	tempData->resize(m_ScalarNumber);

	auto RawPointer = m_ScalarData->data();

	ScalarType tempScalar = 0;

	memcpy(tempData->data(), RawPointer, this->ByteNumberOfScalar(tempScalar)*m_ScalarNumber);

	m_ScalarData.reset(tempData);
}


template<typename ScalarType>
template<typename ScalarType_target>
inline
bool mdkMatrix<ScalarType>::Copy(mdkMatrix<ScalarType_target>& targetMatrix)
{
	if (targetMatrix.IsEmpty() == true)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::Copy" << '\n';
		this->Clear();
		return true;
	}

	m_ScalarData.reset(new std::vector<ScalarType>);

	auto Size = targetMatrix.GetSize();

	auto targetRawPointer = targetMatrix.GetScalarDataRawPointer();

	return this->Copy(targetRawPointer, Size.RowNumber, Size.ColNumber);
}


template<typename ScalarType>
template<typename ScalarType_target>
inline 
bool mdkMatrix<ScalarType>::Copy(ScalarType_target* ScalarPointer, uint64 RowNumber, uint64 ColNumber)
{
	if (ScalarPointer == nullptr)
	{
		mdkWarning << "ScalarPointer is nullptr @ mdkMatrix::Copy" << '\n';
		this->Clear();
		return true;
	}

	ScalarType_target tempScalar = 0;

	auto tempScalarType = this->FindScalarType(tempScalar);

	if (tempScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		mdkError << "input type is unknown @ mdkMatrix::Copy(ScalarType_target* ScalarPointer, uint64 RowNumber, uint64 ColNumber)" << '\n';
		return false;
	}

	m_ScalarData.reset(new std::vector<ScalarType>);

	m_RowNumber = RowNumber;

	m_ColNumber = ColNumber;

	m_ScalarNumber = m_RowNumber*m_ColNumber;

	m_ScalarData->resize(m_ScalarNumber);

	auto RawPointer = m_ScalarData->data();

	if (tempScalarType == m_ScalarType)
	{
		memcpy(RawPointer, ScalarPointer, this->ByteNumberOfScalar(tempScalar)*m_ScalarNumber);
	}
	else
	{
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			RawPointer[i] = ScalarType(ScalarPointer[i]);
		}
	}

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::Fill(ScalarType Scalar)
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::Fill" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] = Scalar;
	}

	return true;
}


template<typename ScalarType>
inline
ScalarType& mdkMatrix<ScalarType>::operator()(uint64 LinearIndex)
{
	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex)" <<'\n';
		m_EmptyScalar = 0;
		return m_EmptyScalar;
	}

	this->CopyOnWrite();

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
const ScalarType& mdkMatrix<ScalarType>::operator()(uint64 LinearIndex) const
{
	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex)" << endl;
		return m_EmptyScalar;
	}

	return m_ScalarData->operator[](LinearIndex);
}

/*
template<typename ScalarType>
inline
ScalarType& mdkMatrix<ScalarType>::operator[](uint64 LinearIndex)
{
	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "LinearIndex >= m_ScalarNumber @ mdkMatrix operator[i]" << '\n';
		m_EmptyScalar = 0;
		return m_EmptyScalar;
	}

	this->CopyOnWrite();

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
const ScalarType& mdkMatrix<ScalarType>::operator[](uint64 LinearIndex) const
{
	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "LinearIndex >= m_ScalarNumber @ mdkMatrix operator[i]" << endl;
		return m_EmptyScalar;
	}

	return m_ScalarData->operator[](LinearIndex);
}
*/

template<typename ScalarType>
inline
ScalarType mdkMatrix<ScalarType>::Element(uint64 LinearIndex)
{
	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Element(i)" << '\n';
		return m_EmptyScalar;
	}

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
ScalarType& mdkMatrix<ScalarType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(i,j)" << '\n';
		return m_EmptyScalar;
	}

	this->CopyOnWrite();

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
const ScalarType& mdkMatrix<ScalarType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(i,j)" << '\n';
		return m_EmptyScalar;
	}

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
ScalarType mdkMatrix<ScalarType>::Element(uint64 RowIndex, uint64 ColIndex)
{
	auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_ScalarNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Element(i,j)" << '\n';
		return m_EmptyScalar;
	}

	return m_ScalarData->operator[](LinearIndex);
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::SubMatrix(uint64 RowIndex_s, uint64 RowIndex_e, uint64 ColIndex_s, uint64 ColIndex_e)
{
	mdkMatrix<ScalarType> tempMatrix; // empty matrix

	if (RowIndex_s >= m_RowNumber || RowIndex_e >= m_RowNumber || ColIndex_s >= m_ColNumber || ColIndex_e >= m_ColNumber
		|| RowIndex_s > RowIndex_e || ColIndex_s > ColIndex_e)
	{
		mdkError << "Invalid input @ mdkMatrix::SubMatrix" << '\n';
		return tempMatrix; 
	}

	auto RowNumber = RowIndex_e - RowIndex_s + 1;
	auto ColNumber = ColIndex_e - ColIndex_s + 1;

	tempMatrix.SetSize(RowNumber, ColNumber);

	auto tempRawPointer = tempMatrix->GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

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


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::Col(uint64 ColIndex)
{
	mdkMatrix<ScalarType> tempColMatrix; // empty matrix

	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Col(uint64 ColIndex)" << '\n';
		return tempColMatrix;
	}

	tempColMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempColMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return tempColMatrix;
}


template<typename ScalarType>
inline 
bool mdkMatrix<ScalarType>::GetCol(uint64 ColIndex, std::vector<ScalarType>& ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, std::vector<ScalarType>& ColData)" << '\n';
		return false;
	}

	ColData.resize(m_RowNumber);

	auto tempRawPointer = ColData.data();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ScalarType>
inline 
bool mdkMatrix<ScalarType>::GetCol(uint64 ColIndex, ScalarType* ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, ScalarType* ColData)" << '\n';
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		ColData[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::SetCol(uint64 ColIndex, mdkMatrix<ScalarType_input>& ColData)
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

	return this->SetCol(ColIndex, ColData.GetScalarDataRawPointer());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::SetCol(uint64 ColIndex, const ScalarType_input* ColData)
{
	if (ColIndex >= m_ColNumber || ColData == nullptr)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, const ScalarType_input* ColData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[Index + i] = ScalarType(ColData[i]);
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool mdkMatrix<ScalarType>::SetCol(uint64 ColIndex, const std::vector<ScalarType_input>& ColData)
{
	if (ColIndex >= m_ColNumber || ColData.size() == 0)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, const std::vector<ScalarType_input>& ColData)" << '\n';
		return false;
	}

	return this->SetCol(ColIndex, ColData.data());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::AppendCol(const mdkMatrix<ScalarType_input>& ColData)
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

	return this->AppendCol(ColData.GetScalarDataRawPointer());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool  mdkMatrix<ScalarType>::AppendCol(const ScalarType_input* ColData)
{
	if (m_RowNumber == 0 || ColData == nullptr)
	{
		mdkError << "Self Size is 0 x ? or Invalid input @ mdkMatrix::AppendCol(const ScalarType_input* ColData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	m_ScalarData->resize((m_ColNumber + m_ColExpansionStep)*m_RowNumber);

	auto RawPointer = m_ScalarData->data();

	uint64 Index = m_ScalarNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[Index + i] = ScalarType(ColData[i]);
	}

	m_ColNumber += 1;

	m_ScalarNumber += m_RowNumber;

	return true;
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
bool  mdkMatrix<ScalarType>::AppendCol(const std::vector<ScalarType_Input>& ColData)
{
	if (m_RowNumber == 0 || ColData.size() != m_RowNumber)
	{
		mdkError << "Self Size is 0 x ? or Invalid input @ mdkMatrix::AppendCol(const std::vector<ScalarType_Input>& ColData)" << '\n';
		return false;
	}

	return this->AppendCol(ColData.data());
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::Row(uint64 RowIndex)
{
	mdkMatrix<ScalarType> tempRowMatrix;

	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::Row(RowIndex)" << '\n';
		return tempRowMatrix;
	}

	tempRowMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempRowMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return tempRowMatrix;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::GetRow(uint64 RowIndex, std::vector<ScalarType>& RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetRow(RowIndex, std::vector<ScalarType>& RowData)" << '\n';
		return false;
	}

	RowData.resize(m_ColNumber);

	auto tempRawPointer = RowData.data();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::GetRow(uint64 RowIndex, ScalarType* RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix GetRow(RowIndex, ScalarType* RowData)" << '\n';
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::SetRow(uint64 RowIndex, mdkMatrix<ScalarType_input>& RowData)
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

	return this->SetRow(RowIndex, RowData.GetScalarDataRawPointer());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool mdkMatrix<ScalarType>::SetRow(uint64 RowIndex, const ScalarType_input* RowData)
{
	if (RowIndex >= m_RowNumber || RowData == nullptr)
	{
		mdkError << "Invalid input @ mdkMatrix::SetRow(RowIndex, const ScalarType_input* RowData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + RowIndex] = ScalarType(RowData[j]);

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool mdkMatrix<ScalarType>::SetRow(uint64 RowIndex, const std::vector<ScalarType_input>& RowData)
{
	if (RowIndex >= m_RowNumber || RowData.size() == 0)
	{
		mdkError << "Invalid input @ mdkMatrix::SetRow(RowIndex, const std::vector<ScalarType_input>& RowData)" << '\n';
		return false;
	}

	return this->SetRow(RowIndex, RowData.data());	
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::AppendRow(mdkMatrix<ScalarType_input>& RowData)
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

	this->AppendRow(RowData.GetScalarDataRawPointer());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool mdkMatrix<ScalarType>::AppendRow(const ScalarType_input* RowData)
{
	if (m_ColNumber == 0 || RowData == nullptr) // must use SetSize before calling this function
	{
		mdkError << "Self Size is ? x 0 or Invalid input @ mdkMatrix::AppendRow(const ScalarType_input* RowData)" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto tempData = new std::vector<ScalarType>;

	tempData->resize((m_RowNumber + m_RowExpansionStep)*m_ColNumber);

	auto tempRawPointer = tempData->data();

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			tempRawPointer[tempIndex+i] = RawPointer[0];

			++RawPointer;
		}

		tempRawPointer[tempIndex + m_RowNumber] = ScalarType(RowData[j]);

		tempIndex += m_RowNumber+1;
	}

	m_RowNumber += 1;

	m_ScalarNumber += m_ColNumber;

	m_ScalarData.reset(tempData);

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool  mdkMatrix<ScalarType>::AppendRow(const std::vector<ScalarType_input>& RowData)
{
	if (m_ColNumber == 0 || RowData.size() != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix AppendRow(const std::vector<ScalarType_input>& RowData)" << '\n';
		return false;
	}

	return this->AppendRow(RowData.data());
}


template<typename ScalarType>
inline bool mdkMatrix<ScalarType>::SetDiangonal(ScalarType Scalar)
{
	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber)
	{
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + j] = Scalar;

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::SetDiangonal(mdkMatrix<ScalarType_input>& DiangonalData)
{
	auto Size = DiangonalData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix SetDiangonal(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetDiangonal(mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetDiangonal(mdkMatrix)" << '\n';
		return false;
	}

	return this->SetDiangonal(DiangonalData.GetScalarDataRawPointer());
}


template<typename ScalarType>
template<typename ScalarType_input>
inline 
bool mdkMatrix<ScalarType>::SetDiangonal(const ScalarType_input* DiangonalData)
{
	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber)
	{
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RawPointer[Index + j] = ScalarType(DiangonalData[j]);

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
template<typename ScalarType_input>
inline
bool mdkMatrix<ScalarType>::SetDiangonal(const std::vector<ScalarType_input>& DiangonalData)
{
	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber || DiangonalData.size() != m_RowNumber)
	{
		return false;
	}

	return this->SetDiangonal(DiangonalData.data());
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::Diangonal()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix GetDiangonal" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return tempMatrix;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::GetDiangonal(std::vector<ScalarType>& DiangonalData)
{
	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix GetDiangonal" << '\n';
		return  false;
	}

	DiangonalData.resize(m_RowNumber);

	auto tempRawPointer = DiangonalData.data();

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::GetDiangonal(ScalarType* DiangonalData)
{
	if (m_ScalarNumber == 0 || m_RowNumber != m_ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix GetDiangonal" << '\n';
		return  false;
	}

	auto RawPointer = m_ScalarData->data();

	uint64 Index = 0;

	uint64 Counter = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		DiangonalData[j] = RawPointer[Index + j];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ScalarType>
inline	
mdkMatrix<ScalarType> operator+(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator : MatrixA + MatrixB" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator : MatrixA + MatrixB" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPtr = tempMatrix.GetScalarDataRawPointer();

	auto RawPtrA = MatrixA.GetScalarDataRawPointer();

	auto RawPtrB = MatrixB.GetScalarDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPtr[i] = RawPtrA[i] + RawPtrB[i];
	}

	return  tempMatrix;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> operator-(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator : MatrixA - MatrixB" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator : MatrixA - MatrixB" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPtr = tempMatrix.GetScalarDataRawPointer();

	auto RawPtrA = MatrixA.GetScalarDataRawPointer();

	auto RawPtrB = MatrixB.GetScalarDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPtr[i] = RawPtrA[i] - RawPtrB[i];
	}

	return  tempMatrix;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> operator*(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixB.GetSize();

	if (SizeA.ColNumber != SizeB.RowNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator : MatrixA * MatrixB" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0 || SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
	{
		mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator : MatrixA * MatrixB" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

	auto tempRawPtr = tempMatrix.GetScalarDataRawPointer();

	auto RawPtrA = MatrixA.GetScalarDataRawPointer();

	auto RawPtrB = MatrixB.GetScalarDataRawPointer();

	uint64 tempIndex = 0;

	uint64 IndexA = 0;

	uint64 IndexB = 0;

	ScalarType sum = 0;

	for (uint64 j = 0; j < SizeB.ColNumber; ++j)
	{
		for (uint64 i = 0; i < SizeA.RowNumber; ++i)
		{
			sum = 0;

			IndexA = 0;

			for (uint64 k = 0; k < SizeA.ColNumber; ++k)
			{
				//sum += MatrixA(i,k) * MatrixB(k,j);
				sum += RawPtrA[IndexA + i] * RawPtrB[IndexB + k];

				IndexA += SizeA.RowNumber;
			}

			tempRawPtr[tempIndex + i] = sum;
		}

		tempIndex += SizeA.RowNumber;

		IndexB += SizeB.RowNumber;
	}

	return tempMatrix;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> operator/(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixA.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator : MatrixA / MatrixB" << '\n';
		return  tempMatrix;
	}

	if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
	{
		mdkWarning << "MatrixA and MatrixB are empty @ mdkMatrix operator : MatrixA / MatrixB" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto tempRawPtr = tempMatrix.GetScalarDataRawPointer();

	auto RawPtrA = MatrixA.GetScalarDataRawPointer();

	auto RawPtrB = MatrixB.GetScalarDataRawPointer();

	for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
	{
		tempRawPtr[i] = RawPtrA[i] / RawPtrB[i];
	}

	return  tempMatrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator+(ScalarType Scalar, mdkMatrix<ScalarType>& Matrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "mdkMatrix operator: Scalar + Matrix" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = Matrix.GetScalarDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Scalar + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator-(ScalarType Scalar, mdkMatrix<ScalarType>& Matrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "mdkMatrix operator: Scalar - Matrix" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = Matrix.GetScalarDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Scalar + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator*(ScalarType Scalar, mdkMatrix<ScalarType>& Matrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "mdkMatrix operator: Scalar * Matrix" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = Matrix.GetScalarDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Scalar + RawPointer[i];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator/(ScalarType Scalar, mdkMatrix<ScalarType>& Matrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto Size = Matrix.GetSize();

	if (Size.RowNumber == 0 || Size.ColNumber == 0)
	{
		mdkError << "mdkMatrix operator: Scalar / Matrix" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = Matrix.GetScalarDataRawPointer();

	for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
	{
		tempRawPointer[i] = Scalar / RawPointer[i];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator+(mdkMatrix<ScalarType>& Matrix, ScalarType Scalar)
{
	return Scalar + Matrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator-(mdkMatrix<ScalarType>& Matrix, ScalarType Scalar)
{
	return Scalar - Matrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator*(mdkMatrix<ScalarType>& Matrix, ScalarType Scalar)
{
	return Scalar * Matrix;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> operator/(mdkMatrix<ScalarType>& Matrix, ScalarType Scalar)
{
	return Scalar / Matrix;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator+=(mdkMatrix<ScalarType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator+=" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] += RawPointer_target[i];
	}

	return true;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator-=(mdkMatrix<ScalarType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator-=" << '\n';
		return false;
	}

	this->CopyOnWrite();

	auto RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] -= RawPointer_target[i];
	}

	return true;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator*=(mdkMatrix<ScalarType>& targetMatrix)
{
	this->CopyOnWrite();

	auto tempMatrix = (*this)*targetMatrix;

	(*this) = tempMatrix;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator/=(mdkMatrix<ScalarType>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator/=" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] /= RawPointer_target[i];
	}

	return;
}


template<typename ScalarType>
inline	
void mdkMatrix<ScalarType>::operator+=(ScalarType Scalar)
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "mdkMatrix operator+= Scalar" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] += Scalar;
	}

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator-=(ScalarType Scalar)
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "mdkMatrix operator-= Scalar" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] -= Scalar;
	}

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator*=(ScalarType Scalar)
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "mdkMatrix operator*= Scalar" << '\n';
		return;
	}

	this->CopyOnWrite();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] *= Scalar;
	}

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator/=(ScalarType Scalar)
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "mdkMatrix operator*= Scalar" << '\n';
		return;
	}

	this->CopyOnWrite();

	if (double(abs(Scalar)) < 0.0000000001)
	{
		mdkWarning << " abs(Scalar) < 0.0000000001 @ mdkMatrix operator/= Scalar" << '\n';
	}

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		RawPointer[i] /= Scalar;
	}

	return;
}


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::string FunctorName)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (FunctorName == "abs")
	{
		return this->ElementOperation([](ScalarType a){return abs(a); });
	}
	else if (FunctorName == "sin")
	{
		return this->ElementOperation([](ScalarType a){return sin(a); });
	}
	else if (FunctorName == "cos")
	{
		return this->ElementOperation([](ScalarType a){return cos(a); });
	}
	else if (FunctorName == "tan")
	{
		return this->ElementOperation([](ScalarType a){return tan(a); });
	}
	else if (FunctorName == "sqrt")
	{
		return this->ElementOperation([](ScalarType a){return sqrt(a); });
	}
	else
	{
		mdkError << " unknown operator @ ElementOperation(std::string FunctorName, ...)" << '\n';
		return tempMatrix;
	}
}


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::function<ScalarType(ScalarType)> Functor)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix ElementOperation" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		tempRawPointer[i] = Functor(RawPointer[i]);
	}

	return tempMatrix;
}


template<typename ScalarType>
template<typename ScalarType_target>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::string FunctorName, mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (FunctorName == "+")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a + b;}, targetMatrix);
	}
	else if (FunctorName == "-")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a - b; }, targetMatrix);
	}
	else if (FunctorName == "*")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a * b; }, targetMatrix);
	}
	else if (FunctorName == "/")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a / b; }, targetMatrix);
	}
	else if (FunctorName == "^")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a^b; }, targetMatrix);
	}
	else
	{
		mdkError << " unknown operator @ ElementOperation(std::string FunctorName, ...)" << '\n';
		return tempMatrix;
	}
}


template<typename ScalarType>
template<typename ScalarType_target>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::function<ScalarType(ScalarType, ScalarType)> Functor, mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
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
		mdkError << "Size does not match @ mdkMatrix ElementOperation" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	auto RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	if (Flag_full == 1)
	{
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			tempRawPointer[i] = Functor(RawPointer[i], ScalarType(RawPointer_target[i]));
		}
	}
	else if (Flag_col == 1)
	{
		uint64 tempIndex = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			for (uint64 i = 0; i < m_RowNumber; ++i)
			{
				tempRawPointer[tempIndex + i] = Functor(RawPointer[tempIndex + i], ScalarType(RawPointer_target[i]));
			}

			tempIndex += m_RowNumber;
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
				tempRawPointer[tempIndex + i] = Functor(RawPointer[tempIndex + i], ScalarType(RawPointer_target[i]));

				tempIndex += m_RowNumber;
			}
		}
	}

	return tempMatrix;
}



template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::string FunctorName, ScalarType Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (FunctorName == "+")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a + b; }, Scalar);
	}
	else if (FunctorName == "-")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a - b; }, Scalar);
	}
	else if (FunctorName == "*")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a * b; }, Scalar);
	}
	else if (FunctorName == "/")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return a / b; }, Scalar);
	}
	else if (FunctorName == "^")
	{
		return this->ElementOperation([](ScalarType a, ScalarType b){return std::pow(a, b); }, Scalar);
	}
	else
	{
		mdkError << " unknown operator @ ElementOperation(std::string Operator, ...)" << '\n';
		return tempMatrix;
	}
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::function<ScalarType(ScalarType, ScalarType)> Functor, ScalarType Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix ElementOperation" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		tempRawPointer[i] = Functor(RawPointer[i], ScalarType(Scalar));
	}
	
	return tempMatrix;
}


template<typename ScalarType>
inline ScalarType mdkMatrix<ScalarType>::Mean()
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		value += RawPointer[i];
	}

	value /= m_ColNumber*m_RowNumber;

	return value;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MeanAlongRow()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MeanAlongCol()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline ScalarType mdkMatrix<ScalarType>::Sum()
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

	for (uint64 i = 0; i < m_ScalarNumber; ++i)
	{
		value += RawPointer[i];
	}

	return value;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::SumAlongRow()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::SumAlongCol()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline ScalarType mdkMatrix<ScalarType>::Max()
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ScalarData->data();

	ScalarType value = RawPointer[0];

	for (uint64 i = 1; i < m_ScalarNumber; ++i)
	{
		value = std::max(value, RawPointer[i]);
	}

	return value;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MaxAlongRow()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MaxAlongCol()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline ScalarType mdkMatrix<ScalarType>::Min()
{
	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return 0;
	}

	auto RawPointer = m_ScalarData->data();

	ScalarType value = RawPointer[0];

	for (uint64 i = 1; i < m_ScalarNumber; ++i)
	{
		value = std::min(value, RawPointer[i]);
	}

	return value;
}


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MinAlongRow()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline mdkMatrix<ScalarType> mdkMatrix<ScalarType>::MinAlongCol()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix Mean" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	ScalarType value = 0;

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


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::GetTranspose()
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_ScalarNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix GetTranspose" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, m_RowNumber);

	auto tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

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


template<typename ScalarType>
inline
uint64 mdkMatrix<ScalarType>::Rank()
{
	auto RawPointer = m_ScalarData->data();

	arma::Mat<ScalarType> tempMat(RawPointer, m_RowNumber, m_ColNumber);

	uint64 value = arma::rank(tempMat);

	return value;
}


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::Inv()
{
	// call Armadillo 

	auto RawPointer = m_ScalarData->data();

	arma::Mat<ScalarType> tempMat(RawPointer, arma::uword(m_RowNumber), arma::uword(m_ColNumber));

	arma::Mat<ScalarType> tempInv = arma::inv(tempMat);

	mdkMatrix<ScalarType> tempMatrix;

	tempMatrix.Copy(tempInv.memptr(), tempInv.n_rows, tempInv.n_cols);

	return tempMatrix;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::PseudoInv()
{
	// call Armadillo 


}


template<typename ScalarType>
inline
mdkMatrixSVDResult<ScalarType> mdkMatrix<ScalarType>::SVD()
{
	// call Armadillo 

	auto RawPointer = m_ScalarData->data();

	arma::Mat<ScalarType> X(RawPointer, arma::uword(m_RowNumber), arma::uword(m_ColNumber));
	
	arma::Mat<ScalarType> U;
	arma::Col<ScalarType> S;
	arma::Mat<ScalarType> V;

	arma::svd(U, S, V, X);

	mdkMatrixSVDResult<ScalarType> Result;

	Result.U.Copy(U.memptr(), U.n_rows, U.n_cols);

	Result.S.SetSize(X.n_rows, X.n_cols);

	Result.S.SetDiangonal(S.memptr());

	Result.V.Copy(V.memptr(), V.n_rows, V.n_cols);

	return Result;
}

}//end namespace mdk