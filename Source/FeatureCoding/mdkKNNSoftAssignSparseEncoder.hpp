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

    if (m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L1Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L2Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::Correlation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is not supported @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.Variance_L1 <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->VarianceOfL1Distance().IsEmpty() == false)
            {
                m_Parameter.Variance_L1 = m_Dictionary->VarianceOfL1Distance().Mean();
            }
            else
            {
                MDK_Error("Variance_L1 <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_Parameter.Variance_L2 <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->VarianceOfL2Distance().IsEmpty() == false)
            {
                m_Parameter.Variance_L2 = m_Dictionary->VarianceOfL2Distance().Mean();
            }
            else
            {
                MDK_Error("Variance_L2 <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_Parameter.Variance_KL <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->VarianceOfKLDivergence().IsEmpty() == false)
            {
                m_Parameter.Variance_KL = m_Dictionary->VarianceOfKLDivergence().Mean();
            }
            else
            {
                MDK_Error("Variance_KL <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
                return false;
            }
        }
    }

    return true;
}


template<typename ElementType>
inline 
void KNNSoftAssignSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                 m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    auto CodeLength = BasisMatrix.GetColNumber();

    //----------------------------------------------------------------------------------------------------

    DenseMatrix<ElementType> SimilarityList(m_Parameter.NeighbourNumber, 1);
    SimilarityList.Fill(ElementType(0));

    DenseMatrix<int_max> NeighbourIndexList;

    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i] )/ m_Parameter.Variance_L1;

            auto Similarity = std::exp(-temp / ElementType(2));
            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / m_Parameter.Variance_L2;

            auto Similarity = std::exp(-temp / ElementType(2));
            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
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
            auto  Similarity = (NeighbourCorrelationList[i] + 1) / 2;   // [-1, 1] => [0, 1]

            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation :
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(CorrelationList, m_Parameter.NeighbourNumber);

        auto NeighbourCorrelationList = CorrelationList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto Similarity = std::abs(NeighbourCorrelationList[i]); // [-1, 0] => [0, 1]           

            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / m_Parameter.Variance_KL;

            auto Similarity = std::exp(-temp / ElementType(2));

            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
        break;

    default:

        MDK_Error("SimilarityType is not supported @ KNNSoftAssignSparseEncoder::EncodingFunction(...)")
        return;
    }

    // Output Similarity, not membership from 0 to 1
    //auto tempSum = SimilarityList.Sum();
    //
    //if (tempSum >= m_Parameter.SimilarityThreshold)
    //{
    //    SimilarityList /= tempSum;
    //}
    //else
    //{
    //   SimilarityList.Fill(ElementType(0));
    //}
   
    CodeInSparseColVector.Construct(NeighbourIndexList, SimilarityList, CodeLength);
}


template<typename ElementType>
inline
ElementType
KNNSoftAssignSparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType, 
                                   const DenseMatrix<ElementType>& VectorA, 
                                   const DenseMatrix<ElementType>& VectorB, 
                                   ElementType Variance)
{

    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ KNNSoftAssignSparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ KNNSoftAssignSparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    return this->ComputeSimilarityBetweenTwoVectors(SimilarityType, VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, Variance, false);
}


template<typename ElementType>
inline
ElementType 
KNNSoftAssignSparseEncoder<ElementType>::
ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType,
                                   const ElementType* VectorA, const ElementType* VectorB, int_max Length,
                                   ElementType Variance, bool CheckInput)
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
        MDK_Error("unknown type of similarity @ KNNSoftAssignSparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
    }

    return Similarity;
}


}// namespace mdk


#endif