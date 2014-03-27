#ifndef __mdkKNNReconstructionSparseEncoder_hpp
#define __mdkKNNReconstructionSparseEncoder_hpp

//#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
KNNReconstructionSparseEncoder<ElementType>::KNNReconstructionSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
KNNReconstructionSparseEncoder<ElementType>::~KNNReconstructionSparseEncoder()
{

}


template<typename ElementType>
void KNNReconstructionSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_NeighbourNumber = 1;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::SetNeighbourNumber(int64 NeighbourNumber)
{
    if (NeighbourNumber <= 0)
    {
        MDK_Error << "Invalid input @ KNNReconstructionSparseEncoder::SetNeighbourNumber(NeighbourNumber)" << '\n';
        return false;
    }

    m_NeighbourNumber = NeighbourNumber;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput() == false)
    {
        return false;
    }

    if (m_NeighbourNumber <= 0)
    {
        MDK_Error << "Invalid input NeighbourNumber (<= 0) @ KNNReconstructionSparseEncoder::CheckInputAndOutput()" << '\n';
        return false;
    }

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::EncodingFunction(int64 IndexOfFeatureVector)
{
    auto FeatureVector = m_FeatureData.GetCol(IndexOfFeatureVector);

    auto L2DistanceList = ComputeListOfL2DistanceFromOneFeatureToFeatureDictionary(FeatureVector, m_Dictionary->m_Record);

    auto NeighbourIndexList = FindKNNFromDistanceList(m_NeighbourNumber, L2DistanceList);

    DenseMatrix<ElementType> SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // solve linear equation using least square method


    return true;
}


}// namespace mdk


#endif