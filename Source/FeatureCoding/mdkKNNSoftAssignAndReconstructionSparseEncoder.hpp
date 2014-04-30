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

    m_Parameter.Clear();
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::SetupDefaultPipelineOutput()
{
    this->FeatureDictionaryBasedSparseEncoder::SetupDefaultPipelineOutput();

    m_ReconstructionErrorNorm_SharedCopy.Clear();

    m_ReconstructionErrorNorm = &m_ReconstructionErrorNorm_SharedCopy;
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::UpdatePipelineOutput()
{
    this->FeatureDictionaryBasedSparseEncoder::UpdatePipelineOutput();

    if (m_ReconstructionErrorNorm != &m_ReconstructionErrorNorm_SharedCopy)
    {
        m_ReconstructionErrorNorm_SharedCopy.ForceShare(m_ReconstructionErrorNorm);
    }
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputFeatureData(FeatureData);
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputDictionary(Dictionary);
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (this->UpdateInputOfReconstructionEncoder() == false)
    {
        MDK_Error("Invalid Parameter for KNNReconstruction @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.DistanceTypeForSoftAssign != "L1Distance"
        && m_Parameter.DistanceTypeForSoftAssign != "L2Distance"
        && m_Parameter.DistanceTypeForSoftAssign != "Correlation"
        && m_Parameter.DistanceTypeForSoftAssign != "KLDivergence")
    {
        MDK_Error("DistanceTypeForSoftAssign is invalid @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }
  
    if (m_Dictionary->StandardDeviation().IsEmpty() == true)
    {
        MDK_Error("StandardDeviation in Dictionary is empty @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    auto Size = m_Dictionary->GetSize();

    m_ReconstructionErrorNorm->FastResize(1, Size.ColNumber);

    return true;
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Preprocess()
{
    this->FeatureDictionaryBasedSparseEncoder::Preprocess();

    this->UpdateInputOfReconstructionEncoder();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Postprocess()
{
    this->FeatureDictionaryBasedSparseEncoder::Postprocess();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::UpdateInputOfReconstructionEncoder()
{
    m_ReconstructionEncoder.m_Parameter.NeighbourNumber = m_Parameter.NeighbourNumber;
    m_ReconstructionEncoder.m_Parameter.DistanceTypeForKNNSearch = m_Parameter.DistanceTypeForKNNSearch;
    m_ReconstructionEncoder.m_Parameter.Nonnegative = m_Parameter.Nonnegative;
    m_ReconstructionEncoder.m_Parameter.SumToOne = m_Parameter.SumToOne;

    m_ReconstructionEncoder.SetInputFeatureData(m_FeatureData);
    m_ReconstructionEncoder.SetInputDictionary(m_Dictionary);

    if (m_ReconstructionEncoder.CheckInput() == false)
    {
        return false;
    }

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType>& StandardDeviation = m_Dictionary->StandardDeviation();

    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                 m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    //-------------------------------------------------------------------------------------------------------------

    m_ReconstructionEncoder.EncodeSingleDataVector(CodeInSparseColVector, DataColVector);

    auto ErrorNormVectorPtr = m_ReconstructionEncoder.GetOutputReconstructionErrorNorm();

    auto ReconstructionErrorNorm = (*ErrorNormVectorPtr)[0];

    // compute ReconstructedDataColVector "X_hat"

    const std::vector<int_max>& NeighbourIndexList = CodeInSparseColVector.IndexList();

    auto SubRecord = BasisMatrix.GetSubMatrix(ALL, NeighbourIndexList);

    DenseMatrix<ElementType> Alpha(CodeInSparseColVector.DataArray().data(), m_Parameter.NeighbourNumber, 1);

    auto ReconstructedDataColVector = MatrixMultiply(SubRecord, Alpha);

    if (m_Parameter.DistanceTypeForSoftAssign == "KLDivergence")
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

        if (temp_sum > ElementType(1) / ElementType(temp_Length))
        {
            ReconstructedDataColVector /= temp_sum;
        }
    }

    //------------------------------------------------------------

    // compute Membership in [0, 1] using ReconstructedDataColVector "X_hat" (not X) --------------------------------------
    DenseMatrix<ElementType> Membership(m_Parameter.NeighbourNumber, 1);

    if (m_Parameter.DistanceTypeForSoftAssign == "L1Distance")
    {
        auto NeighbourDistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = StandardDeviation[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s));
        }
    }
    else if (m_Parameter.DistanceTypeForSoftAssign == "L2Distance")
    {
        auto NeighbourDistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = StandardDeviation[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s));
        }
    }
    else if (m_Parameter.DistanceTypeForSoftAssign == "Correlation")
    {
        auto NeighbourCorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {            
            Membership[i] = std::abs(NeighbourCorrelationList[i]);

            //Membership[i] = (NeighbourCorrelationList[i] + 1) / 2;
        }
    }
    else if (m_Parameter.DistanceTypeForSoftAssign == "KLDivergence")
    {
        auto NeighbourKLDivergenceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(ReconstructedDataColVector, SubRecord);

        ElementType s = 10;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourKLDivergenceList[i] * NeighbourKLDivergenceList[i]) / (s*s));
        }
    }
    else
    {
        MDK_Error("DistanceType is invalid @ KNNReconstructionAndSoftAssignSparseEncoder::EncodingFunction(...)")
        return;
    }

    // normalize (sum to 1) ???
    auto eps_value = std::numeric_limits<ElementType>::epsilon();
    auto L1Norm_value = Membership.Sum();
    auto temp_value = ElementType(1) / ElementType(m_Parameter.NeighbourNumber);
    if (L1Norm_value >= eps_value*temp_value)
    {
        Membership /= L1Norm_value;
    }
    else
    {
        Membership.Fill(temp_value);
    }
    
    // update Membership based on reconstruction error

    // first: get the max StandardDeviation in the neighborhood
    auto s_max = ElementType(0);

    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        auto s_temp = StandardDeviation[NeighbourIndexList[i]];

        s_max = (std::max)(s_max, s_temp); // std::max(s_max, s_temp) can not be compiled
    }
    // second: compute ReconstructionScore in [0, 1]
    auto ReconstructionScore = std::exp(ElementType(-0.5)*(ReconstructionErrorNorm *ReconstructionErrorNorm) / (s_max*s_max));

    // then: update Membership
    Membership *= ReconstructionScore;


    // set the final code    
    for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
    {
        CodeInSparseColVector.SetElement(NeighbourIndexList[i], Membership[i]);
    }
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::PreprocessBeforeUsing_EncodeSingleDataVector()
{
    this->CheckInput();

    this->Preprocess();
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::PostprocessAfterUsing_EncodeSingleDataVector()
{
    this->Postprocess();
}


template<typename ElementType>
inline 
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
                                                                                      const DenseMatrix<ElementType>& DataColVector)
{
    this->SetInputFeatureData(&DataColVector);

    this->EncodingFunction(0, 0);

    CodeInSparseColVector = std::move((*m_CodeInSparseColVectorSet)[0]);
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
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Apply(DenseMatrix<SparseVector<ElementType>>& OutputCodeInSparseColVectorSet,
                                                                     const DenseMatrix<ElementType>* FeatureData,
                                                                     const FeatureDictionary<ElementType>* Dictionary,
                                                                     int_max NeighbourNumber,
                                                                     int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionAndSoftAssignSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseColVectorSet(&OutputCodeInSparseColVectorSet);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}

}// namespace mdk


#endif