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

    if (m_Paramter.NeighbourNumber <= 0)
    {
        MDK_Error("m_NeighbourNumber <= 0  @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.DistanceType != "L1Distance"
        && m_Parameter.DistanceType != "L2Distance"
        && m_Parameter.DistanceType != "Correlation"
        && m_Parameter.DistanceType != "KLDivergence")
    {
        MDK_Error("DistanceType is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}



template<typename ElementType>
inline 
void KNNSoftAssignSparseEncoder<ElementType>::EncodingFunction(SparseVector<ElementType>& CodeInSparseColVector,
                                                               const DenseMatrix<ElementType>& DataColVector,
                                                               int_max ThreadNumber)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType>& ReconstructionStd = m_Dictionary->ReconstructionStd();

    //------

    auto CodeLength = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Membership(m_Parameter.NeighbourNumber, 1);

    DenseMatrix<ElementType> DistanceList;

    if (m_Parameter.DistanceType == "L1Distance")
    {
        DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = ReconstructionStd[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s))
        }

    }
    else if (m_Parameter.DistanceType == "L2Distance")
    {
        DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = ReconstructionStd[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s))
        }
    }
    else if (m_Parameter.DistanceType == "Correlation")
    {
        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            //Membership[i] = std::abs(NeighbourDistanceList[i]); // [-1, 0] => [0, 1]

            Membership[i] = (NeighbourDistanceList[i] + 1) / 2;   // [-1, 1] => [0, 1]
        }
    }
    else if (m_Parameter.DistanceType == "KLDivergence")
    {
        DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        ElementType s = 10;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s))
        }
    }
    else
    {
        MDK_Error("DistanceType is invalid @ KNNSoftAssignSparseEncoder::EncodingFunction(...)")
        return;
    }

    // normalize ???
    Membership /= Membership.L1Norm();

    CodeInSparseColVector.Construct(NeighbourIndexList, Membership, CodeLength);
}


}// namespace mdk


#endif