#ifndef __mdkKNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ElementType>
KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::~KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::Clear()
{
    m_Parameter.Clear();

    m_FeatureData = nullptr;

    m_InitialDictionary = nullptr;

    this->ClearPipelineOutput();
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::ClearPipelineOutput()
{
    m_Dictionary_SharedCopy.Clear();
    m_Dictionary = &m_Dictionary_SharedCopy;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::UpdatePipelineOutput()
{
    if (m_Dictionary != &m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy.Share(m_Dictionary);
    }
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::
SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::SetOutputDictionary(...)")
        return;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy.ForceShare(OutputDictionary);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>* KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::GetOutputDictionary()
{
    return &m_Dictionary_SharedCopy;
}


template<typename ElementType>
bool KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInputDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InputDictionary @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        IsInputDictionaryEmpty = false;
    }

    if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L1Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL1Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_InitialDictionary->VarianceOfL1Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L1 <= 0 @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::L2Distance)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfL2Distance().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_InitialDictionary->VarianceOfL2Distance().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L2 <= 0 @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }
    else if (m_Parameter.ParameterOfKNNSoftAssign.SimilarityType == VectorSimilarityTypeEnum::KLDivergence)
    {
        if (m_Parameter.ParameterOfKNNSoftAssign.Variance_KL <= 0)
        {
            // try to find it in Initial Dictionary

            bool IsOk = false;

            if (IsInputDictionaryEmpty == false)
            {
                if (m_InitialDictionary->VarianceOfKLDivergence().IsEmpty() == false)
                {
                    m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_InitialDictionary->VarianceOfKLDivergence().Mean();
                    IsOk = true;
                }
            }

            if (IsOk == false)
            {
                MDK_Error("ParameterOfKNNSoftAssign.Variance_KL <= 0 @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection < 0
        || m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    if (m_Parameter.MaxNumberOfThreads <= 0)
    {
        MDK_Warning("MaxNumberOfThreads <= 0, set to 1 @ KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.MaxNumberOfThreads = 1;
    }

    return true;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    FeatureDictionaryForSparseCoding<ElementType> OutputDictionary;

    // check InputDictionary ------------------------------------------------------

    bool IsInputDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_InitialDictionary->IsEmpty() == false)
        {
            OutputDictionary = this->CopyInitialDictionaryAndDiscountBasisExperience();
        }
    }

    //----------------------------------------------------------------------------

    this->SetupParameter();

    int_max TotalDataNumber = m_FeatureData->GetColNumber();

    //----------- encode data and update dictionary ----------------------------//

    if (m_Parameter.MaxNumberOfDataInEachBatch >= TotalDataNumber)
    {
        this->UpdateDictionary(OutputDictionary, *m_FeatureData);        
    }
    else
    {
        //------------------------------------------ run Data batch -------------------------------------------------------//
        
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

            this->UpdateDictionary(OutputDictionary, FeatureData_current);

            // update NumberOfDataInNextBatch
            
            TotalDataNumber_used += SampleCounter;

            NumberOfDataInNextBatch = std::min(m_Parameter.MaxNumberOfDataInEachBatch, TotalDataNumber - TotalDataNumber_used);

            if (NumberOfDataInNextBatch <= 0)
            {
                break;
            }            
        }
    }

    this->UpdateDictionary_Final(OutputDictionary, *m_FeatureData);

    this->UpdateDictionary_Final_OtherInformation(OutputDictionary, TotalDataNumber);

    (*m_Dictionary) = std::move(OutputDictionary);
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const DenseMatrix<ElementType>& FeatureData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    this->UpdateParameter(m_Parameter.BasisNumber, DataNumber);

    // select basis from data and Initial Dictionary

    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(&Dictionary);
    m_KNNBasisSelectionDictionaryBuilder.Update();
    auto tempDictionarPtr = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

    // adjust BasisExperience : set BasisExperience of new basis to 1

    DenseMatrix<int_max>& tempBasisID = tempDictionarPtr->BasisID();

    DenseMatrix<ElementType>& tempBasisExperience = tempDictionarPtr->BasisExperience();

    int_max tempBasisNumber = tempDictionarPtr->BasisMatrix().GetColNumber();

    for (int_max k = 0; k < tempBasisNumber; ++k)
    {
        if (tempBasisID[k] == 0) // ID of new basis is 0
        {
            tempBasisExperience[k] = 1;
        }
    }

    // perform KNN average

    m_KNNAverageOnlineDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNAverageOnlineDictionaryBuilder.SetInitialDictionary(tempDictionarPtr);
    m_KNNAverageOnlineDictionaryBuilder.Update();

    // output

    Dictionary.Take(m_KNNAverageOnlineDictionaryBuilder.GetOutputDictionary());
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::
UpdateDictionary_Final(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const DenseMatrix<ElementType>& FeatureData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    this->UpdateParameter(m_Parameter.BasisNumber, DataNumber);

    // adjust BasisExperience : set BasisExperience of new basis to 1

    DenseMatrix<int_max>& BasisID = Dictionary.BasisID();

    DenseMatrix<ElementType>& BasisExperience = Dictionary.BasisExperience();

    int_max BasisNumber = Dictionary.BasisMatrix().GetColNumber();

    for (int_max k = 0; k < BasisNumber; ++k)
    {
        if (BasisID[k] == 0) // ID of new basis is 0
        {
            BasisExperience[k] = 1;
        }
    }

    // perform KNN average only

    m_KNNAverageOnlineDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNAverageOnlineDictionaryBuilder.SetInitialDictionary(&Dictionary);
    m_KNNAverageOnlineDictionaryBuilder.Update();

    // output

    Dictionary.Take(m_KNNAverageOnlineDictionaryBuilder.GetOutputDictionary());
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::
UpdateDictionary_Final_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, int_max DataNumber)
{
    // Set BasisID for new basis

    DenseMatrix<int_max>& BasisID = Dictionary.BasisID();

    int_max tempBasisNumber = Dictionary.BasisMatrix().GetColNumber();

    for (int_max k = 0; k < tempBasisNumber; ++k)
    {
        if (BasisID[k] == 0) // ID of new basis is 0
        {
            BasisID[k] = Dictionary.GenerateNewBasisID();
        }
    }

    // update BasisAge

    Dictionary.BasisAge() += DataNumber;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::SetupParameter()
{
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.BasisNumber;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisPositive = m_Parameter.BasisPositive;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNormalizedWithL2Norm = m_Parameter.BasisNormalizedWithL2Norm;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.BasisNormalizedWithL1Norm = m_Parameter.BasisNormalizedWithL1Norm;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L1 = m_Parameter.ParameterOfKNNSoftAssign.Variance_L1;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_L2 = m_Parameter.ParameterOfKNNSoftAssign.Variance_L2;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.Variance_KL = m_Parameter.ParameterOfKNNSoftAssign.Variance_KL;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = m_Parameter.ParameterOfKNNBasisSelection.ExperienceDiscountFactor;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.WeightOnProbabiliyForBasisSelection 
        = m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.MaxNumberOfThreads = m_Parameter.MaxNumberOfThreads;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_BasisAge = false;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.Flag_Update_BasisID  = false;

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.DebugInfo.FilePathToSaveDebugInfo = m_Parameter.DebugInfo.FilePathToSaveDebugInfo;
    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.DebugInfo.Flag_OutputDebugInfo = m_Parameter.DebugInfo.Flag_OutputDebugInfo;

    //--------------------------------------------------------------------------------------------------------

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.BasisNumber = m_Parameter.BasisNumber;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.BasisPositive = m_Parameter.BasisPositive;
    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.BasisNormalizedWithL2Norm = m_Parameter.BasisNormalizedWithL2Norm;
    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.BasisNormalizedWithL1Norm = m_Parameter.BasisNormalizedWithL1Norm;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber = m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber;
    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityType = m_Parameter.ParameterOfKNNSoftAssign.SimilarityType;
    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ExperienceDiscountFactor = m_Parameter.ParameterOfKNNAverage.ExperienceDiscountFactor;
    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.WhetherToUseScaleFactor  = m_Parameter.ParameterOfKNNAverage.WhetherToUseScaleFactor;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.MaxNumberOfThreads = m_Parameter.MaxNumberOfThreads;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix
        = m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ConstraintOnKNNReconstructionCode = m_Parameter.ConstraintOnKNNReconstructionCode;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.Flag_Update_BasisAge = false;
}


template<typename ElementType>
void KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::UpdateParameter(int_max BasisNumber, int_max DataNumber)
{
    m_Parameter.MaxNumberOfDataInEachBatch = std::min(int_max(500), m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber*BasisNumber);

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = m_Parameter.MaxNumberOfDataInEachBatch;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = m_Parameter.MaxNumberOfDataInEachBatch;
}

// out of memory
/*
template<typename ElementType>
DenseMatrix<int_max>
KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::
ComputeBasisNumberSequence(int_max BasisNumber_final, double factor, int_max DataNumber)
{
    DenseMatrix<int_max> BasisNumberSequence;
    BasisNumberSequence.FastResize(1, DataNumber);

    int_max Length = 0;

    int_max BasisNumber = BasisNumber_final;

    while (true)
    {
        if ( int_max(double(BasisNumber)*factor) <= DataNumber)
        {
            BasisNumberSequence[Length] = BasisNumber;

            Length += 1;   

            BasisNumber = int_max(double(BasisNumber)*factor);
        }
        else
        {
            break;
        }
    }

    BasisNumberSequence = BasisNumberSequence(span(0, Length - 1));

    BasisNumberSequence = BasisNumberSequence(span(Length-1, 0));

    return BasisNumberSequence;
}
*/

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>
KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType>::CopyInitialDictionaryAndDiscountBasisExperience()
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

}//namespace mdk

#endif