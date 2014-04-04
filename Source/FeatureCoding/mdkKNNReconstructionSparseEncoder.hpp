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

    m_Parameter.Clear();

    m_GramianMatrix_DtD.Clear();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput() == false)
    {
        return false;
    }

    if (m_Parameter.NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input NeighbourNumber (<= 0) @ KNNReconstructionSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    if (m_Parameter.LsqlinMethodName == "SVD")
    {
    }
    else if (m_Parameter.LsqlinMethodName == "QR")
    {

    }
    else if (m_Parameter.LsqlinMethodName == "Normal")
    {

    }
    else
    {
        MDK_Error("unknown method for Linear Least Squares Method @ KNNReconstructionSparseEncoder::CheckInputAndOutput()")

        return false;
    }

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::PrecomputeGramianMatrix_DtD()
{
    if (m_Dictionary->m_Record.IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNReconstructionSparseEncoder::PrecomputeGramianMatrix_DtD()")
        return false;
    }

    m_GramianMatrix_DtD = m_Dictionary->m_Record.Transpose()*m_Dictionary->m_Record;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Preprocess()
{
    if (m_Parameter.LsqlinMethodName == "Normal")
    {
        this->PrecomputeGramianMatrix_DtD();
    }
    else
    {
        m_GramianMatrix_DtD.Clear();
    }

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                                                   SparseMatrix<ElementType>& CodeInSparseVector)
{
    auto L2DistanceList = ComputeL2DistanceListFromSingleVectorToVectorSet(SingleFeatureDataVector, m_Dictionary->m_Record);

    auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, L2DistanceList);

    auto SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // solve linear equation using least square method (unconstrained)

    DenseLsqlinOption<ElementType> Option;

    Option.MethodName = m_Parameter.LsqlinMethodName;

    if (Option.MethodName == "Normal" && m_GramianMatrix_DtD.IsEmpty() == false)
    {
        Option.GramianMatrix_CtC = m_GramianMatrix_DtD.GetSubMatrix(NeighbourIndexList, NeighbourIndexList);

        //do not write to GramianMatrix_CtC of m_Option_Of_Lsqlin: this function is in a thread
    }

    auto Result = SolveLinearLeastSquaresProblem(SubRecord, SingleFeatureDataVector, Option);

    CodeInSparseVector.ConstructColVector(NeighbourIndexList, Result.X, m_Dictionary->m_Record.GetColNumber());
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Postprocess()
{
    m_GramianMatrix_DtD.Clear();

    return true;
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