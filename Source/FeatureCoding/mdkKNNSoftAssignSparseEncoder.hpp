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
        && m_Parameter.DistanceType != "Correlation")
    {
        MDK_Error("DistanceType is invalid @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}



template<typename ElementType>
inline 
void KNNSoftAssignSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                                               SparseMatrix<ElementType>& CodeInSparseColVector)
{
    auto D = m_Dictionary->BasisMatrix();

    auto CodeLength = D.GetColNumber();

    DenseMatrix<ElementType> Membership(m_Parameter.NeighbourNumber, 1);

    DenseMatrix<ElementType> DistanceList;

    if (m_Parameter.DistanceType == "L1Distance")
    {
        DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, D);
    }
    else if (m_Parameter.DistanceType == "L2Distance")
    {
        DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, D);
    }
    else if (m_Parameter.DistanceType == "Correlation")
    {
        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, D);
    }
    else
    {
        MDK_Error("DistanceType is invalid @ KNNSoftAssignSparseEncoder::EncodingFunction(...)")
        return;
    }

    auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, DistanceList);

    auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

    if (m_Parameter.DistanceType != "Correlation")
    {
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = m_Dictionary->m_StandardDeviation[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s))
        }
    }
    else
    {
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            //Membership[i] = std::abs(NeighbourDistanceList[i]);

            Membership[i] = (NeighbourDistanceList[i] + 1) / 2;
        }
    }

    // normalize ???
    Membership /= Membership.L1Norm();

    CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Membership, CodeLength);
}


}// namespace mdk


#endif