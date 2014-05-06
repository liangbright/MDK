#ifndef __mdkKNNReconstructionOnlineDictionaryBuilder_hpp
#define __mdkKNNReconstructionOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNReconstructionOnlineDictionaryBuilder<ElementType>::KNNReconstructionOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNReconstructionOnlineDictionaryBuilder<ElementType>::~KNNReconstructionOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::ClearPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNReconstructionOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNReconstructionOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNReconstructionOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_InitialDictionary == nullptr)
    {
        MDK_Error("InitialDictionary is empty (nullptr) @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_InitialDictionary->IsEmpty() == true)
    {
        MDK_Error("InitialDictionary is empty @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
    {
        MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("MaxNumberOfDataInEachBatch > TotalDataNumber @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch < m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber)
    {
        MDK_Error("MaxNumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNReconstruction.SimilarityType != VectorSimilarityTypeEnum::L1Distance
        && m_Parameter.ParameterOfKNNReconstruction.SimilarityType != VectorSimilarityTypeEnum::L2Distance
        && m_Parameter.ParameterOfKNNReconstruction.SimilarityType != VectorSimilarityTypeEnum::Correlation
        && m_Parameter.ParameterOfKNNReconstruction.SimilarityType != VectorSimilarityTypeEnum::AbsoluteValueOfCorrelation
        && m_Parameter.ParameterOfKNNReconstruction.SimilarityType != VectorSimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber != m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber)
    {
        MDK_Warning("NeighbourNumber for KNNSoftAssign is NOT equal to NeighbourNumber for KNNReconstruction" << '\n'
                    << "NeighbourNumber for KNNSoftAssign is set to NeighbourNumber for KNNReconstruction" << '\n'                    
                    << "@ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = m_Parameter.ParameterOfKNNReconstruction.NeighbourNumber;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != m_Parameter.ParameterOfKNNReconstruction.SimilarityType)
    {
        MDK_Warning("SimilarityType for KNNSoftAssign is NOT equal to SimilarityType for KNNReconstruction" << '\n'
                    << "SimilarityType for KNNSoftAssign is set to SimilarityType for KNNReconstruction" << '\n'                    
                    << "@ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = m_Parameter.ParameterOfKNNReconstruction.SimilarityType;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("SimilarityThreshold <= 0 @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL1Distance is empty in InitialDictionary @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
            return false;
        }       
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL2Distance is empty in InitialDictionary @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == true)
        {
            MDK_Error("VarianceOfKLDivergence is empty in InitialDictionary @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
            return false;
        }
    }

    if (m_Parameter.ExperienceDiscountFactor < 0 || m_Parameter.ExperienceDiscountFactor > 1)
    {
        MDK_Error("ExperienceDiscountFactor < 0 or > 1 @ KNNReconstructionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    return true;
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    this->SetupParameter();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary = this->CopyInitialDictionaryAndDiscountBasisExperience();

    DenseMatrix<ElementType>& BasisMatrix = OutputDictionary.BasisMatrix();

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    //----------- encode data and update dictionary ----------------------------//

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {
        m_KNNReconstructionSparseEncoder.SetInputFeatureData(m_FeatureData);
        m_KNNReconstructionSparseEncoder.SetInputDictionary(&OutputDictionary);
        m_KNNReconstructionSparseEncoder.Update();
        auto CodeTable = m_KNNReconstructionSparseEncoder.GetOutputCodeInSparseColVectorSet();

        this->UpdateDictionary(OutputDictionary, *m_FeatureData, *CodeTable);
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//
        // do not re-use data

        // random number for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

        DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);
        DataFlagList.Fill(1);
        // 1 : not used yet
        // 0 : used

        DenseMatrix<ElementType> FeatureData_current;

        DenseMatrix<ElementType> ReconstructedData_current;

        int_max NumberOfDataInNextBatch = m_Parameter.MaxNumberOfDataInEachBatch;

        int_max TotalDataNumber_used = 0;

        while (true)
        {
            // sample a subset from m_FeatureData

            FeatureData_current.FastResize(m_FeatureData->GetRowNumber(), NumberOfDataInNextBatch);

            int_max SampleCounter = 0;
            while (true)
            {
                auto DataIndex = UniformRandomNumber(gen);
                if (DataFlagList[DataIndex] == 1)
                {
                    FeatureData_current.SetCol(SampleCounter, m_FeatureData->GetElementPointerOfCol(DataIndex));

                    DataFlagList[DataIndex] = 0;

                    SampleCounter += 1;
                    if (SampleCounter >= NumberOfDataInNextBatch)
                    {
                        break;
                    }
                }
            }

            m_KNNReconstructionSparseEncoder.SetInputFeatureData(&FeatureData_current);
            m_KNNReconstructionSparseEncoder.SetInputDictionary(&OutputDictionary);
            m_KNNReconstructionSparseEncoder.Update();
            auto CodeTable = m_KNNReconstructionSparseEncoder.GetOutputCodeInSparseColVectorSet();

            this->UpdateDictionary(OutputDictionary, FeatureData_current, *CodeTable);

            // update NumberOfDataInNextBatch

            TotalDataNumber_used += SampleCounter;

            NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

            if (NumberOfDataInNextBatch <= 0)
            {
                break;
            }
        }
    }

    this->UpdateDictionary_OtherInformation(OutputDictionary);

    (*m_Dictionary) = std::move(OutputDictionary);  
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::SetupParameter()
{
    m_KNNReconstructionSparseEncoder.m_Parameter = m_Parameter.ParameterOfKNNReconstruction;
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNReconstructionOnlineDictionaryBuilder<ElementType>::CopyInitialDictionaryAndDiscountBasisExperience()
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    OutputDictionary.Copy(m_InitialDictionary); // m_InitialDictionary is not empty

    DenseMatrix<ElementType>& BasisExperience = OutputDictionary.BasisExperience();

    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // must >= 1
    for (int_max k = 0; k < BasisExperience.GetElementNumber(); k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }

    return OutputDictionary;
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                 const DenseMatrix<ElementType>& FeatureData,
                 const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    // copy initial BasisExperience from Dictionary
    DenseMatrix<ElementType> BasisExperience_init;
    BasisExperience_init.Copy(Dictionary.BasisExperience());

    // get ReconstructedData
    auto ReconstructedData = KNNReconstructionSparseEncoder<ElementType>::ReconstructData(CodeTable, Dictionary.BasisMatrix(), 
                                                                                          m_Parameter.MaxNumberOfThreads);

    // update BasisMatrix and BasisExperience

    this->UpdateBasisMatrixAndBasisExperience(Dictionary.BasisMatrix(), Dictionary.BasisExperience(), FeatureData, CodeTable, ReconstructedData);

    //SeedForNewBasisIDGeneration has been copied (see GenerateDictionary)

    // do not need to update BasisID : No new basis is added, and not old basis is retired
    //DenseMatrix<ElementType>& BasisID = Dictionary.BasisID();

    // update BasisAge -------------------------

    int_max DataNumber = FeatureData.GetColNumber();

    Dictionary.BasisAge() += DataNumber;

    // update SimilarityMatrix and BasisRedundancy in UpdateDictionary_OtherInformation(...)
    //DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    //DenseMatrix<ElementType>& BasisRedundancy = Dictionary->BasisRedundancy();

    //-------- update Variance ------------------------------------------------------------

    this->UpdateVarianceOfL1Distance(Dictionary.VarianceOfL1Distance(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);

    this->UpdateVarianceOfL2Distance(Dictionary.VarianceOfL2Distance(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);

    this->UpdateVarianceOfReconstruction(Dictionary.VarianceOfReconstruction(), FeatureData, CodeTable, ReconstructedData, BasisExperience_init);
    
    if (m_Parameter.ParameterOfKNNReconstruction.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(Dictionary.VarianceOfKLDivergence(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);
    }
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary)
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

    switch (m_Parameter.ParameterOfKNNReconstruction.SimilarityType)
    {
    case VectorSimilarityTypeEnum::L1Distance:
        VarianceList.Share(VarianceOfL1Distance);
        break;

    case VectorSimilarityTypeEnum::L2Distance:
        VarianceList.Share(VarianceOfL2Distance);
        break;

    case VectorSimilarityTypeEnum::KLDivergence:
        VarianceList.Share(VarianceOfKLDivergence);
        break;

    default:
        MDK_Error("unknown SimilarityType @ KNNReconstructionOnlineDictionaryBuilder::UpdateDictionary_OtherInfo()")
        return;
    }

    this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix, VarianceList);

    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    

    Dictionary.SetProperty_SimilarityType(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);

    Dictionary.SetProperty_SimilarityThresholdForComputingBasisRedundancy(m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy);
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateBasisMatrixAndBasisExperience(DenseMatrix<ElementType>&       BasisMatrix,
                                    DenseMatrix<ElementType>&       BasisExperience,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& ReconstructedData)
{
    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> DataReconstructionErrorVector(1, VectorLength);

    //-------------- temp function for basis update ---------------

    auto TempFunction_BasisUpdate = [](ElementType* BasisVector, const ElementType* DataReconstructionErrorVector, int_max Length,
                                       ElementType  ElementOfCodeVector, ElementType SquaredL2NormOfCodeVector,
                                       ElementType  ExperienceOfBasis)
    {
        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        auto temp = ElementOfCodeVector / (SquaredL2NormOfCodeVector + eps_value);

        if (temp > eps_value)
        {
            for (int_max k = 0; k < Length; ++k)
            {
                BasisVector[k] += temp * DataReconstructionErrorVector[k] / ExperienceOfBasis;
            }
        }
    };

    for (int_max k = 0; k < DataNumber; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const std::vector<int_max>& BasisIndexList = CodeTable[k].IndexList();

        const std::vector<ElementType>& CodeVector = CodeTable[k].DataArray();

        int_max NeighbourNumber_k = int_max(BasisIndexList.size());

        if (NeighbourNumber_k > 0)
        {
            auto ReconstructedDataVectorPtr = ReconstructedData.GetElementPointerOfCol(k);

            MatrixSubtract(DataReconstructionErrorVector.GetElementPointer(), DataVectorPtr, ReconstructedDataVectorPtr, VectorLength, false);

            auto SquaredL2NormOfCodeVector = ComputeInnerProductOfTwoVectors(CodeVector.data(), CodeVector.data(), NeighbourNumber_k, false);

            for (int_max n = 0; n < NeighbourNumber_k; ++n)//n
            {
                auto BasisIndex = BasisIndexList[n];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                TempFunction_BasisUpdate(BasisVectorPtr, DataReconstructionErrorVector.GetElementPointer(), VectorLength,
                                         CodeVector[n], SquaredL2NormOfCodeVector,
                                         BasisExperience[BasisIndex]);

                BasisExperience[BasisIndex] += CodeVector[n] * CodeVector[n] / SquaredL2NormOfCodeVector;
            }
        }
    }

    this->ApplyConstraintOnBasis(BasisMatrix);

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix)
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
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix,
                       const DenseMatrix<ElementType>& BasisMatrix,
                       const DenseMatrix<ElementType>& VarianceList)
{
    int_max BasisNumber = BasisMatrix.GetColNumber();
    int_max VectorLength = BasisMatrix.GetRowNumber();

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    SimilarityMatrix.FastResize(BasisNumber, BasisNumber);

    //for (int_max k = 0; k <= BasisNumber - 2; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)    
    {
        auto BasisVectorPtr_k = BasisMatrix.GetElementPointerOfCol(k);

        for (int_max n = k + 1; n < BasisNumber; ++n) // start from k+1
        {
            auto BasisVectorPtr_n = BasisMatrix.GetElementPointerOfCol(n);

            auto Variance = std::max(VarianceList[k], VarianceList[n]);

            auto Similarity = KNNSoftAssignSparseEncoder<ElementType>::ComputeSimilarityBetweenTwoVectors(BasisVectorPtr_k, BasisVectorPtr_n, VectorLength, 
                                                                                                          SimilarityType,Variance, false);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber - 2, m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
{
    int_max BasisNumber = SimilarityMatrix.GetColNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold_For_ComputingBasisRedundancy;

    //for (int_max k = 0; k <= BasisNumber-1; ++k)
    auto TempFunction_UpdateRedundancy = [&](int_max k)
    {
        BasisRedundancy[k] = 0;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            if (k != n)
            {
                if (SimilarityMatrix(n, k) >= SimilarityThreshold)
                {
                    BasisRedundancy[k] += SimilarityMatrix(n, k);
                }
            }
        }
    };

    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(0);

    DenseMatrix<ElementType> WeightList(1, BasisNumber);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

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

                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(0);

    DenseMatrix<ElementType> WeightList(1, BasisNumber);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

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

                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                             const DenseMatrix<ElementType>& FeatureData,
                             const DataContainer<SparseVector<ElementType>>& CodeTable,
                             const DenseMatrix<ElementType>& BasisMatrix,
                             const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(0);

    DenseMatrix<int_max> WeightList(1, BasisNumber);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

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

                WeightList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNReconstructionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                               const DenseMatrix<ElementType>& FeatureData,
                               const DataContainer<SparseVector<ElementType>>& CodeTable,
                               const DenseMatrix<ElementType>& ReconstructedData,
                               const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisExperience.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(0);

    DenseMatrix<ElementType> WeightList(1, BasisNumber);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            auto ReconstructedDataVectorPtr = ReconstructedData.GetElementPointerOfCol(k);

            auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVectorPtr, ReconstructedDataVectorPtr, VectorLength, false);

            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += SquaredL2Distance;

                WeightList[BasisIndex] += 1;
            }
        }
       
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
    }
}

}// namespace mdk


#endif