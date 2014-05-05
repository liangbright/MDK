#ifndef __mdkKNNReconstructionSparseEncoder_hpp
#define __mdkKNNReconstructionSparseEncoder_hpp


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

    if (m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L1Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::L2Distance
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::Correlation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation
        && m_Parameter.SimilarityType != VectorSimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is not supported @ KNNReconstructionSparseEncoder::CheckInput()")
        return false;
    }

    auto Size = m_Dictionary->GetSize();

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::ComputeGramianMatrix_DtD()
{
    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNReconstructionSparseEncoder::ComputeGramianMatrix_DtD()")
        return false;
    }

    const DenseMatrix<ElementType>& D = m_Dictionary->BasisMatrix(); // "auto  D = " will copy

    m_GramianMatrix_DtD = D.Transpose()*D;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Preprocess()
{
    this->ComputeGramianMatrix_DtD();

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Postprocess()
{
    m_GramianMatrix_DtD.Clear();

    return true;
}


template<typename ElementType>
inline
void KNNReconstructionSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    const DenseMatrix<ElementType>& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy

    const DenseMatrix<ElementType> DataColVector(const_cast<ElementType*>(m_FeatureData->GetElementPointerOfCol(DataIndex)),
                                                 m_FeatureData->GetRowNumber(), 1);

    SparseVector<ElementType>& CodeInSparseColVector = (*m_CodeInSparseColVectorSet)[DataIndex];

    //-------------------------------------------------------------------------------------------------

    DenseMatrix<ElementType> DistanceList;

    switch (m_Parameter.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:

        DistanceList = ComputeL1DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);
        break;

    case VectorSimilarityTypeEnum::L2Distance:

        DistanceList = ComputeL2DistanceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);
        break;

    case VectorSimilarityTypeEnum::Correlation:

        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        break;

    case VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation:

        DistanceList = ComputeCorrelationListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);

        DistanceList.ElementOperationInPlace("abs");

        break;

    case VectorSimilarityTypeEnum::KLDivergence:

        DistanceList = ComputeKLDivergenceListOfSingleVectorFromColVectorSet(DataColVector, BasisMatrix);
        break;

    default:
        MDK_Error("SimilarityType is invalid @ KNNReconstructionSparseEncoder::EncodingFunction(...)")
        return;
    }

    auto KNNBasisIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

    auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNNBasisIndexList);

    // use LinearLeastSquaresProblemSolver

    auto CodeVector = this->ComputeCodeVector(DataColVector, KNNBasisMatrix, KNNBasisIndexList, m_GramianMatrix_DtD,
                                              m_Parameter.CodeNonnegative, m_Parameter.CodeSumToOne);

    // ----- create sparse code ------------------------------------------------

    CodeInSparseColVector.Construct(KNNBasisIndexList, CodeVector, CodeVector.GetElementNumber());
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNReconstructionSparseEncoder<ElementType>::
ComputeCodeVector(const DenseMatrix<ElementType>& DataColVector,
                  const DenseMatrix<ElementType>& KNNBasisMatrix,
                  const DenseMatrix<int_max>&     KNNBasisIndexList,
                  const DenseMatrix<ElementType>& GramianMatrix_DtD,
                  bool CodeNonnegative,
                  bool CodeSumToOne)
{
    auto KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ElementType> Solution;

    typedef LinearLeastSquaresProblemSolver<ElementType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (CodeNonnegative == false && CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);
    }
    else if (CodeNonnegative == true && CodeSumToOne == false)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (CodeNonnegative == true && CodeSumToOne == true)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ElementType(0));

        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }
    else //if(CodeNonnegative == false && CodeSumToOne == true)
    {
        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }

    // change to column vector
    Solution.X.Reshape(KNNBasisNumber, 1);

    return Solution.X;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNReconstructionSparseEncoder<ElementType>::
ComputeCodeVector(const DenseMatrix<ElementType>& DataColVector,
                  const DenseMatrix<ElementType>& KNNBasisMatrix,
                  bool CodeNonnegative,
                  bool CodeSumToOne)
{
    auto KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ElementType> Solution;

    typedef LinearLeastSquaresProblemSolver<ElementType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (CodeNonnegative == false && CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

        DenseMatrix<ElementType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                        nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                        &H, &Option);
    }
    else if (CodeNonnegative == true && CodeSumToOne == false)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (CodeNonnegative == true && CodeSumToOne == true)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ElementType(0));

        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }
    else //if(CodeNonnegative == false && CodeSumToOne == true)
    {
        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ElementType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }

    // change to column vector
    Solution.X.Reshape(KNNBasisNumber, 1);

    return Solution.X;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCode(&OutputCodeInDenseMatrix);

    Encoder->m_Parameter = Parameter;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCode(&OutputCodeInSparseMatrix);;

    Encoder->m_Parameter = Parameter;

    Encoder->Update();
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                                                        const DenseMatrix<ElementType>* FeatureData,
                                                        const FeatureDictionary<ElementType>* Dictionary,
                                                        const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                                                        int_max MaxNumberOfThreads = 1)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ElementType>>();

    Encoder->SetInputFeatureData(FeatureData);

    Encoder->SetInputDictionary(Dictionary);

    Encoder->SetMaxNumberOfThreads(MaxNumberOfThreads);

    Encoder->SetOutputCode(&OutputCodeInSparseColVectorList);;

    Encoder->m_Parameter = Parameter;

    Encoder->Update();
}

}// namespace mdk


#endif