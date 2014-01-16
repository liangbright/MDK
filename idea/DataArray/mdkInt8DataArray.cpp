#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkInt8DataArray.h"

namespace mdk
{

mdkInt8DataArray::mdkInt8DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_INT8;
}

mdkInt8DataArray::~mdkInt8DataArray()
{
	this->Clear();
}


bool mdkInt8DataArray::Allocate()
{
	return this->Allocate_Template<int8>();
}


bool mdkInt8DataArray::Expand()
{
	return this->Expand_Template<int8>();
}


bool mdkInt8DataArray::Squeeze()
{
	return this->Squeeze_Template<int8>();
}


void mdkInt8DataArray::Clear()
{
	this->Clear_Template<int8>();
}


bool mdkInt8DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<int8>(ElementIndex, Component0);
}

	
bool mdkInt8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<int8>(ElementIndex, Component0, Component1);
}

	
bool mdkInt8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<int8>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt8DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<int8>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt8DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<int8>(ElementIndex, ComponentIndex, Component);
}

bool mdkInt8DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<int8>(ElementIndex, Component0);
}

	
bool mdkInt8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<int8>(ElementIndex, Component0, Component1);
}

	
bool mdkInt8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<int8>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt8DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<int8>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt8DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<int8>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk