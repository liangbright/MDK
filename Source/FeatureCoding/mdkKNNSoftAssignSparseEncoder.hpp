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

    m_NeighbourNumber = -1;
}


template<typename ElementType>
bool KNNSoftAssignSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_NeighbourNumber <= 0)
    {
        MDK_Error("m_NeighbourNumber <= 0  @ KNNSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}



template<typename ElementType>
inline 
void KNNSoftAssignSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                                               SparseMatrix<ElementType>& CodeInSparseColVector)
{
    auto L2DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, m_Dictionary->m_Record);

    auto NeighbourIndexList = FindKNNByDistanceList(m_NeighbourNumber, L2DistanceList);

    auto NeighbourL2DistanceList = L2DistanceList.GetSubMatrix(NeighbourIndexList);

    auto CodeLength = m_Dictionary->m_Record.GetColNumber();

    DenseMatrix<ElementType> Membership(m_NeighbourNumber, 1);

    for (int_max i = 0; i < m_NeighbourNumber; ++i)
    {
        auto s = m_Dictionary->m_StandardDeviation[NeighbourIndexList[i]];

        Membership[i] = std::exp(ElementType(-0.5)*(NeighbourL2DistanceList[i] * NeighbourL2DistanceList[i]) / (s*s))
    }

    CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Membership, CodeLength);
}


}// namespace mdk


#endif