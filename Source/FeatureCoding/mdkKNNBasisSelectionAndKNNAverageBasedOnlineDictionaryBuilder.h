#ifndef __mdkKNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNAverageOnlineDictionaryBuilder.h"
#include "mdkKNNSimilaritySparseEncoder.h"


namespace mdk
{
template<typename ElementType>
struct ParameterOfKNNBasisSelection_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder
{
    ElementType ExperienceDiscountFactor;

    ElementType WeightOnProbabiliyForBasisSelection;

    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder DebugInfo;
};

template<typename ElementType>
struct ParameterOfKNNAverage_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder
{
    ElementType ExperienceDiscountFactor;

    // ------ whether or not use scale factor --------

    bool WhetherToUseScaleFactor;

    int_max MaxNumberOfInterations;
};

template<typename ElementType>
struct ParameterOfKNNSoftAssign_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold

    // initial values of Variance_L1, Variance_L2, and Variance_KL

    ElementType Variance_L1; // variance to convert L1Distance to Similarity

    ElementType Variance_L2; // variance to convert L2Distance to Similarity

    ElementType Variance_KL; // variance to convert KLDivergence to Similarity
};


struct DebugInfo_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder
{
    bool Flag_OutputDebugInfo;

    CharString FilePathToSaveDebugInfo;

    //---------------------------------------------------
    DebugInfo_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder() { this->Clear(); }
    ~DebugInfo_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder() {}

    void Clear()
    {
        Flag_OutputDebugInfo = false;
        FilePathToSaveDebugInfo.Clear();
    }
};


template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    ParameterOfKNNSoftAssign_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType> ParameterOfKNNSoftAssign;

    ParameterOfKNNBasisSelection_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType> ParameterOfKNNBasisSelection;

    ParameterOfKNNAverage_For_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType> ParameterOfKNNAverage;

    ElementType ExperienceDiscountFactor;

    int_max MaxNumberOfDataInEachBatch;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ElementType SimilarityThreshold_For_ComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;

    // Debug
    DebugInfo_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder DebugInfo;
//-------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder() {}

    void Clear()
    {
        // Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder ------

        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNSoftAssign.NeighbourNumber = 0;
        ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::Unknown;
        ParameterOfKNNSoftAssign.SimilarityThreshold = 0;
        ParameterOfKNNSoftAssign.Variance_L1 = 0;
        ParameterOfKNNSoftAssign.Variance_L2 = 0;
        ParameterOfKNNSoftAssign.Variance_KL = 0;

        ParameterOfKNNBasisSelection.ExperienceDiscountFactor = 1;
        ParameterOfKNNBasisSelection.WeightOnProbabiliyForBasisSelection = 0.5;

        ParameterOfKNNAverage.ExperienceDiscountFactor = 0;
        ParameterOfKNNAverage.WhetherToUseScaleFactor  = false;
        ParameterOfKNNAverage.MaxNumberOfInterations   = 1;

        ExperienceDiscountFactor = 0;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfThreads = 1;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne = false;

        DebugInfo.Clear();        
    }

};


template<typename Element_Type>
class KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder : public FeatureDictionaryBuilder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    Parameter_Of_KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_KNNBasisSelectionDictionaryBuilder;

    KNNAverageOnlineDictionaryBuilder<ElementType> m_KNNAverageOnlineDictionaryBuilder;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNSoftAssignSparseEncoder;

public:
    KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder();
    ~KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    void UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const DenseMatrix<ElementType>& FeatureData);

    void UpdateDictionary_Final(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const DenseMatrix<ElementType>& FeatureData);

    void UpdateDictionary_Final_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, int_max DataNumber);

    void SetupParameter();

    void UpdateParameter(int_max BasisNumber, int_max DataNumber);

    //DenseMatrix<int_max> ComputeBasisNumberSequence(int_max BasisNumber_final, double factor, int_max DataNumber);

    FeatureDictionaryForSparseCoding<ElementType> CopyInitialDictionaryAndDiscountBasisExperience();

private:
    KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder(const KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionAndKNNAverageBasedOnlineDictionaryBuilder.hpp"

#endif