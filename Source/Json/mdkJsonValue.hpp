#ifndef mdk_JsonValue_hpp
#define mdk_JsonValue_hpp


namespace mdk
{

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
	OutputArray.Reshape(1, OutputArray.GetElementNumber());
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
	OutputArray.Reshape(1, OutputArray.GetElementNumber());
	return DefaultArray;
}

}//namespace mdk

#endif