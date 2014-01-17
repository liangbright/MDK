#include "mdkDataArray.h"
#include "mdkDataArrayTemplate.h"

namespace mdk
{

mdkDataArray::mdkDataArray()
{	
	m_DataArray = nullptr;

	m_ScalarType = mdkScalarTypeEnum::MDK_UNKNOWN;

	m_ScalarType_Input = mdkScalarTypeEnum::MDK_UNKNOWN;

	m_ScalarNumberInElement_Input = 0;

	m_ElementNumber_Input = 0;
}


mdkDataArray::~mdkDataArray()
{
	if (m_DataArray != nullptr)
	{
		delete m_DataArray;
	}
}


bool mdkDataArray::SetScalarType(mdkScalarTypeEnum ScalarType)
{
	m_ScalarType_Input = ScalarType;
}


bool mdkDataArray::SetScalarNumberInElement(uint64 Number)
{
	if (m_DataArray != nullptr)
	{
		return false;
	}

	m_ScalarNumberInElement_Input = Number;

	return true;
}


bool mdkDataArray::SetElementNumber(uint64 Number)
{
	if (m_DataArray != nullptr)
	{
		return false;
	}

	m_ElementNumber_Input = Number;

	return true;
}


bool mdkDataArray::Initialize()
{
	if (m_DataArray != nullptr)
	{
		return false;
	}

	switch (m_ScalarType)
	{
	case mdkScalarTypeEnum::MDK_DOUBLE64:

		m_DataArray = new mdkDataArrayTemplate<double>;
		break;

	case mdkScalarTypeEnum::MDK_FLOAT32:

		m_DataArray = new mdkDataArrayTemplate<float>;
		break;

	case mdkScalarTypeEnum::MDK_INT8:

		m_DataArray = new mdkDataArrayTemplate<int8>;
		break;

	case mdkScalarTypeEnum::MDK_INT16:

		m_DataArray = new mdkDataArrayTemplate<int16>;
		break;

	case mdkScalarTypeEnum::MDK_INT32:

		m_DataArray = new mdkDataArrayTemplate<int32>;
		break;

	case mdkScalarTypeEnum::MDK_INT64:

		m_DataArray = new mdkDataArrayTemplate<int64>;
		break;

	case mdkScalarTypeEnum::MDK_UINT8:

		m_DataArray = new mdkDataArrayTemplate<uint8>;
		break;

	case mdkScalarTypeEnum::MDK_UINT16:

		m_DataArray = new mdkDataArrayTemplate<uint16>;
		break;

	case mdkScalarTypeEnum::MDK_UINT32:

		m_DataArray = new mdkDataArrayTemplate<uint32>;
		break;

	case mdkScalarTypeEnum::MDK_UINT64:

		m_DataArray = new mdkDataArrayTemplate<uint64>;
		break;
	}

	return true;
}


bool mdkDataArray::Allocate()
{
	if (m_DataArray != nullptr || m_ScalarNumberInElement_Input == 0 || m_ElementNumber_Input == 0 || m_ScalarType_Input == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	this->Initialize();

	m_DataArray->SetScalarNumberInElement(m_ScalarNumberInElement_Input);

	m_DataArray->SetElementNumber(m_ElementNumber_Input);

	auto IsOK = m_DataArray->Allocate();
	if (IsOK == false)
	{
		return false;
	}

	m_ScalarType = m_ScalarType_Input;

	return true;
}


bool mdkDataArray::CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
	                        uint64 ScalarNumberInElement, uint64 ElementNumber)
{
	if (m_DataArray == nullptr)
	{
		this->SetScalarNumberInElement(ScalarNumberInElement);
		this->SetElementNumber(ElementNumber);
		this->Initialize();
	}

	m_DataArray->CopyData(SourceData, SourceDataType, ScalarNumberInElement, ElementNumber);
}


bool mdkDataArray::SetElement(uint64 ElementIndex, const double* Element)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->SetElement(ElementIndex, Element);
}


bool mdkDataArray::InsertElement(uint64 ElementIndex, const double* Element)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->InsertElement(ElementIndex, Element);
}


bool mdkDataArray::Expand()
{
	return false;
}


bool mdkDataArray::Cut(uint64 StartElementIndex, uint64 EndElementIndex)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->Cut(StartElementIndex, EndElementIndex);
}


bool mdkDataArray::Squeeze()
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->Squeeze();
}


bool mdkDataArray::GetElement(uint64 ElementIndex, double* Element)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->GetElement(ElementIndex, Element);
}


uint64 mdkDataArray::GetScalarNumberInElement()
{
	return m_ScalarNumberInElement_Input;
}


uint64 mdkDataArray::GetElementNumber()
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetElementNumber();
}


uint64 mdkDataArray::GetEffectiveElementNumber()
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetEffectiveElementNumber();
}


mdkScalarTypeEnum mdkDataArray::GetScalarType()
{
	return m_ScalarType;
}


void* mdkDataArray::GetScalarPointer()
{
	if (m_DataArray == nullptr)
	{
		return nullptr;
	}

	return m_DataArray->GetScalarPointer();
}



void mdkDataArray::Clear()
{
	if (m_DataArray != nullptr)
	{
		delete m_DataArray;
	}

	m_DataArray = nullptr;

	m_ScalarType = mdkScalarTypeEnum::MDK_UNKNOWN;

	m_ScalarType_Input = mdkScalarTypeEnum::MDK_UNKNOWN;

	m_ScalarNumberInElement_Input = 0;

	m_ElementNumber_Input = 0;
}


bool mdkDataArray::IsEmpty()
{
	if (m_DataArray == nullptr)
	{
		return true;
	}

	return m_DataArray->IsEmpty();
}

}//end namespace mdk