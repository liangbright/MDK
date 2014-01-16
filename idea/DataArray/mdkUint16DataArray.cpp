#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkUint16DataArray.h"

namespace mdk
{

mdkUint16DataArray::mdkUint16DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_UINT16;
}

mdkUint16DataArray::~mdkUint16DataArray()
{
	this->Clear();
}


bool mdkUint16DataArray::Allocate()
{
	return this->Allocate_Template<uint16>();
}


bool mdkUint16DataArray::Expand()
{
	return this->Expand_Template<uint16>();
}


bool mdkUint16DataArray::Squeeze()
{
	return this->Squeeze_Template<uint16>();
}


void mdkUint16DataArray::Clear()
{
	this->Clear_Template<uint16>();
}


bool mdkUint16DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<uint16>(ElementIndex, Component0);
}

	
bool mdkUint16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<uint16>(ElementIndex, Component0, Component1);
}

	
bool mdkUint16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<uint16>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<uint16>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint16DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<uint16>(ElementIndex, ComponentIndex, Component);
}

bool mdkUint16DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<uint16>(ElementIndex, Component0);
}

	
bool mdkUint16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<uint16>(ElementIndex, Component0, Component1);
}

	
bool mdkUint16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<uint16>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<uint16>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint16DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<uint16>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk