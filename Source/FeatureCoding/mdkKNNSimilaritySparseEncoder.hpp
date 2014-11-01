#ifndef __mdkKNNSimilaritySparseEncoder_hpp
#define __mdkKNNSimilaritySparseEncoder_hpp

namespace mdk
{

template<typename ScalarType>
KNNSimilaritySparseEncoder<ScalarType>::KNNSimilaritySparseEncoder()
{
}


template<typename ScalarType>
KNNSimilaritySparseEncoder<ScalarType>::~KNNSimilaritySparseEncoder()
{
}


template<typename ScalarType>
void KNNSimilaritySparseEncoder<ScalarType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();
    m_Parameter.Clear();
}


template<typename ScalarType>
bool KNNSimilaritySparseEncoder<ScalarType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_Parameter.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0  @ KNNSimilaritySparseEncoder::CheckInput()")
        return false;
    }

    auto IsSimilarityTypeSupported = this->CheckIfSimilarityTypeSupported(m_Parameter.SimilarityType);
    if (IsSimilarityTypeSupported == false)
    {
        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Dictionary->VarianceOfL1Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL1Distance in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_Dictionary->VarianceOfL2Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL2Distance in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_Dictionary->VarianceOfKLDivergence().IsEmpty() == true)
        {
            MDK_Error("VarianceOfKLDivergence in Input Dictionary is empty @ KNNSimilaritySparseEncoder::CheckInput()")
            return false;
        }
    }

    return true;
}


template<typename ScalarType>
inline 
SparseVector<ScalarType> KNNSimilaritySparseEncoder<ScalarType>::
EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex)
{
    return this->EncodeSingleDataVector(DataColVector);
}


template<typename ScalarType>
inline
SparseVector<ScalarType> KNNSimilaritySparseEncoder<ScalarType>::
EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector)
{
	SparseVector<ScalarType> OutputCode;

    const auto& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy
	auto BasisNumber = BasisMatrix.GetColNumber();
	const auto& VarianceOfL1Distance = m_Dictionary->VarianceOfL1Distance();
	const auto& VarianceOfL2Distance = m_Dictionary->VarianceOfL2Distance();
	const auto& VarianceOfKLDivergence = m_Dictionary->VarianceOfKLDivergence();

    //----------------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> SimilarityList(1, m_Parameter.NeighbourNumber);

    DenseMatrix<int_max> NeighbourIndexList(1, m_Parameter.NeighbourNumber);

    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        // calculate mean variance

        auto Variance = ScalarType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfL1Distance[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

        auto NeighbourDistanceList = DistanceList.GetSubMatrix(NeighbourIndexList);

        auto Variance = ScalarType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfL2Distance[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourDistanceList[i] * NeighbourDistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(CorrelationList, m_Parameter.NeighbourNumber);

        auto NeighbourCorrelationList = CorrelationList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            SimilarityList[i] = (NeighbourCorrelationList[i] + 1) / 2;   // [-1, 1] => [0, 1]
        }
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(CorrelationList, m_Parameter.NeighbourNumber);

        auto NeighbourCorrelationList = CorrelationList.GetSubMatrix(NeighbourIndexList);

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            SimilarityList[i] = std::abs(NeighbourCorrelationList[i]); // [-1, 0] => [0, 1]           
        }
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergenceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(KLDivergenceList, m_Parameter.NeighbourNumber);

        auto NeighbourKLDivergenceList = KLDivergenceList.GetSubMatrix(NeighbourIndexList);

        auto Variance = ScalarType(0);
        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            Variance += VarianceOfKLDivergence[NeighbourIndexList[i]];
        }
        Variance /= m_Parameter.NeighbourNumber;

        for (int_max i = 0; i < m_Parameter.NeighbourNumber; ++i)
        {
            auto temp = (NeighbourKLDivergenceList[i] * NeighbourKLDivergenceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    case VectorSimilarityTypeEnum::JSDivergence:
    {
        auto JSDivergenceList = ComputeJSDivergenceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        NeighbourIndexList = FindKNNByDistanceList(JSDivergenceList, m_Parameter.NeighbourNumber);

        SimilarityList = MatrixSubtract(ScalarType(1), JSDivergenceList.GetSubMatrix(NeighbourIndexList));
    }
        break;

    default:

        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::EncodeSingleDataVector(...)")
        return OutputCode;
    }

	OutputCode.Construct(NeighbourIndexList, SimilarityList, BasisNumber);
	return OutputCode;
}

//------------------------------------------------------------ static function --------------------------------------------------------//


template<typename ScalarType>
inline 
bool KNNSimilaritySparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType)
{
    if (SimilarityType != VectorSimilarityTypeEnum::L1Distance
        && SimilarityType != VectorSimilarityTypeEnum::L2Distance
        && SimilarityType != VectorSimilarityTypeEnum::Correlation
        && SimilarityType != VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation
        && SimilarityType != VectorSimilarityTypeEnum::KLDivergence
        && SimilarityType != VectorSimilarityTypeEnum::JSDivergence)
    {
        return false;
    }
    
    return true;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>
KNNSimilaritySparseEncoder<ScalarType>::
ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
               const DenseMatrix<ScalarType>& KNNBasisMatrix,
               const VectorSimilarityTypeEnum  SimilarityType,
               const DenseMatrix<ScalarType>& VarianceList)
{
    int_max KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    DenseMatrix<ScalarType> SimilarityList(1, KNNBasisNumber);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, KNNBasisMatrix);

        // calculate mean variance

        auto Variance = VarianceList.Mean();

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, KNNBasisMatrix);

        auto Variance = VarianceList.Mean();

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, KNNBasisMatrix);

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            SimilarityList[i] = (CorrelationList[i] + 1) / 2;   // [-1, 1] => [0, 1]
        }
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto CorrelationList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, KNNBasisMatrix);

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            SimilarityList[i] = std::abs(CorrelationList[i]); // [-1, 0] => [0, 1]           
        }
    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, KNNBasisMatrix);

        auto Variance = VarianceList.Mean();

        for (int_max i = 0; i < KNNBasisNumber; ++i)
        {
            auto temp = (DistanceList[i] * DistanceList[i]) / Variance;

            SimilarityList[i] = std::exp(-temp * ScalarType(0.1));
        }
    }
        break;

    default:

        MDK_Error("SimilarityType is not supported @ KNNSimilaritySparseEncoder::ComputeCodeVector(...)")
    }

    return SimilarityList;
}


template<typename ScalarType>
inline
ScalarType
KNNSimilaritySparseEncoder<ScalarType>::
ComputeSimilarityBetweenTwoVector(const DenseMatrix<ScalarType>& VectorA, 
                                  const DenseMatrix<ScalarType>& VectorB, 
                                  VectorSimilarityTypeEnum SimilarityType, 
                                  ScalarType Variance)
{

    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVector(...)")
        return ScalarType(0);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVectors(...)")
        return ScalarType(0);
    }

    return this->ComputeSimilarityBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, SimilarityType, Variance, false);
}


template<typename ScalarType>
inline
ScalarType 
KNNSimilaritySparseEncoder<ScalarType>::
ComputeSimilarityBetweenTwoVector(const ScalarType* VectorA, const ScalarType* VectorB, int_max Length,
                                  VectorSimilarityTypeEnum SimilarityType, ScalarType Variance, bool Flag_CheckInput)
{
    ScalarType Similarity = ScalarType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
		auto L1Distance = ComputeL1DistanceBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);
        auto temp = (L1Distance*L1Distance) / Variance;
        Similarity = std::exp(-temp * ScalarType(0.1));
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
		auto L2Distance = ComputeL2DistanceBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);
        auto temp = (L2Distance*L2Distance) / Variance;
        Similarity = std::exp(-temp * ScalarType(0.1));
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
		auto Correlation = ComputeCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = (Correlation + ScalarType(1)) / ScalarType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
		auto Correlation = ComputeCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::UncenteredCorrelation:
    {
		auto Correlation = ComputeUncenteredCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = (Correlation + ScalarType(1)) / ScalarType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUncenteredCorrelation:
    {
		auto Correlation = ComputeUncenteredCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = std::abs(Correlation);

    }
        break;
    /*
    case VectorSimilarityTypeEnum::UnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = (Correlation + ScalarType(1)) / ScalarType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = std::abs(Correlation);

    }
        break;
    */
    case VectorSimilarityTypeEnum::KLDivergence:
    {
		auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, Flag_CheckInput);
		auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, Flag_CheckInput);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) * ScalarType(0.1);

        auto temp = (KLDivergence*KLDivergence) / Variance;
        Similarity = std::exp(-temp * ScalarType(0.1));
    }
        break;

    case VectorSimilarityTypeEnum::JSDivergence:
    {
        auto JSDivergence = ComputeJSDivergenceBetweenTwoVector(VectorA, VectorB, Length, Flag_CheckInput);

        Similarity = ScalarType(1) - JSDivergence;
    }
        break;

    default:
        MDK_Error("unsupported type of similarity @ KNNSimilaritySparseEncoder::ComputeSimilarityBetweenTwoVector(...)")
    }

    return Similarity;
}


}// namespace mdk


#endif