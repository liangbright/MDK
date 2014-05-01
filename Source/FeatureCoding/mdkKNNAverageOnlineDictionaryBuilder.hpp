#ifndef __mdkKNNAverageOnlineDictionaryBuilder_hpp
#define __mdkKNNAverageOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNAverageOnlineDictionaryBuilder<ElementType>::KNNAverageOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNAverageOnlineDictionaryBuilder<ElementType>::~KNNAverageOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNAverageOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNAverageOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNAverageOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("NumberOfDataInEachBatch > TotalDataNumber @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("NumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L1Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L2Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::Correlation
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_L1 <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_L2 <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::Correlation)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_KL <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.weigth_past < 0 || m_Parameter.weigth_past > 1)
    {
        MDK_Error("m_Parameter.weigth_past < 0 || m_Parameter.weigth_past > 1 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.weigth_s < 0 || m_Parameter.weigth_s > 1)
    {
        MDK_Error("m_Parameter.weigth_s < 0 || m_Parameter.weigth_s > 1 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    this->SetputParameter();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

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
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetputParameter()
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdateDictionaryAfterEncoding(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdateDictionary_OtherInfo(FeatureDictionaryForSparseCoding<ElementType>& Dictionary)
{
    //---------------------- already updated ------------------------------------

    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();

    //---------------------- to be updated ------------------------------------

    DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    DenseMatrix<ElementType>& BasisRedundancy = Dictionary.BasisRedundancy();

    //-------------------------------------------------------------------------

    DenseMatrix<ElementType> VarianceList;

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
    {
    case SimilarityTypeEnum::L1Distance:
        VarianceList.Share(VarianceOfL1Distance);
        break;

    case SimilarityTypeEnum::L2Distance:
        VarianceList.Share(VarianceOfL2Distance);
        break;

    case SimilarityTypeEnum::KLDivergence:
        VarianceList.Share(VarianceOfKLDivergence);
        break;

    default:
        MDK_Error("unknown SimilarityType @ KNNAverageOnlineDictionaryBuilder::UpdateDictionary_OtherInfo()")
        return;
    }

    this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix, VarianceList);

    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    

    Dictionary.SetInfo_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                                                                                    const DenseMatrix<ElementType>& FeatureData,
                                                                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                                    const DenseMatrix<ElementType> ExperienceOfRepresentingData)
{
    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Basis_Basis_InnerProductList(1, BasisNumber);
    for (int_max k = 0; k < BasisNumber; ++k)
    {
        auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(k);
        Basis_Basis_InnerProductList[k] = ComputeInnerProductOfTwoVectors(BasisVectorPtr, BasisVectorPtr, VectorLength, false);
    }

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    // temp function for basis update

    auto TempFunction_BasisUpdate = [](ElementType* BasisVector, const ElementType* DataVector, int_max VectorLength,
                                       ElementType factor,  ElementType PreviousExperienceOfBasis)
    {
        for (int_max m = 0; m < VectorLength; ++m)
        {
            auto temp_a = (factor / (factor*factor + 1))*DataVector[k];

            auto temp_b = (factor*factor / (factor*factor + 1))*BasisVector[k];

            BasisVector[k] += (temp_a - temp_b) / PreviousExperienceOfBasis;
        }
    };

    for (int_max k = 0; k < DataNumber; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const std::vector<int_max>& BasisIndexList = CodeTable[k].IndexList();

        for (int_max n = 0; n < int_max(BasisIndexList.size()); ++n)//n
        {
            auto BasisIndex = BasisIndexList[n];

            auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

            auto Basis_Data_InnerProduct = ComputeInnerProductOfTwoVectors(BasisVectorPtr, DataVectorPtr, VectorLength, false);

            auto Basis_Basis_InnerProduct = Basis_Basis_InnerProductList[BasisIndex];

            if (Basis_Basis_InnerProduct > eps_value && Basis_Basis_InnerProduct > eps_value*Basis_Data_InnerProduct)
            {
                auto factor = Basis_Data_InnerProduct / Basis_Basis_InnerProduct;

                auto PreviousExperienceOfBasis = ExperienceOfRepresentingData[BasisIndex];

                TempFunction_BasisUpdate(BasisVectorPtr, DataVectorPtr, VectorLength, factor, PreviousExperienceOfBasis);
            }            
        }        
    }

    this->ApplyConstraintOnBasis(BasisMatrix);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix)
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix,
                       const DenseMatrix<ElementType>& BasisMatrix,
                       const DenseMatrix<ElementType>& VarianceList)
{
    int_max BasisNumber = BasisMatrix.GetColNumber();
    int_max VectorLength = BasisMatrix.GetRowNumber();

    SimilarityMatrix.FastResize(BasisNumber, BasisNumber);

    for (int_max k = 0; k < BasisNumber - 1; ++k)
    {
        auto BasisVectorPtr_k = BasisMatrix.GetElementPointerOfCol(k);

        for (int_max n = k + 1; n < BasisNumber; ++n) // start from k+1
        {
            auto BasisVectorPtr_n = BasisMatrix.GetElementPointerOfCol(n);

            auto Variance = std::max(VarianceList[k], VarianceList[n]);

            auto Similarity = this->ComputeSimilarityBetweenTwoDataVectors(BasisVectorPtr_k, BasisVectorPtr_n, VectorLength, Variance);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    }
}


template<typename ElementType>
inline
ElementType KNNAverageOnlineDictionaryBuilder<ElementType>::
ComputeSimilarityBetweenTwoDataVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, ElementType Variance)
{
    ElementType Similarity = ElementType(0);

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
    {
    case SimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L1Distance*L1Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case SimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L2Distance*L2Distance) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case SimilarityTypeEnum::Correlation:
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB, Length, false);
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            Similarity = std::abs(Correlation);
        }
        else
        {
            Similarity = (Correlation + ElementType(1)) / ElementType(2);
        }
    }
        break;

    case SimilarityTypeEnum::KLDivergence:
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB, Length, false);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA, Length, false);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = (KLDivergence*KLDivergence) / Variance;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ KNNAverageOnlineDictionaryBuilder::ComputeSimilarityBetweenTwoDataVectors(...)")
    }

    return Similarity;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
{
    int_max BasisNumber = SimilarityMatrix.GetElementNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThresholdToComputeBasisRedundancy;

    for (int_max k = 0; k < BasisNumber; ++k)
    {
        BasisRedundancy[k] = 0;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            if (k != n)
            {
                if (SimilarityMatrix(n, k) >= SimilarityThreshold)
                {
                    BasisRedundancy[k] += 1;
                }
            }
        }
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
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

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto L1Distance = ComputeL1DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += L1Distance*L1Distance;

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
        MDK_Warning("MeanStd <= eps_value @ KNNAverageOnlineDictionaryBuilder::UpdateVarianceOfL1Distance(...)"
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
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
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {        
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += SquaredL2Distance;

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
        MDK_Warning("MeanStd <= eps_value @ KNNAverageOnlineDictionaryBuilder::UpdateVarianceOfL2Distance(...)"
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
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
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVectorPtr, BasisVectorPtr, VectorLength, false);

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
        MDK_Warning("MeanStd <= eps_value @ KNNAverageOnlineDictionaryBuilder::UpdateVarianceOfKLDivergence(...)"
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
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
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> ReconstructedDataVector(1, VectorLength);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            this->ReconstructDataVectorByKNNBasisMatrix(ReconstructedDataVector, DataVector, KNNBasisMatrix, KNN_IndexList, GramianMatrix_DtD);

            auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVector.GetElementPointer(), 
                                                                               ReconstructedDataVector.GetElementPointer(),
                                                                               VectorLength, false);

            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += SquaredL2Distance;

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
        MDK_Warning("MeanStd <= eps_value @ KNNAverageOnlineDictionaryBuilder::UpdateVarianceOfReconstruction(...)"
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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector, 
                                      const DenseMatrix<ElementType>& DataVector,
                                      const DenseMatrix<ElementType>& KNNBasisMatrix,                                     
                                      const std::vector<int_max>&     KNNBasisIndexList,
                                      const DenseMatrix<ElementType>& GramianMatrix_DtD)
{
    auto KNNBasisNumber = int_max(KNNBasisIndexList.size());

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