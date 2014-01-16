#include "mdkDebug.h"
#include "mdkType.h"
#include "mdkDoubleDataArray.h"

namespace mdk
{

mdkDoubleDataArray::mdkDoubleDataArray()
{
	this->SetDataType(mdkDataTypeEnum::MDK_DOUBLE);
}


mdkDoubleDataArray::~mdkDoubleDataArray()
{
	this->Clear();
}


bool mdkDoubleDataArray::Allocate()
{
	return this->AllocateWithType<double>();
}


bool mdkDoubleDataArray::Expand()
{
	return this->ExpandWithType<double>();

}

bool mdkDoubleDataArray::Squeeze()
{
	return this->SqueezeWithType<double>();

}

void mdkDoubleDataArray::Clear()
{
	this->ClearWithType<double>();
}

bool mdkDoubleDataArray::SetElement(uint64 ElementIndex, double Component0)
{
	return this->SetElementWithType<double>(ElementIndex, Component0);
}

bool mdkDoubleDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->SetElementWithType<double>(ElementIndex, Component0, Component1);
}

bool mdkDoubleDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->SetElementWithType<double>(ElementIndex, Component0, Component1, Component2);
}

bool mdkDoubleDataArray::SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->SetElementWithType<double>(ElementIndex, Component0, Component1, Component2, Component3);
}

bool mdkDoubleDataArray::SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->SetElementWithType<double>(ElementIndex, ComponentIndex, Component);
}

bool mdkDoubleDataArray::InsertElement(uint64 ElementIndex, double Component0)
{
	return this->InsertElementWithType<double>(ElementIndex, Component0);
}

bool mdkDoubleDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1)
{
	return this->InsertElementWithType<double>(ElementIndex, Component0, Component1);
}

bool mdkDoubleDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2)
{
	return this->InsertElementWithType<double>(ElementIndex, Component0, Component1, Component2);
}

bool mdkDoubleDataArray::InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3)
{
	return this->InsertElementWithType<double>(ElementIndex, Component0, Component1, Component2, Component3);
}

bool mdkDoubleDataArray::InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component)
{
	return this->InsertElementWithType<double>(ElementIndex, ComponentIndex, Component);
}



}//end namespace mdk