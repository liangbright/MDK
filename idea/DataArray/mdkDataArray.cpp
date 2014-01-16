#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkDataArray.h"

namespace mdk
{

mdkDataArray::mdkDataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_UNKNOWN;
}


mdkDataArray::~mdkDataArray()
{
	this->Clear();
}


bool mdkDataArray::SetDataType(mdkDataTypeEnum DataType)
{
	if (m_DataPointer == nullptr)
	{
		m_DataType = DataType;

		return true;
	}

	return false;
}

bool mdkDataArray::SetElementDimension(uint32 Dim)
{
	if (m_DataPointer == nullptr)
	{
		m_ElementDimension = Dim;

		return true;
	}

	return false;
}


bool mdkDataArray::SetElementNumber(uint64 Number)
{
	if (m_DataPointer == nullptr)
	{
		m_EffectiveElementNumber = Number;

		return true;
	}

	return false;
}


int32 mdkDataArray::GetElementDimension()
{
	return m_ElementDimension;
}


uint64 mdkDataArray::GetElementNumber()
{
	return m_ElementNumber;
}


mdkDataTypeEnum mdkDataArray::GetDataType()
{
	return m_DataType;
}


void* mdkDataArray::GetDataPointer()
{
	return m_DataPointer;
}


template<typename DataType>
bool mdkDataArray::AllocateWithType()
{
	if (m_DataPointer == nullptr && m_EffectiveElementNumber > 0)
	{
		try
		{
			m_DataPointer = static_cast<void*>(new DataType[m_EffectiveElementNumber*m_ElementDimension]);
		}
		catch (std::bad_alloc&)
		{
			mdkWarning << "Not enough memory @ mdkDataArray::AllocateWithType()";
			m_DataPointer = nullptr;
			return false;
		}

		m_ElementNumber = m_EffectiveElementNumber;

		return true;
	}

	return false;
}


template<typename DataType>
bool mdkDataArray::ExpandWithType(int32 ElementNumber = 1000)
{
	auto NewSize = (m_ElementNumber + ElementNumber)*m_ElementDimension;

	try
	{
		auto NewAddress = static_cast<void*>(new DataType[NewSize]);
	}
	catch (std::bad_alloc&)
	{
		mdkWarning << "Not enough memory @ mdkDataArray::ExpandWithType()";
		return false;
	}


	std::memset(NewAddress, 0, NewSize*sizeof(DataType));

	if (m_DataPointer != nullptr)
	{
		std::memcpy(NewAddress, m_DataPointer, m_ElementNumber*m_ElementDimension*sizeof(DataType));

		delete[] m_DataPointer;
	}

	m_DataPointer = NewAddress;

	m_ElementNumber += ElementNumber;

	return true;
}


template<typename DataType>
bool mdkDataArray::SqueezeWithType()
{
	auto NewSize = m_EffectiveElementNumber*m_ElementDimension;

	if (m_EffectiveElementNumber > 0 && m_EffectiveElementNumber < m_ElementNumber)
	{
		try
		{
			auto NewAddress = static_cast<void*>(new DataType[NewSize]);
		}
	    catch (std::bad_alloc&)
   	    {
		    mdkWarning << "Not enough memory @ mdkDataArray::SqueezeWithType()";
		    return false;
	    }

		std::memset(NewAddress, 0, NewSize*sizeof(DataType));

		if (m_DataPointer != nullptr)
		{
			std::memcpy(NewData, m_DataPointer, NewSize*sizeof(DataType));

			delete[] m_DataPointer;
		}

		m_DataPointer = NewData;

		m_ElementNumber += ElementNumber;

		return true;
	}

	return false;
}


template<typename DataType>
void mdkDataArray::ClearWithType()
{
	if (m_DataPointer != nullptr)
	{
		auto Pointer = static_cast<DataType*>(m_DataPointer);
		delete[] Pointer;

		m_DataPointer = nullptr;

		m_ElementNumber = 0;

		m_EffectiveElementNumber = 0;

		m_DataType = mdkDataTypeEnum::MDK_UNKNOWN;
	}
}


template<typename DataType>
bool mdkDataArray::SetElementWithType(uint64 ElementIndex, double Component0)
{
	if (ElementIndex >= m_ElementNumber || m_ElementDimension != 1)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;

	return true;
}


template<typename DataType>
bool mdkDataArray::SetElementWithType(uint64 ElementIndex, double Component0, double Component1)
{
	if (ElementIndex >= m_ElementNumber || m_ElementDimension != 2)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;

	return true;
}


template<typename DataType>
bool mdkDataArray::SetElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	if (ElementIndex >= m_ElementNumber || m_ElementDimension != 3)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;
	Pointer[2] = Component2;

	return true;
}


template<typename DataType>
bool mdkDataArray::SetElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	if (ElementIndex >= m_ElementNumber || m_ElementDimension != 4)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;
	Pointer[2] = Component2;
	Pointer[3] = Component3;

	return true;
}


template<typename DataType>
bool mdkDataArray::SetElementWithType(uint64 ElementIndex, const double* Element)
{
	if (ElementIndex >= m_ElementNumber)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	for (uint32 i = 0; i < m_ElementDimension; ++i)
	{
		Pointer[i] = Element[i];
	}
	
	return true;
}


template<typename DataType>
bool mdkDataArray::SetElementComponentWithType(uint64 ElementIndex, uint32 ComponentIndex, double Component)
{
	if (ElementIndex >= m_ElementNumber || ComponentIndex >= m_ElementDimension || ComponentIndex < 1)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[ComponentIndex] = Component;

	return true;
}


template<typename DataType>
bool mdkDataArray::InsertElementWithType(uint64 ElementIndex, double Component0)
{
	if (m_ElementDimension != 1)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;

	return true;
}


template<typename DataType>
bool mdkDataArray::InsertElementWithType(uint64 ElementIndex, double Component0, double Component1)
{
	if (m_ElementDimension != 2)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;

	return true;
}


template<typename DataType>
bool mdkDataArray::InsertElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	if (m_ElementDimension != 3)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<double*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;
	Pointer[2] = Component2;

	return true;
}


template<typename DataType>
bool mdkDataArray::InsertElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	if (m_ElementDimension != 4)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[0] = Component0;
	Pointer[1] = Component1;
	Pointer[2] = Component2;
	Pointer[3] = Component3;

	return true;
}


template<typename DataType>
bool mdkDataArray::InsertElementWithType(uint64 ElementIndex, const double* Element)
{
	if (m_ElementDimension < 1)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	for (uint32 i = 0; i < m_ElementDimension; ++i)
	{
		Pointer[i] = Element[i];
	}

	return true;
}


template<typename DataType>
void mdkDataArray::InsertElementComponentWithType(uint64 ElementIndex, uint32 ComponentIndex, double Component)
{
	if (m_ElementDimension <= ComponentIndex)
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

		m_EffectiveElementNumber = ElementIndex + 1;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	Pointer[ComponentIndex] = Component;

	return true;
}


template<typename DataType>
bool mdkDataArray::GetElementWithType(uint64 ElementIndex, double* Element)
{
	if (ElementIndex > m_ElementNumber)
	{
		return false;
	}

	auto Pointer = static_cast<DataType*>(m_DataPointer);

	Pointer += ElementIndex;

	for (uint32 i = 0; i < m_ElementDimension; ++i)
	{
		Element[i] = Pointer[i];
	}

	return true;
}


bool mdkDataArray::Allocate()
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64 :
		return this->AllocateWithType<double>();

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->AllocateWithType<float>();

	case mdkDataTypeEnum::MDK_INT8:
		return this->AllocateWithType<int8>();

	case mdkDataTypeEnum::MDK_INT16:
		return this->AllocateWithType<int16>();

	case mdkDataTypeEnum::MDK_INT32:
		return this->AllocateWithType<int32>();

	case mdkDataTypeEnum::MDK_INT64:
		return this->AllocateWithType<int64>();

	case mdkDataTypeEnum::MDK_UINT8:
		return this->AllocateWithType<uint8>();

	case mdkDataTypeEnum::MDK_UINT16:
		return this->AllocateWithType<uint16>();

	case mdkDataTypeEnum::MDK_UINT32:
		return this->AllocateWithType<uint32>();

	case mdkDataTypeEnum::MDK_UINT64:
		return this->AllocateWithType<uint64>();

	default:
		return false;
	}
}


bool mdkDataArray::CopyData(void* SourceData, mdkDataTypeEnum SourceDataType, uint64 ElementNumber, uint32 ElementDimension)
{
	if (SourceData == nullptr || ElementNumber < 1 || ElementDimension < 1 || SourceDataType == mdkDataTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	this->Clear();

	double BypesOfSourceDataType = 0;

	try
	{
		switch (SourceDataType)
		{
		case mdkDataTypeEnum::MDK_DOUBLE64:
			m_DataPointer = static_cast<void*>(new double[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 8;
			break;

		case mdkDataTypeEnum::MDK_FLOAT32:
			m_DataPointer = static_cast<void*>(new float[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 4;
			break;

		case mdkDataTypeEnum::MDK_INT8:
			m_DataPointer = static_cast<void*>(new int8[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 1;
			break;

		case mdkDataTypeEnum::MDK_INT16:
			m_DataPointer = static_cast<void*>(new int16[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 2;
			break;

		case mdkDataTypeEnum::MDK_INT32:
			m_DataPointer = static_cast<void*>(new int32[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 4;
			break;

		case mdkDataTypeEnum::MDK_INT64:
			m_DataPointer = static_cast<void*>(new int64[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 8;
			break;

		case mdkDataTypeEnum::MDK_UINT8:
			m_DataPointer = static_cast<void*>(new uint8[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 1;
			break;

		case mdkDataTypeEnum::MDK_UINT16:
			m_DataPointer = static_cast<void*>(new uint16[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 2;
			break;

		case mdkDataTypeEnum::MDK_UINT32:
			m_DataPointer = static_cast<void*>(new uint32[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 4;
			break;

		case mdkDataTypeEnum::MDK_UINT64:
			m_DataPointer = static_cast<void*>(new uint64[ElementNumber*ElementDimension]);
			BypesOfSourceDataType = 8;
			break;
		}
	}
	catch (std::bad_alloc&)
	{
		mdkWarning << "Not enough memory @ mdkDataArray::CopyData()";
		m_DataPointer = nullptr;
		return false;
	}

	std::memcpy(m_DataPointer, SourceData, ElementNumber*ElementDimension*BypesOfSourceDataType);

	m_ElementNumber = ElementNumber;

	m_EffectiveElementNumber = ElementNumber;

	m_ElementDimension = ElementDimension;

	m_DataType = SourceDataType;

	return true;
}


bool mdkDataArray::Squeeze()
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SqueezeWithType<double>();

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SqueezeWithType<float>();

	case mdkDataTypeEnum::MDK_INT8:
		return this->SqueezeWithType<int8>();

	case mdkDataTypeEnum::MDK_INT16:
		return this->SqueezeWithType<int16>();

	case mdkDataTypeEnum::MDK_INT32:
		return this->SqueezeWithType<int32>();

	case mdkDataTypeEnum::MDK_INT64:
		return this->SqueezeWithType<int64>();

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SqueezeWithType<uint8>();

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SqueezeWithType<uint16>();

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SqueezeWithType<uint32>();

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SqueezeWithType<uint64>();

	default:
		return false;
	}
}


void mdkDataArray::Clear()
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->ClearWithType<double>();

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->ClearWithType<float>();

	case mdkDataTypeEnum::MDK_INT8:
		return this->ClearWithType<int8>();

	case mdkDataTypeEnum::MDK_INT16:
		return this->ClearWithType<int16>();

	case mdkDataTypeEnum::MDK_INT32:
		return this->ClearWithType<int32>();

	case mdkDataTypeEnum::MDK_INT64:
		return this->ClearWithType<int64>();

	case mdkDataTypeEnum::MDK_UINT8:
		return this->ClearWithType<uint8>();

	case mdkDataTypeEnum::MDK_UINT16:
		return this->ClearWithType<uint16>();

	case mdkDataTypeEnum::MDK_UINT32:
		return this->ClearWithType<uint32>();

	case mdkDataTypeEnum::MDK_UINT64:
		return this->ClearWithType<uint64>();

	default:
		return;
	}
}


bool mdkDataArray::SetElement(uint64 ElementIndex, double Component0)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementWithType<double>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementWithType<float>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementWithType<int8>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementWithType<int16>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementWithType<int32>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementWithType<int64>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementWithType<uint8>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementWithType<uint16>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementWithType<uint32>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementWithType<uint64>(ElementIndex, Component0);

	default:
		return false;
	}
}


bool mdkDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementWithType<double>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementWithType<float>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementWithType<int8>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementWithType<int16>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementWithType<int32>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementWithType<int64>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementWithType<uint8>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementWithType<uint16>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementWithType<uint32>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementWithType<uint64>(ElementIndex, Component0, Component1);

	default:
		return false;
	}
}


bool mdkDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementWithType<double>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementWithType<float>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementWithType<int8>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementWithType<int16>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementWithType<int32>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementWithType<int64>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementWithType<uint8>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementWithType<uint16>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementWithType<uint32>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementWithType<uint64>(ElementIndex, Component0, Component1, Component2);

	default:
		return false;
	}
}


bool mdkDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementWithType<double>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementWithType<float>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementWithType<int8>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementWithType<int16>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementWithType<int32>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementWithType<int64>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementWithType<uint8>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementWithType<uint16>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementWithType<uint32>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementWithType<uint64>(ElementIndex, Component0, Component1, Component2, Component3);

	default:
		return false;
	}
}


bool mdkDataArray::SetElement(uint64 ElementIndex, const double* Element)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementWithType<double>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementWithType<float>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementWithType<int8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementWithType<int16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementWithType<int32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementWithType<int64>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementWithType<uint8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementWithType<uint16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementWithType<uint32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementWithType<uint64>(ElementIndex, Element);

	default:
		return false;
	}
}


bool mdkDataArray::SetElementComponent(uint64 ElementIndex, uint32 ComponentIndex, double Component)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->SetElementComponentWithType<double>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->SetElementComponentWithType<float>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT8:
		return this->SetElementComponentWithType<int8>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT16:
		return this->SetElementComponentWithType<int16>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT32:
		return this->SetElementComponentWithType<int32>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT64:
		return this->SetElementComponentWithType<int64>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->SetElementComponentWithType<uint8>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->SetElementComponentWithType<uint16>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->SetElementComponentWithType<uint32>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->SetElementComponentWithType<uint64>(ElementIndex, ComponentIndex, Component);

	default:
		return false;
	}
}


bool mdkDataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementWithType<double>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementWithType<float>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementWithType<int8>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementWithType<int16>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementWithType<int32>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementWithType<int64>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementWithType<uint8>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementWithType<uint16>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementWithType<uint32>(ElementIndex, Component0);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementWithType<uint64>(ElementIndex, Component0);

	default:
		return false;
	}
}


bool mdkDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementWithType<double>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementWithType<float>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementWithType<int8>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementWithType<int16>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementWithType<int32>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementWithType<int64>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementWithType<uint8>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementWithType<uint16>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementWithType<uint32>(ElementIndex, Component0, Component1);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementWithType<uint64>(ElementIndex, Component0, Component1);

	default:
		return false;
	}
}


bool mdkDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementWithType<double>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementWithType<float>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementWithType<int8>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementWithType<int16>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementWithType<int32>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementWithType<int64>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementWithType<uint8>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementWithType<uint16>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementWithType<uint32>(ElementIndex, Component0, Component1, Component2);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementWithType<uint64>(ElementIndex, Component0, Component1, Component2);

	default:
		return false;
	}
}


bool mdkDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementWithType<double>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementWithType<float>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementWithType<int8>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementWithType<int16>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementWithType<int32>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementWithType<int64>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementWithType<uint8>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementWithType<uint16>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementWithType<uint32>(ElementIndex, Component0, Component1, Component2, Component3);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementWithType<uint64>(ElementIndex, Component0, Component1, Component2, Component3);

	default:
		return false;
	}
}

bool mdkDataArray::InsertElement(uint64 ElementIndex, const double* Element)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementWithType<double>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementWithType<float>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementWithType<int8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementWithType<int16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementWithType<int32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementWithType<int64>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementWithType<uint8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementWithType<uint16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementWithType<uint32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementWithType<uint64>(ElementIndex, Element);

	default:
		return false;
	}
}


bool mdkDataArray::InsertElementComponent(uint64 ElementIndex, uint32 ComponentIndex, double Component)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->InsertElementComponentWithType<double>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->InsertElementComponentWithType<float>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT8:
		return this->InsertElementComponentWithType<int8>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT16:
		return this->InsertElementComponentWithType<int16>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT32:
		return this->InsertElementComponentWithType<int32>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_INT64:
		return this->InsertElementComponentWithType<int64>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->InsertElementComponentWithType<uint8>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->InsertElementComponentWithType<uint16>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->InsertElementComponentWithType<uint32>(ElementIndex, ComponentIndex, Component);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->InsertElementComponentWithType<uint64>(ElementIndex, ComponentIndex, Component);

	default:
		return false;
	}
}


bool mdkDataArray::GetElement(uint64 ElementIndex, double* Element)
{
	switch (m_DataType)
	{
	case mdkDataTypeEnum::MDK_DOUBLE64:
		return this->GetElementWithType<double>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_FLOAT32:
		return this->GetElementWithType<float>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT8:
		return this->GetElementWithType<int8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT16:
		return this->GetElementWithType<int16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT32:
		return this->GetElementWithType<int32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_INT64:
		return this->GetElementWithType<int64>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT8:
		return this->GetElementWithType<uint8>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT16:
		return this->GetElementWithType<uint16>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT32:
		return this->GetElementWithType<uint32>(ElementIndex, Element);

	case mdkDataTypeEnum::MDK_UINT64:
		return this->GetElementWithType<uint64>(ElementIndex, Element);

	default:
		return false;
	}
}



}//end namespace mdk