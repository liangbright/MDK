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
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputFeatureData(FeatureData);

    m_ReconstructionEncoder.SetInputFeatureData(m_FeatureData);
}


template<typename ElementType>
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary)
{
    this->FeatureDictionaryBasedSparseEncoder::SetInputDictionary(Dictionary);

    m_ReconstructionEncoder.SetInputDictionary(Dictionary);
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (this->UpdateParameterForKNNReconstruction() == false)
    {
        MDK_Error("Invalid Parameter for KNNReconstruction @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }
  
    if (m_Dictionary->ReconstructionStd().IsEmpty() == true)
    {
        MDK_Error("Incomplete Dictionary: ReconstructionStd is empty @ KNNReconstructionAndSoftAssignSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::Preprocess()
{
    return this->UpdateParameterForKNNReconstruction();
}


template<typename ElementType>
bool KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::UpdateParameterForKNNReconstruction()
{
    m_ReconstructionEncoder.m_Parameter.NeighbourNumber = m_Parameter.NeighbourNumber;
    m_ReconstructionEncoder.m_Parameter.DistanceTypeForKNNSearch = m_Parameter.DistanceTypeForKNNSearch;
    m_ReconstructionEncoder.m_Parameter.Nonnegative = m_Parameter.Nonnegative;
    m_ReconstructionEncoder.m_Parameter.SumToOne = m_Parameter.SumToOne;

    if (m_ReconstructionEncoder.CheckInput() == false)
    {
        return false;
    }

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionAndSoftAssignSparseEncoder<ElementType>::EncodingFunction(SparseVector<ElementType>& CodeInSparseColVector,
                                                                                const DenseMatrix<ElementType>& DataColVector,
                                                                                int_max ThreadIndex)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType>& ReconstructionStd = m_Dictionary->ReconstructionStd();

    //---------

    m_ReconstructionEncoder.EncodingFunction(CodeInSparseColVector, DataColVector, ThreadIndex);

    const std::vector<int_max>& NeighbourIndexList = CodeInSparseColVector.IndexList();

    const std::vector<ElementType>& Alpha_v = CodeInSparseColVector.DataArray();
    DenseMatrix<ElementType> Alpha(const_cast<ElementType*>(Alpha_v.data()), m_Parameter.NeighbourNumber, 1);

    auto SubRecord = BasisMatrix.GetSubMatrix(ALL, NeighbourIndexList);

    // compute ReconstructedDataColVector X_hat
    auto ReconstructedDataColVector = MatrixMultiply(SubRecord, Alpha);

    // get reconstruction error ||X-X_hat||
    auto ErrorVector = MatrixSubtract(DataColVector, ReconstructedDataColVector);
    auto ReconstructionErrorL2Norm = ErrorVector.L2Norm();

    // compute Membership in [0, 1] using Reconstructed Data X_hat (not X) --------------------------------------
    DenseMatrix<ElementType> Membership(m_Parameter.NeighbourNumber, 1);

    if (m_Parameter.DistanceTypeForSoftAssign == "L1Distance")
    {
        auto NeighbourDistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = ReconstructionStd[NeighbourIndexList[i]];

            Membership[i] = std::exp(ElementType(-0.5)*(NeighbourDistanceList[i] * NeighbourDistanceList[i]) / (s*s));
        }
    }
    else if (m_Parameter.DistanceTypeForSoftAssign == "L2Distance")
    {
        auto NeighbourDistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(ReconstructedDataColVector, SubRecord);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto s = ReconstructionStd[NeighbourIndexList[i]];

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
    auto L1Norm_value = Membership.L1Norm();
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
        auto s_temp = ReconstructionStd[NeighbourIndexList[i]];

        s_max = (std::max)(s_max, s_temp); // std::max(s_max, s_temp) can not be compiled
    }
    // second: compute ReconstructionScore in [0, 1]
    auto ReconstructionScore = std::exp(ElementType(-0.5)*(ReconstructionErrorL2Norm *ReconstructionErrorL2Norm) / (s_max*s_max));

    // then: update Membership
    Membership *= ReconstructionScore;

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