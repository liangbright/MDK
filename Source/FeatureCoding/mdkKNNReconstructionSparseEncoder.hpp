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
bool KNNReconstructionSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_Parameter.NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input NeighbourNumber (<= 0) @ KNNReconstructionSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::ComputeGramianMatrix_DtD()
{
    if (m_Dictionary->m_Record.IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNReconstructionSparseEncoder::ComputeGramianMatrix_DtD()")
        return false;
    }

    m_GramianMatrix_DtD = m_Dictionary->m_Record.Transpose()*m_Dictionary->m_Record;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Preprocess()
{
    this->ComputeGramianMatrix_DtD();

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                                                   SparseMatrix<ElementType>& CodeInSparseColVector)
{
    auto L2DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, m_Dictionary->m_Record);

    auto NeighbourIndexList = FindKNNByDistanceList(m_Parameter.NeighbourNumber, L2DistanceList);

    auto SubRecord = m_Dictionary->m_Record.GetSubMatrix(ALL, NeighbourIndexList);

    // use LinearLeastSquaresProblemSolver

    auto CodeLength = m_Dictionary->m_Record.GetColNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    if (m_Parameter.Nonnegative == false && m_Parameter.SumToOne == false)
    {
        Option.MethodName = "Normal";

        DenseMatrix<ElementType> H;

        if (m_GramianMatrix_DtD.IsEmpty() == false)
        {
            H = std::move(m_GramianMatrix_DtD.GetSubMatrix(NeighbourIndexList, NeighbourIndexList));
        }

        DenseMatrix<ElementType> A;

        auto Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&SubRecord, &DataColVector,
                                                                            nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                            &H, &Option);

        CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Solution.X, CodeLength);       
    }
    else if (m_Parameter.Nonnegative == true && m_Parameter.SumToOne == false)
    {
        DenseMatrix<ElementType> lb_x(m_Parameter.NeighbourNumber, 1);
        lb_x.Fill(0);

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (m_GramianMatrix_DtD.IsEmpty() == false)
        {
            H = std::move(m_GramianMatrix_DtD.GetSubMatrix(NeighbourIndexList, NeighbourIndexList));
        }

        DenseMatrix<ElementType> A;

        auto Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&SubRecord, &DataColVector,
                                                                            &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                            &H, &Option);

        CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Solution.X, CodeLength);
    }
    else if (m_Parameter.Nonnegative == true && m_Parameter.SumToOne == true)
    {
        DenseMatrix<ElementType> lb_x(m_Parameter.NeighbourNumber, 1);
        lb_x.Fill(0);

        DenseMatrix<ElementType> A(1, m_Parameter.NeighbourNumber);
        A.Fill(1);

        DenseMatrix<ElementType> lb_A = 1;
        DenseMatrix<ElementType> ub_A = 1;

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (m_GramianMatrix_DtD.IsEmpty() == false)
        {
            H = std::move(m_GramianMatrix_DtD.GetSubMatrix(NeighbourIndexList, NeighbourIndexList));
        }

        auto Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&SubRecord, &DataColVector,
                                                                            &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                            &H, &Option);

        CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Solution.X, CodeLength);
    }
    else //if(m_Parameter.Nonnegative == false && m_Parameter.SumToOne == true)
    {
        DenseMatrix<ElementType> A(1, m_Parameter.NeighbourNumber);
        A.Fill(1);

        DenseMatrix<ElementType> lb_A = 1;
        DenseMatrix<ElementType> ub_A = 1;

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (m_GramianMatrix_DtD.IsEmpty() == false)
        {
            H = std::move(m_GramianMatrix_DtD.GetSubMatrix(NeighbourIndexList, NeighbourIndexList));
        }

        auto Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&SubRecord, &DataColVector,
                                                                            nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                            &H, &Option);

        CodeInSparseColVector.ConstructColVector(NeighbourIndexList, Solution.X, CodeLength);
    }
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Postprocess()
{
    m_GramianMatrix_DtD.Clear();

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetNeighbourNumber(NeighbourNumber);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInDenseMatrix(&OutputCodeInDenseMatrix);

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseMatrix(&OutputCodeInSparseMatrix);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        int_max NeighbourNumber,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCodeInSparseColVectorList(&OutputCodeInSparseColVectorList);;

    Encoder->m_Parameter.NeighbourNumber = NeighbourNumber;

    Encoder->Update();
}

}// namespace mdk


#endif