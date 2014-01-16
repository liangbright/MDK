#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkUint32DataArray.h"

namespace mdk
{

mdkUint32DataArray::mdkUint32DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_UINT32;
}

mdkUint32DataArray::~mdkUint32DataArray()
{
	this->Clear();
}


bool mdkUint32DataArray::Allocate()
{
	return this->Allocate_Template<uint32>();
}


bool mdkUint32DataArray::Expand()
{
	return this->Expand_Template<uint32>();
}


bool mdkUint32DataArray::Squeeze()
{
	return this->Squeeze_Template<uint32>();
}


void mdkUint32DataArray::Clear()
{
	this->Clear_Template<uint32>();
}


bool mdkUint32DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<uint32>(ElementIndex, Component0);
}

	
bool mdkUint32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<uint32>(ElementIndex, Component0, Component1);
}

	
bool mdkUint32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<uint32>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<uint32>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint32DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<uint32>(ElementIndex, ComponentIndex, Component);
}

bool mdkUint32DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<uint32>(ElementIndex, Component0);
}

	
bool mdkUint32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<uint32>(ElementIndex, Component0, Component1);
}

	
bool mdkUint32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<uint32>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<uint32>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint32DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<uint32>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk