#ifndef __mdkDataArray_hpp
#define __mdkDataArray_hpp

#include <typeinfo>

#include <cstring>

#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkDataArray.h"

namespace mdk
{

template<typename ScalarType>
mdkDataArray<ScalarType>::mdkDataArray()
{	
	m_ScalarPointer = nullptr;

	m_ElementLength = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_ScalarTypeData = 0;

	m_ExpantionNumber = 1000;
}


template<typename ScalarType>
mdkDataArray<ScalarType>::~mdkDataArray()
{
	this->Clear();
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::SetElementLength(uint64 Length)
{
	if (m_ScalarPointer != nullptr)
	{
		return false;
	}

	m_ElementLength = Length;

	return true;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::SetElementNumber(uint64 Number)
{
	if (m_ScalarPointer != nullptr)
	{
		return false;
	}

	m_ElementNumber = Number;

	return true;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::Allocate()
{
	if (m_ScalarPointer == nullptr && m_ElementLength > 0 && m_ElementNumber > 0)
	{
		auto ScalarNumber = m_ElementLength*m_ElementNumber;

		try
		{
			m_ScalarPointer = static_cast<void*>(new ScalarType[ScalarNumber]);
		}
		catch (...)
		{
			mdkWarning << "Not enough memory @ mdkDataArray<ScalarType>::Allocate()";
			m_ScalarPointer = nullptr;
			return false;
		}

		std::memset(m_ScalarPointer, 0, ScalarNumber*sizeof(ScalarType));

		m_EffectiveElementNumber = m_ElementNumber;

		return true;
	}

	return false;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
	                                    uint64 ElementLength, uint64 ElementNumber)
{
	this->Clear();

	m_ElementLength = ElementLength;
	m_ElementNumber= ElementNumber;

	auto IsAllocated = this->Allocate();
	if (IsAllocated == false)
	{
		return false;
	}

	auto ScalarNumber = ElementLength*ElementNumber;

	auto DataType = this->GetScalarType();

	if (DataType == SourceDataType)
	{
		std::memcpy(m_ScalarPointer, SourceData, ScalarNumber*sizeof(ScalarType));
	}
	else
	{
		auto Data = static_cast<ScalarType*>(m_ScalarPointer);

		switch (SourceDataType)
		{
		case mdkScalarTypeEnum::MDK_DOUBLE64:

			auto SourcePointer = static_cast<double*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_FLOAT32:

			auto SourcePointer = static_cast<float*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT8:

			auto SourcePointer = static_cast<int8*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT16:

			auto SourcePointer = static_cast<int16*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT32:

			auto SourcePointer = static_cast<int32*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT64:
			
			auto SourcePointer = static_cast<int64*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT8:
			
			auto SourcePointer = static_cast<uint8*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT16:

			auto SourcePointer = static_cast<uint16*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT32:

			auto SourcePointer = static_cast<uint32*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT64:

			auto SourcePointer = static_cast<uint64*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				Data[i] = ScalarType(SourceData[i]);
			}
			break;
		}
	}

	m_EffectiveElementNumber = m_ElementNumber;

	return true;
}



template<typename ScalarType>
bool mdkDataArray<ScalarType>::SetElement(uint64 ElementIndex, const double* Element)
{
	if (m_ScalarPointer == nullptr || ElementIndex >= m_ElementNumber)
	{
		return false;
	}

	auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);

	Pointer += ElementIndex*m_ElementLength;

	for (uint64 i = 0; i < m_ElementLength; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}



template<typename ScalarType>
bool mdkDataArray<ScalarType>::InsertElement(uint64 ElementIndex, const double* Element)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (ElementIndex >= m_ElementNumber)
	{
		auto IsOK = this->Expand();
		if (IsOK == false)
		{
			return false;
		}
	}

	if (m_EffectiveElementNumber < ElementIndex + 1)
	{
		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);

	Pointer += ElementIndex*m_ElementLength;

	for (uint64 i = 0; i < m_ElementLength; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::Expand()
{
	auto NewScalarNumber = (m_ElementNumber + m_ExpantionNumber)*m_ElementLength;

	void* NewAddress = nullptr;

	try
	{
		NewAddress = static_cast<void*>(new ScalarType[NewScalarNumber]);
	}
	catch (...)
	{
		mdkWarning << "Not enough memory @ mdkDataArray::Expand()";
		return false;
	}

	std::memset(NewAddress, 0, NewScalarNumber*sizeof(ScalarType));

	if (m_ScalarPointer != nullptr)
	{
		std::memcpy(NewAddress, m_ScalarPointer, m_ElementNumber*m_ElementLength*sizeof(ScalarType));

		auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);
		delete[] Pointer;
	}

	m_ScalarPointer = NewAddress;

	m_ElementNumber += m_ExpantionNumber;

	return true;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::Cut(uint64 StartElementIndex, uint64 EndElementIndex)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto ScalarNumber = m_ElementNumber*m_ElementLength;

	auto NewScalarNumber = (EndElementIndex - StartElementIndex + 1)*m_ElementLength;

	if (NewScalarNumber >= ScalarNumber)
	{
		return false;
	}

	void* NewAddress = nullptr;

	try
	{
		NewAddress = static_cast<void*>(new ScalarType[NewScalarNumber]);
	}
	catch (std::bad_alloc&)
	{
		mdkWarning << "Not enough memory @ mdkDataArray::Cut()";
		return false;
	}
	
	std::memset(NewAddress, 0, NewScalarNumber*sizeof(ScalarType));

	if (m_ScalarPointer != nullptr)
	{
		auto OldAddress = static_cast<ScalarType*>(m_ScalarPointer);

		auto tempPointer = OldAddress + StartElementIndex*m_ElementLength;

		std::memcpy(NewAddress, static_cast<void*>(tempPointer), NewScalarNumber*sizeof(ScalarType));

		delete[] OldAddress;
	}

	m_ScalarPointer = NewAddress;

	m_ElementNumber = EndElementIndex - StartElementIndex + 1;

	if (m_EffectiveElementNumber < StartElementIndex + 1)
	{
		m_EffectiveElementNumber = 0;
	}
	else if (m_EffectiveElementNumber > EndElementIndex + 1)
	{
		m_EffectiveElementNumber = m_ElementNumber;
	}
	else
	{
		m_EffectiveElementNumber = m_EffectiveElementNumber - StartElementIndex;
	}

	return true;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::Squeeze()
{
	if (m_ScalarPointer == nullptr || m_EffectiveElementNumber == m_ElementNumber)
	{
		return false;
	}

	this->Cut(0, m_EffectiveElementNumber-1);
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::GetElement(uint64 ElementIndex, double* Element)
{
	if (m_ScalarPointer == nullptr || ElementIndex >= m_ElementNumber)
	{
		return false;
	}

	auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);

	Pointer += ElementIndex*m_ElementLength;

	for (uint64 i = 0; i < m_ElementLength; ++i)
	{
		Element[i] = Pointer[i];
	}

	return true;
}


template<typename ScalarType>
uint64 mdkDataArray<ScalarType>::GetElementLength()
{
	return m_ElementLength;
}


template<typename ScalarType>
uint64 mdkDataArray<ScalarType>::GetElementNumber()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ElementNumber;
}


template<typename ScalarType>
uint64 mdkDataArray<ScalarType>::GetEffectiveElementNumber()
{
	return m_EffectiveElementNumber;
}

template<typename ScalarType>
mdkScalarTypeEnum mdkDataArray<ScalarType>::GetScalarType()
{
	std::string TypeName(typeid(m_ScalarTypeData).name);

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
void* mdkDataArray<ScalarType>::GetScalarPointer()
{
	return m_ScalarPointer;
}


template<typename ScalarType>
void mdkDataArray<ScalarType>::Clear()
{
	if (m_ScalarPointer != nullptr)
	{
		auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);
		delete[] Pointer;
	}

	m_ScalarPointer = nullptr;

	m_ElementLength = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;
}


template<typename ScalarType>
bool mdkDataArray<ScalarType>::IsEmpty()
{
	if (m_ScalarPointer == nullptr)
	{
		return true;
	}

	return false;
}

}//end namespace mdk

#endif