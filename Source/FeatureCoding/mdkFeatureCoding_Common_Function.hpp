#ifndef __mdkFeatureCoding_Common_Function_hpp
#define __mdkFeatureCoding_Common_Function_hpp


//#include "mdkFeatureCoding_Common_Function.h"

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_NeighbourNumber)
{
    DenseMatrix<int_max> NeighbourIndexList;

    int_max ElementNumber = DistanceList.GetElementNumber();

    if (K_NeighbourNumber > ElementNumber || K_NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input @ mdkFeatureCoding FindKNNByDistanceList(...)")
        return NeighbourIndexList;
    }

    //---------------------------------------------------------------------------------------------

    NeighbourIndexList.Resize(1, K_NeighbourNumber);

    //---------------------------------------------------------------------------------------------

    if (K_NeighbourNumber == ElementNumber)
    {
        for (int_max i = 0; i < K_NeighbourNumber; ++i)
        {
            NeighbourIndexList[i] = i;
        }

        return NeighbourIndexList;
    }
    
    //---------------------------------------------------------------------------------------------

    if (K_NeighbourNumber == 1)
    {
        int_max Index_min = 0;

        ElementType Distance_min = DistanceList[0];

        for (int_max i = 1; i < ElementNumber; ++i)
        {
            if (DistanceList[i] < Distance_min)
            {
                Distance_min = DistanceList[i];

                Index_min = i;
            }
        }

        NeighbourIndexList[0] = Index_min;

        return NeighbourIndexList;
    }

    // K_NeighbourNumber >= 2 -------------------------------------------------------------------

    for (int_max i = 0; i < K_NeighbourNumber; ++i)
    {
        NeighbourIndexList[i] = i;
    }

    for (int_max i = K_NeighbourNumber; i < ElementNumber; ++i)
    {

        //------------------------------------------------------------
        int_max Index_max_in_NeighbourIndexList = 0;

        ElementType Distance_max_in_NeighbourIndexList = DistanceList[NeighbourIndexList[0]];

        for (int_max j = 1; j < K_NeighbourNumber; ++j)
        {
            if (DistanceList[NeighbourIndexList[j]] > Distance_max_in_NeighbourIndexList)
            {
                Distance_max_in_NeighbourIndexList = DistanceList[NeighbourIndexList[j]];

                Index_max_in_NeighbourIndexList = j;
            }
        }
        //------------------------------------------------------------

        if (DistanceList[i] < Distance_max_in_NeighbourIndexList)
        {
            NeighbourIndexList[Index_max_in_NeighbourIndexList] = i;
        }
    }

    return NeighbourIndexList;
}


template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_MaxNumberOfNeighbours, ElementType DistanceThreshold)
{
    DenseMatrix<int_max> tempNeighbourIndexList = FindKNNByDistanceList(DistanceList, K_MaxNumberOfNeighbours);

    DenseMatrix<int_max> NeighbourIndexList;
    NeighbourIndexList.ReserveCapacity(K_MaxNumberOfNeighbours);

    for (int_max i = 0; i < K_MaxNumberOfNeighbours; ++i)
    {
        if (DistanceList[tempNeighbourIndexList[i]] <= DistanceThreshold)
        {
            NeighbourIndexList.AppendCol(tempNeighbourIndexList[i]);
        }
    }

    NeighbourIndexList.Squeeze();

    return NeighbourIndexList;
}


template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_NeighbourNumber)
{
    DenseMatrix<ElementType> DistanceList = SimilarityList;

    DistanceList *= ElementType(-1);

    return FindKNNByDistanceList(DistanceList);
}


template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_MaxNumberOfNeighbours, ElementType SimilarityThreshold)
{
    DenseMatrix<ElementType> DistanceList = SimilarityList;

    DistanceList *= ElementType(-1);

    DenseMatrix<int_max> tempNeighbourIndexList = FindKNNByDistanceList(DistanceList, K_MaxNumberOfNeighbours);

    DenseMatrix<int_max> NeighbourIndexList;
    NeighbourIndexList.ReserveCapacity(K_MaxNumberOfNeighbours);

    for (int_max i = 0; i < K_MaxNumberOfNeighbours; ++i)
    {
        if (SimilarityList[tempNeighbourIndexList[i]] >= SimilarityThreshold)
        {
            NeighbourIndexList.AppendCol({ tempNeighbourIndexList[i] });
        }
    }

    return NeighbourIndexList;
}


//---------------------- Compute Similarity Matrix Between Vectors Stored in DenseMatrix<ElementType> VecorSet ----------------------------//

template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, const char* SimilarityFunctionName)
{
    std::string Name = SimilarityFunctionName;

    return ComputeSimilarityMatrixOfVecorSet(VecorSet, Name);
}


template<typename ElementType>
void ComputeSimilarityMatrixOfVectorSet(DenseMatrix<ElementType>& SimilarityMatrix,
                                       const DenseMatrix<ElementType>& VecorSet,
                                       const char* SimilarityFunctionName)
{
    std::string Name = SimilarityFunctionName;

    return ComputeSimilarityMatrixOfVectorSet(SimilarityMatrix, VecorSet, Name);
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, const std::string& SimilarityFunctionName)
{
    DenseMatrix<ElementType> SimilarityMatrix;

    ComputeSimilarityMatrixOfVectorSet(SimilarityMatrix, VecorSet, SimilarityFunctionName);

    return SimilarityMatrix;
}


template<typename ElementType>
void ComputeSimilarityMatrixOfVecorSet(DenseMatrix<ElementType>& SimilarityMatrix,
                                       const DenseMatrix<ElementType>& VecorSet,
                                       const std::string& SimilarityFunctionName)
{
    if (SimilarityFunctionName == "Correlation")
    {
        ComputeSimilarityMatrixOfVectorSet<ElementType>(SimilarityMatrix, VecorSet,
                                                        [](const DenseMatrix<ElementType>& VecorA, const DenseMatrix<ElementType>& VecorB)
                                                        {return ComputeCorrelationBetweenTwoVectors(VecorA, VecorB);} );
    }
    else if (SimilarityFunctionName == "UncenteredCorrelation")
    {
        ComputeSimilarityMatrixOfVectorSet<ElementType>(SimilarityMatrix, VecorSet,
                                                        [](const DenseMatrix<ElementType>& VecorA, const DenseMatrix<ElementType>& VecorB)
                                                        {return ComputeUncenteredCorrelationBetweenTwoVectors(VecorA, VecorB);} );
    }
    else if (SimilarityFunctionName == "UnnormalizedCorrelation")
    {
        ComputeSimilarityMatrixOfVectorSet<ElementType>(SimilarityMatrix, VecorSet,
                                                        [](const DenseMatrix<ElementType>& VecorA, const DenseMatrix<ElementType>& VecorB)
                                                        {return ComputeUnnormalizedCorrelationBetweenTwoVectors(VecorA, VecorB);} );
    }
}


template<typename ElementType, typename SimilarityFunctionType>
DenseMatrix<ElementType> ComputeSimilarityMatrixOfVectorSet(const DenseMatrix<ElementType>& VecorSet, SimilarityFunctionType SimilarityFunction)
{
    DenseMatrix<ElementType> SimilarityMatrix;

    ComputeSimilarityMatrixOfVectorSet(SimilarityMatrix, VecorSet, SimilarityFunction);

    return SimilarityMatrix;
}


template<typename ElementType, typename SimilarityFunctionType>
void ComputeSimilarityMatrixOfVectorSet(DenseMatrix<ElementType>& SimilarityMatrix,
                                        const DenseMatrix<ElementType>& VecorSet,
                                        SimilarityFunctionType SimilarityFunction)
{
    int_max TotalVectorNumber = VecorSet.GetColNumber();

    SimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);

    SimilarityMatrix.Fill(ElementType(0));  // SimilarityMatrix(i, i) = 0 for all i

    DenseMatrix<ElementType> Vector_k;
    DenseMatrix<ElementType> Vector_n;

    int_max VectorLength = VecorSet.GetRowNumber();

    for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
    {
        Vector_k.ForceShare(VecorSet.GetElementPointerOfCol(k), VectorLength, 1);

        for (int_max n = k + 1; n < TotalVectorNumber; ++n)
        {
            Vector_n.ForceShare(VecorSet.GetElementPointerOfCol(n), VectorLength, 1);

            ElementType Similarity = SimilarityFunction(Vector_k, Vector_n);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    }
}


template<typename ElementType>
inline
ElementType ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType,
                                               const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB,
                                               ElementType Variance)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkFeatureCoding_Common_Function ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkFeatureCoding_Common_Function ComputeSimilarityBetweenTwoVectors(...)")
        return ElementType(0);
    }

    return ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum,
                                              VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, 
                                              Variance, false);
}


template<typename ElementType>
inline
ElementType ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType,
                                               const ElementType* VectorA, const ElementType* VectorB, int_max Length,
                                               ElementType Variance, bool CheckInput)
{
    ElementType Similarity = ElementType(0);

    switch (SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, CheckInput);
        auto temp = (L1Distance*L1Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, CheckInput);
        auto temp = (L2Distance*L2Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case VectorSimilarityTypeEnum::Correlation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::UncenteredCorrelation:
    {
        auto Correlation = ComputeUncenteredCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUncenteredCorrelation:
    {
        auto Correlation = ComputeUncenteredCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::UnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = (Correlation + ElementType(1)) / ElementType(2);
    }
        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfUnnormalizedCorrelation:
    {
        auto Correlation = ComputeUnnormalizedCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);

        Similarity = std::abs(Correlation);

    }
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, CheckInput);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, CheckInput);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = (KLDivergence*KLDivergence) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ mdkFeatureCoding_Common_Function ComputeSimilarityBetweenTwoDataVectors(...)")
    }

    return Similarity;
}


}//namespace mdk


#endif