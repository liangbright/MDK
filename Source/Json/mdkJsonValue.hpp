#ifndef mdk_JsonValue_hpp
#define mdk_JsonValue_hpp


namespace mdk
{

template<int_max TempLength>
JsonValue::JsonValue(DenseVector<int, TempLength> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

template<int_max TempLength>
JsonValue::JsonValue(DenseVector<long long, TempLength> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

template<int_max TempLength>
JsonValue::JsonValue(DenseVector<float, TempLength> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

template<int_max TempLength>
JsonValue::JsonValue(DenseVector<double, TempLength> InputArray)
{
	m_Type = TypeEnum::Type_Null;
	(*this) = std::move(InputArray);
}

template<int_max TempLength>
void JsonValue::operator=(const DenseVector<int, TempLength>& InputArray)
{
	if (m_Type != TypeEnum::Type_IntArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<int>>();
	(*TempPtr) = InputArray;
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_IntArray;
}

template<int_max TempLength>
void JsonValue::operator=(DenseVector<int, TempLength>&& InputArray)
{
	if (m_Type != TypeEnum::Type_IntArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<int>>();
	(*TempPtr) = std::move(InputArray);
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_IntArray;
}

template<int_max TempLength>
void JsonValue::operator=(const DenseVector<long long, TempLength>& InputArray)
{
	if (m_Type != TypeEnum::Type_LongLongArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<long long>>();
	(*TempPtr) = InputArray;
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_LongLongArray;
}

template<int_max TempLength>
void JsonValue::operator=(DenseVector<long long, TempLength>&& InputArray)
{
	if (m_Type != TypeEnum::Type_LongLongArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<long long>>();
	(*TempPtr) = std::move(InputArray);
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_LongLongArray;
}

template<int_max TempLength>
void JsonValue::operator=(const DenseVector<float, TempLength>& InputArray)
{
	if (m_Type != TypeEnum::Type_FloatArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<float>>();
	(*TempPtr) = InputArray;
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_FloatArray;
}

template<int_max TempLength>
void JsonValue::operator=(DenseVector<float, TempLength>&& InputArray)
{
	if (m_Type != TypeEnum::Type_FloatArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<float>>();
	(*TempPtr) = std::move(InputArray);
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_FloatArray;
}

template<int_max TempLength>
void JsonValue::operator=(const DenseVector<double, TempLength>& InputArray)
{
	if (m_Type != TypeEnum::Type_DoubleArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<double>>();
	(*TempPtr) = InputArray;
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_DoubleArray;
}

template<int_max TempLength>
void JsonValue::operator=(DenseVector<double, TempLength>&& InputArray)
{
	if (m_Type != TypeEnum::Type_DoubleArray)
	{
		this->m_OtherData.reset();
	}
	auto TempPtr = std::make_unique<DenseMatrix<double>>();
	(*TempPtr) = std::move(InputArray);
	(*TempPtr).Reshape(1, (*TempPtr).GetElementCount());
	m_OtherData.reset(TempPtr.release());
	m_Type = TypeEnum::Type_DoubleArray;
}

template<typename ScalarType>
ScalarType JsonValue::ToScalar(ScalarType DefaultValue) const
{
	switch (m_Type)
	{
	case TypeEnum::Type_Bool:
		return ScalarType(m_Scalar.Bool);
	case TypeEnum::Type_Int:
		return ScalarType(m_Scalar.Int);
	case TypeEnum::Type_LongLong:
		return ScalarType(m_Scalar.LongLong);
	case TypeEnum::Type_Float:
		return ScalarType(m_Scalar.Float);
	case TypeEnum::Type_Double:
		return ScalarType(m_Scalar.Double);
	default:
		return DefaultValue;
	}
}

template<typename ScalarType>
DenseMatrix<ScalarType> JsonValue::ToScalarArray() const
{
	DenseMatrix<ScalarType> OutputArray;
	switch (m_Type)
	{
	case TypeEnum::Type_IntArray:
		OutputArray.Copy(this->Ref_IntArray());
		break;
	case TypeEnum::Type_LongLongArray:
		OutputArray.Copy(this->Ref_LongLongArray());
		break;
	case TypeEnum::Type_FloatArray:
		OutputArray.Copy(this->Ref_FloatArray());
		break;
	case TypeEnum::Type_DoubleArray:
		OutputArray.Copy(this->Ref_DoubleArray());
	}
	OutputArray.Reshape(1, OutputArray.GetElementCount());
	return OutputArray;
}

template<typename ScalarType>
DenseMatrix<ScalarType> JsonValue::ToScalarArray(const DenseMatrix<ScalarType>& DefaultArray) const
{
	DenseMatrix<ScalarType> OutputArray;
	switch (m_Type)
	{
	case TypeEnum::Type_IntArray:
		OutputArray.Copy(this->Ref_IntArray());
		break;
	case TypeEnum::Type_LongLongArray:
		OutputArray.Copy(this->Ref_LongLongArray());
		break;
	case TypeEnum::Type_FloatArray:
		OutputArray.Copy(this->Ref_FloatArray());
		break;
	case TypeEnum::Type_DoubleArray:
		OutputArray.Copy(this->Ref_DoubleArray());
		break;	
	}
	OutputArray.Reshape(1, OutputArray.GetElementCount());
	return DefaultArray;
}

}//namespace mdk

#endif