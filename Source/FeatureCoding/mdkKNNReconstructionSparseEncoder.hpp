#ifndef __mdkKNNReconstructionSparseEncoder_hpp
#define __mdkKNNReconstructionSparseEncoder_hpp


namespace mdk
{

template<typename ScalarType>
KNNReconstructionSparseEncoder<ScalarType>::KNNReconstructionSparseEncoder()
{
    this->Clear();
}


template<typename ScalarType>
KNNReconstructionSparseEncoder<ScalarType>::~KNNReconstructionSparseEncoder()
{
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();
    m_Parameter.Clear();
    m_GramianMatrix_DtD.Clear();
}


template<typename ScalarType>
bool KNNReconstructionSparseEncoder<ScalarType>::CheckInput()
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

    auto IsSimilarityTypeSupported = KNNSimilaritySparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(m_Parameter.SimilarityType);
    if (IsSimilarityTypeSupported == false)
    {
        MDK_Error("SimilarityType is not supported @ KNNReconstructionSparseEncoder::CheckInput()")
        return false;
    }

	if (m_Dictionary->IsEmpty() == true)
	{
		MDK_Error("Dictionary is empty @ KNNReconstructionSparseEncoder::CheckInput()")
		return false;
	}

    return true;
}


template<typename ScalarType>
bool KNNReconstructionSparseEncoder<ScalarType>::ComputeGramianMatrix_DtD()
{
    if (m_Dictionary->IsEmpty() == true)
    {
        MDK_Error("Dictionary is empty @ KNNReconstructionSparseEncoder::ComputeGramianMatrix_DtD()")
        return false;
    }

    const DenseMatrix<ScalarType>& D = m_Dictionary->BasisMatrix(); // "auto  D = " will copy

    m_GramianMatrix_DtD = D.Transpose()*D;

    return true;
}


template<typename ScalarType>
bool KNNReconstructionSparseEncoder<ScalarType>::Preprocess()
{
	if (this->FeatureDictionaryBasedSparseEncoder::Preprocess() == false)
	{
		return false;
	}
    this->ComputeGramianMatrix_DtD();
    return true;
}


template<typename ScalarType>
bool KNNReconstructionSparseEncoder<ScalarType>::Postprocess()
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
SparseVector<ScalarType> KNNReconstructionSparseEncoder<ScalarType>::
EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex)
{
	return this->EncodeSingleDataVector(DataColVector);
}


template<typename ScalarType>
inline
SparseVector<ScalarType> KNNReconstructionSparseEncoder<ScalarType>::
EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector)
{
    const auto& BasisMatrix = m_Dictionary->BasisMatrix(); // "auto  = " will copy
	int_max BasisNumber = BasisMatrix.GetColNumber();

	SparseVector<ScalarType> OutputCode;
    //-------------------------------------------------------------------------------------------------

    DenseMatrix<ScalarType> DistanceList;

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

    case VectorSimilarityTypeEnum::JSDivergence:

        DistanceList = ComputeJSDivergenceListFromSingleVectorToColVectorSet(DataColVector, BasisMatrix);
        break;

    default:
        MDK_Error("SimilarityType is invalid @ KNNReconstructionSparseEncoder::EncodingFunction(...)")
        return OutputCode;
    }

    auto KNNBasisIndexList = FindKNNByDistanceList(DistanceList, m_Parameter.NeighbourNumber);

    auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNNBasisIndexList);

    // use LinearLeastSquaresProblemSolver

    auto KNNCodeVector = this->ComputeKNNCode(DataColVector, KNNBasisMatrix, KNNBasisIndexList, m_GramianMatrix_DtD,
                                              m_Parameter.CodeNonnegative, m_Parameter.CodeSumToOne);

    // ----- create sparse code ------------------------------------------------

    if (KNNBasisIndexList.IsEmpty() == true)
    {
		MDK_Error("KNNBasisIndexList is empty @ KNNReconstructionSparseEncoder<ScalarType>::EncodeSingleDataVector(...)")
    }

	OutputCode.Construct(KNNBasisIndexList, KNNCodeVector, BasisNumber);
	return OutputCode;
}


template<typename ScalarType>
bool KNNReconstructionSparseEncoder<ScalarType>::UpdateWithPreviousKNNSearchResult()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    if (m_SparseCode.GetLength() != m_FeatureData->GetColNumber())
    {
        MDK_Error(" number of code vectors != number of input data vectors @ KNNReconstructionSparseEncoder::UpdateWithPreviousKNNSearchResult(...)")
        return false;
    }

	this->UpdateReconstructionCodeWithKNNSearchResult(m_SparseCode,
													  *m_FeatureData,
                                                      m_Dictionary->BasisMatrix(), 
                                                      m_Parameter.CodeNonnegative, 
                                                      m_Parameter.CodeSumToOne,
                                                      m_MaxNumberOfThread);
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::GetReconstructedData(DenseMatrix<ScalarType>& ReconstructedDataSet)
{
    if (this->CheckInput() == false)
    {
        return;
    }

	if (m_SparseCode.GetLength() != m_FeatureData->GetColNumber())
    {
        MDK_Error(" number of code vectors != number of input data vectors @ KNNReconstructionSparseEncoder::GetReconstructedData(...)")
        return;
    }

    this->ReconstructData(ReconstructedDataSet,
                          m_SparseCode,
                          m_Dictionary->BasisMatrix(),
                          m_MaxNumberOfThread);
}

//--------------------------------------------------- static function --------------------------------------------------------------------//

template<typename ScalarType>
inline
bool KNNReconstructionSparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType)
{
    return KNNSimilaritySparseEncoder<ScalarType>::CheckIfSimilarityTypeSupported(SimilarityType);
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNReconstructionSparseEncoder<ScalarType>::
ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
               const DenseMatrix<ScalarType>& KNNBasisMatrix,
               const DenseMatrix<int_max>&     KNNBasisIndexList,
               const DenseMatrix<ScalarType>& GramianMatrix_DtD,
               bool CodeNonnegative,
               bool CodeSumToOne)
{
    // Output code vector is NOT the full code vector
    // the length of the output code vector is the length of KNNBasisIndexList

    auto KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ScalarType> Solution;

    typedef LinearLeastSquaresProblemSolver<ScalarType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (CodeNonnegative == false && CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

        DenseMatrix<ScalarType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ScalarType> A;

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);
    }
    else if (CodeNonnegative == true && CodeSumToOne == false)
    {
        DenseMatrix<ScalarType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ScalarType> A;

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (CodeNonnegative == true && CodeSumToOne == true)
    {
        DenseMatrix<ScalarType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ScalarType(0));

        DenseMatrix<ScalarType> A(1, KNNBasisNumber);
        A.Fill(ScalarType(1));

        DenseMatrix<ScalarType> lb_A = ScalarType(1);
        DenseMatrix<ScalarType> ub_A = ScalarType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }
    else //if(CodeNonnegative == false && CodeSumToOne == true)
    {
        DenseMatrix<ScalarType> A(1, KNNBasisNumber);
        A.Fill(ScalarType(1));

        DenseMatrix<ScalarType> lb_A = ScalarType(1);
        DenseMatrix<ScalarType> ub_A = ScalarType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }

    // change to column vector
    Solution.X.Reshape(KNNBasisNumber, 1);

    return Solution.X;
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNReconstructionSparseEncoder<ScalarType>::
ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector, const DenseMatrix<ScalarType>& KNNBasisMatrix, bool CodeNonnegative, bool CodeSumToOne)
{
    auto KNNBasisNumber = KNNBasisMatrix.GetColNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ScalarType> Solution;

    typedef LinearLeastSquaresProblemSolver<ScalarType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (CodeNonnegative == false && CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

        DenseMatrix<ScalarType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        DenseMatrix<ScalarType> A;

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                        nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                        &H, &Option);
    }
    else if (CodeNonnegative == true && CodeSumToOne == false)
    {
        DenseMatrix<ScalarType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        DenseMatrix<ScalarType> A;

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (CodeNonnegative == true && CodeSumToOne == true)
    {
        DenseMatrix<ScalarType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ScalarType(0));

        DenseMatrix<ScalarType> A(1, KNNBasisNumber);
        A.Fill(ScalarType(1));

        DenseMatrix<ScalarType> lb_A = ScalarType(1);
        DenseMatrix<ScalarType> ub_A = ScalarType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }
    else //if(CodeNonnegative == false && CodeSumToOne == true)
    {
        DenseMatrix<ScalarType> A(1, KNNBasisNumber);
        A.Fill(ScalarType(1));

        DenseMatrix<ScalarType> lb_A = ScalarType(1);
        DenseMatrix<ScalarType> ub_A = ScalarType(1);

        Option.MethodType = LinlsqMethodTypeEnum::QuadraticProgramming;

        DenseMatrix<ScalarType> H = MatrixMultiply(KNNBasisMatrix.Transpose(), KNNBasisMatrix);

        Solution = LinearLeastSquaresProblemSolver<ScalarType>::Apply(&KNNBasisMatrix, &DataColVector,
                                                                       nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                       &H, &Option);
    }

    // change to column vector
    Solution.X.Reshape(KNNBasisNumber, 1);

    return Solution.X;
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::
UpdateReconstructionCodeWithKNNSearchResult(ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
											const DenseMatrix<ScalarType>&  FeatureData,                          
											const DenseMatrix<ScalarType>&  BasisMatrix,
											bool CodeNonnegative,
										    bool CodeSumToOne,
											int_max MaxNumberOfThread = 1)
{
    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ScalarType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    //for (int_max k = 0; k <= DataNumber-1; ++k)
    auto TempFunction_ComputeReconstructionCode = [&](int_max k)
    {
        const auto& KNNBasisIndexList = ReconstructionCodeSet[k].IndexList();

        int_max KNNBasisNumber = KNNBasisIndexList.GetLength();

        if (KNNBasisNumber > 0)
        {
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNNBasisIndexList);

            DenseMatrix<ScalarType> DataColVector;
            DataColVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            auto KNNCodeVector = KNNReconstructionSparseEncoder<ScalarType>::ComputeKNNCode(DataColVector,
                                                                                             KNNBasisMatrix,
                                                                                             KNNBasisIndexList,
                                                                                             GramianMatrix_DtD,
                                                                                             CodeNonnegative,
                                                                                             CodeSumToOne);

            ReconstructionCodeSet[k].Construct(KNNBasisIndexList, KNNCodeVector, BasisNumber);
        }
    };

    ParallelForLoop(TempFunction_ComputeReconstructionCode, 0, DataNumber - 1, MaxNumberOfThread);
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>>
KNNReconstructionSparseEncoder<ScalarType>::
ComputeReconstructionCodeFromSimilarityCode(const DenseMatrix<ScalarType>&  FeatureData, 
                                            const ObjectArray<SparseVector<ScalarType>>& SimilarityCodeSet,
                                            const DenseMatrix<ScalarType>&  BasisMatrix,
                                            bool CodeNonnegative,
                                            bool CodeSumToOne,
                                            int_max MaxNumberOfThread)
{
    ObjectArray<SparseVector<ScalarType>> ReconstructionCodeSet = SimilarityCodeSet;

	KNNReconstructionSparseEncoder<ScalarType>::UpdateReconstructionCodeWithKNNSearchResult(ReconstructionCodeSet,
																							FeatureData,
																							BasisMatrix,
																							CodeNonnegative,
																							CodeSumToOne,
																							MaxNumberOfThread);
    return ReconstructionCodeSet;
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>>
KNNReconstructionSparseEncoder<ScalarType>::
ComputeReconstructionCodeFromSoftAssignCode(const DenseMatrix<ScalarType>&  FeatureData, 
                                            const ObjectArray<SparseVector<ScalarType>>& SoftAssignCodeSet,
                                            const DenseMatrix<ScalarType>&  BasisMatrix,
                                            bool CodeNonnegative,
                                            bool CodeSumToOne,
                                            int_max MaxNumberOfThread)
{
    ObjectArray<SparseVector<ScalarType>> ReconstructionCodeSet = SoftAssignCodeSet;

	KNNReconstructionSparseEncoder<ScalarType>::UpdateReconstructionCodeWithKNNSearchResult(ReconstructionCodeSet,
																							FeatureData,
																							BasisMatrix,
																							CodeNonnegative,
																							CodeSumToOne,
																							MaxNumberOfThread);
    return ReconstructionCodeSet;
}


template<typename ScalarType>
DenseMatrix<ScalarType> 
KNNReconstructionSparseEncoder<ScalarType>::
ReconstructData(const SparseVector<ScalarType>& ReconstructionCode, const DenseMatrix<ScalarType>& BasisMatrix)
{
    DenseMatrix<ScalarType> ReconstructedDataVector;

    KNNReconstructionSparseEncoder<ScalarType>::ReconstructData(ReconstructedDataVector, ReconstructionCode, BasisMatrix);

    return ReconstructedDataVector;
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::
ReconstructData(DenseMatrix<ScalarType>&        ReconstructedDataVector, 
                const SparseVector<ScalarType>& ReconstructionCode, 
                const DenseMatrix<ScalarType>&  BasisMatrix)
{
    int_max KNNBasisNumber = int_max(ReconstructionCode.IndexList().GetLength());

    const DenseMatrix<ScalarType> SubCodeVector(const_cast<ScalarType*>(ReconstructionCode.ElementList().GetElementPointer()), KNNBasisNumber, 1);

    DenseMatrix<ScalarType> KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, ReconstructionCode.IndexList());

    MatrixMultiply(ReconstructedDataVector, KNNBasisMatrix, SubCodeVector);
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNReconstructionSparseEncoder<ScalarType>::
ReconstructData(const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet, const DenseMatrix<ScalarType>& BasisMatrix, int_max MaxNumberOfThread)
{
    DenseMatrix<ScalarType> ReconstructedDataSet;

    KNNReconstructionSparseEncoder<ScalarType>::ReconstructData(ReconstructedDataSet, ReconstructionCodeSet, BasisMatrix, MaxNumberOfThread);

    return ReconstructedDataSet;
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::
ReconstructData(DenseMatrix<ScalarType>& ReconstructedDataSet,
                const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet, 
                const DenseMatrix<ScalarType>&  BasisMatrix,
                int_max MaxNumberOfThread)
{
    int_max DataNumber = ReconstructionCodeSet.GetLength();

    int_max VectorLength = BasisMatrix.GetRowNumber();

    ReconstructedDataSet.FastResize(VectorLength, DataNumber);

    //for (int_max k = 0; k <= DataNumber-1; ++k)
    auto TempFunction_ReconstructDataSet = [&](int_max k)
    {
        DenseMatrix<ScalarType> ReconstructedDataVector;

        ReconstructedDataVector.Share(ReconstructedDataSet.GetElementPointerOfCol(k), VectorLength, 1);

        KNNReconstructionSparseEncoder<ScalarType>::ReconstructData(ReconstructedDataVector, ReconstructionCodeSet[k], BasisMatrix);
    };

    ParallelForLoop(TempFunction_ReconstructDataSet, 0, DataNumber - 1, MaxNumberOfThread);
}


template<typename ScalarType>
ScalarType 
KNNReconstructionSparseEncoder<ScalarType>::
ComputeReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  DataColVector,
                                 const SparseVector<ScalarType>& ReconstructionCode,
                                 const DenseMatrix<ScalarType>&  BasisMatrix)
{
    auto ReconstructedDataVector = KNNReconstructionSparseEncoder<ScalarType>::ReconstructData(ReconstructionCode, BasisMatrix);

    ReconstructedDataVector -= DataColVector;

    return ReconstructedDataVector.L2Norm();
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNReconstructionSparseEncoder<ScalarType>::
ComputeReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                 const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                 const DenseMatrix<ScalarType>&  BasisMatrix,
                                 int_max MaxNumberOfThread)
{
    DenseMatrix<ScalarType> ErrorL2NormList;

    KNNReconstructionSparseEncoder<ScalarType>::ComputeReconstructionErrorL2Norm(ErrorL2NormList, FeatureData, 
                                                                                  ReconstructionCodeSet, BasisMatrix, MaxNumberOfThread);

    return ErrorL2NormList;
}


template<typename ScalarType>
void KNNReconstructionSparseEncoder<ScalarType>::
ComputeReconstructionErrorL2Norm(DenseMatrix<ScalarType>& ErrorL2NormList, 
                                 const DenseMatrix<ScalarType>&  FeatureData,
                                 const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                 const DenseMatrix<ScalarType>&  BasisMatrix,
                                 int_max MaxNumberOfThread)
{
    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    ErrorL2NormList.FastResize(1, DataNumber);

    //for (int_max k = 0; k <= DataNumber-1; ++k)
    auto TempFunction_ComputeReconstructionErrorL2Norm = [&](int_max k)
    {
        DenseMatrix<ScalarType> DataColVector;

        DataColVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

        ErrorL2NormList[k] = KNNReconstructionSparseEncoder<ScalarType>::
                             ComputeReconstructionErrorL2Norm(DataColVector, ReconstructionCodeSet[k], BasisMatrix);
    };

    ParallelForLoop(TempFunction_ComputeReconstructionErrorL2Norm, 0, DataNumber - 1, MaxNumberOfThread);
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>>
KNNReconstructionSparseEncoder<ScalarType>::Apply(const DenseMatrix<ScalarType>* FeatureData,
                                                   const FeatureDictionary<ScalarType>* Dictionary,
                                                   const Parameter_Of_KNNReconstructionSparseEncoder<ScalarType>& Parameter,
                                                   int_max MaxNumberOfThread)
{
    auto Encoder = std::make_unique<KNNReconstructionSparseEncoder<ScalarType>>();
    Encoder->SetInputFeatureData(FeatureData);
    Encoder->SetInputDictionary(Dictionary);
	Encoder->SetMaxNumberOfThread(MaxNumberOfThread);
    Encoder->m_Parameter = Parameter;
    Encoder->Update();
	return *Encoder->GetOutputCode();
}

}// namespace mdk


#endif