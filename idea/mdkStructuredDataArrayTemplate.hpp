#ifndef __mdkStructuredDataArrayTemplate_hpp
#define __mdkStructuredDataArrayTemplate_hpp

#include <typeinfo>

#include <memory>
#include <cstdlib>
#include <cstring>

#include "mdkDebug.h"

#include "mdkStructuredDataArrayTemplate.h"

namespace mdk
{

template<typename ScalarType>
mdkStructuredDataArrayTemplate<ScalarType>::mdkStructuredDataArrayTemplate()
{	
	m_ScalarPointer = nullptr;

	m_ScalarType = mdkScalarTypeEnum::MDK_UNKNOWN;

	m_ScalarNumberInElement = 0;

	m_ElementDimension = 0;

	m_ElementSize = nullptr;

	m_ArrayDimension = 0;

	m_ArraySize = nullptr;

	m_ElementNumberInArray = 0;

	m_ScalarNumberInArray = 0;
}


template<typename ScalarType>
mdkStructuredDataArrayTemplate<ScalarType>::~mdkStructuredDataArrayTemplate()
{
	this->Clear();
}


template<typename ScalarType>
void mdkStructuredDataArrayTemplate<ScalarType>::SetElementDimensionAndSize(uint64 Dim, const uint64* Size)
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
}


template<typename ScalarType>
void mdkStructuredDataArrayTemplate<ScalarType>::SetArrayDimensionAndSize(uint64 Dim, const uint64* Size)
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
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::IsEmpty()
{
	if (m_ScalarPointer == nullptr)
	{
		return true;
	}

	return false;
}





template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::Allocate(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize)
{
	if (m_ScalarPointer != nullptr || ElementDimension == 0 || ArrayDimension == 0)
	{
		return false;
	}

	auto ScalarNumber = mdkAbstractStructuredDataArrayTemplate::CalScalarNumber(ElementDimension, ElementSize, ArrayDimension, ArraySize);

	try
	{
		m_ScalarPointer = new ScalarType[ScalarNumber];
	}
	catch (...)
	{
		mdkWarning << "Not enough memory @ mdkStructuredDataArrayTemplate<ScalarType>::Allocate()";
		m_ScalarPointer = nullptr;
		return false;
	}

	std::memset(static_cast<void*>(m_ScalarPointer), 0, ScalarNumber*sizeof(ScalarType));

	m_ScalarType = this->GetScalarType();

	this->SetElementDimensionAndSize(ElementDimension, ElementSize);

	this->SetArrayDimensionAndSize(ArrayDimension, ArraySize);

	m_ScalarNumberInElement = mdkAbstractStructuredDataArrayTemplate::CalScalarNumberInElement(ElementDimension, ElementSize);

	m_ElementNumberInArray = mdkAbstractStructuredDataArrayTemplate::CalElementNumberInArray(ArrayDimension, ArraySize);

	m_ScalarNumberInArray = mdkAbstractStructuredDataArrayTemplate::CalScalarNumberInArray(ElementDimension, ElementSize, ArrayDimension, ArraySize);

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::CopyData(void* SourceData, mdkScalarTypeEnum SourceScalarType,
	                                                      uint64 ElementDimension, const uint64* ElementSize,
	                                                      uint64 ArrayDimension, const uint64* ArraySize)
{
	if (SourceScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	this->Clear();

	auto IsAllocated = this->Allocate(ElementDimension, ElementSize, ArrayDimension, ArraySize);
	if (IsAllocated == false)
	{
		return false;
	}

	if (m_ScalarType == SourceScalarType)
	{
		std::memcpy(static_cast<void*>(m_ScalarPointer), SourceData, ScalarNumber*sizeof(ScalarType));
	}
	else
	{
		switch (SourceScalarType)
		{
		case mdkScalarTypeEnum::MDK_DOUBLE64:

			auto SourcePointer = static_cast<double*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_FLOAT32:

			auto SourcePointer = static_cast<float*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_INT8:

			auto SourcePointer = static_cast<int8*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_INT16:

			auto SourcePointer = static_cast<int16*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_INT32:

			auto SourcePointer = static_cast<int32*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_INT64:

			auto SourcePointer = static_cast<int64*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT8:

			auto SourcePointer = static_cast<uint8*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT16:

			auto SourcePointer = static_cast<uint16*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT32:

			auto SourcePointer = static_cast<uint32*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;

		case mdkScalarTypeEnum::MDK_UINT64:

			auto SourcePointer = static_cast<uint64*>(SourceData);
			auto Pointer = m_ScalarPointer;
			for (uint64 i = 0; i < m_ScalarNumber; ++i)
			{
				Pointer[0] = ScalarType(SourcePointer[0]);
				++Pointer;
				++SourcePointer;
			}
			break;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::CutToCube(const uint64* StartIndex, const uint64* EndIndex)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	uint64 NewElementNumber = 0;

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		NewElementNumber += (EndIndex[i] - StartIndex[i] + 1)*m_ScalarNumberInElement;
	}

	auto NewScalarNumber = NewElementNumber*m_ScalarNumberInElement;

	if (NewScalarNumber >= m_ScalarNumberInArray)
	{
		return false;
	}

	ScalarType* NewAddress = nullptr;

	try
	{
		NewAddress = new ScalarType[NewScalarNumber];
	}
	catch (...)
	{
		mdkWarning << "Not enough memory @ mdkDataArrayTemplate::Cut()";
		return false;
	}

	std::memset(static_cast<void*>(NewAddress), 0, NewScalarNumber*sizeof(ScalarType));

	if (m_ScalarPointer != nullptr)
	{
		for (uint64 i = 0; i < m_ArrayDimension; ++i)
		{
			for (uint64 j = 0; j < m_ElementDimension; ++j)
			{
				NewElementIndex = 

				NewAddress[NewElementIndex] = m_ScalarPointer[ElementIndex]
			}
		}

		delete[] m_ScalarPointer;
	}

	m_ScalarPointer = NewAddress;

	m_ElementNumberInArray = NewElementNumber;

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		m_ArraySize[i] = EndIndex[i] - StartIndex[i] + 1;
	}

	m_ScalarNumberInArray = NewScalarNumber;

	return true;

}


template<typename ScalarType>
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetElementLinearIndex(const uint64* Position)
{
	uint64 Index = 0;

	switch (m_ArrayDimension)
	{
	case 1:
		Index = Position[0];
		break;

	case 2:
		Index = Position[0] + Position[1] * m_ArraySize[0];
		break;

	case 3:
		Index = Position[0] + Position[1] * m_ArraySize[0] + Position[2] * m_ArraySize[1] * m_ArraySize[0];
		break;

	case 4:
		Index = Position[0] + Position[1] * m_ArraySize[0] + Position[2] * m_ArraySize[1] * m_ArraySize[0]
			   + Position[3] * m_ArraySize[2] * m_ArraySize[1] * m_ArraySize[0];
		break;

	default:

		Index = 0;

		auto tempValue = 1;

		for (uint64 i = 0; i < m_ArrayDimension; ++i)
		{
			Index += Position[i] * tempValue;

			tempValue *= m_ArraySize[i];
		}
	}

	return Index;
}


template<typename ScalarType>
void mdkStructuredDataArrayTemplate<ScalarType>::GetElementPosition(uint64 Index, uint64* Position)
{
	switch (m_ArrayDimension)
	{
	case 1:
		Position[0] = Index;

		break;

	case 2:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, m_ArraySize[0]);

		Position[1] = divresult.quot;
		Position[0] = divresult.rem;

		break;

	case 3:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, m_ArraySize[1] * m_ArraySize[0]);

		Position[2] = divresult.quot;

		divresult = div(divresult.rem, m_ArraySize[0]);

		Position[1] = divresult.quot;

		Position[0] = divresult.rem;

		break;

	case 4:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, m_ArraySize[2] * m_ArraySize[1] * m_ArraySize[0]);

		Position[3] = divresult.quot;

		divresult = div(divresult.rem, m_ArraySize[1] * m_ArraySize[0]);

		Position[2] = divresult.quot;

		divresult = div(divresult.rem, m_ArraySize[0]);

		Position[1] = divresult.quot;

		Position[0] = divresult.rem;

		break;

	default:
		std::lldiv_t divresult;
		divresult.rem = Index;

		uint64 tempValue = 1;
		for (uint64 i = 0; i < m_ArrayDimension-1; ++i)
		{
			tempValue *= m_ArraySize[i];
		}

		for (uint64 i = m_ArrayDimension - 1; i >= 1; --i)
		{
			divresult = div(divresult.rem, tempValue);

			Position[i] = divresult.quot;

			tempValue /= m_ArraySize[i];
		}

		Position[0] = divresult.rem;
	}
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::SetElementByLinearIndex(uint64 Index, const double Element[])
{
	if (m_ScalarPointer == nullptr || Index >= m_ElementNumberInArray)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	Pointer += ElementLinearIndex*m_ScalarNumberInElement;

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		Pointer[i] = ScalarType(Element[i]);
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::SetElementByPosition(const uint64* ElementPosition, const double Element[])
{
	auto ElementLinearIndex = this->GetElementLinearIndex(ElementPosition);

	return this->SetElementByLinearIndex(ElementLinearIndex, Element);
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::GetElementByLinearIndex(uint64 Index, double Element[])
{
	if (m_ScalarPointer == nullptr || Index >= m_ElementNumberInArray)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	Pointer += ElementLinearIndex*m_ScalarNumberInElement;

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		Element[i] = double(Pointer[i]);
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::GetElementByPosition(const uint64* Position, double Element[])
{
	auto ElementLinearIndex = this->GetElementLinearIndex(Position);

	return this->GetElementByLinearIndex(ElementLinearIndex, Element);
}


template<typename ScalarType>
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetScalarNumberInArrayInElement()
{
	return m_ScalarNumberInElement;
}


template<typename ScalarType>
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetElementDimension()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ElementDimension;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::GetElementSize(uint64* Size)
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
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetArrayDimension()
{
	if (m_ScalarPointer == nullptr)
	{
		return 0;
	}

	return m_ArrayDimension;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::GetArraySize(uint64* Size)
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
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetElementNumberInArray()
{
	return m_ElementNumberInArray;
}


template<typename ScalarType>
uint64 mdkStructuredDataArrayTemplate<ScalarType>::GetScalarNumberInArray()
{
	return m_ScalarNumberInArray;
}


template<typename ScalarType>
mdkScalarTypeEnum mdkStructuredDataArrayTemplate<ScalarType>::GetScalarType()
{
	ScalarType ScalarTypeData;

	std::string TypeName(typeid(ScalarTypeData).name());

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
ScalarType* mdkStructuredDataArrayTemplate<ScalarType>::GetScalarPointerWithType()
{
	return m_ScalarPointer;
}


template<typename ScalarType>
void* mdkStructuredDataArrayTemplate<ScalarType>::GetScalarPointer()
{
	return static_cast<void*>(m_ScalarPointer);
}


template<typename ScalarType>
void mdkStructuredDataArrayTemplate<ScalarType>::Clear()
{
	if (m_ScalarPointer != nullptr)
	{
		delete[] m_ScalarPointer;
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

	m_ScalarNumberInElement = 0;

	m_ArrayDimension = 0;

	m_ArraySize = nullptr;

	m_ElementNumberInArray = 0;

	m_ScalarNumberInArray = 0;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::FillScalar(double Scalar)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (Scalar == 0.0)
	{
		std::memset(static_cast<void*>(m_ScalarPointer), 0, m_ScalarNumber*sizeof(ScalarType));
	}
	else
	{
		auto tempValue = ScalarType(Scalar);

		auto Pointer = m_ScalarPointer;
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			Pointer[0] = tempValue;
			++Pointer;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::AddScalar(double Scalar)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (Scalar == 0.0)
	{
		return true;
	}
	else
	{
		auto tempValue = ScalarType(Scalar);

		auto Pointer = m_ScalarPointer;
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			Pointer[0] += tempValue;
			++Pointer;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::SubtractScalar(double Scalar)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (Scalar == 0.0)
	{
		return true;
	}
	else
	{
		auto tempValue = ScalarType(Scalar);

		auto Pointer = m_ScalarPointer;
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			Pointer[0] -= tempValue;
			++Pointer;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::MultiplyScalar(double Scalar)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (Scalar == 0.0)
	{
		return true;
	}
	else
	{
		auto tempValue = ScalarType(Scalar);

		auto Pointer = m_ScalarPointer;
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			Pointer[0] *= tempValue;
			++Pointer;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::DivideScalar(double Scalar)
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	if (abs(Scalar) < 0.0000000000000001)
	{
		mdkWarning << "Scalar is nearly zero @ mdkStructuredDataArrayTemplate<ScalarType>::DivideScalar(double Scalar)";
		mdkWarning << "DivideScalar() is not performed, return false";
		return false;
	}
	else
	{
		auto tempValue = ScalarType(Scalar);

		auto Pointer = m_ScalarPointer;
		for (uint64 i = 0; i < m_ScalarNumber; ++i)
		{
			Pointer[0] /= tempValue;
			++Pointer;
		}
	}

	return true;
}



template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::FillElement(const double Element[])
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	for (uint64 i = 0; i < m_ElementNumberInArray; ++i)
	{
		Pointer += m_ScalarNumberInElement;

		for (uint64 j = 0; j < m_ScalarNumberInElement; ++j)
		{
			Pointer[j] = Element[j];
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::AddElement(const double Element[])
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	for (uint64 i = 0; i < m_ElementNumberInArray; ++i)
	{
		Pointer += m_ScalarNumberInElement;

		for (uint64 j = 0; j < m_ScalarNumberInElement; ++j)
		{
			Pointer[j] += Element[j];
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::SubtractElement(const double Element[])
{
	if (m_ScalarPointer == nullptr)
	{
		return false;
	}

	auto Pointer = m_ScalarPointer;

	for (uint64 i = 0; i < m_ElementNumberInArray; ++i)
	{
		Pointer += m_ScalarNumberInElement;

		for (uint64 j = 0; j < m_ScalarNumberInElement; ++j)
		{
			Pointer[j] -= Element[j];
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::IsStructureSame(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	if (m_ScalarPointer == nullptr || SourceArray.IsEmpty() == true 
		|| SourceArray.GetScalarType() == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	auto ElementDimension_source = SourceArray.GetElementDimension();

	auto ArrayDimension_source = SourceArray.GetArrayDimension();

	//check element dimension and size
	if (m_ElementDimension != ElementDimension_source || m_ArrayDimension != ArrayDimension_source)
	{
		return false;
	}

	auto ElementSize_source_upt = std::unique_ptr<uint64[]>(new uint64[ElementDimension_source]);
	auto ElementSize_source = ElementSize_source_upt.get();

	SourceArray.GetElementSize(ElementSize_source);

	auto ArraySize_source_upt = std::unique_ptr<uint64[]>(new uint64[ArrayDimension_source]);
	auto ArraySize_source = ArraySize_source_upt.get();

	SourceArray.GetArraySize(ArraySize_source);

	for (uint64 i = 0; i < m_ElementDimension; ++i)
	{
		if (m_ElementSize[i] != ElementSize_source[i])
		{
			return false;
		}
	}

	for (uint64 i = 0; i < m_ArrayDimension; ++i)
	{
		if (m_ArraySize[i] != ArraySize_source[i])
		{
			return false;
		}
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::CopyArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	auto SourceScalarType = SourceArray.GetScalarType();
	if (SourceScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	auto SourcePointer = SourceArray.GetScalarPointer();

	auto ElementDimension_source = SourceArray.GetElementDimension();

	auto ArrayDimension_source = SourceArray.GetArrayDimension();

	auto ElementSize_source_upt = std::unique_ptr<uint64[]>(new uint64[ElementDimension_source]);
	auto ElementSize_source = ElementSize_source_upt.get();

	SourceArray.GetElementSize(ElementSize_source);

	auto ArraySize_source_upt = std::unique_ptr<uint64[]>(new uint64[ArrayDimension_source]);
	auto ArraySize_source = ArraySize_source_upt.get();

	SourceArray.GetArraySize(ArraySize_source);

	return this->CopyData(SourcePointer, SourceScalarType, ElementDimension_source, ElementSize_source,
		                  ArrayDimension_source, ArraySize_source);
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::AddArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	auto IsOK = this->IsStructureSame(SourceArray);
	if (IsOK == false)
	{
		return false;
	}

	// all sizes match, ok, then add data

	auto SourceScalarType = SourceArray.GetScalarType();

	switch (SourceScalarType)
	{
	case mdkScalarTypeEnum::MDK_DOUBLE64:

		auto SourcePointer = static_cast<double*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_FLOAT32:

		auto SourcePointer = static_cast<float*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT8:

		auto SourcePointer = static_cast<int8*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT16:

		auto SourcePointer = static_cast<int16*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT32:

		auto SourcePointer = static_cast<int32*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT64:

		auto SourcePointer = static_cast<int64*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT8:

		auto SourcePointer = static_cast<uint8*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT16:

		auto SourcePointer = static_cast<uint16*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT32:

		auto SourcePointer = static_cast<uint32*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT64:

		auto SourcePointer = static_cast<uint64*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] += ScalarType(SourceData[i]);
		}
		break;
	}

	return true;
}


template<typename ScalarType>
bool mdkStructuredDataArrayTemplate<ScalarType>::SubtractArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	auto IsOK = this->IsStructureSame(SourceArray);
	if (IsOK == false)
	{
		return false;
	}

	// all sizes match, ok, then minus data

	auto SourceScalarType = SourceArray.GetScalarType();

	switch (SourceScalarType)
	{
	case mdkScalarTypeEnum::MDK_DOUBLE64:

		auto SourcePointer = static_cast<double*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_FLOAT32:

		auto SourcePointer = static_cast<float*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT8:

		auto SourcePointer = static_cast<int8*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT16:

		auto SourcePointer = static_cast<int16*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT32:

		auto SourcePointer = static_cast<int32*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_INT64:

		auto SourcePointer = static_cast<int64*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT8:

		auto SourcePointer = static_cast<uint8*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT16:

		auto SourcePointer = static_cast<uint16*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT32:

		auto SourcePointer = static_cast<uint32*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;

	case mdkScalarTypeEnum::MDK_UINT64:

		auto SourcePointer = static_cast<uint64*>(SourceData);
		for (uint64 i = 0; i < ScalarNumber; ++i)
		{
			m_ScalarPointer[i] -= ScalarType(SourceData[i]);
		}
		break;
	}

	return true;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator<<(const double Element[])
{
	this->FillElement(Element);
	return *this;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator+=(const double Element[])
{
	this->AddElement(Element);
	return *this;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator-=(const double Element[])
{
	this->SubtractElement(Element);
	return *this;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator<<(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	this->CopyArray(SourceArray);
	return *this;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator+=(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	this->AddArray(SourceArray);
	return *this;
}


template<typename ScalarType>
mdkAbstractStructuredDataArrayTemplate& mdkStructuredDataArrayTemplate<ScalarType>::operator-=(const mdkAbstractStructuredDataArrayTemplate& SourceArray)
{
	this->SubtractArray(SourceArray);
	return *this;
}

}//end namespace mdk

#endif