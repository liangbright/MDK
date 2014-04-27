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

    m_SparseEncoder.Clear();

    this->SetupDefaultPipelineOutput();
}


template<typename ElementType>
void SPAMSOnlineDictionaryBuilder<ElementType>::SetupDefaultPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void SPAMSOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
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
FeatureDictionaryForSparseCoding<ElementType>* SPAMSOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
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

    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error("InputFeatureData is empty @ KNNSoftAssignOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void KNNSoftAssignOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    auto DataSize = m_FeatureData->GetSize();

    auto DataNumber = DataSize.ColNumber;

    int_max BatchNumber = m_Parameter.NumberOfDataInEachBatch / DataNumber + 1;

    if (BatchNumber == 1)
    {
        DenseMatrix<ElementType> FeatureData_Combined;

        if (m_InitialDictionary != nullptr)
        {
            DenseMatrix<ElementType>& BasisMatrixOfInitialDictionary = m_InitialDictionary->BasisMatrix();
            int_max InitalBasisNumber = BasisMatrixOfInitialDictionary.GetColNumber();

            FeatureData_Combined = { &BasisMatrixOfInitialDictionary, m_FeatureData };
        }
        else
        {
            FeatureData_Combined.ForceShare(m_FeatureData);
        }

        auto Probability_Combined = GetProbabilityMassFunctionOfCombinedData(m_FeatureData->GetColNumber());
 
        (*m_Dictionary) = this->ExtractDictionaryFromData(m_Parameter.BasisNumber, FeatureData_Combined, Probability_Combined);

        return;
    }

    // BatchNumber > 1
    //------------------------------------------ run mini-batch -------------------------------------------------------//

    for (int_max IterationIndex = 0; IterationIndex < m_Parameter.MaxNumberOfIteration)
    {

    }


}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::ExtractDictionaryFromData(int_max BasisNumber_desired,
                                                                             const DenseMatrix<ElementType>& FeatureData,
                                                                             const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;

    //------------------------------------------- check input ------------------------------------------------------------------------//

    if (BasisNumber_desired <= 0 || FeatureData.IsEmpty() == true || Probability.IsVector() == false)
    {
        MDK_Error("Invalid input @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromData(...)")
        return Dictionary;
    }

    if (FeatureData.GetColNumber() != ProbabilityMassFunctionOfData.GetElementNumber())
    {
        MDK_Error("FeatureData.GetColNumber() != Probability.GetElementNumber() @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromData(...)")
        return Dictionary;
    }

    //------------------------------------------- input check done ---------------------------------------------------------------------//

    // calculate similarity between feature vectors

    auto SimilarityMatrix = ComputeSimilarityMatrixOfVecorSet(FeatureData,
                                                              [&](const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
                                                              { this->ComputeSimilarityBetweenTwoDataVectors(VectorA, VectorB); });

    auto KNNDataVectorIndexTable = FindKNNDataVectorIndexTableBySimilarityMatrix(SimilarityMatrix);

    // estimate the probability mass function based on Probability_init

    auto Probability = this->EstimateBasisProbabilityMassFunctionFromData(KNNDataVectorIndexTable, ProbabilityMassFunctionOfData);

    // if the number of data samples is smaller than the number of bases
    if (BasisNumber_desired >= FeatureData.GetColNumber())
    {
        MDK_Warning("BasisNumber_desired >= FeatureData.GetColNumber() @ KNNSoftAssignOnlineDictionaryBuilder::ExtractDictionaryFromData(...)")

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
        for (int_max n = k + 1; k < TotalFeatureDataNumber; ++k)
        {         
            DataPairScoreList[Counter] =   m_Parameter.weigth_s*SimilarityMatrix(n, k)
                                           + m_Parameter.weigth_p*Probability[n];
                                           + m_Parameter.weigth_p*Probability[k];

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

    int_max PairNumber = DataPairIndexList.GetColNumber();

    DenseMatrix<int_max> DataFlagList(1, TotalFeatureDataNumber);
    DataFlagList.Fill(1);
    // FlagList(i) is 0 : vector_i is not in the current basis set
    // FlagList(i) is 1 : vector_i is in the current basis set

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
                // randomly discard vector a or vector b
                if (k % 2 == 0)
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
            DataVectorIndexList_Basis.AppendCol(k);
        }
    }

    // ------------- create BasisMatrix--------------------------------------------------//

    DenseMatrix<ElementType> BasisMatrix(FeatureData->GetRowNumber(), OutputBasisNumber);

    for (int_max k = 0; k < OutputBasisNumber; ++k)
    {
        auto DataVectorIndex = DataVectorIndexList_Basis[k];

        BasisMatrix.SetCol(k, FeatureData.GetElementPointerOfCol(DataVectorIndex));
    }

    //---------- estimate BasisProbability using KNN method and Similarity --------------//

    auto BasisProbability = this->EstimateBasisProbabilityMassFunctionFromData(DataVectorIndexList_Basis, SimilarityMatrix, ProbabilityMassFunctionOfData);

    // ------- update WeightedNumberOfTrainingSamplesInHistory -----------------------//

    int_max WeightedNumberOfTrainingSamples_ALL = 0;
    if (m_InitialDictionary == nullptr)
    {
        WeightedNumberOfTrainingSamples_ALL = FeatureData.GetColNumber();
    }
    else
    {
        WeightedNumberOfTrainingSamples_ALL = m_InitialDictionary->GetWeightedNumberOfTrainingSamplesInHistory() + FeatureData.GetColNumber();
    }

    //---------- estimate StandardDeviation --------------------------------------------//
     
    DenseMatrix<ElementType> StandardDeviation;
    if (m_InitialDictionary != nullptr)
    {
        StandardDeviation = this->EsimateBasisStandardDeviation(BasisMatrix, FeatureData, m_InitialDictionary->StandardDeviation())
    }
    else
    {
        StandardDeviation = this->EsimateBasisStandardDeviation(BasisMatrix, FeatureData, MDK_Empty_Matrix);
    }

    //---------- update MeanErrorNormOfReconstruction --------------------------------//

    

    //----------------------- create Dictionary --------------------------------------//

    Dictionary.BasisMatrix().Take(BasisMatrix);

    Dictionary.SetWeightedNumberOfTrainingSamplesInHistory(WeightedNumberOfTrainingSamples_ALL);

    Dictionary.ProbabilityMassFunction().Take(BasisProbability);

    Dictionary.StandardDeviation().Take(StandardDeviation);

    //---------------------------------------------- done -------------------------------------------------------//
    
    return Dictionary;
}


template<typename ElementType>
inline
ElementType KNNSoftAssignOnlineDictionaryBuilder<ElementType>::ComputeSimilarityBetweenTwoDataVectors(const DenseMatrix<ElementType>& VectorA,
                                                                                                      const DenseMatrix<ElementType>& VectorB)
{
    ElementType Similarity = ElementType(0);

    if (DistanceType == "L1Distance")
    {
        auto L1Distance = ComputeL1DistanceBetweenTwoVectors(VectorA, VectorB);
        auto temp = L1Distance / m_Parameter.Sigma_L1;
        Similarity = std::exp(-0.5*(temp*temp))
    }
    else if (DistanceType == "L2Distance")
    {
        auto L2Distance = ComputeL2DistanceBetweenTwoVectors(VectorA, VectorB);
        auto temp = L2Distance / m_Parameter.Sigma_L2;
        Similarity = std::exp(-0.5*(temp*temp))
    }
    else if (DistanceType == "Correlation")
    {
        auto Correlation = ComputeCorrelationBetweenTwoVectors(VectorA, VectorB);
        if (m_Parameter.IgnoreSign_Correlation == true)
        {
            Similarity = std::abs(Correlation);
        }
        else
        {
            Similarity = (Correlation + ElementType(1)) / ElementType(2);
        }
    }
    else if (DistanceType == "KLDivergence")
    {
        auto KLDivergence_AB = ComputeKLDivergenceOfVectorAFromVectorB(VectorA, VectorB);
        auto KLDivergence_BA = ComputeKLDivergenceOfVectorAFromVectorB(VectorB, VectorA);
        auto KLDivergence = (KLDivergence_AB + KLDivergence_BA) / ElementType(2);

        auto temp = KLDivergence / m_Parameter.Sigma_KL;
        Similarity = std::exp(-0.5*(temp*temp))
    }
    else
    {
        MDK_Error("unknown type of similarity @ KNNSoftAssignOnlineDictionaryBuilder::ComputeSimilarityBetweenTwoDataVectors(...)")
    }

    return Similarity;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::GetProbabilityMassFunctionOfCombinedData(int_max FeatureDataVectorNumber)
{
    // CombinedData = [ BasisMatrix of m_InitialDictionary,  input FeatureData]

    // if m_InitialDictionary is empty, then CombinedData = input FeatureData 

    DenseMatrix<ElementType> Probability;

    auto Prob = ElementType(0);

    if (m_InitialDictionary == nullptr)
    {
        Probability.FastResize(1, FeatureDataVectorNumber);

        Probability.Fill(ElementType(1));
    }
    else
    {
        auto InitialBasisNumber = m_InitialDictionary->BasisMatrix().GetColNumber();

        const DenseMatrix<ElementType>& PMFOfInitialDicitonary = m_InitialDictionary->ProbabilityMassFunction();

        auto SampleNumber_past = m_InitialDictionary->GetWeightedNumberOfTrainingSamplesInHistory();

        Probability.FastResize(1, InitialBasisNumber + FeatureDataVectorNumber);

        for (int_max k = 0; k < InitialBasisNumber; k++)
        {
            Probability[k] = m_Parameter.weigth_past * PMFOfInitialDicitonary[k] * SampleNumber_past;
        }

        for (int_max k = InitialBasisNumber; k < InitialBasisNumber + FeatureDataVectorNumber; k++)
        {
            Probability[k] = m_Parameter.weigth_current * ElementType(1);
        }
    }
    
    Probability /= Probability.Sum();

    return Probability;
}


template<typename ElementType>
inline
DataContainer<DenseMatrix<int_max>>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::FindKNNDataVectorIndexTableBySimilarityMatrix(const DenseMatrix<ElementType>& SimilarityMatrix)
{
    int_max TotalDataNumber = SimilarityMatrix.GetColNumber();

    DataContainer<DenseMatrix<int_max>> KNNDataVectorIndexTable;
    KNNDataVectorIndexTable.FastResize(TotalDataNumber);

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    DenseMatrix<ElementType> SimilarityList(1, TotalDataNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        SimilarityList.Fill(ElementType(0));

        for (int_max n = 0; n < TotalDataNumber; ++n)
        {
            SimilarityList[n] = SimilarityMatrix(n, k);
        }

        KNNDataVectorIndexTable[k] = FindKNNBySimilarityList(m_Parameter.NeighbourNumber, SimilarityList);
    }

    return KNNDataVectorIndexTable;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::
EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable)
{
    int_max TotalDataNumber = KNNDataVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    Probability.Fill(ElementType(1)); // KNN does not include self, so add 1 first

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNDataVectorIndexTable[k];

        for (int_max m = 0; m < m_Parameter.NeighbourNumber; ++m)
        {
            Probability[KNN_IndexList[m]] += 1;
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
inline DenseMatrix<ElementType> EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable,
                                                                             const DenseMatrix<ElementType>& DataProbabilityMassFunction)
{
    int_max TotalDataNumber = KNNDataVectorIndexTable.GetLength();

    DenseMatrix<ElementType> Probability(1, TotalDataNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNDataVectorIndexTable[k];

        for (int_max m = 0; m < m_Parameter.NeighbourNumber; ++m)
        {
            Probability[KNN_IndexList[m]] += DataProbabilityMassFunction[k];
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
inline 
DataContainer<DenseMatrix<int_max>> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::FindKNNBasisVectorIndexTableBySimilarityMatrix(const DenseMatrix<int_max>&    DataVectorIndexList_Basis,
                                                                                                  const DenseMatrix<ElementType>& SimilarityMatrix)
{
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

        KNNBasisVectorIndexTable[k] = FindKNNBySimilarityList(m_Parameter.NeighbourNumber, SimilarityList);
    }

    return KNNBasisVectorIndexTable;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                                                const DenseMatrix<ElementType>& DataProbabilityMassFunction)
{
    int_max TotalDataNumber = KNNBasisVectorIndexTable.GetLength();

    int_max BasisNumber = KNNBasisVectorIndexTable[0].GetElementNumber();

    DenseMatrix<ElementType> Probability(1, BasisNumber);

    for (int_max k = 0; k < TotalDataNumber; ++k)
    {
        const DenseMatrix<int_max>& KNN_IndexList = KNNBasisVectorIndexTable[k];

        for (int_max m = 0; m < m_Parameter.NeighbourNumber; ++m)
        {
            Probability[KNN_IndexList[m]] += DataProbabilityMassFunction[k];
        }
    }

    Probability /= Probability.Sum();
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
KNNSoftAssignOnlineDictionaryBuilder<ElementType>::EsimateBasisStandardDeviation(const DenseMatrix<ElementType>& FeatureData, 
                                                                                 const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                                                 const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                                 const DenseMatrix<ElementType>& StandardDeviation_init)
{

}

}//namespace mdk

#endif