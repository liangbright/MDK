#ifndef __mdkDataArrayTemplate_hpp
#define __mdkDataArrayTemplate_hpp

#include <typeinfo>

#include <cstring>

#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkDataArrayTemplate.h"

namespace mdk
{

template<typename ScalarType>
mdkDataArrayTemplate<ScalarType>::mdkDataArrayTemplate()
{	
	m_ScalarPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementSize = nullptr;

	m_ArrayDimension = 0;

	m_ArraySize = nullptr;

	m_ScalarTypeData = 0;
}


template<typename ScalarType>
mdkDataArrayTemplate<ScalarType>::~mdkDataArrayTemplate()
{
	this->Clear();
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetElementDimensionAndSize(uint64 Dim, const uint64* Size)
{
	m_ElementDimension = Dim;

	if (m_ElementSize != nullptr)
	{
		delete[] m_ElementSize;
	}

	m_ElementSize = new uint64[Dim];

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		m_ElementSize[i] = Size[i];
	}

	return false;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetArrayDimensionAndSize(uint64 Dim, const uint64* Size)
{
	m_ArrayDimension = Dim;

	if (m_ArraySize != nullptr)
	{
		delete[] m_ArraySize;
	}
	
	m_ArraySize = new uint64[Dim];

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		m_ArraySize[i] = Size[i];
	}
	
	return true;
	
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetElementDimension()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ElementDimension;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::GetElementSize(uint64* Size)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		Size[i]=m_ElementSize[i];
	}

	return true;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetArrayDimension()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ArrayDimension;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::GetArraySize(uint64* Size)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		Size[i] = m_ArraySize[i];
	}
	
	return true;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetElementNumber()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	uint64 ElementNumber = 1;

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		ElementNumber *= m_ArraySize[i];
	}

	return ElementNumber;
}


template<typename ScalarType>
mdkScalarTypeEnum mdkDataArrayTemplate<ScalarType>::GetScalarType()
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
uint64 mdkDataArrayTemplate<ScalarType>::GetElementScalarNumber()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	uint64 ScalarNumber = 1;

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		ScalarNumber *= m_ElementSize[i];
	}

	return ScalarNumber;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetScalarNumber()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	uint64 ScalarNumber = 1;

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		ScalarNumber *= m_ArraySize[i];
	}

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		ScalarNumber *= m_ElementSize[i];
	}

	return ScalarNumber;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::CalScalarNumber(uint64 ElementDimension, const uint64* ElementSize, 
	                                                     uint64 ArrayDimension, const uint64* ArraySize)
{

	uint64 ScalarNumber = 1;

	for (uint64 i = 0; i < ArrayDimension; ++i)
	{
		ScalarNumber *= ArraySize[i];
	}

	for (uint64 i = 0; i < ElementDimension; ++i)
	{
		ScalarNumber *= ElementSize[i];
	}

	return ScalarNumber;
}


template<typename ScalarType>
void* mdkDataArrayTemplate<ScalarType>::GetScalarPointer()
{
	return m_ScalarPointer;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::Allocate(uint64 ElementDimension, const uint64* ElementSize, 
	                                            uint64 ArrayDimension, const uint64* ArraySize)
{
	if (m_ScalarPointer == nullptr && ElementDimension > 0 && ArrayDimension > 0)
	{
		auto ScalarNumber = this->CalScalarNumber(ElementDimension, ElementSize, ArrayDimension, ArraySize);

		try
		{
			m_ScalarPointer = static_cast<void*>(new ScalarType[ScalarNumber]);
		}
		catch (...)
		{
			mdkWarning << "Not enough memory @ mdkDataArrayTemplate<ScalarType>::Allocate()";
			m_ScalarPointer = nullptr;
			return false;
		}

		std::memset(m_ScalarPointer, 0, ScalarNumber*sizeof(ScalarType));

		m_ElementDimension = ElementDimension;


		return true;
	}

	return false;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::Allocate(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize)
{
	ScalarNumber= CalScalarNumber

	return this->Allocate();
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType, 
	                                            uint64 ElementDimension, const uint64* ElementSize,
	                                            uint64 ArrayDimension, const uint64* ArraySize)
{
	this->Clear();

	this->SetElementDimension(ElementDimension);
	this->SetElementSize(ElementSize);

	this->SetArrayDimension(ArrayDimension);
	this->SetArraySize(ArraySize);

	auto IsAllocated = this->Allocate();
	if (IsAllocated == false)
	{
		return false;
	}

	auto ScalarNumber = this->GetScalarNumber();

	auto DataType = mdkDataArrayTemplate<ScalarType>::GetScalarType();

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

	return true;
}


template<typename ScalarType>
void mdkDataArrayTemplate<ScalarType>::Clear()
{
	if (m_ScalarPointer != nullptr)
	{
		auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);
		delete[] Pointer;
	}

	if (m_ElementSize != nullptr)
	{
		delete[] m_ElementSize;
	}

	if (m_ArraySize != nullptr)
	{
		delete[] m_ArraySize;
	}

	m_ScalarPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementSize = nullptr;

	m_ArrayDimension = 0;

	m_ArraySize = nullptr;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::IsEmpty()
{
	if (m_ScalarPointer == nullptr)
	{
		return true;
	}

	return false;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::CalElementLinearIndex(const uint64* ElementPosition)
{
	uint64 Index = ElementPosition[0];

	if (m_ArrayDimension > 1)
	{
		for (uint64 i = 1; i < m_ArrayDimension; ++i)
		{
			Index += ElementPosition[i] * m_ArraySize[i - 1];
		}
	}

	return Index;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetElementByPosition(const uint64* ElementPosition, const double* Element)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto ElementLinearIndex = this->CalElementLinearIndex(ElementPosition);

	return this->SetElementByLinearIndex(ElementLinearIndex, Element);
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetElementByLinearIndex(uint64 ElementLinearIndex, const double* Element)
{
	auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);

	auto ElementScalarNumber = this->GetElementScalarNumber();

	Pointer += ElementLinearIndex*ElementScalarNumber;

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::GetElementByPosition(const uint64* ElementPosition, double* Element)
{
	auto ElementLinearIndex = this->CalElementLinearIndex(ElementPosition);

	return this->GetElementByLinearIndex(ElementLinearIndex, Element);
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::GetElementByLinearIndex(uint64 ElementLinearIndex, double* Element)
{
	auto Pointer = static_cast<ScalarType*>(m_ScalarPointer);

	auto ElementScalarNumber = this->GetElementScalarNumber();

	Pointer += ElementLinearIndex*ElementScalarNumber;

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		Element[i] = double(Pointer[i]);
	}

	return true;
}

}//end namespace mdk

#endif