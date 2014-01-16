#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkInt16DataArray.h"

namespace mdk
{

mdkInt16DataArray::mdkInt16DataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension = 0;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_INT16;
}

mdkInt16DataArray::~mdkInt16DataArray()
{
	this->Clear();
}


bool mdkInt16DataArray::Allocate()
{
	return this->Allocate_Template<int16>();
}


bool mdkInt16DataArray::Expand()
{
	return this->Expand_Template<int16>();
}


bool mdkInt16DataArray::Squeeze()
{
	return this->Squeeze_Template<int16>();
}


void mdkInt16DataArray::Clear()
{
	this->Clear_Template<int16>();
}


bool mdkInt16DataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<int16>(ElementIndex, Component0);
}

	
bool mdkInt16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<int16>(ElementIndex, Component0, Component1);
}

	
bool mdkInt16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<int16>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt16DataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<int16>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt16DataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<int16>(ElementIndex, ComponentIndex, Component);
}

bool mdkInt16DataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<int16>(ElementIndex, Component0);
}

	
bool mdkInt16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<int16>(ElementIndex, Component0, Component1);
}

	
bool mdkInt16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<int16>(ElementIndex, Component0, Component1, Component2);
}


bool mdkInt16DataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<int16>(ElementIndex, Component0, Component1, Component2, Component3);
}

	
bool mdkInt16DataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<int16>(ElementIndex, ComponentIndex, Component);
}

}//end namespace mdk