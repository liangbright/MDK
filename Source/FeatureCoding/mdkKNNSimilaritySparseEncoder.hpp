#ifndef __mdkKNNSimilaritySparseEncoder_hpp
#define __mdkKNNSimilaritySparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
KNNSimilaritySparseEncoder<ElementType>::KNNSimilaritySparseEncoder()
{

}


template<typename ElementType>
KNNSimilaritySparseEncoder<ElementType>::~KNNSimilaritySparseEncoder()
{

}


template<typename ElementType>
void KNNSimilaritySparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_Parameter.Clear();
}


template<typename ElementType>
bool KNNSimilaritySparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_Parameter.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0  @ KNNSimilaritySparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L1Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L2Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::Correlation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Dictionary->VarianceOfL1Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL1Distance in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_Dictionary->VarianceOfL2Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL2Distance in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_Dictionary->VarianceOfKLDivergence().IsEmpty() == true)
        {
            MDK_Error("VarianceOfKLDivergence in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }

    return true;
}


template<typename ElementType>
inline 
void KNNSimilaritySparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                 m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    //----------------------------------------------------------------------------------------------------

    this->EncodeSingleDataVector(CodeInSparseColVector, DataColVector);
}


template<typename ElementType>
inline
void KNNSimilaritySparseEncoder<ElementType>::
EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
const DenseMatrix<ElementType>& DataColVector)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType>& VarianceOfL1Distance = m_Dictionary->VarianceOfL1Distance();
    const DenseMatrix<ElementType>& VarianceOfL2Distance = m_Dictionary->VarianceOfL2Distance();
    const DenseMatrix<ElementType>& VarianceOfKLDivergence = m_Dictionary->VarianceOfKLDivergence();

    auto CodeLength = BasisMatrix.GetColNumber();

    //----------------------------------------------------------------------------------------------------

    DenseMatrix<ElementType> SimilarityList(1, m_Parameter.NeighbourNumber);

    DenseMatrix<int_max> NeighbourIndexList(1, m_Parameter.NeighbourNumber);

    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        // calculate mean variance

        auto Variance = ElementType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfL1Distance[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        auto Variance = ElementType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfL2Distance[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(CorrelationList, m_Parameter.NeighbourNumber);

        auto NeighbourCorrelationList = CorrelationList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            SimilarityList[i] = (NeighbourCorrelationList[i] + 1) / 2;   // [-1, 1] => [0, 1]
        }
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(CorrelationList, m_Parameter.NeighbourNumber);

        auto NeighbourCorrelationList = CorrelationList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            SimilarityList[i] = std::abs(NeighbourCorrelationList[i]); // [-1, 0] => [0, 1]           
        }
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        auto Variance = ElementType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfKLDivergence[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    default:

        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::EncodeSingleDataVector(...)")
        return;
    }

    CodeInSparseColVector.Construct(NeighbourIndexList, SimilarityList, CodeLength);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
KNNSimilaritySparseEncoder<ElementType>::
ComputeCodeVector(const DenseMatrix<ElementType>& DataColVector,
                  const DenseMatrix<ElementType>& KNNBasisMatrix,
                  const VectorSimilarityTypeEnum  SimilarityType,
                  const DenseMatrix<ElementType>& VarianceList)
{
    int_max KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    DenseMatrix<ElementType> SimilarityList(1, KNNBasisNumber);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, KNNBasisMatrix);

        // calculate mean variance

        auto Variance = ElementType(0);
        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            Variance += VarianceList[i];
        }
        Variance /= KNNBasisNumber;

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto Variance = ElementType(0);
        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            Variance += VarianceList[i];
        }
        Variance /= VarianceList;

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            SimilarityList[i] = (CorrelationList[i] + 1) / 2;   // [-1, 1] => [0, 1]
        }
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            SimilarityList[i] = std::abs(CorrelationList[i]); // [-1, 0] => [0, 1]           
        }
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        auto Variance = ElementType(0);
        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            Variance += VarianceList[i];
        }
        Variance /= KNNBasisNumber;

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp / ElementType(2));
        }
    }
        break;

    default:

        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::ComputeCodeVector(...)")
        return;
    }

    return SimilarityList;
}


template<typename ElementType>
inline
ElementType
KNNSimilaritySparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, 
                                   const DenseMatrix<ElementType>& VectorB, 
                                   VectorSimilarityTypeEnum SimilarityType, 
                                   ElementType Variance)
{

    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    return this->ComputeSimilarityBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, SimilarityType, Variance, false);
}


template<typename ElementType>
inline
ElementType 
KNNSimilaritySparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length,
                                   VectorSimilarityTypeEnum SimilarityType, ElementType Variance, bool CheckInput)
{
    ElementType Similarity = ElementType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, CheckInput);
        auto temp = (L1Distance*L1Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, CheckInput);
        auto temp = (L2Distance*L2Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::UncenteredCorrelation:
    {
        auto Correlation = ComputeUncenteredCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUncenteredCorrelation:
    {
        auto Correlation = ComputeUncenteredCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::UnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, CheckInput);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, CheckInput);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = (KLDivergence*KLDivergence) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
    }

    return Similarity;
}


}// namespace mdk


#endif