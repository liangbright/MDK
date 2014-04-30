#ifndef __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_hpp
#define __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::KNNSoftAssignAndAverageOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::~KNNSoftAssignAndAverageOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("NumberOfDataInEachBatch > TotalDataNumber @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("NumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L1Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L2Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::Correlation
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_Dictionary->VarianceOfL1Distance().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Variance_L1 <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::L2Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_Dictionary->VarianceOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("Variance_L2 <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::Correlation)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_Dictionary->VarianceOfKLDivergence().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Variance_KL <= 0 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.weigth_past < 0 || m_Parameter.weigth_past > 1)
    {
        MDK_Error("m_Parameter.weigth_past < 0 || m_Parameter.weigth_past > 1 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.weigth_s < 0 || m_Parameter.weigth_s > 1)
    {
        MDK_Error("m_Parameter.weigth_s < 0 || m_Parameter.weigth_s > 1 @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    this->SetputParameter();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    //--------- build the initial Dictionary -----------------------------//
    //
    m_KNNSoftAssignDictionaryBuilder.SetInputFeatureData(m_FeatureData);
    m_KNNSoftAssignDictionaryBuilder.SetInitialDictionary(m_InitialDictionary);
    m_KNNSoftAssignDictionaryBuilder.SetOutputDictionary(&OutputDictionary);
    m_KNNSoftAssignDictionaryBuilder.Update();

    //----------- encode data and update dictionary ----------------------------//

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    if (m_Parameter.NumberOfDataInEachBatch >= TotalDataNumber)
    {
        m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);
        m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
        m_KNNSoftAssignSparseEncoder.Update();

        this->UpdateDictionaryAfterEncoding(OutputDictionary, *m_FeatureData, *m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet());

        this->UpdateDictionary_OtherInfo(OutputDictionary);

        (*m_Dictionary) = std::move(OutputDictionary);

        return;
    }

    // m_Parameter.MaxNumberOfIteration > 1
    //------------------------------------------ run Data batch -------------------------------------------------------//

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

    DenseMatrix<ElementType> FeatureData_current(m_FeatureData->GetRowNumber(), m_Parameter.NumberOfDataInEachBatch);

    for (int_max IterationIndex = 0; IterationIndex < m_Parameter.MaxNumberOfIteration; IterationIndex++)
    {
        // sample data with m_Parameter.NumberOfDataInEachBatch 
        for (int_max k = 0; k < m_Parameter.NumberOfDataInEachBatch; ++k)
        {
            auto DataIndex = UniformRandomNumber(gen);

            FeatureData_current.SetCol(k, m_FeatureData->GetElementPointerOfCol(DataIndex));
        }

        m_KNNSoftAssignSparseEncoder.SetInputFeatureData(&FeatureData_current);
        m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
        m_KNNSoftAssignSparseEncoder.Update();

         this->UpdateDictionaryAfterEncoding(OutputDictionary, FeatureData_current, *m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet());
    }

    this->UpdateDictionary_OtherInfo(OutputDictionary);

    (*m_Dictionary) = std::move(OutputDictionary);

    return;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::SetputParameter()
{
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.DictionaryName = m_Parameter.DictionaryName;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.BasisNumber;
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.BasisPositive = m_Parameter.BasisPositive;
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.BasisNormalizedWithL1Norm = m_Parameter.BasisNormalizedWithL1Norm;
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.BasisNormalizedWithL2Norm = m_Parameter.BasisNormalizedWithL2Norm;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign = m_Parameter.ParameterOfKNNSoftAssign;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.MaxNumberOfIteration = 1;
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.NumberOfDataInEachBatch = m_Parameter.NumberOfDataInEachBatch;
    m_KNNSoftAssignDictionaryBuilder.m_Parameter.MaxNumberOfThreads = m_Parameter.MaxNumberOfThreads;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.weigth_past = m_Parameter.weigth_past;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.weigth_s = m_Parameter.weigth_s;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.UpdateDictionaryInformation_Other = false;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.ParameterOfKNNReconstruction = m_Parameter.ParameterOfKNNReconstruction;

    m_KNNSoftAssignDictionaryBuilder.m_Parameter.SimilarityThresholdToComputeBasisRedundancy = m_Parameter.SimilarityThresholdToComputeBasisRedundancy;

    //------------------------------

    m_KNNSoftAssignSparseEncoder.m_Parameter = m_Parameter.ParameterOfKNNSoftAssign;    
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::UpdateDictionaryAfterEncoding(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                                                                                const DenseMatrix<ElementType>& FeatureData,
                                                                                                const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& ProbabilityMassFunction = Dictionary.ProbabilityMassFunction();

    //DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    //DenseMatrix<ElementType>& BasisRedundancy = Dictionary->BasisRedundancy();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    auto WeightedNumberOfTrainingSamplesInHistory = Dictionary.GetWeightedNumberOfTrainingSamplesInHistory();

    // update BasisMatrix -----------------------------------------------------------------------------
   
    DenseMatrix<ElementType> WeightOfError(1, BasisNumber);
    WeightOfError.Fill(ElementType(1));
    for (int_max k = 0; k < BasisNumber; ++k)
    {
        WeightOfError[k] += WeightedNumberOfTrainingSamplesInHistory*ProbabilityMassFunction[k];
    }

    this->UpdateBasisMatrix(BasisMatrix, FeatureData, CodeTable, WeightOfError);

    // update ProbabilityMassFunction ----------------------------------------------------------

    this->UpdateProbabilityMassFunction(ProbabilityMassFunction,
                                        WeightedNumberOfTrainingSamplesInHistory,
                                        CodeTable);

    this->UpdateVarianceOfL1Distance(VarianceOfL1Distance, FeatureData, CodeTable, BasisMatrix, WeightedNumberOfTrainingSamplesInHistory);

    this->UpdateVarianceOfL2Distance(VarianceOfL2Distance, FeatureData, CodeTable, BasisMatrix, WeightedNumberOfTrainingSamplesInHistory);

    this->UpdateVarianceOfReconstruction(VarianceOfReconstruction, FeatureData, CodeTable, BasisMatrix, WeightedNumberOfTrainingSamplesInHistory);

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(VarianceOfKLDivergence, FeatureData, CodeTable, BasisMatrix, WeightedNumberOfTrainingSamplesInHistory);
    }

    //this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix);
    //this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    
    //Dictionary.SetInfo_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::UpdateDictionary_OtherInfo(FeatureDictionaryForSparseCoding<ElementType>& Dictionary)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();
    DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    DenseMatrix<ElementType>& BasisRedundancy = Dictionary.BasisRedundancy();

    this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix);
    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    
    Dictionary.SetInfo_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                                                                                    const DenseMatrix<ElementType>& FeatureData,
                                                                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                                    const DenseMatrix<ElementType> WeightOfError)
{
    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    DenseMatrix<ElementType> DataVector;
    DenseMatrix<ElementType> BasisVector;

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

        auto Data_Data_InnerProduct = this->ComputeVectorInnerProduct(DataVector, DataVector);

        const DenseMatrix<int_max>& BasisIndexList = CodeTable[k].IndexList();

        for (int_max n = 0; n < BasisIndexList.GetElementNumber(); ++n)
        {
            auto BasisIndex = BasisIndexList[n];

            BasisVector.ForceShare(BasisMatrix.GetElementPointerOfCol(BasisIndex), VectorLength, 1);
            
            auto Data_Basis_InnnerProduct = this->ComputeVectorInnerProduct(DataVector, BasisVector);

            auto factor = ElementType(0);
            if (Data_Data_InnerProduct > eps_value && Data_Data_InnerProduct > eps_value*Data_Basis_InnnerProduct)
            {
                factor = Data_Basis_InnnerProduct / Data_Data_InnerProduct;

                BasisVector += WeightOfError[BasisIndex] * (factor*DataVector - BasisVector);
            }            
        }        
    }

    this->ApplyConstraintOnBasis(BasisMatrix);
}


template<typename ElementType>
inline 
ElementType
KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::ComputeVectorInnerProduct(const DenseMatrix<ElementType>& A, const DenseMatrix<ElementType>& B)
{
    auto Length = A.GetElementNumber();

    auto Value = ElementType(0);
    for (int_max k = 0; k < Length; ++k)
    {
        Value += A[k] * B[k];
    }
    return Value;
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix)
{
    if (m_Parameter.BasisPositive == true)
    {
        for (int_max k = 0; k < BasisMatrix.GetElementNumber(); ++k)
        {
            if (BasisMatrix[k] < ElementType(0))
            {
                BasisMatrix[k] = ElementType(0);
            }
        }
    }

    if (m_Parameter.BasisNormalizedWithL1Norm == true)
    {
        int_max BasisNumber = BasisMatrix.GetColNumber();
        int_max Length = BasisMatrix.GetRowNumber();

        auto tempValueToFill = ElementType(1) / ElementType(Length);

        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        DenseMatrix<ElementType> BasisVector;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            BasisVector.Share(BasisMatrix.GetElementPointerOfCol(k), Length, 1);

            auto tempL1Norm = BasisVector.L1Norm();
            if (tempL1Norm <= eps_value)
            {
                BasisVector.Fill(tempValueToFill);
            }
            else
            {
                BasisVector /= tempL1Norm;
            }
        }
    }
    else if (m_Parameter.BasisNormalizedWithL2Norm == true)
    {
        int_max BasisNumber = BasisMatrix.GetColNumber();
        int_max Length = BasisMatrix.GetRowNumber();

        auto tempValueToFill = std::sqrt(ElementType(1) / ElementType(Length));

        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        DenseMatrix<ElementType> BasisVector;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            BasisVector.Share(BasisMatrix.GetElementPointerOfCol(k), Length, 1);

            auto tempL2Norm = BasisVector.L2Norm();
            if (tempL2Norm <= eps_value)
            {
                BasisVector.Fill(tempValueToFill);
            }
            else
            {
                BasisVector /= tempL2Norm;
            }
        }
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateProbabilityMassFunction(DenseMatrix<ElementType>&  ProbabilityMassFunction,
                              ElementType WeightedNumberOfTrainingSamplesInHistory,
                              const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    int_max BasisNumber = ProbabilityMassFunction.GetElementNumber();

    // convert Probability to representative ability
    ProbabilityMassFunction *= m_Parameter.weigth_past * WeightedNumberOfTrainingSamplesInHistory;

    int_max DataNumber = CodeTable.GetLength();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        int_max tempNumber = KNN_IndexList.GetElementNumber();

        for (int_max m = 0; m < tempNumber; ++m)
        {
            ProbabilityMassFunction[KNN_IndexList[m]] += (ElementType(1) - m_Parameter.weigth_past) / ElementType(tempNumber);
        }
    }

    auto tempSum = ProbabilityMassFunction.Sum();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();
    if (tempSum <= eps_value)
    {
        ProbabilityMassFunction.Fill(ElementType(1) / ElementType(BasisNumber));
    }
    else
    {
        ProbabilityMassFunction /= tempSum;
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, const DenseMatrix<ElementType>& BasisMatrix)
{

}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
{

}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const ElementType WeightedNumberOfTrainingSamplesInHistory)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(ElementType(0));

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ForceShare(BasisMatrix.GetElementPointerOfCol(BasisIndex), VectorLength, 1);

                ErrorVector = MatrixSubtract(DataVector, BasisVector);

                auto ErrorNorm = ErrorVector.L1Norm();

                Variance_current[BasisIndex] += ErrorNorm*ErrorNorm;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        Variance = m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory*Variance 
                   + (1 - m_Parameter.weigth_past)*Variance_current;

        Variance /= m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory + 1 - m_Parameter.weigth_past;
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::UpdateVarianceOfL1Distance(...)"
                    << '\n' << "set to std::max(eps_value, Variance_L1)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= eps_value)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const ElementType WeightedNumberOfTrainingSamplesInHistory)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(ElementType(0));

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    for (int_max k = 0; k < DataNumber; ++k)
    {        
        const DenseMatrix<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ForceShare(BasisMatrix.GetElementPointerOfCol(BasisIndex), VectorLength, 1);

                ErrorVector = MatrixSubtract(DataVector, BasisVector);

                auto ErrorInnerProduct = this->ComputeVectorInnerProduct(ErrorVector, ErrorVector);

                Variance_current[BasisIndex] += ErrorInnerProduct;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        Variance = m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory*Variance
                    + (1 - m_Parameter.weigth_past)*Variance_current;

        Variance /= m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory + 1 - m_Parameter.weigth_past;
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::UpdateVarianceOfL2Distance(...)"
                    << '\n' << "set to std::max(eps_value, Variance_L2)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= eps_value)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                             const DenseMatrix<ElementType>& FeatureData,
                             const DataContainer<SparseVector<ElementType>>& CodeTable,
                             const DenseMatrix<ElementType>& BasisMatrix,
                             const ElementType WeightedNumberOfTrainingSamplesInHistory)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(ElementType(0));

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ForceShare(BasisMatrix.GetElementPointerOfCol(BasisIndex), VectorLength, 1);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVector, BasisVector);

                Variance_current[BasisIndex] += KLDivergence*KLDivergence;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance_current[n] /= CounterList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        Variance = m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory*Variance
                   + (1 - m_Parameter.weigth_past)*Variance_current;

        Variance /= m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory + 1 - m_Parameter.weigth_past;
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignAndAverageOnlineDictionaryBuilder::UpdateVarianceOfKLDivergence(...)"
                     << '\n' << "set to std::max(eps_value, Variance_KL)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= eps_value)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                               const DenseMatrix<ElementType>& FeatureData,
                               const DataContainer<SparseVector<ElementType>>& CodeTable,
                               const DenseMatrix<ElementType>& BasisMatrix,
                               const ElementType WeightedNumberOfTrainingSamplesInHistory)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(ElementType(0));

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> CodeVector;

    DenseMatrix<ElementType> ReconstructedDataVector;

    DenseMatrix<ElementType> ErrorVector;

    DenseMatrix<ElementType> BasisVector;

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            this->ReconstructDataVectorByKNNBasisMatrix(ReconstructedDataVector, DataVector, KNNBasisMatrix, KNN_IndexList, GramianMatrix_DtD);

            MatrixSubtract(ErrorVector, DataVector, ReconstructedDataVector);

            auto ErrorInnerProduct = this->ComputeVectorInnerProduct(ErrorVector, ErrorVector);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ForceShare(BasisMatrix.GetElementPointerOfCol(BasisIndex), VectorLength, 1);

                Variance_current[BasisIndex] += ErrorInnerProduct;

                CounterList[BasisIndex] += 1;
            }
        }
       
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            Variance[n] /= CounterList[n];
        }
    }

    if (Variance.IsEmpty() == false)
    {
        Variance = m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory*Variance
                   + (1 - m_Parameter.weigth_past)*Variance_current;

        Variance /= m_Parameter.weigth_past*WeightedNumberOfTrainingSamplesInHistory + 1 - m_Parameter.weigth_past;
    }
    else
    {
        Variance.Take(Variance_current);
    }

    auto MeanStd = Variance.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignOnlineDictionaryBuilder::UpdateVarianceOfReconstruction(...)"
                     << '\n' << "set to std::max(eps_value, Variance_L2)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance[n] <= eps_value)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
void KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType>::
ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector, 
                                      const DenseMatrix<ElementType>& DataVector,
                                      const DenseMatrix<ElementType>& KNNBasisMatrix,                                     
                                      const DenseMatrix<int_max>&     KNNBasisIndexList,
                                      const DenseMatrix<ElementType>& GramianMatrix_DtD)
{
    auto KNNBasisNumber = KNNBasisIndexList.GetElementNumber();

    Option_Of_LinearLeastSquaresProblemSolver Option;

    Solution_Of_LinearLeastSquaresProblem<ElementType> Solution;

    if (m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative == false && m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne == false)
    {
        Option.MethodName = "Normal";

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       nullptr, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);
    }
    else if (m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative == true && m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne == false)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(0);

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        DenseMatrix<ElementType> A;

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative == true && m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne == true)
    {
        DenseMatrix<ElementType> lb_x(KNNBasisNumber, 1);
        lb_x.Fill(ElementType(0));

        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                        &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                        &H, &Option);
    }
    else //if(m_Parameter.ParameterOfKNNReconstruction.CodeNonnegative == false && m_Parameter.ParameterOfKNNReconstruction.CodeSumToOne == true)
    {
        DenseMatrix<ElementType> A(1, KNNBasisNumber);
        A.Fill(ElementType(1));

        DenseMatrix<ElementType> lb_A = ElementType(1);
        DenseMatrix<ElementType> ub_A = ElementType(1);

        Option.MethodName = "QuadraticProgramming";

        DenseMatrix<ElementType> H;

        if (GramianMatrix_DtD.IsEmpty() == false)
        {
            H = GramianMatrix_DtD.GetSubMatrix(KNNBasisIndexList, KNNBasisIndexList);
        }

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                        nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                        &H, &Option);
    }

    MatrixMultiply(ReconstructedDataVector, KNNBasisMatrix, Solution.X);
}


}// namespace mdk


#endif