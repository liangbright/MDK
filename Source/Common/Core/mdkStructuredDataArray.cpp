#include <cstring>

#include "mdkDebug.h"

#include "mdkStructuredDataArray.h"
#include "mdkStructuredDataArrayTemplate.h"


namespace mdk
{

mdkStructuredDataArray::mdkStructuredDataArray()
{	
	m_DataArray = nullptr;

	m_ScalarType = mdkScalarTypeEnum::MDK_UNKNOWN;
}


mdkStructuredDataArray::~mdkStructuredDataArray()
{
	this->Clear();
}


bool mdkStructuredDataArray::InitializeDataArrayWithType(mdkScalarTypeEnum ScalarType)
{
	if (m_DataArray != nullptr || ScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	switch (ScalarType)
	{
	case mdkScalarTypeEnum::MDK_DOUBLE64:

		m_DataArray = new mdkStructuredDataArrayTemplate<double>;
		break;

	case mdkScalarTypeEnum::MDK_FLOAT32:

		m_DataArray = new mdkStructuredDataArrayTemplate<float>;
		break;

	case mdkScalarTypeEnum::MDK_INT8:

		m_DataArray = new mdkStructuredDataArrayTemplate<int8>;
		break;

	case mdkScalarTypeEnum::MDK_INT16:

		m_DataArray = new mdkStructuredDataArrayTemplate<int16>;
		break;

	case mdkScalarTypeEnum::MDK_INT32:

		m_DataArray = new mdkStructuredDataArrayTemplate<int32>;
		break;

	case mdkScalarTypeEnum::MDK_INT64:

		m_DataArray = new mdkStructuredDataArrayTemplate<int64>;
		break;

	case mdkScalarTypeEnum::MDK_UINT8:

		m_DataArray = new mdkStructuredDataArrayTemplate<uint8>;
		break;

	case mdkScalarTypeEnum::MDK_UINT16:

		m_DataArray = new mdkStructuredDataArrayTemplate<uint16>;
		break;

	case mdkScalarTypeEnum::MDK_UINT32:

		m_DataArray = new mdkStructuredDataArrayTemplate<uint32>;
		break;

	case mdkScalarTypeEnum::MDK_UINT64:

		m_DataArray = new mdkStructuredDataArrayTemplate<uint64>;
		break;
	}

	m_ScalarType = ScalarType;

	return true;
}


bool mdkStructuredDataArray::Allocate(mdkScalarTypeEnum ScalarType, uint64 ElementDimension, const uint64 ElementSize[], uint64 ArrayDimension, const uint64 ArraySize[])
{
	if (m_DataArray != nullptr || ElementDimension == 0 || ArrayDimension == 0
		|| ScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	auto IsOK = this->InitializeDataArrayWithType(ScalarType);
	if (IsOK == false)
	{
		return false;
	}

	return m_DataArray->Allocate(ElementDimension, ElementSize, ArrayDimension, ArraySize);
}


bool mdkStructuredDataArray::CopyData(void* SourceData, mdkScalarTypeEnum SourceScalarType,
	                                  uint64 ElementDimension, const uint64 ElementSize[],
	                                  uint64 ArrayDimension,   const uint64 ArraySize[])
{
	if (ElementDimension == 0 || ArrayDimension == 0 || SourceScalarType == mdkScalarTypeEnum::MDK_UNKNOWN)
	{
		return false;
	}

	if (m_DataArray == nullptr)
	{
		auto IsOK = this->InitializeDataArrayWithType(SourceScalarType);
		if (IsOK == false)
		{
			return false;
		}
	}

	return m_DataArray->CopyData(SourceData, SourceScalarType, ElementDimension, ElementSize, ArrayDimension, ArraySize);
}


bool mdkStructuredDataArray::CutToCube(const uint64 StartIndex[], const uint64 EndIndex[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->CutToCube(StartIndex, EndIndex);
}


bool mdkStructuredDataArray::SetElementByLinearIndex(uint64 Index, const double Element[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->SetElementByLinearIndex(Index, Element);
}


bool mdkStructuredDataArray::SetElementByPosition(const uint64* Position, const double Element[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->SetElementByPosition(Position, Element);
}


bool mdkStructuredDataArray::GetElementByLinearIndex(uint64 Index, double Element[]) const
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->GetElementByLinearIndex(Index, Element);
}


bool mdkStructuredDataArray::GetElementByPosition(const uint64 Position[], double Element[]) const
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->GetElementByPosition(Position, Element);
}


bool mdkStructuredDataArray::GetCube(const uint64 StartIndex[], const uint64 EndIndex[], mdkStructuredDataArray* OutputDataArray) const
{

}


uint64 mdkStructuredDataArray::GetScalarNumberInElement() const
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetScalarNumberInElement();
}


uint64 mdkStructuredDataArray::GetElementDimension() const
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetElementDimension();
}


bool mdkStructuredDataArray::GetElementSize(uint64 Size[]) const
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->GetElementSize(Size);
}


uint64 mdkStructuredDataArray::GetArrayDimension() const
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetArrayDimension();
}


bool mdkStructuredDataArray::GetArraySize(uint64 Size[]) const
{
	if (m_DataArray == nullptr)
	{
		return false;
	}
	
	return m_DataArray->GetArraySize(Size);
}


uint64 mdkStructuredDataArray::GetElementNumberInArray() const
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetElementNumberInArray();
}


uint64 mdkStructuredDataArray::GetScalarNumberInArray() const
{
	if (m_DataArray == nullptr)
	{
		return 0;
	}

	return m_DataArray->GetScalarNumberInArray();
}


mdkScalarTypeEnum mdkStructuredDataArray::GetScalarType() const
{
	return m_ScalarType;
}


void* mdkStructuredDataArray::GetScalarPointer()
{
	if (m_DataArray == nullptr)
	{
		return nullptr;
	}

	return m_DataArray->GetScalarPointer();
}


void mdkStructuredDataArray::Clear()
{
	if (m_DataArray == nullptr)
	{
		return ;
	}

	delete m_DataArray;

	m_DataArray = nullptr;

	m_ScalarType = mdkScalarTypeEnum::MDK_UNKNOWN;
}


bool mdkStructuredDataArray::FillScalar(double Scalar)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->FillScalar(Scalar);
}


bool mdkStructuredDataArray::AddScalar(double Scalar)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->AddScalar(Scalar);
}


bool mdkStructuredDataArray::SubtractScalar(double Scalar)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->SubtractScalar(Scalar);
}


bool mdkStructuredDataArray::MultiplyScalar(double Scalar)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->MultiplyScalar(Scalar);
}


bool mdkStructuredDataArray::DivideScalar(double Scalar)
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->DivideScalar(Scalar);
}


bool mdkStructuredDataArray::FillElement(const double Element[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->FillElement(Element);
}


bool mdkStructuredDataArray::AddElement(const double Element[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->AddElement(Element);
}


bool mdkStructuredDataArray::SubtractElement(const double Element[])
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	return m_DataArray->SubtractElement(Element);
}


mdkAbstractStructuredDataArrayTemplate* mdkStructuredDataArray::GetDataArrayPointer()
{
	return m_DataArray;
}


bool mdkStructuredDataArray::IsStructureSame(mdkStructuredDataArray& SourceArray) const
{
	if (m_DataArray == nullptr)
	{
		return false;
	}

	auto Pointer = SourceArray.GetDataArrayPointer();

	return m_DataArray->IsStructureSame(*Pointer);
}


bool mdkStructuredDataArray::CopyArray(mdkStructuredDataArray& SourceArray)
{
	if (m_DataArray == nullptr)
	{
		auto SourceScalarType = SourceArray.GetScalarType();

		auto IsOK = this->InitializeDataArrayWithType(SourceScalarType);
		if (IsOK == false)
		{
			return false;
		}
	}

	auto Pointer = SourceArray.GetDataArrayPointer();

	return m_DataArray->CopyArray(*Pointer);
}


bool mdkStructuredDataArray::AddArray(mdkStructuredDataArray& SourceArray)
{
	if (m_DataArray == nullptr)
	{
		auto SourceScalarType = SourceArray.GetScalarType();

		auto IsOK = this->InitializeDataArrayWithType(SourceScalarType);
		if (IsOK == false)
		{
			return false;
		}
	}

	auto Pointer = SourceArray.GetDataArrayPointer();

	return m_DataArray->AddArray(*Pointer);
}


bool mdkStructuredDataArray::SubtractArray(mdkStructuredDataArray& SourceArray)
{
	if (m_DataArray == nullptr)
	{
		auto SourceScalarType = SourceArray.GetScalarType();

		auto IsOK = this->InitializeDataArrayWithType(SourceScalarType);
		if (IsOK == false)
		{
			return false;
		}
	}

	auto Pointer = SourceArray.GetDataArrayPointer();

	return m_DataArray->SubtractArray(*Pointer);
}


mdkStructuredDataArray& mdkStructuredDataArray::operator<<(double Scalar)
{
	this->FillScalar(Scalar);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator+=(double Scalar)
{
	this->AddScalar(Scalar);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator-=(double Scalar)
{
	this->SubtractScalar(Scalar);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator*=(double Scalar)
{
	this->MultiplyScalar(Scalar);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator/=(double Scalar)
{
	this->DivideScalar(Scalar);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator<<(const double Element[])
{
	this->FillElement(Element);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator+=(const double Element[])
{
	this->AddElement(Element);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator-=(const double Element[])
{
	this->SubtractElement(Element);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator<<(mdkStructuredDataArray& SourceArray)
{
	this->CopyArray(SourceArray);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator+=(mdkStructuredDataArray& SourceArray)
{
	this->AddArray(SourceArray);
	return *this;
}


mdkStructuredDataArray& mdkStructuredDataArray::operator-=(mdkStructuredDataArray& SourceArray)
{
	this->SubtractArray(SourceArray);
	return *this;
}

}//end namespace mdk