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

    m_MaxNumberOfNonzeroElementsInEachCode = NeighbourNumber;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput() == false)
    {
        return false;
    }

    if (m_MaxNumberOfNonzeroElementsInEachCode <= 0)
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

    auto NeighbourIndexList = FindKNNByDistanceList(m_MaxNumberOfNonzeroElementsInEachCode, L2DistanceList);

    auto SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // solve linear equation using least square method (unconstrained)

    auto Result = SolveLinearLeastSquaresProblem(SubRecord, FeatureVector);

    for (int64 i = 0; i < m_MaxNumberOfNonzeroElementsInEachCode; ++i)
    {
        (*m_FeatureCodeInCompactFormat)(2 * i, IndexOfFeatureVector) = NeighbourIndexList[i];
        (*m_FeatureCodeInCompactFormat)(2 * i + 1, IndexOfFeatureVector) = Result.X[i];
    }

    return true;
}


template<typename ElementType>
DenseMatrix<ElementType> KNNReconstructionSparseEncoder<ElementType>::Apply(const DenseMatrix<ElementType>* FeatureData,
                                                                            const FeatureDictionary<ElementType>* Dictionary,
                                                                            int64 NeighbourNumber = 3,
                                                                            bool  Flag_OutputCodeInCompactFormat = true, // CompactFormat in default
                                                                            int64 MaxNumberOfThreads = 1)
{
    DenseMatrix<ElementType> OutputFeatureCode;

    this->Apply(OutputFeatureCode, FeatureData, Dictionary, NeighbourNumber, Flag_OutputCodeInCompactFormat, MaxNumberOfThreads);

    return OutputFeatureCode;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputFeatureCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int64 NeighbourNumber = 3,
                                                        bool  Flag_OutputCodeInCompactFormat = true, // CompactFormat in default
                                                        int64 MaxNumberOfThreads = 1)
{
    KNNReconstructionSparseEncoder<ElementType>  Encoder;

    Encoder.SetInputFeatureData(FeatureData);

    Encoder.SetInputDictionary(Dictionary);

    Encoder.SetNeighbourNumber(NeighbourNumber);

    Encoder.SetMaxNumberOfThreads(MaxNumberOfThreads);

    if (Flag_OutputCodeInCompactFormat == true)
    {
        Encoder.SetOutputFeatureCodeInCompactFormat(&OutputFeatureCode);;
    }
    else
    {
        Encoder.SetOutputFeatureCodeInDenseFormat(&OutputFeatureCode);;
    }

    Encoder.Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputFeatureCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int64 NeighbourNumber = 3,
                                                        int64 MaxNumberOfThreads = 1)
{
    KNNReconstructionSparseEncoder<ElementType>  Encoder;

    Encoder.SetInputFeatureData(FeatureData);

    Encoder.SetInputDictionary(Dictionary);

    Encoder.SetNeighbourNumber(NeighbourNumber);

    Encoder.SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder.SetOutputFeatureCodeInSparseFormat(&OutputFeatureCode);;

    Encoder.Update();
}


}// namespace mdk


#endif