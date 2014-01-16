#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkUint64DataArray.h"

namespace mdk
{

mdkUint64DataArray::mdkUint64DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_UINT64;
}

mdkUint64DataArray::~mdkUint64DataArray()
{
	this->Clear();
}


bool mdkUint64DataArray::Allocate()
{
	return this->Allocate_Template<uint64>();
}


bool mdkUint64DataArray::Expand()
{
	return this->Expand_Template<uint64>();
}


bool mdkUint64DataArray::Squeeze()
{
	return this->Squeeze_Template<uint64>();
}


void mdkUint64DataArray::Clear()
{
	this->Clear_Template<uint64>();
}


bool mdkUint64DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<uint64>(ElementIndex, Component0);
}

	
bool mdkUint64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<uint64>(ElementIndex, Component0, Component1);
}

	
bool mdkUint64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<uint64>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<uint64>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint64DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<uint64>(ElementIndex, ComponentIndex, Component);
}

bool mdkUint64DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<uint64>(ElementIndex, Component0);
}

	
bool mdkUint64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<uint64>(ElementIndex, Component0, Component1);
}

	
bool mdkUint64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<uint64>(ElementIndex, Component0, Component1, Component2);
}


bool mdkUint64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<uint64>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkUint64DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<uint64>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk