#ifndef mdk_KNNAverageDictionaryBuilder_hpp
#define mdk_KNNAverageDictionaryBuilder_hpp


namespace mdk
{

template<typename ScalarType>
KNNAverageDictionaryBuilder<ScalarType>::KNNAverageDictionaryBuilder()
{
    this->Clear();
}


template<typename ScalarType>
KNNAverageDictionaryBuilder<ScalarType>::~KNNAverageDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
    m_FeatureData = nullptr;
    m_InitialDictionary = nullptr;
	m_Dictionary.Clear();
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>* KNNAverageDictionaryBuilder<ScalarType>::GetOutputDictionary()
{
    return &m_Dictionary;
}


template<typename ScalarType>
bool KNNAverageDictionaryBuilder<ScalarType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_InitialDictionary == nullptr)
    {
        MDK_Error("InitialDictionary is empty (nullptr) @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_InitialDictionary->IsEmpty() == true)
    {
        MDK_Error("InitialDictionary is empty @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

	if (m_FeatureData->GetRowCount() != m_InitialDictionary->BasisMatrix().GetRowCount())
    {
        MDK_Error("RowCount Of FeatureData != RowCount Of InitialDictionary @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

	if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourCount <= 0)
    {
        MDK_Error("NeighbourCount <= 0 @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

	if (m_Parameter.MaxCountOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourCount)
    {
        MDK_Error("MaxCountOfDataInEachBatch < MaxCountOfNeighbours @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxCountOfInteration <= 0)
    {
        MDK_Error("MaxCountOfInterations <= 0 @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    auto IsSimilarityTypeSupported = KNNSoftAssignSparseEncoder<ScalarType>::
                                     CheckIfSimilarityTypeSupported(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);    

    if (IsSimilarityTypeSupported == false)
    {
        MDK_Error("SimilarityType is not supported @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("SimilarityThreshold <= 0 @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL1Distance in InitialDictionary is empty <= 0 @ KNNAverageDictionaryBuilder::CheckInput()")
            return false;            
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == true)
        {
            MDK_Error("VarianceOfL2Distance in InitialDictionary is empty @ KNNAverageDictionaryBuilder::CheckInput()")
            return false;
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == true)
        {
            MDK_Error("VarianceOfKLDivergence in InitialDictionary is empty 0 @ KNNAverageDictionaryBuilder::CheckInput()")
            return false;
        }
    }

    if (m_Parameter.ExperienceDiscountFactor < 0 || m_Parameter.ExperienceDiscountFactor > 1)
    {
        MDK_Error("ExperienceDiscountFactor < 0 or > 1 @ KNNAverageDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    return true;
}


template<typename ScalarType>
bool KNNAverageDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}
	this->GenerateDictionary();
	return true;
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::GenerateDictionary()
{
    this->SetupParameter();

    auto OutputDictionary = this->PreprocessInitialDictionary(*m_InitialDictionary);

    DenseMatrix<ScalarType> BasisExperience_init;
    BasisExperience_init.Copy(OutputDictionary.BasisExperience());

    int_max TotalDataCount = m_FeatureData->GetColCount();

    //----------- encode data and update dictionary ----------------------------//

    if (m_Parameter.MaxCountOfDataInEachBatch >= TotalDataCount)
    {
        for (int_max k = 0; k < m_Parameter.MaxCountOfInteration; ++k)
        {
            m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);
            m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
            m_KNNSoftAssignSparseEncoder.Update();
            const auto& CodeTable = *m_KNNSoftAssignSparseEncoder.GetOutputCode();

            this->UpdateDictionary(OutputDictionary, *m_FeatureData, CodeTable);
        }
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//
        
        // random number for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> UniformRandomCount(0, TotalDataCount - 1);

        for (int_max k = 0; k < m_Parameter.MaxCountOfInteration; ++k)
        {

            DenseMatrix<int_max> DataFlagList(1, TotalDataCount);
            DataFlagList.Fill(1);
            // 1 : not used yet
            // 0 : used

            DenseMatrix<ScalarType> FeatureData_current;

            int_max CountOfDataInNextBatch = m_Parameter.MaxCountOfDataInEachBatch;

            int_max TotalDataCount_used = 0;

            while (true)
            {
                // sample a subset from m_FeatureData

                FeatureData_current.FastResize(m_FeatureData->GetRowCount(), CountOfDataInNextBatch);

                int_max SampleCounter = 0;
                while (true)
                {
                    auto DataIndex = UniformRandomCount(gen);
                    if (DataFlagList[DataIndex] == 1)
                    {
                        FeatureData_current.SetCol(SampleCounter, m_FeatureData->GetElementPointerOfCol(DataIndex));

                        DataFlagList[DataIndex] = 0;

                        SampleCounter += 1;
                        if (SampleCounter >= CountOfDataInNextBatch)
                        {
                            break;
                        }
                    }
                }

                m_KNNSoftAssignSparseEncoder.SetInputFeatureData(&FeatureData_current);
                m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
                m_KNNSoftAssignSparseEncoder.Update();
                const auto& CodeTable = *m_KNNSoftAssignSparseEncoder.GetOutputCode();

                this->UpdateDictionary(OutputDictionary, FeatureData_current, CodeTable);

                // update CountOfDataInNextBatch

                TotalDataCount_used += SampleCounter;

                CountOfDataInNextBatch = std::min(m_Parameter.MaxCountOfDataInEachBatch, TotalDataCount - TotalDataCount_used);

                if (CountOfDataInNextBatch <= 0)
                {
                    break;
                }
            }

            // do need to use this->UpdateDictionary_OtherInformation(...)
            this->AdjustBasisExperience(OutputDictionary.BasisExperience(), BasisExperience_init, TotalDataCount);
        }
    }

    this->UpdateDictionary_OtherInformation(OutputDictionary, BasisExperience_init, TotalDataCount);

    m_Dictionary = std::move(OutputDictionary);
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::SetupParameter()
{
    m_KNNSoftAssignSparseEncoder.m_Parameter = m_Parameter.ParameterOfKNNSoftAssign;   
	m_KNNSoftAssignSparseEncoder.SetMaxThreadCount(m_Parameter.MaxThreadCount);
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>
KNNAverageDictionaryBuilder<ScalarType>::PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitialDictionary)
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;

    OutputDictionary.Copy(InitialDictionary);

    DenseMatrix<ScalarType>& BasisExperience = OutputDictionary.BasisExperience();

    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // must >= 1
    for (int_max k = 0; k < BasisExperience.GetElementCount(); k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }

    return OutputDictionary;
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                 const DenseMatrix<ScalarType>& FeatureData,
                 const ObjectArray<SparseVector<ScalarType>>& CodeTable)
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
    DenseMatrix<ScalarType> BasisExperience_init;
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


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                  const DenseMatrix<ScalarType>& BasisExperience_init,
                                  int_max TotalDataCount)
{
    //---------------------- already updated ------------------------------------

    DenseMatrix<ScalarType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ScalarType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ScalarType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ScalarType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ScalarType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();

    int_max BasisCount = BasisMatrix.GetColCount();

    //-------------------------- update BasisAge -----------------------------

    if (m_Parameter.Flag_Update_BasisAge == true)
    {
        Dictionary.BasisAge() += TotalDataCount;
    }

    //----------------- adjust BasisExperience if Data is re-used ------------

    this->AdjustBasisExperience(Dictionary.BasisExperience(), BasisExperience_init, TotalDataCount);

    //---------------------- to be updated ------------------------------------

    DenseMatrix<ScalarType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    DenseMatrix<ScalarType>& BasisRedundancy = Dictionary.BasisRedundancy();

    //-------------------------------------------------------------------------

    DenseMatrix<ScalarType> VarianceList;

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
        VarianceList.FastResize(1, BasisCount);
        VarianceList.Fill(0);
        break;
    }

    this->UpdateSimilarityMatrix(SimilarityMatrix, BasisMatrix, VarianceList);

    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);    

    Dictionary.SetProperty_SimilarityType(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);

    Dictionary.SetProperty_SimilarityThresholdForComputeBasisRedundancy(m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy);
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateBasisMatrixAndBasisExperience(DenseMatrix<ScalarType>&       BasisMatrix,
                                    DenseMatrix<ScalarType>&       BasisExperience,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const ObjectArray<SparseVector<ScalarType>>& CodeTable)
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


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                   DenseMatrix<ScalarType>&       BasisExperience,
                                                   const DenseMatrix<ScalarType>& FeatureData,
                                                   const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{
    // temp function for basis_change update

    auto TempFunction_UpdateBasisChange = [](ScalarType* BasisChange, const ScalarType* BasisVector, const ScalarType* DataVector, int_max VectorLength,
                                             ScalarType CodeElement,  ScalarType SquaredL2NormOfReconstructionCodeVector)
    {
        auto eps_value = std::numeric_limits<ScalarType>::epsilon();

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
    //auto TempFunction_UpdateBasis = [](ScalarType* BasisVector, const ScalarType* DataVector, int_max VectorLength,
    //                                   ScalarType CodeElement, ScalarType SquaredL2NormOfReconstructionCodeVector,
    //                                   ScalarType ExperienceOfBasis)
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
    

    int_max DataCount = FeatureData.GetColCount();

    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    DenseMatrix<ScalarType> Basis_Basis_InnerProductList(1, BasisCount);
    for (int_max k = 0; k < BasisCount; ++k)
    {
        auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(k);
        Basis_Basis_InnerProductList[k] = ComputeInnerProductOfTwoVector(BasisVectorPtr, BasisVectorPtr, VectorLength, false);
    }

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    DenseMatrix<ScalarType> BasisMatrix_Change(VectorLength, BasisCount);
    BasisMatrix_Change.Fill(0);

	DenseVector<ScalarType> BasisExperience_Change;
	BasisExperience_Change.Resize(BasisCount);
	BasisExperience_Change.Fill(0);

    DenseMatrix<ScalarType> ReconstructionCodeVector;

    for (int_max k = 0; k < DataCount; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

		const auto& KNNBasisIndexList = CodeTable[k].IndexList();

        int_max KNNBasisCount = KNNBasisIndexList.GetLength();

        if (KNNBasisCount > 0)
        {
            ReconstructionCodeVector.FastResize(KNNBasisCount, 1);
            // compute reconstruction code Vector
            //
            // use KNN-basis to reconstruct each Data vector (pretend we are doing KNN-reconstruction)
            // reconstruct each Data vector with only one basis
            // 
            // ReconstructionError[n] = ||DataVector - ReconstructionCodeVector[n]*BasisVector||
            //

            for (int_max n = 0; n < KNNBasisCount; ++n)//n
            {
                auto BasisIndex = KNNBasisIndexList[n];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto Basis_Data_InnerProduct = ComputeInnerProductOfTwoVector(BasisVectorPtr, DataVectorPtr, VectorLength, false);

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

            auto SquaredL2NormOfReconstructionCodeVector = ComputeInnerProductOfTwoVector(ReconstructionCodeVector.GetElementPointer(),
                                                                                          ReconstructionCodeVector.GetElementPointer(), 
                                                                                          KNNBasisCount, false);

            if (SquaredL2NormOfReconstructionCodeVector > eps_value)
            {
                for (int_max n = 0; n < KNNBasisCount; ++n)//n
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
                            
                            //BasisExperience[BasisIndex] += CodeElement*CodeElement / SquaredL2NormOfReconstructionCodeVector;
							BasisExperience_Change[BasisIndex] += CodeElement*CodeElement / SquaredL2NormOfReconstructionCodeVector;

                        }// else CodeElement = 0, no update
                    }
                }
            }
        }
    }

	// update BasisExperience
	for (int_max k = 0; k <= BasisCount - 1; ++k)
	{
		BasisExperience[k] += BasisExperience_Change[k];
	}

    // update Basis
    //for (int_max k = 0; k <= BasisCount-1; ++k)
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
    ParallelForLoop(TempFunction_UpdateBasisMatrix, 0, BasisCount - 1, m_Parameter.MaxCountOfThread);
    
    this->ApplyConstraintOnBasis(BasisMatrix);

	

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataCount
    //
    // the new "Experience" of the dictionary gained from data is DataCount
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                  DenseMatrix<ScalarType>&       BasisExperience,
                                                  const DenseMatrix<ScalarType>& FeatureData,
                                                  const ObjectArray<SparseVector<ScalarType>>& CodeTable)
{
    // temp function for basis_change update

    auto TempFunction_UpdateBasisChange = [](ScalarType* BasisChange, const ScalarType* BasisVector, const ScalarType* DataVector, int_max VectorLength,
                                             ScalarType Membership)
    {
        for (int_max k = 0; k < VectorLength; ++k)
        {
            auto Error = DataVector[k] - BasisVector[k];
            BasisChange[k] += Membership * Error;
        }
    };

    //------------------------------------------------------

    int_max DataCount = FeatureData.GetColCount();
    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    DenseMatrix<ScalarType> BasisMatrix_Change(VectorLength, BasisCount);
    BasisMatrix_Change.Fill(0);

	DenseVector<ScalarType> BasisExperience_Change;
	BasisExperience_Change.Resize(BasisCount);
	BasisExperience_Change.Fill(0);

    for (int_max k = 0; k < DataCount; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const auto& KNNBasisIndexList = CodeTable[k].IndexList();

		const auto& MembershipList = CodeTable[k].ElementList(); // code from SoftAssignSparseEncorder is membership, not raw Similarity

        int_max KNNBasisCount = KNNBasisIndexList.GetLength();

        if (KNNBasisCount > 0)
        {
            for (int_max n = 0; n < KNNBasisCount; ++n)//n
            {
                auto BasisIndex = KNNBasisIndexList[n];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto BasisVectorChangePtr = BasisMatrix_Change.GetElementPointerOfCol(BasisIndex);

                TempFunction_UpdateBasisChange(BasisVectorChangePtr, BasisVectorPtr, DataVectorPtr, VectorLength, MembershipList[n]);

                //BasisExperience[BasisIndex] += MembershipList[n];
				BasisExperience_Change[BasisIndex] += MembershipList[n];
            }
        }
    }

	// update BasisExperience
	for (int_max k = 0; k <= BasisCount - 1; ++k)
	{
		BasisExperience[k] += BasisExperience_Change[k];
	}

    // update Basis
    //for (int_max k = 0; k <= BasisCount-1; ++k)
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
    ParallelForLoop(TempFunction_UpdateBasisMatrix, 0, BasisCount - 1, m_Parameter.MaxCountOfThread);

    this->ApplyConstraintOnBasis(BasisMatrix);

    // the total Experience is
    // BasisExperience.Sum() <- BasisExperience.Sum() + DataCount
    //
    // the new "Experience" of the dictionary gained from data is DataCount
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix)
{
    if (m_Parameter.BasisPositive == true)
    {
        for (int_max k = 0; k < BasisMatrix.GetElementCount(); ++k)
        {
            if (BasisMatrix[k] < ScalarType(0))
            {
                BasisMatrix[k] = ScalarType(0);
            }
        }
    }

    if (m_Parameter.BasisNormalizedWithL1Norm == true)
    {
        int_max BasisCount = BasisMatrix.GetColCount();
        int_max Length = BasisMatrix.GetRowCount();

        auto tempValueToFill = ScalarType(1) / ScalarType(Length);

        auto eps_value = std::numeric_limits<ScalarType>::epsilon();

        DenseMatrix<ScalarType> BasisVector;

        for (int_max k = 0; k < BasisCount; ++k)
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
        int_max BasisCount = BasisMatrix.GetColCount();
        int_max Length = BasisMatrix.GetRowCount();

        auto tempValueToFill = std::sqrt(ScalarType(1) / ScalarType(Length));

        auto eps_value = std::numeric_limits<ScalarType>::epsilon();

        DenseMatrix<ScalarType> BasisVector;

        for (int_max k = 0; k < BasisCount; ++k)
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


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount)
{
    int_max BasisCount = BasisExperience.GetElementCount();

    DenseMatrix<ScalarType> BasisExperience_Diff = MatrixSubtract(BasisExperience, BasisExperience_init);

    ScalarType TotalGain = BasisExperience_Diff.Sum();

    if (TotalGain > TotalDataCount)
    {
        BasisExperience_Diff *= ScalarType(TotalDataCount) / TotalGain;

        MatrixAdd(BasisExperience, BasisExperience_init, BasisExperience_Diff);
    }
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateSimilarityMatrix(DenseMatrix<ScalarType>& SimilarityMatrix, const DenseMatrix<ScalarType>& BasisMatrix, const DenseMatrix<ScalarType>& VarianceList)
{
    int_max BasisCount = BasisMatrix.GetColCount();
    int_max VectorLength = BasisMatrix.GetRowCount();

    auto SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;

    SimilarityMatrix.FastResize(BasisCount, BasisCount);

    //for (int_max k = 0; k <= BasisCount - 2; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
    {
        auto BasisVectorPtr_k = BasisMatrix.GetElementPointerOfCol(k);

        for (int_max n = k + 1; n < BasisCount; ++n) // start from k+1
        {
            auto BasisVectorPtr_n = BasisMatrix.GetElementPointerOfCol(n);

            auto Variance = std::max(VarianceList[k], VarianceList[n]);

            auto Similarity = KNNSoftAssignSparseEncoder<ScalarType>::ComputeSimilarityBetweenTwoVector(BasisVectorPtr_k, BasisVectorPtr_n, VectorLength, 
                                                                                                         SimilarityType, Variance, false);

            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisCount - 2, m_Parameter.MaxCountOfThread);
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix)
{
    int_max BasisCount = SimilarityMatrix.GetColCount();

    BasisRedundancy.FastResize(1, BasisCount);

    if (BasisCount == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy;

    //for (int_max k = 0; k <= BasisCount-1; ++k)
    auto TempFunction_UpdateRedundancy = [&](int_max k)
    {
        BasisRedundancy[k] = 0;

        for (int_max n = 0; n < BasisCount; ++n)
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

    ParallelForLoop(TempFunction_UpdateRedundancy, 0, BasisCount - 1, m_Parameter.MaxCountOfThread);
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateVarianceOfL1Distance(DenseMatrix<ScalarType>& Variance,
                           const DenseMatrix<ScalarType>& FeatureData,
                           const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                           const DenseMatrix<ScalarType>& BasisMatrix,
                           const DenseMatrix<ScalarType>& BasisExperience_init)
{
    int_max DataCount = FeatureData.GetColCount();

    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    DenseMatrix<ScalarType> Variance_current(1, BasisCount);
    Variance_current.Fill(0);

    DenseMatrix<ScalarType> WeightList(1, BasisCount);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataCount; ++k)
    {
        const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Membership = CodeTable[k].ElementList(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourCount = KNN_IndexList.GetLength();

        if (tempNeighbourCount > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourCount; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto L1Distance = ComputeL1DistanceBetweenTwoVector(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += L1Distance*L1Distance*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
            }
        }
    }

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateVarianceOfL2Distance(DenseMatrix<ScalarType>& Variance,
                           const DenseMatrix<ScalarType>& FeatureData,
                           const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                           const DenseMatrix<ScalarType>& BasisMatrix,
                           const DenseMatrix<ScalarType>& BasisExperience_init)
{
    int_max DataCount = FeatureData.GetColCount();

    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    DenseMatrix<ScalarType> Variance_current(1, BasisCount);
    Variance_current.Fill(0);

    DenseMatrix<ScalarType> WeightList(1, BasisCount);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataCount; ++k)
    {
        const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Membership = CodeTable[k].ElementList(); // code from SoftAssignSparseEncorder is Similarity, not membership 

        auto tempNeighbourCount = KNN_IndexList.GetLength();

        if (tempNeighbourCount > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourCount; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVector(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += SquaredL2Distance*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
            }
        }
    }

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ScalarType>& Variance,
                             const DenseMatrix<ScalarType>& FeatureData,
                             const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                             const DenseMatrix<ScalarType>& BasisMatrix,
                             const DenseMatrix<ScalarType>& BasisExperience_init)
{
    int_max DataCount = FeatureData.GetColCount();

    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    DenseMatrix<ScalarType> Variance_current(1, BasisCount);
    Variance_current.Fill(0);

    DenseMatrix<ScalarType> WeightList(1, BasisCount);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataCount; ++k)
    {
        const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Membership = CodeTable[k].ElementList(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourCount = KNN_IndexList.GetLength();

        if (tempNeighbourCount > 0)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < tempNeighbourCount; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVectorPtr, BasisVectorPtr, VectorLength, false);

                Variance_current[BasisIndex] += KLDivergence*KLDivergence*KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
            }
        }
    }

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance_current[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ScalarType>
void KNNAverageDictionaryBuilder<ScalarType>::
UpdateVarianceOfReconstruction(DenseMatrix<ScalarType>& Variance,
                               const DenseMatrix<ScalarType>& FeatureData,
                               const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                               const DenseMatrix<ScalarType>& BasisMatrix,
                               const DenseMatrix<ScalarType>& BasisExperience_init)
{
    int_max DataCount = FeatureData.GetColCount();

    int_max VectorLength = FeatureData.GetRowCount();

    int_max BasisCount = BasisMatrix.GetColCount();

    auto DataReconstructionErrorL2Norm = this->ComputeDataReconstructionErrorL2Norm(FeatureData, CodeTable, BasisMatrix);

    DenseMatrix<ScalarType> Variance_current(1, BasisCount);
    Variance_current.Fill(0);

    DenseMatrix<ScalarType> WeightList(1, BasisCount);
    WeightList.Fill(0);

    auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    for (int_max k = 0; k < DataCount; ++k)
    {
        const auto& KNN_IndexList = CodeTable[k].IndexList();

		const auto& KNN_Membership = CodeTable[k].ElementList(); // code from SoftAssignSparseEncorder is membership, not raw Similarity 

        auto tempNeighbourCount = KNN_IndexList.GetLength();

        if (tempNeighbourCount > 0)
        {
            for (int_max m = 0; m < tempNeighbourCount; ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k] * KNN_Membership[m];

                WeightList[BasisIndex] += KNN_Membership[m];
            }
        }       
    }

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (WeightList[n] > eps_value)
        {
            Variance[n] /= WeightList[n];
        }
    }

    auto MeanVar = Variance.Mean();

    for (int_max n = 0; n < BasisCount; ++n)
    {
        if (Variance_current[n] > eps_value*MeanVar)
        {
            Variance[n] = Variance[n] * BasisExperience_init[n] + Variance_current[n];
            Variance[n] /= BasisExperience_init[n] + 1;
        }
    }
}


template<typename ScalarType>
DenseMatrix<ScalarType>
KNNAverageDictionaryBuilder<ScalarType>::
ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                     const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                     const DenseMatrix<ScalarType>&  BasisMatrix)
{
    auto ReconstructionCodeSet = KNNReconstructionSparseEncoder<ScalarType>::
                                 ComputeReconstructionCodeFromSimilarityCode(FeatureData,
                                                                             CodeTable,
                                                                             BasisMatrix,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative,
                                                                             m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne,
                                                                             m_Parameter.MaxCountOfThread);

    auto ErrorL2NormList = KNNReconstructionSparseEncoder<ScalarType>::
                           ComputeReconstructionErrorL2Norm(FeatureData, ReconstructionCodeSet, BasisMatrix, m_Parameter.MaxCountOfThread);

    return ErrorL2NormList;
}



}// namespace mdk


#endif