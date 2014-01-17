#ifndef __mdkDataArrayTemplate_hpp
#define __mdkDataArrayTemplate_hpp

#include <typeinfo>

#include <cstring>

#include "mdkDebug.h"
#include "mdkDataArrayTemplate.h"

namespace mdk
{

template<typename ScalarType>
mdkDataArrayTemplate<ScalarType>::mdkDataArrayTemplate()
{	
	m_ScalarPointer = nullptr;

	m_ScalarNumberInElement = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_ScalarTypeData = 0;

	m_ExpantionNumber = 1000;
}


template<typename ScalarType>
mdkDataArrayTemplate<ScalarType>::~mdkDataArrayTemplate()
{
	this->Clear();
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetScalarNumberInElement(uint64 Number)
{
	if (m_ScalarPointer != nullptr)
	{
		return false;
	}

	m_ScalarNumberInElement = Number;

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetElementNumber(uint64 Number)
{
	if (m_ScalarPointer != nullptr)
	{
		return false;
	}

	m_ElementNumber = Number;

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::Allocate()
{
	if (m_ScalarPointer != nullptr || m_ScalarNumberInElement == 0 || m_ElementNumber == 0)
	{
		return false;
	}

	auto ScalarNumber = m_ScalarNumberInElement*m_ElementNumber;

	try
	{
		m_ScalarPointer = new ScalarType[ScalarNumber];
	}
	catch (...)
	{
		mdkWarning << "Not enough memory @ mdkDataArrayTemplate<ScalarType>::Allocate()";
		m_ScalarPointer = nullptr;
		return false;
	}

	std::memset(static_cast<void*>(m_ScalarPointer), 0, ScalarNumber*sizeof(ScalarType));

	m_EffectiveElementNumber = m_ElementNumber;

	return true;	
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
	                                            uint64 ScalarNumberInElement, uint64 ElementNumber)
{
	this->Clear();

	m_ScalarNumberInElement = ScalarNumberInElement;
	m_ElementNumber= ElementNumber;

	auto IsAllocated = this->Allocate();
	if (IsAllocated == false)
	{
		return false;
	}

	auto ScalarNumber = ScalarNumberInElement*ElementNumber;

	auto DataType = this->GetScalarType();

	if (DataType == SourceDataType)
	{
		std::memcpy(static_cast<void*>(m_ScalarPointer), SourceData, ScalarNumber*sizeof(ScalarType));
	}
	else
	{
		switch (SourceDataType)
		{
		case mdkScalarTypeEnum::MDK_DOUBLE64:

			auto SourcePointer = static_cast<double*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_FLOAT32:

			auto SourcePointer = static_cast<float*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT8:

			auto SourcePointer = static_cast<int8*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT16:

			auto SourcePointer = static_cast<int16*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT32:

			auto SourcePointer = static_cast<int32*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_INT64:
			
			auto SourcePointer = static_cast<int64*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT8:
			
			auto SourcePointer = static_cast<uint8*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT16:

			auto SourcePointer = static_cast<uint16*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT32:

			auto SourcePointer = static_cast<uint32*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT64:

			auto SourcePointer = static_cast<uint64*>(SourceData);
			for (uint64 i = 0; i < ScalarNumber; ++i)
			{
				m_ScalarPointer[i] = ScalarType(SourceData[i]);
			}
			break;
		}
	}

	m_EffectiveElementNumber = m_ElementNumber;

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::SetElement(uint64 ElementIndex, const double* Element)
{
	if (m_ScalarPointer == nullptr || ElementIndex >= m_ElementNumber)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	Pointer += ElementIndex*m_ScalarNumberInElement;

	for (uint64 i = 0; i < m_ScalarNumberInElement; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}



template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::InsertElement(uint64 ElementIndex, const double* Element)
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

	auto Pointer = m_ScalarPointer;

	Pointer += ElementIndex*m_ScalarNumberInElement;

	for (uint64 i = 0; i < m_ScalarNumberInElement; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::Expand()
{
	auto NewScalarNumber = (m_ElementNumber + m_ExpantionNumber)*m_ScalarNumberInElement;

	ScalarType* NewAddress = nullptr;

	try
	{
		NewAddress = new ScalarType[NewScalarNumber];
	}
	catch (...)
	{
		mdkWarning << "Not enough memory @ mdkDataArrayTemplate::Expand()";
		return false;
	}

	std::memset(static_cast<void*>(NewAddress), 0, NewScalarNumber*sizeof(ScalarType));

	if (m_ScalarPointer != nullptr)
	{
		std::memcpy(static_cast<void*>(NewAddress), static_cast<void*>(m_ScalarPointer), m_ElementNumber*m_ScalarNumberInElement*sizeof(ScalarType));

		delete[] m_ScalarPointer;
	}

	m_ScalarPointer = NewAddress;

	m_ElementNumber += m_ExpantionNumber;

	return true;
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::Cut(uint64 StartElementIndex, uint64 EndElementIndex)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto ScalarNumber = m_ElementNumber*m_ScalarNumberInElement;

	auto NewScalarNumber = (EndElementIndex - StartElementIndex + 1)*m_ScalarNumberInElement;

	if (NewScalarNumber >= ScalarNumber)
	{
		return false;
	}

	ScalarType* NewAddress = nullptr;

	try
	{
		NewAddress = new ScalarType[NewScalarNumber];
	}
	catch (std::bad_alloc&)
	{
		mdkWarning << "Not enough memory @ mdkDataArrayTemplate::Cut()";
		return false;
	}
	
	std::memset(static_cast<void*>(NewAddress), 0, NewScalarNumber*sizeof(ScalarType));

	if (m_ScalarPointer != nullptr)
	{
		auto tempPointer = m_ScalarPointer + StartElementIndex*m_ScalarNumberInElement;

		std::memcpy(static_cast<void*>(NewAddress), static_cast<void*>(tempPointer), NewScalarNumber*sizeof(ScalarType));

		delete[] m_ScalarPointer;
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
bool mdkDataArrayTemplate<ScalarType>::Squeeze()
{
	if (m_ScalarPointer == nullptr || m_EffectiveElementNumber == m_ElementNumber)
	{
		return false;
	}

	return this->Cut(0, m_EffectiveElementNumber-1);
}


template<typename ScalarType>
bool mdkDataArrayTemplate<ScalarType>::GetElement(uint64 ElementIndex, double* Element)
{
	if (m_ScalarPointer == nullptr || ElementIndex >= m_ElementNumber)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	Pointer += ElementIndex*m_ScalarNumberInElement;

	for (uint64 i = 0; i < m_ScalarNumberInElement; ++i)
	{
		Element[i] = Pointer[i];
	}

	return true;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetScalarNumberInElement()
{
	return m_ScalarNumberInElement;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetElementNumber()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ElementNumber;
}


template<typename ScalarType>
uint64 mdkDataArrayTemplate<ScalarType>::GetEffectiveElementNumber()
{
	return m_EffectiveElementNumber;
}

template<typename ScalarType>
mdkScalarTypeEnum mdkDataArrayTemplate<ScalarType>::GetScalarType()
{
	std::string TypeName(typeid(m_ScalarTypeData).name());

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
void* mdkDataArrayTemplate<ScalarType>::GetScalarPointer()
{
	return static_cast<void*>(m_ScalarPointer);
}


template<typename ScalarType>
void mdkDataArrayTemplate<ScalarType>::Clear()
{
	if (m_ScalarPointer != nullptr)
	{
		delete[] m_ScalarPointer;
	}

	m_ScalarPointer = nullptr;

	m_ScalarNumberInElement = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;
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

}//end namespace mdk

#endif