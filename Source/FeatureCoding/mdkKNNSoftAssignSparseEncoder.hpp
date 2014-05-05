#ifndef __mdkKNNSoftAssignSparseEncoder_hpp
#define __mdkKNNSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
KNNSoftAssignSparseEncoder<ElementType>::KNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
KNNSoftAssignSparseEncoder<ElementType>::~KNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
void KNNSoftAssignSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_Parameter.Clear();
}


template<typename ElementType>
bool KNNSoftAssignSparseEncoder<ElementType>::CheckInput()
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


template<typename ElementType>
inline 
void KNNSoftAssignSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                  m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    //----------------------------------------------------------------------------------------

    this->EncodeSingleDataVector(CodeInSparseColVector, DataColVector);
}


template<typename ElementType>
inline
void KNNSoftAssignSparseEncoder<ElementType>::
EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
                       const DenseMatrix<ElementType>& DataColVector)
{
    m_KNNSimilaritySparseEncoder.EncodeSingleDataVector(CodeInSparseColVector, DataColVector);

    // take reference
    std::vector<ElementType>& Membership = CodeInSparseColVector.DataArray();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto tempSum = ElementType(0);

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
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNSoftAssignSparseEncoder<ElementType>::
ComputeCodeVector(const DenseMatrix<ElementType>& DataColVector,
                  const DenseMatrix<ElementType>& KNNBasisMatrix,
                  const VectorSimilarityTypeEnum  SimilarityType,
                  const ElementType SimilarityThreshold,
                  const DenseMatrix<ElementType>& VarianceList)
{
    int_max BasisNumber = KNNBasisMatrix.GetColNumber();

    auto Membership = KNNSimilaritySparseEncoder<ElementType>::ComputeCodeVector(DataColVector, KNNBasisMatrix, SimilarityType, VarianceList);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto tempSum = ElementType(0);

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
}


template<typename ElementType>
inline
ElementType
KNNSoftAssignSparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, 
                                   const DenseMatrix<ElementType>& VectorB, 
                                   VectorSimilarityTypeEnum SimilarityType, 
                                   ElementType Variance)
{
    return KNNSimilaritySparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorA, VectorB, SimilarityType, Variance);
}


template<typename ElementType>
inline
ElementType 
KNNSoftAssignSparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length,
                                   VectorSimilarityTypeEnum SimilarityType,ElementType Variance, bool CheckInput)
{
    return KNNSimilaritySparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(VectorA, VectorB, Length, SimilarityType, Variance, CheckInput);
}


}// namespace mdk


#endif