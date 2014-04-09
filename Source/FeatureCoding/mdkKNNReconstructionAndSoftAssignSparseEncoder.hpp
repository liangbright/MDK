#ifndef __mdkKNNReconstructionAndSoftAssignSparseEncoder_hpp
#define __mdkKNNReconstructionAndSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::KNNReconstructionAndSoftAssignSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::~KNNReconstructionAndSoftAssignSparseEncoder()
{

}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_ReconstructionEncoder.Clear();

    m_Parameter.Clear();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    m_ReconstructionEncoder.m_Parameter.NeighbourNumber = m_Parameter.NeighbourNumber;
    m_ReconstructionEncoder.m_Parameter.Nonnegative = m_Parameter.Nonnegative;
    m_ReconstructionEncoder.m_Parameter.SumToOne = m_Parameter.SumToOne;

    m_ReconstructionEncoder.SetInputDictionary(m_Dictionary);

    m_ReconstructionEncoder.SetInputFeatureData(m_FeatureData);

    if (m_ReconstructionEncoder.CheckInput() == false)
    {        
        return false;
    }

    if (m_Dictionary->m_StandardDeviation.IsEmpty() == true)
    {
        MDK_Error("Incomplete Dictionary: m_StandardDeviation is empty @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                                                                SparseMatrix<ElementType>& CodeInSparseColVector)
{
    m_ReconstructionEncoder.EncodingFunction(DataColVector, CodeInSparseColVector);

    const std::vector<int_max>& NeighbourIndexList = CodeInSparseColVector.GetRowIndexList();

    const std::vector<ElementType>& Alpha_v = CodeInSparseColVector.GetDataArray();
    DenseMatrix<ElementType> Alpha(const_cast<ElementType*>(Alpha_v.data()), m_Parameter.NeighbourNumber, 1);

    auto SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // compute ReconstructedDataColVector

    auto ReconstructedDataColVector = MatrixMultiply(SubRecord, Alpha);

    auto NeighbourL2DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

    // compute Membership using Reconstructed Data X_hat
    DenseMatrix<ElementType> Membership(m_Parameter.NeighbourNumber, 1);

    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        auto s = m_Dictionary->m_StandardDeviation[NeighbourIndexList[i]];

        Membership[i] = std::exp(ElementType(-0.5)*(NeighbourL2DistanceList[i] * NeighbourL2DistanceList[i]) / (s*s));
    }

    // get reconstruction error ||X-X_hat||
    auto ErrorVector = MatrixSubtract(DataColVector, ReconstructedDataColVector);
    ErrorVector.ElementOperationInPlace("abs");
    auto Error = ErrorVector.Sum();

    // update Membership based on reconstruction error --------------------------------------------
    // if Error is high, then Membership should be low

    // first: get the max StandardDeviation
    ElementType s_max = 0;

    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        s_max += m_Dictionary->m_StandardDeviation[NeighbourIndexList[i]];
    }
    s_max /= m_Parameter.NeighbourNumber;

    // then: update Membership
    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        Membership[i] *= std::exp(ElementType(-0.5)*(NeighbourL2DistanceList[i] * NeighbourL2DistanceList[i]) / (s_max*s_max));
    }

    // set the final code
    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        CodeInSparseColVector.SetElement(NeighbourIndexList[i], Membership[i]);
    }
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionAndSoftAssignSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInDenseMatrix(&OutputCodeInDenseMatrix);

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionAndSoftAssignSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseMatrix(&OutputCodeInSparseMatrix);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionAndSoftAssignSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseColVectorList(&OutputCodeInSparseColVectorList);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}

}// namespace mdk


#endif