#ifndef __mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNAverageOnlineDictionaryBuilder.h"
#include "mdkKNNSimilaritySparseEncoder.h"

namespace mdk
{
template<typename ScalarType>
struct ParameterOfKNNBasisSelection_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder
{
    ScalarType ExperienceDiscountFactor;

    ScalarType WeightOnProbabiliyForBasisSelection;

    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder DebugInfo;
};

template<typename ScalarType>
struct ParameterOfKNNAverage_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder
{
    ScalarType ExperienceDiscountFactor;

    // ------ whether or not use scale factor --------

    bool WhetherToUseScaleFactor;

    int_max MaxNumberOfInterations;
};

template<typename ScalarType>
struct ParameterOfKNNSoftAssign_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;

    ScalarType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold

    // initial values of Variance_L1, Variance_L2, and Variance_KL

    ScalarType Variance_L1; // variance to convert L1Distance to Similarity

    ScalarType Variance_L2; // variance to convert L2Distance to Similarity

    ScalarType Variance_KL; // variance to convert KLDivergence to Similarity
};


struct DebugInfo_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder
{
    bool Flag_OutputDebugInfo;

	std::string FilePathToSaveDebugInfo;

    //---------------------------------------------------
    DebugInfo_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder() { this->Clear(); }
    ~DebugInfo_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder() {}

    void Clear()
    {
        Flag_OutputDebugInfo = false;
		FilePathToSaveDebugInfo = "";
    }
};


template<typename ScalarType>
struct Parameter_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    ParameterOfKNNSoftAssign_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType> ParameterOfKNNSoftAssign;

    ParameterOfKNNBasisSelection_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType> ParameterOfKNNBasisSelection;

    ParameterOfKNNAverage_For_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType> ParameterOfKNNAverage;

    ScalarType ExperienceDiscountFactor;

    int_max MaxNumberOfDataInEachBatch;

	int_max MaxNumberOfThread;

    // parameter for updating dictionary information

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;

    // Debug
    DebugInfo_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder DebugInfo;
//-------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder() {}

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

		MaxNumberOfThread = 1;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne = false;

        DebugInfo.Clear();        
    }

};


template<typename Scalar_Type>
class KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;
	typedef FeatureDictionaryForSparseCoding<ScalarType>  DictionaryType;

public:
    Parameter_Of_KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder<ScalarType> m_Parameter;

private:
    const DenseMatrix<ScalarType>* m_FeatureData;

	const DictionaryType* m_InitialDictionary;

	DictionaryType m_Dictionary;

    KNNBasisSelectionOnlineDictionaryBuilder<ScalarType> m_KNNBasisSelectionDictionaryBuilder;

    KNNAverageOnlineDictionaryBuilder<ScalarType> m_KNNAverageOnlineDictionaryBuilder;

    KNNSoftAssignSparseEncoder<ScalarType> m_KNNSoftAssignSparseEncoder;

public:
    KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder();
    ~KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

	void SetInitialDictionary(const DictionaryType* InitialDictionary);

    bool CheckInput();

	bool Update();

	DictionaryType* GetOutputDictionary();

protected:
    void GenerateDictionary();

	void UpdateDictionary(DictionaryType& Dictionary, const DenseMatrix<ScalarType>& FeatureData);

	void UpdateDictionary_Final(DictionaryType& Dictionary, const DenseMatrix<ScalarType>& FeatureData);

	void UpdateDictionary_Final_OtherInformation(DictionaryType& Dictionary, int_max DataNumber);

    void SetupParameter();

    void UpdateParameter(int_max BasisNumber, int_max DataNumber);

    //DenseMatrix<int_max> ComputeBasisNumberSequence(int_max BasisNumber_final, double factor, int_max DataNumber);

	DictionaryType CopyInitialDictionaryAndDiscountBasisExperience();

private:
    KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder(const KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionAndKNNAverageOnlineDictionaryBuilder.hpp"

#endif