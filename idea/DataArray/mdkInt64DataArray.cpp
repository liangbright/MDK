#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkInt64DataArray.h"

namespace mdk
{

mdkInt64DataArray::mdkInt64DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_INT64;
}

mdkInt64DataArray::~mdkInt64DataArray()
{
	this->Clear();
}


bool mdkInt64DataArray::Allocate()
{
	return this->Allocate_Template<int64>();
}


bool mdkInt64DataArray::Expand()
{
	return this->Expand_Template<int64>();
}


bool mdkInt64DataArray::Squeeze()
{
	return this->Squeeze_Template<int64>();
}


void mdkInt64DataArray::Clear()
{
	this->Clear_Template<int64>();
}


bool mdkInt64DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<int64>(ElementIndex, Component0);
}

	
bool mdkInt64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<int64>(ElementIndex, Component0, Component1);
}

	
bool mdkInt64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<int64>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt64DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<int64>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt64DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<int64>(ElementIndex, ComponentIndex, Component);
}

bool mdkInt64DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<int64>(ElementIndex, Component0);
}

	
bool mdkInt64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<int64>(ElementIndex, Component0, Component1);
}

	
bool mdkInt64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<int64>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt64DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<int64>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt64DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<int64>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk