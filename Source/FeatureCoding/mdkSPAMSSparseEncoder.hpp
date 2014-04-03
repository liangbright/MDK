#ifndef __mdkSPAMSSparseEncoder_hpp
#define __mdkSPAMSSparseEncoder_hpp

//#include "mdkSPAMSSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
SPAMSSparseEncoder<ElementType>::SPAMSSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
SPAMSSparseEncoder<ElementType>::~SPAMSSparseEncoder()
{

}


template<typename ElementType>
void SPAMSSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_MaxNumberOfNonzeroElementsInEachCode = 1;
}


template<typename ElementType>
bool SPAMSSparseEncoder<ElementType>::SetNeighbourNumber(int_max NeighbourNumber)
{
    if (NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input @ SPAMSSparseEncoder::SetNeighbourNumber(NeighbourNumber)")
        return false;
    }

    m_MaxNumberOfNonzeroElementsInEachCode = NeighbourNumber;

    return true;
}


template<typename ElementType>
bool SPAMSSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput() == false)
    {
        return false;
    }

    if (m_MaxNumberOfNonzeroElementsInEachCode <= 0)
    {
        MDK_Error("Invalid input NeighbourNumber (<= 0) @ SPAMSSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    return true;
}


template<typename ElementType>
inline
void SPAMSSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                                                   const FeatureDictionary<ElementType>& InputDictionary,
                                                                   DenseMatrix<ElementType>& SingleFeatureCode,
                                                                   bool Flag_OutputCodeInCompactFormat)
{
    auto L2DistanceList = ComputeL2DistanceListFromSingleVectorToVectorSet(SingleFeatureDataVector, InputDictionary.m_Record);

    auto NeighbourIndexList = FindKNNByDistanceList(m_MaxNumberOfNonzeroElementsInEachCode, L2DistanceList);

    auto SubRecord = InputDictionary.m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // solve linear equation using least square method (unconstrained)

    auto Result = SolveLinearLeastSquaresProblem(SubRecord, SingleFeatureDataVector);

    if (Flag_OutputCodeInCompactFormat == true)
    {
        for (int_max i = 0; i < NeighbourIndexList.GetElementNumber(); ++i)
        {
            SingleFeatureCode[2 * i] = ElementType(NeighbourIndexList[i]); // int_max to ElementType such as double or float
            SingleFeatureCode[2 * i + 1] = Result.X[i];
        }
    }
    else// DenseFormat
    {
        for (int_max i = 0; i < NeighbourIndexList.GetElementNumber(); ++i)
        {
            SingleFeatureCode[NeighbourIndexList[i]] = Result.X[i];
        }
    }

}


template<typename ElementType>
DenseMatrix<ElementType> SPAMSSparseEncoder<ElementType>::Apply(const DenseMatrix<ElementType>* FeatureData,
                                                                            const FeatureDictionary<ElementType>* Dictionary,
                                                                            int_max NeighbourNumber = 3,
                                                                            bool  Flag_OutputCodeInCompactFormat = false, // DenseFormat in default
                                                                            int_max MaxNumberOfThreads = 1)
{
    DenseMatrix<ElementType> OutputFeatureCode;

    this->Apply(OutputFeatureCode, FeatureData, Dictionary, NeighbourNumber, Flag_OutputCodeInCompactFormat, MaxNumberOfThreads);

    return OutputFeatureCode;
}


template<typename ElementType>
bool SPAMSSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputFeatureCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber = 3,
                                                        bool  Flag_OutputCodeInCompactFormat = false, // DenseFormat in default
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<SPAMSSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    if (Flag_OutputCodeInCompactFormat == true)
    {
        Encoder->SetOutputFeatureCodeInCompactFormat(&OutputFeatureCode);;
    }
    else
    {
        Encoder->SetOutputFeatureCodeInDenseFormat(&OutputFeatureCode);;
    }

    Encoder->Update();
}


template<typename ElementType>
bool SPAMSSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputFeatureCode,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber = 3,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<SPAMSSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputFeatureCodeInSparseFormat(&OutputFeatureCode);;

    Encoder->Update();
}


}// namespace mdk


#endif