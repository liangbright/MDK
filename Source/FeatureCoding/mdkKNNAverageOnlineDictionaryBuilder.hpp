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

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::ClearPipelineOutput()
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
        MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("MaxNumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfInterations <= 0)
    {
        MDK_Error("MaxNumberOfInterations <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    auto IsSimilarityTypeSupported = KNNSoftAssignSparseEncoder<ElementType>::
                                     CheckIfSimilarityTypeSupported(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);    

    if (IsSimilarityTypeSupported == false)
    {
        MDK_Error("SimilarityType is not supported @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("SimilarityThreshold <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL1Distance in InitialDictionary is empty <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
            return false;            
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL2Distance in InitialDictionary is empty @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == true)
        {
            MDK_Error("VarianceOfKLDivergence in InitialDictionary is empty 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
            return false;
        }
    }

    if (m_Parameter.ExperienceDiscountFactor < 0 || m_Parameter.ExperienceDiscountFactor > 1)
    {
        MDK_Error("ExperienceDiscountFactor < 0 or > 1 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    return true;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    this->SetupParameter();

    auto OutputDictionary = this->PreprocessInitialDictionary(*m_InitialDictionary);

    DenseMatrix<ElementType> BasisExperience_init;
    BasisExperience_init.Copy(OutputDictionary.BasisExperience());

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    //----------- encode data and update dictionary ----------------------------//

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {
        for (int_max k = 0; k < m_Parameter.MaxNumberOfInterations; ++k)
        {
            m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);
            m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
            m_KNNSoftAssignSparseEncoder.Update();
            auto CodeTable = m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet();

            this->UpdateDictionary(OutputDictionary, *m_FeatureData, *CodeTable);
        }
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//
        
        // random number for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

        for (int_max k = 0; k < m_Parameter.MaxNumberOfInterations; ++k)
        {

            DenseMatrix<int_max> DataFlagList(1, TotalDataNumber);
            DataFlagList.Fill(1);
            // 1 : not used yet
            // 0 : used

            DenseMatrix<ElementType> FeatureData_current;

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

                m_KNNSoftAssignSparseEncoder.SetInputFeatureData(&FeatureData_current);
                m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
                m_KNNSoftAssignSparseEncoder.Update();
                auto CodeTable = m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet();

                this->UpdateDictionary(OutputDictionary, FeatureData_current, *CodeTable);

                // update NumberOfDataInNextBatch

                TotalDataNumber_used += SampleCounter;

                NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

                if (NumberOfDataInNextBatch <= 0)
                {
                    break;
                }
            }

            // do need to use this->UpdateDictionary_OtherInformation(...)
            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), BasisExperience_init, TotalDataNumber);
        }
    }

    this->UpdateDictionary_OtherInformation(OutputDictionary, BasisExperience_init, TotalDataNumber);

    (*m_Dictionary) = std::move(OutputDictionary);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetupParameter()
{
    m_KNNSoftAssignSparseEncoder.m_Parameter = m_Parameter.ParameterOfKNNSoftAssign;   

    m_KNNSoftAssignSparseEncoder.SetMaxNumberOfThreads(m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNAverageOnlineDictionaryBuilder<ElementType>::PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InitialDictionary)
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    OutputDictionary.Copy(InitialDictionary);

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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                 const DenseMatrix<ElementType>& FeatureData,
                 const DataArray<SparseVector<ElementType>>& CodeTable)
{
    //------------------------------------------------------------------------------
    // Input:
    // Dictionary is the initial dictionary
    // CodeTable is the soft-assign code of FeatureData using the initial dictionary
    //
    // Output:
    // Dictionary : it will be updated in this function
    //-------------------------------------------------------------------------------

    // copy the initial BasisExperience
    DenseMatrix<ElementType> BasisExperience_init;
    BasisExperience_init.Copy(Dictionary.BasisExperience());

    // update BasisMatrix and BasisExperience ----------------------------------------

    this->UpdateBasisMatrixAndBasisExperience(Dictionary.BasisMatrix(), Dictionary.BasisExperience(), FeatureData, CodeTable);
    
    // update other information ---------------------------------------------------------------------------------

    // SeedForNewBasisIDGeneration has been copied in this->CopyInitialDictionaryAndDiscountBasisExperience(...)

    // BasisID does not need to be updated : No new basis is added, and not old basis is retired

    // BasisAge updated in this->UpdateDictionary_OtherInformation(...)

    // SimilarityMatrix and BasisRedundancy updated in this->UpdateDictionary_OtherInformation(...)

    //-------- update Variance -----------------------------------------------------------------------------------

    this->UpdateVarianceOfL1Distance(Dictionary.VarianceOfL1Distance(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);
    
    this->UpdateVarianceOfL2Distance(Dictionary.VarianceOfL2Distance(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);

    if (m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
    {
        this->UpdateVarianceOfReconstruction(Dictionary.VarianceOfReconstruction(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(Dictionary.VarianceOfKLDivergence(), FeatureData, CodeTable, Dictionary.BasisMatrix(), BasisExperience_init);
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                  const DenseMatrix<ElementType>& BasisExperience_init,
                                  int_max TotalDataNumber)
{
    //---------------------- already updated ------------------------------------

    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    //-------------------------- update BasisAge -----------------------------

    if (m_Parameter.Flag_Update_BasisAge == true)
    {
        Dictionary.BasisAge() += TotalDataNumber;
    }

    //----------------- adjust BasisExperience if Data is re-used ------------

    this->AdjustBasisExperience(Dictionary.BasisExperience(), BasisExperience_init, TotalDataNumber);

    //---------------------- to be updated ------------------------------------

    DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    DenseMatrix<ElementType>& BasisRedundancy = Dictionary.BasisRedundancy();

    //-------------------------------------------------------------------------

    DenseMatrix<ElementType> VarianceList;

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
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
        VarianceList.FastResize(1, BasisNumber);
        VarianceList.Fill(0);
        break;
    }

    this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix, VarianceList);

    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    

    Dictionary.SetProperty_SimilarityType(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);

    Dictionary.SetProperty_SimilarityThresholdForComputeBasisRedundancy(m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisMatrixAndBasisExperience(DenseMatrix<ElementType>&       BasisMatrix,
                                    DenseMatrix<ElementType>&       BasisExperience,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataArray<SparseVector<ElementType>>& CodeTable)
{
    if (m_Parameter.WhetherToUseScaleFactor == true)
    {
        this->UpdateBasisMatrixAndBasisExperience_UseScaleFactor(BasisMatrix, BasisExperience, FeatureData, CodeTable);
    }
    else
    {
        this->UpdateBasisMatrixAndBasisExperience_NoScaleFactor(BasisMatrix, BasisExperience, FeatureData, CodeTable);
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ElementType>&       BasisMatrix,
                                                   DenseMatrix<ElementType>&       BasisExperience,
                                                   const DenseMatrix<ElementType>& FeatureData,
                                                   const DataArray<SparseVector<ElementType>>& CodeTable)
{
    // temp function for basis_change update

    auto TempFunction_UpdateBasisChange = [](ElementType* BasisChange, const ElementType* BasisVector, const ElementType* DataVector, int_max VectorLength,
                                             ElementType CodeElement,  ElementType SquaredL2NormOfReconstructionCodeVector)
    {
        auto eps_value = std::numeric_limits<ElementType>::epsilon();

        auto temp = CodeElement / (SquaredL2NormOfReconstructionCodeVector + eps_value);

        if (temp > eps_value)
        {
            for (int_max k = 0; k < VectorLength; ++k)
            {
                auto Error = DataVector[k] - BasisVector[k] * CodeElement;

                BasisChange[k] += temp * Error;
            }
        }
    };

    // temp function for basis update
    //
    //auto TempFunction_UpdateBasis = [](ElementType* BasisVector, const ElementType* DataVector, int_max VectorLength,
    //                                   ElementType CodeElement, ElementType SquaredL2NormOfReconstructionCodeVector,
    //                                   ElementType ExperienceOfBasis)
    //{
    //    auto temp = CodeElement / SquaredL2NormOfReconstructionCodeVector;
    //
    //    for (int_max k = 0; k < VectorLength; ++k)
    //    {
    //        auto Error = DataVector[k] - BasisVector[k] * CodeElement;
    //
    //        BasisVector[k] += temp * Error / ExperienceOfBasis;
    //    }
    //};
    

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

    DenseMatrix<ElementType> BasisMatrix_Change(VectorLength, BasisNumber);
    BasisMatrix_Change.Fill(0);

    DenseMatrix<ElementType> ReconstructionCodeVector;

    for (int_max k = 0; k < DataNumber; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const std::vector<int_max>& KNNBasisIndexList = CodeTable[k].IndexList();

        int_max KNNBasisNumber = int_max(KNNBasisIndexList.size());

        if (KNNBasisNumber > 0)
        {
            ReconstructionCodeVector.FastResize(KNNBasisNumber, 1);
            // compute reconstruction code Vector
            //
            // use KNN-basis to reconstruct each Data vector (pretend we are doing KNN-reconstruction)
            // reconstruct each Data vector with only one basis
            // 
            // ReconstructionError[n] = ||DataVector - ReconstructionCodeVector[n]*BasisVector||
            //

            for (int_max n = 0; n < KNNBasisNumber; ++n)//n
            {
                auto BasisIndex = KNNBasisIndexList[n];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto Basis_Data_InnerProduct = ComputeInnerProductOfTwoVectors(BasisVectorPtr, DataVectorPtr, VectorLength, false);

                auto Basis_Basis_InnerProduct = Basis_Basis_InnerProductList[BasisIndex];

                if (Basis_Basis_InnerProduct > eps_value && Basis_Basis_InnerProduct > eps_value*Basis_Data_InnerProduct)
                {
                    ReconstructionCodeVector[n] = Basis_Data_InnerProduct / Basis_Basis_InnerProduct;
                }
                else
                {
                    ReconstructionCodeVector[n] = 0;
                }
            }

            auto SquaredL2NormOfReconstructionCodeVector = ComputeInnerProductOfTwoVectors(ReconstructionCodeVector.GetElementPointer(),
                                                                                           ReconstructionCodeVector.GetElementPointer(), 
                                                                                           KNNBasisNumber, false);

            if (SquaredL2NormOfReconstructionCodeVector > eps_value)
            {
                for (int_max n = 0; n < KNNBasisNumber; ++n)//n
                {
                    auto BasisIndex = KNNBasisIndexList[n];

                    if (SquaredL2NormOfReconstructionCodeVector > eps_value*Basis_Basis_InnerProductList[BasisIndex])
                    {
                        auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                        auto BasisVectorChangePtr = BasisMatrix_Change.GetElementPointerOfCol(BasisIndex);

                        auto CodeElement = ReconstructionCodeVector[n];

                        if (CodeElement > eps_value)
                        {
                            TempFunction_UpdateBasisChange(BasisVectorChangePtr, BasisVectorPtr, DataVectorPtr, VectorLength,
                                                           CodeElement, SquaredL2NormOfReconstructionCodeVector);
                            
                            //TempFunction_UpdateBasis(BasisVectorPtr, DataVectorPtr, VectorLength,
                            //                         CodeElement, SquaredL2NormOfReconstructionCodeVector,
                            //                         BasisExperience[BasisIndex]);
                            
                            BasisExperience[BasisIndex] += CodeElement*CodeElement / SquaredL2NormOfReconstructionCodeVector;

                        }// else CodeElement = 0, no update
                    }
                }
            }
        }
    }

    
    // update Basis
    //for (int_max k = 0; k <= BasisNumber-1; ++k)
    auto TempFunction_UpdateBasisMatrix = [&](int_max k)
    {
        auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(k);

        auto BasisVectorChangePtr = BasisMatrix_Change.GetElementPointerOfCol(k);
        
        auto Experience = BasisExperience[k];

        for (int_max n = 0; n < VectorLength; ++n)
        {
            BasisVectorPtr[n] += BasisVectorChangePtr[n] / Experience;
        }
    };

    ParallelForLoop(TempFunction_UpdateBasisMatrix, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThreads);
    
    this->ApplyConstraintOnBasis(BasisMatrix);

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ElementType>&       BasisMatrix,
                                                  DenseMatrix<ElementType>&       BasisExperience,
                                                  const DenseMatrix<ElementType>& FeatureData,
                                                  const DataArray<SparseVector<ElementType>>& CodeTable)
{
    // temp function for basis_change update

    auto TempFunction_UpdateBasisChange = [](ElementType* BasisChange, const ElementType* BasisVector, const ElementType* DataVector, int_max VectorLength,
                                             ElementType Membership)
    {
        for (int_max k = 0; k < VectorLength; ++k)
        {
            auto Error = DataVector[k] - BasisVector[k];
            BasisChange[k] += Membership * Error;
        }
    };

    //------------------------------------------------------

    int_max DataNumber = FeatureData.GetColNumber();
    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    DenseMatrix<ElementType> BasisMatrix_Change(VectorLength, BasisNumber);
    BasisMatrix_Change.Fill(0);

    for (int_max k = 0; k < DataNumber; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const std::vector<int_max>& KNNBasisIndexList = CodeTable[k].IndexList();

        const std::vector<ElementType>& MembershipList = CodeTable[k].DataArray(); // code from SoftAssignSparseEncorder is membership, not raw Similarity

        int_max KNNBasisNumber = int_max(KNNBasisIndexList.size());

        if (KNNBasisNumber > 0)
        {
            for (int_max n = 0; n < KNNBasisNumber; ++n)//n
            {
                auto BasisIndex = KNNBasisIndexList[n];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto BasisVectorChangePtr = BasisMatrix_Change.GetElementPointerOfCol(BasisIndex);

                TempFunction_UpdateBasisChange(BasisVectorChangePtr, BasisVectorPtr, DataVectorPtr, VectorLength, MembershipList[n]);

                BasisExperience[BasisIndex] += MembershipList[n];
            }
        }
    }

    // update Basis
    //for (int_max k = 0; k <= BasisNumber-1; ++k)
    auto TempFunction_UpdateBasisMatrix = [&](int_max k)
    {
        auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(k);

        auto BasisVectorChangePtr = BasisMatrix_Change.GetElementPointerOfCol(k);

        auto Experience = BasisExperience[k];

        for (int_max n = 0; n < VectorLength; ++n)
        {
            BasisVectorPtr[n] += BasisVectorChangePtr[n] / Experience;
        }
    };

    ParallelForLoop(TempFunction_UpdateBasisMatrix, 0, BasisNumber - 1, m_Parameter.MaxNumberOfThreads);

    this->ApplyConstraintOnBasis(BasisMatrix);

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
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
AdjustBasisExperience(DenseMatrix<ElementType>& BasisExperience, const DenseMatrix<ElementType>& BasisExperience_init, int_max TotalDataNumber)
{
    int_max BasisNumber = BasisExperience.GetElementNumber();

    DenseMatrix<ElementType> BasisExperience_Diff = MatrixSubtract(BasisExperience, BasisExperience_init);

    ElementType TotalGain =BasisExperience_Diff.Sum();

    if (TotalGain > TotalDataNumber)
    {
        BasisExperience_Diff *= ElementType(TotalDataNumber) / TotalGain;

        MatrixAdd(BasisExperience, BasisExperience_init, BasisExperience_Diff);
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, const DenseMatrix<ElementType>& BasisMatrix, const DenseMatrix<ElementType>& VarianceList)
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
                                                                                                          SimilarityType, Variance, false);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber - 2, m_Parameter.MaxNumberOfThreads);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix)
{
    int_max BasisNumber = SimilarityMatrix.GetColNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy;

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
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataArray<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience_init)
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

        const std::vector<ElementType>& KNN_Membership = CodeTable[k].DataArray(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourNumber = int_max(KNN_IndexList.size());

        if (tempNeighbourNumber > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto L1Distance = ComputeL1DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += L1Distance*L1Distance*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
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
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataArray<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience_init)
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

        const std::vector<ElementType>& KNN_Membership = CodeTable[k].DataArray(); // code from SoftAssignSparseEncorder is Similarity, not membership 

        auto tempNeighbourNumber = int_max(KNN_IndexList.size());

        if (tempNeighbourNumber > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += SquaredL2Distance*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
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
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                             const DenseMatrix<ElementType>& FeatureData,
                             const DataArray<SparseVector<ElementType>>& CodeTable,
                             const DenseMatrix<ElementType>& BasisMatrix,
                             const DenseMatrix<ElementType>& BasisExperience_init)
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

        const std::vector<ElementType>& KNN_Membership = CodeTable[k].DataArray(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourNumber = int_max(KNN_IndexList.size());

        if (tempNeighbourNumber > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += KLDivergence*KLDivergence*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
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
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                               const DenseMatrix<ElementType>& FeatureData,
                               const DataArray<SparseVector<ElementType>>& CodeTable,
                               const DenseMatrix<ElementType>& BasisMatrix,
                               const DenseMatrix<ElementType>& BasisExperience_init)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    auto DataReconstructionErrorL2Norm = this->ComputeDataReconstructionErrorL2Norm(FeatureData, CodeTable, BasisMatrix);

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(0);

    DenseMatrix<ElementType> WeightList(1, BasisNumber);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        const std::vector<ElementType>& KNN_Membership = CodeTable[k].DataArray(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourNumber = int_max(KNN_IndexList.size());

        if (tempNeighbourNumber > 0)
        {
            for (int_max m = 0; m < tempNeighbourNumber; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k] * KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
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
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNAverageOnlineDictionaryBuilder<ElementType>::
ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                     const DataArray<SparseVector<ElementType>>& CodeTable,
                                     const DenseMatrix<ElementType>&  BasisMatrix)
{
    auto ReconstructionCodeSet = KNNReconstructionSparseEncoder<ElementType>::
                                 ComputeReconstructionCodeFromSimilarityCode(FeatureData,
                                                                             CodeTable,
                                                                             BasisMatrix,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne,
                                                                             m_Parameter.MaxNumberOfThreads);

    auto ErrorL2NormList = KNNReconstructionSparseEncoder<ElementType>::
                           ComputeReconstructionErrorL2Norm(FeatureData, ReconstructionCodeSet, BasisMatrix, m_Parameter.MaxNumberOfThreads);

    return ErrorL2NormList;
}



}// namespace mdk


#endif