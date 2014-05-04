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

    if (m_Parameter.SimilarityType != SimilarityTypeEnum::L1Distance
        && m_Parameter.SimilarityType != SimilarityTypeEnum::L2Distance
        && m_Parameter.SimilarityType != SimilarityTypeEnum::Correlation
        && m_Parameter.SimilarityType != SimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType == SimilarityTypeEnum::L1Distance)
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
    else if (m_Parameter.SimilarityType == SimilarityTypeEnum::L2Distance)
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
    else if (m_Parameter.SimilarityType == SimilarityTypeEnum::Correlation)
    {
        if (m_Parameter.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNSoftAssignSparseEncoder::CheckInput()")                    
        }
    }
    else if (m_Parameter.SimilarityType == SimilarityTypeEnum::KLDivergence)
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

    DenseMatrix<ElementType> DistanceList;

    DenseMatrix<int_max> NeighbourIndexList;

    switch (m_Parameter.SimilarityType)
    {
    case SimilarityTypeEnum::L1Distance:
    {
        DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

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

    case SimilarityTypeEnum::L2Distance:
    {
        DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

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

    case SimilarityTypeEnum::Correlation:
    {
        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto Similarity = ElementType(0);

            if (m_Parameter.IgnoreSign_Correlation == true)
            {
                Similarity = std::abs(NeighbourDistanceList[i]); // [-1, 0] => [0, 1]
            }
            else
            {
                Similarity = (NeighbourDistanceList[i] + 1) / 2;   // [-1, 1] => [0, 1]
            }

            if (Similarity >= m_Parameter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
        break;

    case SimilarityTypeEnum::KLDivergence:
    {
        DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

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

        MDK_Error("SimilarityType is invalid @ KNNSoftAssignSparseEncoder::EncodingFunction(...)")
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

}// namespace mdk


#endif