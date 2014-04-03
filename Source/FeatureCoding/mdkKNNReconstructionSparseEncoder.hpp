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

    m_NeighbourNumber = 0;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::SetNeighbourNumber(int_max NeighbourNumber)
{
    if (NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input @ KNNReconstructionSparseEncoder::SetNeighbourNumber(NeighbourNumber)")
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
        MDK_Error("Invalid input NeighbourNumber (<= 0) @ KNNReconstructionSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                                                   SparseMatrix<ElementType>& CodeInSparseVector)
{
    auto L2DistanceList = ComputeL2DistanceListFromSingleVectorToVectorSet(SingleFeatureDataVector, m_Dictionary->m_Record);

    auto NeighbourIndexList = FindKNNByDistanceList(m_NeighbourNumber, L2DistanceList);

    auto SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // solve linear equation using least square method (unconstrained)

    auto Result = SolveLinearLeastSquaresProblem(SubRecord, SingleFeatureDataVector);

    CodeInSparseVector.ConstructColVector(NeighbourIndexList, Result.X, m_Dictionary->m_Record.GetColNumber());
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber = 3,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInDenseMatrix(&OutputCode);

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber = 3,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseMatrix(&OutputCode);;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(std::vector<SparseMatrix<ElementType>>& OutputCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber = 3,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseVectorList(&OutputCode);;

    Encoder->Update();
}

}// namespace mdk


#endif