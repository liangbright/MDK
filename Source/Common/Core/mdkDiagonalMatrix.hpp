#include <string>

#include "mdkMatrix.hpp"

namespace mdk
{

template<typename ScalarType>
inline
mdkMatrix<ScalarType>::mdkMatrix()
{
	m_RowNumber = 0;
	m_ColNumber = 0;
	m_ScalarData->reset(new std::vector<ScalarType>);
	m_IsSizeFixed = false;

	m_ScalarType = this->FindScalarType();

	m_ColExpansionStep = 2;

	m_RowExpansionStep = 2;

	m_EmptyScalar = 0;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType>::~mdkMatrix()
{
	m_ScalarData->reset();
}


template<typename ScalarType>
inline
mdkScalarTypeEnum mdkMatrix<ScalarType>::FindScalarType()
{
	ScalarType Scalar;

	std::string TypeName(typeid(Scalar).name);

	if (TypeName == "d")
	{
		return mdkScalarTypeEnum::MDK_DOUBLE64;
	}
	else if (TypeName == "f")
	{
		return mdkScalarTypeEnum::MDK_FLOAT32;
	}
	else if (TypeName == "a")
	{
		return mdkScalarTypeEnum::MDK_INT8;
	}
	else if (TypeName == "s")
	{
		return mdkScalarTypeEnum::MDK_INT16;
	}
	else if (TypeName == "i")
	{
		return mdkScalarTypeEnum::MDK_INT32;
	}
	else if (TypeName == "x")
	{
		return mdkScalarTypeEnum::MDK_INT64;
	}
	else if (TypeName == "h")
	{
		return mdkScalarTypeEnum::MDK_UINT8;
	}
	else if (TypeName == "t")
	{
		return mdkScalarTypeEnum::MDK_UINT16;
	}
	else if (TypeName == "j")
	{
		return mdkScalarTypeEnum::MDK_UINT32;
	}
	else if (TypeName == "y")
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
inline
void mdkMatrix<ScalarType>::Clear()
{
	m_RowNumber = 0;
	m_ColNumber = 0;
	m_ScalarData->reset(new std::vector<ScalarType>);
	m_IsSizeFixed = false;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed == false);
{
	if (m_ScalarData->size() > 0)
	{
		mdkWarning << "must call Clear before SetSize if the array is not empty @ mdkMatrix SetSize" << '\n';
		return false;
	}

	m_RowNumber = RowNumber;
	
	m_ColNumber = ColNumber;

	m_ScalarData->reserve(RowNumber*ColNumber);

	m_IsSizeFixed = IsSizeFixed;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::IsSizeFixed();
{
	return m_IsSizeFixed;
}


template<typename ScalarType>
inline
MatrixSize mdkMatrix<ScalarType>::GetSize();
{
	MatrixSize Size;

	Size.RowNumber = m_RowNumber;

	Size.ColNumber = m_ColNumber;

	return Size;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::GetSize(uint64* RowNumber, uint64* ColNumber);
{
	RowNumber[0]=m_RowNumber;

	ColNumber[0]=m_ColNumber;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::ReSize(uint64 RowNumber, uint64 ColNumber);
{
	if (RowNumber*ColNumber != m_RowNumber*m_ColNumber)
	{
		return false;
	}

	m_RowNumber = RowNumber;

	m_ColNumber = ColNumber;

	return true;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::FixSize(bool Fix = true);
{
	m_IsSizeFixed = Fix;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::IsEmpty()
{
	if (m_RowNumber > 0 && m_ColNumber > 0)
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
inline
mdkScalarTypeEnum mdkMatrix<ScalarType>::GetScalarType()
{
	return m_ScalarType;
}


template<typename ScalarType, typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator=(const mdkMatrix<ScalarType_target>& targetMatrix)
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
			mdkWarning << "Matrix size is changed @ mdkMatrix  operator=";
		}
	}

	mdkScalarType_target = targetMatrix.GetScalarType();

	if (m_ScalarType == mdkScalarType_target)
	{
		m_ScalarData = targetMatrix.GetScalarDataSharedPointer();

		m_RowNumber = RowNumber_target;
		m_ColNumber = ColNumber_target;

		m_IsSizeFixed = targetMatrix.IsSizeFixed();
	}
	else // copy data
	{
		m_ScalarData->reset(new std::vector<ScalarType>);

		m_RowNumber = RowNumber_target;
		m_ColNumber = ColNumber_target;

		m_ScalarData->reserve(m_RowNumber*m_ColNumber);

		targetRawPointer = targetMatrix.GetScalarDataRawPointer();

		for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
		{
			m_ScalarData->push_back(ScalarType(targetRawPointer[i]));
		}

		m_IsSizeFixed = targetMatrix.IsSizeFixed();
	}

	return;
}


template<typename ScalarType, typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator=(ScalarType_target Scalar)
{
	if (m_FixedSize == true)
	{
		if (1 != m_RowNumber || 1 != m_ColNumber)
		{
			mdkError << "Can not change matrix size @ mdkMatrix  operator=";
			return;
		}
	}

	m_ScalarData->reset(new std::vector<ScalarType>);

	m_RowNumber = 1;
	m_ColNumber = 1;

	m_ScalarData->reserve(1);

	m_ScalarData->push_back(ScalarType(Scalar));

	m_IsSizeFixed = false;

	return;
}


template<typename ScalarType>
inline
void mdkMatrix<ScalarType>::operator=(const std::initializer_list<ScalarType>& list)
{
	m_ScalarData->reset(new std::vector<ScalarType>);

	m_RowNumber = 1;
	m_ColNumber = list.size();

	m_ScalarData->reserve(m_ColNumber);
	
	m_ScalarData->insert(m_ScalarData->begin(), list.begin(), list.end());

	m_IsSizeFixed = false;

	return;
}


template<typename ScalarType>
inline
ScalarType& mdkMatrix<ScalarType>::operator()(uint64 LinearIndex)
{
	auto RawPointer = m_ScalarData->data();

	if (LinearIndex >= m_RowNumber*m_ColNumber)
	{
		mdkError << "LinearIndex >= m_RowNumber*m_ColNumber @ mdkMatrix operator(i)" <<endl;
		return m_EmptyScalar;
	}

	return RawPointer[LinearIndex];
}


template<typename ScalarType>
inline
const ScalarType& mdkMatrix<ScalarType>::operator()(uint64 LinearIndex) const
{
	auto RawPointer = m_ScalarData->data();

	if (LinearIndex >= m_RowNumber*m_ColNumber)
	{
		mdkError << "LinearIndex >= m_RowNumber*m_ColNumber @ mdkMatrix operator(i)" << endl;
		return m_EmptyScalar;
	}

	return RawPointer[LinearIndex];
}


template<typename ScalarType>
inline
ScalarType& mdkMatrix<ScalarType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
	auto RawPointer = m_ScalarData->data();

	LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_RowNumber*m_ColNumber)
	{
		mdkError << "LinearIndex >= m_RowNumber*m_ColNumber @ mdkMatrix operator(i,j)" << endl;
		return m_EmptyScalar;
	}

	return RawPointer[LinearIndex];
}


template<typename ScalarType>
inline
const ScalarType& mdkMatrix<ScalarType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
	auto RawPointer = m_ScalarData->data();

	LinearIndex = ColIndex*m_RowNumber + RowIndex;

	if (LinearIndex >= m_RowNumber*m_ColNumber)
	{
		mdkError << "LinearIndex >= m_RowNumber*m_ColNumber @ mdkMatrix operator(i,j)" << endl;
		return m_EmptyScalar;
	}

	return RawPointer[LinearIndex];
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator()(uint64 RowIndex_s, uint64 RowIndex_e, uint64 ColIndex_s, uint64 ColIndex_e)
{
	mdkMatrix<ScalarType> tempMatrix; // empty matrix

	if (RowIndex_s >= m_RowNumber || RowIndex_e >= m_RowNumber || ColIndex_s >= m_ColNumber || ColIndex_e >= m_ColNumber
		|| RowIndex_s > RowIndex_e || ColIndex_s > ColIndex_e)
	{
		return tempMatrix; 
	}

	RowNumber = RowIndex_e - RowIndex_s + 1;
	ColNumber = ColIndex_e - ColIndex_s + 1;

	tempMatrix.SetSize(RowNumber, ColNumber);

	auto tempRawPointer = tempMatrix->GetScalarDataRawPointer();

	uint64 tempLinearIndex = 0;

	auto RawPointer = m_ScalarData->data();

	for (uint64 j = ColIndex_s; j <= ColIndex_e; ++j)
	{
		for (uint64 i = RowIndex_s; i <= RowIndex_e; ++i)
		{
			tempRawPointer[tempLinearIndex] = RawPointer[j*m_RowNumber + i];
			tempLinearIndex += 1;
		}
	}

	return tempMatrix;
}


template<typename ScalarType>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::GetCol(uint64 ColIndex)
{
	mdkMatrix<ScalarType> tempMatrix; // empty matrix

	if (ColIndex >= m_ColNumber)
	{
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempMatrix->GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[tempIndex + i];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline 
std::vector<ScalarType> mdkMatrix<ScalarType>::GetColData(uint64 ColIndex)
{
	std::vector<ScalarType> tempCol;

	if (ColIndex >= m_ColNumber)
	{
		return tempCol;
	}

	tempCol.reserve(m_RowNumber);

	auto tempRawPointer = tempCol->data();

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[tempIndex + i];
	}

	return tempCol;
}


template<typename ScalarType>
inline 
bool mdkMatrix<ScalarType>::GetColData(uint64 ColIndex, ScalarType* ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		ColData[i] = RawPointer[tempIndex + i];
	}

	return true;
}


template<typename ScalarType>
inline 
bool mdkMatrix<ScalarType>::SetCol(uint64 ColIndex, const ScalarType* ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[tempIndex + i] = ColData[i];
	}

	return true;
}


template<typename ScalarType>
inline 
bool mdkMatrix<ScalarType>::SetCol(uint64 ColIndex, const std::vector<ScalarType>& ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		return false;
	}

	auto tempRawPointer = ColData->data();

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[tempIndex + i] = tempRawPointer[i];
	}

	return true;
}


template<typename ScalarType>
inline 
bool  mdkMatrix<ScalarType>::AppendCol(const ScalarType* ColData)
{
	if (m_RowNumber == 0)
	{
		return false;
	}

	m_ScalarData->reserve((m_ColNumber + m_ColExpansionStep)*m_RowNumber);

	auto RawPointer = m_ScalarData->data();

	m_ColNumber += 1;

	uint64 tempIndex = (m_ColNumber-1)*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[tempIndex + i] = ColData[i];
	}

	return true;
}


template<typename ScalarType>
inline 
bool  mdkMatrix<ScalarType>::AppendCol(const std::vector<ScalarType>& ColData)
{
	if (m_RowNumber == 0)
	{
		return false;
	}

	m_ScalarData->reserve((m_ColNumber + m_ColExpansionStep)*m_RowNumber);

	auto RawPointer = m_ScalarData->data();

	m_ColNumber += 1;

	uint64 tempIndex = (m_ColNumber - 1)*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[tempIndex + i] = ColData[i];
	}

	return true;
}


template<typename ScalarType>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::GetRow(uint64 RowIndex)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (RowIndex >= m_RowNumber)
	{
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix->GetScalarDataRawPointer();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i*m_RowNumber + RowIndex];
	}

	return tempMatrix;
}


template<typename ScalarType>
inline
std::vector<ScalarType> mdkMatrix<ScalarType>::GetRowData(uint64 RowIndex)
{
	std::vector<ScalarType> tempRow;

	if (RowIndex >= m_RowNumber)
	{
		return tempRow;
	}

	tempCol.reserve(m_ColNumber);

	auto tempRawPointer = tempRow->data();

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i*m_RowNumber +RowIndex];
	}

	return tempCol;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::GetRowData(uint64 RowIndex, ScalarType* RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_ColNumber; ++i)
	{
		RowData[i] = RawPointer[i*m_RowNumber + i];
	}

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::SetRow(uint64 RowIndex, const ScalarType* RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		return false;
	}

	auto RawPointer = m_ScalarData->data();

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[i*m_RowNumber + i] = RowData[i];
	}

	return true;
}


template<typename ScalarType>
inline
bool mdkMatrix<ScalarType>::SetRow(uint64 ColIndex, const std::vector<ScalarType>& RowData)
{
	if (RowIndex >= m_RowNumber)
	{
		return false;
	}

	auto tempRawPointer = RowData->data();

	auto RawPointer = m_ScalarData->data();

	uint64 tempIndex = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		RawPointer[i*m_RowNumber + i] = tempRawPointer[i];
	}

	return true;
}


template<typename ScalarType>
inline
bool  mdkMatrix<ScalarType>::AppendRow(const ScalarType* RowData)
{
	if (m_ColNumber == 0) // must use SetSize before this function
	{
		return false;
	}

	auto tempData = new std::vector<ScalarType>;

	tempData->reserve((m_RowNumber + m_RowExpansionStep)*m_ColNumber);

	auto tempRawPointer = tempData->data();

	auto RawPointer = m_ScalarData->data();

	uint64 LinearIndex = 0;

	uint64 tempLinearIndex = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			LinearIndex = j*m_RowNumber + i;

			tempLinearIndex = j*(m_RowNumber+1) + i;

			tempRawPointer[tempLinearIndex] = RawPointer[LinearIndex];
		}

		tempLinearIndex += 1;

		tempRawPointer[tempLinearIndex] = RowData[j];
	}

	m_ColNumber += 1;

	m_ScalarData->reset(tempData);

	return true;
}


template<typename ScalarType>
inline
bool  mdkMatrix<ScalarType>::AppendRow(const std::vector<ScalarType>& RowData)
{
	if (m_ColNumber == 0) // must use SetSize before this function
	{
		return false;
	}

	auto tempData = new std::vector<ScalarType>;

	tempData->reserve((m_RowNumber + m_RowExpansionStep)*m_ColNumber);

	auto tempRawPointer = tempData->data();

	auto RawPointer = m_ScalarData->data();

	uint64 LinearIndex = 0;

	uint64 tempLinearIndex = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			LinearIndex = j*m_RowNumber + i;

			tempLinearIndex = j*(m_RowNumber + 1) + i;

			tempRawPointer[tempLinearIndex] = RawPointer[LinearIndex];
		}

		tempLinearIndex += 1;

		tempRawPointer[tempLinearIndex] = RowData[j];
	}

	m_ColNumber += 1;

	m_ScalarData->reset(tempData);

	return true;
}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator+(mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator+" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] + ScalarType(RawPointer_target[i]);
	}

}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator-(mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator-" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] - ScalarType(RawPointer_target[i]);
	}
}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator*(mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	uint64 RowNumber_target;
	uint64 ColNumber_target;

	targetMatrix.GetSize(&RowNumber_target, &ColNumber_target);

	if (RowNumber_target == 0 || ColNumber_target == 0)
	{
		mdkError << "mdkMatrix operator* : target is empty" << '\n';
		return tempMatrix;
	}

	if (RowNumber_target != m_ColNumber)
	{
		mdkError << "mdkMatrix operator* : size does not match" << '\n';
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, ColNumber_target);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	//RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	//RawPointer = m_ScalarData->data();

	uint64 tempIndex = 0;

	ScalarType sum = 0;

	for (uint64 j = 0; j < ColNumber_target; ++i)
	{
		tempIndex = j*m_RowNumber;
		
		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			sum = 0;

			for (uint64 k = 0; k < m_ColNumber; ++k)
			{
				sum += (*this)(i,k) * ScalarType(targetMatrix(k,j));
			}

			tempRawPointer[tempIndex + i] = sum;
		}
	}

	return tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator/(mdkMatrix<ScalarType_target>& targetMatrix)
{
	mdkMatrix<ScalarType> tempMatrix;

	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator/" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] / ScalarType(RawPointer_target[i]);
	}
}


template<typename ScalarType, typename ScalarType_target>
inline	
void mdkMatrix<ScalarType>::operator+=(mdkMatrix<ScalarType_target>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator+=" << '\n';
		return false;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);
	
	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] += ScalarType(RawPointer_target[i]);
	}

	return true;
}


template<typename ScalarType, typename ScalarType_target>
inline	
void mdkMatrix<ScalarType>::operator-=(mdkMatrix<ScalarType_target>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator-=" << '\n';
		return false;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] -= ScalarType(RawPointer_target[i]);
	}

	return true;
}


template<typename ScalarType, typename ScalarType_target>
inline	
void mdkMatrix<ScalarType>::operator*=(mdkMatrix<ScalarType_target>& targetMatrix)
{

	auto tempMatrix = (*this)*targetMatrix;

	(*this) = tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline	
void mdkMatrix<ScalarType>::operator/=(mdkMatrix<ScalarType_target>& targetMatrix)
{
	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (RowNumber != m_RowNumber || RowNumber == 0 || ColNumber != m_ColNumber || ColNumber == 0)
	{
		mdkError << "mdkMatrix operator/=" << '\n';
		return;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer_target = targetMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] /= ScalarType(RawPointer_target[i]);
	}

	return;
}


template<typename ScalarType, typename ScalarType_target>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator+(ScalarType_target Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator+ Scalar" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();	

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] + ScalarType(Scalar);
	}

	return tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator-(ScalarType_target Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator- Scalar" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] - ScalarType(Scalar);
	}

	return tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator*(ScalarType_target Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator* Scalar" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] * ScalarType(Scalar);
	}

	return tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline	
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::operator/(ScalarType_target Scalar)
{
	mdkMatrix<ScalarType> tempMatrix;

	uint64 RowNumber;
	uint64 ColNumber;

	targetMatrix.GetSize(&RowNumber, &ColNumber);

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator/ Scalar" << '\n';
		return  tempMatrix;
	}

	if (double(abs(Scalar)) < 0.0000000001)
	{
		mdkWarning << " abs(Scalar) < 0.0000000001 @ mdkMatrix operator/ Scalar" << '\n';
	}

	tempMatrix.SetSize(RowNumber, ColNumber);

	tempRawPointer = tempMatrix.GetScalarDataRawPointer();

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[i] / ScalarType(Scalar);
	}

	return tempMatrix;
}


template<typename ScalarType, typename ScalarType_target>
inline	
void mdkMatrix<ScalarType>::operator+=(ScalarType_target Scalar)
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator+= Scalar" << '\n';
		return;
	}

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] += ScalarType(Scalar);
	}

	return;
}


template<typename ScalarType, typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator-=(ScalarType_target Scalar)
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator-= Scalar" << '\n';
		return;
	}

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] -= ScalarType(Scalar);
	}

	return;
}


template<typename ScalarType, typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator*=(ScalarType_target Scalar)
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator*= Scalar" << '\n';
		return;
	}

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] *= ScalarType(Scalar);
	}

	return;
}


template<typename ScalarType, typename ScalarType_target>
inline
void mdkMatrix<ScalarType>::operator/=(ScalarType_target Scalar)
{
	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "mdkMatrix operator*= Scalar" << '\n';
		return;
	}

	if (double(abs(Scalar)) < 0.0000000001)
	{
		mdkWarning << " abs(Scalar) < 0.0000000001 @ mdkMatrix operator/= Scalar" << '\n';
	}

	RawPointer = m_ScalarData->data();

	for (uint i = 0; i < m_RowNumber*m_ColNumber; ++i)
	{
		RawPointer[i] /= ScalarType(Scalar);
	}

	return;
}


template<typename ScalarType_target>
inline 
mdkMatrix<ScalarType> mdkMatrix<ScalarType>::ElementOperation(std::string Operator, mdkMatrix<ScalarType_target>& targetMatrix)
{
	if (Operator == "+")
	{

	}
	else if (Operator == "-")
	{

	}
	else if (Operator == "*")
	{

	}
	else if (Operator == "/")
	{

	}
}

}//end namespace mdk