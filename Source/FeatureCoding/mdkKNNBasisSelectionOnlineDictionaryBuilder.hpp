#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::KNNBasisSelectionOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::~KNNBasisSelectionOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("NumberOfDataInEachBatch > TotalDataNumber @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("NumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L1Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::L2Distance
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::Correlation
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != SimilarityTypeEnum::KLDivergence)
    {
        MDK_Error("SimilarityType is invalid @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }
    
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_L1 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_L2 <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::Correlation)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("Variance_KL <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.SimilarityThresholdToComputeBasisRedundancy <= 0)
    {
        MDK_Error("SimilarityThresholdToComputeBasisRedundancy <= 0 @ KNNBasisSelectionOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    // compute the total ExperienceOfRepresentingData of m_InitialDictionary

    auto TotalExperienceOfRepresentingData_init = ElementType(0);
    if (m_InitialDictionary != nullptr)
    {
        TotalExperienceOfRepresentingData_init = m_InitialDictionary->ExperienceOfRepresentingData().Sum();
    }

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    if (m_Parameter.NumberOfDataInEachBatch >= TotalDataNumber)
    {      
        // process all the data 

        OutputDictionary = this->BuildDictionaryFromData(m_Parameter.BasisNumber, *m_FeatureData, m_InitialDictionary);
    }
    else
    {
        // m_Parameter.MaxNumberOfIteration > 1
        //------------------------------------------ run Data batch -------------------------------------------------------//

        // random number for sampling
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

            OutputDictionary = this->BuildDictionaryFromData(m_Parameter.BasisNumber, FeatureData_current, &OutputDictionary);
        }
    }

    //-------- adjust ExperienceOfRepresentingData in case FeatueData is re-used for training -------------//

    auto TotalExperienceOfRepresentingData = OutputDictionary.ExperienceOfRepresentingData().Sum();
    
    auto TotalExperienceOfRepresentingData_est = m_Parameter.ExperienceDiscountFactor * TotalExperienceOfRepresentingData_init + TotalDataNumber;

    if (TotalExperienceOfRepresentingData > TotalExperienceOfRepresentingData_est)
    {
        auto factor = TotalExperienceOfRepresentingData_est / TotalExperienceOfRepresentingData;

        OutputDictionary.ExperienceOfRepresentingData() *= factor;
    }

    // ExperienceOfRepresentingData[k] should always > m_Parameter.ExperienceDiscountFactor
    // if ExperienceOfRepresentingData[k] is too small after training on many datasets, 
    // then, basis k should be removed

    //---------------------------------------------------------------------------------//

    (*m_Dictionary) = std::move(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::BuildDictionaryFromData(int_max BasisNumber_desired,
                                                                           const DenseMatrix<ElementType>& FeatureData,
                                                                           const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;
    Dictionary.SetName(m_Parameter.DictionaryName);

    //------------------------------------------- check input ------------------------------------------------------------------------//

    if (BasisNumber_desired <= 0 || FeatureData.IsEmpty() == true)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")
        return Dictionary;
    }

    if (Dictionary_init != nullptr)
    {
        if (FeatureData.GetRowNumber() != Dictionary_init->BasisMatrix().GetRowNumber())
        {
            MDK_Error("FeatureData.GetRowNumber() != Dictionary_init->GetRowNumber() @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")
            return Dictionary;
        }
    }

    //------------------------------------------- input check done ---------------------------------------------------------------------//

    // --------------------------- combine  FeatureData and BasisMatrix Of InitialDictionary and Compute VectorSimilarityMatrix -----------------

    int_max DataNumber = FeatureData.GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init != nullptr)
    {
        BasisNumber_init = Dictionary_init->BasisMatrix().GetColNumber();
    }

    int_max TotalVectorNumber = BasisNumber_init + DataNumber;

    // DenseMatrix<ElementType> FeatureData_Combined;
    // FeatureData_Combined = { &BasisMatrixOfInitialDictionary, &FeatureData };

    auto RepresentativeAbilityOfEachVector = ComputeRepresentativeAbilityOfEachDataVectorInCombinedData(Dictionary_init, TotalVectorNumber);

    // calculate similarity between feature vectors

    auto VectorSimilarityMatrix = this->ComputeVectorSimilarityMatrix(Dictionary_init, FeatureData);


    MDK_DebugCode
    (
        CharString FilePathAndName = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuiler/Debug/VectorSimilarityMatrix.json";
        SaveDenseMatrixAsJsonDataFile(VectorSimilarityMatrix, FilePathAndName);
    )

    auto KNNVectorIndexTable = FindKNNVectorIndexTableByVectorSimilarityMatrix(VectorSimilarityMatrix);

    // estimate the probability mass function based on RepresentativeAbilityOfEachVector

    auto ProbabilityOfEachVector = this->EstimateKNNSmoothedAndNormalizedRepresentativeAbilityOfEachVector(KNNVectorIndexTable, RepresentativeAbilityOfEachVector);

    // if the number of data samples is smaller than the number of bases -------------------------------------

    if (BasisNumber_desired >= TotalVectorNumber)
    {
        MDK_Warning("BasisNumber_desired >= TotalVectorNumber @ KNNBasisSelectionOnlineDictionaryBuilder::BuildDictionaryFromData(...)")

        DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

        DenseMatrix<ElementType> ExperienceOfRepresentingData(1, TotalVectorNumber);
        
        if (Dictionary_init != nullptr)
        {
            const DenseMatrix<ElementType>& BasisMatrixOfInitialDictionary = Dictionary_init->BasisMatrix();

            BasisNumber_init = BasisMatrixOfInitialDictionary.GetColNumber();

            BasisMatrix = { &BasisMatrixOfInitialDictionary, &FeatureData };

            ExperienceOfRepresentingData.Fill(ElementType(1));
            ExperienceOfRepresentingData(span(0, BasisNumber_init - 1)) = Dictionary_init->ExperienceOfRepresentingData();
        }
        else
        {
            BasisMatrix = FeatureData;

            ExperienceOfRepresentingData.Fill(ElementType(1));
        }
                        
        Dictionary.BasisMatrix().Take(BasisMatrix);

        Dictionary.ExperienceOfRepresentingData().Take(ExperienceOfRepresentingData);

        DenseMatrix<int_max> VectorIndexList_Basis = span(0, BasisMatrix.GetColNumber()-1);

        auto KNNBasisIndexTableOfData = FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(VectorSimilarityMatrix, VectorIndexList_Basis, BasisNumber_init);

        this->UpdateDictionaryInformation(Dictionary, FeatureData, KNNBasisIndexTableOfData,
                                          VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

        return Dictionary;
    }

    //------------------------------------------ extract  basis from data -----------------------------------------------------------------//
    // the number of combined data is greater than the number of bases
    // extract a subset from the data set to be the bases

    // get VectorPairScoreList

    int_max NumberOfVectorPair = TotalVectorNumber*(TotalVectorNumber - 1) / 2;
   
    DenseMatrix<ElementType> VectorPairScoreList(1, NumberOfVectorPair);

    DenseMatrix<int_max> VectorPairIndexList(2, NumberOfVectorPair);

    int_max Counter = 0;
    for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
    {
        for (int_max n = k + 1; n < TotalVectorNumber; ++n)
        {         
            VectorPairScoreList[Counter] = m_Parameter.weigth_s*VectorSimilarityMatrix(n, k)
                                          + (ElementType(1) - m_Parameter.weigth_s)*(ProbabilityOfEachVector[n] + ProbabilityOfEachVector[k]) / ElementType(2);

            VectorPairIndexList(0, Counter) = k;
            VectorPairIndexList(1, Counter) = n;

            Counter += 1;
        }
    }

    DenseMatrix<int_max> tempIndexList_sort;

    Sort(VectorPairScoreList, VectorPairScoreList, tempIndexList_sort, "descend");

    DenseMatrix<int_max> tempPairIndexList = VectorPairIndexList;

    for (int_max k = 0; k < NumberOfVectorPair; ++k)
    {
        auto tempColIndex = tempIndexList_sort[k];
        VectorPairIndexList(0, k) = tempPairIndexList(0, tempColIndex);
        VectorPairIndexList(1, k) = tempPairIndexList(1, tempColIndex);
    }

    //------------------------------------- select basis ---------------------------------//
    // get the data pair vector_a, vector_b with the highest score in the current basis set
    // remove vector_a or vector_b (that has a lower probability) from the current basis set
    // repeat, until the number of vectors in the basis set is m_Parameter.BasisNumber 

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    int_max PairNumber = VectorPairIndexList.GetColNumber();

    DenseMatrix<int_max> VectorFlagList(1, TotalVectorNumber);
    VectorFlagList.Fill(1);
    // FlagList(i) is 0 : vector_i is removed from in the current basis set
    // FlagList(i) is 1 : vector_i is still in the current basis set

    int_max OutputBasisNumber = TotalVectorNumber; // OutputBasisNumber is the number of vectors In Current Basis Set

    for (int_max k = 0; k < PairNumber; ++k)
    {
        if (OutputBasisNumber <= BasisNumber_desired)
        {
            break;
        }

        auto Index_a = VectorPairIndexList(0, k);
        auto Index_b = VectorPairIndexList(1, k);

        if (VectorFlagList[Index_a] > 0 && VectorFlagList[Index_b] > 0)
        {
            auto Prob_a = ProbabilityOfEachVector[Index_a];
            auto Prob_b = ProbabilityOfEachVector[Index_b];

            if (Prob_a == Prob_b)
            {
                bool temp_bool = BoolRandomNumber(gen_bool);
                // randomly discard vector a or vector b
                //if (k % 2 == 0)
                if (temp_bool == true)
                {
                    VectorFlagList[Index_a] = 0;
                }
                else
                {
                    VectorFlagList[Index_b] = 0;
                }
            }
            else if (Prob_a > Prob_b)
            {
                VectorFlagList[Index_b] = 0;
            }
            else// if (Prob_a < Prob_b)
            {
                VectorFlagList[Index_a] = 0;
            }

            OutputBasisNumber -= 1;
        }
    }

    DenseMatrix<int_max> VectorIndexList_Basis;
    VectorIndexList_Basis.ReserveCapacity(OutputBasisNumber);

    for (int_max k = 0; k < TotalVectorNumber; ++k)
    {
        if (VectorFlagList[k] > 0)
        {
            VectorIndexList_Basis.AppendCol({ k });
        }
    }

    // ------------- create BasisMatrix--------------------------------------------------//

    DenseMatrix<ElementType> BasisMatrix(FeatureData.GetRowNumber(), OutputBasisNumber);

    for (int_max k = 0; k < OutputBasisNumber; ++k)
    {
        auto VectorIndex = VectorIndexList_Basis[k];
        if (VectorIndex < BasisNumber_init)
        {
            BasisMatrix.SetCol(k, Dictionary_init->BasisMatrix().GetElementPointerOfCol(VectorIndex));
        }
        else
        {
            int_max tempIndex_k = VectorIndex - BasisNumber_init;

            BasisMatrix.SetCol(k, FeatureData.GetElementPointerOfCol(tempIndex_k));
        }
    }

    // ------------- Apply Constraint On Basis --------------------------------------------------//

    this->ApplyConstraintOnBasis(BasisMatrix);
    
    // ----- Write BasisMatrix into Dictionary -----------------------------------------//

    Dictionary.BasisMatrix().Take(BasisMatrix);

    // -------- get KNNBasisVectorIndexTable (i.e., encode FeatureData) -----------------------//

    auto KNNBasisIndexTableOfData = FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(VectorSimilarityMatrix, VectorIndexList_Basis, BasisNumber_init);

    // --------------- Update DictionaryInformation ------------------------------//

    this->UpdateDictionaryInformation(Dictionary, FeatureData, KNNBasisIndexTableOfData, 
                                      VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeRepresentativeAbilityOfEachDataVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
                                                           int_max CombinedFeatureDataVectorNumber)
{
    //---------------------------------------------------------------------------------------
    // Input:    
    // CombinedData = [ BasisMatrix of Dictionary_init,  FeatureData]
    //
    // CombinedFeatureDataVectorNumber = InitialBasisNumber + FeatureDataNumber
    //
    // if Dictionary_init is empty, then CombinedData = input FeatureData 
    //
    // Output:
    // RepresentativeAbility is the ability to represent many data vectors
    //
    // 
    // If a data vector k in CombinedData is from Dictionary_init, then it can represent many data vectors in history
    // Therefore, RepresentativeAbility[k] = m_Parameter.weigth_past * the number of data in history that it represents 
    //     
    // If a data vector n in CombinedData is from FeatureData, then it only represents itself
    // Therefore RepresentativeAbility[n] =  (1 - m_Parameter.weigth_past) * 1 (only itself)
    //----------------------------------------------------------------------------------------

    DenseMatrix<ElementType> RepresentativeAbility(1, CombinedFeatureDataVectorNumber);

    RepresentativeAbility.Fill(ElementType(1));

    if (Dictionary_init != nullptr)
    {
        auto InitialBasisNumber = Dictionary_init->BasisMatrix().GetColNumber();

        const DenseMatrix<ElementType>& ExperienceOfRepresentingData_init = Dictionary_init->ExperienceOfRepresentingData();

        for (int_max k = 0; k < InitialBasisNumber; k++)
        {
            RepresentativeAbility[k] = ExperienceOfRepresentingData_init[k];
        }

        for (int_max k = InitialBasisNumber; k < CombinedFeatureDataVectorNumber; k++)
        {
            RepresentativeAbility[k] = ElementType(1);
        }
    }

    return RepresentativeAbility;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init, 
                                                                                 const DenseMatrix<ElementType>& FeatureData)
{
    //------------------------------------------------------------------------------------------
    // Input:
    // FeatureData is a set of column data vectors
    // 
    // Output:
    // VectorSimilarityMatrix of Combined data = [Dictionary_init->BasisMatrix(), FeatureData]
    // 
    // ColNumber = RowNumber = the total number of data vectors    
    // VectorSimilarityMatrix(i, j) = VectorSimilarityMatrix(j, i)
    // VectorSimilarityMatrix(i, i) = 0 for all i
    //
    // a pair of vector_i and vector_j is called data pair
    //------------------------------------------------------------------------------------------

    DenseMatrix<ElementType> VectorSimilarityMatrix;

    int_max VectorLength = FeatureData.GetRowNumber();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    std::random_device rd_uniform;
    std::mt19937 gen_uniform(rd_uniform());
    std::uniform_real_distribution<ElementType> UniformRandomNumber(0, ElementType(1));

    //-------------------------------------------------------------------------------------------------------------
    auto TempFunction_AndRandomNumberToSimilarity = [&](ElementType Similarity)
    {
        // if Similarity <= SimilarityThreshold and > eps_value
        // add some random noise with abs < SimilarityThreshold
        // then,  data pair with Similarity <= eps_value will be sorted/sampled randomly
        // It is a kind of random sampling
        //
        // if Similarity <= eps_value
        // then set Similarity to zero or eps_value randomly

        if (Similarity <= SimilarityThreshold && Similarity > eps_value)
        {
            auto tempValue = UniformRandomNumber(gen_uniform);

            Similarity *= tempValue;

        }

        if (Similarity <= eps_value)
        {
            bool Flag = BoolRandomNumber(gen_bool);

            if (Flag == true)
            {
                Similarity = ElementType(0);
            }
            else
            {
                Similarity = eps_value;
            }
        }

        return Similarity;
    };

    //--------------------------------------------------------------------------------------------------------------

    if (Dictionary_init == nullptr)
    {
        int_max TotalVectorNumber = FeatureData.GetColNumber();

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
        {
            auto VectorPtr_k = FeatureData.GetElementPointerOfCol(k);

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                auto VectorPtr_n = FeatureData.GetElementPointerOfCol(n);

                auto Similarity = this->ComputeSimilarityBetweenTwoDataVectors(VectorPtr_k, VectorPtr_n, VectorLength);

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);
             
                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        }
    }
    else
    {
        const DenseMatrix<ElementType>& BasisMatrix_init = Dictionary_init->BasisMatrix();

        int_max BasisNumber_init = BasisMatrix_init.GetColNumber();
        
        const DenseMatrix<ElementType>& SimilarityMatrix_init = Dictionary_init->SimilarityMatrix();

        int_max DataNumber = FeatureData.GetColNumber();

        auto TotalVectorNumber = BasisNumber_init + DataNumber;

        VectorSimilarityMatrix.FastResize(TotalVectorNumber, TotalVectorNumber);
        VectorSimilarityMatrix.Fill(ElementType(0));  // VectorSimilarityMatrix(i, i) = 0 for all i

        for (int_max k = 0; k < TotalVectorNumber - 1; ++k)
        {
            const ElementType* VectorPtr_k = nullptr;
            const ElementType* VectorPtr_n = nullptr;

            if (k < BasisNumber_init)
            {
                VectorPtr_k = BasisMatrix_init.GetElementPointerOfCol(k);
            }
            else if (k >= BasisNumber_init)
            {
                int_max temp_k = k - BasisNumber_init;

                VectorPtr_k = FeatureData.GetElementPointerOfCol(temp_k);
            }

            for (int_max n = k + 1; n < TotalVectorNumber; ++n) // start from k+1
            {
                VectorPtr_n = FeatureData.GetElementPointerOfCol(n);

                if (n < BasisNumber_init)
                {
                    VectorPtr_n = BasisMatrix_init.GetElementPointerOfCol(n);
                }
                else if (n >= BasisNumber_init)
                {
                    int_max temp_n = n - BasisNumber_init;

                    VectorPtr_n = FeatureData.GetElementPointerOfCol(temp_n);
                }

                auto Similarity = ElementType(0);

                if (k < BasisNumber_init && n < BasisNumber_init)
                {
                    Similarity = SimilarityMatrix_init(n, k);
                }
                else
                {
                    auto Similarity = this->ComputeSimilarityBetweenTwoDataVectors(VectorPtr_k, VectorPtr_n, VectorLength);
                }

                Similarity = TempFunction_AndRandomNumberToSimilarity(Similarity);

                VectorSimilarityMatrix(k, n) = Similarity;

                VectorSimilarityMatrix(n, k) = Similarity;
            }
        }
    }

    return VectorSimilarityMatrix;
}


template<typename ElementType>
inline
ElementType KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
ComputeSimilarityBetweenTwoDataVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length)
{
    ElementType Similarity = ElementType(0);

    switch (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType)
    {
    case SimilarityTypeEnum::L1Distance:
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L1Distance*L1Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    case SimilarityTypeEnum::L2Distance:
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB, Length, false);
        auto temp = (L2Distance*L2Distance) / m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
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

        auto temp = (KLDivergence*KLDivergence) / m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
        Similarity = std::exp(-temp / ElementType(2));
    }
        break;

    default:
        MDK_Error("unknown type of similarity @ KNNBasisSelectionOnlineDictionaryBuilder::ComputeSimilarityBetweenTwoDataVectors(...)")
    }

    return Similarity;
}


template<typename ElementType>
DataContainer<DenseMatrix<int_max>>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix)
{
    //-------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix(...)
    // 
    // Output:
    // KNNVectorIndexTable[k] is the KNN index list of the vector with index k in VectorSimilarityMatrix
    // Index Range in KNNVectorIndexTable[k] is from 0 to BasisNumber_init + DataNumber -1
    //
    // VectorSimilarityMatrix(i, i) = 0 for all i is guaranteed in this->ComputeVectorSimilarityMatrix(...)
    // Then the data-index of the data vector k is not included in KNNVectorIndexTable[k]
    // i.e., self is not a KNN-neighbor of self
    //--------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = VectorSimilarityMatrix.GetColNumber();

    DataContainer<DenseMatrix<int_max>> KNNVectorIndexTable;
    KNNVectorIndexTable.FastResize(TotalDataNumber);

    DenseMatrix<ElementType> SimilarityList(1, TotalDataNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        for (int_max n = 0; n < TotalDataNumber; ++n)
        {
            SimilarityList[n] = VectorSimilarityMatrix(n, k);
        }

        KNNVectorIndexTable[k] = FindKNNBySimilarityList(SimilarityList,
                                                         m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                         m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    }

    return KNNVectorIndexTable;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
EstimateKNNSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable)
{
    //--------------------------------------------------------------------------------
    // Input:
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // 
    // Output:
    // Probability is the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    //
    // assume each data vector can be a basis
    // then estimate the PMF of each data vector by counting the number of KNN-neighbors
    //--------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(1)); // KNN does not include self, so add 1 to count self

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            Probability[KNN_IndexList[m]] += 1;
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
EstimateKNNSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                  const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector)
{
    //-----------------------------------------------------------------------------------------------------------------
    // Input:
    // KNNVectorIndexTable is from this->FindKNNVectorIndexTableByVectorSimilarityMatrix(...)
    // RepresentativeAbilityOfEachVector is from this->GetRepresentativeAbilityOfEachDataVectorInCombinedData(...)
    // 
    // Output:
    // Probability is  the KNN-Smoothed And Normalized Representative Ability Of Each Vector
    // 
    // assume each data vector can be a basis
    // then estimate the PMF of each data vector by counting the normalized number of KNN-neighbors
    //
    // the normalized number of KNN-neighbors of the data vector k is stored in DataProbabilityMassFunction[k]
    //
    // this is no self-counting 
    // because KNN does not include self in KNNVectorIndexTable 
    //
    // note: 
    // If a data vector is from initial dictionary, and one of its KNN-neighbors is also from initial dictionary
    // Then, the PMF at this data vector is significantly increased
    // (i.e., it has more neighbors "2*K", compared to a real data vector)
    // Therefore: The PFM estimation accuracy maybe decreased
    // But: the previous basis has a higher chance to be re-selected, and this is good   
    //  
    // A difference method is used to update PMF at each basis, 
    // see this->UpdateProbabilityMassFunctionAtEachBasisVector()
    //-----------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(0)); // do not need to fill 1, self is already counted in RepresentativeAbilityOfEachVector

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            Probability[KNN_IndexList[m]] += RepresentativeAbilityOfEachVector[k];
        }
    }

    auto SumProb = Probability.Sum();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();
    if (SumProb <= eps_value)
    {
        Probability.Fill(ElementType(1) / ElementType(TotalDataNumber));
    }
    else
    {
        Probability /= SumProb;
    }

    return Probability;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix)
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                            const DenseMatrix<ElementType>& FeatureData,
                            const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                            const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                            const DenseMatrix<int_max>& VectorIndexList_Basis,
                            const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& ExperienceOfRepresentingData = Dictionary.ExperienceOfRepresentingData();

    DenseMatrix<ElementType>& SimilarityMatrix = Dictionary.SimilarityMatrix();

    DenseMatrix<ElementType>& BasisRedundancy = Dictionary.BasisRedundancy();

    int_max BasisNumber = Dictionary.BasisMatrix().GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init != nullptr)
    { 
        BasisNumber_init = Dictionary_init->BasisMatrix().GetColNumber();
    }

    //--------------------- initialize ExperienceOfRepresentingData ----------------------------------------------------------------
    // BasisiNumber in updated BasisMatrix may be different than that in Dictionary_init

    ExperienceOfRepresentingData.FastResize(1, BasisNumber);

    ExperienceOfRepresentingData.Fill(ElementType(1)); // fill 1 for new basis

    if (Dictionary_init != nullptr)
    {
        const DenseMatrix<ElementType>& ExperienceOfRepresentingData_init = Dictionary_init->ExperienceOfRepresentingData();

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto tempIndex = VectorIndexList_Basis[k];
            if (tempIndex < BasisNumber_init) 
            {   // this basis is from Dictionary_init
                ExperienceOfRepresentingData[k] = ExperienceOfRepresentingData_init[tempIndex];
            }
        }
    }

    //--------------------- update Variance Information ----------------------------------------------------------------

    if (m_Parameter.UpdateDictionaryInformation_Variance == true)
    {
        this->UpdateDictionaryInformation_Variance(Dictionary, FeatureData, KNNBasisIndexTableOfData,
                                                   VectorSimilarityMatrix, VectorIndexList_Basis, Dictionary_init);
    }

    //--------------------- update ExperienceOfRepresentingData ----------------------------------------------------------------

    this->UpdateExperienceOfRepresentingDataForEachBasisVector(ExperienceOfRepresentingData, KNNBasisIndexTableOfData);

    // ----------- update SimilarityMatrix ------------------------------------//

    SimilarityMatrix.FastResize(BasisNumber, BasisNumber);
    for (int_max k = 0; k < BasisNumber; ++k)
    {
        auto VectorIndex_k = VectorIndexList_Basis[k];

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            auto VectorIndex_n = VectorIndexList_Basis[n];

            auto Similarity = VectorSimilarityMatrix(VectorIndex_n, VectorIndex_k);

            SimilarityMatrix(n, k) = Similarity;
            SimilarityMatrix(k, n) = Similarity;
        }
    }

    Dictionary.SetInfo_SimilarityType(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);

    //---------- Update BasisRedundancy --------------------------------------------//

    this->UpdateBasisRedundancy(BasisRedundancy, SimilarityMatrix);

    Dictionary.SetInfo_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init)
{
    DenseMatrix<ElementType>& BasisMatrix = Dictionary.BasisMatrix();

    DenseMatrix<ElementType>& ExperienceOfRepresentingData = Dictionary.ExperienceOfRepresentingData();

    DenseMatrix<ElementType>& VarianceOfL1Distance = Dictionary.VarianceOfL1Distance();

    DenseMatrix<ElementType>& VarianceOfL2Distance = Dictionary.VarianceOfL2Distance();

    DenseMatrix<ElementType>& VarianceOfKLDivergence = Dictionary.VarianceOfKLDivergence();

    DenseMatrix<ElementType>& VarianceOfReconstruction = Dictionary.VarianceOfReconstruction();
    
    int_max BasisNumber = BasisMatrix.GetColNumber();

    int_max BasisNumber_init = 0;
    if (Dictionary_init != nullptr)
    {
        BasisNumber_init = Dictionary_init->BasisMatrix().GetColNumber();
    }

    //---------- initialize Variance --------------------------------------------//

    VarianceOfL1Distance.FastResize(1, BasisNumber);
    if (Dictionary_init != nullptr)
    {
        const DenseMatrix<ElementType>& VarianceOfL1Distance_init = Dictionary_init->VarianceOfL1Distance();

        const DenseMatrix<ElementType>& VarianceOfL2Distance_init = Dictionary_init->VarianceOfL2Distance();

        const DenseMatrix<ElementType>& VarianceOfKLDivergence_init = Dictionary_init->VarianceOfKLDivergence();

        const DenseMatrix<ElementType>& VarianceOfReconstruction_init = Dictionary_init->VarianceOfReconstruction();

        auto MeanStd_L1Distance = ElementType(0);
        if (VarianceOfL1Distance_init.IsEmpty() == false)
        {
            MeanStd_L1Distance = VarianceOfL1Distance_init.Mean();
        }
        else
        {
            MeanStd_L1Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
        }

        auto MeanStd_L2Distance = ElementType(0);
        if (VarianceOfL2Distance_init.IsEmpty() == false)
        {
            MeanStd_L2Distance = VarianceOfL2Distance_init.Mean();
        }
        else
        {
            MeanStd_L2Distance = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
        }

        auto MeanStd_KLDivergence = ElementType(0);
        if (VarianceOfKLDivergence_init.IsEmpty() == false)
        {
            MeanStd_KLDivergence = VarianceOfKLDivergence_init.Mean();
        }
        else
        {
            MeanStd_KLDivergence = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;
        }

        ElementType MeanStd_Reconstruction = MeanStd_L2Distance;

        for (int_max k = 0; k < BasisNumber; ++k)
        {
            auto VectorIndex_k = VectorIndexList_Basis[k];

            if (VectorIndex_k < BasisNumber_init)
            {
                VarianceOfL1Distance[k]     = VarianceOfL1Distance_init[VectorIndex_k];
                VarianceOfL2Distance[k]     = VarianceOfL2Distance_init[VectorIndex_k];
                VarianceOfKLDivergence[k]   = VarianceOfKLDivergence_init[VectorIndex_k];
                VarianceOfReconstruction[k] = VarianceOfReconstruction_init[VectorIndex_k];
            }
            else
            {
                VarianceOfL1Distance[k] = MeanStd_L1Distance;
                VarianceOfL2Distance[k] = MeanStd_L2Distance;
                VarianceOfKLDivergence[k] = MeanStd_KLDivergence;
                VarianceOfReconstruction[k] = MeanStd_Reconstruction;
            }
        }

    }
    else
    {
        VarianceOfL1Distance.Clear();
        VarianceOfL2Distance.Clear();
        VarianceOfKLDivergence.Clear();
        VarianceOfReconstruction.Clear();
    }

    //----------- update Variance ----------------------------------------------------//

    this->UpdateVarianceOfL1Distance(VarianceOfL1Distance,
                                     FeatureData,
                                     KNNBasisIndexTableOfData,
                                     BasisMatrix,
                                     ExperienceOfRepresentingData);

    this->UpdateVarianceOfL2Distance(VarianceOfL2Distance,
                                     FeatureData,
                                     KNNBasisIndexTableOfData,
                                     BasisMatrix,
                                     ExperienceOfRepresentingData);

    this->UpdateVarianceOfReconstruction(VarianceOfReconstruction,
                                         FeatureData,
                                         KNNBasisIndexTableOfData,
                                         BasisMatrix,
                                         ExperienceOfRepresentingData);

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == SimilarityTypeEnum::KLDivergence)
    {
        this->UpdateVarianceOfKLDivergence(VarianceOfKLDivergence,
                                           FeatureData,
                                           KNNBasisIndexTableOfData,
                                           BasisMatrix,
                                           ExperienceOfRepresentingData);
    }
}


template<typename ElementType>
DataContainer<DenseMatrix<int_max>>
KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                                        const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                                                                        int_max BasisNumber_init)
{
    //--------------------------------------------------------------------------------------------------------------
    // Input:
    // VectorIndexList_Basis is from this->BuildDictionaryFromData(...)
    // VectorIndexList_Basis[n] is the index of the basis vector in VectorSimilarityMatrix
    //
    // VectorSimilarityMatrix is from this->ComputeVectorSimilarityMatrix();
    // VectorSimilarityMatrix(i, j) is the similarity between data vector i and data vector j
    // VectorSimilarityMatrix(i, i) = 0 for all i, guaranteed in this->ComputeVectorSimilarityMatrix(...)
    //
    // Output:
    // KNNBasisIndexTableOfData[k] is the basis-index list of KNN-bases of the data vector k
    // the length of KNNBasisIndexTableOfData[k] is from 0 to m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber
    // Index Range in KNNBasisIndexTableOfData[k] is from 0 to BasisNumber - 1
    //----------------------------------------------------------------------------------------------------------------

    int_max BasisNumber = VectorIndexList_Basis.GetElementNumber();

    int_max TotalVectorNumber = VectorSimilarityMatrix.GetColNumber();

    auto DataNumber = TotalVectorNumber - BasisNumber;

    DataContainer<DenseMatrix<int_max>> KNNBasisIndexTableOfData;
    KNNBasisIndexTableOfData.FastResize(DataNumber);

    DenseMatrix<ElementType> SimilarityList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        int_max VectorIndex_Data = BasisNumber_init + k;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            int_max VectorIndex_basis = VectorIndexList_Basis[n];

            SimilarityList[n] = VectorSimilarityMatrix(VectorIndex_basis, VectorIndex_Data);
        }

        KNNBasisIndexTableOfData[k] = FindKNNBySimilarityList(SimilarityList,
                                                              m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                              m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    }

    return KNNBasisIndexTableOfData;
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateExperienceOfRepresentingDataForEachBasisVector(DenseMatrix<ElementType>& ExperienceOfRepresentingData,
                                                     const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData)
{
    //--------------------------------------------------------------------------------------
    // Input:
    // ExperienceOfRepresentingData is initialized
    //
    // KNNBasisIndexTableOfData is from this->FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(...)
    //
    // Output:
    // Probability[n]: PMF at the basis with basis-index n (call it : the basis n)
    //
    // Probability[n] is estimated by counting the normalized number of KNN data vectors of the basis n
    // this is no double counting, because KNN does not include self in KNNBasisVectorIndexTable 
    //--------------------------------------------------------------------------------------

    // discount the previous Experience
    ExperienceOfRepresentingData *= m_Parameter.ExperienceDiscountFactor;

    int_max BasisNumber = ExperienceOfRepresentingData.GetElementNumber();

    int_max DataNumber = KNNBasisIndexTableOfData.GetLength();

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        int_max tempNeighbourNumber = KNN_IndexList.GetElementNumber();

        for (int_max m = 0; m < tempNeighbourNumber; ++m)
        {
            ExperienceOfRepresentingData[KNN_IndexList[m]] += ElementType(1) / ElementType(tempNeighbourNumber);
        }
    }

    // the total Experience is
    // ExperienceOfRepresentingData.Sum() ~ m_Parameter.ExperienceDiscountFactor * ExperienceOfRepresentingData.Sum() + DataNumber
    //
    // the new "Experience" of the dictionary gained from data is DataNumber
}


template<typename ElementType>
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& ExperienceOfRepresentingData)
{
   int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L1);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
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
            Variance[n] = ExperienceOfRepresentingData[n] * Variance[n] + Variance_current[n];
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
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL1Distance(...)"
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                           const DenseMatrix<ElementType>& BasisMatrix,
                           const DenseMatrix<ElementType>& ExperienceOfRepresentingData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_L2);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {        
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
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
            Variance[n] = ExperienceOfRepresentingData[n] * Variance[n] + Variance_current[n];
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
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfL2Distance(...)"
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                             const DenseMatrix<ElementType>& FeatureData,
                             const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                             const DenseMatrix<ElementType>& BasisMatrix,
                             const DenseMatrix<ElementType>& ExperienceOfRepresentingData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> Variance_current(1, BasisNumber);
    Variance_current.Fill(m_Parameter.ParameterOfKNNSoftAssign.Variance_KL);

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < DataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            auto DataVectorPtr = FeatureData.GetElementPointerOfCol(k);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
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
            Variance[n] = ExperienceOfRepresentingData[n] * Variance[n] + Variance_current[n];
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
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfKLDivergence(...)"
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::
UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                               const DenseMatrix<ElementType>& FeatureData,
                               const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                               const DenseMatrix<ElementType>& BasisMatrix,
                               const DenseMatrix<ElementType>& ExperienceOfRepresentingData)
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
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisIndexTableOfData[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ForceShare(FeatureData.GetElementPointerOfCol(k), VectorLength, 1);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            this->ReconstructDataVectorByKNNBasisMatrix(ReconstructedDataVector, DataVector, KNNBasisMatrix, KNN_IndexList, GramianMatrix_DtD);

            auto SquaredL2Distance = ComputeSquaredL2DistanceBetweenTwoVectors(DataVector.GetElementPointer(), 
                                                                               ReconstructedDataVector.GetElementPointer(),
                                                                               VectorLength, false);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
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
        for (int_max n = 0; n < BasisNumber; ++n)
        {
            Variance[n] = ExperienceOfRepresentingData[n] * Variance[n] + Variance_current[n];
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
        MDK_Warning("MeanStd <= eps_value @ KNNBasisSelectionOnlineDictionaryBuilder::UpdateVarianceOfReconstruction(...)"
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
void KNNBasisSelectionOnlineDictionaryBuilder<ElementType>::ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector,
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


}//namespace mdk

#endif