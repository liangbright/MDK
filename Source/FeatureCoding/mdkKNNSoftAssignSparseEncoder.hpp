#ifndef __mdkKNNSoftAssignSparseEncoder_hpp
#define __mdkKNNSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ScalarType>
KNNSoftAssignSparseEncoder<ScalarType>::KNNSoftAssignSparseEncoder()
{
	this->Clear();
}


template<typename ScalarType>
KNNSoftAssignSparseEncoder<ScalarType>::~KNNSoftAssignSparseEncoder()
{
}


template<typename ScalarType>
void KNNSoftAssignSparseEncoder<ScalarType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();
    m_Parameter.Clear();
}


template<typename ScalarType>
bool KNNSoftAssignSparseEncoder<ScalarType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_Parameter.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0  @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    // set input to m_KNNSimilaritySparseEncoder

    m_KNNSimilaritySparseEncoder.m_Parameter.NeighbourNumber = m_Parameter.NeighbourNumber;
    m_KNNSimilaritySparseEncoder.m_Parameter.SimilarityType = m_Parameter.SimilarityType;

    m_KNNSimilaritySparseEncoder.SetInputDictionary(m_Dictionary);
    m_KNNSimilaritySparseEncoder.SetInputFeatureData(m_FeatureData);

    if (m_KNNSimilaritySparseEncoder.CheckInput() == false)
    {
        return false;
    }

    return true;
}


template<typename ScalarType>
inline 
SparseVector<ScalarType> KNNSoftAssignSparseEncoder<ScalarType>::
EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex)
{
    return this->EncodeSingleDataVector(DataColVector);
}


template<typename ScalarType>
inline
SparseVector<ScalarType> KNNSoftAssignSparseEncoder<ScalarType>::
EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector)
{
	auto OutputCode = m_KNNSimilaritySparseEncoder.EncodeSingleDataVector(DataColVector);

    // take reference
	auto& Membership = OutputCode.ElementList();

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    auto tempSum = ScalarType(0);

    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        if (Membership[i] < m_Parameter.SimilarityThreshold)
        {
            Membership[i] = 0;
        }

        Membership[i] += eps_value;

        tempSum += Membership[i];
    }

    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        Membership[i] /= tempSum;
    }

	return OutputCode;
}

//------------------------------------------------------- static function --------------------------------------------------------------------//

template<typename ScalarType>
inline
bool KNNSoftAssignSparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType)
{
    return KNNSimilaritySparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(SimilarityType);
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNSoftAssignSparseEncoder<ScalarType>::
ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
               const DenseMatrix<ScalarType>& KNNBasisMatrix,
               const VectorSimilarityTypeEnum  SimilarityType,
               const ScalarType SimilarityThreshold,
               const DenseMatrix<ScalarType>& VarianceList)
{
    int_max BasisNumber = KNNBasisMatrix.GetColNumber();

    auto Membership = KNNSimilaritySparseEncoder<ScalarType>::ComputeKNNCode(DataColVector, KNNBasisMatrix, SimilarityType, VarianceList);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    auto tempSum = ScalarType(0);

    for (int_max i = 0; i < BasisNumber; ++i)
    {
        if (Membership[i] < SimilarityThreshold)
        {
            Membership[i] = 0;
        }

        Membership[i] += eps_value;

        tempSum += Membership[i];
    }

    for (int_max i = 0; i < BasisNumber; ++i)
    {
        Membership[i] /= tempSum;
    }

    return Membership;
}


template<typename ScalarType>
inline
ScalarType
KNNSoftAssignSparseEncoder<ScalarType>::
ComputeSimilarityBetweenTwoVector(const DenseMatrix<ScalarType>& VectorA, 
                                  const DenseMatrix<ScalarType>& VectorB, 
                                  VectorSimilarityTypeEnum SimilarityType, 
                                  ScalarType Variance)
{
    return KNNSimilaritySparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(VectorA, VectorB, SimilarityType, Variance);
}


template<typename ScalarType>
inline
ScalarType
KNNSoftAssignSparseEncoder<ScalarType>::
ComputeSimilarityBetweenTwoVector(const ScalarType* VectorA, const ScalarType* VectorB, int_max Length,
                                  VectorSimilarityTypeEnum SimilarityType,ScalarType Variance, bool Flag_CheckInput)
{
	return KNNSimilaritySparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(VectorA, VectorB, Length, SimilarityType, Variance, Flag_CheckInput);
}


}// namespace mdk


#endif