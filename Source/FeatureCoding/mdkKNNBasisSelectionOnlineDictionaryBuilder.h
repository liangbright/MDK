#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_h

#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkDataContainer.h"
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

    CharString FilePathToSaveDebugInfo;

//---------------------------------------------------
    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
        Flag_OutputDebugInfo = false;
        FilePathToSaveDebugInfo.Clear();
    }
};


template<typename ElementType>
struct SoftAssign_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold

    // initial values of Variance_L1, Variance_L2, and Variance_KL

    ElementType Variance_L1; // variance to convert L1Distance to Similarity

    ElementType Variance_L2; // variance to convert L2Distance to Similarity

    ElementType Variance_KL; // variance to convert KLDivergence to Similarity
};


template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    //----------------- parameter for soft assign --------------------------------------------------//

    SoftAssign_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> ParameterOfKNNSoftAssign;
         
    //------------------- parameter for basis selection --------------------------------------//

    ElementType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateBasisExperienceForEachBasisVector(...)

    ElementType WeightOnProbabiliyForBasisSelection;
    // WeightOnProbabiliyForBasisSelection to sort vector pair    
    // range [0, 1]

    //----------------------- parameter for data processing ------------------------------//

    int_max MaxNumberOfDataInEachBatch; // the maximum number of data in each batch/thread

    int_max MaxNumberOfThreads;

    //--------------------- parameter for updating dictionary information -------------//

    bool Update_BasisID;

    bool Update_BasisAge;

    bool Update_BasisExperience;

    bool Update_Variance;

    bool Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    bool Update_SimilarityMatrix;

    bool Update_BasisRedundancy;

    ElementType SimilarityThreshold_For_ComputingBasisRedundancy;

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

        MaxNumberOfThreads = 1;

        Update_BasisID  = false; // new basis ID = 0

        Update_BasisAge = false; // new basis age = 0

        Update_Variance = false; // Variance at new basis is initial value from ParameterOfKNNSoftAssign

        Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        Update_BasisExperience  = false; // new basis Experience = 1

        Update_SimilarityMatrix = true;

        Update_BasisRedundancy = true;

        SimilarityThreshold_For_ComputingBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;

        DebugInfo.Clear();
    }
};


template<typename ElementType>
class KNNBasisSelectionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{

public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;


public:
    KNNBasisSelectionOnlineDictionaryBuilder();
    ~KNNBasisSelectionOnlineDictionaryBuilder();

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

    FeatureDictionaryForSparseCoding<ElementType> CopyInitialDictionaryAndDiscountBasisExperience();

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromData(const int_max BasisNumber_desired,
                                                                          const DenseMatrix<ElementType>& FeatureData,
                                                                          const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    DenseMatrix<int_max> SelectBasisFromCombinedDataBySimilarityAndProbability(const int_max BasisNumber_desired,
                                                                               const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                               const DenseMatrix<ElementType>& ProbabilityOfEachVector);

    void UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<SparseVector<ElementType>>& CodeTable,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    void UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                              const DenseMatrix<ElementType>& FeatureData,
                                              const DataContainer<SparseVector<ElementType>>& CodeTable,
                                              const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                              const DenseMatrix<int_max>& VectorIndexList_Basis,
                                              const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    DenseMatrix<ElementType> ComputeRepresentativeAbilityOfEachVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                                                    int_max CombinedDataVectorNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                           const DenseMatrix<ElementType>& FeatureData);

    DataContainer<DenseMatrix<int_max>> FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix);
                    
    DenseMatrix<ElementType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                            const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable);
        
    DenseMatrix<ElementType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                            const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                                            const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    DataContainer<SparseVector<ElementType>> EncodeFeatureDataBySimilarity(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                           const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                                           int_max BasisNumber_init);

    void UpdateBasisExperience(DenseMatrix<ElementType>& BasisExperience, const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);   

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<SparseVector<ElementType>>& CodeTable,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<SparseVector<ElementType>>& CodeTable,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const DenseMatrix<ElementType>& BasisExperience);
    
    DenseMatrix<ElementType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                                                  const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                  const DenseMatrix<ElementType>&  BasisMatrix);

private:
    KNNBasisSelectionOnlineDictionaryBuilder(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.hpp"

#endif