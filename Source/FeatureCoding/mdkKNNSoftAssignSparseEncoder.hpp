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

    m_Paramter.Clear();
}


template<typename ElementType>
bool KNNSoftAssignSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_Paramter.MaxNumberOfNeighbours <= 0)
    {
        MDK_Error("m_NeighbourNumber <= 0  @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType != "L1Distance"
        && m_Parameter.SimilarityType != "L2Distance"
        && m_Parameter.SimilarityType != "Correlation"
        && m_Parameter.SimilarityType != "KLDivergence")
    {
        MDK_Error("SimilarityType is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType == "L1Distance")
    {
        if (m_Parameter.Sigma_L1 <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->StandardDeviationOfL1Distance().IsEmpty() == false)
            {
                m_Parameter.Sigma_L1 = m_Dictionary->StandardDeviationOfL1Distance().Mean();
            }
            else
            {
                MDK_Error("Sigma_L1 <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.SimilarityType == "L2Distance")
    {
        if (m_Parameter.Sigma_L2 <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->StandardDeviationOfL2Distance().IsEmpty() == false)
            {
                m_Parameter.Sigma_L2 = m_Dictionary->StandardDeviationOfL2Distance().Mean();
            }
            else
            {
                MDK_Error("Sigma_L2 <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.SimilarityType == "Correlation")
    {
        if (m_Parameter.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNSoftAssignSparseEncoder::CheckInput()")                    
        }
    }
    else if (m_Parameter.SimilarityType == "KLDivergence")
    {
        if (m_Parameter.Sigma_KL <= 0)
        {
            // try to find it in Dictionary

            if (m_Dictionary->StandardDeviationOfKLDivergence().IsEmpty() == false)
            {
                m_Parameter.Sigma_KL = m_Dictionary->StandardDeviationOfKLDivergence().Mean();
            }
            else
            {
                MDK_Error("Sigma_KL <= 0 @ KNNSoftAssignSparseEncoder::CheckInput()")
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

    if (m_Parameter.SimilarityType == "L1Distance")
    {
        DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = NeighbourDistanceList[i] / m_Paramter.Sigma_L1;

            auto Similarity = std::exp(ElementType(-0.5)*(temp * temp));
            if (Similarity >= m_Paramter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }            
        }
    }
    else if (m_Parameter.SimilarityType == "L2Distance")
    {
        DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {           
            auto temp = NeighbourDistanceList[i] / m_Paramter.Sigma_L2;

            auto Similarity = std::exp(ElementType(-0.5)*(temp * temp));
            if (Similarity >= m_Paramter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
    else if (m_Parameter.SimilarityType == "Correlation")
    {
        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto Similarity = ElementType(0);

            if (m_Paramter.IgnoreSign_Correlation == true)
            {
                Similarity = std::abs(NeighbourDistanceList[i]); // [-1, 0] => [0, 1]
            }
            else
            {
                Similarity = (NeighbourDistanceList[i] + 1) / 2;   // [-1, 1] => [0, 1]
            }           

            if (Similarity >= m_Paramter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
    else if (m_Parameter.SimilarityType == "KLDivergence")
    {
        DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = NeighbourDistanceList[i] / m_Paramter.Sigma_KL;

            auto Similarity = std::exp(ElementType(-0.5)*(temp * temp));
            if (Similarity >= m_Paramter.SimilarityThreshold)
            {
                SimilarityList[i] = Similarity;
            }
        }
    }
    else
    {
        MDK_Error("DistanceType is invalid @ KNNSoftAssignSparseEncoder::EncodingFunction(...)")
        return;
    }

    // get Membership by normalizing SimilarityList

    auto tempSum = SimilarityList.Sum();

    if (tempSum >= m_Paramter.SimilarityThreshold)
    {
        SimilarityList /= tempSum;
    }
    else
    {
        SimilarityList.Fill(ElementType(0));
    }
   
    CodeInSparseColVector.Construct(NeighbourIndexList, SimilarityList, CodeLength);
}

}// namespace mdk


#endif