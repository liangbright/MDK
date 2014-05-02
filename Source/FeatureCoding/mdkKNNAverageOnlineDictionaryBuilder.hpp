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

    if (m_Parameter.MaxNumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("MaxNumberOfDataInEachBatch > TotalDataNumber @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.MaxNumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("MaxNumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
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

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold <= 0)
    {
        MDK_Error("SimilarityThreshold <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == false)
            {
                m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_Dictionary->VarianceOfL1Distance().Mean();
                IsOk = true;
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

            if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == false)
            {
                m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_Dictionary->VarianceOfL2Distance().Mean();
                IsOk = true;
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

            if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == false)
            {
                m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_Dictionary->VarianceOfKLDivergence().Mean();
                IsOk = true;
            }

            if (IsOk == false)
            {
                MDK_Error("Variance_KL <= 0 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.ExperienceDiscountFactor < 0 || m_Parameter.ExperienceDiscountFactor > 1)
    {
        MDK_Error("ExperienceDiscountFactor < 0 or > 1 @ KNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThresholdToComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThresholdToComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    return true;
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    this->SetupParameter();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;
    OutputDictionary.Copy(m_InitialDictionary);

    DenseMatrix<ElementType>& BasisMatrix = OutputDictionary.BasisMatrix();

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    //----------- encode data and update dictionary ----------------------------//

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {
        m_KNNSoftAssignSparseEncoder.SetInputFeatureData(m_FeatureData);
        m_KNNSoftAssignSparseEncoder.SetInputDictionary(&OutputDictionary);
        m_KNNSoftAssignSparseEncoder.Update();
        auto CodeTable = m_KNNSoftAssignSparseEncoder.GetOutputCodeInSparseColVectorSet();

        this->UpdateBasisMatrix(BasisMatrix, *m_FeatureData, *CodeTable, OutputDictionary.BasisExperience());

        this->UpdateDictionaryInformation(OutputDictionary, *m_FeatureData, *CodeTable);
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

            this->UpdateBasisMatrix(BasisMatrix, FeatureData_current, *CodeTable, OutputDictionary.BasisExperience());

            this->UpdateDictionaryInformation(OutputDictionary, FeatureData_current, *CodeTable);

            // update NumberOfDataInNextBatch

            TotalDataNumber_used += SampleCounter;

            NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

            if (NumberOfDataInNextBatch <= 0)
            {
                break;
            }
        }
    }

    this->UpdateDictionaryInformation_Other(OutputDictionary);

    (*m_Dictionary) = std::move(OutputDictionary);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::SetupParameter()
{
    m_KNNSoftAssignSparseEncoder.m_Parameter = m_Parameter.ParameterOfKNNSoftAssign;    
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                            const DenseMatrix<ElementType>& FeatureData,
                            const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& BasisAge = Dictionary.BasisAge();

    DenseMatrix<ElementType>& BasisExperience = Dictionary.BasisExperience();

    //DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();
    //DenseMatrix<ElementType>& BasisRedundancy = Dictionary->BasisRedundancy();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();
    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();
    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();
    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();

    //-------- update Variance ------------------------------------------------------------

    this->UpdateVarianceOfL1Distance(VarianceOfL1Distance, FeatureData, CodeTable, BasisMatrix, BasisExperience);
    
    this->UpdateVarianceOfL2Distance(VarianceOfL2Distance, FeatureData, CodeTable, BasisMatrix, BasisExperience);

    if (m_Parameter.Update_VarianceOfReconstruction_Using_KNNBasisMatrix == true)
    {
        this->UpdateVarianceOfReconstruction(VarianceOfReconstruction, FeatureData, CodeTable, BasisMatrix, BasisExperience);
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(VarianceOfKLDivergence, FeatureData, CodeTable, BasisMatrix, BasisExperience);
    }

    // update BasisExperience ----------------------------------------------------------

    this->UpdateBasisExperience(BasisExperience, CodeTable);

    // update BasisAge -------------------------

    int_max DataNumber = FeatureData.GetColNumber();

    BasisAge += DataNumber;

    // update SimilarityMatrix and BasisRedundancy in UpdateDictionaryInformation_Other(...)
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ElementType>& Dictionary)
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

    Dictionary.SetProperty_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
}


template<typename ElementType>
void KNNAverageOnlineDictionaryBuilder<ElementType>::UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                                                                       const DenseMatrix<ElementType>& FeatureData,
                                                                       const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                       const DenseMatrix<ElementType>& BasisExperience)
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
                                       ElementType CodeElement,  ElementType SquaredL2NormOfReconstructionCodeVector,
                                       ElementType ExperienceOfBasis)
    {
        auto temp = CodeElement / SquaredL2NormOfReconstructionCodeVector;

        for (int_max k = 0; k < VectorLength; ++k)
        {
            auto Error = DataVector[k] - BasisVector[k] * CodeElement;
            BasisVector[k] += temp * Error / ExperienceOfBasis;
        }
    };

    DenseMatrix<ElementType> ReconstructionCodeVector;

    for (int_max k = 0; k < DataNumber; ++k)//k
    {
        auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

        const std::vector<int_max>& BasisIndexList = CodeTable[k].IndexList();

        int_max NeighbourNumber_k = int_max(BasisIndexList.size());

        if (NeighbourNumber_k > 0)
        {
            ReconstructionCodeVector.FastResize(NeighbourNumber_k, 1);
            // compute reconstruction code Vector
            //
            // use KNN-basis to reconstruct each Data vector (pretend we are doing KNN-reconstruction)
            // reconstruct each Data vector with only one basis
            // 
            // ReconstructionError[n] = ||DataVector - ReconstructionCodeVector[n]*BasisVector||
            //

            for (int_max n = 0; n < NeighbourNumber_k; ++n)//n
            {
                auto BasisIndex = BasisIndexList[n];

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
                                                                                           VectorLength, false);

            if (SquaredL2NormOfReconstructionCodeVector > eps_value)
            {
                for (int_max n = 0; n < NeighbourNumber_k; ++n)//n
                {
                    auto BasisIndex = BasisIndexList[n];

                    auto BasisVectorPtr = BasisMatrix.GetElementPointerOfCol(BasisIndex);

                    auto CodeElement = ReconstructionCodeVector[n];

                    if (CodeElement > eps_value && CodeElement > eps_value*SquaredL2NormOfReconstructionCodeVector)
                    {
                        TempFunction_BasisUpdate(BasisVectorPtr, DataVectorPtr, VectorLength, 
                                                 CodeElement, SquaredL2NormOfReconstructionCodeVector, 
                                                 BasisExperience[BasisIndex]);
                    }// else CodeElement = 0, no update
                }
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
UpdateBasisExperience(DenseMatrix<ElementType>& BasisExperience, const DataContainer<SparseVector<ElementType>>& CodeTable)
{
    int_max BasisNumber = BasisExperience.GetElementNumber();

    int_max DataNumber = CodeTable.GetLength();

    // discount the previous Experience
    BasisExperience *= m_Parameter.ExperienceDiscountFactor;
    // must >= 1
    for (int_max k = 0; k < BasisNumber; k++)
    {
        if (BasisExperience[k] < 1)
        {
            BasisExperience[k] = 1;
        }
    }

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        auto tempNeighbourNumber = int_max(KNN_IndexList.size());

        for (int_max m = 0; m < tempNeighbourNumber; ++m)
        {
            BasisExperience[KNN_IndexList[m]] += ElementType(1) / ElementType(tempNeighbourNumber);
        }
    }

    // the total Experience is
    // BasisExperience.Sum() ~ m_Parameter.ExperienceDiscountFactor * BasisExperience.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
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

    //for (int_max k = 0; k <= BasisNumber - 2; ++k)
    auto TempFunction_ComputeSimilarity = [&](int_max k)
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
    };

    ParallelForLoop(TempFunction_ComputeSimilarity, 0, BasisNumber - 2, m_Parameter.MaxNumberOfThreads);
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
    int_max BasisNumber = SimilarityMatrix.GetColNumber();

    BasisRedundancy.FastResize(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThresholdToComputeBasisRedundancy;

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
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

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
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
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
        if (Variance[n] <= MeanStd)
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
                           const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

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
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
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
        if (Variance[n] <= MeanStd)
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
                             const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

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
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
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
        if (Variance[n] <= MeanStd)
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
                               const DenseMatrix<ElementType>& BasisExperience)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);
    CounterList.Fill(0);

    auto DataReconstructionErrorL2Norm = this->ComputeDataReconstructionErrorL2Norm(FeatureData, CodeTable, BasisMatrix);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[k].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            for (int_max m = 0; m < int_max(KNN_IndexList.size()); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];

                Variance_current[BasisIndex] += DataReconstructionErrorL2Norm[k];

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
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n]  = Variance[n] * BasisExperience[n] + Variance_current[n];
            Variance[n] /= BasisExperience[n] + 1;
        }
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
        if (Variance[n] <= MeanStd)
        {
            Variance[n] = MeanStd;
        }
    }
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNAverageOnlineDictionaryBuilder<ElementType>::
ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                     const DataContainer<SparseVector<ElementType>>& CodeTable,
                                     const DenseMatrix<ElementType>&  BasisMatrix)
{
    int_max DataNumber = CodeTable.GetLength();

    int_max VectorLength = BasisMatrix.GetRowNumber();

    DenseMatrix<ElementType> DataReconstructionErrorL2Norm(1, DataNumber);
    DataReconstructionErrorL2Norm.Fill(0);

    DenseMatrix<ElementType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    //for(int_max DataIndex = 0; DataIndex <= DataNumber - 1; ++DataIndex)
    auto TempFunction_Reconstruction = [&](int_max DataIndex)
    {
        const std::vector<int_max>& KNN_IndexList = CodeTable[DataIndex].IndexList();

        if (KNN_IndexList.size() > 0)
        {
            DenseMatrix<ElementType> DataVector;            
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(DataIndex), VectorLength, 1);

            DenseMatrix<ElementType> ReconstructedDataVector(1, VectorLength);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            this->ReconstructDataVectorByKNNBasisMatrix(ReconstructedDataVector, DataVector, KNNBasisMatrix, KNN_IndexList, GramianMatrix_DtD);

            auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVector.GetElementPointer(),
                                                                               ReconstructedDataVector.GetElementPointer(),
                                                                               VectorLength, false);

            DataReconstructionErrorL2Norm[DataIndex] = SquaredL2Distance;
        }
    };

    ParallelForLoop(TempFunction_Reconstruction, 0, DataNumber - 1, m_Parameter.MaxNumberOfThreads);

    return DataReconstructionErrorL2Norm;
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

    typedef LinearLeastSquaresProblemSolver<ElementType>::MethodTypeEnum LinlsqMethodTypeEnum;

    if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == false && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == false)
    {
        Option.MethodType = LinlsqMethodTypeEnum::NormalEquation;

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
    else if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == true && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == false)
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

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                       &lb_x, nullptr, &A, nullptr, nullptr, nullptr,
                                                                       &H, &Option);

    }
    else if (m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == true && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == true)
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

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                        &lb_x, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                        &H, &Option);
    }
    else //if(m_Parameter.ConstraintOnKNNReconstructionCode.CodeNonnegative == false && m_Parameter.ConstraintOnKNNReconstructionCode.CodeSumToOne == true)
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

        Solution = LinearLeastSquaresProblemSolver<ElementType>::Apply(&KNNBasisMatrix, &DataVector,
                                                                        nullptr, nullptr, &A, &lb_A, &ub_A, nullptr,
                                                                        &H, &Option);
    }

    MatrixMultiply(ReconstructedDataVector, KNNBasisMatrix, Solution.X);
}


}// namespace mdk


#endif