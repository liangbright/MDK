#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkFloatDataArray.h"

namespace mdk
{

mdkFloatDataArray::mdkFloatDataArray()
{
	m_DataPointer = nullptr;

	m_ElementDimension=3;

	m_ElementNumber = 0;

	m_EffectiveElementNumber = 0;

	m_DataType = mdkDataTypeEnum::MDK_FLOAT;
}

mdkFloatDataArray::~mdkFloatDataArray()
{
	this->Clear();
}


bool mdkFloatDataArray::Allocate()
{
	return this->Allocate_Template<float>();
}


bool mdkFloatDataArray::Expand()
{
	return this->Expand_Template<float>();

}

bool mdkFloatDataArray::Squeeze()
{
	return this->Squeeze_Template<float>();

}

void mdkFloatDataArray::Clear()
{
	this->Clear_Template<float>();
}

bool mdkFloatDataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElement_Template<float>(ElementIndex, Component0);
}

bool mdkFloatDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElement_Template<float>(ElementIndex, Component0, Component1);
}

bool mdkFloatDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElement_Template<float>(ElementIndex, Component0, Component1, Component2);
}

bool mdkFloatDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElement_Template<float>(ElementIndex, Component0, Component1, Component2, Component3);
}

bool mdkFloatDataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElement_Template<float>(ElementIndex, ComponentIndex, Component);
}

bool mdkFloatDataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElement_Template<float>(ElementIndex, Component0);
}

bool mdkFloatDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElement_Template<float>(ElementIndex, Component0, Component1);
}

bool mdkFloatDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElement_Template<float>(ElementIndex, Component0, Component1, Component2);
}

bool mdkFloatDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElement_Template<float>(ElementIndex, Component0, Component1, Component2, Component3);
}

bool mdkFloatDataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElement_Template<float>(ElementIndex, ComponentIndex, Component);
}



}//end namespace mdk