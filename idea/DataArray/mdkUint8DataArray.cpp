#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkUint8DataArray.h"

namespace mdk
{

mdkUint8DataArray::mdkUint8DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_UINT8;
}

mdkUint8DataArray::~mdkUint8DataArray()
{
	this->Clear();
}


bool mdkUint8DataArray::Allocate()
{
	return this->Allocate_Template<uint8>();
}


bool mdkUint8DataArray::Expand()
{
	return this->Expand_Template<uint8>();
}


bool mdkUint8DataArray::Squeeze()
{
	return this->Squeeze_Template<uint8>();
}


void mdkUint8DataArray::Clear()
{
	this->Clear_Template<uint8>();
}


bool mdkUint8DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<uint8>(ElementIndex, Component0);
}

	
bool mdkUint8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<uint8>(ElementIndex, Component0, Component1);
}

	
bool mdkUint8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<uint8>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<uint8>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint8DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<uint8>(ElementIndex, ComponentIndex, Component);
}

bool mdkUint8DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<uint8>(ElementIndex, Component0);
}

	
bool mdkUint8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<uint8>(ElementIndex, Component0, Component1);
}

	
bool mdkUint8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<uint8>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<uint8>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint8DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<uint8>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk