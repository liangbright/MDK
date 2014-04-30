#ifndef __mdkKNNSoftAssignOnlineDictionaryBuilder_hpp
#define __mdkKNNSoftAssignOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::KNNSoftAssignOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::~KNNSoftAssignOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNSoftAssignOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNSoftAssignOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNSoftAssignOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInitialDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InitialDictionary @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInitialDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber <= 0)
    {
        MDK_Error("NeighbourNumber <= 0 @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch > m_FeatureData->GetColNumber())
    {
        MDK_Error("NumberOfDataInEachBatch > TotalDataNumber @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.NumberOfDataInEachBatch < m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber)
    {
        MDK_Error("NumberOfDataInEachBatch < MaxNumberOfNeighbours @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "L1Distance"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "L2Distance"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "Correlation"
        && m_Parameter.ParameterOfKNNSoftAssign.SimilarityType != "KLDivergence")
    {
        MDK_Error("SimilarityType is invalid @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold < 0)
    {
        MDK_Error("SimilarityThreshold is invalid @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L1Distance")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1 <= 0)
        {
            // try to find it in Initial Dictionary
            
            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1 = m_Dictionary->StandardDeviationOfL1Distance().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Sigma_L1 <= 0 @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L2Distance")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2 = m_Dictionary->StandardDeviationOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("Sigma_L2 <= 0 @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "Correlation")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            MDK_Warning("IgnoreSign_Correlation is true @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "KLDivergence")
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInitialDictionaryEmpty == false)
            {
                if (m_InitialDictionary->StandardDeviationOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL = m_Dictionary->StandardDeviationOfKLDivergence().Mean();
                    IsOk = true;
                }
            }
            
            if (IsOk == false)
            {
                MDK_Error("Sigma_KL <= 0 @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.SimilarityThresholdToComputeBasisRedundancy <= 0)
    {
        MDK_Error("SimilarityThresholdToComputeBasisRedundancy <= 0 @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    if (m_Parameter.NumberOfDataInEachBatch >= TotalDataNumber)
    {
        DenseMatrix<ElementType> FeatureData_Combined;

        if (m_InitialDictionary != nullptr)
        {
            DenseMatrix<ElementType>& BasisMatrixOfInitialDictionary = m_InitialDictionary->BasisMatrix();

            FeatureData_Combined = { &BasisMatrixOfInitialDictionary, m_FeatureData };
        }
        else
        {
            FeatureData_Combined.ForceShare(m_FeatureData);
        }

        auto Probability_Combined = GetProbabilityMassFunctionOfCombinedData(m_InitialDictionary, FeatureData_Combined.GetColNumber());
 
        (*m_Dictionary) = this->BuildDictionaryFromDataBatch(m_Parameter.BasisNumber, FeatureData_Combined, Probability_Combined, m_InitialDictionary);

        return;
    }

    // m_Parameter.MaxNumberOfIteration > 1
    //------------------------------------------ run Data batch -------------------------------------------------------//

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> UniformRandomNumber(0, TotalDataNumber - 1);

    FeatureDictionaryForSparseCoding<ElementType> Dictionary_temp;

    if (m_InitialDictionary != nullptr)
    {
        Dictionary_temp.Copy(m_InitialDictionary);
    }

    DenseMatrix<ElementType> FeatureData_current(m_FeatureData->GetRowNumber(), m_Parameter.NumberOfDataInEachBatch);

    for (int_max IterationIndex = 0; IterationIndex < m_Parameter.MaxNumberOfIteration; IterationIndex++)
    {
        // sample data with m_Parameter.NumberOfDataInEachBatch 
        for (int_max k = 0; k < m_Parameter.NumberOfDataInEachBatch; ++k)
        {
            auto DataIndex = UniformRandomNumber(gen);

            FeatureData_current.SetCol(k, m_FeatureData->GetElementPointerOfCol(DataIndex));
        }
        
        DenseMatrix<ElementType> FeatureData_Combined;

        if (Dictionary_temp.IsEmpty() == false)
        {
            DenseMatrix<ElementType>& BasisMatrixOfInitialDictionary = Dictionary_temp.BasisMatrix();

            FeatureData_Combined = { &BasisMatrixOfInitialDictionary, &FeatureData_current };
        }
        else
        {
            FeatureData_Combined.ForceShare(FeatureData_current);
        }

        auto Probability_Combined = GetProbabilityMassFunctionOfCombinedData(&Dictionary_temp, FeatureData_Combined.GetColNumber());

        Dictionary_temp = this->BuildDictionaryFromDataBatch(m_Parameter.BasisNumber, FeatureData_Combined, Probability_Combined, &Dictionary_temp);
    }

    (*m_Dictionary) = std::move(Dictionary_temp);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::BuildDictionaryFromDataBatch(int_max BasisNumber_desired,
                                                                                const DenseMatrix<ElementType>& FeatureData,
                                                                                const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData,
                                                                                const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;
    Dictionary.SetName(m_Parameter.DictionaryName);

    //------------------------------------------- check input ------------------------------------------------------------------------//

    if (BasisNumber_desired <= 0 || FeatureData.IsEmpty() == true || ProbabilityMassFunctionOfData.IsVector() == false)
    {
        MDK_Error("Invalid input @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromDataBatch(...)")
        return Dictionary;
    }

    if (FeatureData.GetColNumber() != ProbabilityMassFunctionOfData.GetElementNumber())
    {
        MDK_Error("FeatureData.GetColNumber() != Probability.GetElementNumber() @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromDataBatch(...)")
        return Dictionary;
    }

    //------------------------------------------- input check done ---------------------------------------------------------------------//

    // calculate similarity between feature vectors

    auto SimilarityMatrix = this->ComputeSimilarityMatrix(FeatureData,
                                                          [&](const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
                                                          { return this->ComputeSimilarityBetweenTwoDataVectors(VectorA, VectorB); });

    MDK_DebugCode
    (
        CharString FilePathAndName = "C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNSoftAssignOnlineDictionaryBuiler/Debug/SimilarityMatrix.json";
        SaveDenseMatrixAsJsonDataFile(SimilarityMatrix, FilePathAndName);
    )

    auto KNNDataVectorIndexTable = FindKNNDataVectorIndexTableBySimilarityMatrix(SimilarityMatrix);

    // estimate the probability mass function based on Probability_init

    auto Probability = this->EstimateBasisProbabilityMassFunction_Data(KNNDataVectorIndexTable, ProbabilityMassFunctionOfData);

    // if the number of data samples is smaller than the number of bases
    if (BasisNumber_desired >= FeatureData.GetColNumber())
    {
        MDK_Warning("BasisNumber_desired >= FeatureData.GetColNumber() @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromDataBatch(...)")

        Dictionary.BasisMatrix().Copy(FeatureData);
        Dictionary.ProbabilityMassFunction().Copy(Probability);
        return Dictionary;
    }

    //------------------------------------------ extract dictionary from data -----------------------------------------------------------------//
    // the number of data samples is greater than the number of bases
    // extract a subset from the data set to be the bases

    // get DataPairScoreList

    int_max TotalFeatureDataNumber = FeatureData.GetColNumber();
    int_max tempLength = TotalFeatureDataNumber*(TotalFeatureDataNumber - 1) / 2;
   
    DenseMatrix<ElementType> DataPairScoreList(1, tempLength);

    DenseMatrix<int_max> DataPairIndexList(2, tempLength);

    int_max Counter = 0;
    for (int_max k = 0; k < TotalFeatureDataNumber-1; ++k)
    {
        for (int_max n = k + 1; n < TotalFeatureDataNumber; ++n)
        {         
            DataPairScoreList[Counter] = m_Parameter.weigth_s*SimilarityMatrix(n, k)
                                        + (1 - m_Parameter.weigth_s)*(Probability[n] + Probability[k]) / ElementType(2);

            DataPairIndexList(0, Counter) = k;
            DataPairIndexList(1, Counter) = n;

            Counter += 1;
        }
    }

    DenseMatrix<int_max> tempIndexList_sort;

    Sort(DataPairScoreList, DataPairScoreList, tempIndexList_sort, "descend");

    DenseMatrix<int_max> tempDataPairIndexList = DataPairIndexList;

    for (int_max k = 0; k < DataPairIndexList.GetColNumber(); ++k)
    {
        auto tempColIndex = tempIndexList_sort[k];
        DataPairIndexList(0, k) = tempDataPairIndexList(0, tempColIndex);
        DataPairIndexList(1, k) = tempDataPairIndexList(1, tempColIndex);
    }

    //------------------------------------- select basis ---------------------------------//
    // get the data pair vector_a, vector_b with the highest score in the current basis set
    // remove vector_a or vector_b (that has a lower probability) from the current basis set
    // repeat, until the number of vectors in the basis set is m_Parameter.BasisNumber 

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    int_max PairNumber = DataPairIndexList.GetColNumber();

    DenseMatrix<int_max> DataFlagList(1, TotalFeatureDataNumber);
    DataFlagList.Fill(1);
    // FlagList(i) is 0 : vector_i is removed from in the current basis set
    // FlagList(i) is 1 : vector_i is still in the current basis set

    int_max OutputBasisNumber = TotalFeatureDataNumber; // OutputBasisNumber is the number of vectors In Current Basis Set

    for (int_max k = 0; k < PairNumber; ++k)
    {
        if (OutputBasisNumber <= BasisNumber_desired)
        {
            break;
        }

        auto Index_a = DataPairIndexList(0, k);
        auto Index_b = DataPairIndexList(1, k);

        if (DataFlagList[Index_a] > 0 && DataFlagList[Index_b] > 0)
        {
            auto Prob_a = Probability[Index_a];
            auto Prob_b = Probability[Index_b];

            if (Prob_a == Prob_b)
            {
                bool temp_bool = BoolRandomNumber(gen_bool);
                // randomly discard vector a or vector b
                //if (k % 2 == 0)
                if (temp_bool == true)
                {
                    DataFlagList[Index_a] = 0;
                }
                else
                {
                    DataFlagList[Index_b] = 0;
                }
            }
            else if (Prob_a > Prob_b)
            {
                DataFlagList[Index_b] = 0;
            }
            else// if (Prob_a < Prob_b)
            {
                DataFlagList[Index_a] = 0;
            }

            OutputBasisNumber -= 1;
        }
    }

    DenseMatrix<int_max> DataVectorIndexList_Basis;
    DataVectorIndexList_Basis.ReserveCapacity(OutputBasisNumber);

    for (int_max k = 0; k < TotalFeatureDataNumber; ++k)
    {
        if (DataFlagList[k] > 0)
        {
            DataVectorIndexList_Basis.AppendCol({ k });
        }
    }

    // ------------- create BasisMatrix--------------------------------------------------//

    DenseMatrix<ElementType> BasisMatrix(FeatureData.GetRowNumber(), OutputBasisNumber);

    for (int_max k = 0; k < OutputBasisNumber; ++k)
    {
        auto DataVectorIndex = DataVectorIndexList_Basis[k];

        BasisMatrix.SetCol(k, FeatureData.GetElementPointerOfCol(DataVectorIndex));
    }

    // get  KNNBasisVectorIndexTable ------------------------------------------------------//

    auto KNNBasisVectorIndexTable = FindKNNBasisVectorIndexTableBySimilarityMatrix(DataVectorIndexList_Basis, SimilarityMatrix);

    //---------- estimate BasisProbability using KNN method and Similarity --------------//

    auto BasisProbability = this->EstimateBasisProbabilityMassFunction_Basis(OutputBasisNumber, KNNBasisVectorIndexTable, ProbabilityMassFunctionOfData);

    // ------- update WeightedNumberOfTrainingSamplesInHistory -----------------------//

    int_max WeightedNumberOfTrainingSamples_ALL = 0;
    if (Dictionary_init == nullptr)
    {
        WeightedNumberOfTrainingSamples_ALL = FeatureData.GetColNumber();
    }
    else
    {
        WeightedNumberOfTrainingSamples_ALL = Dictionary_init->GetWeightedNumberOfTrainingSamplesInHistory() + FeatureData.GetColNumber();
    }

    //---------- estimate BasisRedundancy --------------------------------------------//

    auto BasisRedundancy = this->EstimateBasisRedundancy(DataVectorIndexList_Basis, SimilarityMatrix);

    //---------- estimate StandardDeviationOfL1Distance --------------------------------------------//
     
    DenseMatrix<ElementType> StandardDeviationOfL1Distance_init;
    if (Dictionary_init != nullptr)
    {
        StandardDeviationOfL1Distance_init.ForceShare(Dictionary_init->StandardDeviationOfL1Distance());
    }

    auto StandardDeviationOfL1Distance = this->EsimateBasisStandardDeviationOfL1Distance(FeatureData, 
                                                                                         KNNBasisVectorIndexTable, 
                                                                                         BasisMatrix, 
                                                                                         StandardDeviationOfL1Distance_init);

    //---------- estimate StandardDeviationOfL2Distance --------------------------------------------//
     
    DenseMatrix<ElementType> StandardDeviationOfL2Distance_init;
    if (Dictionary_init != nullptr)
    {
        StandardDeviationOfL2Distance_init.ForceShare(Dictionary_init->StandardDeviationOfL2Distance());
    }

    auto StandardDeviationOfL2Distance = this->EsimateBasisStandardDeviationOfL2Distance(FeatureData, 
                                                                                         KNNBasisVectorIndexTable, 
                                                                                         BasisMatrix, 
                                                                                         StandardDeviationOfL2Distance_init);

    //---------- estimate StandardDeviationOfKLDivergence --------------------------------------------//
    DenseMatrix<ElementType> StandardDeviationOfKLDivergence;
    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "KLDivergence")
    {
        DenseMatrix<ElementType> StandardDeviationOfKLDivergence_init;
        if (Dictionary_init != nullptr)
        {
            StandardDeviationOfKLDivergence_init.ForceShare(Dictionary_init->StandardDeviationOfKLDivergence());
        }

        StandardDeviationOfKLDivergence = this->EsimateBasisStandardDeviationOfKLDivergence(FeatureData,
                                                                                            KNNBasisVectorIndexTable,
                                                                                            BasisMatrix,
                                                                                            StandardDeviationOfKLDivergence_init);
    }

    //---------- update StandardDeviationOfReconstruction --------------------------------//

    DenseMatrix<ElementType> StandardDeviationOfReconstruction_init;
    if (Dictionary_init != nullptr)
    {
        StandardDeviationOfReconstruction_init.ForceShare(Dictionary_init->StandardDeviationOfReconstruction());
    }

    auto StandardDeviationOfReconstruction = this->EsimateBasisStandardDeviationOfReconstruction(FeatureData,
                                                                                         KNNBasisVectorIndexTable,
                                                                                         BasisMatrix,
                                                                                         StandardDeviationOfReconstruction_init);
    
    //----------------------- create Dictionary --------------------------------------//

    Dictionary.BasisMatrix().Take(BasisMatrix);

    Dictionary.SetWeightedNumberOfTrainingSamplesInHistory(WeightedNumberOfTrainingSamples_ALL);

    Dictionary.ProbabilityMassFunction().Take(BasisProbability);

    Dictionary.SetInfo_SimilarityThresholdToComputeBasisRedundancy(m_Parameter.SimilarityThresholdToComputeBasisRedundancy);
    Dictionary.SetInfo_SimilarityTypeToComputeBasisRedundancy(m_Parameter.ParameterOfKNNSoftAssign.SimilarityType);
    Dictionary.BasisRedundancy().Take(BasisRedundancy);

    Dictionary.StandardDeviationOfL1Distance().Take(StandardDeviationOfL2Distance);

    Dictionary.StandardDeviationOfL2Distance().Take(StandardDeviationOfL2Distance);

    Dictionary.StandardDeviationOfKLDivergence().Take(StandardDeviationOfKLDivergence);

    Dictionary.StandardDeviationOfReconstruction().Take(StandardDeviationOfReconstruction);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
GetProbabilityMassFunctionOfCombinedData(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
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
    // Probability is the PMF of every data vector in CombinedData
    //
    // If a data vector k in CombinedData is from FeatureData, 
    // then it only represents itself, PMF[k] ~ 1 * (1 - m_Parameter.weigth_past)
    //
    // If a data vector k in CombinedData is from BasisMatrix of Dictionary_init,
    // then it represents more, PMF[k] ~ the number of data in history that it represents *  m_Parameter.weigth_past

    DenseMatrix<ElementType> Probability(1, CombinedFeatureDataVectorNumber);

    Probability.Fill(ElementType(0));

    if (Dictionary_init != nullptr)
    {
        auto InitialBasisNumber = Dictionary_init->BasisMatrix().GetColNumber();

        const DenseMatrix<ElementType>& PMFOfInitialDicitonary = Dictionary_init->ProbabilityMassFunction();

        auto SampleNumber_past = Dictionary_init->GetWeightedNumberOfTrainingSamplesInHistory();

        for (int_max k = 0; k < InitialBasisNumber; k++)
        {
            Probability[k] = m_Parameter.weigth_past * PMFOfInitialDicitonary[k] * SampleNumber_past;
        }

        for (int_max k = InitialBasisNumber; k < CombinedFeatureDataVectorNumber; k++)
        {
            Probability[k] = (1 - m_Parameter.weigth_past) * ElementType(1);
        }
    }

    auto SumProb = Probability.Sum();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();
    if (SumProb <= eps_value)
    {
        Probability.Fill(ElementType(1) / ElementType(CombinedFeatureDataVectorNumber));
    }
    else
    {
        Probability /= SumProb;
    }

    return Probability;
}


template<typename ElementType>
template<typename SimilarityFunctionType>
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::ComputeSimilarityMatrix(const DenseMatrix<ElementType>& FeatureData,
                                                                           SimilarityFunctionType SimilarityFunction)
{
    //------------------------------------------------------------------------------------------
    // Input:
    // FeatureData is a set of column data vectors
    // SimilarityFunction(VectorA, VectorB) computes the similarity between VectorA and VectorB
    // 
    // Output:
    // SimilarityMatrix
    // ColNumber = RowNumber = the total number of data vectors    
    // SimilarityMatrix(i, j) = SimilarityMatrix(j, i)
    // SimilarityMatrix(i, i) = 0 for all i
    //
    // a pair of vector_i and vector_j is called data pair
    //------------------------------------------------------------------------------------------

    int_max TotalDataNumber = FeatureData.GetColNumber();

    DenseMatrix<ElementType> SimilarityMatrix(TotalDataNumber, TotalDataNumber);

    SimilarityMatrix.Fill(ElementType(0));  // SimilarityMatrix(i, i) = 0 for all i

    DenseMatrix<ElementType> Vector_k;
    DenseMatrix<ElementType> Vector_n;

    int_max VectorLength = FeatureData.GetRowNumber();

    std::random_device rd_bool;
    std::mt19937 gen_bool(rd_bool());
    std::bernoulli_distribution BoolRandomNumber(0.5);

    std::random_device rd_uniform;
    std::mt19937 gen_uniform(rd_uniform());
    std::uniform_real_distribution<ElementType> UniformRandomNumber(0, ElementType(1));

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    auto SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    for (int_max k = 0; k < TotalDataNumber - 1; ++k)
    {
        Vector_k.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(k)), VectorLength, 1);

        for (int_max n = k + 1; n < TotalDataNumber; ++n) // start from k+1
        {
            Vector_n.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(n)), VectorLength, 1);

            ElementType Similarity = SimilarityFunction(Vector_k, Vector_n);

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
             
            SimilarityMatrix(k, n) = Similarity;

            SimilarityMatrix(n, k) = Similarity;
        }
    }

    return SimilarityMatrix;
}


template<typename ElementType>
inline
ElementType KNNSoftAssignOnlineDictionaryBuilder<ElementType>::ComputeSimilarityBetweenTwoDataVectors(const DenseMatrix<ElementType>& VectorA,
                                                                                                      const DenseMatrix<ElementType>& VectorB)
{
    ElementType Similarity = ElementType(0);

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L1Distance")
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB);
        auto temp = L1Distance / m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1;
        Similarity = std::exp(-0.5*(temp*temp));
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "L2Distance")
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB);
        auto temp = L2Distance / m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2;
        Similarity = std::exp(-0.5*(temp*temp));
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "Correlation")
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB);
        if (m_Parameter.ParameterOfKNNSoftAssign.IgnoreSign_Correlation == true)
        {
            Similarity = std::abs(Correlation);
        }
        else
        {
            Similarity = (Correlation + ElementType(1)) / ElementType(2);
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == "KLDivergence")
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = KLDivergence / m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL;
        Similarity = std::exp(-(temp*temp) / ElementType(2));
    }
    else
    {
        MDK_Error("unknown type of similarity @ KNNSoftAssignOnlineDictionaryBuilder::ComputeSimilarityBetweenTwoDataVectors(...)")
    }

    return Similarity;
}


template<typename ElementType>
DataContainer<DenseMatrix<int_max>>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::FindKNNDataVectorIndexTableBySimilarityMatrix(const DenseMatrix<ElementType>& SimilarityMatrix)
{
    //-------------------------------------------------------------------------------------------------------------
    // Input:
    // SimilarityMatrix is from this->ComputeSimilarityMatrix(...)
    // 
    // Output:
    // KNNDataVectorIndexTable[k] is the data-index list of the data vector k
    // Index Range in KNNDataVectorIndexTable[k] is from 0 to TotalDataNumber-1
    //
    // If SimilarityMatrix(i, i) = 0 for all i
    // Then the data-index of the data vector k is not included in KNNDataVectorIndexTable[k]
    // i.e., self is not a KNN-neighbor of self
    //
    // SimilarityMatrix(i, i) = 0 for all i is guaranteed in this->ComputeSimilarityMatrix(...)
    //--------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = SimilarityMatrix.GetColNumber();

    DataContainer<DenseMatrix<int_max>> KNNDataVectorIndexTable;
    KNNDataVectorIndexTable.FastResize(TotalDataNumber);

    DenseMatrix<ElementType> SimilarityList(1, TotalDataNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        for (int_max n = 0; n < TotalDataNumber; ++n)
        {
            SimilarityList[n] = SimilarityMatrix(n, k);
        }

        KNNDataVectorIndexTable[k] = FindKNNBySimilarityList(SimilarityList, 
                                                             m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                             m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    }

    return KNNDataVectorIndexTable;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EstimateBasisProbabilityMassFunction_Data(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable)
{
    //--------------------------------------------------------------------------------
    // Input:
    // KNNDataVectorIndexTable is from this->FindKNNDataVectorIndexTableBySimilarityMatrix(...)
    // 
    // Output:
    // Probability is the PFM of each data vector
    //
    // assume each data vector can be a basis
    // then estimate the PMF of each data vector by counting the number of KNN-neighbors
    //--------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNDataVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(1)); // KNN does not include self, so add 1 to count self

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNDataVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            Probability[KNN_IndexList[m]] += 1;
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EstimateBasisProbabilityMassFunction_Data(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable,
                                          const DenseMatrix<ElementType>& DataProbabilityMassFunction)
{
    //---------------------------------------------------------------------------------------------------------
    // Input:
    // KNNDataVectorIndexTable is from this->FindKNNDataVectorIndexTableBySimilarityMatrix(...)
    // DataProbabilityMassFunction is from this->EstimateBasisProbabilityMassFunction_Data(...)
    // 
    // Output:
    // Probability is the PFM of each data vector
    // 
    // assume each data vector can be a basis
    // then estimate the PMF of each data vector by counting the normalized number of KNN-neighbors
    //
    // the normalized number of KNN-neighbors of the data vector k is stored in DataProbabilityMassFunction[k]
    //
    // this is no double counting 
    // because KNN does not include self in KNNDataVectorIndexTable 
    //---------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNDataVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(0)); // do not need to fill 1, self is already counted in DataProbabilityMassFunction

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNDataVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            Probability[KNN_IndexList[m]] += DataProbabilityMassFunction[k];
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
DataContainer<DenseMatrix<int_max>> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::FindKNNBasisVectorIndexTableBySimilarityMatrix(const DenseMatrix<int_max>&    DataVectorIndexList_Basis,
                                                                                                  const DenseMatrix<ElementType>& SimilarityMatrix)
{   
    //--------------------------------------------------------------------------------------------------------------
    // Input:
    // DataVectorIndexList_Basis is from this->BuildDictionaryFromDataBatch(...)
    // DataVectorIndexList_Basis[n] is the data-index of the basis vector that is selected from data vectors
    //
    // SimilarityMatrix is from this->ComputeSimilarityMatrix();
    // SimilarityMatrix(i, j) is the similarity between data vector i and data vector j
    // SimilarityMatrix(i, i) = 0 for all i 
    //
    // Output:
    // KNNBasisVectorIndexTable[k] is the basis-index list of KNN-bases of the data vector k
    // the length of KNNBasisVectorIndexTable[k] is from 0 to m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber
    // Index Range in KNNBasisVectorIndexTable[k] is from 0 to BasisNumber - 1
    //
    // When the data vector k is a basis:
    // If SimilarityMatrix(i, i) = 0 for all i    
    // Then, KNNBasisVectorIndexTable[k] dose not include the basis-index of the the data vector k
    //
    // SimilarityMatrix(i, i) = 0 for all i is guaranteed in this->ComputeSimilarityMatrix(...)
    //----------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = SimilarityMatrix.GetColNumber();

    DataContainer<DenseMatrix<int_max>> KNNBasisVectorIndexTable;
    KNNBasisVectorIndexTable.FastResize(TotalDataNumber);

    int_max BasisNumber = DataVectorIndexList_Basis.GetElementNumber();

    DenseMatrix<ElementType> SimilarityList(1, BasisNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        int_max DataVectorIndex = k;

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            int_max DataVectorIndex_basis = DataVectorIndexList_Basis[n];

            SimilarityList[n] = SimilarityMatrix(DataVectorIndex, DataVectorIndex_basis);
        }

        KNNBasisVectorIndexTable[k] = FindKNNBySimilarityList(SimilarityList, 
                                                              m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber,
                                                              m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold);
    }

    return KNNBasisVectorIndexTable;
}


template<typename ElementType>
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EstimateBasisProbabilityMassFunction_Basis(int_max BasisNumber,
                                           const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                           const DenseMatrix<ElementType>& DataProbabilityMassFunction)
{
    //--------------------------------------------------------------------------------------
    // Input:
    // BasisNumber is the number of bases that is selected from data vectors
    // KNNBasisVectorIndexTable is from this->FindKNNBasisVectorIndexTableBySimilarityMatrix(...)
    // KNNBasisVectorIndexTable[k] is the KNN basis-index list of data vector k
    // DataProbabilityMassFunction is from this->EstimateBasisProbabilityMassFunction_Data(...)
    // DataProbabilityMassFunction[k] is the PMF of data vector k
    //
    // Output:
    // Probability[n]: PMF of the basis with basis-index n (call it : the basis n)
    //
    // Probability[n] is estimated by counting the normalized number of KNN data vectors of the basis n
    // this is no double counting, because KNN does not include self in KNNBasisVectorIndexTable 
    //--------------------------------------------------------------------------------------

    int_max TotalDataNumber = KNNBasisVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, BasisNumber);

    Probability.Fill(ElementType(0));

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
        {
            Probability[KNN_IndexList[m]] += DataProbabilityMassFunction[k];
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
DenseMatrix<int_max>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::EstimateBasisRedundancy(const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                                           const DenseMatrix<ElementType>& SimilarityMatrix)
{

    //-----------------------------------------------------------------------------------------------------------------------------------
    // Input:
    // DataVectorIndexList_Basis is from this->BuildDictionaryFromDataBatch(...)
    // DataVectorIndexList_Basis[n] is the data-index of the basis vector that is selected from data vectors
    //
    // Output:
    // BasisRedundancy[n] is the number of neighbors of the basis n
    //
    // The neighbors are selected so that Similarity >= m_Parameter.SimilarityThresholdToComputeBasisRedundancy
    //
    // Range of BasisRedundancy[n] is from 0 to (BasisNumber - 1)
    //-------------------------------------------------------------------------------------------------------------------------------------

    int_max TotalDataNumber = SimilarityMatrix.GetColNumber();

    int_max BasisNumber = DataVectorIndexList_Basis.GetElementNumber();

    DenseMatrix<int_max> BasisRedundancy(1, BasisNumber);

    if (BasisNumber == 1)
    {
        BasisRedundancy[0] = 0;

        return BasisRedundancy;
    }

    auto SimilarityThreshold = m_Parameter.SimilarityThresholdToComputeBasisRedundancy;

    for (int_max k = 0; k < BasisNumber; ++k)
    {
        BasisRedundancy[k] = 0;
        
        auto DataIndex_basis_k = DataVectorIndexList_Basis[k];

        for (int_max n = 0; n < BasisNumber; ++n)
        {
            auto DataIndex_basis_n = DataVectorIndexList_Basis[n];

            if (DataIndex_basis_k != DataIndex_basis_n)
            {
                if (SimilarityMatrix(DataIndex_basis_n, DataIndex_basis_k) >= SimilarityThreshold)
                {
                    BasisRedundancy[k] += 1;
                }
            }
        }
    }

    return BasisRedundancy;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EsimateBasisStandardDeviationOfL1Distance(const DenseMatrix<ElementType>&            FeatureData,
                                          const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                          const DenseMatrix<ElementType>&            BasisMatrix,
                                          const DenseMatrix<ElementType>&            StandardDeviation_init)
{
    int_max TotalDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> StandardDeviation(1, BasisNumber);
    StandardDeviation.Fill(ElementType(0));

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(k)), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ShallowCopy(const_cast<ElementType*>(BasisMatrix.GetElementPointerOfCol(BasisIndex)), VectorLength, 1);

                ErrorVector = MatrixSubtract(DataVector, BasisVector);

                StandardDeviation[BasisIndex] += ErrorVector.L1Norm();

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            StandardDeviation[n] /= CounterList[n];
        }
    }

    if (StandardDeviation_init.IsEmpty() == false)
    {
        StandardDeviation = m_Parameter.weigth_past*StandardDeviation_init + (1 - m_Parameter.weigth_past)*StandardDeviation;
    }

    auto MeanStd = StandardDeviation.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignOnlineDictionaryBuilder::EsimateBasisStandardDeviationOfL1Distance(...)"
                    << '\n' << "set to std::max(eps_value, Sigma_L1)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Sigma_L1);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (StandardDeviation[n] <= eps_value)
        {
            StandardDeviation[n] = MeanStd;
        }
    }

    return StandardDeviation;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EsimateBasisStandardDeviationOfL2Distance(const DenseMatrix<ElementType>&            FeatureData,
                                          const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                          const DenseMatrix<ElementType>&            BasisMatrix,                                                                                                                                  
                                          const DenseMatrix<ElementType>&            StandardDeviation_init)
{
    int_max TotalDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> StandardDeviation(1, BasisNumber);
    StandardDeviation.Fill(ElementType(0));

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {        
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(k)), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ShallowCopy(const_cast<ElementType*>(BasisMatrix.GetElementPointerOfCol(BasisIndex)), VectorLength, 1);

                ErrorVector = MatrixSubtract(DataVector, BasisVector);

                StandardDeviation[BasisIndex] += ErrorVector.L2Norm();

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            StandardDeviation[n] /= CounterList[n];
        }
    }

    if (StandardDeviation_init.IsEmpty() == false)
    {
        StandardDeviation = m_Parameter.weigth_past*StandardDeviation_init + (1 - m_Parameter.weigth_past)*StandardDeviation;
    }

    auto MeanStd = StandardDeviation.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignOnlineDictionaryBuilder::EsimateBasisStandardDeviationOfL2Distance(...)"
                    << '\n' << "set to std::max(eps_value, Sigma_L2)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (StandardDeviation[n] <= eps_value)
        {
            StandardDeviation[n] = MeanStd;
        }
    }

    return StandardDeviation;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EsimateBasisStandardDeviationOfKLDivergence(const DenseMatrix<ElementType>&            FeatureData,
                                            const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                            const DenseMatrix<ElementType>&            BasisMatrix,
                                            const DenseMatrix<ElementType>&            StandardDeviation_init)
{
    int_max TotalDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> StandardDeviation(1, BasisNumber);
    StandardDeviation.Fill(ElementType(0));

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<ElementType> ErrorVector;

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(k)), VectorLength, 1);

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ShallowCopy(const_cast<ElementType*>(BasisMatrix.GetElementPointerOfCol(BasisIndex)), VectorLength, 1);

                auto KLDivergence = ComputeKLDivergenceOfVectorAFromVectorB(DataVector, BasisVector);

                StandardDeviation[BasisIndex] += KLDivergence;

                CounterList[BasisIndex] += 1;
            }
        }
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            StandardDeviation[n] /= CounterList[n];
        }
    }

    if (StandardDeviation_init.IsEmpty() == false)
    {
        StandardDeviation = m_Parameter.weigth_past*StandardDeviation_init + (1 - m_Parameter.weigth_past)*StandardDeviation;
    }

    auto MeanStd = StandardDeviation.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignOnlineDictionaryBuilder::EsimateBasisStandardDeviationOfKLDivergence(...)"
                     << '\n' << "set to std::max(eps_value, Sigma_KL)");

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Sigma_KL);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (StandardDeviation[n] <= eps_value)
        {
            StandardDeviation[n] = MeanStd;
        }
    }

    return StandardDeviation;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::ReconstructDataVectorByKNNBasisMatrix(const DenseMatrix<ElementType>& DataVector,    
                                                                                         const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                                                         const DenseMatrix<ElementType>& GramianMatrix_DtD,
                                                                                         const DenseMatrix<int_max>&     KNNBasisIndexList)
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

    auto ReconstructedDataVector = MatrixMultiply(KNNBasisMatrix, Solution.X);

    return ReconstructedDataVector;
}


template<typename ElementType>
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EsimateBasisStandardDeviationOfReconstruction(const DenseMatrix<ElementType>&            FeatureData,
                                              const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                              const DenseMatrix<ElementType>&            BasisMatrix,
                                              const DenseMatrix<ElementType>&            StandardDeviation_init)
{
    int_max TotalDataNumber = FeatureData.GetColNumber();

    int_max VectorLength = FeatureData.GetRowNumber();

    int_max BasisNumber = BasisMatrix.GetColNumber();

    DenseMatrix<ElementType> StandardDeviation(1, BasisNumber);
    StandardDeviation.Fill(ElementType(0));

    DenseMatrix<ElementType> DataVector;

    DenseMatrix<ElementType> ReconstructedDataVector;

    DenseMatrix<ElementType> ErrorVector;

    DenseMatrix<ElementType> BasisVector;

    DenseMatrix<int_max> CounterList(1, BasisNumber);

    DenseMatrix<ElementType> GramianMatrix_DtD = BasisMatrix.Transpose() *BasisMatrix;

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        if (KNN_IndexList.IsEmpty() == false)
        {
            DataVector.ShallowCopy(const_cast<ElementType*>(FeatureData.GetElementPointerOfCol(k)), VectorLength, 1);

            // Reconstruct DataVector using KNN 
            auto KNNBasisMatrix = BasisMatrix.GetSubMatrix(ALL, KNN_IndexList);

            ReconstructedDataVector = this->ReconstructDataVectorByKNNBasisMatrix(DataVector, KNNBasisMatrix, GramianMatrix_DtD, KNN_IndexList);

            ErrorVector = MatrixSubtract(DataVector, ReconstructedDataVector);
            auto ErrorVectorNorm = ErrorVector.L2Norm();

            for (int_max m = 0; m < KNN_IndexList.GetElementNumber(); ++m)
            {
                auto BasisIndex = KNN_IndexList[m];
                BasisVector.ShallowCopy(const_cast<ElementType*>(BasisMatrix.GetElementPointerOfCol(BasisIndex)), VectorLength, 1);

                StandardDeviation[BasisIndex] += ErrorVectorNorm;

                CounterList[BasisIndex] += 1;
            }
        }
       
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (CounterList[n] > 0)
        {
            StandardDeviation[n] /= CounterList[n];
        }
    }

    if (StandardDeviation_init.IsEmpty() == false)
    {
        StandardDeviation = m_Parameter.weigth_past*StandardDeviation_init + (1 - m_Parameter.weigth_past)*StandardDeviation;
    }

    auto MeanStd = StandardDeviation.Mean();

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (MeanStd <= eps_value)
    {
        MDK_Warning("MeanStd <= eps_value @ KNNSoftAssignOnlineDictionaryBuilder::EsimateBasisStandardDeviationOfReconstruction(...)"
                     << '\n' << "set to std::max(eps_value, Sigma_L2)")

        MeanStd = std::max(eps_value, m_Parameter.ParameterOfKNNSoftAssign.Sigma_L2);
    }

    for (int_max n = 0; n < BasisNumber; ++n)
    {
        if (StandardDeviation[n] <= eps_value)
        {
            StandardDeviation[n] = MeanStd;
        }
    }

    return StandardDeviation;
}


}//namespace mdk

#endif