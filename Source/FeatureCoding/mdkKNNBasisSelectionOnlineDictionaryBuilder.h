#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_h

#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkObjectArray.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

struct DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    bool Flag_OutputDebugInfo;

    std::string FilePathToSaveDebugInfo;

//---------------------------------------------------
    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
        Flag_OutputDebugInfo = false;
		FilePathToSaveDebugInfo = "";
    }
};


template<typename ScalarType>
struct ParameterOfKNNSoftAssign_For_KNNBasisSelectionOnlineDictionaryBuilder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;

    ScalarType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold

    // initial values of Variance_L1, Variance_L2, and Variance_KL

    ScalarType Variance_L1; // variance to convert L1Distance to Similarity

    ScalarType Variance_L2; // variance to convert L2Distance to Similarity

    ScalarType Variance_KL; // variance to convert KLDivergence to Similarity
};


template<typename ScalarType>
struct Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    //----------------- parameter for soft assign --------------------------------------------------//

    ParameterOfKNNSoftAssign_For_KNNBasisSelectionOnlineDictionaryBuilder<ScalarType> ParameterOfKNNSoftAssign;
         
    //------------------- parameter for basis selection --------------------------------------//

    ScalarType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateBasisExperienceForEachBasisVector(...)

    ScalarType WeightOnProbabiliyForBasisSelection;
    // WeightOnProbabiliyForBasisSelection to sort vector pair    
    // range [0, 1]

    //----------------------- parameter for data processing ------------------------------//

    int_max MaxNumberOfDataInEachBatch; // the maximum number of data in each batch/thread

    int_max MaxNumberOfInteration;

    int_max MaxNumberOfThread;

    //--------------------- parameter for updating dictionary information -------------//

    bool Flag_Update_BasisID;

    bool Flag_Update_BasisAge;

    bool Flag_Update_Variance;

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    bool Flag_Update_SimilarityMatrix;

    bool Flag_Update_BasisRedundancy;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;


    //-------------- parameter for debug information output ----------------------------//

    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder DebugInfo;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
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

        ExperienceDiscountFactor = 0;

        WeightOnProbabiliyForBasisSelection = 0;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfInteration = 1;

        MaxNumberOfThread = 1;

        Flag_Update_BasisID  = false; // new basis ID = 0

        Flag_Update_BasisAge = false; // new basis age = 0

        Flag_Update_Variance = false; // Variance at new basis is initial value from ParameterOfKNNSoftAssign

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        Flag_Update_SimilarityMatrix = true;

        Flag_Update_BasisRedundancy = true;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;

        DebugInfo.Clear();
    }
};


template<typename Scalar_Type>
class KNNBasisSelectionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ScalarType> m_Parameter;

private:
    // input data
    const DenseMatrix<ScalarType>* m_FeatureData;

	// input initial dictionary
    const FeatureDictionaryForSparseCoding<ScalarType>* m_InitialDictionary;

	// output dictionary
    FeatureDictionaryForSparseCoding<ScalarType> m_Dictionary;

public:
    KNNBasisSelectionOnlineDictionaryBuilder();
    ~KNNBasisSelectionOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* InitialDictionary);

    bool CheckInput();

	bool Update();

    FeatureDictionaryForSparseCoding<ScalarType>* GetOutputDictionary();

protected:
    void GenerateDictionary();

    FeatureDictionaryForSparseCoding<ScalarType> PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitialDictionary);
    
    DenseMatrix<int_max> ComputeDataNumberInEachBatch(int_max TotalDataNumber);

    FeatureDictionaryForSparseCoding<ScalarType> BuildDictionaryFromDataBatch(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                                                                               const DenseMatrix<ScalarType>& FeatureData);

    DenseMatrix<int_max> SelectBasis(const int_max BasisNumber_desired,
                                     const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                     const DenseMatrix<ScalarType>& ProbabilityOfEachVector);

    void UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                     const DenseMatrix<ScalarType>& FeatureData,
                                     const DataArray<SparseVector<ScalarType>>& CodeTable,
                                     const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init);

    void UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, int_max DataNumber);

    void UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                              const DenseMatrix<ScalarType>& FeatureData,
                                              const DataArray<SparseVector<ScalarType>>& CodeTable,
                                              const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                              const DenseMatrix<int_max>& VectorIndexList_Basis,
                                              const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init);

    DenseMatrix<ScalarType> ComputeInitialRepresentativeAbilityOfEachVector(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                                                                             int_max TotalVectorNumber);

    DenseMatrix<ScalarType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                                                           const DenseMatrix<ScalarType>& FeatureData);

    DataArray<DenseMatrix<int_max>> FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ScalarType>& VectorSimilarityMatrix);
                    
    DenseMatrix<ScalarType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                                                                            const DataArray<DenseMatrix<int_max>>& KNNVectorIndexTable);
        
    DenseMatrix<ScalarType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                                                                            const DataArray<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                                            const DenseMatrix<ScalarType>& RepresentativeAbilityOfEachVector);

    void ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, int_max DataNumber, ScalarType TotalExperience_init);

    DataArray<SparseVector<ScalarType>> EncodeFeatureDataBySimilarity(const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                                                       const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                                       int_max BasisNumber_init);

    void UpdateBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const DataArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix);   

    void UpdateVarianceOfL1Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const DataArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience);

    void UpdateVarianceOfL2Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const DataArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ScalarType>& Variance,
                                      const DenseMatrix<ScalarType>& FeatureData,
                                      const DataArray<SparseVector<ScalarType>>& CodeTable,
                                      const DenseMatrix<ScalarType>& BasisMatrix,
                                      const DenseMatrix<ScalarType>& BasisExperience);

    void UpdateVarianceOfReconstruction(DenseMatrix<ScalarType>& Variance,
                                        const DenseMatrix<ScalarType>& FeatureData,
                                        const DataArray<SparseVector<ScalarType>>& CodeTable,
                                        const DenseMatrix<ScalarType>& BasisMatrix,
                                        const DenseMatrix<ScalarType>& BasisExperience);
    
    DenseMatrix<ScalarType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                                                  const DataArray<SparseVector<ScalarType>>& CodeTable,
                                                                  const DenseMatrix<ScalarType>&  BasisMatrix);

private:
    KNNBasisSelectionOnlineDictionaryBuilder(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.hpp"

#endif