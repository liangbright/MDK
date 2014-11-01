#ifndef __mdkKNNSoftAssignAndReconstructionSparseEncoder_hpp
#define __mdkKNNSoftAssignAndReconstructionSparseEncoder_hpp

namespace mdk
{

template<typename ScalarType>
KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::KNNSoftAssignAndReconstructionSparseEncoder()
{
    this->Clear();
}


template<typename ScalarType>
KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::~KNNSoftAssignAndReconstructionSparseEncoder()
{
}


template<typename ScalarType>
void KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();
    m_Parameter.Clear();
}


template<typename ScalarType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::CheckInput()
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


template<typename ScalarType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::ComputeGramianMatrix_DtD()
{
    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNSoftAssignAndReconstructionSparseEncoder::ComputeGramianMatrix_DtD()")
        return false;
    }

    const DenseMatrix<ScalarType>& D = m_Dictionary->BasisMatrix(); // "auto  D = " will copy

    m_GramianMatrix_DtD = D.Transpose()*D;

    return true;
}


template<typename ScalarType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::Preprocess()
{
	if (this->FeatureDictionaryBasedSparseEncoder::Preprocess() == false)
	{
		return false;
	}
    this->ComputeGramianMatrix_DtD();
    return true;
}


template<typename ScalarType>
bool KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::Postprocess()
{
	if (this->FeatureDictionaryBasedSparseEncoder::Postprocess() == false)
	{
		return false;
	}
    m_GramianMatrix_DtD.Clear();
    return true;
}

template<typename ScalarType>
inline
SparseVector<ScalarType> KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::
EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex)
{
	return this->EncodeSingleDataVector(DataColVector);
}


template<typename ScalarType>
inline
SparseVector<ScalarType> KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::
EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector)
{
    const auto& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy
	auto BasisNumber = BasisMatrix.GetColNumber();
	const auto& VarianceOfReconstruction = m_Dictionary->VarianceOfReconstruction();

    //----------------------------------- perform Soft Assign ---------------------------------------------------------

	auto SoftAssignCode = m_KNNSoftAssignSparseEncoder.EncodeSingleDataVector(DataColVector);
	DenseMatrix<int_max> KNNBasisIndexList = SoftAssignCode.IndexList();
	DenseMatrix<ScalarType> KNNSimilarityList = SoftAssignCode.ElementList();
    int_max KNNBasisNumber = KNNBasisIndexList.GetElementNumber();

    //-------------------------------- compute ReconstructedDataColVector "X_hat" ----------------------------------------

    auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNNBasisIndexList);

    auto CodeVector = KNNReconstructionSparseEncoder<ScalarType>::ComputeKNNCode(DataColVector, 
                                                                                 KNNBasisMatrix, 
                                                                                 KNNBasisIndexList,
                                                                                 m_GramianMatrix_DtD, 
                                                                                 m_Parameter.CodePositive,
                                                                                 m_Parameter.CodeSumToOne);

    auto ReconstructedDataColVector = this->ReconstructDataColVector(KNNBasisMatrix, CodeVector);

    // compute ReconstructionErrorSquaredL2Norm

    auto ReconstructionErrorSquaredL2Norm = ComputeSquaredL2DistanceBetweenTwoVector(DataColVector, ReconstructedDataColVector);

    // compute Membership in [0, 1] using ReconstructedDataColVector
    
    auto Membership = this->ComputeMembershipUsingReconstructedDataColVector(ReconstructedDataColVector, KNNBasisMatrix);
    
    // update Membership based on reconstruction error

    // first: get the mean VarianceOfReconstruction in the neighborhood
    auto Variance = ScalarType(0);

    for (int_max i = 0; i < KNNBasisNumber; ++i)
    {
        Variance += VarianceOfReconstruction[KNNBasisIndexList[i]];
    }

    Variance /= KNNBasisNumber;

    // second: compute ReconstructionScore in [0, 1]
    auto ReconstructionScore = std::exp(ScalarType(-0.5)*ReconstructionErrorSquaredL2Norm / Variance);

    // then: update Membership
    Membership *= ReconstructionScore;

    // create the final code    
	SparseVector<ScalarType> OutputCode;
	OutputCode.Construct(KNNBasisIndexList, Membership, BasisNumber);
	return OutputCode;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType> 
KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::
ReconstructDataColVector(const DenseMatrix<ScalarType>& KNNBasisMatrix,
                         const DenseMatrix<ScalarType>& CodeVector)
{
    auto ReconstructedDataColVector = MatrixMultiply(KNNBasisMatrix, CodeVector);

    if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        auto temp_sum = ScalarType(0);

        auto temp_Length = ReconstructedDataColVector.GetElementNumber();

        for (int_max k = 0; k < temp_Length; ++k)
        {
            if (ReconstructedDataColVector[k] < ScalarType(0))
            {
                ReconstructedDataColVector[k] = ScalarType(0);
            }
            else
            {
                temp_sum += ReconstructedDataColVector[k];
            }
        }

        auto temp_Value = ScalarType(1) / ScalarType(temp_Length);

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


template<typename ScalarType>
inline 
DenseMatrix<ScalarType> 
KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::
ComputeMembershipUsingReconstructedDataColVector(const DenseMatrix<ScalarType>& ReconstructedDataColVector,
                                                 const DenseMatrix<ScalarType>& KNNBasisMatrix)
{
    const DenseMatrix<ScalarType>& VarianceOfL1Distance   = m_Dictionary->VarianceOfL1Distance();
    const DenseMatrix<ScalarType>& VarianceOfL2Distance   = m_Dictionary->VarianceOfL2Distance();
    const DenseMatrix<ScalarType>& VarianceOfKLDivergence = m_Dictionary->VarianceOfKLDivergence();
    
    DenseMatrix<ScalarType> VarianceList;
    
    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
        VarianceList.ForceShare(VarianceOfL1Distance);
        break;

    case VectorSimilarityTypeEnum::L2Distance:
        VarianceList.ForceShare(VarianceOfL2Distance);
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
        VarianceList.ForceShare(VarianceOfKLDivergence);
        break;

    default:
        break;
    }

    return KNNSoftAssignSparseEncoder<ScalarType>::ComputeKNNCode(ReconstructedDataColVector, 
                                                                   KNNBasisMatrix,
                                                                   m_Parameter.SimilarityType,
                                                                   m_Parameter.SimilarityThreshold,
                                                                   VarianceList);
  
}



template<typename ScalarType>
DataArray<SparseVector<ScalarType>> 
KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>::Apply(const DenseMatrix<ScalarType>* FeatureData,
                                                               const FeatureDictionary<ScalarType>* Dictionary,
                                                               int_max NeighbourNumber,
                                                               int_max MaxNumberOfThread)
{
    auto Encoder = std::make_unique<KNNSoftAssignAndReconstructionSparseEncoder<ScalarType>>();

    Encoder->SetInputFeatureData(FeatureData);
    Encoder->SetInputDictionary(Dictionary);
	Encoder->SetMaxNumberOfThread(MaxNumberOfThread);
    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;
    Encoder->Update();
	return *Encoder->GetOutputCode();
}

}// namespace mdk


#endif