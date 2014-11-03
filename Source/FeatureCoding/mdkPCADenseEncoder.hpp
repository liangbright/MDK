#ifndef __mdkPCADenseEncoder_hpp
#define __mdkPCADenseEncoder_hpp

namespace mdk
{

template<typename ScalarType>
PCADenseEncoder<ScalarType>::PCADenseEncoder()
{
}


template<typename ScalarType>
PCADenseEncoder<ScalarType>::~PCADenseEncoder()
{
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType> PCADenseEncoder<ScalarType>::
EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex)
{
	return this->EncodeSingleDataVector(DataColVector);
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType> PCADenseEncoder<ScalarType>::
EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector)
{
    const auto& D = m_Dictionary->BasisMatrix(); // "auto D =" will copy

	auto CodeLength = D.GetColNumber();
	auto DataDimension = DataColVector.GetElementNumber();

	DenseMatrix<ScalarType> tempData(DataDimension, 1);
	for (int_max i = 0; i < DataDimension; ++i)
	{
		tempData[i] = DataColVector[i] - m_Dictionary->m_MeanVector[i];
	}

	DenseMatrix<ScalarType> OutputCode(CodeLength, 1);
    for (int_max j = 0; j < CodeLength; ++i)
    {
        auto temp = ScalarType(0);
        auto dj = D->GetElementPointerOfCol(j);
        for (int_max i = 0; i < DataDimension; ++i)
        {
            temp += tempData[i] * dj[i];
        }
		OutputCode[j] = temp;
    }

	return OutputCode;
}



}// namespace mdk


#endif