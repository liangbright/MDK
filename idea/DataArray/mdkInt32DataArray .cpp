#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkInt32DataArray.h"

namespace mdk
{

mdkInt32DataArray::mdkInt32DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_INT32;
}

mdkInt32DataArray::~mdkInt32DataArray()
{
	this->Clear();
}


bool mdkInt32DataArray::Allocate()
{
	return this->Allocate_Template<int32>();
}


bool mdkInt32DataArray::Expand()
{
	return this->Expand_Template<int32>();
}


bool mdkInt32DataArray::Squeeze()
{
	return this->Squeeze_Template<int32>();
}


void mdkInt32DataArray::Clear()
{
	this->Clear_Template<int32>();
}


bool mdkInt32DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<int32>(ElementIndex, Component0);
}

	
bool mdkInt32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<int32>(ElementIndex, Component0, Component1);
}

	
bool mdkInt32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<int32>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt32DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<int32>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt32DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<int32>(ElementIndex, ComponentIndex, Component);
}

bool mdkInt32DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<int32>(ElementIndex, Component0);
}

	
bool mdkInt32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<int32>(ElementIndex, Component0, Component1);
}

	
bool mdkInt32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<int32>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt32DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<int32>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt32DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<int32>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk