#ifndef __mdkKNNSoftAssignAndReconstructionSparseEncoder_hpp
#define __mdkKNNSoftAssignAndReconstructionSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::KNNSoftAssignAndReconstructionSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::~KNNSoftAssignAndReconstructionSparseEncoder()
{

}


template<typename ElementType>
void KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_Parameter.Clear();
}


template<typename ElementType>
void KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputFeatureData(FeatureData);
}


template<typename ElementType>
void KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputDictionary(Dictionary);
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }
 
    // set input to m_KNNSoftAssignSparseEncoder

    m_KNNSoftAssignSparseEncoder.m_Parameter.NeighbourNumber = m_Parameter.NeighbourNumber;
    m_KNNSoftAssignSparseEncoder.m_Parameter.SimilarityType = m_Parameter.SimilarityType;
    m_KNNSoftAssignSparseEncoder.m_Parameter.SimilarityThreshold = m_Parameter.SimilarityThreshold;
    m_KNNSoftAssignSparseEncoder.SetInputDictionary(m_Dictionary);
    m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);

    if (m_KNNSoftAssignSparseEncoder.CheckInput() == false)
    {
        return false;
    }

    return true;
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::ComputeGramianMatrix_DtD()
{
    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNSoftAssignAndReconstructionSparseEncoder::ComputeGramianMatrix_DtD()")
        return false;
    }

    const DenseMatrix<ElementType>& D = m_Dictionary->BasisMatrix(); // "auto  D = " will copy

    m_GramianMatrix_DtD = D.Transpose()*D;

    return true;
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Preprocess()
{
    this->ComputeGramianMatrix_DtD();

    return true;
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Postprocess()
{
    m_GramianMatrix_DtD.Clear();

    return true;
}


template<typename ElementType>
inline
void KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                 m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    //----------------------------------- perform Soft Assign ---------------------------------------------------------

    SparseVector<ElementType> SoftAssignCodeInSparseVector;

    m_KNNSoftAssignSparseEncoder.EncodeSingleDataVector(SoftAssignCodeInSparseVector, DataColVector);

    DenseMatrix<int_max> KNNBasisIndexList = SoftAssignCodeInSparseVector.IndexList();

    DenseMatrix<ElementType> KNNSimilarityList = SoftAssignCodeInSparseVector.DataArray();

    int_max KNNBasisNumber = KNNBasisIndexList.GetElementNumber();

    //-------------------------------- compute ReconstructedDataColVector "X_hat" ----------------------------------------

    auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNNBasisIndexList);

    auto CodeVector = KNNReconstructionSparseEncoder<ElementType>::ComputeCodeVector(DataColVector, 
                                                                                     KNNBasisMatrix, 
                                                                                     KNNBasisIndexList,
                                                                                     m_GramianMatrix_DtD, 
                                                                                     m_Parameter.CodeNonnegative,
                                                                                     m_Parameter.CodeSumToOne);

    auto ReconstructedDataColVector = this->ReconstructDataColVector(KNNBasisMatrix, CodeVector);

    // compute ReconstructionErrorSquaredL2Norm

    auto ReconstructionErrorSquaredL2Norm = ComputeSquaredL2DistanceBetweenTwoVectors(DataColVector, ReconstructedDataColVector);

    // compute Membership in [0, 1] using ReconstructedDataColVector
    
    auto Membership = this->ComputeMembershipUsingReconstructedDataColVector(ReconstructedDataColVector, KNNBasisMatrix);
    
    // update Membership based on reconstruction error

    // first: get the mean VarianceOfReconstruction in the neighborhood
    auto Variance = ElementType(0);

    for (int_max i = 0; i < KNNBasisNumber; ++i)
    {
        Variance += VarianceOfReconstruction[KNNBasisIndexList[i]];
    }

    Variance /= KNNBasisNumber;

    // second: compute ReconstructionScore in [0, 1]
    auto ReconstructionScore = std::exp(ElementType(-0.5)*ReconstructionErrorSquaredL2Norm / Variance);

    // then: update Membership
    Membership *= ReconstructionScore;

    // create the final code    
    CodeInSparseColVector.Construct(KNNBasisIndexList, Membership, KNNBasisNumber);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::
ReconstructDataColVector(const DenseMatrix<ElementType>& KNNBasisMatrix,
                         const DenseMatrix<ElementType>& CodeVector)
{
    auto ReconstructedDataColVector = MatrixMultiply(KNNBasisMatrix, CodeVector);

    if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        auto temp_sum = ElementType(0);

        auto temp_Length = ReconstructedDataColVector.GetElementNumber();

        for (int_max k = 0; k < temp_Length; ++k)
        {
            if (ReconstructedDataColVector[k] < ElementType(0))
            {
                ReconstructedDataColVector[k] = ElementType(0);
            }
            else
            {
                temp_sum += ReconstructedDataColVector[k];
            }
        }

        auto temp_Value = ElementType(1) / ElementType(temp_Length);

        if (temp_sum >= temp_Value)
        {
            ReconstructedDataColVector /= temp_sum;
        }
        else
        {
            ReconstructedDataColVector.Fill(temp_Value);
        }
    }

    return ReconstructedDataColVector;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::
ComputeMembershipUsingReconstructedDataColVector(const DenseMatrix<ElementType>& ReconstructedDataColVector,
                                                 const DenseMatrix<ElementType>& KNNBasisMatrix)
{

    const DenseMatrix<ElementType>& VarianceOfReconstruction = m_Dictionary->VarianceOfReconstruction();
    
    DenseMatrix<ElementType> VarianceList;
    
    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
        VarianceList.ForceShare(VarianceOfL1Distance);
        break;

    case VectorSimilarityTypeEnum::L2Distance:
        VarianceList.ForceShare(VarianceOfL2Distance);
        break;

    case VectorSimilarityTypeEnum::KLDivergence
        VarianceList.ForceShare(VarianceOfKLDivergence);
        break;

    default:
        break;
    }

    return KNNSoftAssignSparseEncoder<ElementType>::ComputeCodeVector(ReconstructedDataColVector, KNNBasisMatrix,
                                                                      m_Parameter.SimilarityType,
                                                                      m_Parameter.SimilarityThreshold,
                                                                      VarianceList);
  
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNSoftAssignAndReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInDenseMatrix(&OutputCodeInDenseMatrix);

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNSoftAssignAndReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseMatrix(&OutputCodeInSparseMatrix);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}


template<typename ElementType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<SparseVector<ElementType>>& OutputCodeInSparseColVectorSet,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNSoftAssignAndReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseColVectorSet(&OutputCodeInSparseColVectorSet);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}

}// namespace mdk


#endif