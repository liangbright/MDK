#ifndef __mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder_hpp
#define __mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder_hpp


namespace mdk
{

template<typename ScalarType>
KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ScalarType>
KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::~KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
    m_FeatureData = nullptr;
    m_InitialDictionary = nullptr;
	m_Dictionary.Clear();
}


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::
SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>* KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::GetOutputDictionary()
{
    return &m_Dictionary;
}


template<typename ScalarType>
bool KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_FeatureData->IsEmpty() == true)
    {
        MDK_Error("InputFeatureData is empty @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    bool IsInputDictionaryEmpty = true;

    if (m_InitialDictionary != nullptr)
    {
        if (m_FeatureData->GetRowNumber() != m_InitialDictionary->BasisMatrix().GetRowNumber())
        {
            MDK_Error("RowNumber Of FeatureData != RowNumber Of InputDictionary @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L1 <= 0 @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("ParameterOfKNNSoftAssign.Variance_L2 <= 0 @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
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
                MDK_Error("ParameterOfKNNSoftAssign.Variance_KL <= 0 @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }
    }

    if (m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection < 0
        || m_Parameter.ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection > 1)
    {
        MDK_Error("WeightOnProbabiliyForBasisSelection < 0 or > 1 @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy <= 0)
    {
        m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.ParameterOfKNNSoftAssign.SimilarityThreshold;
    }

    if (m_Parameter.MaxNumberOfThread <= 0)
    {
        MDK_Warning("MaxNumberOfThread <= 0, set to 1 @ KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder::CheckInput()")

        m_Parameter.MaxNumberOfThread = 1;
    }

    return true;
}


template<typename ScalarType>
bool KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}
	this->GenerateDictionary();
	return true;
}


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::GenerateDictionary()
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;

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

        DenseMatrix<ScalarType> FeatureData_current;

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
	//----------------------------------
    m_Dictionary.Take(OutputDictionary);
}


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::
UpdateDictionary(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const DenseMatrix<ScalarType>& FeatureData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    this->UpdateParameter(m_Parameter.BasisNumber, DataNumber);

    // select basis from data and Initial Dictionary

    m_KNNBasisSelectionDictionaryBuilder.SetInputFeatureData(&FeatureData);
    m_KNNBasisSelectionDictionaryBuilder.SetInitialDictionary(&Dictionary);
    m_KNNBasisSelectionDictionaryBuilder.Update();
    auto tempDictionarPtr = m_KNNBasisSelectionDictionaryBuilder.GetOutputDictionary();

    // adjust BasisExperience : set BasisExperience of new basis to 1

    auto& tempBasisID = tempDictionarPtr->BasisID();

    auto& tempBasisExperience = tempDictionarPtr->BasisExperience();

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


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::
UpdateDictionary_Final(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const DenseMatrix<ScalarType>& FeatureData)
{
    int_max DataNumber = FeatureData.GetColNumber();

    this->UpdateParameter(m_Parameter.BasisNumber, DataNumber);

    // adjust BasisExperience : set BasisExperience of new basis to 1

    auto& BasisID = Dictionary.BasisID();

    auto& BasisExperience = Dictionary.BasisExperience();

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


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::
UpdateDictionary_Final_OtherInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, int_max DataNumber)
{
    // Set BasisID for new basis

    auto& BasisID = Dictionary.BasisID();

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


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::SetupParameter()
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

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.MaxNumberOfThread = m_Parameter.MaxNumberOfThread;

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

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.MaxNumberOfThread = m_Parameter.MaxNumberOfThread;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix
        = m_Parameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy = m_Parameter.SimilarityThreshold_For_ComputeBasisRedundancy;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.ConstraintOnKNNReconstructionCode = m_Parameter.ConstraintOnKNNReconstructionCode;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.Flag_Update_BasisAge = false;
}


template<typename ScalarType>
void KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::UpdateParameter(int_max BasisNumber, int_max DataNumber)
{
    m_Parameter.MaxNumberOfDataInEachBatch = std::min(int_max(500), m_Parameter.ParameterOfKNNSoftAssign.NeighbourNumber*BasisNumber);

    m_KNNBasisSelectionDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = m_Parameter.MaxNumberOfDataInEachBatch;

    m_KNNAverageOnlineDictionaryBuilder.m_Parameter.MaxNumberOfDataInEachBatch = m_Parameter.MaxNumberOfDataInEachBatch;
}

// out of memory
/*
template<typename ScalarType>
DenseMatrix<int_max>
KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::
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

template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>
KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType>::CopyInitialDictionaryAndDiscountBasisExperience()
{
    FeatureDictionaryForSparseCoding<ScalarType> OutputDictionary;

    OutputDictionary.Copy(m_InitialDictionary); // m_InitialDictionary is not empty

    auto& BasisExperience = OutputDictionary.BasisExperience();

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